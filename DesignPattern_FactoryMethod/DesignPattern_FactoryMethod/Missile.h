#pragma once
#include <iostream>

class Missile
{
public:
	Missile() {}
	virtual ~Missile() { std::cout << "Missile ¼Ò¸êÀÚ" << std::endl; }

	virtual void Move() = 0;
	virtual void Notice() = 0;
};

class NormalMissle : public Missile
{
private:
	virtual void Move() override;
	virtual void Notice() override;

public:
	NormalMissle() {}
	virtual ~NormalMissle() { std::cout << "NormalMissile ¼Ò¸êÀÚ" << std::endl; }

};

class SinMissle : public Missile
{
private:
	virtual void Move() override;
	virtual void Notice() override;

public:
	SinMissle() {}
	virtual ~SinMissle() { std::cout << "SinMissle ¼Ò¸êÀÚ" << std::endl; }

};

class LazerMissle : public Missile
{
private:
	virtual void Move() override;
	virtual void Notice() override;

public:
	LazerMissle() {}
	virtual ~LazerMissle() { std::cout << "LazerMissle ¼Ò¸êÀÚ" << std::endl; }

};