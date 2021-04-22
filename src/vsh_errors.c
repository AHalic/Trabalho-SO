#include "vsh_errors.h"
#include <stdio.h>
/**
 * CODIGO DE ERROS
 * 101 - error execvp
 * 102 - error pipe
 * 103 - error fork
 */

int error_execvp() {
    perror("Unabled to execute the execvp ");
    return 101;
}

int error_pipe() {
    perror("Unabled to execute pipe ");
    return 102;
}

int error_fork() {
    perror("Unabled to fork ");
    return 103;
}