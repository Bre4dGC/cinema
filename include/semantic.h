#pragma once
#include <stddef.h>
#include <stdbool.h>

#include "parser.h"

typedef enum {
    TYPE_ERROR,
    TYPE_UNKNOWN,
    TYPE_INT,
    TYPE_SHORT,
    TYPE_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_VOID,
    TYPE_ARRAY,
    TYPE_STRUCT,
    TYPE_ENUM,
    TYPE_UNION,
} type_kind_t;

typedef struct {
    type_kind_t kind;
    char* name;
    size_t size;
    size_t align;
} type_t;

extern type_t type_int;
extern type_t type_short;
extern type_t type_long;
extern type_t type_float;
extern type_t type_double;
extern type_t type_void;
extern type_t type_char;

typedef enum {
    SYMBOL_VAR,
    SYMBOL_FUNC,
    SYMBOL_TYPEDEF,
    SYMBOL_ENUM,
    SYMBOL_STRUCT,
    SYMBOL_UNION
} symbol_kind_t;

typedef struct {
    symbol_kind_t kind;
    char* name;
    type_t* type;
    
    union {
        struct {
            type_t* type;
            size_t length;
        } array;
        
        struct {
            type_t* return_type;
            struct {
                type_t** params;
                size_t param_count;
            } signature;
        } func;
        
        struct {
            type_t** members;
            size_t member_count;
        } compound;
    };
} symbol_t;

typedef struct {
    symbol_t** symbols;
    size_t count;
    size_t capacity;
} symbol_table_t;

typedef enum {
    SCOPE_GLOBAL,
    SCOPE_LOCAL,
    SCOPE_FUNCTION,
    SCOPE_BLOCK
} scope_kind_t;

typedef struct scope {
    scope_kind_t kind;
    struct scope* parent;
    symbol_table_t* sym_table;
} scope_t;

typedef struct {

} semantic_t;

void analyse(ast_t* ast);