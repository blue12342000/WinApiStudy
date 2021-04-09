#pragma once
#include "Config.h"

class Image
{
public:
	enum class IMAGE_LOAD_TYPE : BYTE
	{
		ILT_RESOURCE,
		ILT_FILE,
		ILT_EMPTY,
		ILT_END
	};

	typedef struct ImageInfo
	{
		// 리소스 ID
		DWORD resourceId;
		// 그릴 HDC
		HDC hMemoryDC;
		// 이미지 정보
		HBITMAP hBitMap;
		// 기존 이미지 정보
		HBITMAP hOldBitMap;
		// 가로
		int width;
		// 세로
		int height;
		// 이미지 로드 방식
		IMAGE_LOAD_TYPE loadType;

		COLORREF transColor;

		ImageInfo()
		{
			resourceId = 0;
			hMemoryDC = NULL;
			hBitMap = NULL;
			hOldBitMap = NULL;
			width = 0;
			height = 0;
			loadType = IMAGE_LOAD_TYPE::ILT_END;

			transColor = RGB(0, 0, 0);
			
		}
	} ImageInfo, *LPImageInfo;

private:
	LPImageInfo lpImageInfo;
	bool isTrans;
	int frameX;
	int frameY;
	int totalFrames;

public:
	// 빈비트맵으로 초기화
	HRESULT Init(int width, int height);

	// 파일로부터 이미지를 로드하는 함수
	HRESULT Init(std::string filename, int width, int height, int frameX = 1, int frameY = 1);
	HRESULT Init(std::string filename, int width, int height, int frameX, int frameY, int totalFrames);

	HRESULT Init(std::string filename, int width, int height, int frameX, int frameY, int totalFrames, bool isTrans, COLORREF transColor);

	void Update() {}

	// 화면에 출력
	void Render(HDC hdc, int startX = 0, int startY = 0, int drawWidth = -1, int drawHeight = -1, int drawStartX = -1, int drawStartY = -1);
	void RenderFrame(HDC hdc, int startX, int startY, int frameIndex);

	void Release();

	inline HDC GetMemoryDC() { return (lpImageInfo) ? lpImageInfo->hMemoryDC : NULL; }
	inline int GetWidth() { return (lpImageInfo) ? lpImageInfo->width : 0; }
	inline int GetHeight() { return (lpImageInfo) ? lpImageInfo->height : 0; }
	inline int GetTotalFrames() { return totalFrames; }
};

