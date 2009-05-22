// lwipwin32.cpp : Defines the entry point for the console application.
//
#include <stdio.h>

#include "lwip/netif.h" 
#include "lwip/tcpip.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "pcapif.h"

void init_lwip();
void set_lwip();

extern err_t ethernetif_init(struct netif *netif);

int main(int argc, char* argv[])
{
  // see SYS_ARCH_PROTECT
  InitializeCriticalSection(&gCriticalSection);
  
  if (ERR_IF == open_dev())
    exit(1);

  init_lwip();
  set_lwip();

  // start for ever
  while(TRUE)
    Sleep(1000);
  //close_dev();
  
  return 0;
}

void init_lwip()
{
  sys_init();
  mem_init();
  memp_init();
  pbuf_init();
  tcpip_init(NULL, NULL);
}

void set_lwip()
{
  struct ip_addr addr, netmask, gateway;
  static struct netif e_netif;
  
  netif_init();
  
  IP4_ADDR(&addr, 192, 168, 80, 161);
  IP4_ADDR(&netmask, 255, 255, 255, 0);
  IP4_ADDR(&gateway, 192, 168, 80, 201);
  
  netif_add(&e_netif, &addr, &netmask, &gateway, 
    NULL, ethernetif_init, tcpip_input);
  netif_set_default(&e_netif);
  netif_set_up(&e_netif);
}