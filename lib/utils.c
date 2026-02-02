#include <stdlib.h>
#include <time.h>
#include <math.h>

void init_random(void) {
    srand((unsigned int)time(NULL));
}

float rand_float(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

float log_map(float t, float min, float max) {
    // t in [0,1]
    float logMin = logf(min);
    float logMax = logf(max);
    return expf(logMin + t * (logMax - logMin));
}

float inv_log_map(float v, float min, float max) {
    float logMin = logf(min);
    float logMax = logf(max);
    return (logf(v) - logMin) / (logMax - logMin);
}