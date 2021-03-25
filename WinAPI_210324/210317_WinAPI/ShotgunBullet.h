#pragma once
#include "Bullet.h"

class ShotgunBullet : public Bullet
{
private:
	int count = 0;
	Bullet* bullets = nullptr;
	bool isSpread = false;

public:
	ShotgunBullet();
	~ShotgunBullet();

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fire(POINTFLOAT pos, int size, float radian, float speed);
};

