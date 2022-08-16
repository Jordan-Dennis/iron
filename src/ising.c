#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


const int n = 1e2;
const char dir[] = "/home/jordan/Documents/PHYS3920/computational_project/pub/";
const float MAX = 5.;
const int REPS = 1e3;


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
int energy(int spins[])
{
    int energy = 0;
    for (int spin = 0; spin < n; spin++) 
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
    for (int spin=0; spin < n; spin++)
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
    for (int spin=0; spin < n; spin++)
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


int main(int number_of_args, char *args[])
{
    if (number_of_args == 2)
    {
        printf("%s\n", args[1]);
        printf("%i\n", strcmp(args[1], "hello"));
        if (strcmp(args[1], "hello"))
        {
            printf("I made it");
        }
    }
    else 
    {
        printf("\033[31merror\033[0mPlease provide a question number");
    }

    for (float temperature=1.; temperature <= MAX; temperature++)
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
            fprintf(data, "%i, %i, %i\n", 0, spin, value);
        }


        for (int epoch = 0; epoch <= REPS * n; epoch++)
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


