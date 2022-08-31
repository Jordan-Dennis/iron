#include<math.h>
#include<stdio.h>
#include<string.h>
#include"include/toml.h"
#include"include/sims.h"
#include"include/ising.h"
#include"include/errors.h"


typedef struct Temperatures
{
    int length;
    float* temps;
} Temperatures;


/*
 * parse_temperatures
 * ------------------
 * A helper function for parsing the arguments to the question 1 code. 
 *
 * parameters
 * ----------
 * float temperatures[]: The array to store the temperatures in.
 * float low: The lowest temperature to simulate.
 * float high: The highest temperature to simulate. 
 * float step: The increment for the temperature. 
 */
void parse_temperatures(Config* config)
{ 
    // TODO: I decided to pass the config so that the high low and step 
    // automatically dropped out of scope.
	Temperatures* temperatures = malloc(sizeof Temperatures);
    
    // TODO: Finish the castings here and then complete this function.
    float low = atof(find(config, "temperatures", "low"));
    float high = find(config, "temperatures", "high");
    float step = find(config, "temperatures", "step");
    float length = (int) ((high - low) / step);
	
    temperatures -> length = length;

    float* temps =  calloc(length, sizeof float);
    temps[0] = low;
    float temperature = low;
	int index = 1;
	while (temperature <= high)
	{
		temperature += step;
		temperatures[index] = temperature;
        index++;
	}
}
// TODO: Should I implement the temperatures as well as a struct? 

/*
 * first_and_last 
 * --------------
 * A one dimensional ising model with periodic boundaries using the 
 * metropolis algorithm. Provide the initial and the final outputs 
 * from the simulation for at least three different temperatures.
 * What do you notice about the size of the chunks of color at 
 * low temperatures compared to high temperatures. 
 */
void first_and_last(void)
{
    Config* config = __config__("config.toml");
    char* out = find(config, "out", "address");
    int num_spins = find(config, "spins", "number");
    int reps = find(config, "reps", "number");
    int dims = find(config, "dimensions", "number");
    
    float *temperatures = range(low, high, step);

    int results[2][num_spins][num_temps];

    for (int temperature = 0; temperature <= num_temps; temperature++)
    {
        int spins[num_spins];
        random_system(spins, num_spins); 

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
    }

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
void physical_parameters(void)
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
 * histogram
 * ---------
 * Create a histogram of the m values you obtain by running a 
 * simulation of 500 spins at 1., 2. and 3. temperatures 100 times.
 */
void histogram(void)
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
