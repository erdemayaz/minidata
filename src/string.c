#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* duplicate_string(char* origin)
{
    char *dup = (char*) malloc(sizeof(char) * strlen(origin));
    strcpy(dup, origin);
    return dup;
}

char** split_string(char* string, const char delimiter, int alloc_size, int* size)
{
    char **words = (char**) malloc(sizeof(char*) * alloc_size);
    char *temp = duplicate_string(string);
    char *token = NULL;
    int i = 0;
    token = strtok(temp, &delimiter);
    while(token != NULL)
    {
        words[i] = duplicate_string(token);
        token = strtok(NULL, &delimiter);
        ++i;
    }
    *size = i;
    free(temp);
    return words;
}
