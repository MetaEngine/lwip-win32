#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "lwip/sys.h"

typedef struct queue_node
{
  void* msg;
  struct queue_node* next;
} queue_node_t;

typedef struct queue
{
  struct queue_node* head;
  struct queue_node* tail;
  u32_t enqueue;            // enqueue counter
  u32_t dequeue;            // dequeue counter
  sys_sem_t sem;            // semaphore for dequeue
} queue_t;

void enqueue(queue_t* q, void* msg);
void* dequeue(queue_t* q, u32_t timeout);
queue_t* queue_create();
void queue_free(queue_t* q);

#endif