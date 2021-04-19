#ifndef VSH_COMMANDS_H
#define VSH_COMMANDS_H

void configure_mask();

void execute_command(char* command, int bg, int fd[bg+1][2], int pos);

int quit_shell(char* command);

void destroy_commands(char** commands, int n_commands);

#endif