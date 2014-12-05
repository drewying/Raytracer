#ifndef BezierPatch_h
#define BezierPatch_h

#include "Shape.h"
#include "../Animateable.h"
#include "BezierCurve.h"
#include <map>

class Ray;

class BezierPatch : public Shape {
public:
    BezierPatch(Material *material, std::vector<Vector>points);

    virtual ~BezierPatch();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

    std::vector<Shape*> tesselate(int resolution) const;

protected:
    typedef std::pair<double, double> uvPosition;
    const double MINIMUM_SUBDIVISION_DISTANCE = 0.01;
    const double MINIMUM_BOUNDING_DISTANCE = 0.06;
    Vector n;
    std::vector<Vector> points;
    BezierCurve* ucurves[4];
    BezierCurve* vcurves[4];
    BoundingBox boundingBox;
    Vector calculateBezierPatchPoint(double u, double v) const;
    Vector calculateBezierPatchPointNormal(double u, double v) const;
    std::vector<uvPosition>* findIntersections(const double u0, const double u1, const double v0, const double v1, const Ray &ray) const;

    std::map<std::pair<uvPosition, uvPosition>, BoundingBox> boundingBoxCache;
    std::pair<uvPosition, uvPosition> getBoundingBoxCacheKey(const double u0, const double u1, const double v0, const double v1) const;

    BoundingBox getBoundsForRange(const double u0, const double u1, const double v0, const double v1);
    void buildCache(const double u0, const double u1, const double v0, const double v1);
};

#endif
