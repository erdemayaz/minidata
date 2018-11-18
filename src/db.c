#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file.h"
#include "../include/db.h"

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