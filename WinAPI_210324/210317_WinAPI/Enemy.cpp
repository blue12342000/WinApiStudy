#include "Enemy.h"
#include "Tank.h"

HRESULT Enemy::Init()
{
	return E_NOTIMPL;
}

void Enemy::Update()
{
	if (state == Enemy::EnemyState::DEAD) return;

	if (tankPtr)
	{
		POINTFLOAT delta = { tankPtr->GetPoint().x - pos.x , tankPtr->GetPoint().y - pos.y };
		angle = atan2f(delta.y, delta.x);

		pos.x += cosf(angle) * speed;
		pos.y += sinf(angle) * speed;
	}

	rc.left = pos.x - size / 2;
	rc.right = pos.x + size / 2;
	rc.top = pos.y - size / 2;
	rc.bottom = pos.y + size / 2;
}

void Enemy::Render(HDC hdc)
{
	if(state == Enemy::EnemyState::ALIVE) Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

void Enemy::Release()
{
}
