#include "WP.h"

VOID WINAPI OnCommand(_In_ HWND hWnd, _In_ INT nID, _In_ HWND hwSource, _In_ UINT uNotify)
{
	HWND hButton = FindWindowExW(hWnd, NULL, L"BUTTON", NULL);
	if (hButton == hwSource)
	{
		PROCESSENTRY32W entry;
		HANDLE hSnapShot, hProcess;
		BOOL hRes;
		WCHAR fPath[MAX_PATH], username[32], mbBuf[1000];
		LPWSTR lpMsgBuf;
		entry.dwSize = sizeof(PROCESSENTRY32);
		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
		hRes = Process32FirstW(hSnapShot, &entry);
		while (hRes)
		{
			if (wcscmp(entry.szExeFile, L"explorer.exe") == 0)
			{
				hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)entry.th32ProcessID);
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
				GetEnvironmentVariableW(L"username", username, 32);
				if (SUCCEEDED(StringCchPrintfW(fPath, MAX_PATH, L"C:\\Users\\%s\\AppData\\Local\\IconCache.db", username)))
				{
					BOOL del = DeleteFileW(fPath);
					DWORD le = GetLastError();
					if (del)
					{
						StringCchCopyW(mbBuf, 1000, L"The icon cache database was successfully purged.");
						MessageBoxW(NULL, mbBuf, L"Success", MB_OK | MB_ICONASTERISK);

					}
					else
					{
						if (le == ERROR_FILE_NOT_FOUND)
						{
							MessageBoxW(NULL, L"Nothing to purge: The icon cache database could not be found.", L"Information", MB_OK | MB_ICONASTERISK);
						}
						else
						{
							FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, le, 0, (LPWSTR)&lpMsgBuf, 0, NULL);
							if (SUCCEEDED(StringCchPrintfW(mbBuf, 1000, L"The following error occured while attempting to purge the cache:\r\n\r\n%s", lpMsgBuf)))
							{
								MessageBoxW(NULL, mbBuf, L"Error", MB_OK | MB_ICONSTOP);
							}
							LocalFree((HLOCAL)lpMsgBuf);
						}
					}

				}
				// NOTE: No break after successful find because there may be more than one instance of explorer.exe running (folder windows etc.); only safe method is to kill all
			}
			hRes = Process32NextW(hSnapShot, &entry);
		}
	}
}
