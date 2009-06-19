// lwipwin32.cpp : Defines the entry point for the console application.
//
#include <stdio.h>

#include "lwip/netif.h" 
#include "lwip/tcpip.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "net_tap.h"

extern err_t ethernetif_init(struct netif *netif);

void init()
{
  struct ip_addr ip, mask, gw;
  static struct netif netif;

  tcpip_init(NULL, NULL);

  IP4_ADDR(&ip, 192, 168, 80, 161);
  IP4_ADDR(&mask, 255, 255, 255, 0);
  IP4_ADDR(&gw, 192, 168, 80, 201);

  netif_add(&netif, &ip, &mask, &gw, NULL, ethernetif_init, tcpip_input);
  netif_set_default(&netif);
  netif_set_up(&netif);
}

int main(int argc, char* argv[])
{
  // see SYS_ARCH_PROTECT
  InitializeCriticalSection(&gCriticalSection);

  if (ERR_IF == open_tap())
    exit(1);

  init();

  while(TRUE)
    Sleep(1000);

  // Todo close_dev();

  return 0;
}

