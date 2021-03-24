#pragma once
#include <Windows.h>
#include "GameNode.h"
#include "Tank.h"

class MainGame : public GameNode
{
	Tank tank;
public:

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

