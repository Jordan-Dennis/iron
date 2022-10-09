#include<stdio.h>
#include<stdlib.h>
#include"../include/toml.h"

int main(int num_args, char **args)
{
    if (num_args != 2)
    {
        printf("Error: Expected a single argument");
        exit(1);
    }

    char *file_name = args[1];
    Config *config = init_config(file_name);
    
    for (int key = 0; key < config -> length; key++)
    {
        printf("%s: %s\n", 
            config -> pairs[key] -> key, 
            config -> pairs[key] -> value);
    }
}
