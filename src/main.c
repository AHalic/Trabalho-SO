#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "vsh_io.h"
#include "vsh_handler.h"
#include "vsh_commands.h"
#include "vsh_errors.h"

#define true 1
#define false 0

// pega a parte de executar comando que tava na main e faz uma função para isso
// adicionando como seria mandado os sinais
int execute_programs(int n_commands, char** commands_vector) {
    pid_t command_line;

    // faz um fork para auxiliar com a criação dos processos da linha de comando
    if ((command_line = fork()) == -1) return error_fork();
    
    // filho para auxiliar na chamada de processos
    if(!command_line && n_commands > 1) {
        // background
        setsid(); 
        // vejo quem eh pai e filho (pra poder ver se o pai n eh a shell e ver o pid do fork aux)
        printf("COMMAND LINE PID: %d, son of %d\n", getpid(), getppid()); 

        // sa.sa_handler = &handle_sigusr_vaccinated;
        // sigaction(SIGUSR1, &sa, NULL);
 
            // faz os processos dps serem tudo da msm sessao
            
            // aux pra pegar o pid
            pid_t pids[n_commands]; // usado pra guardar o pid dos filhos
            pid_t pid; 
            
            // atualiza o handler para os filhos lidarem com o SIGUSR1 da msm forma 
            signal(SIGUSR1, handle_sigusr_sick);
            // precisa ignorar SIGINT, SIGQUIT e SIGTSTP

            int fd[n_commands - 1][2];
            open_pipe(n_commands, fd);
            for (int i = 0; i < n_commands; i++) {
                pids[i] = execute_command(commands_vector[i], n_commands-1, fd, i);  
                printf("%d\n", pids[i]);
            }
      
            close_pipe(n_commands-1, fd, n_commands, 0);
            close_pipe(n_commands-1, fd, n_commands, 1); 

            int status; // ver o status do pai com os filhos
            int signaledUsr = 0; // ver se foi sinalizado SIGUSR1 pro pai

/*
waitpid(-1, &wstatus, WUNTRACED);
        if (WIFSIGNALED(wstatus)) {
            printf("[PARENT]: Child returned with code %d.\n",
                   WTERMSIG(wstatus)); // no mac 30, no linux 10
        }
        if (WIFSTOPPED(wstatus)) {
            printf("[PARENT]: Child returned with code %d.\n",
                   WSTOPSIG(wstatus)); // no mac 17, no linux 19
        }

    int counter = 0;
    while (pid > 0) {
        pid = waitpid(-1, &status, WNOHANG);
        if (WIFSIGNALED(status)) {
            printf("signaled %d\n", WTERMSIG(status));
            // usa pra verificar se o filho terminou com sigusr1 (talvez seria legal n setar o handler do filho mas sim do pai)
            if (WTERMSIG(status) == SIGUSR1) {
                printf("signed sigusr1\n");
                signaledUsr = 1; // alguem mandou sigusr 1
                break;
            } else if (WTERMSIG(status) == SIGCHILD)
                counter++;
        

        if (counter == n_commands)
            break;
        // if()
        //     printf("Meu filho pid=%d terminou\n", ok);
        // else if (ok == 0)
        //     printf("Nenhum filho terminou\n");
        // else if (ok == -1)
        //     break;
    }

*/



            // // verifica se algum filho sinalizou SIGUSR1 
            // while ((pid = waitpid(0, &status, WNOHANG)) > -1) {
            //     // usado para verificar o termino dos filhos foi anormal 
            //     if (WIFSIGNALED(status)) {
            //         printf("signaled %d\n", WTERMSIG(status));
            //         // usa pra verificar se o filho terminou com sigusr1 (talvez seria legal n setar o handler do filho mas sim do pai)
            //         if (WTERMSIG(status) == SIGUSR1) {
            //             printf("signed sigusr1\n");
            //             signaledUsr = 1; // alguem mandou sigusr 1
            //             break;
            //         }
            //     }
            // }

            int counter = 0;
            while (1) {
                if (pid = waitpid(0, &status, WNOHANG) < 0) printf("oi\n");
                if (WIFSIGNALED(status)) {
                    printf("signaled %d\n", WTERMSIG(status));
                    // usa pra verificar se o filho terminou com sigusr1 (talvez seria legal n setar o handler do filho mas sim do pai)
                    if (WTERMSIG(status) == SIGUSR1) {
                        printf("signed sigusr1\n");
                        signaledUsr = 1; // alguem mandou sigusr 1
                        break;
                    } else if (WTERMSIG(status) == SIGCHLD)
                        counter++;
                }

                if (counter == n_commands)
                    break;
            
            }     

            // se alguem mandou o sigusr1
            if (signaledUsr) {
                // manda sinal para todos os filhos (talvez eh estranho mandar pro filho q mandou usr1? 
                // e se ele ja terminou, oq acontece?) 
                // tou mandando SIGUSR! mas precisaria modificar a funcao de handler
                for (int i = 0; i < n_commands; i++) {
                    if (pids[i] != 0) {
                        printf("pid: %d\n", pids[i]);
                        kill(pids[i], SIGTERM);
                    }
                }
                
                printf("oi\n");
                // manda pra shell a mensagem do jacare
            }
        }

        raise(SIGTERM);
    } 
    else if (n_commands == 1) {
        // foreground
        signal(SIGUSR1, SIG_IGN);
        pid_t pid = execute_command(commands_vector[0], n_commands-1, NULL, 0);
        wait(NULL);
    }
}

int main(int argc, char* argv[]) {
    // Configura a mascara de sinais bloqueados (SIGINT)
    // configure_signals_vsh();

    // struct sigaction sa = {0};
    // sa.sa_flags = SA_RESTART;
    // sa.sa_handler = &handle_sigusr_vsh;
    // sigaction(SIGUSR1, &sa, NULL);

    int n_commands = 0;
    // handle_sigusr_vaccinated();
    printf("SHELL PID: %d\n", getpid());
    // signal(SIGUSR1, handle_sigusr_vaccinated);

    // ler primeira linha antes do loop
    do {
        show_command_line();
        char** commands_vector = read_command_line(&n_commands);

        if (n_commands == 0) continue;

        if (quit_shell(commands_vector[0])) {
            free(commands_vector);
            return 0;
        }
        
        execute_programs(n_commands, commands_vector);
        destroy_commands(commands_vector, n_commands);

    } while (true);

    return 0;    
}
