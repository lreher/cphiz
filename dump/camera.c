#include "camera.h"
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#endif

void camera_init(Camera* cam)
{
    cam->x = 0.0f;
    cam->y = 3.0f;
    cam->z = 10.0f;

    cam->yaw   = 3.1415926f; /* facing toward origin */
    cam->pitch = 0.0f;
}

void camera_apply(const Camera* cam)
{
    float fx = cosf(cam->pitch) * sinf(cam->yaw);
    float fy = sinf(cam->pitch);
    float fz = cosf(cam->pitch) * cosf(cam->yaw);

    gluLookAt(
        cam->x, cam->y, cam->z,
        cam->x + fx, cam->y + fy, cam->z + fz,
        0.0f, 1.0f, 0.0f
    );
}