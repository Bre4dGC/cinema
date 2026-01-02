#pragma once
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    size_t pos;
    size_t ln;
    size_t col;
} loc_t;

typedef struct {
    char* data;
    size_t len;
} string_t;

string_t* read_file(char* filename);
void free_input(string_t* input);
