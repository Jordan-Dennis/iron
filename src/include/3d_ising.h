#ifndef ISING3D_H
#define ISING3D_H


/*
 * Ising3D
 * -------
 * Represents the two-dimensional simulation of the ising model. 
 * These models are assumed to be square.
 *
 * parameters
 * ----------
 * int length: The number of spins in a single row/col.
 * int **ensemble: The matrix of spins. 
 */
typedef struct Ising3D {
    int     length;
    float   temperature;
    int     ***ensemble;
} Ising3D;

 
Ising3d *init_ising_1d(int length, float temperature);
int spin_energy_ising_3d(Ising3D *system, int row, int col);
void metropolis_step_ising_3d(Ising3D *system);
void flip_spin_ising_3d(Ising3D *system, int row, int col);
void print_ising_3d(Ising3D *system);
float energy_ising_3d(Ising3D *system);
float magnetisation_ising_3d(Ising3D *system);
float free_energy_ising_3d(Ising3D *system);
float heat_capacity_ising_3d(Ising3D *system);

#endif
