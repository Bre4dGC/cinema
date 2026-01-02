#pragma once

#include "utils.h"

typedef enum {
    // Service
    TOKEN_ILLEGAL, TOKEN_EOF,

    // Literal
    TOKEN_IDENT, TOKEN_NUMBER, TOKEN_STRING,

    // Operators
    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_SLASH,    // /
    TOKEN_PERCENT,  // %
    TOKEN_ASTERISK, // *
    TOKEN_ASSIGN,   // =
    TOKEN_NOT,      // !
    TOKEN_PIPE,     // |
    TOKEN_AMPER,    // &
    TOKEN_LT,       // <
    TOKEN_GT,       // >
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_LBRACE,   // {
    TOKEN_RBRACE,   // }
    TOKEN_LBRACKET, // [
    TOKEN_RBRACKET, // ]
    TOKEN_COLON,    // :
    TOKEN_SEMICOLON,// ;
    TOKEN_DOT,      // .
    TOKEN_COMMA,    // ,
    TOKEN_DQUOTE,   // "
    TOKEN_SQUOTE,   // '
    TOKEN_ADD,      // +=
    TOKEN_SUB,      // -=
    TOKEN_MUL,      // *=
    TOKEN_DIV,      // /=
    TOKEN_MOD,      // %=
    TOKEN_EQUAL,    // ==
    TOKEN_NOTQL,    // !=
    TOKEN_GTQL,     // <=
    TOKEN_LTQL,     // >=
    TOKEN_INCR,     // ++
    TOKEN_DECR,     // --

    // Data Types
    TOKEN_INT,
    TOKEN_SHORT,
    TOKEN_LONG,
    TOKEN_CHAR,
    TOKEN_FLOAT,
    TOKEN_DOUBLE,
    TOKEN_VOID,

    // Keywords
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_FOR,
    TOKEN_SWITCH,
    TOKEN_CASE,
    TOKEN_STRUCT,
    TOKEN_ENUM,
    TOKEN_UNION,
    TOKEN_TYPEDEF,
    TOKEN_RETURN,
    TOKEN_CONTINUE,
    TOKEN_BREAK,
    TOKEN_DEFAULT,
    TOKEN_SIGNED,
    TOKEN_UNSIGNED,
    TOKEN_CONST,
    TOKEN_STATIC,

    // Special
    TOKEN_SIZEOF,
    TOKEN_ALIGNOF,
    TOKEN_NAMEOF,
    TOKEN_TYPEOF,
} token_kind_t;

typedef struct {
    char* liter;
    token_kind_t kind;
} token_t;

typedef struct {
    char ch;
    loc_t loc;
    string_t* input;
    token_t* tokens;
} lexer_t;

lexer_t* new_lexer(string_t* input);
token_t* tokenize(lexer_t* lexer);
void free_lexer(lexer_t* lexer);
