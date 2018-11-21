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
    if(create_database(name))
    {
        printf("Database created\n");
    }
    else
    {
        if(!mkdir(db_folder))
        {
            if(create_database(name))
            {
                printf("Database created\n");
            }
            else
            {
                printf("Database could not create\n");
            }
        }
        else
        {
            printf("'%s' folder could not create\n", db_folder);
        }
    }
}

void task_drop(char* name)
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
            else
            {
                printf("'DATABASE' command takes 1 parameter(name:identity)\n");
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
    }
}