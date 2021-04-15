#include "stdafx.h"
#include "ImageManager.h"
#include "Image.h"

HRESULT ImageManager::Init()
{
    mImage.insert(make_pair("ENEMY", new Image()));
    mImage["ENEMY"]->Init("Image/ufo.bmp", 530, 32, 10, 1, 10, true);
    mImage.insert(make_pair("BULLET", new Image()));
    mImage["BULLET"]->Init("Image/bullet.bmp", 21, 21, 1, 1, 1, true);
    mImage.insert(make_pair("ROCKET", new Image()));
    mImage["ROCKET"]->Init("Image/rocket.bmp", 52, 64, 1, 1, 1, true);
    return S_OK;
}

void ImageManager::Release()
{
    for (auto& pair : mImage)
    {
        pair.second->Release();
        delete pair.second;
    }
    mImage.clear();
}

Image* ImageManager::FindImage(string key)
{
    Image* image = nullptr;
    if (mImage.find(key) != mImage.end())
    {
        return mImage[key];
    }

    return image;
}
