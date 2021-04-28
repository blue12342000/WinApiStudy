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

/*
* int data;
* if (data & 0xF000000)
* {
*	  // 못뚫는 벽이다
* }
* 
*/

// int 4byte 32bit
// 0001 // 숲 - 뚫고가는 환경
// 0111 // 벽돌, 철벽, 물 - 못가는 지형
// 0x0F000000 비트연산을 해서 true / false
// 0111 // 형태 템플릿x 0~15 frameX최초
// 0001	// 형태 템플릿y 0~15 frameY최초
// 0001 // 위치
// 0011	// 위치
// 0111	// 위치
// 1111	// 위치
// 0xFFFF


typedef struct tagBlockPiece
{
	bool isDestroy;
	RECT hitBox;
} BLOCK_PIECE;

typedef struct tagBlock
{
	int data;
	int remainPiece;
	int frameX;
	int frameY;
	int split;
	RECT hitBox;
	vector<BLOCK_PIECE> vPiece;
	vector<BLOCK_PIECE> vDeactivePiece;
} BLOCK_INFO;

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
	int split;
} TILE_INFO;

class Image;
class Button;
class TilemapTool : public GameNode
{
private:
	// 샘플 타일 이미지
	static Image* sampleTile;
	// 메인타일의 정보
	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	static Image* lpTileMap;
	static BLOCK_INFO blockInfo[TILE_X * TILE_Y];

	bool isDrag;
	POINT selectStartTile;
	POINT selectEndTile;

	Button* btnSave;
	Button* btnLoad;

	Button* btnStage1;
	Button* btnStage2;
	Button* btnStage3;

	static int selectedMap;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	static void Save(int* index);
	static void Load(int* index);
	static void ChangeStage(int* stage);

	static void SetBlockInfo();

	virtual ~TilemapTool() {};
};

