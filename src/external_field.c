#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"include/utils.h"


// TODO: So this is clearly not working because the line along 0 magnetic 
// field is not behaving as I would expect. 


typedef struct ising_t 
{
    float temperature;
    float magnetic_field;
    int length;
    int **ensemble;
} ising_t;


ising_t *init_ising_t(float temperature, float magnetic_field, int length)
{
    int **ensemble = (int**) calloc(length, sizeof(int*));

    for (int row = 0; row < length; row++)
        ensemble[row] = (int*) calloc(length, sizeof(int));

    for (int row = 0; row < length; row++)
        for (int col = 0; col < length; col++)
            ensemble[row][col] = random_spin();

    ising_t *system = (ising_t*) malloc(sizeof(ising_t));
    system -> magnetic_field = magnetic_field;
    system -> temperature = temperature;
    system -> ensemble = ensemble;
    system -> length = length;

    return system;
}


void free_ising_t(ising_t *system)
{
    int length = system -> length;

    for (int row = 0; row < length; row++)
        free(system -> ensemble[row]);

    free(system -> ensemble);
    free(system);
}


static inline float spin_energy_ising_t(ising_t *system, int row, int col)
{
    float magnetic_field = system -> magnetic_field;
    int **ensemble = system -> ensemble;
    int length = system -> length;
    float neighbours = 0.0;
    neighbours += ensemble[modulo(row + 1, length)][col];
    neighbours += ensemble[modulo(row - 1, length)][col];
    neighbours += ensemble[row][modulo(col + 1, length)];
    neighbours += ensemble[row][modulo(col - 1, length)];
    return (neighbours + magnetic_field) * ensemble[row][col]; 
}


float energy_ising_t(ising_t *system)
{
    int length = system -> length;
    int row, col;
    float energy = 0.;

    for (row = 0; row < length; row++)
        for (col = 0; col < length; col++)
            energy += (float) spin_energy_ising_t(system, row, col);

    return energy / 2.;
}


static inline int should_flip(float change, float temperature)
{
    return (change < 0) || (normalised_random() < exp(- change / temperature));
}


static inline void flip(ising_t *system, int row, int col)
{
    system -> ensemble[row][col] *= -1;
}


void metropolis_step_ising_t(ising_t *system)
{
    int length = system -> length;
    int row = random_index(length);
    int col = random_index(length);
    float change = 2 * spin_energy_ising_t(system, row, col);
    float temperature = system -> temperature;
    if (should_flip(change, temperature)) 
        flip(system, row, col); 
}


float magnetisation_ising_t(ising_t *system)
{
    int **ensemble = system -> ensemble;
    int length = system -> length;
    float magnetisation = 0.;

    for (int row = 0; row < length; row++)
        for (int col = 0; col < length; col++)
            magnetisation += (float) ensemble[row][col];

    return magnetisation;
}


float energy_ising_t(ising_t *system)
{
    int length = system -> length;
    float energy = 0;

    for (int row = 0; row < length; row++)
        for (int col = 0; col < length; col++)
            energy -= spin_energy_ising_2d(system, row, col);

    return energy / 2.;
}


float entropy_ising_t(ising_t *system)
{
    int len = system -> length;
    int **ensemble = system -> ensemble; 
    int up = 0;

    for (int row = 0; row < len; row++)
    {
        for (int col = 0; col < len; col++)
        {
            up += ensemble[row][col] == ensemble[modulo(row + 1, len)][col];
            up += ensemble[row][col] == ensemble[row][modulo(col + 1, len)];
        }
    }
        
    int total = 2 * len * len;
    int down = total - up;

    if (up == 0 || up == total)
    {
        return log(2);
    }
    else
    {
        return total * log(total) - up * log(up) - down * log(down);
    }
}


void print_ising_t(ising_t *system)
{
    int row, col, length = system -> length, **ensemble = system -> ensemble;

    printf("Temperature: %f\n", system -> temperature);
    printf("Magnetic Field: %f\n", system -> magnetic_field);

    for (row = 0; row < length; row++)
    {
        for (col = 0; col < length; col++)
        {
            printf("%i,", ensemble[row][col] > 0);
        }
        printf("\n");
    }
}


int main(void)
{
    // TODO: So I want to redo this entirely, I collect all of the physical 
    // parameters of the system at multiple different magnetisations and
    // plot these in a contour like way on the chart. Also need to add in 
    // The magnetic susceptibility to make this proper lit. 
    int size = 5;
    int length = 20;
    int epochs = length * 1e3;
    int temperature, magnetic_field;
    float _temperature, _magnetic_field;
    float magnetisations[size][size], energies[size][size];
    float _magnetisations[epochs], _energies[epochs];
    float magnetisation, energy, ratio;
    float critical_temperature = 2 / log(1 + sqrt(2));
    float maximum_temperature = 2. * critical_temperature;
    ising_t *system = init_ising_t(2. * critical_temperature, 0., length);

    for (int _ = 0; _ < length * 1e3; _++)
        metropolis_step_ising_t(system);

    for (temperature = 0; temperature < size; temperature++)
    {
        ratio = ((float) temperature / (float) size);
        _temperature = maximum_temperature - ratio * maximum_temperature;
        system -> temperature = _temperature;

        for (magnetic_field = 0; magnetic_field < size; magnetic_field++)
        {
            _magnetic_field = 2. * magnetic_field / size; 
            system -> magnetic_field = _magnetic_field;       

            for (int _ = 0; _ < epochs; _++) 
            {
                metropolis_step_ising_t(system);
                _magnetisations[_] = magnetisation_ising_t(system);
                _energies[_] = energy_ising_t(system);
            }

            magnetisation = mean(_magnetisations, length);
            energy = mean(_energies, length);
            magnetisations[temperature][magnetic_field] = magnetisation; 
            energies[temperature][magnetic_field] = energy;
        }
    }

    free(system); 

    char *save_file_name = "pub/data/external_field.csv";
    FILE *save_file = fopen(save_file_name, "w");

    if (save_file == NULL) 
    {
        printf("Error: Could not open '%s', for writing!", save_file_name);
        exit(1);
    }

    fprintf(save_file, "tau, B, m, U\n");
    for (temperature = 0; temperature < size; temperature++)
    {
        ratio = ((float) temperature / (float) size);
        _temperature = maximum_temperature - ratio * maximum_temperature;
        for (magnetic_field = 0; magnetic_field < size; magnetic_field++)
        {
            _magnetic_field = 2. * magnetic_field / size;
            fprintf(save_file, "%f,", _temperature);
            fprintf(save_file, "%f,", _magnetic_field);
            fprintf(save_file, "%f,", magnetisations[temperature][magnetic_field]);
            fprintf(save_file, "%f\n", energies[temperature][magnetic_field]);
        }
    }

    fclose(save_file);
}
