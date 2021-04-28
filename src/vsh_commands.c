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
#include "vsh_list.h"


int quit_shell(char* command, session_list* s_list) {
    // Ve se existe a substring armageddon

    if (strstr(command, "armageddon")) {
        int group;
        for(session_list* aux = s_list; aux != NULL; aux = aux->next){
            group = aux->gid * -1;
            kill(group, SIGKILL);
        }
        return 1;
    }

    return 0;
}

int destroy_zombies(char* command, pid_t group_pid){
    int status;
    pid_t pid;
    group_pid *= -1;

    if (strstr(command, "liberamoita")){
        while((pid = waitpid(-1, &status, WNOHANG)) > -1) {
        }  
        
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