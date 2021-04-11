#pragma once
#include "Singleton.h"
#include <bitset>

class KeyManager : public Singleton<KeyManager>
{
private:
	bitset<256> keyDown;
	bitset<256> keyUp;

public:
	HRESULT Init();
	void Release();

	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);
	bool IsStayKeyDown(int key);
};

