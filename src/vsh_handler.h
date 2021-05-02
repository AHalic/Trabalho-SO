#ifndef VSH_HANDLER_H
#define VSH_HANDLER_H

/**
 * Função que trata o sinal SIGUSR1 e 
 * SIGUSR2 recebido pela vsh
**/
void handle_sigusr_vsh();

/**
 * Função que configura a máscara de sinais da vsh
 * A máscara define como os sinais SIGINT, SIGQUIT, SITSTP
 * são tratados, todos são tratados da forma padrão
**/
void configure_signals_vsh();

/**
 * Função que configura a máscara de sinais da vsh
 * A máscara define como os sinais SIGINT, SIGQUIT, SITSTP, 
 * SIGUSR1 e SIGUSR2 são tratados.
 * SIGINT, SIGQUIT, SITSTP - sinais bloqueados
 * SIGUSR1, SIGUSR2 - função definida handle_sigusr_vsh
**/
void configure_signals_vsh_sigusr();

/**
 * Função que configura a máscara de sinais de processos foreground
 * A máscara define como os sinais SIGINT, SIGQUIT, SITSTP, 
 * SIGUSR1 e SIGUSR2 são tratados.
 * SIGINT, SIGQUIT, SITSTP - sinais bloqueados
 * SIGUSR1, SIGUSR2 - sinais ignorados
**/
void configure_signals_fg();

/**
 * Função que configura a máscara de sinais da vsh
 * A máscara define como os sinais SIGINT, SIGQUIT, SITSTP
 * são tratados, todos são ignorados
**/
void configure_signals_vsh_ignore();

#endif