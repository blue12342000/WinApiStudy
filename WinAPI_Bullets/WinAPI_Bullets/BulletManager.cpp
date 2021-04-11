#include "stdafx.h"
#include "BulletManager.h"
#include "Bullet.h"
#include "SpiralBullet.h"
#include "HallucinationBullet.h"

void BulletManager::Init()
{
	vBullets.reserve(1000);
	mvDeacive.insert(make_pair(BULLET_TYPE::BASIC, vector<Bullet*>(vBullets)));
	mvDeacive.insert(make_pair(BULLET_TYPE::SPIRAL, vector<Bullet*>(vBullets)));
	mvDeacive.insert(make_pair(BULLET_TYPE::HALLUCINATION, vector<Bullet*>(vBullets)));
}

void BulletManager::Release()
{
	for (int i = 0; i < vBullets.size(); ++i)
	{
		vBullets[i]->Release();
		delete vBullets[i];
	}
	vBullets.clear();
}

void BulletManager::Update()
{
	for (int i = 0; i < vBullets.size();)
	{
		if (!vBullets[i]->IsActive())
		{
			mvDeacive[vBullets[i]->GetType()].push_back(vBullets[i]);
			vBullets.erase(vBullets.begin() + i);
		}
		else
		{
			vBullets[i]->Update();
			++i;
		}
	}
}

void BulletManager::Render(HDC hdc)
{
	for (int i = 0; i < vBullets.size(); ++i)
	{
		vBullets[i]->Render(hdc);
	}
}

void BulletManager::Fire(POINTFLOAT pos, float speed, float angle, int delay)
{
	if (mvDeacive[BULLET_TYPE::BASIC].empty())
	{
		vBullets.push_back(new Bullet());
		vBullets[vBullets.size() - 1]->Init();
	}
	else
	{
		vector<Bullet*>& vDeactive = mvDeacive[BULLET_TYPE::BASIC];
		vBullets.push_back(vDeactive[vDeactive.size() - 1]);
		vDeactive.pop_back();
	}
	if (delay > 0) vBullets[vBullets.size() - 1]->Fire(pos, speed, angle, delay);
	else vBullets[vBullets.size() - 1]->Fire(pos, speed, angle);
}

void BulletManager::SpreadFire(POINTFLOAT pos, float angle, int delay)
{
	for (int i = 0; i < 50; ++i)
	{
		if (mvDeacive[BULLET_TYPE::BASIC].empty())
		{
			vBullets.push_back(new Bullet());
			vBullets[vBullets.size() - 1]->Init();
		}
		else
		{
			vector<Bullet*>& vDeactive = mvDeacive[BULLET_TYPE::BASIC];
			vBullets.push_back(vDeactive[vDeactive.size() - 1]);
			vDeactive.pop_back();
		}
		if (delay > 0) vBullets[vBullets.size() - 1]->Fire(pos, (float)(rand() % 30) / 10.0f + 1.5f, angle + (float)((rand() % 101) - 50) / 100.0f, delay);
		else vBullets[vBullets.size() - 1]->Fire(pos, (float)(rand() % 30) / 10.0f + 1.5f, angle + (float)((rand() % 101) - 50) / 100.0f);
	}
}

void BulletManager::RoundFire(POINTFLOAT pos, float speed, int delay)
{
	for (int i = 0; i < 360; i += 10)
	{
		if (mvDeacive[BULLET_TYPE::BASIC].empty())
		{
			vBullets.push_back(new Bullet());
			vBullets[vBullets.size() - 1]->Init();
		}
		else
		{
			vector<Bullet*>& vDeactive = mvDeacive[BULLET_TYPE::BASIC];
			vBullets.push_back(vDeactive[vDeactive.size() - 1]);
			vDeactive.pop_back();
		}
		if (delay > 0) vBullets[vBullets.size() - 1]->Fire(pos, speed, 3.141582f * ((float)i / 180), delay);
		else vBullets[vBullets.size() - 1]->Fire(pos, speed, 3.141582f * ((float)i / 180));
	}
}

void BulletManager::SpiralFire(POINTFLOAT pos, float speed, bool rightRotate, int delay)
{
	for (int i = 0; i < 360; i += 30)
	{
		if (rightRotate)
		{
			if (mvDeacive[BULLET_TYPE::SPIRAL].empty())
			{
				vBullets.push_back(new SpiralBullet());
				vBullets[vBullets.size() - 1]->Init();
			}
			else
			{
				vector<Bullet*>& vDeactive = mvDeacive[BULLET_TYPE::SPIRAL];
				vBullets.push_back(vDeactive[vDeactive.size() - 1]);
				vDeactive.pop_back();
			}
			if (delay > 0) vBullets[vBullets.size() - 1]->Fire(pos, speed, 3.141582f * ((float)i / 180), delay);
			else vBullets[vBullets.size() - 1]->Fire(pos, speed, 3.141582f * ((float)i / 180));
		}
		else
		{
			if (mvDeacive[BULLET_TYPE::SPIRAL].empty())
			{
				vBullets.push_back(new SpiralBullet());
				vBullets[vBullets.size() - 1]->Init();
			}
			else
			{
				vector<Bullet*>& vDeactive = mvDeacive[BULLET_TYPE::SPIRAL];
				vBullets.push_back(vDeactive[vDeactive.size() - 1]);
				vDeactive.pop_back();
			}
			if (delay > 0) vBullets[vBullets.size() - 1]->Fire(pos, speed, -3.141582f * ((float)i / 180) - 0.000001f, delay);
			else vBullets[vBullets.size() - 1]->Fire(pos, speed, -3.141582f * ((float)i / 180) - 0.000001f);
		}
	}
}

void BulletManager::HalluFire(POINTFLOAT pos, float speed, float angle, int delay)
{
	if (mvDeacive[BULLET_TYPE::HALLUCINATION].empty())
	{
		vBullets.push_back(new HallucinationBullet());
		vBullets[vBullets.size() - 1]->Init();
	}
	else
	{
		vector<Bullet*>& vDeactive = mvDeacive[BULLET_TYPE::HALLUCINATION];
		vBullets.push_back(vDeactive[vDeactive.size() - 1]);
		vDeactive.pop_back();
	}
	((HallucinationBullet*)vBullets[vBullets.size() - 1])->SetHalluType(BULLET_TYPE::BASIC);
	((HallucinationBullet*)vBullets[vBullets.size() - 1])->SetBulletManager(this);
	if (delay > 0) vBullets[vBullets.size() - 1]->Fire(pos, speed, angle, delay);
	else vBullets[vBullets.size() - 1]->Fire(pos, speed, angle);
}

void BulletManager::HalluSprialFire(POINTFLOAT pos, float speed, float angle, int delay)
{
	if (mvDeacive[BULLET_TYPE::HALLUCINATION].empty())
	{
		vBullets.push_back(new HallucinationBullet());
		vBullets[vBullets.size() - 1]->Init();
	}
	else
	{
		vector<Bullet*>& vDeactive = mvDeacive[BULLET_TYPE::HALLUCINATION];
		vBullets.push_back(vDeactive[vDeactive.size() - 1]);
		vDeactive.pop_back();
	}
	((HallucinationBullet*)vBullets[vBullets.size() - 1])->SetHalluType(BULLET_TYPE::SPIRAL);
	((HallucinationBullet*)vBullets[vBullets.size() - 1])->SetBulletManager(this);
	if (delay > 0) vBullets[vBullets.size() - 1]->Fire(pos, speed, angle, delay);
	else vBullets[vBullets.size() - 1]->Fire(pos, speed, angle);
}
