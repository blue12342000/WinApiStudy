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

#define U_GR_CENTER 0x01

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern bool isDebugMode;

using namespace std;

#include "KeyManager.h"

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