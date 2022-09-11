#include<math.h>

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
int energy(System* system, int spin)
{
    int *spins = system -> spins;
    int num = system -> number;
    return ;
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
    int up_spins = 0;
    for (int spin = 0; spin < number; spin++)
    {
        up_spins += (int)((system -> spins)[spin] > 0);
    }
    int down_spins = number - up_spins;
    int multiplicity = factorial(number) /\
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
 * heat_capacity
 * -------------
 * Calculate the heat capacity of the ensamble.
 *
 * parameters
 * ----------
 * System* system: The spin ensamble to calculate the heat capacity of
 * 
 * returns
 * -------
 * float heat_capacity: The heat capacity. 
 */
float heat_capacity(System* system)
{
    int number = system -> number;
    float energies[number];

    for (int spin = 0; spin < (number); spin++)
    {
        energies[spin] = spin_energy(system, spin);
    }

    float energy_mean = mean(energies, number);
    float energy_variance = variance(energies, energy_mean, number); 
    float heat_capacity = energy_variance / pow(system -> temperature, 2);
    printf("Energy Mean: %f\n", energy_mean);
    printf("Energy Variance: %f\n", energy_variance);
    return heat_capacity;
}


int main(void)
{
    int num = 100;
    int reps = 1000;
    int spins[num];
    float low = 0.1;
    float high = 4.0;
    float step = 0.1;

    for (int spin = 0; spin < 100; spin++)
    {
        spins[spin] = random_spin();
    }

    float energies[(int) (high - low) / step];
    float heat_capacities[(int) (high - low) / step];

    for (int temperature = low; temperature <= high; temperature += step)
    {
        float simulation_energies[reps];
        for (int epoch = 0; epoch < reps; epoch++)
        {
            int spin = (int) (normalised_random() * (system -> number));
            int energy_change = - 2 * spin_energy(system, spin);
           
            if (energy_change < 0)
            {
                spins[spin] *= -1;
            } 
            else
            {
                float probability = exp(- energy_change / temperature);
                if (probability > normalised_random()) 
                {
                    spins[spin] *= -1;
                }
            }
            simulation_energies[epoch] = 
                spins[spin] * spins[(spin + 1) % num] / 2 + 
                spins[spin] * spins[(spin - 1) % num] / 2;
        }
        
    }
}
