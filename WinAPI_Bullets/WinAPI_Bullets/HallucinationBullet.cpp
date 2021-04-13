#include "stdafx.h"
#include "HallucinationBullet.h"
#include "Image.h"
#include "BulletManager.h"

void HallucinationBullet::Init()
{
	type = BULLET_TYPE::HALLUCINATION;
	image = ImageManager::GetInstance()->FindImage("BULLET");
	size = 21;
	pos = { 0, 0 };
	rect = GetRect(pos, size, size, U_GR_CENTER);

	left = true;
	elpasedTime = 0;
}

void HallucinationBullet::Update()
{
	if (IsActive())
	{
		elpasedTime += TimerManager::GetInstance()->GetDeltaTime();
		pos.x += (cosf(angle) * speed * TimerManager::GetInstance()->GetDeltaTime());
		pos.y += (sinf(angle) * speed * TimerManager::GetInstance()->GetDeltaTime());

		rect = GetRect(pos, size, size, U_GR_CENTER);

		if (step < ((int)(elpasedTime * 15)))
		{
			if (left)
			{
				switch (halluType)
				{
				case BULLET_TYPE::BASIC:
					if (parent) parent->Fire(pos, speed, angle - (PI / 3), 3);
					break;
				case BULLET_TYPE::SPIRAL:
					if (parent) parent->SpiralFire(pos, speed, false, 3);
					break;
				}
			}
			else
			{
				switch (halluType)
				{
				case BULLET_TYPE::BASIC:
					if (parent) parent->Fire(pos, speed, angle + (PI / 3), 3);
					break;
				case BULLET_TYPE::SPIRAL:
					if (parent) parent->SpiralFire(pos, speed, true, 3);
					break;
				}
			}
			left = !left;
			step = ((int)(elpasedTime * 15));
		}

		if (pos.x < 0 || pos.y < 0 || pos.x > WINSIZE_WIDTH || pos.y > WINSIZE_HEIGHT) state = BULLET_STATE::DEACTIVE;
	}
}

void HallucinationBullet::Fire(POINTFLOAT pos, float speed, float angle, float delay)
{
	Bullet::Fire(pos, speed, angle, delay);
	elpasedTime = 0;
	left = true;
	step = 0;
}

void HallucinationBullet::SetBulletManager(BulletManager* parent)
{
	this->parent = parent;
}
