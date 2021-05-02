#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vsh_io.h"
#include "vsh_handler.h"
#include "vsh_commands.h"
#include "vsh_errors.h"
#include "vsh_execute.h"
#include "vsh_list.h"

#define true 1

int main(int argc, char* argv[]) {
    system("clear"); 

    // configura a mascara de sinais para os SIGUSRs
    configure_signals_vsh_sigusr();

    // cria lista vazia para gid
    session_list* gid_list = init_empty_gid_list();

    int n_commands = 0, session;

    // ler primeira linha antes do loop
    do {
        // mostra linha de comando e recebe comandos
        show_command_line();
        char** commands_vector = read_command_line(&n_commands);

        // se nao tiver comandos, volta para o inicio
        if (n_commands == 0) continue;

        // se tiver apenas 1 comando, verificar se sao comandos da shell
        if(n_commands == 1){
            if(destroy_zombies(commands_vector[0])) {
                destroy_commands(commands_vector, 1);
                continue;
            }
            if(quit_shell(commands_vector[0], gid_list)) {
                destroy_list(gid_list);
                destroy_commands(commands_vector, 1);
                break;
            }
        }

        // executa os programas/processos e obtem o gid da sessao
        session = execute_programs(n_commands, commands_vector);
        // para impressao do prompt apos a execucao de um processo
        usleep(1000);

        // adiciona pid da sessao
        if (n_commands > 1)
            gid_list = add_gid(gid_list, session);

        // reconfigura os sinais SIGINT, SIGQUIT, SIGTSTP para default
        configure_signals_vsh();
        
        // libera o vetor de comandos
        destroy_commands(commands_vector, n_commands);

    } while(true);

    return 0;    
}
