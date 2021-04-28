#include "Button.h"
#include "Image.h"
#include "TilemapTool.h"

HRESULT Button::Init(const char* fileName, int x, int y)
{
    state = ButtonState::NONE;
    img = ImageManager::GetSingleton()->FindImage(fileName);
    if (img == nullptr)
    {
        string warning = string(fileName) + "가 없다";
        MessageBox(g_hWnd, warning.c_str(), "WARNING", MB_OK);
    }
    pos = { x, y };
    SetRect(&rc, x- img->GetFrameWidth() / 2, y - img->GetFrameHeight() / 2, x + img->GetFrameWidth() / 2, y + img->GetFrameHeight() / 2);

    func = nullptr;
    lpFuncArg = nullptr;

    return S_OK;
}

HRESULT Button::Init()
{
    return E_NOTIMPL;
}

void Button::Release()
{
}

void Button::Update()
{
    if (PtInRect(&rc, KeyManager::GetSingleton()->GetMousePoint())
        && KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
    {
        state = ButtonState::DOWN;
    }
    else
    {
        if (state == ButtonState::DOWN)
        {
            // 눌렸었다
            if (func)
            {
                (lpFuncArg) ? func(lpFuncArg) : func(&funcArg);
            }
        }
        state = ButtonState::UP;
    }
}

void Button::Render(HDC hdc)
{
    switch (state)
    {
    case ButtonState::UP:
        img->FrameRender(hdc, rc.left, rc.top, 0, 1);
        break;
    case ButtonState::DOWN:
        img->FrameRender(hdc, rc.left, rc.top, 0, 0);
        break;
    }
}
