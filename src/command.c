#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/command.h"
#include "../include/string.h"

void get_command(char* buffer)
{
    printf("> ");
    fflush(stdin);
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void clear_command(char* buffer)
{
    if(buffer != NULL)
    {
        memset(buffer, 0, BUFFER_SIZE);
    }
}

void clear_buffer(char* buffer)
{
    if(buffer)
    {
        memset(buffer, 0, BUFFER_SIZE);
    }
}

void create_command(char* string, command* c)
{
    int word_size = 0;
    split_string(c, string, &word_size);
    char *command_word = c->words[0];
    c->word_size = word_size;
    if(word_size > WORD_SIZE)
    {
        c->type = COMMAND_BUFFER_OVERFLOW;
        return;
    }

    if(strcmp(command_word, "create") == 0 || strcmp(command_word, "CREATE") == 0)
    {
        c->type = COMMAND_CREATE;
    } 
    else if(strcmp(command_word, "entity") == 0 || strcmp(command_word, "ENTITY") == 0)
    {
        c->type = COMMAND_ENTITY;
    } 
    else if(strcmp(command_word, "database") == 0 || strcmp(command_word, "DATABASE") == 0)
    {
        c->type = COMMAND_DATABASE;
    } 
    else if(strcmp(command_word, "commit") == 0 || strcmp(command_word, "COMMIT") == 0)
    {
        c->type = COMMAND_COMMIT;
    } 
    else if(strcmp(command_word, "drop") == 0 || strcmp(command_word, "DROP") == 0)
    {
        c->type = COMMAND_DROP;
    } 
    else if(strcmp(command_word, "exit") == 0 || strcmp(command_word, "EXIT") == 0)
    {
        c->type = COMMAND_EXIT;
    } 
    else if(strcmp(command_word, "quit") == 0 || strcmp(command_word, "QUIT") == 0)
    {
        c->type = COMMAND_EXIT;
    }  
    else if(strcmp(command_word, "close") == 0 || strcmp(command_word, "CLOSE") == 0)
    {
        c->type = COMMAND_CLOSE;
    } 
    else if(strcmp(command_word, "context") == 0 || strcmp(command_word, "CONTEXT") == 0)
    {
        c->type = COMMAND_CONTEXT;
    }
    else 
    {
        c->type = COMMAND_UNDEFINED;
    }
}