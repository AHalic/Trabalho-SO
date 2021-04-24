#include "vsh_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void configure_sigaction_fg() {
    
}

void configure_sigaction_bg() {

}

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

void handle_sigusr_sick() {
    printf("SICK PID: %d, PPID: %d GROUP:%d\n", getpid(), getppid(), getgid());
    exit(SIGUSR1);
}

void handle_sigusr_vaccinated() {
    printf("I am dad %d\n", getpid());
}

void handle_sigint() {
    return;
}

void handle_sigquit() {
    return;
}

void handle_sigtstp() {
    return;
}
