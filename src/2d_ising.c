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
    
    printf("2D Ising System at %f:\n", system -> temperature);
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
    float temperature = system -> temperature;
   
    if (energy_change < 0)
    {
        flip_spin_ising_2d(system, row, col);
    } 
    else if (normalised_random() <  exp(- energy_change / temperature))
    {
        flip_spin_ising_2d(system, row, col);
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

    int length = (int) ((stop - start) / step);

    float energies_and_error[length][2][3];
    float entropies_and_error[length][2][3];
    float free_energies_and_error[length][2][3];
    float heat_capacities_and_error[length][3];
    
    for (int num_spin = 0; num_spin < 3; num_spin++)
    {
        int num_spins = spin_nums[num_spin];
        int num_epochs = num_spins * 1e3;
        int size = num_spins * num_spins;

        Ising2D *system = init_ising_2d(num_spins, stop);

        for (int epoch = 0; epoch < num_epochs; epoch++)
        {
            metropolis_step_ising_2d(system);
        }

        for (int temp = 0; temp < length; temp++)
        {
            float temperature = stop - (temp + 1) * step;
            float sim_energies[num_epochs];
            float sim_entropies[num_epochs];
            float sim_free_energies[num_epochs];

            system -> temperature = temperature;
            
            for (int epoch = 0; epoch < num_epochs; epoch++)
            { 
                metropolis_step_ising_2d(system);
                float energy = energy_ising_2d(system);
                float entropy = entropy_ising_2d(system);

                sim_energies[epoch] = energy;
                sim_entropies[epoch] = entropy;
                sim_free_energies[epoch] = energy - temperature * entropy;
            }
            
            float mean_energy = mean(sim_energies, num_epochs);
            float mean_entropy = mean(sim_entropies, num_epochs);
            float mean_free_energy = mean(sim_free_energies, num_epochs);

            float var_energy = variance(sim_energies, mean_energy, num_epochs);
            float var_entropy = variance(sim_entropies, mean_entropy, num_epochs);
            float var_free_energy = variance(sim_free_energies, mean_free_energy, num_epochs);
            float mean_heat_capacity = var_energy / temp / temp;

            energies_and_error[temp][1][num_spin] = sqrt(var_energy) / size;
            energies_and_error[temp][0][num_spin] = mean_energy / size;
            entropies_and_error[temp][1][num_spin] = sqrt(var_entropy) / size;
            entropies_and_error[temp][0][num_spin] = mean_entropy / size;
            free_energies_and_error[temp][1][num_spin] = sqrt(var_free_energy) / size;
            free_energies_and_error[temp][0][num_spin] = mean_free_energy / size;
            heat_capacities_and_error[temp][num_spin] = mean_heat_capacity / size;
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
        for (int temp = 0; temp < length; temp++)
        {
            float temperature = stop - (temp + 1) * step;
            fprintf(data, "%i, ", spin_nums[num_spin]);
            fprintf(data, "%f, ", temperature);
            fprintf(data, "%f, ", energies_and_error[temp][0][num_spin]);
            fprintf(data, "%f, ", energies_and_error[temp][1][num_spin]);
            fprintf(data, "%f, ", entropies_and_error[temp][0][num_spin]);
            fprintf(data, "%f, ", entropies_and_error[temp][1][num_spin]);
            fprintf(data, "%f, ", free_energies_and_error[temp][0][num_spin]);
            fprintf(data, "%f, ", free_energies_and_error[temp][1][num_spin]);
            fprintf(data, "%f\n", heat_capacities_and_error[temp][num_spin]);
        }
    }
	
	fclose(data);
}


/*
 * zeros
 * -----
 * Construct an array of zeros as floats.
 *
 * parameters
 * ----------
 * int size: The length of the one-dimensional array. 
 *
 * returns
 * float *arr: A floating point array full of zeros. 
 */
float *zeros(int size)
{
    float *zeros = (float*) calloc(size, sizeof(float));
    
    for (int elem = 0; elem < size; elem++)
    {
        zeros[elem] = 0.0;
    }

    return zeros;
}


/*
 * magnetisation_vs_temperature
 * ----------------------------
 * This maps the positive and negative magnetisations of the system to 
 * the temperature. 
 *
 * parameters
 * ----------
 * Config *config: The configuration of the system to use.
 */
void magnetisation_vs_temperature(Config* config)
{
    int low_num_spins = atoi(find(config, "low_number_of_spins"));
    int mid_num_spins = atoi(find(config, "mid_number_of_spins"));
    int high_num_spins = atoi(find(config, "high_number_of_spins"));
    int spin_nums[3] = {low_num_spins, mid_num_spins, high_num_spins};
    char *save_file_name = find(config, "save_file");
    float start = atof(find(config, "lowest_temperature"));
    float stop = atof(find(config, "highest_temperature"));
    float step = atof(find(config, "temperature_step"));
    int length = (int) ((stop - start) / step);
    int num_reps = 100;

    float magnetisations[3][length][2][2]; // Num, temp, sign, est/err
    Ising2D *system;

    for (int num = 0; num < 3; num++)
    {
        float sim_mags[length][num_reps];
        
        for (int iter = 0; iter < num_reps; iter++)
        {
            system = init_ising_2d(spin_nums[num], stop - step);
            
            // Running the burnin-period.  
            for (int epoch = 0; epoch < 1e3 * spin_nums[num]; epoch++)
            {
                metropolis_step_ising_2d(system);
            }

            for (int temp = 0; temp < length; temp++)
            {
                for (int _ = 0; _ < 1e3 * spin_nums[num]; _++)
                {
                    metropolis_step_ising_2d(system);
                }

                sim_mags[temp][iter] = (float) magnetisation_ising_2d(system);
                system -> temperature = (stop - ((float) (temp + 1)) * step);
//                print_ising_2d(system);
            }
//            printf("Am I just in front of the seg fault?\n");
       }

        for (int temp = 0; temp < length; temp++)
        {
            int num_pos = 0;

//            printf("Temp: %i\n", temp);
            for (int iter = 0; iter < num_reps; iter++)
            {
//                printf("%.1f,", sim_mags[temp][iter]);
                num_pos += (sim_mags[temp][iter] > 0);
            } 
//            printf("\n");    
    
            int num_neg = num_reps - num_pos;

            if ((num_neg == 0) || (num_pos == 0))
            {
                printf("Error: Either no negative or positive runs occurred");
                exit(1);
            }

            float *positives = zeros(num_pos);
            float *negatives = zeros(num_neg);

            int pos = 0, neg = 0;
            for (int iter = 0; iter < num_reps; iter++)
            {
                if (sim_mags[temp][iter] > 0)
                {
                    positives[pos] = sim_mags[temp][iter];
                    pos++;
                }
                else
                {
                    negatives[neg] = sim_mags[temp][iter];
                    neg++;
                }
            }

//            printf("Positives:\n");
//            for (int pos = 0; pos < num_pos; pos++)
//            {
//                printf("%.1f,", positives[pos]);
//            }
//            printf("\n");
           
            float pos_mag_est = mean(positives, num_pos);
            float pos_mag_err = variance(positives, pos_mag_est, num_pos);

//            printf("Mean: %.1f\n", pos_mag_est);

            float neg_mag_est = mean(negatives, num_neg);
            float neg_mag_err = variance(negatives, neg_mag_est, num_neg);

            // TODO: Improve the error by dividing by sqrt(reps)
            magnetisations[num][temp][0][0] = pos_mag_est;
            magnetisations[num][temp][0][1] = sqrt(pos_mag_err / (float) num_pos);
            magnetisations[num][temp][1][0] = neg_mag_est;
            magnetisations[num][temp][1][1] = sqrt(neg_mag_err / (float) num_neg);
        }
    }

    FILE *save_file = fopen(save_file_name, "w");

    if (save_file == NULL)
    {
        printf("Error: Could not open '%s' for writing!", save_file_name);
        exit(1);
    }

    for (int num = 0; num < 3; num++)
    {
        for (int temp = 0; temp < length; temp++)
        {
            float temperature = stop - (temp + 1) * step;
            fprintf(save_file, "%i, %.1f, ", spin_nums[num], temperature);
            fprintf(save_file, "%f, ", magnetisations[num][temp][0][0]);
            fprintf(save_file, "%f, ", magnetisations[num][temp][0][1]);
            fprintf(save_file, "%f, ", magnetisations[num][temp][1][0]);
            fprintf(save_file, "%f\n", magnetisations[num][temp][1][1]);
        }
    }
    
    fclose(save_file); 
}


/*
 * cooling_and_heating
 * -------------------
 * Steadily heat and then cool the system to observe the phase transistion 
 * in each direction.
 *
 * parameters
 * ----------
 * Config *config: The configuration of the system.
 */
void heating_and_cooling(Config *config)
{
    int num_spins = atoi(find(config, "number_of_spins"));
    char *save_file_name = find(config, "save_file");
    float start = atof(find(config, "lowest_temperature"));
    float stop = atof(find(config, "highest_temperature"));
    float step = atof(find(config, "temperature_step"));
    int length = (int) ((stop - start) / step);

    Ising2D *system = init_ising_2d(num_spins, start);
    FILE *save_file = fopen(save_file_name, "w");

    for (int epoch = 0; epoch < 1e3 * num_spins; epoch++)
    {
        metropolis_step_ising_2d(system);
    }

    save_ising_2d(system, save_file);

    while (system -> temperature < stop)
    {
        system -> temperature += step;
        for (int epoch = 0; epoch < 1e3 * num_spins; epoch++)
        {
            metropolis_step_ising_2d(system);
        }
    }

    save_ising_2d(system, save_file);

    do
    {
        system -> temperature -= step;
        for (int epoch = 0; epoch < 1e3 * num_spins; epoch++)
        {
            metropolis_step_ising_2d(system);
        }
    } while (system -> temperature > (start + step));

    save_ising_2d(system, save_file);
}


/*
 * external_magnetic_field
 * -----------------------
 * Compare the absolute magnetisation and the energy to the trength of 
 * an external magnetic field. 
 *
 *
 *
 *
 */
