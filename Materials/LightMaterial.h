#ifndef LightMaterial_h
#define LightMaterial_h

#include "Material.h"
#include "../Color.h"

class LightMaterial : public Material {
public:
    LightMaterial(Color lightColor);

    virtual ~LightMaterial();

    virtual Color materialColor(const HitRecord &hit, const double time) const;

    virtual Color emmitColor(const HitRecord &hit, const double time) const;

    virtual Ray bounce(const HitRecord &hit, double time) const;

    virtual bool isDiffuse() const {return true;}

private:

    LightMaterial(const LightMaterial &);

    LightMaterial &operator=(const LightMaterial &);

    Color emit;
};

#endif
