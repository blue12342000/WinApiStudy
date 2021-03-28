#pragma once
#include "GameNode.h"
#include "Bullet.h"
#include "ShotgunBullet.h"
#include "GuideBullet.h"

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

	Bullet* bullets = nullptr;
	ShotgunBullet* skillBulletPtr = nullptr;

	int guideBulletCount;
	GuideBullet* guideBulletPtr = nullptr;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Move(POINTFLOAT delta);
	void Fire();
	void FireSpecial();
	void FireGuide();
	void Dead();

	void RotateFire(float angle);
	void SetRect();

	inline void SetPoint(POINTFLOAT pos) { this->pos = pos; SetRect(); }
	inline POINTFLOAT GetPoint() { return pos; }
	inline int GetBulletNum() { return bulletNum; }
	inline Bullet* GetBullets() { return bullets; }
	inline ShotgunBullet* GetSkillBulletPtr() { return skillBulletPtr; }
	inline int GetGuideBulletCount() { return guideBulletCount; }
	inline GuideBullet* GetGuideBulletPtr() { return guideBulletPtr; }
};

