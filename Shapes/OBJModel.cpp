#include "OBJModel.h"
#include "../HitRecord.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../Animateable.h"
#include "../Matrix.h"

#include <string>
#include <float.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>


OBJModel::OBJModel(Material *material, std::string filename) : Shape(material){
    parse(filename);
}

OBJModel::~OBJModel() {}

HitRecord OBJModel::intersect(const Ray &ray, const double dist, const double time) const {
    return shapes->intersect(ray, dist, time);
}

Vector OBJModel::get2DVector(const Vector vector, const double time) const {
    return vector;
}

BoundingBox OBJModel::getBounds(const double time) const {
    return shapes->getBounds(time);
}

void OBJModel::parse(std::string filename){
    std::ifstream infile;
    std::string line;
    infile.open (filename);

    std::vector<double>vertices;
    std::vector<double>textureCoordinates;
    std::vector<double>normals;
    std::vector<int>faces;

    while (std::getline(infile, line)){
        std::string word;
        std::stringstream stream (line);

        //Parse header
        if (line[0] == 'v' && line[1] == 'n'){
            stream >> word >> word;
            double x = atof(word.c_str());

            stream >> word;
            double y = atof(word.c_str());

            stream >> word;
            double z = atof(word.c_str());

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
        else if (line[0] == 'v' && line[1] == 't'){
            stream >> word >> word;
            double x = atof(word.c_str());

            stream >> word;
            double y = atof(word.c_str());

            textureCoordinates.push_back(x);
            textureCoordinates.push_back(y);
            //Textures can be 3
        }
        else if (line[0] == 'v') {
            stream >> word >> word;
            double x = atof(word.c_str());

            stream >> word;
            double y = atof(word.c_str());

            stream >> word;
            double z = atof(word.c_str());

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

        } else if (line[0] == 'f') {
            stream >> word >> word;
            int x = atoi(word.c_str());

            stream >> word;
            int y = atoi(word.c_str());

            stream >> word;
            int z = atoi(word.c_str());

            //stream >> word;
            //int w = atoi(word.c_str());

            //std::cout << line << std::endl;
            //std::cout << x << " " << y << " " << x << " " << w << std::endl;

            faces.push_back(x);
            faces.push_back(y);
            faces.push_back(z);
            //faces.push_back(w);
        }
    }

    infile.close();

    auto triangles = new std::vector<Shape*>();
    for (int i=0; i < faces.size(); i+=3) {

        int pointA = (faces[i] * 3)-3;
        int pointB = (faces[i+1] * 3)-3;
        int pointC = (faces[i+2] * 3)-3;

        //pointA *= 3;

        //std::cout << i << " " << pointA << std::endl;

        Vector A = Vector(vertices[pointA], vertices[pointA+1], vertices[pointA+2]);
        Vector B = Vector(vertices[pointB], vertices[pointB+1], vertices[pointB+2]);
        Vector C = Vector(vertices[pointC], vertices[pointC+1], vertices[pointC+2]);

        //A.normalize();
        //B.normalize();
        //C.normalize();
        //std::cout << "Adding Triangle " << A << " " << B << " " << C << std::endl;
        triangles->push_back(new Triangle(matl, A,B,C));

    }
    shapes = new ShapeGroup(triangles);
}