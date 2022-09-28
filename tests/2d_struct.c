#include<stdio.h>
#include<stdlib.h>

typedef struct IntMatrix 
{
    int length;
    int **ensemble;
} IntMatrix;


IntMatrix* init_int_matrix(int length)
{
    int **matrix = (int**) calloc(length, sizeof(int*));  
    for (int col = 0; col < length; col++)
    {
        matrix[col] = (int*) calloc(length, sizeof(int));
    } 

    IntMatrix* spins = (IntMatrix*) calloc(1, sizeof(IntMatrix));
    spins -> length = length;
    spins -> ensemble = matrix;  
}


int main(void) 
{
    int length = 2;
    IntMatrix* matrix = init_int_matrix(length);
    printf("Struct initialised!\n");
    matrix -> ensemble[0][0] = 0;
    matrix -> ensemble[0][1] = 1;
    matrix -> ensemble[1][0] = 2;
    matrix -> ensemble[1][1] = 3;
    printf("Values assigned\n");
    printf("Multi-dimensional Indexing\n");
    printf("%i,%i: %i\n", 0, 0, matrix -> ensemble[0][0]);
    printf("%i,%i: %i\n", 0, 1, matrix -> ensemble[0][1]);
    printf("%i,%i: %i\n", 1, 0, matrix -> ensemble[1][0]);
    printf("%i,%i: %i\n", 1, 1, matrix -> ensemble[1][1]);
}
