#include <iostream>
#include "MissileFactory.h"

using namespace std;

class AAA
{
public:
	virtual void print() {}
};

struct aaa : public AAA
{
	int a;

	void print()
	{
		__super::print();
	}
};

void Test()
{
	MissileFactory* mf1 = new NormalMissileFactory();
	MissileFactory* mf2 = new SinMissileFactory();
	MissileFactory* mf3 = new LazerMissileFactory();
	mf1->AddMissile();
	mf2->AddMissile();
	mf3->AddMissile();
	delete mf1;
	delete mf2;
	delete mf3;
}

int main()
{
	Test();

	aaa a;

	return 0;
}