
/*************	Time-stamp: <2009-09-16 16:48:09 savoj>   ************/
// ERROR : 0x2200-0x2216


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
#endif // CENTRAL_EXTERN

#ifdef KA9Q_TCP_DEBUG_ACT
#define DEBUG_ACT
#endif

/* ======================================================================== */
/* Start a TCP server. Create TCB in listen state and post upcall for	    */
/* when a connection comes in						    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
// this routine opens a tcb in server mode (not passive mode)
guint start_passive_tcp(SOCKET_STRUCT *lsocket_pt,
		  SOCKET_STRUCT *fsocket_pt _U_,
		  unsigned int task _U_,
		  unsigned int stack _U_,
		  int mode _U_
){
#if 0
	INET_STRUCT *inet_pt;
	unsigned int inet_no;

	inet_no=find_free_inet_st();
	if(inet_no >= MAX_INET){
		prnt_msg(TCP_ERR,0x2200);
		return FAIL;
	}
	inet_pt=&inet_st[inet_no];

	inet_pt->tcb_no = open_tcp(lsocket_pt,fsocket_pt,mode,0,
				   TCP_INIT_RTNS_TYPE1,0,-1);
	if(inet_pt->tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2201);
		rls_inet_st(inet_no);
		return FAIL;
	}
	inet_pt->stack = stack;
//	inet_pt->task = task;
	inet_pt->next = Inet_list;
	Inet_list = inet_no;
#endif // 0
	SOCKADDR_IN_STRUCT sock;
	int s;
	USOCK_STRUCT *up;
	guint tcb_no;

	s=socket(AF_INET,SOCK_STREAM,0);
	if((unsigned int)s >= Nsock){
		prnt_msg(TCP_ERR,0x2214);
		return NULL_TCB;
	}
	up=&usock_st[s];
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = lsocket_pt->address;
	sock.sin_port	     = lsocket_pt->port;
	bind(s,(SOCKADDR_STRUCT *)&sock,SOCKSIZE);
//	tcb_no=so_tcp_listen(up,0); // TCP_PASSIVE
	tcb_no=so_tcp_listen(up,1); // TCP_SERVER
	return tcb_no;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Start a TCP server. Create TCB in listen state and post upcall for	    */
/* when a connection comes in						    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
guint8 start_tcp(unsigned int port,unsigned int task _U_,unsigned int stack){

	INET_STRUCT *inet_pt;
	SOCKET_STRUCT lsocket;
	unsigned int inet_no;

	inet_no=find_free_inet_st();
	if(inet_no >= MAX_INET){
		prnt_msg(TCP_ERR,0x2200);
		return FAIL;
	}
	inet_pt=&inet_st[inet_no];

	lsocket.address = INADDR_ANY;
	lsocket.port = port;
	inet_pt->tcb_no = open_tcp(&lsocket,NULL,TCP_SERVER,0,
				   TCP_INIT_RTNS_TYPE0,0,-1);
	if(inet_pt->tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2201);
		rls_inet_st(inet_no);
		return FAIL;
	}
	inet_pt->stack = stack;
//	inet_pt->task = task;
	inet_pt->next = Inet_list;
	Inet_list = inet_no;
	switch(port){
#ifdef KFTPD_INSTALLED
	case  IPPORT_FTPD:
	case  IPPORT_FTP:
		tcb_st[inet_pt->tcb_no].type=task; // type
		break;
#endif
#ifdef TELNETD_INSTALLED
	case IPPORT_TELNETD :
		tcb_st[inet_pt->tcb_no].type=task; // type
		break;
#endif
	default:
#ifdef HI1_INSTALLED
		if(port == ipport_hi1){
		    tcb_st[inet_pt->tcb_no].type=task; // type
		    break;
		}
		else
#endif
#ifdef SYSCON_SERVER
		if(port == ipport_syscon_server){
		    tcb_st[inet_pt->tcb_no].type=task; // type
		    break;
		}
		else
#endif
		{
		    break;
		}
	}
	return OK;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Close down a TCP server created earlier by inet_start		    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int stop_tcp(unsigned int port){

	unsigned int in,inprev;

	inprev = NULL_INET;
	for(in = Inet_list;in != NULL_INET;inprev=in,in = inet_st[in].next)
		if(tcb_st[inet_st[in].tcb_no].conn.local.port == port)
			break;
	if(in == NULL_INET){
		prnt_msg(TCP_ERR,0x2202);
		return -1;
	}
	close_tcp(inet_st[in].tcb_no);
// to be checked
//	free(in->name);
	if(inprev != NULL_INET)
		inet_st[inprev].next = inet_st[in].next;
	else
		Inet_list = inet_st[in].next;
	rls_inet_st(in);
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
unsigned int open_tcp(
SOCKET_STRUCT *lsocket_pt,  /* Local socket */
SOCKET_STRUCT *fsocket_pt,  /* Foreign socket */
int mode,	 /* Active/passive/server */
guint window,	   /* Receive window (and send buffer) sizes */
guint8 rtn_type,
unsigned int tos,
unsigned int user	  /* User linkage area */
){
	CONNECTION_STRUCT conn;
	TCB_STRUCT *tcb_pt;
	unsigned int tcb_no;

#ifdef DEBUG_ACT
	prnt_string("open_tcp\r\n");
#endif
	if(lsocket_pt == NULL){
		prnt_msg(TCP_ERR,0x2203);
		Net_error = NET_ERROR_INVALID;
		return NULL_TCB;
	}
	conn.local.address = lsocket_pt->address;
	conn.local.port = lsocket_pt->port;
	if(fsocket_pt != NULL){
		conn.remote.address = fsocket_pt->address;
		conn.remote.port = fsocket_pt->port;
	}
	else {
		conn.remote.address = 0;
		conn.remote.port = 0;
	}
	if((tcb_no = lookup_tcb(&conn)) >= MAX_TCB){
		if((tcb_no = create_tcb(&conn)) >= MAX_TCB){
			prnt_msg(TCP_ERR,0x2204);
			Net_error = NET_ERROR_NO_MEM;
			return NULL_TCB;
		}
		tcb_pt=&tcb_st[tcb_no];
	}
	else{
		tcb_pt=&tcb_st[tcb_no];
		if(tcb_pt->state != TCP_LISTEN){
			prnt_msg(TCP_ERR,0x2205);
			Net_error = NET_ERROR_CON_EXISTS;
			return NULL_TCB;
		}
	}
#ifdef DEBUG_ACT
	prnt_int(tcb_no);prnt_char(CR);
#endif
	tcb_pt->user = user;
	if(window != 0)
		tcb_pt->window = tcb_pt->rcv_wnd = window;
	else
		tcb_pt->window = tcb_pt->rcv_wnd = Tcp_window;
	tcb_pt->snd_wnd = 1;	   /* Allow space for sending a SYN */
	tcb_pt->rtn_type=rtn_type;
	tcb_pt->tos = tos;
	switch(mode){
	  case TCP_SERVER:
#ifdef DEBUG_ACT
		prnt_string("open tcp_server\r\n");
#endif
		tcb_pt->clone_flag = 1;
		settcpstate(tcb_no,TCP_LISTEN);
		break;
	  case TCP_PASSIVE:
#ifdef DEBUG_ACT
		prnt_string("open tcp_passive\r\n");
#endif
		settcpstate(tcb_no,TCP_LISTEN);
		break;
	  case TCP_ACTIVE:
#ifdef DEBUG_ACT
		prnt_string("open tcp_active\r\n");
#endif
		/* Send SYN, go into TCP_SYN_SENT state */
		tcb_pt->active_flag = 1;
		send_syn(tcb_no);
		settcpstate(tcb_no,TCP_SYN_SENT);
		tcp_output(tcb_no);
		break;
	  default	  :
		prnt_msg(TCP_ERR,0x2206);
		break;
	}
	return tcb_no;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* This really means "I have no more data to send". It only closes the      */
/* connection in one direction, and we can continue to receive data	    */
/* indefinitely.							    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int close_tcp(unsigned int tcb_no){

	TCB_STRUCT *tcb_pt;

#ifdef DEBUG_ACT
	prnt_string("close_tcp:");prnt_int(tcb_no);prnt_char(CR);
#endif
	if(tcb_no >= MAX_TCB){
		Net_error = NET_ERROR_INVALID;
		return -1;
	}
	tcb_pt=&tcb_st[tcb_no];

	switch(tcb_pt->state){
	  case TCP_CLOSED:
		return 0;	/* Unlikely */
	  case TCP_LISTEN:
	  case TCP_SYN_SENT:
		close_self(tcb_no,NORMAL_CLOSE_REASON);
		return 0;
	  case TCP_SYN_RECEIVED:
	  case TCP_ESTABLISHED:
		tcb_pt->sndcnt++;
		tcb_pt->snd_nxt++;
		settcpstate(tcb_no,TCP_FINWAIT1);
		tcp_output(tcb_no);
		return 0;
	  case TCP_CLOSE_WAIT:
		tcb_pt->sndcnt++;
		tcb_pt->snd_nxt++;
		settcpstate(tcb_no,TCP_LAST_ACK);
		tcp_output(tcb_no);
		return 0;
	  case TCP_FINWAIT1:
	  case TCP_FINWAIT2:
	  case TCP_CLOSING:
	  case TCP_LAST_ACK:
	  case TCP_TIME_WAIT:
		Net_error = NET_ERROR_CON_CLOS;
		return -1;
	}
	return -1;	/* "Can't happen" */
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Delete TCB, free resources. The user is not notified, even if the TCB is   */
/* not in the TCP_CLOSED state. This function should normally be called by the*/
/* user only in response to a state change upcall to TCP_CLOSED state.	      */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int del_tcp(unsigned int this_tcb){

	unsigned int tcb_no;
	unsigned int tcbprev_no = NULL_TCB;
	RESEQ_STRUCT *rp,*rp1;
	TCB_STRUCT *tcb_pt;

#ifdef DEBUG_ACT
	prnt_string("del_tcp:");prnt_int(this_tcb);prnt_char(CR);
#endif
	if(this_tcb >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2215);
		Net_error = NET_ERROR_INVALID;
		return -1;	/* this_tcb was NULL */
	}
	/* Remove from list */
	for(tcb_no=Tcbs;tcb_no < MAX_TCB;tcbprev_no=tcb_no,tcb_no=tcb_st[tcb_no].next){
		if(tcb_no == this_tcb)
			break;
	}
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2216);
		prnt_int(this_tcb);prnt_char(CR);
		Net_error = NET_ERROR_INVALID;
		return -1;	/* this_tcb was not on list */
	}
	tcb_pt=&tcb_st[this_tcb];
	if(tcbprev_no < MAX_TCB)
		tcb_st[tcbprev_no].next = tcb_st[this_tcb].next;
	else
		Tcbs = tcb_st[this_tcb].next;	    /* was first on list */
//	stoptimer(tcb_no,TCP_SDL_T);
	if(tcb_pt->timer_no < TIMER_NO ){
		stop_tcp_timer(tcb_no);
	}
	tcb_st[tcb_no].timer_no=NULL_TIMER;
	for(rp = tcb_pt->reseq;rp != NULL;rp = rp1){
		rp1 = rp->next;
		rls_reseq(rp1);
//		free_p(&rp->bp);
//		free(rp);
	}
	tcb_pt->reseq = NULL;
//	free_p(&tcb_pt->rcvq);
//	free_p(&tcb_pt->sndq);
//	free(tcb);
	rls_mr1_list(tcb_pt->rcvq);
	rls_mr1_list(tcb_pt->sndq);
	if(tcb_pt->user < Nsock){
		rls_usock_st(tcb_pt->user);
	}
	rls_tcb_st(tcb_no);
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Return 1 if arg is a valid TCB, 0 otherwise */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int tcpval(unsigned int tcb_no){

	unsigned int tcb1_no;

	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2207);
		return 0;	/* Null pointer can't be valid */
	}
	for(tcb1_no=Tcbs;tcb1_no < MAX_TCB;tcb1_no = tcb_st[tcb1_no].next){
		if(tcb1_no == tcb_no)
			return 1;
	}
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Kick a particular TCP connection					    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int kick_tcp(unsigned int tcb_no){

	TCB_STRUCT *tcb_pt;

	if(!tcpval(tcb_no)){
		return -1;
	}
	tcb_pt=&tcb_st[tcb_no];
	tcb_pt->force_flag = 1;   /* Send ACK even if no data */
	tcb_pt->backoff = 0;
	tcp_timeout(tcb_no);
	return 0;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Kick all TCP connections to specified address; return number kicked	    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int kick(guint32 addr){

	unsigned int tcb_no;
	int cnt = 0;

	for(tcb_no=Tcbs;tcb_no < MAX_TCB;tcb_no = tcb_st[tcb_no].next){
		if(tcb_st[tcb_no].conn.remote.address == addr){
			kick_tcp(tcb_no);
			cnt++;
		}
	}
	return cnt;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Clear all TCP connections						    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void reset_all(void){

	unsigned int tcb_no,tcbnext_no;

	for(tcb_no=Tcbs;tcb_no < MAX_TCB;tcb_no = tcbnext_no){
		tcbnext_no = tcb_st[tcb_no].next;
		reset_tcp(tcb_no);
	}
//	kwait(NULL);	/* Let the RSTs go forth */
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void reset_tcp(unsigned int tcb_no){

	TCP_STRUCT fakeseg;
	IP_STRUCT fakeip;
	TCB_STRUCT *tcb_pt;

//#ifdef DEBUG_ACT
	prnt_string("reset_tcp:");prnt_int(tcb_no);prnt_char(CR);
//#endif
	if(tcb_no >= MAX_TCB)
		return;
	tcb_pt=&tcb_st[tcb_no];
	if(tcb_pt->state != TCP_LISTEN){
		/* Compose a fake segment with just enough info to generate the
		 * correct RST reply
		 */
		mem_set(&fakeseg,0,sizeof(fakeseg));
		mem_set(&fakeip,0,sizeof(fakeip));
		fakeseg.dest = tcb_pt->conn.local.port;
		fakeseg.source = tcb_pt->conn.remote.port;
		fakeseg.ack_flag = 1;

		/* Here we try to pick a sequence number with the greatest likelihood */
		/* of being in his receive window.				      */
//TESTTEST
//		fakeseg.ack = tcb_pt->snd_nxt + tcb_pt->snd_wnd - 1;
		fakeseg.ack = tcb_pt->snd_nxt ;
		fakeip.dest = tcb_pt->conn.local.address;
		fakeip.source = tcb_pt->conn.remote.address;
		fakeip.tos = tcb_pt->tos;
		reset(&fakeip,&fakeseg);
	}
	close_self(tcb_no,RESET_CLOSE_REASON);
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
guint start_active_tcp(SOCKET_STRUCT *lsocket_pt,
		     SOCKET_STRUCT *fsocket_pt){
	SOCKADDR_IN_STRUCT sock;
	int s;
	guint tcb_no;

#ifdef DEBUG_ACT
	prnt_string("start_active_tcp start\r\n");
#endif
	s=socket(AF_INET,SOCK_STREAM,0);
	if((unsigned int)s >= Nsock){
		prnt_msg(TCP_ERR,0x2212);
		return NULL_TCB;
	}
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = lsocket_pt->address;
	sock.sin_port	     = lsocket_pt->port;
	bind(s,(SOCKADDR_STRUCT *)&sock,SOCKSIZE);

	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = fsocket_pt->address;
	sock.sin_port	     = fsocket_pt->port;
	tcb_no=connect(s,(SOCKADDR_STRUCT *)&sock,sizeof(SOCKADDR_IN_STRUCT));
	if(tcb_no == NULL_TCB){
		prnt_msg(TCP_ERR,0x2213);
		rls_usock_st(s);
		return NULL_TCB;
	}
	tcb_st[tcb_no].user=s;
#ifdef DEBUG_ACT
	prnt_string("start_active_tcp end\r\n");
#endif
	return tcb_no;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Initiate active open. For datagram sockets, merely bind the remote address. */
/* ======================================================================== */
#ifdef TCP_INSTALLED
guint connect(
int s,			   /* Socket index */
SOCKADDR_STRUCT *peername, /* Peer name */
int peernamelen 	   /* Length of peer name */
){
	USOCK_STRUCT *up;
	WORD_MR mr1;
	guint tcb_no;

#ifdef DEBUG_ACT
	prnt_string("connect start:\r\n");
#endif
	if((unsigned int)s >= Nsock){
		errno = EBADF_ERROR;
		prnt_msg(TCP_ERR,0x2208);
		return NULL_TCB;
	}
	up=&usock_st[s];
	if(peername == NULL){
		/* Connect must specify a remote address */
		errno = EFAULT_ERROR;
		prnt_msg(TCP_ERR,0x2209);
		return NULL_TCB;
	}
	/* Check name format, if checking routine is available */
	if(checkipaddr(peername,peernamelen) == -1){
		errno = EAFNOSUPPORT_ERROR;
		prnt_msg(TCP_ERR,0x2210);
		return NULL_TCB;
	}
#if 0
	if(up->peername != NULL)
		free(up->peername);
	up->peername = mallocw(peernamelen);
	memcpy(up->peername,peername,peernamelen);
	up->peernamelen = peernamelen;
#endif // 0
	if(up->peername_mr1 < NO_MEM_REC){
		mem_rec_rls(up->peername_mr1);
	}
	mr1=mem_rec_req(peernamelen);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2211);
		return NULL_TCB;
	}
	mem_cpy(mem_rec[mr1].parm,(guint8 *)(peername),peernamelen);
	up->peername_mr1=mr1;

	/* a connect routine is optional - don't fail if it isn't present */
	tcb_no=so_tcp_conn(up);
#ifdef DEBUG_ACT
	prnt_string("connect end:\r\n");prnt_int(tcb_no);prnt_char(CR);
#endif
	return tcb_no;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
