#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _alias{
    char* name;
    char* value;
} alias;

typedef struct dynamic_array{
    void* ptr;
    int size;
    int capacity;
} vector;

extern vector alias_list;
extern vector directory_stack;

//------------------- ALIAS LIST ---------------------------

char* insert_alias(vector* v, char* alias_name, char* alias_value);

char* find_alias(vector* v, char* alias_name);

char* remove_alias(vector* v, char* alias_name);

int print_alias(vector* v);

//------------ DIRECTORY sTACK ----------------------

char* push_dir(vector* v, char* directory);

char* pop_dir(vector* v);

int print_stack(vector* v);

#endif