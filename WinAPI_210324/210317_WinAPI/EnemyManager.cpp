#include "EnemyManager.h"
#include "Enemy.h"

HRESULT EnemyManager::Init()
{
	// 1) ¹è¿­
	Enemy* enemys = new Enemy[10];
	for (int i = 0; i < 10; ++i)
	{
		enemys[i].Init();
	}

	// 2) vector push_back();
	for (int i = 0; i < 10; ++i)
	{
		vEnemy.push_back(new Enemy());
		vEnemy[i]->Init();
	}

	// 3) vector resize();
	vEnemy.resize(20, new Enemy());
	for (int i = 0; i < 20; ++i)
	{
		vEnemy[i]->Init();
	}

	// 4) vector reserve();
	vEnemy.reserve(20);
	for (int i = 0; i < 20; ++i)
	{
		vEnemy.push_back(new Enemy());
		vEnemy[i]->Init();
	}

	return S_OK;
}

void EnemyManager::Update()
{
}

void EnemyManager::Render(HDC hdc)
{
}

void EnemyManager::Release()
{
}
