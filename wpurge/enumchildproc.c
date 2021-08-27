#include "WP.h"

BOOL CALLBACK EnumChildProc(_In_ HWND hWnd, _In_ LPARAM lParam)
{
	HFONT hfDefault = *(HFONT *)lParam;
	SendMessageW(hWnd, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(TRUE, 0));
	return TRUE;
}
