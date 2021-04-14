#include "Missile.h"
#include <iostream>

using namespace std;

void NormalMissle::Move()
{
	cout << "발사 시점 각도로 직선으로 날아간다." << endl;
}

void NormalMissle::Notice()
{
	cout << "일반 미사일이 생성되었다." << endl;
}

void SinMissle::Move()
{
	cout << "웨이브를 그리며 날아간다" << endl;
}

void SinMissle::Notice()
{
	cout << "싸인 미사일이 생성되었다." << endl;
}

void LazerMissle::Move()
{
	cout << "이것은 미사일이 맞는가??" << endl;
}

void LazerMissle::Notice()
{
	cout << "레이저 미사일이 생성되었다." << endl;
}
