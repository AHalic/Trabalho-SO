#include "vsh_io.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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
    char* line_buf = NULL, *aux;
    size_t line_buf_size = 0;
    // printf("\n");

    // lendo linha de comando
    getline(&line_buf, &line_buf_size, stdin);

    // retirando o caracter \n ou \r do fim da string (lido pelo getline)
    line_buf[strcspn(line_buf, "\r\n")] = 0; 
    // printf("comando %s.\n", line_buf);    

    *n_process = n_process_counter(line_buf);
    // printf("%d\n", *n_process);

    // se nao tem processo, retorna NULL
    if (*n_process == 0) return NULL;


    // aloca espaco para comandos
    char** commands_vector = (char**) malloc (sizeof(char*) * (*n_process));
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

    // free(line_buf);

    return commands_vector;
}

void execute_command(char* command){
    char* token = strtok(command, " ");
    char* exec = token;
    // printf("exec %s %s %s\n", exec, command, token);
    char** argv = (char**) malloc (sizeof(char*) * 4);
    argv[0] = "bg"; // parametro de rodar em background
    int i = 1; 

    while (token) {
        char* token = strtok(NULL, " ");
        if (!token) {
            // eu n sei pq sem esse if tava dando errado
            // printf("sou null\n");
            break;
        }
        
        else {
            argv[i] = token;
            // printf("%s", argv[i]);
        }
        
    }

    
    
    
    // do
    // {
        
    //     argv[++i] = strtok(NULL, " ");
    //     if (argv == NULL)
    //     {
    //         printf("oie sou null\n");
    //         return;
    //     }
    //     printf("argv %s\n", argv[i]);
    // } while (argv[i] != NULL);
    
    // printf("%s %s\n", exec, argv);
    if(!fork())
        if(execvp(exec, argv) == -1)
            printf("nao foi exec\n");

    return;
}