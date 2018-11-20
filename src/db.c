#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file.h"
#include "../include/db.h"

DB* load_database(char* name)
{
    DB *db = (DB*) malloc(sizeof(DB));
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) + 8));
    sprintf(file_name, "db/%s.mndt", name);
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
    char *file_name = (char*) malloc(sizeof(char) * (strlen(name) + 8));
    sprintf(file_name, "db/%s.mndt", name);
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