#pragma once
#include  <Windows.h>
#include  "Unit.h"

extern void RenderRect(HDC hdc, RECT rc, HBRUSH hBrush, HPEN hPen);

class UIBattleStatus
{
	POINT unitAPoint;
	Unit* unitA = nullptr;

	POINT unitBPoint;
	Unit* unitB = nullptr;

	HBRUSH hbBackground = NULL;
	HPEN hpBackground = NULL;
	RECT bgRect;

	HBRUSH hbRemainHp = NULL;
	HPEN hpRemainHp = NULL;
	RECT remainHpRect;

	HBRUSH hbMaxHp = NULL;
	HPEN hpMaxHp = NULL;
	RECT maxHpRect;

	int hpBarWidth = 100;
	int hpBarHeight = 40;

public:
	void SetHbBackground(HBRUSH hbBackground) { this->hbBackground = hbBackground; }
	void SetHpBackground(HPEN hpBackground) { this->hpBackground = hpBackground; }
	void SetHbRemainHp(HBRUSH hbRemainHp) { this->hbRemainHp = hbRemainHp; }
	void SetHpRemainHp(HPEN hpRemainHp) { this->hpRemainHp = hpRemainHp; }
	void SetHbMaxHp(HBRUSH hbMaxHp) { this->hbMaxHp = hbMaxHp; }
	void SetHpMaxHp(HPEN hpMaxHp) { this->hpMaxHp = hpMaxHp; }

	void SetHpBarWidth(int hpBarWidth) { this->hpBarWidth = hpBarWidth; }
	void SetHpBarHeight(int hpBarHeight) { this->hpBarHeight = hpBarHeight; }

	void Init(int width, int height, Unit* unitA, Unit* unitB, POINT unitAPoint, POINT unitBPoint)
	{
		bgRect = { 0, 0, width, height };

		this->unitA = unitA;
		this->unitB = unitB;

		this->unitAPoint = unitAPoint;
		this->unitBPoint = unitBPoint;
	}

	void Render(HDC hdc)
	{
		RenderRect(hdc, bgRect, hbBackground, hpBackground);

		TextOut(hdc, unitAPoint.x - 30, unitAPoint.y + 15, "PLAYER", 6);
		TextOut(hdc, unitBPoint.x - 25, unitBPoint.y + 15, "ENEMY", 5);

		maxHpRect = RectCenter({ unitAPoint.x, unitAPoint.y + 60 }, hpBarWidth, hpBarHeight);
		RenderRect(hdc, maxHpRect, hbMaxHp, hpMaxHp);
		maxHpRect = RectCenter({ unitBPoint.x, unitBPoint.y + 60 }, hpBarWidth, hpBarHeight);
		RenderRect(hdc, maxHpRect, hbMaxHp, hpMaxHp);

		remainHpRect = RectCenter({ unitAPoint.x, unitAPoint.y + 60 }, (float)hpBarWidth * unitA->GetHp() / unitA->GetMaxHp(), hpBarHeight);
		RenderRect(hdc, remainHpRect, hbRemainHp, hpRemainHp);
		remainHpRect = RectCenter({ unitBPoint.x, unitBPoint.y + 60 }, (float)hpBarWidth * unitB->GetHp() / unitB->GetMaxHp(), hpBarHeight);
		RenderRect(hdc, remainHpRect, hbRemainHp, hpRemainHp);
	}

	void Release()
	{
		if (hbMaxHp != NULL) DeleteObject(hbMaxHp);
		if (hpMaxHp != NULL) DeleteObject(hpMaxHp);

		if (hbRemainHp != NULL) DeleteObject(hbRemainHp);
		if (hpRemainHp != NULL) DeleteObject(hpRemainHp);
	}
};