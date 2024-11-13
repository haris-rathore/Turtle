#include "parser.h"

char** lexer(const char* line){
    int i = 0;
    int indices_count = 0;
    int indices[MAX_STRING_LENGTH];
    while(line[i] == ' '){
        i++;
    }
    while(line[i] != '\0'){
        if(line[i] == '\''){
            indices[indices_count] = i;
            indices_count++;
            while(line[++i] != '\''){}
            i++;
            indices[indices_count] = i;
            indices_count++;
        }
        else if(line[i] == '\"'){
            indices[indices_count] = i;
            indices_count++;
            while(line[++i] != '\"'){}
            i++;
            indices[indices_count] = i;
            indices_count++;
        }
        else{
            indices[indices_count] = i;
            indices_count++;
            do{
                i++;
            }
            while(line[i] != ' ' && line[i] != '\0');
            indices[indices_count] = i;
            indices_count++;
        }
        while(line[i] == ' '){
            i++;
        }
    }
    if(indices_count == 0){
        return NULL;
    }
    char** result = (char**) malloc((sizeof(char *) * (indices_count/2 + 1))); // +1 for NULL
    for(int i = 0; i < indices_count; i+=2){
        if(line[indices[i]] == '\'' || line[indices[i]] == '\"'){
            result[i/2] = (char*) malloc(sizeof(char) * (indices[i+1] - indices[i] - 1));
            for(int j = indices[i] + 1; j < indices[i+1] - 1; j++){
                result[i/2][j - indices[i] - 1] = line[j];
            }
            result[i/2][indices[i+1] - indices[i] - 2] = '\0';
        }
        else{
            result[i/2] = (char*)malloc(sizeof(char) * (indices[i+1] - indices[i] + 1));
            for(int j = indices[i]; j < indices[i+1]; j++){
                result[i/2][j - indices[i]] = line[j];
            }
            result[i/2][indices[i+1] - indices[i]] = '\0';
        }
    }
    result[indices_count/2] = NULL;
    return result;
}

Node* parser(char** tokens){
    if(tokens == NULL){
        return NULL;
    }
    Node* result = (Node*) malloc(sizeof(Node));
    result->left = NULL;
    result->right = NULL;
    result->values = NULL;
    char** look = tokens;
    int i = 0;
    while(look[i] != NULL){
        if(strcmp(look[i], "&&") == 0 || strcmp(look[i], "||") == 0 || strcmp(look[i], ";") == 0){
            result->type = CHAIN;
            result->subtype = strdup(look[i]);
            free(look[i]);
            look[i] = NULL;
            result->left = parser(tokens);
            result->right = parser(look + i + 1);
            return result;
        }
        i++;
    }

    i = 0;
    while(look[i] != NULL){
        if(strcmp(look[i], "|") == 0){
            result->type = PIPE;
            result->subtype = "|";
            free(look[i]);
            look[i] = NULL;
            result->left = parser(tokens);
            result->right = parser(look + i + 1);
            return result;
        }
        i++;
    }

    i = 0;
    while(look[i] != NULL){
        if(strcmp(look[i], ">") == 0 || strcmp(look[i], "<") == 0 || strcmp(look[i], ">>") == 0){
            result->type = IO;
            result->subtype = strdup(look[i]);
            free(look[i]);
            look[i] = NULL;
            result->left = parser(tokens);
            result->right = parser(look + i + 1);
            return result;
        }
        i++;
    }

    char* alias_val = find_alias(&alias_list, tokens[0]);
    if(alias_val){
        remove_alias(&alias_list, tokens[0]);
        char buffer[MAX_STRING_LENGTH];
        int i = 0;
        char* temp = alias_val;
        while(temp[i] != '\0'){
            buffer[i] = temp[i];
            i++;
        }
        buffer[i] = ' ';
        i++;
        for(int n = 1; tokens[n] != NULL; n++){
            for(int j = 0; tokens[n][j] != '\0'; j++){
                buffer[i] = tokens[n][j];
                i++;
            }
            buffer[i] = ' ';
            i++;
            free(tokens[n]);
        }
        buffer[i] = '\0';
        result = parser(lexer(buffer));
        insert_alias(&alias_list, tokens[0], alias_val);
        free(alias_val);
        free(tokens[0]);
    }
    else{
        result->type = COMMAND;
        int i = 0;
        while(tokens[i] != NULL){
            i++;
        }
        result->values = (char**) malloc(sizeof(char*) * (i + 1));
        for(int j = 0; j < i; j++){
            result->values[j] = strdup(tokens[j]);
            free(tokens[j]);
        }
        result->values[i] = NULL;
    }
    return result;
}