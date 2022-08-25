#include<stdio.h>
#include<stdlib.h>
#include"include/errors.h"

const char* RED = "\033[31;1m";
const char* WHITE = "\033[97m";


/*
 * validate_args
 * -------------
 * Checks if the user has provided  a configuration file from which to load
 * the parameters of the program.
 *
 * parameters
 * ----------
 * int num_args: Should come from the first argument of the main function.
 * 
 * exit
 * ----
 * 1: If no configuration file was provided.  
 */
void validate_args(int num_args)
{
	if (num_args < 2)
	{
		printf("%sError:%s Please parse a configuration file!", RED, WHITE);
		exit(1);
	}
}


/* 
 * validate_file
 * -------------
 * Makes sure that a file was correctly opened.
 * 
 * parameters
 * ----------
 * FILE* file: The stream pointer to the file. 
 * char* file_name: The name of the file. 
 * 
 * exit
 * ----
 * 1: If the file pointer is NULL.
 */
void validate_file(FILE* file, char* file_name)
{
	if (!file)
	{
		printf("%sError:%s Could not open '%s'!", RED, WHITE, file_name);
		exit(1);
	}
}


/*
 * validate_option
 * ---------------
 * To be called if no valid option was passed to the program.
 *
 * parameters
 * ----------
 * char* option: The option that was provided. 
 */
void validate_option(char* option)
{
	printf("%sError:%s '%s' is not a valid option!", RED, WHITE, option);
	exit(1);
}


/*
 * validate_char
 * -------------
 * Checks that the expected char was recieved and rasies an error if not. 
 *
 * parameters
 * ----------
 * char target: The expected char.
 * char recieved: The recieved char.
 */
void validate_char(char target, char recieved)
{
    if (target != recieved)
    {
        printf("Error: '%c' expected but got %c", target, recieved);
        exit(1);
    }
}
