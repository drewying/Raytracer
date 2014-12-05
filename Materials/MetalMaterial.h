#ifndef MetalMaterial_h
#define MetalMaterial_h

#include "Material.h"
#include "../Color.h"

class MetalMaterial : public Material {
public:
    MetalMaterial();

    virtual ~MetalMaterial();

    virtual Color materialColor(const HitRecord &hit, const double time) const;

    virtual Color emmitColor(const HitRecord &hit, const double time) const;

    virtual Ray bounce(const HitRecord &hit, double time) const;

    virtual bool isDiffuse() const {return false;}

private:

    MetalMaterial(const MetalMaterial &);

    MetalMaterial &operator=(const MetalMaterial &);

};

#endif
