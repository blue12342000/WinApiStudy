#include "stdafx.h"
#include "SpiralBullet.h"
#include "Image.h"

void SpiralBullet::Init()
{
	type = BULLET_TYPE::SPIRAL;
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
}

void SpiralBullet::Update()
{
	if (IsActive())
	{
		if (state == BULLET_STATE::DELAY)
		{
			if (--delayTimer < 0) state = BULLET_STATE::ACTIVE;
		}
		else
		{
			pos.x += (cosf(angle) * speed);
			pos.y += (sinf(angle) * speed);

			if (pos.x < 0 || pos.y < 0 || pos.x > WINSIZE_WIDTH || pos.y > WINSIZE_HEIGHT) state = BULLET_STATE::DEACTIVE;

			angle += ((destAngle - angle) * 0.05f);
		}

		rect = GetRect(pos, size, size, U_GR_CENTER);
	}
}

void SpiralBullet::Fire(POINTFLOAT pos, float speed, float angle, int delay)
{
	Bullet::Fire(pos, speed, angle, delay);
	if (angle < 0) destAngle = angle - 3.141592f;
	else destAngle = angle + 3.141592f;
}