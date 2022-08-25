#include<stdio.h>
#include<string.h>
#include<math.h>
#include"include/ising.h"
#include"include/question_1.h"
#include"include/toml.h"


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
void question_1_a(int num_spins, float temperatures[], int num_temps, int reps)
{
    printf("Invoking toml!\n");
    char* out = find(__toml__("config.toml"), "readables", "1a");
    printf("File: %s\n", out);
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
        fprintf(data, "T%i,", temperature);
    }
    fprintf(data, "\n");

    // Writing the initial state data to the file.
    for (int spin = 0; spin < num_spins; spin++) // Columns
    {
        // Writing each row.
        fprintf(data, "%i, %i", 0, spin);
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
        fprintf(data, "%i, %i", 1, spin);
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
void question_1_c(int num_spins, float temperatures[], int num_temps, int reps)
{

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
void question_1_e(int num_spins, float temperatures[], int num_temps, int reps)
{

}
