#include "vsh_execute.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "vsh_errors.h"
#include "vsh_handler.h"
#include "vsh_commands.h"
#include "vsh_io.h"

/**
 * Para executar um comando (processo) em foreground, a shell precisa criar um processo auxiliar (um filho)
 * para poder executar a funcao execvp(). Por ser um processo que deve rodar em foreground, eh preciso esperar 
 * o processo auxiliar terminar. A função deve retornar o pid do processo filho criado.
 * @param {char*}exec  String do comando que sera executado.
 * @param {char**}argv Vetor de strings em que cada string eh um argumento. argv[0] deve ser o exec.
 * 
 * @return Numero do processo do filho.
 */
static int execute_command_child_fg (char* exec, char** argv) {
    int pid;
    if((pid = fork()) == -1) return error_fork(); // fork para executar o comando em outro processo

    if(!pid)                                      // se for o filho, rode o comando
        if(execvp(exec, argv) == -1){
            int error = error_execvp();
            exit(error);
        }

    int wstatus;
    waitpid(pid, &wstatus, WUNTRACED);            // espera o filho terminar para continuar 

    return pid;
}

/**
 * Para executar os comandos (processos) em background, a shell precisa criar um processo auxiliar para 
 * determinar uma nova sessao de grupo. A saida de um processo, eh a entrada do processo seguinte. Para 
 * isso eh utilizado a funcao dup(). A funcao deve retornar o pid do processo criado. 
 * 
 * @param {char*}exec  String do comando que deve ser executado.
 * @param {char**}argv Vetor de strings em que cada string eh um argumento. argv[0] deve ser o exec.
 * @param {int}pos     Posicao do comando na linha do terminal, iniciando do 0. Ex.: Em "ps1 | ps2 | ps3", 
 *                      ps2 tem pos 1.
 * @param {int}n_com   Numero de comandos na linha do terminal.
 * @param {int}fd[][2] Matriz de pipes.
 * 
 * @return Numero do processo do filho.
 */ 
static int execute_command_child_bg(char* exec, char** argv, int pos, int n_com, int fd[n_com][2]){
    int pid;
    if((pid = fork()) == -1) return error_fork(); // fork para executar o comando em outro processo

    // fd[n_com][0] = leitura
    // fd[n_com][1] = escrita

    if(!pid) {
        if (!pos) {                            // se for o primeiro comando, lê stdin, escreve pipe
            close(1);                          // fecha stdout
            close_pipe(n_com, fd, 0, 1);       // fecha os pipes que nao vao ser utilizados para escrita
            close_pipe(n_com, fd, n_com, 0);   // fecha os pipes que nao vao ser utilizados para leitura

            if(dup(fd[0][1]) == -1) return error_dup();      // para escrever no pipe 
            close(fd[0][1]);                   // fecha pipe usada
        }
        else if (pos == n_com) {               // se for o ultimo comando, lê pipe, escreve stdout
            close(0);                          // fecha stdin
            close_pipe(n_com, fd, n_com, 1);   // fecha os pipes que nao vao ser utilizados para escrita
            close_pipe(n_com, fd, pos-1, 0);   // fecha os pipes que nao vao ser utilizados para leitura
            if(dup(fd[pos-1][0]) == -1) return error_dup();  // para ler do ultimo pipe
            close(fd[pos-1][0]);               // fecha pipe usada
        }
        else {                                 // caso nao for nem o primeiro nem o ultimo comando, lê e escreve em pipes
            close(0);                          // fecha stdin
            close(1);                          // fecha stdout
            close_pipe(n_com, fd, pos, 1);     // fecha os pipes que nao vao ser utilizados para escrita
            close_pipe(n_com, fd, pos-1, 0);   // fecha os pipes que nao vao ser utilizados para leitura
            if(dup(fd[pos-1][0]) == -1) return error_dup();      // para ler do pipe pos - 1
            if(dup(fd[pos][1]) == -1) return error_dup();        // para escrever no pipe pos
            close(fd[pos-1][0]);               // fecha pipe usada
            close(fd[pos][1]);                 // fecha pipe usada
        }
        
        // executa comando
        if(execvp(exec, argv) == -1){
            int error = error_execvp();
            if(argv != NULL)
                free(argv);
            exit(error);
        }
    }

    return pid;
}

int execute_command(char* command, int bg, int fd[bg][2], int pos) {
    char* token = strtok(command, " ");
    char* exec = strdup(token);
    char* argv[4];
    
    argv[0] = exec; // primeiro argumento eh o executavel

    // ler e separa argumentos
    int i = 1; 
    while (i < 4) {
        char* token = strtok(NULL, " ");
        if (!token) {
            break;
        } 
        else {
            argv[i++] = token;
        }   
    }

    argv[i] = NULL;  // ultimo comando eh NULL

    // executa programas e recebe seu pid
    pid_t pid;
    if(!bg)
        pid = execute_command_child_fg(exec, argv);
    else
        pid = execute_command_child_bg(exec, argv, pos, bg, fd);

    free(exec);
    return pid;
}

int execute_programs(int n_commands, char** commands_vector) {
    // faz um fork para auxiliar com a criação dos processos da linha de comando
    pid_t command_line;
    if ((command_line = fork()) == -1) return error_fork();

    if(command_line && n_commands == 1) {
        // Caso seja a vsh e foi inserido apenas um processo na linha de comandos

        configure_signals_vsh_ignore();
        int status = 0;
        // espera o processo auxiliar terminar
        pid_t pid = waitpid(command_line, &status, WUNTRACED);
    }
    else if (!command_line) {
        // Caso seja o processo auxiliar

        if (n_commands > 1) {
            // Caso tenha mais de um comando (rodam em background)
            pid_t group;
            if((group = setsid()) == -1) return error_setsid(); 

            sigset_t mask;
            sigemptyset(&mask);
            sigaddset(&mask, SIGINT);
            sigaddset(&mask, SIGQUIT);
            sigaddset(&mask, SIGTSTP);

            pid_t pids[n_commands];        // usado pra guardar o pid dos filhos
            pid_t pid;                     // aux pra pegar o pid
            int status;                    // ver o status do pai com os filhos
            int signaledUsr = 0;           // ver se foi sinalizado SIGUSR1 pro pai
            int fd[n_commands - 1][2];     // vetor de pipes

            open_pipe(n_commands, fd);
            for (int i = 0; i < n_commands; i++) {
                // executa programas e retorna pid
                pids[i] = execute_command(commands_vector[i], n_commands-1, fd, i);   
            }
        
            close_pipe(n_commands-1, fd, n_commands, 0);
            close_pipe(n_commands-1, fd, n_commands, 1); 

            // verifica estado dos filhos
            while ((pid = waitpid(0, &status, WNOHANG)) > -1) {
                if (WIFSIGNALED(status)) {
                    // se filho terminou com SIGUSR1, mata todos os filhos
                    if (WTERMSIG(status) == SIGUSR1 || WTERMSIG(status) == SIGUSR2) {
                        kill(group * -1, SIGKILL);
                    }
                }
            }

            exit(0); // termina processo auxiliar
        } 
        else if (n_commands == 1){
            // Caso tenha apenas um comando (roda em foreground)

            configure_signals_fg();
            pid_t pid = execute_command(commands_vector[0], n_commands-1, NULL, 0);
            exit(0); // termina processo auxiliar
        }

        exit(0);
    }

    return command_line;    // retorna pid do fork (que eh usado como gid de background)
}
