#ifndef TOML_H
#define TOML_H
#include<stdio.h>
#include<stdbool.h>


/*
 * Debug
 * -----
 * A simple logging structure for debugging the toml communications.
 *
 * fields
 * ------
 * char* file_name: The name of the log file to use.
 */ 
typedef struct Debug
{
    char* file_name;
} Debug;


// TODO: For this to work there will need to be multiple different types of
// Pair so that calue can have multiple different pointer types. 
// I will also interpret the data as I go. That is to say that instead of 
// having a char it will get cast to an int or float. 
// Mwahahahaha I can have a void pointer that points to an arbitrary point
// in memory and I can use this to create my perfect Toml reading device. 

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
    char *key, *group;
    void *value; 
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
    Pair** pairs;
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
typedef struct TomlLexer
{
    char *toml, *current_group; 
    int cursor, length; 
} Toml;



Toml* __toml__(char* file_name);
Pair* __pair__(char* key, char* value, char* group);
Config* __config__(char* file_name);


// So I want to add functionality to do an arbitrary level of nesting for the 
// group. How can I do this? group -> global/group/subgroup


int done(Toml* toml);
char peek(Toml* toml);
char next(Toml* toml);
char* word(Toml* toml);
void whitespace(Toml* toml);
char* group(Toml* toml);
Pair* entry(Toml* toml);
Config* parse(Toml* toml);


void add_pair_to_config(Config* config, Pair* pair);
char* find(Config* config, char* header, char* field);
#endif 
