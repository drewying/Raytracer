#ifndef ColorMaterial_h
#define ColorMaterial_h

#include "DiffuseMaterial.h"
#include "../Color.h"

class ColorMaterial : public DiffuseMaterial {
public:
    ColorMaterial(const Color &color);

    virtual ~ColorMaterial();

    virtual Color getColor(const HitRecord &hit, double time) const;

private:
    Color color;
};

#endif
