#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct _NODE_ {
    struct _QUEUE_ *queue;
    int p;
    struct _NODE_ *next;
};

struct _PRIORITY_QUEUE_ {
    struct _NODE_ *head;
};



struct _NODE_ *create_priority_node(struct _QUEUE_ *q, int p) {
    struct _NODE_ *n = (struct _NODE_*) malloc(sizeof(struct _NODE_));
    n->queue = q;
    n->p=p;
    n->next = NULL;
    return n;
}

struct _PRIORITY_QUEUE_ *create_priority_queue() {
    struct _PRIORITY_QUEUE_ *p_q = (struct _PRIORITY_QUEUE_*) malloc(sizeof(struct _PRIORITY_QUEUE_));
    if(!p_q) {
        perror("FAILED TO ALLOCATE PRIORITY QUEUE!");
        exit(EXIT_FAILURE);
    }
    p_q->head = NULL;
    return p_q;    
}

void push_pq(struct _PRIORITY_QUEUE_ *p_q, struct _QUEUE_ *q, int priority) {
    struct _NODE_ *head = p_q->head;
    struct _NODE_ *ptr = p_q->head;
    struct _NODE_ *temp = create_priority_node(q, priority);

    if(p_q->head == NULL) {
        p_q->head = create_priority_node(q, priority);
        return;
    }

    if(ptr->p < priority) {
        temp->next = p_q->head;
        p_q->head = temp;
    }

    else {
        while(head->next != NULL && head->next->p > priority) {
            head=head->next;
        }
        temp->next = head->next;
        head->next = temp;
    }

}


void pop_pq(struct _PRIORITY_QUEUE_ *p_q) {
    struct _NODE_ *temp = p_q->head;
    p_q->head = p_q->head->next;
    free (temp);
}

struct _QUEUE_ *peek_pq(struct _PRIORITY_QUEUE_ *p_q) {
    return p_q->head->queue;
}

void display_test(struct _PRIORITY_QUEUE_ *p_q) {
    struct _NODE_ *ptr = p_q->head;
    while(ptr!=NULL) {
        printf("%d -> ", ptr->p);
        ptr=ptr->next;
    }
    printf("NULL\n");
}


#endif
