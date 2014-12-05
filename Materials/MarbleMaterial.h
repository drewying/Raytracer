#ifndef MarbleMaterial_h
#define MarbleMaterial_h

#include "../PerlinNoise.h"
#include "../Color.h"
#include "DiffuseMaterial.h"

class MarbleMaterial : public DiffuseMaterial {
public:

    MarbleMaterial(double stripesPerUnit, double scale, int octaves);

    virtual ~MarbleMaterial();

    virtual Color getColor(const HitRecord &hit, double time) const;

    double frequency;
    double scale;
    int octaves;
    Color color0;
    Color color1;
    Color color2;

private:
    Color getColorAtCoordinate(const Vector& p) const;
    PerlinNoise noise;
};

#endif