#ifndef Sample_h
#define Sample_h

#include <math.h>
#include <vector>
#include <random>

namespace Sample{
    double tentFilter(double x);
    std::vector<double> filterSamples(std::vector<double> samples);
    std::vector<double> createJitteredSamples(int count);
}

#endif

