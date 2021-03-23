#include <Windows.h>
#include <ctime>
#include <cmath>
#include "DyingMotion.h"
#include "UIBattleStatus.h"
#include "Obstacle.h"
#include "AIUnit.h"

HINSTANCE g_hIndtance;
HWND g_hWnd;
LPSTR g_lpszClass = (LPSTR)TEXT("Win API");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdPAram, int nCmdShow);
RECT RectCenter(POINT p, int width, int height);
void RenderRect(HDC hdc, RECT rc, HBRUSH hBrush, HPEN hPen);
void RenderCircle(HDC hdc, RECT rc, HBRUSH hBrush, HPEN hPen);
void DyingMotionToCenter(HDC hdc, int x, int y, int size);
void RenderPlayer(HDC hdc, RECT rc, POINT p, HBRUSH hBrush, HPEN hPen);
void RenderPlayer2(HDC hdc, RECT rc, POINT p, HBRUSH hBrush, HPEN hPen);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdPAram, int nCmdShow)
{
	srand(time(0));

	g_hIndtance = hInstance;

	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = g_hIndtance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 100, 100, 815, 600, NULL, NULL, g_hIndtance, NULL);

	ShowWindow(g_hWnd, nCmdShow);

	MSG message;
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}

// º®
RECT leftWall = {0, 0, 30, 1000};
RECT rightWall = { 770, 0, 800, 1000 };

// UI
UIBattleStatus battleStatus;

// À¯´Ö
Unit unitA({ 100, 300 }, 30, 100, 40);////
AIUnit unitB({ 700, 300 }, 30, 100, 40);////

// Á×´Â ÀÌÆåÆ®
DyingMotion dyingMotion(12, 10, 0, 20);

// ¹æÇØ¹°
Obstacle obstacle[2]{ { 5, 0, 30, { 200, 300, 250, 500 } }, { 5, 40, 50, { 450,200,500,400 }} };
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static HANDLE hTimer;
	static bool isMoveUp = false;

	switch (iMessage)
	{
	case WM_CREATE:
		hTimer = (HANDLE)SetTimer(hWnd, 0, 20, NULL);
		unitA.SetHPEN(CreatePen(PS_ALTERNATE, 1, RGB(0, 255, 0)));
		unitA.SetHBRUSH(CreateSolidBrush(RGB(150, 150, 255)));
		unitA.SetBulletHPEN(CreatePen(PS_DASHDOT, 1, RGB(0, 0, 0)));
		unitA.SetBulletHBRUSH((HBRUSH)GetStockObject(NULL_BRUSH));

		unitB.SetHPEN(CreatePen(PS_ALTERNATE, 1, RGB(0, 255, 0)));
		unitB.SetHBRUSH(CreateSolidBrush(RGB(255, 150, 150)));
		unitB.SetBulletHPEN(CreatePen(PS_DASH, 1, RGB(255, 255, 0)));
		unitB.SetBulletHBRUSH(CreateSolidBrush(RGB(150, 100, 50)));

		obstacle[0].hBrush = CreateSolidBrush(RGB(70, 130, 180));
		obstacle[1].hBrush = CreateSolidBrush(RGB(0, 250, 154));

		battleStatus.Init(800, 100, &unitA, &unitB, { 200, 0 }, { 550, 0 });

		battleStatus.SetHbRemainHp(CreateSolidBrush(RGB(200, 0, 0)));
		battleStatus.SetHpRemainHp(CreatePen(PS_COSMETIC, 1, RGB(200, 80, 80)));
		battleStatus.SetHbMaxHp(CreateSolidBrush(RGB(100, 100, 100)));
		battleStatus.SetHpMaxHp(CreatePen(PS_COSMETIC, 1, RGB(0, 0, 0)));
		battleStatus.SetHbBackground(CreateSolidBrush(RGB(100, 100, 100)));

		battleStatus.SetHpBarWidth(200);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W':
		case 'w':
			unitA.Move({ 0, -5 });
			break;
		case 'S':
		case 's':
			unitA.Move({ 0, 5 });
			break;
		case 'Q':
		case 'q':
			unitA.UnitChange();
			break;
		case 'E':
		case 'e':
			unitA.UnitChange();
			break;
		case VK_SPACE:
			unitA.Shoot(false);
			break;
		}
		InvalidateRect(g_hWnd, NULL, true);
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);
		Rectangle(hdc, leftWall.left, leftWall.top, leftWall.right, leftWall.bottom);
		Rectangle(hdc, rightWall.left, rightWall.top, rightWall.right, rightWall.bottom);

		unitA.Render(hdc);
		unitB.Render(hdc);

		battleStatus.Render(hdc);
		dyingMotion.Render(hdc);

		obstacle[0].Render(hdc);
		obstacle[1].Render(hdc);

		EndPaint(g_hWnd, &ps);
		break;
	case WM_TIMER:
		
		unitA.Update();
		unitB.Update();

		obstacle[0].Update();
		obstacle[1].Update();

		//A
  		if (unitB.IsAlive() && unitA.GetBullet().IsCollition(unitB.GetRect()))
		{
			unitA.InitBullet();
			unitB.Hit();
		}
		else if (obstacle[0].isPlay && unitA.GetBullet().IsCollition(obstacle[0].rc))
		{
			unitA.InitBullet();
		}
		else if (obstacle[1].isPlay && unitA.GetBullet().IsCollition(obstacle[1].rc))
		{
			unitA.InitBullet();
		}
		else if (unitA.GetBullet().IsCollition(rightWall))
		{
			unitA.InitBullet();
		}

		//B
		if (unitA.IsAlive() && unitB.GetBullet().IsCollition(unitA.GetRect()))
		{
			unitB.InitBullet();
			unitA.Hit();
		}
		else if (obstacle[0].isPlay && unitB.GetBullet().IsCollition(obstacle[0].rc))
		{
			unitB.InitBullet();
		}
		else if (obstacle[1].isPlay && unitB.GetBullet().IsCollition(obstacle[1].rc))
		{
			unitB.InitBullet();
		}
		else if (unitB.GetBullet().IsCollition(leftWall))
		{
			unitB.InitBullet();
		}

		dyingMotion.Update();
		InvalidateRect(g_hWnd, NULL, true);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, (UINT_PTR)hTimer);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

RECT RectCenter(POINT p, int width, int height)
{
	RECT rc = { p.x - width / 2, p.y - height / 2, p.x + width / 2, p.y + height / 2 };
	return rc;
}

void RenderRect(HDC hdc, RECT rc, HBRUSH hBrush, HPEN hPen)
{
	HBRUSH hOldBrush = NULL;
	HPEN hOldPen = NULL;
	if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hPen);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	if (hPen != NULL) SelectObject(hdc, hOldPen);
	if (hBrush != NULL) SelectObject(hdc, hOldBrush);
}

void RenderCircle(HDC hdc, RECT rc, HBRUSH hBrush, HPEN hPen)
{
	HBRUSH hOldBrush = NULL;
	HPEN hOldPen = NULL;
	if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hPen);
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
	if (hPen != NULL) SelectObject(hdc, hOldPen);
	if (hBrush != NULL) SelectObject(hdc, hOldBrush);
}

void DyingMotionToCenter(HDC hdc, int x, int y, int size)
{
	Ellipse(hdc, x - (size / 2), y - (size / 2), x + (size / 2), y + (size / 2));
}
//x
void RenderPlayer(HDC hdc, RECT rc, POINT p, HBRUSH hBrush, HPEN hPen)
{
	MoveToEx(hdc, p.x, p.y - 20, NULL);
	LineTo(hdc, p.x - 40, p.y - 30);
	LineTo(hdc, p.x + 60, p.y - 10);
	LineTo(hdc, p.x, p.y - 20);
	Ellipse(hdc, rc.left, rc.top + 30, rc.right, rc.bottom - 30);
	Ellipse(hdc, rc.left + 50, rc.top + 40, rc.right + 60, rc.bottom - 40);
	MoveToEx(hdc, p.x, p.y + 20, NULL);
	LineTo(hdc, p.x - 40, p.y + 30);
	LineTo(hdc, p.x + 60, p.y + 10);
	LineTo(hdc, p.x, p.y + 20);
}

void RenderPlayer2(HDC hdc, RECT rc, POINT p, HBRUSH hBrush, HPEN hPen)
{
	MoveToEx(hdc, p.x, p.y - 20, NULL);
	LineTo(hdc, p.x + 40, p.y - 30);
	LineTo(hdc, p.x - 60, p.y - 10);
	LineTo(hdc, p.x, p.y - 20);
	Ellipse(hdc, rc.left, rc.top + 30, rc.right, rc.bottom - 30);
	Ellipse(hdc, rc.left - 60, rc.top + 40, rc.right - 50, rc.bottom - 40);
	MoveToEx(hdc, p.x, p.y + 20, NULL);
	LineTo(hdc, p.x + 40, p.y + 30);
	LineTo(hdc, p.x - 60, p.y + 10);
	LineTo(hdc, p.x, p.y + 20);
} //x
