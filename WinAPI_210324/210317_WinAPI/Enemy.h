#pragma once
#include "GameNode.h"
#include "Config.h"

class Image;
class Tank;
class Enemy : public GameNode
{
public:
	enum class EnemyState
	{
		ALIVE,
		DEAD
	};

	enum class EnemyMove
	{
		EM_LEFT,
		EM_RIGHT
	};

private:
	Image* image;
	EnemyState state = Enemy::EnemyState::ALIVE;
	EnemyMove move = EnemyMove::EM_LEFT;
	POINTFLOAT pos;
	float angle;
	float speed;
	int size;
	RECT rc;

	int hp;

	Tank* tankPtr = nullptr;

	HBRUSH hBrushBlack = NULL;
	HBRUSH hBrushRed = NULL;

	int elapsedTime;
	int frame;

public:
	Enemy() {}
	Enemy(int size, float speed) :size(size), speed(speed) { state = Enemy::EnemyState::ALIVE; }
	~Enemy() {}

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void HitDamage(int damage);

	inline void SetPos(POINTFLOAT pos) { this->pos = pos; }
	inline POINTFLOAT GetPos() { return pos; }
	inline void SetTank(Tank* tankPtr) { this->tankPtr = tankPtr; }
	inline void SetState(Enemy::EnemyState state) { this->state = state; }
	inline Enemy::EnemyState GetState() { return state; }
	inline RECT GetRect() { return rc; }
};

