#include "vsh_io.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "vsh_errors.h"
#include "vsh_commands.h"

void show_command_line() {
    printf("vsh> ");
}

/**
 * Conta a quantidade de processos recebido na linha de comando
 * @param {char* line_buf} String da linha de comando
 * 
 * @return quantidade de comandos na linha de comando
 */
static int n_process_counter(char *line_buf) {
    // se a string for NULL ou len = 0, nao tem comandos
    if (line_buf == NULL || !strlen(line_buf))
        return 0;

    // se nao entrou no if, tem ao minimo 1 comando
    int n_process = 1;

    // ler a quantidade de pipes para ver se tem mais comandos
    for (int i = 0; i < strlen(line_buf); i++)
    {
        if(line_buf[i] == '|')
            n_process++;
    }

    return n_process;
}

/**
 * Retira "\n" ou "\r" no final da linha e substitui por "\0"
 * 
 * @return string lida.
 */
static char* parseInput() {
    char *line_buf = NULL;
    size_t line_buf_size = 0;

    // lendo linha de comando
    int charRead = getline(&line_buf, &line_buf_size, stdin);

    // se tem mais que o char EOF (no minimo 2 char)
    if (charRead > 1) {
        line_buf[strlen(line_buf) - 1] = '\0'; // coloca "\0" no fim da string (problems com "\r\n")
        rewind(stdin);                         // volta para o inicio da stream
    }
    else { 
        // se linha de comando for vazia
        rewind(stdin);                        // volta para o inicio da stream
        return NULL;                          // retorna string vazia
    }

    return line_buf;                          // retorna string 
}

char **read_command_line(int *n_process) {
    char* line_buf = parseInput();            // faz parsing da lista de comando
 
   *n_process = n_process_counter(line_buf);  // conta quantidade de processos

    if (*n_process == 0 || line_buf == NULL)  // se nao tem processo, retorna NULL
        return NULL;

    // aloca espaco para comandos
    char **commands_vector = (char **)malloc(sizeof(char *) * (*n_process));
    
    int aux_i = 0;

    // armazena o primeiro comando
    char *aux_command = strtok(line_buf, "|");
    char *command = strdup(aux_command);
    commands_vector[aux_i++] = command;
    
    // armazena os comandos restantes 
    while (aux_command && (aux_i < 5 && aux_i <= *n_process && *n_process > 1)) {
        aux_command = strtok(NULL, "|");

        if(aux_command){
            char* next_command = strdup(aux_command);
            commands_vector[aux_i++] = next_command;
        }
    }

    free(line_buf);                          // libera string do getline
    return commands_vector;
}

void close_pipe(int n_command, int fd[n_command][2], int not, int rdwt) {
    for (int i = 0; i < n_command; i++) {
        // se indice for diferente do not
        if (i != not) {
            close(fd[i][rdwt]);
        }
    }
}

void open_pipe(int n_commands, int fd[n_commands-1][2]) {
    if (n_commands > 1) {
        for (int i = 0; i < n_commands - 1; i++) {
            // se deu erro com abertura de pipe
            if(pipe(fd[i]) == -1)
                error_pipe();
        }
    }
}