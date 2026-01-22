#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "utils.h"

#define TOKEN_CAPACITY  256
#define STRING_LENGTH   1024
#define LITER_LENGTH    32

token_t tokens[] = {
    // Operators
    {"+", TOKEN_PLUS},      {"-", TOKEN_MINUS},
    {"/", TOKEN_SLASH},     {"*", TOKEN_ASTERISK},
    {"%", TOKEN_PERCENT},
    {"=", TOKEN_ASSIGN},    {"!", TOKEN_NOT},
    {"|", TOKEN_PIPE},      {"&", TOKEN_AMPER},
    {"<", TOKEN_LT},        {">", TOKEN_GT},
    {"(", TOKEN_LPAREN},    {")", TOKEN_RPAREN},
    {"{", TOKEN_LBRACE},    {"}", TOKEN_RBRACE},
    {"[", TOKEN_LBRACKET},  {"]", TOKEN_RBRACKET},
    {":", TOKEN_COLON},     {";", TOKEN_SEMICOLON},
    {".", TOKEN_DOT},       {",", TOKEN_COMMA},
    {"\"", TOKEN_DQUOTE},   {"'",  TOKEN_SQUOTE},
    {"+=", TOKEN_ADD},      {"-=", TOKEN_SUB},
    {"*=", TOKEN_MUL},      {"/=", TOKEN_DIV},
    {"%=", TOKEN_MOD},
    {"==", TOKEN_EQUAL},    {"!=", TOKEN_NOTQL},
    {"<=", TOKEN_GTQL},     {">=", TOKEN_LTQL},
    {"++", TOKEN_INCR},     {"--", TOKEN_DECR},

    // Data Types
    {"int",    TOKEN_INT},  {"char",   TOKEN_CHAR},
    {"long",   TOKEN_LONG}, {"short",  TOKEN_SHORT},
    {"float",  TOKEN_FLOAT},{"double", TOKEN_DOUBLE},
    {"void",   TOKEN_VOID},

    // Keywords
    {"if",       TOKEN_IF},     {"else",     TOKEN_ELSE},
    {"while",    TOKEN_WHILE},  {"for",      TOKEN_FOR},
    {"switch",   TOKEN_SWITCH}, {"case",     TOKEN_CASE},
    {"struct",   TOKEN_STRUCT}, {"enum",     TOKEN_ENUM},
    {"union",    TOKEN_UNION},  {"typedef",  TOKEN_TYPEDEF},
    {"return",   TOKEN_RETURN}, {"continue", TOKEN_CONTINUE},
    {"break",    TOKEN_BREAK},  {"default",  TOKEN_DEFAULT},
    {"signed",   TOKEN_SIGNED}, {"unsigned", TOKEN_UNSIGNED},
    {"const",    TOKEN_CONST},  {"static",   TOKEN_STATIC},
    {"do",       TOKEN_DO},

    // Special
    {"sizeof", TOKEN_SIZEOF}, {"alignof", TOKEN_ALIGNOF},
    {"nameof", TOKEN_NAMEOF}, {"typeof", TOKEN_TYPEOF},
};

lexer_t* new_lexer(string_t* input)
{
    lexer_t* lexer = malloc(sizeof(lexer_t));
    lexer->ch = input->data[0];
    lexer->loc = (loc_t){0, 1, 1};
    lexer->input = input;
    lexer->tokens = malloc(sizeof(token_t) * TOKEN_CAPACITY);
    return lexer;
}

void skip_wspace(lexer_t* lexer);
void skip_char(lexer_t *lexer);
token_t token_operator(lexer_t* lexer);
token_t token_string(lexer_t* lexer);
token_t token_literal(lexer_t* lexer);

token_t* tokenize(lexer_t* lexer)
{
    token_t* tokens = malloc(sizeof(token_t) * TOKEN_CAPACITY);
    if(!tokens) return NULL;

    size_t i = 0;
    for(; lexer->ch != '\0'; ++i){
        skip_wspace(lexer);

        switch(lexer->ch){
            case '+': case '-':
            case '*': case '/':
            case '%':
            case '=': case '!':
            case '<': case '>':
            case '(': case ')':
            case '{': case '}':
            case '[': case ']':
            case ':': case ';':
            case '.': case ',':
            case '|': case '&':
                tokens[i] = token_operator(lexer);
                break;
            case '"': case '\'':
                tokens[i] = token_string(lexer);
                break;
            default:
                if(isalnum(lexer->ch)) tokens[i] = token_literal(lexer);
                else {
                    fprintf(stderr, "[Error] Illegal character: %c\n", lexer->ch);
                    char* illegal = malloc(8);
                    if(illegal) strcpy(illegal, "ILLEGAL");
                    tokens[i] = (token_t){illegal, TOKEN_ILLEGAL};
                }
                break;
        }

        #ifdef DEBUG
        printf("Token(%s, %u)\n", tokens[i].liter ? tokens[i].liter : "(null)", tokens[i].kind);
        #endif
    }
    if(i < TOKEN_CAPACITY) {
        tokens[i] = (token_t){"EOF", TOKEN_EOF};
    }
    
    return tokens;
}

void skip_char(lexer_t *lexer)
{
    if(!lexer || !lexer->input->data) return;

    int nextpos = lexer->loc.pos + 1;
    lexer->loc.pos = nextpos;
    if(lexer->loc.pos >= lexer->input->len){
        lexer->ch = 0;
    }
    else {
        lexer->ch = lexer->input->data[lexer->loc.pos];
        lexer->loc.col++;
    }
}

void skip_wspace(lexer_t* lexer)
{
    while(true){
        switch(lexer->ch){
            case '\r': case '\t': case ' ':
                skip_char(lexer);
                break;
            case '\n':
                skip_char(lexer);
                lexer->loc.ln++;
                lexer->loc.col = 1;
                break;
            default: return;
        }
    }
}

token_t token_operator(lexer_t* lexer)
{
    if(!lexer) return (token_t){NULL, TOKEN_ILLEGAL};

    char op[3] = {0};
    op[0] = lexer->ch;
    skip_char(lexer);
    
    if(lexer->ch == '=' || (op[0] == lexer->ch && (op[0] == '+' || op[0] == '-' ))){
        op[1] = lexer->ch;
        skip_char(lexer);
    }
    op[strlen(op)] = '\0';

    size_t op_len = strlen(op) + 1;
    char* op_str = malloc(op_len);
    if(!op_str) return (token_t){NULL, TOKEN_ILLEGAL};
    strcpy(op_str, op);

    size_t size = sizeof(tokens) / sizeof(token_t);
    for(size_t i = 0; i < size; ++i){
        if(strcmp(op, tokens[i].liter) == 0){
            return (token_t){op_str, tokens[i].kind};
        }
    }
    return (token_t){op_str, TOKEN_ILLEGAL};
}

token_t token_string(lexer_t* lexer)
{
    if(!lexer) return (token_t){NULL, TOKEN_ILLEGAL};
    skip_char(lexer);

    char str_stack[STRING_LENGTH] = {0};
    for(int i = 0; lexer->ch != '"' && lexer->ch != '\'' && lexer->ch != '\0'; ++i){
        if(i >= STRING_LENGTH){
            fprintf(stderr, "[Error] Out of string size\n");
            char* illegal = malloc(8);
            if(illegal) strcpy(illegal, "ILLEGAL");
            return (token_t){illegal, TOKEN_ILLEGAL};
        }
        str_stack[i] = lexer->ch;
        skip_char(lexer);
    }
    skip_char(lexer);

    size_t str_len = strlen(str_stack) + 1;
    char* str = malloc(str_len);
    if(!str) return (token_t){NULL, TOKEN_ILLEGAL};
    strcpy(str, str_stack);

    return (token_t){str, TOKEN_STRING};
}

token_t token_literal(lexer_t* lexer)
{
    if(!lexer) return (token_t){NULL, TOKEN_ILLEGAL};

    char liter[LITER_LENGTH] = {0};
    token_kind_t kind = 0;

    if(isdigit(lexer->ch))      kind = TOKEN_NUMBER;
    else if(isalpha(lexer->ch)) kind = TOKEN_IDENT;

    for(int i = 0; isalnum(lexer->ch); ++i){
        if(i >= LITER_LENGTH){
            fprintf(stderr, "[Error] Out of literal size\n");
            char* illegal = malloc(8);
            if(illegal) strcpy(illegal, "ILLEGAL");
            return (token_t){illegal, TOKEN_ILLEGAL};
        }
        if(kind == TOKEN_NUMBER && !isdigit(lexer->ch)){
            fprintf(stderr, "[Error] Invalid number literal\n");
            char* illegal = malloc(8);
            if(illegal) strcpy(illegal, "ILLEGAL");
            return (token_t){illegal, TOKEN_ILLEGAL};
        }
        liter[i] = lexer->ch;
        skip_char(lexer);
    }
    liter[strlen(liter)] = '\0';

    if(kind == TOKEN_IDENT){
        size_t size = sizeof(tokens) / sizeof(token_t);
        for(size_t i = 0; i < size; ++i){
            if(strcmp(liter, tokens[i].liter) == 0){
                kind = tokens[i].kind;
                break;
            }
        }
    }

    size_t liter_len = strlen(liter) + 1;
    char* liter_str = malloc(liter_len);
    if(!liter_str) return (token_t){NULL, TOKEN_ILLEGAL};
    strcpy(liter_str, liter);

    return (token_t){liter_str, kind};
}

void free_lexer(lexer_t* lexer)
{
    if(!lexer) return;
    if(lexer->tokens) {
        free(lexer->tokens);
    }
    free(lexer);
}

void free_tokens(token_t* tokens)
{
    if(!tokens) return;
    for(size_t i = 0; i < TOKEN_CAPACITY; ++i) {
        if(tokens[i].kind == TOKEN_EOF) break;
        if(tokens[i].liter) {
            free(tokens[i].liter);
        }
    }
    free(tokens);
}
