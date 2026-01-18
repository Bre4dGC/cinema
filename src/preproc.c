#include <string.h>

#include "preproc.h"

#define MAX_LINE_LENGTH 1024
#define MAX_MACRO_LENGTH 256

preproc_statement_t* parse_preproc_statement(string_t* input)
{
    if(!input) return NULL;

    preproc_statement_t* statements = malloc(sizeof(preproc_statement_t));
    statements->kind = PREPROC_DEFINE;
    statements->content = malloc(MAX_MACRO_LENGTH);
    memset(statements->content, 0, MAX_MACRO_LENGTH);
    size_t i = 0;

    while(i < input->len && (input->data[i] != '\n' || input->data[i] == '\\') && i < MAX_MACRO_LENGTH - 1){
        statements->content[i] = input->data[i];
        i++;
    }
    statements->content[i] = '\0';
    return statements;
}

preproc_t* preprocess(string_t* input)
{
    preproc_t* preproc = malloc(sizeof(preproc_t));
    preproc->condition_stack = 0;
    preproc->statements = NULL;
    preproc->includes = NULL;

    while(input->len > 0){
        preproc_statement_t* statements = parse_preproc_statement(input);
        if(!statements) break;
        preproc->statements[preproc->condition_stack++] = *statements;
        free(statements);
    }
    return preproc;
}

void free_preproc(preproc_t* preproc)
{
    if(!preproc) return;
    for(size_t i = 0; i < preproc->condition_stack; ++i){
        free(preproc->statements[i].content);
    }
    free(preproc->statements);
    for(size_t i = 0; i < preproc->include_count; ++i){
        free(preproc->includes[i].content);
    }
    free(preproc->includes);
    free(preproc);
}
