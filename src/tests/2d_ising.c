#include<stdio.h>
#include<stdlib.h>
#include"../include/2d_ising.h"

int main(void)
{
    Config *config = init_config("src/tests/2d_test_config.toml");
//    first_and_last_ising_2d(config);
    magnetisation_vs_temperature(config);
}
