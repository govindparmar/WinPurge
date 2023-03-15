#include "WP.h"

CONST WCHAR g_wszClassName[] = L"PurgeCacheWnd";

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd)
{
	MSG Msg;
	HWND hWnd, hRun;
	NONCLIENTMETRICSW ncm;
	HFONT hfDefault;

	ZeroMemory(&ncm, sizeof(NONCLIENTMETRICSW));
	ncm.cbSize = sizeof(NONCLIENTMETRICSW);

	SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &ncm, FALSE);
	hfDefault = CreateFontIndirectW(&ncm.lfMessageFont);
	
	if (RegisterWCEX(hInstance) == (ATOM)0)
	{
		MessageBoxW(NULL, L"Window registration failed", L"Clear Icon Cache", MB_OK | MB_ICONSTOP);
		return -1;
	}

	hWnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, g_wszClassName, L"Clear Icon Cache", WS_VISIBLE | WS_SYSMENU, 100, 100, 200, 150, NULL, NULL, hInstance, NULL);
	if (NULL == hWnd)
	{
		MessageBoxW(NULL, L"Window creation failed", L"Clear Icon Cache", MB_OK | MB_ICONSTOP);
		return -1;
	}
	hRun = CreateWindowW(L"BUTTON", L"Clear Icon Cache", WS_VISIBLE | WS_CHILD | BS_TEXT, 10, 10, 150, 30, hWnd, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	EnumChildWindows(hWnd, EnumChildProc, (LPARAM)&hfDefault);
	UpdateWindow(hWnd);


	while (GetMessageW(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
	}

	return (INT)Msg.wParam;
}
