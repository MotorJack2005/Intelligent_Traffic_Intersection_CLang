#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct _STACK_ {
    struct _NODE_STACK_ {
        void *data;
        struct _NODE_STACK_ *next;
    }*head;
    
};

struct _NODE_STACK_ *create_node_stack(void *data) {
    struct _NODE_STACK_ *s = (struct _NODE_STACK_*) malloc(sizeof(struct _NODE_STACK_));
    s->data = data;
    s->next = NULL;
    return s;
}

struct _STACK_ *create_stack() {
    struct _STACK_ *new_stack = (struct _STACK_*)malloc(sizeof(struct _STACK_));
    if(!new_stack) {
        perror("FAILED TO ALLOCATE STACK!");
        exit(EXIT_FAILURE);
    }
    new_stack->head = NULL;
    return new_stack;
}

void *peek(struct _STACK_ *s) {
    return s->head;
}

void push(struct _STACK_ *s, void *data) {
    struct _NODE_STACK_ *p = create_node_stack(data);

    if(s->head == NULL) {
        s->head = p;
        return;
    }
    p->next = s->head;
    s->head = p;
}

void pop(struct _STACK_ *s) {
    if(s->head == NULL) {
        perror("STACK IS EMPTY!");
        exit(EXIT_FAILURE);
    }
    struct _NODE_STACK_ *p = s->head;
    s->head = s->head->next;
    free(p);
}


#endif
