#include "execution.h"

void interpret();
void compile();

void execute(executor_t* executor, ast_t* ast)
{

    switch(executor->kind){
        case EXECUTOR_INTERPRETER:
            break;
        case EXECUTOR_COMPILER:
            break;
    }
}

void interpret()
{

}

void compile()
{

}