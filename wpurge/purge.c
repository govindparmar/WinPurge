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
		WCHAR fPath[MAX_PATH], username[32], mbBuf[1000];
		LPWSTR lpMsgBuf;
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
				if(SUCCEEDED(StringCchPrintf(fPath, MAX_PATH, L"C:\\Users\\%s\\AppData\\Local\\IconCache.db", username)))
				{
					BOOL del = DeleteFile(fPath);
					DWORD le = GetLastError();
					if (del)
					{
						if (SUCCEEDED(StringCchPrintf(mbBuf, 1000, L"The icon cache database was successfully purged.", fPath)))
						{
							MessageBox(0, mbBuf, L"Success", MB_OK | MB_ICONASTERISK);
						}
					}
					else
					{
						if (le == ERROR_FILE_NOT_FOUND)
						{
							MessageBox(0, L"Nothing to purge: The icon cache database could not be found.", L"Information", MB_OK | MB_ICONASTERISK);
						}
						else
						{
							FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, le, 0, (LPWSTR)&lpMsgBuf, 0, NULL);
							if (SUCCEEDED(StringCchPrintf(mbBuf, 1000, L"The following error occured while attempting to purge the cache:\r\n\r\n%s", lpMsgBuf)))
							{
								MessageBox(0, mbBuf, L"Error", MB_OK | MB_ICONSTOP);
							}
						}
					}

				}
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
