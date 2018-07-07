#include <cfloat>
#include "Point.h"
#include "../HitRecord.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../Animateable.h"
#include "../Matrix.h"

Point::Point(Material *material, const Vector &position)
        : Shape(material){
    this->position = position;
}

Point::~Point() {}

HitRecord Point::intersect(const Ray &ray, const double dist, const double time) const {
    //There is no hitting a point!
    HitRecord hit = HitRecord(dist);
    return hit;
}

Vector Point::get2DVector(const Vector vector, const double time) const {
    return vector;
}

BoundingBox Point::getBounds(const double time) const {
    return BoundingBox(position, position);
}