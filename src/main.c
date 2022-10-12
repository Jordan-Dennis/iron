#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"include/toml.h"
#include"include/1d_ising.h"
#include"include/2d_ising.h"


int main_ising_1d(char **args)
{
    Config *config = init_config(args[1]);

    if (strcmp(args[0], "first_and_last") == 0)
    {
        first_and_last_ising_1d(config);
    }
    else if (strcmp(args[0], "physical_parameters") == 0)
    {
        physical_parameters_ising_1d(config);
    }
    else if (strcmp(args[0], "magnetisation") == 0)
    {
        magnetisation_vs_temperature_ising_1d(config);
    }
    else
    {
        printf("Error: A valid option was not specified.\n");
        printf("The valid options are:\n");
        printf(" - first_and_last\n");
        printf(" - physical_parameters\n");
        printf(" - histogram \n");
    }

    return 0;
}


int main_ising_2d(char **args)
{
    Config *config = init_config(args[1]);

    if (strcmp(args[0], "first_and_last") == 0)
    {
        first_and_last_ising_2d(config);
    }
    else if (strcmp(args[0], "physical_parameters") == 0)
    {
        physical_parameters_ising_2d(config);
    }
    else if (strcmp(args[0], "magnetisation") == 0)
    {
        magnetisation_vs_temperature_ising_2d(config);
    }
    else if (strcmp(args[0], "heating_and_cooling") == 0)
    {
        heating_and_cooling_ising_2d(config);
    }
    else
    {
        printf("Error: A valid option was not specified.\n");
        printf("The valid options are:\n");
        printf(" - first_and_last\n");
        printf(" - physical_parameters\n");
        printf(" - magnetisation\n");
        printf(" - heating_and_cooling\n");
    }

    return 0;
}


int main(int num_args, char **args)
{
    if (!(num_args == 4))
    {
        printf("Error: Please specify the program you want to load!");
        exit(1);
    }

    char **new_args = (char**) calloc(2, sizeof(char*));
    new_args[0] = args[2];
    new_args[1] = args[3];

    if (strcmp(args[1], "1d") == 0)
    {
        main_ising_1d(new_args);
    }
    else if (strcmp(args[1], "2d") == 0)
    {
        main_ising_2d(new_args);
    }
    else
    {
        printf("Error: Please specify either 1d or 2d from this switchboard!");
        exit(1);
    }

    free(new_args);
}
