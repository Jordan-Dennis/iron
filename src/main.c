#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"../include/toml.h"
#include"../include/1d_ising.h"
#include"../include/2d_ising.h"


int main_ising_1d(char *args)
{
    Config *config = init_config("src/tests/2d_test_config.toml");

    if (strcmp(args, "first_and_last") == 0)
    {
        first_and_last_ising_2d(config);
    }
    else if (strcmp(args, "physical_parameters") == 0)
    {
        physical_parameters_ising_2d(config);
    }
    else if (strcmp(args, "magnetisation_vs_temperature") == 0)
    {
       histogram(config);
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


int main_ising_2d(char *args)
{
    Config *config = init_config("src/tests/2d_test_config.toml");

    if (strcmp(args, "first_and_last") == 0)
    {
        first_and_last_ising_2d(config);
    }
    else if (strcmp(args, "physical_parameters") == 0)
    {
        physical_parameters_ising_2d(config);
    }
    else if (strcmp(args, "magnetisation_vs_temperature") == 0)
    {
        magnetisation_vs_temperature(config);
    }
    else if (strcmp(args, "heating_and_cooling") == 0)
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

    return 0;
}


int main(int num_args, char **args)
{
    if (!(num_args == 3))
    {
        printf("Error: Please specify the program you want to load!");
        exit(1);
    }


    if (strcmp(args[1], "1d") == 0)
    {
        main_ising_1d(args[2]);
    }
    else if (strcmp(args[1], "2d") == 0)
    {
        main_ising_2d(args[2]);
    }
    else
    {
        printf("Error: Please specify either 1d or 2d from this switchboard!");
        exit(1);
    }
}
