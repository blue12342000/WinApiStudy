#pragma once
#include <Windows.h>
#include "GameNode.h"
#include "Tank.h"

class MainGame : public GameNode
{
private:
	HDC hdc;
	PAINTSTRUCT ps;
	HANDLE timer;

	bool IsInited = false;

	Tank tank;
public:

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

