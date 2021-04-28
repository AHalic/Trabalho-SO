#ifndef VSH_COMMANDS_H
#define VSH_COMMANDS_H

#include <sys/types.h>
#include "vsh_list.h"


/**
 * 
**/ 
int quit_shell(char* command, session_list* s_list);

int destroy_zombies(char* command, pid_t _pid);

void destroy_commands(char** commands, int n_commands);

#endif