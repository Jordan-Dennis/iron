#include<stdio.h>
#include<stdlib.h>
#include"../include/1d_ising.h"

int main(void)
{
    int length = 25;
    float temperature = 1.;
    Ising1D *system = init_ising_1d(length, temperature);

    printf("Energy: %f\n", energy_ising_1d(system));
    print_ising_1d(system);
    for (int epoch = 0; epoch < 1000 * length; epoch++)
    {
        metropolis_step_ising_1d(system);
    }
    printf("Energy: %f\n", energy_ising_1d(system));
    print_ising_1d(system);
}
