#ifndef ISING1D_H
#define ISING1D_H
#include"toml.h"


/*
 * Ising1D
 * -------
 * Represents an ising model in 1 dimension.
 *
 * parameters
 * ----------
 * int length: The number of spins in the system.
 * float temperature: The temperature of the system in natural units.
 * int *system: The orientation of the spins in the system. 
 */
typedef struct Ising1D 
{
    int length;
    float temperature;
    int *ensemble;
} Ising1D;

Ising1D* init_ising_1d(int length, float temperature);
int spin_energy_ising_1d(Ising1D *system, int spin);
void metropolis_step_ising_1d(Ising1D *system);
void flip_spin_ising_1d(Ising1D *system, int spin);
void print_ising_1d(Ising1D *system);
void first_and_last(Config *config);
float energy_ising_1d(Ising1D *system);
float magnetisation_ising_1d(Ising1D *system);
float entropy_ising_1d(Ising1D *system);
float free_energy_ising_1d(Ising1D *system);
float heat_capacity_ising_1d(Ising1D *system);

#endif
