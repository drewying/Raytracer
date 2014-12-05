//
// Created by Drew Ingebretsen on 9/7/14.
// Copyright (c) 2014 CS6400. All rights reserved.
//


#include "Shape.h"
#include "../Materials/Material.h"

#ifndef Triangle_H
#define Triangle_H


class Triangle : public Shape {

public:
    Triangle();

    Triangle(Material *matl, Vector p0, Vector p1, Vector p2);
    Triangle(Material *matl, Vector p0, Vector p1, Vector p2, Vector normal);

    virtual ~Triangle();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const override;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

protected:
    BoundingBox boundingBox;
    Vector normal;
    Vector p0;
    Vector p1;
    Vector p2;
};


#endif