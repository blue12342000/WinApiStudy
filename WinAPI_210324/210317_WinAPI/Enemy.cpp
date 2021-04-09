#include "Enemy.h"
#include "Tank.h"
#include "Image.h"

HRESULT Enemy::Init()
{
	hp = 10;
	hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
	hBrushRed = CreateSolidBrush(RGB(255, 0, 0));

	image = new Image();
	if (FAILED(image->Init("Image/ufo.bmp", 530, 32, 10, 1, 10, true, RGB(255, 0, 255))))
	{
		MessageBox(g_hWnd, "ufo.bmp 이미지로드 실패", "이미지 로드 실패", MB_OK);
		image->Release();
		image = nullptr;
	}

	size = 32;

	elapsedTime = 0;
	frame = 0;

	return S_OK;
}

void Enemy::Update()
{
	if (state == Enemy::EnemyState::DEAD) return;

	if (++elapsedTime % 10 == 0)
	{
		++frame %= 10;
	}

	if (tankPtr)
	{
		POINTFLOAT delta = { tankPtr->GetPoint().x - pos.x , tankPtr->GetPoint().y - pos.y };
		angle = atan2f(delta.y, delta.x);

		pos.x += cosf(angle) * speed;
		pos.y += sinf(angle) * speed;
	}
	else
	{
		if (move == EnemyMove::EM_LEFT)
		{
			pos.x -= 1;
			if (pos.x < (WINSIZE_X / 2 - 100))
			{
				move = EnemyMove::EM_RIGHT;
			}
		}
		else
		{
			pos.x += 1;
			if (pos.x > (WINSIZE_X / 2 + 100))
			{
				move = EnemyMove::EM_LEFT;
			}
		}
	}

	rc.left = pos.x - size / 2;
	rc.right = pos.x + size / 2;
	rc.top = pos.y - size / 2;
	rc.bottom = pos.y + size / 2;
}

void Enemy::Render(HDC hdc)
{
	if (state == Enemy::EnemyState::ALIVE)
	{
		RenderEllipse(hdc, rc);
		image->RenderFrame(hdc, pos.x, pos.y, frame);
		//if (hp > 2) RenderRectangle(hdc, rc, hBrushBlack);
		//else if (hp > 1) RenderRectangle(hdc, rc, hBrushRed);
		//else RenderRectangle(hdc, rc);
	}
}

void Enemy::Release()
{
	if (hBrushBlack == NULL) DeleteObject(hBrushBlack);
	if (hBrushRed == NULL) DeleteObject(hBrushRed);

	if (image)
	{
		image->Release();
		delete image;
		image = nullptr;
	}
}

void Enemy::HitDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		state = Enemy::EnemyState::DEAD;
	}
}
