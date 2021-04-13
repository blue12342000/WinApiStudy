#pragma once
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;
	string text;
	char str[128];

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	unsigned int GetFps();
	float GetDeltaTime();
};

