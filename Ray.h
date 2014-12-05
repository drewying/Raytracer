#ifndef Ray_h
#define Ray_h

#include "Vector.h"

class Ray {
public:
    Ray() {
    }

    Ray(const Vector &origin, const Vector &direction)
            : org(origin), dir(direction) {
    }

    ~Ray() {
    }

    Ray(const Ray &copy)
            : org(copy.org), dir(copy.dir) {
    }

    Ray &operator=(const Ray &copy) {
        org = copy.org;
        dir = copy.dir;
        return *this;
    }

    const Vector &origin() const {
        return org;
    }

    const Vector &direction() const {
        return dir;
    }

private:
    Vector org;
    Vector dir;
};

#endif
