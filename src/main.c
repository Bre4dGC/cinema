#include "preproc.h"
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "execution.h"
#include "utils.h"

int main(void)
{
    string_t* input = read_file("example.c");

    preproc_t* prep = preprocess(input);

    lexer_t*  lexer  = new_lexer(input);
    token_t*  tokens = tokenize(lexer);
    parser_t* parser = new_parser(tokens);
    ast_t*    ast    = ast_build(parser);
    
    analyse(ast);
    
    executor_t* executor = new_executor(ast);

    execute(executor, ast);

    free_executor(executor);
    free_ast(ast);
    free_parser(parser);
    free_lexer(lexer);
    free_preproc(prep);
    free_input(input);
    return 0;
}
