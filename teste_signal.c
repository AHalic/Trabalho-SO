#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    int pid, status, filhos[2];
    if((filhos[0] = fork()) == 0){
        printf("ssou 1 %d\n", getpid());
        sleep(25);
        exit(1);
    }
    else if ((filhos[1] = fork()) == 0) {
        printf("ssou 2 %d\n", getpid());
        sleep(25);
        exit(2);
    }
    else {
        sleep(5);
        while ((pid = waitpid(0, &status, WNOHANG)) > -1) {
            if (WIFSIGNALED(status)) {
                printf("signaled: %d\n", WTERMSIG(status));

                if (WTERMSIG(status) == SIGUSR1) {
                    printf("achei usr, o pid é %d\n", pid);
                    if (pid) {
                        for (int i = 0; i < 2; i++) {
                            kill(filhos[i], SIGKILL);
                        }
                        
                    }
                    
                }
            }
        }
    }
}