#include "vsh_list.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

SessionList* init_empty_gid_list() {
    return NULL;
}

SessionList* init_gid_list(int gid) {
    SessionList* s_list = (SessionList*) malloc (sizeof(SessionList));
    s_list->gid = gid;
    s_list->next = NULL;

    return s_list;
}

SessionList* add_gid(SessionList* s_list, int gid) {
    SessionList* aux = (SessionList*) malloc (sizeof(SessionList));
    aux->gid = gid;
    aux->next = s_list;
    return aux;
}

void destroy_list(SessionList* s_list) {
    SessionList* aux1, * aux2;
    aux1 = s_list;
    while(aux1) {
        aux2 = aux1->next;
        free(aux1);
        aux1 = aux2;
    }
}