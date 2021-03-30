#include "Config.h"
#include "KeyManager.h"

KeyManager::KeyManager()
{
}


KeyManager::~KeyManager()
{
}

HRESULT KeyManager::Init()
{
	keyUp.set();
	keyDown.reset();
	isFocusWindow = false;
	return E_NOTIMPL;
}

void KeyManager::Update()
{
	isFocusWindow = GetForegroundWindow() == g_hWnd;
}

bool KeyManager::IsOnceKeyUp(int key)
{
	if (!isFocusWindow) return false;
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

bool KeyManager::IsOnceKeyDown(int key)
{
	if (!isFocusWindow) return false;
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

bool KeyManager::IsStayKeyDown(int key)
{
	if (!isFocusWindow) return false;
	if (GetAsyncKeyState(key) & 0x8000)
	{
		return true;
	}

	return false;
}
