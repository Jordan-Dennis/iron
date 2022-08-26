#include<stdio.h>
#include<stdlib.h>
#include"include/io.h"
#include"include/debug.h"
#include"include/errors.h"


/*
 * __debug__
 * ---------
 * A utility for logging the events of the simulation. 
 * 
 * parameters
 * ----------
 * char* file_name: The name of the file to store the log in. 
 *
 * returns
 * -------
 * Debug* logger: The logger that will handle io.
 */
Debug* __debug__(char* file_name)
{
    Debug* logger = malloc(sizeof(Debug));
    logger -> file_name = file_name;
    wipe(logger);
    return logger;
}


/*
 * log
 * ---
 * Log a message with the debugger. 
 *
 * parameters
 * ----------
 * Debug* logger: The debugger to log the message with.
 * char* message: The message to log. 
 */
void debug(Debug* debug, char* message)
{
    char* contents = read(debug -> file_name); 
    
    FILE* file = fopen(debug -> file_name, "w");
    validate_file(file, debug -> file_name);
    fprintf(file, "%s", contents);
    fprintf(file, "%s\n", message);
    fclose(file);
}



/*
 * wipe
 * ----
 * Delete the contents of the log.
 * 
 * parameters
 * ----------
 * Debug* debug: The debugger to wipe. 
 */
void wipe(Debug* debug)
{
    FILE* file = fopen(debug -> file_name, "w");
    validate_file(file, debug -> file_name);
    fclose(file);
} 
