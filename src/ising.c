#include <stdio.h>
#include <stdlib.h>
#include <math.h>


const float k = 1.83e-23;
const int N = 32 * 1e0;
const float temperature = 1.;


// TODO: Provide doc-string.
float random(void) { return (float) rand() / (float) RAND_MAX }


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
    for (int spin = 0; spin < N; spin++) {
        // Just the nearest neighours contribute to the energy.
        energy += spin_energy[spin];
    }
    return energy;
}


// TODO: Provide doc-string.
int spin_energy(int spin, int spins[])
{
    return spins[spin] * spins[spin - 1] + 
        spins[spin] * spins[spin + 1];
}


void main()
{
    // Generating the initial state.
    int spins[N];
    for (int spin = 0; spin < N; spin++) {
        spins[spin] = random_spin();
        // printf("%i ", spins[spin]);
    }
    
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
    // (-1, +1, -1)
    // (+1, +1, +1 => +2) --> (+1, -1, +1 => -2)
    // So I also need to calculate how the neighbours contribution to 
    // the energy will change. This may well be different to what I 
    // have.
    int energy_change = -2 * spin_energy(spin, spins); 
    // Since the only possible value of energy that is greater than 
    // zero is 1, this means that the probability of the flip 
    // is a constant. NOTE: This is only for a given temperature. 
    float probability = exp(-energy_change / k * T);
}


