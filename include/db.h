#ifndef _DB_H_
#define _DB_H_

typedef struct record
{
    char *content;
    int size;
} RECORD;

typedef struct entity
{
    RECORD *records;
    int size;
} ENTITY;

typedef struct db 
{
    int id;
    FILE *file;
    ENTITY *entities;
    int size;
} DB;

typedef enum context_t
{
    CTX_TERMINAL,
    CTX_DATABASE,
    CTX_ENTITY,
    CTX_RECORD
} context_t;

typedef union context_o
{
    DB *db;
    ENTITY *ent;
    RECORD *rec;
} context_o;

typedef struct context
{
    context_t type;
    context_o object;
} CTX;

CTX* init_ctx();

void destroy_ctx(CTX* c);

char* get_database_path(char* name);

DB* load_database(char* name);

int create_database(char* name);

int drop_database(char* name);

#endif
