#ifndef _DB_H_
#define _DB_H_

typedef struct db 
{
    int id;
    FILE *file;
} DB;

char* get_database_path(char* name);

DB* load_database(char* name);

int create_database(char* name);

int drop_database(char* name);

#endif
