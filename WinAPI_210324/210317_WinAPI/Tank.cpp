#include "Tank.h"
#include "SpecialBullet.h"

HRESULT Tank::Init()
{
	pos.x = 100;
	pos.y = 200;

	angle = -90;

	size = 100;
	curr = 0;

	SetRect();
	bulletNum = 10;
	bullets = new Bullet[bulletNum];
	for (int i = 0; i < bulletNum; ++i)
	{
		bullets[i].Init();
	}

	skillBulletPtr = new ShotgunBullet();
	skillBulletPtr->Init();

	guideBulletCount = 10;
	guideBulletPtr = new GuideBullet[guideBulletCount];
	for (int i = 0; i < guideBulletCount;++i) guideBulletPtr[i].Init();

	signatureBullet = new SpecialBullet();
	signatureBullet->Init();

	return S_OK;
}

void Tank::Update()
{
	for (int i = 0; i < 10; ++i)
	{
		if (bullets[i].Distance(pos) > 800)
		{
			bullets[i].Init();
		}

		bullets[i].Update();
	}
	skillBulletPtr->Update();
	for (int i = 0; i < guideBulletCount; ++i) guideBulletPtr[i].Update();
	signatureBullet->Update();
}

void Tank::Render(HDC hdc)
{
	float radian = PI * angle / 180;
	Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);
	MoveToEx(hdc, pos.x, pos.y, NULL);
	LineTo(hdc, pos.x + cos(radian) * 30, pos.y + sin(radian) * 30);

	for (int i = 0; i < 10; ++i)
	{
		bullets[i].Render(hdc);
	}

	skillBulletPtr->Render(hdc);
	for (int i = 0; i < guideBulletCount; ++i) guideBulletPtr[i].Render(hdc);
	signatureBullet->Render(hdc);
}

void Tank::Release()
{
	for (int i = 0; i < bulletNum; ++i)
	{
		bullets[i].Release();
	}
	delete[] bullets;
	skillBulletPtr->Release();
	delete skillBulletPtr;

	for (int i = 0; i < guideBulletCount; ++i) guideBulletPtr[i].Release();
	delete[] guideBulletPtr;

	signatureBullet->Release();
	delete signatureBullet;
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
	bullets[curr].Fire({ (pos.x + cos(radian) * 30), (pos.y + sin(radian) * 30) }, 10, radian, 5);
}

void Tank::FireSpecial()
{
	if (!skillBulletPtr->GetIsShoot())
	{
		float radian = PI * angle / 180;
		skillBulletPtr->Fire({ (pos.x + cos(radian) * 30), (pos.y + sin(radian) * 30) }, 10, radian, 5);
	}
}

void Tank::FireGuide()
{
	for (int i = 0; i < guideBulletCount; ++i)
	{
		if (!guideBulletPtr[i].GetIsShoot())
		{
			float radian = PI * angle / 180;
			guideBulletPtr[i].Fire({ (pos.x + cos(radian) * 30), (pos.y + sin(radian) * 30) }, 10, radian, 5);
			break;
		}
	}
}

void Tank::FireSignature()
{
	if (!signatureBullet->GetIsShoot())
	{
		float radian = PI * angle / 180;
		signatureBullet->Fire({ (pos.x + cos(radian) * 30), (pos.y + sin(radian) * 30) }, 10, radian, 1);
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
