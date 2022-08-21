#ifndef TOML_QUARD
#define TOML_QUARD
#include<stdio.h>
#include<stdbool.h>

typedef struct Toml 
{
    FILE* toml;
} Toml;

bool done(Toml* toml);
char peek(Toml* toml);
char next(Toml* toml);
char* group(Toml* toml);
char* until(Toml* toml, char exit);
char* find(Toml* toml, char* header, char* field);
Toml* __toml__(char* file_name);

typedef struct KeyEntry
{
    char* key;
    char* value;
} KeyValue;

KeyValue* entry(Toml* toml);
KeyValue* __key_value__(char* key, char* value);

#endif 
