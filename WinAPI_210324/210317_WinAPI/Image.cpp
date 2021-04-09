#include "Image.h"

HRESULT Image::Init(int width, int height)
{
	HDC hdc = GetDC(g_hWnd);

	lpImageInfo = new ImageInfo();
	lpImageInfo->resourceId = 0;
	lpImageInfo->hMemoryDC = CreateCompatibleDC(hdc);
	lpImageInfo->hBitMap = CreateCompatibleBitmap(hdc, width, height);
	lpImageInfo->hOldBitMap = (HBITMAP)SelectObject(lpImageInfo->hMemoryDC, lpImageInfo->hBitMap);
	lpImageInfo->width = width;
	lpImageInfo->height = height;
	lpImageInfo->loadType = IMAGE_LOAD_TYPE::ILT_EMPTY;

	ReleaseDC(g_hWnd, hdc);

	if (lpImageInfo->hBitMap == NULL)
	{
		Release();
		return E_FAIL;
	}

	this->isTrans = false;
	this->lpImageInfo->transColor = RGB(0, 0, 0);

	return S_OK;
}

HRESULT Image::Init(std::string filename, int width, int height, int frameX, int frameY)
{
	HDC hdc = GetDC(g_hWnd);

	lpImageInfo = new ImageInfo();
	lpImageInfo->resourceId = 0;
	lpImageInfo->hMemoryDC = CreateCompatibleDC(hdc);
	lpImageInfo->hBitMap = (HBITMAP)LoadImage(g_hInstance, filename.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	lpImageInfo->hOldBitMap = (HBITMAP)SelectObject(lpImageInfo->hMemoryDC, lpImageInfo->hBitMap);
	lpImageInfo->width = width / frameX;
	lpImageInfo->height = height / frameY;
	lpImageInfo->loadType = IMAGE_LOAD_TYPE::ILT_FILE;

	this->frameX = frameX;
	this->frameY = frameY;
	this->totalFrames = frameX * frameY;

	ReleaseDC(g_hWnd, hdc);

	if (lpImageInfo->hBitMap == NULL)
	{
		Release();
		return E_FAIL;
	}

	this->isTrans = false;
	this->lpImageInfo->transColor = RGB(0, 0, 0);

	return S_OK;
}

HRESULT Image::Init(std::string filename, int width, int height, int frameX, int frameY, int totalFrames)
{
	if (FAILED(Init(filename, width, height, frameX, frameY)))
	{
		return E_FAIL;
	}
	this->totalFrames = totalFrames;
	this->isTrans = false;
	this->lpImageInfo->transColor = RGB(0, 0, 0);
	return S_OK;
}

HRESULT Image::Init(std::string filename, int width, int height, int frameX, int frameY, int totalFrames, bool isTrans, COLORREF transColor)
{
	if (FAILED(Init(filename, width, height, frameX, frameY)))
	{
		return E_FAIL;
	}
	this->totalFrames = totalFrames;
	this->isTrans = isTrans;
	this->lpImageInfo->transColor = transColor;
	return S_OK;
}

void Image::Render(HDC hdc, int startX, int startY, int drawWidth, int drawHeight, int drawStartX, int drawStartY)
{
	if (lpImageInfo)
	{
		if (isTrans)
		{
			GdiTransparentBlt(hdc, startX, startY, lpImageInfo->width, lpImageInfo->height, lpImageInfo->hMemoryDC, 0, 0, lpImageInfo->width, lpImageInfo->height, lpImageInfo->transColor);
		}
		else
		{
			BitBlt(hdc, startX, startY, (drawWidth < 0) ? lpImageInfo->width : drawWidth, (drawHeight < 0) ? lpImageInfo->height : drawHeight, lpImageInfo->hMemoryDC, (drawStartX < 0) ? 0 : drawStartX, (drawStartY < 0) ? 0 : drawStartY, SRCCOPY);
		}
	}
}

void Image::RenderFrame(HDC hdc, int startX, int startY, int frameIndex)
{
	if (lpImageInfo)
	{
		frameIndex %= totalFrames;
		startX -= lpImageInfo->width / 2;
		startY -= lpImageInfo->height / 2;

		if (isTrans)
		{
			GdiTransparentBlt(hdc, startX, startY, lpImageInfo->width, lpImageInfo->height, lpImageInfo->hMemoryDC, lpImageInfo->width * (frameIndex % frameX), lpImageInfo->height * (frameIndex / frameX), lpImageInfo->width, lpImageInfo->height, lpImageInfo->transColor);
		}
		else
		{
			BitBlt(hdc, startX, startY, lpImageInfo->width, lpImageInfo->height, lpImageInfo->hMemoryDC, lpImageInfo->width * (frameIndex % frameX), lpImageInfo->height * (frameIndex / frameX), SRCCOPY);
		}
	}
}

void Image::Release()
{
	if (lpImageInfo)
	{
		SelectObject(lpImageInfo->hMemoryDC, lpImageInfo->hOldBitMap);
		DeleteObject(lpImageInfo->hBitMap);
		DeleteDC(lpImageInfo->hMemoryDC);

		delete lpImageInfo;
		lpImageInfo = nullptr;
	}
}
