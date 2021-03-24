#include "MainGame.h"

void MainGame::Init()
{
	timer = (HANDLE)SetTimer(g_hWnd, 0, 10, NULL);
	tank.Init();
}

void MainGame::Update()
{
	tank.Update();
}

void MainGame::Render(HDC hdc)
{
	tank.Render(hdc);
}

void MainGame::Release()
{
	tank.Release();
	KillTimer(g_hWnd, 0);
}

LRESULT MainGame::MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_CREATE:
		tank.Init();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'w':
		case 'W':
			tank.Move({ 0, -5 });
			break;
		case 'a':
		case 'A':
			tank.Move({ -5, 0 });
			break;
		case 's':
		case 'S':
			tank.Move({ 0, 5 });
			break;
		case 'd':
		case 'D':
			tank.Move({ 5, 0 });
			break;
		case VK_SPACE:
			tank.Fire();
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_LBUTTONDOWN:
		tank.RotateFire(-10);
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_RBUTTONDOWN:
		tank.RotateFire(+10);
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);
		Render(hdc);
		EndPaint(g_hWnd, &ps);
		break;
	case WM_TIMER:
		Update();
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_DESTROY:
		Release();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
