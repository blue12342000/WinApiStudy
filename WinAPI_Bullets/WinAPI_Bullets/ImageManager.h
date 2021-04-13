#pragma once
#include "Singleton.h"
#include <map>

using namespace std;

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<string, Image*> mImage;

public:
	HRESULT Init();
	void Release();
	
	Image* FindImage(string key);
};

