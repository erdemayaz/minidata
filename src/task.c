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
#include "../include/string.h"
#include "../tool/miniscanner.h"

extern DB *db;
extern char* db_folder;
extern char register_string[BUFFER_SIZE];
extern CTX *ctx;
extern int flow_status;
extern int flow_mode; // 1 - cli, 2 - sli
extern int runtime;

void task_close(int notify)
{
    if(db != NULL)
    {
        free_database();
        set_ctx_host();
    }
    else
    {
        if(notify)
        {
            if(flow_mode == 1)
                printf("Not exist database in context\n");
            flow_status = -1;
        }
    }
}

void task_database(char* name)
{
    if(db != NULL)
    {
        if(strcmp(db->name, name) == 0)
        {
            if(flow_mode == 1)
                printf("This database is also opened\n");
            flow_status = -1;
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
        flow_status = 0;
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
        flow_status = 0;
        return;
    }

    set_database_dir(register_string, name);
    if(!exist_dir(register_string) && mkdir(register_string))
    {
        printf("'%s' folder could not created\n", register_string);
        flow_status = 0;
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
        flow_status = 0;
    }
}

void task_drop_database(char* name)
{
    if(db != NULL && strcmp(db->name, name) == 0)
    {
        if(flow_mode == 1)
            printf("Database is using\n");
        flow_status = -1;
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
            flow_status = 0;
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
            if(ctx->type == CTX_ENTITY && ctx->object.ent == e)
                context_up();
            
            if(free_entity(e))
            {
                enqueue_commit(create_commit(COMMIT_DROP_ENTITY, name));
            }
            else
            {
                printf("Entity '%s' could not dropped\n", name);
                flow_status = 0;
            }
        }
        else
        {
            if(flow_mode == 1)
                printf("There is no entity in database as '%s'\n", name);
            flow_status = -1;
        }
    }
    else
    {
        if(flow_mode == 1)
            printf("Not exist database in context\n");
        flow_status = -1;
    }
}

void task_drop_field(char* name)
{
    if(db != NULL)
    {
        if(ctx->type == CTX_ENTITY)
        {
            FIELD *field = find_field(name);
            if(field)
            {
                if(drop_field(field) == 0)
                {
                    printf("Field '%s' could not dropped\n", name);
                    flow_status = 0;
                }
            }
            else
            {
                if(flow_mode == 1)
                    printf("Entity has not field as '%s'\n", name);
                flow_status = -1;
            }
        }
        else
        {
            if(flow_mode == 1)
                printf("Not exist entity in context\n");
            flow_status = -1;
        }
    }
    else
    {
        if(flow_mode == 1)
            printf("Not exist database in context\n");
        flow_status = -1;
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
            db->committed = 0;
        }
        else
        {
            if(find_entity(name) == NULL)
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
                        flow_status = 0;
                        return;
                    }
                }
                db->entities[db->size++] = e;
                db->committed = 0;
            }
            else
            {
                printf("Already exists a entity in database with this name\n");
            }
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
        flow_status = 0;
    }
}

void task_commit()
{
    if(!commit())
    {
        if(flow_mode == 1)
            printf("Commit failed\n");
        flow_status = -1;
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
}

void task_create_field(char *name, data_t type, uint32_t size)
{
    int status;
    FIELD *f = create_field(name, type, size, &status);
    if(f)
    {
        ENTITY *e = ctx->object.ent;
        if(e->fields == NULL)
        {
            e->fields = new_field_list(4);
            e->list_size = 4;
            e->fields[0] = f;
            e->size = 1;
        }
        else
        {
            int old_list_size = e->list_size;
            if(e->size >= e->list_size)
            {
                FIELD **temp = expand_field_list(e->fields, &e->list_size);
                if(temp != NULL)
                {
                    if(e->fields != temp)
                        e->fields = temp;
                }
                else
                {
                    printf("Operating system did not allocate memory, field list could not expanded\n");
                    free_field(f);
                    e->list_size = old_list_size;
                    flow_status = 0;
                    return;
                }
            }
            e->fields[e->size++] = f;
        }
        e->committed = 0;
        db->committed = 0;
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
        flow_status = 0;
    }
}

void perform(command* c)
{
    switch(c->type)
    {
        case COMMAND_DATABASE:
            if(c->token_size == 2)
            {
                if(c->tokens[1] == KEYWORD_ENTITIES)
                {
                    if(db != NULL)
                    {
                        task_list_entities();
                    }
                    else
                    {
                        if(flow_mode == 1)
                            printf("Not exist database in context\n");
                        flow_status = -1;
                    }
                }
                else
                {
                    if(c->tokens[1] == TOKEN_IDENTIFIER)
                    {
                        task_database(c->values[1]);
                        if(db == NULL)
                        {
                            if(flow_mode == 1)
                                printf("Connection failed\n");
                            flow_status = -1;
                        }
                    }
                    else
                    {
                        printf("'DATABASE' command takes 1 optional parameter(name:identity)\n");
                        flow_status = 0;
                    }
                }
            }
            else if(c->token_size == 1)
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
                flow_status = 0;
            }
            break;
        case COMMAND_CREATE:
            if(c->token_size == 5)
            {
                if(c->tokens[1] == KEYWORD_FIELD)
                {
                    if(db != NULL)
                    {
                        if(ctx->type == CTX_ENTITY)
                        {
                            data_t type;
                            if(c->tokens[2] == KEYWORD_STRING)
                            {
                                type = TYPE_STRING;
                            }
                            else if(c->tokens[2] == KEYWORD_NUMBER)
                            {
                                type = TYPE_NUMBER;
                            }
                            else if(c->tokens[2] == KEYWORD_OBJECT)
                            {
                                type = TYPE_OBJECT;
                            }
                            else if(c->tokens[2] == KEYWORD_ARRAY)
                            {
                                type = TYPE_ARRAY;
                            }
                            else if(c->tokens[2] == KEYWORD_BOOLEAN)
                            {
                                type = TYPE_BOOLEAN;
                            }
                            else if(c->tokens[2] == KEYWORD_NULL)
                            {
                                type = TYPE_NULL;
                            }
                            else
                            {
                                printf("'%s' is undefined data type\n", c->values[2]);
                                flow_status = 0;
                                return;
                            }
                            int status;
                            int size;
                            if(c->tokens[3] == TOKEN_INTNUM)
                            {
                                size = string_to_integer(c->values[3], &status);
                                if(status == 0)
                                {
                                    if(size < 0)
                                    {
                                        printf("Incorrect input for size\n");
                                        flow_status = 0;
                                        return;
                                    }
                                }
                                else
                                {
                                    printf("Incorrect input for size\n");
                                    flow_status = 0;
                                    return;
                                }
                            }
                            else
                            {
                                printf("Incorrect input for size\n");
                                flow_status = 0;
                                return;
                            }
                            if(c->tokens[4] == TOKEN_IDENTIFIER)
                            {
                                task_create_field(c->values[4], type, (uint32_t) size);
                            }
                            else
                            {
                                printf("Incorrect input for name\n");
                                flow_status = 0;
                                return;
                            }
                        }
                        else
                        {
                            if(flow_mode == 1)
                                printf("Not exist entity in context\n");
                            flow_status = -1;
                        }
                    }
                    else
                    {
                        if(flow_mode == 1)
                            printf("Not exist database in context\n");
                        flow_status = -1;
                    }
                }
                else
                {
                    printf("'CREATE' command takes 2 parameter(type:[DATABASE, ENTITY, FIELD], name:identity)\n");
                    flow_status = 0;
                }
            }
            else if(c->token_size == 3)
            {
                if(c->tokens[1] == KEYWORD_DATABASE)
                {
                    if(c->tokens[2] == TOKEN_IDENTIFIER)
                    {
                        task_create_database(c->values[2]);
                    }
                    else
                    {
                        printf("'CREATE DATABASE' command takes 1 parameter(name:identity)\n");
                        flow_status = 0;
                    }
                }
                else if(c->tokens[1] == KEYWORD_ENTITY)
                {
                    if(db != NULL)
                    {
                        if(c->tokens[2] == TOKEN_IDENTIFIER)
                        {
                            task_create_entity(c->values[2]);
                        }
                        else
                        {
                            printf("'CREATE ENTITY' command takes 1 parameter(name:identity)\n");
                        }
                    }
                    else
                    {
                        if(flow_mode == 1)
                            printf("Not exist database in context\n");
                        flow_status = -1;
                    }
                }
                else
                {
                    printf("'%s' is undefined type\n", c->values[1]);
                    flow_status = 0;
                }
            }
            else
            {
                printf("'CREATE' command takes 2 parameter(type:[DATABASE, ENTITY, FIELD], name:identity)\n");
                flow_status = 0;
            }
            break;
        case COMMAND_DROP:
            if(c->token_size == 3)
            {
                if(c->tokens[1] == KEYWORD_DATABASE)
                {
                    if(c->tokens[2] == TOKEN_IDENTIFIER)
                    {
                        task_drop_database(c->values[2]);
                    }
                    else
                    {
                        printf("'DROP DATABASE' command takes 1 parameter(name:identity)\n");
                        flow_status = 0;
                    }
                }
                else if(c->tokens[1] == KEYWORD_ENTITY)
                {
                    if(c->tokens[2] == TOKEN_IDENTIFIER)
                    {
                        task_drop_entity(c->values[2]);
                    }
                    else
                    {
                        printf("'DROP ENTITY' command takes 1 parameter(name:identity)\n");
                        flow_status = 0;
                    }
                }
                else if(c->tokens[1] == KEYWORD_FIELD)
                {
                    if(c->tokens[2] == TOKEN_IDENTIFIER)
                    {
                        task_drop_field(c->values[2]);
                    }
                    else
                    {
                        printf("'DROP FIELD' command takes 1 parameter(name:identity)\n");
                        flow_status = 0;
                    }
                }
                else
                {
                    printf("'DROP' command takes 2 parameter(type:[DATABASE, ENTITY, FIELD], name:identity)\n");
                    flow_status = 0;
                }
            }
            else
            {
                printf("'DROP' command takes 2 parameter(type:[DATABASE, ENTITY], name:identity)\n");
                flow_status = 0;
            }
            break;
        case COMMAND_CLOSE:
            if(c->token_size == 1)
            {
                task_close(1);
            }
            else
            {
                printf("'CLOSE' command takes any parameter\n");
                flow_status = 0;
            }
            break;
        case COMMAND_ENTITY:
            if(db != NULL)
            {
                if(c->token_size == 2)
                {
                    if(c->tokens[1] == KEYWORD_FIELDS)
                    {
                        if(ctx->type == CTX_ENTITY)
                        {
                            int i;
                            ENTITY *e = ctx->object.ent;
                            for(i = 0; i < e->size; ++i)
                            {
                                printf("%s\n", e->fields[i]->name);
                            }
                        }
                        else
                        {
                            if(flow_mode == 1)
                                printf("Not exist entity in context\n");
                            flow_status = -1;
                        }
                    }
                    else if(c->tokens[1] == KEYWORD_COUNT)
                    {
                        if(ctx->type == CTX_ENTITY)
                        {
                            printf("<%d fields>\n", ctx->object.ent->size);
                        }
                        else
                        {
                            if(flow_mode == 1)
                                printf("Not exist entity in context\n");
                            flow_status = -1;
                        }
                    }
                    else if(c->tokens[1] == TOKEN_IDENTIFIER)
                    {
                        ENTITY *e = find_entity(c->values[1]);
                        if(e)
                        {
                            set_ctx_entity(e);
                            load_entity(e);
                        }
                        else
                        {
                            printf("There is not entity '%s' in database\n", c->values[1]);
                            flow_status = 0;
                        }
                    }
                    else
                    {
                        printf("'ENTITY' command takes 1 optional parameter(type:[ENTITY, COUNT] || name:identity)\n");
                        flow_status = 0;
                    }
                }
                else if(c->token_size == 1)
                {
                    if(ctx->type == CTX_ENTITY)
                    {
                        printf("<%s, %d fields>\n", ctx->object.ent->name, ctx->object.ent->size);
                    }
                    else
                    {
                        if(flow_mode == 1)
                            printf("Not exist entity in context\n");
                        flow_status = -1;
                    }
                }
                else
                {
                    printf("'ENTITY' command takes 1 optional parameter(type:[ENTITY, COUNT] || name:identity)\n");
                    flow_status = 0;
                }
            }
            else
            {
                if(flow_mode == 1)
                    printf("Not exist database in context\n");
                flow_status = -1;
            }
            break;
        case COMMAND_EXIT:
            break;
        case COMMAND_UNDEFINED:
            break;
        case COMMAND_BUFFER_OVERFLOW:
            break;
        case COMMAND_COMMIT: 
            if(c->token_size == 1)
            {
                if(db != NULL)
                {
                    task_commit();
                }
                else
                {
                    if(flow_mode == 1)
                        printf("Not exist database in context\n");
                    flow_status = -1;
                }
            }
            else
            {
                printf("'COMMIT' command takes any parameter\n");
                flow_status = 0;
            }
            break;
        case COMMAND_CONTEXT:
            if(c->token_size == 1)
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
            else if(c->token_size == 2)
            {
                if(c->tokens[1] == KEYWORD_UP)
                {
                    context_up();
                }
                else
                {
                    printf("'CONTEXT' command takes 1 optional parameter(type:[UP])\n");
                    flow_status = 0;
                }
            }
            else
            {
                printf("'CONTEXT' command takes 1 optional parameter(type:[UP])\n");
                flow_status = 0;
            }
            break;
        case COMMAND_SET:
            if(c->token_size > 1)
            {
                if(c->tokens[1] == KEYWORD_RUNTIME)
                {
                    if(c->token_size == 3)
                    {
                        if(c->tokens[2] == KEYWORD_ON)
                        {
                            runtime = 1;
                        }
                        else if(c->tokens[2] == KEYWORD_OFF)
                        {
                            runtime = 0;
                        }
                        else
                        {
                            if(flow_mode == 1)
                                printf("'SET RUNTIME' command takes 1 parameter(type:[ON, OFF])\n");
                            flow_status = -1;
                        }
                    }
                    else
                    {
                        if(flow_mode == 1)
                            printf("'SET RUNTIME' command takes 1 parameter(type:[ON, OFF])\n");
                        flow_status = -1;
                    }
                }
                else if(c->tokens[1] == KEYWORD_CHARSET)
                {
                    // set charset
                }
                else
                {
                    if(flow_mode == 1)
                        printf("'SET' command takes 1 parameter(type:[RUNTIME, CHARSET])\n");
                    flow_status = -1;
                }
            }
            else
            {
                if(flow_mode == 1)
                    printf("'SET' command takes 1 parameter(type:[RUNTIME, CHARSET])\n");
                flow_status = -1;
            }
            break;
    }
}