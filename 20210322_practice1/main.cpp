#include <Windows.h>
#include <ctime>
#include <cmath>

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

struct DyingMotion
{
	bool isPlay = false;
	int pice;
	POINT pos;
	float speed;
	int tick;
	int lifecycle;

	DyingMotion() {}
	DyingMotion(int pice, float speed, int tick, int lifecycle)
	{
		this->pice = pice;
		this->speed = speed;
		this->tick = tick;
		this->lifecycle = lifecycle;
	}

	void Play(POINT pos)
	{
		isPlay = true;
		this->pos = pos;
		this->tick = 0;
	}

	void Update()
	{
		if (isPlay == true)
		{
			this->tick = tick + 1;
			if (tick == lifecycle)
				isPlay = false;
		}
	}

	void Render(HDC hdc)
	{
		if (isPlay == true)
		{
			for (int i = 0; i < 360; i += 360 / pice)
			{
				float radian = 3.14f * i / 180;
				POINT point = pos;

				point.x += cos(radian) * speed * tick;
				point.y += sin(radian) * speed * tick;

				HBRUSH hBrush, hOldBrush;
				hBrush = CreateSolidBrush(RGB(250, 250, 210));
				hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				DyingMotionToCenter(hdc, point.x, point.y, 20);
			}
		}
	}
};
DyingMotion dyingMotion(12, 10, 0, 20);


struct Obstacle
{
	bool isPlay = false;
	int pice;
	POINT pos;
	int tick;
	int lifecycle;
	RECT rc;
	HBRUSH hBrush = NULL;
	HPEN hPen = NULL;

	Obstacle() {}
	Obstacle(int pice, int tick, int lifecycle, RECT rc)
	{
		this->pice = pice;
		this->tick = tick;
		this->lifecycle = lifecycle;
		this->rc = rc;
	}

	void Play(POINT pos)
	{
		isPlay = true;
		this->pos = pos;
		this->tick = 0;
	}

	void Update()
	{
		if (isPlay == true)
		{
			this->tick = tick + 1;
			if (tick == lifecycle)
			{
				isPlay = false;
				this->tick = 0;
			}
		}
		else
		{
			this->tick = tick + 1;
			if (tick == lifecycle)
			{
				isPlay = true;
				this->tick = 0;
			}
		}
	}

	void Render(HDC hdc)
	{
		if (isPlay == true)
		{
			RenderRect(hdc, rc, hBrush, hPen);
		}
	}
};
Obstacle obstacle[2]{ { 5, 0, 30, { 200, 300, 250, 500 } }, { 5, 40, 50, { 450,200,500,400 }} };


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


	bool up()
	{
		const double PI = 3.1415926;
		double b = cos(pos.x * PI / 90);
		if (b == 1)
		{
			return 1;
		}
		else if (b == -1)
		{
			return 0;
		}
	}

	void Render(HDC hdc)
	{

		HBRUSH hOldBrush = NULL;
		HPEN hOldPen = NULL;
		const double PI = 3.1415926;
		double b = cos(pos.x * PI / 90);

		RECT rectA = { rc.left + 10, rc.top - 50 * b, rc.right - 10, rc.bottom - 50 * b };
		RECT rectB = { rc.left + 10, rc.top + 50 * b, rc.right - 10, rc.bottom + 50 * b };
		if (up())
		{
			RenderCircle(hdc, rectA, hBrush, hPen);
		}
		else
		{
			RenderCircle(hdc, rectB, NULL, NULL);
		}
		RenderCircle(hdc, rc, hBrush, hPen);
		if (up())
		{
			RenderCircle(hdc, rectB, NULL, NULL);
		}
		else
		{
			RenderCircle(hdc, rectA, hBrush, hPen);
		}

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

	int hp;
	int maxHp;

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
	Unit(POINT pos, int width, int height, float bulletSize) :pos(pos), width(width), height(height), hp(100), maxHp(100) { bullet = UnitBullet(pos, bulletSize, 10); SetRect(); }

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

	int GetHp() { return hp; }
	int GetMaxHp() { return maxHp; }

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

	bool IsAlive()
	{
		return state != DEAD;
	}

	void Hit()
	{
		hp -= 20;
		if (hp <= 0)
		{
			hp = 0;
			state = DEAD;
			dyingMotion.Play(pos);
		}
		SetRect();
	}

	void Update()
	{
		if (isShoot)
		{
			bullet.Move(isShootLeft);
		}
	}
	
	void Render1(HDC hdc)///////
	{
		if (state == ALIVE)
		{
			HBRUSH hOldBrush = NULL;
			HPEN hOldPen = NULL;
			if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hPen);
			RenderPlayer(hdc, rc, pos, hBrush, hPen);
			if (hPen != NULL) SelectObject(hdc, hOldPen);
			if (hBrush != NULL) SelectObject(hdc, hOldBrush);
			bullet.Render(hdc);
		}
		else if (isShoot)
		{
			bullet.Render(hdc);
		}
	}

	void Render2(HDC hdc)/////
	{
		if (state == ALIVE)
		{
			HBRUSH hOldBrush = NULL;
			HPEN hOldPen = NULL;
			if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hPen);
			RenderPlayer2(hdc, rc, pos, hBrush, hPen);
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

class UIBattleStatus
{
	POINT unitAPoint;
	Unit* unitA = nullptr;
	
	POINT unitBPoint;
	Unit* unitB = nullptr;

	HBRUSH hbBackground = NULL;
	HPEN hpBackground = NULL;
	RECT bgRect;

	HBRUSH hbRemainHp = NULL;
	HPEN hpRemainHp = NULL;
	RECT remainHpRect;

	HBRUSH hbMaxHp = NULL;
	HPEN hpMaxHp = NULL;
	RECT maxHpRect;

	int hpBarWidth = 100;
	int hpBarHeight = 40;

public:
	void SetHbBackground(HBRUSH hbBackground) { this->hbBackground = hbBackground; }
	void SetHpBackground(HPEN hpBackground) { this->hpBackground = hpBackground; }
	void SetHbRemainHp(HBRUSH hbRemainHp) { this->hbRemainHp = hbRemainHp; }
	void SetHpRemainHp(HPEN hpRemainHp) { this->hpRemainHp = hpRemainHp; }
	void SetHbMaxHp(HBRUSH hbMaxHp) { this->hbMaxHp = hbMaxHp; }
	void SetHpMaxHp(HPEN hpMaxHp) { this->hpMaxHp = hpMaxHp; }

	void SetHpBarWidth(int hpBarWidth) { this->hpBarWidth = hpBarWidth; }
	void SetHpBarHeight(int hpBarHeight) { this->hpBarHeight = hpBarHeight; }

	void Init(int width, int height, Unit* unitA, Unit* unitB, POINT unitAPoint, POINT unitBPoint)
	{
		bgRect = { 0, 0, width, height };

		this->unitA = unitA;
		this->unitB = unitB;

		this->unitAPoint = unitAPoint;
		this->unitBPoint = unitBPoint;
	}

	void Render(HDC hdc)
	{
		RenderRect(hdc, bgRect, hbBackground, hpBackground);

		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH), hOldBrush;
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		TextOut(hdc, unitAPoint.x - 30, unitAPoint.y + 15, "PLAYER", 6);
		TextOut(hdc, unitBPoint.x - 25, unitBPoint.y + 15, "ENEMY", 5);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);

		maxHpRect = RectCenter({ unitAPoint.x, unitAPoint.y + 60 }, hpBarWidth, hpBarHeight);
		RenderRect(hdc, maxHpRect, hbMaxHp, hpMaxHp);
		maxHpRect = RectCenter({ unitBPoint.x, unitBPoint.y + 60 }, hpBarWidth, hpBarHeight);
		RenderRect(hdc, maxHpRect, hbMaxHp, hpMaxHp);

		remainHpRect = RectCenter({ unitAPoint.x, unitAPoint.y + 60 }, (float)hpBarWidth * unitA->GetHp() / unitA->GetMaxHp(), hpBarHeight);
		RenderRect(hdc, remainHpRect, hbRemainHp, hpRemainHp);
		remainHpRect = RectCenter({ unitBPoint.x, unitBPoint.y + 60 }, (float)hpBarWidth * unitB->GetHp() / unitB->GetMaxHp(), hpBarHeight);
		RenderRect(hdc, remainHpRect, hbRemainHp, hpRemainHp);
	}
};

RECT leftWall = {0, 0, 30, 1000};
RECT rightWall = { 770, 0, 800, 1000 };
Unit unitA({ 100, 300 }, 30, 100, 40);////
Unit unitB({ 700, 300 }, 30, 100, 40);////
HBRUSH g_hBrush = CreateSolidBrush(RGB(0, 0, 255));
UIBattleStatus battleStatus;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static HANDLE hTimer;
	static bool isMoveUp = false;

	switch (iMessage)
	{
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

		Rectangle(hdc, 0, 0, 800, 100);

		EndPaint(g_hWnd, &ps);
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
