#include <stdio.h>
#include <math.h>

#include <vector.h>
#include <particle.h>
#include <camera.h>
#include <utils.h>

#include "raylib.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#pragma clang diagnostic pop

#define MAX_ROCKETS 1000

#define EXPLOSION_PARTICLES 2000
#define MAX_EXPLOSION_PARTICLES 1000000

// Slider Values
float rocketInterval = 2.0f;
float rocketIntervalSliderNorm = 0.5f;

float explosionLifetimeSlider = 1.0f;
float rocketSpeedSlider = 1.5f;

// Structs
typedef struct Rocket {
    Particle particle;   
    float life;
    bool active;
} Rocket;

typedef struct ExplosionParticle {
    Particle particle;   // your existing particle
    float startingLife; // seconds given
    float life;   // seconds remaining
    bool active;  // is it alive?
} ExplosionParticle;

// Create Explosion Particles
ExplosionParticle explosionParticles[MAX_EXPLOSION_PARTICLES];

Texture2D particleTexture;

void shootRocket (Rocket* rocket) {
    rocket->active = true;
    rocket->life = 2.0f;

    Particle *p = &rocket->particle;

    p->position = (V3){0.0f, 1.0f, 0.0f};  // starting slightly above ground
    p->velocity = (V3){
        rand_float(-5 * rocketSpeedSlider, 5 * rocketSpeedSlider), 
        rand_float(10 * rocketSpeedSlider, 15 * rocketSpeedSlider),
        rand_float(-5 * rocketSpeedSlider, 5 * rocketSpeedSlider),
    };
    p->acceleration = (V3){0.0f, -9.8f, 0.0f}; // gravity
    p->damping = 0.99f;      // slight damping

    particle_set_mass(p, 1.0f); // 1kg
}

// Bad
int particle_index = 0;
void explode (V3 position) {
    for (int i = particle_index; i < particle_index + EXPLOSION_PARTICLES; i++) {
        ExplosionParticle *ep = &explosionParticles[i];
        ep->active = true;
        ep->startingLife = explosionLifetimeSlider;
        ep->life = explosionLifetimeSlider;

        V3 dir = v3_random_vector();
        float speed = rand_float(2.0f, 10.0f);
        ep->particle.velocity = v3_multiplied(dir, speed);
        
        ep->particle.position = position;
        // ep->particle.acceleration = (V3){rand_float(-5.0f, 5.0f), rand_float(-5.0f, 5.0f), rand_float(-5.0f, 5.0f)};
        ep->particle.acceleration = (V3){0.0f, -9.8f, 0.0f};
        ep->particle.damping = rand_float(0.2, 0.98);

        particle_set_mass(&ep->particle, 0.1f);
    }

    particle_index += EXPLOSION_PARTICLES;
    if (particle_index >= MAX_EXPLOSION_PARTICLES) {
        particle_index = 0;
    }
}

int main(void) {
    init_random();
    SetTraceLogLevel(LOG_NONE); // set no logging

    // Window
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Physics Demo");

    // InitWindow(1200, 800, "Physics Demo - Fireworks");
    // ToggleFullscreen();

    // Camera
    Camera3D cam3d = {0};
    CameraControl cameraCtrl;
    camera_init(&cameraCtrl, &cam3d, (Vector3){0, 20, 40});

    // Create Rockets
    Rocket rockets[MAX_ROCKETS];
    for (int i = 0; i < MAX_ROCKETS; i++) {
        rockets[i].active = false;
    }
    
    float rocketTimer = 0; // ROCKET_INTERVAL - 1.0f; // start shooting
    int currentRocket = 0;

    // Set up particle for fireworks
    Color fireworkColor = ORANGE;
    Image img = GenImageColor(16, 16, BLANK);

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            float dx = x - 7.5f;
            float dy = y - 7.5f;
            float dist = sqrtf(dx*dx + dy*dy) / 7.5f;
            float alpha = 1.0f - dist;
            if (alpha < 0.0f) alpha = 0.0f;

            ImageDrawPixel(&img, x, y,
                (Color){255, 255, 255, (unsigned char)(255 * alpha)});
        }
    }

    particleTexture = LoadTextureFromImage(img);
    UnloadImage(img);

    // Set FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Get delta time
        float dt = GetFrameTime();

        // Set camera & handle inputf
        process_camera_input(&cameraCtrl, dt);
        camera_apply_control(&cam3d, &cameraCtrl);

        // Shoot rocket every x seconds
        rocketTimer += dt;

        // Explode
        if (rocketTimer >= rocketInterval) {
            shootRocket(&rockets[currentRocket++ % MAX_ROCKETS]);
            rocketTimer = 0.0f;
        }

        // Calculate rocket position and lifetime
        for (int i = 0; i < MAX_ROCKETS; i++) {
            if (!rockets[i].active) continue;

            Rocket *r = &rockets[i];

            particle_integrate(&r->particle, dt);
            r->life -= dt;

            if (r->life <= 0.0f) {
                r->active = false;  // disappear

                explode(r->particle.position);
                continue;
            }
        }

        // Calculate explosion particles position
        for (int i = 0; i < MAX_EXPLOSION_PARTICLES; i++) {
            ExplosionParticle *ep = &explosionParticles[i];
            if (!ep->active) continue;

            particle_integrate(&ep->particle, dt);
            ep->life -= dt;

            if (ep->life <= 0.0f) {
                ep->active = false;
            }
        }

        /* Render */ 
        BeginDrawing();
        ClearBackground(BLACK);

        // Labels
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(GREEN));

        GuiLabel((Rectangle){ 340, 40, 100, 20 }, "Particle Rate");
        GuiSlider((Rectangle){ 340, 60, 200, 20 }, "0.01s", "3s", &rocketIntervalSliderNorm, 0.0f, 1.0f);
        rocketInterval = log_map(rocketIntervalSliderNorm, 0.01f, 3.0f);

        GuiLabel((Rectangle){ 340, 100, 100, 20 }, "Explosion Lifetime");
        GuiSlider((Rectangle){ 340, 120, 200, 20 }, "0.5s", "5s", &explosionLifetimeSlider, 0.5f, 5.0f);

        GuiLabel((Rectangle){ 340, 160, 100, 20 }, "Rocket Speed");
        GuiSlider((Rectangle){ 340, 180, 200, 20 }, "1", "100", &rocketSpeedSlider, 0.5f, 4.0f);

        GuiColorPicker((Rectangle){ 50, 40, 200, 200 }, "Firework Color", &fireworkColor);

        BeginMode3D(cam3d);

        // Mesh sphere = GenMeshSphere(1.0f, 8, 8); // low res
        // Model sphereModel = LoadModelFromMesh(sphere);

            // // Draw Rockets
            // for (int i = 0; i < MAX_ROCKETS; i++) {
            //     if (!rockets[i].active) continue;

            //     DrawModel(
            //         sphereModel,
            //         V3_TO_RAYLIB(rockets[i].particle.position),
            //         0.01f,
            //         WHITE
            //     );
            // }

            // Draw Explosion Particles
            BeginBlendMode(BLEND_ADDITIVE); // make it look snazzy
            for (int i = 0; i < MAX_EXPLOSION_PARTICLES; i++) {
                ExplosionParticle *ep = &explosionParticles[i];
                if (!ep->active) continue;

                float t = ep->life / ep->startingLife;

                Color color = fireworkColor;
                color.a = (unsigned char)(255 * t);   // fade over lifetime

                DrawBillboard(
                    cam3d,
                    particleTexture,
                    V3_TO_RAYLIB(ep->particle.position),
                    0.1f,   // e.g. 0.1f
                    color
                );
            }
            EndBlendMode();


            DrawGrid(5, 1);

        EndMode3D();
        
        DrawText("Fireworks Physics Demo", 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}