#ifndef _DB_H_
#define _DB_H_

typedef struct db 
{
    int id;
    FILE *file;
} DB;

DB* load_database(char* name);

int create_database(char* name);

#endif
