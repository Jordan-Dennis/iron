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


/*
 * save_ising_1d
 * -------------
 * Convinient function that save the current state of the system to a 
 * file. 
 *
 * parameters
 * ----------
 * const Ising1D *system: The current state of the system.
 * FILE *file: The file to save the output. 
 */
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
 *
 * parameters
 * ----------
 * Config *config: The configuration file detailing the setup of the system. 
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

    int ind;
    float temp;

    for (temp = start, ind = 0; temp < stop; temp += step, ind++)
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



/*
 * physical_parameters
 * -------------------
 * Compute and plot figures for energy, free energy, entropy and 
 * heat capacity and the reduced magnetisation per dipole of the 
 * 1d ising model against temperatur, T, using your simulation
 * with N = 100. Obtain values for at least ten different 
 * temeperatures. 
 * 
 * Compute time averages of these quantities for the best results
 * and make sure that the system reaches thermodynamic equilibrium
 * before taking measurements. Present against the analytic solutions.
 *
 * parameters
 * ----------
 * Config *config: The configuration file detailing the simulation. 
 */
void physical_parameters_ising_1d(Config* config)
{
    int spins = atoi(find(config, "number_of_spins"));
    char *save_file_name = find(config, "save_file");
    float start = atof(find(config, "lowest_temperature"));
    float stop = atof(find(config, "highest_temperature"));
    float step = atof(find(config, "temperature_step"));

    int num_temps = (int) ((stop - start) / step);
    int epochs = 1e3 * spins;

    float energies[num_temps][2];
    float entropies[num_temps][2];
    float free_energies[num_temps][2];
    float heat_capacities[num_temps][2];
    
    int ind;    
    float temp;

    for (temp = start, ind = 0; temp < stop; temp += step, ind++)
    {
        Ising1D *system = init_ising_1d(spins, temp);
    
        // Running the burnin period. 
        for (int epoch = 0; epoch <= epochs; epoch++)
        { 
            metropolis_step_ising_1d(system);
        }

        float _energies[epochs];
        float _entropies[epochs];
        float _free_energies[epochs];
        
        for (int epoch = 0; epoch < epochs; epoch++)
        { 
            metropolis_step_ising_1d(system);
            float energy = energy_ising_1d(system);
            float entropy = entropy_ising_1d(system);

            _energies[epoch] = energy;
            _entropies[epoch] = entropy;
            _free_energies[epoch] = energy - temp * entropy;
        }

        float energy_est = mean(_energies, epochs);
        float entropy_est = mean(_entropies, epochs);
        float free_energy_est = mean(_free_energies, epochs);
        float heat_capacity_est = (energies[ind - 1][0] - energy_est / spins) / step;

        float energy_err = sqrt(variance(_energies, energy_est, epochs) / size);
        float entropy_err = sqrt(variance(_entropies, entropy_est, epochs) / size);
        float free_energy_err = sqrt(variance(_free_energies, free_energy_est, epochs) / size);
        float heat_capacity_err = (energies[ind - 1][1] + energy_err) / step

        energies[ind][1] = energy_err;
        energies[ind][0] = energy_est / spins;
        entropies[ind][1] = entropy_err;
        entropies[ind][0] = entropy_est / spins;
        free_energies[ind][1] = free_energy_err;
        free_energies[ind][0] = free_energy_est / spins;
        heat_capacities[ind][1] = heat_capacity_err;
        heat_capacities[ind][0] = heat_capacity_est;
    }

	// Writing the data to the file
	FILE* data = fopen(save_file_name, "w");

    if (data == NULL)
    {
        printf("Error: Could not open '%s'", save_file_name);
        exit(1);
    }

	// Writing the header row to the data. 
	fprintf(data, "Temperature, ");
    fprintf(data, "Energy, Energy Error, "); 
    fprintf(data, "Entropy, Entropy Error, "); 
    fprintf(data, "Free Energy, Free Energy Error, "); 
    fprintf(data, "Heat Capacity, Heat Capacity Error\n");

    for (temp = start, ind = 0; temp < stop; temp += step, ind++)
	{
        fprintf(data, "%f, ", temp);
		fprintf(data, "%f, ", energies[ind][0]);
		fprintf(data, "%f, ", energies[ind][1]);
		fprintf(data, "%f, ", entropies[ind][0]);
		fprintf(data, "%f, ", entropies[ind][1]);
		fprintf(data, "%f, ", free_energies[ind][0]);
		fprintf(data, "%f, ", free_energies[ind][1]);
        fprintf(data, "%f, ", heat_capacities[ind][0]);
        fprintf(data, "%f\n", heat_capacities[ind][1]);
	}
	
	fclose(data);
}



/*
 * histogram
 * ---------
 * Create a histogram of the m values you obtain by running a 
 * simulation of 500 spins at 1., 2. and 3. temperatures 100 times.
 *
 * parameters
 * ----------
 * Config *config: The configuration file detailing the simulation. 
 */
void magnetisation_ising_1d(Config* config)
{
    int num_spins[2] = {100, 500};
    int reps_per_temp = atoi(find(config, "reps_per_temp"));
    char *save_file_name = find(config, "save_file");
    float start = atof(find(config, "lowest_temperature"));
    float stop = atof(find(config, "highest_temperature"));
    float step = atof(find(config, "temperature_step"));
   
    int length = (int) ((stop - start) / step); 
    float magnetisations[length][reps_per_temp][2]; 

    for (int number = 0; number < 2; number++)
    {
        int ind;    
        float temp;

        for (temp = start, ind = 0; temp < stop; temp += step, ind++)
        {
            Ising1D *system = init_ising_1d(num_spins[number], temp);

            for (int rep = 0; rep < reps_per_temp; rep++)
            {
                int num_epochs = 1e3 * num_spins[number];

                // Running the burnin period. 
                for (int epoch = 0; epoch <= num_epochs; epoch++)
                { 
                    metropolis_step_ising_1d(system);
                }

                // Running the simulation 
                float sim_magnetisation[num_epochs];

                for (int epoch = 0; epoch < num_epochs; epoch++)
                { 
                    metropolis_step_ising_1d(system);
                    sim_magnetisation[epoch] = magnetisation_ising_1d(system);
                }

                float mean_magnetisation = mean(sim_magnetisation, num_epochs);
                magnetisations[ind][rep][number] = mean_magnetisation;
            }
        }
    }

    // Opening the data file. 
    FILE* data = fopen(save_file_name, "w");

    if (data == NULL)
    {
        printf("Error: Could not open '%s'", save_file_name);
        exit(1);
    }
    
    // Printing the header row to the file. 
    for (float temp = start; temp < stop; temp += step)
    {
        fprintf(data, "T%f, ", temp);
    }

    fprintf(data, "\n");

    // Writing the data to the file.
    // TODO: I really need my toml manager to be able to handle arrays.
    for (int rep = 0; rep < reps_per_temp; rep++)
    {
        for (int number = 0; number < 2; number++)
        {
            for (int temp = 0; temp < length; temp++)
            {
                char* fstring = "%f,";

                if ((temp == length - 1) && (number == 1))
                {
                    fstring = "%f\n";
                }

                fprintf(data, fstring, magnetisations[temp][rep][number]);
            }
        }
    }

    // Closing the file
    fclose(data);
}
