#include "stdafx.h"
#include "MainGame.h"

HINSTANCE g_hInstance;
HWND g_hWnd;
LPSTR g_lpszClass = (LPSTR)TEXT("vector를 이용한 여러 탄환 패턴");
MainGame g_mainGame;
bool isDebugMode = true;
POINTFLOAT g_mouse = { 0, 0 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void SetWindowSize(HWND hWnd, int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	g_hInstance = hInstance;

	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, WINSIZE_WIDTH, WINSIZE_HEIGHT, NULL, NULL, g_hInstance, NULL);
	ShowWindow(g_hWnd, nShowCmd);
	SetWindowSize(g_hWnd, WINPOS_STARTX, WINPOS_STARTY, WINSIZE_WIDTH, WINSIZE_HEIGHT);

	if (FAILED(g_mainGame.Init()))
	{
		MessageBox(g_hWnd, "게임 초기화에 실패하였습니다.", "실행오류", MB_OK);
		return 0;
	}

	TimerManager::GetInstance()->Init();

	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TimerManager::GetInstance()->Update();
			g_mainGame.Update();
			g_mainGame.Render();
		}
	}
	g_mainGame.Release();

	return message.wParam;
}

void SetWindowSize(HWND hWnd, int x, int y, int width, int height)
{
	RECT wndRect;
	DWORD wndStyle = GetWindowLong(hWnd, GWL_STYLE);
	DWORD wndStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
	SetRect(&wndRect, 0, 0, WINSIZE_WIDTH, WINSIZE_HEIGHT);
	
	AdjustWindowRect(&wndRect, wndStyle, GetMenu(hWnd) != NULL);
	//AdjustWindowRectEx(&wndRect, wndStyle, GetMenu(hWnd) != NULL, wndStyleEx);

	if (wndStyle & WS_VSCROLL) wndRect.right += GetSystemMetrics(SM_CYVSCROLL);
	if (wndStyle & WS_HSCROLL) wndRect.bottom += GetSystemMetrics(SM_CXVSCROLL);
	SetWindowPos(hWnd, NULL, x, y, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, SWP_NOMOVE | SWP_NOZORDER);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.WndProc(hWnd, iMessage, wParam, lParam);
}