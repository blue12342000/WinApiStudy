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

bool Bullet::IsRectCollision(RECT other)
{
	if (rc.right < other.left || rc.left > other.right
		|| rc.bottom < other.top || rc.top > other.bottom
		|| other.right < rc.left || other.left > rc.right
		|| other.bottom < rc.top || other.top > rc.bottom)
	{
		return false;
	}

	return true;
}

bool Bullet::IsCircleCollision(POINTFLOAT otherPos, int otherRadius)
{
	return false;
}

bool Bullet::IsCollision(RECT other)
{
	// 사각형 안에 확인
	if (other.left < pos.x && pos.x < other.right
		&& other.top < pos.y && pos.y < other.bottom)
	{
		return true;
	}

	// 각 꼭지점이 원안에 있는지 확인
	POINTFLOAT points[4] = { {other.left, other.top}, {other.left, other.bottom}, {other.right, other.top}, {other.right, other.bottom} };
	float dist;
	float maxDist = size * size;
	for (auto p : points)
	{
		dist = pow(p.x - pos.x, 2) + pow(p.y - pos.y, 2);
		if (dist < maxDist)
		{
			return true;
		}
	}

	// 각 면에서의 수직으로 나간 거리가 반지름보다 작은가 확인
	if (other.left < pos.x && pos.x < other.right)
	{
		float minDistY = min(abs(pos.y - other.top), abs(pos.y - other.bottom));
		if (minDistY < size)
		{
			return true;
		}
	}
	else if (other.top < pos.y && pos.y < other.bottom)
	{
		float minDistX = min(abs(pos.x - other.left), abs(pos.x - other.right));
		if (minDistX < size)
		{
			return true;
		}
	}

	return false;
}
