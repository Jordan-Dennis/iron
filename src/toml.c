#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"include/io.h"
#include"include/toml.h"
#include"include/debug.h"
#include"include/errors.h"


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
    char* str = calloc(1, sizeof(char));
    while (isalpha(peek(toml)) || isdigit(peek(toml))) 
    {
        str = realloc(str, (strlen(str) + 2) * sizeof(char));
        strcat(str, (char[]){next(toml), 0});
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
    validate_char('[', peek(toml));

    char lsparen = next(toml);
    char* head = word(toml);
 
    validate_char(']', peek(toml));    

    char rparen = next(toml);
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
    return __pair__(key, value);
}


typedef struct Group
{
    char* group;
    int length;
    Pair** pairs;
} Group;


Group* __group__(char* header)
{
    Group* group = malloc(sizeof(Group));
    group -> group = header;
    group -> pairs = NULL;
    group -> length = 0;
    return group;
}


void add_pair_to_group(Group* group, Pair* pair)
{
    if (!(group -> pairs))
    {
        group -> pairs =  malloc(sizeof(Pair));
    }
    else
    {
        group -> pairs = realloc(group -> pairs, 
            (group -> length + 1) * sizeof(Pair));
    }
    group -> pairs[group -> length] = pair;
    group -> length++;    
}


typedef struct Config
{
    int length;
    Group** groups;
} Config;


void add_group_to_config(Config* config, Group* group)
{
    if (!(config -> groups))
    {
        config -> groups =  malloc(sizeof(Group));
    }
    else
    {
        config -> groups = realloc(config -> groups, 
            (config -> length + 1) * sizeof(Group));
    }
    config -> groups[config -> length] = group;
    config -> length++; 
}


Config* parse(Toml* toml)
{
    Config* config = malloc(sizeof(Config));
    Group* match;
    Pair* pair;
    debug(toml -> debug, "Entered parse!\n");
    while (!done(toml))
    {
        if (peek(toml) == '[')
        {
            match = __group__(group(toml));
            debug(toml -> debug, match -> group);
            add_group_to_config(config, match);
        }
        else if (isdigit(peek(toml)) || isalpha(peek(toml)))
        {
            pair = entry(toml);
            debug(toml -> debug, pair -> key);
            debug(toml -> debug, pair -> value);
            add_pair_to_group(match, pair);
        }
        else 
        {
            next(toml);
        }
    }
    return config;
}


char* find(Toml* toml, char* header, char* field)
{
    Config* config = parse(toml);
    debug(toml -> debug, "Finished Parsing");
    for (int group = 0; group < (config -> length); group++)
    {
        Group* outer = (config -> groups)[group];
        if (strcmp((outer -> group), header) == 0)
        {   
            for (int pair = 0; pair < (outer -> length); pair++)
            {
                Pair* inner = (outer -> pairs)[pair];
                if (strcmp((inner -> key), field) == 0)
                {
                    return inner -> value;
                }
            }
        }
    }
    exit(1);
}




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
    debug(toml -> debug, "Debugging Session Started\n");
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
Pair* __pair__(char* key, char* value)
{
    Pair* dict = calloc(1, sizeof(Pair));
    dict -> key = key;
    dict -> value = value;
    return dict;
}  
