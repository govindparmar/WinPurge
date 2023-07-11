#include "WP.h"

VOID WINAPI OnDestroy(_In_ HWND hWnd)
{
	PostQuitMessage(ERROR_SUCCESS);
}
