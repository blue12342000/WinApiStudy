#include "Tank.h"

HRESULT Tank::Init()
{
	pos.x = 100;
	pos.y = 200;

	angle = -45;

	size = 100;
	curr = 0;

	SetRect();
	bulletPtr = new ShotgunBullet();
	bulletPtr->Init();

	return S_OK;
}

void Tank::Update()
{
	for (int i = 0; i < 10; ++i)
	{
		if (bullet[i].Distance(pos) > 800)
		{
			bullet[i].Init();
		}

		bullet[i].Update();
	}
	bulletPtr->Update();
}

void Tank::Render(HDC hdc)
{
	float radian = PI * angle / 180;
	Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);
	MoveToEx(hdc, pos.x, pos.y, NULL);
	LineTo(hdc, pos.x + cos(radian) * 30, pos.y + sin(radian) * 30);

	for (int i = 0; i < 10; ++i)
	{
		bullet[i].Render(hdc);
	}

	bulletPtr->Render(hdc);
}

void Tank::Release()
{
	bulletPtr->Release();
	delete bulletPtr;
}

void Tank::Move(POINTFLOAT delta)
{
	pos.x += delta.x;
	pos.y += delta.y;
	SetRect();
}

void Tank::Fire()
{
	curr = (curr + 1) % 10;

	float radian = PI * angle / 180;
	bullet[curr].Fire({ (pos.x + cos(radian) * 30), (pos.y + sin(radian) * 30) }, 10, radian, 5);
}

void Tank::FireSpecial()
{
	if (!bulletPtr->IsShoot())
	{
		float radian = PI * angle / 180;
		bulletPtr->Fire({ (pos.x + cos(radian) * 30), (pos.y + sin(radian) * 30) }, 10, radian, 5);
	}
}

void Tank::Dead()
{

}

void Tank::RotateFire(float angle)
{
	this->angle += angle;
	this->angle = (this->angle + 360) % 360;
}

void Tank::SetRect()
{
	shape.left = pos.x - size / 2;
	shape.right = pos.x + size / 2;
	shape.top = pos.y - size / 2;
	shape.bottom = pos.y + size / 2;
}
