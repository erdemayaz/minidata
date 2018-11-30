#ifndef _FILE_H_
#define _FILE_H_

FILE* open_file(char* path);

FILE* open_source_file(char * path);

FILE* open_file_write(char* path);

char* read_file(char * path);

void close_file(FILE* file);

FILE* create_file(char* name, int* status);

int exist_file(char* path);

int exist_dir(char* dir);

#endif
