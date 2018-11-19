#include <stdio.h>
#include "../include/command.h"
#include "../include/db.h"

void task_new()
{
    
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

void perform(command* c)
{
    switch(c->type)
    {
        case COMMAND_NEW:
            task_new();
            break;
        case COMMAND_CREATE:
            if(c->word_size == 2)
                task_create(c->words[1]);
            else
                printf("> 'CREATE' command takes 1 parameter(name:string)\n");
            break;
    }
}