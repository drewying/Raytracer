//
// Created by Drew Ingebretsen on 9/10/14.
// Copyright (c) 2014 CS6400. All rights reserved.
//


#include "Shape.h"

#ifndef Plane_H
#define Plane_H


class Plane : public Shape {
public:
    Plane(Material *material, const Vector &n, const Vector &point);

    virtual ~Plane();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

private:
    Plane(const Plane &);

    Plane &operator=(const Plane &);

    Vector n;
    double d;

};


#endif
