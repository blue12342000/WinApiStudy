#include "ChLasswellKing.h"
#include "Image.h"

HRESULT ChLasswellKing::Init()
{
	width = 111 * 2;
	height = 83 * 2;
	
	fileName[(int)CharaterState::CS_IDLE] = "Image/Lasswell_Hess_King/Lasswell(Hess_King)/Lasswell King (5)/unit_idle_100023005.bmp";
	fileName[(int)CharaterState::CS_ATK] = "Image/Lasswell_Hess_King/Lasswell(Hess_King)/Lasswell King (5)/unit_atk_100023005.bmp";
	img = new Image[(int)CharaterState::CS_END];
	if (FAILED(img[(int)CharaterState::CS_IDLE].Init(fileName[(int)CharaterState::CS_IDLE], width * 3, height * 4, 3, 4)))
	{
		MessageBox(g_hWnd, "IDLE 캐릭터이미지 로드에 실패하였습니다.", "Error", MB_OK);
	}
	if (FAILED(img[(int)CharaterState::CS_ATK].Init(fileName[(int)CharaterState::CS_ATK], width * 3, height * 8, 3, 8)))
	{
		MessageBox(g_hWnd, "ATK 캐릭터이미지 로드에 실패하였습니다.", "Error", MB_OK);
	}

	elapedTime = 0;
	frame = 0;

	pos.x = 0;
	pos.y = 0;

	return E_NOTIMPL;
}

void ChLasswellKing::Update()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		state = CharaterState::CS_ATK;
		frame = 0;
		elapedTime = 0;
	}

	Charactor::Update();
}
