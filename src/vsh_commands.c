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
#include "vsh_list.h"

void configure_signals_vsh() {
    struct sigaction sa_dft;
    sa_dft.sa_flags =  SA_RESTART;
    sa_dft.sa_handler = SIG_DFL;

    sigaction(SIGINT, &sa_dft, NULL);
    sigaction(SIGQUIT, &sa_dft, NULL);
    sigaction(SIGTSTP, &sa_dft, NULL);  
}

void configure_signals_vsh_sigusr() {
    struct sigaction block_signal;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTSTP);

    block_signal.sa_mask = mask;
    block_signal.sa_handler = &handle_sigusr_vsh;
    block_signal.sa_flags = SA_RESTART;

    sigaction(SIGUSR1, &block_signal, NULL);
    sigaction(SIGUSR2, &block_signal, NULL);
}

void configure_signals_vsh_ignore() {  
    struct sigaction sa_ign;
    sa_ign.sa_flags = SA_RESTART;
    sa_ign.sa_handler = SIG_IGN;

    sigaction(SIGINT, &sa_ign, NULL);
    sigaction(SIGQUIT, &sa_ign, NULL);
    sigaction(SIGTSTP, &sa_ign, NULL);
}

void configure_signals_fg() {
    struct sigaction sa_dft, sa_sigusr;
    sa_dft.sa_flags =  sa_sigusr.sa_flags = SA_RESTART;

    sa_dft.sa_handler = SIG_DFL;
    sa_sigusr.sa_handler = SIG_IGN;
    
    sigaction(SIGINT, &sa_dft, NULL);
    sigaction(SIGQUIT, &sa_dft, NULL);
    sigaction(SIGTSTP, &sa_dft, NULL);

    struct sigaction block_signal;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTSTP);

    block_signal.sa_mask = mask;
    block_signal.sa_handler = SIG_IGN;
    block_signal.sa_flags = SA_RESTART;
    
    sigaction(SIGUSR1, &block_signal, NULL);
    sigaction(SIGUSR2, &block_signal, NULL);
}

int quit_shell(char* command, SessionList* s_list) {
    // Ve se existe a substring armageddon
    printf("is list null?: %d\n", s_list == NULL);
    for(SessionList* aux = s_list; aux != NULL; aux = aux->next){
        printf("group: %d\n", aux->gid);
    }

    if (strstr(command, "armageddon")) {
        int group;
        for(SessionList* aux = s_list; aux != NULL; aux = aux->next){
            group = aux->gid * -1;
            kill(group, SIGKILL);
        }
        return 1;
    }

    return 0;
}

int destroy_zombies(char* command, pid_t group_pid){
    int status;
    pid_t pid;
    group_pid *= -1;

    if (strstr(command, "liberamoita")){
         // loop nos filhos mortos
        printf("liberamoita in\n");
        while((pid = waitpid(group_pid, &status, WNOHANG)) > -1) {
            // atualiza contador
            if (WIFEXITED(status) && (WEXITSTATUS(status)!= EXIT_SUCCESS)){
                printf("pid in: %d\n", pid);
            }
        }  

        // print teste
        // free(command);
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