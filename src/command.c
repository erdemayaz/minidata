#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/command.h"
#include "../include/string.h"
#include "../tool/miniscanner.h"

extern int yylex();
extern void yyflush();
extern void yysetscan(char *text);
extern int yylineno;
extern char* yytext;

void get_command(command *c)
{
    int ntoken;
    c->token_size = 0;
    ntoken = yylex();
    while(ntoken)
    {
        if(c->token_size < WORD_SIZE)
        {
            c->tokens[c->token_size] = ntoken;
            switch(ntoken)
            {
                case TOKEN_CHAR:
                    c->values[c->token_size][0] = yytext[1];
                    c->values[c->token_size][1] = '\0';
                    break;
                case TOKEN_FLOATNUM:
                    strcpy(c->values[c->token_size], yytext);
                    break;
                case TOKEN_INTNUM:
                    strcpy(c->values[c->token_size], yytext);
                    break;
                case TOKEN_IDENTIFIER:
                    strcpy(c->values[c->token_size], yytext);
                    break;
                case TOKEN_LETTER:
                    strcpy(c->values[c->token_size], yytext);
                    break;
                case TOKEN_STRING:
                    strcpy(c->values[c->token_size], yytext);
                    break;
            }
            c->token_size++;
        }
        else
        {
            c->type = COMMAND_BUFFER_OVERFLOW;
            yyflush();
            return;
        }
        ntoken = yylex();
    }

    switch(c->tokens[0])
    {
        case KEYWORD_CREATE:
            c->type = COMMAND_CREATE;
            break;
        case KEYWORD_ENTITY:
            c->type = COMMAND_ENTITY;
            break;
        case KEYWORD_DATABASE:
            c->type = COMMAND_DATABASE;
            break;
        case KEYWORD_COMMIT:
            c->type = COMMAND_COMMIT;
            break;
        case KEYWORD_DROP:
            c->type = COMMAND_DROP;
            break;
        case KEYWORD_EXIT:
            c->type = COMMAND_EXIT;
            break;
        case KEYWORD_QUIT:
            c->type = COMMAND_EXIT;
            break;
        case KEYWORD_CLOSE:
            c->type = COMMAND_CLOSE;
            break;
        case KEYWORD_CONTEXT:
            c->type = COMMAND_CONTEXT;
            break;
        case KEYWORD_SET:
            c->type = COMMAND_SET;
            break;
        default:
            c->type = COMMAND_UNDEFINED;
            break;
    }
}

void get_command_from_buffer(command *c, char *buffer)
{
    yyflush();
    yysetscan(buffer);
    get_command(c);
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