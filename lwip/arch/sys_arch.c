#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <stdio.h>

#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"

#include "arch/sys_arch.h"
#include "arch/queue.h"

static struct sys_timeouts lwip_timeouts[LWIP_TASK_MAX + 1];

void sys_init(void)
{
  u8_t i;
  for(i = 0; i < sizeof(lwip_timeouts) / sizeof(struct sys_timeouts); i++)
    lwip_timeouts[i].next = NULL;
}

/*----------------------------------------------------------------------*/
sys_sem_t sys_sem_new(u8_t count)
{
  sys_sem_t sem = CreateSemaphore(NULL, count, MAXLONG, NULL);
  
  if (sem == INVALID_HANDLE_VALUE) 
  {
    fprintf(stderr, "CreateSemaphore error: %d\n", GetLastError());
    return SYS_SEM_NULL;
  }
  return sem;
}

/*----------------------------------------------------------------------*/
void sys_sem_free(sys_sem_t sem)
{
  CloseHandle(sem);
}

/*----------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t sem)
{
  if (!ReleaseSemaphore(sem, 1, NULL)) 
  {
    fprintf(stderr, "ReleaseSemaphore error: %d\n", GetLastError());
  }
}


/*----------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{
  DWORD dwWaitResult = WaitForSingleObject(sem, (timeout != 0) ? timeout : INFINITE);
  switch (dwWaitResult) 
  {
  case WAIT_OBJECT_0: 
    return ERR_OK;
  case WAIT_ABANDONED:
  case WAIT_TIMEOUT: 
  default:
    return SYS_ARCH_TIMEOUT;
  }
}


/*----------------------------------------------------------------------*/
sys_mbox_t sys_mbox_new(int size)
{
  return queue_create();
}

/*----------------------------------------------------------------------*/
void sys_mbox_free(sys_mbox_t mbox)
{
  queue_free(mbox);
}

/*----------------------------------------------------------------------*/
void sys_mbox_post(sys_mbox_t mbox, void *msg)
{
  sys_mbox_trypost(mbox, msg); 
}

err_t sys_mbox_trypost(sys_mbox_t mbox, void *msg) 
{
  enqueue(mbox, msg);
  return ERR_OK;
}

/*----------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
  *msg = dequeue(mbox, timeout);
  if (*msg == NULL)
    return SYS_ARCH_TIMEOUT;
  return 0;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t mbox, void **msg)
{
  return sys_arch_mbox_fetch(mbox, msg, 0);
}

/*----------------------------------------------------------------------*/
struct sys_timeouts * sys_arch_timeouts(void)
{
  u8_t prio = GetThreadPriority(GetCurrentThread());
  u8_t offset = prio - LWIP_START_PRIO;
  if (prio == THREAD_PRIORITY_ERROR_RETURN) 
  {
    fprintf(stderr, "CreateThread failed with %d.\n", GetLastError()); 
    return &lwip_timeouts[LWIP_TASK_MAX];
  }
  if (offset >= 0 && offset < LWIP_TASK_MAX)
    return &lwip_timeouts[offset];
  return &lwip_timeouts[LWIP_TASK_MAX];
}


/*------------------------------------------------------------------------*/
sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{
  sys_thread_t t = (sys_thread_t)_beginthreadex( 
    NULL,                            // default security attributes
    stacksize,                       // use default stack size  
    (LPTHREAD_START_ROUTINE)thread,  // thread function name
    arg,                             // argument to thread function 
    0,                               // use default creation flags 
    NULL);                           // returns the thread identifier 
  
  if (t == NULL) 
  {
    fprintf(stderr, "CreateThread failed with %d.\n", GetLastError()); 
    ExitProcess(3);
  }
  if (!SetThreadPriority(t, prio))
    fprintf(stderr, "SetThreadPriority failed with %d\n", GetLastError());
  
  return t;
}