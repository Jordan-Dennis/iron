#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"include/toml.h"

bool done(Toml* toml)
{
    return peek(toml) == EOF;
}


char peek(Toml* toml)
{
    FILE* file = toml -> toml;
    int next = fgetc(file);
    return ungetc(next, file);
}


char next(Toml* toml)
{
    if (done(toml))
    {
        printf("Error: Unexpected EOF!");
        exit(1);
    }
    return fgetc(toml -> toml);
}


char* until(Toml* toml, char exit)
{
    char* word = calloc(1, sizeof(char));
    while (peek(toml) != exit)
    {
        if ((peek(toml) != ' ') && (peek(toml) != '\n'))
        { 
            word = realloc(word, (strlen(word) + 2) * sizeof(char));
            strcat(word, (char[]){next(toml), 0});
        }
        else
        {
            next(toml);
        }
    }  
    char _ = next(toml); 
    return word;
}


char* group(Toml* toml)
{
    if (peek(toml) != '[')
    {
        printf("Error: '[' expected but got %c", next(toml));
        exit(1);
    }
    char lsparen = next(toml);
    char* head = until(toml, ']'); 
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
            printf("Header: %s, Target: %s\n", head, header);
            if (strcmp(head, header) == 0)
            {
                while (peek(toml) != '[')
                {
                    KeyValue* dict = entry(toml);
                    printf("Field: %s, Target: %s", dict -> key, field);
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


Toml* __toml__(char* file_name)
{
    Toml* toml = calloc(1, sizeof(Toml));
    FILE* file = fopen(file_name, "r");
    if (!file)
    {
        printf("Error: File '%s' could not be opened!", file_name);
        exit(1);
    }
    toml -> toml = file;
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
