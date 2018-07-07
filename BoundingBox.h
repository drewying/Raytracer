#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <cfloat>
#include "Vector.h"
#include "Ray.h"

class BoundingBox {
public:
    BoundingBox() {
        max = Vector(-DBL_MAX, -DBL_MAX, -DBL_MAX);
        min = Vector(DBL_MAX, DBL_MAX, DBL_MAX);
    }

    BoundingBox(Vector min, Vector max) {
        this->min = min;
        this->max = max;
    }

    Vector center() const;

    void expand(const Vector point);
    void expandByAmount(double amount);

    bool intersect(const Ray &ray, double min, double max) const;

    Vector min;
    Vector max;

};

#endif