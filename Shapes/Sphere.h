#ifndef Sphere_h
#define Sphere_h

#include "Shape.h"
#include "../Animateable.h"

class Ray;

class Sphere : public Shape {
public:
    Sphere(Material *material, const Vector &center, double radius);

    virtual ~Sphere();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

    Vector center;

    double radius;

protected:
    double inv_radius;
};

#endif
