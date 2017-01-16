/*
**************************
file info:
	author:fang.junpeng
	start time:2017-01-02
	queue
***************************
*/

#include "list_fang.h"

typedef LinkedList_t LinkedQueue_t;

void LinkedQueue_init(LinkedQueue_t*);

void LinkedQueue_add(LinkedQueue_t*,Node_t*);

Node_t* LinkedQueue_top(LinkedQueue_t*);

Node_t* LinkedQueue_end(LinkedQueue_t*);

Node_t* LinkedQueue_pop(LinkedQueue_t*);

u16 LinkedQueue_len(LinkedQueue_t*);

s8 LinkedQueue_empty(LinkedQueue_t*);



