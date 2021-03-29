#include "Enemy.h"
#include "Tank.h"

HRESULT Enemy::Init()
{
	hp = 10;
	hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
	hBrushRed = CreateSolidBrush(RGB(255, 0, 0));

	return S_OK;
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
	else
	{
		if (move == EnemyMove::EM_LEFT)
		{
			pos.x -= 1;
			if (pos.x < (WINSIZE_X / 2 - 100))
			{
				move = EnemyMove::EM_RIGHT;
			}
		}
		else
		{
			pos.x += 1;
			if (pos.x > (WINSIZE_X / 2 + 100))
			{
				move = EnemyMove::EM_LEFT;
			}
		}
	}

	rc.left = pos.x - size / 2;
	rc.right = pos.x + size / 2;
	rc.top = pos.y - size / 2;
	rc.bottom = pos.y + size / 2;
}

void Enemy::Render(HDC hdc)
{
	if (state == Enemy::EnemyState::ALIVE)
	{
		if (hp > 2) RenderRectangle(hdc, rc, hBrushBlack);
		else if (hp > 1) RenderRectangle(hdc, rc, hBrushRed);
		else RenderRectangle(hdc, rc);
	}
}

void Enemy::Release()
{
	if (hBrushBlack == NULL) DeleteObject(hBrushBlack);
	if (hBrushRed == NULL) DeleteObject(hBrushRed);
}

void Enemy::HitDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		state = Enemy::EnemyState::DEAD;
	}
}
