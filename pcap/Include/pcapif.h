#ifndef __PCAP_IF_H__
#define __PCAP_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

err_t open_dev();
err_t close_dev();
err_t get_packet(unsigned int* len, char** pkt_data);
err_t send_packet(char** buf, int len);

#ifdef __cplusplus
}
#endif

#endif