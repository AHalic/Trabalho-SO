#include "vsh_commands.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "vsh_io.h"
#include "vsh_errors.h"
#include "vsh_handler.h"

void configure_signals_vsh() {
    // Configura a mascara de sinais bloqueados (SIGINT)
    struct sigaction sa_sigint, sa_sigquit, sa_sigtstp;
    sa_sigint.sa_flags = sa_sigquit.sa_flags = sa_sigtstp.sa_flags = SA_RESTART;

    sa_sigint.sa_handler = &handle_sigint;
    sa_sigquit.sa_handler = &handle_sigquit;
    sa_sigtstp.sa_handler = &handle_sigtstp;
    
    sigaction(SIGINT, &sa_sigint, NULL);
    sigaction(SIGQUIT, &sa_sigquit, NULL);
    sigaction(SIGTSTP, &sa_sigtstp, NULL);
}

int quit_shell(char* command) {
    // Ve se existe a substring armageddon
    if (strstr(command, "armageddon")) {
        free(command);
        return 1;
    }

    return 0;
}

void destroy_commands(char** commands, int n_commands) {
    for (int i = 0; i < n_commands; i++) {
        free(commands[i]);
    }
    free(commands);
}