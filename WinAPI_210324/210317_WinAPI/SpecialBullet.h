#pragma once
#include "Bullet.h"

class Enemy;
class GuideBullet;
class SpecialBullet : public Bullet
{
private:
	enum class SpecialBulletState
	{
		SBS_INIT,
		SBS_MAGIC_CIRCLE,
		SBS_STAR_LIGHT,
		SBS_SPREAD,
		SBS_TORNADO,
		SBS_GAIDE_ARROW,
		SBS_END
	};

	struct PointCircle
	{
		bool isActive = false;
		bool isFixed = false;
		POINTFLOAT pos;
		float radius;
		RECT rc;

		RECT GetRc()
		{
			rc = {(int)(pos.x - radius / 2), (int)(pos.y - radius / 2), (int)(pos.x + radius / 2), (int)(pos.y + radius / 2) };
			return rc;
		}
	};

	struct PointLine
	{
		POINTFLOAT origin;
		POINTFLOAT from;
		float angle;
		float length;
	};

private:
	// 탄의 진행 상태
	SpecialBulletState state = SpecialBulletState::SBS_END;

	// 유도탄에서 필요한 기능
	int timer;
	int rotateDelay;
	float rodateRadian;

	// 서클의 각 시작각
	int circleAngle[4];
	PointCircle pointCircle[100];

	// 별 윤곽 그리기
	float circleRadian[4];
	PointLine pointLine[5];

	// 원회전
	float circleStartAngle;

	// 유도탄
	GuideBullet* guideBullet;

	HBRUSH hBrush[9];
	HPEN hPen[9];

	Enemy* target = nullptr;

private:
	void MoveCenter();
	void MoveCircleLine();
	void MoveStarLine();
	void MoveCircleTonado();
	void FireGuidArrow();
	void FireFinish();

public:
	SpecialBullet();
	~SpecialBullet();

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fire(POINTFLOAT pos, int size, float radian, float speed);

	void SetTarget(Enemy* target);
};

