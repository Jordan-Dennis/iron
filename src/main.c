#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/question_1.h"
#include"include/ising.h"
#include"include/toml.h"
#include"include/question_2.h"


/*
 * parse_temperatures
 * ------------------
 * A helper function for parsing the arguments to the question 1 code. 
 *
 * parameters
 * ----------
 * int number_of_args: The number of arguments.
 * char args[]: The arguments.
 * float temperatures[]: The array to store the temperatures in.
 */
void parse_temperatures(int number_of_args, char *args[], float temperatures[])
{ 
    if (number_of_args > 3)
    {
        for (int arg = 3; arg < number_of_args; arg++)
        {
            temperatures[arg - 3] = atof(args[arg]);
        }
    }
}


/*
 * main
 * ----
 * Navigate the implementations for questions 1 - 3. Please provide the 
 * number of spins as the first argument followed by the temperatures that
 * you wish to run the simulation at. 
 *
 * For example:
 *
 * out/ising 100 1. 2. 3.
 *           --- -------
 *            |     |
 *        num_spins |
 *             temperatures
 */
int main(int num_args, char *args[])
{
    printf("Welcome!\n");
    printf("Requested Q: %s\n", args[1]);
    if (num_args >= 2)
    {
        if (strcmp(args[1], "1a") == 0)
        {
            printf("Entering Q1a)!\n");
            int number_of_spins = atoi(args[2]);
            float temperatures[num_args - 3];
            parse_temperatures(num_args, args, temperatures);
            question_1_a(number_of_spins, temperatures, num_args - 3, 1000);
        }
        else if (strcmp(args[1], "1c") == 0)
        {
            printf("Entering Q1c)!\n");
            int number_of_spins = atoi(args[2]);
            float temperatures[num_args - 3];
            parse_temperatures(num_args, args, temperatures);
            question_1_c(number_of_spins, temperatures, num_args - 3, 1000);
        }
        else if (strcmp(args[1], "1e") == 0)
        {
            printf("Entering Q1e)!\n");
            int number_of_spins = atoi(args[2]);
            float temperatures[num_args - 3];
            parse_temperatures(num_args, args, temperatures);
            question_1_e(number_of_spins, temperatures, num_args - 3, 1000);
        }
        else
        {
            printf("Error: Option not found!\n");
        }
    }
    else 
    {
        printf("Error: Please provide a question number!\n");
    }

    return 0;
}
