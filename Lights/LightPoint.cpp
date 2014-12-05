#include "LightPoint.h"

LightPoint::LightPoint(const Vector &position, const Color &color)
        : position(position), color(color) {
}

LightPoint::~LightPoint() {
}

double LightPoint::getLight(Color &light_color, Vector &light_direction, const Vector &hitpos) const {
    light_color = color;
    Vector dir = position-hitpos; //position - hitpos;
    double len = dir.normalize();
    light_direction = dir;
    return len;
}
