#include "stdafx.h"
#include "Image.h"

HRESULT Image::Init(int width, int height, bool isTrans, COLORREF transColor)
{
	HDC hdc = GetDC(g_hWnd);
	hMemoryDc = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, width, height);
	this->width = width;
	this->height = height;
	this->frameX = 1;
	this->frameY = 1;
	this->totalFrames = 1;
	this->isTrans = isTrans;
	this->transColor = transColor;

	if (!hBitmap)
	{
		Release();
		return E_FAIL;
	}

	blendInfo = nullptr;

	DeleteObject(SelectObject(hMemoryDc, hBitmap));
	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(string filePath, int width, int height, int frameX, int frameY, int totalFrames, bool isTrans, COLORREF transColor)
{
	HDC hdc = GetDC(g_hWnd);
	hMemoryDc = CreateCompatibleDC(hdc);
	hBitmap = (HBITMAP)LoadImage(g_hInstance, filePath.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	this->width = width / frameX;
	this->height = height / frameY;
	this->frameX = frameX;
	this->frameY = frameY;
	this->totalFrames = (totalFrames > 0) ? totalFrames : (frameX * frameY);
	this->isTrans = isTrans;
	this->transColor = transColor;

	if (!hBitmap)
	{
		Release();
		return E_FAIL;
	}

	DeleteObject(SelectObject(hMemoryDc, hBitmap));

	blendInfo = new BlendInfo();
	blendInfo->hBlendDC = CreateCompatibleDC(hMemoryDc);
	blendInfo->hBitmap = CreateCompatibleBitmap(hMemoryDc, width, height);
	DeleteObject(SelectObject(blendInfo->hBlendDC, blendInfo->hBitmap));
	blendInfo->bf.AlphaFormat = 0;
	blendInfo->bf.BlendFlags = 0;
	blendInfo->bf.BlendOp = AC_SRC_OVER;

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

void Image::Release()
{
	if (hBitmap) DeleteObject(hBitmap);
	if (hMemoryDc) DeleteDC(hMemoryDc);
	if (blendInfo)
	{
		DeleteObject(blendInfo->hBitmap);
		DeleteDC(blendInfo->hBlendDC);
	}

	hBitmap = NULL;
	hMemoryDc = NULL;
}

void Image::Update()
{
}

void Image::Render(HDC hdc, int destX, int destY)
{
	if (isTrans)
	{
		GdiTransparentBlt(hdc, destX, destY, width, height,
			hMemoryDc, 0, 0, width, height, transColor);
	}
	else
	{
		BitBlt(hdc, destX, destY, width, height,
			hMemoryDc, 0, 0, SRCCOPY);
	}
}

void Image::Render(HDC hdc, POINT pos, int frame, int angle)
{
	if (isTrans)
	{
		GdiTransparentBlt(hdc, pos.x, pos.y, width, height,
			hMemoryDc, width * (frame % frameX), height * (frame / frameX), width, height, transColor);
	}
	else
	{
		BitBlt(hdc, pos.x, pos.y, width, height,
			hMemoryDc, width * (frame % frameX), height * (frame / frameX), SRCCOPY);
	}
}

void Image::Render(HDC hdc, POINTFLOAT pos, int frame, int angle)
{
	Render(hdc, POINT{ (int)pos.x, (int)pos.y }, frame);
}

void Image::AlphaRender(HDC hdc, POINTFLOAT pos, int frame, int alpha, UINT uFlag)
{
	if (blendInfo)
	{
		if (uFlag & U_EF_MOSAIC)
		{
			HDC hTempDC = CreateCompatibleDC(hMemoryDc);
			HBITMAP hTempBitmap = CreateCompatibleBitmap(hMemoryDc, width * 30, height * 30);
			DeleteObject(SelectObject(hTempDC, hTempBitmap));
			StretchBlt(hTempDC, 0, 0, width * 30, height * 30, hMemoryDc, width * (frame % frameX), height * (frame / frameX), width, height, SRCCOPY);
			StretchBlt(hTempDC, 0, 0, width, height, hTempDC, 0, 0, width * 30, height * 30, SRCCOPY);

			BitBlt(blendInfo->hBlendDC, 0, 0, width, height, hdc, pos.x, pos.y, SRCCOPY);

			GdiTransparentBlt(blendInfo->hBlendDC, 0, 0, width, height,
				hTempDC, 0, 0, width, height, transColor);

			DeleteObject(hTempBitmap);
			DeleteDC(hTempDC);
		}
		else if (uFlag & U_EF_BLUR)
		{
			HDC hTempDC = CreateCompatibleDC(hMemoryDc);
			HBITMAP hTempBitmap = CreateCompatibleBitmap(hMemoryDc, width, height);
			DeleteObject(SelectObject(hTempDC, hTempBitmap));

			BitBlt(hTempDC, 0, 0, width, height, hMemoryDc, width * (frame % frameX), height * (frame / frameX), SRCCOPY);
			int blur = 5;
			for (int y = blur - 1; y < height; y += blur)
			{
				for (int x = blur - 1; x < width; x += blur)
				{
					COLORREF pixelColor, blurColor;
					DWORD r, g, b;
					bool isBlur = false;
					for (int i = 0; i < blur; ++i)
					{
						for (int j = 0; j < blur; ++j)
						{
							pixelColor = GetPixel(hTempDC, x - i, y - j);
							if (!isBlur && pixelColor != transColor)
							{
								blurColor = pixelColor;
								isBlur = true;
							}
							else if (isBlur && pixelColor != transColor)
							{
								blurColor += pixelColor;
							}
						}
					}
					if (isBlur)
					{
						blurColor = RGB(GetRValue(blurColor) / 4, GetGValue(blurColor) / 4, GetBValue(blurColor) / 4);
						for (int i = 0; i < blur; ++i)
						{
							for (int j = 0; j < blur; ++j)
							{
								if (GetPixel(hMemoryDc, x-i, y-j) != transColor) SetPixelV(hTempDC, x-i, y-j, blurColor);
							}
						}
					}
				}
			}

			BitBlt(blendInfo->hBlendDC, 0, 0, width, height, hdc, pos.x, pos.y, SRCCOPY);
			GdiTransparentBlt(blendInfo->hBlendDC, 0, 0, width, height,
				hTempDC, 0, 0, width, height, transColor);

			DeleteObject(hTempBitmap);
			DeleteDC(hTempDC);
		}
		else
		{
			BitBlt(blendInfo->hBlendDC, 0, 0, width, height, hdc, pos.x, pos.y, SRCCOPY);
			Render(blendInfo->hBlendDC, POINT{0, 0}, frame);
		}


		blendInfo->bf.SourceConstantAlpha = alpha;
		AlphaBlend(hdc, pos.x, pos.y, width, height, blendInfo->hBlendDC, 0, 0, width, height, blendInfo->bf);
	}
	else
	{
		Render(hdc, pos, frame);
	}
}

HRESULT Image::Reverse(Image* origin)
{
	hMemoryDc = CreateCompatibleDC(origin->hMemoryDc);
	hBitmap = CreateCompatibleBitmap(hMemoryDc, origin->width * origin->frameX, origin->height * origin->frameY);
	this->width = origin->width;
	this->height = origin->height;
	this->frameX = origin->frameX;
	this->frameY = origin->frameY;
	this->totalFrames = origin->totalFrames;
	this->isTrans = origin->isTrans;
	this->transColor = origin->transColor;

	if (!hBitmap)
	{
		Release();
		return E_FAIL;
	}

	for (int y = 0; y < frameY; ++y)
	{
		for (int x = 0; x < frameX; ++x)
		{
			StretchBlt(hMemoryDc, width * (x + 1), height * y, -width, height,
				origin->hMemoryDc, width * x, height * y, width, height, SRCCOPY);
		}
	}

	DeleteObject(SelectObject(hMemoryDc, hBitmap));
	return S_OK;
}
