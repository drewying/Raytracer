#include <iostream>
#include "MetalMaterial.h"
#include "../HitRecord.h"
#include "../Lights/Light.h"
#include "../Shapes/Shape.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../MathFunc.h"
#include "../Sample.h"
#include "GlassMaterial.h"

GlassMaterial::GlassMaterial(double indexOfRefraction, double reflection){
    this->indexOfRefraction = indexOfRefraction;
    this->reflection = reflection;
}

GlassMaterial::~GlassMaterial() {}

Color GlassMaterial::materialColor(const HitRecord &hit, const double time) const{
    return Color(1.0, 1.0, 1.0);
}

/*Color GlassMaterial::reflect(const HitRecord &hit, const double time) const{
    Vector normal = hit.getNormal();
    normal.normalize();
    Vector in_dir = hit.getRay().direction();
    double nt = this->indexOfRefraction;
    double R0 = (nt - 1.0f) / (nt + 1.0f);
    R0 *= R0;

    double brdf = 1.0;
    float R, P;
    float cosine = Dot(in_dir, normal);

    if (cosine < 0.0f) // ray is incoming
    {
        // determine reflectance
        cosine = -cosine;
        float temp1 = 1.0f - cosine;
        R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;
        P = (R + 0.5f) / 2.0f;

        static std::random_device rd;
        static std::default_random_engine generator(rd());
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);

        double random = distribution(generator);

        if (random <= P)  // reflect
        {
            brdf  = R / P;
        }
        else              // transmit
        {
            brdf  = (1.0f - R) / (1.0f - P);
        }
    }
    else // ray is outgoing
    {

        double temp2 = -(Dot(in_dir, -normal));
        double root = 1.0f - (nt * nt) * (1.0f - temp2 * temp2);
        if (root < 0.0f) // total internal reflection
        {
            brdf = 1.0f;
        }
        else
        {
            // determine reflectance
            float temp1 = 1.0f - cosine;
            R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;
            P = (R + 0.5f) / 2.0f;

            static std::random_device rd;
            static std::default_random_engine generator(rd());
            static std::uniform_real_distribution<double> distribution(0.0, 1.0);
            double random = distribution(generator);
            if (random <= P)  // reflect
            {
                brdf =  R / P;
            }
            else              // transmit
            {
                brdf = (1.0f - R) / (1.0f - P);
            }
        }
    }
    Color(brdf, brdf, brdf);
}*/


Color GlassMaterial::emmitColor(const HitRecord &hit, const double time) const{
    return Color(0.0, 0.0, 0.0);
}

Ray GlassMaterial::bounce(const HitRecord &hit, double time) const{
    Vector normal = hit.getNormal();
    normal.normalize();
    Vector incident = hit.getRay().direction();

    static std::random_device rd;
    static std::default_random_engine generator(rd());
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double random = distribution(generator);

    Vector nl = Dot(normal, incident) < 0 ? normal : normal * -1.0;
    double into = Dot(nl, normal);
    double refractiveIndexAir = 1;
    double refractiveIndexGlass = 1.5;
    double refractiveIndexRatio = pow(refractiveIndexAir / refractiveIndexGlass, (into > 0) - (into < 0));
    double cosI = Dot(incident,nl);
    double cos2t = 1 - refractiveIndexRatio * refractiveIndexRatio * (1 - cosI * cosI);
    if (cos2t < 0) {
        //Reflect
        return Ray(hit.getHitPosition(), incident - normal * (2 * Dot(incident, normal)));
    }
    Vector refractedDirection = incident * (refractiveIndexRatio) - (normal * (((into > 0) - (into < 0) * (cosI * refractiveIndexRatio + sqrt(cos2t)))));
    refractedDirection.normalize();

    double a = refractiveIndexGlass - refractiveIndexAir;
    double b = refractiveIndexGlass + refractiveIndexAir;
    double R0 = a * a / (b * b);
    double c = 1 - (into > 0 ? -cosI : Dot(refractedDirection,normal));
    double Re = R0 + (1 - R0) * pow(c, 5);
    double P = reflection + 0.5 * Re;
    //Prob check?
    if (random  < P){
        return Ray(hit.getHitPosition(), incident - normal * (2 * Dot(incident, normal)));
    } else{
        return Ray(hit.getHitPosition(), refractedDirection);
    }

}

/*Ray GlassMaterial::bounce(const HitRecord &hit, double time) const{
    Vector normal = hit.getNormal();
    normal.normalize();
    Vector direction = hit.getRay().direction();

    static std::random_device rd;
    static std::default_random_engine generator(rd());
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double random = distribution(generator);

    double eta = this->indexOfRefraction;
    double R0 = (eta - 1.0f) / (eta + 1.0f);
    R0 *= R0;
    Vector outRay;
    double R, P;
    double cosine = Dot(direction, normal);

    if (cosine < 0.0f) // Ray is incoming
    {
        cosine = -cosine;
        double temp1 = 1.0f - cosine;
        R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;
        P = (R + 0.5f) / 2.0f;

        if (random <= P){
            outRay = direction - normal * (2 * Dot(direction, normal));
        }
        else {
            eta = 1.0f / eta;
            double root  = 1.0f - (eta * eta) * (1.0f - cosine * cosine);
            outRay = direction * eta + normal * (eta * cosine - sqrt(root));
        }
    }
    else { //Ray is outgoing (flipped normal)
        normal = -normal;
        double temp2 = -(Dot(direction, normal));
        double root = 1.0f - (eta * eta) * (1.0f - temp2 * temp2);
        if (root < 0.0f) {
            outRay = direction - normal * (2 * Dot(direction, normal));
        }
        else {
            // determine reflectance
            double temp1 = 1.0f - cosine;
            R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;
            P = (R + 0.5f) / 2.0f;

            double random = distribution(generator);
            if (random <= P){
                outRay = direction - normal * (2 * Dot(direction, normal));
            } else {
                outRay = direction * eta + normal * (eta * temp2 - sqrt(root));
            }
        }
    }
    return Ray(hit.getHitPosition(), outRay);
}*/