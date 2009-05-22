#include <process.h>

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "arch/sys_arch.h"
#include "netif/etharp.h"
#include "pcapif.h"

#define IFNAME0 'e'
#define IFNAME1 'n'

struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};

static void  ethernetif_input(struct netif *netif);

static void low_level_init(struct netif *netif)
{
  struct ethernetif *ethernetif = netif->state;
  
  netif->hwaddr_len = ETHARP_HWADDR_LEN;
  netif->hwaddr[0] = 0x00;
  netif->hwaddr[1] = 0x15;
  netif->hwaddr[2] = 0xd5;
  netif->hwaddr[3] = 0x28;
  netif->hwaddr[4] = 0x35;
  netif->hwaddr[5] = 0x23;
  netif->mtu = 1500;
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
  
  /* Do whatever else is needed to initialize interface. */
  sys_thread_new("ether_input", ethernetif_input, netif, 0, 0);
}

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  struct ethernetif *ethernetif = netif->state;
  struct pbuf *q;
  char* buf;
  char* index;

#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
  
  index = buf = mem_malloc(p->tot_len);
  for(q = p; q != NULL; q = q->next) {
    memcpy(index, q->payload, q->len);
    index += q->len;
  }
  send_packet(&buf, p->tot_len);
  mem_free(buf);
  
#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
  
  LINK_STATS_INC(link.xmit);
  
  return ERR_OK;
}

static struct pbuf * low_level_input(struct netif *netif)
{
  struct ethernetif *ethernetif = netif->state;
  struct pbuf *p, *q;
  u32_t len;
  char* pkt_data;
  char* data;
  
  get_packet(&len, &pkt_data);
  
#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif
  
  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
  if (p != NULL) {
    
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    
    for(q = p, data = pkt_data; q != NULL; q = q->next) {
      memcpy(q->payload, data, q->len);
      data += q->len;
    }
    
#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
    
    LINK_STATS_INC(link.recv);
  } else {
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
  }
  
  return p;  
}

err_t ethernetif_init(struct netif *netif)
{
  struct ethernetif *ethernetif;
  
  LWIP_ASSERT("netif != NULL", (netif != NULL));
  
  ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }
  
#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */
  
  /*
  * Initialize the snmp variables and counters inside the struct netif.
  * The last argument should be replaced with your link speed, in units
  * of bits per second.
  */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 100 * 1024 * 1024);
  
  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
  * You can instead declare your own function an call etharp_output()
  * from it if you have to do some checks before sending (e.g. if link
  * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;
  
  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
  
  /* initialize the hardware */
  low_level_init(netif);
  
  return ERR_OK;
}

static void ethernetif_input(struct netif *netif)
{
  struct ethernetif *ethernetif;
  struct eth_hdr *ethhdr;
  struct pbuf *p;
  
  ethernetif = netif->state;
  
  /* move received packet into a new pbuf */
  while (TRUE)
  {
    p = low_level_input(netif);
    if (p == NULL) 
      continue;
    
    ethhdr = p->payload;
    switch (htons(ethhdr->type)) {
    case ETHTYPE_IP:
      /* update ARP table */
      etharp_ip_input(netif, p);
      /* skip Ethernet header */
      //pbuf_header(p, -(14+ETH_PAD_SIZE));
      /* pass to network layer */
      netif->input(p, netif);
      break;
    case ETHTYPE_ARP:
      etharp_arp_input(netif, ethernetif->ethaddr, p);
      break;
#if PPPOE_SUPPORT
      /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
      /* full packet send to tcpip_thread to process */
      if (netif->input(p, netif)!=ERR_OK)
      { 
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
        pbuf_free(p);
        p = NULL;
      }
      break;
    default:
      pbuf_free(p);
      p = NULL;
      break;
    }
  }
}
