#include <iostream>
#include <MacTypes.h>
#include "MetalMaterial.h"
#include "../HitRecord.h"
#include "../Lights/Light.h"
#include "../Shapes/Shape.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../MathFunc.h"
#include "../Sample.h"

MetalMaterial::MetalMaterial() {}

MetalMaterial::~MetalMaterial() {}

Color MetalMaterial::materialColor(const HitRecord &hit, const double time) const{
    /*
    double fresnel_reflect = color(time) + (1 - color(time))*pow(1-costheta,5);
        result += rColor*fresnel_reflect;
     */
    return Color(0.9,0.9,0.9);
}

Color MetalMaterial::emmitColor(const HitRecord &hit, const double time) const{
    return Color(0.0, 0.0, 0.0);
}

Ray MetalMaterial::bounce(const HitRecord &hit, double time) const{
    //double costheta = fabs(Dot(hit.getRay().direction(), hit.getNormal()));
    //return Ray(hit.getHitPosition(), hit.getRay().direction() + (hit.getNormal() * (costheta * 2.0)));
    //double costheta = Dot(hit.getRay().direction(), hit.getNormal());
    //return Ray(hit.getHitPosition(), hit.getRay().direction() - (hit.getNormal() * (costheta * 2.0)));
    Vector normal = hit.getNormal();
    normal.normalize();
    Vector direction = hit.getRay().direction();
    double cosine = Dot(direction, normal);
    return Ray(hit.getHitPosition(), direction - (normal*2.0*cosine));
}