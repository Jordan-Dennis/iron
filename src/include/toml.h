#ifndef TOML_QUARD
#define TOML_QUARD
#include"debug.h"
#include<stdio.h>
#include<stdbool.h>

typedef struct Toml 
{
    char* toml; 
    int cursor, length; 
    Debug* debug;
} Toml;

int done(Toml* toml);
char peek(Toml* toml);
char next(Toml* toml);
void whitespace(Toml* toml);
char* group(Toml* toml);
char* word(Toml* toml);
char* find(Toml* toml, char* header, char* field);
Toml* __toml__(char* file_name);

typedef struct Pair {char* key; char* value; } Pair;

Pair* entry(Toml* toml);
Pair* __pair__(char* key, char* value);

#endif 
