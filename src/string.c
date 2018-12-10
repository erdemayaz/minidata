#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/command.h"

char* string(char* array, unsigned int size)
{
    char *s = (char*) malloc(sizeof(char) * (size + 1));
    strcpy(s, array);
    s[size] = '\0';
    return s;
}

char* duplicate_string(char* origin)
{
    char *dup = (char*) malloc(sizeof(char) * (strlen(origin) + 1));
    strcpy(dup, origin);
    return dup;
}

void split_string(command *c, char* string, int* size)
{
    char *token = NULL;
    int i = 0;
    token = strtok(string, " ");
    while(token != NULL)
    {
        if(i >= WORD_SIZE)
        {
            *size = i + 1;
            return;
        }
        strcpy(c->words[i], token);
        token = strtok(NULL, " ");
        ++i;
    }
    *size = i;
}
