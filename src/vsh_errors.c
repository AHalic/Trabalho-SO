#include "vsh_errors.h"
#include <stdio.h>
/**
 * CODIGO DE ERROS
 * 101 - error execvp
 * 102 - error pipe
 * 103 - error fork
 * 104 - error dup
 * 105 - error setsid
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

int error_dup(){
    perror("Unabled to dup ");
    return 104;
}

int error_setsid(){
    perror("Unabled to setsid ");
    return 105;
}