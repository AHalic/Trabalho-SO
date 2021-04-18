#ifndef VSH_COMMANDS_H
#define VSH_COMMANDS_H

void configure_mask();

void execute_command(char* command, int bg);

int quit_shell(char* command);

#endif