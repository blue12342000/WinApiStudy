#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"

HRESULT TilemapTool::Init()
{
    SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

    sampleTile = ImageManager::GetSingleton()->AddImage(
        "ª˘«√≈∏¿œ", "Image/maptiles.bmp", 640, 288,
        SAMPLE_TILE_X, SAMPLE_TILE_Y);

    for (int i = 0; i < TILE_Y; i++)
    {
        for (int j = 0; j < TILE_X; j++)
        {
            tileInfo[i * TILE_X + j].frameX = 3;
            tileInfo[i * TILE_X + j].frameY = 0;

            tileInfo[i * TILE_X + j].rcTile.left = TILESIZE * j;
            tileInfo[i * TILE_X + j].rcTile.top = TILESIZE * i;
            tileInfo[i * TILE_X + j].rcTile.right = 
                tileInfo[i * TILE_X + j].rcTile.left + TILESIZE;
            tileInfo[i * TILE_X + j].rcTile.bottom = 
                tileInfo[i * TILE_X + j].rcTile.top + TILESIZE;
        }
    }

    return S_OK;
}

void TilemapTool::Release()
{
}

void TilemapTool::Update()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
    {
        POINT mousePoint = KeyManager::GetSingleton()->GetMousePoint();
        if (mousePoint.x > TILEMAPTOOLSIZE_X - sampleTile->GetWidth() && mousePoint.x < TILEMAPTOOLSIZE_X
            && mousePoint.y > 0 && mousePoint.y < TILEMAPTOOLSIZE_Y)
        {
            selectTile.x = (mousePoint.x - (TILEMAPTOOLSIZE_X - sampleTile->GetWidth())) / TILESIZE;
            selectTile.y = mousePoint.y / TILESIZE;
        }

        for (int i = 0; i < TILE_X * TILE_Y; i++)
        {
            if (PtInRect(&tileInfo[i].rcTile, mousePoint))
            {
                tileInfo[i].frameX = selectTile.x;
                tileInfo[i].frameY = selectTile.y;
                break;
            }
        }
    }
}

void TilemapTool::Render(HDC hdc)
{
    PatBlt(hdc, 0, 0,
        TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

    sampleTile->Render(hdc, TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), 0);

    sampleTile->FrameRender(hdc, TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), TILEMAPTOOLSIZE_Y / 2, selectTile.x, selectTile.y);

    for (int i = 0; i < TILE_X * TILE_Y; i++)
    {
        sampleTile->FrameRender(hdc,
            tileInfo[i].rcTile.left,
            tileInfo[i].rcTile.top,
            tileInfo[i].frameX,
            tileInfo[i].frameY);
    }
}
