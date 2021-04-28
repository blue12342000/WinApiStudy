#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Button.h"

Image* TilemapTool::sampleTile = nullptr;
TILE_INFO TilemapTool::tileInfo[TILE_X * TILE_Y];
Image* TilemapTool::lpTileMap = nullptr;
BLOCK_INFO TilemapTool::blockInfo[TILE_X * TILE_Y];
int TilemapTool::selectedMap = 1;

HRESULT TilemapTool::Init()
{
    SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

    sampleTile = ImageManager::GetSingleton()->AddImage("샘플타일", "Image/maptiles.bmp", SAMPLE_TILE_X * TILESIZE, SAMPLE_TILE_Y * TILESIZE, SAMPLE_TILE_X, SAMPLE_TILE_Y);

    for (int i = 0; i < TILE_Y; i++)
    {
        for (int j = 0; j < TILE_X; j++)
        {
            tileInfo[i * TILE_X + j].frameX = 3;
            tileInfo[i * TILE_X + j].frameY = 0;
            tileInfo[i * TILE_X + j].rcTile.left = TILESIZE * j;
            tileInfo[i * TILE_X + j].rcTile.top = TILESIZE * i;
            tileInfo[i * TILE_X + j].rcTile.right = tileInfo[i * TILE_X + j].rcTile.left + TILESIZE;
            tileInfo[i * TILE_X + j].rcTile.bottom = tileInfo[i * TILE_X + j].rcTile.top + TILESIZE;
        }
    }
    // KEY : _TIMEMAP_20_20
    // == "_TILEMAP_" + to_string(TILE_X) + "_" + to_string(TILE_X)
    lpTileMap = ImageManager::GetSingleton()->AddImage("_TILEMAP_" + to_string(TILE_X) + "_" + to_string(TILE_X), TILESIZE * TILE_X, TILESIZE * TILE_Y);

    isDrag = false;
    selectStartTile = { 0, 0 };
    selectEndTile = { 0, 0 };

    ImageManager::GetSingleton()->AddImage("저장버튼", "Image/Save_Button.bmp", 60 * 2, 56 * 2 , 1 , 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("불러오기버튼", "Image/Load_Button.bmp", 60 * 2, 56 * 2, 1, 2, true, RGB(255, 0, 255));

    ImageManager::GetSingleton()->AddImage("스테이지1버튼", "Image/Stage1_Button.bmp", 92 * 2, 56 * 2, 1, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("스테이지2버튼", "Image/Stage2_Button.bmp", 92 * 2, 56 * 2, 1, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("스테이지3버튼", "Image/Stage3_Button.bmp", 92 * 2, 56 * 2, 1, 2, true, RGB(255, 0, 255));

    btnSave = new Button();
    btnSave->Init("저장버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), TILEMAPTOOLSIZE_Y - 200);
    btnSave->SetFunction(TilemapTool::Save, &selectedMap);

    btnLoad = new Button();
    btnLoad->Init("불러오기버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 170, TILEMAPTOOLSIZE_Y - 200);
    btnLoad->SetFunction(TilemapTool::Load, &selectedMap);

    btnStage1 = new Button();
    btnStage1->Init("스테이지1버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), TILEMAPTOOLSIZE_Y - 100);
    btnStage1->SetFunction(TilemapTool::ChangeStage, 1);

    btnStage2 = new Button();
    btnStage2->Init("스테이지2버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 170, TILEMAPTOOLSIZE_Y - 100);
    btnStage2->SetFunction(TilemapTool::ChangeStage, 2);

    btnStage3 = new Button();
    btnStage3->Init("스테이지3버튼", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 340, TILEMAPTOOLSIZE_Y - 100);
    btnStage3->SetFunction(TilemapTool::ChangeStage, 3);

    Load(&selectedMap);

    return S_OK;
}

void TilemapTool::Release()
{
    btnSave->Release();
    delete btnSave;

    btnLoad->Release();
    delete btnLoad;

    SAFE_RELEASE(btnStage1);
    SAFE_RELEASE(btnStage2);
    SAFE_RELEASE(btnStage3);
}

void TilemapTool::Update()
{
    if (btnSave) btnSave->Update();
    if (btnLoad) btnLoad->Update();
    if (btnStage1) btnStage1->Update();
    if (btnStage2) btnStage2->Update();
    if (btnStage3) btnStage3->Update();

    if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
    {
        isDrag = false;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
    {
        POINT mousePoint = KeyManager::GetSingleton()->GetMousePoint();
        if (mousePoint.x > TILEMAPTOOLSIZE_X - sampleTile->GetWidth() && mousePoint.x < TILEMAPTOOLSIZE_X
            && mousePoint.y > 0 && mousePoint.y < sampleTile->GetHeight())
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

                            sampleTile->FrameRender(lpTileMap->GetMemDC(),
                                tileInfo[index].rcTile.left,
                                tileInfo[index].rcTile.top,
                                tileInfo[index].frameX,
                                tileInfo[index].frameY);

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
                            if (deltaTile.y == 0) break;
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

    //POINT currTile = { 0, 0 };
    //POINT deltaTile = { selectEndTile.x - selectStartTile.x, selectEndTile.y - selectStartTile.y };
    //POINT destPoint = { TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), TILEMAPTOOLSIZE_Y / 2 };
    //float scale = (deltaTile.x != 0 || deltaTile.y != 0) ? 1 : 3;
    //if (deltaTile.x < 0) destPoint.x += abs(deltaTile.x) * TILESIZE * scale;
    //if (deltaTile.y < 0) destPoint.y += abs(deltaTile.y) * TILESIZE * scale;
    //deltaTile = { (deltaTile.x != 0) ? deltaTile.x / abs(deltaTile.x) : 0, (deltaTile.y != 0) ? deltaTile.y / abs(deltaTile.y) : 0 };
    //while (abs(currTile.y) <= abs(selectEndTile.y - selectStartTile.y))
    //{
    //    sampleTile->ScaleRender(hdc, destPoint.x + TILESIZE * scale * currTile.x, destPoint.y + TILESIZE * scale * currTile.y, selectStartTile.x + currTile.x, selectStartTile.y + currTile.y, scale);
    //
    //    currTile.x += deltaTile.x;
    //    if (currTile.x == selectEndTile.x - selectStartTile.x + deltaTile.x)
    //    {
    //        currTile.x = 0;
    //        currTile.y += deltaTile.y;
    //        if (deltaTile.y == 0) break;
    //    }
    //}

    lpTileMap->Render(hdc);
    for (int i = 0; i < TILE_X * TILE_Y; i++)
    {
        //sampleTile->FrameRender(hdc,
        //    tileInfo[i].rcTile.left,
        //    tileInfo[i].rcTile.top,
        //    tileInfo[i].frameX,
        //    tileInfo[i].frameY);

        if (i < blockInfo[i].hitBox.left || i > blockInfo[i].hitBox.right || blockInfo[i].hitBox.bottom)
        {

        }

        //Rectangle(hdc, blockInfo[i].hitBox.left, blockInfo[i].hitBox.top, blockInfo[i].hitBox.right, blockInfo[i].hitBox.bottom);
        //for (auto p : blockInfo[i].vPiece)
        //{
        //    Rectangle(hdc, blockInfo[i].hitBox.left + p.hitBox.left, blockInfo[i].hitBox.top + p.hitBox.top, blockInfo[i].hitBox.left + p.hitBox.right, blockInfo[i].hitBox.top + p.hitBox.bottom);
        //}
    }


    btnSave->Render(hdc);
    btnLoad->Render(hdc);
    if (btnStage1) btnStage1->Render(hdc);
    if (btnStage2) btnStage2->Render(hdc);
    if (btnStage3) btnStage3->Render(hdc);
}

void TilemapTool::Save(int* index)
{
    string filePath = "Save/saveMapData" + to_string(*index) + ".map";
    DWORD writtenBytes;
    HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_WRITE, 0, 0, /*파일이 없을때*/CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    WriteFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &writtenBytes, 0);
    CloseHandle(hFile);
}

void TilemapTool::Load(int* index)
{
    string filePath = "Save/saveMapData" + to_string(*index) + ".map";
    DWORD readBytes;
    HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, 0, /*파일이 없을때*/OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (!ReadFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &readBytes, 0))
    {
        //MessageBox(g_hWnd, "파일로드 실패", "에러", MB_OK);
    }
    CloseHandle(hFile);

    SetBlockInfo();
}

void TilemapTool::ChangeStage(int* stage)
{
    selectedMap = *stage;
    string text = "스테이지" + to_string(selectedMap) + "로 변경되었습니다.";

    for (int i = 0; i < TILE_X * TILE_Y; i++)
    {
        tileInfo[i].frameX = 0;
        tileInfo[i].frameY = 0;
    }

    MessageBox(g_hWnd, text.c_str(), "스테이지 변경", MB_OK);
}

void TilemapTool::SetBlockInfo()
{
    HDC hdc = lpTileMap->GetMemDC();

    int index, bIndex;
    for (int y = 0; y < TILE_Y; ++y)
    {
        for (int x = 0; x < TILE_X; ++x)
        {
            index = y * TILE_X + x;
            blockInfo[index].hitBox = tileInfo[index].rcTile;
            blockInfo[index].frameX = tileInfo[index].frameX;
            blockInfo[index].frameY = tileInfo[index].frameY;
            if (blockInfo[index].frameY == 1)
            {
                blockInfo[index].split = 2;
                blockInfo[index].vPiece.resize(4);
            }
            else if (blockInfo[index].frameY == 2)
            {
                blockInfo[index].split = 4;
                blockInfo[index].vPiece.resize(16);
            }
            else
            {
                blockInfo[index].split = 1;
                blockInfo[index].vPiece.resize(1);
            }

            sampleTile->FrameRender(hdc, tileInfo[index].rcTile.left, tileInfo[index].rcTile.top, tileInfo[index].frameX, tileInfo[index].frameY);

            int size = TILESIZE / ((int)(sqrt(blockInfo[index].vPiece.size()) + DBL_EPSILON));
            for (int by = 0; by < blockInfo[index].split; ++by)
            {
                for (int bx = 0; bx < blockInfo[index].split; ++bx)
                {
                    bIndex = by * blockInfo[index].split + bx;
                    SetRect(&blockInfo[index].vPiece[bIndex].hitBox, size * bx, size * by, size * (bx + 1), size * (by + 1));
                }
            }
        }
    }
}
