#pragma once
#include "GameNode.h"
#include "Bullet.h"

class Tank : public GameNode
{
private:
	POINT pos;
	int size;
	float attackValue;
	string name;
	RECT shape;

	float angle;

	int curr;
	Bullet bullet[10];

public:
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Move(POINT delta);
	void Fire();
	void Dead();

	void RotateFire(float angle);
	void SetRect();
};

