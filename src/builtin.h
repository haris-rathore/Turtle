#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "parser.h"
#include "vector.h"

typedef enum internal{
    EXIT,
    PWD,
    CD,
    PUSHD,
    POPD,
    DIRS,
    ALIAS,
    UNALIAS,
    ECHO,
    TRUE,
    FALSE
} builtin;

extern const char* BUILTIN_COMMANDS[];

extern const int NUMBER_OF_BUILTIN_COMMANDS;

extern char buffer[MAX_STRING_LENGTH];

int exec_builtin(builtin command, char** args);

#endif