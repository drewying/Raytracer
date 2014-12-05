#ifndef BPTModel_h
#define BPTModel_h

#include "BPTModel.h"
#include "../Animateable.h"
#include "Shape.h"
#include "Triangle.h"
#include "ShapeGroup.hpp"

#include <string>

class Ray;

class BPTModel : public Shape {
public:
    BPTModel(Material *material, std::string filename, bool tesselate);

    virtual ~BPTModel();

    virtual HitRecord intersect(const Ray &ray, const double dist, const double time) const;

    virtual Vector get2DVector(const Vector vector, const double time) const;

    virtual BoundingBox getBounds(const double time) const;

protected:
    ShapeGroup *shapes;
    void parse(std::string filename, bool tesselate);
};

#endif
