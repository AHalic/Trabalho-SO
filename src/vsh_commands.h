#ifndef VSH_COMMANDS_H
#define VSH_COMMANDS_H

void configure_signals_vsh();

int quit_shell(char* command);

void destroy_commands(char** commands, int n_commands);

#endif