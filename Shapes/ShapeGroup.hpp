#ifndef ShapeGroup_h
#define ShapeGroup_h

#include "Shape.h"

class ShapeGroup : public Shape {
public:
  ShapeGroup(std::vector<Shape*> *shapes);
  ShapeGroup(Shape* shape);
  ShapeGroup(Shape* shape1, Shape* shape2);

  virtual ~ShapeGroup();

  virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

  virtual Vector get2DVector(const Vector vector, const double time) const;

  virtual BoundingBox getBounds(const double time) const;

  Shape* buildBranch(std::vector<Shape*> *shapes, int axis) const;

  static std::pair<std::vector<Shape*>*, std::vector<Shape*>*> split(std::vector<Shape*> *shapes, double pivot, int axis);

protected:
  Shape *left;
  Shape *right;
  BoundingBox boundingBox = BoundingBox(Vector(0,0,0), Vector(0,0,0));
};

#endif