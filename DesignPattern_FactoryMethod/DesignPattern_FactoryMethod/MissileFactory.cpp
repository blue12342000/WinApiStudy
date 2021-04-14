#include "MissileFactory.h"
#include "Missile.h"

MissileFactory::~MissileFactory()
{
	for (auto p : vMissileDatas) delete p;
	std::cout << "MissileFactory ¼Ò¸êÀÚ" << std::endl;
}

void MissileFactory::AddMissile()
{
	Missile* m = CreateMissile();
	vMissileDatas.push_back(m);
	m->Notice();
}

Missile* NormalMissileFactory::CreateMissile()
{

	return new NormalMissle();
}

Missile* SinMissileFactory::CreateMissile()
{
	return new SinMissle();
}

Missile* LazerMissileFactory::CreateMissile()
{
	return new LazerMissle();
}
