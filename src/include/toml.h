#ifndef TOML_H
#define TOML_H
#include<stdio.h>
#include<stdbool.h>


/*
 * Pair
 * ----
 * A key value pair stored in the toml.
 *
 * parameters
 * ----------
 * char* key: The key of the entry.
 * char* value: The value of the entry.
 * char* group: The group over the key value pair.
 */
typedef struct Pair 
{ 
    char *key, *value;
} Pair;


/*
 * Config
 * ------
 * A saved configuration from which to launch the program. A configuration 
 * consists of groups of pairs with named values. 
 *
 * fields
 * ------
 * int length: The number of pairs in the config.
 * Pair** pairs: The pairs stored by the config.
 */
typedef struct Config
{
    int length;
    Pair **pairs;
} Config;


// The next question is if I should lex the damn thing or just continue in 
// the fashion that I started with. 


/*
 * Toml
 * ----
 * A simple .toml lexer that passes over the file.
 *
 * fields
 * ------
 * char* toml: The source code of the toml.
 * char* current_group: The group over the current cursor.
 * int length: The number of chars in the toml source.
 * int cursor: The current position of the lexer in the file.
 * Debug* debug: A debugger for error catching. 
 */
typedef struct Toml
{
    char *toml, *current_group; 
    int cursor, length; 
} Toml;



Toml* init_toml(char* file_name);
Pair* init_pair(char* key, char* value);
Config* init_config(char* file_name);

char peek(Toml* toml);
char next(Toml* toml);
char* word(Toml* toml);
void whitespace(Toml* toml);
void comment(Toml *toml);
void skip(Toml *toml);
Pair* entry(Toml* toml);
Config* parse(Toml* toml);

void add_pair_to_config(Config* config, Pair* pair);
char *find(Config* config, char* key);
#endif 
