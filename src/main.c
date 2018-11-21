#include <stdio.h>
#include <stdlib.h>
#include "../include/file.h"
#include "../include/db.h"
#include "../include/command.h"
#include "../include/task.h"

CTX *ctx;
DB *db;
char *db_folder = "db/";

void cli()
{
	char *buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE);
	char *command_text = NULL;
	command *c = NULL;
	while(1)
	{
		command_text = get_command(buffer);
		c = create_command(command_text);
		if(c != NULL)
		{
			if(c->type == COMMAND_EXIT)
			{
				task_close(0);
				break;
			}
			perform(c);
		}
		else
		{
			printf("Undefined command\n");
		}
		clear_command(command_text);
		free_command(c);
	}
	free(buffer);
}

int main(int argc, char* argv[]) 
{
	ctx = init_ctx();
	db = NULL;
	cli();
	destroy_ctx(ctx);
	return 0;
}