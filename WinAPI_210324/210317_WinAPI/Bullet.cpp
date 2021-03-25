#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

HRESULT Bullet::Init()
{
	isShoot = false;

	return S_OK;
}

void Bullet::Update()
{
	if (!isShoot) return;

	if (Distance(pos) > 400)
	{
		isShoot = false;
		return;
	}

	pos.x += cos(radian) * speed;
	pos.y += sin(radian) * speed;
	SetRect();
}

void Bullet::Render(HDC hdc)
{
	if (isShoot) Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

void Bullet::Release()
{
}

void Bullet::Fire(POINTFLOAT pos, int size, float radian, float speed)
{
	isShoot = true;
	this->pos = pos;
	this->origin = pos;
	this->size = size;
	this->radian = radian;
	this->speed = speed;
	SetRect();
}

float Bullet::Distance(POINTFLOAT target)
{
	return sqrt(pow(target.x - origin.x, 2) + pow(target.y - origin.y, 2));
}

void Bullet::SetRect()
{
	rc.left = pos.x - size / 2;
	rc.right = pos.x + size / 2;
	rc.top = pos.y - size / 2;
	rc.bottom = pos.y + size / 2;
}
