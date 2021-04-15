#pragma once
#include "EnemyManager.h"

class Image;
class SpaceShip;
class MainGame
{
private:
	bool isInitalize = false;
	HDC hdc;

	Image* backBuffer;
	Image* background;

	EnemyManager enemyManager;

	SpaceShip* spaceShip;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

