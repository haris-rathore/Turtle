#ifndef UTILS_H
#define UTILS_H

#define MAX_STRING_LENGTH 1024

#define COPY(str) (str ? strndup(str, MAX_STRING_LENGTH) : NULL)

#define STDIN_FD 0          
#define STDOUT_FD 1    
#define PIPE_READ_END 0
#define PIPE_WRITE_END 1    

#endif
