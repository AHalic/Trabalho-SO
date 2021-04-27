#include "vsh_list.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

session_list* init_empty_gid_list() {
    return NULL;
}

session_list* add_gid(session_list* s_list, int gid) {
    session_list* aux = (session_list*) malloc (sizeof(session_list));
    aux->gid = gid;
    aux->next = s_list;
    return aux;
}

void destroy_list(session_list* s_list) {
    session_list* aux1, * aux2;
    aux1 = s_list;
    while(aux1) {
        aux2 = aux1->next;
        free(aux1);
        aux1 = aux2;
    }
}