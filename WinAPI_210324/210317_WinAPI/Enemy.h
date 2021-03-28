#pragma once
#include "GameNode.h"
#include "Config.h"

class Tank;
class Enemy : public GameNode
{
public:
	enum class EnemyState
	{
		ALIVE,
		DEAD
	};

private:
	EnemyState state = Enemy::EnemyState::ALIVE;
	POINTFLOAT pos;
	float angle;
	float speed;
	int size;
	RECT rc;

	int hp;

	Tank* tankPtr = nullptr;

	HBRUSH hBrushBlack = NULL;
	HBRUSH hBrushRed = NULL;

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

