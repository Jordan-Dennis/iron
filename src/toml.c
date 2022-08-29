#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"include/toml.h"
#include"include/errors.h"


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
// TODO: It was an interesting experiment but I think that it is time to 
// remove this feature, because it is just clutter at this point. 
///*
// * __debug__
// * ---------
// * A utility for logging the events of the simulation. 
// * 
// * parameters
// * ----------
// * char* file_name: The name of the file to store the log in. 
// *
// * returns
// * -------
// * Debug* logger: The logger that will handle io.
// */
//Debug* __debug__(char* file_name)
//{
//    Debug* logger = malloc(sizeof(Debug));
//    logger -> file_name = file_name;
//    wipe(logger);
//    return logger;
//}
//
//
///*
// * log
// * ---
// * Log a message with the debugger. 
// *
// * parameters
// * ----------
// * Debug* logger: The debugger to log the message with.
// * char* message: The message to log. 
// */
//void debug(Debug* debug, char* message)
//{
//    char* contents = read(debug -> file_name); 
//    
//    FILE* file = fopen(debug -> file_name, "w");
//    validate_file(file, debug -> file_name);
//    fprintf(file, "%s", contents);
//    fprintf(file, "%s\n", message);
//    fclose(file);
//}
//
//
//
///*
// * wipe
// * ----
// * Delete the contents of the log.
// * 
// * parameters
// * ----------
// * Debug* debug: The debugger to wipe. 
// */
//void wipe(Debug* debug)
//{
//    FILE* file = fopen(debug -> file_name, "w");
//    validate_file(file, debug -> file_name);
//    fclose(file);
//} 


/*
 * __toml__
 * --------
 * Read a toml file into memory.
 *
 * parameters
 * ----------
 * char* file_name: The name of the toml.
 *
 * returns
 * -------
 * Toml* toml: The parsed toml file. 
 */
Toml* __toml__(char* file_name)
{
    char* contents = read(file_name);
    Toml* toml = malloc(sizeof(Toml));
    toml -> toml = contents;
    toml -> cursor = 0;
    toml -> length = strlen(contents);
    toml -> debug = __debug__("log.txt");
    // debug(toml -> debug, "Debugging Session Started\n");
    return toml;
}


/*
 * __pair__
 * --------
 * Create a key value pair. 
 *
 * parameters
 * ---------- 
 * char* key: The key = ....
 * char* value: The ... = value.
 *
 * returns
 * -------
 * Pair* pair: The key value pair. 
 */
Pair* __pair__(char* key, char* value, char* group)
{
    Pair* dict = calloc(1, sizeof(Pair));
    dict -> key = key;
    dict -> value = value;
    dict -> group = group;
    return dict;
} 




/*
 * done
 * ----
 * Check for the end of file.
 * 
 * parameters
 * ----------
 * Toml* toml: The Toml containing the scanned file.
 *
 * returns
 * ------- 
 * int done: True if the end of the file has been reached else false.
 */ 
int done(Toml* toml)
{
    return (toml -> cursor == toml -> length);
}


/*
 * peek
 * ----
 * View the next char in the stream from the toml.
 *
 * parameters
 * ----------
 * Toml* toml: The toml stream to peak. 
 *
 * returns
 * -------
 * char next: The next character in the stream. 
 */
char peek(Toml* toml)
{
    return toml -> toml[toml -> cursor];
}


/*
 * next
 * ----
 * Get the next character in the stream.
 *
 * parameters
 * ----------
 * Toml* toml: The toml stream from which to get the new character.
 *
 * returns
 * -------
 * char next: The next file in the stream. 
 */
char next(Toml* toml)
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
 * word
 * -----
 * Parse a word from the toml file until an exit is reached. 
 * ignoring whitespace.
 *
 * parameters
 * ----------
 * Toml* toml: The toml stream to parse from.
 * char exit: The exit point of the stream.
 *
 * returns
 * -------
 * char* word: The word that was parsed. 
 */
char* word(Toml* toml)
{
    // debug(toml -> debug, "Entered word");
    char* str = calloc(1, sizeof(char));
    while (isalpha(peek(toml)) 
        || isdigit(peek(toml)) 
        || (peek(toml) == '/')
        || (peek(toml) == '.'))
    {
        str = realloc(str, (strlen(str) + 2) * sizeof(char));
        strcat(str, (char[]){next(toml), 0});
        // debug(toml -> debug, str);
    }
    return str;
}


/*
 * whitespace
 * ----------
 * Skip whitespace. 
 *
 * parameters
 * ----------
 * Toml* toml: The toml file that is parsing. 
 */
void whitespace(Toml* toml)
{
    while (isspace(peek(toml))) { next(toml); }
}


/*
 * group
 * -----
 * Parse a header from the toml. This will have the format [group_name]
 * and should not contain whitespace. 
 *
 * parameters
 * ----------
 * Toml* toml: The toml to parse. 
 * 
 * returns
 * -------
 * char* group: The group name.
 */
char* group(Toml* toml)
{
    // debug(toml -> debug, "Entered group");
    validate_char('[', peek(toml));

    char lsparen = next(toml);
    char* head = word(toml);
 
    validate_char(']', peek(toml));    

    char rparen = next(toml);
    toml -> current_group = head;
    return head;
}


/*
 * entry
 * -----
 * Read a '=' separated entry in the toml file into memory.
 *
 * parameters
 * ----------
 * Toml* toml: The toml file that is parsing. 
 *
 * returns
 * -------
 * Pair* dict: The '=' separated entry.
 */
Pair* entry(Toml* toml)
{
    // debug(toml -> debug, "Entered entry!\n");
    char* key = word(toml);
    whitespace(toml);
    validate_char('=', peek(toml));
    next(toml);
    whitespace(toml);
    char* value = word(toml); 
    return __pair__(key, value, toml -> current_group);
}


/*
 * add_pair_to_config
 * ------------------
 * Add a pair to the configuration file.
 *
 * parameters
 * ----------
 * Config* config: The configuration.
 * Pair* pair: The pair to add. 
 */
void add_pair_to_config(Config* config, Pair* pair)
{
    if (!(config -> pairs))
    {
        config -> pairs =  malloc(sizeof(Pair));
    }
    else
    {
        config -> pairs = realloc(config -> pairs, 
            (config -> length + 1) * sizeof(Pair));
    }
    config -> pairs[config -> length] = pair;
    config -> length++; 
}


/*
 * parse
 * -----
 * Parse the toml file into a string of pair entries.
 *
 * parameters
 * ----------
 * Toml* toml: The toml file to parse.
 *
 * returns
 * -------
 * Config* params: The parameters of the program. 
 */
Config* parse(Toml* toml)
{
    Config* config = malloc(sizeof(Config));
    config -> pairs = malloc(sizeof(Pair));
    config -> length = 0;

    Pair* pair;
    // debug(toml -> debug, "Entered parse!\n");
    while (!done(toml))
    {
        if (peek(toml) == '[')
        {
            toml -> current_group = group(toml);
            // debug(toml -> debug, toml -> current_group);
        }
        else if (isdigit(peek(toml)) || isalpha(peek(toml)))
        {
            pair = entry(toml);
            // debug(toml -> debug, pair -> key);
            // debug(toml -> debug, pair -> value);
            add_pair_to_config(config, pair);
        }
        else 
        {
            next(toml);
        }
    }
    return config;
}


/*
 * find
 * ----
 * Search the parsed toml file for an entry.
 *
 * parameters
 * ----------
 * Config* config: The parsed toml file.
 * char* header: The target group.
 * char* field: The target field.
 *
 * returns
 * -------
 * char* out: The value at the specified location.
 */
char* find(Config* config, char* header, char* field)
{
    for (int pair = 0; pair < (config -> length); pair++)
    {
        Pair* inner = (config -> pairs)[pair];
        if ((strcmp((inner -> key), field) == 0) 
            && (strcmp((inner -> group), header) == 0))
        {
            return inner -> value;
        }
    }
    printf("Error: Could not find toml entry!");
    exit(1);
}


/*
 * __config__
 * ----------
 * Constructor for the configurations.
 *
 * parameters
 * ----------
 * char* file_name: The name of the toml file to read the configuration from.
 *
 * returns
 * -------
 * Config* config: The program configuration.
 */
Config* __config__(char* file_name)
{
    Toml* toml = __toml__(file_name);
    Config* config = parse(toml);
    return config;
}


