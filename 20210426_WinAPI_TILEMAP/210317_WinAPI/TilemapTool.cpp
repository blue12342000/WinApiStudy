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

    isDrag = false;
    selectStartTile = { 0, 0 };
    selectEndTile = { 0, 0 };

    return S_OK;
}

void TilemapTool::Release()
{
}

void TilemapTool::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
    {
        isDrag = false;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
    {
        POINT mousePoint = KeyManager::GetSingleton()->GetMousePoint();
        if (mousePoint.x > TILEMAPTOOLSIZE_X - sampleTile->GetWidth() && mousePoint.x < TILEMAPTOOLSIZE_X
            && mousePoint.y > 0 && mousePoint.y < TILEMAPTOOLSIZE_Y)
        {
            if (!isDrag)
            {
                isDrag = true;
                selectStartTile.x = (mousePoint.x - (TILEMAPTOOLSIZE_X - sampleTile->GetWidth())) / TILESIZE;
                selectStartTile.y = mousePoint.y / TILESIZE;
            }
            else
            {
                selectEndTile.x = (mousePoint.x - (TILEMAPTOOLSIZE_X - sampleTile->GetWidth())) / TILESIZE;
                selectEndTile.y = mousePoint.y / TILESIZE;
                if (selectEndTile.x < 0) selectEndTile.x = 0;
                else if (selectEndTile.x >= SAMPLE_TILE_X) selectEndTile.x = SAMPLE_TILE_X - 1;
                if (selectEndTile.y < 0) selectEndTile.y = 0;
                else if (selectEndTile.y >= SAMPLE_TILE_Y) selectEndTile.y = SAMPLE_TILE_Y - 1;
            }
        }

        if (!isDrag)
        {
            for (int i = 0; i < TILE_X * TILE_Y; i++)
            {
                if (PtInRect(&tileInfo[i].rcTile, mousePoint))
                {
                    POINT currTile = {0, 0};
                    POINT deltaTile = { selectEndTile.x - selectStartTile.x, selectEndTile.y - selectStartTile.y };
                    deltaTile = { (deltaTile.x != 0)?deltaTile.x / abs(deltaTile.x):0, (deltaTile.y != 0)?deltaTile.y / abs(deltaTile.y):0 };
                    int index = i + currTile.y * SAMPLE_TILE_X + currTile.x;
                    while (abs(currTile.y) <= abs(selectEndTile.y - selectStartTile.y))
                    {
                        if (i / TILE_X + currTile.y < TILE_Y && i % TILE_X + currTile.x < TILE_X
                            && i / TILE_X + currTile.y >= 0 && i % TILE_X + currTile.x >= 0)
                        {
                            tileInfo[index].frameX = selectStartTile.x + currTile.x;
                            tileInfo[index].frameY = selectStartTile.y + currTile.y;

                            currTile.x += deltaTile.x;
                            if (currTile.x == selectEndTile.x - selectStartTile.x + deltaTile.x)
                            {
                                currTile.x = 0;
                                currTile.y += deltaTile.y;
                                if (deltaTile.y == 0) break;
                            }
                        }
                        else
                        {
                            currTile.x = 0;
                            currTile.y += deltaTile.y;
                        }

                        index = i + currTile.y * SAMPLE_TILE_X + currTile.x;
                    }
                    break;
                }
            }
        }
    }
}

void TilemapTool::Render(HDC hdc)
{
    PatBlt(hdc, 0, 0, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

    sampleTile->Render(hdc, TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), 0);

    POINT currTile = { 0, 0 };
    POINT deltaTile = { selectEndTile.x - selectStartTile.x, selectEndTile.y - selectStartTile.y };
    POINT destPoint = { TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), TILEMAPTOOLSIZE_Y / 2 };
    float scale = (deltaTile.x != 0 || deltaTile.y != 0) ? 1 : 3;
    if (deltaTile.x < 0) destPoint.x += abs(deltaTile.x) * TILESIZE * scale;
    if (deltaTile.y < 0) destPoint.y += abs(deltaTile.y) * TILESIZE * scale;
    deltaTile = { (deltaTile.x != 0) ? deltaTile.x / abs(deltaTile.x) : 0, (deltaTile.y != 0) ? deltaTile.y / abs(deltaTile.y) : 0 };
    while (abs(currTile.y) <= abs(selectEndTile.y - selectStartTile.y))
    {
        sampleTile->ScaleRender(hdc, destPoint.x + TILESIZE * scale * currTile.x, destPoint.y + TILESIZE * scale * currTile.y, selectStartTile.x + currTile.x, selectStartTile.y + currTile.y, scale);

        currTile.x += deltaTile.x;
        if (currTile.x == selectEndTile.x - selectStartTile.x + deltaTile.x)
        {
            currTile.x = 0;
            currTile.y += deltaTile.y;
            if (deltaTile.y == 0) break;
        }
    }

    for (int i = 0; i < TILE_X * TILE_Y; i++)
    {
        sampleTile->FrameRender(hdc,
            tileInfo[i].rcTile.left,
            tileInfo[i].rcTile.top,
            tileInfo[i].frameX,
            tileInfo[i].frameY);
    }
}
