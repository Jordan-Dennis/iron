#include<stdio.h>
#include<string.h>
#include<math.h>
#include"ising.h"


int main(int number_of_args, char *args[])
{
    if (number_of_args == 2)
    {
        printf("%s\n", args[1]);
        printf("%i\n", strcmp(args[1], "hello"));
        if (strcmp(args[1], "hello"))
        {
            printf("I made it");
        }
    }
    else 
    {
        printf("\033[31merror\033[0mPlease provide a question number");
    }

    for (float temperature=1.; temperature <= MAX; temperature++)
    {
        char file[10];
        sprintf(file, "1d_%i.txt", (int)temperature);
        char address[strlen(file) + strlen(dir)];
        strcpy(address, dir);
        strcat(address, file);
        FILE *data = fopen(address, "w");
        
        int spins[n]; 
        // Copy initial state so that it can be written to a file.
        for (int spin = 0; spin < n; spin++) 
        {
            int value = random_spin();
            spins[spin] = value;
            fprintf(data, "%i, %i, %i\n", 0, spin, value);
        }


        for (int epoch = 0; epoch <= REPS * n; epoch++)
        { 
            int spin = (int) (normalised_random() * n);
            int energy_change = -2 * spin_energy(spin, spins) - 4 * spins[spin];
           
            if (energy_change <= 0)
            {
                spins[spin] = -spins[spin];
            } 
            else
            {
                float probability = exp(-energy_change / temperature);
                if (probability <= normalised_random()) 
                {
                    spins[spin] = -spins[spin];
                }
            }
        }
           
           
        for (int spin = 0; spin < n; spin++)
        {
            fprintf(data, "%i, %i, %i\n", 1, spin, spins[spin]);
        }
           
        fclose(data);
    }
    return 0;
}


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
void question_1_a(int number_of_spins, int temperatures[])
{

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
void question_1_c(int number_o_spins, int temperatures[])
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
void question_1_e(int number_of_spins, int temperatures[])
{

}
