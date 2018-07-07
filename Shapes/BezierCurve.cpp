#include "BezierCurve.h"
#include "../HitRecord.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../Animateable.h"
#include "../Matrix.h"

BezierCurve::BezierCurve(Material *material, Vector p0, Vector p1, Vector p2, Vector p3) : Shape(material){
    points[0] = p0;
    points[1] = p1;
    points[2] = p2;
    points[3] = p3;
    boundingBox = BoundingBox();
    boundingBox.expand(p0);
    boundingBox.expand(p1);
    boundingBox.expand(p2);
    boundingBox.expand(p3);

    //Calc normal
    this->n = Cross(Vector(p1-p2), Vector(p2-p0));
}

BezierCurve::BezierCurve() {}

BezierCurve::~BezierCurve() {}

HitRecord BezierCurve::intersect(const Ray &ray, const double dist, const double time) const {
    HitRecord hit = HitRecord(dist);

    //These are the endpoints of the line segment
    double t = findIntersection(0.0, 1.0, ray);
    if (t != 0.0){
        hit.hit(t, this, matl, ray, n);
    }

    return hit;
}

Vector BezierCurve::get2DVector(const Vector vector, const double time) const {
    return vector;
}

BoundingBox BezierCurve::getBounds(const double time) const {
    return boundingBox;
}


Vector BezierCurve::calculateBezierPoint(double t) const{

    Vector p0 = points[0];
    Vector p1 = points[1];
    Vector p2 = points[2];
    Vector p3 = points[3];

    double u = 1.0 - t;
    double tt = t*t;
    double ttt = t* t * t;
    double uu = u*u;
    double uuu = u * u * u;


    Vector p = uuu * p0; //first term
    p += 3 * uu * t * p1; //second term
    p += 3 * u * tt * p2; //third term
    p += ttt * p3; //fourth term

  return p;
}

double BezierCurve::findIntersection(const double t0, const double t1, const Ray &ray) const{


    const double MINIMUM_LINE_DISTANCE = 0.03;
    const double THRESHOLD = 0.003;

    double tMid = (t0 + t1) / 2.0;

    Vector o = ray.origin();
    Vector pMid = calculateBezierPoint(tMid);
    Vector d = ray.direction();

    Vector gamma = pMid-o;
    gamma.normalize();
    double c = Dot(gamma, gamma);
    double b = Dot(gamma, d);

    double length = sqrt(c-b);

    if(length < THRESHOLD) {
        return tMid;
    } else{

        double lValue = 0;
        double rValue = 0;

        Vector p0 = calculateBezierPoint(t0);
        Vector p1 = calculateBezierPoint(t1);

        p0.normalize();
        p1.normalize();

        if ((p0-p1).length() < MINIMUM_LINE_DISTANCE){
            return 0;
        }

        lValue = findIntersection(t0, tMid, ray);
        rValue = findIntersection(tMid, t1, ray);
        return (rValue > lValue) ? rValue : lValue;
    }
}