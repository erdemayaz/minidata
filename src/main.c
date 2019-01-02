#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/file.h"
#include "../include/db.h"
#include "../include/command.h"
#include "../include/task.h"
#include "../include/commit.h"
#include "../include/context.h"

CTX *ctx;
DB *db;
char *db_folder = "db/";
commit_queue *queue;
int flow_status;
int flow_mode;
int runtime;

clock_t begin, end;

void sli(char *source_name)
{
	FILE *f = open_source_file(source_name);
	if(f)
	{
		char command_text[BUFFER_SIZE];
		command c;
		int command_len;
		while(fgets(command_text, BUFFER_SIZE, f) != NULL)
		{
			command_len = strlen(command_text);
			if(command_text[command_len - 1] == '\n')
				command_text[command_len - 1] = '\0';
			if(command_text[0] == '\0')
				continue; 
			clear_command(&c);
			get_command_from_buffer(&c, command_text);
			if(c.type == COMMAND_UNDEFINED)
			{
				printf("Undefined command\n");
			}
			else if(c.type == COMMAND_BUFFER_OVERFLOW)
			{
				printf("Command buffer overflow\n");
			}
			else
			{
				if(c.type == COMMAND_EXIT)
				{
					task_close(0);
					break;
				}
				perform(&c);
				if(flow_status == 0)
				{
					break;
				}
			}
		}
		fclose(f);
	}
	else
	{
		printf("File not found\n");
	}
}

void cli()
{
	command c;
	while(1)
	{
		printf("> ");
		clear_command(&c);
		get_command(&c);
		begin = clock();
		if(c.type == COMMAND_UNDEFINED)
		{
			printf("Undefined command\n");
		}
		else if(c.type == COMMAND_BUFFER_OVERFLOW)
		{
			printf("Command buffer overflow\n");
		}
		else
		{
			if(c.type == COMMAND_EXIT)
			{
				task_close(0);
				break;
			}
			perform(&c);
		}
		end = clock();
		if(runtime)
			printf("<%.3lfs>\n", ((double) (end - begin)) / CLOCKS_PER_SEC);
	}
}

int main(int argc, char* argv[]) 
{
	ctx = init_ctx();
	db = NULL;
	queue = create_commit_queue();
	flow_status = 1;
	runtime = 0;

	if(argc == 1)
	{
		flow_mode = 1;
		cli();
	}
	else if(argc == 2)
	{
		flow_mode = 2;
		begin = clock();
		sli(argv[1]);
		end = clock();
		printf("elapsed time: %.3lfs\n", ((double) (end - begin)) / CLOCKS_PER_SEC);
	}
	else
	{
		printf("Program can takes 1 optional parameter\n");
	}
	
	free_commit_queue(queue);
	destroy_ctx(ctx);
	return flow_status;
}