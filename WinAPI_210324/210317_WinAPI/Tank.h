#pragma once
#include "GameNode.h"
#include "Bullet.h"
#include "ShotgunBullet.h"

class Tank : public GameNode
{
private:
	POINTFLOAT pos;
	int size;
	float attackValue;
	string name;
	RECT shape;

	int angle;

	int curr;
	Bullet bullet[10];
	Bullet* bulletPtr = nullptr;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Move(POINTFLOAT delta);
	void Fire();
	void FireSpecial();
	void Dead();

	void RotateFire(float angle);
	void SetRect();
};

