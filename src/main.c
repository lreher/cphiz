#include <stdio.h>

#include "vector.h"
#include "particle.h"
#include <camera.h>
#include <input.h>
#include <utils.h>

#include <math.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define MAX_PARTICLES 10000

// Slider Values
float explosionForceSlider = 20.0f;
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
    p->acceleration = (V3){0.0f, -1.0f, 0.0f}; // gravity
    p->inverseMass = rand_float(0.3f, 1.2f);   // mass = 2kg
    p->damping = 0.99f;      // slight damping
}

int main(void) {
    init_random();
    SetTraceLogLevel(LOG_NONE); // set no logging

    // Window
    InitWindow(1200, 800, "Physics Demo");

    // Camera
    CameraControl cameraCtrl;
    camera_init(&cameraCtrl, (Vector3){0, 2, 10});

    Camera3D cam3d = {0};
    cam3d.position = cameraCtrl.position;
    cam3d.up       = (Vector3){0,1,0};
    cam3d.fovy     = 60.0f;
    cam3d.projection = CAMERA_PERSPECTIVE;

    // Create Physics Particles
    Particle p1;
    resetParticle(&p1);

    Particle particles[MAX_PARTICLES];
    for (int i = 0; i < (int)particlesSlider; i++) {
        resetParticle(&particles[i]);
    }

    // Set FPS
    SetTargetFPS(60);
    double lastTime = GetTime();

    while (!WindowShouldClose()) {
        int numParticles = (int)particlesSlider;

        // Handle input
        if (IsKeyPressed(KEY_SPACE)) {
            for (int i = 0; i < numParticles; i++) {
                Particle *p = &particles[i];
                resetParticle(p);
            }
        }

        // Get delta time
        float dt = GetFrameTime();

        // Keyboard input
        process_input(&cameraCtrl, dt);

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

            float radius = 1.0f / p->inverseMass / 10.0f;

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
        
        DrawText("Funky Physics Demo", 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// void test_vector() {
//     // Initialize 0 vector
//     printf("0 Vector\n");
//     Vector3 v0 = v3_zero();
//     v3_print(v0);

//     // Initialize specific vector
//     printf("V1 Vector\n");
//     Vector3 v1 = v3_create(1.0f, 1.2f, 1.4f);
//     v3_print(v1);

//     // Initialize specific vector with short notation
//     printf("V2 Vector\n");
//     Vector3 v2 = {0.2f, 0.1f, 0.0f};
//     v3_print(v2);

//     // Invert vector
//     printf("Invert V1\n");
//     v3_invert(&v1);
//     v3_print(v1);

//     // Normalize vector
//     printf("Normalize V1\n");
//     v3_normalize(&v1);
//     v3_print(v1);

//     // Miltiply vector
//     printf("Multiply V1 * 10\n");
//     v3_multiply(&v1, 10.0f);
//     v3_print(v1);

//     // Get multiplied vector
//     printf("Get V1 * 10\n");
//     v3_print(v3_multiplied(v1, 10.0f));
//     v3_print(v1); // does not modify original

//     // Add vector
//     printf("Add V1 + V(10)\n");
//     Vector3 toAdd = {
//         10.0f,
//         10.0f,
//         10.0f
//     };
//     v3_add(&v1, toAdd);
//     v3_print(v1);

//     // Get added vector
//     printf("GET V1 + V(10)\n");
//     v3_print(v3_added(v1, toAdd));
//     v3_print(v1); // does not modify original

//     printf("Add scaled\n");
//     v3_add_scaled(&v1, toAdd, 10);
//     v3_print(v1);

//     // Subtract vector
//     printf("Subtract V1 - V(3)\n");
//     Vector3 toSub = {
//         3.0f,
//         3.0f,
//         3.0f
//     };
//     v3_subtract(&v1, toSub);
//     v3_print(v1);

//     // Get subtracted vector
//     printf("GET V1 - V(3)\n");
//     v3_print(v3_subtracted(v1, toSub));
//     v3_print(v1); // does not modify original

//     // Component product
//     printf("Component Product V1 * V(2)\n");
//     Vector3 mult = {
//         2.0f,
//         2.0f,
//         2.0f
//     };
//     v3_component_product(&v1, mult);
//     v3_print(v1);

//     // Get component product 
//     printf("GET Component Product V1 * V(2)\n");
//     v3_print(v3_get_component_product(v1, mult));
//     v3_print(v1); // does not modify original

//     // Scalar product
//     printf("Scalar Product V1 * V(20)\n");
//     Vector3 scalar = {
//         20.0f,
//         20.0f,
//         20.0f
//     };

//     printf("%f\n", v3_scalar_product(v1, scalar));

//     // Vector product 
//     Vector3 vp1 = {
//         2.0f,
//         2.0f,
//         2.0f
//     };
//     Vector3 vp2 = {
//         1.0f,
//         2.0f,
//         3.0f
//     };

//     // Get vector product 
//     printf("GET Vector Product V1 * V2\n");
//     v3_print(v3_get_vector_product(vp1, vp2));
//     v3_print(vp1); // does not modify original

//     // Modifying original vector product
//     printf("Vector Product V1 * V2\n");
//     v3_vector_product(&vp1, vp2);
//     v3_print(vp1);

// }