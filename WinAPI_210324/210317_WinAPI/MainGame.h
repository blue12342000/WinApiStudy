#pragma once
#include <Windows.h>
#include "GameNode.h"
#include "Tank.h"
#include "Enemy.h"

class MainGame : public GameNode
{
private:
	HDC hdc;
	PAINTSTRUCT ps;
	HANDLE timer;

	bool IsInited = false;

	Tank tank;

	int enemyNum;
	Enemy* enemy;

public:

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	bool IsRectCollision(RECT target, RECT other);
	bool IsCircleCollision(POINTFLOAT targetPos, int targetRadius, POINTFLOAT otherPos, int otherRadius);
	bool IsCollision(RECT target, POINTFLOAT otherPos, int otherRadius);

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

