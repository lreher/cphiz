#include <stdio.h>

#include "vector.h"
#include "particle.h"
#include "camera.h"

#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <math.h>

#define DEG2RAD(x) ((x) * 0.01745329251f)

// INPUT
static void process_input(GLFWwindow* window, Camera* cam, double dt)
{
    const float move_speed = 5.0f;
    const float turn_speed = DEG2RAD(90.0f); /* 90 degrees per second */

    /* ---- rotation ---- */

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cam->yaw += turn_speed * dt;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cam->yaw -= turn_speed * dt;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam->pitch += turn_speed * dt;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam->pitch -= turn_speed * dt;

    /* clamp pitch to avoid flipping */
    const float limit = DEG2RAD(89.0f);
    if (cam->pitch > limit)  cam->pitch = limit;
    if (cam->pitch < -limit) cam->pitch = -limit;

    /* ---- movement ---- */

    float forward_x = sinf(cam->yaw);
    float forward_z = cosf(cam->yaw);

    float right_x = cosf(cam->yaw);
    float right_z = -sinf(cam->yaw);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cam->x += forward_x * move_speed * dt;
        cam->z += forward_z * move_speed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cam->x -= forward_x * move_speed * dt;
        cam->z -= forward_z * move_speed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cam->x -= right_x * move_speed * dt;
        cam->z -= right_z * move_speed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cam->x += right_x * move_speed * dt;
        cam->z += right_z * move_speed * dt;
    }
}

// GEOMETRY
/*
 * Draws a solid sphere using latitude/longitude slices.
 * Replacement for glutSolidSphere.
 */

static void draw_sphere(float radius, int slices, int stacks)
{
    for (int i = 0; i < stacks; i++)
    {
        float lat0 = M_PI * (-0.5f + (float)i / stacks);
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / stacks);

        float y0 = sinf(lat0);
        float y1 = sinf(lat1);

        float r0 = cosf(lat0);
        float r1 = cosf(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++)
        {
            float lon = 2 * M_PI * (float)j / slices;
            float x = cosf(lon);
            float z = sinf(lon);

            glNormal3f(x * r0, y0, z * r0);
            glVertex3f(x * r0 * radius, y0 * radius, z * r0 * radius);

            glNormal3f(x * r1, y1, z * r1);
            glVertex3f(x * r1 * radius, y1 * radius, z * r1 * radius);
        }
        glEnd();
    }
}

static void draw_grid(int size, int step)
{
    glDisable(GL_LIGHTING);
    glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_LINES);
    for (int i = -size; i <= size; i += step)
    {
        glVertex3f((float)i, 0, (float)-size);
        glVertex3f((float)i, 0, (float) size);

        glVertex3f((float)-size, 0, (float)i);
        glVertex3f((float) size, 0, (float)i);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

static void render(Camera* cam, int w, int h)
{
    glViewport(0, 0, w, h);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera_apply(cam);

    /* grid */
    draw_grid(20, 1);

    /* test sphere */
    glColor3f(0.8f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    draw_sphere(1.0f, 24, 16);

    draw_sphere(1.0f, 24, 16);

    glPopMatrix();
}

int main(void) {

    GLFWwindow* window;

    // Init GLFW
    const int glInit = glfwInit();
    if (!glInit) {
        printf("Failed to initialize GLFW, shutting down...\n");
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Sphere + Camera", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    // Lighting stuff
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    float light_pos[]     = { 5, 5, 5, 1 };
    float light_diffuse[] = { 1, 1, 1, 1 };
    float light_ambient[] = { 0.2f, 0.2f, 0.2f, 1 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);

    float mat_diffuse[]  = { 0.7f, 0.7f, 0.7f, 1 };
    float mat_specular[] = { 0.3f, 0.3f, 0.3f, 1 };

    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf (GL_FRONT, GL_SHININESS, 32.0f);

    Camera camera;
    camera_init(&camera);

    double last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        double dt = now - last_time;
        last_time = now;

        glfwPollEvents();
        process_input(window, &camera, dt);

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        render(&camera, w, h);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;

    // Physics stuff
    // Particle p1 = {
    //     .force = { 0.0f, 100.0f, 0.0f},
    //     .acceleration = { 0.0f, -1.0f, 0.0f},
    //     .damping = 0.99,
    // };

    // // Test game loop running x times
    // for (int i = 0; i < 20; i++) {
    //     const real duration = i * 2.0f;
    //     particle_integrate(&p1, duration);

    //     v3_print(p1.position);
    // }
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