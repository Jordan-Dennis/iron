#include<math.h>
#include<stdio.h>
#include<string.h>
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
    system -> epsilon = epsilon;
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
    float magnetic_field = system -> magnetic_field;

    int row = random_index(length);
    int col = random_index(length);

    int spin = ensemble[row][col];
    int neighbours = 
        ensemble[modulo(row + 1, length)][col] + 
        ensemble[modulo(row - 1, length)][col] + 
        ensemble[row][modulo(col + 1, length)] + 
        ensemble[row][modulo(col - 1, length)];

    float magnetic_change = -2 * spin * magnetic_field;
    float interaction_change = 2 * epsilon * neighbours * spin;
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
    float epsilon = system -> epsilon;
    float magnetic_field = system -> magnetic_field;
    float magnetic = 0.0;
    float neighbours = 0.0;
    float interactions = 0.0;

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            neighbours += ensemble[modulo(row + 1, length)][col];
            neighbours += ensemble[modulo(row - 1, length)][col];
            neighbours += ensemble[row][modulo(col + 1, length)];
            neighbours += ensemble[row][modulo(col - 1, length)];

            magnetic += ensemble[row][col] * magnetic_field;
            interactions -= neighbours * epsilon * ensemble[row][col];
        }
    }

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
    int length = system -> length;
    int **ensemble = system -> ensemble;

    printf("Epsilon: %f\n", system -> epsilon);
    printf("Temperature: %f\n", system -> temperature);
    printf("Magnetic Field: %f\n", system -> magnetic_field);

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            printf("%i,", ensemble[row][col] > 0);
        }
        printf("\n");
    }

    printf("Energy: %f\n", energy_ising_t(system));
    printf("Entropy: %f\n", entropy_ising_t(system));
    printf("Magnetisation: %f\n", magnetisation_ising_t(system));
}


void save_ising_t(FILE *save_file, ising_t *system)
{
    int length = system -> length;
    int **ensemble = system -> ensemble;

    fprintf(save_file, "Epsilon: %f\n", system -> epsilon);
    fprintf(save_file, "Temperature: %f\n", system -> temperature);
    fprintf(save_file, "Magnetic Field: %f\n", system -> magnetic_field);

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            fprintf(save_file, "%i,", ensemble[row][col]);
        }
        fprintf(save_file, "\n");
    }
}


void snapshots(void)
{
    int length = 20;
    int epochs = length * length * 1e3;
    FILE *save_file = fopen("pub/data/external_field.txt", "w");

    for (int _epsilon = 0; _epsilon < 3; _epsilon++)
    {
        for (int _field = 0; _field < 3; _field++)
        {
            for (int _temperature = 0; _temperature < 3; _temperature++)
            {
                float epsilon = (float) _epsilon - 1.0;
                float magnetic_field = (float) _field;
                float temperature = (float) _temperature + 1.0;

                ising_t *system = init_ising_t(temperature, magnetic_field, epsilon, length);

                for (int epoch = 0; epoch < epochs; epoch++)
                {
                    metropolis_step_ising_t(system);
                }

                save_ising_t(save_file, system);
                free_ising_t(system);
            }
        }
    }

    fclose(save_file);
}


int physical_parameters(void)
{
    // TODO: So I want to redo this entirely, I collect all of the physical 
    // parameters of the system at multiple different magnetisations and
    // plot these in a contour like way on the chart. Also need to add in 
    // The magnetic susceptibility to make this proper lit. 
    const int runs = 5;
    const int length = 20;
    const int num = length * length;
    const int epochs = length * 1e3;
    const int num_temps = 10;
    const int num_fields = 3;
    const int num_epsilons = 3;

    float energies[num_temps][num_fields][num_epsilons][2];
    float entropies[num_temps][num_fields][num_epsilons][2];
    float free_energies[num_temps][num_fields][num_epsilons][2];
    float magnetisations[num_temps][num_fields][num_epsilons][2];
    float heat_capacities[num_temps][num_fields][num_epsilons][2];


    for (int _epsilon = 0; _epsilon < 3; _epsilon++)
    {
        float epsilon = (float) _epsilon - 1.0;
        for (int _field = 0; _field < 3; _field++)
        {
            float magnetic_field = (float) _field;
            ising_t *system = init_ising_t(3., magnetic_field, epsilon, length);
    
            // Running the burn-in
            for (int epoch = 0; epoch < epochs; epoch++)
            {
                metropolis_step_ising_t(system);
            }

            for (int _temperature = 0; _temperature < num_temps; _temperature++)
            {
                float temperature = 3.0 - (3.0 / (float) num_temps) * (float)  _temperature;
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

                float energy = mean(_energies, runs);
                float entropy = mean(_entropies, runs);
                float free_energy = energy - temperature * entropy;
                float heat_capacity = mean(_heat_capacities, runs); 
                float magnetisation = mean(_magnetisations, runs);

                energies[_temperature][_field][_epsilon][0] = energy / num;
                entropies[_temperature][_field][_epsilon][0] = entropy / num;
                free_energies[_temperature][_field][_epsilon][0] = free_energy / num;
                magnetisations[_temperature][_field][_epsilon][0] = magnetisation / num;
                heat_capacities[_temperature][_field][_epsilon][0] = heat_capacity / num;

                float energy_err = sqrt(variance(_energies, energy, runs) / runs);
                float entropy_err = sqrt(variance(_entropies, entropy, runs) / runs);
                float free_energy_err = sqrt(energy_err + temperature * entropy_err / runs);
                float heat_capacity_err = variance(_heat_capacities, heat_capacity, runs); 
                float magnetisation_err = sqrt(variance(_magnetisations, magnetisation, runs) / runs);

                energies[_temperature][_field][_epsilon][1] = energy_err / num;
                entropies[_temperature][_field][_epsilon][1] = entropy_err / num;
                free_energies[_temperature][_field][_epsilon][1] = free_energy_err / num;
                magnetisations[_temperature][_field][_epsilon][1] = magnetisation_err / num;
                heat_capacities[_temperature][_field][_epsilon][1] = heat_capacity_err / num;


                if (epsilon == 1.0)
                {
                    printf("Energy: %f\n", energy);
                }
            }

            free_ising_t(system);
        }
    } 

    char *save_file_name = "pub/data/external_field.csv";
    FILE *save_file = fopen(save_file_name, "w");

    if (save_file == NULL) 
    {
        printf("Error: Could not open '%s', for writing!", save_file_name);
        exit(1);
    }

    fprintf(save_file, "epsilon, magnetic_field, tau, ");
    fprintf(save_file, "energy, energy_err, entropy, ");
    fprintf(save_file, "entropy_err, free_energy, free_energy_err, ");
    fprintf(save_file, "magnetisation, magnetisation_err, ");
    fprintf(save_file, "heat_capacity, heat_capacity_err\n");

    for (int _epsilon = 0; _epsilon < num_epsilons; _epsilon++)
    {
        for (int _field = 0; _field < num_fields; _field++)
        {
            for (int _temperature = 0; _temperature < num_temps; _temperature++)
            {
                float epsilon = (float) _epsilon - 1.0;
                float magnetic_field = (float) _field;
                float temperature = 3.0 - (3.0 / num_temps) * _temperature;

                // RODO: I need to fix these so that it is _temperature, _field, _epsilon 
                fprintf(save_file, "%f, %f, %f, ", epsilon, magnetic_field, temperature);
                fprintf(save_file, "%f, ", energies[_temperature][_field][_epsilon][0]);
                fprintf(save_file, "%f, ", energies[_temperature][_field][_epsilon][1]);
                fprintf(save_file, "%f, ", entropies[_temperature][_field][_epsilon][0]);
                fprintf(save_file, "%f, ", entropies[_temperature][_field][_epsilon][1]);
                fprintf(save_file, "%f, ", free_energies[_temperature][_field][_epsilon][0]);
                fprintf(save_file, "%f, ", free_energies[_temperature][_field][_epsilon][1]);
                fprintf(save_file, "%f, ", magnetisations[_temperature][_field][_epsilon][0]);
                fprintf(save_file, "%f, ", magnetisations[_temperature][_field][_epsilon][1]);
                fprintf(save_file, "%f, ", heat_capacities[_temperature][_field][_epsilon][0]);
                fprintf(save_file, "%f\n", heat_capacities[_temperature][_field][_epsilon][1]);
            }
        }
    }

    fclose(save_file);
}


int main(int num_args, char **args)
{
    if (num_args != 2)
    {
        printf("Error: Please provided the task name. You options are:\n");
        printf("    - snapshots\n");
        printf("    - physical_parameters\n");
        exit(1);
    }

    if (strcmp(args[1], "snapshots") == 0)
    {
        snapshots();
    }
    else if (strcmp(args[1], "physical_parameters") == 0)
    {
        physical_parameters();
    }

    return 0;
}
