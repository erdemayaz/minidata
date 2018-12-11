#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dir.h>
#include "../include/command.h"
#include "../include/db.h"
#include "../include/file.h"
#include "../include/task.h"
#include "../include/commit.h"
#include "../include/context.h"

extern DB *db;
extern char* db_folder;
extern char register_string[BUFFER_SIZE];
extern CTX *ctx;

void task_close(int notify)
{
    if(db != NULL)
    {
        free(db->name);
        free(db);
        db = NULL;
        set_ctx_host();
    }
    else
    {
        if(notify)
            printf("Not exist database in context\n");
    }
}

void task_database(char* name)
{
    if(db != NULL)
    {
        if(strcmp(db->name, name) == 0)
        {
            printf("This database is also opened\n");
            return;
        }
        else
        {
            task_close(1);
        }
    }
    db = load_database(name);
    if(db == NULL)
    {
        printf("Database '%s' does not exist\n", name);
    } 
    else
    {
        set_ctx_db();
    }
}

void task_create_database(char* name)
{
    if(!exist_dir(db_folder) && mkdir(db_folder))
    {
        printf("'%s' folder could not created\n", db_folder);
        return;
    }

    set_database_dir(register_string, name);
    if(!exist_dir(register_string) && mkdir(register_string))
    {
        printf("'%s' folder could not created\n", register_string);
        return;
    }

    int status;
    if(create_database(name, &status))
    {
        //printf("Database created\n");
    }
    else
    {
        if(status == 1)
        {
            printf("Already exists a database with this name\n");
        }
        else if(status == 2)
        {
            printf("Database file could not created\n");
        }
    }
}

void task_drop_database(char* name)
{
    if(db != NULL && strcmp(db->name, name) == 0)
    {
        printf("Database is using\n");
    }
    else
    {
        if(drop_database(name))
        {
            //printf("Database dropped\n");
        }
        else
        {
            printf("Database could not drop\n");
        }
    }
}

void task_drop_entity(char* name)
{
    if(db != NULL)
    {
        ENTITY *e = find_entity(name);
        if(e)
        {
            if(free_entity(e))
            {
                enqueue_commit(create_commit(COMMIT_DROP_ENTITY, name));
            }
            else
            {
                printf("Entity '%s' could not dropped\n", name);
            }
        }
        else
        {
            printf("There is no entity in database as '%s'\n", name);
        }
    }
    else
    {
        printf("Not exist database in context\n");
    }
}

void task_create_entity(char* name)
{
    int status;
    ENTITY *e = create_entity(name, &status);
    if(e)
    {
        if(db->entities == NULL)
        {
            db->entities = new_entity_list(4);
            db->list_size = 4;
            db->entities[0] = e;
            db->size = 1;
        }
        else
        {
            int old_list_size = db->list_size;
            if(db->size >= db->list_size)
            {
                ENTITY **temp = expand_entity_list(db->entities, &db->list_size);
                if(temp != NULL)
                {
                    if(db->entities != temp)
                        db->entities = temp;
                }
                else
                {
                    printf("Operating system did not allocate memory, entities list could not expanded\n");
                    drop_entity(e);
                    db->list_size = old_list_size;
                    return;
                }
            }
            db->entities[db->size++] = e;
        }
    }
    else
    {
        if(status == 1)
        {
            printf("Already exists a entity in database with this name\n");
        }
        else if(status == 2)
        {
            printf("Entity file could not created\n");
        }
    }
}

void task_commit()
{
    if(!commit())
    {
        printf("Commit failed\n");
    }
}

void task_list_entities()
{
    if(db->size > 0)
    {
        int i;
        for(i = 0; i < db->size; ++i)
        {
            printf("%s\n", db->entities[i]->name);
        }
    }
    else
    {
        printf("\n");
    }
    printf("\n");
}

void perform(command* c)
{
    switch(c->type)
    {
        case COMMAND_DATABASE:
            if(c->word_size == 2)
            {
                if(strcmp(c->words[1], "ENTITIES") == 0)
                {
                    if(db != NULL)
                    {
                        task_list_entities();
                    }
                    else
                    {
                        printf("Not exist database in context\n");
                    }
                }
                else
                {
                    task_database(c->words[1]);
                    if(db == NULL)
                    {
                        printf("Connection failed\n");
                    }
                }
            }
            else if(c->word_size == 1)
            {
                if(db != NULL)
                {
                    printf("<%s, %d entities>\n", db->name, db->size);
                }
                else
                {
                    printf("<no database>\n");
                }
            }
            else
            {
                printf("'DATABASE' command takes 1 optional parameter(name:identity)\n");
            }
            break;
        case COMMAND_CREATE:
            if(c->word_size == 3)
            {
                if(strcmp(c->words[1], "DATABASE") == 0)
                {
                    task_create_database(c->words[2]);
                }
                else if(strcmp(c->words[1], "ENTITY") == 0)
                {
                    if(db != NULL)
                    {
                        task_create_entity(c->words[2]);
                    }
                    else
                    {
                        printf("Not exist database in context\n");
                    }
                }
                else if(strcmp(c->words[1], "FIELD") == 0)
                {
                    if(db != NULL)
                    {
                        
                        // create field
                    }
                    else
                    {
                        printf("Not exist database in context\n");
                    }
                }
                else
                {
                    printf("'%s' is undefined type\n", c->words[1]);
                }
            }
            else
            {
                printf("'CREATE' command takes 2 parameter(type:[DATABASE, ENTITY, FIELD], name:identity)\n");
            }
            break;
        case COMMAND_DROP:
            if(c->word_size == 3)
            {
                if(strcmp(c->words[1], "DATABASE") == 0)
                {
                    task_drop_database(c->words[2]);
                }
                else if(strcmp(c->words[1], "ENTITY") == 0)
                {
                    task_drop_entity(c->words[2]);
                }
                else
                {
                    printf("'DROP' command takes 2 parameter(type:[DATABASE, ENTITY], name:identity)\n");
                }
            }
            else
            {
                printf("'DROP' command takes 2 parameter(type:[DATABASE, ENTITY], name:identity)\n");
            }
            break;
        case COMMAND_CLOSE:
            if(c->word_size == 1)
                task_close(1);
            else
                printf("'CLOSE' command takes any parameter\n");
            break;
        case COMMAND_ENTITY:
            if(db != NULL)
            {
                if(c->word_size == 2)
                {
                    if(strcmp(c->words[1], "FIELDS") == 0)
                    {

                    }
                    else if(strcmp(c->words[1], "COUNT") == 0)
                    {
                        printf("<%d entities>\n", db->size);
                    }
                    else
                    {
                        ENTITY *e = find_entity(c->words[1]);
                        if(e)
                        {
                            set_ctx_entity(e);
                        }
                        else
                        {
                            printf("There is not entity '%s' in database\n", c->words[1]);
                        }
                    }
                }
                else if(c->word_size == 1)
                {
                    if(ctx->type == CTX_ENTITY)
                    {
                        printf("<%s, %d fields\n", ctx->object.ent->name, ctx->object.ent->size);
                    }
                    else
                    {
                        printf("Not exist entity in context\n");
                    }
                }
                else
                {
                    printf("'ENTITY' command takes 1 optional parameter(name:identity)\n");
                }
            }
            else
            {
                printf("Not exist database in context\n");
            }
            break;
        case COMMAND_EXIT:
            break;
        case COMMAND_UNDEFINED:
            break;
        case COMMAND_BUFFER_OVERFLOW:
            break;
        case COMMAND_COMMIT: 
            if(c->word_size == 1)
            {
                if(db != NULL)
                {
                    task_commit();
                }
                else
                {
                    printf("Not exist database in context\n");
                }
            }
            else
            {
                printf("'COMMIT' command takes any parameter\n");
            }
            break;
        case COMMAND_CONTEXT:
            if(c->word_size == 1)
            {
                context_t type = get_ctx_type();
                switch(type)
                {
                    case CTX_HOST:
                        printf("<host>\n");
                        break;
                    case CTX_DATABASE:
                        printf("<database, %s>\n", db->name);
                        break;
                    case CTX_ENTITY:
                        printf("<entity, %s>\n", ctx->object.ent->name);
                        break;
                    case CTX_FIELD:
                        break;
                }
            }
            else if(c->word_size == 2)
            {
                if(strcmp(c->words[1], "UP") == 0)
                {

                }
                else
                {
                    printf("'CONTEXT' command takes 1 optional parameter(type:[UP])\n");
                }
            }
            else
            {
                printf("'CONTEXT' command takes 1 optional parameter(type:[UP])\n");
            }
            break;
    }
}