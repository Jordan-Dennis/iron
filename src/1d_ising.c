#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/ising.h"
#include"include/statistics.h"


typedef struct Ising1D 
{
    int length;
    float temperature;
    int *system;
}

Ising1D* init_ising_1d(int length, float temperature)
{

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
int spin_energy_ising_1d(System* system, int spin)
{
    int *spins = system -> spins;
    int num = system -> number;
    return spins[spin] * (spins[(spin + 1) % num] + spins[(spin - 1) % num]);
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
float energy_ising_1d(System* system)
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
float entropy_ising_1d(System* system) 
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
float free_energy_ising_1d(System* system)
{
    return energy_ising_1d(system) - (system -> temperature) * entropy(system);
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
float magnetisation_ising_1d(System* system)
{
    float magnetisation = 0;
    for (int spin = 0; spin < (system -> number); spin++)
    {
        magnetisation += (system -> spins)[spin];
    }
    return magnetisation / (system -> number);
}
