// Arquivo de testes do comando Libera Moita

// includes:
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

// defines: 
#define PAUSE_BETWEEN_LAUNCHES 2
#define CHILD_PATH "./maybe_it_works"

// code: 
static void liberamoita(int signum) {
    static int errors = 0; // filhos com erro
    static int total = 0; // filhos que terminaram
    float percent = 0; // percentual
    int status = 0; // status

    // loop nos filhos mortos
    while(waitpid(-1, &status, WNOHANG) > 0) {
        // atualiza contador
        if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
            errors++; // incrementa os erros
        }
        total++; // incrementa os acertos
    }   
    // print
    percent = (total != 0) ? (float)(errors) / total * 100 : 0;
    printf("Errors: %d, Total: %d, Percent: %.2f%%\n", errors, total, percent);
}

int main(int argc, char *argv[]) {
    pid_t pid = 0;
    int sleep_remaining = 0; // remaining time to sleep

    // set handler to SIGCHLD
    if (signal(SIGCHLD, liberamoita) == SIG_ERR) {
        printf("Erro handler\n");
        exit(EXIT_FAILURE);
    }

        // sleep the expected time, even if an interruption occurs
        sleep_remaining = PAUSE_BETWEEN_LAUNCHES;
        while ((sleep_remaining = sleep(sleep_remaining)) > 0) { }
        pid = fork();
        if (pid >= 0) { // fork successful
            if (pid == 0) { // child
                if (! execl(CHILD_PATH, CHILD_PATH, (char *)(NULL))) { // exec child program
                    printf("Error no exec\n");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            printf("Error no fork\n");
        }

    return 0;
}