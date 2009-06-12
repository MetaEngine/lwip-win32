#include <stdio.h>
#include <windows.h>

#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"

#include "arch/sys_arch.h"

static struct sys_timeouts lwip_timeouts[LWIP_TASK_MAX + 1];

#define MAX_MBOX_NB 8

static struct mbox_map 
{
  LPTSTR slotname;
  HANDLE hFile;
  sys_mbox_t mbox;
} mbox_map[MAX_MBOX_NB];

LPTSTR SLOT = TEXT("\\\\.\\mailslot\\lwip_mailslot");

void sys_init(void)
{
  u8_t i;
  for (i = 0; i < MAX_MBOX_NB; ++i)
  {
    mbox_map[i].slotname = TEXT("\\\\.\\mailslot\\lwip_mailslot") + i;
    mbox_map[i].hFile = NULL;
    mbox_map[i].mbox = NULL;
  }
  for(i = 0; i < sizeof(lwip_timeouts) / sizeof(struct sys_timeouts); i++)
    lwip_timeouts[i].next = NULL;
}


/*----------------------------------------------------------------------*/
sys_sem_t sys_sem_new(u8_t count)
{
  sys_sem_t sem = CreateSemaphore(NULL, count, count, NULL);
  
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
  DWORD dwWaitResult = WaitForSingleObject(sem, timeout);
  switch (dwWaitResult) 
  {
  case WAIT_OBJECT_0: 
    return 1;
  case WAIT_ABANDONED:
  case WAIT_TIMEOUT: 
  default:
    return 0;
  }
}


/*----------------------------------------------------------------------*/
sys_mbox_t sys_mbox_new(int size)
{
  u8_t i;
  sys_mbox_t slot;
  for (i = 0; mbox_map[i].mbox != NULL; ++i)
    ;
  slot = CreateMailslot(mbox_map[i].slotname, size, MAILSLOT_WAIT_FOREVER, 
    (LPSECURITY_ATTRIBUTES) NULL);
  
  if (slot == INVALID_HANDLE_VALUE) 
  { 
    fprintf(stderr, "CreateMailslot failed with %d\n", GetLastError());
    return SYS_MBOX_NULL; 
  } 
  mbox_map[i].mbox = slot;
  return slot;
}

/*----------------------------------------------------------------------*/
void sys_mbox_free(sys_mbox_t mbox)
{
  u8_t i;
  for (i = 0; mbox_map[i].mbox != mbox; ++i);
  CloseHandle(mbox);
  CloseHandle(mbox_map[i].hFile);
  mbox_map[i].mbox = NULL;
  mbox_map[i].hFile = NULL;
}

/*----------------------------------------------------------------------*/
void sys_mbox_post(sys_mbox_t mbox, void *msg)
{
  sys_mbox_post(mbox, msg); 
}

err_t sys_mbox_trypost(sys_mbox_t mbox, void *msg) 
{
  DWORD cbWritten; 
  HANDLE hFile;
  u8_t i; 
  BOOL fResult;

  for (i = 0; mbox_map[i].mbox != mbox; ++i);
  if ((hFile = mbox_map[i].hFile) == NULL)
  {
    hFile = CreateFile(mbox_map[i].slotname, 
      GENERIC_WRITE, 
      FILE_SHARE_READ,
      (LPSECURITY_ATTRIBUTES) NULL, 
      OPEN_EXISTING, 
      FILE_ATTRIBUTE_NORMAL, 
      (HANDLE) NULL); 
    
    if (hFile == INVALID_HANDLE_VALUE) 
    { 
      fprintf(stderr, "CreateFile failed with %d.\n", GetLastError()); 
      return ERR_RTE; 
    } 
    mbox_map[i].hFile = hFile;
  }
  fResult = WriteFile(hFile, 
    msg, 
    256,  
    &cbWritten, 
    (LPOVERLAPPED) NULL); 
  
  if (!fResult) 
  { 
    fprintf(stderr, "WriteFile failed with %d.\n", GetLastError()); 
    return ERR_RTE;
  }
  return ERR_OK;
}

/*----------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
  DWORD cbMessage, cMessage, cbRead; 
  BOOL fResult; 
  HANDLE hEvent;
  OVERLAPPED ov;
  DWORD tmp = (DWORD) timeout;
  
  cbMessage = cMessage = cbRead = 0;
  Sleep(timeout);
  
  // Get message count from mail slot
  fResult = GetMailslotInfo(mbox,   // mailslot handle 
    (LPDWORD) NULL,                 // no maximum message size 
    &cbMessage,                     // size of next message 
    &cMessage,                      // number of messages 
    (LPWORD)NULL);                          // no read time-out 
  if (!fResult) 
  { 
    fprintf(stderr, "GetMailslotInfo failed with %d.\n", GetLastError()); 
    return SYS_ARCH_TIMEOUT; 
  }
  if (cMessage == 0)
    return SYS_ARCH_TIMEOUT;
  
  // Get a message from mail slot
  hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ExampleSlot"));
  if( NULL == hEvent )
    return FALSE;
  ov.Offset = 0;
  ov.OffsetHigh = 0;
  ov.hEvent = hEvent;

  *msg = mem_malloc(cbMessage);
  
  fResult = ReadFile(mbox, 
    *msg, 
    cbMessage, 
    &cbRead, 
    &ov); 
  if (!fResult) 
  { 
    fprintf(stderr, "ReadFile failed with %d.\n", GetLastError()); 
    mem_free(*msg); 
    return SYS_ARCH_TIMEOUT; 
  }
  
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
  sys_thread_t t = _beginthreadex( 
    NULL,                   // default security attributes
    stacksize,              // use default stack size  
    (LPTHREAD_START_ROUTINE)thread,                 // thread function name
    arg,                    // argument to thread function 
    0,                      // use default creation flags 
    NULL);   // returns the thread identifier 
  
  if (t == NULL) 
  {
    fprintf(stderr, "CreateThread failed with %d.\n", GetLastError()); 
    ExitProcess(3);
  }
  if (!SetThreadPriority(t, prio))
    fprintf(stderr, "SetThreadPriority failed with %d\n", GetLastError());
  
  return t;
}