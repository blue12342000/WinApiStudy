#pragma once
#include "GameNode.h"

class Bullet : public GameNode
{
protected:
	bool isShoot = false;
	POINTFLOAT pos;
	POINTFLOAT origin;
	int size;
	float radian;
	float speed;
	RECT rc;

public:
	Bullet();
	~Bullet();

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	virtual void Fire(POINTFLOAT pos, int size, float radian, float speed);

	float Distance(POINTFLOAT target);
	void SetRect();

	inline bool IsShoot() { return isShoot; }
};

