#include <Windows.h>
#include <ctime>

HINSTANCE g_hIndtance;
HWND g_hWnd;
LPSTR g_lpszClass = (LPSTR)TEXT("Win API");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdPAram, int nCmdShow);
RECT RectCenter(POINT p, int width, int height);

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

	//g_hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, g_hIndtance, NULL);

	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, g_hIndtance, NULL);

	ShowWindow(g_hWnd, nCmdShow);

	MSG message;
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}

struct UnitBullet
{
	int size;
	float speed;
	POINT pos;
	RECT rc;
	HPEN hPen;
	HBRUSH hBrush;

	UnitBullet() {}
	UnitBullet(POINT pos, int size, float speed) :pos(pos), size(size), speed(speed) { SetRect(); }

	void SetPos(POINT pos)
	{
		this->pos = pos;
		SetRect();
	}

	void SetRect()
	{
		rc = RectCenter(pos, size, size);
	}

	void Move(bool isShootLeft)
	{
		if (isShootLeft) pos.x -= speed;
		else pos.x += speed;
		SetRect();
	}

	void SetHPEN(HPEN hPen)
	{
		this->hPen = hPen;
	}

	void SetHBRUSH(HBRUSH hBrush)
	{
		this->hBrush = hBrush;
	}

	void Render(HDC hdc)
	{
		HBRUSH hOldBrush = NULL;
		HPEN hOldPen = NULL;
		if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hPen);
		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		if (hPen != NULL) SelectObject(hdc, hOldPen);
		if (hBrush != NULL) SelectObject(hdc, hOldBrush);
	}

	bool IsCollition(RECT other)
	{
		POINT points[4] = { {other.left, other.top}, {other.left, other.bottom}, {other.right, other.top}, {other.right, other.bottom} };

		for (POINT p : points)
		{
			if (p.x > rc.left && p.y > rc.top && p.x < rc.right && p.y < rc.bottom)
			{
				return true;
			}
		}

		POINT originPoints[4] = { {rc.left, rc.top}, {rc.left, rc.bottom}, {rc.right, rc.top}, {rc.right, rc.bottom} };

		for (POINT p : originPoints)
		{
			if (p.x > other.left && p.y > other.top && p.x < other.right && p.y < other.bottom)
			{
				return true;
			}
		}

		return false;
	}
};

class Unit
{
private:
	enum UnitState
	{
		ALIVE, DEAD
	};
	bool isShoot = false;
	bool isShootLeft = false;

	int width;
	int height;
	UnitState state = ALIVE;
	POINT pos;
	RECT rc;

	HPEN hPen = NULL;
	HBRUSH hBrush = NULL;

	UnitBullet bullet;

public:
	Unit() {}
	Unit(POINT pos, int width, int height, float bulletSize) :pos(pos), width(width), height(height) { bullet = UnitBullet(pos, bulletSize, 10); SetRect(); }

	void SetRect()
	{
		rc = RectCenter(pos, width, height);
	}

	RECT GetRect()
	{
		return rc;
	}

	UnitBullet GetBullet()
	{
		return bullet;
	}

	POINT GetPos()
	{
		return pos;
	}

	void SetHPEN(HPEN hPen)
	{
		this->hPen = hPen;
	}

	void SetHBRUSH(HBRUSH hBrush)
	{
		this->hBrush = hBrush;
	}

	void SetBulletHPEN(HPEN hPen)
	{
		bullet.SetHPEN(hPen);
	}

	void SetBulletHBRUSH(HBRUSH hBrush)
	{
		bullet.SetHBRUSH(hBrush);
	}

	void InitBullet()
	{
		isShoot = false;
		bullet.SetPos(pos);
	}

	void Move(POINT delta)
	{
		if (state == DEAD) return;
		
		pos.x += delta.x;
		pos.y += delta.y;
		if (!isShoot) bullet.SetPos(pos);
		SetRect();
	}

	void Shoot(bool isShootLeft)
	{
		if (state == DEAD) return;
		if (!isShoot)
		{
			isShoot = true;
			this->isShootLeft = isShootLeft;
		}
	}

	void Hit()
	{
		height -= 80;
		if (height <= 0)
		{
			state = DEAD;
		}
	}

	void Update()
	{
		if (isShoot)
		{
			bullet.Move(isShootLeft);
		}
	}
	
	void Render(HDC hdc)
	{
		if (state == ALIVE)
		{
			HBRUSH hOldBrush = NULL;
			HPEN hOldPen = NULL;
			if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hPen);
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			if (hPen != NULL) SelectObject(hdc, hOldPen);
			if (hBrush != NULL) SelectObject(hdc, hOldBrush);
			bullet.Render(hdc);
		}
		else if (isShoot)
		{
			bullet.Render(hdc);
		}
	}
};

RECT leftWall = {0, 0, 30, 1000};
RECT rightWall = { 670, 0, 700, 1000 };
Unit unitA({ 100, 300 }, 30, 400, 40);
Unit unitB({ 600, 300 }, 30, 400, 40);
HBRUSH g_hBrush = CreateSolidBrush(RGB(0, 0, 255));
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	// 그려질 선의 정보를 바꾼다.
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	static HANDLE hTimer;
	static bool isMoveUp = false;

	switch (iMessage)
	{
	case WM_CREATE:
		hTimer = (HANDLE)SetTimer(hWnd, 0, 50, NULL);
		unitA.SetHPEN(CreatePen(PS_ALTERNATE, 1, RGB(0, 255, 0)));
		unitA.SetHBRUSH(CreateSolidBrush(RGB(150, 150, 255)));
		unitA.SetBulletHPEN(CreatePen(PS_DASHDOT, 1, RGB(0, 0, 0)));
		unitA.SetBulletHBRUSH((HBRUSH)GetStockObject(NULL_BRUSH));

		unitB.SetHPEN(CreatePen(PS_ALTERNATE, 1, RGB(0, 255, 0)));
		unitB.SetHBRUSH(CreateSolidBrush(RGB(255, 150, 150)));
		unitB.SetBulletHPEN(CreatePen(PS_DASH, 1, RGB(255, 255, 0)));
		unitB.SetBulletHBRUSH(CreateSolidBrush(RGB(150, 100, 50)));
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
			SetClassLong(hWnd, GCLP_HBRBACKGROUND, (LONG)g_hBrush);
			break;
		case 'E':
		case 'e':
			SetClassLong(hWnd, GCLP_HBRBACKGROUND, (LONG)GetStockObject(BLACK_BRUSH));
			break;
		case VK_SPACE:
			unitA.Shoot(false);
			break;
		}
		InvalidateRect(g_hWnd, NULL, true);
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);
		unitA.Render(hdc);
		unitB.Render(hdc);
		Rectangle(hdc, leftWall.left, leftWall.top, leftWall.right, leftWall.bottom);
		Rectangle(hdc, rightWall.left, rightWall.top, rightWall.right, rightWall.bottom);
		EndPaint(g_hWnd, &ps);
		break;
	case WM_TIMER:
		if (isMoveUp) unitB.Move({ 0, -5 });
		else unitB.Move({ 0, 5 });

		if (rand() % 100 < 5) unitB.Shoot(true);
		unitA.Update();
		unitB.Update();

		if (unitA.GetBullet().IsCollition(unitB.GetRect()))
		{
			unitA.InitBullet();
			unitB.Hit();
		}
		else if (unitA.GetBullet().IsCollition(rightWall))
		{
			unitA.InitBullet();
		}

		if (unitB.GetBullet().IsCollition(unitA.GetRect()))
		{
			unitB.InitBullet();
			unitA.Hit();
		}
		else if (unitB.GetBullet().IsCollition(leftWall))
		{
			unitB.InitBullet();
		}

		if (unitB.GetPos().y < 10) isMoveUp = false;
		else if (unitB.GetPos().y > 490) isMoveUp = true;

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