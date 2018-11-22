#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include "../include/command.h"
#include "../include/db.h"
#include "../include/file.h"

extern DB *db;
extern char* db_folder;

void task_close(int notify)
{
    if(db != NULL)
    {
        close_file(db->file);
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
        task_close(1);
    db = load_database(name);
    if(db == NULL)
        printf("Database '%s' does not exist\n", name);
}

void task_create(char* name)
{
    int status;
    if(create_database(name, &status))
    {
        printf("Database created\n");
    }
    else
    {
        if(status == 1)
        {
            printf("Already exists a database with this name\n");
        }
        else if(status == 2)
        {
            if(!mkdir(db_folder))
            {
                if(create_database(name, &status))
                {
                    printf("Database created\n");
                }
                else
                {
                    printf("An unpredictable error occurred\n");
                }
            }
            else
            {
                printf("'%s' folder could not create\n", db_folder);
            }
        }
    }
}

void task_drop(char* name)
{
    if(db != NULL && strcmp(db->name, name) == 0)
    {
        printf("Database is using\n");
    }
    else
    {
        if(drop_database(name))
        {
            printf("Database dropped\n");
        }
        else
        {
            printf("Database could not drop\n");
        }
    }
}

void task_entity(char* name)
{
    if(db->entities == NULL)
    {

    }
    else
    {
        
    }
}

void perform(command* c)
{
    switch(c->type)
    {
        case COMMAND_DATABASE:
            if(c->word_size == 2)
            {
                task_database(c->words[1]);
                if(db != NULL)
                {
                    printf("Connected to database\n");
                }
            }
            else if(c->word_size == 1)
            {
                if(db != NULL)
                {
                    printf("<%s>\n", db->name);
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
            if(c->word_size == 2)
                task_create(c->words[1]);
            else
                printf("'CREATE' command takes 1 parameter(name:identity)\n");
            break;
        case COMMAND_DROP:
            if(c->word_size == 2)
                task_drop(c->words[1]);
            else
                printf("'DROP' command takes 1 parameter(name:identity)\n");
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
                    task_entity(c->words[1]);
                else
                    printf("'ENTITY' command takes 1 parameter(name:identity)\n");
            }
            else
            {
                printf("Not exist database in context\n");
            }
            break;
    }
}