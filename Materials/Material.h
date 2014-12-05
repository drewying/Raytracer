#ifndef Material_h
#define Material_h

class Color;

class HitRecord;

class Ray;

class RenderContext;

class Material {
public:
    Material();

    virtual ~Material();

    virtual Color materialColor(const HitRecord &hit, const double time) const = 0;

    virtual Color emmitColor(const HitRecord &hit, const double time) const = 0;

    virtual Ray bounce(const HitRecord &hit, double time) const = 0;

    virtual bool isDiffuse() const = 0;

private:
    Material(const Material &);

    Material &operator=(const Material &);
};

#endif

