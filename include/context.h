#ifndef _CONTEXT_H_
#define _CONTEXT_H_

typedef enum context_t
{
    CTX_HOST,
    CTX_DATABASE,
    CTX_ENTITY,
    CTX_FIELD
} context_t;

typedef union context_o
{
    DB *db;
    ENTITY *ent;
    FIELD *fld;
} context_o;

typedef struct context
{
    context_t type;
    context_o object;
} CTX;

CTX* init_ctx();

void destroy_ctx(CTX* c);

void set_ctx_host();

void set_ctx_db();

void set_ctx_entity(ENTITY *entity);

void set_ctx_field(FIELD *field);

context_t get_ctx_type();

context_o get_ctx_object();

#endif
