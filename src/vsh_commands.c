#include "vsh_commands.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "vsh_io.h"
#include "vsh_errors.h"
#include "vsh_handler.h"

void configure_signals_vsh() {
    struct sigaction sa_sigint, sa_sigquit, sa_sigtstp, sa_sigusr;
    sa_sigint.sa_flags = sa_sigquit.sa_flags = sa_sigtstp.sa_flags = sa_sigusr.sa_flags = SA_RESTART;

    sa_sigint.sa_handler = &handle_sigint;
    sa_sigquit.sa_handler = &handle_sigquit;
    sa_sigtstp.sa_handler = &handle_sigtstp;
    sa_sigusr.sa_handler = &handle_sigusr_vsh;
    
    sigaction(SIGINT, &sa_sigint, NULL);
    sigaction(SIGQUIT, &sa_sigquit, NULL);
    sigaction(SIGTSTP, &sa_sigtstp, NULL);
    sigaction(SIGUSR1, &sa_sigusr, NULL);
    sigaction(SIGUSR2, &sa_sigusr, NULL);
}

void configure_signals_fg() {
    struct sigaction sa_dft, sa_sigusr;
    sa_dft.sa_flags =  sa_sigusr.sa_flags = SA_RESTART;

    sa_dft.sa_handler = SIG_DFL;
    sa_sigusr.sa_handler = SIG_IGN;
    
    sigaction(SIGINT, &sa_dft, NULL);
    sigaction(SIGQUIT, &sa_dft, NULL);
    sigaction(SIGTSTP, &sa_dft, NULL);
    sigaction(SIGUSR1, &sa_sigusr, NULL);
    sigaction(SIGUSR2, &sa_sigusr, NULL);
}

int quit_shell(char* command, pid_t* pids, int n_commands) {
    // Ve se existe a substring armageddon
    if (strstr(command, "armageddon")) {
        for(int i = 0; i < n_commands; i++){
            kill(pids[i], SIGTERM);
        }
        
        // pid_t pid;
        // int status;
        //  // loop nos filhos mortos
        // while((pid = waitpid(-1, &status, WNOHANG)) > -1) {
        //     // atualiza contador
        //     printf("pid: %d\n", pid);
        //     if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
        //         kill(pid, )
        //     }
        //     total++; // incrementa os acertos
        // }   
        
        exit(0);
        free(command);
        return 1;
    }

    return 0;
}

int destroy_zombies(char* command, int pid){
    static int errors = 0; // filhos com erro
    static int total = 0; // filhos que terminaram
    float percent = 0; // percentual
    int status = 0; // status

    if (strstr(command, "liberamoita")){
         // loop nos filhos mortos

        if (pid) {
            while(waitpid(0, NULL, WUNTRACED) > -1);
            // nao funcionou isso
            // a minha ideia era q podiamos usar pra wait os filhos
        }
        else {
            while((pid = waitpid(-1, &status, WNOHANG)) > -1) {
            // atualiza contadorif (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
            }
        }   
        
        return 1;
    }

    return 0;
}

void destroy_commands(char** commands, int n_commands) {
    for (int i = 0; i < n_commands; i++) {
        free(commands[i]);
    }
    free(commands);
}