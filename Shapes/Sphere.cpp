#include <float.h>
#include "Sphere.h"
#include "../HitRecord.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../Animateable.h"
#include "../Matrix.h"

Sphere::Sphere(Material *material, const Vector &center, double radius)
        : Shape(material){

    this->radius = radius;
    this->center = center;
    this->inv_radius = 1.0/radius;
}

Sphere::~Sphere() {}

HitRecord Sphere::intersect(const Ray &ray, const double dist, const double time) const {
    HitRecord hit = HitRecord(dist);

    Vector v = center - ray.origin();
    /*if (Dot(v, v) < radius * radius) {
        return hit;
    }*/

    double b = Dot(v, ray.direction());
    double discriminant = b * b - Dot(v,v) + radius * radius;

    if (discriminant < 0) {
        return hit;
    }
    double d = sqrt(discriminant);
    double tfar = b + d;
    double eps = 1e-4;
    if (tfar <= eps) {
        return hit;
    }
    double tnear = b - d;
    if (tnear <= eps) {
        Vector hitpos = ray.origin() + ray.direction() * tfar;
        Vector n = (hitpos - center);
        hit.hit(tfar, this, matl, ray, n);
    } else{
        Vector hitpos = ray.origin() + ray.direction() * tnear;
        Vector n = (hitpos - center);
        hit.hit(tnear, this, matl, ray, n);
    }
    return hit;
}

Vector Sphere::get2DVector(const Vector vector, const double time) const {
    Vector norm = (vector - center);
    norm.normalize();
    const double x = norm.x();
    const double y = norm.y();
    const double z = norm.z();

    const double PI = 3.141592653589793;
    const double INV_PI = 0.318309886184;
    const double INV_PI_2 = 0.159154943092;
    const double PI_2 = 6.28318530718;

    double theta = acos(z);
    double phi = atan2(y, x);
    if(phi < 0.0)
        phi += PI_2;

    return Vector(phi*INV_PI_2, (PI - theta) * INV_PI, 0);
}

BoundingBox Sphere::getBounds(const double time) const {
  double r = radius;
  Vector c = center;
  Vector diag(r, r, r);
  return BoundingBox(c-diag, c+diag);
}