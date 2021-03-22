#include <Windows.h>
#include <cmath>

HINSTANCE g_hInstance;
HWND g_hWnd;
LPSTR g_lpszClass = (LPSTR)TEXT("Window API 포물선그리기");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdParam, int nCmdShow);
void RenderLine(HDC hdc, POINT from, POINT to);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	g_hInstance = hInstance;

	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, g_hInstance, NULL);

	ShowWindow(g_hWnd, nCmdShow);

	MSG message;
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}

struct Circle
{
	int radius;
	POINTFLOAT originPoint;
	POINTFLOAT point;
	RECT rc;

	float speedX;
	float speedY;
	float radian;

	Circle() {}
	Circle(int x, int y, int r)
	{
		point.x = x;
		point.y = y;
		radius = r;
		SetRect();

		originPoint.x = x;
		originPoint.y = y;
	}

	void ThrowCircle(float speed, float angle)
	{
		this->speedX = speed;
		this->speedY = speed;
		this->radian = angle * 3.141592f / 180;
	}

	void SetRect()
	{
		rc.left = point.x - radius;
		rc.right = point.x + radius;
		rc.top = point.y - radius;
		rc.bottom = point.y + radius;
	}

	void Move()
	{
		POINTFLOAT deltaPoint = { speedX * cos(radian), speedY * sin(radian) };

		point.x += deltaPoint.x;
		point.y -= deltaPoint.y;

		speedY -= 9.8f;

		SetRect();
	}

	void Move(int tick)
	{
		//y = a * pow(x - h, 2) + k;
		POINTFLOAT deltaPoint = { tick, 5 * pow(tick * 0.1f - 1, 2) - 5 };

		point.x = originPoint.x + deltaPoint.x * 10;
		point.y = originPoint.y + deltaPoint.y * 10;

		SetRect();
	}

	void Render(HDC hdc)
	{
		Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
	}
};

Circle circle(100, 400, 20);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int tick = 0;
	switch (iMessage)
	{
	case WM_CREATE:
		circle.ThrowCircle(9.8f * 6, 45);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			circle.Move(++tick);
			InvalidateRect(g_hWnd, NULL, NULL);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);

		circle.Render(hdc);
		RenderLine(hdc, { 0, 420 }, { 800, 420 });

		EndPaint(g_hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void RenderLine(HDC hdc, POINT from, POINT to)
{
	MoveToEx(hdc, from.x, from.y, NULL);
	LineTo(hdc, to.x, to.y);
}