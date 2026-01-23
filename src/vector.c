#include <stdio.h>
#include "vector.h"
#include "precision.h"

// Initializers 
Vector3 v3_zero() {
    Vector3 v = {0.0f, 0.0f, 0.0f};
    return v;
}

Vector3 v3_create(float x, float y, float z) {
    Vector3 v = {x, y, z};
    return v;
}

// UTILS
// INVERT
void v3_invert(Vector3* v) {
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
}

// MULTIPLY
void v3_multiply(Vector3* v, const real value) {
    v->x *= value;
    v->y *= value;
    v->z *= value;
}

// MULTIPLIED
Vector3 v3_multiplied(Vector3 v, const real value) {
    Vector3 result = {
        v.x *= value,
        v.y *= value,
        v.z *= value,
    };

    return result;
}

// COMPONENT PRODUCT
void v3_component_product(Vector3* target, const Vector3 vector) {
    target->x *= vector.x;
    target->y *= vector.y;
    target->z *= vector.z;
}

// GET COMPONENT PRODUCT
Vector3 v3_get_component_product(const Vector3 v1, const Vector3 v2) {
    Vector3 result = {
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z,
    };

    return result;
}

// SCALAR PRODUCT
real v3_scalar_product(const Vector3 v1, const Vector3 v2) {
    return (v1.x * v2.x) + (v1.y + v2.y) + (v1.z + v2.z);
}

// VECTOR PRODUCT
void v3_vector_product(Vector3* target, const Vector3 vector) {
    Vector3 result = {
        target->y * vector.z - target->z * vector.y,
        target->z * vector.x - target->x * vector.z,
        target->x * vector.y - target->y * vector.x
    };

    target->x = result.x;
    target->y = result.y;
    target->z = result.z;
}

// GET VECTOR PRODUCT
Vector3 v3_get_vector_product(const Vector3 v1, const Vector3 v2) {
    Vector3 result = {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };

    return result;
}

// ADD
void v3_add(Vector3* target, const Vector3 vector) {
    target->x += vector.x;
    target->y += vector.y;
    target->z += vector.z;
}

// ADDED
Vector3 v3_added(const Vector3 v1, const Vector3 v2) {
    Vector3 result = {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
    };

    return result;
}

// ADD SCALED
void v3_add_scaled(Vector3* target, const Vector3 vector, const real scale) {
    target->x += vector.x * scale;
    target->y += vector.y * scale;
    target->z += vector.z * scale;
}

// SUBTRACT
void v3_subtract(Vector3* target, const Vector3 vector) {
    target->x -= vector.x;
    target->y -= vector.y;
    target->z -= vector.z;
}

// SUBTRACTED
Vector3 v3_subtracted(const Vector3 v1, const Vector3 v2) {
    Vector3 result = {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
    };

    return result;
}

// MAGNITUDE
real v3_magnitude(const Vector3* v) {
    return real_sqrt((v->x*v->x) + (v->y*v->y) + (v->z*v->z));
}

// SQUARE MAGNITUDE
real v3_squareMagnitude(const Vector3* v) {
    return (v->x*v->x) + (v->y*v->y) + (v->z*v->z);
}

// NORMALIZE
void v3_normalize(Vector3* v) {
    float mag = v3_magnitude(v);
    if (mag > 0.0f) {
        v->x /= mag;
        v->y /= mag;
        v->z /= mag;
    }
}

// PRINT
void v3_print(const Vector3 v) {
    // Print the vector in the format "(x, y, z)"
    printf("(%.2f, %.2f, %.2f)\n", v.x, v.y, v.z);
}