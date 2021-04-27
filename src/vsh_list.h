#ifndef VSH_LIST_H
#define VSH_LIST_H

// Lista de group id.
// next: proximo item da lista
// gid: valor do group id
typedef struct list {
    struct list* next;
    int gid;
} session_list;

/**
 * Inicializa uma lista vazia
 * 
 * @return NULL
 */ 
session_list* init_empty_gid_list();

/**
 * Adiciona gid para lista.
 * @param {session_list* s_list} Lista de gid.
 * @param {int gid} Valor de gid.
 * 
 * @return lista com gid adicionado.
 */
session_list* add_gid(session_list* s_list, int gid);

/**
 * Libera memoria alocada por lista.
 * @param {session_list* s_list} Lista de gid.
 */
void destroy_list(session_list* s_list);

#endif