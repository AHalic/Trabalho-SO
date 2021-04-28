#ifndef VSH_ERRORS_H
#define VSH_ERRORS_H


/**
 * Função que imprime o erro gerado pela função execvp
 * @return valor de erro {101}
**/
int error_execvp();

/**
 * Função que imprime o erro gerado pela função pipe
 * @return valor de erro {102}
**/
int error_pipe();

/**
 * Função que imprime o erro gerado pela função fork
 * @return valor de erro {103}
**/
int error_fork();

#endif