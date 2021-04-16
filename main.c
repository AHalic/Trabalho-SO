#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "messages.h"

#define true 1
#define false 0

int main() {
    // Configura a mascara de sinais bloqueados 
    // DUVIDA SE PRECISA
    sigset_t shell_mask;
    sigemptyset(&shell_mask);
    sigaddset(&shell_mask, SIGINT);
    sigprocmask(SIG_SETMASK, &shell_mask, NULL);

    char *comandos;
    char* line_buf = NULL;

    while(true) {
        show_command_line();
        getline(&line_buf, NULL, stdin);  // n funcionou
        
    }
}