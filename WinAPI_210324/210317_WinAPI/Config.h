//config.h
//게임엔진
#pragma once

#include <Windows.h>
//매크로로 문자로 정의
#define WINSIZE_X 1080
#define WINSIZE_Y 720
#include <string>
#include <cmath>

#define PI 3.141592f

using namespace std;

extern HWND g_hWnd; //파일 어디서든 쓸 수 있는 변수

inline void RenderRectangle(HDC hdc, RECT rc, HBRUSH hBrush = NULL, HPEN hPen = NULL)
{
	HBRUSH hOldBrush = NULL;
	HPEN hOldPen = NULL;
	if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hBrush);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	if (hPen != NULL) SelectObject(hdc, hOldPen);
	if (hBrush != NULL) SelectObject(hdc, hOldBrush);
}

inline void RenderEllipse(HDC hdc, RECT rc, HBRUSH hBrush = NULL, HPEN hPen = NULL)
{
	HBRUSH hOldBrush = NULL;
	HPEN hOldPen = NULL;
	if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hBrush);
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
	if (hPen != NULL) SelectObject(hdc, hOldPen);
	if (hBrush != NULL) SelectObject(hdc, hOldBrush);
}