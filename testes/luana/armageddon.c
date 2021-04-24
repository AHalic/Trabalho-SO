// Arquivo de testes do comando Armageddon

// includes:
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


// kill( all pids, SIG_TERM );
// thats all folks

int main(){
    // int bit = 0;
    pid_t pid;

    if((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }

    if(pid == 0){
        //O código aqui dentro será executado no processo filho
        printf("pid do filho: %i\n", getpid());
    } else{
        //O código neste trecho será executado no processo pai
        printf("pid do pai: %i\n", getpid());
    }


    printf("Esta regiao sera executada por ambos processos\n\n");

    // while(bit != 1){ 
        puts("oi\n");
        if(pid == 0){
            printf("filho %i morre\npei\n", getpid());
            kill(pid, SIGTERM);
        }

        else if(pid == 1){
            printf("eu sou seu pai %i\n", getpid());
        }

        else{
            printf("saindo do loop\n");
            // bit = 1;
        } 
    // }

    printf("UE %i\n", getpid());
    exit(0);
}
