#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dir.h>
#include "../include/file.h"
#include "../include/db.h"
#include "../include/string.h"
#include "../include/data.h"
#include "../include/commit.h"
#include "../include/context.h"

extern char *db_folder;
extern DB *db;
extern commit_queue *queue;
extern CTX *ctx;

char register_string[BUFFER_SIZE];

char* get_database_dir(char* name)
{
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) + strlen(db_folder)));
    sprintf(file_name, "%s%s/", db_folder, name);
    return file_name;
}

void set_database_dir(char* buffer, char* name)
{
    sprintf(buffer, "%s%s/", db_folder, name);
}

char* get_database_path(char* name)
{
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) * 2 + strlen(db_folder) + 6));
    sprintf(file_name, "%s%s/%s.mndt", db_folder, name, name);
    return file_name;
}

void set_database_path(char* buffer, char* name)
{
    sprintf(buffer, "%s%s/%s.mndt", db_folder, name, name);
}

char* get_entity_path(char* name)
{
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) * 2 + strlen(db_folder) + 6));
    sprintf(file_name, "%s%s/%s.mnty", db_folder, db->name, name);
    return file_name;
}

void set_entity_path(char* buffer, char* name)
{
    sprintf(buffer, "%s%s/%s.mnty", db_folder, db->name, name);
}

char* get_field_path(char* name)
{
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) * 2 + strlen(db_folder) + 6));
    sprintf(file_name, "%s%s/%s.mnfd", db_folder, db->name, name);
    return file_name;
}

void set_field_path(char* buffer, char* name)
{
    sprintf(buffer, "%s%s/%s.mnfd", db_folder, db->name, name);
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

            db->committed = 1;

            if(db->size > 0)
            {
                db->entities = new_entity_list(db->size);
                int i;
                uint8_t type;
                uint32_t size;
                void *buffer = (void*) malloc(BUFFER_SIZE);
                for(i = 0; i < db->size; ++i)
                {
                    fread(&type, 1, 1, f);
                    fread(&size, 4, 1, f);
                    fread(buffer, size, 1, f);
                    if(type == TYPE_STRING)
                    {
                        db->entities[i] = (ENTITY*) malloc(sizeof(ENTITY));
                        db->entities[i]->name = string((char*) buffer, size);
                        db->entities[i]->file = NULL;
                        db->entities[i]->fields = NULL;
                        db->entities[i]->list_size = 0;
                        db->entities[i]->committed = 1;
                    }
                    else
                    {
                        // big problem
                    }
                }
                db->list_size = db->size;
                free(buffer);
            }
            else
            {
                db->entities = NULL;
                db->list_size = 0;
            }
            
            fclose(f);
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
    set_database_path(register_string, name);
    FILE *f = create_file(register_string, status);
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
    set_database_path(register_string, name);
    if(exist_file(register_string))
    {
        if(remove(register_string) == 0)
        {
            set_database_dir(register_string, name);
            rmdir(register_string);
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

ENTITY* create_entity(char* name, int* status)
{
        ENTITY *entity = (ENTITY*) malloc(sizeof(ENTITY));
        entity->file = NULL;
        entity->name = duplicate_string(name);
        entity->fields = NULL;
        entity->size = 0;
        entity->committed = 0;
        *status = 0;
        return entity;
}

ENTITY** new_entity_list(uint32_t size)
{
    return (ENTITY**) malloc(sizeof(ENTITY*) * size);
}

ENTITY** expand_entity_list(ENTITY** entities, uint32_t* size)
{
    *size = *size + (*size / 2);
    return (ENTITY**) realloc(entities, *size * (sizeof(ENTITY*)));
}

ENTITY* find_entity(char* name)
{
    int i;
    for(i = 0; i < db->size; ++i)
    {
        if(strcmp(db->entities[i]->name, name) == 0)
        {
            return db->entities[i];
        }
    }
    return NULL;
}

int free_entity(ENTITY* entity)
{
    int i;
    int res = 0;
    for(i = 0; i < db->size; ++i)
    {
        if(db->entities[i] == entity)
        {
            if(entity->file != NULL)
                fclose(entity->file);
            free(entity->name);
            free(entity);
            for(i = i; i + 1 < db->size; ++i)
            {
                db->entities[i] = db->entities[i + 1];
            }
            db->size--;
            res++;
            break;
        }
    }
    return res;
}

int drop_entity(ENTITY* entity)
{
    char *file_name = get_entity_path(entity->name);
    if(exist_file(file_name))
    {
        if(remove(file_name) == 0)
        {
            free(file_name);
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

int commit_db()
{
    set_database_path(register_string, db->name);
    FILE *f = open_file_write(register_string);
    if(f)
    {
        int i;
        write_string_unit(f, db->name);
        write_unsigned_integer_unit(f, db->size);
        if(db->size > 0)
        {
            for(i = 0; i < db->size; ++i)
                write_string_unit(f, db->entities[i]->name);
        }
        db->committed = 1;
        fclose(f);
        return 1;
    }
    else
    {
        return 0;
    }
}

int commit_entity(ENTITY *entity)
{
    set_entity_path(register_string, entity->name);
    FILE *f = open_file_write(register_string);
    if(f)
    {
        int i;
        write_string_unit(f, entity->name);
        write_unsigned_integer_unit(f, entity->size);
        if(entity->size > 0)
        {
            for(i = 0; i < entity->size; ++i)
                write_string_unit(f, entity->fields[i]->name);
        }
        entity->committed = 1;
        fclose(f);
        return 1;
    }
    else
    {
        return 0;
    }
}

int commit_field(FIELD *field)
{
    set_field_path(register_string, field->name);
    return 0;
}

int commit()
{
    int i;
    if(db->committed == 0)
    {
        if(commit_db() == 0)
        {
            return 0;
        }
    }
        
    for(i = 0; i < db->size; ++i)
    {
        if(db->entities[i]->committed == 0)
        {
            if(commit_entity(db->entities[i]) == 0)
            {
                return 0;
            }
        }
    }

    char buffer[BUFFER_SIZE];
    commit_t *c = NULL;
    while(queue->size > 0)
    {
        c = dequeue_commit();
        if(c != NULL)
        {
            if(c->type == COMMIT_DROP_ENTITY)
            {
                set_entity_path(buffer, c->object_name);
                remove(buffer);
            }
            if(c->object_name)
                free(c->object_name);
            free(c);
            c = NULL;
        }
        else
        {
            // queue operation problem
            return 0;
        }
    }
    return 1;
}

void append_entity(ENTITY *entity)
{
    
}

FIELD* create_field(char* name, data_t type, uint32_t size, int* status)
{
        FIELD *field = (FIELD*) malloc(sizeof(FIELD));
        field->type = type;
        field->name = duplicate_string(name);
        field->size = size;
        field->committed = 0;
        *status = 0;
        return field;
}

FIELD** new_field_list(uint32_t size)
{
    return (FIELD**) malloc(sizeof(FIELD*) * size);
}

FIELD** expand_field_list(FIELD** fields, uint32_t* size)
{
    *size = *size + (*size / 2);
    return (FIELD**) realloc(fields, *size * (sizeof(FIELD*)));
}

FIELD* find_field(char* name)
{
    return NULL;
}

void free_field(FIELD *field)
{
    if(field->name)
        free(field->name);
    free(field);
    field = NULL;
}