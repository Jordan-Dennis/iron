#ifndef ERRORS_H
#define ERRORS_H

void validate_args(int num_args);
void validate_file(FILE* file, char* file_name);
void validate_option(char* option);
void validate_char(char target, char recieved);

#endif
