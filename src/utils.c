#include<math.h>
#include<stdio.h>
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
