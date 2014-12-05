#include <random>
#include <iostream>
#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(){

    std::random_device rd;
    std::default_random_engine generator(rd());

    //std::default_random_engine generator;

    grad.push_back(Vector(1.0, 1.0, 0.0));
    grad.push_back(Vector(-1.0, 1.0, 0.0));
    grad.push_back(Vector(1.0, -1.0, 0.0));
    grad.push_back(Vector(-1.0, -1.0, 0.0));

    grad.push_back(Vector(1.0, 0.0, 1.0));
    grad.push_back(Vector(-1.0, 0.0, 1.0));
    grad.push_back(Vector(1.0, 0.0, -1.0));
    grad.push_back(Vector(-1.0, 0.0, -1.0));

    grad.push_back(Vector(0.0, 1.0, 1.0));
    grad.push_back(Vector(0.0, -1.0, 1.0));
    grad.push_back(Vector(0.0, 1.0, -1.0));
    grad.push_back(Vector(0.0, -1.0, -1.0));

    grad.push_back(Vector(1.0, 1.0, 0.0));
    grad.push_back(Vector(-1.0, 1.0, 0.0));
    grad.push_back(Vector(0.0, -1.0, 1.0));
    grad.push_back(Vector(0.0, -1.0, -1.0));

    for (int i = 0; i < 16; i++)
        phi.push_back(i);

    //shuffle phi
    std::shuffle(phi.begin(), phi.begin()+14, generator); //15?
}

PerlinNoise::~PerlinNoise(){}

double PerlinNoise::omega(double t) const {
    t = (t > 0.0)? t : -t;
    return (-6.0 * t * t * t * t * t + 15.0 * t * t * t * t - 10.0 * t * t * t + 1.0);
}

Vector PerlinNoise::gamma(int i, int j, int k) const{
    int index;
    index = phi[abs(k) % 16];
    index = phi[abs(j + index) % 16];
    index = phi[abs(i + index) % 16];
    return grad[index];
}

double PerlinNoise::knot(int i, int j, int k, Vector &v) const{
    return (omega(v.x()) * omega(v.y()) * omega(v.z()) * (Dot(gamma(i, j, k), v)));
}

double PerlinNoise::turbulence(const Vector &p, int depth) const{

    double sum = 0.0;
    double weight = 1.0;
    Vector pTemp(p);

    sum = fabs(noise(pTemp));

    for (int i = 1; i < depth; i++){
        weight = weight * 2;
        double x = p.x() * weight;
        double y = p.y() * weight;
        double z = p.z() * weight;

        pTemp = Vector(x, y, z);

        sum += fabs(noise(pTemp)) / weight;
    }

    return sum;
}

double PerlinNoise::dturbulence(const Vector &p, int depth, double d) const{

    double sum = 0.0;
    double weight = 1.0;
    Vector pTemp(p);

    sum = fabs(noise(pTemp)) / d;

    for (int i = 1; i < depth; i++){
        weight = weight * d;
        double x = p.x() * weight;
        double y = p.y() * weight;
        double z = p.z() * weight;

        pTemp = Vector(x, y, z);

        sum += fabs(noise(pTemp)) / d;
    }

    return sum;

}

double PerlinNoise::noise(const Vector &p) const{
    int fi, fj, fk;
    double fu, fv, fw, sum;

    Vector v;

    fi = int(floor(p.x()));
    fj = int(floor(p.y()));
    fk = int(floor(p.z()));

    fu = p.x() - double(fi);
    fv = p.y() - double(fj);
    fw = p.z() - double(fk);

    sum = 0.0;

    v = Vector(fu, fv, fw);
    sum += knot(fi, fj, fk, v);

    v = Vector(fu - 1, fv, fw);
    sum += knot(fi + 1, fj, fk, v);

    v = Vector(fu, fv - 1, fw);
    sum += knot(fi, fj + 1, fk, v);

    v = Vector(fu, fv, fw - 1);
    sum += knot(fi, fj, fk + 1, v);

    v = Vector(fu - 1, fv - 1, fw);
    sum += knot(fi + 1, fj + 1, fk, v);

    v = Vector(fu - 1, fv, fw - 1);
    sum += knot(fi + 1, fj, fk + 1, v);

    v = Vector(fu, fv - 1, fw - 1);
    sum += knot(fi, fj + 1, fk + 1, v);

    v = Vector(fu - 1, fv - 1, fw - 1);
    sum += knot(fi + 1, fj + 1, fk + 1, v);

    return sum;
}