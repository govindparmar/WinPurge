#include <Windows.h>
#include <WindowsX.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <strsafe.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam);
VOID WINAPI OnClose(HWND hWnd);
VOID WINAPI OnDestroy(HWND hWnd);
VOID WINAPI OnCommand(HWND hWnd, INT nID, HWND hwSource, UINT uNotify);

WCHAR szClassName[] = L"PurgeCacheWnd";


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wc;
	MSG Msg;
	HWND hWnd, hRun;

	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szClassName;
	wc.lpszMenuName = NULL;
	wc.style = 0;

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szClassName, L"Clear Icon Cache", WS_VISIBLE | WS_SYSMENU, 100, 100, 200, 150, NULL, NULL, hInstance, NULL);
	hRun = CreateWindow(L"BUTTON", L"Clear Icon Cache", WS_VISIBLE | WS_CHILD | BS_TEXT, 10, 10, 150, 30, hWnd, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	EnumChildWindows(hWnd, EnumChildProc, 0);
	UpdateWindow(hWnd);


	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return (int)Msg.wParam;

}
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam)
{
	HFONT hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, 0);
	return TRUE;
}

VOID WINAPI OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

VOID WINAPI OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

VOID WINAPI OnCommand(HWND hWnd, INT nID, HWND hwSource, UINT uNotify)
{
	HWND hButton = FindWindowEx(hWnd, NULL, L"BUTTON", NULL);
	if (hButton == hwSource)
	{
		PROCESSENTRY32 entry;
		HANDLE hSnapShot, hProcess;
		BOOL hRes;
		WCHAR fPath[MAX_PATH], username[32];
		entry.dwSize = sizeof(PROCESSENTRY32);
		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
		hRes = Process32First(hSnapShot, &entry);
		while (hRes)
		{
			if (wcscmp(entry.szExeFile, L"explorer.exe") == 0)
			{
				hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)entry.th32ProcessID);
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
				GetEnvironmentVariable(L"username", username, 32);
				StringCchPrintf(fPath, MAX_PATH, L"C:\\Users\\%s\\AppData\\Local\\IconCache.db", username);
				DeleteFile(fPath);
				// NOTE: No break after successful find because there may be more than one instance of explorer.exe running (folder windows etc.); only safe method is to kill all
			}
			hRes = Process32Next(hSnapShot, &entry);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HWND hButton = FindWindowEx(hWnd, NULL, L"BUTTON", NULL);
	switch (Msg)
	{
		HANDLE_MSG(hWnd, WM_CLOSE, OnClose);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}
