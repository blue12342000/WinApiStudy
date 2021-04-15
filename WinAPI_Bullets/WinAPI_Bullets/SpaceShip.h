#pragma once
#include "GameObject.h"

class Image;
class SpaceShip : public GameObject
{
private:
	Image* image;
	int width;
	int height;
	POINTFLOAT pos;
	RECT rect;
	
	float moveSpeed;

	int frame;
	int elapsedTime;

	float fireAngle;
	bool isAlive;
	bool fadeOut = true;
	float alpha;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void OnDead();

	inline void SetPos(POINTFLOAT pos) { this->pos = pos; }
};

