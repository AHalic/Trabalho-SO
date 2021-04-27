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
#include "vsh_list.h"

#define true 1
#define false 0

int main(int argc, char* argv[]) {
    // Configura a mascara de sinais
    configure_signals_vsh_sigusr();

    SessionList* gid_list = init_empty_gid_list();
    int n_commands = 0, session;

    // ler primeira linha antes do loop
    do {
        show_command_line();
        char** commands_vector = read_command_line(&n_commands);

        if (n_commands == 0) continue;

        if(n_commands == 1){
            if(destroy_zombies(commands_vector[0], 1))
                continue;
            if(quit_shell(commands_vector[0], gid_list)) {
                printf("oi\n");
                usleep(100);
                destroy_list(gid_list);
                free(commands_vector);
                break;
            }
        }

        session = execute_programs(n_commands, commands_vector);
        printf("s: %d\n", session);
        SessionList* gid_list = add_gid(gid_list, session);
        configure_signals_vsh();

        // if (n_commands > 1) {
        //     destroy_zombies("liberamoita", 1);
        // }
        
        destroy_commands(commands_vector, n_commands);

    } while (true);

    return 0;    
}
