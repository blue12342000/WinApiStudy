#pragma once
#include "GameObject.h"

class Bullet;
enum class BULLET_TYPE;
class BulletManager : public GameObject
{
private:
	vector<Bullet*> vBullets;
	map<BULLET_TYPE, vector<Bullet*>> mvDeacive;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void Fire(POINTFLOAT pos, float speed, float angle, int delay = 0);
	void SpreadFire(POINTFLOAT pos, float angle, int delay = 0);
	void RoundFire(POINTFLOAT pos, float speed, int delay = 0);

	void SpiralFire(POINTFLOAT pos, float speed, bool rightRotate = false, int delay = 0);
	void HalluFire(POINTFLOAT pos, float speed, float angle, int delay = 0);
	void HalluSprialFire(POINTFLOAT pos, float speed, float angle, int delay = 0);

	void DelayedFire(POINTFLOAT pos, float speed, float angle, int delay = 0);
	void SpiralDelayedFire(POINTFLOAT pos, float speed, float angle, int delay = 0);
};

