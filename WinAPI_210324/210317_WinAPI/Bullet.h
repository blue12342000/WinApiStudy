#pragma once
#include "GameNode.h"

class Image;
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
	int damage = 1;

	Image* imageBullet = nullptr;

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

	virtual bool IsRectCollision(RECT other);
	virtual bool IsCircleCollision(POINTFLOAT otherPos, int otherRadius);
	virtual bool IsCollision(RECT other);

	inline void SetIsShoot(bool isShoot) { this->isShoot = isShoot; }
	inline bool GetIsShoot() { return isShoot; }
	inline RECT GetRect() { return rc; }
	inline int GetSize() { return size; }
	inline POINTFLOAT GetPos() { return pos; }
	inline int GetDamage() { return damage; }
};

