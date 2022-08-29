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
 * 2d_spin_energy
 * --------------
 * Calculates the spin energy in two dimensions. 
 * 
 * parameters
 * ----------
 * int spin: The index of the specific spin.
 * int spins[]: The array representing the ensamble of spins.
 * int num_spins: The number of spins in the representation.
 * 
 * returns
 * -------
 * int: The energy contribution of this specific spin.
 * 
 * TODO: Is there some way to take my existing 1d ising code and make
 * that work in two dimensions without having to re-write it. The answer is 
 * yes and it just comes down to correct determination of the neighbours 
 * and then I can just use it out of the box. In other words I just need 
 * to overide the spin_energy method. 

 * The other option is of course just to re-write all of the code here. 
 * Maybe that will be the simplest. 
 * How do I find the edges?

 * i % 5:   00, 01, 02, 03, 04  (i / 5)
 *
 *          00, 01, 02, 03, 04  00
 *          05, 06, 07, 08, 09  01
 *          10, 11, 12, 13, 14  02
 *          15, 16, 17, 18, 19  03
 *          20, 21, 22, 23, 24  04
 * 
 * Hence we are on a boundary if (i / num) == 0 or (i / num) == (num - 1)
 * or (i % num) == 0 or (i % num) == (num - 1). Now consider the neighbours
 *                              
 *          000, 001, 002, 003, *04 
 *          005, 006, 007, *08, *09 
 *          010, 011, 012, 013, *14 
 *          015, 016, 017, 018, 019 
 *          020, 021, 022, 023, 024 
 *
 * So in general the neighbours are (spin - num) , (spin + num). These are the 
 * above and below. Then the left and right neighbours are just (spin + 1)
 * and (spin - 1).
 *
 * If we are on the left boundary (i.e. (i % num == 0) then I need to consider:
 *  1. (spin + num),
 *  2. (spin - num), 
 *  3. (spin + 1),
 *  4. (spin + num - 1).
 * If I am on the right boundary (i.e. (i % num == (num - 1)) then I need to 
 * consider:
 *  1. (spin + num)
 *  2. (spin - num)
 *  3. (spin - 1)
 *  4. (spin - num + 1)
 * If I am on the top boundary (i.e. (i / num == 0)) then I need to consider:
 *  1. (spin + num)
 *  2. (num * (num - 1) + spin)
 *  3. (spin + 1)
 *  4. (spin - 1)
 * Finally if I am on the bottom boundary (i.e. (i / num == (num - 1)) then
 * I need to consider:
 *  1. (spin - num)
 *  2. (spin - num * (num - 1))
 *  3. (spin + 1)
 *  4. (spin - 1)
 *
 * OK it should work perfectly with just this change to the spin energy 
 * calculation.
 *
 * Hang on I need to work out corners. These are the squares where both 
 * (i / num == 0 and i % num == 0) or 
 * (i / num == 0 and i % num == (num - 1)) or
 * (i / num == (num - 1) and i % num == 0) or 
 * (i / num == (num - 1) and i % num == (num - 1)). Alternatively,
 * They should be calculatable in advance as 0, num - 1, num * (num - 1) and 
 * num * num - 1
 * 
 * At the top left corner (i.e. spin == 0) I need to consider:
 *  1. (spin + 1) == 1
 *  2. (spin + num) == num
 *  3. (num * (num - 1))
 *  4. (num - 1)
 */
int 2d_spin_energy(int spin, int spins[], int num_spins)
{
    int num = (int) sqrt(num_spins);
    int energy = 0;

    switch (spin % num)
    {
        case 0: 
        {
            energy += spins[spin] * (spins[spin + 1] + spins[spin + num - 1]);
            break;
        }
        case num - 1: 
        {
            energy += spins[spin] * (spins[spin - 1] + spins[spin - num + 1]);
            break;
        }
        default:
        {
            energy += spins[spin] * (spins[spin + num] + spins[spin - num]);
            break;
        }
    }

    switch ((int) spin / num)
    {
        case 0:
        {
            energy += spins[spin] * (spins[spin + num] + 
                spins[spin + num * (num - 1)]);
            break;
        }
        case num - 1:
        {
            energy += spins[spin] * (spins[spin - num] + 
                spins[spin - num * (num - 1)]) 
            break;
        }
        default:
        {
            energy += spins[spin] * (spins[spin - num] + spins[spin + num]);
            break;
        }
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
int 1d_spin_energy(int spin, int spins[], int num_spins)
{
    int energy = 0;
    switch (spin) 
    {
        case 0: 
        {
            energy += spins[0] * (spins[num_spins] + spins[1]);
            break;
        }
        case num_spins - 1:
        {
            energy += spins[num_spins - 1] * (spins[0] + spins[spin - 1]);
            break;
        }
        default:
        {
            energy += spins[spin] * (spins[spin - 1] + spins[spin + 1]);
            break;
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
    int energy_change = - 4 * spin_energy(spin, spins, num_spins);
   
    if (energy_change <= 0)
    {
        spins[spin] = -spins[spin];
    } 
    else
    {
        float probability = exp(- energy_change / temperature);
        if (probability >= normalised_random()) 
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
 * magnetisation
 * -------------
 * Calculate the net magnetisation of the system.
 * 
 * parameters
 * ----------
 * int spins[]: The spin system.
 * int num_spins: The number of spins in the system.
 *
 * returns
 * -------
 * int magnetisation: The net magnetisation.
 */
int magnetisation(int spins[], int num_spins)
{
    int magnetisation = 0;
    for (int spin = 0; spin < num_spins; spin++)
    {
        if (spin > 0)
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
