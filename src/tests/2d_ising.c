#include<stdio.h>
#include<stdlib.h>
#include"../include/2d_ising.h"

int main(void)
{
    Config *config = init_config("src/tests/2d_test_config.toml");
    physical_parameters_ising_2d(config);
}
