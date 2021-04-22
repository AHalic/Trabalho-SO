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
    printf("vsh > ");
}

static int n_process_counter(char *line_buf) {
    // conta a quantidade de processo na linha
    if (line_buf == NULL || !strlen(line_buf))
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

static char* parseInput() {
    char *line_buf = NULL;
    size_t line_buf_size = 0;

    // lendo linha de comando
    int charRead = getline(&line_buf, &line_buf_size, stdin);

    // se tem mais que o char EOF (no minimo 2 char)
    if (charRead > 1) {
        line_buf[strlen(line_buf) - 1] = '\0';
        rewind(stdin);
    }
    else { 
        // se linha de comando for vazia
        rewind(stdin);
        return NULL;
    }

    return line_buf;
}

char **read_command_line(int *n_process) {
    char* line_buf = parseInput();

    *n_process = n_process_counter(line_buf);

    // se nao tem processo, retorna NULL
    if (*n_process == 0 || line_buf == NULL)
        return NULL;

    if (quit_shell(line_buf)) exit(0);

    // aloca espaco para comandos
    char **commands_vector = (char **)malloc(sizeof(char *) * (*n_process)); // free
    
    int aux_i = 0;

    // ler a primeira linha
    char *aux_command = strtok(line_buf, "|");
    char *command = strdup(aux_command);
    commands_vector[aux_i++] = command;
    
    // verifica se o commando token eh null
    while (aux_command && (aux_i < 5 && *n_process <= 5)) {
        aux_command = strtok(NULL, "|");

        if(aux_command){
            char* next_command = strdup(aux_command);
            commands_vector[aux_i++] = next_command;
        }
    }

    free(line_buf);
    return commands_vector;
}

void close_pipe(int n_command, int fd[n_command][2], int not, int rdwt) {
    for (int i = 0; i < n_command; i++) {
        if (i != not) {
            close(fd[i][rdwt]);
        }
    }
}

void open_pipe(int n_commands, int fd[n_commands][2]) {
    if (n_commands > 1) {
        // int fd[n_commands - 1][2];
        for (int i = 0; i < n_commands - 1; i++) {
            if(pipe(fd[i]) == -1)
                error_pipe();
        }
    }
}