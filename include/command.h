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
    COMMAND_CONTEXT,
    COMMAND_SET
} command_type;

typedef struct command
{
    int tokens[WORD_SIZE];
    char values[WORD_SIZE][BUFFER_SIZE];
    int token_size;
    command_type type;
} command;

void get_command(command *c);

void get_command_from_buffer(command *c, char *buffer);

void clear_buffer(char* buffer);

void clear_command(command *c);

#endif
