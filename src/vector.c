#include "vector.h"

vector alias_list;
vector directory_stack;

//------------------- ALIAS LIST ---------------------------

char* insert_alias(vector* v, char* alias_name, char* alias_value){
    if(!v->ptr){
        v->capacity = 8;
        v->ptr = malloc(sizeof(alias) * v->capacity);
    }
    else if(v->size == v->capacity){
        v->capacity *= 2;
        v->ptr = realloc(v->ptr, sizeof(alias) * v->capacity);
    }
    alias* temp = (alias*)v->ptr;
    temp[v->size].name = strdup(alias_name);
    temp[v->size].value = strdup(alias_value);
    v->size++;
    return temp[v->size - 1].name;
}

char* find_alias(vector* v, char* alias_name){
    alias* temp = (alias*)v->ptr;
    for(int i = 0; i < v->size; i++){
        if(strcmp(temp[i].name, alias_name) == 0){
            return strdup(temp[i].value);
        }
    }
    return NULL;
}

char* remove_alias(vector* v, char* alias_name){
    alias* temp = (alias*)v->ptr;
    for(int i = 0; i < v->size; i++){
        if(strcmp(temp[i].name, alias_name) == 0){
            char* result = strdup(temp[i].name);
            free(temp[i].name);
            free(temp[i].value);
            for(int j = i; j < v->size - 1; j++){
                temp[j].name = temp[j+1].name;
                temp[j].value = temp[j+1].value;
            }
            v->size--;
            return result;
        }
    }
    return NULL;
}


int print_alias(vector* v){
    alias* temp = (alias*) v->ptr;
    for(int i = 0; i < v->size; i++){
        printf("%s='%s'\n", temp[i].name, temp[i].value);
    }
    return 0;
}

//------------ DIRECTORY sTACK ----------------------

char* push_dir(vector* v, char* directory){
    if(!v->ptr){
        v->capacity = 8;
        v->ptr = malloc(sizeof(char*) * v->capacity);
    }
    else if(v->size == v->capacity){
        v->capacity *= 2;
        v->ptr = realloc(v->ptr, sizeof(char*) * v->capacity);
    }
    char** temp = (char**)v->ptr;
    temp[v->size] = strdup(directory);
    v->size++;
    return temp[v->size - 1];
}

char* pop_dir(vector* v){
    if(v->size == 0){
        printf("popd: directory stack empty\n");
        return NULL;
    }
    char** temp = (char**) v->ptr;
    char* result = strdup(temp[v->size - 1]);
    free(temp[v->size - 1]);
    v->size--;
    return result;
}

int print_stack(vector* v){
    char** temp = (char**)v->ptr;
    for(int i = v->size - 1; i >= 0; i--){
        printf("%s\n", temp[i]);
    }
    return 0;
}