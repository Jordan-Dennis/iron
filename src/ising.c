#include <stdio.h>
#include <stdlib.h>
#include <math.h>


const int N = 1e3;
const float T = 1.;
const char DATA[] = "/home/jordan/Documents/PHYS3920/"
    "computational_project/out/data.txt";


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
 * potential_energy
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
int ensamble_energy(int spins[])
{
    int energy = 0;
    for (int spin = 0; spin < N; spin++) 
    {
        // Just the nearest neighours contribute to the energy.
        energy += spin_energy(spin, spins);
    }
    return energy;
}


int main(void)
{

    FILE *data = fopen(DATA, "w");

    int spins[N]; 
    // Copy initial state so that it can be written to a file.
    for (int spin = 0; spin < N; spin++) 
    {
        int value = random_spin();
        spins[spin] = value;
        fprintf(data, "%i, %i, %i\n", 0, spin, value);
    }


    for (int epoch = 0; epoch <= 1e3 * N; epoch++)
    {
        int spin = (int) (normalised_random() * 16);
        // (+1, +1, -1 =>  0) --> (+1, -1, -1 =>  0)
        // (+1, -1, -1 =>  0) --> (+1, +1, -1 =>  0)
        // (+1, -1, +1 => -2) --> (+1, +1, +1 => +2)
        // (-1, +1, -1 => -2) --> (-1, -1, -1 => +2)
        // (+1, +1, +1 => +2) --> (+1, -1, +1 => -2)
        // (-1, -1, -1 => +2) --> (-1, +1, -1 => -2)
        // (-1, +1, +1 =>  0) --> (-1, -1, +1 =>  0)
        // (-1, -1, +1 =>  0) --> (-1, +1, +1 =>  0)
        // For the edge ones I think you add or subtract 2 based on the 
        // sign that was flipped. Seems to work.
        int energy_change = -2 * spin_energy(spin, spins) - 4 * spins[spin];
       
        if (energy_change <= 0)
        {
            spins[spin] = -spins[spin];
        } 
        else
        {
            float probability = exp(-energy_change / T);
            if (probability <= normalised_random()) 
            {
                spins[spin] = -spins[spin];
            }
        }
    }
       
       
    for (int spin = 0; spin < N; spin++)
    {
        fprintf(data, "%i, %i, %i\n", 1, spin, spins[spin]);
    }
       
    fclose(data);
    return 0;
}


