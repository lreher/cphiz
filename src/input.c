#include "input.h"
#include <math.h>

void process_input(CameraControl *ctrl, float dt) {
    float moveSpeed = 5.0f * dt;
    float rotSpeed  = 1.5f * dt;

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
}