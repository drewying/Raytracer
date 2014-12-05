#ifndef PerlinNoise_h
#define PerlinNoise_h

#include <algorithm>
#include <vector>
#include "Vector.h"


class PerlinNoise{
public:
    PerlinNoise();
    ~PerlinNoise();

    double omega(double t) const;
    Vector gamma(int i, int j, int k) const;
    double knot(int i, int j, int k, Vector &v) const;

    double turbulence(const Vector &p, int depth) const;
    double dturbulence(const Vector &p, int depth, double d) const;

    double noise(const Vector &p) const;


private:
    std::vector<Vector> grad;
    std::vector<int> phi;

};

#endif
