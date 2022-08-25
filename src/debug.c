#include<stdlib.h>
#include<stdio.h>
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
    char* contents = read(debug); 
    
    FILE* file = fopen(debug -> file_name, "w");
    validate_file(file, debug -> file_name);
    fprintf(file, "%s", contents);
    fprintf(file, "%s\n", message);
    fclose(file);
}


/*
 * read
 * ----
 * Save the current state of the file to memory. 
 *
 * parameters
 * ----------
 * Debug* debug: The debugger to retrieve the state from.
 *
 * returns
 * -------
 * char* file_contents: The contents of the file.  
 */
char* read(Debug* debug)                                                        
{                                                          
    FILE* source = fopen(debug -> file_name, "r"); 
    validate_file(source, debug -> file_name); 

    fseek(source, 0, SEEK_END); 
    float numbytes = ftell(source); 
    fseek(source, 0, SEEK_SET); 
                                                                                   
    char* text = (char*) calloc(numbytes, sizeof(char));                           
    fread(text, sizeof(char), numbytes, source);                                   
    fclose(source); 
    return text;                                                                   
}  
