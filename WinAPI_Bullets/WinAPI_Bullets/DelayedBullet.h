#pragma once
#include "Bullet.h"

class DelayedBullet : public Bullet
{
private:
	float elapsedTime;
	bool isReTarget;

public:
	void Init();
	void Update();

	void Fire(POINTFLOAT pos, float speed, float angle, float delay = 0);
};

