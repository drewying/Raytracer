#include "TextureBackground.h"
#include "../Materials/BitmapTextureMaterial.h"
#include "../Vector.h"
#include "../Ray.h"

TextureBackground::TextureBackground(const std::string filename) : textureMaterial(filename) {}

TextureBackground::~TextureBackground(){}

Color TextureBackground::getBackgroundColor(const Ray &ray){

    Vector vector = ray.direction();

    double x = (vector.x()/2)+0.5;
    double y = (vector.y()/2)+0.5;

    return textureMaterial.getColorAtCoordinate(x, y);
}

