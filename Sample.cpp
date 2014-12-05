#include "Sample.h"

double Sample::tentFilter(double x){
    if (x < 0.5){
        return sqrt(2.0*x) - 1.0;
    }
    else{
        return 1.0 - sqrt(2.0 - 2.0 * x);
    }
}

std::vector<double> Sample::filterSamples(std::vector<double> samples){
    std::vector<double> filteredSamples;
    for (auto sample : samples){
        filteredSamples.push_back(tentFilter(sample));
    }
    return filteredSamples;
}

std::vector<double> Sample::createJitteredSamples(int count){

    std::vector<double> samples;

    double inc = 1.0 / (double)count;

    std::random_device rd;
    std::default_random_engine generator(rd());

    for (int x=0; x < count; x++){

        std::uniform_real_distribution<double> xdistribution(x*inc, (x*inc)+inc);

        double ysample = xdistribution(generator);

        samples.push_back(ysample);

    }
    return samples;

}
