#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma   warning(disable:4761)  

#define LWIP_STK_SIZE      4096

#define LWIP_TASK_MAX    5	//max number of lwip tasks
#define LWIP_START_PRIO  -1   //first prio of lwip tasks

typedef HANDLE sys_sem_t;
typedef HANDLE sys_mbox_t;
typedef HANDLE sys_thread_t;

#define SYS_MBOX_NULL   (sys_mbox_t)0
#define SYS_SEM_NULL    (sys_sem_t)0

/* Critical Region Protection */
CRITICAL_SECTION gCriticalSection;

#define SYS_ARCH_DECL_PROTECT(lev)
#define SYS_ARCH_PROTECT(lev) EnterCriticalSection(&gCriticalSection)
#define SYS_ARCH_UNPROTECT(lev) LeaveCriticalSection(&gCriticalSection)

#endif
