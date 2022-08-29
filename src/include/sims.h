#ifndef SIMS_H
#define SIMS_H 

void first_and_last(int num_spins, float temperatures[], 
    int num_temps, int reps);

void physical_parameters(int num_spins, float temperatures[], 
    int num_temps, int reps);

void histogram(int num_spins, float temperatures[], int num_temps, int reps);

void ising(int num_spins, int reps, float temperature);

#endif

