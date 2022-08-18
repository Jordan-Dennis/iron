#include<stdio.h>
#include<string.h>
#include"question_1.h"
#include"ising.h"


/*
 * parse_args_q1
 * -------------
 * A helper function for parsing the arguments to the question 1 code. 
 *
 * parameters
 * ----------
 * int number_of_args: The number of arguments.
 * char args[]: The arguments.
 * float temperatures[]: The array to store the temperatures in.
 * int number_of_spins: A pointed to the address for the number of spins.
 * 
 * returns
 * -------
 * void: Modifications to the state are internal.
 */
void parse_args_q1()
{

}


/*
 * main
 * ----
 * Navigate the implementations for questions 1 - 3.
 *
 */
int main(int number_of_args, char *args[])
{
    if (number_of_args >= 2)
    {
        if (strcmp(args[1], "1a"))
        {
            if (number_of_args > 3)
            {
                int number_of_spins = args[2];
                float temperatures[number_of_args - 3];
                for (int arg = 3; arg < number_of_args; arg++)
                {
                    temperatures[arg - 3] = args[arg];
                }
            }
            question_1_a(number_of_spins, temperatures);
        }
        else if (strcmp(args[1], "1c")
        {
            int number_of_spins = args[2];
            float temperature = args[3];
            question_1_c(number_of_spins, temperatures);
        }
        else if (strcmp(args[1], "1e")
        {
            int number_of_spins = args[2];
            float temperature = args[3];
            question_1_e(number_of_spins, temperatures);
        }
    }
    else 
    {
        printf("\033[31merror\033[0mPlease provide a question number");
    }

    return 0;
}
