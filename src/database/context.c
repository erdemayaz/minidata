#include <stdio.h>
#include <stdlib.h>
#include "../../include/db.h"
#include "../../include/context.h"

extern CTX *ctx;
extern DB *db;

CTX* init_ctx()
{
    CTX *c = (CTX*) malloc(sizeof(CTX));
    c->type = CTX_HOST;
    return c;
}

void destroy_ctx(CTX* c)
{
    if(c)
    {
        free(c);
    }
}

void set_ctx_host()
{
    ctx->type = CTX_HOST;
}

void set_ctx_db()
{
    ctx->type = CTX_DATABASE;
    ctx->object.db = db;
}

void set_ctx_entity(ENTITY *entity)
{
    ctx->type = CTX_ENTITY;
    ctx->object.ent = entity;
}

void set_ctx_field(FIELD *field)
{
    ctx->type = CTX_FIELD;
    ctx->object.fld = field;
}

context_t get_ctx_type()
{
    return ctx->type;
}

context_o get_ctx_object()
{
    return ctx->object;
}

void context_up()
{
    switch(ctx->type)
    {
        case CTX_HOST:
            break;
        case CTX_DATABASE:
            break;
        case CTX_ENTITY:
            ctx->type = CTX_DATABASE;
            ctx->object.db = db;
            break;
        case CTX_FIELD:

            break;
    }
}