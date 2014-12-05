#ifndef GlassMaterial_h
#define GlassMaterial_h

#include "Material.h"
#include "../Color.h"

class GlassMaterial : public Material {
public:
    GlassMaterial(double ior, double reflection);

    virtual ~GlassMaterial();

    virtual Color materialColor(const HitRecord &hit, const double time) const;

    virtual Color emmitColor(const HitRecord &hit, const double time) const;

    virtual Ray bounce(const HitRecord &hit, double time) const;

    virtual bool isDiffuse() const {return false;}

private:

    double indexOfRefraction;
    double reflection;

    GlassMaterial(const GlassMaterial &);

    GlassMaterial &operator=(const GlassMaterial &);

};

#endif
