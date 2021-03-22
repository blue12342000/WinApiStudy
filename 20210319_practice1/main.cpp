#define _USE_MATH_DEFINES

#include <Windows.h>
#include <string>
#include <cmath>


using namespace std;

HINSTANCE g_hInstance;
HWND g_hWnd;
LPSTR g_lpszClass = (LPSTR)TEXT("윈도우 API");


LRESULT CALLBACK WinProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpsCmdParam, int nCmdShow);

void RenderRect(HDC hdc, int x, int y, int width, int height);
void RenderRectToCenter(HDC hdc, int x, int y, int width, int height);
void RenderEllipse(HDC hdc, int x, int y, int radius);
void RenderEllipseToCenter(HDC hdc, int x, int y, int radius);
void RenderLineToPoint(HDC hdc, POINT p1, POINT p2);
void RenderStar(HDC hdc, POINT center, int size);

struct Car
{
	int x, y;
	POINT mainFramePos = { 0, -50 };
	POINT mfWidthHeight = { 200, 80 };
	int tierRadius = 40;
	POINT tier[2]{ {-50, -20}, {50, -20} };

	Car() {}
	Car(int x, int y) :x(x), y(y) {}

	void Render(HDC hdc)
	{
		RenderRectToCenter(hdc, x + mainFramePos.x, y + mainFramePos.y - 50, mfWidthHeight.x / 2, mfWidthHeight.y);
		RenderRectToCenter(hdc, x + mainFramePos.x, y + mainFramePos.y, mfWidthHeight.x, mfWidthHeight.y);
		for (POINT p : tier)
		{
			RenderEllipseToCenter(hdc, x + p.x, y + p.y, tierRadius);
		}
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpsCmdParam, int nCmdShow)
{
	g_hInstance = hInstance;

	// 클래스생성
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WinProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, 1080, 800, NULL, NULL, g_hInstance, NULL);

	ShowWindow(g_hWnd, nCmdShow);

	MSG message;
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}

// 실습. 구구단 2단 출력
/*
	별그리는 함수를 만들고 클릭시 그 위치에 출력
	마우스 왼쪽 클릭 + 이동시 네모상자를 이동
	바닥에 네모를 10개 만들고 마우스 드레그를 통해 위로 이동된 위치에 별로 출력
*/

class Cube
{
private:
	POINT point;
	int width;
	int height;
	RECT rc;

	Cube* childCube = nullptr;

public:
	Cube() {};
	Cube(int x, int y, int width, int height)
	{
		point.x = x;
		point.y = y;
		this->width = width;
		this->height = height;
		SetRect();
	}

	RECT GetRc()
	{
		return rc;
	}

	void SetChild(Cube* cube)
	{
		childCube = cube;
	}

	POINT GetPoint()
	{
		return point;
	}

	Cube* GetChildCube()
	{
		return childCube;
	}

	void Move(int x, int y)
	{
		RePosition(x, y);
		if (childCube)
		{
			int deltaX, deltaY;
			int newX = childCube->GetPoint().x, newY = childCube->GetPoint().y;
			RECT childRc = childCube->GetRc();
			int collision = IsCollision(childRc);
			if (collision & 1)
			{
				// left
				deltaX = rc.left - childRc.left;
				if (deltaX > 0)
				{
					newX += deltaX;
				}
			}
			if (collision & 2)
			{
				// top
				deltaY = rc.top - childRc.top;
				if (deltaY > 0)
				{
					newY += deltaY;
				}
			}
			if (collision & 4)
			{
				// right
				deltaX = rc.right - childRc.right;
				if (deltaX < 0)
				{
					newX += deltaX;
				}
			}
			if (collision & 8)
			{
				// bottom
				deltaY = rc.bottom - childRc.bottom;
				if (deltaY < 0)
				{
					newY += deltaY;
				}
			}

			childCube->Move(newX, newY);
		}
	}

	void RePosition(int x, int y)
	{
		point.x = x;
		point.y = y;
		SetRect();
	}

	void Render(HDC hdc)
	{
		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		if (childCube) childCube->Render(hdc);
	}

	void Release()
	{
		if (childCube) childCube->Release();
		delete childCube;
	}

	int IsCollision(RECT targetRc)
	{
		int result = 0;
		// left, top, right, bottom;
		if (targetRc.left < rc.left)
		{
			result |= 1;
		}
		if (targetRc.top < rc.top)
		{
			result |= 2;
		}
		if (targetRc.right > rc.right)
		{
			result |= 4;
		}
		if (targetRc.bottom > rc.bottom)
		{
			result |= 8;
		}
		return result;
	}

	bool IsInCube(int x, int y)
	{
		if (x < rc.left || y < rc.top || x > rc.right || y > rc.bottom)
		{
			return false;
		}
		return true;
	}

private:
	void SetRect()
	{
		rc.left = point.x - width / 2;
		rc.right = point.x + width / 2;
		rc.top = point.y - height / 2;
		rc.bottom = point.y + height / 2;
	}
};

class TransformCube
{
public:
	enum CuteState
	{
		Rect = 0, Star
	};
	int x;
	int y;
	int size = 50;
	CuteState state = CuteState::Rect;

	bool IsSelectCube(int mx, int my)
	{
		if (state != CuteState::Rect) return false;
		if (x - size < mx && mx < x + size && y - size < my && my < y + size)
		{
			return true;
		}
		return false;
	}

	void Render(HDC hdc)
	{
		POINT p;
		p.x = x;
		p.y = y;
		switch (state)
		{
		case TransformCube::Rect:
			RenderRectToCenter(hdc, x, y, size * 2, size * 2);
			break;
		case TransformCube::Star:
			RenderStar(hdc, p, size);
			break;
		}
	}
};

TransformCube* pTransformCube = nullptr;
int x;
int y;
Cube cube(80, 80, 100, 100);
LRESULT CALLBACK WinProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	TransformCube* pPoint = NULL;
	HDC hdc;
	PAINTSTRUCT ps;
	string text = "";
	static TransformCube transformCube[10];

	switch (iMessage)
	{
	case WM_CREATE:
		cube.SetChild(new Cube(80, 80, 60, 60));
		cube.GetChildCube()->SetChild(new Cube(80, 80, 20, 20));
		for (int i = 0; i < 10; ++i)
		{
			transformCube[i] = { 50 + i * 100, 500 };
		}
		break;
	case WM_KEYDOWN:

		switch (wParam)
		{
		case 'w':
		case 'W':
			cube.Move(cube.GetPoint().x, cube.GetPoint().y - 5);
			break;
		case 'a':
		case 'A':
			cube.Move(cube.GetPoint().x - 5, cube.GetPoint().y);
			break;
		case 's':
		case 'S':
			cube.Move(cube.GetPoint().x, cube.GetPoint().y + 5);
			break;
		case 'd':
		case 'D':
			cube.Move(cube.GetPoint().x + 5, cube.GetPoint().y);
			break;
		}

		InvalidateRect(g_hWnd, NULL, true);
		break;
	case WM_MOUSEMOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (pTransformCube)
		{
			pTransformCube->x = x;
			pTransformCube->y = y;
			InvalidateRect(g_hWnd, NULL, true);
		}

		break;
	case WM_LBUTTONDOWN:
		for (int i = 0; i < 10; ++i)
		{
			if (transformCube[i].IsSelectCube(x, y))
			{
				pTransformCube = &transformCube[i];
				break;
			}
		}
		InvalidateRect(g_hWnd, NULL, true);
		break;
	case WM_LBUTTONUP:
		if (pTransformCube)
		{
			pTransformCube->state = TransformCube::CuteState::Star;
		}

		pTransformCube = nullptr;
		InvalidateRect(g_hWnd, NULL, true);
		break;
	case WM_RBUTTONDOWN:
		InvalidateRect(g_hWnd, NULL, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);

		text = to_string(x) + ", " + to_string(y);
		TextOut(hdc, 10, 10, text.c_str(), text.length());

		for (int i = 0; i < 10; ++i)
		{
			transformCube[i].Render(hdc);
		}

		cube.Render(hdc);

		EndPaint(g_hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void RenderRect(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

void RenderRectToCenter(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

void RenderEllipse(HDC hdc, int x, int y, int radius)
{
	Ellipse(hdc, x, y, x + radius * 2, y + radius * 2);
}

void RenderEllipseToCenter(HDC hdc, int x, int y, int radius)
{
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
}

void RenderLineToPoint(HDC hdc, POINT p1, POINT p2)
{
	MoveToEx(hdc, p1.x, p1.y, NULL);
	LineTo(hdc, p2.x, p2.y);
}

void RenderStar(HDC hdc, POINT center, int size)
{
	RenderEllipseToCenter(hdc, center.x, center.y, size);

	POINT point[5];
	double radian = M_PI / 2;
	for (int i = 0; i < 5; ++i)
	{
		point[i].x = center.x - cos(radian) * size;
		point[i].y = center.y - sin(radian) * size;

		radian += (M_PI * 2 / 5);
	}

	MoveToEx(hdc, point[0].x, point[0].y, NULL);
	LineTo(hdc, point[3].x, point[3].y);
	LineTo(hdc, point[1].x, point[1].y);
	LineTo(hdc, point[4].x, point[4].y);
	LineTo(hdc, point[2].x, point[2].y);
	LineTo(hdc, point[0].x, point[0].y);
}