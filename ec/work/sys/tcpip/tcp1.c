
// ERROR : 0x2000-0x2099

#define HANG_DEBUG
#define COMPILER_NULL_IS_USED

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else
#include "hdr.h"
#include "/ec/work/sys/tcpip/tcpipvar.ext"
#ifdef KIATEL_DYNAMIC_MEM
extern MEM_REC_ST mem_rec[];
#endif
#ifdef TCP_INSTALLED
extern TMR timer[TIMER_NO];
#ifdef MY_TCPIP_FOR_OAM
#ifdef OPERATOR_SIDE
extern unsigned int tcb0,tcb1;
#endif
#endif
#endif
#endif // CENTRAL_EXTERN

#ifdef KA9Q_TCP_DEBUG_ACT
#define DEBUG_ACT
#endif

/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int so_tcp(USOCK_STRUCT *up,int protocol _U_){

	up->type = USOCK_TYPE_TCP;
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
guint so_tcp_listen(USOCK_STRUCT *up,int backlog){

	SOCKADDR_IN_STRUCT *local;
	SOCKET_STRUCT lsock;
/*
	if(up->name == NULL)
		autobind(up);
*/
	if(up->localname_mr1 >= NO_MEM_REC){
		autobind(up);
	}
//	local = (SOCKADDR_IN_STRUCT *)up->name;
	local = (SOCKADDR_IN_STRUCT *)(mem_rec[up->localname_mr1].parm);
	lsock.address = local->sin_addr.s_addr;
	lsock.port = local->sin_port;
	up->cb.tcb_no = open_tcp(&lsock,NULL,backlog ? TCP_SERVER:TCP_PASSIVE,0,
			      TCP_NORMAL_RTNS_TYPE1,up->tos,up->index);
	return up->cb.tcb_no;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
guint so_tcp_conn(USOCK_STRUCT *up){

	int s;
//	unsigned int tcb_no;
	SOCKET_STRUCT lsock,fsock;
	SOCKADDR_IN_STRUCT *local,*remote;

#ifdef DEBUG_ACT
	prnt_string("so_tcp_conn start:\r\n");
#endif
#if 0
	if(up->name == NULL){
		autobind(up);
	}
#endif // 0
	if(up->localname_mr1 >= NO_MEM_REC){
		autobind(up);
	}
//	if(checkipaddr(up->peername,up->peernamelen) == -1){
	if(checkipaddr((SOCKADDR_STRUCT *)(mem_rec[up->peername_mr1].parm),mem_rec[up->peername_mr1].size) == -1){
		prnt_msg(TCP_ERR,0x2000);
		errno = EAFNOSUPPORT_ERROR;
		return NULL_TCB;
	}
	s = up->index;
	/* Construct the TCP-style ports from the sockaddr structs */
#if 0
	local = (struct sockaddr_in *)up->name;
	remote = (struct sockaddr_in *)up->peername;
#endif // 0
	local = (SOCKADDR_IN_STRUCT *)mem_rec[up->localname_mr1].parm;
	remote = (SOCKADDR_IN_STRUCT *)mem_rec[up->peername_mr1].parm;

	if(local->sin_addr.s_addr == INADDR_ANY)
		/* Choose a local address */
		local->sin_addr.s_addr = locaddr(remote->sin_addr.s_addr);

	lsock.address = local->sin_addr.s_addr;
	lsock.port = local->sin_port;
	fsock.address = remote->sin_addr.s_addr;
	fsock.port = remote->sin_port;

	/* Open the TCB in active mode */
	up->cb.tcb_no = open_tcp(&lsock,&fsock,TCP_ACTIVE,0,TCP_NORMAL_RTNS_TYPE1,up->tos,s);
#ifdef DEBUG_ACT
	prnt_int(up->cb.tcb_no);prnt_char(CR);
#endif

#if 0
	/* Wait for the connection to complete */
	while(((tcb_no = up->cb.tcb_no) < MAX_TCB) && (tcb_st[tcb_no].state != TCP_ESTABLISHED)){
		if(up->noblock){
			errno = EWOULDBLOCK_ERROR;
			prnt_msg(TCP_ERR,0x2001);
			return NULL_TCB;
		}
//		else if((errno = kwait(up)) != 0){
//			return NULL_TCB;
//		}
	}
	if(tcb_no == NULL_TCB){
		/* Probably got refused */
//		FREE(up->peername);
		mem_rec_rls(up->peername_mr1);
		errno = ECONNREFUSED_ERROR;
		prnt_msg(TCP_ERR,0x2002);
		return NULL_TCB;
	}
#endif // 0
#ifdef DEBUG_ACT
	prnt_string("so_tcp_conn end:");prnt_int(up->cb.tcb_no);prnt_char(CR);
#endif
	return (up->cb.tcb_no);
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int so_tcp_qlen(USOCK_STRUCT *up,int rtx){

	int len;

	switch(rtx){
	 case 0:
		len = tcb_st[up->cb.tcb_no].rcvcnt;
		break;
	 case 1:
		len = tcb_st[up->cb.tcb_no].sndcnt;
		break;
//alaki
	 default	:
		prnt_msg(TCP_ERR,0x2003);
		return 0;
	}
	return len;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int so_tcp_kick(USOCK_STRUCT *up){

	kick_tcp(up->cb.tcb_no);
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int so_tcp_shut(USOCK_STRUCT *up,int how){

	switch(how){
	  case 0: /* No more receives -- replace upcall */
//		up->cb.tcb->r_upcall = trdiscard;
		tcb_st[up->cb.tcb_no].trdiscard_flag=SET;
		break;
	  case 1: /* Send EOF */
		close_tcp(up->cb.tcb_no);
		break;
	  case 2: /* Blow away TCB */
		reset_tcp(up->cb.tcb_no);
		up->cb.tcb_no = NULL_TCB;
		break;
	  default:
		prnt_msg(TCP_ERR,0x2004);
		break;
	}
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int so_tcp_close(USOCK_STRUCT *up){

	if(up->cb.tcb_no < MAX_TCB){ /* In case it's been reset */
//		up->cb.tcb->r_upcall = trdiscard;
		tcb_st[up->cb.tcb_no].trdiscard_flag=SET;
		/* Tell the TCP_CLOSED upcall there's no more socket */
		tcb_st[up->cb.tcb_no].user = -1;
		close_tcp(up->cb.tcb_no);
	}
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Discard data received on a TCP connection. Used after a receive shutdown or*/
/* close_s until the TCB disappears.					      */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void trdiscard(unsigned int tcb_no,guint32 cnt){

	WORD_MR mr1;

	recv_tcp(tcb_no,&mr1,cnt);
	mem_rec_rls(mr1);
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Issue an automatic bind of a local address */
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
void autobind(USOCK_STRUCT *up){

	SOCKADDR_IN_STRUCT local;
#ifdef DEBUG_ACT
	prnt_string("autobind:\r\n");
#endif
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = Lport++;
	bind(up->index,(SOCKADDR_STRUCT *)&local,sizeof(SOCKADDR_IN_STRUCT));
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ======================================================================== */
/* ======================================================================== */
#if 0
#ifdef TCP_INSTALLED
char *tcpstate(USOCK_STRUCT *up){

	if(up->cb.tcb_no >= MAX_TCB)
		return NULL;
	return Tcpstates[tcb_st[up->cb.tcb_no].state];
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int so_tcp_stat(USOCK_STRUCT *up){

	st_tcp(up->cb.tcb_no);
	return 0;
}
#endif // TCP_INSTALLED
#endif // 0
/* ======================================================================== */
/* TCP state change upcall routine					    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void s_tscall(unsigned int tcb_no,unsigned int old _U_,unsigned int new){

	unsigned int s,ns;
	USOCK_STRUCT *up,*nup,*oup;
	union sp sp;
	TCB_STRUCT *tcb_pt;
	WORD_MR mr1;

#ifdef DEBUG_ACT
	prnt_string("s_tscall:");prnt_int(tcb_no);toprnq(CR);
#endif
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2005);
		return;
	}
	tcb_pt=&tcb_st[tcb_no];
	s = tcb_pt->user;
	switch(new){
	  case TCP_CLOSED:
		/* Clean up. If the user has already closed the socket,
		 * then up will be null (s was set to -1 by the close routine).
		 * If not, then this is an abnormal close (e.g., a reset)
		 * and clearing out the pointer in the socket structure will
		 * prevent any further operations on what will be a freed
		 * control block. Also wake up anybody waiting on events
		 * related to this tcb so they will notice it disappearing.
		 */
		if(s < Nsock){
			oup = up =&usock_st[s];
			up->cb.tcb_no = NULL_TCB;
			up->errcodes[0] = tcb_pt->reason;
			up->errcodes[1] = tcb_pt->type;
			up->errcodes[2] = tcb_pt->code;
//			ksignal(up,0); /* Wake up anybody waiting */
		}
		del_tcp(tcb_no);
		break;
	  case TCP_SYN_RECEIVED:
		/* Handle an incoming connection. If this is a server TCB,
		 * then we're being handed a "clone" TCB and we need to
		 * create a new socket structure for it. In either case,
		 * find out who we're talking to and wake up the guy waiting
		 * for the connection.
		 */
		if(s >= Nsock){
			prnt_msg(TCP_ERR,0x2006);
			return;
		}
		oup = up =&usock_st[s];
		if(tcb_pt->clone_flag){
			/* Clone the socket */
			ns = socket(AF_INET,SOCK_STREAM,0);
			if(ns >= Nsock){
				prnt_msg(TCP_ERR,0x2007);
				return;
			}
			nup = &usock_st[ns];
			mem_cpy((guint8 *)nup,(guint8 *)up,sizeof(USOCK_STRUCT));
//			ASSIGN(*nup,*up);
			tcb_pt->user = ns;
			nup->cb.tcb_no = tcb_no;
			/* Allocate new memory for the name areas */
			mr1=mem_rec_req(SOCKSIZE);
			if(mr1 >= NO_MEM_REC){
				prnt_msg(TCP_ERR,0x2008);
				return;
			}
			nup->localname_mr1=mr1;
			mr1=mem_rec_req(SOCKSIZE);
			if(mr1 >= NO_MEM_REC){
				prnt_msg(TCP_ERR,0x2009);
				return;
			}
			nup->peername_mr1=mr1;
#if 0
			nup->name = mallocw(SOCKSIZE);
			nup->peername = mallocw(SOCKSIZE);
#endif // 0
//			nup->index = ns;
			/* Store the new socket # in the old one */
			up->rdysock = ns;
			up = nup;
			s = ns;
		} else {
			/* Allocate space for the peer's name */
			mr1=mem_rec_req(SOCKSIZE);
			if(mr1 >= NO_MEM_REC){
				prnt_msg(TCP_ERR,0x2010);
				return;
			}
			up->peername_mr1=mr1;
#if 0
			up->peername = mallocw(SOCKSIZE);
#endif // 0
			/* Store the old socket # in the old socket */
			up->rdysock = s;
		}
		/* Load the addresses. Memory for the name has already
		 * been allocated, either above or in the original bind.
		 */
//		sp.sa = up->name;
		sp.sa=(SOCKADDR_STRUCT *)(mem_rec[up->localname_mr1].parm);
		sp.in->sin_family = AF_INET;
		sp.in->sin_addr.s_addr = tcb_st[up->cb.tcb_no].conn.local.address;
		sp.in->sin_port = tcb_st[up->cb.tcb_no].conn.local.port;
//		up->namelen = SOCKSIZE;

//		sp.sa = up->peername;
		sp.sa=(SOCKADDR_STRUCT *)(mem_rec[up->peername_mr1].parm);
		sp.in->sin_family = AF_INET;
		sp.in->sin_addr.s_addr = tcb_st[up->cb.tcb_no].conn.remote.address;
		sp.in->sin_port = tcb_st[up->cb.tcb_no].conn.remote.port;
//		up->peernamelen = SOCKSIZE;

		/* Wake up the guy accepting it, and let him run */
//		ksignal(oup,1);
//		kwait(NULL);
		break;
	  default:	  /* Ignore all other state transitions */
		break;
	}
//	ksignal(up,0);	/* In case anybody's waiting */

}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* State change upcall that takes incoming TCP connections		    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void i_upcall(unsigned int tcb_no,unsigned int oldstate,unsigned int newstate){

	unsigned int inet_no;
	SOCKADDR_IN_STRUCT sock;
	USOCK_STRUCT *usock_pt;
	TCB_STRUCT *tcb_pt;
	int s;
	WORD_MR mr1;

#ifdef DEBUG_ACT
	prnt_string("i_upcall\r\n");
#endif
	if(oldstate != TCP_LISTEN){
		prnt_msg(TCP_ERR,0x2012);
		prnt_int(tcb_no);prnt_char('-');
		prnt_byte(oldstate);prnt_char('-');
		prnt_byte(tcb_st[tcb_no].type);prnt_char(CR);
		return; /* "can't happen" */
	}
	if(newstate == TCP_CLOSED){
		prnt_msg(TCP_ERR,0x2013);
		/* Called when server is shut down */
		del_tcp(tcb_no);
		return;
	}
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2014);
		return;
	}
	tcb_pt=&tcb_st[tcb_no];
	for(inet_no=Inet_list;inet_no<MAX_INET;inet_no=inet_st[inet_no].next){
		if(tcb_st[inet_st[inet_no].tcb_no].conn.local.port ==
				    tcb_st[tcb_no].conn.local.port){
			break;
		}
	}
	if(inet_no >= MAX_INET){
		prnt_msg(TCP_ERR,0x2015);
		return; /* not in table - "can't happen" */
	}
	/* Create a socket, hook it up with the TCB */
	s=socket(AF_INET,SOCK_STREAM,0);
	if((unsigned int)s >= Nsock){
		prnt_msg(TCP_ERR,0x2016);
		return;
	}
	usock_pt=&usock_st[s];
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = tcb_pt->conn.local.address;
	sock.sin_port = tcb_pt->conn.local.port;
	bind(s,(SOCKADDR_STRUCT *)&sock,SOCKSIZE);

	sock.sin_addr.s_addr = tcb_pt->conn.remote.address;
	sock.sin_port = tcb_pt->conn.remote.port;
	mr1=mem_rec_req(SOCKSIZE);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2017);
		return;
	}
	mem_cpy(mem_rec[mr1].parm,(guint8 *)(&sock),SOCKSIZE);
	usock_pt->peername_mr1=mr1;

	usock_pt->cb.tcb_no = tcb_no;
	tcb_pt->user = s;
	tcb_pt->rtn_type=TCP_NORMAL_RTNS_TYPE1;

#if 0
	/* Set the normal upcalls */
	tcb_pt->r_upcall = s_trcall;
	tcb_pt->t_upcall = s_ttcall;
	tcb_pt->s_upcall = s_tscall;

	/* And spawn the server task */
	newproc(inet_pt->name,inet_pt->stack,inet_pt->task,s,NULL,NULL,0);
#endif // 0
	tcb_st[tcb_no].trdiscard_flag=RESET;
	switch(tcb_st[tcb_no].type){
#ifdef Q931_FOR_H225
	case H225_Q931_TCP_TYPE   :
		break;
#endif
#ifdef H245_GATEWAY_SIDE
	case H245_TCP_TYPE	  :
		break;
#endif
#ifdef MY_TCPIP_FOR_OAM
	case OAM_TCP_TYPE	  :
		break;
#endif
#ifdef KFTPD_INSTALLED
	case KFTPD_CONTROL_TCP_TYPE	  :
		kftpd_new_client_connected_rtn(tcb_no);
		break;
	case KFTPD_DATA_TCP_TYPE	  :
		break;
#endif
#ifdef TELNETD_INSTALLED
	case TELNETD_TCP_TYPE	  :
		telnetd_new_client_connected_rtn(tcb_no);
		break;
#endif
#ifdef HI1_INSTALLED
	case HI1_TCP_TYPE     :
		hi1_new_client_connected_rtn(tcb_no);
		break;
#endif
#ifdef HI1_DISTRIBUTOR
	case HI1_DISTRIBUTOR_TCP_TYPE	  :
		break;
#endif
#ifdef SYSCON_SERVER
	case SYSCON_SERVER_TCP_TYPE	:
		syscon_server_new_client_connected_rtn(tcb_no);
		break;
#endif
#ifdef SYSCON_CLIENT
	case SYSCON_CLIENT_TCP_TYPE	:
		break;
#endif
	default 		  :
		prnt_msg(TCP_ERR,0x2094);
		break;
	}
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Create a user socket, return socket index					*/
/* The mapping to actual protocols is as follows:				*/
/*										*/
/*										*/
/* ADDRESS FAMILY	Stream		Datagram	Raw	    Seq. Packet */
/*										*/
/* AF_INET		TCP		UDP		IP			*/
/* AF_AX25		I-frames	UI-frames				*/
/* AF_NETROM						NET/ROM L3  NET/ROM L4	*/
/* AF_LOCAL		stream loopback packet loopback 			*/
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
unsigned int socket(
unsigned int af,	 /* Address family */
unsigned int type,	 /* Stream or datagram */
unsigned int protocol	 /* Used for raw IP sockets */
){
	USOCK_STRUCT *usock_pt;
	unsigned int s;

	s=find_free_usock_st();
	if(s >= Nsock){
		prnt_msg(IP_ERR,0x2098);
		errno = EMFILE_ERROR;
		return -1;
	}
	usock_pt=&usock_st[s];
	usock_pt->state=IN_USE;
	usock_pt->sock_value =_mk_fd(s,_FL_SOCK);
	usock_pt->index = s;
	usock_pt->refcnt = 1;
	errno = 0;
	usock_pt->rdysock = -1;
//	usock_pt->owner = Curproc;
	switch(af){
	  case AF_LOCAL:
		switch(type){
		  case SOCK_STREAM:
			usock_pt->type = USOCK_TYPE_LOCAL_STREAM;
			break;
		  case SOCK_DGRAM:
			usock_pt->type = USOCK_TYPE_LOCAL_DGRAM;
			break;
		  default:
			errno = ESOCKTNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2018);
			break;
		}
		break;
	  case AF_AX25:
		switch(type){
		  case SOCK_STREAM:
			usock_pt->type = USOCK_TYPE_AX25I;
			break;
		  case SOCK_DGRAM:
			usock_pt->type = USOCK_TYPE_AX25UI;
			break;
		  default:
			errno = ESOCKTNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2019);
			break;
		}
		break;
	  case AF_NETROM:
		switch(type){
		  case SOCK_RAW:
			usock_pt->type = USOCK_TYPE_NETROML3;
			break;
		  case SOCK_SEQPACKET:
			usock_pt->type = USOCK_TYPE_NETROML4;
			break;
		  default:
			prnt_msg(IP_ERR,0x2020);
			errno = ESOCKTNOSUPPORT_ERROR;
			break;
		}
		break;
	  case AF_INET:
		switch(type){
		  case SOCK_STREAM:
			usock_pt->type = USOCK_TYPE_TCP;
			break;
		  case SOCK_DGRAM:
			usock_pt->type = USOCK_TYPE_UDP;
			break;
		  case SOCK_RAW:
			usock_pt->type = USOCK_TYPE_RAW;
			break;
		  default:
			errno = ESOCKTNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2021);
			break;
		}
		break;
	  default:
		errno = EAFNOSUPPORT_ERROR;
		prnt_msg(IP_ERR,0x2022);
		break;
	}
	switch(usock_pt->type){
#ifdef TCP_INSTALLED
	  case USOCK_TYPE_TCP	      :
		if(so_tcp(usock_pt,protocol) == -1){
			errno = ESOCKTNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2023);
			return -1;
		}
		return s;
#endif
#ifdef UDP_INSTALLED
	  case USOCK_TYPE_UDP	      :
		if(so_udp(usock_pt,protocol) == -1){
			errno = ESOCKTNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2024);
			return -1;
		}
		return s;
#endif
#if 0
	  case USOCK_TYPE_RAW	      :
		if(so_ip_sock(usock_pt,protocol) == -1){
			errno = ESOCKTNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2025);
			return -1;
		}
		return s;
#endif // 0
#ifdef	AX25
	  case USOCK_TYPE_AX25I       :
	  case USOCK_TYPE_AX25UI      :
		errno = ESOCKTNOSUPPORT_ERROR;
		prnt_msg(IP_ERR,0x2026);
		return -1;
#endif	/* AX25 */
#ifdef	NETROM
	  case USOCK_TYPE_NETROML3    :
	  case USOCK_TYPE_NETROML4    :
		errno = ESOCKTNOSUPPORT_ERROR;
		prnt_msg(IP_ERR,0x2027);
		return -1;
#endif	/* NETROM */

#ifdef	LOCSOCK
	  case USOCK_TYPE_LOCAL_STREAM:
	  case USOCK_TYPE_LOCAL_DGRAM :
		errno = ESOCKTNOSUPPORT_ERROR;
		prnt_msg(IP_ERR,0x2028);
		return -1;
#endif
	  default		:
		errno = ESOCKTNOSUPPORT_ERROR;
		prnt_msg(IP_ERR,0x2029);
		return -1;
	}
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ======================================================================== */
/* Attach a local address/port to a socket. If not issued before a connect  */
/* or listen, will be issued automatically				    */
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
unsigned int
bind(
unsigned int s, 	/* Socket index */
SOCKADDR_STRUCT *name,	/* Local name */
unsigned int namelen	/* Length of name */
){
	USOCK_STRUCT *usock_pt;
	WORD_MR mr1;

	if(s >= Nsock){
		errno = EBADF_ERROR;
		prnt_msg(IP_ERR,0x2030);
		return -1;
	}
	usock_pt=&usock_st[s];
	if(name == NULL){
		errno = EFAULT_ERROR;
		prnt_msg(IP_ERR,0x2031);
		return -1;
	}
	if(usock_pt->localname_mr1 < NO_MEM_REC){
		/* Bind has already been issued */
		errno = EINVAL_ERROR;
		prnt_msg(IP_ERR,0x2032);
		return -1;
	}
	switch(usock_pt->type){
#ifdef TCP_INSTALLED
	  case USOCK_TYPE_TCP	      :
		if(checkipaddr(name,namelen) == -1){
			/* Incorrect length or family for chosen protocol */
			errno = EAFNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2033);
			return -1;
		}
		break;
#endif
#ifdef UDP_INSTALLED
	  case USOCK_TYPE_UDP	      :
		if(checkipaddr(name,namelen) == -1){
			/* Incorrect length or family for chosen protocol */
			errno = EAFNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2034);
			return -1;
		}
		break;
#endif
#if 0
	  case USOCK_TYPE_RAW	      :
		if(checkipaddr(name,namelen) == -1){
			/* Incorrect length or family for chosen protocol */
			errno = EAFNOSUPPORT_ERROR;
			prnt_msg(IP_ERR,0x2035);
			return -1;
		}
		break;
#endif // 0
#ifdef	AX25
	  case USOCK_TYPE_AX25I       :
	  case USOCK_TYPE_AX25UI      :
		prnt_msg(IP_ERR,0x2036);
		break;
#endif	/* AX25 */
#ifdef	NETROM
	  case USOCK_TYPE_NETROML3    :
	  case USOCK_TYPE_NETROML4    :
		prnt_msg(IP_ERR,0x2037);
		break;
#endif	/* NETROM */

#ifdef	LOCSOCK
	  case USOCK_TYPE_LOCAL_STREAM:
	  case USOCK_TYPE_LOCAL_DGRAM :
		prnt_msg(IP_ERR,0x2038);
		break;
#endif
	  default		:
		prnt_msg(IP_ERR,0x2039);
		break;
	}
	/* Stash name in an allocated block */
	mr1=mem_rec_req(namelen);
	if(mr1 >= NO_MEM_REC){
		errno = EFAULT_ERROR;
		prnt_msg(IP_ERR,0x2040);
		return -1;
	}
	mem_cpy(mem_rec[mr1].parm,(guint8 *)name,namelen);
	usock_pt->localname_mr1=mr1;

	/* a bind routine is optional - don't fail if it isn't present */
	switch(usock_pt->type){
#ifdef TCP_INSTALLED
	  case USOCK_TYPE_TCP	      :
		return 0;
#endif
#ifdef UDP_INSTALLED
	  case USOCK_TYPE_UDP	      :
		if(so_udp_bind(usock_pt) == -1){
			errno = EOPNOTSUPP_ERROR;
			prnt_msg(IP_ERR,0x2041);
			return -1;
		}
		return 0;
#endif
#if 0
	  case USOCK_TYPE_RAW	      :
		return 0;
#endif // 0
#ifdef	AX25
	  case USOCK_TYPE_AX25I       :
	  case USOCK_TYPE_AX25UI      :
		prnt_msg(IP_ERR,0x2042);
		errno = EOPNOTSUPP_ERROR;
		return -1;
#endif	/* AX25 */
#ifdef	NETROM
	  case USOCK_TYPE_NETROML3    :
	  case USOCK_TYPE_NETROML4    :
		prnt_msg(IP_ERR,0x2043);
		errno = EOPNOTSUPP_ERROR;
		return -1;
#endif	/* NETROM */

#ifdef	LOCSOCK
	  case USOCK_TYPE_LOCAL_STREAM:
	  case USOCK_TYPE_LOCAL_DGRAM :
		prnt_msg(IP_ERR,0x2044);
		errno = EOPNOTSUPP_ERROR;
		return -1;
#endif
	  default		:
		prnt_msg(IP_ERR,0x2045);
		errno = EOPNOTSUPP_ERROR;
		return -1;
	}
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ======================================================================== */
/* Generate an initial sequence number and put a SYN on the send queue	    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void send_syn(unsigned int tcb_no){

	TCB_STRUCT *tcb_pt;

	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2046);
		return;
	}
	tcb_pt=&tcb_st[tcb_no];
	tcb_pt->iss = geniss();
	tcb_pt->rttseq = tcb_pt->snd_wl2 = tcb_pt->snd_una = tcb_pt->iss;
	tcb_pt->snd_ptr = tcb_pt->snd_nxt = tcb_pt->rttseq;
	tcb_pt->sndcnt++;
	tcb_pt->force_flag = 1;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Convert TCP header in host format into mbuf ready for transmission,	    */
/* link in data (if any).						    */
/*									    */
/* If checksum field is zero, recompute it, otherwise take the value	    */
/* in the host header.							    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void htontcp(
TCP_STRUCT *tcp_pt,
WORD_MR *mr3,		/* Data in, packet out */
guint32 ipsrc,		 /* For computing header checksum */
guint32 ipdest
){
	unsigned int hdrlen;
	guint8 *cp;
	WORD_MR mr1;

#ifdef DEBUG_ACT
	prnt_string("htontcp\r\n");
#endif
	mr1=*mr3;
#if 0
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2047);
		return;
	}
#endif // 0
	hdrlen = TCPLEN;
	if(tcp_pt->mss_flag){
		hdrlen += MSS_LENGTH;
	}
	if(tcp_pt->tstamp_flag){
		hdrlen += TSTAMP_LENGTH;
	}
	if(tcp_pt->wscale_flag){
		hdrlen += WSCALE_LENGTH;
	}
	hdrlen = (hdrlen + 3) & 0xfc;	/* Round up to multiple of 4 */
//	pushdown(bpp,NULL,hdrlen);
//	cp = (*bpp)->data;
	if(mr1 >= NO_MEM_REC){
		mr1=mem_rec_req(hdrlen);
	}
	else{
		mr1=my_pushdown(mr1,hdrlen);
	}
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2048);
		return;
	}
	cp = mem_rec[mr1].parm;
	mem_set(cp,0,hdrlen);
	cp = put16(cp,tcp_pt->source);
	cp = put16(cp,tcp_pt->dest);
	cp = put32(cp,tcp_pt->seq);
	cp = put32(cp,tcp_pt->ack);
	*cp++ = hdrlen << 2;	/* Offset field */
	*cp = 0;
	if(tcp_pt->congest_flag)
		*cp |= 64;
	if(tcp_pt->urg_flag)
		*cp |= 32;
	if(tcp_pt->ack_flag)
		*cp |= 16;
	if(tcp_pt->psh_flag)
		*cp |= 8;
	if(tcp_pt->rst_flag)
		*cp |= 4;
	if(tcp_pt->syn_flag)
		*cp |= 2;
	if(tcp_pt->fin_flag)
		*cp |= 1;
	cp++;
	cp = put16(cp,tcp_pt->wnd);
	cp = put16(cp,tcp_pt->checksum);
	cp = put16(cp,tcp_pt->up);

	/* Write options, if any */
	if(tcp_pt->mss_flag){
		*cp++ = MSS_KIND;
		*cp++ = MSS_LENGTH;
		cp = put16(cp,tcp_pt->mss);
	}
	if(tcp_pt->tstamp_flag){
		*cp++ = TSTAMP_KIND;
		*cp++ = TSTAMP_LENGTH;
		cp = put32(cp,tcp_pt->tsval);
		cp = put32(cp,tcp_pt->tsecr);
	}
	if(tcp_pt->wscale_flag){
		*cp++ = WSCALE_KIND;
		*cp++ = WSCALE_LENGTH;
		*cp++ = tcp_pt->wsopt;
	}
	if(tcp_pt->checksum == 0){
		/* Recompute header checksum */
		PSEUDO_HEADER_STRUCT ph;

		ph.source = ipsrc;
		ph.dest = ipdest;
		ph.protocol = TCP_PTCL;
//		ph.length = len_p(*bpp);
//		put16(&(*bpp)->data[16],cksum(&ph,*bpp,ph.length));
		ph.length = mem_rec[mr1].size;
		put16(&(mem_rec[mr1].parm[16]),cksum(&ph,mr1,ph.length));
	}
	*mr3=mr1;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Pull TCP header off mbuf */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int ntohtcp(
TCP_STRUCT *tcp_pt,
WORD_MR *mr3
){
	int hdrlen,i,optlen,kind;
	int flags;
	guint8 hdrbuf[TCPLEN],*cp;
	guint8 options[TCP_MAXOPT];
	WORD_MR mr1;
#ifdef DEBUG_ACT
	prnt_string("ntohtcp start\r\n");
	print_mr(*mr3);
#endif
	mr1=*mr3;
	mem_set(tcp_pt,0,sizeof(TCP_STRUCT));
//	i = pullup(bpp,hdrbuf,TCPLEN);
	mr1=my_pullup_copy_data(mr1,hdrbuf,TCPLEN);
	*mr3=mr1;
//SHARABSHARAB : raft payeen
/*
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2049);
		return -1;
	}
*/
	/* Note that the results will be garbage if the header is too short.
	 * We don't check for this because returned ICMP messages will be
	 * truncated, and we at least want to get the port numbers.
	 */
	tcp_pt->source = get16(&hdrbuf[0]);
	tcp_pt->dest = get16(&hdrbuf[2]);
	tcp_pt->seq = get32(&hdrbuf[4]);
	tcp_pt->ack = get32(&hdrbuf[8]);
	hdrlen = (hdrbuf[12] & 0xf0) >> 2;
	flags = hdrbuf[13];
	tcp_pt->congest_flag = (flags & 64) ? 1 : 0;
	tcp_pt->urg_flag = (flags & 32) ? 1 : 0;
	tcp_pt->ack_flag = (flags & 16) ? 1 : 0;
	tcp_pt->psh_flag = (flags & 8) ? 1 : 0;
	tcp_pt->rst_flag = (flags & 4) ? 1 : 0;
	tcp_pt->syn_flag = (flags & 2) ? 1 : 0;
	tcp_pt->fin_flag = (flags & 1) ? 1 : 0;
	tcp_pt->wnd = get16(&hdrbuf[14]);
	tcp_pt->checksum = get16(&hdrbuf[16]);
	tcp_pt->up = get16(&hdrbuf[18]);
	optlen = hdrlen - TCPLEN;

	/* Check for option field */
//	if(i < TCPLEN || hdrlen < TCPLEN)
	if(hdrlen < TCPLEN){
		prnt_msg(TCP_ERR,0x2050);
		return -1;	/* Header smaller than legal minimum */
	}
	if(optlen == 0){
#ifdef DEBUG_ACT
		prnt_string("ntohtcp end1\r\n");
		print_mr(*mr3);
#endif
		return (int)hdrlen;	/* No options, all done */
	}

//SHARABSHARAB : oomad inja
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2049);
		prnt_int(hdrlen);prnt_char('-');
		prnt_int(TCPLEN);prnt_char('-');
		prnt_int(optlen);prnt_char(CR);
		return -1;
	}
//	if(optlen > len_p(*bpp)){
	if((unsigned int)optlen > mem_rec[mr1].size){
		/* Remainder too short for options length specified */
		prnt_msg(TCP_ERR,0x2051);
		return -1;
	}
//	pullup(bpp,options,optlen);	/* "Can't fail" */
	mr1=my_pullup_copy_data(mr1,options,optlen);	 /* "Can't fail" */
	*mr3=mr1;
#if 0
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2052);
		return -1;
	}
#endif // 0
	/* Process options */
	for(cp=options,i=optlen; i > 0;){
		kind = *cp++;
		i--;
		/* Process single-byte options */
		switch(kind){
		  case EOL_KIND:
			return (int)hdrlen;	/* End of options list */
		  case NOOP_KIND:
			continue;	/* Go look for next option */
		}
		/* All other options have a length field */
		optlen = *cp++;

		/* Process valid multi-byte options */
		switch(kind){
		  case MSS_KIND:
			if(optlen == MSS_LENGTH){
				tcp_pt->mss = get16(cp);
				tcp_pt->mss_flag = 1;
			}
			break;
		  case WSCALE_KIND:
			if(optlen == WSCALE_LENGTH){
				tcp_pt->wsopt = *cp;
				tcp_pt->wscale_flag = 1;
			}
			break;
		  case TSTAMP_KIND:
			if(optlen == TSTAMP_LENGTH){
				tcp_pt->tsval = get32(cp);
				tcp_pt->tsecr = get32(cp+4);
				tcp_pt->tstamp_flag = 1;
			}
			break;
		}
		optlen = max(2,optlen); /* Enforce legal minimum */
		i -= optlen;
		cp += optlen - 2;
	}
#ifdef DEBUG_ACT
	prnt_string("ntohtcp end1\r\n");
	print_mr(*mr3);
#endif
	return (int)hdrlen;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Copy data from within mbuf to user-provided buffer, starting at	    */
/* 'offset' bytes from start of mbuf and copying no more than 'len'         */
/* bytes. Return actual number of bytes copied				    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
unsigned int
extract(
WORD_MR mr1,
unsigned int offset,
guint8 *buf_pt,
unsigned int len
){
	unsigned int copied = 0;
	unsigned int n;

#ifdef DEBUG_ACT
	prnt_string("extract\r\n");
	prnt_int(mr1);prnt_char('-');
	prnt_int(offset);prnt_char('-');
	prnt_int(len);prnt_char(CR);
	print_mr(mr1);
#endif
	/* Skip over offset if greater than first mr1(s) */
	while((mr1 <NO_MEM_REC) && (offset >= mem_rec[mr1].size)){
		offset -= mem_rec[mr1].size;
		mr1 = mem_rec[mr1].rp;
	}
	while((mr1 < NO_MEM_REC) && (len != 0)){
		n = min(len,mem_rec[mr1].size - offset);  /* offset must be < bp->cnt */
		mem_cpy(buf_pt,&(mem_rec[mr1].parm[offset]),n);
		copied += n;
		buf_pt += n;
		len -= n;
		if((n + offset) == mem_rec[mr1].size)
			mr1 = mem_rec[mr1].rp;	/* Buffer exhausted, get next */
		offset = 0;		/* No more offset after first */
	}
	return copied;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Append new_mr1 to end of first_mr1 chain				    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void append(
WORD_MR *first_mr1,
WORD_MR new_mr1
){
	WORD_MR mr1;

#ifdef DEBUG_ACT
	prnt_string("append start:");
	prnt_int(*first_mr1);prnt_char(CR);prnt_char(LF);
	print_mr(new_mr1);
#endif
	if(new_mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2053);
		return;
	}
	mem_rec[new_mr1].rp=NULL_MEM_REC;
	mr1=*first_mr1;
	if(mr1 == NULL_MEM_REC){
		/* First one on chain */
		*first_mr1=new_mr1;
	}
	else{
		for(;mem_rec[mr1].rp<NO_MEM_REC;mr1=mem_rec[mr1].rp){}
		mem_rec[mr1].rp=new_mr1;
	}
#ifdef DEBUG_ACT
	prnt_string("append end:");
	prnt_int(*first_mr1);prnt_char(CR);prnt_char(LF);
	print_mr(new_mr1);
	print_mr(*first_mr1);
#endif
}
#endif
/* ======================================================================== */
/* Look up TCP connection						    */
/* Return TCB pointer or NULL if nonexistant.				    */
/* Also move the entry to the top of the list to speed future searches.     */
/* ======================================================================== */
#ifdef TCP_INSTALLED
unsigned int lookup_tcb(CONNECTION_STRUCT *conn){

	TCB_STRUCT *tcb_pt;
	unsigned int tcblast_no=NULL_TCB;
	unsigned int tcb_no;

#ifdef DEBUG_ACT
	prnt_string("lookup_tcb\r\n");
#endif
	for(tcb_no=Tcbs;tcb_no < MAX_TCB;tcblast_no = tcb_no,tcb_no=tcb_st[tcb_no].next){
		tcb_pt=&tcb_st[tcb_no];
		/* Yet another structure compatibility hack */
		if(conn->remote.port == tcb_pt->conn.remote.port
		 && conn->local.port == tcb_pt->conn.local.port
		 && conn->remote.address == tcb_pt->conn.remote.address
		 && conn->local.address == tcb_pt->conn.local.address){
			if(tcblast_no < MAX_TCB){
				/* Move to top of list */
				tcb_st[tcblast_no].next = tcb_pt->next;
				tcb_pt->next = Tcbs;
				Tcbs = tcb_no;
			}
			return tcb_no;
		}
	}
	return NULL_TCB;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Create a TCB, return pointer. Return pointer if TCB already exists.	    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
unsigned int create_tcb(CONNECTION_STRUCT *conn){

	unsigned int tcb_no;
	TCB_STRUCT *tcb_pt;
	TCP_RTT_STRUCT *tp_pt;

#if 0
	if((tcb_no = lookup_tcb(conn)) < MAX_TCB)
		return tcb_no;
#endif // 0
#ifdef DEBUG_ACT
	prnt_string("create_tcb\r\n");
#endif
	tcb_no=find_free_tcb_st();
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2080);
		return NULL_TCB;
	}
	tcb_pt=&tcb_st[tcb_no];
	tcb_pt->our_state=IN_USE;
//	ASSIGN(tcb_pt->conn,*conn);
	mem_cpy((guint8 *)&(tcb_pt->conn),(guint8 *)(conn),sizeof(CONNECTION_STRUCT));

	tcb_pt->state = TCP_CLOSED;
	tcb_pt->cwind = tcb_pt->mss = Tcp_mss;
	tcb_pt->ssthresh = 65535;
	if((tp_pt = rtt_get(tcb_pt->conn.remote.address)) != NULL){
		tcb_pt->srtt = tp_pt->srtt;
		tcb_pt->mdev = tp_pt->mdev;
	}
	else {
		tcb_pt->srtt = Tcp_irtt;   /* mdev = 0 */
	}
	/* Initialize timer intervals */
#if 0
	set_timer(&tcb_pt->timer,tcb_pt->srtt);
	tcb_pt->timer.func = tcp_timeout;
	tcb_pt->timer.arg = tcb;
#endif // 0
	tcb_pt->next_timer_count=change_to_kiatel_count(tcb_pt->srtt);
	tcb_pt->next = Tcbs;
	Tcbs = tcb_no;
	return tcb_no;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#if ((defined HI1_DISTRIBUTOR) && (defined SYSTEM_SIDE))
void restart_hi1_client(guint rec_no){
	SOCKET_STRUCT lsocket,fsocket;
	guint tcb_no;
	SUBSCRIBER_GROUP *sgp;
	int i;
	sgp=&subscriber_group[rec_no];
	lsocket.address=kt_system_st.ipv4_address;	// assume no hot_standby
	lsocket.port=3490;
	fsocket.address=find_ipv4_from_string(sgp->hi1_subs_ipv4_str);
	fsocket.port=ipport_hi1;  // assume using same port on other side
	tcb_no=start_active_tcp(&lsocket,&fsocket);
	if(tcb_no >= MAX_TCB){
	    mes2prnti(mess[HI1_ERR],0x3120);
	    sgp->hi1_subs_ipv4_str[0]=0;
	    sgp->tcb_no=NULL_TCB;
	    sgp->first_tcb=0;
	    return;
	}
	tcb_st[tcb_no].type=HI1_DISTRIBUTOR_TCP_TYPE;
	tcb_st[tcb_no].rec_no=rec_no;
	for(i=0,sgp=&subscriber_group[0];i<no_of_subscriber_groups;i++,sgp++){
	    if(i == rec_no) continue;
	    if(sgp->tcb_no == subscriber_group[rec_no].tcb_no){
		sgp->tcb_no=tcb_no;
	    }
	}
	subscriber_group[rec_no].tcb_no=tcb_no;
}
#endif
/* ======================================================================== */
/* Close our TCB */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void close_self(unsigned int tcb_no,int reason){
#ifdef HI1_DISTRIBUTOR
	guint saved_rec_no=(unsigned int)(-1);
#endif
#ifdef SYSCON_CLIENT
	guint saved_syscon_no=(unsigned int)(-1);
#endif
	RESEQ_STRUCT *rp1;
	RESEQ_STRUCT *rp;
	TCB_STRUCT *tcb_pt;
#ifdef DEBUG_ACT
	prnt_string("close_self:");
	prnt_int(tcb_no);prnt_char('-');
	prnt_int(reason);prnt_char(CR);
#endif
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2084);
		return;
	}
	tcb_pt=&tcb_st[tcb_no];
//	stop_timer(&tcb->timer);
//SABZSABZ : to be asked
	// our system is ftp client, server is turned on ,but its ftp server is closed.
	/* This is how the remote side refuses connect requests.*/
	if((reason == RESET_CLOSE_REASON) || (reason == TIMEOUT_CLOSE_REASON) || (reason == NETWORK_CLOSE_REASON)){
	  switch(tcb_pt->type){
#ifdef KFTPD_INSTALLED
	    case KFTPD_DATA_TCP_TYPE	      :
		if(tcb_pt->rec_no < MAX_KFTPD_REC){
		    to_aux_kftpd_q(tcb_pt->rec_no, AUX_KFTPD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:ftp server data tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
	    case KFTPD_CONTROL_TCP_TYPE 	      :
		if(tcb_pt->rec_no < MAX_KFTPD_REC){
		    to_kftpd_q(tcb_pt->rec_no, KFTPD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:ftp server control tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
#endif
#ifdef KFTP_INSTALLED
	    case KFTP_DATA_TCP_TYPE   :
		if(tcb_pt->rec_no < MAX_KFTP_REC){
		    to_aux_kftp_q(tcb_pt->rec_no, AUX_KFTP_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:ftp client data tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
	    case KFTP_CONTROL_TCP_TYPE		      :
		if(tcb_pt->rec_no < MAX_KFTP_REC){
		    to_kftp_q(tcb_pt->rec_no, KFTP_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:ftp client control tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
#endif
#ifdef ROS_FOR_LI_INSTALLED
	    case LI_ROS_TCP_TYPE    :
		if(tcb_pt->rec_no < MAX_LI_XPORT_REC){
		    to_li_xport_que(tcb_pt->rec_no, LI_XPORT_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:ros for li client tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
#endif
#ifdef TELNETD_INSTALLED
	    case TELNETD_TCP_TYPE	      :
		if(tcb_pt->rec_no < MAX_TELNETD_REC){
		    to_telnetd_q(tcb_pt->rec_no, TELNETD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:telnet server tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
#endif
#ifdef HI1_INSTALLED
	    case HI1_TCP_TYPE		  :
		if(tcb_pt->rec_no < MAX_HI1_REC){
		    to_hi1_q(tcb_pt->rec_no, HI1_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:hi1 server tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
#endif
#ifdef HI1_DISTRIBUTOR
	    case HI1_DISTRIBUTOR_TCP_TYPE	      :
		if(tcb_pt->rec_no < no_of_subscriber_groups){
		    saved_rec_no=tcb_pt->rec_no;
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:hi1 client tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
#endif
#ifdef SYSCON_SERVER
	    case SYSCON_SERVER_TCP_TYPE 	    :
		if(tcb_pt->rec_no < MAX_SYSCON_CLIENT){
		    guint syscon_client_no=tcb_pt->rec_no;
		    SYSCON_SERVER_STRUCT *sssp=&syscon_server_st[syscon_client_no];
		    sssp->tcp_no=NULL_TCB;
		    do_syscon_server_finish(syscon_client_no);
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:syscon server tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
#endif
#ifdef SYSCON_CLIENT
	    case SYSCON_CLIENT_TCP_TYPE 	    :
		if(tcb_pt->rec_no < MAX_SYSCON_SERVER){
		    saved_syscon_no=tcb_pt->rec_no;
		}
		tcb_pt->rec_no=(unsigned int)(-1);
		prnt_string("close_self:syscon client tcp failure:");prnt_int(tcb_no);prnt_char(CR);
		break;
#endif
	    default		      :
		break;
	  }
	}

	if(tcb_pt->timer_no < TIMER_NO ){
		stop_tcp_timer(tcb_no);
	}
	tcb_st[tcb_no].timer_no=NULL_TIMER;
	tcb_pt->reason = reason;

	/* Flush reassembly queue; nothing more can arrive */
	for(rp = tcb_pt->reseq;rp != NULL;rp = rp1){
		rp1 = rp->next;
		rls_reseq(rp);
//		free_p(&rp->bp);
//		free(rp);
	}
	tcb_pt->reseq = NULL;
	settcpstate(tcb_no,TCP_CLOSED);
#ifdef HI1_DISTRIBUTOR
	if(saved_rec_no != (unsigned int)(-1))
	    restart_hi1_client(saved_rec_no);
#endif
#ifdef SYSCON_CLIENT
	if(saved_syscon_no != (unsigned int)(-1))
	    restart_syscon_client(saved_syscon_no);
#endif
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Sequence number comparisons						    */
/* Return true if x is between low and high inclusive,			    */
/* false otherwise							    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int seq_within(long x,long low,long high){

	if(low <= high){
		if(low <= x && x <= high)
			return 1;
	} else {
		if(low >= x && x >= high)
			return 1;
	}
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int seq_lt(long x,long y){

	return (long)(x-y) < 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
#ifdef	notdef
int seq_le(long x,y){
	return (long)(x-y) <= 0;
}
#endif	/* notdef */
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int seq_gt(long x,long y){
	return (long)(x-y) > 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int seq_ge(long x,long y){
	return (long)(x-y) >= 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void settcpstate(unsigned int tcb_no,TCP_STATE_ENUM newstate){

	TCP_STATE_ENUM oldstate;
	TCB_STRUCT *tcb_pt;

#ifdef DEBUG_ACT
	prnt_string("settcpstate:");
	prnt_int(tcb_no);prnt_char('-');
	prnt_int(newstate);prnt_char(CR);
#endif
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2054);
		return;
	}
	tcb_pt=&tcb_st[tcb_no];
	oldstate = tcb_pt->state;
	tcb_pt->state = newstate;

	/* Update MIB variables */
	switch(oldstate){
	  case TCP_CLOSED:
		if(newstate == TCP_SYN_SENT)
			Tcp_mib[tcpActiveOpens].value++;
		break;
	  case TCP_LISTEN:
		if(newstate == TCP_SYN_RECEIVED)
			Tcp_mib[tcpPassiveOpens].value++;
		break;
	  case TCP_SYN_SENT:
		if(newstate == TCP_CLOSED)
			Tcp_mib[tcpAttemptFails].value++;
		break;
	  case TCP_SYN_RECEIVED:
		switch(newstate){
		  case TCP_CLOSED:
		  case TCP_LISTEN:
			Tcp_mib[tcpAttemptFails].value++;
			break;
		  default	:
			break;
		}
		break;
	  case TCP_ESTABLISHED:
	  case TCP_CLOSE_WAIT:
		switch(newstate){
		case TCP_CLOSED :
		case TCP_LISTEN :
			Tcp_mib[tcpEstabResets].value++;
			break;
		default 	:
			break;
		}
		Tcp_mib[tcpCurrEstab].value--;
		break;
	  default		  :
		break;
	}
	if((newstate == TCP_ESTABLISHED) || (newstate == TCP_CLOSE_WAIT)){
		Tcp_mib[tcpCurrEstab].value++;
	}
	switch(tcb_pt->rtn_type){
	  case TCP_INIT_RTNS_TYPE0	:
		i_upcall(tcb_no,oldstate,newstate);
		break;
	  case TCP_NORMAL_RTNS_TYPE1	:
		s_tscall(tcb_no,oldstate,newstate);
		break;
	  default			:
		prnt_msg(TCP_ERR,0x2058);
		break;
	}
	switch(newstate){
	  case TCP_SYN_RECEIVED 	:
	  case TCP_ESTABLISHED		:

		  /* Notify the user that he can begin sending data */
		  if(tcb_pt->rtn_type == TCP_NORMAL_RTNS_TYPE1 ){
			  s_ttcall(tcb_no,tcb_pt->window - tcb_pt->sndcnt);
			  switch(tcb_st[tcb_no].type){
#ifdef Q931_FOR_H225
			  case H225_Q931_TCP_TYPE	  :
				  break;
#endif
#ifdef H245_GATEWAY_SIDE
			  case H245_TCP_TYPE	  :
				  break;
#endif
#ifdef MY_TCPIP_FOR_OAM
			  case OAM_TCP_TYPE	  :
				  break;
#endif
#ifdef KFTPD_INSTALLED
			  case KFTPD_CONTROL_TCP_TYPE	  :
				  break;
			  case KFTPD_DATA_TCP_TYPE	  :
				  if(tcb_st[tcb_no].rec_no < MAX_KFTPD_REC){
					  to_aux_kftpd_q(tcb_st[tcb_no].rec_no, AUX_KFTPD_L2_TCP_ESTABLISHED_STIM, NULL_MEM_REC);
				  }
				  break;
#endif
#ifdef KFTP_INSTALLED
			  case KFTP_CONTROL_TCP_TYPE	  :
				  if((tcb_st[tcb_no].rec_no < MAX_KFTP_REC) && (newstate == TCP_ESTABLISHED)){
					  to_kftp_q(tcb_st[tcb_no].rec_no, KFTP_L2_TCP_ESTABLISHED_STIM, NULL_MEM_REC);
				  }
				  break;
			  case KFTP_DATA_TCP_TYPE	  :
				  if((tcb_st[tcb_no].rec_no < MAX_KFTP_REC) && (newstate == TCP_ESTABLISHED)){
					  to_aux_kftp_q(tcb_st[tcb_no].rec_no, AUX_KFTP_L2_TCP_ESTABLISHED_STIM, NULL_MEM_REC);
				  }
				  break;
#endif
#ifdef ROS_FOR_LI_INSTALLED
			  case LI_ROS_TCP_TYPE		:
				  if((tcb_st[tcb_no].rec_no < MAX_LI_XPORT_REC) && (newstate == TCP_ESTABLISHED)){
					  to_li_xport_que(tcb_st[tcb_no].rec_no, LI_XPORT_L2_TCP_ESTABLISHED_STIM, NULL_MEM_REC);
				  }
				  break;
#endif
#ifdef TELNETD_INSTALLED
			  case TELNETD_TCP_TYPE   :
/*
				  if((tcb_st[tcb_no].rec_no < MAX_TELNETD_REC) && (newstate == TCP_ESTABLISHED)){
					  to_telnetd_q(tcb_st[tcb_no].rec_no, TELNETD_L2_TCP_ESTABLISHED_STIM, NULL_MEM_REC);
				  }
*/
				  break;
#endif
#ifdef HI1_INSTALLED
			  case HI1_TCP_TYPE   :
/*
				  if((tcb_st[tcb_no].rec_no < MAX_HI1_REC) && (newstate == TCP_ESTABLISHED)){
					  to_hi1_q(tcb_st[tcb_no].rec_no, HI1_L2_TCP_ESTABLISHED_STIM, NULL_MEM_REC);
				  }
*/
				  break;
#endif
#ifdef HI1_DISTRIBUTOR
			  case HI1_DISTRIBUTOR_TCP_TYPE     :
				  break;
#endif
#ifdef SYSCON_SERVER
			  case SYSCON_SERVER_TCP_TYPE	:
				  break;
#endif
#ifdef SYSCON_CLIENT
			  case SYSCON_CLIENT_TCP_TYPE	:
				  break;
#endif

			  default			  :
				  prnt_msg(TCP_ERR,0x2093);
				  prnt_byte(tcb_st[tcb_no].type);prnt_char(CR);
				  break;
			  }
		  }
		  break;
	default 		      :
		break;
	}
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Round trip timing cache routines.					    */
/* These functions implement a very simple system for keeping track of	    */
/* network performance for future use in new connections.		    */
/* The emphasis here is on speed of update (rather than optimum cache hit   */
/* ratio) since rtt_add is called every time a TCP connection updates	    */
/* its round trip estimate.						    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void rtt_add(guint32 addr,guint32 rtt){ /* Destination IP address */

	TCP_RTT_STRUCT *tp_pt;
	long abserr;

	if(addr == 0){
		prnt_msg(TCP_ERR,0x2060);
		return;
	}
	tp_pt = &Tcp_rtt[(unsigned short)addr % RTTCACHE];
	if(tp_pt->addr != addr){
		/* New entry */
		tp_pt->addr = addr;
		tp_pt->srtt = rtt;
		tp_pt->mdev = 0;
	} else {
		/* Run our own SRTT and MDEV integrators, with rounding */
		abserr = (rtt > tp_pt->srtt) ? rtt - tp_pt->srtt : tp_pt->srtt - rtt;
		tp_pt->srtt = ((AGAIN-1)*tp_pt->srtt + rtt + (AGAIN/2)) >> LAGAIN;
		tp_pt->mdev = ((DGAIN-1)*tp_pt->mdev + abserr + (DGAIN/2)) >> LDGAIN;
	}
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
TCP_RTT_STRUCT *rtt_get(guint32 addr){

	TCP_RTT_STRUCT *tp_pt;

	if(addr == 0){
//		prnt_msg(TCP_ERR,0x2061);
		return NULL;
	}
	tp_pt = &Tcp_rtt[(unsigned short)addr % RTTCACHE];
	if(tp_pt->addr != addr){
//		prnt_msg(TCP_ERR,0x2062);
		return NULL;
	}
	return tp_pt;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* TCP garbage collection - called by storage allocator when free space     */
/* runs low. The send and receive queues are crunched. If the situation     */
/* is red, the resequencing queue is discarded; otherwise it is 	    */
/* also crunched.							    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void tcp_garbage(int red _U_){

#if 0
	TCB_STRUCT *tcb_pt;
	RESEQ_STRUCT *rp,*rp1;

	for(tcb = Tcbs;tcb != NULL;tcb = tcb->next){
		mbuf_crunch(&tcb->rcvq);
		mbuf_crunch(&tcb->sndq);
		for(rp = tcb->reseq;rp != NULL;rp = rp1){
			rp1 = rp->next;
			if(red){
				free_p(&rp->bp);
				free(rp);
			} else {
				mbuf_crunch(&rp->bp);
			}
		}
		if(red)
			tcb->reseq = NULL;
	}
#endif // 0
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Send an acceptable reset (RST) response for this segment		    */
/* The RST reply is composed in place on the input segment		    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void reset(
IP_STRUCT *ip,	/* Offending IP header */
TCP_STRUCT *seg /* Offending TCP header */
){
	WORD_MR mr1;
	unsigned int tmp;

//#ifdef DEBUG_ACT
	prnt_string("reset\r\n");
//#endif
	if(seg->rst_flag){
		prnt_msg(TCP_ERR,0x2063);
	return; /* Never send an RST in response to an RST */
	}
	/* Swap port numbers */
	tmp = seg->source;
	seg->source = seg->dest;
	seg->dest = tmp;

	if(seg->ack_flag){
		/* This reset is being sent to clear a half-open connection.*/
		/* Set the sequence number of the RST to the incoming ACK   */
		/* so it will be acceptable.				    */

		seg->ack_flag = 0;
		seg->seq = seg->ack;
		seg->ack = 0;
	} else {
		/* We're rejecting a connect request (SYN) from TCP_LISTEN state
		 * so we have to "acknowledge" their SYN.
		 */
		seg->ack_flag = 1;
		seg->ack = seg->seq;
		seg->seq = 0;
		if(seg->syn_flag)
			seg->ack++;
	}
	/* Set remaining parts of packet */
	seg->urg_flag = 0;
	seg->psh_flag = 0;
	seg->rst_flag = 1;
	seg->syn_flag = 0;
	seg->fin_flag = 0;
	seg->mss_flag = 0;
	seg->wscale_flag = 0;
	seg->tstamp_flag = 0;
	seg->wnd = 0;
	seg->up = 0;
	seg->checksum = 0;	/* force recomputation */
#if 0
	hbp = ambufw(NET_HDR_PAD);	/* Prealloc room for headers */
	hbp->data += NET_HDR_PAD;
	htontcp(seg,&hbp,ip->dest,ip->source);
#endif // 0
	mr1=NULL_MEM_REC;
	htontcp(seg,&mr1,ip->dest,ip->source);
	/* Ship it out (note swap of addresses) */
//	ip_send(ip->dest,ip->source,TCP_PTCL,ip->tos,0,&hbp,len_p(hbp),0,0);
	ip_send(ip->dest,ip->source,TCP_PTCL,ip->tos,0,mr1,0,0);
	Tcp_mib[tcpOutRsts].value++;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Process an incoming acknowledgement and window indication.		    */
/* From page 72.							    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void update(
unsigned int tcb_no,
TCP_STRUCT *seg,
unsigned int length
){
	gint32 acked,t;
	int winupd = 0;
	gint32 swind;	 /* Incoming window, scaled (non-SYN only) */
	long rtt;	/* measured round trip time */
	gint32 abserr;	 /* abs(rtt - srtt) */
	TCB_STRUCT *tcb_pt;

#ifdef DEBUG_ACT
	prnt_string("update\r\n");
#endif
	tcb_pt=&tcb_st[tcb_no];
	acked = 0;
	if(seq_gt(seg->ack,tcb_pt->snd_nxt)){
		tcb_pt->force_flag = 1;   /* Acks something not yet sent */
		return;
	}
	/* Decide if we need to do a window update.			*/
	/* This is always checked whenever a legal ACK is received,	*/
	/* even if it doesn't actually acknowledge anything,            */
	/* because it might be a spontaneous window reopening.		*/

	if((seq_gt(seg->seq,tcb_pt->snd_wl1)) ||
	   ((seg->seq == tcb_pt->snd_wl1) && seq_ge(seg->ack,tcb_pt->snd_wl2))
	){
		if((seg->syn_flag) || (!tcb_pt->ws_ok_flag)){
			swind = seg->wnd;
		}
		else{
			swind = seg->wnd << tcb_pt->snd_wind_scale;
		}
		if(swind > tcb_pt->snd_wnd){
			winupd = 1;	/* Don't count as duplicate ack */

			/* If the window had been closed, crank back the send */
			/* pointer so we'll immediately resume transmission.  */
			/* Otherwise we'd have to wait until the next probe.  */

			if(tcb_pt->snd_wnd == 0)
				tcb_pt->snd_ptr = tcb_pt->snd_una;
		}
		/* Remember for next time */
		tcb_pt->snd_wnd = swind;
		tcb_pt->snd_wl1 = seg->seq;
		tcb_pt->snd_wl2 = seg->ack;
	}
	/* See if anything new is being acknowledged */
	if(seq_lt(seg->ack,tcb_pt->snd_una))
		return; /* Old ack, ignore */

	if(seg->ack == tcb_pt->snd_una){
		/* Ack current, but doesn't ack anything */
		if((tcb_pt->sndcnt == 0) || (winupd) || (length != 0) ||
		   (seg->syn_flag) || (seg->fin_flag)
		){
			/* Either we have nothing in the pipe, this segment */
			/* was sent to update the window, or it carries     */
			/* data/syn/fin. In any of these cases we	    */
			/* wouldn't necessarily expect an ACK.              */
			return;
		}
		/* Van Jacobson "fast recovery" code */
		if(++tcb_pt->dupacks == TCPDUPACKS){
			/* We've had a burst of do-nothing acks, so     */
			/* we almost certainly lost a packet.		*/
			/* Resend it now to avoid a timeout. (This is	*/
			/* Van Jacobson's 'quick recovery' algorithm.)  */

			long ptrsave;

			/* Knock the threshold down just as though	*/
			/* this were a timeout, since we've had         */
			/* network congestion.				*/

			tcb_pt->ssthresh = tcb_pt->cwind/2;
			tcb_pt->ssthresh = max(tcb_pt->ssthresh,tcb_pt->mss);

			/* Manipulate the machinery in tcp_output() to	*/
			/* retransmit just the missing packet		*/

			ptrsave = tcb_pt->snd_ptr;
			tcb_pt->snd_ptr = tcb_pt->snd_una;
			tcb_pt->cwind = tcb_pt->mss;
			tcp_output(tcb_no);
			tcb_pt->snd_ptr = ptrsave;

			/* "Inflate" the congestion window, pretending as */
			/* though the duplicate acks were normally acking */
			/* the packets beyond the one that was lost.	  */

			tcb_pt->cwind = tcb_pt->ssthresh + TCPDUPACKS*tcb_pt->mss;
		}
		else if(tcb_pt->dupacks > TCPDUPACKS){
			/* Continue to inflate the congestion window	*/
			/* until the acks finally get "unstuck".        */

			tcb_pt->cwind += tcb_pt->mss;
		}
		/* Clamp the congestion window at the amount currently	*/
		/* on the send queue, with a minimum of one packet.	*/
		/* This keeps us from increasing the cwind beyond what	*/
		/* we're actually putting in the pipe; otherwise a big  */
		/* burst of data could overwhelm the net.		*/

		tcb_pt->cwind = min(tcb_pt->cwind,tcb_pt->sndcnt);
		tcb_pt->cwind = max(tcb_pt->cwind,tcb_pt->mss);
		return;
	}
#ifdef DEBUG_ACT
	prnt_string("We're here, so the ACK must have actually acked something\r\n");
#endif
	/* We're here, so the ACK must have actually acked something */
	if((tcb_pt->dupacks >= TCPDUPACKS) && (tcb_pt->cwind > tcb_pt->ssthresh)){

		/* The acks have finally gotten "unstuck". So now we */
		/* can "deflate" the congestion window, i.e. take it */
		/* back down to where it would be after slow start   */
		/* finishes.					     */

		tcb_pt->cwind = tcb_pt->ssthresh;
	}
	tcb_pt->dupacks = 0;
	acked = seg->ack - tcb_pt->snd_una;

	/* Expand congestion window if not already at limit and if	*/
	/* this packet wasn't retransmitted                             */

	if((tcb_pt->cwind < tcb_pt->snd_wnd) && (!tcb_pt->retran_flag)){
		if(tcb_pt->cwind < tcb_pt->ssthresh){
			/* Still doing slow start/CUTE, expand by amount acked */
			tcb_pt->cwind += min(acked,tcb_pt->mss);
		} else {
			/* Steady-state test of extra path capacity */
			tcb_pt->cwind += ((long)tcb_pt->mss * tcb_pt->mss) / tcb_pt->cwind;
		}
		/* Don't expand beyond the offered window */
		if(tcb_pt->cwind > tcb_pt->snd_wnd)
			tcb_pt->cwind = tcb_pt->snd_wnd;
	}
	tcb_pt->cwind = min(tcb_pt->cwind,tcb_pt->sndcnt);	 /* Clamp */
	tcb_pt->cwind = max(tcb_pt->cwind,tcb_pt->mss);

	/* Round trip time estimation */
	rtt = -1;	/* Init to invalid value */
	if((tcb_pt->ts_ok_flag) && (seg->tstamp_flag)){
		/* Determine RTT from timestamp echo */
		rtt = msclock() - seg->tsecr;
	}
	else if((tcb_pt->rtt_run_flag) && seq_ge(seg->ack,tcb_pt->rttseq)){

		/* use standard round trip timing */
		/* A timed sequence number has been acked */
		tcb_pt->rtt_run_flag = 0;
		if(!(tcb_pt->retran_flag)){

			/* This packet was sent only once and now	   */
			/* it's been acked, so process the round trip time */

			rtt = msclock() - tcb_pt->rtt_time;
		}
	}
	if(rtt >= 0){
		tcb_pt->rtt = rtt; /* Save for display */

		abserr = (rtt > tcb_pt->srtt) ? rtt - tcb_pt->srtt : tcb_pt->srtt - rtt;
		/* Run SRTT and MDEV integrators, with rounding */
		tcb_pt->srtt = ((AGAIN-1)*tcb_pt->srtt + rtt + (AGAIN/2)) >> LAGAIN;
		tcb_pt->mdev = ((DGAIN-1)*tcb_pt->mdev + abserr + (DGAIN/2)) >> LDGAIN;

		rtt_add(tcb_pt->conn.remote.address,rtt);
		/* Reset the backoff level */
		tcb_pt->backoff = 0;

		/* Update our tx throughput estimate */
		t = msclock();
		tcb_pt->outlen = (7*tcb_pt->outlen + seg->ack - tcb_pt->rttack)/8;
		tcb_pt->outrate = (7*tcb_pt->outrate + t - tcb_pt->lastack)/8;
		tcb_pt->lastack = t;
	}
	tcb_pt->sndcnt -= acked;   /* Update virtual byte count on snd queue */
	tcb_pt->snd_una = seg->ack;

	/* If we're waiting for an ack of our SYN, note it and adjust count */
	if(!(tcb_pt->synack_flag)){
		tcb_pt->synack_flag = 1;
		acked--;	/* One less byte to pull from real snd queue */
	}
	/* Remove acknowledged bytes from the send queue and update the */
	/* unacknowledged pointer. If a FIN is being acked,		*/
	/* pullup won't be able to remove it from the queue, but that   */
	/* causes no harm.						*/

//	pullup(&tcb_pt->sndq,NULL,(unsigned int)acked);
	my_pullup_linklist_only(&tcb_pt->sndq,(unsigned int)acked);

	/* Stop retransmission timer, but restart it if there is still
	 * unacknowledged data.
	 */
//	stop_timer(&tcb_pt->timer);
//	stoptimer(tcb_no,TCP_SDL_T);
	stop_tcp_timer(tcb_no);
	tcb_st[tcb_no].timer_no=NULL_TIMER;
	if(tcb_pt->snd_una != tcb_pt->snd_nxt){
//		start_timer(&tcb_pt->timer);
		start_tcp_timer(tcb_no,tcb_pt->next_timer_count);
	}

	/* If retransmissions have been occurring, make sure the	*/
	/* send pointer doesn't repeat ancient history                  */

	if(seq_lt(tcb_pt->snd_ptr,tcb_pt->snd_una))
		tcb_pt->snd_ptr = tcb_pt->snd_una;

	/* Clear the retransmission flag since the oldest			*/
	/* unacknowledged segment (the only one that is ever retransmitted)	*/
	/* has now been acked.							*/

	tcb_pt->retran_flag = 0;

	/* If outgoing data was acked, notify the user so he can send more	*/
	/* unless we've already sent a FIN.                                     */

#if 0
	if( (acked != 0) && (tcb_pt->t_upcall) &&
	    ((tcb_pt->state == TCP_ESTABLISHED) || (tcb_pt->state == TCP_CLOSE_WAIT))
	){
		(*tcb_pt->t_upcall)(tcb_no,tcb_pt->window - tcb_pt->sndcnt);
	}
#endif // 0
	if( (acked != 0)  &&
	    ((tcb_pt->state == TCP_ESTABLISHED) || (tcb_pt->state == TCP_CLOSE_WAIT))
	){
		s_ttcall(tcb_no,tcb_pt->window - tcb_pt->sndcnt);
	}
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Determine if the given sequence number is in our receiver window.	    */
/* NB: must not be used when window is closed!				    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int in_window(
unsigned int tcb_no,
gint32 seq
){
	TCB_STRUCT *tcb_pt;

	tcb_pt=&tcb_st[tcb_no];
	return seq_within(seq,tcb_pt->rcv_nxt,(long)(tcb_pt->rcv_nxt+tcb_pt->rcv_wnd-1));
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Process an incoming SYN						    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void proc_syn(
unsigned int tcb_no,
guint8 tos,
TCP_STRUCT *seg
){
	TCB_STRUCT *tcb_pt;
	unsigned int mtu;
	TCP_RTT_STRUCT *tp;

#ifdef DEBUG_ACT
	prnt_string("proc_syn\r\n");
#endif
	tcb_pt=&tcb_st[tcb_no];
	tcb_pt->force_flag = 1;   /* Always send a response */

	/* Note: It's not specified in RFC 793, but SND.WL1 and         */
	/* SND.WND are initialized here since it's possible for the     */
	/* window update routine in update() to fail depending on the	*/
	/* IRS if they are left unitialized.				*/

	/* Check incoming precedence and increase if higher */
	if(PREC(tos) > PREC(tcb_pt->tos))
		tcb_pt->tos = tos;
	tcb_pt->rcv_nxt = seg->seq + 1;    /* p 68 */
	tcb_pt->snd_wl1 = tcb_pt->irs = seg->seq;
	tcb_pt->snd_wnd = seg->wnd;	   /* Never scaled in a SYN */
	if(seg->mss_flag)
		tcb_pt->mss = seg->mss;
	if(seg->wscale_flag){
		tcb_pt->snd_wind_scale = seg->wsopt;
		tcb_pt->rcv_wind_scale = DEF_WSCALE;
		tcb_pt->ws_ok_flag = 1;
	}
	if((seg->tstamp_flag) && (Tcp_tstamps)){
		tcb_pt->ts_ok_flag = 1;
		tcb_pt->ts_recent = seg->tsval;
	}
	/* Check the MTU of the interface we'll use to reach this guy       */
	/* and lower the MSS so that unnecessary fragmentation won't occur  */

	if((mtu = ip_mtu(tcb_pt->conn.remote.address)) != 0){
		/* Allow space for the TCP and IP headers */
		if(tcb_pt->ts_ok_flag)
			mtu -= (TSTAMP_LENGTH + TCPLEN + IPLEN + 3) & ~3;
		else
			mtu -= TCPLEN + IPLEN;
		tcb_pt->cwind = tcb_pt->mss = min(mtu,tcb_pt->mss);
	}
	/* See if there's round-trip time experience */
	if((tp = rtt_get(tcb_pt->conn.remote.address)) != NULL){
		tcb_pt->srtt = tp->srtt;
		tcb_pt->mdev = tp->mdev;
	}
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Add an entry to the resequencing queue in the proper place		    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void add_reseq(
unsigned int tcb_no,
guint8 tos,
TCP_STRUCT *seg,
WORD_MR *mr3,
guint length
){
	RESEQ_STRUCT *rp,*rp1;
	TCB_STRUCT *tcb_pt;


	/* Allocate reassembly descriptor */
#if 0
	if((rp = (struct reseq *)malloc(sizeof (struct reseq))) == NULL){
		/* No space, toss on floor */
		free_p(bpp);
		return;
	}
	ASSIGN(rp->seg,*seg);
	rp->bp = (*bpp);
	*bpp = NULL;
#endif // 0
	rp=find_free_reseq();
	if(rp == NULL){
		prnt_msg(TCP_ERR,0x2064);
		if(*mr3 < NO_MEM_REC)
			mem_rec_rls(*mr3);
		return;
	}
	mem_cpy((guint8 *)&(rp->seg),(guint8 *)seg,sizeof(TCP_STRUCT));
	rp->tos = tos;
	rp->mr1=*mr3; *mr3=NULL_MEM_REC;
	rp->length = length;

	tcb_pt=&tcb_st[tcb_no];
	/* Place on reassembly list sorting by starting seq number */
	rp1 = tcb_pt->reseq;
	if((rp1 == NULL) || seq_lt(seg->seq,rp1->seg.seq)){
		/* Either the list is empty, or we're less than all other       */
		/* entries; insert at beginning.				*/

		rp->next = rp1;
		tcb_pt->reseq = rp;
	} else {
		/* Find the last entry less than us */
		for(;;){
			if((rp1->next == NULL) || seq_lt(seg->seq,rp1->next->seg.seq)){
				/* We belong just after this one */
				rp->next = rp1->next;
				rp1->next = rp;
				break;
			}
			rp1 = rp1->next;
		}
	}
#ifdef DEBUG_ACT
	prnt_string("add_reseq:");prnt_long((unsigned long)(tcb_pt->reseq));prnt_char(CR);
	prnt_string("seq.seq=");prnt_long(seg->seq);prnt_char(CR);
#endif
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Fetch the first entry off the resequencing queue			    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void get_reseq(
unsigned int tcb_no,
guint8 *tos,
TCP_STRUCT *seg,
WORD_MR *mr1,
guint *length
){
	RESEQ_STRUCT *rp;
	TCB_STRUCT *tcb_pt;

	tcb_pt=&tcb_st[tcb_no];
	if((rp = tcb_pt->reseq) == NULL){
		prnt_msg(TCP_ERR,0x2064);
		return;
	}
	tcb_pt->reseq = rp->next;

	*tos = rp->tos;
//	ASSIGN(*seg,rp->seg);
	mem_cpy((guint8 *)seg,(guint8 *)&(rp->seg),sizeof(TCP_STRUCT));
//	*bp = rp->bp;
	*mr1= rp->mr1;
	*length = rp->length;
//	free(rp);
#ifdef DEBUG_ACT
	prnt_string("get_reseq:");prnt_long((unsigned long)(tcb_pt->reseq));prnt_char(CR);
	prnt_string("seq.seq=");prnt_long(seg->seq);prnt_char(CR);
#endif
	rls_reseq(rp);
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Trim segment to fit window. Return 0 if OK, -1 if segment is 	    */
/* unacceptable.							    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int trim(
unsigned int tcb_no,
TCP_STRUCT *seg,
WORD_MR *mr3,
guint *length
){

	TCB_STRUCT *tcb_pt;
	long dupcnt,excess;
	unsigned int len;	/* Segment length including flags */
	char accept = 0;

#ifdef DEBUG_ACT
	prnt_string("trim:");prnt_int(tcb_no);prnt_char(CR);
#endif
	tcb_pt=&tcb_st[tcb_no];
	len = *length;
	if(seg->syn_flag)
		len++;
	if(seg->fin_flag)
		len++;

	/* Segment acceptability tests */
	if(tcb_pt->rcv_wnd == 0){
		/* If our window is closed, then the other end is	*/
		/* probably probing us. If so, they might send us acks	*/
		/* with seg.seq > rcv.nxt. Be sure to accept these	*/

		if((len == 0) && seq_within(seg->seq,tcb_pt->rcv_nxt,tcb_pt->rcv_nxt+tcb_pt->window))
			return 0;
		return -1;	/* reject all others */
	}
	if(tcb_pt->rcv_wnd > 0){
		/* Some part of the segment must be in the window */
		if(in_window(tcb_no,seg->seq)){
			accept++;	/* Beginning is */
		} else if(len != 0){
			if(in_window(tcb_no,(long)(seg->seq+len-1)) || /* End is */
			 seq_within(tcb_pt->rcv_nxt,seg->seq,(long)(seg->seq+len-1))){ /* Straddles */
				accept++;
			}
		}
	}
	if(!accept){
		tcb_pt->rerecv += len;
		if(*mr3 < NO_MEM_REC)
			mem_rec_rls(*mr3);
		return -1;
	}
	if((dupcnt = tcb_pt->rcv_nxt - seg->seq) > 0){
		tcb_pt->rerecv += dupcnt;
		/* Trim off SYN if present */
		if(seg->syn_flag){
			/* SYN is before first data byte */
			seg->syn_flag = 0;
			seg->seq++;
			dupcnt--;
		}
		if(dupcnt > 0){
//			pullup(bpp,NULL,(unsigned int)dupcnt);
//JASHNJASHN start : to be asked
			if(*mr3 >= NO_MEM_REC){
				prnt_msg(TCP_ERR,0x2097);
				return -1;
			}
//JASHNJASHN end
			*mr3=my_pullup_only(*mr3,(unsigned int)dupcnt);
			seg->seq += dupcnt;
			*length -= dupcnt;
		}
	}
	if((excess = seg->seq + *length - (tcb_pt->rcv_nxt + tcb_pt->rcv_wnd)) > 0){
		tcb_pt->rerecv += excess;
		/* Trim right edge */
		*length -= excess;
//		trim_mbuf(bpp,*length);
		trim_mbuf(mr3,*length);
		seg->fin_flag = 0;     /* FIN follows last data byte */
	}
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
RESEQ_STRUCT *find_free_reseq(void){

	WORD_MR mr1;
	RESEQ_STRUCT *rp;

	mr1=mem_rec_req(sizeof(RESEQ_STRUCT));
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2066);
		return NULL;
	}
	rp=(RESEQ_STRUCT *)mem_rec[mr1].parm;
	rp->my_mr1=mr1;
	return rp;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void rls_reseq(RESEQ_STRUCT *rp){

	if(rp == NULL){
		prnt_msg(TCP_ERR,0x2067);
		return;
	}
	if(rp->my_mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2068);
		return;
	}
	mem_rec_rls(rp->my_mr1);
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Trim mbuf to specified length by lopping off end */
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
void trim_mbuf(WORD_MR *mr3,guint length){

	guint tot = 0;
	WORD_MR mr1,mr2,new_mr1,old_mr1;
	guint size;

#ifdef DEBUG_ACT
	prnt_string("trim_mbuf:");
	print_mr(*mr3);prnt_char(CR);
#endif
	mr1=*mr3;
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x2069);
		return; /* Nothing to trim */
	}
	if(length == 0){
		/* Toss the whole thing */
		prnt_msg(IP_ERR,0x2070);
//		mem_rec_rls(mr1);
		return;
	}
	/* Find the point at which to trim. If length is greater than	*/
	/* the packet, we'll just fall through without doing anything   */

	old_mr1=NULL_MEM_REC;
	for(;mr1<NO_MEM_REC;old_mr1=mr1,mr1=mem_rec[mr1].rp){
		if((tot + mem_rec[mr1].size) < length){
			tot += mem_rec[mr1].size;
		}
		else{
			/* Cut here */
			size = length - tot;
			mr2=mem_rec_req(size);
			if(mr2 >= NO_MEM_REC){
				prnt_msg(IP_ERR,0x2081);
				return;
			}
			mem_cpy(mem_rec[mr2].parm,mem_rec[mr1].parm,size);
			mem_rec[mr2].rp=NULL_MEM_REC;
			if(old_mr1 < NO_MEM_REC)
				mem_rec[old_mr1].rp=mr2;
			if(mr1 == *mr3) *mr3=mr2;
			while(mr1 < NO_MEM_REC){
				new_mr1=mem_rec[mr1].rp;
				mem_rec_rls(mr1);
				mr1=new_mr1;
			}
			return;
		}
	}
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
int checkipaddr(SOCKADDR_STRUCT *name,int namelen){

	SOCKADDR_IN_STRUCT *sock;

	sock = (SOCKADDR_IN_STRUCT *)name;
	if((sock->sin_family != AF_INET) || (namelen != sizeof(SOCKADDR_IN_STRUCT)))
		return -1;
	return 0;
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ======================================================================== */
/* Return time since startup in milliseconds. Resolution is improved	    */
/* below 55 ms (the clock tick interval) by reading back the instantaneous  */
/* 8254 counter value and combining it with the global clock tick counter.  */
/*									    */
/* Reading the 8254 is a bit tricky since a tick could occur asynchronously */
/* between the two reads. The tick counter is examined before and after the */
/* hardware counter is read. If the tick counter changes, try again.	    */
/* Note: the hardware counter counts down from 65536.			    */
/* ======================================================================== */
//SHARAB start : do exist in ip.c
#if 0
#ifdef TCP_INSTALLED
long msclock(void){
#if 0
	int32 hi;
	uint lo;
	uint64 x;

	do {
		hi = rdclock();
		lo = clockbits();
	} while(hi != rdclock());

	x = ((uint64)hi << 16) - lo;
	return (x * 11) / 13125;
#endif // 0
	return(msclock_cnt++);
}
#endif // TCP_INSTALLED
#endif // 0
//SHARAB end
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void init_tcb_st(void){

	unsigned int i;
	TCB_STRUCT *tcb_pt;

	for(i=0,tcb_pt=&tcb_st[0];i < MAX_TCB;i++,tcb_pt++){
		tcb_pt->our_state=IN_QUE;
		tcb_pt->state=TCP_CLOSED;
//		tcb_pt->conn;
//		tcb_pt->reason; 	/* Reason for closing */
//		tcb_pt->type;
//		tcb_pt->code;
		tcb_pt->snd_una=0;	/* First unacknowledged sequence number */
		tcb_pt->snd_nxt=0;	/* Next sequence num to be sent for the first time */
		tcb_pt->snd_ptr=0;	/* Working transmission pointer */
		tcb_pt->snd_wl1=0;	/* Sequence number used for last window update */
		tcb_pt->snd_wl2=0;	/* Ack number used for last window update */
		tcb_pt->snd_wnd=0;	/* Other end's offered receive window */
		tcb_pt->snd_up=0;	/* Send urgent pointer */
		tcb_pt->snd_wind_scale=0;/* Send window scale */

		tcb_pt->iss=0;		/* Initial send sequence number */
		tcb_pt->resent=0;	/* Count of bytes retransmitted */
		tcb_pt->cwind=0;	/* Congestion window */
		tcb_pt->ssthresh=0;	/* Slow-start threshold */
		tcb_pt->dupacks=0;	/* Count of duplicate (do-nothing) ACKs */

		tcb_pt->rcv_nxt=0;	/* Incoming sequence number expected next */
		tcb_pt->rcv_wnd=0;	/* Our offered receive window */
		tcb_pt->rcv_up=0;	/* Receive urgent pointer */
		tcb_pt->rcv_wind_scale=0;   /* Recv window scale */

		tcb_pt->last_ack_sent=0;    /* Last ack sent for timestamp purposes */
		tcb_pt->ts_recent=0;	    /* Most recent incoming timestamp */

		tcb_pt->irs=0;		    /* Initial receive sequence number */
		tcb_pt->rerecv=0;	    /* Count of duplicate bytes received */
		tcb_pt->mss=0;		    /* Maximum segment size */

		tcb_pt->window=0;	    /* Receiver window and send queue limit */
		tcb_pt->limit=0;	    /* Send queue limit */
		tcb_pt->rtn_type=TCP_INIT_RTNS_TYPE0;
		tcb_pt->force_flag=0;	/* We owe the other end an ACK or window update */
		tcb_pt->clone_flag=0;	/* Server-type TCB, cloned on incoming SYN */
		tcb_pt->retran_flag=0;	/* A retransmission has occurred */
		tcb_pt->active_flag=0;	/* TCB created with an active open */
		tcb_pt->synack_flag=0;	/* Our SYN has been acked */
		tcb_pt->rtt_run_flag=0; /* We're timing a segment */
		tcb_pt->congest_flag=0; /* Copy of last IP congest bit received */
		tcb_pt->ts_ok_flag=0;	/* We're using timestamps */
		tcb_pt->ws_ok_flag=0;	/* We're using window scaling */
		tcb_pt->my_reset_flag=0;
		tcb_pt->my_send_rst_flag=0;
//		tcb_pt->tos;		/* Type of service (for IP) */
		tcb_pt->backoff=0;	/* Backoff interval */

		tcb_pt->rcvq=NULL_MEM_REC;	/* Receive queue */
		tcb_pt->sndq=NULL_MEM_REC;	/* Send queue */
		tcb_pt->rcvcnt=0;		/* Count of items on rcvq */
		tcb_pt->sndcnt=0;		/* Number of unacknowledged sequence numbers on */
						/* sndq. NB: includes SYN and FIN, which don't  */
						/* actually appear on sndq!			*/

		tcb_pt->reseq=NULL;	   /* Out-of-order segment queue */
		tcb_pt->timer_no=NULL_TIMER;  /* Retransmission timer */
		tcb_pt->rtt_time=0;	   /* Stored clock values for RTT */
		tcb_pt->rttseq=0;	   /* Sequence number being timed */
		tcb_pt->rttack=0;	   /* Ack at start of timing (for txbw calc) */
		tcb_pt->srtt=0; 	   /* Smoothed round trip time, milliseconds */
		tcb_pt->mdev=0; 	   /* Mean deviation, milliseconds */
		tcb_pt->rtt=0;		   /* Last received RTT (for debugging) */

		tcb_pt->user=NULL_SOCKET;  /* User parameter (e.g., for mapping to an */
					   /* application control block 	      */

		tcb_pt->quench=0;	   /* Count of incoming ICMP source quenches */
		tcb_pt->unreach=0;	   /* Count of incoming ICMP unreachables */
		tcb_pt->timeouts=0;	   /* Count of retransmission timeouts */
		tcb_pt->lastack=0;	   /* Time of last received ack */
		tcb_pt->outlen=0;	   /* Average tx ack amount */
		tcb_pt->outrate=0;	   /* Average tx ack interval, ms */
		tcb_pt->lastrx=0;	   /* Time of last received data */
		tcb_pt->inlen=0;	   /* Average receive data size */
		tcb_pt->inrate=0;	   /* Average receive packet interval,ms */
		tcb_pt->trdiscard_flag=RESET;
		tcb_pt->type=NULL_TCP_TYPE;
#if ((defined H245_GATEWAY_SIDE) || (defined MY_TCPIP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT))
		tcb_pt->rec_no=(guint)(-1);
#endif
		tcb_pt->next=i+1;
		tcb_pt->prev=i-1;
	}
	fq_tcb_st=0;
	eq_tcb_st=MAX_TCB-1;
	tcb_st[MAX_TCB-1].next=NULL_TCB;
	tcb_st[0].prev=NULL_TCB;
	return;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
unsigned int find_free_tcb_st(void){

	unsigned int tcb_no;
	TCB_STRUCT *tcb_pt;
#ifdef DEBUG_ACT
	prnt_string("find_free_tcb_st:\r\n");
#endif
	if(fq_tcb_st >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2071);
		return NULL_TCB;
	}
	tcb_no=fq_tcb_st;
#ifdef DEBUG_ACT
	prnt_int(tcb_no);prnt_char(CR);
#endif
	tcb_pt=&tcb_st[tcb_no];
	fq_tcb_st=tcb_pt->next;
	if(fq_tcb_st < MAX_TCB)
		tcb_st[fq_tcb_st].prev=NULL_TCB;
	else
		eq_tcb_st=NULL_TCB;
	tcb_pt->next=NULL_TCB;
	tcb_pt->our_state=OUT_OF_QUE;
	return tcb_no;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void rls_tcb_st(unsigned int tcb_no){

	TCB_STRUCT *tcb_pt;
#ifdef DEBUG_ACT
	prnt_string("rls_tcb_st:");prnt_int(tcb_no);prnt_char(CR);
#endif
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2072);
		prnt_int(tcb_no);prnt_char(CR);
		return;
	}
	tcb_pt=&tcb_st[tcb_no];
	if(tcb_pt->our_state == IN_QUE){
		prnt_msg(TCP_ERR,0x2073);
		prnt_int(tcb_no);prnt_char(CR);
		return;
	}
	switch(tcb_pt->type){
#ifdef Q931_FOR_H225
	  case	H225_Q931_TCP_TYPE	:
		break;
#endif
#ifdef H245_GATEWAY_SIDE
	  case	H245_TCP_TYPE		:
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no < NO_H245_REC){
				h245_rec[rec_no].tcb_no=NULL_TCB;
			}
		}
		break;
#endif
#ifdef MY_TCPIP_FOR_OAM
	  case	OAM_TCP_TYPE		:
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no != (guint)(-1)){
#ifdef SYSTEM_SIDE
				oam[rec_no].link_no=(unsigned int)(-1);
				start_oam_tcp_passive_port(rec_no);
#endif
#ifdef OPERATOR_SIDE
				if(rec_no == 0) tcb0=(unsigned int)(-1);
				if(rec_no == 1) tcb1=(unsigned int)(-1);
				start_oam_tcp_active_port(rec_no);
#endif
			}
		}
		break;
#endif
#ifdef HI1_DISTRIBUTOR
	  case	HI1_DISTRIBUTOR_TCP_TYPE    :
		break;
#endif
#ifdef SYSCON_CLIENT
	  case	SYSCON_CLIENT_TCP_TYPE	  :
		break;
#endif
// it must!!! not be done here
#if 0
#ifdef KFTPD_INSTALLED
	case KFTPD_CONTROL_TCP_TYPE		  :
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no < MAX_KFTPD_REC){
				kftpd_rec[rec_no].control_tcp_no=NULL_TCB;
			}
		}
		break;
	case KFTPD_DATA_TCP_TYPE		  :
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no < MAX_KFTPD_REC){
				kftpd_rec[rec_no].data_tcp_no=NULL_TCB;
			}
		}
		break;
#endif
#endif // 0
// it must!!! not be done here, normally this routine is called after rec_no is released,
// it is possible that rec_no is involved in another call and this routine destroys its
// field to tcp_no
#if 0
#ifdef KFTP_INSTALLED
	case KFTP_CONTROL_TCP_TYPE		  :
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no < MAX_KFTP_REC){
				kftp_rec[rec_no].control_tcp_no=NULL_TCB;
			}
		}
		break;
	case KFTP_DATA_TCP_TYPE 	  :
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no < MAX_KFTP_REC){
				kftp_rec[rec_no].data_tcp_no=NULL_TCB;
			}
		}
		break;
#endif
#ifdef ROS_FOR_LI_INSTALLED
	case LI_ROS_TCP_TYPE	:
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no < MAX_KFTP_REC){
				li_xport_rec[rec_no].xfer_no=NULL_TCB;
			}
		}
		break;
#endif
#ifdef TELNETD_INSTALLED
	case TELNETD_TCP_TYPE		  :
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no < MAX_TELNETD_REC){
				telnetd_rec[rec_no].tcp_no=NULL_TCB;
			}
		}
		break;
#endif
#ifdef HI1_INSTALLED
	case HI1_TCP_TYPE	      :
		{
			guint rec_no;
			rec_no=tcb_pt->rec_no;
			if(rec_no < MAX_HI1_REC){
				hi1_rec[rec_no].tcp_no=NULL_TCB;
			}
		}
		break;
#endif
#ifdef SYSCON_SERVER
	  case	SYSCON_SERVER_TCP_TYPE	  :
		break;
#endif
#endif // 0
	  default			:
#if 0
		prnt_msg(TCP_ERR,0x2082);
		prnt_int(tcb_no);prnt_char('-');prnt_int(tcb_pt->rec_no);prnt_char(CR);
#endif // 0
		break;
	}
	tcb_pt->our_state=IN_QUE;
	tcb_pt->state=TCP_CLOSED;
//	tcb_pt->conn;
//	tcb_pt->reason; 	/* Reason for closing */
//	tcb_pt->type;
//	tcb_pt->code;
	tcb_pt->snd_una=0;	/* First unacknowledged sequence number */
	tcb_pt->snd_nxt=0;	/* Next sequence num to be sent for the first time */
	tcb_pt->snd_ptr=0;	/* Working transmission pointer */
	tcb_pt->snd_wl1=0;	/* Sequence number used for last window update */
	tcb_pt->snd_wl2=0;	/* Ack number used for last window update */
	tcb_pt->snd_wnd=0;	/* Other end's offered receive window */
	tcb_pt->snd_up=0;	/* Send urgent pointer */
	tcb_pt->snd_wind_scale=0;/* Send window scale */

	tcb_pt->iss=0;		/* Initial send sequence number */
	tcb_pt->resent=0;	/* Count of bytes retransmitted */
	tcb_pt->cwind=0;	/* Congestion window */
	tcb_pt->ssthresh=0;	/* Slow-start threshold */
	tcb_pt->dupacks=0;	/* Count of duplicate (do-nothing) ACKs */

	tcb_pt->rcv_nxt=0;	/* Incoming sequence number expected next */
	tcb_pt->rcv_wnd=0;	/* Our offered receive window */
	tcb_pt->rcv_up=0;	/* Receive urgent pointer */
	tcb_pt->rcv_wind_scale=0;   /* Recv window scale */

	tcb_pt->last_ack_sent=0;    /* Last ack sent for timestamp purposes */
	tcb_pt->ts_recent=0;	    /* Most recent incoming timestamp */

	tcb_pt->irs=0;		    /* Initial receive sequence number */
	tcb_pt->rerecv=0;	    /* Count of duplicate bytes received */
	tcb_pt->mss=0;		    /* Maximum segment size */

	tcb_pt->window=0;	    /* Receiver window and send queue limit */
	tcb_pt->limit=0;	    /* Send queue limit */
	tcb_pt->rtn_type=TCP_INIT_RTNS_TYPE0;

	tcb_pt->force_flag=0;	/* We owe the other end an ACK or window update */
	tcb_pt->clone_flag=0;	/* Server-type TCB, cloned on incoming SYN */
	tcb_pt->retran_flag=0;	/* A retransmission has occurred */
	tcb_pt->active_flag=0;	/* TCB created with an active open */
	tcb_pt->synack_flag=0;	/* Our SYN has been acked */
	tcb_pt->rtt_run_flag=0; /* We're timing a segment */
	tcb_pt->congest_flag=0; /* Copy of last IP congest bit received */
	tcb_pt->ts_ok_flag=0;	/* We're using timestamps */
	tcb_pt->ws_ok_flag=0;	/* We're using window scaling */
	tcb_pt->my_reset_flag=0;
	tcb_pt->my_send_rst_flag=0;
//	tcb_pt->tos;		/* Type of service (for IP) */
	tcb_pt->backoff=0;	/* Backoff interval */

	tcb_pt->rcvq=NULL_MEM_REC;	/* Receive queue */
	tcb_pt->sndq=NULL_MEM_REC;	/* Send queue */
	tcb_pt->rcvcnt=0;		/* Count of items on rcvq */
	tcb_pt->sndcnt=0;		/* Number of unacknowledged sequence numbers on */
					/* sndq. NB: includes SYN and FIN, which don't  */
					/* actually appear on sndq!			*/

	tcb_pt->reseq=NULL;	   /* Out-of-order segment queue */
	tcb_pt->timer_no=NULL_TIMER;  /* Retransmission timer */
	tcb_pt->rtt_time=0;	   /* Stored clock values for RTT */
	tcb_pt->rttseq=0;	   /* Sequence number being timed */
	tcb_pt->rttack=0;	   /* Ack at start of timing (for txbw calc) */
	tcb_pt->srtt=0; 	   /* Smoothed round trip time, milliseconds */
	tcb_pt->mdev=0; 	   /* Mean deviation, milliseconds */
	tcb_pt->rtt=0;		   /* Last received RTT (for debugging) */

	tcb_pt->user=NULL_SOCKET;  /* User parameter (e.g., for mapping to an */
				   /* application control block 	      */

	tcb_pt->quench=0;	   /* Count of incoming ICMP source quenches */
	tcb_pt->unreach=0;	   /* Count of incoming ICMP unreachables */
	tcb_pt->timeouts=0;	   /* Count of retransmission timeouts */
	tcb_pt->lastack=0;	   /* Time of last received ack */
	tcb_pt->outlen=0;	   /* Average tx ack amount */
	tcb_pt->outrate=0;	   /* Average tx ack interval, ms */
	tcb_pt->lastrx=0;	   /* Time of last received data */
	tcb_pt->inlen=0;	   /* Average receive data size */
	tcb_pt->inrate=0;	   /* Average receive packet interval,ms */
	tcb_pt->trdiscard_flag=RESET;
	tcb_pt->prev=eq_tcb_st;
	if(eq_tcb_st < MAX_TCB)
		tcb_st[eq_tcb_st].next=tcb_no;
	else
		fq_tcb_st=tcb_no;
	eq_tcb_st=tcb_no;
	tcb_pt->next=NULL_TCB;
	tcb_pt->type=NULL_TCP_TYPE;
#if ((defined H245_GATEWAY_SIDE) || (defined MY_TCPIP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT))
	tcb_pt->rec_no=(guint)(-1);
#endif
	return;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
void init_usock_st(void){

	unsigned int i,j;
	USOCK_STRUCT *usock_pt;

	for(i=0,usock_pt=&usock_st[0];i < MAX_USOCK;i++,usock_pt++){
		usock_pt->state=IN_QUE;
		usock_pt->index=0;
		usock_pt->sock_value=0;
		usock_pt->refcnt=0;
		usock_pt->noblock=0;
//		usock_pt->type=0;
		usock_pt->rdysock=0;
#ifdef TCP_INSTALLED
		usock_pt->cb.tcb_no=NULL_TCB;
#endif
		usock_pt->localname_mr1=NULL_MEM_REC;
		usock_pt->peername_mr1=NULL_MEM_REC;
		for(j=0;j<4;j++){
			usock_pt->errcodes[j]=0;      /* Protocol-specific error codes */
		}
		usock_pt->tos=0;	/* Internet type-of-service */
		usock_pt->flag=0;	/* Mode flags, defined in socket.h */

		usock_pt->next=i+1;
		usock_pt->prev=i-1;
	}
	fq_usock_st=0;
	eq_usock_st=MAX_USOCK-1;
	usock_st[MAX_USOCK-1].next=NULL_USOCK;
	usock_st[0].prev=NULL_USOCK;
	return;
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
unsigned int find_free_usock_st(void){

	unsigned int usock_no;
	USOCK_STRUCT *usock_pt;

	if(fq_usock_st >= MAX_USOCK){
		prnt_msg(IP_ERR,0x2074);
		return NULL_USOCK;
	}
	usock_no=fq_usock_st;
	usock_pt=&usock_st[usock_no];
	fq_usock_st=usock_pt->next;
	if(fq_usock_st < MAX_USOCK)
		usock_st[fq_usock_st].prev=NULL_USOCK;
	else
		eq_usock_st=NULL_USOCK;
	usock_pt->next=NULL_USOCK;
	usock_pt->state=OUT_OF_QUE;
	return usock_no;
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
void rls_usock_st(unsigned int usock_no){

	USOCK_STRUCT *usock_pt;
	unsigned int j;

	if(usock_no >= MAX_USOCK){
		prnt_msg(IP_ERR,0x2075);
		prnt_int(usock_no);prnt_char(CR);
		return;
	}
	usock_pt=&usock_st[usock_no];
	if(usock_pt->state == IN_QUE){
		prnt_msg(IP_ERR,0x2076);
		prnt_int(usock_no);prnt_char(CR);
		return;
	}
	usock_pt->state=IN_QUE;
	usock_pt->index=0;
	usock_pt->sock_value=0;
	usock_pt->refcnt=0;
	usock_pt->noblock=0;
//	usock_pt->type=0;
	usock_pt->rdysock=0;
#ifdef TCP_INSTALLED
	usock_pt->cb.tcb_no=NULL_TCB;
#endif
	if(usock_pt->localname_mr1 < NO_MEM_REC){
	    mem_rec_rls(usock_pt->localname_mr1);
	}
	usock_pt->localname_mr1=NULL_MEM_REC;
	if(usock_pt->peername_mr1 < NO_MEM_REC){
	    mem_rec_rls(usock_pt->peername_mr1);
	}
	usock_pt->peername_mr1=NULL_MEM_REC;
	for(j=0;j<4;j++){
		usock_pt->errcodes[j]=0;      /* Protocol-specific error codes */
	}
	usock_pt->tos=0;	/* Internet type-of-service */
	usock_pt->flag=0;	/* Mode flags, defined in socket.h */
	usock_pt->prev=eq_usock_st;
	if(eq_usock_st < MAX_USOCK)
		usock_st[eq_usock_st].next=usock_no;
	else
		fq_usock_st=usock_no;
	eq_usock_st=usock_no;
	usock_pt->next=NULL_USOCK;
	return;
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ======================================================================== */
/* Timer timeout							    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void tcp_timeout(unsigned int timer_no){

	TMR *tp;
	unsigned int tcb_no;
	TCB_STRUCT *tcb_pt;
	gint32 ptrsave;

	tp=&timer[timer_no];
#ifdef DEBUG_ACT
	prnt_string("tcp_timeout\r\n");
	print_call_frame();
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
	print_addr(tp->addr);
#endif
#endif
#endif
	if(tp->it.t != TCP_SDL_T){
		return;
	}
	tcb_no=tp->it.i;
	if(tcb_no >= MAX_TCB){
		return;
	}
	tcb_pt = &tcb_st[tcb_no];

	/* Make sure the timer has stopped (we might have been kicked) */
//	stop_timer(&tcb_pt->timer);
//	stoptimer(tcb_no,TCP_SDL_T);
//	tcb_st[tcb_no].timer_no=NULL_TIMER;

	tcb_pt->timer_no=NULL_TIMER;

	switch(tcb_pt->state){
	case TCP_TIME_WAIT:	/* 2MSL timer has expired */
		close_self(tcb_no,NORMAL_CLOSE_REASON);
		break;
	default:		/* Retransmission timer has expired */
		if(tcb_pt->timeouts < DEF_RETRIES){
			prnt_msg(TCP_ERR,0x2095);
			prnt_int(tcb_no);prnt_char('-');prnt_int(tcb_pt->type);prnt_char('-');
			prnt_int(tcb_pt->rec_no);prnt_char(CR);
			tcb_pt->timeouts++;
			tcb_pt->retran_flag = 1;  /* Indicate > 1  transmission */
			tcb_pt->backoff++;
			/* Reduce slowstart threshold to half current window */
			tcb_pt->ssthresh = tcb_pt->cwind / 2;
			tcb_pt->ssthresh = max(tcb_pt->ssthresh,tcb_pt->mss);
			/* Shrink congestion window to 1 packet */
			tcb_pt->cwind = tcb_pt->mss;
			/* Retransmit just the oldest unacked packet */
			ptrsave = tcb_pt->snd_ptr;
			tcb_pt->snd_ptr = tcb_pt->snd_una;
			tcp_output(tcb_no);
			tcb_pt->snd_ptr = ptrsave;
		}
		else{
			prnt_msg(TCP_ERR,0x2096);
			prnt_int(tcb_no);prnt_char('-');prnt_int(tcb_pt->type);prnt_char('-');
			prnt_int(tcb_pt->rec_no);prnt_char(CR);
			switch(tcb_pt->type){
#ifdef KFTPD_INSTALLED
			case KFTPD_DATA_TCP_TYPE	  :
				if(tcb_pt->rec_no < MAX_KFTPD_REC){
					to_aux_kftpd_q(tcb_pt->rec_no, AUX_KFTPD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
				}
				tcb_pt->rec_no=(unsigned int)(-1);
				prnt_string("ftp server data tcp failure:");prnt_int(tcb_no);prnt_char(CR);
				break;
			case KFTPD_CONTROL_TCP_TYPE		  :
				if(tcb_pt->rec_no < MAX_KFTPD_REC){
					to_kftpd_q(tcb_pt->rec_no, KFTPD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
				}
				tcb_pt->rec_no=(unsigned int)(-1);
				prnt_string("ftp server control tcp failure:");prnt_int(tcb_no);prnt_char(CR);
				break;
#endif
#ifdef KFTP_INSTALLED
			case KFTP_DATA_TCP_TYPE   :
				if(tcb_pt->rec_no < MAX_KFTP_REC){
					to_aux_kftp_q(tcb_pt->rec_no, AUX_KFTP_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
				}
				tcb_pt->rec_no=(unsigned int)(-1);
				prnt_string("ftp client data tcp failure:");prnt_int(tcb_no);prnt_char(CR);
				break;
			case KFTP_CONTROL_TCP_TYPE		  :
				if(tcb_pt->rec_no < MAX_KFTP_REC){
					to_kftp_q(tcb_pt->rec_no, KFTP_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
				}
				tcb_pt->rec_no=(unsigned int)(-1);
				prnt_string("ftp client control tcp failure:");prnt_int(tcb_no);prnt_char(CR);
				break;
#endif
#ifdef ROS_FOR_LI_INSTALLED
			case LI_ROS_TCP_TYPE	:
				if(tcb_pt->rec_no < MAX_LI_XPORT_REC){
					to_li_xport_que(tcb_pt->rec_no, LI_XPORT_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
				}
				tcb_pt->rec_no=(unsigned int)(-1);
				prnt_string("ros for li client tcp failure:");prnt_int(tcb_no);prnt_char(CR);
				break;
#endif
#ifdef TELNETD_INSTALLED
			case TELNETD_TCP_TYPE		  :
				if(tcb_pt->rec_no < MAX_TELNETD_REC){
					to_telnetd_q(tcb_pt->rec_no, TELNETD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
				}
				tcb_pt->rec_no=(unsigned int)(-1);
				prnt_string("telnet server control tcp failure:");prnt_int(tcb_no);prnt_char(CR);
				break;
#endif
#ifdef HI1_INSTALLED
			case HI1_TCP_TYPE	      :
				if(tcb_pt->rec_no < MAX_HI1_REC){
					to_hi1_q(tcb_pt->rec_no, HI1_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
				}
				tcb_pt->rec_no=(unsigned int)(-1);
				prnt_string("hi1 server control tcp failure:");prnt_int(tcb_no);prnt_char(CR);
				break;
#endif
#ifdef SYSCON_SERVER
			case SYSCON_SERVER_TCP_TYPE		:
				if(tcb_pt->rec_no < MAX_SYSCON_CLIENT){
				    guint syscon_client_no=tcb_pt->rec_no;
				    SYSCON_SERVER_STRUCT *sssp=&syscon_server_st[syscon_client_no];
				    sssp->tcp_no=NULL_TCB;
				    do_syscon_server_finish(syscon_client_no);
				}
				tcb_pt->rec_no=(unsigned int)(-1);
				prnt_string("syscon server control tcp failure:");prnt_int(tcb_no);prnt_char(CR);
				break;
#endif
			default 		  :
				break;
			}
			reset_tcp(tcb_no);
		}
	}
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Backoff function - the subject of much research			    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
gint32 backoff(int n){

	if(n > 31)
		n = 31; /* Prevent truncation to zero */

	return 1L << n; /* Binary exponential back off */
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_INSTALLED) || (defined ARP_INSTALLED))
unsigned int change_to_kiatel_count(unsigned int cnt){
	return(cnt/20);
}
#endif // TCP_INSTALLED || ARP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void init_inet_st(void){

	unsigned int i;
	INET_STRUCT *inet_pt;

	for(i=0,inet_pt=&inet_st[0];i < MAX_INET;i++,inet_pt++){
		inet_pt->state=IN_QUE;
		inet_pt->tcb_no=NULL_TCB;
//		inet_pt->stack;
//		inet_pt->task_no;

		inet_pt->next=i+1;
		inet_pt->prev=i-1;
	}
	fq_inet_st=0;
	eq_inet_st=MAX_INET-1;
	inet_st[MAX_INET-1].next=NULL_INET;
	inet_st[0].prev=NULL_INET;
	return;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
unsigned int find_free_inet_st(void){

	unsigned int inet_no;
	INET_STRUCT *inet_pt;

	if(fq_inet_st >= MAX_INET){
		prnt_msg(TCP_ERR,0x2077);
		return NULL_INET;
	}
	inet_no=fq_inet_st;
	inet_pt=&inet_st[inet_no];
	fq_inet_st=inet_pt->next;
	if(fq_inet_st < MAX_INET)
		inet_st[fq_inet_st].prev=NULL_INET;
	else
		eq_inet_st=NULL_INET;
	inet_pt->next=NULL_INET;
	inet_pt->state=OUT_OF_QUE;
	return inet_no;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void rls_inet_st(unsigned int inet_no){

	INET_STRUCT *inet_pt;

	if(inet_no >= MAX_INET){
		prnt_msg(TCP_ERR,0x2078);
		prnt_int(inet_no);prnt_char(CR);
		return;
	}
	inet_pt=&inet_st[inet_no];
	if(inet_pt->state == IN_QUE){
		prnt_msg(TCP_ERR,0x2079);
		prnt_int(inet_no);prnt_char(CR);
		return;
	}
	inet_pt->state=IN_QUE;
	inet_pt->tcb_no=NULL_TCB;
//	inet_pt->stack;
//	inet_pt->task_no;
	inet_pt->prev=eq_inet_st;
	if(eq_inet_st < MAX_INET)
		inet_st[eq_inet_st].next=inet_no;
	else
		fq_inet_st=inet_no;
	eq_inet_st=inet_no;
	inet_pt->next=NULL_INET;
	return;
}
#endif // TCP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
void tcp_udp_init(void){

#ifdef TCP_INSTALLED
	Tcbs = NULL_TCB;
	Inet_list=NULL_INET;
	msclock_cnt=0;
	Tcp_window = DEF_WND;
	Tcp_mss = DEF_MSS;	   /* Maximum segment size to be sent with SYN */
	Tcp_irtt = DEF_RTT;	  /* Initial guess at round trip time */
	Tcp_tstamps = 1;
	init_tcb_st();
	init_inet_st();
#endif
#ifdef UDP_INSTALLED
	Udps = NULL_UCB;
	init_ucb_st();
#endif
	Nsock=MAX_USOCK;
	Lport = 1024;
	init_usock_st();
}
#endif // ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
/* ========================================================================== */
/* ========================================================================== */
#ifdef TCP_INSTALLED
void copy_tcb_fields_to_new_one(guint old_tcb_no,guint new_tcb_no){

	TCB_STRUCT *old_tcb_pt,*new_tcb_pt;
	guint saved_next,saved_prev;


	old_tcb_pt=&tcb_st[old_tcb_no];
	new_tcb_pt=&tcb_st[new_tcb_no];

	saved_next=new_tcb_pt->next;
	saved_prev=new_tcb_pt->prev;

	mem_cpy((guint8 *)new_tcb_pt,(guint8 *)old_tcb_pt,sizeof(TCB_STRUCT));

	new_tcb_pt->next=saved_next;
	new_tcb_pt->prev=saved_prev;

	switch(new_tcb_pt->type){
#ifdef Q931_FOR_H225
	  case	H225_Q931_TCP_TYPE	:
		break;
#endif
#ifdef H245_GATEWAY_SIDE
	  case	H245_TCP_TYPE		:
		break;
#endif
#ifdef MY_TCPIP_FOR_OAM
	  case	OAM_TCP_TYPE		:
		break;
#endif
#ifdef KFTPD_INSTALLED
	case KFTPD_CONTROL_TCP_TYPE	:
	case KFTPD_DATA_TCP_TYPE	:
		break;
#endif
#ifdef KFTP_INSTALLED
	case KFTP_CONTROL_TCP_TYPE	:
	case KFTP_DATA_TCP_TYPE 	:
		break;
#endif
#ifdef ROS_FOR_LI_INSTALLED
	case LI_ROS_TCP_TYPE		:
		break;

#endif
#ifdef TELNETD_INSTALLED
	case TELNETD_TCP_TYPE		:
		break;
#endif
#ifdef HI1_INSTALLED
	case HI1_TCP_TYPE		:
		break;
#endif
#ifdef HI1_DISTRIBUTOR
	case HI1_DISTRIBUTOR_TCP_TYPE	:
		break;
#endif
#ifdef SYSCON_SERVER
	case SYSCON_SERVER_TCP_TYPE	:
		break;
#endif
#ifdef SYSCON_CLIENT
	case SYSCON_CLIENT_TCP_TYPE	:
		break;
#endif
	  default			:
		prnt_msg(TCP_ERR,0x2311);
		prnt_byte(new_tcb_pt->type);prnt_char(CR);
		break;
	}
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef TCP_INSTALLED
guint start_tcp_timer(guint tcb_no,guint cnt){

	guint i;

#ifdef HANG_DEBUG
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2085);
		prnt_int(tcb_no);prnt_char(CR);
		return NULL_TIMER;
	}
#endif

#ifdef DEBUG_ACT
	prnt_string("start_tcp_timer\r\n");
	prnt_int(tcb_no);prnt_int(cnt);prnt_char(CR);
	print_call_frame();
#endif
	if (tcb_st[tcb_no].timer_no == NULL_TIMER){
		i=setimer(cnt,tcb_no,TCP_SDL_T);
		if (i < TIMER_NO){
			tcb_st[tcb_no].timer_no=i;
			return i;
		}
		else{
			prnt_msg(TCP_ERR,0x2086);
			return NULL_TIMER;
		}
	}
	else{
		unsigned int tmr_no=tcb_st[tcb_no].timer_no;

		prnt_msg(TCP_ERR,0x2087);
		print_call_frame();
#ifdef NEW_DEBUG
		if (tmr_no < TIMER_NO){
			print_addr(timer[tmr_no].addr);
		}
#endif
		return NULL_TIMER;
	}
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef TCP_INSTALLED
void stop_tcp_timer(guint tcb_no){

#ifdef HANG_DEBUG
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2088);
		prnt_int(tcb_no);prnt_char(CR);
		return;
	}
#endif
#ifdef DEBUG_ACT
	prnt_string("stop_tcp_timer\r\n");
	prnt_int(tcb_no);prnt_char(CR);
	print_call_frame();
#endif
	if (tcb_st[tcb_no].timer_no < TIMER_NO){
		stoptimer(tcb_st[tcb_no].timer_no,TCP_SDL_T);
		tcb_st[tcb_no].timer_no=NULL_TIMER;
	}
	else{
		prnt_msg(TCP_ERR,0x2089);
		prnt_int(tcb_no);prnt_char('-');
		prnt_int(tcb_st[tcb_no].timer_no);prnt_char(CR);
		print_call_frame();
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void send_rst(unsigned int tcb_no){

	TCB_STRUCT *tcb_pt;
#ifdef DEBUG_ACT
	prnt_string("send_rst:");prnt_int(tcb_no);prnt_char(CR);
#endif
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2090);
		return;
	}
	tcb_pt=&tcb_st[tcb_no];
	tcb_pt->my_reset_flag = 1;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void my_reset_tcp(unsigned int tcb_no){

#ifdef DEBUG_ACT
	prnt_string("my_reset_tcp:");prnt_int(tcb_no);prnt_char(CR);
#endif
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2091);
		return;
	}
	if(tcb_st[tcb_no].state == TCP_CLOSED){
		prnt_msg(TCP_ERR,0x2099);
		prnt_int(tcb_no);prnt_char(CR);
		print_call_frame();
		return;
	}
	send_rst(tcb_no);
	tcp_output(tcb_no);
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void my_release_tcp(unsigned int tcb_no){

#ifdef DEBUG_ACT
	prnt_string("my_release_tcp:");prnt_int(tcb_no);prnt_char(CR);
#endif
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2092);
		return;
	}
	del_tcp(tcb_no);
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
