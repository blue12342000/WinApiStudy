#include "stdafx.h"
#include "HallucinationBullet.h"
#include "Image.h"
#include "BulletManager.h"

void HallucinationBullet::Init()
{
	type = BULLET_TYPE::HALLUCINATION;
	if (!image)
	{
		image = new Image();
		if (FAILED(image->Init("Image/bullet.bmp", 21, 21, 1, 1, 1, true)))
		{
			MessageBox(g_hWnd, "탄환 생성 실패", "탄환 로드 실패", MB_OK);
		}
	}
	size = 21;
	pos = { 0, 0 };
	rect = GetRect(pos, size, size, U_GR_CENTER);
	elpasedTime = 0;
}

void HallucinationBullet::Update()
{
	if (IsActive())
	{
		++elpasedTime;
		pos.x += (cosf(angle) * speed);
		pos.y += (sinf(angle) * speed);

		rect = GetRect(pos, size, size, U_GR_CENTER);

		if (elpasedTime % 5 == 0)
		{
			if (elpasedTime % 10 == 0)
			{
				switch (halluType)
				{
				case BULLET_TYPE::BASIC:
					if (parent) parent->Fire(pos, speed*2, angle - (3.141592f / 4), 100);
					break;
				case BULLET_TYPE::SPIRAL:
					if (parent) parent->SpiralFire(pos, speed, false, 150);
					break;
				}
			}
			else
			{
				switch (halluType)
				{
				case BULLET_TYPE::BASIC:
					if (parent) parent->Fire(pos, speed*2, angle + (3.141592f / 4), 100);
					break;
				case BULLET_TYPE::SPIRAL:
					if (parent) parent->SpiralFire(pos, speed, true, 150);
					break;
				}
			}
		}
		if (pos.x < 0 || pos.y < 0 || pos.x > WINSIZE_WIDTH || pos.y > WINSIZE_HEIGHT) state = BULLET_STATE::DEACTIVE;
	}
}

void HallucinationBullet::Fire(POINTFLOAT pos, float speed, float angle, int delay)
{
	Bullet::Fire(pos, speed, angle, delay);
	elpasedTime = 0;
}

void HallucinationBullet::SetBulletManager(BulletManager* parent)
{
	this->parent = parent;
}
