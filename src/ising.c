#include <stdio.h>
#include <stdlib.h>
#include <math.h>


const int N = 1e2;
const float temperature = 1.;


/*
 * random
 * ------
 * Generate a random number over the range [0, 1].
 *
 * returns
 * -------
 * float: random number.
 */
float random(void)
{ 
    return (float) rand() / (float) RAND_MAX
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
    float normalised_random = random();
    float translated_random = normalised_random - .5;
    return (int) ((translated_random > 0) - (translated_random < 0));
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
        energy += spin_energy[spin];
    }
    return energy;
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


void main()
{
    // Generating the initial state.
    int spins[N];
    for (int spin = 0; spin < N; spin++) 
    {
        spins[spin] = random_spin();
    }
    

    for (int epoch=0; epoch <= 1e2 * N; epoch++)
    {
        // Metropolis walk over the states.
        // So basically I chose a spin using the rand function
        int spin = (int) rand();
        // Flipping a spin changes the sign of its energy. The middle one 
        // is the one that is in question in the bellow comments. The
        // format is:
        //
        // left, self, right -> energy_change
        //
        // Some useful information to note is that same sign = 1 and 
        // different sign = -1.
        // (+1, +1, -1 =>  0) --> (+1, -1, -1 =>  0)
        // (+1, -1, -1 =>  0) --> (+1, +1, -1 =>  0)
        // (+1, -1, +1 => -2) --> (+1, +1, +1 => +2)
        // (-1, +1, -1 => -2) --> (-1, -1, -1 => +2)
        // (+1, +1, +1 => +2) --> (+1, -1, +1 => -2)
        // (-1, -1, -1 => +2) --> (-1, +1, -1 => -2)
        // (-1, +1, +1 =>  0) --> (-1, -1, +1 =>  0)
        // (-1, -1, +1 =>  0) --> (-1, +1, +1 =>  0)
        // So I also need to calculate how the neighbours contribution to 
        // the energy will change. This may well be different to what I 
        // have.
        //
        // Energy change will be different for the end spins. What will this 
        // be? Just the change in the single element. 
        //
        // Might be easiest to call energy on the neighbourhood of the 
        // spin. This is just the 5 symmetric elements that will be required.
        // centred on the spin.
        // 
        // For the edge ones I think you add or subtract 2 based on the 
        // sign that was flipped. Seems to work.
        int energy_change = -2 * spin_energy(spin, spins) - 4 * spins[spin]; 
        // Since the only possible value of energy that is greater than 
        // zero is 1, this means that the probability of the flip 
        // is a constant. NOTE: This is only for a given temperature. 
        
        if (energy_change <= 0)
        {
            spins[spin] = -spins[spin];
        } 
        else
        {
            float probability = exp(-energy_change / T);
            if (probability <= random()) 
            {
                spins[spin] = -spins[spin];
            }
        }

        if (epoch == 0) || (epoch == N)
        {
            FILE *data = fopen("../out/ising_epoch_zero.txt", "w");
            for (int spin = 0; spin < N, spin++)
            {
                fputc(data, spins[spin]);
                fputc(data, ",");
            }
            fclose(&data);
        }
    }
}


