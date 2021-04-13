#pragma once
#include "GameObject.h"

enum class BULLET_TYPE
{
	BASIC,
	SPIRAL,
	HALLUCINATION,
	DELAYED,
	NONE
};

enum class BULLET_STATE
{
	DELAY,
	ACTIVE,
	DEACTIVE,
	NONE
};

class Image;
class Bullet : public GameObject
{
protected:
	BULLET_STATE state;
	BULLET_TYPE type;
	int size;
	int damage;
	float angle;
	float speed;
	POINTFLOAT pos;
	RECT rect;

	Image* image = nullptr;
	float delayTimer;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void Fire(POINTFLOAT pos, float speed, float angle, float delay = 0);

	inline bool IsActive() { return state != BULLET_STATE::DEACTIVE; }
	inline BULLET_TYPE GetType() { return type; }
	inline POINTFLOAT GetPos() { return pos; }
};

