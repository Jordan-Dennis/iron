#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/statistics.h"


/*
 * random
 * ------
 * Generate a random number over the range [0, 1].
 *
 * returns
 * -------
 * float: random number.
 */
float normalised_random(void)
{ 
    return (float) rand() / (float) RAND_MAX;
}


/* 
 * random_sign
 * -----------
 * Generate + or - 1 randomly.
 *
 * returns
 * -------
 * int: +1 or -1 randomly.
 */ 
int random_spin(void) 
{
    float normed_random = normalised_random();
    float translated_random = normed_random - .5;
    return (int) ((translated_random > 0) - (translated_random < 0));
}


/*
 * random_index
 * ------------
 * Generate a random index in the correct range.
 * 
 * parameters
 * ----------
 * int length: The length of the array that is getting indexed. 
 *
 * returns
 * -------
 * int rand_ind: A random index in the range [0, length]
 */
int random_index(int length)
{
    float norm_rand = normalised_random();
    float range_rand = norm_rand * length;
    return (int) range_rand;
}


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


/*
 * init_2d_ising_system
 * --------------------
 * Construct a two dimensional model of the ising system.
 *
 * parameters
 * ----------
 * int length: The number of spins desired along one edge of the model.
 *
 * returns
 * -------
 * Ising2D* model: A random spin state of the desired size. 
 */
Ising2D* init_ising_2d(int length, float temperature)
{
    int **ensemble = (int**) calloc(length, sizeof(int*));  
    for (int col = 0; col < length; col++)
    {
        ensemble[col] = (int*) calloc(length, sizeof(int));
    } 

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            ensemble[row][col] = random_spin();
        }
    }

    Ising2D* system = (Ising2D*) calloc(1, sizeof(Ising2D));
    system -> length = length;
    system -> temperature = temperature;
    system -> ensemble = ensemble;
    return system;
}


/*
 * modulo
 * ------
 * A number theory based version of the % operator
 *
 * parameters
 * ----------
 * int dividend: The n in n % m 
 * int divisor: The m in n % m
 *
 * returns
 * -------
 * int nmodm: The number theory modulo.
 */
int modulo(int dividend, int divisor)
{
    if (divisor == 0)
    {
        return 0;
    }
    return ((dividend % divisor) + divisor) % divisor;
}


/*
 * spin_energy_ising_2d
 * --------------------
 * Calculate the energy of a specific spin in the system. 
 *
 * parameters
 * ----------
 * const Ising2D *system: The system of spins.
 * int row: The row coordinate of the spin.
 * int col: The column coordinate of the spin. 
 *
 * returns
 * -------
 * The energy contribution of this specific spin. 
 */
float spin_energy_ising_2d(const Ising2D *system, int row, int col)
{
    int **ensemble = system -> ensemble;
    int length = system -> length;
    return ensemble[row][col] * (
        ensemble[modulo(row + 1, length)][col] +
        ensemble[modulo(row - 1, length)][col] +
        ensemble[row][modulo(col + 1, length)] +
        ensemble[row][modulo(col - 1, length)]);
}


/*
 * energy_ising_2d
 * ---------------
 * Calculate the energy of the 2D ising simulation with periodic boundary 
 * conditions. 
 *
 * parameters
 * ----------
 * const Ising2D *system: The 2D ising system.
 *
 * returns
 * -------
 * int energy: The energy.
 */
float energy_ising_2d(const Ising2D *system)
{
    int length = system -> length;
    float energy = 0;

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            energy -= spin_energy_ising_2d(system, row, col);
        }
    }

    return energy / 2.;
}


/*
 * flip_spin_ising_2d
 * ------------------
 * Reverse the direction of a specific spin within the system.
 *
 * parameters
 * ----------
 * Ising2D *system: The system to be modified.
 * int row: The row coordinate of the spin getting flipped.
 * int col: The column coordinate of the spin getting flipped.
 */
void flip_spin_ising_2d(Ising2D *system, int row, int col)
{
    system -> ensemble[row][col] *= -1;
}


/*
 * print_ising_2d
 * --------------
 * A convinience function for debugging. Prints the system compactly 
 * into the terminal on a uniform grid.
 *
 * parameters
 * ----------
 * Ising2D *system: The system to display.
 */
void print_ising_2d(Ising2D *system)
{
    int length = system -> length;
    int **ensemble = system -> ensemble;
    
    printf("2D Ising System:\n");
    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            printf("%i", ensemble[row][col] > 0);
        }
        printf("\n");
    }
}


/*
 * metropolis_step
 * ---------------
 * Evolve the spin state according to a metropolis algorithm. 
 *
 * parameters
 * ----------
 * System* system: The spin ensamble to evolve.  
 */
void metropolis_step(Ising2D *system)
{

    int row = random_index(system -> length);
    int col = random_index(system -> length);
    int energy_change = 2 * spin_energy_ising_2d(system, row, col);
   
    if (energy_change < 0)
    {
        flip_spin_ising_2d(system, row, col);
    } 
    else
    {
        float probability = exp(- energy_change / (system -> temperature));
        if (probability > normalised_random()) 
        {
            flip_spin_ising_2d(system, row, col);
        }
    }
}

// OK so ... the question is should I allo myself to read the system 
// from a file so that I can make deterministic tests against the python 
// code? Perhaps I think for now though I forge ahead with the same 
// statergy that I had before.


int main(void)
{
    int length = 100;
    Ising2D *system = init_ising_2d(length, 10.);

    printf("Energy: %f\n", energy_ising_2d(system));
    print_ising_2d(system);
    for (int epoch = 0; epoch < 1000 * length; epoch++)
    {
        metropolis_step(system);
    }
    printf("Energy: %f\n", energy_ising_2d(system));
    print_ising_2d(system);
}
