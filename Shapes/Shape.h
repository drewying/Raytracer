#ifndef Shape_h
#define Shape_h

#include "../HitRecord.h"
#include "../Ray.h"
#include "../Matrix.h"
#include "../Animateable.h"
#include "../BoundingBox.h"

class Material;

class Vector;

class Shape {
public:
    Shape();

    Shape(Material *matl);

    virtual ~Shape();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const = 0;

    virtual Vector get2DVector(const Vector vector, const double time) const = 0;

    virtual BoundingBox getBounds(const double time) const = 0;

    Material *matl;

protected:


private:
    Shape(const Shape &);

    Shape &operator=(const Shape &);
};


#endif

