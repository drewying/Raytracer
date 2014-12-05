#include "BPTModel.h"
#include "../HitRecord.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../Animateable.h"
#include "../Matrix.h"
#include "BezierCurve.h"
#include "BezierPatch.h"
#include "ShapeGroup.hpp"

#include <string>
#include <float.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>

BPTModel::BPTModel(Material *material, std::string filename, bool tesselate) : Shape(material){
    parse(filename, tesselate);
}

BPTModel::~BPTModel() {}

HitRecord BPTModel::intersect(const Ray &ray, const double dist, const double time) const {
    return shapes->intersect(ray, dist, time);
}

Vector BPTModel::get2DVector(const Vector vector, const double time) const {
    return vector;
}

BoundingBox BPTModel::getBounds(const double time) const {
    return shapes->getBounds(time);
}

void BPTModel::parse(std::string filename, bool tesselate){
    std::ifstream infile;
    std::string line;
    infile.open (filename);
    std::getline(infile, line);
    auto parsedShapes = new std::vector<Shape*>();
    while (std::getline(infile, line)){
        std::vector<Vector> points(16);
        for (int i=0; i < 16; i++){
            std::getline(infile, line);
            std::stringstream stream (line);
            std::string word;

            stream >> word;
            double x = atof(word.c_str());

            stream >> word;
            double y = atof(word.c_str());

            stream >> word;
            double z = atof(word.c_str());
            points[i] = Vector(x,y,z);
        }
        BezierPatch *patch = new BezierPatch(matl, points);
        if (tesselate){
            auto triangles = patch->tesselate(15);
            parsedShapes->insert(parsedShapes->end(), triangles.begin(), triangles.end());
            delete patch;
        } else{
            parsedShapes->push_back(patch);
        }

    }

    shapes = new ShapeGroup(parsedShapes);
}