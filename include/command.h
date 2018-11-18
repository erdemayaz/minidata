#ifndef _COMMAND_H_
#define _COMMAND_H_

#define BUFFER_SIZE 256

typedef enum command_type
{
	COMMAND_CREATE,
	COMMAND_READ,
	COMMAND_UPDATE,
	COMMAND_DELETE,
	COMMAND_DATABASE,
	COMMAND_DROP,
    COMMAND_EXIT,
    COMMAND_SELECT,
    COMMAND_LOGIN,
    COMMAND_LOGOUT,
    COMMAND_NEW,
    COMMAND_REMOVE
} command_type;

typedef struct command
{
    char *text;
    command_type type;
} command;

char* get_command(char* buffer);

void clear_buffer(char* buffer);

void clear_command(char* buffer);

void free_command(command* c);

command* create_command(char* string);

#endif
