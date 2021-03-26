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
	int bulletNum;
	Bullet* bullets;
	Bullet* skillBulletPtr = nullptr;

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

	inline POINTFLOAT GetPoint() { return pos; }
	inline int GetBulletNum() { return bulletNum; }
	inline Bullet* GetBullets() { return bullets; }
	inline Bullet* GetSkillBulletPtr() { return skillBulletPtr; }
};

