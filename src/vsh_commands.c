#include "vsh_commands.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "vsh_errors.h"

void configure_mask() {
    // Configura a mascara de sinais bloqueados (SIGINT)
    sigset_t shell_mask;
    sigemptyset(&shell_mask);
    sigaddset(&shell_mask, SIGINT);
    sigprocmask(SIG_SETMASK, &shell_mask, NULL);
}

static void execute_command_child (char* exec, char** argv, int bg) {
    // chama filho para processo
    int pid = fork();
    if(!pid)
        if(execvp(exec, argv) == -1){
            error_execvp();
            free(argv);
            exit(1);
        }

    // Espera o filho terminar para continuar se for fg
    if(!bg){
        int wstatus;
        waitpid(pid, &wstatus, WUNTRACED);
    }
   
}

void execute_command(char* command, int bg){
    char* token = strtok(command, " ");
    char* exec = token;
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

    execute_command_child(exec, argv, bg);
    free(argv);
}

int quit_shell(char* command) {
    if (!strcmp(command, "armageddon"))
        return 1;
    return 0;
}