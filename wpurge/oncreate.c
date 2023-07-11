#include "WP.h"

BOOL WINAPI OnCreate(_In_ HWND hWnd, _In_ LPCREATESTRUCTW lpCreateStruct)
{
	CONST HINSTANCE hInstance = lpCreateStruct->hInstance;
	INITCOMMONCONTROLSEX iccx;
	HWND hRunButton;

	iccx.dwICC = ICC_STANDARD_CLASSES;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);

	InitCommonControlsEx(&iccx);

	hRunButton = CreateWindowW(L"BUTTON", L"Clear Icon Cache", WS_VISIBLE | WS_CHILD | BS_TEXT, 10, 10, 150, 30, hWnd, NULL, hInstance, NULL);

	return TRUE;
}
