#ifndef PlyModel_h
#define PlyModel_h

#include "PlyModel.h"
#include "../Animateable.h"
#include "Shape.h"
#include "Triangle.h"
#include "ShapeGroup.hpp"

#include <string>

class Ray;

class PLYModel : public Shape {
public:
    PLYModel(Material *material, std::string filename);

    virtual ~PLYModel();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

protected:
  ShapeGroup *shapes;
  void parse(std::string filename);
};

#endif
