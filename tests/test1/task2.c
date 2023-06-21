#include <stdio.h>
#include <stdlib.h>

typedef struct{
    double *results;
    int len;
} Data;

typedef struct tagQueueElement{
    Data *data;
    struct tagQueueElement *next;
} QueueElement;

typedef struct tagQueue{
    // bez wartowników
    QueueElement *head; // pierwszy element
    QueueElement *tail; // ostatni element
    // size_t len;
} Queue;

void init_queue(Queue *pqueue){
    pqueue = (Queue*)malloc(sizeof(Queue));
    pqueue->head = NULL;
    pqueue->tail = NULL;
}

void free_queue(Queue *pqueue){
    QueueElement *curr = pqueue->head;
    QueueElement *next;

    while(curr){
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(pqueue);
}

void push_copy(Queue *pqueue, const Data *pdata){
    QueueElement *new = (QueueElement*)malloc(sizeof(QueueElement));
    new->data = pdata;
    pqueue->tail->next = new;
    pqueue->tail = new;
    // pqueue->len++;
}

int peek(const Queue *pqueue, Data *pdata){
    if(!pqueue->head)
        return 0;
    
    *pdata = *pqueue->head->data; // kopiuje dane, mam nadzieję, że to o to chodziło
    return 1;
}

int pop(Queue *pqueue, Data *pdata){
    if(!pqueue->head)
        return -1;
    
    pdata = pqueue->head->data;
    QueueElement *to_delete = pqueue->head;
    pqueue->head = pqueue->head->next;
    free(to_delete);
    if(!pqueue->head)
        return 0;
    return 1;
}

int get_total_count(const Queue *pqueue){
    int res = 0;
    QueueElement *curr = pqueue->head;
    while(curr){
        res += curr->data->len;
        curr = curr->next;
    }
    return res;
}

int main(){

    return 0;
}