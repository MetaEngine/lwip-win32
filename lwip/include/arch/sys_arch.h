#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define LWIP_STK_SIZE      4096

#define LWIP_TASK_MAX    5	//max number of lwip tasks
#define LWIP_START_PRIO  -1   //first prio of lwip tasks

typedef HANDLE sys_sem_t;
typedef struct queue *sys_mbox_t;
typedef HANDLE sys_thread_t;

#define SYS_MBOX_NULL   (sys_mbox_t)0
#define SYS_SEM_NULL    (sys_sem_t)0

/* Global Critical Region Protection */
CRITICAL_SECTION gCriticalSection;

#define SYS_ARCH_DECL_PROTECT(x)
#define SYS_ARCH_PROTECT(x) EnterCriticalSection(&gCriticalSection)
#define SYS_ARCH_UNPROTECT(x) LeaveCriticalSection(&gCriticalSection)

#endif
