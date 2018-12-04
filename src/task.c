#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dir.h>
#include "../include/command.h"
#include "../include/db.h"
#include "../include/file.h"
#include "../include/task.h"

extern DB *db;
extern char* db_folder;

void task_close(int notify)
{
    if(db != NULL)
    {
        free(db->name);
        free(db);
        db = NULL;
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
        printf("Database '%s' does not exist\n", name);
}

void task_create_database(char* name)
{
    if(!exist_dir(db_folder) && mkdir(db_folder))
    {
        printf("'%s' folder could not created\n", db_folder);
        return;
    }

    char *db_dir = get_database_dir(name);
    if(!exist_dir(db_dir) && mkdir(db_dir))
    {
        printf("'%s' folder could not created\n", db_dir);
        return;
    }
    free(db_dir);

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
                else
                {
                    printf("'%s' is undefined type\n", c->words[1]);
                }
            }
            else
            {
                printf("'CREATE' command takes 2 parameter(type:[DATABASE, ENTITY], name:identity)\n");
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
            
            break;
        case COMMAND_EXIT:
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
    }
}