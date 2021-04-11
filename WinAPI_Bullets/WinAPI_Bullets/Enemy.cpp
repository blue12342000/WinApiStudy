#include "stdafx.h"
#include "Enemy.h"
#include "Image.h"

void Enemy::Init()
{
	image = new Image();
	if (FAILED(image->Init("Image/ufo.bmp", 530, 32, 10, 1, 10, true)))
	{
		MessageBox(g_hWnd, "적 이미지 로드 실패", "적 로드 실패", MB_OK);
		delete image;
		image = nullptr;
	}

	pos = {0, 0};
	width = 53;
	height = 32;
	rect = GetRect(pos, width, height, U_GR_CENTER);

	frame = 0;
	elapsedTime = 0;

	bulletManager.Init();
}

void Enemy::Release()
{
	if (image)
	{
		image->Release();
		delete image;
		image = nullptr;
	}

	bulletManager.Release();
}

void Enemy::Update()
{
	if (++elapsedTime % 10 == 0)
	{
		++frame %= image->GetTotalFrames();
	}

	rect = GetRect(pos, width, height, U_GR_CENTER);

	bulletManager.Update();
}

void Enemy::Render(HDC hdc)
{
	if (isDebugMode) Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	image->Render(hdc, POINT{ rect.left, rect.top }, frame);
	if (isDebugMode) Ellipse(hdc, (int)pos.x - 5, (int)pos.y - 5, (int)pos.x + 5, (int)pos.y + 5);

	bulletManager.Render(hdc);
}

void Enemy::Fire(float speed, float angle)
{
	bulletManager.Fire(pos, speed, angle);
}

void Enemy::SpreadFire(float angle)
{
	bulletManager.SpreadFire(pos, angle);
}

void Enemy::RoundFire(float speed)
{
	bulletManager.RoundFire(pos, speed);
}

void Enemy::SpiralFire(float speed, bool rotateRight)
{
	bulletManager.SpiralFire(pos, speed, rotateRight);
}

void Enemy::HalluFire(float speed, float angle)
{
	bulletManager.HalluFire(pos, speed, angle);
}

void Enemy::HalluSprialFire(float speed, float angle)
{
	bulletManager.HalluSprialFire(pos, speed, angle);
}

