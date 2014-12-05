#ifndef ColorBackground_h
#define ColorBackground_h

#include "Background.h"
#include "../Color.h"

class ColorBackground : public Background {
public:
    ColorBackground(const Color &color);

    virtual ~ColorBackground();

    virtual Color getBackgroundColor(const Ray &ray);

private:
    ColorBackground(const ColorBackground &);

    ColorBackground &operator=(const ColorBackground &);

    Color bgcolor;
};

#endif

