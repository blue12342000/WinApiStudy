#include "stdafx.h"
#include "SpaceShip.h"
#include "Image.h"

void SpaceShip::Init()
{
	image = ImageManager::GetInstance()->FindImage("ROCKET");

	pos = { 0, 0 };
	width = 52;
	height = 64;
	rect = GetRect(pos, width, height, U_GR_CENTER);

	moveSpeed = 100;

	frame = 0;
	elapsedTime = 0;

	isAlive = true;
	alpha = 255;
}

void SpaceShip::Release()
{
}

void SpaceShip::Update()
{
	if (isAlive && KeyManager::GetInstance()->IsStayKeyDown(VK_RETURN))
	{
		isAlive = false;
	}

	if (!isAlive)
	{
		OnDead();
	}
	else
	{
		if (KeyManager::GetInstance()->IsStayKeyDown('S'))
		{
			pos.y -= moveSpeed * TimerManager::GetInstance()->GetDeltaTime();
		}
		if (KeyManager::GetInstance()->IsStayKeyDown('Z'))
		{
			pos.x -= moveSpeed * TimerManager::GetInstance()->GetDeltaTime();
		}
		if (KeyManager::GetInstance()->IsStayKeyDown('X'))
		{
			pos.y += moveSpeed * TimerManager::GetInstance()->GetDeltaTime();
		}
		if (KeyManager::GetInstance()->IsStayKeyDown('C'))
		{
			pos.x += moveSpeed * TimerManager::GetInstance()->GetDeltaTime();
		}
	}

	rect = GetRect(pos, width, height, U_GR_CENTER);
}

void SpaceShip::Render(HDC hdc)
{
	//Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	if (image)
	{
		image->AlphaRender(hdc, { (float)rect.left, (float)rect.top }, 0, alpha, U_EF_BLUR);

		image->Render(hdc, POINT{ rect.left + 100, rect.top }, 0);
	}
}

void SpaceShip::OnDead()
{
	if (fadeOut)
	{
		alpha -= (255 * TimerManager::GetInstance()->GetDeltaTime());
		if (alpha < 0)
			fadeOut = false;
	}
	else
	{
		alpha += (255 * TimerManager::GetInstance()->GetDeltaTime());
		if (alpha > 255)
			fadeOut = true;
	}
}
