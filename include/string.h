#ifndef _STRING_H_
#define _STRING_H_

#include "../include/command.h"

char* string(char* array, unsigned int size);

char* duplicate_string(char* origin);

void split_string(command *c, char* string, int* size);

int string_to_integer(char *sequence, int *status);

#endif
