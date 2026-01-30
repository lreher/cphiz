#include <stdio.h>

#include "vector.h"
#include "particle.h"
#include <camera.h>
#include <input.h>

#include <math.h>

#include "raylib.h"


int main(void) {

    // Window
    InitWindow(1200, 800, "Physics Demo");

    // Camera
    CameraControl cameraCtrl;
    camera_init(&cameraCtrl, (Vector3){0, 2, 6});

    Camera3D cam3d = {0};
    cam3d.position = cameraCtrl.position;
    cam3d.up       = (Vector3){0,1,0};
    cam3d.fovy     = 60.0f;
    cam3d.projection = CAMERA_PERSPECTIVE;

    // Set FPS
    SetTargetFPS(60);
    double lastTime = GetTime();

    while (!WindowShouldClose()) {

        // Delta time
        double now = GetTime();
        float dt = (float)(now - lastTime);
        lastTime = now;

        // Keyboard input
        process_input(&cameraCtrl, dt);

        // Apply camera
        cam3d.position = cameraCtrl.position;
        camera_apply(&cam3d, &cameraCtrl);

        // Render
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(cam3d);

            DrawGrid(20, 1);
            DrawSphere((Vector3){0, 1, 0}, 1.0f, LIGHTGRAY);
            DrawSphereWires((Vector3){0, 1, 0}, 1.0f, 32, 32, GRAY);

        EndMode3D();

        DrawText("Funky Physics Demo", 10, 10, 20, DARKGRAY);

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