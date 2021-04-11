#include "stdafx.h"
#include "KeyManager.h"

HRESULT KeyManager::Init()
{
    keyUp.set();
    keyDown.set();

    return S_OK;
}

void KeyManager::Release()
{
}

bool KeyManager::IsOnceKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        if (!keyDown[key])
        {
            keyDown[key] = true;
            return true;
        }
    }
    else
    {
        keyDown[key] = false;
    }

    return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        keyUp[key] = false;
    }
    else
    {
        if (!keyUp[key])
        {
            keyUp[key] = true;
            return true;
        }
    }

    return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        return true;
    }
    return false;
}
