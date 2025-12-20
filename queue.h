#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>


struct _NODE_QUEUE_ {
    void *data;
    struct _NODE_QUEUE_  *next; 
};

struct _QUEUE_ {
    struct _NODE_QUEUE_ *first;
    struct _NODE_QUEUE_ *last;
};



struct _NODE_QUEUE_ *create_node_queue(void *data) {
    struct _NODE_QUEUE_ *n_queue = (struct _NODE_QUEUE_*) malloc(sizeof(struct _NODE_QUEUE_));
    n_queue->data = data;
    n_queue->next = NULL;
    return n_queue;
}

struct _QUEUE_ *create_queue() {
    struct _QUEUE_ *new_queue = (struct _QUEUE_*) malloc(sizeof(struct _QUEUE_));
    if(!new_queue) {
        perror("FAILED TO ALLOCATE QUEUE!");
        exit(EXIT_FAILURE);
    }
    new_queue->last = NULL;
    new_queue->first = new_queue->last;
    return new_queue;
}


void enqueue(struct _QUEUE_ *queue, void *data) {
    struct _NODE_QUEUE_ *new_node_queue = create_node_queue(data);
    if(queue->last == NULL) {
        queue->first = queue->last = new_node_queue;
        return;
    }
    queue->last->next = new_node_queue;
    queue->last = new_node_queue;
}


void dequeue(struct _QUEUE_ *queue) {
    if(queue->first==NULL && queue->last==NULL) {
        return;
    }
    struct _NODE_QUEUE_ *temp = queue->first;
    queue->first = queue->first->next;
    if(queue->first==NULL) {
        queue->last = NULL;
    }
    free(temp);
}



#endif