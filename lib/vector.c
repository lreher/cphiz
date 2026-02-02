#include <stdio.h>
#include "vector.h"
#include "precision.h"

// Initializers 
V3 v3_zero() {
    V3 v = {0.0f, 0.0f, 0.0f};
    return v;
}

V3 v3_create(float x, float y, float z) {
    V3 v = {x, y, z};
    return v;
}

// UTILS
// INVERT
void v3_invert(V3* v) {
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
}

// MULTIPLY
void v3_multiply(V3* v, const real value) {
    v->x *= value;
    v->y *= value;
    v->z *= value;
}

// MULTIPLIED
V3 v3_multiplied(V3 v, const real value) {
    V3 result = {
        v.x *= value,
        v.y *= value,
        v.z *= value,
    };

    return result;
}

// COMPONENT PRODUCT
void v3_component_product(V3* target, const V3 vector) {
    target->x *= vector.x;
    target->y *= vector.y;
    target->z *= vector.z;
}

// GET COMPONENT PRODUCT
V3 v3_get_component_product(const V3 v1, const V3 v2) {
    V3 result = {
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z,
    };

    return result;
}

// SCALAR PRODUCT
real v3_scalar_product(const V3 v1, const V3 v2) {
    return (v1.x * v2.x) + (v1.y + v2.y) + (v1.z + v2.z);
}

// VECTOR PRODUCT
void v3_vector_product(V3* target, const V3 vector) {
    V3 result = {
        target->y * vector.z - target->z * vector.y,
        target->z * vector.x - target->x * vector.z,
        target->x * vector.y - target->y * vector.x
    };

    target->x = result.x;
    target->y = result.y;
    target->z = result.z;
}

// GET VECTOR PRODUCT
V3 v3_get_vector_product(const V3 v1, const V3 v2) {
    V3 result = {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };

    return result;
}

// ADD
void v3_add(V3* target, const V3 vector) {
    target->x += vector.x;
    target->y += vector.y;
    target->z += vector.z;
}

// ADDED
V3 v3_added(const V3 v1, const V3 v2) {
    V3 result = {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
    };

    return result;
}

// ADD SCALED
void v3_add_scaled(V3* target, const V3 vector, const real scale) {
    target->x += vector.x * scale;
    target->y += vector.y * scale;
    target->z += vector.z * scale;
}

// SUBTRACT
void v3_subtract(V3* target, const V3 vector) {
    target->x -= vector.x;
    target->y -= vector.y;
    target->z -= vector.z;
}

// SUBTRACTED
V3 v3_subtracted(const V3 v1, const V3 v2) {
    V3 result = {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
    };

    return result;
}

// MAGNITUDE
real v3_magnitude(const V3* v) {
    return real_sqrt((v->x*v->x) + (v->y*v->y) + (v->z*v->z));
}

// SQUARE MAGNITUDE
real v3_squareMagnitude(const V3* v) {
    return (v->x*v->x) + (v->y*v->y) + (v->z*v->z);
}

// NORMALIZE
void v3_normalize(V3* v) {
    float mag = v3_magnitude(v);
    if (mag > 0.0f) {
        v->x /= mag;
        v->y /= mag;
        v->z /= mag;
    }
}

// V3 to Vector3 
Vector3 V3_TO_RAYLIB(V3 v) {
    return (Vector3){ v.x, v.y, v.z };
}

// PRINT
void v3_print(const V3 v) {
    // Print the vector in the format "(x, y, z)"
    printf("(%.2f, %.2f, %.2f)\n", v.x, v.y, v.z);
}