#include <stdlib.h>

#include "parser.h"

parser_t* new_parser(token_t* tokens)
{
    parser_t* parser = malloc(sizeof(parser_t));
    parser->tokens = tokens;
    parser->pos = 0;
    return parser;
}

ast_t* new_node(const node_kind_t kind)
{
    ast_t* node = malloc(sizeof(ast_t));
    node->kind = kind;
    return node;
}

ast_t* parse_expr(parser_t* parser);
ast_t* parse_block(parser_t* parser);
ast_t* parse_statement(parser_t* parser);
ast_t* parse_var(parser_t* parser);
ast_t* parse_func(parser_t* parser);
ast_t* parse_struct(parser_t* parser);
ast_t* parse_enum(parser_t* parser);
ast_t* parse_union(parser_t* parser);
ast_t* parse_array(parser_t* parser);
ast_t* parse_switch(parser_t* parser);
ast_t* parse_while(parser_t* parser);
ast_t* parse_for(parser_t* parser);

ast_t* ast_build(parser_t* parser)
{
    if(!parser) return NULL;
    
    ast_t* ast = new_node(NODE_BLOCK);
    ast->node.block.statements = NULL;
    ast->node.block.count = 0;
    ast->node.block.capacity = 0;
    
    while(parser->tokens->kind != TOKEN_EOF){
        
    }

    return ast;
}

ast_t* parse_expr(parser_t* parser)
{

    return NULL;
}

ast_t* parse_block(parser_t* parser)
{
    if(parser->tokens[parser->pos].kind != TOKEN_LBRACE){
        return NULL;
    }
    parser->pos++;

    while(parser->tokens[parser->pos].kind != TOKEN_RBRACE){
        
    }
    parser->pos++;
    return NULL;
}

ast_t* parse_statement(parser_t* parser)
{

    return NULL;
}

bool is_modif(int token_kind)
{
    switch(token_kind){
        case TOKEN_CONST:
        case TOKEN_STATIC:
            return true;
    }
    return false;
}

bool is_datatype(int token_kind)
{
    switch(token_kind){
        case TOKEN_INT:
        case TOKEN_SHORT:
        case TOKEN_LONG:
        case TOKEN_FLOAT:
        case TOKEN_DOUBLE:
        case TOKEN_CHAR:
            return true;
    }
    return false;
}

ast_t* parse_var(parser_t* parser)
{
    int modif = 0;
    if(is_modif(parser->tokens[parser->pos].kind)){
        modif = parser->tokens[parser->pos].kind;
        parser->pos++;
    }

    int datatype = parser->tokens[parser->pos].kind;
    if(!is_datatype(datatype)) return NULL;
    parser->pos++;

    bool is_ptr = false;
    if(parser->tokens[parser->pos].kind == TOKEN_ASTERISK){
        is_ptr = true;
        parser->pos++;
    }

    if(parser->tokens[parser->pos].kind != TOKEN_IDENT){
        return NULL;
    }
    char* name = parser->tokens[parser->pos].liter;
    parser->pos++;

    ast_t* value = NULL;
    if(parser->tokens[parser->pos].kind == TOKEN_ASSIGN){
        parser->pos++;
        value = parse_expr(parser);
    }

    ast_t* ast = new_node(NODE_VAR);
    ast->node.var.modif = modif;
    ast->node.var.datatype = datatype;
    ast->node.var.is_ptr = is_ptr;
    ast->node.var.name = name;
    ast->node.var.value = value;
    
    return ast;
}

ast_t* parse_func(parser_t* parser)
{
    int modif = 0;
    if(is_modif(parser->tokens[parser->pos].kind)){
        modif = parser->tokens[parser->pos].kind;
        parser->pos++;
    }

    int datatype = parser->tokens[parser->pos].kind;
    if(!is_datatype(datatype)) return NULL;
    parser->pos++;

    bool is_ptr = false;
    if(parser->tokens[parser->pos].kind == TOKEN_ASTERISK){
        is_ptr = true;
        parser->pos++;
    }

    if(parser->tokens[parser->pos].kind != TOKEN_IDENT){
        return NULL;
    }
    char* name = parser->tokens[parser->pos].liter;
    parser->pos++;

    ast_t** params = parse_statement(parser);
    size_t param_count = 0;

    ast_t* body = parse_block(parser);

    ast_t* ast = new_node(NODE_FUNC);
    ast->node.func.modif = modif;
    ast->node.func.ret_type = datatype;
    ast->node.func.is_ptr = is_ptr;
    ast->node.func.name = name;
    ast->node.func.params = params;
    ast->node.func.param_count = param_count;
    ast->node.func.body = body;

    return ast;
}

ast_t* parse_struct(parser_t* parser)
{

}

ast_t* parse_enum(parser_t* parser)
{

}

ast_t* parse_union(parser_t* parser)
{

}

ast_t* parse_array(parser_t* parser)
{

}

ast_t* parse_switch(parser_t* parser)
{

}

ast_t* parse_while(parser_t* parser)
{

}

ast_t* parse_for(parser_t* parser)
{

}

void free_parser(parser_t* parser)
{
    if(!parser) return;
    free_tokens(parser->tokens);
    free(parser);
}
