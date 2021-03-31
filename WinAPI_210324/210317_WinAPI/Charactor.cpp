#include "Charactor.h"
#include "Image.h"

HRESULT Charactor::Init()
{
	width = 111 * 2;
	height = 83 * 2;

	fileName = "Image/Lasswell_Hess_King/Lasswell(Hess_King)/Lasswell King (5)/unit_idle_100023005.bmp";
	img = new Image();
	if (FAILED(img->Init(fileName, width * 3, height * 4, 3, 4)))
	{
		img->Release();
		delete img;
		img = nullptr;
		MessageBox(g_hWnd, "캐릭터이미지 로드에 실패하였습니다.", "Error", MB_OK);
	}

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
		++frame %= img->GetTotalFrames();
	}

	rc.left = pos.x - width / 2;
	rc.top = pos.y - height / 2;
	rc.right = pos.x + width / 2;
	rc.bottom = pos.y + height / 2;
}

void Charactor::Render(HDC hdc)
{
	if (img) img->RenderFrame(hdc, rc.left, rc.top, frame);
}

void Charactor::Release()
{
	if (img)
	{
		img->Release();
		delete img;
		img = nullptr;
	}
}
