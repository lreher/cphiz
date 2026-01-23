#ifndef CORE_H
#define CORE_H

#include "precision.h"

typedef struct {
    real x;
    real y;
    real z;
} Vector3;

// Vector Initializers
Vector3 v3_zero();
Vector3 v3_create(float x, float y, float z);

// Vector Operations
void v3_multiply(Vector3* v, const real value);
Vector3 v3_multiplied(Vector3 v, const real value);

void v3_add(Vector3* target, const Vector3 vector);
Vector3 v3_added(const Vector3 v1, const Vector3 v2);
void v3_add_scaled(Vector3* target, const Vector3 vector, const real scale);

void v3_subtract(Vector3* target, const Vector3 vector);
Vector3 v3_subtracted(const Vector3 v1, const Vector3 v2);

// Vector Products
void v3_component_product(Vector3* target, const Vector3 vector);
Vector3 v3_get_component_product(const Vector3 v1, const Vector3 v2);

real v3_scalar_product(const Vector3 v1, const Vector3 v2);

void v3_vector_product(Vector3* target, const Vector3 vector);
Vector3 v3_get_vector_product(const Vector3 v1, const Vector3 v2);

// Vector Utils
void v3_invert(Vector3* v);

real v3_maginitude(const Vector3* v);
real v3_squareMagnitude(const Vector3* v);

void v3_normalize(Vector3* v);

void v3_print(const Vector3 v);

#endif