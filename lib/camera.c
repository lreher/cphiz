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

void process_camera_input(CameraControl *ctrl, float dt) {
    float moveSpeed = 5.0f * dt;
    float rotSpeed  = 1.5f * dt;

    // Set speed
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        moveSpeed = 20.0f * dt;
    }
    if (IsKeyUp(KEY_LEFT_SHIFT)) {
        moveSpeed = 5.0f * dt;
    }

    // Rotate camera
    if (IsKeyDown(KEY_LEFT))  ctrl->yaw   -= rotSpeed;
    if (IsKeyDown(KEY_RIGHT)) ctrl->yaw   += rotSpeed;
    if (IsKeyDown(KEY_UP))    ctrl->pitch += rotSpeed;
    if (IsKeyDown(KEY_DOWN))  ctrl->pitch -= rotSpeed;

    if (ctrl->pitch > 1.5f)  ctrl->pitch = 1.5f;
    if (ctrl->pitch < -1.5f) ctrl->pitch = -1.5f;

    // Forward/backward vectors
    Vector3 forward = { sinf(ctrl->yaw), 0, -cosf(ctrl->yaw) };
    Vector3 right   = { cosf(ctrl->yaw), 0, sinf(ctrl->yaw) };

    // Move
    if (IsKeyDown(KEY_W)) {
        ctrl->position.x += forward.x * moveSpeed;
        ctrl->position.z += forward.z * moveSpeed;
    }
    if (IsKeyDown(KEY_S)) {
        ctrl->position.x -= forward.x * moveSpeed;
        ctrl->position.z -= forward.z * moveSpeed;
    }
    if (IsKeyDown(KEY_A)) {
        ctrl->position.x -= right.x * moveSpeed;
        ctrl->position.z -= right.z * moveSpeed;
    }
    if (IsKeyDown(KEY_D)) {
        ctrl->position.x += right.x * moveSpeed;
        ctrl->position.z += right.z * moveSpeed;
    }
    if (IsKeyDown(KEY_Q)) {
        ctrl->position.y -= moveSpeed;
    }
    if (IsKeyDown(KEY_E)) {
        ctrl->position.y += moveSpeed;
    }
}
