#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/ising.h"
#include"include/statistics.h"


/*
 * random
 * ------
 * Generate a random number over the range [0, 1].
 *
 * returns
 * -------
 * float: random number.
 */
float normalised_random(void)
{ 
    return (float) rand() / (float) RAND_MAX;
}


/* 
 * random_sign
 * -----------
 * Generate + or - 1 randomly.
 *
 * returns
 * -------
 * int: +1 or -1 randomly.
 */ 
int random_spin(void) 
{
    float normed_random = normalised_random();
    float translated_random = normed_random - .5;
    return (int) ((translated_random > 0) - (translated_random < 0));
}

typedef struct Ising2D {
    int length;
    int **ensemble;
} Ising2D; 


Ising2D* init_2d_ising_system(int length)
{
    int **ensemble = (int**) calloc(length, sizeof(int*));  
    for (int col = 0; col < length; col++)
    {
        ensemble[col] = (int*) calloc(length, sizeof(int));
    } 

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            ensemble[row][col] = random_spin();
        }
    }

    Ising2D* system = (Ising2D*) calloc(1, sizeof(Ising2D));
    system -> length = length;
    system -> ensemble = ensemble;
}


int main(void)
{
    int length = 10;
    Ising2D* system = init_2d_ising_system(length);

    for (int row = 0; row < length; row++)
    {
        for (int col = 0; col < length; col++)
        {
            printf("%i,", system -> ensemble[row][col]);
        }
        printf("\n");
    }
}
