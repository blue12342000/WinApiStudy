#pragma once
#include "GameNode.h"

class Image;
class Charactor : public GameNode
{
public:
	enum class CharaterState : int
	{
		CS_IDLE,
		CS_ATK,
		CS_MOVE,
		CS_JUMP,
		CS_DEATH,
		CS_END
	};

protected:
	CharaterState state;

	int elapedTime;
	int frame;
	int width;
	int height;
	POINTFLOAT pos;

	string fileName[(int)CharaterState::CS_END];
	Image* img;
	RECT rc;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	inline void SetPos(POINTFLOAT pos) { this->pos = pos; }
	inline bool IsAlive() { return state != CharaterState::CS_DEATH; }
};

