//
// Created by Drew Ingebretsen on 9/7/14.
// Copyright (c) 2014 CS6400. All rights reserved.
//

#include <cfloat>
#include "Triangle.h"
#include "../Vector.h"
#include "../Materials/Material.h"

Triangle::Triangle() : Shape() {}

Triangle::Triangle(Material *matl, Vector p0, Vector p1, Vector p2) : Shape(matl), p0(p0), p1(p1), p2(p2) {
    normal = Cross(Vector(p1-p2), Vector(p2-p0));
    boundingBox = BoundingBox();
    boundingBox.expand(p0);
    boundingBox.expand(p1);
    boundingBox.expand(p2);

}

Triangle::Triangle(Material *matl, Vector p0, Vector p1, Vector p2, Vector normal) : Shape(matl), p0(p0), p1(p1), p2(p2), normal(normal) {
    boundingBox = BoundingBox();
    boundingBox.expand(p0);
    boundingBox.expand(p1);
    boundingBox.expand(p2);
}

Triangle::~Triangle() {}

HitRecord Triangle::intersect(const Ray &ray, const double dist, const double time) const {
    double tval;

    double A = p0.x() - p1.x();
    double B = p0.y() - p1.y();
    double C = p0.z() - p1.z();

    double D = p0.x() - p2.x();
    double E = p0.y() - p2.y();
    double F = p0.z() - p2.z();

    double G = ray.direction().x();
    double H = ray.direction().y();
    double I = ray.direction().z();

    double J = p0.x() - ray.origin().x();
    double K = p0.y() - ray.origin().y();
    double L = p0.z() - ray.origin().z();

    double EIHF = E*I-H*F;
    double GFDI = G*F-D*I;
    double DHEG = D*H-E*G;

    double denom = (A*EIHF + B*GFDI + C*DHEG);

    double beta = (J*EIHF + K*GFDI + L*DHEG) / denom;

    if (beta <= 0.0f || beta >= 1.0f) return false;

    double AKJB = A*K-J*B;
    double JCAL = J*C-A*L;
    double BLKC = B*L-K*C;

    float gamma = (I*AKJB + H*JCAL + G*BLKC)/denom;

    if (gamma <= 0.0f || beta + gamma >= 1.0f) return false;

    tval = -(F*AKJB + E*JCAL + D*BLKC) / denom;

    HitRecord hit(dist);

    if (tval > 0){
        hit.hit(tval, this, matl, ray, normal);
    }

    return hit;
}

Vector Triangle::get2DVector(const Vector vector, const double time) const{
    return vector;
}

BoundingBox Triangle::getBounds(const double time) const {
  return boundingBox;
}