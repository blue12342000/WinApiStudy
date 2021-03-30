#include "MainGame.h"
#include "SpecialBullet.h"

HRESULT MainGame::Init()
{
	timer = (HANDLE)SetTimer(g_hWnd, 0, 10, NULL);
	srand(time(NULL));

	tank.Init();
	tank.SetPoint({ WINSIZE_X / 2, WINSIZE_Y - 100 });
	
	stage = GS_ONE;
	CreateEnemy(stage);

	//keyMgr.Init();
	KeyManager::GetInstance()->Init();

	IsInited = true;
	return S_OK;
}

void MainGame::Update()
{
	KeyManager::GetInstance()->Update();
	if (KeyManager::GetInstance()->IsStayKeyDown('W'))
	{
		tank.Move({ 0, -5 });
	}
	else if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		tank.Move({ -5, 0 });
	}
	else if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		tank.Move({ 0, 5 });
	}
	else if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		tank.Move({ 5, 0 });
	}
	else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		tank.Fire();
	}
	else if (KeyManager::GetInstance()->IsOnceKeyDown('G'))
	{
		tank.FireGuide();
	}
	else if (KeyManager::GetInstance()->IsOnceKeyDown('F'))
	{
		tank.FireSpecial();
	}
	else if (KeyManager::GetInstance()->IsOnceKeyDown('H'))
	{
		tank.FireSignature();
		tank.GetSpecialBullet()->SetTarget(&enemy[0]);
	}

	int bulletNum = tank.GetBulletNum();
	Bullet* bullets = tank.GetBullets();
	Bullet* skillBullet = tank.GetSkillBulletPtr();

	if (stage != GS_END)
	{
		bool isComplete = true;
		for (int i = 0; i < enemyNum; ++i)
		{
			for (int l = 0; l < bulletNum; ++l)
			{
				if (bullets[l].GetIsShoot() && enemy[i].GetState() == Enemy::EnemyState::ALIVE
					&& IsCollision(enemy[i].GetRect(), bullets[l].GetPos(), bullets[l].GetSize()))
				{
					enemy[i].HitDamage(bullets[l].GetDamage());
					bullets[l].SetIsShoot(false);
					break;
				}
			}
			if (skillBullet->GetIsShoot() && enemy[i].GetState() == Enemy::EnemyState::ALIVE
				&& skillBullet->IsCollision(enemy[i].GetRect()))
			{
				enemy[i].HitDamage(skillBullet->GetDamage());
			}

			for (int l = 0; l < tank.GetGuideBulletCount(); ++l)
			{
				if (tank.GetGuideBulletPtr()[l].GetIsShoot())
				{
					if (enemy[i].GetState() == Enemy::EnemyState::ALIVE)
					{
						if (IsCollision(enemy[i].GetRect(), tank.GetGuideBulletPtr()[l].GetPos(), tank.GetGuideBulletPtr()[l].GetSize()))
						{
							tank.GetGuideBulletPtr()[l].SetIsShoot(false);
							enemy[i].HitDamage(tank.GetGuideBulletPtr()[l].GetDamage());
						}
						else if (!tank.GetGuideBulletPtr()[l].IsHasTarget()
							&& 0 < enemy[i].GetPos().x && enemy[i].GetPos().x < WINSIZE_X
							&& 0 < enemy[i].GetPos().y && enemy[i].GetPos().y < WINSIZE_Y)
						{
							tank.GetGuideBulletPtr()[l].SetTarget(&enemy[i]);
						}

					}
				}
			}
			enemy[i].Update();
			if (enemy[i].GetState() == Enemy::EnemyState::ALIVE)
			{
				isComplete = false;
			}
		}

		if (isComplete)
		{
			stage = (GameStage)(stage + 1);
			if (stage != GS_END)
			{
				Reset();
				CreateEnemy(stage);
			}
		}
	}
	tank.Update();
}

void MainGame::Render(HDC hdc)
{
	tank.Render(hdc);
	for (int i = 0; i < enemyNum; ++i)
	{
		enemy[i].Render(hdc);
	}
	if (stage != GS_END)
	{
		string str = "STAGE " + to_string(stage);
		TextOut(hdc, 10, 10, str.c_str(), str.length());
	}
	else
	{
		string str = "STAGE END";
		TextOut(hdc, 10, 10, str.c_str(), str.length());
	}
}

void MainGame::Release()
{
	tank.Release();
	for (int i = 0; i < enemyNum; ++i)
	{
		enemy[i].Release();
	}
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
	// 사각형 안에 확인
	if (target.left < otherPos.x && otherPos.x < target.right
		&& target.top < otherPos.y && otherPos.y < target.bottom)
	{
		return true;
	}
	
	// 각 꼭지점이 원안에 있는지 확인
	POINTFLOAT points[4] = { {target.left, target.top}, {target.left, target.bottom}, {target.right, target.top}, {target.right, target.bottom} };
	float dist;
	float maxDist = otherRadius * otherRadius;
	for (auto p : points)
	{
		dist = pow(p.x - otherPos.x, 2) + pow(p.y - otherPos.y, 2);
		if (dist < maxDist)
		{
			return true;
		}
	}

	// 각 면에서의 수직으로 나간 거리가 반지름보다 작은가 확인
	if (target.left < otherPos.x && otherPos.x < target.right)
	{
		float minDistY = min(abs(otherPos.y - target.top), abs(otherPos.y - target.bottom));
		if (minDistY < otherRadius)
		{
			return true;
		}
	}
	else if (target.top < otherPos.y && otherPos.y < target.bottom)
	{
		float minDistX = min(abs(otherPos.x - target.left), abs(otherPos.x - target.right));
		if (minDistX < otherRadius)
		{
			return true;
		}
	}

	return false;
}

void MainGame::Reset()
{
	tank.SetPoint({ WINSIZE_X / 2, WINSIZE_Y - 100 });
	for (int i = 0; i < tank.GetBulletNum(); ++i)
	{
		tank.GetBullets()[i].SetIsShoot(false);
	}
	tank.GetSkillBulletPtr()->SetIsShoot(false);
	for (int i=0;i< tank.GetGuideBulletCount(); ++i)
	{
		tank.GetGuideBulletPtr()[i].SetIsShoot(false);
		tank.GetGuideBulletPtr()[i].SetTarget(nullptr);
	}
}

void MainGame::CreateEnemy(int count)
{
	if (enemy)
	{
		for (int i = 0; i < enemyNum; ++i) enemy[i].Release();
		delete[] enemy;
	}

	enemyNum = count;
	enemy = new Enemy[enemyNum];
	for (int i = 0; i < enemyNum; ++i)
	{
		enemy[i] = Enemy(10, rand() % 2 + 1);
		enemy[i].Init();
		//enemy[i].SetTank(&tank);
		enemy[i].SetPos({ (float)(WINSIZE_X / 2), ((float)(WINSIZE_Y / 2)) });
	}
}

LRESULT MainGame::MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_CREATE:
		break;
	//case WM_KEYDOWN:
	//	switch (wParam)
	//	{
	//	case 'w':
	//	case 'W':
	//		tank.Move({ 0, -5 });
	//		break;
	//	case 'a':
	//	case 'A':
	//		tank.Move({ -5, 0 });
	//		break;
	//	case 's':
	//	case 'S':
	//		tank.Move({ 0, 5 });
	//		break;
	//	case 'd':
	//	case 'D':
	//		tank.Move({ 5, 0 });
	//		break;
	//	case 'g':
	//	case 'G':
	//		tank.FireGuide();
	//		break;
	//	case 'f':
	//	case 'F':
	//		tank.FireSpecial();
	//		break;
	//	case 'h':
	//	case 'H':
	//		tank.FireSignature();
	//		tank.GetSpecialBullet()->SetTarget(&enemy[0]);
	//		break;
	//	case VK_SPACE:
	//		tank.Fire();
	//		break;
	//	case VK_LEFT:
	//		tank.RotateFire(-5);
	//		break;
	//	case VK_RIGHT:
	//		tank.RotateFire(+5);
	//		break;
	//	}
	//	InvalidateRect(hWnd, NULL, true);
	//	break;
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
