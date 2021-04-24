#ifndef VSH_EXECUTE_H
#define VSH_EXECUTE_H

int execute_command(char* command, int bg, int fd[bg][2], int pos);

int execute_programs(int n_commands, char** commands_vector);

#endif