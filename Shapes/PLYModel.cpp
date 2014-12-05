#include "PLYModel.h"
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


PLYModel::PLYModel(Material *material, std::string filename) : Shape(material){
    parse(filename);
}

PLYModel::~PLYModel() {}

HitRecord PLYModel::intersect(const Ray &ray, const double dist, const double time) const {
  return shapes->intersect(ray, dist, time);
}

Vector PLYModel::get2DVector(const Vector vector, const double time) const {
    return vector;
}

BoundingBox PLYModel::getBounds(const double time) const {
    return shapes->getBounds(time);
}

void PLYModel::parse(std::string filename){
  std::ifstream infile;
  std::string line;
  infile.open (filename);

  int numVertices;
  int numTriangles;

  std::vector<double>points;

  if (infile.is_open()){

    std::string word;

    //Parse header
    while (line.find("end_header") == std::string::npos){
      if (line.find("element vertex") != std::string::npos){
        std::stringstream stream (line);

        stream >> word >> word >> word;
        numVertices = atoi(word.c_str());
      }

      if (line.find("element face") != std::string::npos){
        std::stringstream stream (line);
        std::string word;

        stream >> word >> word >> word;
        numTriangles = atoi(word.c_str());
      }
      getline(infile, line);
    }

    //Parse points:
    for (int i=0; i < numVertices; i++){

      infile >> word;
      double x = atof(word.c_str());

      infile >> word;
      double y = atof(word.c_str());

      infile >> word;
      double z = atof(word.c_str());

      points.push_back(x);
      points.push_back(y);
      points.push_back(z);

      getline(infile, line);
    }

    auto triangles = new std::vector<Shape*>();
    for (int i=0; i < numTriangles; i++) {

      infile >> word >> word;
      int pointA = atoi(word.c_str())*3;

      infile >> word;
      int pointB = atoi(word.c_str())*3;

      infile >> word;
      int pointC = atoi(word.c_str())*3;

      Vector A = Vector(points[pointA], points[pointA+1], points[pointA+2]);
      Vector B = Vector(points[pointB], points[pointB+1], points[pointB+2]);
      Vector C = Vector(points[pointC], points[pointC+1], points[pointC+2]);

      //A.normalize();
      //B.normalize();
      //C.normalize();
      //std::cout << "Adding Triangle " << A << " " << B << " " << C << std::endl;
      triangles->push_back(new Triangle(matl, A,B,C));
      //triangles->push_back(new Triangle(matl, A,C,B));

      getline(infile, line);
    }

    shapes = new ShapeGroup(triangles);

    infile.close();



  }
}