#include "utils.h"

string_t* read_file(char* filename)
{
    FILE* source = fopen(filename, "r");
    if (!source) {
        perror("Error to open file");
        return NULL;
    }

    // Allocate memory for the input structure
    string_t* input = malloc(sizeof(string_t));
    if (!input) {
        fclose(source);
        return NULL;
    }

    fseek(source, 0, SEEK_END);
    input->len = ftell(source);
    fseek(source, 0, SEEK_SET);

    input->data = malloc(input->len + 1);
    if (!input->data) {
        free(input);
        fclose(source);
        return NULL;
    }

    fread(input->data, sizeof(char), input->len, source);
    input->data[input->len] = '\0';

    fclose(source);
    return input;
}

void free_input(string_t* input)
{
    if(!input) return;
    free(input->data);
    free(input);
}
