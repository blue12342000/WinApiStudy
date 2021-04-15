#include "stdafx.h"
#include "MainGame.h"
#include "Image.h"
#include "Enemy.h"
#include "SpaceShip.h"

HRESULT MainGame::Init()
{
	srand(time(NULL));
	
	KeyManager::GetInstance()->Init();
	ImageManager::GetInstance()->Init();

	//hTimer = (HWND)SetTimer(g_hWnd, NULL, 10, NULL);

	backBuffer = new Image();
	if (FAILED(backBuffer->Init(WINSIZE_WIDTH, WINSIZE_HEIGHT)))
	{
		delete backBuffer;
		backBuffer = nullptr;
		return E_FAIL;
	}

	background = new Image();
	if (FAILED(background->Init("Image/background.bmp", WINSIZE_WIDTH, WINSIZE_HEIGHT)))
	{
		delete background;
		background = nullptr;
		return E_FAIL;
	}

	hdc = GetDC(g_hWnd);

	spaceShip = new SpaceShip();
	spaceShip->Init();
	spaceShip->SetPos({WINSIZE_WIDTH / 2, WINSIZE_HEIGHT / 2});

	isInitalize = true;
	return S_OK;
}

void MainGame::Release()
{
	if (backBuffer)
	{
		backBuffer->Release();
		delete backBuffer;
		backBuffer = nullptr;
	}

	if (background)
	{
		background->Release();
		delete background;
		background = nullptr;
	}

	SAFE_RELEASE(spaceShip);

	enemyManager.Release();

	KeyManager::GetInstance()->ReleaseSingleton();
	ImageManager::GetInstance()->Release();
	ImageManager::GetInstance()->ReleaseSingleton();

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	enemyManager.Update();
	spaceShip->Update();
}

void MainGame::Render()
{
	HDC hBackDC = backBuffer->GetMemoryDC();

	background->Render(hBackDC);
	enemyManager.Render(hBackDC);
	spaceShip->Render(hBackDC);

	// Timer FPS
	TimerManager::GetInstance()->Render(hBackDC);
	backBuffer->Render(hdc);
}

LRESULT MainGame::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		g_mouse.x = (float)(LOWORD(lParam));
		g_mouse.y = (float)(HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		enemyManager.CreateEnemy(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		enemyManager.Fire(4.5f, 3.141582f * ((float)(rand() % 360) / 180));
		break;
	case WM_TIMER:
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
