#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"include/utils.h"


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


/*
 * random_index
 * ------------
 * Generate a random index in the correct range.
 * 
 * parameters
 * ----------
 * int length: The length of the array that is getting indexed. 
 *
 * returns
 * -------
 * int rand_ind: A random index in the range [0, length]
 */
int random_index(int length)
{
    float norm_rand = normalised_random();
    float range_rand = norm_rand * length;
    return (int) range_rand;
}


/*
 * mean
 * ----
 * Calculate the mean of the values in an array.
 *
 * parameters
 * ----------
 * float* array: The sample of values.
 * int length: The number of values in the sample.
 * 
 * returns
 * -------
 * float* mean: The geometric mean of the sample. 
 */
float mean(float array[], int length) 
{
    float mean = 0;
    for (int entry = 0; entry < length; entry++)
    {
        mean += array[entry];
        // printf("%f, ", array[entry]);
    }
    // printf("\n");
    return mean / length;
}


/*
 * variance
 * --------
 * Calculates the variance of a sample. For optimisation purposes it is 
 * assumed that the mean has already been calculated and this is passed as 
 * an argument. 
 *
 * parameters
 * ----------
 * float* array: The sample.
 * float mean: The mean of the sample.
 * int length: The number of values in the sample.
 *
 * returns 
 * -------
 * float var: The variance of the sample. 
 */
float variance(float* array, float mean, int length)
{
    float variance = 0;
    for (int entry = 0; entry < length; entry++)
    {
        variance += pow((array[entry] - mean), 2);
    }
    return variance / (length - 1);
}


/*
 * modulo
 * ------
 * A number theory based version of the % operator
 *
 * parameters
 * ----------
 * int dividend: The n in n % m 
 * int divisor: The m in n % m
 *
 * returns
 * -------
 * int nmodm: The number theory modulo.
 */
int modulo(int dividend, int divisor)
{
    if (divisor == 0)
    {
        return 0;
    }
    return ((dividend % divisor) + divisor) % divisor;
}

