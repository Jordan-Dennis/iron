#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../include/2d_ising.h"

int main(int num_args, char **args)
{
    if (!(num_args == 2))
    {
        printf("Error: Please specify the program you want to load!");
        exit(1);
    }

    Config *config = init_config("src/tests/2d_test_config.toml");

    if (strcmp(args[1], "first_and_last") == 0)
    {
        first_and_last_ising_2d(config);
    }
    else if (strcmp(args[1], "physical_parameters") == 0)
    {
        physical_parameters_ising_2d(config);
    }
    else if (strcmp(args[1], "magnetisation_vs_temperature") == 0)
    {
        magnetisation_vs_temperature(config);
    }
    else if (strcmp(args[1], "heating_and_cooling") == 0)
    {
        heating_and_cooling(config);
    }
    else
    {
        printf("Error: A valid option was not specified.\n");
        printf("The valid options are:\n");
        printf(" - first_and_last\n");
        printf(" - physical_parameters\n");
        printf(" - magnetisation_vs_temperature\n");
        printf(" - heating_and_cooling\n");
    }
}
