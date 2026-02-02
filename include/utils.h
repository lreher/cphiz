

#ifndef UTILS_H
#define UTILS_H
#pragma once

void init_random(void);
float rand_float(float min, float max);

float log_map(float t, float min, float max);
float inv_log_map(float v, float min, float max);

#endif