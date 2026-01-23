#include <stdio.h>
#include "vector.h"
#include "particle.h"


int main() {
    Particle p1 = {
        .velocity = { 0.0f, 100.0f, 0.0f},
        .acceleration = { 0.0f, -1.0f, 0.0f},
        .damping = 0.99,
    };

    // Test game loop running x times
    for (int i = 0; i < 20; i++) {
        const real duration = i * 2.0f;
        particle_integrate(&p1, duration);

        v3_print(p1.position);
    }

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