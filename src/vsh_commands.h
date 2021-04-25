#ifndef VSH_COMMANDS_H
#define VSH_COMMANDS_H
#include <sys/types.h>

void configure_signals_vsh();

void configure_signals_fg();

int quit_shell(char* command, pid_t* pids, int n_commands);

int destroy_zombies();

void destroy_commands(char** commands, int n_commands);

#endif