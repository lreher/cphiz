#ifndef CAMERA_H
#define CAMERA_H

typedef struct
{
    float x, y, z;     /* position */
    float yaw;         /* left/right in radians */
    float pitch;       /* up/down in radians */
} Camera;

void camera_init(Camera* cam);
void camera_apply(const Camera* cam);

#endif