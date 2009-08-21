#include "lwip/tcp.h"
#include "lwip/ip_addr.h"
/* This is the data for the actual web page. */
static char indexdata[] =
"HTTP/1.0 200 OK\r\n\
Content-type: text/html\r\n\
\r\n\
<html> \
<head><title>A test page</title></head> \
<body> \
This is a small test page. \
</body> \
</html>";

/* This is the callback function that is called
when a TCP segment has arrived in the connection. */
static err_t http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	char *rq;
	/* If we got a NULL pbuf in p, the remote end has closed
	the connection. */
	if(p != NULL) {
		/* The payload pointer in the pbuf contains the data
		in the TCP segment. */
		rq = p->payload;
		/* Check if the request was an HTTP "GET / HTTP/1.1". */
		if(rq[0] == 'G' && rq[1] == 'E' && rq[2] == 'T') {
				/* Send the web page to the remote host. A zero
				in the last argument means that the data should
				not be copied into internal buffers. */
				tcp_write(pcb, indexdata, sizeof(indexdata), 0);
		}
		/* Free the pbuf. */
		pbuf_free(p);
	}
	/* Close the connection. */
	tcp_close(pcb);
	return ERR_OK;
}

/* This is the callback function that is called when
a connection has been accepted. */
static err_t http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	/* Set up the function http_recv() to be called when data
	arrives. */
	tcp_recv(pcb, http_recv);
	return ERR_OK;
}

/* The initialization function. */
void http_init(void)
{
	struct tcp_pcb *pcb;
	/* Create a new TCP PCB. */
	pcb = tcp_new();
	/* Bind the PCB to TCP port 80. */
	tcp_bind(pcb, NULL, 80);
	/* Change TCP state to LISTEN. */
	pcb = tcp_listen(pcb);
	/* Set up http_accet() function to be called
	when a new connection arrives. */
	tcp_accept(pcb, http_accept);
}

