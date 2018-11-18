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
            task_create(c->text);
            break;
    }
}