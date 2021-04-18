#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "./src/vsh_io.c"
#include "./src/vsh_handler.c"

#define true 1
#define false 0

int main() {
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
        
        
        printf("Number of arguments: %d\n", n_arguments);
        for (int i = 0; i < n_arguments; i++) {
            printf("command: %s\n", processes[i]);
            execute_command(processes[i]);
        }


        // getline(&line_buf, &line_buf_size, stdin);

        // Retirando o caracter \n ou \r do fim da string (lido pelo getline)
        // exec = strdup(line_buf);
        // line_buf[strcspn(line_buf, "\r\n")] = 0; 
        // printf("A linha lida é %s\n", line_buf);
        
        // process1 = strtok(line_buf, "|");
        // printf("A linha lida é %s\n", line_buf);

        // printf("A linha lida é %s\n", exec);
        // // printf("Os processos são: %s\n", process1);
        // process2 = strtok(NULL, "|");

        // exec = strtok(process1, " ");
        // args = strtok(NULL, "\0");

        // printf("o executavel é %s\n", exec);
        // printf("os args são |%s|\n", args);
        // printf("O proximo processo eh |%s|\n", process2);
    // } while (strcmp(line_buf, "armageddon") && !feof(stdin));
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