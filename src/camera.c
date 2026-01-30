#include "camera.h"
#include <math.h>

void camera_init(CameraControl *cam, Vector3 position) {
    cam->position = position;
    cam->yaw = 0.0f;
    cam->pitch = 0.0f;
}

void camera_apply(Camera3D *cam3d, CameraControl *ctrl) {
    cam3d->target.x = cam3d->position.x + sinf(ctrl->yaw);
    cam3d->target.y = cam3d->position.y + tanf(ctrl->pitch); // small vertical look
    cam3d->target.z = cam3d->position.z - cosf(ctrl->yaw);
}