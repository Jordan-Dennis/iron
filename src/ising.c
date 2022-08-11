#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


const int n = 1e2;
const char dir[] = "/home/jordan/Documents/PHYS3920/computational_project/pub/";


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
    for (int spin = 0; spin < n; spin++) 
    {
        // Just the nearest neighours contribute to the energy.
        energy += spin_energy(spin, spins);
    }
    return energy;
}


int main(void)
{
    for (float temperature=1.; temperature <= 3.; temperature++)
    {
        char file[10];
        sprintf(file, "1d_%i.txt", (int)temperature);
        char address[strlen(file) + strlen(dir)];
        strcpy(address, dir);
        strcat(address, file);
        FILE *data = fopen(address, "w");
        

        int spins[n]; 
        // Copy initial state so that it can be written to a file.
        for (int spin = 0; spin < n; spin++) 
        {
            int value = random_spin();
            spins[spin] = value;
            // fprintf(data, "%i, %i, %i\n", 0, spin, value);
        }


        for (int epoch = 0; epoch <= 1e3 * n; epoch++)
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
           
           
        for (int spin = 0; spin < n; spin++)
        {
            fprintf(data, "%i, %i, %i\n", 1, spin, spins[spin]);
        }
           
        fclose(data);
    }
    return 0;
}


