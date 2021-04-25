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
    return 0;
}

void armageddon(pid_t* pids, int n_commands){
    for(int i = 0; i < n_commands; i++){
        kill(pids[i], SIGTERM);
    }
    
    exit(0);
}
