#include "stdafx.h"
#include "Enemy.h"
#include "Image.h"

void Enemy::Init()
{
	image = ImageManager::GetInstance()->FindImage("ENEMY");

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

void Enemy::DelayedFire(float speed)
{
	bulletManager.DelayedFire(pos, speed, 0);
}

