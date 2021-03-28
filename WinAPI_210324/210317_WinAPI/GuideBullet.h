#pragma once
#include "Bullet.h"

class Enemy;
class GuideBullet : public Bullet
{
private:
	int timer;
	int rotateDelay;
	float rodateRadian;
	Enemy* target = nullptr;

public:
	GuideBullet();
	~GuideBullet();

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fire(POINTFLOAT pos, int size, float radian, float speed);

	void SetTarget(Enemy* target);
	inline bool IsHasTarget() { return target != nullptr; }
};

