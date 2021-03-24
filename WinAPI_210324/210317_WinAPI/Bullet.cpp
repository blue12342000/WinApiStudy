#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::Bullet(POINT pos, int size, float radian, float speed):pos(pos), size(size), radian(radian), speed(speed)
{
	isShoot = false;
	SetRect();
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	isShoot = false;
}

void Bullet::Update()
{
	if (!isShoot) return;
	pos.x += cos(radian) * speed;
	pos.y += sin(radian) * speed;
	SetRect();
}

void Bullet::Render(HDC hdc)
{
	if (isShoot)
		Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

void Bullet::Release()
{
}

void Bullet::Fire()
{
	isShoot = true;
}

float Bullet::Distance(POINT target)
{
	return sqrt(pow(target.x - pos.x, 2) + pow(target.y - pos.y, 2));
}

void Bullet::SetRect()
{
	rc.left = pos.x - size / 2;
	rc.right = pos.x + size / 2;
	rc.top = pos.y - size / 2;
	rc.bottom = pos.y + size / 2;
}
