#include "BitmapTextureMaterial.h"
#include "../Sample.h"
#include "../Lights/Light.h"
#include "../Matrix.h"
#include "../Ray.h"
#include "../HitRecord.h"
#include "../Shapes/Shape.h"

#include <fstream>
#include <iostream>

BitmapTextureMaterial::BitmapTextureMaterial(const std::string filename) {
    loadBitmap(filename.c_str());
}

BitmapTextureMaterial::~BitmapTextureMaterial() {}

Color BitmapTextureMaterial::getColor(const HitRecord &hit, double time) const{
    Ray ray = hit.getRay();
    Vector hitpos = ray.origin() + ray.direction() * hit.minT();
    Vector uv = hit.getShape()->get2DVector(hitpos, time);

    double x = uv.x();
    double y = uv.y();
    return getColorAtCoordinate(x, y);
}

Color BitmapTextureMaterial::getColorAtCoordinate(double x, double y) const{

    int col = int(x*textureWidth);
    int row = int(y*textureHeight);

    row*=3;
    col*=3;

    int r = textureData[textureWidth * row + col-1];
    int g = textureData[textureWidth * row + col-2];
    int b = textureData[textureWidth * row + col-3];

    return Color((double)r/255.0, (double)g/255.0, (double)b/255.0);
}

void BitmapTextureMaterial::loadBitmap(const char* FilePath) {
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

    hFile.seekg(0, std::ios::end);
    int Length = hFile.tellg();
    hFile.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> FileInfo(Length);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

    if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
    }

    if (FileInfo[28] != 24)
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. 24 bit Image Required.");
    }

    bitsPerPixel = FileInfo[28];
    textureWidth = FileInfo[18] + (FileInfo[19] << 8);
    textureHeight = FileInfo[22] + (FileInfo[23] << 8);
    std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
    std::uint32_t size = ((textureWidth * bitsPerPixel + 31) / 32) * 4 * textureHeight;
    textureData.resize(size);

    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(textureData.data()), size);
    hFile.close();
}