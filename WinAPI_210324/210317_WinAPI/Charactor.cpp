#include "Charactor.h"
#include "Image.h"

HRESULT Charactor::Init()
{
	state = CharaterState::CS_END;

	width = 0;
	height = 0;

	img = nullptr;

	elapedTime = 0;
	frame = 0;

	pos.x = 0;
	pos.y = 0;

	return S_OK;
}

void Charactor::Update()
{
	if (elapedTime++ % 5 == 0)
	{
		++frame %= img[(int)state].GetTotalFrames();
	}

	rc.left = pos.x - width / 2;
	rc.top = pos.y - height / 2;
	rc.right = pos.x + width / 2;
	rc.bottom = pos.y + height / 2;
}

void Charactor::Render(HDC hdc)
{
	if (img) img[(int)state].RenderFrame(hdc, rc.left, rc.top, frame);
}

void Charactor::Release()
{
	if (img)
	{
		for (int i = 0; i < (int)CharaterState::CS_END; ++i)
		{
			img[(int)state].Release();
		}
		delete[] img;
		img = nullptr;
	}
}
