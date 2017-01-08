#include "queue_fang.h"

void LinkedQueue_init(LinkedQueue_t*queue){
    LinkedList_init(queue);
}

void LinkedQueue_add(LinkedQueue_t*queue,Node_t*node){
    LinkedList_add(queue,node);
}

Node_t* LinkedQueue_top(LinkedQueue_t*queue){
    return LinkedList_top(queue);
}
Node_t* LinkedQueue_end(LinkedQueue_t*queue){
    return LinkedList_end(queue);
}
Node_t* LinkedQueue_pop(LinkedQueue_t*queue){
    Node_t* node;
    node = LinkedList_next(queue);
    if(0 != node)
        queue->front = queue->cursor;
    return node;
}

u16 LinkedQueue_len(LinkedQueue_t*queue){
    return LinkedList_len(queue);
}

s8 LinkedQueue_empty(LinkedQueue_t*queue){
    return LinkedList_empty(queue);
}
