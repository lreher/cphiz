#include <stdio.h>
#include <math.h>

#include "raylib.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#pragma clang diagnostic pop

#define MAX_PARTICLES 10000


int main(void) {
    init_random();
    SetTraceLogLevel(LOG_NONE); // set no logging

    // Window
    InitWindow(1200, 800, "Physics Demo - Explosion");

    // Camera
    CameraControl cameraCtrl;
    camera_init(&cameraCtrl, (Vector3){0, 2, 10});

    Camera3D cam3d = {0};
    cam3d.position = cameraCtrl.position;
    cam3d.up       = (Vector3){0,1,0};
    cam3d.fovy     = 60.0f;
    cam3d.projection = CAMERA_PERSPECTIVE;

    // Set FPS
    SetTargetFPS(60);
    double lastTime = GetTime();

    while (!WindowShouldClose()) {
        int numParticles = (int)particlesSlider;

        // Get delta time
        float dt = GetFrameTime();

        // Keyboard input
        process_camera_input(&cameraCtrl, dt);

        // Apply camera
        cam3d.position = cameraCtrl.position;
        camera_apply(&cam3d, &cameraCtrl);

        /* Render */ 
        BeginDrawing();
        ClearBackground(BLACK);

        // Labels
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(GREEN));

        GuiLabel((Rectangle){ 50, 40, 100, 20 }, "Explosion Force");
        GuiSlider((Rectangle){ 50, 60, 200, 20 }, "1", "50", &explosionForceSlider, 1.0f, 100.0f);

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
            
            particle_integrate(p, lastTime);

            float radius = particle_get_mass(p) / 10.0f;

            real totalSpeed = v3_magnitude(&p->velocity) * 30;

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