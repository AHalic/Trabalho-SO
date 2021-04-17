#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "messages.h"

#define true 1
#define false 0

int main() {
    // Configura a mascara de sinais bloqueados (SIGINT)
    // sigset_t shell_mask;
    // sigemptyset(&shell_mask);
    // siaddset(&shell_mask, SIGINT);
    // sigprocmask(SIG_SETMASK, &shell_mask, NULL);

    //le o resto do arquivo e retorna n
    char* line_buf = NULL, *exec, *args, *process1, *process2;
    char** processes;
    size_t line_buf_size = 0;
    int line_count = 0;
    int n_arguments;

    // ler primeira linha fora do loop
    do
    {
        show_command_line();
        // processes = read_command_line(&n_arguments);
        getline(&line_buf, &line_buf_size, stdin);

        // Retirando o caracter \n ou \r do fim da string (lido pelo getline)
        line_buf[strcspn(line_buf, "\r\n")] = 0; 
        process1 = strtok(line_buf, "|");
        printf("Os processos são: %s\n", process1);
        process2 = strtok(NULL, "|");

        exec = strtok(process1, " ");
        args = strtok(NULL, "\0");

        printf("A linha lida é %s\n", line_buf);
        printf("o executavel é %s\n", exec);
        printf("os args são |%s|\n", args);
        printf("O proximo processo eh |%s|\n", process2);
    } while (strcmp(line_buf, "armageddon"));
    
    
    free(line_buf);

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