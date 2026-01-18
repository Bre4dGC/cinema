#pragma once

#include "parser.h"

typedef enum {
    EXECUTOR_INTERPRETER,
    EXECUTOR_COMPILER,
} executor_kind_t;

typedef struct {
    executor_kind_t kind;
} executor_t;

void execute(executor_t* executor, ast_t* ast);
