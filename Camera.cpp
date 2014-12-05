#include "Camera.h"
#include "Ray.h"
#include "MathFunc.h"
#include "HitRecord.h"
#include "Materials/Material.h"
#include "Shapes/Shape.h"
#include "Backgrounds/Background.h"

#include <math.h>
#include <float.h>
#include <iostream>
#include <thread>
#include "Sample.h"

Camera::Camera(const Vector &eye, const Vector &lookAt, const Vector &up, const Vector &right) : eye(eye), lookAt(lookAt), up(up), right(right){
    focalLength = 1.0;
    sampleCount = 25;
    apertureSize = 0.0;
}

Camera::~Camera() {
}

void Camera::preprocess(double aspect_ratio) {
    right *= aspect_ratio;
}

Ray Camera::makeRay(double x, double y, double r1, double r2) const {

    Vector base = right * x + up * y;
    Vector centered = base - Vector(right.x()/2.0, up.y()/2.0, 0.0);

    Vector U = r1 * apertureSize * up;
    Vector V = r2 * apertureSize * right;
    Vector UV = U+V;

    Vector direction = centered + lookAt;
    direction = (direction * focalLength) - UV;

    direction.normalize();

    Vector origin = eye + UV;

    return Ray(origin, direction);
}

Image* Camera::render(Scene *scene, double time) {

    Image *image = new Image(xResolution, yResolution);
    preprocess(image->aspect_ratio());

    if (cameraMode == PhotonMapTrace){
        scene->buildPhotonMap(time);
    }

    double dx = 1.0 / xResolution;
    double xmin = 0.0;
    double dy = 1.0 / yResolution;
    double ymin = 0.0;

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < yResolution; i++) {
        if (i % 50 == 0){
            std::cout << "Completed " << ((double)i/(double)yResolution)*100 << " %" << std::endl;
        }
        double y = ymin + i * dy;
        for (int j = 0; j < xResolution; j+=1) {
            double x = xmin + j * dx;
            Color finalColor = samplePixel(scene, x, y, time);
            image->set(j, i, finalColor);
        }
    }

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
        auto timeSample = timeSamples[k];
        auto dofXSample = dofXSamples[xDist[k]];
        auto dofYSample = dofYSamples[yDist[k]];

        double xOffset = pixelXSample/(xResolution);
        double yOffset = pixelYSample/(yResolution);
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


