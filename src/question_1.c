#include<math.h>
#include<stdio.h>
#include<string.h>
#include"include/toml.h"
#include"include/ising.h"
#include"include/errors.h"
#include"include/question_1.h"


/*
 * question_1_a
 * ------------
 * A one dimensional ising model with periodic boundaries using the 
 * metropolis algorithm. Provide the initial and the final outputs 
 * from the simulation for at least three different temperatures.
 * What do you notice about the size of the chunks of color at 
 * low temperatures compared to high temperatures. 
 *
 * parameters
 * ----------
 * int number_of_spins: The number of spins to simulate.
 * int[] temperatures: The temperatures to run the simualtion at.
 *
 * notes
 * -----
 * Writes the outputs of the first and final states to external files 
 * for use with GNUPlot. 
 */
void first_and_last(int num_spins, float temperatures[], 
    int num_temps, int reps)
{
    char* out = find(__config__("config/config.toml"), "readables", "1a");
    int results[2][num_spins][num_temps];

    for (int temperature = 0; temperature <= num_temps; temperature++)
    {
        printf("Temperature: %f\n", temperatures[temperature]);
        int spins[num_spins];
        random_system(spins, num_spins); 
        // system_to_file(spins, out, num_spins);
        // Saving the initial configuration of this temeprature in the 
        // memory.
        for (int spin = 0; spin < num_spins; spin++)
        {
            results[0][spin][temperature] = spins[spin];
        }

        // Running the metropolis algorithm over the system. 
        for (int epoch = 0; epoch <= reps; epoch++)
        { 
            metropolis_step(spins, temperatures[temperature], num_spins);
        }

        // Saving the final state of the system to memory   
        for (int spin = 0; spin < num_spins; spin++)
        {
            results[1][spin][temperature] = spins[spin];
        }
        // system_to_file(spins, out, num_spins);
    }

    //TODO: Look into migrating this upward into the existing loops to 
    // simplify the logic of the code. 
    // Opening the file. 
    FILE* data = fopen(out, "w");
    if (!data)
    {
        printf("Error: Could not open file!\n");
        exit(1);
    }

    // Writing a header row to the file. 
    fprintf(data, "# S/F, spin");
    for (int temperature = 0; temperature < num_temps; temperature++)
    {
        fprintf(data, "T%i, ", temperature);
    }
    fprintf(data, "\n");

    // Writing the initial state data to the file.
    for (int spin = 0; spin < num_spins; spin++) // Columns
    {
        // Writing each row.
        fprintf(data, "%i, %i, ", 0, spin);
        for (int temperature = 0; temperature < num_temps; temperature++)
        {
            fprintf(data, "%i, ", results[0][spin][temperature]);
        }
        fprintf(data, "\n");
    }

    // Writing the final state data to the file. 
    for (int spin = 0; spin < num_spins; spin++) // Columns
    {
        // Writing each row.
        fprintf(data, "%i, %i, ", 1, spin);
        for (int temperature = 0; temperature < num_temps; temperature++)
        {
            fprintf(data, "%i, ", results[1][spin][temperature]);
        }
        fprintf(data, "\n");
    }

}


/*
 * question_1_c
 * ------------
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
 * int number_of_spins: The number of spins to simulate.
 * int temepratures[]: The temperatures to simulate. 
 * 
 * notes
 * -----
 * Writes the output to a file for GNUPlot to plot. 
 */
void physical_parameters(int num_spins, float temperatures[], 
    int num_temps, int reps)
{
    char* out = find(__config__("config/config.toml"), "readables", "1c");

    // Arrays to store the collected data on the physical state. 
    float sim_heat_capacity[num_temps];
    float sim_energy[num_temps];
    float sim_free_energy[num_temps];
    float sim_entropy[num_temps];

    // Simulating the system. 
    for (int temperature = 0; temperature <= num_temps; temperature++)
    {
        int spins[num_spins];
        random_system(spins, num_spins); 

        // Running the burnin period. 
        for (int epoch = 0; epoch <= 1000; epoch++)
        { 
            metropolis_step(spins, temperatures[temperature], num_spins);
        }

        // Collecting the data'
		// TODO: I think that I will have new arrays here to simply store
		// the information and then I will invoke a mean and variance function 
		// on these arrays. Damn, I keep adding linear complexity. 
        for (int epoch = 0; epoch <= reps; epoch++)
        { 
            metropolis_step(spins, temperatures[temperature], num_spins);

            sim_heat_capacity[temperature] = 
				heat_capacity(spins, temperature, num_spins);

            sim_energy[temperature] = energy(spins, num_spins);

            sim_free_energy[temperature] = 
				free_energy(spins, temperature, num_spins);

            sim_entropy[temperature] = entropy(spins, num_spins);
        }
        
        // Averaging over the simulation. 
        sim_heat_capacity[temperature] = sim_heat_capacity[temperature] / reps;
        sim_free_energy[temperature] = sim_free_energy[temperature] / reps;
        sim_energy[temperature] = sim_energy[temperature] / reps;
        sim_entropy[temperature] = sim_entropy[temperature] / reps;

		// TODO: Add the variance calculation. 
    }

	// Writing the data to the file
	FILE* data = fopen(out, "w");
	if (!data)
	{
		printf("Error: Could not open file!\n");
		exit(1);
	}

	// Writing the header row to the data. 
	fprintf(data, "# Temperature, Heat Capacity, Free Energy," 
		"Entropy, Energy\n");

	for (int temperature = 0; temperature < num_temps; temperature++)
	{
		fprintf(data, "%f, %f, %f, %f, %f\n", 
			temperatures[temperature], sim_heat_capacity[temperature],
			sim_free_energy[temperature], sim_energy[temperature],
			sim_entropy[temperature]);
	}
	
	// Closing the file
	fclose(data);
}


/*
 * question_1_e
 * ------------
 * Create a histogram of the m values you obtain by running a 
 * simulation of 500 spins at 1., 2. and 3. temperatures 100 times.
 *
 * parameters
 * ----------
 * int number_of_spins: The number of spins in the simulation.
 * int temperatures: The temperatures to run the simulation at. 
 *
 * notes
 * -----
 * Writes the output to a file for gnuplot. 
 */
void histogram(int num_spins, float temperatures[], int num_temps, int reps)
{
    int reps_per_temp = 100;
    int magnetisations[num_temps][reps_per_temp]; 
    for (int temperature = 0; temperature <= num_temps; temperature++)
    {
        for (int rep = 0; rep < reps_per_temp; rep++)
        {
            int spins[num_spins];
            random_system(spins, num_spins); 

            // Running the burnin period. 
            for (int epoch = 0; epoch <= 1000; epoch++)
            { 
                metropolis_step(spins, temperatures[temperature], num_spins);
            }

            // Running the simulation 
            float rep_magnetisation = 0;
            for (int epoch = 0; epoch <= reps; epoch++)
            { 
                metropolis_step(spins, temperatures[temperature], num_spins);
                rep_magnetisation += (float) magnetisation(spins, num_spins) / reps;
            }
            magnetisations[temperature][rep] = rep_magnetisation;
        }
    }

    // Opening the data file. 
    char* out = find(__config__("config/config.toml"), "readables", "1e");
    FILE* data = fopen(out, "w");
    validate_file(data, out);
    
    // Printing the header row to the file. 
    fprintf(data, "# ");
    for (int temperature = 0; temperature < num_temps; temperature++)
    {
        fprintf(data, "T%f, ", temperatures[temperature]);
    }
    fprintf(data, "\n");

    // Writing the data to the file.
    for (int rep = 0; rep < reps_per_temp; rep++)
    {
        for (int temperature = 0; temperature < num_temps; temperature++)
        {
            fprintf(data, "%i, ", magnetisations[temperature][rep]);
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
void ising(int num_spins, int reps, float temperature)
{
    int spins[num_spins];
    random_system(spins, num_spins); 

    // Running the burnin period. 
    for (int epoch = 0; epoch <= 1000; epoch++)
    { 
        metropolis_step(spins, temperature, num_spins);
    }

    // Running the simulation 
    int evolution[num_spins][reps];
    for (int epoch = 0; epoch <= reps; epoch++)
    { 
        metropolis_step(spins, temperature, num_spins);
        for (int spin = 0; spin < num_spins; spin++)
        {
            evolution[spin][epoch] = spins[spin];
        }
    }
    
    FILE* data = fopen("pub/data/ising.csv", "w");
    for (int spin = 0; spin < num_spins; spin++)
    {
        fprintf(data, "%i, %i, ", 0, spin);
        for (int epoch = 0; epoch <= reps; epoch++)
        { 
            fprintf(data, "%i, ", evolution[spin][epoch]);
        }
        fprintf(data, "\n");
    }
    for (int spin = 0; spin < num_spins; spin++)
    {
        fprintf(data, "%i, %i, ", 1, spin);
        for (int epoch = 0; epoch <= reps; epoch++)
        { 
            fprintf(data, "%i, ", evolution[spin][epoch]);
        }
        fprintf(data, "\n");
    }
    fclose(data);
}
