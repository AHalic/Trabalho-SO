#ifndef VSH_LIST_H
#define VSH_LIST_H

typedef struct list {
    struct list* next;
    int gid;
} SessionList;

SessionList* init_empty_gid_list();

SessionList* init_gid_list(int gid);

SessionList* add_gid(SessionList* s_list, int gid);

void destroy_list(SessionList* s_list);

#endif