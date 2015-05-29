#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam);
TCHAR szClassName[] = TEXT("PurgeCacheWnd");


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
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

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szClassName, TEXT("Clear Caches"), WS_VISIBLE | WS_SYSMENU, 100, 100, 200, 150, NULL, NULL, hInstance, NULL);
	hRun = CreateWindow(TEXT("BUTTON"), TEXT("Start"), WS_VISIBLE | WS_CHILD | BS_TEXT, 10, 10, 150, 30, hWnd, NULL, hInstance, NULL);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_COMMAND:
		if ((HWND)lParam == FindWindowEx(hWnd, NULL, TEXT("BUTTON"), NULL))
		{
			PROCESSENTRY32 entry;
			HANDLE hSnapShot, hProcess, hFile;
			BOOL hRes;
			TCHAR fPath[MAX_PATH], username[32];
			entry.dwSize = sizeof(PROCESSENTRY32);
			hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
			hRes = Process32First(hSnapShot, &entry);
			while (hRes)
			{
				if (wcscmp(entry.szExeFile, TEXT("explorer.exe")) == 0)
				{
					hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)entry.th32ProcessID);
					TerminateProcess(hProcess, 9);
					CloseHandle(hProcess);
					GetEnvironmentVariable(TEXT("username"), username, 32);
					wsprintf(fPath, TEXT("C:\\Users\\%s\\AppData\\Local\\IconCache.db"), username);
					DeleteFile(fPath);

				}
				hRes = Process32Next(hSnapShot, &entry);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}