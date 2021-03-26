#include "ShotgunBullet.h"

ShotgunBullet::ShotgunBullet()
{
}

ShotgunBullet::~ShotgunBullet()
{
}

HRESULT ShotgunBullet::Init()
{
	Bullet::Init();
	count = 36;
	bullets = new Bullet[count];

	return S_OK;
}

void ShotgunBullet::Update()
{
	if (!isShoot) return;

	pos.x += cos(radian) * speed;
	pos.y += sin(radian) * speed;
	SetRect();

	if (Distance(pos) > 300 && !isSpread)
	{
		isSpread = true;
		for (int i = 0; i < count; ++i)
		{
			bullets[i].Fire(pos, 10, radian + PI * 2.0f / count * i, 5);
		}
	}

	if (isSpread)
	{
		isShoot = false;
		for (int i = 0; i < count; ++i)
		{
			bullets[i].Update();
			isShoot |= bullets[i].GetIsShoot();
		}
	}
}

void ShotgunBullet::Render(HDC hdc)
{
	if (isShoot && !isSpread) Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
	else if (isShoot && isSpread)
	{
		for (int i = 0; i < count; ++i)
		{
			bullets[i].Render(hdc);
		}
	}
}

void ShotgunBullet::Release()
{
	delete[] bullets;
}

void ShotgunBullet::Fire(POINTFLOAT pos, int size, float radian, float speed)
{
	Bullet::Fire(pos, size, radian, speed);
	isSpread = false;
}

bool ShotgunBullet::IsRectCollision(RECT other)
{
	if (!isSpread)
	{
		if (Bullet::IsRectCollision(other))
		{
			isShoot = false;
			return true;
		}
	}
	else
	{
		for (int i = 0; i < count; ++i)
		{
			if (bullets[i].GetIsShoot() && bullets[i].IsRectCollision(other))
			{
				bullets[i].SetIsShoot(false);
				return true;
			}
		}
	}

	return false;
}

bool ShotgunBullet::IsCircleCollision(POINTFLOAT otherPos, int otherRadius)
{
	return false;
}

bool ShotgunBullet::IsCollision(POINTFLOAT otherPos, int otherRadius)
{
	return false;
}
