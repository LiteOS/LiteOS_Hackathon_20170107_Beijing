/*
**************************
file info:
	author:fang.junpeng
	start time:2017-01-01
	list
***************************
*/

#include "baseTypeDef.h"

#ifndef LIST_FANG_H
#define LIST_FANG_H

/************data struct*****************/
typedef struct Node{
    void* ele;
    struct Node *next;
}Node_t;

/*
front---List head
end----List tail
cursor--current pointer of Node for LinkedList_next function in traverse's Application
len-----num of List's Nodes
*/
typedef struct{
    Node_t *front,*end,*cursor;
    u16 len;//num of the Nodes
}LinkedList_t;

/************List operation*****************/
void LinkedList_init(LinkedList_t*);

//make the cursor to be the front
void LinkedList_reset(LinkedList_t*);

void LinkedList_add(LinkedList_t*,Node_t*);

Node_t* LinkedList_top(LinkedList_t*);

Node_t* LinkedList_end(LinkedList_t*);

Node_t* LinkedList_next(LinkedList_t*);


/*
return:-1---error;0---false;1---true
*/
s8 LinkedList_empty(LinkedList_t*);

u16 LinkedList_len(LinkedList_t*);

#endif

