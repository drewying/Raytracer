#include "BoundingBox.h"

void BoundingBox::expand(Vector point) {
    min = Min(min, point);
    max = Max(max, point);
}

void BoundingBox::expandByAmount(double amount) {
    min = Vector(min.x()-amount, min.y()-amount, min.z()-amount);
    max = Vector(max.x()+amount, max.y()+amount, max.z()+amount);
}


Vector BoundingBox::center() const {
    return (min + max) / 2.0;
}

bool BoundingBox::intersect(const Ray &ray, double minimum, double maximum) const {

    //Code from http://people.csail.mit.edu/amy/papers/box-jgt.ps

    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    if (ray.direction().x() >= 0) {
        tmin = (min.x() - ray.origin().x()) / ray.direction().x();
        tmax = (max.x() - ray.origin().x()) / ray.direction().x();
    }
    else {
        tmin = (max.x() - ray.origin().x()) / ray.direction().x();
        tmax = (min.x() - ray.origin().x()) / ray.direction().x();
    }
    if (ray.direction().y() >= 0) {
        tymin = (min.y() - ray.origin().y()) / ray.direction().y();
        tymax = (max.y() - ray.origin().y()) / ray.direction().y();
    }
    else {
        tymin = (max.y() - ray.origin().y()) / ray.direction().y();
        tymax = (min.y() - ray.origin().y()) / ray.direction().y();
    }
    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    if (ray.direction().z() >= 0) {
        tzmin = (min.z() - ray.origin().z()) / ray.direction().z();
        tzmax = (max.z() - ray.origin().z()) / ray.direction().z();
    } else {
        tzmin = (max.z() - ray.origin().z()) / ray.direction().z();
        tzmax = (min.z() - ray.origin().z()) / ray.direction().z();
    }
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    return ((tmin < maximum) && (tmax > minimum));

}
