#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"
#include "builtin.h"
#include <string.h>
#include <fcntl.h>

extern int last_success;
extern int last_chain;

void m_free(Node* node);

int execute(Node* node);

#endif