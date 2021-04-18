#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "vsh_io.h"
#include "vsh_handler.h"

#define true 1
#define false 0

int main(int argc, char* argv[]) {
    // printf("arg0: %s\n\n", argv[0]);
    // Configura a mascara de sinais bloqueados (SIGINT)
    // sigset_t shell_mask;
    // sigemptyset(&shell_mask);
    // sigaddset(&shell_mask, SIGINT);
    // sigprocmask(SIG_SETMASK, &shell_mask, NULL);

    //le o resto do arquivo e retorna n
    // char* line_buf = NULL, *exec, *args, *process1, *process2;
    char** processes;
    // size_t line_buf_size = 0;
    int n_arguments = 0;

    // ler primeira linha fora do loop
    do {
        show_command_line();
        processes = read_command_line(&n_arguments);

        if (n_arguments == 0) continue;

        if (!strcmp(processes[0], "armageddon"))
            return 0;
        
        for (int i = 0; i < n_arguments; i++) {
            // printf("command: %s\n", processes[i]);
            execute_command(processes[i], n_arguments-1);
        }
    } while (1);
}

/*

vsh > ./main a b c | ls

processos = "./main a b c", "ls"
...
// coloca um \0 no final?
processo = "./main a b c"
executavel = strtok(processo, " ");
argumentos = strtok(NULL, "\0");

if (execve(executavel, arg, envp) == -1)
    perror("Could not execve");

*/