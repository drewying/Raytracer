#include "Camera.h"
#include "Ray.h"
#include "MathFunc.h"
#include "HitRecord.h"
#include "Materials/Material.h"
#include "Shapes/Shape.h"
#include "Backgrounds/Background.h"

#include <cmath>
#include <cfloat>
#include <iostream>
#include <thread>
#include "Sample.h"

Camera::Camera(const Vector &position, const Vector &lookAt, const unsigned int xResolution, const unsigned int yResolution) : position(position), lookAt(lookAt), xResolution(xResolution), yResolution(yResolution) {}

Camera::~Camera() {
}

Ray Camera::makeRay(double x, double y, double r1, double r2) const {
    Vector base = right * x + up * y;

    Vector U = r1 * apertureSize * up;
    Vector V = r2 * apertureSize * right;
    Vector UV = U + V;

    Vector direction = base + lookAt;
    direction = (direction * focalLength) - UV;

    direction.normalize();

    Vector origin = position + UV;

    return Ray(origin, direction);
}

Image* Camera::render(Scene *scene, double time) {

    Image *image = new Image(xResolution, yResolution);
    
    if (cameraMode == PhotonMapTrace){
        scene->buildPhotonMap(time);
    }

    double theta = fieldOfView * (M_PI / 180.0); 
    double aspectRatio = (double)yResolution / (double)xResolution;
    double halfWidth = tan(theta / 2);
    double dx = (halfWidth * 2.0) / (double)xResolution;
    double xmin = -halfWidth;
    double dy = (halfWidth * 2.0 * aspectRatio) / (double)yResolution;
    double ymin = -halfWidth * aspectRatio;

    std::cout << "Rendering..." << std::endl;

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < yResolution; i++) {
        if (i % (yResolution / 10) == 0){
            std::cout << "*";
        }
        double y = ymin + i * dy;
        for (int j = 0; j < xResolution; j+=1) {
            double x = xmin + j * dx;
            Color finalColor = samplePixel(scene, x, y, time);
            image->set(j, i, finalColor);
        }
    }

    std::cout << std::endl << "Complete" << std::endl;

    return image;
}

Color Camera::samplePixel(const Scene* scene, double x, double y, int time) const{
    static const auto pixelXSamples = Sample::filterSamples(Sample::createJitteredSamples(sampleCount));
    static const auto pixelYSamples = Sample::filterSamples(Sample::createJitteredSamples(sampleCount));
    static const auto timeSamples = Sample::filterSamples(Sample::createJitteredSamples(sampleCount));
    static const auto dofXSamples = Sample::filterSamples(Sample::createJitteredSamples(sampleCount));
    static const auto dofYSamples = Sample::filterSamples(Sample::createJitteredSamples(sampleCount));

    static std::random_device rd;
    static std::default_random_engine generator(rd());
    std::vector<int> xDist = std::vector<int>(sampleCount);
    std::vector<int> yDist = std::vector<int>(sampleCount);

    for (int i = 0; i < sampleCount; i++){
        xDist[i] = i;
        yDist[i] = i;
    }

    std::shuffle(xDist.begin(), xDist.end(), generator);
    std::shuffle(yDist.begin(), yDist.end(), generator);

    Color finalColor = Color(0,0,0);

    for (int k=0; k < sampleCount; k++){

        auto pixelXSample = pixelXSamples[xDist[k]];
        auto pixelYSample = pixelYSamples[yDist[k]];
        auto timeSample   = timeSamples[k];
        auto dofXSample   = dofXSamples[xDist[k]];
        auto dofYSample   = dofYSamples[yDist[k]];

        double xOffset    = pixelXSample / (xResolution);
        double yOffset    = pixelYSample / (yResolution);
        double timeOffset = timeSample*0.33;

        //if (timeOffset > 0.0)
            timeOffset = 0.0;

        Ray ray = makeRay(x + xOffset, y + yOffset, dofXSample, dofYSample);

        Color result;
        if (cameraMode == PathTrace){
            result = scene->tracePath(ray, 0, time+timeOffset);
        } else if (cameraMode == RayTrace){
            result = scene->traceRay(ray, time+timeOffset);
        } else if (cameraMode == PhotonMapTrace){
            result = scene->tracePhoton(ray, time+timeOffset);
        }

        finalColor += (result * (1.0/(double)sampleCount));
    }

    return finalColor;
}


