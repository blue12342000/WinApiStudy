#pragma once
#include <Windows.h>

extern void RenderRect(HDC, RECT, HBRUSH, HPEN);

struct Obstacle
{
	bool isPlay = false;
	int pice;
	POINT pos;
	int tick;
	int lifecycle;
	RECT rc;
	HBRUSH hBrush = NULL;
	HPEN hPen = NULL;

	Obstacle() {}
	Obstacle(int pice, int tick, int lifecycle, RECT rc)
	{
		this->pice = pice;
		this->tick = tick;
		this->lifecycle = lifecycle;
		this->rc = rc;
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
			{
				isPlay = false;
				this->tick = 0;
			}
		}
		else
		{
			this->tick = tick + 1;
			if (tick == lifecycle)
			{
				isPlay = true;
				this->tick = 0;
			}
		}
	}

	void Render(HDC hdc)
	{
		if (isPlay == true)
		{
			RenderRect(hdc, rc, hBrush, hPen);
		}
	}
};