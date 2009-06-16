#include "lwip/err.h"

#include "pcap.h"

pcap_t *tap;

err_t open_tap()
{
  pcap_if_t *alldevs;
  pcap_if_t *d;
  char errbuf[PCAP_ERRBUF_SIZE];
  int inum, i=0;
  
  /* Retrieve the device list */
  if(pcap_findalldevs(&alldevs, errbuf) == -1)
  {
    fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
    return ERR_IF;
  }
  
  /* Print the list */
  for(d = alldevs; d; d = d->next)
  {
    printf("%d. %s", ++i, d->name);
    if (d->description)
      printf(" (%s)\n", d->description);
    else
      printf(" (No description available)\n");
  }
  
  if(i == 0)
  {
    printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
    return ERR_IF;
  }
  
  printf("Enter the interface number (1-%d):",i);
  scanf("%d", &inum);
  
  if(inum < 1 || inum > i)
  {
    printf("\nInterface number out of range.\n");
    /* Free the device list */
    pcap_freealldevs(alldevs);
    return ERR_IF;
  }
  
  /* Jump to the selected adapter */
  for(d = alldevs, i = 0; i < inum-1 ;d = d->next, i++);
  
  /* Open the adapter */
  if ((tap = pcap_open_live(d->name,
    65536, // portion of the packet to capture. 
    1, // promiscuous mode (nonzero means promiscuous)
    1, // read timeout
    errbuf // error buffer
    )) == NULL)
  {
    fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
    /* Free the device list */
    pcap_freealldevs(alldevs);
    return ERR_IF;
  }
  
  printf("\nlistening on %s...\n", d->description);
  
  /* At this point, we don't need any more the device list. Free it */
  pcap_freealldevs(alldevs);
  return ERR_OK;
}

err_t close_tap()
{
  pcap_close(tap); 
  tap = NULL;
  return ERR_OK;
}

err_t get_packet(unsigned int* len, char** pkt_data) 
{
  int res;
  struct pcap_pkthdr *header;
  while((res = pcap_next_ex(tap, &header, pkt_data)) >= 0){
    
    if(res == 0)
      /* Timeout elapsed */
      continue;
    
    *len = header->len;
    return ERR_OK;
  }
  
	fprintf(stderr, "Error reading the packets: %s\n", pcap_geterr(tap));
  exit(1);
  return ERR_IF;
}

err_t send_packet(char** buf, int len)
{
  	if (pcap_sendpacket(tap,	// Adapter
		*buf,				// buffer with the packet
		len					// size
		) != 0)
	{
		fprintf(stderr, "\nError sending the packet: \n", pcap_geterr(tap));
		return ERR_IF;
	}
    return ERR_OK;
}