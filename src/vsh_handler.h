#ifndef VSH_HANDLER_H
#define VSH_HANDLER_H

void handle_sigusr_vsh();

void handle_sigusr_sick();

void handle_sigusr_vaccinated();

void handle_sigint();

void handle_sigquit();

void handle_sigtstp();

#endif