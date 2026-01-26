#include <stdio.h>

#include "vector.h"
#include "particle.h"
#include "camera.h"

#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

// PROJECTION
static void setup_projection(int width, int height) {
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

// GEOMETRY
/*
 * Draws a solid sphere using latitude/longitude slices.
 * Replacement for glutSolidSphere.
 */
static void draw_sphere(float radius, int slices, int stacks)
{
    for (int i = 0; i < stacks; i++) {
        float lat0 = M_PI * (-0.5f + (float)i / stacks);
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / stacks);

        float y0 = sinf(lat0);
        float y1 = sinf(lat1);

        float r0 = cosf(lat0);
        float r1 = cosf(lat1);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; j++) {
            float lon = 2.0f * M_PI * (float)j / slices;
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

// SCENE
static void draw_grid(void)
{
    glBegin(GL_LINES);
    for (int i = -50; i <= 50; i += 5) {
        glVertex3f((float)i, 0.0f, -50.0f);
        glVertex3f((float)i, 0.0f,  50.0f);

        glVertex3f(-50.0f, 0.0f, (float)i);
        glVertex3f( 50.0f, 0.0f, (float)i);
    }
    glEnd();
}

static void render_scene(const Camera* cam)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    camera_apply(cam);

    /* Grid */
    glColor3f(0.7f, 0.7f, 0.7f);
    draw_grid();

    /* Sphere */
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    draw_sphere(1.0f, 32, 16);
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

    setup_projection(800, 600);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    /* Simple lighting (optional but recommended for spheres) */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float light_pos[] = { -10.0f, 10.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    Camera cam = {
        .x = -6.0f,
        .y = 4.0f,
        .z = 10.0f,
        .pitch = 20.0f,
        .yaw = -25.0f
    };

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // // Render here
        // glClear(GL_COLOR_BUFFER_BIT);
        render_scene(&cam);


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
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