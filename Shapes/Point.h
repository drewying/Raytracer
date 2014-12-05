#ifndef Point_h
#define Point_h

#include "Shape.h"
#include "../Animateable.h"

class Ray;

class Point : public Shape {
public:
    Point(Material *material, const Vector &position);

    virtual ~Point();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

protected:
    Vector position;
};

#endif
