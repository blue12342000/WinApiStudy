#include "stdafx.h"
#include "SpiralBullet.h"
#include "Image.h"

void SpiralBullet::Init()
{
	type = BULLET_TYPE::SPIRAL;
	image = ImageManager::GetInstance()->FindImage("BULLET");
	size = 21;
	pos = { 0, 0 };
	rect = GetRect(pos, size, size, U_GR_CENTER);
}

void SpiralBullet::Update()
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

			angle += ((destAngle - angle) * 0.05f);
		}

		rect = GetRect(pos, size, size, U_GR_CENTER);
	}
}

void SpiralBullet::Fire(POINTFLOAT pos, float speed, float angle, float delay)
{
	Bullet::Fire(pos, speed, angle, delay);
	if (angle < 0) destAngle = angle - PI;
	else destAngle = angle + PI;
}