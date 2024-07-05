#pragma once

#include "global_macros.hpp"

template<typename T>
struct vec3;

using fvec3 = vec3<float>;

void PlaneFromPointsASM(float* plane, float* v0, float* v1, float* v2);


void CrossProduct(const vec3_t v1, const vec3_t v2, vec3_t cross);
vec_t VectorNormalize(vec3_t v);       // returns vector length
vec_t VectorNormalize2(const vec3_t v, vec3_t out);
void VectorInverse(vec3_t v);

void MatrixMultiply(float in1[3][3], float in2[3][3], float out[3][3]);
void PerpendicularVector(vec3_t dst, const vec3_t src);
void RotatePointAroundVector(vec3_t dst, const vec3_t dir, const vec3_t point, float degrees);
void vectoangles(const vec3_t value1, vec3_t angles);

fvec3 AxisToAngles(const vec3_t axis[3]);


float AngleNormalize360(float angle);
float AngleNormalize180(float angle);