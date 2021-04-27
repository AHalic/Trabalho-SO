#ifndef VSH_COMMANDS_H
#define VSH_COMMANDS_H

#include <sys/types.h>
#include "vsh_list.h"

void configure_signals_vsh();

void configure_signals_vsh_sigusr();

void configure_signals_fg();

void configure_signals_vsh_ignore();

int quit_shell(char* command, SessionList* s_list);

int destroy_zombies(char* command, pid_t _pid);

void destroy_commands(char** commands, int n_commands);

#endif