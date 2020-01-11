#include "GdiplusWindow.h"

LRESULT __stdcall GWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GdiplusWindow* win = reinterpret_cast<GdiplusWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (win != NULL) {
		return win->WndProc(hWnd, msg, wParam, lParam);
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
