#ifndef LightPoint_h
#define LightPoint_h

#include "Light.h"
#include "../Color.h"
#include "../Vector.h"

class LightPoint : public Light {
public:
    LightPoint(const Vector &position, const Color &color);

    virtual ~LightPoint();

    virtual double getLight(Color &light_color, Vector &light_direction, const Vector &pos) const;

private:
    LightPoint(const LightPoint &);

    LightPoint &operator=(const LightPoint &);

    Vector position;
    Color color;
};

#endif

