#include "BonneJenet.h"
#include "Image.h"

HRESULT BonneJenet::Init()
{
	width = 64 * 2;
	height = 112 * 2;

	fileName[(int)CharaterState::CS_IDLE] = "Image/The King of Fighters 11/Bonne Jenet/Bonne Jenet_idle_bmp.bmp";
	fileName[(int)CharaterState::CS_MOVE] = "Image/The King of Fighters 11/Bonne Jenet/Bonne Jenet_move_bmp.bmp";
	img = new Image[(int)CharaterState::CS_END];
	if (FAILED(img[(int)CharaterState::CS_IDLE].Init(fileName[(int)CharaterState::CS_IDLE], width * 16, height, 16, 1)))
	{
		MessageBox(g_hWnd, "IDLE 캐릭터이미지 로드에 실패하였습니다.", "Error", MB_OK);
	}
	if (FAILED(img[(int)CharaterState::CS_MOVE].Init(fileName[(int)CharaterState::CS_MOVE], width * 14, height, 14, 1)))
	{
		MessageBox(g_hWnd, "MOVE 캐릭터이미지 로드에 실패하였습니다.", "Error", MB_OK);
	}

	elapedTime = 0;
	frame = 0;

	pos.x = 0;
	pos.y = 0;

	return E_NOTIMPL;
}

void BonneJenet::Update()
{
	state = CharaterState::CS_IDLE;
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		state = CharaterState::CS_MOVE;
		pos.x -= 10;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		state = CharaterState::CS_MOVE;
		pos.x += 10;
	}
	//if (KeyManager::GetInstance()->IsStayKeyDown('W'))
	//{
	//	state = CharaterState::CS_MOVE;
	//}
	//f (KeyManager::GetInstance()->IsStayKeyDown('S'))
	//
	//	state = CharaterState::CS_MOVE;
	//

	Charactor::Update();
}