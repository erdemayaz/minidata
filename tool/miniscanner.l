%{
#include <stdio.h>
#include "miniscanner.h"
%}

%%
"create"    { return KEYWORD_CREATE; }
"database"  { return KEYWORD_DATABASE; }
"drop"      { return KEYWORD_DROP; }
"exit"      { return KEYWORD_EXIT; }
"quit"      { return KEYWORD_QUIT; }
"close"     { return KEYWORD_CLOSE; }
"entity"    { return KEYWORD_ENTITY; }
"entities"  { return KEYWORD_ENTITIES; }
"commit"    { return KEYWORD_COMMIT; }
"context"   { return KEYWORD_CONTEXT; }
"set"       { return KEYWORD_SET; }
"field"     { return KEYWORD_FIELD; }
"fields"    { return KEYWORD_FIELDS; }
"string"    { return KEYWORD_STRING; }
"number"    { return KEYWORD_NUMBER; }
"object"    { return KEYWORD_OBJECT; }
"array"     { return KEYWORD_ARRAY; }
"boolean"   { return KEYWORD_BOOLEAN; }
"null"      { return KEYWORD_NULL; }
"count"     { return KEYWORD_COUNT; }
"up"        { return KEYWORD_UP; }
"runtime"   { return KEYWORD_RUNTIME; }
"on"        { return KEYWORD_ON; }
"off"       { return KEYWORD_OFF; }
"charset"   { return KEYWORD_CHARSET; }

[+-]?[0-9]+						{ return TOKEN_INTNUM; }
[+-]?[0-9]+([.][0-9]+)?			{ return TOKEN_FLOATNUM; }

L?'(\\.|[^\\'])'				{ return TOKEN_CHAR; }
L?\"(\\.|[^\\"])*\"	            { return(TOKEN_STRING); }

[_a-zA-Z][_a-zA-Z0-9]{0,100}	{ return TOKEN_IDENTIFIER; }
[a-zA-Z]+						{ return TOKEN_LETTER; }

[\n]        { return 0; }
[ \t\v\f]	{ ; }
.           { ; }
%%

int yywrap()
{
    return 1;
}