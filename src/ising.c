#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ising.h"


/*
 * length
 * ------
 * Find the length of an array of integers.
 * 
 * parameters
 * ----------
 * int spins[]: The array of integers.
 * 
 * returns
 * -------
 * int length: The length of the array.
 */
int length(int spins[])
{
    return (int)(sizeof(spins) / sizeof(int));
}


/*
 * factorial
 * ---------
 * Calculate the factorial of a number.
 *
 * parameters
 * ----------
 * int : The number to calculate the factorial of. Must be greater 
 * than 0.
 */
int factorial(int number)
{
    return (number == 0 || number == 1) ? 1 : factorial(number - 1); 
}


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
 * spin_energy
 * -----------
 * Calculate the energy of one spin interacting with its immediate 
 * neighbours in normalised units.
 *
 * parameters
 * ----------
 * int spin: The index of the specific spin.
 * int spins[]: The array representing the ensamble of spins.
 * 
 * returns
 * -------
 * int: The energy contribution of this specific spin.
 */
int spin_energy(int spin, int spins[])
{
    return spins[spin] * spins[spin - 1] + 
        spins[spin] * spins[spin + 1];
}


/*
 * energy
 * ----------------
 * The energy stored by the ensamble of spins.
 * 
 * parameters
 * ----------
 * int spins[]: The spin state of the ensamble.
 *
 * returns
 * -------
 * int: The energy of the ensamble in units of epsilon.
 */
int energy(int spins[], int n)
{
    int energy = 0;
    int number_of_spins = length(spins);
    for (int spin = 0; spin < number_of_spins; spin++) 
    {
        // Just the nearest neighours contribute to the energy.
        energy += spin_energy(spin, spins);
    }
    return energy;
}


/*
 * entropy
 * -------
 * Calculate the entropy at a given moment.
 * 
 * parameters
 * ----------
 * int spins[] : An array representing the spins of the system.
 */
float entropy(int spins[]) 
{
    int up_spins = 0;
    int number_of_spins = length(spins);
    for (int spin=0; spin < number_of_spins; spin++)
    {
        up_spins += (int)(spins[spin] > 0);
    }
    int down_spins = n - up_spins;
    int multiplicity = factorial(n) /\
        (factorial(up_spins) * factorial(down_spins));
    return log(multiplicity);
}


/*
 * free_energy
 * -----------
 * The free energy of the ensamble of spins.
 * 
 * parameters
 * ----------
 * int spins[]: The spin esamble.
 */
float free_energy(int spins[], float temperature)
{
    return (float)energy(spins) - temperature * entropy(spins);
}


/*
 * heat_capacity
 * -------------
 * Calculate the heat capacity of the ensamble.
 *
 * parameters
 * ----------
 * int spins[]: The spins ensamble.
 */
float heat_capacity(int spins[], float temperature)
{
    int total = 0;
    int number_of_spins = length(spins);
    for (int spin=0; spin < number_of_spins; spin++)
    {
        total += spins[spin];
    }
    float energy_average = (float)total / (float)n;
    // Note that the square of all the spins is 1
    // Hence the expected value of the distance from 
    // 0 must be 1.
    float energy_variance = 1 - energy_average;
    return energy_variance / (temperature * temperature);
}


/*
 * metropolis_step
 * ---------------
 * Evolve the spin state according to a metropolis algorithm. 
 *
 * parameters
 * ----------
 * int spins[]: The spin system to evolve. 
 * float temperature: The temperature of the system in natural units.
 * 
 * returns
 * -------
 * void: The array modifications are done in place. 
 */
void metropolis_step(int spins[], float temperature)
{

    int spin = (int) (normalised_random() * n);
    int energy_change = -2 * spin_energy(spin, spins) - 4 * spins[spin];
   
    if (energy_change <= 0)
    {
        spins[spin] = -spins[spin];
    } 
    else
    {
        float probability = exp(-energy_change / temperature);
        if (probability <= normalised_random()) 
        {
            spins[spin] = -spins[spin];
        }
    }
}


/*
 * random_system
 * -------------
 * Generate a random spin system.
 * 
 * parameters
 * ----------
 * int spins[]: An empty array to house the spins. 
 * 
 * returns 
 * -------
 * void: The spins array is internal modified on the heap. 
 */
void random_system(int spins[])
{
    int number_of_spins = length(spins);
    for (int spin = 0; spin < number_of_spins; spin++) 
    {
        int value = random_spin();
        spins[spin] = value;
    }
}


/*
 * system_to_file
 * --------------
 * Write the current state of the system to a file. 
 *
 * parameters
 * ----------
 * char file_name[]: The file name to write the spin system to.
 * int spins[]: The current state of the system. 
 * 
 * returns
 * -------
 * void: Writes the state to a file.
 */
void system_to_file(char file_name[], int spins[])
{
    FILE *data = fopen(address, "w");
    int number_of_spins = length(spins);
    for (int spin = 0; spin < number_of_spins; spin++)
    {
        fprintf(data, "%i, %i, %i\n", 1, spin, spins[spin]);
    }
       
    fclose(data);
}
