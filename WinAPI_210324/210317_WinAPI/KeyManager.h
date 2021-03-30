#pragma once
#include "Singleton.h"
#include <bitset>
#include <Windows.h>

#define MAX_VK_KEY 0xFF

class KeyManager : public Singleton<KeyManager>
{
private:
	std::bitset<MAX_VK_KEY> keyUp;
	std::bitset<MAX_VK_KEY> keyDown;

	bool isFocusWindow;

public:
	KeyManager();
	~KeyManager();

	HRESULT Init();
	void Update();

	bool IsOnceKeyUp(int key);
	bool IsOnceKeyDown(int key);
	bool IsStayKeyDown(int key);

	void SetKeyUp(int key, bool state) { keyUp.set(key, state); }
	void SetKeyDown(int key, bool state) { keyDown.set(key, state); }
};

