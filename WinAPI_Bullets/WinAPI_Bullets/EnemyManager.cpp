#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy.h"

void EnemyManager::Init()
{
	vEnemys.reserve(100);
}

void EnemyManager::Release()
{
	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->Release();
		delete vEnemys[i];
	}
	vEnemys.clear();
}

void EnemyManager::Update()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown('Q'))
	{
		SpreadFire(3.141582f * ((float)(rand() % 360) / 180));
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		RoundFire(4);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
	{
		SpiralFire(4, false);
		SpiralFire(3, true);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('R'))
	{
		HalluFire(4, 3.141582f * ((float)(rand() % 360) / 180));
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('T'))
	{
		HalluSprialFire(4, 3.141582f * ((float)(rand() % 360) / 180));
	}

	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->Update();
	}
}

void EnemyManager::Render(HDC hdc)
{
	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->Render(hdc);
	}
}

void EnemyManager::CreateEnemy(int x, int y)
{
	vEnemys.push_back(new Enemy());
	vEnemys[vEnemys.size() - 1]->Init();
	vEnemys[vEnemys.size() - 1]->SetPos(x, y);
}

void EnemyManager::Fire(float speed, float angle)
{
	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->Fire(speed, angle);
	}
}

void EnemyManager::SpreadFire(float angle)
{
	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->SpreadFire(angle);
	}
}

void EnemyManager::RoundFire(float speed)
{
	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->RoundFire(speed);
	}
}

void EnemyManager::SpiralFire(float speed, bool rotateRight)
{
	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->SpiralFire(speed, rotateRight);
	}
}

void EnemyManager::HalluFire(float speed, float angle)
{
	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->HalluFire(speed, angle);
	}
}

void EnemyManager::HalluSprialFire(float speed, float angle)
{
	for (int i = 0; i < vEnemys.size(); ++i)
	{
		vEnemys[i]->HalluSprialFire(speed, angle);
	}
}
