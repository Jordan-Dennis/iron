#ifndef ISING2D_H
#define ISING2D_H
#include"toml.h"


/*
 * Ising2D
 * -------
 * Represents the two-dimensional simulation of the ising model. 
 * These models are assumed to be square.
 *
 * parameters
 * ----------
 * int length: The number of spins in a single row/col.
 * int **ensemble: The matrix of spins. 
 */
typedef struct Ising2D {
    int     length;
    float   temperature;
    int     **ensemble;
} Ising2D;

 
int magnetisation_ising_2d(const Ising2D *system);
void metropolis_step_ising_2d(Ising2D *system);
void flip_spin_ising_2d(Ising2D *system, int row, int col);
void print_ising_2d(Ising2D *system);
void first_and_last_ising_2d(Config *config);
void physical_parameters_ising_2d(Config* config);
void magnetisation_vs_temperature(Config *config);
float spin_energy_ising_2d(const Ising2D *system, int row, int col);
float energy_ising_2d(const Ising2D *system);
float free_energy_ising_2d(const Ising2D *system);
float heat_capacity_ising_2d(const Ising2D *system);
Ising2D *init_ising_2d(int length, float temperature);

#endif
