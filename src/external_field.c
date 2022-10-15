#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"include/utils.h"


// TODO: So this is clearly not working because the line along 0 magnetic 
// field is not behaving as I would expect. 


typedef struct ising_t 
{
    float temperature;
    float epsilon;
    float magnetic_field;
    int length;
    int **ensemble;
} ising_t;


ising_t *init_ising_t(
    float temperature, 
    float magnetic_field, 
    float epsilon, 
    int length)
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


void metropolis_step_ising_t(ising_t *system)
{
    int length = system -> length;
    int **ensemble = system -> ensemble;
    float epsilon = system -> epsilon;
    float temperature = system -> temperature;

    int row = random_index(length);
    int col = random_index(length);

    int spin = ensemble[row][col];
    int neighbours = 
        ensemble[modulo(row + 1, length)][col] + 
        ensemble[modulo(row - 1, length)][col] + 
        ensemble[row][modulo(col + 1, length)] + 
        ensemble[row][modulo(col - 1, length)];

    float magnetic_change = -2 * spin * magnetic_field;
    float interaction_change = -2 * neighbours * spin;
    float energy_change = magnetic_change + interaction_change;

    if ((energy_change < 0) || 
        (exp(- energy_change / temperature) > normalised_random()))
    {
        system -> ensemble[row][col] *= -1;
    }
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
    int **ensemble = system -> ensemble;
    float epislon = system -> epsilon;
    float magnetic_field = system -> magnetic_field;
    float neighbours = 0;

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            neighbours += ensemble[modulo(row + 1, length)][col];
            neighbours += ensemble[modulo(row - 1, length)][col];
            neighbours += ensemble[row][modulo(col + 1, length)];
            neighbours += ensemble[row][modulo(col - 1, length)];
        }
    }

    float interactions = neighbours * epsilon * ensemble[row][col];
    float magnetic = ensemble[row][col] * magnetic_field;
    return interactions / 2. + magnetic;
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
    printf("Epsilon: %f\n", system -> epsilon);

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
    const int runs = 5;
    const int length = 20;
    const int epochs = length * 1e3;
    const int num_temps = 10;
    const int num_fields = 3;
    const int num_epsilons = 3

    float energies[num_temps][num_fields][num_epsilons];
    float free_energies[num_temps][num_fields][num_epsilons];
    float magnetisations[num_temps][num_fields][num_epsilons];
    float heat_capacities[num_temps][num_fields][num_epsilons]; 


    // TODO: So I need to finish programming the loops but then I can run this 
    // and I should be done. Then I just need to type up the report. Sweet. 
    // I think that this will all work out. Just gotta check the due date one 
    // more time. 
    #pragma omp parallel for num_threads(3)
    for (float epsilon = -1.0; epsilon <= 1.0; epsilon++)
    {
        for (float magnetic_field = 0.0; magnetic_field < 3.0; magnetic_field++)
        {
            ising_t *system = init_ising_t(3., magnetic_field, epsilon, length);
    
            // Running the burn-in
            for (int epoch = 0; epoch < epochs; epoch++)
            {
                metropolis_step_ising_t(system);
            }

            for (float temperature = 3.0; temperature > 0.0; temperature -= 0.2)
            {
                system -> temperature = temperature;

                float _energies[runs];
                float _entropies[runs];
                float _free_energies[runs];
                float _magnetisations[runs];
                float _heat_capacities[runs];
                
                for (int run = 0; run < runs; run++)
                {
                   float __energies[epochs];
                   float __entropies[epochs];
                   float __magnetisations[epochs];

                    for (int epoch = 0; epoch < epochs; epoch++)
                    {
                        metropolis_step_ising_t(system);
                        __energies[epoch] = energy_ising_t(system);
                        __entropies[epoch] = entropy_ising_t(system);
                        __magnetisations[epoch] = magnetisation_ising_t(system);
                    }

                    _energies[run] = mean(__energies, epochs);
                    _entropies[run] = mean(__entropies, epochs);
                    _magnetisations[run] = mean(__magnetisations, epochs);
                    _heat_capacities[run] = 
                        variance(__energies, _energies[run], epochs) / 
                        temperature / temperature;
                }

                energies[_temperature][_field][_epsilon] = mean(_energies, runs);
                entropies[_temperature][_field][_epsilon] = mean(_entropies, runs);
                magnetisations[_temperature][_field][_epsilon] = mean(_magnetisations, runs);
                heat_capacities[_temperature][_field][_epsilon] = mean(_heat_capacities, runs);
            }
        }
    }
    

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
