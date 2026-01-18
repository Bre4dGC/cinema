#include <stdalign.h>

#include "semantic.h"

type_t type_int    = {TYPE_INT,    "int",    sizeof(int),    alignof(int)};
type_t type_short  = {TYPE_SHORT,  "short",  sizeof(short),  alignof(short)};
type_t type_long   = {TYPE_LONG,   "long",   sizeof(long),   alignof(long)};
type_t type_float  = {TYPE_FLOAT,  "float",  sizeof(float),  alignof(float)};
type_t type_double = {TYPE_DOUBLE, "double", sizeof(double), alignof(double)};
type_t type_void   = {TYPE_VOID,   "void",   sizeof(void),   alignof(void)};
type_t type_char   = {TYPE_CHAR,   "char",   sizeof(char),   alignof(char)};

type_t* new_type_array(type_t* type, size_t length);
type_t* new_type_function(type_t* return_type, type_t** param_types, const size_t param_count);
type_t* new_type_compound(type_kind_t kind, char* name, type_t** member_types, const size_t member_count);
bool types_compatible(const type_t* a, const type_t* b);

symbol_table_t* new_symbol_table(void);
symbol_t* lookup_symbol(symbol_table_t* st, const char* name);
symbol_t* define_symbol(symbol_table_t* st, symbol_kind_t kind, const char* name, type_t* type);
scope_t* push_scope(scope_t* parent, scope_kind_t kind);
void pop_scope(scope_t* scope);
void free_symbol_table(symbol_table_t* st);
bool scope_symbol_exist(scope_t* scope, const char* name);

type_t* new_type_array(type_t* type, size_t length)
{
    
}

type_t* new_type_function(type_t* return_type, type_t** param_types, const size_t param_count)
{

}

type_t* new_type_compound(type_kind_t kind, char* name, type_t** member_types, const size_t member_count)
{

}

bool types_compatible(const type_t* a, const type_t* b)
{

}


symbol_table_t* new_symbol_table(void)
{

}

symbol_t* lookup_symbol(symbol_table_t* st, const char* name)
{

}

symbol_t* define_symbol(symbol_table_t* st, symbol_kind_t kind, const char* name, type_t* type)
{

}

scope_t* push_scope(scope_t* parent, scope_kind_t kind)
{

}

void pop_scope(scope_t* scope)
{

}

void free_symbol_table(symbol_table_t* st)
{

}

bool scope_symbol_exist(scope_t* scope, const char* name)
{

}

