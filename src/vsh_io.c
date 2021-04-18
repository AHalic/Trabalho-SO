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

static int n_process_counter(char *line_buf) {
    // conta a quantidade de processo na linha
    if (line_buf == NULL)
        return 0;

    int n_process = 1;

    // ler a quantidade de pipes
    for (int i = 0; i < strlen(line_buf); i++)
    {
        if(line_buf[i] == '|')
            n_process++;
    }

    return n_process;
}

char **read_command_line(int *n_process) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;

    // lendo linha de comando
    getline(&line_buf, &line_buf_size, stdin);

    // retirando o caracter \n ou \r do fim da string (lido pelo getline)
    line_buf[strcspn(line_buf, "\r\n")] = '\0';
    
    *n_process = n_process_counter(line_buf);

    // se nao tem processo, retorna NULL
    if (*n_process == 0)
        return NULL;

    // aloca espaco para comandos
    char **commands_vector = (char **)malloc(sizeof(char *) * (*n_process+1)); // free
    
    int aux_i = 0;

    // ler a primeira linha
    char *aux_command = strtok(line_buf, "|");
    char *command = strdup(aux_command);
    commands_vector[aux_i++] = command;
    
    // verifica se o commando token eh null
    while (aux_command && (aux_i < 5 && *n_process < 5)) {
        aux_command = strtok(NULL, "|");

        if(aux_command){
            char* next_command = strdup(aux_command);
            commands_vector[aux_i++] = next_command;
        }
    }

    free(line_buf);
    return commands_vector;
}