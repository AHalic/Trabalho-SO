#include "vsh_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_command_line() {
    printf("vsh > ");
}

static int n_process_counter(char* line_buf) {
    // conta a quantidade de processo na linha
    if (line_buf == NULL) return 0;

    char* process = strtok(line_buf, "|");

    if (process == NULL) return 0;

    int n_process = 1;

    while(process != NULL) {
        process = strtok(NULL, "|");
        n_process++;
    }

    printf("%d\n", n_process);
    return n_process;
}

char** read_command_line(int* n_process) {
    *n_process = 0;

    printf("\n");
    char* line_buf = NULL, *aux;
    printf("\n");

    // lendo linha de comando
    getline(&line_buf, NULL, stdin);
    printf("a\n");
    
    // retirando o caracter \n ou \r do fim da string (lido pelo getline)
    line_buf[strcspn(line_buf, "\r\n")] = 0; // dando problema aqui 

    printf("%s\n", line_buf);

    printf("b\n");
    // duplicando linha de comando para contar pipe
    aux = strdup(line_buf);
    *n_process = n_process_counter(aux);
    printf("\n");

    // se nao tem processo, retorna NULL
    if (n_process == 0) return NULL;
    printf("\n");

    // aloca espaco para comandos
    char** commands_vector = (char**) malloc (sizeof(char*) * (*n_process));
    int aux_i = 0;
    printf("\n");

    // ler a primeira linha
    char* command = strtok(line_buf, "|");
    commands_vector[aux_i++] = command;
    printf("\n");

    // verifica se o commando token eh null
    while(command != NULL) {
        int i = 0;

        command = strtok(NULL, "|");
        while(command[i++] == ' ') {
            command = strtok(NULL, " ");
        }

        // adiciona comando a vetor de comandos
        commands_vector[aux_i++] = command;
    }

    // free(line_buf);
    // free(aux);

    return commands_vector;
}