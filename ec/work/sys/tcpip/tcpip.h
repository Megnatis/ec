

#if ((defined H225_GATEKEEPER_SIDE) || (defined H225_GATEWAY_SIDE))
#define GKPR_UDP_PORT_RAS1 1718
#define GKPR_UDP_PORT_RAS2 1719
#endif

#ifdef H225_GATEWAY_SIDE
#define GW_UDP_PORT_RAS 	19840
#endif
#define TCP_PORT_CS		1720

#ifdef ETHERNET_INSTALLED

//SHARAB start
struct socket {
    gint32 address;	     /* IP address */
    gint16 port;	     /* port number */
};
#define NULLSOCK	(struct socket *)0

/* Generic socket address structure */
struct sockaddr {
    short sa_family;
    char sa_data[14];
};

/* This is a structure for "historical" reasons (whatever they are) */
struct in_addr {
    unsigned long s_addr;
};

/* Socket address, DARPA Internet style */
struct sockaddr_in {
    short sin_family;
    unsigned short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

//SHARAB end

#define ETH_RCV_MODE_TURN_OFF_RECEIVER				  1
#define ETH_RCV_MODE_RECEIVE_ONLY_PACKETS_SENT_TO_THIS_INTERFACE  2
#define ETH_RCV_MODE_MODE_2_PLUS_BROADCAST_PACKETS		  3
#define ETH_RCV_MODE_MODE_3_PLUS_LIMITED_MULTICAST_PACKETS	  4
#define ETH_RCV_MODE_MODE_3_PLUS_ALL_MULTICAST_PACKETS		  5
#define ETH_RCV_MODE_ALL_PACKETS				  6

typedef struct {
	gint class;	 /* Interface class (ether/slip/etc) */
	gint int_no;	 /* Interrupt vector */
	short handle;	 /* Driver handle */
	unsigned int iface_no;
	_go32_dpmi_seginfo rmcb_seginfo;
	_go32_dpmi_registers rmcb_registers;
	gint32 dosbase;
	gint32 dossize;
	gint32 wptr;	 /* write pointer into DOS buffer */
	gint32 rptr;	 /* write pointer into DOS buffer */
	gint32 cnt;	 /* Count of unread bytes in buffer */
	gint32 overflows;
	gint32 rcvd_len;
	gint32 to_be_added_cnt;
}PACKET_DRIVER_STRUCT;

#define EADDR_LEN	6
/* Format of an Ethernet header */
typedef struct {
	guint8 dest[EADDR_LEN];
	guint8 source[EADDR_LEN];
	guint16 type;
}ETHER_STRUCT;

#define ETHERLEN	14
/* Ethernet type fields */
#define IP_TYPE 	0x800	/* Type field for IP */
#define ARP_TYPE	0x806	/* Type field for ARP */
#define REVARP_TYPE	0x8035	/* Type field for reverse ARP */

#define RUNT		60	/* smallest legal size packet, no fcs */
#define GIANT		1514	/* largest legal size packet, no fcs */


/* Packet driver interface types (not a complete list) */
#define TC500		1
#define PC2000		10
#define WD8003		14
#define PC8250		15
#define ANYTYPE 	0xffff

/* Packet driver function call numbers. From Appendix B. */
#define DRIVER_INFO		1
#define ACCESS_TYPE		2
#define RELEASE_TYPE		3
#define SEND_PKT		4
#define TERMINATE		5
#define GET_ADDRESS		6
#define RESET_INTERFACE 	7
#define GET_PARAMETERS		10
#define AS_SEND_PKT		11
#define SET_RCV_MODE		20
#define GET_RCV_MODE		21
#define SET_MULTICAST_LIST	22
#define GET_MULTICAST_LIST	23
#define GET_STATISTICS		24
#define SET_ADDRESS		25

/* Packet driver error return codes. From Appendix C. */

#define NO_ERROR	0
#define BAD_HANDLE	1	/* invalid handle number */
#define NO_CLASS	2	/* no interfaces of specified class found */
#define NO_TYPE 	3	/* no interfaces of specified type found */
#define NO_NUMBER	4	/* no interfaces of specified number found */
#define BAD_TYPE	5	/* bad packet type specified */
#define NO_MULTICAST	6	/* this interface does not support multicast */
#define CANT_TERMINATE	7	/* this packet driver cannot terminate */
#define BAD_MODE	8	/* an invalid receiver mode was specified */
#define NO_SPACE	9	/* operation failed because of insufficient space */
#define TYPE_INUSE	10	/* the type had previously been accessed, and not released */
#define BAD_COMMAND	11	/* the command was out of range, or not implemented */
#define CANT_SEND	12	/* the packet couldn't be sent (usually hardware error) */
#define CANT_SET	13	/* hardware address couldn't be changed (> 1 handle open) */
#define BAD_ADDRESS	14	/* hardware address has bad length or format */
#define CANT_RESET	15	/* couldn't reset interface (> 1 handle open) */

#ifdef LOW_MEMORY
#define PK_BUF_SIZE  0xffffL
#define IP_BUF_SIZE  0xffffL
#else
#define PK_BUF_SIZE  0xfffffL
#define IP_BUF_SIZE  0xfffffL
#endif

#define MAX_ETHERNET_PACKET_SIZE	5000
#define PKTDRVR_BUF_PACKET_SIZE 0xffff
#define C_FLAG		0x01
#endif //  ETHERNET_INSTALLED

#ifdef IP_INSTALLED
#define IP_BUF_PACKET_SIZE	0xffff

#define INSERT	0
#define APPEND	1
#define PREPEND 2

#define ETHER

/* Packet driver interface classes */
#define CL_NONE 	0
#define CL_ETHERNET	1
#define CL_PRONET_10	2
#define CL_IEEE8025	3
#define CL_OMNINET	4
#define CL_APPLETALK	5
#define CL_SERIAL_LINE	6
#define CL_STARLAN	7
#define CL_ARCNET	8
#define CL_AX25 	9
#define CL_KISS 	10
#define CL_IEEE8023	11
#define CL_FDDI 	12
#define CL_INTERNET_X25 13
#define CL_LANSTAR	14
#define CL_SLFP 	15
#define CL_NETROM	16
#define CL_PPP		17
#define CL_QTSO 	18
#define NCLASS		19


typedef struct iface_packet {
	WORD_MR my_mr1;
	guint8	tos;
	guint32 gateway;
	WORD_MR mr1;
	struct iface_packet *next;
}IFACE_PACKET_STRUCT;

/* Interface control structure */
typedef struct iface{
	unsigned int next;     /* Linked list pointer */

	guint32 addr;		/* IP address */
	gint32 broadcast;	/* Broadcast address */
	gint32 netmask; 	/* Network mask */

	guint32 mtu;		/* Maximum transmission unit size */
	guint8 state;
	/* Device dependent */
	unsigned int pktdrvr_no;		/* Subdevice number to pass to send */
	guint8 *hwaddr; 	 /* Device hardware address, if any */
	unsigned int forw;	 /* Forwarding interface for output, if rx only */
	/* Counters */
	gint32 ipsndcnt;	 /* IP datagrams sent */
	gint32 rawsndcnt;	 /* Raw packets sent */
	gint32 iprecvcnt;	 /* IP datagrams received */
	gint32 rawrecvcnt;	 /* Raw packets received */
	gint32 lastsent;	 /* Clock time of last send */
	gint32 lastrecv;	 /* Clock time of last receive */
	IFACE_PACKET_STRUCT *outq;
}IFACE_STRUCT;


#define MAXTRIES	16	/* Maximum number of transmission attempts */

/* Extract a short from a long */
#define hiword(x)	((guint16)((x) >> 16))
#define loword(x)	((guint16)(x))

/* Extract a byte from a short */
#define hibyte(x)	((unsigned char)((x) >> 8))
#define lobyte(x)	((unsigned char)(x))

#define INADDR_ANY	0x0L	/* Local IP wildcard address */

#define NET_HDR_PAD	70	/* mbuf size to preallocate for headers */

/* IP header, INTERNAL representation */
#define IPLEN		20	/* Length of standard IP header */
#define IP_MAXOPT	40	/* Largest option field, bytes */

#define TLB		30	/* Default reassembly timeout, sec */
#define IPVERSION	4
#define IP_CS_OLD	1	/* use saved checksum */
#define IP_CS_NEW	0	/* calculate checksum */

typedef struct {
	guint32 source;  /* Source address */
	guint32 dest;	 /* Destination address */
	guint length;	 /* Total length */
	guint id;	 /* Identification */
	guint offset;	 /* Fragment offset in bytes */
	guint checksum;  /* Header checksum */

	unsigned int congest:1;   /* Congestion experienced bit (exp) */
	unsigned int df:1;	  /* Don't fragment flag */
	unsigned int mf:1;	  /* More Fragments flag */

	guint8 version; 	  /* IP version number */
	guint8 tos;		  /* Type of service */
	guint8 ttl;		  /* Time to live */
	guint8 protocol;	  /* Protocol */
	guint8 optlen;		  /* Length of options field, bytes */
	guint8 options[IP_MAXOPT];/* Options field */
}IP_STRUCT;

/* Address size definitions */
#define IPALEN	4		/* Length in bytes of an IP address */
#define MAXHWALEN	20	/* Maximum length of a hardware address */

/* Format of a Management Information Base entry for statistics gathering */
typedef struct {
//	char *name;
	gint32 value;
}MIB_ENTRY_STRUCT;

#define ipForwarding		1
#define ipDefaultTTL		2
#define ipInReceives		3
#define ipInHdrErrors		4
#define ipInAddrErrors		5
#define ipForwDatagrams 	6
#define ipInUnknownProtos	7
#define ipInDiscards		8
#define ipInDelivers		9
#define ipOutRequests		10
#define ipOutDiscards		11
#define ipOutNoRoutes		12
#define ipReasmTimeout		13
#define ipReasmReqds		14
#define ipReasmOKs		15
#define ipReasmFails		16
#define ipFragOKs		17
#define ipFragFails		18
#define ipFragCreates		19

#define NUMIPMIB		20

//TENNIS 0xff->
#define NULL_IFACE		(guint)(-1)

//alaki : to prevent syntax error
#define LOOPBACK_IFACE_NO	0xfe
#define ENCAP_IFACE_NO		0xfd

#define HASHMOD 7		/* Modulus used by hash_ip() function */

/* Class-of-service bits */
#define IP_COS		0x1c	/* Mask for class-of-service bits */
#define LOW_DELAY	0x10
#define THROUGHPUT	0x08
#define RELIABILITY	0x04

/* IP TOS fields */
#define PREC(x) 	(((x)>>5) & 0x7)
#define DELAY		0x10
#define THRUPUT 	0x8
#define RELIABLITY	0x4

/* Pseudo-header for TCP and UDP checksumming */
typedef struct {
	guint32 source;  /* IP source */
	guint32 dest;	 /* IP destination */
	char protocol;	 /* Protocol */
	guint length;	 /* Data field length */
}PSEUDO_HEADER_STRUCT;

/* IP routing table entry */
typedef struct{
	guint8 state;
	unsigned int prev;	/* Linked list pointers */
	unsigned int next;
	guint32 target; 	/* Target IP address */
	unsigned int bits;	/* Number of significant bits in target */
	guint32 gateway;	/* IP address of local gateway for this target */
	gint32 metric;		/* Hop count or whatever */
	unsigned int iface_no;	/* Device interface structure */
#if 0
	unsigned int rtprivate:1; /* Don't advertise this route */
	unsigned int rttrig:1;	  /* Trigger is pending for this route */
	unsigned int timer;	  /* Time until aging of this entry */
#endif // 0
	gint32 uses;		  /* Usage count */
}IP_ROUTE_STRUCT;

/* Cache for the last-used routing entry, speeds up the common case where    */
/* we handle a burst of packets to the same destination 		     */

typedef struct {
	guint32 target;
	unsigned int route_no;
}ROUTE_CACHE_STRUCT;

typedef struct{
	unsigned int iface_no;
	WORD_MR mr1;
	int rxbroadcast;
}HOPPER_QUE_STRUCT;

#define HOPPER_QUE_LEN		2048

#define NULL_IP_ROUTE		0xffff

#define RXBROADCAST_RESET	0
#define RXBROADCAST_SET 	0

/* Fields in option type byte */
#define OPT_COPIED	0x80	/* Copied-on-fragmentation flag */
#define OPT_CLASS	0x60	/* Option class */
#define OPT_NUMBER	0x1f	/* Option number */

/* IP option numbers */
#define IP_EOL		0	/* End of options list */
#define IP_NOOP 	1	/* No Operation */
#define IP_SECURITY	2	/* Security parameters */
#define IP_LSROUTE	3	/* Loose Source Routing */
#define IP_TIMESTAMP	4	/* Internet Timestamp */
#define IP_RROUTE	7	/* Record Route */
#define IP_STREAMID	8	/* Stream ID */
#define IP_SSROUTE	9	/* Strict Source Routing */

/* Timestamp option flags */
#define TS_ONLY 	0	/* Time stamps only */
#define TS_ADDRESS	1	/* Addresses + Time stamps */
#define TS_PRESPEC	3	/* Prespecified addresses only */

/* Fragment descriptor in a reassembly list */
typedef struct frag {
	WORD_MR my_mr1;
	struct frag *prev;	/* Previous fragment on list */
	struct frag *next;	/* Next fragment */
	WORD_MR buf;		/* Actual fragment data */
	guint offset;	 /* Starting offset of fragment */
	guint last;	 /* Ending offset of fragment */
}FRAG_STRUCT;

/* Reassembly descriptor */
typedef struct reasm {
	WORD_MR my_mr1;
	struct reasm *next;	 /* Linked list pointer */
	unsigned int timer_no;	 /* Reassembly timeout timer */
	FRAG_STRUCT *fraglist;	 /* Head of data fragment chain */
	guint length;		 /* Entire datagram length, if known */
	guint32 source; 	 /* src/dest/id/protocol uniquely describe a datagram */
	guint32 dest;
	guint id;
	char protocol;
}REASM_STRUCT;

#define IP_REASM_TIMEOUT_COUNT		(13*(1000/20))	 // 13 sec

/* IP protocol field values */
#define ICMP_PTCL	1	/* Internet Control Message Protocol */
#define IP4_PTCL	4	/* IP inside IP */
#define TCP_PTCL	6	/* Transmission Control Protocol */
#define UDP_PTCL	17	/* User Datagram Protocol */
#define IP_PTCL 	94	/* JI's IP inside IP */
#define ESP_PTCL	50	/* Encapsulating Security Protocol */
#define AH_PTCL 	51	/* Authentication header */

#define MAXTTL		255	/* Maximum possible IP time-to-live value */

#define FINGER_SERVER_TASK	0
#define FTP_SERVER_TASK 	1
#define SOUND_SERVER_TASK	2
#define DISCARD_SERVER_TASK	3
#define ECHO_SERVER_TASK	4
#define TELNET_SERVER_TASK	5

#endif //  IP_INSTALLED

#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))

#define NET_ERROR_NONE	      0       /* No error */
#define NET_ERROR_CON_EXISTS  1       /* Connection already exists */
#define NET_ERROR_NO_CONN     2       /* Connection does not exist */
#define NET_ERROR_CON_CLOS    3       /* Connection closing */
#define NET_ERROR_NO_MEM      4       /* No memory for TCB creation */
#define NET_ERROR_WOULDBLK    5       /* Would block */
#define NET_ERROR_NOPROTO     6       /* Protocol or mode not supported */
#define NET_ERROR_INVALID     7       /* Invalid arguments */

/* Socket structure */
typedef struct {
	guint32 address;	/* IP address */
	unsigned int port;	/* port number */
}SOCKET_STRUCT;

#define 	MAX_UCB 	10
//SHARAB : 10->100
#define 	MAX_USOCK	100

#define 	NULL_UCB	(unsigned int)(-1)
#define 	NULL_USOCK	(unsigned int)(-1)

#define 	MAX_SOCKET	10
#define 	NULL_SOCKET	(unsigned int)(-1)

enum USOCK_TYPE{
	USOCK_NOTUSED,
	USOCK_TYPE_TCP,
	USOCK_TYPE_UDP,
	USOCK_TYPE_AX25I,
	USOCK_TYPE_AX25UI,
	USOCK_TYPE_RAW,
	USOCK_TYPE_NETROML3,
	USOCK_TYPE_NETROML4,
	USOCK_TYPE_LOCAL_STREAM,
	USOCK_TYPE_LOCAL_DGRAM,
};
/* Generic socket address structure */
typedef struct {
	short sa_family;
	char sa_data[14];
}SOCKADDR_STRUCT;

/* This is a structure for "historical" reasons (whatever they are) */
typedef struct {
	unsigned long s_addr;
}IN_ADDR_STRUCT;

/* Socket address, DARPA Internet style */
typedef struct {
	short sin_family;
	unsigned short sin_port;
	IN_ADDR_STRUCT sin_addr;
	char sin_zero[8];
}SOCKADDR_IN_STRUCT;

#define SOCKSIZE	(sizeof(SOCKADDR_STRUCT))
#define MAXSOCKSIZE	SOCKSIZE /* All sockets are of the same size for now */

typedef union cb{
#ifdef TCP_INSTALLED
	unsigned int tcb_no;
#endif
	unsigned int ax25_cb_no;
#ifdef UDP_INSTALLED
	unsigned int ucb_no;
#endif
	unsigned int raw_ip_no;
	unsigned int raw_nr_no;
	unsigned int nr4cb_no;
	unsigned int loc_no;
}CB_UNION;

/* User sockets */
typedef struct {
	guint8 state;
	unsigned int prev;
	unsigned int next;
	unsigned int index;
	unsigned sock_value;
	int refcnt;
	char noblock;
	enum USOCK_TYPE type;
	int rdysock;
	CB_UNION cb;
	WORD_MR localname_mr1;
	WORD_MR peername_mr1;

	guint8 errcodes[4];	 /* Protocol-specific error codes */
	guint8 tos;		 /* Internet type-of-service */
	int flag;		 /* Mode flags, defined in socket.h */
}USOCK_STRUCT;

#define AF_INET 	0
#define AF_AX25 	1
#define AF_NETROM	2
#define AF_LOCAL	3
#define NAF		4

#define SOCK_STREAM	0
#define SOCK_DGRAM	1
#define SOCK_RAW	2
#define SOCK_SEQPACKET	3

//from errno.h
#define E2BIG_ERROR	3
#define EACCES_ERROR	4
#define EAGAIN_ERROR	5
#define EBADF_ERROR	6
#define EBUSY_ERROR	7
#define ECHILD_ERROR	8
#define EDEADLK_ERROR	9
#define EEXIST_ERROR	10
#define EFAULT_ERROR	11
#define EFBIG_ERROR	12
#define EINTR_ERROR	13
#define EINVAL_ERROR	14
#define EIO_ERROR	15
#define EISDIR_ERROR	16
#define EMFILE_ERROR	17
#define EMLINK_ERROR	18
#define ENAMETOOLONG_ERROR	19
#define ENFILE_ERROR	20
#define ENODEV_ERROR	21
#define ENOENT_ERROR	22
#define ENOEXEC_ERROR	23
#define ENOLCK_ERROR	24
#define ENOMEM_ERROR	25
#define ENOSPC_ERROR	26
#define ENOSYS_ERROR	27
#define ENOTDIR_ERROR	28
#define ENOTEMPTY_ERROR 29
#define ENOTTY_ERROR	30
#define ENXIO_ERROR	31
#define EPERM_ERROR	32
#define EPIPE_ERROR	33
#define EROFS_ERROR	34
#define ESPIPE_ERROR	35
#define ESRCH_ERROR	36
#define EXDEV_ERROR	37

#define EWOULDBLOCK_ERROR	100
#define ENOTCONN_ERROR		101
#define ESOCKTNOSUPPORT_ERROR	102
#define EAFNOSUPPORT_ERROR	103
#define EISCONN_ERROR		104
#define EOPNOTSUPP_ERROR	105
#define EALARM_ERROR		106
#define EABORT_ERROR		107
#undef	EINTR_ERROR
#define EINTR_ERROR		108
#define ECONNREFUSED_ERROR	109
#define EMSGSIZE_ERROR		110
#define EADDRINUSE_ERROR	111
#define EMIN_ERROR		100
#define EMAX_ERROR		112

#define _fd_type(fd)	(((fd) >> 13) & 3)
#define _fd_seq(fd)	((fd) & 8191)
#define _mk_fd(fd,type) ((fd) | ((type) << 13))
typedef enum {
	_FL_FILE,	/* Associated with file */
	_FL_SOCK,	/* Associated with network socket */
	_FL_ASY,	/* Asynch port */
	_FL_DISPLAY,	/* Associated with display driver */
	_FL_PIPE,	/* Pipe mode */
	_FL_SOUND	/* Sound mode */
}ENUM_FILE_TYPE;

#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))


/* TCP segment header -- internal representation			       */
/* Note that this structure is NOT the actual header as it appears on the      */
/* network (in particular, the offset field is missing).		       */
/* All that knowledge is in the functions ntohtcp() and htontcp() in tcpsubr.c */

#ifdef TCP_INSTALLED

#define TCPLEN		20	/* Minimum Header length, bytes */
#define TCP_MAXOPT	40	/* Largest option field, bytes */

typedef struct {
	guint source;	 /* Source port */
	guint dest;	 /* Destination port */
	gint32 seq;	 /* Sequence number */
	gint32 ack;	 /* Acknowledgment number */
	guint wnd;	 /* Receiver flow control window */
	guint checksum;  /* Checksum */
	guint up;			 /* Urgent pointer */
	guint mss;			 /* Optional max seg size */
	guint8 wsopt;			 /* Optional window scale factor */
	guint32 tsval;			 /* Outbound timestamp */
	guint32 tsecr;			 /* Timestamp echo field */
	unsigned int congest_flag:1; /* Echoed IP congestion experienced bit */
	unsigned int urg_flag:1;
	unsigned int ack_flag:1;
	unsigned int psh_flag:1;
	unsigned int rst_flag:1;
	unsigned int syn_flag:1;
	unsigned int fin_flag:1;
	unsigned int mss_flag:1;     /* MSS option present */
	unsigned int wscale_flag:1;  /* Window scale option present */
	unsigned int tstamp_flag:1;  /* Timestamp option present */
}TCP_STRUCT;

/* TCP options */
#define EOL_KIND	0
#define NOOP_KIND	1
#define MSS_KIND	2
#define MSS_LENGTH	4
#define WSCALE_KIND	3
#define WSCALE_LENGTH	3
#define TSTAMP_KIND	8
#define TSTAMP_LENGTH	10

/* Resequencing queue entry */
typedef struct reseq{
	WORD_MR my_mr1;
	struct reseq *next;	/* Linked-list pointer */
	TCP_STRUCT seg; 	/* TCP header */
	WORD_MR mr1;		/* data */
	guint length;		/* data length */
	char tos;		/* Type of service */
}RESEQ_STRUCT;

#define NORMAL_CLOSE_REASON	0	/* Normal close */
#define RESET_CLOSE_REASON	1	/* Reset by other end */
#define TIMEOUT_CLOSE_REASON	2	/* Excessive retransmissions */
#define NETWORK_CLOSE_REASON	3	/* Network problem (ICMP message) */

/* These numbers match those defined in the MIB for TCP connection state */
typedef enum {
	TCP_CLOSED=1,
	TCP_LISTEN,
	TCP_SYN_SENT,
	TCP_SYN_RECEIVED,
	TCP_ESTABLISHED,
	TCP_FINWAIT1,
	TCP_FINWAIT2,
	TCP_CLOSE_WAIT,
	TCP_LAST_ACK,
	TCP_CLOSING,
	TCP_TIME_WAIT,
}TCP_STATE_ENUM;

/* Connection structure (two sockets) */
typedef struct {
	SOCKET_STRUCT local;
	SOCKET_STRUCT remote;
}CONNECTION_STRUCT;

/* TCP connection control block */
typedef struct {

	unsigned int next;	 /* Linked list pointer */
	unsigned int prev;	 /* Linked list pointer */

	guint8 our_state;	/* Que state */
	TCP_STATE_ENUM state;	/* Connection state */
	CONNECTION_STRUCT conn;
	guint8 reason;		/* Reason for closing */
	/* If reason == NETWORK_CLOSE_REASON, the ICMP type and code values are stored here */
	guint8 type;
	guint8 code;

	/* Send sequence variables start */
	gint32 snd_una;      /* First unacknowledged sequence number */
	gint32 snd_nxt;      /* Next sequence num to be sent for the first time */
	gint32 snd_ptr;      /* Working transmission pointer */
	gint32 snd_wl1;      /* Sequence number used for last window update */
	gint32 snd_wl2;      /* Ack number used for last window update */
	gint32 snd_wnd;      /* Other end's offered receive window */
	guint snd_up;	     /* Send urgent pointer */
	guint8 snd_wind_scale;/* Send window scale */
	/* Send sequence variables end */

	gint32 iss;		 /* Initial send sequence number */
	gint32 resent;		 /* Count of bytes retransmitted */
	gint32 cwind;		 /* Congestion window */
	gint32 ssthresh;	 /* Slow-start threshold */
	gint	     dupacks;	 /* Count of duplicate (do-nothing) ACKs */

	/* Receive sequence variables */
	gint32 rcv_nxt; 	/* Incoming sequence number expected next */
	gint32 rcv_wnd; 	/* Our offered receive window */
	guint  rcv_up;		/* Receive urgent pointer */
	guint8	rcv_wind_scale; /* Recv window scale */
	/* Receive sequence variables */

	gint32 last_ack_sent;	 /* Last ack sent for timestamp purposes */
	gint32 ts_recent;	 /* Most recent incoming timestamp */

	gint32 irs;		 /* Initial receive sequence number */
	gint32 rerecv;		 /* Count of duplicate bytes received */
	gint32 mss;		 /* Maximum segment size */

	gint32 window;		 /* Receiver window and send queue limit */
	gint32 limit;		 /* Send queue limit */

	guint8 rtn_type;
	      /* Control flags */
	unsigned int force_flag:1;   /* We owe the other end an ACK or window update */
	unsigned int clone_flag:1;   /* Server-type TCB, cloned on incoming SYN */
	unsigned int retran_flag:1;  /* A retransmission has occurred */
	unsigned int active_flag:1;  /* TCB created with an active open */
	unsigned int synack_flag:1;  /* Our SYN has been acked */
	unsigned int rtt_run_flag:1; /* We're timing a segment */
	unsigned int congest_flag:1; /* Copy of last IP congest bit received */
	unsigned int ts_ok_flag:1;   /* We're using timestamps */
	unsigned int ws_ok_flag:1;   /* We're using window scaling */
//LION start
	unsigned int my_reset_flag:1;
	unsigned int my_send_rst_flag:1;
//LION end
	guint8	  tos;		     /* Type of service (for IP) */
	int	  backoff;	/* Backoff interval */

	unsigned int rcvq;	/* Receive queue */
	unsigned int sndq;	/* Send queue */
	gint32 rcvcnt;		/* Count of items on rcvq */
	gint32 sndcnt;		/* Number of unacknowledged sequence numbers on */
				/* sndq. NB: includes SYN and FIN, which don't  */
				/* actually appear on sndq!			*/

	RESEQ_STRUCT *reseq;	/* Out-of-order segment queue */
	unsigned int timer_no;	/* Retransmission timer */
	gint32 rtt_time;	/* Stored clock values for RTT */
	gint32 rttseq;		/* Sequence number being timed */
	gint32 rttack;		/* Ack at start of timing (for txbw calc) */
	gint32 srtt;		/* Smoothed round trip time, milliseconds */
	gint32 mdev;		/* Mean deviation, milliseconds */
	gint32 rtt;		/* Last received RTT (for debugging) */

	unsigned int user;	/* User parameter (e.g., for mapping to an */
				/* application control block		   */

	gint32 quench;		/* Count of incoming ICMP source quenches */
	gint32 unreach; 	/* Count of incoming ICMP unreachables */
	gint32 timeouts;	/* Count of retransmission timeouts */
	gint32 lastack; 	/* Time of last received ack */
	gint32 outlen;		/* Average tx ack amount */
	gint32 outrate; 	/* Average tx ack interval, ms */
	gint32 lastrx;		/* Time of last received data */
	gint32 inlen;		/* Average receive data size */
	gint32 inrate;		/* Average receive packet interval,ms */
	guint8 trdiscard_flag;
	guint next_timer_count;
#if ((defined H245_GATEWAY_SIDE) || (defined MY_TCPIP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT))
	guint rec_no;
#endif
}TCB_STRUCT;

typedef struct{
	WORD_MR mr1;
	unsigned int next;
}TCB_QUE_STRUCT;

typedef struct {
	guint8 state;
	unsigned int next;
	unsigned int prev;
	unsigned int tcb_no;
	unsigned int stack;
	unsigned int task_no;
}INET_STRUCT;

#define 	TCP_INIT_RTNS_TYPE0	0 // should call NULL,NULL,i_upcall
#define 	TCP_NORMAL_RTNS_TYPE1	1 // should call :s_trcall,s_ttcall,s_tscall rtns

#define 	NULL_INET	(unsigned int)(-1)
#define 	NULL_TCB	(unsigned int)(-1)

#define 	MAX_INET	10
//SHARAB : 10->100
#define 	MAX_TCB 	100

union sp {
	SOCKADDR_STRUCT *sa;
	SOCKADDR_IN_STRUCT *in;
//	struct sockaddr_ax *ax;
//	struct sockaddr_nr *nr;
};

/* Codes for the tcp_open call */
#define TCP_PASSIVE	0
#define TCP_ACTIVE	1
#define TCP_SERVER	2	/* Passive, clone on opening */


#define tcpRtoAlgorithm 	0
#define tcpRtoMin		1
#define tcpRtoMax		2
#define tcpMaxConn		3
#define tcpActiveOpens		4
#define tcpPassiveOpens 	5
#define tcpAttemptFails 	6
#define tcpEstabResets		7
#define tcpCurrEstab		8
#define tcpInSegs		9
#define tcpOutSegs		0xa
#define tcpRetransSegs		0xb
#define tcpInErrs		0xc
#define tcpOutRsts		0xd

#define MAX_TCP_MIB		0xe

#define DEF_MSS 512	/* Default maximum segment size */
#define DEF_WND 2048	/* Default receiver window */
#define RTTCACHE 16	/* # of TCP round-trip-time cache entries */
#define DEF_RTT 5000	/* Initial guess at round trip time (5 sec) */
#define MSL2	30	/* Guess at two maximum-segment lifetimes */
#define MIN_RTO 500L	/* Minimum timeout, milliseconds */
#define DEF_WSCALE	0	/* Our window scale option */
//SHARAB : in jnos20f : 10
#define DEF_RETRIES	6  /* default number of retries before resetting tcb */

#define geniss()	((guint32)msclock() << 12) /* Increment clock at 4 MB/sec */

/* Number of consecutive duplicate acks to trigger fast recovery */
#define TCPDUPACKS	3

/* TCP round-trip time cache */
typedef struct {
	guint32 addr;		  /* Destination IP address */
	gint32 srtt;		 /* Most recent SRTT */
	gint32 mdev;		 /* Most recent mean deviation */
}TCP_RTT_STRUCT;

/* Round trip timing parameters */
#define AGAIN	8	/* Average RTT gain = 1/8 */
#define LAGAIN	3	/* Log2(AGAIN) */
#define DGAIN	4	/* Mean deviation gain = 1/4 */
#define LDGAIN	2	/* log2(DGAIN) */
#endif //  TCP_INSTALLED

#ifdef UDP_INSTALLED
/* User Datagram Protocol control block
 * Each entry on the receive queue consists of the
 * remote socket structure, followed by any data
 */
/* Structure of a UDP protocol header */
typedef struct{
	guint source;	 /* Source port */
	guint dest;	 /* Destination port */
	guint length;	 /* Length of header and data */
	guint checksum;  /* Checksum over pseudo-header, header and data */
}UDP_STRUCT;
#define UDPHDR	8	/* Length of UDP header */
typedef struct {
	unsigned int next;
	unsigned int prev;
	guint8 state;
	SOCKET_STRUCT socket;	/* Local port accepting datagrams */
//	void (*r_upcall)(struct iface *iface,struct udp_cb *,int);
				/* Function to call when one arrives */
	WORD_MR rcvq;		/* Queue of pending datagrams */
	int rcvcnt;		/* Count of pending datagrams */
	int user;		/* User link */
//TENNIS
	guint ucb_no;
}UCB_STRUCT;
guint32 pull32(WORD_MR *mr1);
long pull16(WORD_MR *mr1);
int pull8(WORD_MR *mr1);

#define udpInDatagrams		0
#define udpNoPorts		1
#define udpInErrors		2
#define udpOutDatagrams 	3

#define MAX_UDP_MIB		4
#endif //  UDP_INSTALLED


#ifdef ARP_INSTALLED
/* Lifetime of a valid ARP entry */
#define ARPLIFE 	900	  /* 15 minutes */
/* Lifetime of a pending ARP entry */
#define PENDTIME	15	/* 15 seconds */

/* ARP definitions (see RFC 826) */

#define ARPLEN	16		/* Size of ARP hdr, minus hardware addresses */


/* ARP opcodes */
typedef enum {
	ARP_REQUEST=1,
	ARP_REPLY,
	REVARP_REQUEST,
	REVARP_REPLY
}ARP_OPCODE_ENUM;
/* Hardware types */
typedef enum  {
	ARP_NETROM=0,	/* Fake for NET/ROM (never actually sent) */
	ARP_ETHER,	/* Assigned to 10 megabit Ethernet */
	ARP_EETHER,	/* Assigned to experimental Ethernet */
	ARP_AX25,	/* Assigned to AX.25 Level 2 */
	ARP_PRONET,	/* Assigned to PROnet token ring */
	ARP_CHAOS,	/* Assigned to Chaosnet */
	ARP_IEEE802,	/* Who uses this? */
	ARP_ARCNET,
	ARP_APPLETALK
}ARP_HWTYPE_ENUM;
#define NHWTYPES 9

/* Table of hardware types known to ARP */
typedef struct {
	guint hwalen;	 /* Hardware length */
	guint iptype;	 /* Hardware type field for IP */
	guint arptype;	 /* Hardware type field for ARP */
	guint pendtime;  /* # secs to wait pending response */
	guint8 *bdcst;		  /* Hardware broadcast address */
//	char *(*format)(char *,guint8 *);
				/* Function that formats addresses */
//	int (*scan)(guint8 *,char *);
				/* Reverse of format */
}ARP_TYPE_STRUCT;

/* Format of ARP table */
typedef struct arp_tab {

	WORD_MR my_mr1;
	struct arp_tab *next;		/* Doubly-linked list pointers */
	struct arp_tab *prev;
	unsigned int timer_no;		/* Time until aging this entry */
	WORD_MR pending;	   /* Queue of datagrams awaiting resolution */
	guint32 ip_addr;		 /* IP Address, host order */
	ARP_HWTYPE_ENUM hardware;	/* Hardware type */
	enum {
		ARP_PENDING,	/* Incomplete */
		ARP_VALID	/* Complete */
	} state;
	guint8 *hw_addr;	 /* Hardware address */
	WORD_MR my_hw_addr_mr1;
	unsigned int pub:1;	/* Respond to requests for this entry? */
}ARP_TAB_STRUCT;
typedef struct arp_stat {
	unsigned recv;		/* Total number of ARP packets received */
	unsigned badtype;	/* Incoming requests for unsupported hardware */
	unsigned badlen;	/* Incoming length field(s) didn't match types */
	unsigned badaddr;	/* Bogus incoming addresses */
	unsigned inreq; 	/* Incoming requests for us */
	unsigned replies;	/* Replies sent */
	unsigned outreq;	/* Outoging requests sent */
}ARP_STAT_STRUCT;
/* Format of an ARP request or reply packet. From p. 3 */
typedef struct {
	ARP_HWTYPE_ENUM hardware;	/* Hardware type */
	guint protocol; 		/* Protocol type */
	guint8	hwalen; 		/* Hardware address length, bytes */
	guint8	pralen; 		/* Length of protocol address */
	ARP_OPCODE_ENUM opcode; 	/* ARP opcode (request/reply) */
	guint8	shwaddr[MAXHWALEN];	/* Sender hardware address field */
	guint32 sprotaddr;		/* Sender Protocol address field */
	guint8	thwaddr[MAXHWALEN];	/* Target hardware address field */
	guint32 tprotaddr;		/* Target protocol address field */
}ARP_STRUCT;
#endif // ARP_INSTALLED
/* ========================================================================= */
/* ============================= arp.c ===================================== */
/* ========================================================================= */
#ifdef ARP_INSTALLED
void arp_input(unsigned int iface_no,WORD_MR mr1);
ARP_TAB_STRUCT*arp_add(guint32 ipaddr,ARP_HWTYPE_ENUM hardware,guint8 *hw_addr,int pub);
void arp_timeout(unsigned int timer_no);
void arp_drop(struct arp_tab *ap_pt);
guint8 *res_arp(unsigned int iface_no,ARP_HWTYPE_ENUM hardware,guint32 target,WORD_MR mr1);
ARP_TAB_STRUCT *arp_lookup(ARP_HWTYPE_ENUM hardware,guint32 ipaddr);
void arp_output(unsigned int iface_no,ARP_HWTYPE_ENUM hardware,guint32 target);
WORD_MR htonarp(ARP_STRUCT *arp_pt);
int ntoharp(ARP_STRUCT *arp_pt,WORD_MR *mr3);
void init_arp_type_array(void);
//JASHN start
void send_gratuituos_arp(void);
void gratuituos_arp_output(unsigned int iface_no, ARP_HWTYPE_ENUM hardware, guint32 target);
//JASHN end
#endif // ARP_INSTALLED
/* ========================================================================= */
/* ============================= misc.c ==================================== */
/* ========================================================================= */
#ifdef KIATEL_DYNAMIC_MEM
guint8 *put32(guint8 *cp,guint32 x);
guint8 *put16(guint8 *cp,unsigned int x);
unsigned int get16(guint8 *cp);
guint32 get32(guint8 *cp);
unsigned int eac(guint32 sum);	     /* Carries in high order 16 bits */
#ifdef IP_INSTALLED
unsigned int cksum(PSEUDO_HEADER_STRUCT *ph, WORD_MR mr1, unsigned int len);
#endif
unsigned int lcsum(guint16 *buf,unsigned int cnt);
WORD_MR my_pullup_only(WORD_MR mr1,unsigned int cnt);
WORD_MR my_pullup_copy_data(WORD_MR mr1,guint8 *dat,unsigned int cnt);
WORD_MR my_append(WORD_MR mr1,WORD_MR mr2);
WORD_MR my_pushdown(WORD_MR mr1,unsigned int cnt);
WORD_MR my_pushdown_copy_data(WORD_MR mr1,guint8 *dat,unsigned int cnt);
unsigned int my_pullup_linklist_copy_data(WORD_MR *mr3,guint8 *dat_pt,unsigned int cnt);
unsigned int my_pullup_linklist_only(WORD_MR *mr3,unsigned int cnt);
void rls_mr1_list(WORD_MR mr1);
guint32 pull32(WORD_MR *mr3);
long pull16(WORD_MR *mr3);
int pull8(WORD_MR *mr3);
void enqueue(WORD_MR *q_mr1,WORD_MR mr1);
WORD_MR dequeue(WORD_MR *q_mr1);
#endif
/* ========================================================================= */
/* ============================= pktdrvr.c ================================= */
/* ========================================================================= */
#ifdef ETHERNET_INSTALLED
guint8 pk_attach(unsigned int int_no,guint32 addr);
guint8 all_pk_attach(void);
guint8 driver_info(unsigned int int_no,short handle,unsigned int *version,unsigned int *class,unsigned int *type,unsigned int *number,unsigned int *basic);
guint8 pk_send(unsigned int pktdrvr_no,unsigned int mr1);
void pkint0(_go32_dpmi_registers *reg);
void pkint1(_go32_dpmi_registers *reg);
guint8 pk_stop(unsigned int iface_no,BYTE do_terminate);
long access_type( unsigned int int_no,unsigned int if_class,unsigned int if_type,
		  unsigned int if_number,unsigned int rm_segment,unsigned int rm_offset);
guint8 release_type(unsigned int int_no,short handle);
guint8 terminate(unsigned int int_no,short handle);
guint8 get_address(unsigned int int_no,short handle,guint8 *buf,unsigned int len);
guint8 set_rcv_mode(unsigned int int_no,short handle,unsigned int mode);
void pktdrvr_rx_hndlr(void);
void pktdrvr_tx_hndlr(void);
void init_pktdrvr_buf(unsigned int i);
void init_pktdrvr_st(void);
#endif // ETHERNET_INSTALLED
/* ========================================================================= */
/* ============================= udp.c ===================================== */
/* ========================================================================= */
#ifdef UDP_INSTALLED
//LION : guint8 ->
guint start_udp(SOCKET_STRUCT *lsocket_pt);
int so_udp(USOCK_STRUCT *up _U_,int protocol _U_);
int so_udp_bind(USOCK_STRUCT *up);
int so_udp_conn(USOCK_STRUCT *up);
int so_udp_recv(USOCK_STRUCT *up,WORD_MR *mr3,SOCKADDR_STRUCT *from,int *fromlen);
int so_udp_send(USOCK_STRUCT *up,WORD_MR mr1,SOCKADDR_STRUCT *to);
int so_udp_qlen(USOCK_STRUCT *up,int rtx);
int so_udp_close(USOCK_STRUCT *up);
int so_udp_shut(USOCK_STRUCT *up,int how _U_);
void s_urcall(unsigned int iface_no _U_,unsigned int ucb_no _U_,int cnt _U_);
int so_udp_stat(USOCK_STRUCT *up);
unsigned int open_udp(SOCKET_STRUCT *lsocket,void (*r_upcall)() _U_);
int send_udp(SOCKET_STRUCT *lsocket,SOCKET_STRUCT *fsocket,guint8 tos,
	     guint8 ttl,WORD_MR mr1,guint length,guint id,char df);
int recv_udp(unsigned int ucb_no,SOCKET_STRUCT *fsocket,WORD_MR *mr3);
void udp_recv_hndlr(void);
int del_udp(unsigned int this_ucb);
void udp_input(unsigned int iface_no,IP_STRUCT *ip,WORD_MR mr1,int rxbroadcast,gint32 said _U_);
unsigned int lookup_ucb(SOCKET_STRUCT *socket);
void udp_garbage(int red _U_);
void htonudp(UDP_STRUCT *udp_pt,WORD_MR *mr3,PSEUDO_HEADER_STRUCT *ph);
int ntohudp(UDP_STRUCT *udp_pt,WORD_MR *mr3);
guint udpcksum(WORD_MR mr1);
int close_s_udp(int s);
void init_ucb_st(void);
unsigned int find_free_ucb_st(void);
void rls_ucb_st(unsigned int ucb_no);
int xmit_udp(unsigned int ucb_no,SOCKET_STRUCT *fsocket_pt,WORD_MR mr1);
void my_ras2_rtn(unsigned int ucb_no,unsigned int mr1,SOCKET_STRUCT fsocket);
#endif // UDP_INSTALLED
/* ========================================================================= */
/* ============================= iprx.c ==================================== */
/* ========================================================================= */
#ifdef IP_INSTALLED
int ntohether(ETHER_STRUCT *ether,WORD_MR *mr3);
void eproc(unsigned int iface_no,WORD_MR mr1);
void ip_recv(unsigned int iface_no,IP_STRUCT *ip_pt,WORD_MR mr1,int rxbroadcast,gint32 spi);
int fraghandle(IP_STRUCT *ip_pt,WORD_MR *mr3);
REASM_STRUCT *lookup_reasm(IP_STRUCT *ip_pt);
REASM_STRUCT *creat_reasm(IP_STRUCT *ip_pt);
void free_reasm(REASM_STRUCT *r);
void ip_timeout(unsigned int timer_no _U_);
FRAG_STRUCT *newfrag(unsigned int offset,unsigned int last,WORD_MR mr1);
void freefrag(FRAG_STRUCT *fp);
#endif // IP_INSTALLED
/* ========================================================================= */
/* ============================= iptx.c ==================================== */
/* ========================================================================= */
#ifdef IP_INSTALLED
guint8 ip_send(guint32 source,guint32 dest,guint8 protocol,guint8 tos,
	       guint8 ttl,WORD_MR mr1,guint id,char df);
guint32 locaddr(guint32 addr);
unsigned int ismyaddr(guint32 addr);
guint8 net_route(unsigned int iface_no,WORD_MR mr1);
unsigned int rt_lookup(guint32 target);
guint8 q_pkt(unsigned int iface_no,guint32 gateway,IP_STRUCT *ip_pt,WORD_MR mr1,int ckgood);
void ttldec_all_iface(void);
void ttldec(unsigned int iface_no);
void if_tx(unsigned int iface_no);
guint8 enet_send(WORD_MR mr1,unsigned int iface_no,guint32 gateway,guint8 tos _U_);
guint8 enet_output(unsigned int iface_no,guint8  *dest,guint8  *source,
		   guint type,WORD_MR mr1);
void htonether(ETHER_STRUCT *ether,WORD_MR *mr3);
IFACE_PACKET_STRUCT *create_my_pkt_rec(void);
void rls_my_pkt_rec(IFACE_PACKET_STRUCT *pkt_pt);
void iface_outq_hndlr(void);
#endif // IP_INSTALLED

/* ========================================================================= */
/* ============================== ip.c ===================================== */
/* ========================================================================= */

#ifdef IP_INSTALLED
guint hash_ip(guint32 addr);
void init_hopper_q(void);
guint8 to_hopper_q(unsigned int iface_no,WORD_MR mr1,guint8 rxbroadcast);
guint8 from_hopper_q(HOPPER_QUE_STRUCT **a);
int ip_route_hndlr(void);
void htonip(IP_STRUCT *ip_pt,WORD_MR *mr3,int cflag);
int ntohip(IP_STRUCT *ip,WORD_MR *mr3);
unsigned int my_rt_add(guint32 target,guint bits,guint32 gateway,guint iface_no);
unsigned int rt_add(guint32  target,unsigned int bits,guint32  gateway,
		    unsigned int iface_no,gint32   metric,gint32   ttl _U_,
		    guint8   private _U_);
int my_rt_drop(guint32 target);
int rt_drop(guint32 target,unsigned int bits);
unsigned int rt_blookup(guint32 target,unsigned int bits);
void init_ip_route_st(void);
unsigned int find_free_ip_route_rec(void);
void rls_ip_route_rec(unsigned int route_no);
unsigned int find_free_iface_struct(void);
void init_iface_struct(void);
guint ip_mtu(guint32 addr);
void ip_init(void);
void init_ip_buf(unsigned int i);
#endif //  IP_INSTALLED

/* ========================================================================= */
/* ============================= tcprx.c =================================== */
/* ========================================================================= */
#ifdef TCP_INSTALLED
void s_trcall(unsigned int tcb_no _U_,guint32 cnt _U_);
void so_tcp_recv_hndlr(void);
int so_tcp_recv(USOCK_STRUCT *up,WORD_MR *mr3,SOCKADDR_STRUCT *from _U_, int *fromlen _U_);
long recv_tcp(unsigned int tcb_no,WORD_MR *mr3,long cnt);
void tcp_input(unsigned int iface_no _U_,IP_STRUCT *ip,WORD_MR mr1,
	       int rxbroadcast,gint32 said _U_);
#endif
/* ========================================================================= */
/* ============================= tcptx.c =================================== */
/* ========================================================================= */
#ifdef TCP_INSTALLED
//SHARAB : guint32->gint32
void s_ttcall(unsigned int tcb_no _U_,gint32 cnt _U_);
void so_tcp_send_hndlr(void);
int so_tcp_send(USOCK_STRUCT *up,WORD_MR mr1,SOCKADDR_STRUCT *to _U_);
long send_tcp(unsigned int tcb_no,WORD_MR mr1);
void tcp_output(unsigned int tcb_no);
#endif

/* ========================================================================= */
/* ============================= tcp2.c ==================================== */
/* ========================================================================= */
#ifdef TCP_INSTALLED
//LION
guint start_passive_tcp(SOCKET_STRUCT *lsocket_pt,
		  SOCKET_STRUCT *fsocket_pt _U_,unsigned int task _U_,
		  unsigned int stack _U_,int mode _U_);
guint8 start_tcp(unsigned int port,unsigned int task _U_,unsigned int stack);
int stop_tcp(unsigned int port);
unsigned int open_tcp(SOCKET_STRUCT *lsocket_pt,SOCKET_STRUCT *fsocket_pt,
		      int mode,guint window,guint8 rtn_type,unsigned int tos,
		      unsigned int user);
int close_tcp(unsigned int tcb_no);
int del_tcp(unsigned int this_tcb);
int tcpval(unsigned int tcb_no);
int kick_tcp(unsigned int tcb_no);
int kick(guint32 addr);
void reset_all(void);
void reset_tcp(unsigned int tcb_no);
//LION
guint start_active_tcp(SOCKET_STRUCT *lsocket_pt,SOCKET_STRUCT *fsocket_pt);
//LION
guint connect(int s,SOCKADDR_STRUCT *peername,int peernamelen);
#endif //  TCP_INSTALLED

/* ========================================================================= */
/* ============================= tcp1.c ==================================== */
/* ========================================================================= */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
void autobind(USOCK_STRUCT *up);
unsigned int socket(unsigned int af,unsigned int type,unsigned int protocol);
unsigned int bind(unsigned int s,SOCKADDR_STRUCT *name,unsigned int namelen);
void trim_mbuf(WORD_MR *mr3,guint length);
void rls_usock_st(unsigned int usock_no);
unsigned int find_free_usock_st(void);
int checkipaddr(SOCKADDR_STRUCT *name,int namelen);
void tcp_udp_init(void);
void init_usock_st(void);
#endif

#ifdef TCP_INSTALLED
int so_tcp(USOCK_STRUCT *up,int protocol _U_);
//LION
guint so_tcp_listen(USOCK_STRUCT *up,int backlog);
//LION
guint so_tcp_conn(USOCK_STRUCT *up);
int so_tcp_qlen(USOCK_STRUCT *up,int rtx);
int so_tcp_kick(USOCK_STRUCT *up);
int so_tcp_shut(USOCK_STRUCT *up,int how);
int so_tcp_close(USOCK_STRUCT *up);
void trdiscard(unsigned int tcb_no,guint32 cnt);
char *tcpstate(USOCK_STRUCT *up);
int so_tcp_stat(USOCK_STRUCT *up);
void s_tscall(unsigned int tcb_no,unsigned int old _U_,unsigned int new);
void i_upcall(unsigned int tcb_no,unsigned int oldstate,unsigned int newstate);
void send_syn(unsigned int tcb_no);
void htontcp(TCP_STRUCT *tcp_pt,WORD_MR *mr3,guint32 ipsrc,guint32 ipdest);
int ntohtcp(TCP_STRUCT *tcp_pt,WORD_MR *mr3);
unsigned int extract(WORD_MR mr1,unsigned int offset,guint8 *buf_pt,unsigned int len);
void append(WORD_MR *first_mr1,WORD_MR new_mr1);
unsigned int lookup_tcb(CONNECTION_STRUCT *conn);
unsigned int create_tcb(CONNECTION_STRUCT *conn);
void close_self(unsigned int tcb_no,int reason);
int seq_within(long x,long low,long high);
int seq_lt(long x,long y);
int seq_le(long x,long y);
int seq_gt(long x,long y);
int seq_ge(long x,long y);
void settcpstate(unsigned int tcb_no,TCP_STATE_ENUM newstate);
void rtt_add(guint32 addr,guint32 rtt);
TCP_RTT_STRUCT *rtt_get(guint32 addr);
void tcp_garbage(int red _U_);
void reset(IP_STRUCT *ip,TCP_STRUCT *seg);
void update(unsigned int tcb_no,TCP_STRUCT *seg,unsigned int length);
int in_window(unsigned int tcb_no,gint32 seq);
void proc_syn(unsigned int tcb_no,guint8 tos,TCP_STRUCT *seg);
void add_reseq(unsigned int tcb_no,guint8 tos,TCP_STRUCT *seg,WORD_MR *mr3,guint length);
void get_reseq(unsigned int tcb_no,guint8 *tos,TCP_STRUCT *seg,WORD_MR *mr1,guint *length);
int trim(unsigned int tcb_no,TCP_STRUCT *seg,WORD_MR *mr3,guint *length);
RESEQ_STRUCT *find_free_reseq(void);
void rls_reseq(RESEQ_STRUCT *rp);
gint32 msclock(void);
void init_tcb_st(void);
unsigned int find_free_tcb_st(void);
void rls_tcb_st(unsigned int tcb_no);
void tcp_timeout(unsigned int timer_no);
gint32 backoff(int n);
void init_inet_st(void);
unsigned int find_free_inet_st(void);
void rls_inet_st(unsigned int inet_no);
void copy_tcb_fields_to_new_one(guint old_tcb_no,guint new_tcb_no);
guint start_tcp_timer(guint tcb_no,guint cnt);
void stop_tcp_timer(guint tcb_no);
void send_rst(unsigned int tcb_no);
void my_reset_tcp(unsigned int tcb_no);
void my_release_tcp(unsigned int tcb_no);
#endif // TCP_INSTALLED
#if ((defined TCP_INSTALLED) || (defined ARP_INSTALLED))
unsigned int change_to_kiatel_count(unsigned int cnt);
#endif

/* ========================================================================= */
/* ============================= router.c ================================== */
/* ========================================================================= */

#ifdef ROUTER_SYSTEM
typedef struct{
	guint int_no;
	guint32 addr;
	guint bits;
}ROUTER_STRUCT;
void init_router_st(void);
void init_all_router_routes(void);
#endif

#ifdef TCP_INSTALLED
#define H225_Q931_TCP_TYPE	0
#define H245_TCP_TYPE		1
#define OAM_TCP_TYPE		2
#define KFTPD_CONTROL_TCP_TYPE	3
#define KFTPD_DATA_TCP_TYPE	4
#define LI_ROS_TCP_TYPE 	5
#define KFTP_CONTROL_TCP_TYPE	6
#define KFTP_DATA_TCP_TYPE	7
#define TELNETD_TCP_TYPE	8
#define HI1_TCP_TYPE		9
#define HI1_DISTRIBUTOR_TCP_TYPE	10
#define SYSCON_SERVER_TCP_TYPE	11
#define SYSCON_CLIENT_TCP_TYPE	12

#define NULL_TCP_TYPE		0xff
#define TCP_NULL_TIMER		(guint)(-1)
#endif

#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM) ||\
     (defined HTBY_LINK_ON_MY_UDP) || (defined KIATEL_UDP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT) || (defined SYSCONUDP))
guint add_this_oam_route(guint32 address);
#endif

#if ((defined KIATEL_UDP_INSTALLED) || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE) || (defined TCP_INSTALLED))
typedef struct {
#ifdef DYNAMIC_IP_ALLOCATION
	guint32 hot_ipv4_address;
#else
	guint32 ipv4_address;
#endif
#ifdef DYNAMIC_IP_ALLOCATION
#if ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE))
	guint32 standby_ipv4_address;
	guint htby_link_hot_port;
	guint htby_link_standby_port;
#endif
#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	guint win_oam_terminal_hot_port[NO_KIATEL_UDP_OAM_TERMINAL];
	guint win_oam_terminal_standby_port[NO_KIATEL_UDP_OAM_TERMINAL];
	guint32 win_oam_terminal_remote_ipv4_address[NO_KIATEL_UDP_OAM_TERMINAL];
	guint win_oam_terminal_remote_port[NO_KIATEL_UDP_OAM_TERMINAL];
	guint win_oam_no;
#endif
#if ((defined SYSCONUDP) && (defined SYSTEM_SIDE))
	guint sysconudp_hot_port[NO_KIATEL_UDP_SYSCONUDP];
	guint sysconudp_standby_port[NO_KIATEL_UDP_SYSCONUDP];
	guint32 sysconudp_remote_ipv4_address[NO_KIATEL_UDP_SYSCONUDP];
	guint sysconudp_remote_port[NO_KIATEL_UDP_SYSCONUDP];
	guint sysconudp_no;
#endif
#if ((defined MY_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	guint our_oam_terminal_hot_port[NO_OPRTR];
	guint our_oam_terminal_standby_port[NO_OPRTR];
	guint32 our_oam_terminal_remote_ipv4_address[NO_OPRTR];
	guint our_oam_terminal_remote_port[NO_OPRTR];
	guint our_oam_no;
#endif
#endif
	guint32 ipv4_subnet_mask;
	unsigned int int_no;
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
#ifdef DYNAMIC_IP_ALLOCATION
	gchar hot_ipv4_address_str[MAX_IPV4_STRING_SIZE];
#ifdef KIATEL_UDP_FOR_OAM
	gchar win_oam_terminal_remote_ipv4_address_str[NO_KIATEL_UDP_OAM_TERMINAL][MAX_IPV4_STRING_SIZE];
#endif
#else
	gchar ipv4_address_str[MAX_IPV4_STRING_SIZE];
// raft payeen
//	guint tftpd_socket_no;
#endif
#endif
#if (((defined KIATEL_UDP_INSTALLED) && (defined DYNAMIC_IP_ALLOCATION)) || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE))
	guint tftpd_socket_no;
#endif
#ifdef KFTPD_INSTALLED
	guint no_active_ftp_client;
	guint max_ftp_client;
#endif
#ifdef TELNETD_INSTALLED
	guint no_active_telnet_client;
	guint max_telnet_client;
#endif
#ifdef HI1_INSTALLED
	guint no_active_hi1_client;
	guint max_hi1_client;
#endif
}KIATEL_SYSTEM_STRUCT;
#endif

#ifdef KIATEL_UDP_INSTALLED
#define KIATEL_UDP_UPPER_LAYER_WIN_OAM_TERMINAL_TYPE	 0
//#define KIATEL_UDP_UPPER_LAYER_OAM_BILLING_TYPE	   1
//#define KIATEL_UDP_UPPER_LAYER_ALARM_PANEL_TYPE	   2
#define KIATEL_UDP_UPPER_LAYER_TFTPD_TYPE		 3
#define KIATEL_UDP_UPPER_LAYER_OUR_OAM_TERMINAL_TYPE	 4
#define KIATEL_UDP_UPPER_LAYER_HTBY_TYPE		 5
#define KIATEL_UDP_UPPER_LAYER_SYSCONUDP_TYPE		 6

#define KIATEL_UDP_UPPER_LAYER_NULL_TYPE	     (guchar)(-1)

#define KIATEL_UDP_NETBUF_SIZE			4096
#define MAX_KIATEL_UDP_DATA_CHUNK		2048

typedef struct{
	guint rp;
	guint lp;
	guchar state;
	guint32 remote_address;
	guint remote_port;
	guint local_port;
	unsigned int ucb_no;
	guchar upper_layer_type;
	guint upper_layer_no;
	BUF  netrx_st;
//	guchar rx[KIATEL_UDP_NETBUF_SIZE];
	guint kt_udp_no;
	guchar	rdy;
}KIATEL_UDP_STRUCT;

#define MAX_KIATEL_UDP_REC	16
#define NULL_KIATEL_UDP_REC	(guint)(-1)

typedef struct{
	guint32 remote_address;
	guint remote_port;
	guint local_port;
}KIATEL_UDP_KEY_STRUCT;
#endif

#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
guchar send_pckt_to_oam_terminal_on_kiatel_udp(char *send_buf, unsigned short send_buf_ptr, unsigned int kt_udp_no);
void send_directory_message_to_terminal_win_oam(char *dirbuf);
void send_heartbeat_message_to_all_win_oams(void);
void send_credit_message_to_all_win_oams(char *str);
void send_authentication_message_to_all_win_oams(void);
void send_system_message_to_all_win_oams(char *);
void send_alarm_message_to_all_win_oams(char *);
void send_traffic_message_to_all_win_oams(char *);
void send_voice_message_to_all_win_oams(guchar voice_type);
void send_cas3_historical_message_to_all_win_oams(char *);
void send_billing_message_to_all_win_oams(char *str);
void send_malicious_message_to_all_win_oams(char *str);
void send_ss7_packet_message_to_all_win_oams(char *str);
void create_cas3_historical_string(char *buf,int parm,int j);
void init_cas3_hst_info(unsigned int ch);
typedef struct{
	unsigned int link_no;
	BYTE link_type;
	BYTE send_heartbeat;
	BYTE is_terminal;
	BYTE send_alarm;
	BYTE send_traffic;
	BYTE send_billing;
	unsigned int heartbeat;
}WINOAM_RCRD;
#endif

#if ((defined SYSCONUDP) && (defined SYSTEM_SIDE))
guchar send_pckt_to_sysconudp(WORD_MR mr1, unsigned int kt_udp_no);
typedef struct{
	unsigned int link_no;
}SYSCONUDP_RCRD;
#endif

#if ((defined KIATEL_UDP_INSTALLED) || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE) || (defined TCP_INSTALLED))
void read_kiatel_udp_config_file(void);
#endif

#ifdef KIATEL_UDP_INSTALLED
#ifdef SYSTEM_SIDE
void init_kiatel_udp(void);
void init_kiatel_udp_after_kt_g_malloc_init(void);
void init_kiatel_udp_rec(void);
guint find_free_kiatel_udp_rec(void);
void rls_kiatel_udp_rec(guint udp_no);
guchar add_to_allocated_kiatel_udp_list(guint kt_udp_no);
guchar remove_from_allocated_kiatel_udp_list(guint kt_udp_no);
void init_kiatel_udp_hash(void);
gboolean kiatel_udp_equal(gconstpointer k1, gconstpointer k2);
guint kiatel_udp_hash_rule(gconstpointer k);
gboolean insert_kiatel_udp_hash_table(guint kt_udp_no);
gboolean remove_kiatel_udp_hash_table(guint kt_udp_no);
void kiatel_udp_key_destroy(gpointer k);
guint lookup_kiatel_udp_hash_table(guint32 remote_address,guint remote_port,guint local_port);
void kiatel_udp_netrx_buffer_hndlr(void);
#endif
#ifdef OPERATOR_SIDE
guint32 find_ipv4_from_string(gchar *ipv4_str_org);
#endif
#endif

#ifdef IP_INSTALLED
#define ICMP_INSTALLED
/* Internet Control Message Protocol */

/* Message types */
#define ICMP_ECHO_REPLY 	0	/* Echo Reply */
#define ICMP_DEST_UNREACH	3	/* Destination Unreachable */
#define ICMP_QUENCH		4	/* Source Quench */
#define ICMP_REDIRECT		5	/* Redirect */
#define ICMP_ECHO		8	/* Echo Request */
#define ICMP_TIME_EXCEED	11	/* Time-to-live Exceeded */
#define ICMP_PARAM_PROB 	12	/* Parameter Problem */
#define ICMP_TIMESTAMP		13	/* Timestamp */
#define ICMP_TIME_REPLY 	14	/* Timestamp Reply */
#define ICMP_INFO_RQST		15	/* Information Request */
#define ICMP_INFO_REPLY 	16	/* Information Reply */
#define ICMP_ADDR_MASK		17	/* Address mask request */
#define ICMP_ADDR_MASK_REPLY	18	/* Address mask reply */
#define ICMP_IPSP		19	/* Problem with secured packet */
#define ICMP_TYPES		20

#define ICMPLEN 	8	/* Length of ICMP header on the net */

/* Destination Unreachable codes */
#define ICMP_NET_UNREACH	0	/* Net unreachable */
#define ICMP_HOST_UNREACH	1	/* Host unreachable */
#define ICMP_PROT_UNREACH	2	/* Protocol unreachable */
#define ICMP_PORT_UNREACH	3	/* Port unreachable */
#define ICMP_FRAG_NEEDED	4	/* Fragmentation needed and DF set */
#define ICMP_ROUTE_FAIL 	5	/* Source route failed */
#define ICMP_ADMIN_PROHIB	13	/* Administrativly prohibbited */

/* Internal format of an ICMP header (checksum is missing) */
struct icmp {
	guint8 type;
	guint8 code;
	union icmp_args {
		guint mtu;
		gint32 unused;
		guint8 pointer;
		gint32 address;
		struct {
			guint id;
			guint seq;
		} echo;
	} args;
};
int icmp_output(IP_STRUCT *ip_pt _U_,WORD_MR mr1 _U_,guint8 type _U_,
			  guint8 code _U_,union icmp_args *args _U_);
#ifdef ICMP_INSTALLED
struct mib_entry {
	char *name;
	union {
		gint32 integer;
	} value;
};

/* SNMP MIB variables, used for statistics and control. See RFC 1066 */
#define icmpInMsgs		Icmp_mib[1].value.integer
#define icmpInErrors		Icmp_mib[2].value.integer
#define icmpInDestUnreachs	Icmp_mib[3].value.integer
#define icmpInTimeExcds 	Icmp_mib[4].value.integer
#define icmpInParmProbs 	Icmp_mib[5].value.integer
#define icmpInSrcQuenchs	Icmp_mib[6].value.integer
#define icmpInRedirects 	Icmp_mib[7].value.integer
#define icmpInEchos		Icmp_mib[8].value.integer
#define icmpInEchoReps		Icmp_mib[9].value.integer
#define icmpInTimestamps	Icmp_mib[10].value.integer
#define icmpInTimestampReps	Icmp_mib[11].value.integer
#define icmpInAddrMasks 	Icmp_mib[12].value.integer
#define icmpInAddrMaskReps	Icmp_mib[13].value.integer
#define icmpOutMsgs		Icmp_mib[14].value.integer
#define icmpOutErrors		Icmp_mib[15].value.integer
#define icmpOutDestUnreachs	Icmp_mib[16].value.integer
#define icmpOutTimeExcds	Icmp_mib[17].value.integer
#define icmpOutParmProbs	Icmp_mib[18].value.integer
#define icmpOutSrcQuenchs	Icmp_mib[19].value.integer
#define icmpOutRedirects	Icmp_mib[20].value.integer
#define icmpOutEchos		Icmp_mib[21].value.integer
#define icmpOutEchoReps 	Icmp_mib[22].value.integer
#define icmpOutTimestamps	Icmp_mib[23].value.integer
#define icmpOutTimestampReps	Icmp_mib[24].value.integer
#define icmpOutAddrMasks	Icmp_mib[25].value.integer
#define icmpOutAddrMaskReps	Icmp_mib[26].value.integer
#define NUMICMPMIB	26

void icmp_input(unsigned int iface_no,IP_STRUCT *ip,WORD_MR mr1,int rxbroadcast,gint32 said _U_);
int ntohicmp(struct icmp *icmp,WORD_MR *);
void htonicmp(struct icmp *icmp,WORD_MR *);

#define NUNREACH	14

/* Time Exceeded codes */
#define ICMP_TTL_EXCEED 	0	/* Time-to-live exceeded */
#define ICMP_FRAG_EXCEED	1	/* Fragment reassembly time exceeded */

#define NEXCEED 	2

/* Redirect message codes */
#define ICMP_REDR_NET	0	/* Redirect for the network */
#define ICMP_REDR_HOST	1	/* Redirect for the host */
#define ICMP_REDR_TOS	2	/* Redirect for Type of Service, or-ed with prev */

#define NREDIRECT	3

/* Bad security packet codes */
#define ICMP_IPSP_SPI_UNKNOWN	0
#define ICMP_IPSP_AUTH_FAIL	1
#define ICMP_IPSP_DECRYPT_FAIL	2
#define NIPSP			3

struct icmplink {
	guint8 proto;
	void (*funct)(gint32,gint32,gint32,guint8,guint8,WORD_MR);
};
void restore(int state);
#endif // ICMP_INSTALLED
//MAHGOL start
#ifdef IP_INSTALLED
char * mesle_adamizad_inet_ntoa(gint32 a);
#endif
//MAHGOL end
#endif // IP_INSTALLED

