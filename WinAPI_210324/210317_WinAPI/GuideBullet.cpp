#include "GuideBullet.h"
#include "Enemy.h"

GuideBullet::GuideBullet()
{
}

GuideBullet::~GuideBullet()
{
}

HRESULT GuideBullet::Init()
{
	isShoot = false;
	damage = 2;
	rotateDelay = 50;
	return S_OK;
}

void GuideBullet::Update()
{
	if (!isShoot) return;

	if (target)
	{
		++timer;
		if (target->GetState() == Enemy::EnemyState::DEAD)
		{
			target = nullptr;
		}
		else
		{
			POINTFLOAT delta = { target->GetPos().x - pos.x, target->GetPos().y - pos.y };
			float targetRadian = atan2f(delta.y, delta.x);
			float deltaRadian = targetRadian - radian;
			
			// 가장 최단거리로 변경해줌
			if (deltaRadian < 0 && deltaRadian < -PI) deltaRadian += PI * 2;
			else if (deltaRadian > 0 && deltaRadian > PI) deltaRadian -= PI * 2;

			if (timer >= rotateDelay) timer = rotateDelay - 1;
			rodateRadian = deltaRadian / (rotateDelay - timer);
			radian += rodateRadian;
		}
	}
	else
	{
		if (pos.x < 0 || pos.x > WINSIZE_X || pos.y < 0 || pos.y > WINSIZE_Y)
		{
			isShoot = false;
		}
	}

	pos.x += cosf(radian) * speed;
	pos.y += sinf(radian) * speed;
	SetRect();
}

void GuideBullet::Render(HDC hdc)
{
	Bullet::Render(hdc);
}

void GuideBullet::Release()
{
}

void GuideBullet::Fire(POINTFLOAT pos, int size, float radian, float speed)
{
	isShoot = true;
	this->pos = pos;
	this->origin= pos;
	this->size = size;
	this->radian = radian;
	this->speed = speed;
	rodateRadian = radian;
	timer = 0;
	SetRect();
}

void GuideBullet::SetTarget(Enemy * target)
{
	this->target = target;
	timer = 0;
}
