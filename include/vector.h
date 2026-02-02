#ifndef CORE_H
#define CORE_H

#include "precision.h"
#include "raylib.h"

typedef struct {
    real x;
    real y;
    real z;
} V3;

// Vector Initializers
V3 v3_zero();
V3 v3_create(float x, float y, float z);

// Vector Operations
void v3_multiply(V3* v, const real value);
V3 v3_multiplied(V3 v, const real value);

void v3_add(V3* target, const V3 vector);
V3 v3_added(const V3 v1, const V3 v2);
void v3_add_scaled(V3* target, const V3 vector, const real scale);

void v3_subtract(V3* target, const V3 vector);
V3 v3_subtracted(const V3 v1, const V3 v2);

// Vector Products
void v3_component_product(V3* target, const V3 vector);
V3 v3_get_component_product(const V3 v1, const V3 v2);

real v3_scalar_product(const V3 v1, const V3 v2);

void v3_vector_product(V3* target, const V3 vector);
V3 v3_get_vector_product(const V3 v1, const V3 v2);

// Vector Utils
void v3_invert(V3* v);

real v3_magnitude(const V3* v);
real v3_squareMagnitude(const V3* v);

void v3_normalize(V3* v);

Vector3 V3_TO_RAYLIB(V3 v);

V3 v3_random_vector(void);

void v3_print(const V3 v);

#endif