#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/ising.h"
#include"include/statistics.h"


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
}


/*
 * init_ising_1d
 * -------------
 * Represents an ising system at some temperature and size. 
 * 
 * parameters
 * ----------
 * int length: The number of spins in the system.
 * float temperature: The temperature of the system in natural units. 
 *
 * returns
 * -------
 * Ising1D* system: The ising model. 
 */
Ising1D* init_ising_1d(int length, float temperature)
{
    int *ensemble = (int*) calloc(length, sizeof(int));
    for (int spin = 0; spin < length; spin++)
    {
        ensemble[spin] = random_spin();
    }

    Ising1D* system = (Ising1D*) malloc(sizeof(Ising1D));
    system -> length = length;
    system -> temperature = temperature;
    system -> ensemble = ensemble;
    return system;
}


/*
 * spin_energy_ising_1d
 * --------------------
 * Calculate the energy of one spin interacting with its immediate 
 * neighbours in normalised units for one diemsnional case of spins isolated
 * along a line. 
 *
 * parameters
 * ----------
 * int spin: The index of the specific spin.
 * Ising1D* system: The array representing the ensamble of spins.
 * 
 * returns
 * -------
 * int: The energy contribution of this specific spin.
 */
int spin_energy_ising_1d(Ising1D* system, int spin)
{
    int *ensemble = system -> ensemble;
    int length = system -> number;
    return ensemble[spin] * (
        ensemble[modulo(spin + 1, length)] + 
        ensemble[modulo(spin - 1, length)]);
}


/*
 * energy
 * ----------------
 * The energy stored by the ensamble of spins.
 * 
 * parameters
 * ----------
 * Ising1D* system: The system representing the ensamble. 
 *
 * returns
 * -------
 * int: The energy of the ensamble in units of epsilon.
 */
float energy_ising_1d(Ising1D* system)
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
 * Ising1D* system: The spin ensamble to calculate the entropy of.
 *
 * returns
 * -------
 * float entropy: The entropy of the system.  
 */
float entropy_ising_1d(Ising1D* system) 
{
    int number = system -> number;
    int* ensemble = (system -> ensemble); 
    int up_spins = 0;

    for (int spin = 0; spin < number; spin++)
    {
        if (ensemble[spin] == (ensemble[(spin + 1) % number]))
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
 * Ising1D* system: The spin ensamble to calculate the free energy of. 
 * 
 * returns
 * -------
 * float free_energy: The free energy.
 */
float free_energy_ising_1d(Ising1D* system)
{
    float temperature = system -> temperature;
    return energy_ising_1d(system) - temperature * entropy_ising_1d(system);
}


/*
 * metropolis_step
 * ---------------
 * Evolve the spin state according to a metropolis algorithm. 
 *
 * parameters
 * ----------
 * Ising1D* system: The spin ensamble to evolve.  
 */
void metropolis_step(Ising1D* system)
{
    int spin = random_index(system -> length);
    int energy_change = 2 * spin_energy_ising_1d(system, spin);
   
    if (energy_change < 0)
    {
        flip_spin_ising_1d(system, spin);
    } 
    else
    {
        float probability = exp(- energy_change / (system -> temperature));
        if (probability > normalised_random()) 
        {
            flip_spin_ising_1d(system, spin);
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
 * Ising1D* system: The spin ensamble to calculate the magnetisation of.  
 *
 * returns
 * -------
 * int magnetisation: The net magnetisation.
 */
float magnetisation_ising_1d(Ising1D* system)
{
    int length = system -> length;
    float magnetisation = 0;
    for (int spin = 0; spin < length; spin++)
    {
        magnetisation += (system -> ensemble)[spin];
    }
    return magnetisation / length;
}
