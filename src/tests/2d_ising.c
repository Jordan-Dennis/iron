#include<stdio.h>
#include<stdlib.h>
#include"../include/2d_ising.h"

int main(void)
{
    int length = 25;
    float temperature = 1.;
    Ising2D *system = init_ising_2d(length, temperature);

    printf("Energy: %f\n", energy_ising_2d(system));
    print_ising_2d(system);
    for (int epoch = 0; epoch < 1000 * length; epoch++)
    {
        metropolis_step_ising_2d(system);
    }
    printf("Energy: %f\n", energy_ising_2d(system));
    print_ising_2d(system);
}
