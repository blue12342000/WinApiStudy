#pragma once
#include "Config.h"

extern HWND g_hWnd;

class GameNode
{
public:
	virtual void Init() {}
	virtual void Update() {}
	virtual void Render(HDC hdc) {}
	virtual void Release() {}
};

