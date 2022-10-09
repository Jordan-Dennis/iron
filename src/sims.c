#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"include/toml.h"
#include"include/sims.h"
#include"include/ising.h"
#include"include/errors.h"
#include"include/statistics.h"





/*
 * parse_system
 * ------------
 * Convienience function for retrieving the system from the configuration file.
 *
 * parameters
 * ----------
 * Config* config: The parsed configuration file to scan for values.
 *
 * returns 
 * -------
 * System* system: The system with a random spin state and no allocated 
 *                 temperature.
 */
System* parse_system(Config* config)
{
    int spin_number = atoi(find(config, "spins", "number"));
    int repititions = atoi(find(config, "reps", "number"));
    int dimensions = atoi(find(config, "dimensions", "number")); 

    System* system = (System*) malloc(sizeof(System));
    system -> number = spin_number;
    system -> dimension = dimensions;
    random_system(system);

    return system;
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
void first_and_last(Config* config)
{
    // TODO: All of this parsing should get done by the toml interpretter 
    System* system = parse_system(config);
    char* out = find(config, "out", "address");
    int reps = atoi(find(config, "reps", "number"));
    Temperatures* temperatures = parse_temperatures(config);

    // Free config because I have finished with it. 
    free(config);

    // Storing the data.
    int results[2][system -> number][temperatures -> length];

    for (int temp = 0; temp <= (temperatures -> length); temp++)
    {
        // Updating the system temperature
        (system -> temperature) = (temperatures -> temps)[temp];
        
        // Saving the initial configuration of this temeprature in the 
        // memory.
        for (int spin = 0; spin < (system -> number); spin++)
        {
            results[0][spin][temp] = (system -> spins)[spin];
        }

        // Running the metropolis algorithm over the system. 
        for (int epoch = 0; epoch <= reps; epoch++)
        { 
            metropolis_step(system);
        }

        // Saving the final state of the system to memory   
        for (int spin = 0; spin < (system -> number); spin++)
        {
            results[1][spin][temp] = (system -> spins)[spin];
        }

        // Generating random spin system for next temperature
        random_system(system);
    }

    FILE* data = fopen(out, "w");
    validate_file(data, out);

    // Writing a header row to the file. 
    fprintf(data, "# S/F, spin");

    for (int temperature = 0; temperature < (temperatures -> length); temperature++)
    {
        fprintf(data, "T%i, ", temperature);
    }

    fprintf(data, "\n");

    // Writing the initial state data to the file.
    if (system -> dimension == 1)
    {
        for (int spin = 0; spin < (system -> number); spin++) // Columns
        {
            // Writing each row.
            fprintf(data, "%i, %i, ", 0, spin);
            for (int temperature = 0; 
                temperature < (temperatures -> length); 
                temperature++)
            {
                fprintf(data, "%i, ", results[0][spin][temperature]);
            }
            fprintf(data, "\n");
        }

        // Writing the final state data to the file. 
        for (int spin = 0; spin < (system -> number); spin++) // Columns
        {
            // Writing each row.
            fprintf(data, "%i, %i, ", 1, spin);
            for (int temperature = 0; 
                temperature < (temperatures -> length); 
                temperature++)
            {
                fprintf(data, "%i, ", results[1][spin][temperature]);
            }
            fprintf(data, "\n");
        }
    }
    else 
    {
        int number = sqrt(system -> number);
        for (int x = 0; x < number; x++)
        {
            for (int y = 0; y < number; y++)
            {
                fprintf(data, "%i, %i, ", x, y);

                for (int temperature = 0;
                    temperature < (temperatures -> length);
                    temperature++)
                {
                    fprintf(data, "%i, ", results[0][(x * number) + y][temperature]);
                }
                
                fprintf(data, "\n");
            }
        }

        for (int x = 0; x < number; x++)
        {
            for (int y = 0; y < number; y++)
            {
                fprintf(data, "%i, %i, ", x, y);

                for (int temperature = 0;
                    temperature < (temperatures -> length);
                    temperature++)
                {
                    fprintf(data, "%i, ", results[1][(x * number) + y][temperature]);
                }
                
                fprintf(data, "\n");
            }
        }

    }
    free(system);
}


// TODO: Make the temperature object just store the low high step info 
// and then use the more advanced for loop. 
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
 */
void physical_parameters(Config* config)
{
    char* out = find(config, "out", "address");
    int reps = atoi(find(config, "reps", "number"));
    System* system = parse_system(config);
    Temperatures* temperatures = parse_temperatures(config);

    // Arrays to store the collected data on the physical state. 
    float energies_and_error[temperatures -> length][2];
    float entropies_and_error[temperatures -> length][2];
    float free_energies_and_error[temperatures -> length][2];
    float heat_capacities_and_error[temperatures -> length];
    
    // Simulating the system. 
    for (int temperature = 0; 
        temperature <= (temperatures -> length); 
        temperature++)
    {
        // Setting the system temperature.
        float temp = (temperatures -> temps)[temperature];
        (system -> temperature) = temp;

        // Running the burnin period. 
        for (int epoch = 0; epoch <= 1000; epoch++)
        { 
            metropolis_step(system);
        }

        // Collecting the data'
		// TODO: I think that I will have new arrays here to simply store
		// the information and then I will invoke a mean and variance function 
		// on these arrays. Damn, I keep adding linear complexity. 
        float simulation_energies[reps];
        float simulation_entropies[reps];
        float simulation_free_energies[reps];
        
        for (int epoch = 0; epoch < reps; epoch++)
        { 
            metropolis_step(system);
            float epoch_energy = energy(system);
            float epoch_entropy = entropy(system);

            simulation_energies[epoch] = epoch_energy;
            simulation_entropies[epoch] = epoch_entropy;
            simulation_free_energies[epoch] = epoch_energy - temp * epoch_entropy;
        }

        float mean_energy = mean(simulation_energies, reps);
        float mean_entropy = mean(simulation_entropies, reps);
        float mean_free_energy = mean(simulation_free_energies, reps);

        float var_energy = variance(simulation_energies, mean_energy, reps);
        float var_entropy = variance(simulation_entropies, mean_entropy, reps);
        float var_free_energy = variance(simulation_free_energies, mean_free_energy, reps);
        float mean_heat_capacity = var_energy / temp / temp;

        int number = system -> number;

        energies_and_error[temperature][1] = sqrt(var_energy) / number;
        energies_and_error[temperature][0] = mean_energy / number;
        entropies_and_error[temperature][1] = sqrt(var_entropy) / number;
        entropies_and_error[temperature][0] = mean_entropy / number;
        free_energies_and_error[temperature][1] = sqrt(var_free_energy) / number;
        free_energies_and_error[temperature][0] = mean_free_energy / number;
        heat_capacities_and_error[temperature] = mean_heat_capacity / number;
      
        random_system(system); 
    }

	// Writing the data to the file
	FILE* data = fopen(out, "w");
    validate_file(data, out);

	// Writing the header row to the data. 
	fprintf(data, "# Temperature, ");
    fprintf(data, "Energy, Energy Error, "); 
    fprintf(data, "Entropy, Entropy Error, "); 
    fprintf(data, "Free Energy, Free Energy Error, "); 
    fprintf(data, "Heat Capacity\n");

    // TODO: from here
	for (int temp = 0; temp < (temperatures -> length); temp++)
	{
        fprintf(data, "%f, ", (temperatures -> temps)[temp]);
		fprintf(data, "%f, ", energies_and_error[temp][0]);
		fprintf(data, "%f, ", energies_and_error[temp][1]);
		fprintf(data, "%f, ", entropies_and_error[temp][0]);
		fprintf(data, "%f, ", entropies_and_error[temp][1]);
		fprintf(data, "%f, ", free_energies_and_error[temp][0]);
		fprintf(data, "%f, ", free_energies_and_error[temp][1]);
        fprintf(data, "%f\n ", heat_capacities_and_error[temp]);
	}
	
	fclose(data);
}


/*
 * histogram
 * ---------
 * Create a histogram of the m values you obtain by running a 
 * simulation of 500 spins at 1., 2. and 3. temperatures 100 times.
 */
void histogram(Config* config)
{
    System* system = parse_system(config);
    char* out = find(config, "out", "address");
    int reps = atoi(find(config, "reps", "number"));
    int second_number = atoi(find(config, "spins", "second_number"));
    Temperatures* temperatures = parse_temperatures(config);
    int reps_per_temp = atoi(find(config, "temperatures", "reps"));

    float magnetisations[temperatures -> length][reps_per_temp][2]; 

    for (int number = 0; number < 2; number++)
    {
        for (int temperature = 0; temperature < (temperatures -> length); temperature++)
        {
            (system -> temperature) = (temperatures -> temps)[temperature];
            for (int rep = 0; rep < reps_per_temp; rep++)
            {
                // Running the burnin period. 
                for (int epoch = 0; epoch <= 1000; epoch++)
                { 
                    metropolis_step(system);
                }

                // Running the simulation 
                float simulation_magnetisation[reps];

                for (int epoch = 0; epoch < reps; epoch++)
                { 
                    metropolis_step(system);
                    simulation_magnetisation[epoch] = magnetisation(system);
                }

                float mean_magnetisation = mean(simulation_magnetisation, reps);
                magnetisations[temperature][rep][number] = mean_magnetisation;

                // Randomising for the next iteration.
                random_system(system); 
            }
        }

        system -> number = second_number;
        random_system(system);
    }

    // Opening the data file. 
    FILE* data = fopen(out, "w");
    validate_file(data, out);
    
    // Printing the header row to the file. 
    fprintf(data, "# ");
    for (int temperature = 0; temperature < (temperatures -> length); temperature++)
    {
        fprintf(data, "T%f, ", (temperatures -> temps)[temperature]);
    }
    fprintf(data, "\n");

    // Writing the data to the file.
    // TODO: I really need my toml manager to be able to handle arrays.
    for (int rep = 0; rep < reps_per_temp; rep++)
    {
        for (int number = 0; number < 2; number++)
        {
            for (int temperature = 0; 
                temperature < (temperatures -> length); 
                temperature++)
            {
                fprintf(data, "%f, ", magnetisations[temperature][rep][number]);
            }
        }
        fprintf(data, "\n");
    }

    // Closing the file
    fclose(data);
}


/*
 * ising
 * -----
 * Generates a gif of the one dimensional ising model. 
 *
 * parameters
 * ----------
 * int num_spins: The number of spins in the simulation.
 * int reps: The number of repitions in the simulation gif.
 * float temperature: The temperature to run the simulation at.
 */
//void ising(int num_spins, int reps, float temperature)
//{
//    int spins[num_spins];
//    random_system(spins, num_spins); 
//
//    // Running the burnin period. 
//    for (int epoch = 0; epoch <= 1000; epoch++)
//    { 
//        metropolis_step(spins, temperature, num_spins);
//    }
//
//    // Running the simulation 
//    int evolution[num_spins][reps];
//    for (int epoch = 0; epoch <= reps; epoch++)
//    { 
//        metropolis_step(spins, temperature, num_spins);
//        for (int spin = 0; spin < num_spins; spin++)
//        {
//            evolution[spin][epoch] = spins[spin];
//        }
//    }
//    
//    FILE* data = fopen("pub/data/ising.csv", "w");
//
//    for (int spin = 0; spin < num_spins; spin++)
//    {
//        fprintf(data, "%i, %i, ", 0, spin);
//        for (int epoch = 0; epoch <= reps; epoch++)
//        { 
//            fprintf(data, "%i, ", evolution[spin][epoch]);
//        }
//        fprintf(data, "\n");
//    }
//
//    for (int spin = 0; spin < num_spins; spin++)
//    {
//        fprintf(data, "%i, %i, ", 1, spin);
//        for (int epoch = 0; epoch <= reps; epoch++)
//        { 
//            fprintf(data, "%i, ", evolution[spin][epoch]);
//        }
//        fprintf(data, "\n");
//    }
//
//    fclose(data);
//}
