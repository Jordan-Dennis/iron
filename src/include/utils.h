#ifndef UTILS_H
#define UTILS_H

int random_spin(void);
int random_index(int length);
float normalised_random(void);
float mean(float* array, int length);
float variance(float* array, float mean, int length);

#endif
