#include <OpenGL/glu.h>
#include "camera.h"
#include <math.h>

#define DEG2RAD(x) ((x) * 0.01745329251f)

void camera_apply(const Camera* camera)
{
    glRotatef(-camera->pitch, 1, 0, 0);
    glRotatef(-camera->yaw,   0, 1, 0);
    glTranslatef(-camera->x, -camera->y, -camera->z);
}