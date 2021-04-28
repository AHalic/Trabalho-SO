#ifndef VSH_COMMANDS_H
#define VSH_COMMANDS_H

#include <sys/types.h>
#include "vsh_list.h"


/**
 * Verifica se o comando inserido na linha de comandos é "armageddon", 
 * se sim, mata todos os processos dos grupos presentes na lista
 * e encerra a vsh
 * @param {char*}command comando inserido na linha de comandos
 * @param {session_list*}s_list lista contendo os grupos
 * @return 1 se o comando é armageddon, 0 se não
**/ 
int quit_shell(char* command, session_list* s_list);

/**
 * Verifica se o comando inserido na linha de comandos é "liberamoita", 
 * se sim, espera todos os processos filhos se encerrarem 
 * dessa forma, são retirados os processos zombies da vsh
 * @param {char*}command comando inserido na linha de comandos
 * @param {pid_t}group_pid
 * @return 1 se o comando é liberamoita, 0 se não
**/ 
int destroy_zombies(char* command, pid_t group_pid);

/**
 * Destrói os comandos alocados
 * @param {char**}commands vetor de comandos inseridos na linha de comandos
 * @param {int}n_commands quantidade de comandos a ser liberado
**/ 
void destroy_commands(char** commands, int n_commands);

#endif