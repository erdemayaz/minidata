#ifndef _DB_H_
#define _DB_H_

#include <stdint.h>

typedef struct record
{
    char *content;
    int size;
} RECORD;

typedef struct entity
{
    char *name;
    FILE *file;
    RECORD **records;
    int size;
    uint8_t commited;
} ENTITY;

typedef struct db 
{
    int id;
    char *name;
    ENTITY **entities;
    int size;
    unsigned int list_size;
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

char* get_database_dir(char* name);

char* get_database_path(char* name);

char* get_entity_path(char* name);

void set_entity_path(char* buffer, char* name);

DB* load_database(char* name);

void init_database(FILE* f, char* name);

int create_database(char* name, int* status);

int drop_database(char* name);

ENTITY* create_entity(char* name, int* status);

ENTITY** new_entity_list(uint32_t size);

ENTITY** expand_entity_list(ENTITY** entities, uint32_t* size);

ENTITY* find_entity(char* name);

void free_entity(ENTITY* entity);

int drop_entity(ENTITY* entity);

void append_entity(ENTITY *entity);

int commit();

#endif
