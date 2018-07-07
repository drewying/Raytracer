#include <cfloat>
#include "ShapeInstance.h"
#include "Shape.h"

ShapeInstance::ShapeInstance(Shape* shape) : shape(shape){
    matl = shape->matl;
    transformMatrix.addFrame(0, identityMatrix());
}

ShapeInstance::~ShapeInstance(){
    delete shape;
}

HitRecord ShapeInstance::intersect(const Ray &ray, const double dist, const double time) const{
    HitRecord hit = shape->intersect(transformRay(ray, time), dist, time);

    if (hit.didHit()){
        HitRecord newHit = HitRecord(DBL_MAX);
        Matrix m = transformMatrix(time);
        Vector n = transformVec(m.getInverse().getTranspose(), hit.getNormal());
        Ray r = Ray(transformLoc(m, hit.getRay().origin()), transformVec(m, hit.getRay().direction()));
        newHit.hit(hit.minT(), this, hit.getMaterial(), r, n);
        return newHit;
    }

    return hit;
}

Vector ShapeInstance::get2DVector(const Vector vector, const double time) const {
    return shape->get2DVector(transformPoint(vector, time), time);
}

Ray ShapeInstance::transformRay(Ray const &ray, const double time) const {
    Matrix m = transformMatrix(time).getInverse();
    return Ray(transformLoc(m, ray.origin()), transformVec(m, ray.direction()));
}

Vector ShapeInstance::transformPoint(const Vector &vec, const double time) const {
    Matrix m = transformMatrix(time).getInverse();
    return transformLoc(m, vec);
}

BoundingBox ShapeInstance::getBounds(const double time) const {
  auto box = shape->getBounds(time);
  auto m = transformMatrix(time);
  auto pointa = transformLoc(m, box.min);
  auto pointb = transformLoc(m, box.max);

  box.expand(pointa);
  box.expand(pointb);

  return box;
}

