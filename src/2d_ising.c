#include<omp.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"include/toml.h"
#include"include/utils.h"
#include"include/2d_ising.h"


/*
 * init_2d_ising_system
 * --------------------
 * Construct a two dimensional model of the ising system.
 *
 * parameters
 * ----------
 * int length: The number of spins desired along one edge of the model.
 *
 * returns
 * -------
 * Ising2D* model: A random spin state of the desired size. 
 */
Ising2D* init_ising_2d(int length, float temperature)
{
    int **ensemble = (int**) calloc(length, sizeof(int*));  
    for (int col = 0; col < length; col++)
    {
        ensemble[col] = (int*) calloc(length, sizeof(int));
    } 

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            ensemble[row][col] = random_spin();
        }
    }

    Ising2D* system = (Ising2D*) calloc(1, sizeof(Ising2D));
    system -> length = length;
    system -> temperature = temperature;
    system -> ensemble = ensemble;
    return system;
}


/*
 * spin_energy_ising_2d
 * --------------------
 * Calculate the energy of a specific spin in the system. 
 *
 * parameters
 * ----------
 * const Ising2D *system: The system of spins.
 * int row: The row coordinate of the spin.
 * int col: The column coordinate of the spin. 
 *
 * returns
 * -------
 * The energy contribution of this specific spin. 
 */
float spin_energy_ising_2d(const Ising2D *system, int row, int col)
{
    int **ensemble = system -> ensemble;
    int length = system -> length;
    return ensemble[row][col] * (
        ensemble[modulo(row + 1, length)][col] +
        ensemble[modulo(row - 1, length)][col] +
        ensemble[row][modulo(col + 1, length)] +
        ensemble[row][modulo(col - 1, length)]);
}


/*
 * energy_ising_2d
 * ---------------
 * Calculate the energy of the 2D ising simulation with periodic boundary 
 * conditions. 
 *
 * parameters
 * ----------
 * const Ising2D *system: The 2D ising system.
 *
 * returns
 * -------
 * int energy: The energy.
 */
float energy_ising_2d(const Ising2D *system)
{
    int length = system -> length;
    float energy = 0;

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            energy -= spin_energy_ising_2d(system, row, col);
        }
    }

    return energy / 2.;
}


/*
 * flip_spin_ising_2d
 * ------------------
 * Reverse the direction of a specific spin within the system.
 *
 * parameters
 * ----------
 * Ising2D *system: The system to be modified.
 * int row: The row coordinate of the spin getting flipped.
 * int col: The column coordinate of the spin getting flipped.
 */
void flip_spin_ising_2d(Ising2D *system, int row, int col)
{
    system -> ensemble[row][col] *= -1;
}


/*
 * print_ising_2d
 * --------------
 * A convinience function for debugging. Prints the system compactly 
 * into the terminal on a uniform grid.
 *
 * parameters
 * ----------
 * Ising2D *system: The system to display.
 */
void print_ising_2d(Ising2D *system)
{
    int length = system -> length;
    int **ensemble = system -> ensemble;
    
    printf("2D Ising System:\n");
    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            printf("%i", ensemble[row][col] > 0);
        }
        printf("\n");
    }
}


/*
 * metropolis_step
 * ---------------
 * Evolve the spin state according to a metropolis algorithm. 
 *
 * parameters
 * ----------
 * System* system: The spin ensamble to evolve.  
 */
void metropolis_step_ising_2d(Ising2D *system)
{
    int row = random_index(system -> length);
    int col = random_index(system -> length);
    int energy_change = 2 * spin_energy_ising_2d(system, row, col);
   
    if (energy_change < 0)
    {
        flip_spin_ising_2d(system, row, col);
    } 
    else
    {
        float probability = exp(- energy_change / (system -> temperature));
        if (probability > normalised_random()) 
        {
            flip_spin_ising_2d(system, row, col);
        }
    }
}


/*
 * entropy_ising_2d
 * ----------------
 * Calculate the entropy of a two dimensional ising model.
 *
 * parameters
 * ----------
 * Ising2D *system: The system to calculate the entropy of.
 * 
 * returns
 * -------
 * float entropy: The entropy of the system. 
 */
float entropy_ising_2d(Ising2D *system)
{
    int len = system -> length;
    int **ensemble = system -> ensemble; 
    int up = 0;

    for (int row = 0; row < len; row++)
    {
        for (int col = 0; col < len; col++)
        {
            up += ensemble[row][col] == ensemble[modulo(row + 1, len)][col];
            up += ensemble[row][col] == ensemble[row][modulo(col + 1, len)];
        }
    }
        
    int total = 2 * len * len;
    int down = total - up;

    if (up == 0 || up == total)
    {
        return log(2);
    }
    else
    {
        return total * log(total) - up * log(up) - down * log(down);
    }
}


/*
 * magnetisation_ising_2d
 * ----------------------
 * Calculate the magnetisation of the 2D ising model. 
 *
 * parameters
 * ----------
 * Ising2D *system: The system to calculate the total magnetisation of.
 *
 * returns
 * -------
 * int magnetisation: The magetisation of the system. 
 */
int magnetisation_ising_2d(const Ising2D *system)
{
    int length = system -> length;
    int **ensemble = system -> ensemble;
    int magnetisation = 0;

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            magnetisation += ensemble[row][col];
        }
    }

    return magnetisation; 
}


/* 
 * save_ising_2d
 * -------------
 * Save the current state of the system to a file. 
 *
 * parameters
 * ----------
 * Ising2D *system: The ising model.
 * FILE *save_file: The file to save the model in. 
 */
void save_ising_2d(Ising2D *system, FILE *save_file)
{
    int length = system -> length;
    int **ensemble = system -> ensemble;
    float temp = system -> temperature;
    
    fprintf(save_file, "# Temperature = %f\n", temp);

    for (int row = 0; row < length; row++)
    {
        for(int col = 0; col < length - 1; col++)
        {
            fprintf(save_file, "%i,", ensemble[row][col]);
        }
        
        fprintf(save_file, "%i\n", ensemble[row][length - 1]);
    }
}


// TODO: Needs documentation. 
void first_and_last_ising_2d(Config *config)
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
        Ising2D* system = init_ising_2d(num_spins, temp);

        save_ising_2d(system, save_file);

        // Running the metropolis algorithm over the system. 
        for (int epoch = 0; epoch <= num_spins * 1e3; epoch++)
        { 
            metropolis_step_ising_2d(system);
        }

        save_ising_2d(system, save_file);
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
void physical_parameters_ising_2d(Config* config)
{
    int low_num_spins = atoi(find(config, "low_number_of_spins"));
    int mid_num_spins = atoi(find(config, "mid_number_of_spins"));
    int high_num_spins = atoi(find(config, "high_number_of_spins"));
    int spin_nums[] = {low_num_spins, mid_num_spins, high_num_spins};
    char *save_file_name = find(config, "save_file");
    float start = atof(find(config, "lowest_temperature"));
    float stop = atof(find(config, "highest_temperature"));
    float step = atof(find(config, "temperature_step"));

    int num_temps = (int) ((stop - start) / step);

    float energies_and_error[num_temps][2][3];
    float entropies_and_error[num_temps][2][3];
    float free_energies_and_error[num_temps][2][3];
    float heat_capacities_and_error[num_temps][3];
    
    int ind;    
    float temp;

    //#pragma omp parallel for num_threads(3) shared(energies_and_error)
    for (int num_spin = 0; num_spin < 3; num_spin++)
    {
        printf("Running num_spin: %i\n", num_spin);
        int num_spins = spin_nums[num_spin];
        int num_epochs = num_spins * 1e3;
        int size = num_spins * num_spins;

        Ising2D *system = init_ising_2d(num_spins, stop);

        for (int epoch = 0; epoch < num_epochs; epoch++)
        {
            metropolis_step_ising_2d(system);
        }

        for (temp = stop - step, ind = 0; temp >= start; temp -= step, ind++)
        {
            system -> temperature = temp;
        
            float sim_energies[num_epochs];
            float sim_entropies[num_epochs];
            float sim_free_energies[num_epochs];
            
            for (int epoch = 0; epoch < num_epochs; epoch++)
            { 
                metropolis_step_ising_2d(system);
                float energy = energy_ising_2d(system);
                float entropy = entropy_ising_2d(system);

                sim_energies[epoch] = energy;
                sim_entropies[epoch] = entropy;
                sim_free_energies[epoch] = energy - temp * entropy;
            }
            
            float mean_energy = mean(sim_energies, num_epochs);
            float mean_entropy = mean(sim_entropies, num_epochs);
            float mean_free_energy = mean(sim_free_energies, num_epochs);

            float var_energy = variance(sim_energies, mean_energy, num_epochs);
            float var_entropy = variance(sim_entropies, mean_entropy, num_epochs);
            float var_free_energy = variance(sim_free_energies, mean_free_energy, num_epochs);
            float mean_heat_capacity = var_energy / temp / temp;

            energies_and_error[ind][1][num_spin] = sqrt(var_energy) / size;
            energies_and_error[ind][0][num_spin] = mean_energy / size;
            entropies_and_error[ind][1][num_spin] = sqrt(var_entropy) / size;
            entropies_and_error[ind][0][num_spin] = mean_entropy / size;
            free_energies_and_error[ind][1][num_spin] = sqrt(var_free_energy) / size;
            free_energies_and_error[ind][0][num_spin] = mean_free_energy / size;
            heat_capacities_and_error[ind][num_spin] = mean_heat_capacity / size;
        }
    }
    
	// Writing the data to the file
	FILE* data = fopen(save_file_name, "w");

    if (data == NULL)
    {
        printf("Error: Could not open '%s'", save_file_name);
        exit(1);
    }

	// Writing the header row to the data. 
	fprintf(data, "Number, Temperature, ");
    fprintf(data, "Energy, Energy Error, "); 
    fprintf(data, "Entropy, Entropy Error, "); 
    fprintf(data, "Free Energy, Free Energy Error, "); 
    fprintf(data, "Heat Capacity\n");

    // TODO: Change the 3D tensors into two 2D tensors. 
    for (int num_spin = 0; num_spin < 3; num_spin++)
    {
        for (temp = start, ind = 0; temp < stop; temp += step, ind++)
        {
            fprintf(data, "%i, ", spin_nums[num_spin]);
            fprintf(data, "%f, ", temp);
            fprintf(data, "%f, ", energies_and_error[ind][0][num_spin]);
            fprintf(data, "%f, ", energies_and_error[ind][1][num_spin]);
            fprintf(data, "%f, ", entropies_and_error[ind][0][num_spin]);
            fprintf(data, "%f, ", entropies_and_error[ind][1][num_spin]);
            fprintf(data, "%f, ", free_energies_and_error[ind][0][num_spin]);
            fprintf(data, "%f, ", free_energies_and_error[ind][1][num_spin]);
            fprintf(data, "%f\n", heat_capacities_and_error[ind][num_spin]);
        }
    }
	
	fclose(data);
}


void magnetisation_vs_temperature()
{

}
