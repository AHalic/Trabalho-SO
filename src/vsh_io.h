#ifndef VSH_H
#define VSH_H

/**
 * Mostra na tela "vsh > ".
 */
void show_command_line();

/**
 * Ler linha de comando e atualiza a quantidade de processos.
 * @param {int*}n_process Ponteiro para variavel que armazenara a quantidade de processos.
 * 
 * @return Vetor de strings dos comandos lidos. Cada comando eh uma string.
 */
char** read_command_line(int* n_process);

/**
 * Fecha todos os pipes exceto o inidicado pelo inteiro not. Os pipes fechados ou sao de leitura
 * ou sao de escrita. 
 * @param {int}n_command Quantidade de comandos da linha de comando - 1.
 * @param {int*}fd[2] Matriz de pipes no formato [n_command][2].
 * @param {int}not Pipe que nao deve ser fechado.
 * @param {int}rdwt 0 se for os pipes referente a leitura, 1 se for referente a escrita.
 */
void close_pipe(int n_command, int fd[n_command][2], int not, int rdwt);

/**
 * Abre todos os pipes
 * @param {int}n_commands Quantidade de comandos na linha - 1.
 * @param {int}fg[n_commands][2] Matriz para pipes inicializada no formato [n_commands][2]. 
 */
void open_pipe(int n_commands, int fd[n_commands][2]);

#endif