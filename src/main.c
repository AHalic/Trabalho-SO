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

#define true 1
#define false 0

// pega a parte de executar comando que tava na main e faz uma função para isso
// adicionando como seria mandado os sinais
int execute_programs(int n_commands, char** commands_vector) {
    pid_t command_line;
    // faz um fork para auxiliar com a criação dos processos da linha de comando
    if ((command_line = fork()) == -1) return error_fork();
    
    if(!command_line && n_commands > 1) {
        // background

        // faz os processos dps serem tudo da msm sessao
        pid_t group = setsid(); 
        // printf("returns group %d\n", group);
        // vejo quem eh pai e filho (pra poder ver se o pai n eh a shell e ver o pid do fork aux)
        // printf("COMMAND LINE PID: %d, son of %d\n", getpid(), getppid()); 

        struct sigaction sa_sigusr, sa_sigint, sa_sigquit, sa_sigtstp;
        sa_sigusr.sa_flags = sa_sigint.sa_flags = sa_sigquit.sa_flags = sa_sigtstp.sa_flags = SA_RESTART;
        // sa_sigusr.sa_handler = &handle_sigusr_sick;
        // sigaction(SIGUSR1, &sa_sigusr, NULL);

        sa_sigint.sa_handler = &handle_sigint;
        sigaction(SIGINT, &sa_sigint, NULL);

        sa_sigquit.sa_handler = &handle_sigquit;
        sigaction(SIGQUIT, &sa_sigquit, NULL);

        sa_sigtstp.sa_handler = &handle_sigquit;
        sigaction(SIGQUIT, &sa_sigtstp, NULL);

        // aux pra pegar o pid
        pid_t pids[n_commands]; // usado pra guardar o pid dos filhos
        pid_t pid; 
        int status; // ver o status do pai com os filhos
        int signaledUsr = 0; // ver se foi sinalizado SIGUSR1 pro pai
        int fd[n_commands - 1][2];

        open_pipe(n_commands, fd);
        for (int i = 0; i < n_commands; i++) {
            pids[i] = execute_command(commands_vector[i], n_commands-1, fd, i);  
        }
    
        close_pipe(n_commands-1, fd, n_commands, 0);
        close_pipe(n_commands-1, fd, n_commands, 1); 
        while ((pid = waitpid(0, &status, WNOHANG)) > -1) {
            if (WIFSIGNALED(status)) {
                // printf("signaled aq %d\n", WTERMSIG(status));
                // usa pra verificar se o filho terminou com sigusr1 (talvez seria legal n setar o handler do filho mas sim do pai)
                if (WTERMSIG(status) == SIGUSR1) {
                    for (int i = 0; i < n_commands; i++) {
                        kill(pids[i], SIGKILL);
                    }
                    // signaledUsr = 1; // alguem mandou sigusr 1
                }
            }
        }

        // se alguem mandou o sigusr1
        // if (signaledUsr) {
        //     // manda sinal para todos os filhos (talvez eh estranho mandar pro filho q mandou usr1? 
        //     // e se ele ja terminou, oq acontece?) 
        //     // tou mandando SIGUSR! mas precisaria modificar a funcao de handler
        //     for (int i = 0; i < n_commands; i++) {
        //         if (pids[i] != 0) {
        //             printf("pid: %d\n", pids[i]);
        //             kill(pids[i], SIGTERM);
        //         }
        //     }
            
        //     printf("oi\n");
        // }
    
        exit(0);
    } 
    else if (n_commands == 1) {
        // foreground
        if(!command_line){
            signal(SIGUSR1, SIG_IGN);
            pid_t pid = execute_command(commands_vector[0], n_commands-1, NULL, 0);
            exit(0);
        }
        // wait(NULL);
        waitpid(command_line, NULL, WUNTRACED);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    // Configura a mascara de sinais bloqueados (SIGINT)
    // configure_signals_vsh();

    // struct sigaction sa = {0};
    // sa.sa_flags = SA_RESTART;
    // sa.sa_handler = &handle_sigusr_vsh;
    // sigaction(SIGUSR1, &sa, NULL);

    int n_commands = 0;
    // printf("SHELL PID: %d\n", getpid());
    // printf("GRUPO ID: %d\n", getgid());
    // signal(SIGUSR1, handle_sigusr_vsh);

    // ler primeira linha antes do loop
    do {
        show_command_line();
        char** commands_vector = read_command_line(&n_commands);

        if (n_commands == 0) continue;

        if (quit_shell(commands_vector[0])) {
            free(commands_vector);
            return 0;
        }
        
        execute_programs(n_commands, commands_vector);
        destroy_commands(commands_vector, n_commands);

    } while (true);

    return 0;    
}
