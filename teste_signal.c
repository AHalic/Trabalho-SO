#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    int pid, status;
    if(!fork()){
        printf("ssou 1 %d\n", getpid());
        sleep(25);
        exit(1);
    }
    else if (!fork()) {
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
                    kill(pid, SIGKILL);
                }
            }
        }
    }
}