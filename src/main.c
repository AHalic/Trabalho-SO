#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "vsh_io.h"
#include "vsh_handler.h"
#include "vsh_commands.h"
#include "vsh_errors.h"

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
        

        if (n_commands > 1) {
            int fd[n_commands - 1][2];
            open_pipe(n_commands, fd);
            for (int i = 0; i < n_commands; i++)
                execute_command(commands_vector[i], n_commands-1, fd, i);   
        }
        else 
            execute_command(commands_vector[0], n_commands-1, NULL, 0);

        
             
        // for (int i = 0; i < n_commands; i++) {
        //     if (n_commands == 1) {
        //         execute_command(commands_vector[i], n_commands-1, NULL, i);
        //     }
        //     else
        //         execute_command(commands_vector[i], n_commands-1, fd, i);
        // }
        
        destroy_commands(commands_vector, n_commands);

    } while (true);

    
}
