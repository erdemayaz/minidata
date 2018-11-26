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
    data_unit *du = create_data_unit(TYPE_STRING, (uint32_t) strlen(string), string);
    write_data_unit(file, du);
    free_data_unit(du);
}

void write_unsigned_integer_unit(FILE* file, uint32_t integer)
{
    data_unit *du = create_data_unit(TYPE_NUMBER, 4, &integer);
    write_data_unit(file, du);
    free_data_unit(du);
}

void write_integer_unit(FILE* file, int32_t integer)
{
    data_unit *du = create_data_unit(TYPE_NUMBER, 4, &integer);
    write_data_unit(file, du);
    free_data_unit(du);
}