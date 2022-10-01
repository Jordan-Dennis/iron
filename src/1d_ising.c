#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"include/toml.h"
#include"include/utils.h"
#include"include/1d_ising.h"



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
    int length = system -> length;
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

    for (int spin = 0; spin < system -> length; spin++) 
    {
        energy -= spin_energy_ising_1d(system, spin);
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
    int length = system -> length;
    int* ensemble = system -> ensemble; 
    int up_spins = 0;

    for (int spin = 0; spin < length; spin++)
    {
        if (ensemble[spin] == (ensemble[modulo(spin + 1, length)]))
        {
            up_spins++;
        }
    }

    int down_spins = length - up_spins;

    float entropy = length * log(length) - up_spins * log(up_spins) 
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
 * flip_spin_ising_1d
 * ------------------
 * Flip a spin in the system.
 *
 * parameters
 * ----------
 * Ising1D *system: The system within which to flip.
 * int spin: The index of the spin to flip.
 */
void flip_spin_ising_1d(Ising1D *system, int spin)
{
    system -> ensemble[spin] *= -1;
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
void metropolis_step_ising_1d(Ising1D* system)
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


/*
 * print_ising_1d
 * --------------
 * A useful debugging utility for developing the model.
 * 
 * parameters
 * ----------
 * Ising1D *system: The system to print.
 */
void print_ising_1d(Ising1D *system)
{
    int length = system -> length;
    int *ensemble = system -> ensemble;
    
    printf("1D Ising System:\n");
    for (int spin = 0; spin < length; spin++)
    {
        printf("%i", ensemble[spin] > 0); 
    }
    printf("\n");
}


void save_ising_1d(const Ising1D *system, FILE *file)
{
    int length = system -> length;
    int *ensemble = system -> ensemble;
    float temperature = system -> temperature;

    fprintf(file, "# Temperature: %f\n", temperature);

    for (int spin = 0; spin < (length - 1); spin++) 
    {
        fprintf(file, "%i,", ensemble[spin]);
    }

    fprintf(file, "%i\n", ensemble[length - 1]);
}


/*
 * first_and_last 
 * --------------
 * A one dimensional ising model with periodic boundaries using the 
 * metropolis algorithm. Provide the initial and the final outputs 
 * from the simulation for at least three different temperatures.
 * What do you notice about the size of the chunks of color at 
 * low temperatures compared to high temperatures. 
 */
void first_and_last(Config *config)
{
    int num_spins = atoi(find(config, "number_of_spins"));
    char *save_file_name = find(config, "save_file");
    float start = atof(find(config, "lowest_temperature"));
    float stop = atof(find(config, "highest_temperature"));
    float step = atof(find(config, "temperature_step"));

    free(config);

    int num_temps = (int) ((stop - start) / step);
    FILE *save_file = fopen(save_file_name, "w");

    for (int temp = start, ind = 0; temp < stop; temp += step, ind++)
    {
        Ising1D* system = init_ising_1d(num_spins, temp);

        save_ising_1d(system, save_file);

        // Running the metropolis algorithm over the system. 
        for (int epoch = 0; epoch <= num_spins * 1e3; epoch++)
        { 
            metropolis_step_ising_1d(system);
        }

        save_ising_1d(system, save_file);
    }
}




