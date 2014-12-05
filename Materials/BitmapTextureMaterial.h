#ifndef BitmapTextureMaterial_h
#define BitmapTextureMaterial_h

#include "DiffuseMaterial.h"
#include "../Color.h"
#include <string>
#include <vector>

class BitmapTextureMaterial : public DiffuseMaterial {
friend class TextureBackground;
public:
    BitmapTextureMaterial(const std::string filename);

    virtual ~BitmapTextureMaterial();

    virtual Color getColor(const HitRecord &hit, double time) const;

    virtual Color getColorAtCoordinate(double x, double y) const;

private:
    BitmapTextureMaterial(const BitmapTextureMaterial &);

    BitmapTextureMaterial &operator=(const BitmapTextureMaterial &);

    void loadBitmap(const char* FilePath);

    int textureHeight;

    int textureWidth;

    std::vector<unsigned char> textureData;

    int bitsPerPixel;
};

#endif
