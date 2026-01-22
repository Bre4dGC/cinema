#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "preproc.h"

#define MAX_LINE_LENGTH 4096

static void skip_wspace(string_t* input, size_t* pos)
{
    while(*pos < input->len && isspace(input->data[*pos]) && input->data[*pos] != '\n'){
        (*pos)++;
    }
}

static void skip_line(string_t* input, size_t* pos)
{
    while(*pos < input->len && input->data[*pos] != '\n'){
        (*pos)++;
    }
    if(*pos < input->len && input->data[*pos] == '\n'){
        (*pos)++;
    }
}

static int is_ident_char(char c)
{
    return isalnum(c) || c == '_';
}

static size_t read_ident(string_t* input, size_t* pos, char* buf, size_t buf_size)
{
    size_t start = *pos;
    size_t len = 0;
    
    if(*pos < input->len && is_ident_char(input->data[*pos])){
        while(*pos < input->len && is_ident_char(input->data[*pos]) && len < buf_size - 1){
            buf[len++] = input->data[*pos];
            (*pos)++;
        }
    }
    buf[len] = '\0';
    return len;
}

static int parse_define(string_t* input, size_t* pos, macro_t* macro)
{
    skip_wspace(input, pos);

    if(read_ident(input, pos, macro->name, MAX_MACRO_NAME) == 0){
        return 0;
    }
    
    skip_wspace(input, pos);

    size_t value_start = *pos;
    size_t value_len = 0;
    
    while(*pos < input->len && input->data[*pos] != '\n' && value_len < MAX_MACRO_VALUE - 1){
        if(input->data[*pos] == '\\' && *pos + 1 < input->len && input->data[*pos + 1] == '\n'){
            (*pos) += 2;
            continue;
        }
        macro->value[value_len++] = input->data[*pos];
        (*pos)++;
    }

    while(value_len > 0 && isspace(macro->value[value_len - 1])){
        value_len--;
    }
    macro->value[value_len] = '\0';
    
    skip_line(input, pos);
    return 1;
}

static int parse_include(string_t* input, size_t* pos, char* filename, size_t filename_size)
{
    skip_wspace(input, pos);
    
    if(*pos >= input->len) return 0;
    
    // #include <file> or #include "file"
    char quote_char = '\0';
    if(input->data[*pos] == '<'){
        quote_char = '>';
    } else if(input->data[*pos] == '"'){
        quote_char = '"';
    } else {
        return 0;
    }
    
    (*pos)++;
    size_t filename_len = 0;
    
    while(*pos < input->len && input->data[*pos] != quote_char && filename_len < filename_size - 1){
        filename[filename_len++] = input->data[*pos];
        (*pos)++;
    }
    
    if(*pos < input->len && input->data[*pos] == quote_char){
        (*pos)++;
    }
    
    filename[filename_len] = '\0';
    skip_line(input, pos);
    return 1;
}

static int check_condition(string_t* input, size_t* pos, macro_t* macros, size_t macro_count, int* cond_active)
{
    skip_wspace(input, pos);
    
    char ident[MAX_MACRO_NAME];
    if(read_ident(input, pos, ident, MAX_MACRO_NAME) == 0){
        return 0;
    }
    
    // if macro is defined
    int found = 0;
    for(size_t i = 0; i < macro_count; i++){
        if(strcmp(macros[i].name, ident) == 0){
            found = 1;
            break;
        }
    }
    
    *cond_active = found;
    skip_line(input, pos);
    return 1;
}

static int expand_macro(string_t* input, size_t* pos, macro_t* macros, size_t macro_count, 
                        char* output, size_t* output_pos, size_t output_size)
{
    char ident[MAX_MACRO_NAME];
    size_t start_pos = *pos;
    
    if(read_ident(input, pos, ident, MAX_MACRO_NAME) == 0){
        return 0;
    }

    // if ident is a macro
    for(size_t i = 0; i < macro_count; i++){
        if(strcmp(macros[i].name, ident) == 0){
            size_t value_len = strlen(macros[i].value);
            if(*output_pos + value_len < output_size){
                memcpy(output + *output_pos, macros[i].value, value_len);
                *output_pos += value_len;
            }
            return 1;
        }
    }

    *pos = start_pos;
    return 0;
}

static string_t* process_input(string_t* input, macro_t* macros, size_t macro_count)
{
    size_t output_size = input->len * 2;
    char* output = malloc(output_size);
    if(!output) return NULL;
    
    size_t output_pos = 0;
    size_t pos = 0;
    int in_cond = 0;
    int cond_active = 1;
    
    while(pos < input->len){
        while(pos < input->len && isspace(input->data[pos]) && input->data[pos] != '\n'){
            if(cond_active && output_pos < output_size - 1){
                output[output_pos++] = input->data[pos];
            }
            pos++;
        }
        
        if(pos >= input->len) break;

        if(input->data[pos] == '#'){
            size_t directive_start = pos;
            pos++; // skip '#'
            
            skip_wspace(input, &pos);

            if(pos + 6 < input->len && strncmp(input->data + pos, "define", 6) == 0 && 
                (pos + 6 >= input->len || !is_ident_char(input->data[pos + 6]))){
                pos += 6;
                skip_line(input, &pos);
                continue;
            }
            else if(pos + 7 < input->len && strncmp(input->data + pos, "include", 7) == 0 
                && (pos + 7 >= input->len || !is_ident_char(input->data[pos + 7]))){
                pos += 7;
                char filename[256];
                if(parse_include(input, &pos, filename, sizeof(filename))){
                    // skip includes (could be expanded later)
                }
                continue;
            }
            else if(pos + 5 < input->len && strncmp(input->data + pos, "ifdef", 5) == 0 
                && (pos + 5 >= input->len || !is_ident_char(input->data[pos + 5]))){
                pos += 5;
                in_cond = 1;
                check_condition(input, &pos, macros, macro_count, &cond_active);
                continue;
            }
            else if(pos + 6 < input->len && strncmp(input->data + pos, "ifndef", 6) == 0
                && (pos + 6 >= input->len || !is_ident_char(input->data[pos + 6]))){
                pos += 6;
                in_cond = 1;
                int temp_active;
                check_condition(input, &pos, macros, macro_count, &temp_active);
                cond_active = !temp_active;
                continue;
            }
            else if(pos + 4 < input->len && strncmp(input->data + pos, "else", 4) == 0
                && (pos + 4 >= input->len || !is_ident_char(input->data[pos + 4]))){
                pos += 4;
                if(in_cond){
                    cond_active = !cond_active;
                }
                skip_line(input, &pos);
                continue;
            }
            else if(pos + 5 < input->len && strncmp(input->data + pos, "endif", 5) == 0 
                && (pos + 5 >= input->len || !is_ident_char(input->data[pos + 5]))){
                pos += 5;
                in_cond = 0;
                cond_active = 1;
                skip_line(input, &pos);
                continue;
            }
            else {
                // unknown directive, restore position and continue normally
                pos = directive_start;
            }
        }

        if(cond_active){
            if(!expand_macro(input, &pos, macros, macro_count, output, &output_pos, output_size)){
                if(output_pos < output_size - 1){
                    output[output_pos++] = input->data[pos++];
                }
                else {
                    pos++;
                }
            }
        } else {
            if(input->data[pos] == '\n'){
                if(output_pos < output_size - 1){
                    output[output_pos++] = '\n';
                }
            }
            pos++;
        }
    }
    
    output[output_pos] = '\0';
    
    string_t* result = malloc(sizeof(string_t));
    if(!result){
        free(output);
        return NULL;
    }
    
    result->data = output;
    result->len = output_pos;
    
    return result;
}

string_t* preprocess(string_t* input)
{
    if(!input) return NULL;

    macro_t macros[MAX_DEFINES] = {0};
    size_t macro_count = 0;
    
    size_t pos = 0;
    int in_cond = 0;
    int cond_active = 1;
    
    // collect all macros
    while(pos < input->len){
        while(pos < input->len && isspace(input->data[pos]) && input->data[pos] != '\n'){
            pos++;
        }
        
        if(pos >= input->len) break;
        
        if(input->data[pos] == '#'){
            size_t directive_start = pos;
            pos++;
            skip_wspace(input, &pos);
            
            if(pos + 6 < input->len && strncmp(input->data + pos, "define", 6) == 0 &&
                (pos + 6 >= input->len || !is_ident_char(input->data[pos + 6]))){
                pos += 6;
                macro_t macro = {0};
                if(parse_define(input, &pos, &macro)){
                    int found = 0;
                    for(size_t i = 0; i < macro_count; i++){
                        if(strcmp(macros[i].name, macro.name) == 0){
                            found = 1;
                            break;
                        }
                    }
                    if(!found && macro_count < MAX_DEFINES){
                        macros[macro_count++] = macro;
                    }
                }
                continue;
            }
            else if(pos + 5 < input->len && strncmp(input->data + pos, "ifdef", 5) == 0
                && (pos + 5 >= input->len || !is_ident_char(input->data[pos + 5]))){
                pos += 5;
                in_cond = 1;
                check_condition(input, &pos, macros, macro_count, &cond_active);
                continue;
            }
            else if(pos + 6 < input->len && strncmp(input->data + pos, "ifndef", 6) == 0
                && (pos + 6 >= input->len || !is_ident_char(input->data[pos + 6]))){
                pos += 6;
                in_cond = 1;
                int temp_active;
                check_condition(input, &pos, macros, macro_count, &temp_active);
                cond_active = !temp_active;
                continue;
            }
            else if(pos + 4 < input->len && strncmp(input->data + pos, "else", 4) == 0 
                && (pos + 4 >= input->len || !is_ident_char(input->data[pos + 4]))){
                pos += 4;
                if(in_cond){
                    cond_active = !cond_active;
                }
                skip_line(input, &pos);
                continue;
            }
            else if(pos + 5 < input->len && strncmp(input->data + pos, "endif", 5) == 0 
                && (pos + 5 >= input->len || !is_ident_char(input->data[pos + 5]))){
                pos += 5;
                in_cond = 0;
                cond_active = 1;
                skip_line(input, &pos);
                continue;
            }
        }
        pos++;
    }
    return process_input(input, macros, macro_count);
}

void free_preproc(preproc_t* preproc)
{
    if(!preproc) return;
    for(size_t i = 0; i < preproc->include_count; i++){
        free(preproc->includes[i].data);
    }
    free(preproc);
}
