#pragma once
#include "GameObject.h"
#include "BulletManager.h"

class Image;
class Enemy : public GameObject
{
private:
	Image* image;
	int width;
	int height;
	POINTFLOAT pos;
	RECT rect;

	int frame;
	int elapsedTime;

	float fireAngle;
	BulletManager bulletManager;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	inline void SetPos(int x, int y) { pos.x = x; pos.y = y; }

	void Fire(float speed, float angle);
	void SpreadFire(float angle);
	void RoundFire(float speed);

	void SpiralFire(float speed, bool rotateRight);
	void HalluFire(float speed, float angle);
	void HalluSprialFire(float speed, float angle);

	void DelayedFire(float speed);
};

