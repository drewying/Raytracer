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
    Camera(const Vector &eye, const Vector &lookAt, const Vector &up, const Vector &right);

    virtual ~Camera();

    virtual Ray makeRay(double x, double y, double r1, double r2) const;

    virtual Image* render(Scene *scene, double time);

    Vector eye;
    Vector lookAt;
    Vector up;
    Vector right;
    double apertureSize;
    double focalLength;
    int sampleCount;
    CameraMode cameraMode = PathTrace;
    int xResolution;
    int yResolution;

private:
    Camera(const Camera &);
    Camera &operator=(const Camera &);

    Color samplePixel(const Scene* scene, double x, double y, int time) const;
    void preprocess(double aspect_ratio);
};

#endif

