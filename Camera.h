#ifndef Camera_h
#define Camera_h

#include "Vector.h"
#include "Ray.h"
#include "Scene.h"
#include "Image.h"

class Scene;

enum CameraMode { RayTrace, PathTrace, PhotonMapTrace };

class Camera {
public:
    Camera(const Vector &position, const Vector &lookAt, const unsigned int xResolution, const unsigned int yResolution);

    virtual ~Camera();

    virtual Ray makeRay(double x, double y, double r1, double r2) const;

    virtual Image* render(Scene *scene, double time);

    Vector position = Vector();
    Vector lookAt = Vector();
    Vector up = Vector(0.0, 1.0, 0.0);
    Vector right = Vector(1.0, 0.0, 0.0);
    double apertureSize = 0.0;
    double focalLength = 1.0;
    int sampleCount = 1;
    CameraMode cameraMode = RayTrace;
    unsigned int fieldOfView = 90;
    unsigned int xResolution = 640;
    unsigned int yResolution = 480;

private:
    Camera(const Camera &);

    Color samplePixel(const Scene* scene, double x, double y, int time) const;
};

#endif

