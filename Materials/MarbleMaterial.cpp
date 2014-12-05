#include "MarbleMaterial.h"
#include "../Color.h"
#include "../HitRecord.h"
#include "../Ray.h"
#include "../Sample.h"
#include "../Lights/Light.h"


MarbleMaterial::MarbleMaterial(double stripesPerUnit, double scale, int octaves) : scale(scale), octaves(octaves){
    frequency = M_PI * stripesPerUnit;
    color0 = Color(0.8, 0.8, 0.8);
    color1 = Color(0.4, 0.2, 0.1);
    color2 = Color(0.06, 0.04, 0.02);
}

MarbleMaterial::~MarbleMaterial(){}

Color MarbleMaterial::getColor(const HitRecord &hit, double time) const{

    Ray ray = hit.getRay();
    Vector p = ray.origin() + ray.direction() * hit.minT();

    double temp = scale * noise.turbulence(frequency * p, octaves);
    double t = 2.0 * fabs(sin(frequency * p.x() + temp));

    if (t < 1.0){
        return (color1 * t + color2 * (1.0 - t));
    }
    else{
        t -= 1.0;
        return (color0 * t + color1 * (1.0 - t));
    }
}