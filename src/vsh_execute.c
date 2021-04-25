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
 * @param {char* exec}  String do comando que sera executado.
 * @param {char** argv} Vetor de strings em que cada string eh um argumento. argv[0] deve ser o exec.
 * 
 * @return Numero do processo do filho.
 */
static int execute_command_child_fg (char* exec, char** argv) {
    // fork para executar o comando em outro processo
    int pid = fork();

    // se for o filho, rode o comando
    if(!pid)
        if(execvp(exec, argv) == -1){
            int error = error_execvp();
            // free(argv);
            exit(error);
        }

    // espera o filho terminar para continuar 
    int wstatus;
    waitpid(pid, &wstatus, WUNTRACED);

    if(!pid)
        return getpid();
    return pid;
}

/**
 * Para executar os comandos (processos) em background, a shell precisa criar um processo auxiliar para 
 * determinar uma nova sessao de grupo. A saida de um processo, eh a entrada do processo seguinte. Para 
 * isso eh utilizado a funcao dup(). A funcao deve retornar o pid do processo criado. 
 * 
 * @param {char* exec}  String do comando que deve ser executado.
 * @param {char** argv} Vetor de strings em que cada string eh um argumento. argv[0] deve ser o exec.
 * @param {int pos}     Posicao do comando na linha do terminal, iniciando do 0. Ex.: Em "ps1 | ps2 | ps3", 
 *                      ps2 tem pos 1.
 * @param {int n_com}   Numero de comandos na linha do terminal.
 * @param {int fd[][2]} Matriz de pipes.
 * 
 * @return Numero do processo do filho.
 */ 
static int execute_command_child_bg(char* exec, char** argv, int pos, int n_com, int fd[n_com][2]){
    // fork para executar o comando em outro processo
    int pid = fork();
    
    if(!pid) {
        // TODO: dar close nos pipes n usados
        if (!pos) {
            close(1);
            close_pipe(n_com, fd, 0, 1);
            close_pipe(n_com, fd, n_com, 0);
            dup(fd[0][1]);
        }
        else if (pos == n_com) {
            close(0);
            close_pipe(n_com, fd, n_com, 1);
            close_pipe(n_com, fd, pos-1, 0);
            dup(fd[pos-1][0]);
        }
        else {
            close(0);
            close(1);
            close_pipe(n_com, fd, pos, 1);
            close_pipe(n_com, fd, pos-1, 0);
            dup(fd[pos-1][0]);
            dup(fd[pos][1]);
        }
        
        if(execvp(exec, argv) == -1){
            int error = error_execvp();
            free(argv);
            exit(error);
        }
    }

    if(!pid)
        return getpid();
    return pid;
}

int execute_command(char* command, int bg, int fd[bg][2], int pos) {
    char* token = strtok(command, " ");
    char* exec = strdup(token);
    char* argv[4];
    
    // primeiro argumento eh o executavel
    argv[0] = exec;

    // ler argumentos
    int i = 1; 
    while (i < 4) {
        char* token = strtok(NULL, " ");
        if (!token) {
            break;
        } else {
            argv[i++] = token;
        }   
    }

    argv[i] = NULL;

    // TODO: quando armageddon não eh 1o da ruim
    // if (quit_shell(exec)) exit(0);

    pid_t pid;
    if(!bg)
        pid = execute_command_child_fg(exec, argv);
    else
        pid = execute_command_child_bg(exec, argv, pos, bg, fd);

    free(exec);
    return pid;
}

int execute_programs(int n_commands, char** commands_vector) {
    pid_t command_line;
    // faz um fork para auxiliar com a criação dos processos da linha de comando
    // printf("sou shell %d da sessao %d\n", getpid(), getsid(getpid()));
    if ((command_line = fork()) == -1) return error_fork();
    
    // background
    if(!command_line && n_commands > 1) {
        // faz os processos serem da mesma sessao
        // printf("sou commandline %d da sessao %d\n", getpid(), getsid(getpid()));
        
        pid_t process_aux = fork();
        if (!process_aux) {
            pid_t group = setsid(); 

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
            // printf("sou %d da sessao %d\n", getpid(), getsid(getpid()));
            while ((pid = waitpid(0, &status, WNOHANG)) > -1) {
                if (WIFSIGNALED(status)) {
                    // se filho terminou com SIGUSR1, mata todos os filhos
                    if (WTERMSIG(status) == SIGUSR1) {
                        kill(group, SIGKILL);
                    }
                }
            }

            // printf("%d aqui\n", getpid());
            exit(0); // termina processo auxiliar
        }
        exit(1);
    } 
    else if (n_commands == 1) {
        // foreground
        
        if(!command_line){
            // printf("pai %d\n", getpid());
            configure_signals_fg();
            pid_t pid = execute_command(commands_vector[0], n_commands-1, NULL, 0);
            exit(0); // termina processo auxiliar
        }

    }
    // espera o processo auxiliar terminar
    waitpid(command_line, NULL, WUNTRACED);

    return 0;
}