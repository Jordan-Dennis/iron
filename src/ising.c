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
 * 2d_spin_energy
 * --------------
 * Calculates the spin energy in two dimensions. 
 * 
 * parameters
 * ----------
 * 
 * returns
 * -------
 * int: The energy contribution of this specific spin.
 */
int two_dimensional_spin_energy(System* system, int spin)
{
    int *spins = system -> spins;
    int num = (int) sqrt(system -> number);
    int energy = 0;

    if (spin % num == 0)
    {
        energy += spins[spin] * (spins[spin + 1] + spins[spin + num - 1]);
    }
    else if (spin % num == num - 1)
    {
        energy += spins[spin] * (spins[spin - 1] + spins[spin - num + 1]);
    }
    else
    {
        energy += spins[spin] * (spins[spin + num] + spins[spin - num]);
    }

    if ((int) spin / num == 0)
    {
        energy += spins[spin] * (spins[spin + num] + 
            spins[spin + num * (num - 1)]);
    }
    else if ((int) spin / num == num - 1)
    {
        energy += spins[spin] * (spins[spin - num] + 
            spins[spin - num * (num - 1)]);
    }
    else
    {
        energy += spins[spin] * (spins[spin - num] + spins[spin + num]);
    }

    return energy;
}


/*
 * one_dimensional_spin_energy
 * -----------
 * Calculate the energy of one spin interacting with its immediate 
 * neighbours in normalised units for one diemsnional case of spins isolated
 * along a line. 
 *
 * parameters
 * ----------
 * int spin: The index of the specific spin.
 * System* system: The array representing the ensamble of spins.
 * 
 * returns
 * -------
 * int: The energy contribution of this specific spin.
 */
int one_dimensional_spin_energy(System* system, int spin)
{
    int *spins = system -> spins;
    int num = system -> number;
    return spins[spin] * (spins[(spin + 1) % num] + spins[(spin - 1) % num]);
}


/*
 * spin_energy
 * -----------
 * Calculate the spin energy of a spin irrespective of the dimensions.
 *
 * parameters
 * ----------
 * System* system: The spin system of which to calculate the energy.
 *
 * returns
 * -------
 * int energy: The energy of the system in natural units. 
 */
int spin_energy(System* system, int spin)
{
    int energy;
    switch (system -> dimension)
    {
        case 1:
        {
            energy = one_dimensional_spin_energy(system, spin);
            break;
        }
        case 2:
        {
            energy = two_dimensional_spin_energy(system, spin);
            break;
        }
        default:
        {
            printf("Error: Dimensions must be 2 or 1!");
            exit(1);
        }
    }
    return energy;
}


/*
 * energy
 * ----------------
 * The energy stored by the ensamble of spins.
 * 
 * parameters
 * ----------
 * System* system: The system representing the ensamble. 
 *
 * returns
 * -------
 * int: The energy of the ensamble in units of epsilon.
 */
float energy(System* system)
{
    int energy = 0;

    for (int spin = 0; spin < system -> number; spin++) 
    {
        energy -= spin_energy(system, spin);
    }

    return (float) energy / (float) 2;
}


/*
 * entropy
 * -------
 * Calculate the entropy at a given moment.
 * 
 * parameters
 * ----------
 * System* system: The spin ensamble to calculate the entropy of.
 *
 * returns
 * -------
 * float entropy: The entropy of the system.  
 */
float entropy(System* system) 
{
    int number = system -> number;
    int* spins = (system -> spins); 
    int up_spins = 0;

    for (int spin = 0; spin < number; spin++)
    {
        if (spins[spin] == (spins[(spin + 1) % number]))
        {
            up_spins++;
        }
    }

    int down_spins = number - up_spins;

    float entropy = number * log(number) - up_spins * log(up_spins) 
        - down_spins * log(down_spins);
    
    return entropy;
}


/*
 * free_energy
 * -----------
 * The free energy of the ensamble of spins.
 * 
 * parameters
 * ----------
 * System* system: The spin ensamble to calculate the free energy of. 
 * 
 * returns
 * -------
 * float free_energy: The free energy.
 */
float free_energy(System* system)
{
    return energy(system) - (system -> temperature) * entropy(system);
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
void metropolis_step(System* system)
{

    int spin = (int) (normalised_random() * (system -> number));
    int energy_change = 2 * spin_energy(system, spin);
   
    if (energy_change < 0)
    {
        (system -> spins[spin]) = -(system -> spins[spin]);
    } 
    else
    {
        float probability = exp(- energy_change / (system -> temperature));
        if (probability > normalised_random()) 
        {
            (system -> spins[spin]) = -(system -> spins[spin]);
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
 * System* system: The in memory representation of the spins system. 
 * 
 * returns 
 * -------
 * void: The spins array is internal modified on the heap. 
 */
void random_system(System* system)
{
    system -> spins = calloc((system -> number), sizeof(int));
    for (int spin = 0; spin < (system -> number); spin++) 
    {
        (system -> spins)[spin] = random_spin();
    }
}


/*
 * magnetisation
 * -------------
 * Calculate the net magnetisation of the system.
 * 
 * parameters
 * ----------
 * System* system: The spin ensamble to calculate the magnetisation of.  
 *
 * returns
 * -------
 * int magnetisation: The net magnetisation.
 */
int magnetisation(System* system)
{
    int magnetisation = 0;
    for (int spin = 0; spin < (system ->  number); spin++)
    {
        if ((system -> spins[spin]) > 0)
        {
            magnetisation++;
        }
        else
        {
            magnetisation--;
        }
    }
    return magnetisation;
}
