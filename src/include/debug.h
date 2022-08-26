#ifndef DEBUG_H
#define DEBUG_H

typedef struct Debug
{
    char* file_name;
} Debug;


Debug* __debug__(char* file_name);
void debug(Debug* logger, char* message);
void wipe(Debug* debug);

#endif
