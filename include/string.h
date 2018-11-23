#ifndef _STRING_H_
#define _STRING_H_

char* string(char* array, unsigned int size);

char* duplicate_string(char* origin);

char** split_string(char* string, const char delimiter, int alloc_size, int* word_size);

#endif
