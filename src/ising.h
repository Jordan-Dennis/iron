#ifndef ising.h
#define ising.h
#include<stdio.h>

int factorial(int number);
float normalised_random(void);
int random_spin(void);
int energy(int spins[]);
float entropy(int spins[]);
float free_energy(int spins[], float temperature);
float heat_capacity(int spins[], float temperature);
#endif
