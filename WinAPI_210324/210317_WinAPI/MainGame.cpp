#include "MainGame.h"

HRESULT MainGame::Init()
{
	timer = (HANDLE)SetTimer(g_hWnd, 0, 10, NULL);
	tank.Init();
	enemyNum = 10;
	enemy = new Enemy[enemyNum];
	for (int i = 0; i < enemyNum; ++i)
	{
		enemy[i] = Enemy(10, rand() % 2 + 0.3f);
		enemy[i].SetTank(&tank);
		enemy[i].SetPos({(float)(rand() % WINSIZE_X), (float)(rand() % 100) });
	}
	IsInited = true;
	return S_OK;
}

void MainGame::Update()
{
	tank.Update();
	int bulletNum = tank.GetBulletNum();
	Bullet* bullets = tank.GetBullets();
	Bullet* skillBullet = tank.GetSkillBulletPtr();

	for (int i = 0; i < enemyNum; ++i)
	{
		enemy[i].Update();
		for (int l = 0; l < bulletNum; ++l)
		{
			if (bullets[l].GetIsShoot() && enemy[i].GetState() == Enemy::EnemyState::ALIVE
				&& IsRectCollision(enemy[i].GetRect(), bullets[l].GetRect()))
			{
				enemy[i].SetState(Enemy::EnemyState::DEAD);
				bullets[l].SetIsShoot(false);
				break;
			}
		}
		if (skillBullet->GetIsShoot() && enemy[i].GetState() == Enemy::EnemyState::ALIVE
			&& skillBullet->IsRectCollision(enemy[i].GetRect()))
		{
			enemy[i].SetState(Enemy::EnemyState::DEAD);
		}
	}
	
}

void MainGame::Render(HDC hdc)
{
	tank.Render(hdc);
	for (int i = 0; i < enemyNum; ++i)
	{
		enemy[i].Render(hdc);
	}
}

void MainGame::Release()
{
	tank.Release();
	enemy->Release();
	delete[] enemy;
	KillTimer(g_hWnd, 0);
}

bool MainGame::IsRectCollision(RECT target, RECT other)
{
	if (target.right < other.left || target.left > other.right
		|| target.bottom < other.top || target.top > other.bottom
		|| other.right < target.left || other.left > target.right
		|| other.bottom < target.top || other.top > target.bottom)
	{
		return false;
	}

	return true;
}

bool MainGame::IsCircleCollision(POINTFLOAT targetPos, int targetRadius, POINTFLOAT otherPos, int otherRadius)
{
	return false;
}

bool MainGame::IsCollision(RECT target, POINTFLOAT otherPos, int otherRadius)
{
	return false;
}

LRESULT MainGame::MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'w':
		case 'W':
			tank.Move({ 0, -5 });
			break;
		case 'a':
		case 'A':
			tank.Move({ -5, 0 });
			break;
		case 's':
		case 'S':
			tank.Move({ 0, 5 });
			break;
		case 'd':
		case 'D':
			tank.Move({ 5, 0 });
			break;
		case 'f':
		case 'F':
			tank.FireSpecial();
			break;
		case VK_SPACE:
			tank.Fire();
			break;
		case VK_LEFT:
			tank.RotateFire(-5);
			break;
		case VK_RIGHT:
			tank.RotateFire(+5);
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);
		if (IsInited) Render(hdc);
		EndPaint(g_hWnd, &ps);
		break;
	case WM_TIMER:
		if (IsInited) Update();
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_DESTROY:
		Release();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
