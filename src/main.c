#include "utils.h"
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <string.h>
#include "parser.h"
#include "executor.h"

int main(int argc, char* argv[])
{
    if(argc == 2){
        FILE* file = fopen(argv[1], "r");
        if(!file){
            return EXIT_FAILURE;
        }
        char line[MAX_STRING_LENGTH];
        while(fgets(line, MAX_STRING_LENGTH, file) != NULL){
            if(line[strlen(line) - 1] == '\n'){
                line[strlen(line) - 1] = '\0';
            }
            char **tokens = lexer(line);
            Node* node = parser(tokens);
            execute(node);
            m_free(node);
            free(tokens);
        }
    }
    else{
        while(1){
            
            char *input = readline("prompt> ");
            if(!input)
                break;

            char **tokens = lexer(input);
            Node* node = parser(tokens);
            execute(node);
            m_free(node);
            free(input);
            free(tokens);
        }
    }
    exit(EXIT_SUCCESS);;
}
