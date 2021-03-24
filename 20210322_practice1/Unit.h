#pragma once
#include <Windows.h>
#include <cmath>
#include "DyingMotion.h"

extern RECT RectCenter(POINT p, int width, int height);
extern void RenderRect(HDC hdc, RECT rc, HBRUSH hBrush, HPEN hPen);
extern void RenderCircle(HDC hdc, RECT rc, HBRUSH hBrush, HPEN hPen);
extern void DyingMotionToCenter(HDC hdc, int x, int y, int size);
extern void RenderPlayer(HDC hdc, RECT rc, POINT p, HBRUSH hBrush, HPEN hPen);
extern void RenderPlayer2(HDC hdc, RECT rc, POINT p, HBRUSH hBrush, HPEN hPen);

extern DyingMotion dyingMotion;

struct UnitBullet
{
	int size;
	float speed;
	POINT pos;
	RECT rc;
	HPEN hPen;
	HBRUSH hBrush;

	UnitBullet() {}
	UnitBullet(POINT pos, int size, float speed) :pos(pos), size(size), speed(speed) { SetRect(); }

	void SetPos(POINT pos)
	{
		this->pos = pos;
		SetRect();
	}

	void SetRect()
	{
		rc = RectCenter(pos, size, size);
	}

	void Move(bool isShootLeft)
	{
		if (isShootLeft) pos.x -= speed;
		else pos.x += speed;
		SetRect();
	}

	void SetHPEN(HPEN hPen)
	{
		this->hPen = hPen;
	}

	void SetHBRUSH(HBRUSH hBrush)
	{
		this->hBrush = hBrush;
	}


	bool up()
	{
		const double PI = 3.1415926;
		double b = cos(pos.x * PI / 90);
		if (b == 1)
		{
			return 1;
		}
		else if (b == -1)
		{
			return 0;
		}
	}

	void Render(HDC hdc)
	{

		HBRUSH hOldBrush = NULL;
		HPEN hOldPen = NULL;
		const double PI = 3.1415926;
		double b = cos(pos.x * PI / 90);

		RECT rectA = { rc.left + 10, rc.top - 50 * b, rc.right - 10, rc.bottom - 50 * b };
		RECT rectB = { rc.left + 10, rc.top + 50 * b, rc.right - 10, rc.bottom + 50 * b };

		if (up())
		{
			RenderCircle(hdc, rectA, hBrush, hPen);
		}
		else
		{
			RenderCircle(hdc, rectB, NULL, NULL);
		}
		RenderCircle(hdc, rc, hBrush, hPen);
		if (up())
		{
			RenderCircle(hdc, rectB, NULL, NULL);
		}
		else
		{
			RenderCircle(hdc, rectA, hBrush, hPen);
		}
	}

	bool IsCollition(RECT other)
	{
		POINT points[4] = { {other.left, other.top}, {other.left, other.bottom}, {other.right, other.top}, {other.right, other.bottom} };

		for (POINT p : points)
		{
			if (p.x > rc.left && p.y > rc.top && p.x < rc.right && p.y < rc.bottom)
			{
				return true;
			}
		}

		POINT originPoints[4] = { {rc.left, rc.top}, {rc.left, rc.bottom}, {rc.right, rc.top}, {rc.right, rc.bottom} };

		for (POINT p : originPoints)
		{
			if (p.x > other.left && p.y > other.top && p.x < other.right && p.y < other.bottom)
			{
				return true;
			}
		}

		if (((rc.left < other.left && other.right < rc.right) && (other.top < rc.top && rc.top < other.bottom))
			|| ((other.left < rc.left && rc.right < other.right) && (rc.top < other.top && other.top < rc.bottom)))
		{
			return true;
		}

		return false;
	}

	void Release()
	{
		if (hBrush != NULL) DeleteObject(hBrush);
		if (hPen != NULL) DeleteObject(hPen);
	}
};

class Unit
{
private:
	enum UnitType
	{
		NORMAL, INVISIBLE_DRAGON
	};

	UnitType unitType = NORMAL;

protected:
	enum UnitState
	{
		ALIVE, DEAD
	};
	bool isShoot = false;
	bool isShootLeft = false;

	int hp;
	int maxHp;

	int width;
	int height;
	UnitState state = ALIVE;
	POINT pos;
	RECT rc;

	HPEN hPen = NULL;
	HBRUSH hBrush = NULL;

	UnitBullet bullet;

public:
	Unit() {}
	Unit(POINT pos, int width, int height, float bulletSize) :pos(pos), width(width), height(height), hp(100), maxHp(100) { bullet = UnitBullet(pos, bulletSize, 10); SetRect(); }

	void SetRect()
	{
		rc = RectCenter(pos, width, height);
	}

	RECT GetRect()
	{
		return rc;
	}

	UnitBullet GetBullet()
	{
		return bullet;
	}

	POINT GetPos()
	{
		return pos;
	}

	int GetHp() { return hp; }
	int GetMaxHp() { return maxHp; }

	void SetHPEN(HPEN hPen)
	{
		this->hPen = hPen;
	}

	void SetHBRUSH(HBRUSH hBrush)
	{
		this->hBrush = hBrush;
	}

	void SetBulletHPEN(HPEN hPen)
	{
		bullet.SetHPEN(hPen);
	}

	void SetBulletHBRUSH(HBRUSH hBrush)
	{
		bullet.SetHBRUSH(hBrush);
	}

	void InitBullet()
	{
		isShoot = false;
		bullet.SetPos(pos);
	}

	void Move(POINT delta)
	{
		if (unitType == INVISIBLE_DRAGON) return;
		if (state == DEAD) return;

		pos.x += delta.x;
		pos.y += delta.y;
		if (!isShoot) bullet.SetPos(pos);
		SetRect();
	}

	void Shoot(bool isShootLeft)
	{
		if (unitType == INVISIBLE_DRAGON)
		{
			PostQuitMessage(0);
			return;
		}

		if (state == DEAD) return;
		if (!isShoot)
		{
			isShoot = true;
			this->isShootLeft = isShootLeft;
		}
	}

	bool IsAlive()
	{
		return state != DEAD;
	}

	void Hit()
	{
		if (unitType == INVISIBLE_DRAGON) return;

		hp -= 20;
		if (hp <= 0)
		{
			hp = 0;
			state = DEAD;
			dyingMotion.Play(pos);
		}
		SetRect();
	}

	void UnitChange()
	{
		if (unitType == INVISIBLE_DRAGON) unitType = NORMAL;
		else unitType = INVISIBLE_DRAGON;
	}

	virtual void Update()
	{
		if (unitType == INVISIBLE_DRAGON) return;

		if (isShoot)
		{
			bullet.Move(isShootLeft);
		}
	}

	virtual void Render(HDC hdc)
	{
		if (unitType == NORMAL)
		{
			if (state == ALIVE)
			{
				HBRUSH hOldBrush = NULL;
				HPEN hOldPen = NULL;
				if (hBrush != NULL) hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				if (hPen != NULL) hOldPen = (HPEN)SelectObject(hdc, hPen);
				RenderPlayer(hdc, rc, pos, hBrush, hPen);
				if (hPen != NULL) SelectObject(hdc, hOldPen);
				if (hBrush != NULL) SelectObject(hdc, hOldBrush);
				bullet.Render(hdc);
			}
			else if (isShoot)
			{
				bullet.Render(hdc);
			}
		}
	}

	void Release()
	{
		if (hBrush != NULL) DeleteObject(hBrush);
		if (hPen != NULL) DeleteObject(hPen);
		bullet.Release();
	}
};