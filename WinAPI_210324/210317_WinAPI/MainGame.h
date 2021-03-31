#pragma once
#include <Windows.h>
#include "GameNode.h"
#include "Tank.h"
#include "Enemy.h"

class Image;
class Charactor;
class MainGame : public GameNode
{
private:
	enum GameStage
	{
		GS_ONE = 1, GS_TWO, GS_THREE, GS_FOUR, GS_FIVE, GS_SIX, GS_SEVEN, GS_AEIGHT, GS_END
	};

	HDC hdc;
	HDC bgdc;
	PAINTSTRUCT ps;
	HANDLE timer;

	bool IsInited = false;

	Tank tank;

	int enemyNum;
	Enemy* enemy;

	GameStage stage = GS_END;

	Image* backBuffer;
	Image* imageBin;

	Charactor* lasswellKing;
public:

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	bool IsRectCollision(RECT target, RECT other);
	bool IsCircleCollision(POINTFLOAT targetPos, int targetRadius, POINTFLOAT otherPos, int otherRadius);
	bool IsCollision(RECT target, POINTFLOAT otherPos, int otherRadius);

	void Reset();
	void CreateEnemy(int count);

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

