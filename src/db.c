#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file.h"
#include "../include/db.h"

char* get_database_path(char* name)
{
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) + 8));
    sprintf(file_name, "db/%s.mndt", name);
    return file_name;
}

DB* load_database(char* name)
{
    DB *db = (DB*) malloc(sizeof(DB));
    char *file_name = get_database_path(name);
    if(exist_file(file_name))
    {
        FILE *f = open_file(file_name);
        free(file_name);
        if(f != NULL)
        {
            db->file = f;
            return db;
        }
        else
        {
            free(db);
            return NULL;
        }
    }
    else
    {
        free(db);
        free(file_name);
		return NULL;
    }
}

int create_database(char* name)
{
    char *file_name = get_database_path(name);
    FILE *f = create_file(file_name);
    free(file_name);
    if(f)
    {
        close_file(f);
        return 1;
    } else {
        return 0;
    }
}

int drop_database(char* name)
{
    char *file_name = get_database_path(name);
    if(exist_file(file_name))
    {
        if(remove(file_name) == 0)
        {
            free(file_name);
            return 1;
        }
        else
        {
            free(file_name);
            return 0;
        }
    }
    else
    {
        free(file_name);
        return 0;
    }
}