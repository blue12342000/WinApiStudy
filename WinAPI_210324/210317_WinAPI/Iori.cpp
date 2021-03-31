#include "Iori.h"
#include "Image.h"

HRESULT Iori::Init()
{
	pos.x = WINSIZE_X / 2;
	pos.y = WINSIZE_Y - 200;

	img = new Image();
	if (FAILED(img->Init("Image/Iori_walk.bmp", 612, 104)))
	{
		MessageBox(g_hWnd, "이미지 로드 실패", "Error", MB_OK);
		img->Release();
		delete img;
		img = nullptr;
	}
	return S_OK;
}

void Iori::Update()
{
	if (elapedTime++ % 100 == 0)
	{
		++frame %= 9;
	}
}

void Iori::Render(HDC hdc)
{
	if (img)
	{
		img->RenderFrame(hdc, pos.x, pos.y, frame);
	}
}

void Iori::Release()
{
	if (img)
	{
		img->Release();
		delete img;
	}
}
