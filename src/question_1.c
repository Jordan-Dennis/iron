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
    for (float temperature; temperature <= num_temps; temperature++)
    {
        int spins[num_spins];
        random_system(spins, num_spins); 
        system_to_file(spins, out, num_spins);

        for (int epoch = 0; epoch <= reps; epoch++)
        { 
            metropolis_step(spins, temperature, num_spins);
        }
        
        system_to_file(spins, out, num_spins);
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
