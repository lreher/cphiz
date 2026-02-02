#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

// Camera struct
typedef struct {
    Vector3 position;
    float yaw;
    float pitch;
} CameraControl;

// Initialize camera
void camera_init(CameraControl *camControl, Camera3D *cam3d, Vector3 position);

// Update camera target for raylib
void camera_apply_control(Camera3D *cam3d, CameraControl *ctrl);

// Handle camera input
void process_camera_input(CameraControl *ctrl, float dt);

#endif