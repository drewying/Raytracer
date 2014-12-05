#ifndef HitRecord_h
#define HitRecord_h

#include "Ray.h"
#include "Vector.h"

class Material;

class Shape;

#define EPSILON (1.e-6)

class HitRecord {
public:
    HitRecord(double max) {
        shape = 0;
        material = 0;
        min = max;
        h = false;
    }

    ~HitRecord() {
    }

    bool hit(double t, const Shape *hitShape, const Material *hitMaterial, const Ray &r, const Vector &n) {
        if (t > EPSILON && t < min) {
            min = t;
            shape = hitShape;
            material = hitMaterial;
            ray = r;
            normal = n;
            h = true;
            return true;
        } else {
            return false;
        }
    }

    double minT() const {
        return min;
    }

    const Shape *getShape() const {
        return shape;
    }

    const Material *getMaterial() const {
        return material;
    }

    const Ray getRay() const {
        return ray;
    }

    const Vector getHitPosition() const {
        return ray.origin() + ray.direction() * min;
    }

    const Vector getNormal() const {
      return normal;
    }

    bool didHit(){
        return h;
    }

private:
    bool h;
    const Shape *shape;
    const Material *material;
    double min;
    Ray ray;
    Vector normal;
};

#endif
