//
// Created by Drew Ingebretsen on 9/10/14.
// Copyright (c) 2014 CS6400. All rights reserved.
//

#include <cfloat>
#include "Plane.h"


Plane::Plane(Material *material, const Vector &normal, const Vector &point) : Shape(material), n(normal) {
    n.normalize();
    d = Dot(n, point);
}

Plane::~Plane() {}

HitRecord Plane::intersect(const Ray &ray, const double dist, const double time) const {
    HitRecord hit = HitRecord(dist);
    double denom = Dot(n, ray.direction());
    if (Abs(denom) > 1.e-6) {
        double t = (d - Dot(n, ray.origin())) / denom;
        hit.hit(t, this, matl, ray, n);
    }
    return hit;
}

Vector Plane::get2DVector(const Vector vector, const double time) const{
    Vector v = vector;
    v.normalize();
    double x = (v.x()/2)+0.5;
    double y = (v.y()/2)+0.5;

    return Vector(x, y, 0.0);
}

BoundingBox Plane::getBounds(const double time) const{
    return BoundingBox(Vector(-1.0,-1.0,-1.0), Vector(1.0,1.0,1.0));
}