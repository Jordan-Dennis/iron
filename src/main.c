#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/question_1.h"
#include"include/ising.h"
#include"include/errors.h"
#include"include/toml.h"
#include"include/question_2.h"


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
void parse_temperatures(float temperatures[], float low, float high, 
	float step)
{ 
	temperatures[0] = low;
	float temperature = low;
	int index = 1;
	while (temperature <= high)
	{
		temperature += step;
		temperatures[index] = temperature;
        index++;
	}
}


/*
 * spin_energy
 * -----------
 * A function pointer to allow the conditional assignment of the calculation 
 * of spin energy. 
 *
 * parameters
 * ----------
 * int spin: The spin to calculate the energy contribution of.
 * int spins[]: The current state of the system. 
 * int num_spins: The number of spins in the system.
 */
int *spin_energy(int spin, int spins[], int num_spins);


/*
 * main
 * ----
 * Navigate the implementations for questions 1 - 3. Please provide the 
 * number of spins as the first argument followed by the temperatures that
 * you wish to run the simulation at. 
 */
int main(int num_args, char *args[])
{
	// Check a config was provided. 
	validate_args(num_args);

	// Check that the file was safely opened. 
	Config* task = __config__(args[1]);
	
    // Parsing the question index from the file. 
	char* question = find(task, "task", "task");

    if (strcmp(question, "ising") == 0)
    {
        float temperature = atof(find(task, "temperatures", "temperature"));
        int num_spins = atoi(find(task, "spins", "number"));
        int reps = atoi(find(task, "reps", "number"));
        ising(num_spins, reps, temperature);
        return 0;
    }

	// Reading the temperature array from the file.
	float low_temp = atof(find(task, "temperatures", "low"));
	float high_temp = atof(find(task, "temperatures", "high"));
	float step = atof(find(task, "temperatures", "step"));
	int num_temps = (int) ((high_temp - low_temp) / step);
	float temperatures[num_temps];
	parse_temperatures(temperatures, low_temp, high_temp, step);
	int number_of_spins = atoi(find(task, "spins", "number"));
	int reps = atoi(find(task, "reps", "number"));
    int dimension = atoi(find(task, "dimensions", "number"));

    switch (dimension)
    {
        case 1:
        {
            spin_energy = &1d_spin_energy;
            break;
        }
        case 2:
        {
            spin_energy = &2d_spin_energy;
            break;
        }
        default:
        {
            printf("Error: Dimensions must be 2 or 1!");
            exit(1);
        }
    }


	if (strcmp(question, "1a") == 0)
	{
		printf("Entering Q1a)!\n");
		question_1_a(number_of_spins, temperatures, num_temps, reps);
	}
	else if (strcmp(question, "1c") == 0)
	{
		printf("Entering Q1c)!\n");
		question_1_c(number_of_spins, temperatures, num_temps, reps);
	}
	else if (strcmp(question, "1e") == 0)
	{
		printf("Entering Q1e)!\n");
		question_1_e(number_of_spins, temperatures, num_temps, reps);
	}
	else
	{
		validate_option(question);
	}

    return 0;
}
