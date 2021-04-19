#include "vsh_errors.h"
#include <stdio.h>

void error_execvp() {
    perror("Wasn't able to execute the execvp ");
}

void error_pipe() {
    perror("Wasn't able to execute pipe ");
}