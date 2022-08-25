#include<stdio.h>
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
 * bool done: True if the end of the file has been reached else false.
 */ 
bool done(Toml* toml)
{
    return peek(toml) == EOF;
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
    toml -> cursor += 1;
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
char* word(Toml* toml, char exit)
{
    char* word = calloc(1, sizeof(char));
    while (isalpha(next(toml))) {
    {
        word = realloc(word, (strlen(word) + 2) * sizeof(char));
        strcat(word, (char[]){next(toml), 0});
    }  
    return word;
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


KeyValue* entry(Toml* toml)
{
    char* key = until(toml, '=');
    char* value = until(toml, '\n');   
    return __key_value__(key, value);
}


char* find(Toml* toml, char* header, char* field)
{
    while (!done(toml))
    {
        if (peek(toml) == '[')
        {
            char* head = group(toml);
            if (strcmp(head, header) == 0)
            {
                while (peek(toml) != '[')
                {
                    KeyValue* dict = entry(toml);
                    if (strcmp(dict -> key, field) == 0)
                    {
                        return dict -> value;
                    }
                } 
            } 
        }
        else
        {
            next(toml);
        }
    }
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
    char* toml = read(file_name);
    Toml* toml = malloc(sizeof(Toml));
    toml -> toml = toml;
    toml -> cursor = 0;
    toml -> debug = __debug__("log.txt");
    return toml;
}


KeyValue* __key_value__(char* key, char* value)
{
    if (!((key) && (value)))
    {
        printf("Error: Cannot assign null pointers!");
        exit(1);
    }
    KeyValue* dict = calloc(1, sizeof(KeyValue));
    dict -> key = key;
    dict -> value = value;
    return dict;
}  
