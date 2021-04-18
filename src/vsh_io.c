#include "vsh_io.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "vsh_errors.h"

void show_command_line() {
    printf("vsh > ");
}

static int n_process_counter(char* line_buf) {
    // conta a quantidade de processo na linha
    if (line_buf == NULL) return 0;

    int n_process = 1;
    
    for (int i = 0; i < strlen(line_buf); i++) {
        if(!strncmp(&line_buf[i], "|", 1))
            n_process++;
    }

    return n_process;
}

char** read_command_line(int* n_process) {
    char* line_buf = NULL;
    size_t line_buf_size = 0;

    // lendo linha de comando
    getline(&line_buf, &line_buf_size, stdin);

    // retirando o caracter \n ou \r do fim da string (lido pelo getline)
    line_buf[strcspn(line_buf, "\r\n")] = 0; 

    *n_process = n_process_counter(line_buf);

    // se nao tem processo, retorna NULL
    if (*n_process == 0) return NULL;


    // aloca espaco para comandos
    char** commands_vector = (char**) malloc (sizeof(char*) * (*n_process));  // free
    int aux_i = 0;

    
    // ler a primeira linha
    char* command = strtok(line_buf, "|");
    commands_vector[aux_i++] = command;


    // verifica se o commando token eh null
    while(command != NULL) {
        int i = 0;

        command = strtok(NULL, "|");
        // printf("com %s\n", command);
 
        commands_vector[aux_i++] = command;
    }

    return commands_vector;
}

void execute_command(char* command, int type){
    char* token = strtok(command, " ");
    char* exec = token;
    char** argv = (char**) malloc (sizeof(char*) * 4); // free

    argv[0] = exec;
    int i = 1; 

    while (i < 4) {
        char* token = strtok(NULL, " ");
        if (!token) {
            break;
        } else {
            argv[i++] = token;
        }   
    }

    int pid = fork();
    if(!pid)
        if(execvp(exec, argv) == -1){
            error_execvp();
            free(argv);
            exit(1);
        }

    // Espera o filho terminar para continuar se for fg
    if(!type){
        int wstatus;
        waitpid(pid, &wstatus, WUNTRACED);
    }
    free(argv);
}