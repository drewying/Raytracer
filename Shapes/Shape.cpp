#include "Shape.h"
#include "../HitRecord.h"
#include "../Materials/Material.h"
#include "../Ray.h"
#include "../Vector.h"

Shape::Shape() {
}

Shape::Shape(Material *matl) : matl(matl) {
}

Shape::~Shape() {
}