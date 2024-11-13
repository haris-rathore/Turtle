#include "executor.h"



int last_success = SUCCESS;
int last_chain = AND;

void m_free(Node* node){
    if(node != NULL){
        if(node->values != NULL){
            int i = 0;
            while(node->values[i] != NULL){
                free(node->values[i]);
                i++;
            }
            free(node->values);
        }
        m_free(node->left);
        m_free(node->right);
        free(node);
    }
}

int execute(Node* node){
    if(node == NULL){
        return 0;
    }
    if(node->type == CHAIN){
        if((last_success == SUCCESS && last_chain == AND) || (last_success != SUCCESS && last_chain == OR)){
            last_success = execute(node->left);
        }
        if(strcmp(node->subtype, "&&") == 0){
            last_chain = AND;
        }
        if(strcmp(node->subtype, "||") == 0){
            last_chain = OR;
        }
        if(strcmp(node->subtype, ";") == 0){
            last_chain = AND;
            last_success = SUCCESS;
        }
        if(node->right->type == CHAIN){
            return execute(node->right);
        }
        if(node->right->type == COMMAND || node->right->type == IO){
            if((last_success == SUCCESS && last_chain == AND) || (last_success != SUCCESS && last_chain == OR)){
                return execute(node->right);
            }
        }
    }
    if(node->type == IO){
        if(strcmp(node->subtype, ">") == 0){
            int out_file = open(*(node->right->values), O_WRONLY | O_CREAT, 0644);
            int dup_stdout = dup(1);
            dup2(out_file, 1);
            last_success = execute(node->left);
            dup2(dup_stdout, 1);
            close(out_file);
            if(node->right->right != NULL)
                return execute(node->right->right);
            else
                return 0;
        }
        if(strcmp(node->subtype, ">>") == 0){
            int out_file = open(*(node->right->values), O_WRONLY | O_CREAT |O_APPEND, 0644);
            int dup_stdout = dup(1);
            dup2(out_file, 1);
            last_success = execute(node->left);
            dup2(dup_stdout, 1);
            close(out_file);
            return execute(node->right->right);
        }
        if(strcmp(node->subtype, "<") == 0){
            int in_file = open(*(node->right->values), O_RDONLY);
            int dup_stdin = dup(0);
            dup2(in_file, 0);
            last_success = execute(node->left);
            dup2(dup_stdin, 0);
            close(in_file);
            return execute(node->right->right);
        }
    }
    if(node->type == PIPE){
        int pipe_fd[2];
        if(pipe(pipe_fd)){
            return 1;
        };

        int dup_stdout = dup(STDOUT_FD);
        int dup_stdin = dup(STDIN_FD);

        close(STDOUT_FD);
        dup2(pipe_fd[PIPE_WRITE_END], STDOUT_FD);
        execute(node->left);

        close(pipe_fd[PIPE_WRITE_END]);
        dup2(dup_stdout, STDOUT_FD);
        close(STDIN_FD);
        dup2(pipe_fd[PIPE_READ_END], STDIN_FD);
        execute(node->right);
        
        dup2(dup_stdin, STDIN_FD);
        dup2(dup_stdout, STDOUT_FD);
        close(pipe_fd[PIPE_READ_END]);
    }
    if(node->type == COMMAND){
        for(int i = 0; i < NUMBER_OF_BUILTIN_COMMANDS; i++){
            if(strcmp(node->values[0], BUILTIN_COMMANDS[i]) == 0){
                return exec_builtin(i, node->values);
            };
        }
        int rc = fork();
        if(rc == 0){
            if(execvp(node->values[0], node->values) < 0){
                exit(EXIT_FAILURE);;
            }
        }
        else{
            int status;
            wait(&status);
            return status;
        }
    }
    return 0;
}