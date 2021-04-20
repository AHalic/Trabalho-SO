#ifndef VSH_H
#define VSH_H

void show_command_line();

char** read_command_line(int* n_process);

void close_pipe(int n_command, int fd[n_command][2], int not, int rdwt);

void open_pipe(int n_commands, int fd[n_commands][2]);

#endif