#ifndef BezierCurve_h
#define BezierCurve_h

#include "Shape.h"
#include "../Animateable.h"

class Ray;

class BezierCurve : public Shape {
public:
    BezierCurve(Material *material, Vector p0, Vector p1, Vector p2, Vector p3);

    BezierCurve();

    virtual ~BezierCurve();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

    Vector calculateBezierPoint(double t) const;

protected:
    BoundingBox boundingBox;
    Vector n;
    double findIntersection(const double t0, const double t1, const Ray &ray) const;
    Vector points[4];
};

#endif
