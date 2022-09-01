#include<stdlib.h>
#include<stdio.h>

typedef struct State
{
    float* temps;
    float* energies;
} State;


int main(void)
{
    State* state = malloc(sizeof(State));
    state -> temps = calloc(10, sizeof(float));
    state -> energies = calloc(10, sizeof(float));

    for (int i = 0; i < 10; i++)
    {
        (state -> temps)[i] = 1.;
        (state -> energies)[i] = 1.;
    }

    printf("%f", (state -> temps)[0]);
    printf("%f", (state -> energies)[0]);
}
