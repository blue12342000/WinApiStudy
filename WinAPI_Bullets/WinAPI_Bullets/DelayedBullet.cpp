#include "stdafx.h"
#include "DelayedBullet.h"

void DelayedBullet::Init()
{
	type = BULLET_TYPE::DELAYED;
	image = ImageManager::GetInstance()->FindImage("BULLET");
	size = 21;
	pos = { 0, 0 };
	rect = GetRect(pos, size, size, U_GR_CENTER);

	elapsedTime = 0;
	isReTarget = false;
}

void DelayedBullet::Update()
{
	if (IsActive())
	{
		if (state == BULLET_STATE::DELAY)
		{
			delayTimer -= TimerManager::GetInstance()->GetDeltaTime();
			if (delayTimer < 0)
			{
				state = BULLET_STATE::ACTIVE;
				if (isReTarget) angle = atan2f(g_mouse.y - pos.y, g_mouse.x - pos.x);
			}
		}
		else
		{
			if (!isReTarget)
			{
				elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
				if (elapsedTime > 1.0f)
				{
					// 1초가 지난후 타갯으로 앵글 변환
					isReTarget = true;
					speed *= 2;

					state = BULLET_STATE::DELAY;
					delayTimer = 0.75f;
				}
			}

			pos.x += (cosf(angle) * speed * TimerManager::GetInstance()->GetDeltaTime());
			pos.y += (sinf(angle) * speed * TimerManager::GetInstance()->GetDeltaTime());

			if (pos.x < 0 || pos.y < 0 || pos.x > WINSIZE_WIDTH || pos.y > WINSIZE_HEIGHT) state = BULLET_STATE::DEACTIVE;
		}

		rect = GetRect(pos, size, size, U_GR_CENTER);
	}
}

void DelayedBullet::Fire(POINTFLOAT pos, float speed, float angle, float delay)
{
	Bullet::Fire(pos, speed, angle, delay);
	elapsedTime = 0;
	isReTarget = false;
}
