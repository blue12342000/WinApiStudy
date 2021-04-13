#include "stdafx.h"
#include "Timer.h"

HRESULT Timer::Init()
{
    isHardware = false;
    timeScale = 0.0f;
    timeElapsed = 0.0f;
    currTime = 0;
    lastTime = 0;
    periodFrequency = 0;
    fpsTimeElapsed = 0;
    fpsFrameCount = 0;

    isHardware = QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);
    if (isHardware)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
        timeScale = 1.0f / periodFrequency;
    }
    else
    {
        lastTime = timeGetTime();
        timeScale = 0.001f;
    }

    return S_OK;
}

void Timer::Tick()
{
    // 현재 진동수 or 현재 시간을 구해서 경과시간을 계산
    if (isHardware)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    }
    else
    {
        currTime = timeGetTime();
    }
    timeElapsed = (currTime - lastTime) * timeScale;

    fpsTimeElapsed += timeElapsed;
    ++fpsFrameCount;
    if (fpsTimeElapsed >= 1.0f)
    {
        fps = fpsFrameCount;
        fpsFrameCount = 0;
        fpsTimeElapsed -= 1;
    }

    lastTime = currTime;
}
