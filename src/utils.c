#include <stdlib.h>
#include <time.h>

void init_random(void) {
    srand((unsigned int)time(NULL));
}

float rand_float(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}