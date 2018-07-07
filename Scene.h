#ifndef Scene_h
#define Scene_h

#include "PhotonMap.h"
#include "Camera.h"
#include "Color.h"
#include "Animateable.h"
#include "Shapes/Shape.h"
#include "Shapes/Sphere.h"
#include "Lights/Light.h"
#include "Backgrounds/Background.h"
#include <string>
#include <vector>

class Scene {
public:
    Scene();

    virtual ~Scene();

    std::vector<Shape*> getShapes() const {
        return shapes;
    }

    void addShape(Shape *obj) {
        shapes.push_back(obj);
    }

    inline Background *getBackground() const {
        return background;
    }

    void setBackground(Background *bg) {
        background = bg;
    }

    Color getAmbient() const {
        return ambient;
    }

    void setAmbient(const Color &amb) {
        ambient = amb;
    }

    Color traceRay(const Ray &ray, int time) const;

    Color tracePath(const Ray &ray, int depth, int time) const;

    Color tracePhoton(const Ray &ray, int time) const;

    void buildPhotonMap(int time);

    void mapPhoton(const Ray &ray, Color power, int depth, int time);

    HitRecord getClosestIntersection(const Ray &ray, bool includeLights, int time) const;

private:

    PhotonMap photonMap = PhotonMap(1000000);
    std::vector<Shape*> getLights(int time) const;
    Scene(const Scene &);
    Scene &operator=(const Scene &);
    Background *background;
    Color ambient;

    std::vector<Shape*> shapes;

};

#endif
