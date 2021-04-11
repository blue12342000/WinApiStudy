#include "stdafx.h"
#include "MainGame.h"
#include "Image.h"
#include "Enemy.h"

HRESULT MainGame::Init()
{
	srand(time(NULL));
	
	KeyManager::GetInstance()->Init();

	hTimer = (HWND)SetTimer(g_hWnd, NULL, 10, NULL);

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

	enemyManager.Release();

	KeyManager::GetInstance()->ReleaseSingleton();
}

void MainGame::Update()
{
	enemyManager.Update();
	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render(HDC hdc)
{
	HDC hBackDC = backBuffer->GetMemoryDC();

	background->Render(hBackDC);
	enemyManager.Render(hBackDC);

	backBuffer->Render(hdc);
}

LRESULT MainGame::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		enemyManager.CreateEnemy(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		enemyManager.Fire(4.5f, 3.141582f * ((float)(rand() % 360) / 180));
		break;
	case WM_TIMER:
		if (isInitalize) Update();
		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);
		if (isInitalize) Render(hdc);
		EndPaint(g_hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(g_hWnd, 0);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
