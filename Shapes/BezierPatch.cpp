#include <float.h>
#include <curses.h>
#include "BezierPatch.h"
#include "../HitRecord.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../Animateable.h"
#include "../Matrix.h"
#include "Triangle.h"

BezierPatch::BezierPatch(Material *material, std::vector<Vector>points) : Shape(material){


    boundingBox = BoundingBox();
    this->points = points;
    ucurves[0] = new BezierCurve(matl, points[0], points[1], points[2], points[3]);
    ucurves[1] = new BezierCurve(matl, points[4], points[5], points[6], points[7]);
    ucurves[2] = new BezierCurve(matl, points[8], points[9], points[10], points[11]);
    ucurves[3] = new BezierCurve(matl, points[12], points[13], points[14], points[15]);

    vcurves[0] = new BezierCurve(matl, points[0], points[4], points[8], points[12]);
    vcurves[1] = new BezierCurve(matl, points[1], points[5], points[9], points[13]);
    vcurves[2] = new BezierCurve(matl, points[2], points[6], points[10], points[14]);
    vcurves[3] = new BezierCurve(matl, points[3], points[7], points[11], points[15]);

    for (int i = 0; i < 4; i++){
        auto ucurve = ucurves[i];
        auto vcurve = vcurves[i];
        boundingBox.expand(ucurve->getBounds(0).min);
        boundingBox.expand(ucurve->getBounds(0).max);
        boundingBox.expand(vcurve->getBounds(0).min);
        boundingBox.expand(vcurve->getBounds(0).max);
    }
    buildCache(0.0, 1.0, 0.0, 1.0);
}

BezierPatch::~BezierPatch() {}

HitRecord BezierPatch::intersect(const Ray &ray, const double dist, const double time) const {
    HitRecord hit = HitRecord(dist);

    auto intersections = findIntersections(0.0, 1.0, 0.0, 1.0, ray);
    if (intersections->size() > 0){
        uvPosition intersection;
        if (intersections->size() == 1){
            intersection = (*intersections)[0];
        } else {
            double z = DBL_MAX;
            for (auto i : (*intersections)){
                auto p = calculateBezierPatchPoint(i.first, i.second);
                double d = (p-ray.origin()).length();
                if (d < z){
                    intersection = i;
                    z = d;
                }
            }
        }

        auto p = calculateBezierPatchPoint(intersection.first, intersection.second);
        auto n = calculateBezierPatchPointNormal(intersection.first, intersection.second);
        hit.hit((p-ray.origin()).length(), this, matl, ray, n);
    }

    delete intersections;

    return hit;
}

Vector BezierPatch::get2DVector(const Vector vector, const double time) const {
    return vector;
}

BoundingBox BezierPatch::getBounds(const double time) const {
    return boundingBox;
}

std::vector<Shape*> BezierPatch::tesselate(int resolution) const {
    std::vector<Shape*> triangles;

    Vector points[resolution+1][resolution+1];

    for (int i = 0; i <= resolution; i++) {
        double u = (double)i / (double)resolution;
        for (int j = 0; j <= resolution; j++) {
            double v = (double) j / (double)resolution;
            points[i][j] = calculateBezierPatchPoint(u, v);
        }
    }

    for (int i = 0; i < resolution; i++){
        for (int j = 0; j < resolution; j++){
            Vector point1 = points[i][j];
            Vector point2 = points[i+1][j];
            Vector point3 = points[i][j+1];
            Vector point4 = points[i+1][j+1];
            triangles.emplace_back(new Triangle(matl, point1, point2, point3));
            triangles.emplace_back(new Triangle(matl, point2, point4, point3));
        }
    }
    return triangles;
};

Vector BezierPatch::calculateBezierPatchPointNormal(double u, double v) const{
    Vector Pu[4];
    Vector Pv[4];
    // compute 4 control points along u direction
    for (int i = 0; i < 4; ++i) {
        BezierCurve *curve = ucurves[i];
        Pu[i] =  curve->calculateBezierPoint(u);
    }
    for (int i = 0; i < 4; ++i) {
        BezierCurve *curve = vcurves[i];
        Pv[i] =  curve->calculateBezierPoint(v);
    }

    double vv = v*v;
    Vector tangentu = Pu[0]*(-1 + 2*v - vv) +
            Pu[1]*(1 - 4*v + 3*vv) +
            Pu[2]*(2*v - 3*vv) +
            Pu[3]*(vv);

    double uu = u*u;
    Vector tangentv = Pv[0]*(-1 + 2*u - uu) +
            Pv[1]*(1 - 4*u + 3*uu) +
            Pv[2]*(2*u - 3*uu) +
            Pv[3]*(uu);

    return Cross(tangentv, tangentu);
}

Vector BezierPatch::calculateBezierPatchPoint(double u, double v) const{
    Vector Pu[4];
    // compute 4 control points along u direction
    for (int i = 0; i < 4; ++i) {
        BezierCurve *curve = ucurves[i];
        Pu[i] =  curve->calculateBezierPoint(u);
    }
    BezierCurve curve1(matl, Pu[0], Pu[1], Pu[2], Pu[3]);
    return curve1.calculateBezierPoint(v);
}

void BezierPatch::buildCache(const double u0, const double u1, const double v0, const double v1){

    //Generate Bounding Box
    boundingBoxCache[getBoundingBoxCacheKey(u0, u1, v0, v1)] = getBoundsForRange(u0, u1, v0, v1);

    Vector p0 = calculateBezierPatchPoint(u0, v0);
    Vector p1 = calculateBezierPatchPoint(u1, v1);
    if ((p0-p1).length() < MINIMUM_BOUNDING_DISTANCE) {
        return;
    } else{
        double uMid = (u0 + u1) / 2.0;
        double vMid = (v0 + v1) / 2.0;
        buildCache(u0, uMid, v0, vMid);
        buildCache(uMid, u1, v0, vMid);
        buildCache(u0, uMid, vMid, v1);
        buildCache(uMid, u1, vMid, v1);
    }

};

std::pair<BezierPatch::uvPosition, BezierPatch::uvPosition> BezierPatch::getBoundingBoxCacheKey(const double u0, const double u1, const double v0, const double v1) const{
    return std::make_pair(std::make_pair(u0, u1), std::make_pair(v0, v1));
}

BoundingBox BezierPatch::getBoundsForRange(const double u0, const double u1, const double v0, const double v1) {
    BoundingBox box = BoundingBox();
    for (double u = u0; u <= u1; u+=0.001){
        for (double v = v0; v <= v1; v+=0.001){
            Vector p = calculateBezierPatchPoint(u, v);
            box.expand(p);
        }
    }
    return box;
}

std::vector<BezierPatch::uvPosition>* BezierPatch::findIntersections(const double u0, const double u1, const double v0, const double v1, const Ray &ray) const{


    const double HIT_THRESHOLD = 0.001;

    double uMid = (u0 + u1) / 2.0;
    double vMid = (v0 + v1) / 2.0;

    Vector o = ray.origin();
    Vector d = ray.direction();
    auto uvPos = std::make_pair(uMid, vMid);

    Vector pMid = calculateBezierPatchPoint(uMid, vMid);
    Vector gamma = pMid-o;
    d.normalize();
    gamma.normalize();
    double c = Dot(gamma, gamma);
    double b = Dot(gamma, d);

    double length = sqrt(c-b);

    auto intersections = new std::vector<uvPosition>();
    //std::cout << length << std::endl;
    if(length < HIT_THRESHOLD) {
        intersections->emplace_back(uvPos);
    } else{
        Vector p0 = calculateBezierPatchPoint(u0, v0);
        Vector p1 = calculateBezierPatchPoint(u1, v1);
        double boxLength = (p0-p1).length();
        if (boxLength < MINIMUM_SUBDIVISION_DISTANCE){
            return intersections;
        }

        auto q1Box = boundingBoxCache.find(getBoundingBoxCacheKey(u0, uMid, v0, vMid))->second;
        if (q1Box.intersect(ray, 0, DBL_MAX) || boxLength < MINIMUM_BOUNDING_DISTANCE){
            auto q1intersections = findIntersections(u0, uMid, v0, vMid, ray);
            intersections->insert(intersections->end(), q1intersections->begin(), q1intersections->end());
            delete q1intersections;
        }

        auto q2Box = boundingBoxCache.find(getBoundingBoxCacheKey(uMid, u1, v0, vMid))->second;
        if (q2Box.intersect(ray, 0, DBL_MAX) || boxLength < MINIMUM_BOUNDING_DISTANCE){
            auto q2intersections = findIntersections(uMid, u1, v0, vMid, ray);
            intersections->insert(intersections->end(), q2intersections->begin(), q2intersections->end());
            delete q2intersections;
        }

        auto q3Box = boundingBoxCache.find(getBoundingBoxCacheKey(u0, uMid, vMid, v1))->second;
        if (q3Box.intersect(ray, 0, DBL_MAX) || boxLength < MINIMUM_BOUNDING_DISTANCE){
            auto q3intersections = findIntersections(u0, uMid, vMid, v1, ray);
            intersections->insert(intersections->end(), q3intersections->begin(), q3intersections->end());
            delete q3intersections;
        }

        auto q4Box = boundingBoxCache.find(getBoundingBoxCacheKey(uMid, u1, vMid, v1))->second;
        if (q4Box.intersect(ray, 0, DBL_MAX) || boxLength < MINIMUM_BOUNDING_DISTANCE){
            auto q4intersections = findIntersections(uMid, u1, vMid, v1, ray);
            intersections->insert(intersections->end(), q4intersections->begin(), q4intersections->end());
            delete q4intersections;
        }
    }
    return intersections;
}
