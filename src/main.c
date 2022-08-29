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
	Config* task = __config__(args[1]);
	char* question = find(task, "task", "index");

    if (strcmp(question, "ising") == 0)
    {
        ising();
    }
	if (strcmp(question, "first_and_last") == 0)
	{
		first_and_last();
	}
	else if (strcmp(question, "physical_parameters") == 0)
	{
		physical_parameters();
	}
	else if (strcmp(question, "histogram") == 0)
	{
		histogram();
	}
	else
	{
		validate_option(question);
	}

    return 0;
}
