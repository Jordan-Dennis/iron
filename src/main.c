#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/question_1.h"
#include"include/ising.h"
#include"include/debug.h"
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
        printf("%i -> %f\n", index, temperature);
		temperatures[index] = temperature;
        index++;
	}
}


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

	// Reading the temperature array from the file.
	float low_temp = atof(find(task, "temperatures", "low"));
	float high_temp = atof(find(task, "temperatures", "high"));
	float step = atof(find(task, "temperatures", "step"));

    printf("Low: %f\n", low_temp);
    printf("High: %f\n", high_temp);
    printf("Step: %f\n", step);

	float temperatures[(int) ((high_temp - low_temp) / step)];
	parse_temperatures(temperatures, low_temp, high_temp, step);
	int num_temps = (int) sizeof(temperatures) / sizeof(float);

	// Reading the number of spins from the file.
	int number_of_spins = atoi(find(task, "spins", "number"));

	// Reading the number of reps from the file
	int reps = atoi(find(task, "reps", "number"));

	// Parsing the question index from the file. 
	// TODO: Make it possible to parse a toml array of multiple indexes.
	char* question = find(task, "question", "index");
	
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
