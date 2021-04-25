#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "vsh_io.h"
#include "vsh_handler.h"
#include "vsh_commands.h"
#include "vsh_errors.h"
#include "vsh_execute.h"

#define true 1
#define false 0

int main(int argc, char* argv[]) {
    // Configura a mascara de sinais bloqueados (SIGINT)
    // configure_signals_vsh();

    // struct sigaction sa = {0};
    // sa.sa_flags = SA_RESTART;
    // sa.sa_handler = &handle_sigusr_vsh;
    // sigaction(SIGUSR1, &sa, NULL);

    int n_commands = 0;

    // signal(SIGUSR1, handle_sigusr_vsh);

    // ler primeira linha antes do loop
    do {
        show_command_line();
        char** commands_vector = read_command_line(&n_commands);

        if (n_commands == 0) continue;

        // if (quit_shell(commands_vector[0])) {
        //     free(commands_vector);
        //     return 0;
        // }
        
        execute_programs(n_commands, commands_vector);
        destroy_commands(commands_vector, n_commands);

    } while (true);

    return 0;    
}
