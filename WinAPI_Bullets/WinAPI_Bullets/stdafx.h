#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <ctime>
#include <map>

#define WINPOS_STARTX 0
#define WINPOS_STARTY 0
#define WINSIZE_WIDTH 600
#define WINSIZE_HEIGHT 800

#define PI 3.141592f
#define U_GR_CENTER 0x01

#define U_EF_NONE 0x00
#define U_EF_MOSAIC 0x01
#define U_EF_BLUR 0x02
#define U_EF_BLACKHOLE 0x04
#define U_EF_PUZZLE 0x08

#define SAFE_DELETE(p) { if (p) {delete p; p = nullptr;} }
#define SAFE_RELEASE(p) { if (p) {p->Release(); delete p; p = nullptr;} }

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern bool isDebugMode;
extern POINTFLOAT g_mouse;

using namespace std;

#include "KeyManager.h"
#include "ImageManager.h"
#include "TimerManager.h"

inline RECT GetRect(POINTFLOAT pos, int width, int height, UINT uFlag)
{
	if (uFlag & U_GR_CENTER)
	{
		return RECT{ (int)pos.x - width / 2, (int)pos.y - height / 2, (int)pos.x + width / 2, (int)pos.y + height / 2 };
	}
	else
	{
		return RECT{ (int)pos.x, (int)pos.y, (int)pos.x + width, (int)pos.y + height };
	}
}