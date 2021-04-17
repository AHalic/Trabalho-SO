#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "messages.h"

#define true 1
#define false 0

int main() {
    // Configura a mascara de sinais bloqueados 
    // DUVIDA SE PRECISA
    // sigset_t shell_mask;
    // sigemptyset(&shell_mask);
    // sigaddset(&shell_mask, SIGINT);
    // sigprocmask(SIG_SETMASK, &shell_mask, NULL);




    //le o resto do arquivo e retorna n
    char* line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;

    // ler primeira linha fora do loop
    do
    {
        show_command_line();
        getline(&line_buf, &line_buf_size, stdin);

        // Retirando o caracter \n ou \r do fim da string (lido pelo getline)
        line_buf[strcspn(line_buf, "\r\n")] = 0; 
        // printf("%s\n", line_buf);
    } while (strcmp(line_buf, "armageddon"));
    
    
    free(line_buf);

}