#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"include/ising.h"


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
int spin_energy(int spin, int spins[], int num_spins)
{
    int energy;
    if (spin == 0) 
    {
        energy = spins[0] * (spins[num_spins] + spins[1]);
    }
    else if (spin == num_spins - 1) 
    {
        energy = spins[num_spins - 1] * (spins[0] + spins[spin - 1]);
    }
    else
    {
        energy = spins[spin] * (spins[spin - 1] + spins[spin + 1]);
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
 * int spins[]: The spin state of the ensamble.
 * int num_spins: The number of spins in the system. 
 *
 * returns
 * -------
 * int: The energy of the ensamble in units of epsilon.
 */
int energy(int spins[], int num_spins)
{
    int energy = 0;
    for (int spin = 0; spin < num_spins; spin++) 
    {
        // Just the nearest neighours contribute to the energy.
        energy += spin_energy(spin, spins, num_spins);
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
 * int num_spins: The number of spins in the system.
 *
 * returns
 * -------
 * float entropy: The entropy of the system.  
 */
float entropy(int spins[], int num_spins) 
{
    int up_spins = 0;
    for (int spin=0; spin < num_spins; spin++)
    {
        up_spins += (int)(spins[spin] > 0);
    }
    int down_spins = num_spins - up_spins;
    int multiplicity = factorial(num_spins) /\
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
 * float temperature: The temperature of the ensamble.
 * int num_spins: The number of spins in the ensamble.
 * 
 * returns
 * -------
 * float free_energy: The free energy.
 */
float free_energy(int spins[], float temperature, int num_spins)
{
    return (float)energy(spins, num_spins) - temperature *\
        entropy(spins, num_spins);
}


/*
 * heat_capacity
 * -------------
 * Calculate the heat capacity of the ensamble.
 *
 * parameters
 * ----------
 * int spins[]: The spins ensamble.
 * float temperature: The temperature of the ensamble.
 * int num_spins: The number of spins in the ensamle.
 * 
 * returns
 * -------
 * float heat_capacity: The heat capacity. 
 */
float heat_capacity(int spins[], float temperature, int num_spins)
{
    int total = 0;
    for (int spin=0; spin < num_spins; spin++)
    {
        total += spins[spin];
    }
    float energy_average = (float) total / (float) num_spins;
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
 * int num_spins: The number of spins in the system. 
 * 
 * returns
 * -------
 * void: The array modifications are done in place. 
 */
void metropolis_step(int spins[], float temperature, int num_spins)
{

    int spin = (int) (normalised_random() * num_spins);
    int energy_change = -2 * spin_energy(spin, spins, num_spins) - 
        4 * spins[spin];
   
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
 * int num_spins: The number of spins in the system. 
 * 
 * returns 
 * -------
 * void: The spins array is internal modified on the heap. 
 */
void random_system(int spins[], int num_spins)
{
    for (int spin = 0; spin < num_spins; spin++) 
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
 * int num_spins: The number of spins in the system. 
 * 
 * returns
 * -------
 * void: Writes the state to a file.
 */
void system_to_file(int spins[], char* file_name, int num_spins) 
{
    FILE *data = fopen(file_name, "w");
    if (!data)
    {
        printf("Error: File not found!");
        exit(1);
    }
    printf("Writing to file!");
    for (int spin = 0; spin < num_spins; spin++)
    {
        fprintf(data, "%i, %i, %i\n", 1, spin, spins[spin]);
    }
       
    fclose(data);
}
