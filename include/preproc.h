#pragma once

#include "utils.h"

#define MAX_DEFINES 128
#define MAX_INCLUDES 128

typedef enum {
    PREPROC_DEFINE,
    PREPROC_INCLUDE,
    PREPROC_IFDEF,
    PREPROC_IFNDEF,
    PREPROC_ELSE,
    PREPROC_ENDIF,
} preproc_kind_t;

typedef enum {
    PREPROC_TOKEN_INVALID,
    PREPROC_TOKEN_IDENT,
    PREPROC_TOKEN_NUMBER,
    PREPROC_TOKEN_STRING,
    PREPROC_TOKEN_CHAR,
    PREPROC_TOKEN_PUNCT,
    PREPROC_TOKEN_NEWLINE,
    PREPROC_TOKEN_EOF,
} preproc_token_kind_t;

typedef struct {
    preproc_kind_t kind;
    char* content;
} preproc_statement_t;

typedef struct {
    preproc_statement_t statements[MAX_DEFINES];
    string_t includes[MAX_INCLUDES];
    int condition_stack;
} preproc_t;

preproc_statement_t* parse_preproc_statement(string_t* input);
preproc_t* preprocess(string_t* input);
void free_preproc(preproc_t* preproc);
