#include <iostream>
#include "LightMaterial.h"
#include "../HitRecord.h"
#include "../Lights/Light.h"
#include "../Shapes/Shape.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../MathFunc.h"
#include "../Sample.h"

LightMaterial::LightMaterial(Color lightColor){
    emit = lightColor;
}

LightMaterial::~LightMaterial() {}

Color LightMaterial::materialColor(const HitRecord &hit, const double time) const{
    return emit;
}

Color LightMaterial::emmitColor(const HitRecord &hit, const double time) const{
    return emit;
}

Ray LightMaterial::bounce(const HitRecord &hit, double time) const{
    return Ray(hit.getHitPosition(), hit.getRay().direction());
    /*static std::random_device rd;
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
        randomVector *= -1;
    }

    return Ray(hit.getHitPosition(), randomVector);*/
}