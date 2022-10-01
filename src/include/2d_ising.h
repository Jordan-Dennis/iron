#ifndef ISING2D_H
#define ISING2D_H


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

 
Ising2D *init_ising_1d(int length, float temperature);
void metropolis_step_ising_2d(Ising2D *system);
void flip_spin_ising_2d(Ising2D *system, int row, int col);
void print_ising_2d(Ising2D *system);
float spin_energy_ising_2d(const Ising2D *system, int row, int col);
float energy_ising_2d(const Ising2D *system);
float magnetisation_ising_2d(const Ising2D *system);
float free_energy_ising_2d(const Ising2D *system);
float heat_capacity_ising_2d(const Ising2D *system);

#endif
