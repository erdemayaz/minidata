#ifndef _DATA_H_
#define _DATA_H_

typedef enum data
{
    TYPE_NULL       = 0,
    TYPE_BOOLEAN    = 1,
    TYPE_NUMBER     = 2,
    TYPE_STRING     = 3,
    TYPE_OBJECT     = 4,
    TYPE_ARRAY      = 5,
} data_t;

typedef struct data_unit
{
    uint8_t type;
    uint32_t size;
    void *data;
} data_unit;

data_unit* create_data_unit(unsigned char type, unsigned int size, void* data);

void write_data_unit(FILE* file, data_unit* unit);

data_unit* read_data_unit(FILE* file);

void free_data_unit(data_unit* unit);

void write_string_unit(FILE* file, char* string);

char* read_string_unit(FILE* file);

void write_unsigned_integer_unit(FILE* file, uint32_t integer);

uint32_t read_unsigned_integer_unit(FILE* file);

void write_integer_unit(FILE* file, int32_t integer);

int32_t read_integer_unit(FILE* file);

void write_unsigned_character_unit(FILE* file, uint8_t character);

uint8_t read_unsigned_character_unit(FILE* file);

#endif
