#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"include/utils.h"


typedef struct ising_t 
{
    float temperature;
    float magnetic_field;
    int length;
    int **ensemble;
} ising_t;


// TODO: Do I want to initialise with the temperature and the magnetic field?
// Yes I think that I do otherwise we end up with a dLux type scenario where 
// the state is not guaranteed to be correct. I might also try setters and 
// getters for the struct. 
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


inline float spin_energy_ising_t(ising_t *system, int row, int col)
{
    float magnetic_filed = system -> magnetic_field;
    int **ensemble = system -> ensemble;
    int length = system -> length;
    float neighbours = 0.0;
    neighbours += ensemble[modulo(row + 1, length)][col];
    neighbours += ensemble[modulo(row - 1, length)][col];
    neighbours += ensemble[row][modulo(col + 1, length)];
    neighbours += ensemble[row][modulo(col - 1, length)];
    return (neighbours + magnetic_field) * ensemble[row][column]; 
}


// TODO: So I am re-writing a lot of code at the moment. The question 2 
// should be able to run on this but I think that I will leave it for the 
// moment. 
float energy_ising_t(ising_t *system)
{
    int length = system -> length;

    for (int row = 0; row < length; row++)
        for (int col = 0; col < length; col++)
            energy += spin_energy_ising_t(system, row, col);

    return energy / 2.;
}


inline int should_flip(float change, float temperature)
{
    return (change < 0) || (normalised_random() < exp(- change / temperature));
}


inline void flip(ising_t *system, int row, int col)
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
    if should_flip(change, temperature) flip(system, row, col); 
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


int main(void)
{
    int size = 10;
    int length = 50;
    int epochs = length * 1e3;
    float magnetisations[size][size];
    float energies[size][size];
    float _magnetisations[epochs];
    float _energies[epochs];
    float magnetisation;
    float energy;
    float critical_temperature = 2 / log(1 + sqrt(2));
    ising_t *system = init_ising_t(2. * critical_temperature, 0., length);

    for (int _ = 0; _ < length * 1e3; _++)
        metropolis_step_ising_t(system);

    for (float temperature = size/2.; temperature >= -size/2.; temperature--)
    {
        system -> temperature = (1. + temperature / 5.) * critical_temperature;
        for (float magnetic_field = 0; magnetic_field < size; magnetic_field++)
        {
            system -> magnetic_field = 2. * magnetic_field / size;            
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

    float _temperature;
    float _magnetic_field;

    for (float temperature = size/2.; temperature >= -size/2.; temperature--)
    {
        _temperature = (1. + temperature / 5.) * critical_temperature;
        for (float magnetic_field = 0; magnetic_field < size; magnetic_field++)
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
