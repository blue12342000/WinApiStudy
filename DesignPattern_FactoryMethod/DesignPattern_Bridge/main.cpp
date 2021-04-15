#include <iostream>

using namespace std;

class Pattern
{
public:
	virtual void Move() = 0;
};

class StraightPattern : public Pattern
{
public:
	virtual void Move()
	{
		cout << "직선을 그리며 날아간다" << endl;
	}
};

class SignPattern : public Pattern
{
public:
	virtual void Move()
	{
		cout << "사인곡선을 그리며 날아간다" << endl;
	}
};

class Missile
{
protected:
	Pattern* pattern;

public:
	Missile():pattern(nullptr) {}
	Missile(Pattern* pattern):pattern(pattern) {}
	virtual ~Missile() {}

	virtual void Move() = 0;
};

class SmallMissile : public Missile
{
public:
	SmallMissile() {}
	SmallMissile(Pattern* pattern) :Missile(pattern) {}
	virtual ~SmallMissile() {}

	virtual void Move() override
	{
		cout << "작은 미사일 : ";
		if (pattern) pattern->Move();
	}
};

class BigMissile : public Missile
{
public:
	BigMissile() {}
	BigMissile(Pattern* pattern) :Missile(pattern) {}
	virtual ~BigMissile() {}

	virtual void Move() override
	{
		cout << "큰 미사일 : ";
		if (pattern) pattern->Move();
	}
};

int main()
{
	Missile* small = new SmallMissile(new StraightPattern());
	Missile* big = new BigMissile(new SignPattern());

	small->Move();
	big->Move();

	delete small;
	delete big;

	return 0;
}