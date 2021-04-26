#pragma once
#include "GameNode.h"

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9
#define TILESIZE		32

// 메인 그리기 공간 정보
#define	TILE_X			20
#define	TILE_Y			20

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
} TILE_INFO;

class Image;
class TilemapTool : public GameNode
{
private:
	// 샘플 타일 이미지
	Image* sampleTile;

	// 메인타일의 정보
	TILE_INFO tileInfo[TILE_X * TILE_Y];

	POINT selectTile;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~TilemapTool() {};
};

