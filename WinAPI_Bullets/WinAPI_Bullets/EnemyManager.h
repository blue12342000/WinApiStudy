#pragma once
#include "GameObject.h"

class Enemy;
class EnemyManager: public GameObject
{
private:
	vector<Enemy*> vEnemys;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void CreateEnemy(int x, int y);
	void Fire(float speed, float angle);
	void SpreadFire(float angle);
	void RoundFire(float speed);
	void SpiralFire(float speed, bool rotateRight);
	void HalluFire(float speed, float angle);
	void HalluSprialFire(float speed, float angle);
	void DelayedFire(float speed);
};

