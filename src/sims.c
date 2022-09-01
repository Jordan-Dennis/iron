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
Temperatures* parse_temperatures(Config* config)
{ 
    // TODO: I decided to pass the config so that the high low and step 
    // automatically dropped out of scope.
	Temperatures* temperatures = malloc(sizeof Temperatures);
    
    // TODO: Finish the castings here and then complete this function.
    float low = atof(find(config, "temperatures", "low"));
    float high = atof(find(config, "temperatures", "high"));
    float step = atof(find(config, "temperatures", "step"));
    float length = (int) ((high - low) / step);
	
    temperatures -> length = length;
    temperatures -> temps = calloc(length, sizeof float);

	for (int at_temp = 1; at_temp <= length; at_temp++)
    {
        (temperatures -> temps)[temperature] = low + step * at_temp;
	}

    return temperatures;
}


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

    System* system = (System*) malloc(sizeof System);
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
    char* out = find(config, "out", "address");
    System* system = parse_system(config);
    Temperatures* temperatures = parse_temperatures(config);

    // Free config because I have finished with it. 
    free(config);

    // Storing the data.
    int results[2][system -> number][temperatures -> length];
    for (int temp = 0; temp <= (temp -> length); temp++)
    {
        // Updating the system temperature
        (system -> temperature) = (temperatures -> temps)[temp];
        
        // Saving the initial configuration of this temeprature in the 
        // memory.
        for (int spin = 0; spin < num_spins; spin++)
        {
            results[0][spin][temp] = spins[spin];
        }

        // Running the metropolis algorithm over the system. 
        for (int epoch = 0; epoch <= reps; epoch++)
        { 
            metropolis_step(system);
        }

        // Saving the final state of the system to memory   
        for (int spin = 0; spin < num_spins; spin++)
        {
            results[1][spin][temp] = spins[spin];
        }

        // Generating random spin system for next temperature
        random_system(system);
    }
    free(system);

    FILE* data = fopen(out, "w");
    validate_file(data, out);

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


typedef struct State 
{
    int length; 
    float free_energy[];
    float  
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
void physical_parameters(Config* config)
{
    char* out = find(config, "out", "address");
    System* system = parse_system(config);
    Temperatures* temperatures = parse_temperatures(config);

    // Arrays to store the collected data on the physical state. 
    float sim_heat_capacity[temperatures -> length];
    float sim_energy[temperatures -> length];
    float sim_free_energy[temperatures -> length];
    float sim_entropy[temperatures -> length];

    // Simulating the system. 
    for (int temperature = 0; 
        temperature <= (temperatures -> length); 
        temperature++)
    {
        // Setting the system temperature.
        (system -> temperature) = (temperatures -> temps)[temperature];

        // Running the burnin period. 
        for (int epoch = 0; epoch <= 1000; epoch++)
        { 
            metropolis_step(system);
        }

        // Collecting the data'
		// TODO: I think that I will have new arrays here to simply store
		// the information and then I will invoke a mean and variance function 
		// on these arrays. Damn, I keep adding linear complexity. 
        for (int epoch = 0; epoch <= reps; epoch++)
        { 
            metropolis_step(system);

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
        random_system(system); 
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
