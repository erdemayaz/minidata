#ifndef _FILE_H_
#define _FILE_H_

FILE* open_file(char* path);

char* read_file(char * path);

void close_file(FILE* file);

FILE* create_file(char* name);

int exist_file(char* path);

#endif
