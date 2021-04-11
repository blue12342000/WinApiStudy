#include "stdafx.h"
#include "Bullet.h"
#include "Image.h"

void Bullet::Init()
{
	type = BULLET_TYPE::BASIC;
	state = BULLET_STATE::DEACTIVE;
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
	delayTimer = 0;
}

void Bullet::Release()
{
	if (image)
	{
		image->Release();
		delete image;
		image = nullptr;
	}
}

void Bullet::Update()
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
		}

		rect = GetRect(pos, size, size, U_GR_CENTER);
	}
}

void Bullet::Render(HDC hdc)
{
	if (IsActive()) image->Render(hdc, rect.left, rect.top);
}

void Bullet::Fire(POINTFLOAT pos, float speed, float angle, int delay)
{
	if (delay > 0) state = BULLET_STATE::DELAY;
	else state = BULLET_STATE::ACTIVE;
	this->pos = pos;
	this->speed = speed;
	this->angle = angle;
	rect = GetRect(pos, size, size, U_GR_CENTER);
	delayTimer = delay;
}