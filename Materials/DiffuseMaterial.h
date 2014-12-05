#ifndef DiffuseMaterial_h
#define DiffuseMaterial_h

#include "Material.h"
#include "../Color.h"

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial();

    virtual ~DiffuseMaterial();

    virtual Color materialColor(const HitRecord &hit, const double time) const;

    virtual Color emmitColor(const HitRecord &hit, const double time) const;

    virtual Color getColor(const HitRecord &hit, double time) const;

    virtual Ray bounce(const HitRecord &hit, double time) const;

    virtual bool isDiffuse() const {return true;}

private:

    DiffuseMaterial(const DiffuseMaterial &);

    DiffuseMaterial &operator=(const DiffuseMaterial &);

    Color color;
};

#endif
