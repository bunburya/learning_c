#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brainfuck.h"

QUEUE *new_queue() {
    QUEUE *new = (QUEUE *) malloc(sizeof(QUEUE));
    new->first = 0;
    new->last = 0;
    new->current = 0;
    new->len = 0;
    return new;
}

NODE *new_node() {
    NODE *new = (NODE *) malloc(sizeof(NODE));
    if (new) {
        memset(new, 0, sizeof(NODE));
    }
    return new;
}

void append(QUEUE *q, int val) {
    NODE *new = new_node();
    new->val = val;
    new->prev = q->last;
    new->next = 0;
    if (q->first == 0) {
        q->first = new;
        q->last = new;
    } else {
        q->last->next = new;
        q->last = new;
    }
    q->len++;
}

int popleft(QUEUE *q) {
    NODE *old = q->first;
    /*if (old == 0) {
        printf("POPPING FROM EMPTY QUEUE\n");
    }*/
    int val = old->val;
    q->first = old->next;
    q->len--;
    if (q->first != 0) {
        q->first->prev = 0;
    }
    free(old);
    return val;
}

int next(QUEUE *q) {
    if (q->current == 0) {
        q->current = q->first;
    } else {
        q->current = q->current->next;
    }
    if (q->current == 0) {
        return EOF;
    } else {
        return q->current->val;
    }
}

QUEUE *copy(QUEUE *q) {
    /*printf("creating copy\n");*/
    QUEUE *new = new_queue();
    int i;
    NODE *prior = q->current;
    q->current = 0;
    while ((i = next(q)) != EOF) {
        /*printf("copying %d\n", i);*/
        append(new, i);
    }
    q->current = prior;
    return new;
}
