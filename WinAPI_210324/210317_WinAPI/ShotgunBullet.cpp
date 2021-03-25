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
			bullets[i].Fire(pos, 10, radian + PI * i * 30 / 180, 5);
		}
	}

	if (isSpread)
	{
		for (int i = 0; i < count; ++i)
		{
			bullets[i].Update();
			isShoot = bullets[i].IsShoot();
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