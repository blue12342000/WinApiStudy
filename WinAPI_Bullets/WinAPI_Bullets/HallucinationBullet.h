#pragma once
#include "Bullet.h"

class BulletManager;
class HallucinationBullet : public Bullet
{
private:
	BULLET_TYPE halluType;

	bool left;
	int step;
	float elpasedTime;

	BulletManager* parent;

public:
	virtual void Init();
	virtual void Update();

	void Fire(POINTFLOAT pos, float speed, float angle, float delay = 0);

	inline void SetHalluType(BULLET_TYPE halluType) { this->halluType = halluType; }
	void SetBulletManager(BulletManager* parent);
};

