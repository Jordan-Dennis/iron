#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include"include/toml.h"
#include"include/errors.h"


/*
  *read
  *----
  *Save the current state of the file to memory. 
 *
  *parameters
  *----------
  *char *file_name: The file to read.
 *
  *returns
  *-------
  *char *file_contents: The contents of the file.  
 */
char *read(char *file_name)                                                        
{                                                          
    FILE *source = fopen(file_name, "r"); 
    if (source == NULL)
    {
        printf("Error: Failed to open '%s'", file_name);
        exit(1);
    }

    fseek(source, 0, SEEK_END); 
    float numbytes = ftell(source); 
    fseek(source, 0, SEEK_SET); 
                                                                                   
    char *text = (char*) calloc(numbytes, sizeof(char));                           
    fread(text, sizeof(char), numbytes, source);                                   
    fclose(source); 
    return text;                                                                   
}


/*
  *init_toml
  *---------
  *Read a toml file into memory.
 *
  *parameters
  *----------
  *char *file_name: The name of the toml.
 *
  *returns
  *-------
  *Toml *toml: The parsed toml file. 
 */
Toml *init_toml(char *file_name)
{
    char *contents = read(file_name);
    Toml *toml = malloc(sizeof(Toml));
    toml -> toml = contents;
    toml -> cursor = 0;
    toml -> length = strlen(contents);
    return toml;
}


/*
  *init_pair
  *---------
  *Create a key value pair. 
 *
  *parameters
  *---------- 
  *char *key: The key = ....
  *char *value: The ... = value.
 *
  *returns
  *-------
  *Pair *pair: The key value pair. 
 */
Pair *init_pair(char *key, char *value)
{
    Pair *dict = calloc(1, sizeof(Pair));
    dict -> key = key;
    dict -> value = value;
    return dict;
} 




/*
  *done
  *----
  *Check for the end of file.
  *
  *parameters
  *----------
  *Toml *toml: The Toml containing the scanned file.
 *
  *returns
  *------- 
  *int done: True if the end of the file has been reached else false.
 */ 
int done(Toml *toml)
{
    return (toml -> cursor == toml -> length);
}


/*
  *peek
  *----
  *View the next char in the stream from the toml.
 *
  *parameters
  *----------
  *Toml *toml: The toml stream to peak. 
 *
  *returns
  *-------
  *char next: The next character in the stream. 
 */
char peek(Toml *toml)
{
    return toml -> toml[toml -> cursor];
}


/*
  *next
  *----
  *Get the next character in the stream.
 *
  *parameters
  *----------
  *Toml *toml: The toml stream from which to get the new character.
 *
  *returns
  *-------
  *char next: The next file in the stream. 
 */
char next(Toml *toml)
{
    if (done(toml))
    {
        printf("Error: Unexpected EOF!");
        exit(1);
    }
    char next = toml -> toml[toml -> cursor];
    toml -> cursor++;
    return next;
}


/*
  *word
  *-----
  *Parse a word from the toml file until an exit is reached. 
  *ignoring whitespace.
 *
  *parameters
  *----------
  *Toml *toml: The toml stream to parse from.
  *char exit: The exit point of the stream.
 *
  *returns
  *-------
  *char *word: The word that was parsed. 
 */
char *word(Toml *toml)
{
    char *str = calloc(1, sizeof(char));
//    while (isalpha(peek(toml)) 
//        || isdigit(peek(toml)) 
//        || (peek(toml) == '/')
//        || (peek(toml) == '.')
//        || (peek(toml) == '_'))
    while (!(isspace(peek(toml))))
    {
        str = realloc(str, (strlen(str) + 2)  *sizeof(char));
        strcat(str, (char[]){next(toml), 0});
    }
    return str;
}


/*
  *whitespace
  *----------
  *Skip whitespace. 
 *
  *parameters
  *----------
  *Toml *toml: The toml file that is parsing. 
 */
void whitespace(Toml *toml)
{
    while (isspace(peek(toml))) { next(toml); }
}


/*
  *skip
  *----
  *Skip a character in a configuration file.
 *
  *parameters
  *----------
  *Toml *toml: The file in which to skip a character.
 */
void skip(Toml *toml)
{
    toml -> cursor++;
}


/*
  *comment
  *-------
  *Parse a comment in a configuration file. 
 *
  *parameters
  *----------
  *Toml *toml: The file in which the comment is found. 
 */
void comment(Toml *toml)
{
    if (peek(toml) != '#')
    {
        printf("Error: Expected '#' but recieved %c", peek(toml));
        exit(1);
    }
    while (peek(toml) != '\n')
    {
        skip(toml);
    }
}


/*
  *entry
  *-----
  *Read a '=' separated entry in the toml file into memory.
 *
  *parameters
  *----------
  *Toml *toml: The toml file that is parsing. 
 *
  *returns
  *-------
  *Pair *dict: The '=' separated entry.
 */
Pair *entry(Toml *toml)
{
    char *key = word(toml);
    whitespace(toml);

    if (peek(toml) != '=')
    {
        printf("Error: Expected '=' but recieved '%c'", peek(toml));
        exit(1);
    }

    next(toml);
    whitespace(toml);
    char *value = word(toml); 
    return init_pair(key, value);
}


/*
  *add_pair_to_config
  *------------------
  *Add a pair to the configuration file.
 *
  *parameters
  *----------
  *Config *config: The configuration.
  *Pair *pair: The pair to add. 
 */
void add_pair_to_config(Config *config, Pair *pair)
{
    if (!(config -> pairs))
    {
        config -> pairs =  malloc(sizeof(Pair));
    }
    else
    {
        config -> pairs = realloc(config -> pairs, 
            (config -> length + 1)  *sizeof(Pair));
    }
    config -> pairs[config -> length] = pair;
    config -> length++; 
}


/*
  *parse
  *-----
  *Parse the toml file into a string of pair entries.
 *
  *parameters
  *----------
  *Toml *toml: The toml file to parse.
 *
  *returns
  *-------
  *Config *params: The parameters of the program. 
 */
Config *parse(Toml *toml)
{
    Config *config = malloc(sizeof(Config));
    config -> pairs = malloc(sizeof(Pair));
    config -> length = 0;

    while (!done(toml))
    {
        if (peek(toml) == '#')
        {
            comment(toml);
        }
        else if (isspace(peek(toml)))
        {
            whitespace(toml);
        }
        else if (isdigit(peek(toml)) || isalpha(peek(toml)))
        {
            add_pair_to_config(config, entry(toml));
        }
        else
        {
            printf("Error: Unexpected character %c", peek(toml));
            exit(1);
        }
    }
    return config;
}


/*
  *find
  *----
  *Search the parsed toml file for an entry.
 *
  *parameters
  *----------
  *Config *config: The parsed toml file.
  *char *header: The target group.
  *char *field: The target field.
 *
  *returns
  *-------
  *char *out: The value at the specified location.
 */
char *find(Config *config, char *key)
{
    for (int pair = 0; pair < (config -> length); pair++)
    {
        Pair *inner = (config -> pairs)[pair];
        if (strcmp((inner -> key), key) == 0) 
        {
            return inner -> value;
        }
    }
    printf("Error: Could not find toml entry: %s!", key);
    exit(1);
}


/*
  *init_config 
  *-----------
  *Constructor for the configurations.
 *
  *parameters
  *----------
  *char *file_name: The name of the toml file to read the configuration from.
 *
  *returns
  *-------
  *Config *config: The program configuration.
 */
Config *init_config(char *file_name)
{
    Toml *toml = init_toml(file_name);
    Config *config = parse(toml);
    return config;
}


