#include "vsh_errors.h"
#include <stdio.h>
/**
 * CODIGO DE ERROS
 * 101 - error execvp
 * 
 */

int error_execvp() {
    perror("Wasn't able to execute the execvp ");
    return 101;
}