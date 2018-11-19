#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/command.h"
#include "../include/string.h"

char* get_command(char* buffer)
{
    printf("> ");
    fgets(buffer, BUFFER_SIZE, stdin);
    char *temp = (char*) malloc(sizeof(char) * strlen(buffer));
    strcpy(temp, buffer);
    temp[strlen(buffer) - 1] = '\0';
    return temp;
}

void clear_command(char* buffer)
{
    if(buffer)
    {
        free(buffer);
    }
}

void clear_buffer(char* buffer)
{
    if(buffer)
    {
        memset(buffer, 0, BUFFER_SIZE);
    }
}

void free_command(command* c)
{
    if(c)
    {
        free(c);
    }
}

command* create_command(char* string)
{
    command *c = (command*) malloc(sizeof(command));
    if(c)
    {
        int word_size = 0;
        char **words = split_string(string, ' ', WORD_SIZE, &word_size);
        char *command_word = words[0];
        if(strcmp(command_word, "CREATE") == 0)
        {
            c->type = COMMAND_CREATE;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "READ") == 0)
        {
            c->type = COMMAND_READ;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "UPDATE") == 0)
        {
            c->type = COMMAND_UPDATE;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "DELETE") == 0)
        {
            c->type = COMMAND_DELETE;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "DATABASE") == 0)
        {
            c->type = COMMAND_DATABASE;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "DROP") == 0)
        {
            c->type = COMMAND_DROP;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "EXIT") == 0)
        {
            c->type = COMMAND_EXIT;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "QUIT") == 0)
        {
            c->type = COMMAND_EXIT;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        }  
        else if(strcmp(command_word, "SELECT") == 0)
        {
            c->type = COMMAND_SELECT;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "LOGIN") == 0)
        {
            c->type = COMMAND_LOGIN;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "LOGOUT") == 0)
        {
            c->type = COMMAND_LOGOUT;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "NEW") == 0)
        {
            c->type = COMMAND_NEW;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        } 
        else if(strcmp(command_word, "REMOVE") == 0)
        {
            c->type = COMMAND_REMOVE;
            c->text = string;
            c->words = words;
            c->word_size = word_size;
        }
        else 
        {
            free_command(c);
            return NULL;
        }
        return c;
    } 
    else 
    {
        return NULL;
    }
}