#include <stdio.h>
#include <math.h>

#include "vector.h"
#include "particle.h"
#include <camera.h>
#include <utils.h>

#include "raylib.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#pragma clang diagnostic pop

#define MAX_PARTICLES 10000

// Slider Values
float explosionForceSlider = 100.0f;
float particlesSlider = 100.0f;
float sphereSizeSlider = 0.5f;

void resetParticle(Particle* p) {
    p->position = (V3){0.0f, 1.0f, 0.0f};  // starting slightly above ground
    p->velocity = (V3){0.0f, 0.0f, 0.0f};
    p->force = (V3){ 
        rand_float(-explosionForceSlider, explosionForceSlider), 
        rand_float(explosionForceSlider / 2, explosionForceSlider),
        rand_float(-explosionForceSlider, explosionForceSlider),
    }; 
    p->acceleration = (V3){0.0f, -9.8f, 0.0f}; // gravity
    p->damping = 0.99f;      // slight damping

    particle_set_mass(p, rand_float(0.5f, 2.0f)); // 0.5 - 2 kg
}

int main(void) {
    init_random();
    SetTraceLogLevel(LOG_NONE); // set no logging

    // Window
    InitWindow(1200, 800, "Physics Demo - Explosion");

    // Camera
    Camera3D cam3d = {0};
    CameraControl cameraCtrl;
    camera_init(&cameraCtrl, &cam3d, (Vector3){0, 2, 5});

    // Create Physics Particles
    Particle particles[MAX_PARTICLES];
    for (int i = 0; i < (int)particlesSlider; i++) {
        resetParticle(&particles[i]);
    }

    // Set FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Get delta time
        float dt = GetFrameTime();

        int numParticles = (int)particlesSlider;
        
        // Handle input
        if (IsKeyPressed(KEY_SPACE)) {
            for (int i = 0; i < numParticles; i++) {
                Particle *p = &particles[i];
                resetParticle(p);
            }
        }

        // Set camera & handle input
        process_camera_input(&cameraCtrl, dt);
        camera_apply_control(&cam3d, &cameraCtrl);

        /* Render */ 
        BeginDrawing();
        ClearBackground(BLACK);

        // Labels
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(GREEN));

        GuiLabel((Rectangle){ 50, 40, 100, 20 }, "Explosion Force");
        GuiSlider((Rectangle){ 50, 60, 200, 20 }, "10N", "1000N", &explosionForceSlider, 10.0f, 1000.0f);

        GuiLabel((Rectangle){ 50, 100, 100, 20 }, "Particles");
        GuiSlider((Rectangle){ 50, 120, 200, 20 }, "1", "10000", &particlesSlider, 1.0f, (float)MAX_PARTICLES);

        GuiLabel((Rectangle){ 50, 160, 100, 20 }, "Sphere Size");
        GuiSlider((Rectangle){ 50, 180, 200, 20 }, "0.1", "2", &sphereSizeSlider, 0.1f, 2.0f);

        // Spheres
        BeginMode3D(cam3d);

        Mesh sphere = GenMeshSphere(sphereSizeSlider, 8, 8); // low res
        Model sphereModel = LoadModelFromMesh(sphere);

        for (int i = 0; i < numParticles; i++) {
            Particle *p = &particles[i];
            
            particle_integrate(p, dt);

            float radius = particle_get_mass(p) / 10.0f;

            real totalSpeed = v3_magnitude(&p->velocity) * 15;

            if (totalSpeed > 255) totalSpeed = 255;
            Color color = { 255, totalSpeed, 0, 255 };

            DrawModel(sphereModel, V3_TO_RAYLIB(p->position), radius, color);

            // bounce
            if (particles[i].position.y < 0.0f 
              && particles[i].position.x <= 50.0f
              && particles[i].position.x >= -50.0f
              && particles[i].position.z <= 50.0f
              && particles[i].position.z >= -50.0f) {
                particles[i].position.y = 0.0f; // stay on floor
                particles[i].velocity.y *= -0.5f; // bounce with damping
                particles[i].velocity.x *= 0.8f; // bounce with damping
                particles[i].velocity.z *= 0.8f; // bounce with damping
            }
        }

        DrawGrid(100, 1);
        // DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){50.0f, 50.0f}, WHITE);

        EndMode3D();
        
        DrawText("Explosion Physics Demo", 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}