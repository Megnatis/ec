
/*************	Time-stamp: <2009-03-04 09:48:44 savoj>   ************/
#ifdef ETHERNET_INSTALLED
PACKET_DRIVER_STRUCT pktdrvr_st[PK_MAX];
BUF pktdrvr_tx_st[PK_MAX];
guint8 pktdrvr_tx[PK_MAX][PK_BUF_SIZE];
//BUF pktdrvr_rx_st[PK_MAX];
//guint8 pktdrvr_rx[PK_MAX][PK_BUF_SIZE];
#endif // ETHERNET_INSTALLED

#ifdef ARP_INSTALLED
/* Hash table headers */
ARP_TAB_STRUCT *Arp_tab[HASHMOD];
ARP_STAT_STRUCT Arp_stat;
/* ARP protocol linkages, indexed by arp's hardware type */
ARP_TYPE_STRUCT Arp_type[NHWTYPES];
guint8 Ether_bdcst[6];
#endif // ARP_INSTALLED

#ifdef IP_INSTALLED
IFACE_STRUCT iface_st[MAX_IFACE];
unsigned int Ifaces;
BUF ip_tx_st[MAX_IFACE];
BUF ip_rx_st[MAX_IFACE];
guint8 ip_rx[MAX_IFACE][IP_BUF_SIZE];
guint8 ip_tx[MAX_IFACE][IP_BUF_SIZE];
IP_ROUTE_STRUCT ip_route_st[NO_IP_ROUTE];
unsigned int eq_ip_route_st;
unsigned int fq_ip_route_st;
ROUTE_CACHE_STRUCT Rt_cache[HASHMOD];
guint32 Rtlookups;
guint32 Rtchits;
unsigned int Routes[32][HASHMOD];	/* Routing table */
unsigned int R_default; 		/* Default route entry */
unsigned int hopperq_e;
unsigned int hopperq_f;
HOPPER_QUE_STRUCT hopperq[HOPPER_QUE_LEN];
MIB_ENTRY_STRUCT Ip_mib[NUMIPMIB];
unsigned int Id_cntr;		 /* Datagram serial number */
REASM_STRUCT *Reasmq;
#ifdef ROUTER_SYSTEM
ROUTER_STRUCT router_st[MAX_ROUTER_PATH];
#endif
int Derr;
#endif // IP_INSTALLED

#ifdef TCP_INSTALLED
MIB_ENTRY_STRUCT Tcp_mib[MAX_TCP_MIB];
TCB_STRUCT tcb_st[MAX_TCB];
unsigned int eq_tcb_st;
unsigned int fq_tcb_st;
unsigned int Inet_list;
INET_STRUCT inet_st[MAX_INET];
unsigned int eq_inet_st;
unsigned int fq_inet_st;
unsigned int Tcp_window;
unsigned int Tcbs;	/* Head of control block list */
unsigned int Tcp_mss;	/* Maximum segment size to be sent with SYN */
gint32 Tcp_irtt;	/* Initial guess at round trip time */
int Tcp_trace;		/* State change tracing flag */
int Tcp_syndata;
TCP_RTT_STRUCT Tcp_rtt[RTTCACHE];
int Tcp_tstamps;
guint8 flag;
long msclock_cnt;
#endif // TCP_INSTALLED

#ifdef UDP_INSTALLED
unsigned int Udps;     /* Hash table for UDP structures */
UCB_STRUCT ucb_st[MAX_UCB];
unsigned int eq_ucb_st;
unsigned int fq_ucb_st;
MIB_ENTRY_STRUCT Udp_mib[MAX_UDP_MIB];
#endif // UDP_INSTALLED


#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
USOCK_STRUCT usock_st[MAX_USOCK];
unsigned int eq_usock_st;
unsigned int fq_usock_st;
unsigned int Nsock;
int errno;
int Net_error;
unsigned int Lport;
#endif

#ifdef ETHERNET_INSTALLED
#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM) ||\
     (defined HTBY_LINK_ON_MY_UDP) || (defined KIATEL_UDP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT) || (defined SYSCONUDP))
unsigned int oam_iface_no;
#endif
#endif
#if ((defined KIATEL_UDP_INSTALLED) || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE) || \
     (defined TCP_INSTALLED))
KIATEL_SYSTEM_STRUCT	 kt_system_st;
#endif

#ifdef KIATEL_UDP_INSTALLED
KIATEL_UDP_STRUCT	 kt_udp_rec[MAX_KIATEL_UDP_REC];
#ifdef SYSTEM_SIDE
guint			 fq_kt_udp_rec, eq_kt_udp_rec;
guint			 first_allocated_kt_udp,last_allocated_kt_udp;
KT_GHashTable		 *kt_udp_hash_table;
#endif
#endif

#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
WINOAM_RCRD winoam[NO_KIATEL_UDP_OAM_TERMINAL];
#endif
#if ((defined SYSCONUDP) && (defined SYSTEM_SIDE))
SYSCONUDP_RCRD sysconudp[NO_KIATEL_UDP_SYSCONUDP];
#endif

#ifdef ICMP_INSTALLED
struct mib_entry Icmp_mib[] = {
	{"",                     {0}},
	{"icmpInMsgs",           {0}},
	{"icmpInErrors",         {0}},
	{"icmpInDestUnreachs",   {0}},
	{"icmpInTimeExcds",      {0}},
	{"icmpInParmProbs",      {0}},
	{"icmpInSrcQuenchs",     {0}},
	{"icmpInRedirects",      {0}},
	{"icmpInEchos",          {0}},
	{"icmpInEchoReps",       {0}},
	{"icmpInTimestamps",     {0}},
	{"icmpInTimestampReps",  {0}},
	{"icmpInAddrMasks",      {0}},
	{"icmpInAddrMaskReps",   {0}},
	{"icmpOutMsgs",          {0}},
	{"icmpOutErrors",        {0}},
	{"icmpOutDestUnreachs",  {0}},
	{"icmpOutTimeExcds",     {0}},
	{"icmpOutParmProbs",     {0}},
	{"icmpOutSrcQuenchs",    {0}},
	{"icmpOutRedirects",     {0}},
	{"icmpOutEchos",         {0}},
	{"icmpOutEchoReps",      {0}},
	{"icmpOutTimestamps",    {0}},
	{"icmpOutTimestampReps", {0}},
	{"icmpOutAddrMasks",     {0}},
	{"icmpOutAddrMaskReps",  {0}}
};
struct icmplink Icmplink[] = {
	{0,		 0}
};
int Icmp_trace;
int Icmp_echo = 1;
/* ICMP message types */
char *Icmptypes[] = {
	"Echo Reply",
	NULL_POINTER,
	NULL_POINTER,
	"Unreachable",
	"Source Quench",
	"Redirect",
	NULL_POINTER,
	NULL_POINTER,
	"Echo Request",
	NULL_POINTER,
	NULL_POINTER,
	"Time Exceeded",
	"Parameter Problem",
	"Timestamp",
	"Timestamp Reply",
	"Information Request",
	"Information Reply",
	"Address mask request",
	"Address mask reply",
	"Security Association"
};

/* ICMP unreachable messages */
char *Unreach[] = {
	"Network",
	"Host",
	"Protocol",
	"Port",
	"Fragmentation",
	"Source route",
	"Dest net unknown",
	"Dest host unknown",
	"Source host isolated",
	"Net prohibited",
	"Host prohibited",
	"Net TOS",
	"Host TOS",
	"Administratively Prohibited"
};
/* ICMP Time exceeded messages */
char *Exceed[] = {
	"Time-to-live",
	"Fragment reassembly"
};

/* ICMP redirect messages */
char *Redirect[] = {
	"Network",
	"Host",
	"TOS & Network",
	"TOS & Host"
};
/* Bad SAID messages */
char *Said_icmp[] = {
	"Unknown SAID",
	"Authentication failed",
	"Decryption failed"
};

volatile gint32 Clock;
#endif
