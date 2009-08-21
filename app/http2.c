#include "lwip/api.h"

const static char indexdata[] =
"<html> \
        <head><title>A test page</title></head> \
        <body> \
        This is a small test page. \
        </body> \
        </html>";
const static char http_html_hdr[] = 
"HTTP/1.0 200 OK\r\n\
Content-type: text/html\r\n\r\n";

static void process_connection(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *rq;
    u16_t len;
    
    inbuf = netconn_recv(conn);
    netbuf_data(inbuf, &rq, &len);
    /* HTTP "GET /\r\n" */
    if(rq[0] == 'G' && rq[1] == 'E' && rq[2] == 'T') {
        netconn_write(conn, http_html_hdr, sizeof(http_html_hdr),
                NETCONN_NOCOPY);
        netconn_write(conn, indexdata, sizeof(indexdata),
                NETCONN_NOCOPY);
        netconn_close(conn);
    }
}

void http_task()
{
    struct netconn *conn, *newconn;
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 80);
    netconn_listen(conn);
    
    while(1) {
        newconn = netconn_accept(conn);
        process_connection(newconn);
        if (newconn == NULL)
          continue;
        netconn_delete(newconn);
    }
}

