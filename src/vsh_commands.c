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
    // Configura a mascara de sinais bloqueados (SIGINT)
    struct sigaction sa_sigint, sa_sigquit, sa_sigtstp;
    sa_sigint.sa_flags = sa_sigquit.sa_flags = sa_sigtstp.sa_flags = SA_RESTART;

    sa_sigint.sa_handler = &handle_sigint;
    sa_sigquit.sa_handler = &handle_sigquit;
    sa_sigtstp.sa_handler = &handle_sigtstp;

    sigaction(SIGINT, &sa_sigint, NULL);
    sigaction(SIGQUIT, &sa_sigquit, NULL);
    sigaction(SIGTSTP, &sa_sigtstp, NULL);
}

static int execute_command_child_fg (char* exec, char** argv) {

    int pid = fork();
    if(!pid)
        // printf("PID: %d\n", getpid());
        if(execvp(exec, argv) == -1){
            int error = error_execvp();
            free(argv);
            exit(error);
        }

    // Espera o filho terminar para continuar 
    int wstatus;
    waitpid(pid, &wstatus, WUNTRACED);

    if(!pid)
        return getpid();
    return pid;
}

static int execute_command_child_bg(char* exec, char** argv, int pos, int n_com, int fd[n_com][2]){
    int pid = fork();
    
    if(!pid) {
        printf("Child forked: %d Dad: %d\n", getpid(), getppid());
        if (pos == 0)
            // raise(SIGUSR1); // tava usando para mandar o SIGUSR1
        
        // TODO: dar close nos pipes n usados
        if (!pos) {
            close(1);
            close_pipe(n_com, fd, 0, 1);
            close_pipe(n_com, fd, n_com, 0);
            dup(fd[0][1]);
        }
        else if (pos == n_com) {
            close(0);
            close_pipe(n_com, fd, n_com, 1);
            close_pipe(n_com, fd, pos-1, 0);
            dup(fd[pos-1][0]);
        }
        else {
            close(0);
            close(1);
            close_pipe(n_com, fd, pos, 1);
            close_pipe(n_com, fd, pos-1, 0);
            dup(fd[pos-1][0]);
            dup(fd[pos][1]);
        }
        
        if(execvp(exec, argv) == -1){
            int error = error_execvp();
            free(argv);
            exit(error);
        }
    }

    if(!pid)
        return getpid();
    return pid;
}

int execute_command(char* command, int bg, int fd[bg][2], int pos) {
    char* token = strtok(command, " ");
    char* exec = strdup(token);
    char** argv = (char**) malloc (sizeof(char*) * 4); 

    // primeiro argumento eh o executavel
    argv[0] = exec;

    // ler argumentos
    int i = 1; 
    while (i < 4) {
        char* token = strtok(NULL, " ");
        if (!token) {
            break;
        } else {
            argv[i++] = token;
        }   
    }
    argv[i] = NULL;

    // TODO: quando armageddon não é 1º da ruim
    if (quit_shell(exec)) exit(0);
    pid_t pid;
    if(!bg)
        pid = execute_command_child_fg(exec, argv);
    else
        pid = execute_command_child_bg(exec, argv, pos, bg, fd);

    free(argv);
    return pid;
}

int quit_shell(char* command) {
    // Ve se existe a substring armageddon
    if (strstr(command, "armageddon")) {
        free(command);
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