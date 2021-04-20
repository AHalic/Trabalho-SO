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

void configure_mask() {
    // Configura a mascara de sinais bloqueados (SIGINT)
    sigset_t shell_mask;
    sigemptyset(&shell_mask);
    sigaddset(&shell_mask, SIGINT);
    sigaddset(&shell_mask, SIGSTOP);
    sigprocmask(SIG_SETMASK, &shell_mask, NULL);
}

static void execute_command_child_fg (char* exec, char** argv) {

    int pid = fork();
    if(!pid)
        if(execvp(exec, argv) == -1){
            int error = error_execvp();
            free(argv);
            exit(error);
        }

    // Espera o filho terminar para continuar 
    int wstatus;
    waitpid(pid, &wstatus, WUNTRACED);
}

static void execute_command_child_bg(char* exec, char** argv, int pos, int n_com, int fd[n_com][2]){
    
    int pid = fork();
    if(!pid) {
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

}

void execute_command(char* command, int bg, int fd[bg][2], int pos) {
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

    if(!bg)
        execute_command_child_fg(exec, argv);
    else
        execute_command_child_bg(exec, argv, pos, bg, fd);

    free(argv);
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