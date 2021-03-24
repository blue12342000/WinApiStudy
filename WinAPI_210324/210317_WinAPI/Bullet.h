#pragma once
#include "GameNode.h"

class Bullet : public GameNode
{
private:
	bool isShoot = false;
	POINT pos;
	int size;
	float radian;
	float speed;
	RECT rc;

public:
	Bullet();
	Bullet(POINT pos, int size, float radian, float speed);
	~Bullet();

	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fire();

	float Distance(POINT target);
	void SetRect();
};

