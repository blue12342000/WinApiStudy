#include "stdafx.h"
#include "TimerManager.h"
#include "Timer.h"

HRESULT TimerManager::Init()
{
    timer = new Timer();
    timer->Init();



    return S_OK;
}

void TimerManager::Release()
{
    SAFE_DELETE(timer);
}

void TimerManager::Update()
{
    if (timer)
    {
        timer->Tick();
    }
}

void TimerManager::Render(HDC hdc)
{
    if (timer)
    {
        wsprintf(str, "FPS: %d", timer->GetFps());
        TextOut(hdc, 10, 10, str, strlen(str));
    }
}

unsigned int TimerManager::GetFps()
{
    if (timer)
    {
        return timer->GetFps();
    }
    return 0;
}

float TimerManager::GetDeltaTime()
{
    if (timer)
    {
        return timer->GetDeltaTime();
    }
    return 0.0f;
}
