#include "vsh_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigusr_vsh() {
    printf("\n"
            "              _  _\n"
            "    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.\n"
            "   (.(.)----'`        ^^'                /^   ^^-._\n"
            "    (    `                 \\             |    _    ^^-._\n"
            "    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-.\n"
            "     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )\n"
            "              (((`   (((`          (((`  (((`        `'--'^\n"
            "I feel weird...\n");
}

void configure_signals_vsh() {
    struct sigaction sa_dft = {0};
    sa_dft.sa_flags =  SA_RESTART;
    sa_dft.sa_handler = SIG_DFL;

    sigaction(SIGINT, &sa_dft, NULL);
    sigaction(SIGQUIT, &sa_dft, NULL);
    sigaction(SIGTSTP, &sa_dft, NULL);  
}

void configure_signals_vsh_sigusr() {
    struct sigaction block_signal = {0};
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTSTP);

    block_signal.sa_mask = mask;
    block_signal.sa_handler = &handle_sigusr_vsh;
    block_signal.sa_flags = SA_RESTART;

    sigaction(SIGUSR1, &block_signal, NULL);
    sigaction(SIGUSR2, &block_signal, NULL);
}

void configure_signals_vsh_ignore() {  
    struct sigaction sa_ign = {0};
    sa_ign.sa_flags = SA_RESTART;
    sa_ign.sa_handler = SIG_IGN;

    sigaction(SIGINT, &sa_ign, NULL);
    sigaction(SIGQUIT, &sa_ign, NULL);
    sigaction(SIGTSTP, &sa_ign, NULL);
}

void configure_signals_fg() {
    struct sigaction sa_dft = {0}, sa_sigusr = {0};
    sa_dft.sa_flags =  sa_sigusr.sa_flags = SA_RESTART;

    sa_dft.sa_handler = SIG_DFL;
    sa_sigusr.sa_handler = SIG_IGN;
    
    sigaction(SIGINT, &sa_dft, NULL); 
    sigaction(SIGQUIT, &sa_dft, NULL);
    sigaction(SIGTSTP, &sa_dft, NULL);

    struct sigaction block_signal = {0};
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTSTP);

    block_signal.sa_mask = mask;
    block_signal.sa_handler = SIG_IGN;
    block_signal.sa_flags = SA_RESTART;
    
    sigaction(SIGUSR1, &block_signal, NULL);
    sigaction(SIGUSR2, &block_signal, NULL);
}

