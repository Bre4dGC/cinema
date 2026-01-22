#pragma once

#include "utils.h"

#define MAX_DEFINES 128
#define MAX_INCLUDES 128
#define MAX_MACRO_NAME 64
#define MAX_MACRO_VALUE 256

typedef struct {
    char name[MAX_MACRO_NAME];
    char value[MAX_MACRO_VALUE];
} macro_t;

typedef struct {
    macro_t macros[MAX_DEFINES];
    size_t macro_count;
    string_t includes[MAX_INCLUDES];
    size_t include_count;
} preproc_t;

string_t* preprocess(string_t* input);
void free_preproc(preproc_t* preproc);
