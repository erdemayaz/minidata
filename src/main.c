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
	char command_text[BUFFER_SIZE];
	command c;
	while(1)
	{
		get_command(command_text);
		if(command_text[0] == '\0')
			continue;
		create_command(command_text, &c);
		if(c.type != -1)
		{
			if(c.type == COMMAND_EXIT)
			{
				task_close(0);
				break;
			}
			perform(&c);
		}
		else
		{
			printf("Undefined command\n");
		}
	}
}

int main(int argc, char* argv[]) 
{
	ctx = init_ctx();
	db = NULL;
	cli();
	destroy_ctx(ctx);
	return 0;
}