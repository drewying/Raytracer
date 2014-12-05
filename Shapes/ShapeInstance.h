#ifndef ShapeInstance_h
#define ShapeInstance_h

#include "../Animateable.h"
#include "Shape.h"

class ShapeInstance : public Shape{
public:
    ShapeInstance(Shape* shape);

    ~ShapeInstance();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

    Animateable<Matrix> transformMatrix;

private:
    Shape *shape;
    Ray transformRay (const Ray &ray, const double time) const;
    Vector transformPoint (const Vector &vec, const double time) const;
};

#endif