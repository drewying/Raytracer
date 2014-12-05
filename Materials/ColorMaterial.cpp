#include <iostream>
#include "ColorMaterial.h"
#include "../HitRecord.h"
#include "../Lights/Light.h"
#include "../Shapes/Shape.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../MathFunc.h"
#include "../Sample.h"

ColorMaterial::ColorMaterial(const Color &color) : color(color){}

ColorMaterial::~ColorMaterial() {}

Color ColorMaterial::getColor(const HitRecord &hit, double time) const{
    return color;
}