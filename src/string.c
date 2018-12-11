#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
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

int string_to_integer(char *sequence, int *status)
{
    char *end = NULL;
    long number = 0;
    errno = 0;
    number = strtol(sequence, &end, 10);
    if(sequence == end)
        *status = 2;
    else if(errno == ERANGE && number == LONG_MIN)
        *status = 3;
    else if(errno == ERANGE && number == LONG_MAX)
        *status = 4;
    else if(errno == EINVAL) /* not in all c99 implementations - gcc OK */
        *status = 5;
    else if(errno != 0 && number == 0)
        *status = 6;
    else if(errno == 0 && sequence && !*end)
        *status = 0;
    else if(errno == 0 && sequence && *end != 0)
        *status = 1;
    if(number >= INT_MIN && number <= INT_MAX)
    {
        return (int) number;
    }
    else
    {
        *status = 7;
        return 0;
    }
}