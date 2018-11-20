#include <stdio.h>
#include <stdlib.h>
#include "../include/command.h"
#include "../include/db.h"
#include "../include/file.h"

extern DB *db;

void task_close()
{
    if(db != NULL)
    {
        close_file(db->file);
        free(db);
        db = NULL;
    }
    else
    {
        printf("not exist database in context\n");
    }
}

void task_database(char* name)
{
    if(db != NULL)
        task_close();
    db = load_database(name);
}

void task_create(char* name)
{
    if(create_database(name))
    {
        printf("> database created\n");
    }
    else
    {
        printf("> database could not create\n");
    }
}

void task_drop(char* name)
{
    if(drop_database(name))
    {
        printf("> database dropped\n");
    }
    else
    {
        printf("> database could not drop\n");
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
                    printf("connected to database...\n");
                }
            }
            else
            {
                printf("> 'DATABASE' command takes 1 parameter(name:string)\n");
            }
            break;
        case COMMAND_CREATE:
            if(c->word_size == 2)
                task_create(c->words[1]);
            else
                printf("> 'CREATE' command takes 1 parameter(name:string)\n");
            break;
        case COMMAND_DROP:
            if(c->word_size == 2)
                task_drop(c->words[1]);
            else
                printf("> 'DROP' command takes 1 parameter(name:string)\n");
    }
}