#ifndef _DB_H_
#define _DB_H_

#include <stdint.h>
#include "../include/data.h"

typedef struct field
{
    data_t type;
    char *name;
    uint32_t size;
    uint8_t committed;
} FIELD;

typedef struct entity
{
    char *name;
    FILE *file;
    FIELD **fields;
    uint32_t size;
    uint32_t list_size;
    uint8_t committed;
} ENTITY;

typedef struct db 
{
    int id;
    char *name;
    ENTITY **entities;
    int size;
    uint32_t list_size;
    uint8_t committed;
} DB;

char* get_database_dir(char* name);

void set_database_dir(char* buffer, char* name);

char* get_database_path(char* name);

void set_database_path(char* buffer, char* name);

char* get_entity_path(char* name);

void set_entity_path(char* buffer, char* name);

char* get_field_path(char* name);

void set_field_path(char* buffer, char* name);

DB* load_database(char* name);

int load_entity(ENTITY *entity);

void init_database(FILE* f, char* name);

int create_database(char* name, int* status);

int drop_database(char* name);

ENTITY* create_entity(char* name, int* status);

ENTITY** new_entity_list(uint32_t size);

ENTITY** expand_entity_list(ENTITY** entities, uint32_t* size);

ENTITY* find_entity(char* name);

int free_entity(ENTITY* entity);

int drop_entity(ENTITY* entity);

int drop_field(FIELD* field);

void append_entity(ENTITY *entity);

int commit_db();

int commit_entity(ENTITY *entity);

int commit_field(FIELD *field);

int commit();

FIELD* create_field(char* name, data_t type, uint32_t size, int* status);

FIELD** new_field_list(uint32_t size);

FIELD** expand_field_list(FIELD** fields, uint32_t* size);

FIELD* find_field(char* name);

void free_field(FIELD *field);

#endif
