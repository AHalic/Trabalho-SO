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

    char** commands_vector;
    int n_arguments = 0;

    // ler primeira linha antes do loop
    do {
        show_command_line();
        commands_vector = read_command_line(&n_arguments);

        if (n_arguments == 0) continue;

        if (quit_shell(commands_vector[0])) return 0;
        
        for (int i = 0; i < n_arguments; i++) {
            execute_command(commands_vector[i], n_arguments-1);
        }

    } while (true);
}
