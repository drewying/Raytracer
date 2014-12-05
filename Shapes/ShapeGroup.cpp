#include "ShapeGroup.hpp"
#include "../Vector.h"
#include "Shape.h"
#include <vector>
#include <float.h>

ShapeGroup::ShapeGroup(Shape* shape){
  left = shape;
  right = shape;
  boundingBox = left->getBounds(0);
}

ShapeGroup::ShapeGroup(Shape* shape1, Shape* shape2){
  left = shape1;
  right = shape2;
  boundingBox = BoundingBox(Vector(1.0,1.0,1.0), Vector(-1.0,-1.0,-1.0));
  boundingBox.expand(shape1->getBounds(0).min);
  boundingBox.expand(shape1->getBounds(0).max);
  boundingBox.expand(shape2->getBounds(0).min);
  boundingBox.expand(shape2->getBounds(0).max);
}

ShapeGroup::ShapeGroup(std::vector<Shape*> *shapes){

  if (shapes->size() == 1){
    std::cout << "Shouldn't be here" << std::endl;
    //*this = ShapeGroup(shapes->at(0));
  }
  if (shapes->size() == 2){
    std::cout << "Shouldn't be here" << std::endl;
    //*this = ShapeGroup(shapes->at(0), shapes->at(1));
  }
  std::cout << "Parsing " << shapes->size() << " shapes." << std::endl;

  boundingBox = BoundingBox(Vector(1.0,1.0,1.0), Vector(-1.0,-1.0,-1.0));

  for (auto shape : *shapes){
    boundingBox.expand(shape->getBounds(0).min);
    boundingBox.expand(shape->getBounds(0).max);
  }

  Vector pivot = (boundingBox.min + boundingBox.max) / 2.0;

  auto pair = ShapeGroup::split(shapes, pivot.x(), 0);

  left = buildBranch(pair.first, 1);
  right = buildBranch(pair.second, 1);
}

ShapeGroup::~ShapeGroup(){}

std::pair<std::vector<Shape*>*, std::vector<Shape*>*> ShapeGroup::split(std::vector<Shape*> *shapes, double pivot, int axis){
  double centroid;
  auto A = new std::vector<Shape*>();
  auto B = new std::vector<Shape*>();

  for (auto s : *shapes){
    BoundingBox box = s->getBounds(0);
    centroid = (box.min.data[axis] + box.max.data[axis])/2.0;

    if(centroid < pivot)
      A->push_back(s);
    else
      B->push_back(s);
  }

  if (A->size() == 0){
    std::size_t const half = B->size() / 2;
    auto X = new std::vector<Shape*>(B->begin(), B->begin() + half);
    auto Y = new std::vector<Shape*>(B->begin() + half, B->end());
    return std::make_pair(X, Y);
  }
  else if (B->size() == 0){
    std::size_t const half = A->size() / 2;
    auto X = new std::vector<Shape*>(A->begin(), A->begin() + half);
    auto Y = new std::vector<Shape*>(A->begin() + half, A->end());
    return std::make_pair(X, Y);
  }


  return std::make_pair(A, B);
}

HitRecord ShapeGroup::intersect(const Ray &ray, const double dist, const double time) const{
  if (!boundingBox.intersect(ray, 0.0, dist)){return HitRecord(dist);}
  HitRecord hitRight = right->intersect(ray, dist, time);
  HitRecord hitLeft = left->intersect(ray, dist, time);

  if (hitRight.getShape() && hitLeft.getShape()){
    if (hitLeft.minT() < hitRight.minT()){
      return hitLeft;
    } else{
      return hitRight;
    }
  }

  if (hitRight.getShape()){
    return hitRight;
  }
  if (hitLeft.getShape()){
    return hitLeft;
  }

  return HitRecord(dist);
}

Vector ShapeGroup::get2DVector(const Vector vector, const double time) const{
  return Vector(0,0,0);
}

BoundingBox ShapeGroup::getBounds(const double time) const{
  return boundingBox;
}

Shape* ShapeGroup::buildBranch(std::vector<Shape*> *shapes, int axis) const{
    if (shapes->size() == 1) return shapes->at(0);
    if (shapes->size() == 2) return new ShapeGroup(shapes->at(0), shapes->at(1));

    Vector min = Vector(1.0,1.0,1.0);
    Vector max = Vector(-1.0,-1.0,-1.0);

    for (auto shape : *shapes){
      min = Min(min, shape->getBounds(0).min);
      max = Max(max, shape->getBounds(0).max);
    }

    Vector pivot = (min + max) / 2.0;

    auto pair = ShapeGroup::split(shapes, pivot.data[axis], axis);

    auto l = buildBranch(pair.first, (axis+1) % 3);
    auto r = buildBranch(pair.second, (axis+1) % 3);

    return new ShapeGroup(l, r);
}