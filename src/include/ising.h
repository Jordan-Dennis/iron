#ifndef ISING_H
#define ISING_H
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int factorial(int number);
float normalised_random(void);
int random_spin(void);
int energy(int spins[], int num_spins);
int magnetisation(int spins[], int num_spins);
float entropy(int spins[], int num_spins);
float free_energy(int spins[], float temperature, int num_spins);
float heat_capacity(int spins[], float temperature, int num_spins);
void metropolis_step(int spins[], float temperature, int num_spins);
void random_system(int spins[], int num_spins);
void system_to_file(int spins[], char* file_name, int num_spins);
// TODO: So I could have that the dimensions was passed as an argument
// everywhere or I could have a function chose_spin_energy(int dimension)
// that would chose the spin energy and which I could invoke in main. 
// I think I will go with the second option, and sophistocate my model 
// by having a system struct. 
int spin_energy(int spin, int spins[], int num_spins);
int two_dimensional_spin_energy(int spin, int spins[], int num_spins);
int one_dimensional_spin_energy(int spin, int spins[], int num_spins);

#endif
