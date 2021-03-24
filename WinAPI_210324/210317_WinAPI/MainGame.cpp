#include "MainGame.h"

LRESULT MainGame::MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_CREATE:
		tank.Init();

		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);
		tank.Render(hdc);
		EndPaint(g_hWnd, &ps);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
