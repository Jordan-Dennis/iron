#include"include/statistics.h"

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
float mean(float* array, int length) 
{
    float mean;
    for (int entry = 0; entry < length; entry++)
    {
        mean += array[entry] / length;
    }
    return mean;
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
    float variance;
    for (int entry = 0; entry < length; entry++)
    {
        variance += (array[entry] - mean)*(array[entry] - mean) / (length - 1);
    }
    return variance;
}
