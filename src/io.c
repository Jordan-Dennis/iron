#include<stdio.h>
#include<stdlib.h>
#include"include/errors.h"
#include"include/io.h"


/*
 * read
 * ----
 * Save the current state of the file to memory. 
 *
 * parameters
 * ----------
 * char* file_name: The file to read.
 *
 * returns
 * -------
 * char* file_contents: The contents of the file.  
 */
char* read(char* file_name)                                                        
{                                                          
    FILE* source = fopen(file_name, "r"); 
    validate_file(source, file_name); 

    fseek(source, 0, SEEK_END); 
    float numbytes = ftell(source); 
    fseek(source, 0, SEEK_SET); 
                                                                                   
    char* text = (char*) calloc(numbytes, sizeof(char));                           
    fread(text, sizeof(char), numbytes, source);                                   
    fclose(source); 
    return text;                                                                   
} 


