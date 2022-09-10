#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/toml.h"
#include"include/sims.h"
#include"include/ising.h"
#include"include/errors.h"


/*
 * main
 * ----
 * Navigate the implementations for questions 1 - 3. Please provide the 
 * number of spins as the first argument followed by the temperatures that
 * you wish to run the simulation at. 
 */
int main(int num_args, char *args[])
{
	validate_args(num_args);// Check a config was provided. 
	Config* config = __config__(args[1]);
	char* question = find(config, "task", "index");

    if (strcmp(question, "ising") == 0)
    {
        // ising(config);
    }
	if (strcmp(question, "first_and_last") == 0)
	{
		first_and_last(config);
	}
	else if (strcmp(question, "physical_parameters") == 0)
	{
		physical_parameters(config);
	}
	else if (strcmp(question, "histogram") == 0)
	{
		histogram(config);
	}
	else
	{
		validate_option(question);
	}

    return 0;
}
