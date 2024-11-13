#include "builtin.h"

const char* BUILTIN_COMMANDS[] = {
    "exit",
    "pwd",
    "cd",
    "pushd",
    "popd",
    "dirs",
    "alias",
    "unalias",
    "echo",
    "true",
    "false"
};

const int NUMBER_OF_BUILTIN_COMMANDS = sizeof(BUILTIN_COMMANDS)/sizeof(char*);
char buffer[MAX_STRING_LENGTH];


int exec_builtin(builtin command, char** args){
    int return_code = 0;
    switch (command)
    {
        case EXIT:
            exit(return_code);
        case PWD:
            if(buffer == getcwd(buffer, MAX_STRING_LENGTH)){
                printf("%s\n", buffer);
            }
            fflush(stdout);
            break;
        case CD:
            if((args[1] == NULL && chdir(getenv("HOME")) != 0) || chdir(args[1]) != 0){
                return_code = 1;
            }
            break;
        case PUSHD:
            if(buffer == getcwd(buffer, MAX_STRING_LENGTH)){
                if(push_dir(&directory_stack, buffer) == NULL || chdir(args[1]) != 0){
                    return_code = 1;
                }
            }
            break;
        case POPD:
            char* str = pop_dir(&directory_stack);
            if(str){
                if(chdir(str) != 0){
                    return_code = 1;
                }
                free(str);
            }
            else{
                return_code = 1;
            }
            break;
        case DIRS:
            return_code = print_stack(&directory_stack);
            break;
        case ALIAS:
            if(args[1] != NULL){
                if(args[2] != NULL){
                    if(find_alias(&alias_list, args[1])){
                        remove_alias(&alias_list, args[1]);
                    }
                    if(insert_alias(&alias_list, args[1], args[2]) == NULL){
                        return_code = 1;
                    }
                }
                else{
                    char *temp = find_alias(&alias_list, args[1]);
                    printf("%s='%s'\n", args[1], temp);
                }
            }
            else
                return_code = print_alias(&alias_list);
            break;
        case UNALIAS:
            if(remove_alias(&alias_list, args[1]) == NULL){
                return_code = 1;
            }
            break;
        case ECHO:
            int i = 1;
            while(args[i]){
                if(args[i+1]){
                    printf("%s ", args[i]);
                }
                else{
                    printf("%s", args[i]);
                }
                i++;
            }
            printf("\n");
            fflush(stdout);
            break;
        case TRUE:
            break;
        case FALSE:
            return_code = 1;
            break;
        default:
            break;
    }
    return return_code;
};