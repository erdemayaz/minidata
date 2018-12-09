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

extern char *db_folder;
extern DB *db;
extern commit_queue *queue;

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
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) * 2 + strlen(db_folder) + 6));
    sprintf(file_name, "%s%s/%s.mndt", db_folder, name, name);
    return file_name;
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
                        db->entities[i]->commited = 1;
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
        ENTITY *entity = (ENTITY*) malloc(sizeof(ENTITY));
        entity->file = NULL;
        entity->name = duplicate_string(name);
        entity->records = NULL;
        entity->size = 0;
        entity->commited = 0;
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

int commit()
{
    char *path_name = get_database_path(db->name);
    FILE *f = open_file_write(path_name);
    free(path_name);
    if(f)
    {
        int i;
        write_string_unit(f, db->name);
        write_unsigned_integer_unit(f, db->size);
        if(db->size > 0)
        {
            char entity_name[BUFFER_SIZE];
            for(i = 0; i < db->size; ++i)
            {
                write_string_unit(f, db->entities[i]->name);
                if(db->entities[i]->commited == 0)
                {
                    int status;
                    set_entity_path(entity_name, db->entities[i]->name);
                    FILE *entity_file = create_file(entity_name, &status);
                    if(entity_file)
                    {
                        // you can perform other entity (initialize) operations in here
                        fclose(entity_file);
                    }
                    else
                    {
                        // problem seems like complex
                        // evaluate @var status
                    }
                    db->entities[i]->commited++;
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
                printf("dequeued commit is null\n");
            }
        }

        fclose(f);
        return 1;
    }
    else
    {
        free(path_name);
        return 0;
    }
}

void append_entity(ENTITY *entity)
{
    
}