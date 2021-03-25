#pragma once
#include "Config.h"

class GameNode
{
public:
	virtual HRESULT Init() { return E_FAIL; }
	virtual void Update() {}
	virtual void Render(HDC hdc) {}
	virtual void Release() {}
};

