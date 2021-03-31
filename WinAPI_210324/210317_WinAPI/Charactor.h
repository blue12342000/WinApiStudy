#pragma once
#include "GameNode.h"

class Image;
class Charactor : public GameNode
{
private:
	int elapedTime;
	int frame;
	int width;
	int height;
	POINTFLOAT pos;

	string fileName;
	Image* img;
	RECT rc;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	inline void SetPos(POINTFLOAT pos) { this->pos = pos; }
};

