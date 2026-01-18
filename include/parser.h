#pragma once
#include <stdbool.h>

#include "lexer.h"

typedef struct {
    token_t* tokens;
    size_t pos;
} parser_t;

typedef enum {
    NODE_LITER,  NODE_BLOCK,  NODE_STMT,
    NODE_EXPR,   NODE_BINOP,  NODE_UNARYOP,
    NODE_REF,    NODE_CALL,   NODE_TYPEDEF,
    NODE_VAR,    NODE_ARRAY,  NODE_FUNC,
    NODE_IF,     NODE_SWITCH, NODE_CASE,
    NODE_WHILE,  NODE_FOR,
    NODE_STRUCT, NODE_ENUM,   NODE_UNION,
    NODE_RETURN, NODE_BREAK,  NODE_CONTINUE,
} node_kind_t;

typedef struct ast ast_t;

typedef struct {
    int type;
    char* value;
} node_liter_t;

typedef struct {
    ast_t** statements;
    size_t count;
    size_t capacity;
} node_block_t;

typedef struct {
    ast_t* left;
    ast_t* right;
    int oper;
} node_binop_t;

typedef struct {
    ast_t* right;
    int operator;
    bool is_postfix;
} node_unaryop_t;

typedef struct{
    char* name;
    ast_t* value;
} node_assign_t;

typedef struct {
    char* name;
} node_ref_t;

typedef struct {
    char* name;
    ast_t** args;
    size_t arg_count;
} node_call_t;

typedef struct {
    char* name;
    int datatype;
    bool is_ptr;
} node_typedef_t;

typedef struct {
    ast_t* body;
} node_return_t;

typedef struct {
    int modif;
    int datatype;
    bool is_ptr;
    char* name;
    ast_t* value;
} node_var_t;

typedef struct {
    ast_t** elements;
    size_t count;
    size_t capacity;
} node_array_t;

typedef struct {
    ast_t* condition;
    ast_t* then_block;
    ast_t* else_block;
    ast_t* elif_blocks;
} node_if_t;

typedef struct {
    ast_t* condition;
    ast_t* body;
} node_while_t;

typedef struct {
    ast_t* init;
    ast_t* condition;
    ast_t* update;
    ast_t* body;
} node_for_t;

typedef struct {
    int modif;
    int ret_type;
    bool is_ptr;
    char* name;
    ast_t** params;
    size_t param_count;
    ast_t* body;
} node_func_t;

typedef struct {
    ast_t* condition;
    ast_t* body;
} node_case_t;

typedef struct {
    ast_t* target;
    struct {
        ast_t** nodes;
        size_t count;
        size_t capacity;
    } case_block;
} node_switch_t;

typedef struct {
    char* name;
    struct {
        ast_t** nodes;
        size_t count;
        size_t capacity;
    } member;
} node_struct_t;

typedef struct {
    char* name;
    struct {
        ast_t** nodes;
        size_t count;
        size_t capacity;
    } member;
} node_enum_t;

typedef struct {
    char* name;
    struct {
        ast_t** nodes;
        size_t count;
        size_t capacity;
    } member;
} node_union_t;

struct ast{
    node_kind_t kind;
    union {
        node_liter_t liter;
        node_block_t block;
        node_binop_t binop;
        node_unaryop_t unaryop;
        node_ref_t ref;
        node_call_t call;
        node_assign_t assign;
        node_var_t var;
        node_array_t array;
        node_func_t func;
        node_if_t if_stmt;
        node_while_t while_stmt;
        node_for_t for_stmt;
        node_switch_t switch_stmt;
        node_case_t case_stmt;
        node_struct_t struct_def;
        node_enum_t enum_def;
        node_union_t union_def;
        node_return_t return_stmt;
    } node;
};

parser_t* new_parser(token_t* tokens);
ast_t* ast_build(parser_t* parser);
