#include "SpecialBullet.h"
#include "GuideBullet.h"
#include "Enemy.h"

void SpecialBullet::MoveCenter()
{
	POINTFLOAT delta = { WINSIZE_X / 2 - pos.x, WINSIZE_Y / 2 - pos.y };
	float targetRadian = atan2f(delta.y, delta.x);
	float deltaRadian = targetRadian - radian;

	// 가장 최단거리로 변경해줌
	if (deltaRadian < -PI)
	{
		deltaRadian += PI * 2;
	}

	if (timer < rotateDelay) rodateRadian = deltaRadian / (rotateDelay - timer);
	else rodateRadian = deltaRadian;
	radian += rodateRadian;

	pos.x += cos(radian) * speed;
	pos.y += sin(radian) * speed;
	++timer;

	if (timer > 100000) timer = rotateDelay;
}

void SpecialBullet::MoveCircleLine()
{
	if (timer < 200)
	{
		for (int i = 0; i < 100; ++i)
		{
			pointCircle[i].pos.x += cos(circleAngle[i / 25]) * 1.5f;
			pointCircle[i].pos.y += sin(circleAngle[i / 25]) * 1.5f;
			pointCircle[i].radius = 10 + timer / 20;

			if (timer == 199 && i % 25 == 0)
			{
				pointCircle[i].isFixed = true;
			}
		}
	}
	else if (timer < 450)
	{
		POINTFLOAT target, delta, pre;
		float targetAngle;
		float targetDistance;
		
		int deltaTimer = timer - 200;
		if (deltaTimer % 10 == 0 && deltaTimer / 10 < 25)
		{
			for (int i = 0; i < 4; ++i)
			{
				circleAngle[i] += (PI / 2) / 25;
				pointCircle[25 * i + (deltaTimer / 10 + 1)].isActive = true;
			}
		}

		for (int i = 0; i < 100; ++i)
		{
			if (pointCircle[i].isFixed)
			{
				pre = pointCircle[i].pos;
				continue;
			}

			target = { pos.x + cosf(circleAngle[i / 25]) * 300, pos.y + sinf(circleAngle[i / 25]) * 300 };
			delta = { target.x - pre.x, target.y - pre.y };
			targetAngle = atan2f(delta.y, delta.x);
			targetDistance = sqrt(delta.x * delta.x + delta.y * delta.y);

			pointCircle[i].pos.x += cos(targetAngle) * targetDistance / 10;
			pointCircle[i].pos.y += sin(targetAngle) * targetDistance / 10;

		}
		if (deltaTimer % 10 == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				pointCircle[25 * i + (deltaTimer / 10)].isFixed = true;
			}
		}
	}
	++timer;
}

void SpecialBullet::MoveStarLine()
{
	if (timer < 50)
	{
		float lineDestance;
		for (int i = 0; i < 5; ++i)
		{
			lineDestance = sqrt(pow(pointLine[i].origin.x - pointLine[(i + 2) % 5].origin.x, 2) + pow(pointLine[i].origin.y - pointLine[(i + 2) % 5].origin.y, 2));
			pointLine[i].length += lineDestance / 50;
		}
	}
	else if (timer < 100)
	{
		float lineDestance;
		for (int i = 0; i < 5; ++i)
		{
			lineDestance = sqrt(pow(pointLine[i].origin.x - pointLine[(i + 2) % 5].origin.x, 2) + pow(pointLine[i].origin.y - pointLine[(i + 2) % 5].origin.y, 2));
			pointLine[i].from.x += cos(pointLine[i].angle) * lineDestance / 50;
			pointLine[i].from.y += sin(pointLine[i].angle) * lineDestance / 50;
			pointLine[i].length -= lineDestance / 50;
		}
	}

	++timer;
	if (timer > 10000000) timer = 0;
}

void SpecialBullet::MoveCircleTonado()
{
	if (timer < 200)
	{
		circleStartAngle += PI / (400 - timer);
		if (circleStartAngle > PI * 2) circleStartAngle -= PI * 2;
		for (int i = 0; i < 100; ++i)
		{
			pointCircle[i].pos = {pos.x + cos(circleStartAngle + PI * 2 / 100 * i) * 300, pos.y + sin(circleStartAngle + PI * 2 / 100 * i) * 300};
		}
	}

	++timer;
}

void SpecialBullet::FireGuidArrow()
{
	circleStartAngle += PI / 200;
	if (circleStartAngle > PI * 2) circleStartAngle -= PI * 2;
	float circleAngle, outAngle;
	for (int i = 0; i < 100; ++i)
	{
		circleAngle = circleStartAngle + PI * 2 / 100 * i;
		if (circleAngle > PI * 2) circleAngle -= PI * 2;
		pointCircle[i].pos = { pos.x + cos(circleAngle) * 300, pos.y + sin(circleAngle) * 300 };
		if (timer < 400)
		{
			if (timer % 50 == 0)
			{
				outAngle = circleAngle - PI / 4;
				if (outAngle < 0) outAngle += PI * 2;
				guideBullet[i * 8 + timer / 50].Fire(pointCircle[i].pos, 10, outAngle, 4);
				guideBullet[i * 8 + timer / 50].SetTarget(target);
			}
		}
	}

	++timer;
}

void SpecialBullet::FireFinish()
{
	if (timer < 200)
	{
		float circleAngle = circleStartAngle, fireAngle;
		if (timer % 2 == 0)
		{
			circleAngle += PI * 2 / 100 * timer / 2;
			pointCircle[timer / 2].isActive = false;
			for (int h = 0; h < 5; ++h)
			{
				fireAngle = circleAngle + PI * 2 / 5 * h;
				if (fireAngle > PI * 2) fireAngle -= PI * 2;
				guideBullet[timer / 2 * 5 + h].Init();
				guideBullet[timer / 2 * 5 + h].Fire(pointCircle[timer / 2].pos, 10, fireAngle, 4);
				guideBullet[timer / 2 * 5 + h].SetTarget(target);
			}
		}
	}

	++timer;
}

SpecialBullet::SpecialBullet()
{
}


SpecialBullet::~SpecialBullet()
{
}

HRESULT SpecialBullet::Init()
{
	isShoot = false;
	pos = {0, 0};
	origin = {0, 0};
	size = 0;
	radian = 0;
	speed = 0;
	damage = 1;
	rc = { 0, 0, 0, 0 };
	rotateDelay = 100;
	state = SpecialBulletState::SBS_END;

	hBrush[0] = CreateSolidBrush(RGB(255, 0, 0));
	hBrush[1] = CreateSolidBrush(RGB(255, 127, 0));
	hBrush[2] = CreateSolidBrush(RGB(255, 255, 0));
	hBrush[3] = CreateSolidBrush(RGB(0, 255, 0));
	hBrush[4] = CreateSolidBrush(RGB(0, 0, 255));
	hBrush[5] = CreateSolidBrush(RGB(75, 0, 130));
	hBrush[6] = CreateSolidBrush(RGB(148, 0, 211));

	hBrush[7] = CreateSolidBrush(RGB(0,0,0));
	hBrush[8] = CreateSolidBrush(RGB(255, 255, 255));

	hPen[0] = CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
	hPen[1] = CreatePen(PS_SOLID, 10, RGB(255, 127, 0));
	hPen[2] = CreatePen(PS_SOLID, 10, RGB(255, 255, 0));
	hPen[3] = CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
	hPen[4] = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
	hPen[5] = CreatePen(PS_SOLID, 10, RGB(75, 0, 130));
	hPen[6] = CreatePen(PS_SOLID, 10, RGB(148, 0, 211));
	hPen[7] = CreatePen(PS_SOLID, 10, RGB(0, 0, 0));
	hPen[8] = CreatePen(PS_SOLID, 10, RGB(255, 255, 255));

	guideBullet = new GuideBullet[800];
	for (int i=0;i<800; ++i)
	{
		guideBullet[i].Init();
	}

	return S_OK;
}

void SpecialBullet::Update()
{
	if (!isShoot) return;

	switch (state)
	{
	case SpecialBullet::SpecialBulletState::SBS_INIT:
		MoveCenter();
		if (pow(pos.x - WINSIZE_X / 2, 2) + pow(pos.y - WINSIZE_Y / 2, 2) < 16)
		{
			// 4픽셀 이하라면 다음단계로
			pos = { (float)WINSIZE_X / 2, (float)WINSIZE_Y / 2 };
			state = SpecialBullet::SpecialBulletState::SBS_MAGIC_CIRCLE;
			timer = 0;
			for (int i = 0; i < 100; ++i)
			{
				pointCircle[i].isActive = (i % 25 == 0);
				pointCircle[i].isFixed = false;
				pointCircle[i].pos = { (float)WINSIZE_X / 2, (float)WINSIZE_Y / 2 };
				pointCircle[i].radius = 10;
			}

			for (int i = 0; i < 4; ++i)
			{
				circleAngle[i] = PI / 2 * i;
			}
		}
		SetRect();
		break;
	case SpecialBullet::SpecialBulletState::SBS_MAGIC_CIRCLE:
		MoveCircleLine();
		if (timer == 450)
		{
			state = SpecialBullet::SpecialBulletState::SBS_STAR_LIGHT;
			timer = 0;

			for (int i = 0; i < 5; ++i)
			{
				pointLine[i].origin = {pos.x + cos(-PI / 2 + (PI * 2 / 5) * i) * 300, pos.y + sin(-PI / 2 + (PI * 2 / 5) * i) * 300 };
				pointLine[i].from = pointLine[i].origin;
				pointLine[i].length = 0;
			}

			for (int i = 0; i < 5; ++i)
			{
				pointLine[i].angle = atan2(pointLine[(i + 2) % 5].origin.y - pointLine[i].origin.y, pointLine[(i + 2) % 5].origin.x - pointLine[i].origin.x);
			}
		}
		break;
	case SpecialBullet::SpecialBulletState::SBS_STAR_LIGHT:
		MoveStarLine();
		if (timer == 100)
		{
			state = SpecialBullet::SpecialBulletState::SBS_TORNADO;
			circleStartAngle = -PI / 2;
			timer = 0;
		}
		break;
	case SpecialBullet::SpecialBulletState::SBS_SPREAD:
		break;
	case SpecialBullet::SpecialBulletState::SBS_TORNADO:
		MoveCircleTonado();
		if (timer == 200)
		{
			state = SpecialBullet::SpecialBulletState::SBS_GAIDE_ARROW;
			timer = 0;
		}
		break;
	case SpecialBullet::SpecialBulletState::SBS_GAIDE_ARROW:
		FireGuidArrow();
		for (int i = 0; i < 800; ++i)
		{
			guideBullet[i].Update();
			if (timer == 400)
			{
				guideBullet[i].Init();
				guideBullet[i].SetIsShoot(false);
			}
		}
		if (timer == 400)
		{
			state = SpecialBullet::SpecialBulletState::SBS_END;
			timer = 0;
		}
		break;
	case SpecialBullet::SpecialBulletState::SBS_END:
		FireFinish();
		if (timer % 4 == 0 || timer >= 200)
		{
			for (int i = 0; i < 800; ++i)
			{
				guideBullet[i].Update();
			}
		}
		if (timer == 400)
		{
			isShoot = false;
		}
		break;
	}
}

void SpecialBullet::Render(HDC hdc)
{
	if (!isShoot) return;

	RECT rc23 = {0, 0, 0, 0};
	float angle = PI / 2;
	switch (state)
	{
	case SpecialBullet::SpecialBulletState::SBS_INIT:
		RenderEllipse(hdc, rc, hBrush[timer % 7]);
		break;
	case SpecialBullet::SpecialBulletState::SBS_MAGIC_CIRCLE:
		RenderEllipse(hdc, rc, hBrush[7]);
		for (int i = 0; i < 100; ++i)
		{
			if (pointCircle[i].isActive && !pointCircle[i].isFixed) RenderEllipse(hdc, pointCircle[i].GetRc(), hBrush[timer % 2 + 7]);
			else if (pointCircle[i].isActive && pointCircle[i].isFixed) RenderEllipse(hdc, pointCircle[i].GetRc(), hBrush[0]);
		}
		break;
	case SpecialBullet::SpecialBulletState::SBS_STAR_LIGHT:
		for (int i = 0; i < 100; ++i)
		{
			RenderEllipse(hdc, pointCircle[i].GetRc(), hBrush[0]);
		}
		for (int i = 0; i < 5; ++i)
		{
			RenderLineTo(hdc, pointLine[i].from, pointLine[i].angle, pointLine[i].length, NULL, hPen[0]);
		}
		break;
	case SpecialBullet::SpecialBulletState::SBS_SPREAD:
		break;
	case SpecialBullet::SpecialBulletState::SBS_TORNADO:
		for (int i = 0; i < 100; ++i)
		{
			RenderEllipse(hdc, pointCircle[i].GetRc(), hBrush[0]);
		}
		break;
	case SpecialBullet::SpecialBulletState::SBS_GAIDE_ARROW:
		for (int i = 0; i < 100; ++i)
		{
			RenderEllipse(hdc, pointCircle[i].GetRc(), hBrush[0]);
		}
		for (int i = 0; i < 800; ++i)
		{
			guideBullet[i].Render(hdc);
		}
		break;
	case SpecialBullet::SpecialBulletState::SBS_END:
		for (int i = 0; i < 100; ++i)
		{
			if (pointCircle[i].isActive) RenderEllipse(hdc, pointCircle[i].GetRc(), hBrush[0]);
		}
		for (int i = 0; i < 800; ++i)
		{
			guideBullet[i].Render(hdc);
		}
		break;
	}
}

void SpecialBullet::Release()
{
	for (int i = 0; i < 9; ++i)
	{
		DeleteObject(hBrush[i]);
		DeleteObject(hPen[i]);
	}
	for (int i = 0; i < 800; ++i) guideBullet[i].Release();
	delete[] guideBullet;
}

void SpecialBullet::Fire(POINTFLOAT pos, int size, float radian, float speed)
{
	state = SpecialBulletState::SBS_INIT;

	isShoot = true;
	this->pos = pos;
	this->origin = pos;
	this->size = size;
	this->radian = radian;
	this->speed = speed;
	rodateRadian = radian;
	timer = 0;
	SetRect();
}

void SpecialBullet::SetTarget(Enemy * target)
{
	this->target = target;
}
