#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

FILE* open_file(char* path) 
{
	if(path != NULL) 
	{
		FILE *f = fopen(path, "ab+");
		if(f != NULL)
		{
			return f;
		}
	}
	return NULL;
}

FILE* open_file_write(char* path)
{
	if(path != NULL) 
	{
		FILE *f = fopen(path, "wb");
		if(f != NULL)
		{
			return f;
		}
	}
	return NULL;
}

char* read_file(char * path) 
{
	char *string = NULL;
	long length = 0;
	
	if(path != NULL) 
	{
		FILE *f = fopen(path, "r");
		if(f != NULL)
    	{
      		fseek(f, 0, SEEK_END);
      		length = ftell (f);
      		fseek(f, 0, SEEK_SET);
      		string = (char*) malloc((length + 1) * sizeof(char));
      		if(string != NULL)
      		{
        		fread(string, sizeof(char), length, f);
      		}
      		fclose (f);
    	}
    	string[length] = '\0';
	}
	return string;
}

void close_file(FILE* file)
{
	if(file != NULL)
		fclose(file);
}

FILE* create_file(char* name, int* status)
{
	FILE *f = fopen(name, "rb+");
	if(f == NULL)
	{
		f = fopen(name, "wb");
		if(f != NULL)
		{
			return f;
		}
		else
		{
			*status = 2;
			return NULL;
		}
	} 
	else 
	{
		close_file(f);
		*status = 1;
		return NULL;
	}
}

int exist_file(char* path)
{
	FILE *f = fopen(path, "rb+");
	if(f)
	{
		fclose(f);
		return 1;
	}
	else
	{
		return 0;
	}
}

int exist_dir(char* dir)
{
	struct stat info;
	if(stat(dir, &info) != 0)
		return 0;
	else if(info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}