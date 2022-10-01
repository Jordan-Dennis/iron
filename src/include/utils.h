#ifndef UTILS_H
#define UTILS_H

int random_spin(void);
int random_index(int length);
int modulo(int dividend, int divisor);
float normalised_random(void);
float mean(float* array, int length);
float variance(float* array, float mean, int length);

#endif
