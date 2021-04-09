#pragma once
#include "GameNode.h"
#include <vector>

class Enemy;
class EnemyManager : public GameNode
{
private:
	vector<Enemy*> vEnemy;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};