#pragma once
#include "GameNode.h"

enum class ButtonState
{
	DOWN,
	UP,
	NONE
};

typedef void (*func_t)(int*);

class Image;
class Button : public GameNode
{
private:
	ButtonState state;
	Image* img;
	RECT rc;
	POINT pos;

	func_t func;
	int* lpFuncArg;
	int funcArg;

public:
	HRESULT Init(const char* fileName, int x, int y);
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	inline void SetFunction(func_t func, int funcArg) { this->func = func; this->funcArg = funcArg; };
	inline void SetFunction(func_t func, int* lpFuncArg) { this->func = func; this->lpFuncArg = lpFuncArg; };
};

