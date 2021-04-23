#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void handle() {
    printf("I'm sick\n");
}

int main() {
    int n_commands = 4;
    pid_t pids[4];
    int signaledUsr = 0;
    int status; 
    signal(SIGUSR1, handle);

    pid_t pid = getpid();

    if (pid) {
        for (int i = 0; i < n_commands; i++) {
            printf("status: %d\n", signaledUsr);

            if(pid) {
                pid = fork();
                pids[i] = pid;
                if(!pid)
                    printf("pid: %d ppid: %d\n", getpid(), getppid());
            } else if (i == 2) {
                sleep(2);
                raise(SIGUSR1);
            }

            if(pid) {
                // while ((pid = waitpid(0, &status, WNOHANG)) > -1) {
                pid = waitpid(0, &status, WNOHANG);
                
                if(pid < -1) return 1;

                if (WIFSIGNALED(status)) {
                    printf("i: %d signaled %d\n", i, WTERMSIG(status));
                    // usa pra verificar se o filho terminou com sigusr1 (talvez seria legal n setar o handler do filho mas sim do pai)
                    if (WTERMSIG(status) == SIGUSR1) {
                        printf("signed sigusr1\n");
                        signaledUsr = 1; // alguem mandou sigusr 1
                        break;
                    }
                }
            }
        }
    }


    return 0;
}