#pragma once

typedef struct {
    float x, y, z;      // position
    float pitch, yaw;   // rotation (degrees)
} Camera;

void camera_apply(const Camera* cam);