#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dir.h>
#include "../include/file.h"
#include "../include/db.h"
#include "../include/string.h"
#include "../include/data.h"

extern char *db_folder;
extern DB *db;

CTX* init_ctx()
{
    CTX *c = (CTX*) malloc(sizeof(CTX));
    c->type = CTX_TERMINAL;
    return c;
}

void destroy_ctx(CTX* c)
{
    if(c)
    {
        free(c);
    }
}

char* get_database_dir(char* name)
{
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) + strlen(db_folder)));
    sprintf(file_name, "%s%s/", db_folder, name);
    return file_name;
}

char* get_database_path(char* name)
{
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) + strlen(db_folder) + 5));
    sprintf(file_name, "%s%s/%s.mndt", db_folder, name, name);
    return file_name;
}

char* get_entity_path(char* name)
{
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) + strlen(db_folder) + 5));
    sprintf(file_name, "%s%s/%s.mnty", db_folder, db->name, name);
    return file_name;
}

DB* load_database(char* name)
{
    DB *db = (DB*) malloc(sizeof(DB));
    char *file_name = get_database_path(name);
    if(exist_file(file_name))
    {
        FILE *f = open_file(file_name);
        free(file_name);
        if(f != NULL)
        {
            db->file = f;
            data_unit *du = NULL;

            du = read_data_unit(f);
            char *db_name = string((char*) du->data, du->size);
            free(du->data);
            db->name = db_name;
            free_data_unit(du);

            du = read_data_unit(f);
            int *temp = (int*) du->data; 
            db->size = *temp;
            free(du->data);
            free_data_unit(du);

            db->entities = NULL;

            return db;
        }
        else
        {
            free(db);
            return NULL;
        }
    }
    else
    {
        free(db);
        free(file_name);
		return NULL;
    }
}

void init_database(FILE* f, char* name)
{
    uint32_t zero = 0;
    data_unit *unit = NULL;
    unit = create_data_unit(TYPE_STRING, strlen(name), (void*) name);
    write_data_unit(f, unit);
    free_data_unit(unit);
    unit = create_data_unit(TYPE_NUMBER, 4, (void*) &zero);
    write_data_unit(f, unit);
    free_data_unit(unit);
}

int create_database(char* name, int* status)
{
    char *file_name = get_database_path(name);
    FILE *f = create_file(file_name, status);
    free(file_name);
    if(f)
    {
        *status = 0;
        init_database(f, name);
        close_file(f);
        return 1;
    } else {
        return 0;
    }
}

int drop_database(char* name)
{
    char *file_name = get_database_path(name);
    if(exist_file(file_name))
    {
        if(remove(file_name) == 0)
        {
            free(file_name);
            char *dir = get_database_dir(name);
            rmdir(dir);
            free(dir);
            return 1;
        }
        else
        {
            free(file_name);
            return 0;
        }
    }
    else
    {
        free(file_name);
        return 0;
    }
}

ENTITY* create_entity(char* name, int* status)
{
    char *file_name = get_entity_path(name);
    FILE *f = create_file(file_name, status);
    free(file_name);
    if(f)
    {
        close_file(f);
        ENTITY *entity = (ENTITY*) malloc(sizeof(ENTITY));
        entity->file = NULL;
        entity->name = duplicate_string(name);
        entity->records = NULL;
        entity->size = 0;
        *status = 0;
        return entity;
    }
    else
    {
        return NULL;
    }
}

ENTITY** new_entity_list(uint32_t size)
{
    return (ENTITY**) malloc(sizeof(ENTITY*) * size);
}

ENTITY** expand_entity_list(ENTITY** entities, uint32_t size)
{
    return (ENTITY**) realloc(entities, (size + (size / 2)));
}

void free_entity(ENTITY* entity)
{
    if(entity->file != NULL)
        fclose(entity->file);
    free(entity->name);
    free(entity);
}

int drop_entity(ENTITY* entity)
{
    char *file_name = get_entity_path(entity->name);
    if(exist_file(file_name))
    {
        if(remove(file_name) == 0)
        {
            free(file_name);
            free_entity(entity);
            return 1;
        }
        else
        {
            free(file_name);
            return 0;
        }
    }
    else
    {
        free(file_name);
        return 0;
    }
}