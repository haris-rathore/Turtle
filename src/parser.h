#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "vector.h"

#define MAX_STRING_LENGTH 1024

#define AND 0
#define OR 1
#define SEMICOLON 2
#define SUCCESS 0


typedef enum node_type{
    COMMAND,
    CHAIN,
    IO,
    PIPE
} Node_type;

typedef struct node{
    Node_type type;
    char* subtype;
    char** values;
    struct node* left;
    struct node* right;
} Node;

char** lexer(const char* line);

Node* parser(char** tokens);

#endif