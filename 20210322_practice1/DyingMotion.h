#pragma once
#include <Windows.h>
#include <cmath>

extern void DyingMotionToCenter(HDC hdc, int x, int y, int size);

struct DyingMotion
{
	bool isPlay = false;
	int pice;
	POINT pos;
	float speed;
	int tick;
	int lifecycle;

	DyingMotion() {}
	DyingMotion(int pice, float speed, int tick, int lifecycle)
	{
		this->pice = pice;
		this->speed = speed;
		this->tick = tick;
		this->lifecycle = lifecycle;
	}

	void Play(POINT pos)
	{
		isPlay = true;
		this->pos = pos;
		this->tick = 0;
	}

	void Update()
	{
		if (isPlay == true)
		{
			this->tick = tick + 1;
			if (tick == lifecycle)
				isPlay = false;
		}
	}

	void Render(HDC hdc)
	{
		if (isPlay == true)
		{
			for (int i = 0; i < 360; i += 360 / pice)
			{
				float radian = 3.14f * i / 180;
				POINT point = pos;

				point.x += cos(radian) * speed * tick;
				point.y += sin(radian) * speed * tick;

				HBRUSH hBrush, hOldBrush;
				hBrush = CreateSolidBrush(RGB(250, 250, 210));
				hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				DyingMotionToCenter(hdc, point.x, point.y, 20);
			}
		}
	}
};
