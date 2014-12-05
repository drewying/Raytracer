#include "ColorBackground.h"

ColorBackground::ColorBackground(const Color &bgcolor)
        : bgcolor(bgcolor) {
}

ColorBackground::~ColorBackground() {
}

Color ColorBackground::getBackgroundColor(const Ray &) {
    return bgcolor;
}
