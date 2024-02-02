
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tftp.h"
#include "tftp_support.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define min(x,y) (x<y ? x : y)
#define true        1

/*                                                                          */
/* number of bytes actually read, or (-1) if an error occurs.               */
/* On error, *err will hold the reason.                                     */
/*                                                                          */

int
tftp_get(char *filename,
         struct sockaddr_in *server,
         char *buf,
         int len,
         int mode,
         int *err)
{
    int res = 0;
    int s, actual_len, data_len, recv_len, from_len;
    static int get_port = 7700;
    struct sockaddr_in local_addr, server_addr, from_addr;
    char data[SEGSIZE+sizeof(struct tftphdr)];
    struct tftphdr *hdr = (struct tftphdr *)data;
    char *cp, *fp;
    struct timeval timeout;
    unsigned short last_good_block = 0;
    struct servent *server_info;
    fd_set fds;
    int total_timeouts = 0;

    *err = 0;  // Just in case

    // Create initial request
    hdr->th_opcode = htons(RRQ);  // Read file
    cp = (char *)&hdr->th_stuff;
    fp = filename;
    while (*fp) *cp++ = *fp++;
    *cp++ = '\0';
    if (mode == TFTP_NETASCII) {
        fp = "NETASCII";
    } else if (mode == TFTP_OCTET) {
        fp = "OCTET";
    } else {
        *err = TFTP_INVALID;
        return -1;
    }
    while (*fp) *cp++ = *fp++;
    *cp++ = '\0';
    server_info = getservbyname("tftp", "udp");
    if (server_info == (struct servent *)0) {
        *err = TFTP_NETERR;
        return -1;
    }

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        // Couldn't open a communications channel
        *err = TFTP_NETERR;
        return -1;
    }
    memset((char *)&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
//  structure has no field named sin_len
/// local_addr.sin_len = sizeof(local_addr);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(get_port++);
    if (bind(s, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        // Problem setting up my end
        *err = TFTP_NETERR;
        close(s);
        return -1;
    }
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
//  structure has no field named sin_len
/// server_addr.sin_len = sizeof(server_addr);
    server_addr.sin_addr = server->sin_addr;
    if (server->sin_port == 0) {
        server_addr.sin_port = server_info->s_port; // Network order already
    } else {
        server_addr.sin_port = server->sin_port;
    }

    // Send request
    if (sendto(s, data, sizeof(data), 0, 
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        // Problem sending request
        *err = TFTP_NETERR;
        close(s);
        return -1;
    }

    // Read data
    fp = buf;
    while (true) {
        timeout.tv_sec = TFTP_TIMEOUT_PERIOD;
        timeout.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(s, &fds);
        if (select(s+1, &fds, 0, 0, &timeout) <= 0) {
            if ((++total_timeouts > TFTP_TIMEOUT_MAX) || (last_good_block == 0)) {
                // Timeout - no data received
                *err = TFTP_TIMEOUT;
                close(s);
                return -1;
            }
            // Try resending last ACK
            hdr->th_opcode = htons(ACK);
            hdr->th_block = htons(last_good_block);
            if (sendto(s, data, 4 /* FIXME */, 0, 
                       (struct sockaddr *)&from_addr, from_len) < 0) {
                // Problem sending request
                *err = TFTP_NETERR;
                close(s);
                return -1;
            }
        } else {
            recv_len = sizeof(data);
            from_len = sizeof(from_addr);
            if ((data_len = recvfrom(s, &data, recv_len, 0, 
                                     (struct sockaddr *)&from_addr, &from_len)) < 0) {
                // What happened?
                *err = TFTP_NETERR;
                close(s);
                return -1;
            }
            if (ntohs(hdr->th_opcode) == DATA) {
                actual_len = 0;
                if (ntohs(hdr->th_block) == (last_good_block+1)) {
                    // Consume this data
                    cp = hdr->th_data;
                    data_len -= 4;  /* Sizeof TFTP header */
                    actual_len = data_len;
                    res += actual_len;
                    while (data_len-- > 0) {
                        if (len-- > 0) {
                            *fp++ = *cp++;
                        } else {
                            // Buffer overflow
                            *err = TFTP_TOOLARGE;
                            close(s);
                            return -1;
                        }
                    }
                    last_good_block++;
                }
                // Send out the ACK
                hdr->th_opcode = htons(ACK);
                hdr->th_block = htons(last_good_block);
                if (sendto(s, data, 4 /* FIXME */, 0, 
                           (struct sockaddr *)&from_addr, from_len) < 0) {
                    // Problem sending request
                    *err = TFTP_NETERR;
                    close(s);
                    return -1;
                }
                if ((actual_len >= 0) && (actual_len < SEGSIZE)) {
                    // End of data
                    close(s);
                    return res;
                }
            } else 
            if (ntohs(hdr->th_opcode) == ERROR) {
                *err = ntohs(hdr->th_code);
                close(s);
                return -1;
            } else {
                // What kind of packet is this?
                *err = TFTP_PROTOCOL;
                close(s);
                return -1;
            }
        }
    }
}

/*                                                                          */
/* Send data to a file on a server via TFTP.                                */
/*                                                                          */
int
tftp_put(char *filename,
         struct sockaddr_in *server,
         char *buf,
         int len,
         int mode,
         int *err)
{
    int res = 0;
    int s, actual_len, data_len, recv_len, from_len;
    static int put_port = 7800;
    struct sockaddr_in local_addr, server_addr, from_addr;
    char data[SEGSIZE+sizeof(struct tftphdr)];
    struct tftphdr *hdr = (struct tftphdr *)data;
    char *cp, *fp, *sfp;
    struct timeval timeout;
    unsigned short last_good_block = 0;
    struct servent *server_info;
    fd_set fds;
    int total_timeouts = 0;

    *err = 0;  // Just in case

    server_info = getservbyname("tftp", "udp");
    if (server_info == (struct servent *)0) {
        *err = TFTP_NETERR;
        return -1;
    }

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        // Couldn't open a communications channel
        *err = TFTP_NETERR;
        return -1;
    }
    memset((char *)&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
//  structure has no field named sin_len
/// local_addr.sin_len = sizeof(local_addr);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(put_port++);
    if (bind(s, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        // Problem setting up my end
        *err = TFTP_NETERR;
        close(s);
        return -1;
    }
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
//  structure has no field named sin_len
/// server_addr.sin_len = sizeof(server_addr);
    server_addr.sin_addr = server->sin_addr;
    if (server->sin_port == 0) {
        server_addr.sin_port = server_info->s_port; // Network order already
    } else {
        server_addr.sin_port = server->sin_port;
    }

    while (1) {
        // Create initial request
        hdr->th_opcode = htons(WRQ);  // Create/write file
        cp = (char *)&hdr->th_stuff;
        fp = filename;
        while (*fp) *cp++ = *fp++;
        *cp++ = '\0';
        if (mode == TFTP_NETASCII) {
            fp = "NETASCII";
        } else if (mode == TFTP_OCTET) {
            fp = "OCTET";
        } else {
            *err = TFTP_INVALID;
            return -1;
        }
        while (*fp) *cp++ = *fp++;
        *cp++ = '\0';
        // Send request
        if (sendto(s, data, sizeof(data), 0, 
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            // Problem sending request
            *err = TFTP_NETERR;
            close(s);
            return -1;
        }
        // Wait for ACK
        timeout.tv_sec = TFTP_TIMEOUT_PERIOD;
        timeout.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(s, &fds);
        if (select(s+1, &fds, 0, 0, &timeout) <= 0) {
            if (++total_timeouts > TFTP_TIMEOUT_MAX) {
                // Timeout - no ACK received
                *err = TFTP_TIMEOUT;
                close(s);
                return -1;
            }
        } else {
            recv_len = sizeof(data);
            from_len = sizeof(from_addr);
            if ((data_len = recvfrom(s, &data, recv_len, 0, 
                                     (struct sockaddr *)&from_addr, &from_len)) < 0) {
                // What happened?
                *err = TFTP_NETERR;
                close(s);
                return -1;
            }
            if (ntohs(hdr->th_opcode) == ACK) {
                // Write request accepted - start sending data
                break;
            } else 
            if (ntohs(hdr->th_opcode) == ERROR) {
                *err = ntohs(hdr->th_code);
                close(s);
                return -1;
            } else {
                // What kind of packet is this?
                *err = TFTP_PROTOCOL;
                close(s);
                return -1;
            }
        }
    }

    // Send data
    sfp = buf;
    last_good_block = 1;
    while (res < len) {
        // Build packet of data to send
        data_len = min(SEGSIZE, len-res);
        hdr->th_opcode = htons(DATA);
        hdr->th_block = htons(last_good_block);
        cp = hdr->th_data;
        fp = sfp;
        actual_len = data_len + 4;
        // FIXME - what about "netascii" data?
        while (data_len-- > 0) *cp++ = *fp++;
        // Send data packet
        if (sendto(s, data, actual_len, 0, 
                   (struct sockaddr *)&from_addr, from_len) < 0) {
            // Problem sending request
            *err = TFTP_NETERR;
            close(s);
            return -1;
        }
        // Wait for ACK
        timeout.tv_sec = TFTP_TIMEOUT_PERIOD;
        timeout.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(s, &fds);
        if (select(s+1, &fds, 0, 0, &timeout) <= 0) {
            if (++total_timeouts > TFTP_TIMEOUT_MAX) {
                // Timeout - no data received
                *err = TFTP_TIMEOUT;
                close(s);
                return -1;
            }
        } else {
            recv_len = sizeof(data);
            from_len = sizeof(from_addr);
            if ((data_len = recvfrom(s, &data, recv_len, 0, 
                                     (struct sockaddr *)&from_addr, &from_len)) < 0) {
                // What happened?
                *err = TFTP_NETERR;
                close(s);
                return -1;
            }
            if (ntohs(hdr->th_opcode) == ACK) {
                if (ntohs(hdr->th_block) == last_good_block) {
                    // Advance pointers, etc
                    sfp = fp;
                    res += (actual_len - 4);
                    last_good_block++;
                } else {
                    printf("Send block #%d, got ACK for #%d\n", 
                                last_good_block, ntohs(hdr->th_block));
                }
            } else 
            if (ntohs(hdr->th_opcode) == ERROR) {
                *err = ntohs(hdr->th_code);
                close(s);
                return -1;
            } else {
                // What kind of packet is this?
                *err = TFTP_PROTOCOL;
                close(s);
                return -1;
            }
        }
    }
    close(s);
    return res;
}

static char buf[32*1024];

#define GETFILE "srch1"
#define PUTFILE "hehe"

void dump_buf(char *buf,unsigned int size);

void *simple_program1 (void *q)
{
    int res, err, len;
    struct sockaddr_in host;

    fprintf(stderr,"Start TFTP test\n");
    memset((char *)&host, 0, sizeof(host));
//  structure has no field named sin_len
/// host.sin_len = sizeof(host);
    host.sin_family = AF_INET;
/// host.sin_addr = bp->bp_siaddr;
    host.sin_addr.s_addr = inet_addr("192.168.100.99");
    host.sin_port = 0;
    fprintf(stderr,"Trying tftp_get %s %16s...\n", GETFILE, inet_ntoa(host.sin_addr));
    res = tftp_get( GETFILE, &host, buf, sizeof(buf), TFTP_OCTET, &err);
    fprintf(stderr,"res = %d, err = %d\n", res, err);
    if (res > 0) {
       dump_buf(buf, min(res,1024));
    }
    len = res;
    fprintf(stderr,"Trying tftp_put %s %16s, length %d\n",
                PUTFILE, inet_ntoa(host.sin_addr), len);
    res = tftp_put( PUTFILE, &host, buf, len, TFTP_OCTET, &err);
    fprintf(stderr,"put - res: %d\n", res);
    return 0;
}

void dump_buf(char *buf,unsigned int size)
{
    unsigned int i;
    for(i=0;i<size;i++){
        putchar(i);putchar('-');
    }
    putchar(13);putchar(10);
}

pthread_t simple_threadA;

int main(void)
{
    pthread_attr_t attr1;

    /* if a thread requires joining ,consider explicitly creating it as joinable */
    /* this provides portability as not all implementations may create threads as*/
    /* joinable by default */ 

    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);

    pthread_create(&simple_threadA, &attr1, simple_program1, NULL);
    pthread_join(simple_threadA, NULL);

    // ??? can it be used for two threads
    // ??? can it be released before join
    pthread_attr_destroy(&attr1);

    pthread_exit(NULL);
}
