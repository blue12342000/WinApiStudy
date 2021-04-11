#pragma once

class Image
{
public:
	static const int ROTATE_SPLIT = 36;

private:
	HDC hMemoryDc;
	HBITMAP hBitmap;
	int width;
	int height;
	int frameX;
	int frameY;
	int totalFrames;
	bool isTrans;
	COLORREF transColor;

public:
	HRESULT Init(int width, int height, bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));
	HRESULT Init(string filePath, int width, int height, int frameX = 1, int frameY = 1, int totalFrame = 1, bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));
	void Release();
	void Update();

	void Render(HDC hdc, int destX = 0, int destY = 0);
	void Render(HDC hdc, POINT pos, int frame, int angle = 0);
	void Render(HDC hdc, POINTFLOAT pos, int frame, int angle = 0);

	HRESULT Reverse(Image* origin);

	inline HDC GetMemoryDC() { return hMemoryDc; }
	inline int GetTotalFrames() { return totalFrames; }
};

