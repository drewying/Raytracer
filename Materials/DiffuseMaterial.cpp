#include <iostream>
#include <MacTypes.h>
#include "DiffuseMaterial.h"
#include "../HitRecord.h"
#include "../Lights/Light.h"
#include "../Shapes/Shape.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../MathFunc.h"
#include "../Sample.h"

DiffuseMaterial::DiffuseMaterial() {}

DiffuseMaterial::~DiffuseMaterial() {}

Color DiffuseMaterial::materialColor(const HitRecord &hit, const double time) const{
    return getColor(hit, time);
}

Color DiffuseMaterial::emmitColor(const HitRecord &hit, const double time) const{
    return Color(0.0, 0.0, 0.0);
}

Color DiffuseMaterial::getColor(const HitRecord &hit, double time) const{
    return Color(1.0,1.0,1.0);
}

Ray DiffuseMaterial::bounce(const HitRecord &hit, double time) const{
    static std::random_device rd;
    static std::default_random_engine generator(rd());
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    double pi = M_PI;
    double phi = 2 * pi * distribution(generator);
    double r = sqrt(distribution(generator));
    double x = r * cos(phi);
    double y = r * sin(phi);
    double z = sqrt(1 - x * x - y * y);
    Vector randomVector = Vector(x,y,z);

    randomVector.normalize();

    auto normal = hit.getNormal();

    // if the point is in the wrong hemisphere, mirror it
    if (Dot(normal, randomVector) < 0.0) {
        randomVector *= -1.0;
    }

    return Ray(hit.getHitPosition(), randomVector);
}