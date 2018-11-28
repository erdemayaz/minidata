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

void flush_command(command* c)
{
    
}

void create_command(char* string, command* c)
{
    int word_size = 0;
    split_string(c, string, &word_size);
    char *command_word = c->words[0];

    if(strcmp(command_word, "CREATE") == 0)
    {
        c->type = COMMAND_CREATE;
        c->text = string;
        c->word_size = word_size;
    } 
    else if(strcmp(command_word, "ENTITY") == 0)
    {
        c->type = COMMAND_ENTITY;
        c->text = string;
        c->word_size = word_size;
    } 
    else if(strcmp(command_word, "DATABASE") == 0)
    {
        c->type = COMMAND_DATABASE;
        c->text = string;
        c->word_size = word_size;
    } 
    else if(strcmp(command_word, "DROP") == 0)
    {
        c->type = COMMAND_DROP;
        c->text = string;
        c->word_size = word_size;
    } 
    else if(strcmp(command_word, "EXIT") == 0)
    {
        c->type = COMMAND_EXIT;
        c->text = string;
        c->word_size = word_size;
    } 
    else if(strcmp(command_word, "QUIT") == 0)
    {
        c->type = COMMAND_EXIT;
        c->text = string;
        c->word_size = word_size;
    }  
    else if(strcmp(command_word, "CLOSE") == 0)
    {
        c->type = COMMAND_CLOSE;
        c->text = string;
        c->word_size = word_size;
    } 
    else 
    {
        c->type = -1;
    }
}