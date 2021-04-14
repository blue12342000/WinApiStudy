#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Missile;
class MissileFactory
{
private:
	vector<Missile*> vMissileDatas;

public:
	MissileFactory() {}
	virtual ~MissileFactory();

	virtual void AddMissile() final;

private:
	virtual Missile* CreateMissile() = 0;
};

class NormalMissileFactory : public MissileFactory
{
private:
	virtual Missile* CreateMissile() override;

public:
	NormalMissileFactory() {}
	~NormalMissileFactory() { std::cout << "NormalMissileFactory ¼Ò¸êÀÚ" << std::endl; }
};

class SinMissileFactory : public MissileFactory
{
private:
	virtual Missile* CreateMissile() override;

public:
	SinMissileFactory() {}
	~SinMissileFactory() { std::cout << "SinMissileFactory ¼Ò¸êÀÚ" << std::endl; }
};

class LazerMissileFactory : public MissileFactory
{
private:
	virtual Missile* CreateMissile() override;

public:
	LazerMissileFactory() {}
	~LazerMissileFactory() { std::cout << "LazerMissileFactory ¼Ò¸êÀÚ" << std::endl; }
};