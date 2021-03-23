#pragma once
#include "Unit.h"

class AIUnit : public Unit
{
private:
	bool isMoveUp = false;

public:
	AIUnit() {}
	AIUnit(POINT pos, int width, int height, float bulletSize) :Unit(pos, width, height, bulletSize) {}

	void Update()
	{
		if (isMoveUp) Move({0, -5});
		else Move({0, 5});

		if (rand() % 100 < 5) Shoot(true);
		Unit::Update();

		if (pos.y < 100) isMoveUp = false;
		else if (pos.y > 490) isMoveUp = true;
	}

	void Render(HDC hdc) override
	{
		if (state == ALIVE)
		{
			HBRUSH hOldBrush = NULL;
			HPEN hOldPen = NULL;
			if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hPen);
			RenderPlayer2(hdc, rc, pos, hBrush, hPen);
			if (hPen != NULL) SelectObject(hdc, hOldPen);
			if (hBrush != NULL) SelectObject(hdc, hOldBrush);
			bullet.Render(hdc);
		}
		else if (isShoot)
		{
			bullet.Render(hdc);
		}
	}
};