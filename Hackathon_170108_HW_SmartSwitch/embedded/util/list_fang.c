#include "list_fang.h"

#define NULL (0)
#define nullptr ((void*)0)
#define TRUE (1)
#define FALSE (0)

void LinkedList_init(LinkedList_t* list){
    //check
    if(NULL== list)
        return;
    list->front = list->end = list->cursor = NULL;
    list->len = 0;
}

void LinkedList_reset(LinkedList_t*list){
    //check
    if(NULL== list)
        return;
    list->cursor = list->front;
}


void LinkedList_add(LinkedList_t*list,Node_t*node){
    Node_t* endNode;
    //check
    if(NULL== list || NULL == node)
        return;
    if(0 == list->len){
        list->front = list->end = list->cursor = node;
    }
    else{
        endNode = list->end;
        endNode->next = list->end = node;
        node->next = NULL;
    }
    list->len++;
}

Node_t* LinkedList_top(LinkedList_t*list){
    //check
    if(NULL== list)
        return NULL;
    if(0 == list->len)
        return NULL;
    else
        return list->front;
}

Node_t* LinkedList_end(LinkedList_t*list){
    //check
    if(NULL== list)
        return NULL;
    if(0 == list->len)
        return NULL;
    else
        return list->end;
}

Node_t* LinkedList_next(LinkedList_t*list){
    Node_t* node;
    //check
    if(NULL== list)
        return NULL;
    if(0 == list->len)
        return NULL;
    if(list->cursor == list->end){
        node = list->end;
        list->cursor = NULL;
    }
    else{
        node = list->cursor;
        list->cursor = (list->cursor)->next;
    }
    return node;
}

s8 LinkedList_empty(LinkedList_t*list){
    //check
    if(NULL== list)
        return -1;
    if(0 == list->len)
        return TRUE;
    else
        return FALSE;
}

u16 LinkedList_len(LinkedList_t*list){
    //check
    if(NULL== list)
        return 0;
    return list->len;
}
