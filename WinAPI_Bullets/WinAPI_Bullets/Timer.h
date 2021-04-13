#pragma once
class Timer
{
private:
	bool isHardware;
	// 클럭수에따른 1클럭당 시간을 구함
	float timeScale;
	// 시간의 경과형
	float timeElapsed;
	// 현재시간 ( 고성능 타이머 진동수 )
	__int64 currTime;
	// 이전시간 ( 고성능 타이머 진동수 )
	__int64 lastTime;
	// 고성능 타이머 주파수 (초당 진동수)
	__int64 periodFrequency;

	float fpsTimeElapsed;
	unsigned long fpsFrameCount;
	unsigned long fps;

public:
	HRESULT Init();
	void Tick();

	inline unsigned long GetFps() { return fps; }
	inline float GetDeltaTime() { return timeElapsed; }
};

