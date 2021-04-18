#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "vsh_io.h"
#include "vsh_handler.h"
#include "vsh_commands.h"

#define true 1
#define false 0

int main(int argc, char* argv[]) {
    // Configura a mascara de sinais bloqueados (SIGINT)
    // configure_mask();

    int n_commands = 0;

    // ler primeira linha antes do loop
    do {
        show_command_line();
        char** commands_vector = read_command_line(&n_commands);

        if (n_commands == 0) continue;

        if (quit_shell(commands_vector[0])) {
            free(commands_vector);
            return 0;
        }
        
        for (int i = 0; i < n_commands; i++) {
            execute_command(commands_vector[i], n_commands-1);
        }
        
        destroy_commands(commands_vector, n_commands);

    } while (true);

    
}
