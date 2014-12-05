#include "Scene.h"
#include "PhotonMap.h"
#include "Backgrounds/Background.h"
#include "Camera.h"
#include "HitRecord.h"
#include "Image.h"
#include "Lights/Light.h"
#include "Materials/Material.h"
#include "Shapes/Shape.h"
#include "Ray.h"
#include "Sample.h"
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <random>
#include <MacTypes.h>

Scene::Scene() {
    background = 0;
    ambient = Color(0, 0, 0);
}

Scene::~Scene() {
    delete background;
    for (auto shape : shapes)
        delete shape;
    for (auto light : lights)
        delete light;
}

HitRecord Scene::getClosestIntersection(const Ray &ray, bool includeLights, int time) const {
    float t = DBL_MAX; //For Z-Depth Handling
    HitRecord finalHit = HitRecord(t);
    for (auto shape : shapes) {
        HitRecord hit = shape->intersect(ray, t, time);
        if (hit.didHit()) {
            //Check the z buffer
            if (includeLights || hit.getMaterial()->emmitColor(hit, time).r() == 0){
                t = hit.minT();
                finalHit = hit;
            }
        }
    }

    return finalHit;
}

void Scene::buildPhotonMap(int time){
    std::cout << "Building Photon Map" << std::endl;

    const int photonCount = 100000;

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    auto lights = getLights(time);

    for (auto l : lights){
        for (int i = 0; i < photonCount; i++){

            double x = distribution(generator);
            double y = distribution(generator);
            double z = distribution(generator);

            Vector origin = l->getBounds(time).center();
            Vector randomVector = Vector(x,y,z);

            mapPhoton(Ray(origin,randomVector), l->matl->emmitColor(HitRecord(0), time), 0, 0);

        }
    }

    //photonMap.scale_photon_power(0.005/lights.size());
    photonMap.scale_photon_power(0.005/lights.size());
    std::cout << "Balancing Photon Map" << std::endl;
    photonMap.balance();
    std::cout << "Done building photon map" << std::endl;
}

void Scene::mapPhoton(const Ray &ray, Color incomingPower, int depth, int time) {

    HitRecord hit = getClosestIntersection(ray, false, time);

    if (!hit.didHit()){
        return;
    }

    if (depth != 0)
        photonMap.store(incomingPower.data, hit.getHitPosition().data, ray.direction().data);

    const int maxPhotonDepth = 3;

    if (depth > maxPhotonDepth) {
        return;
    }else{

        Ray newRay = hit.getMaterial()->bounce(hit, time);
        Vector normal = hit.getNormal();
        normal.normalize();
        double cosphi = Dot(hit.getNormal(), -hit.getRay().direction());
        Color materialColor = hit.getMaterial()->materialColor(hit, time);
        Color outgoingPower = materialColor * incomingPower * cosphi;
        mapPhoton(newRay, outgoingPower, depth+1, time);
    }

}

Color Scene::tracePhoton(const Ray &ray, int time) const {
    //Detemine if there is a hit
    HitRecord hit = getClosestIntersection(ray, false, time);

    if (!hit.didHit()){
        return background->getBackgroundColor(ray);
    }

    if (hit.getMaterial()->isDiffuse() == false){
        return traceRay(hit.getMaterial()->bounce(hit, time), time);
    }

    double colorArray[3];
    photonMap.irradiance_estimate(colorArray, hit.getHitPosition().data, hit.getNormal().data, 0.1, 300);
    Color light = Color(colorArray[0], colorArray[1],colorArray[2]);
    Color materialColor = hit.getMaterial()->materialColor(hit, time);
    return materialColor * (light + traceRay(ray, time));

}

std::vector<Shape*> Scene::getLights(int time) const{
    static std::vector<Shape*> li;
    if (li.size() == 0){
        for (auto shape : shapes) {
            auto emit = shape->matl->emmitColor(HitRecord(DBL_MAX), time);
            if (emit.r() > 0.0 || emit.b() > 0.0 || emit.g() > 0.0){
                li.push_back(shape);
            }
        }
    }
    return li;
}

Color Scene::traceRay(const Ray &ray, int time) const {
    HitRecord hit = getClosestIntersection(ray, false, time);
    if (hit.didHit() == false){
        return background->getBackgroundColor(ray);
    }

    if (hit.getMaterial()->isDiffuse() == false){
        return traceRay(hit.getMaterial()->bounce(hit, time), time);
    }

    Color finalColor = Color(0,0,0);
    auto li = getLights(time);

    for (auto l : li){
        Vector normal = hit.getNormal();
        normal.normalize();
        Vector lightDirection = l->getBounds(time).center() - hit.getHitPosition();
        double dist = lightDirection.normalize();
        double cosphi = Dot(normal, lightDirection);
        if (cosphi > 0){
            finalColor += (l->matl->emmitColor(hit, time) * cosphi);
            Ray shadowRay(hit.getHitPosition(), lightDirection);
            for (auto s : shapes){
                HitRecord shadowHit = s->intersect(shadowRay, dist, time);
                if (shadowHit.didHit() && s != l){
                    finalColor -= (l->matl->emmitColor(hit, time) * cosphi);
                }
            }
        }
    }
    return finalColor * hit.getMaterial()->materialColor(hit, time);
}

Color Scene::tracePath(const Ray &ray, int depth, int time) const{

    //Detemine if there is a hit
    HitRecord hit = getClosestIntersection(ray, true, time);

    if (!hit.didHit()){
        return background->getBackgroundColor(ray);
    }

    Color finalColor = hit.getMaterial()->emmitColor(hit, time);

    double survival = 1.0;

    const int maxRayDepth = 10;
    if (depth > maxRayDepth) {
        return Color(0,0,0);
    }

    if (finalColor.r() > 0.0 || finalColor.g() > 0.0 || finalColor.b() > 0.0){
        return finalColor;
    }

    Ray newRay = hit.getMaterial()->bounce(hit, time);
    Color matColor = hit.getMaterial()->materialColor(hit, time);
    Color newRayReflectColor = tracePath(newRay, depth+1, time);

    //Uniform
    //double cosphi = 2.0 * Dot(hit.getNormal(), newRay.direction());
    //finalColor += ((matColor * newRayReflectColor) * cosphi) * survival;
    //Importance
    finalColor += (matColor * newRayReflectColor) * survival;

    return finalColor;
}

