#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/ising.h"
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
    float range_rand = norm_rand * length + 0.5;
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
    int length;
    int **ensemble;
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
Ising2D* init_ising_2d(int length)
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
    system -> ensemble = ensemble;
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
    printf("Entered the energy lair\n");
    int length = system -> length;
    int **ensemble = system -> ensemble;
    float energy = 0;
    printf("Commencing the for loop\n");

    for (int row = 0; row < length; row++)
    {
        printf("Row: %i\n", row);
        for (int col = 0; col < length; col++)
        {
            printf("Col: %i\n", col);
            energy -= ensemble[row][col] * (
                ensemble[modulo(row + 1, length)][col] +
                ensemble[modulo(row - 1, length)][col] +
                ensemble[row][modulo(col + 1, length)] +
                ensemble[row][modulo(col - 1, length)]);
        }
    }

    return energy;
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


// OK so ... the question is should I allo myself to read the system 
// from a file so that I can make deterministic tests against the python 
// code? Perhaps I think for now though I forge ahead with the same 
// statergy that I had before.


int main(void)
{
    int length = 5;
    Ising2D *system = init_ising_2d(5);
    float system_energy = energy_ising_2d(system);
    printf("Energy: %f\n", system_energy);
    print_ising_2d(system);
}
