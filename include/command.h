#ifndef _COMMAND_H_
#define _COMMAND_H_

#define BUFFER_SIZE 256
#define WORD_SIZE   8

typedef enum command_type
{
	COMMAND_CREATE,
	COMMAND_DATABASE,
	COMMAND_DROP,
    COMMAND_EXIT,
    COMMAND_CLOSE,
    COMMAND_ENTITY,
    COMMAND_COMMIT,
    COMMAND_UNDEFINED,
    COMMAND_BUFFER_OVERFLOW,
    COMMAND_CONTEXT
} command_type;

typedef struct command
{
    char words[WORD_SIZE][BUFFER_SIZE];
    int word_size;
    command_type type;
} command;

void get_command(char* buffer);

void clear_buffer(char* buffer);

void clear_command(char* buffer);

void create_command(char* string, command* c);

#endif
