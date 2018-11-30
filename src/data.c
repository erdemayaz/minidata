#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/data.h"

data_unit* create_data_unit(unsigned char type, unsigned int size, void* data)
{
    data_unit *unit = (data_unit*) malloc(sizeof(data_unit));
    unit->type = (uint8_t) type;
    unit->size = (uint32_t) size;
    unit->data = data;
    return unit;
}

void write_data_unit(FILE* file, data_unit* unit)
{
    fwrite(&unit->type, 1, 1, file);
    fwrite(&unit->size, 4, 1, file);
    fwrite(unit->data, unit->size, 1, file);
}

data_unit* read_data_unit(FILE* file)
{
    data_unit *unit = (data_unit*) malloc(sizeof(data_unit));
    fread(&unit->type, 1, 1, file);
    fread(&unit->size, 4, 1, file);
    unit->data = (void*) malloc(unit->size);
    fread(unit->data, unit->size, 1, file);
    return unit;
}

void free_data_unit(data_unit* unit)
{
    // !!! ATTENTION : void *data not free !!!
    if(unit)
        free(unit);
}

void write_string_unit(FILE* file, char* string)
{
    uint32_t size = strlen(string);
    uint8_t type = TYPE_STRING;
    fwrite(&type, 1, 1, file);
    fwrite(&size, 4, 1, file);
    fwrite(string, size, 1, file);
}

char* read_string_unit(FILE* file)
{
    uint32_t size = 0;
    uint8_t type = -1;
    fread(&type, 1, 1, file);
    fread(&size, 4, 1, file);
    if(type == TYPE_STRING)
    {
        char *string = (char*) malloc(sizeof(char) * (size + 1));
        fread(string, size, 1, file);
        string[size] = '\0';
        return string;
    }
    else
    {
        fseek(file, size, SEEK_END);
        return NULL;
    }
}

void write_unsigned_integer_unit(FILE* file, uint32_t integer)
{
    uint32_t size = 4;
    uint8_t type = TYPE_NUMBER;
    fwrite(&type, 1, 1, file);
    fwrite(&size, 4, 1, file);
    fwrite(&integer, size, 1, file);
}

void write_integer_unit(FILE* file, int32_t integer)
{
    uint32_t size = 4;
    uint8_t type = TYPE_NUMBER;
    fwrite(&type, 1, 1, file);
    fwrite(&size, 4, 1, file);
    fwrite(&integer, size, 1, file);
}