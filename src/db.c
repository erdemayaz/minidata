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
    set_database_path(register_string, name);
    if(exist_file(register_string))
    {
        FILE *f = open_file(register_string);
        if(f != NULL)
        {
            DB *db = (DB*) malloc(sizeof(DB));

            db->name = read_string_unit(f);
            db->size = read_integer_unit(f);
            db->committed = 1;

            if(db->size > 0)
            {
                db->entities = new_entity_list(db->size);
                int i;
                char *n = NULL;
                uint32_t size = INT32_MIN;
                for(i = 0; i < db->size; ++i)
                {
                    n = read_string_unit(f);
                    size = read_unsigned_integer_unit(f);
                    if(n != NULL && size != INT32_MIN)
                    {
                        db->entities[i] = (ENTITY*) malloc(sizeof(ENTITY));
                        db->entities[i]->name = n;
                        db->entities[i]->file = NULL;
                        db->entities[i]->fields = NULL;
                        db->entities[i]->size = size;
                        db->entities[i]->list_size = 0;
                        db->entities[i]->committed = 1;
                    }
                    else
                    {
                        // to do: free entity memories
                        free(db->name);
                        free(db->entities);
                        free(db);
                        return NULL;
                    }
                }
                db->list_size = db->size;
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
            return NULL;
        }
    }
    else
    {
		return NULL;
    }
}

int load_entity(ENTITY *entity)
{
    if(entity->list_size > 0)
        return 0;
    
    set_entity_path(register_string, entity->name);
    if(exist_file(register_string) == 0)
        return 0;
    FILE *f = open_file(register_string);
    if(f)
    {
        char *name = read_string_unit(f);
        if(name != NULL)
        {
            if(strcmp(name, entity->name) == 0)
            {
                free(name);
                name = entity->name;
            }
            else
            {
                free(name);
                return 0;
            }
        }
        else
        {
            return 0;
        }

        uint32_t size = read_unsigned_integer_unit(f);
        if(size != INT32_MIN && size >= 0)
        {
            if(size != entity->size)
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }

        if(entity->fields == NULL)
        {
            if(size > 0)
            {
                if(size < 4)
                {
                    entity->fields = new_field_list(4);
                    entity->list_size = 4;
                }
                else
                {
                    entity->fields = new_field_list(size);
                    entity->list_size = size;
                }
            }
            else
            {
                entity->list_size = 0;
            }
        }
        else
        {
            return 0;
        }

        int i;
        for(i = 0; i < size; ++i)
        {
            entity->fields[i] = (FIELD*) malloc(sizeof(FIELD));
            entity->fields[i]->name = read_string_unit(f);
            entity->fields[i]->type = read_unsigned_character_unit(f);
            entity->fields[i]->size = read_unsigned_integer_unit(f);
        }
        fclose(f);
        return 1;
    }
    else
    {
        return 0;
    }
}

void init_database(FILE* f, char* name)
{
    uint32_t zero = 0;
    write_string_unit(f, name);
    write_unsigned_integer_unit(f, zero);
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
    DB *database = load_database(name);
    if(database != NULL)
    {
        set_database_path(register_string, name);
        if(remove(register_string) == 0)
        {
            int i;
            int ok = 1;
            for(i = 0; i < database->size; ++i)
            {
                sprintf(register_string, "%s%s/%s.mnty", 
                    db_folder, database->name, database->entities[i]->name);
                if(remove(register_string) != 0)
                {
                    ok = 0;
                    break;
                }
            }
            if(ok)
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

void free_entity_list(ENTITY** entity_list, int size)
{
    int i;
    for(i = 0; i < size; ++i)
    {
        if(entity_list[i]->size > 0)
        {
            free_field_list(entity_list[i]->fields, entity_list[i]->size);
        }
        free(entity_list[i]->name);
        free(entity_list[i]);
    }
    free(entity_list);
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

int drop_field(FIELD* field)
{
    int i;
    ENTITY *entity = ctx->object.ent;
    for(i = 0; i < entity->size; ++i)
    {
        if(entity->fields[i] == field)
        {
            free_field(field);
            for(i = i; i + 1 < entity->size; ++i)
            {
                entity->fields[i] = entity->fields[i + 1];
            }
            entity->size--;
            db->committed = 0;
            entity->committed = 0;
            return 1;
        }
    }
    return 0;
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
            {
                write_string_unit(f, db->entities[i]->name);
                write_unsigned_integer_unit(f, db->entities[i]->size);
            }
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
            {
                write_string_unit(f, entity->fields[i]->name);
                write_unsigned_character_unit(f, entity->fields[i]->type);
                write_unsigned_integer_unit(f, entity->fields[i]->size);
            }
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

    int i;
    ENTITY *entity = ctx->object.ent;
    for(i = 0; i < entity->size; ++i)
    {
        if(strcmp(entity->fields[i]->name, name) == 0)
        {
            return entity->fields[i];
        }
    }
    return NULL;
}

void free_field(FIELD *field)
{
    if(field->name)
        free(field->name);
    free(field);
    field = NULL;
}

void free_field_list(FIELD** fields, int size)
{
    int i;
    for(i = 0; i < size; ++i)
    {
        free_field(fields[i]);
    }
    free(fields);
    fields = NULL;
}

void free_database()
{
    if(db->size > 0)
    {
        free_entity_list(db->entities, db->size);
    }
    free(db->name);
    free(db);
    db = NULL;
}