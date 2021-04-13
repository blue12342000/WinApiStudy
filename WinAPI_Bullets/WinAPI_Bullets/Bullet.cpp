#include "stdafx.h"
#include "Bullet.h"
#include "Image.h"

void Bullet::Init()
{
	type = BULLET_TYPE::BASIC;
	state = BULLET_STATE::DEACTIVE;
	image = ImageManager::GetInstance()->FindImage("BULLET");
	size = 21;
	pos = { 0, 0 };
	rect = GetRect(pos, size, size, U_GR_CENTER);
	delayTimer = 0;
}

void Bullet::Release()
{
}

void Bullet::Update()
{
	if (IsActive())
	{
		if (state == BULLET_STATE::DELAY)
		{
			delayTimer -= TimerManager::GetInstance()->GetDeltaTime();
			if (delayTimer < 0) state = BULLET_STATE::ACTIVE;
		}
		else
		{
			pos.x += (cosf(angle) * speed * TimerManager::GetInstance()->GetDeltaTime());
			pos.y += (sinf(angle) * speed * TimerManager::GetInstance()->GetDeltaTime());

			if (pos.x < 0 || pos.y < 0 || pos.x > WINSIZE_WIDTH || pos.y > WINSIZE_HEIGHT) state = BULLET_STATE::DEACTIVE;
		}

		rect = GetRect(pos, size, size, U_GR_CENTER);
	}
}

void Bullet::Render(HDC hdc)
{
	if (IsActive()) image->Render(hdc, rect.left, rect.top);
}

void Bullet::Fire(POINTFLOAT pos, float speed, float angle, float delay)
{
	if (delay > 0) state = BULLET_STATE::DELAY;
	else state = BULLET_STATE::ACTIVE;
	this->pos = pos;
	this->speed = speed;
	this->angle = angle;
	rect = GetRect(pos, size, size, U_GR_CENTER);
	delayTimer = delay;
}