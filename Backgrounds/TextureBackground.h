#ifndef TextureBackground_h
#define TextureBackground_h

#include "Background.h"
#include "../Color.h"
#include "../Materials/BitmapTextureMaterial.h"

class TextureBackground : public Background {
public:
    TextureBackground(const std::string filename);

    virtual ~TextureBackground();

    virtual Color getBackgroundColor(const Ray &ray);

private:
    BitmapTextureMaterial textureMaterial;

};

#endif

