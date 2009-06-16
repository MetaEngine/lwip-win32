#include "lwip/mem.h"
#include "arch/sys_arch.h"
#include "arch/queue.h"

void enqueue(queue_t* q, void* msg) 
{
  queue_node_t* node;

  node = (queue_node_t *)mem_malloc(sizeof(queue_node_t)); 
  node->msg = msg; 
  node->next = NULL; 
  
  SYS_ARCH_DECL_PROTECT(old_level);
  SYS_ARCH_PROTECT(old_level);
  if (q->head == NULL) 
  { 
    q->head = q->tail = node; 
    // wake up thread waiting on this semaphore
    sys_sem_signal(q->sem);
  } 
  else
  {
    q->tail->next = node;
    q->tail = node;
  }
  q->len += 1;
  SYS_ARCH_UNPROTECT(old_level);
}

void* dequeue(queue_t* q, u32_t timeout)
{
  void* msg;
  queue_node_t* node;

  if (q->head == NULL)
  {
    if (0 == sys_arch_sem_wait(q->sem, timeout) || q->head == NULL)
      return NULL;
  }

  node = q->head;
  msg = node->msg;
  
  SYS_ARCH_DECL_PROTECT(old_level);
  SYS_ARCH_PROTECT(old_level);
  q->head = node->next;
  q->len -= 1;
  // even if q->len is 0, we need not set q->tail to NULL
  mem_free(node);
  SYS_ARCH_UNPROTECT(old_level);
  
  node = NULL;
  return msg;
}

queue_t* queue_create()
{
  queue_t* q;
  q = (queue_t *)mem_malloc(sizeof(queue_t));
  q->head = q->tail = NULL;
  q->len = 0;
  q->sem = sys_sem_new(0);
  return q;
}

void queue_free(queue_t* q)
{
  queue_node_t* node;
  queue_node_t* next;

  node = q->head;

  SYS_ARCH_DECL_PROTECT(old_level);
  SYS_ARCH_PROTECT(old_level);
  while(node != NULL)
  {
    next = node->next;
    mem_free(node);
    node = next;
  }
  sys_sem_free(q->sem);
  mem_free(q);
  q = NULL;
  SYS_ARCH_UNPROTECT(old_level);
}

