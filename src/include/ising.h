#ifndef ISING_H
#define ISING_H


/*
 * SpinSystem
 * ----------
 * Represents the spins system and stores all of the relevant information
 * pertaining to it. 
 * 
 * fields
 * ------
 * int number: The number of spins in the system. 
 * int dimension: The number of dimensions that the spin system occupies. 
 * float temperature: The temperature of the system of spins. 
 * int spins[]: The spins system represented as an array of spins. 
 */
typedef struct System
{
    int number;
    int dimension;
    float temperature; // ?? Do I want this to be here
    int *spins;
} System;


int factorial(int number);
float normalised_random(void);
int random_spin(void);


int energy(System* system);
int magnetisation(System* system);
float entropy(System* system);
float free_energy(System* system);
float heat_capacity(System* system);
void metropolis_step(System* system);
void random_system(System* system);
int spin_energy(System* system, int spin);

#endif
