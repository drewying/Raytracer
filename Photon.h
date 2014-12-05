#include "Vector.h"
#include "Color.h"

class Photon{
public:
    Vector pos;                 // photon position
    int plane;                  // splitting plane for kd-tree
    unsigned char theta, phi;     // incoming direction
    Color power;               // photon power (uncompressed)
};

typedef struct NearestPhotons {
    int max;
    int found;
    int got_heap;
    Vector pos;
    Vector normal;
    double *dist2;
    const Photon **index;
} NearestPhotons;