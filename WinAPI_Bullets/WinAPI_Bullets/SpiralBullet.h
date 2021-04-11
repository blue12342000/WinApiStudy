#pragma once
#include "Bullet.h"

class SpiralBullet : public Bullet
{
private:
	float destAngle;

public:
	void Init();
	void Update();

	void Fire(POINTFLOAT pos, float speed, float angle, int delay);
};

