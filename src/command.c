#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/command.h"

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
    {;
        int i = 0, j;
        while(string[i] != ' ' && string[i] != '\0') ++i;
        char command_word[i];
        for(j = 0; j < i; ++j)
            command_word[j] = string[j];
        command_word[i] = '\0';
        if(strcmp(command_word, "CREATE") == 0)
        {
            c->type = COMMAND_CREATE;
            c->text = string;
        } 
        else if(strcmp(command_word, "READ") == 0)
        {
            c->type = COMMAND_READ;
            c->text = string;
        } 
        else if(strcmp(command_word, "UPDATE") == 0)
        {
            c->type = COMMAND_UPDATE;
            c->text = string;
        } 
        else if(strcmp(command_word, "DELETE") == 0)
        {
            c->type = COMMAND_DELETE;
            c->text = string;
        } 
        else if(strcmp(command_word, "DATABASE") == 0)
        {
            c->type = COMMAND_DATABASE;
            c->text = string;
        } 
        else if(strcmp(command_word, "DROP") == 0)
        {
            c->type = COMMAND_DROP;
            c->text = string;
        } 
        else if(strcmp(command_word, "EXIT") == 0)
        {
            c->type = COMMAND_EXIT;
            c->text = string;
        } 
        else if(strcmp(command_word, "QUIT") == 0)
        {
            c->type = COMMAND_EXIT;
            c->text = string;
        }  
        else if(strcmp(command_word, "SELECT") == 0)
        {
            c->type = COMMAND_SELECT;
            c->text = string;
        } 
        else if(strcmp(command_word, "LOGIN") == 0)
        {
            c->type = COMMAND_LOGIN;
            c->text = string;
        } 
        else if(strcmp(command_word, "LOGOUT") == 0)
        {
            c->type = COMMAND_LOGOUT;
            c->text = string;
        } 
        else if(strcmp(command_word, "NEW") == 0)
        {
            c->type = COMMAND_NEW;
            c->text = string;
        } 
        else if(strcmp(command_word, "REMOVE") == 0)
        {
            c->type = COMMAND_REMOVE;
            c->text = string;
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