#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdlib.h>


struct _LIST_NODE_ {
        void *data;
        struct _LIST_NODE_ *next;
};
struct _LINKEDLIST_ {
     struct _LIST_NODE_ *head;
};



struct _LIST_NODE_ *create_list_node(void *data) {
    struct _LIST_NODE_ *n = (struct _LIST_NODE_*)malloc(sizeof(struct _LIST_NODE_));
    n->data = data;
    n->next = NULL;
    return n;
}


struct _LINKEDLIST_ *create_list() {
    struct _LINKEDLIST_ *l = (struct _LINKEDLIST_*)malloc(sizeof(struct _LINKEDLIST_));
    l->head = NULL;
    return l;
}


void add_element(struct _LINKEDLIST_ *l, void *data) {
    struct _LIST_NODE_ *ptr = l->head;
    if(l->head == NULL) {
        l->head = create_list_node(data);
    }
    else {
        while(ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = create_list_node(data);
    }
}


#endif
