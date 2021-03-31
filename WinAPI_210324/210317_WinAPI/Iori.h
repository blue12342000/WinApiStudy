#pragma once
#include "GameNode.h"

class Image;
class Iori : public GameNode
{
private:
	Image* img;
	POINTFLOAT pos;
	// 100이 될때마다 애니메이션 프레임을 1씩 증가
	int elapedTime;
	// 애니메이션 프레임 0~9
	int frame;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

