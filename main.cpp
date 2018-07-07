#include "Backgrounds/Background.h"
#include "Backgrounds/ColorBackground.h"
#include "Backgrounds/TextureBackground.h"
#include "Camera.h"
#include "Color.h"
#include "Image.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/LightMaterial.h"
#include "Materials/MetalMaterial.h"
#include "Materials/BitmapTextureMaterial.h"
#include "Materials/MarbleMaterial.h"
#include "Materials/Material.h"
#include "Lights/Light.h"
#include "Shapes/Shape.h"
#include "Camera.h"
#include "Lights/LightPoint.h"
#include "Ray.h"
#include "Scene.h"
#include "Time.h"
#include "Shapes/Sphere.h"
#include "Shapes/Triangle.h"
#include "Shapes/Plane.h"
#include "Shapes/ShapeInstance.h"
#include "Shapes/BezierPatch.h"
#include "Shapes/Point.h"
#include "Animateable.h"
#include "Shapes/PLYModel.h"
#include "Materials/ColorMaterial.h"
#include "Materials/GlassMaterial.h"
#include "Shapes/BezierCurve.h"
#include "Shapes/OBJModel.h"
#include "Shapes/BPTModel.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <thread>

Scene* setupScene();
void takePicture(Camera* camera, Scene *scene, int timePosition);
void takeMovie(Camera *camera, Scene *scene, int seconds);

int main(int argc, char **argv) {

    Vector position      = Vector(0.0, 0.0,  1.0);
    Vector lookAt        = Vector(0.0, 0.0, -1.0);
    Camera *camera       = new Camera(position, lookAt, 800, 600);
    camera->cameraMode   = RayTrace;
    camera->focalLength  = 51.47;
    camera->apertureSize = 0.0;
    camera->fieldOfView  = 90;
    camera->sampleCount  = 1;

    Scene *scene = setupScene();
    takePicture(camera, scene, 1);
    //takeMovie(camera, scene, 30);

    return 0;
}

void takeMovie(Camera *camera, Scene *scene, int seconds){
    // system("rm out.mpeg");

    double frames = seconds * 30;

    double t1 = Time::currentSeconds();

    std::cout << "Beginning Movie Render..." << std::endl;

    int i=1;

    while (i <= frames){
        std::vector<std::thread> threads;
        for (int j=0; j<4; j++){

            threads.push_back(std::thread([=](){
                Image *image = camera->render(scene, i);
                image->write("image"+std::to_string(i)+".ppm");
                delete image;
            }));

            i++;

            if (i > frames){
                break;
            }

        }

        for (auto& t : threads){
            t.join();
        }

        std::cout << "Completed " << (i/frames)*100 << "%" << std::endl;
    }

    double t4 = Time::currentSeconds();
    system("/usr/local/bin/ffmpeg -framerate 30 -start_number 1 -i image%d.ppm -b:v 1024k -r 30 out.mpeg");
    system("rm *.ppm");

    std::cout << "Render finished. Total time:" << std::setprecision(3) << t4 - t1 << " seconds" << std::endl;

}

void takePicture(Camera* camera, Scene *scene, int timePosition){
    double t1 = Time::currentSeconds();
    std::cout << "Beginning Picture Render..." << std::endl;
    Image *image = camera->render(scene, timePosition);
    image->write("out.bmp");
    double t4 = Time::currentSeconds();
    std::cout << "Render finished. Total time:" << std::setprecision(3) << t4 - t1 << " seconds" << std::endl;
}

Scene* setupScene(){

    const Color DARK_RED = Color(252.0/255.0, 40.0/255.0, 85.0/255.0);
    const Color YELLOW = Color(231.0/255.0, 203.0/255.0, 90.0/255.0);
    const Color LIGHT_BLUE = Color(35.0/255.0, 185.0/255.0, 194.0/255.0);
    const Color DARK_ORANGE = Color(245.0/255.0, 127.0/255.0, 77.0/255.0);
    const Color LIGHT_ORANGE = Color(253.0/255.0, 176.0/255.0, 97.0/255.0);
    const Color TURQOISE = Color(106.0/255.0, 231.0/255.0,194.0/255.0);
    const Color DARK_GREEN = Color(124.0/255.0, 172.0/255.0, 142.0/255.0);
    const Color DARK_BLUE = Color(56.0/255.0,  93.0/255.0, 114.0/255.0);

    const Color JADE_MAIN = Color(40.0/255.0, 150.0/255.0, 105/255.0);
    const Color JADE_ONE = Color(30.0/255.0, 130.0/255.0, 100/255.0);
    const Color JADE_TWO = Color(120.0/255.0, 200.0/255.0, 150/255.0);

    const Color WHITE = Color(0.75, 0.75, 0.75);
    const Color BLUE = Color(0.25,0.25,0.75);
    const Color GREEN = Color(0.25, 0.75, 0.25);
    const Color RED = Color(0.75,0.25,0.25);

    MarbleMaterial *marbleMaterial = new MarbleMaterial(0.3, 5, 8);
    marbleMaterial->color0 = JADE_MAIN;
    marbleMaterial->color1 = JADE_ONE;
    marbleMaterial->color2 = JADE_TWO;


    ColorBackground *background = new ColorBackground(Color(0.0,1.0,0.0));
    Vector pointA = Vector(-15.0,29.0,-20.0);
    Vector pointB = Vector( -5.0,29.0,-20.0);
    Vector pointC = Vector(  5.0,29.0,-20.0);
    Vector pointD = Vector( 15.0,29.0,-20.0);

    Vector pointE = Vector( 10.0,29.0,-15.0);
    Vector pointF = Vector(  10.0,29.0,-15.0);

    Vector pointG = Vector(-15.0,29.0,-10.0);
    Vector pointH = Vector( -5.0,29.0,-10.0);
    Vector pointI = Vector(  5.0,29.0,-10.0);
    Vector pointJ = Vector( 15.0,29.0,-10.0);



    LightMaterial *lightMaterial = new LightMaterial(Color(50.0, 50.0, 50.0));
    Triangle *light  = new Triangle(lightMaterial, pointA, pointE, pointG);
    Triangle *light1 = new Triangle(lightMaterial, pointB, pointH, pointE);
    Triangle *light2 = new Triangle(lightMaterial, pointC, pointF, pointI);
    Triangle *light3 = new Triangle(lightMaterial, pointD, pointJ, pointF);

    Sphere *sphere =  new Sphere(new GlassMaterial(1.5, 0.1), Vector( 0.35, -0.7, -0.5), 0.3);
    Sphere *sphere1 = new Sphere(new MetalMaterial(),         Vector(-0.5, -0.7, -0.7), 0.3);
    //Sphere *glassSphere = new Sphere(new GlassMaterial(1.5, 0.1), Vector(4.0,-4.0, -6.0), 4.0);
    //Sphere *glassSphere1 = new Sphere(new GlassMaterial(1.5, 0.1), Vector(-5.0,-4.5, -10.0), 3.5);


    //BPTModel *teapot = new BPTModel(marbleMaterial, "teapot.bpt", true);
    //ShapeInstance *instance = new ShapeInstance(teapot);
    //instance->transformMatrix.addFrame(0, translate(-3.5,-8.0,-4.0) * rotateX(-M_PI/2.0) * rotateZ(4.1) * scale(1.75,1.75,1.75));
    //std::cout << instance->getBounds(0).center() << std::endl;

    //auto bmpTexture = new BitmapTextureMaterial("study1.bmp");

    Plane *left   = new Plane(new ColorMaterial(RED),   Vector(  1.0,   0.0,   0.0), Vector( -1.0,  0.0,   0.0));
    Plane *right  = new Plane(new ColorMaterial(BLUE),  Vector( -1.0,   0.0,   0.0), Vector(  1.0,  0.0,   0.0));
    Plane *back   = new Plane(new ColorMaterial(WHITE), Vector(  0.0,   0.0,  -1.0), Vector(  0.0,  0.0,   1.0));
    Plane *front  = new Plane(new ColorMaterial(WHITE), Vector(  0.0,   0.0,   1.0), Vector(  0.0,  0.0,  -1.0));
    Plane *top    = new Plane(new ColorMaterial(WHITE), Vector(  0.0,  -1.0,   0.0), Vector(  0.0,  1.0,   0.0));
    Plane *bottom = new Plane(new ColorMaterial(WHITE), Vector(  0.0,   1.0,   0.0), Vector(  0.0, -1.0,   0.0));

    Scene *scene = new Scene();

    Point *pointLight = new Point(new LightMaterial(Color(1.0, 1.0, 1.0)), Vector(0.0, 0.8, 0.0));
    scene->addShape(pointLight);
    //scene->addShape(light);
    //scene->addShape(light3);


    scene->addShape(back);
    scene->addShape(front);
    scene->addShape(top);
    scene->addShape(bottom);
    scene->addShape(right);
    scene->addShape(left);

    scene->addShape(sphere);
    scene->addShape(sphere1);

    return scene;
}