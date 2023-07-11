#pragma once
#define USING_PATHCCH
#include <Windows.h>
#include <WindowsX.h>
#include <Psapi.h>
#include <CommCtrl.h>
#include <TlHelp32.h>
#ifdef USING_PATHCCH
#include <PathCch.h>
#else
#include <shlwapi.h>
#endif
#include <strsafe.h>
#include <sal.h>

#define EXTERN extern 

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

BOOL CALLBACK EnumChildProc(_In_ HWND hWnd, _In_ LPARAM lParam);
VOID WINAPI OnClose(_In_ HWND hWnd);
VOID WINAPI OnCommand(_In_ HWND hWnd, _In_ INT nID, _In_ HWND hwSource, _In_ UINT uNotify);
BOOL WINAPI OnCreate(_In_ HWND hWnd, _In_ LPCREATESTRUCTW lpCreateStruct);
VOID WINAPI OnDestroy(_In_ HWND hWnd);
VOID WINAPI OnPaint(_In_ HWND hWnd);
ATOM WINAPI RegisterWCEX(_In_ HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd);

EXTERN CONST WCHAR g_wszClassName[];

