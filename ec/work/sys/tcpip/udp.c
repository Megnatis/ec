
/*************	Time-stamp: <2007-07-11 11:56:39 Administrator>   ************/
// ERROR : 0x2500-0x2536

//#define DEBUG_ACT
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
#ifdef OPERATOR_SIDE
#ifdef MY_UDP_FOR_OAM
extern BUF hwrx_st[NO_LAPB_CH];
#endif
#endif
#endif // CENTRAL_EXTERN
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
guint start_udp(SOCKET_STRUCT *lsocket_pt){

	SOCKADDR_IN_STRUCT sock;
	int s;
	USOCK_STRUCT *up;

	s=socket(AF_INET,SOCK_DGRAM,0);
	if((unsigned int)s >= Nsock){
		prnt_msg(UDP_ERR,0x2214);
		return NULL_UCB;
	}
	up=&usock_st[s];
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = lsocket_pt->address;
	sock.sin_port	     = lsocket_pt->port;
	if(bind(s,(SOCKADDR_STRUCT *)&sock,SOCKSIZE) == -1){
		prnt_msg(UDP_ERR,0x2531);
		return NULL_UCB;
	}

	if(up->localname_mr1 >= NO_MEM_REC){
		autobind(up);
	}
// followings is done by the bind routine
#if 0
	{
		SOCKET_STRUCT lsock;
		SOCKADDR_IN_STRUCT *local;
		local = (SOCKADDR_IN_STRUCT *)(mem_rec[up->localname_mr1].parm);
		lsock.address = local->sin_addr.s_addr;
		lsock.port = local->sin_port;
		up->cb.ucb_no = open_udp(&lsock,s_urcall);
		ucb_st[up->cb.ucb_no].user = s;
	}
#endif //0
#ifdef DEBUG_ACT
	prnt_string("start_udp\r\n");
	prnt_long(lsocket_pt->address);prnt_char('-');
	prnt_int(lsocket_pt->port);prnt_char('-');
	prnt_int(up->cb.ucb_no);prnt_char('-');
	prnt_char(CR);
#endif
	return up->cb.ucb_no;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp(USOCK_STRUCT *up _U_,int protocol _U_){
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp_bind(USOCK_STRUCT *up){

	int s;
	SOCKADDR_IN_STRUCT *sp;
	SOCKET_STRUCT lsock;

	s = up->index;
//	sp = (SOCKADDR_IN_STRUCT *)up->name;
	if(up->localname_mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2533);
		return -1;
	}
	sp = (SOCKADDR_IN_STRUCT *)(mem_rec[up->localname_mr1].parm);
	lsock.address = sp->sin_addr.s_addr;
	lsock.port = sp->sin_port;
	up->cb.ucb_no = open_udp(&lsock,s_urcall);
	if(up->cb.ucb_no == NULL_UCB){
		prnt_msg(UDP_ERR,0x2532);
		return -1;
	}
	ucb_st[up->cb.ucb_no].user = s;
#ifdef DEBUG_ACT
	prnt_string("so_udp_bind\r\n");
	prnt_long(lsock.address);prnt_char('-');
	prnt_int(lsock.port);prnt_char('-');
	prnt_int(up->cb.ucb_no);prnt_char('-');
	prnt_int(s);prnt_char('-');
	prnt_char(CR);
#endif
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp_conn(USOCK_STRUCT *up){

//	if(up->name == NULL){
	if(up->localname_mr1 >= NO_MEM_REC){
		autobind(up);
	}
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp_recv(USOCK_STRUCT *up,WORD_MR *mr3,SOCKADDR_STRUCT *from,int *fromlen){

	int cnt;
	SOCKADDR_IN_STRUCT *remote;
	SOCKET_STRUCT fsocket;
	unsigned int ucb_no;
	WORD_MR mr1;

	mr1=*mr3;
	while(((ucb_no = up->cb.ucb_no) < MAX_UCB) &&
	      (cnt = recv_udp(ucb_no,&fsocket,&mr1)) == -1){
		if(up->noblock){
			errno = EWOULDBLOCK_ERROR;
			prnt_msg(UDP_ERR,0x2500);
			return -1;
		}
#if 0
		else if((errno = kwait(up)) != 0){
			return -1;
		}
#endif // 0
	}
	if(ucb_no >= MAX_UCB){
		/* Connection went away */
		errno = ENOTCONN_ERROR;
		prnt_msg(UDP_ERR,0x2501);
		return -1;
	}
	if((from != NULL) && (fromlen != (int *)NULL) && ((unsigned int)*fromlen >= SOCKSIZE)){
		remote = (SOCKADDR_IN_STRUCT *)from;
		remote->sin_family = AF_INET;
		remote->sin_addr.s_addr = fsocket.address;
		remote->sin_port = fsocket.port;
		*fromlen = SOCKSIZE;
	}
	*mr3=mr1;
	return cnt;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp_send(USOCK_STRUCT *up,WORD_MR mr1,SOCKADDR_STRUCT *to){

	SOCKADDR_IN_STRUCT *local,*remote;
	SOCKET_STRUCT lsock,fsock;

//	if(up->name == NULL)
	if(up->localname_mr1 >= NO_MEM_REC){
		autobind(up);
	}
//	local = (SOCKADDR_IN_STRUCT *)up->name;
	local = (SOCKADDR_IN_STRUCT *)(mem_rec[up->localname_mr1].parm);
	lsock.address = local->sin_addr.s_addr;
	lsock.port = local->sin_port;
	if(to != NULL) {
		remote = (SOCKADDR_IN_STRUCT *)to;
//	} else if(up->peername != NULL){
	} else if(up->peername_mr1 < NO_MEM_REC){
//		remote = (SOCKADDR_IN_STRUCT *)up->peername;
		remote = (SOCKADDR_IN_STRUCT *)mem_rec[up->peername_mr1].parm;
	} else {
		mem_rec_rls(mr1);
		errno = ENOTCONN_ERROR;
		prnt_msg(UDP_ERR,0x2502);
		return -1;
	}
	fsock.address = remote->sin_addr.s_addr;
	fsock.port = remote->sin_port;
//football
//	send_udp(&lsock,&fsock,up->tos,120,mr1,0,0,0);
#ifdef DEBUG_ACT
	prnt_string("so_udp_send:");
	prnt_long(fsock.address);prnt_char('-');
	prnt_int(fsock.port);prnt_char('-');
	prnt_long(lsock.address);prnt_char('-');
	prnt_int(lsock.port);prnt_char(CR);
#endif

	send_udp(&lsock,&fsock,4,121,mr1,0,0,0);
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp_qlen(USOCK_STRUCT *up,int rtx){

	int len;

	switch(rtx){
	  case 0:
		len = ucb_st[up->cb.ucb_no].rcvcnt;
		break;
	  case 1:
		len = 0;
		break;
//alaki
	  default	:
		prnt_msg(UDP_ERR,0x2503);
		return 0;
	}
	return len;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp_close(USOCK_STRUCT *up){

	if(up->cb.ucb_no < MAX_UCB){
		del_udp(up->cb.ucb_no);
	}
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp_shut(USOCK_STRUCT *up,int how _U_){

	int s;

	s = up->index;
	close_s_udp(s);
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
void s_urcall(unsigned int iface_no _U_,unsigned int ucb_no _U_,int cnt _U_){

//	ksignal(itop(udp->user),1);
//	kwait(NULL);
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int so_udp_stat(USOCK_STRUCT *up _U_){

#if 0
	st_udp(up->cb.ucb_no,0);
#endif // 0
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Create a UDP control block for lsocket, so that we can queue 	    */
/* incoming datagrams.							    */
/* ======================================================================== */
#ifdef UDP_INSTALLED
unsigned int open_udp(SOCKET_STRUCT *lsocket,void (*r_upcall)() _U_){

	UCB_STRUCT *ucb_pt;
	unsigned int ucb_no;

	if((ucb_no = lookup_ucb(lsocket)) < MAX_UCB){
		/* Already exists */
		Net_error = NET_ERROR_CON_EXISTS;
		prnt_msg(UDP_ERR,0x2504);
		return NULL_UCB;
	}
//	up = (struct udp_cb *)callocw(1,sizeof (struct udp_cb));
	ucb_no = find_free_ucb_st();
	if(ucb_no >= MAX_UCB){
		prnt_msg(UDP_ERR,0x2505);
		return NULL_UCB;
	}
#ifdef DEBUG_ACT
	prnt_string("open_udp:");
	prnt_int(ucb_no);prnt_char(CR);
#endif
	ucb_pt=&ucb_st[ucb_no];
	ucb_pt->state=IN_USE;
	ucb_pt->socket.address = lsocket->address;
	ucb_pt->socket.port = lsocket->port;
//	ucb_pt->r_upcall = r_upcall;

	ucb_pt->next = Udps;
	Udps = ucb_no;
#ifdef DEBUG_ACT
/*
	prnt_int(ucb_no);prnt_char('-');
	prnt_int(ucb_st[ucb_no].next);prnt_char('-');
	prnt_int(Udps);prnt_char(CR);
*/
#endif
	return ucb_no;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Send a UDP datagram							    */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int send_udp(
SOCKET_STRUCT *lsocket, 	/* Source socket */
SOCKET_STRUCT *fsocket, 	/* Destination socket */
guint8 tos,			/* Type-of-service for IP */
guint8 ttl,			/* Time-to-live for IP */
WORD_MR mr1,			/* Data field, if any */
guint length,			/* Length of data field */
guint id,			/* Optional ID field for IP */
char df 			/* Don't Fragment flag for IP */
){
	PSEUDO_HEADER_STRUCT ph;
	UDP_STRUCT udp;
	gint32 laddr;

#ifdef DEBUG_ACT
	prnt_string("send_udp:\r\n");
	prnt_long(lsocket->address);prnt_char('-');
	prnt_int(lsocket->port);prnt_char('-');
	prnt_long(fsocket->address);prnt_char('-');
	prnt_int(fsocket->port);prnt_char('-');
	prnt_char(CR);
	print_mr(mr1);
#endif
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2523);
		return -1;
	}
	if(length != 0){
		trim_mbuf(&mr1,length);
	}
	else{
		length = mem_rec[mr1].size;
	}
//???
//football : uncommeted
	length += UDPHDR;

	laddr = lsocket->address;
	if(laddr == INADDR_ANY)
		laddr = locaddr(fsocket->address);

	udp.source = lsocket->port;
	udp.dest = fsocket->port;
	udp.length = length;

	/* Create IP pseudo-header, compute checksum and send it */
	ph.length = length;
	ph.source = laddr;
	ph.dest = fsocket->address;
	ph.protocol = UDP_PTCL;

	htonudp(&udp,&mr1,&ph);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2521);
		return 0;
	}
	Udp_mib[udpOutDatagrams].value++;
#ifdef DEBUG_ACT
	prnt_string("send_udp:ip_send:\r\n");
	prnt_long(laddr);prnt_char('-');
	prnt_long(fsocket->address);prnt_char(CR);
	print_mr(mr1);
#endif
	ip_send(laddr,fsocket->address,UDP_PTCL,tos,ttl,mr1,id,df);
	return (int)length;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Accept a waiting datagram, if available. Returns length of datagram */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int recv_udp(
unsigned int ucb_no,
SOCKET_STRUCT *fsocket,     /* Place to stash incoming socket */
WORD_MR *mr3		    /* Place to stash data packet */
){
	SOCKET_STRUCT sp;
	WORD_MR mr1;
	guint length;
	UCB_STRUCT *ucb_pt;

	mr1=*mr3;
	if(ucb_no >= MAX_UCB){
		Net_error = NET_ERROR_NO_CONN;
		prnt_msg(UDP_ERR,0x2506);
		return -1;
	}
	ucb_pt=&ucb_st[ucb_no];
	if(ucb_pt->rcvcnt == 0){
		Net_error = NET_ERROR_WOULDBLK;
		return -1;
	}
#ifdef DEBUG_ACT
/*
	prnt_string("recv_udp\r\n");
	prnt_int(ucb_no);prnt_char(CR);
*/
#endif
//	buf = dequeue(&ucb_pt->rcvq);
	mr1=dequeue(&ucb_pt->rcvq);
	ucb_pt->rcvcnt--;

	/* Strip socket header */
//	pullup(&buf,&sp,sizeof(struct socket));
	mr1=my_pullup_copy_data(mr1,(guint8 *)&sp,sizeof(SOCKET_STRUCT));
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2525);
		return -1;
	}

	/* Fill in the user's foreign socket structure, if given */
	if(fsocket != NULL){
		fsocket->address = sp.address;
		fsocket->port = sp.port;
	}
#ifdef DEBUG_ACT
/*
	prnt_int(fsocket->address);prnt_char('-');
	prnt_int(fsocket->port);prnt_char(CR);
*/
#endif
	/* Hand data to user */
#if 0
	length = len_p(buf);
	if(bp != NULL)
		*bp = buf;
	else
		free_p(&buf);
#endif // 0
	length=mem_rec[mr1].size;
	*mr3=mr1;
	return (int)length;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
void udp_recv_hndlr(void){

	WORD_MR mr1;
	unsigned int ucb_no;
	int cnt;
	SOCKET_STRUCT fsocket;
	for(ucb_no=Udps;ucb_no<MAX_UCB;ucb_no=ucb_st[ucb_no].next){
again:
		cnt = recv_udp(ucb_no,&fsocket,&mr1);
		if(cnt == -1){
			continue;
		}
#ifdef DEBUG_ACT
		prnt_string("udp_recv_hndlr:\r\n");
		prnt_long(fsocket.address);prnt_char('-');
		prnt_int(fsocket.port);prnt_char('-');
		prnt_int(ucb_no);prnt_char('-');
		prnt_char(CR);
		print_mr(mr1);
#endif
//#if (((defined KIATEL_UDP_FOR_OAM) || (defined TFTPD_INSTALLED)) && (defined SYSTEM_SIDE))
#if (defined KIATEL_UDP_INSTALLED)
		{
		    guint kt_udp_no;

#ifdef SYSTEM_SIDE
		    kt_udp_no=lookup_kiatel_udp_hash_table(fsocket.address, fsocket.port, ucb_st[ucb_no].socket.port);
#endif
#ifdef OPERATOR_SIDE
//		    if(kt_udp_rec[0].local_port == ucb_st[ucb_no].socket.port)
		    if(kt_udp_rec[0].remote_address == fsocket.address)
			kt_udp_no=0;
		    else
//		    if(kt_udp_rec[1].local_port == ucb_st[ucb_no].socket.port)
		    if(kt_udp_rec[1].remote_address == fsocket.address)
			kt_udp_no=1;
		    else
			kt_udp_no=(guint)(-1);
#endif
		    if(kt_udp_no < MAX_KIATEL_UDP_REC){

			KIATEL_UDP_STRUCT *kt_udp_pt;
			guint16 size;

			kt_udp_pt=&kt_udp_rec[kt_udp_no];
			switch(kt_udp_pt->upper_layer_type){
#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
			  case KIATEL_UDP_UPPER_LAYER_WIN_OAM_TERMINAL_TYPE	   :
			    size=*((guint16 *)(&mem_rec[mr1].parm[0]));
			    if(size > (mem_rec[mr1].size-2)){
				    // to be completed.
				    // more than one packet, save incomplete packet , but
				    // dont' write it to buffer now.
				    prnt_msg(UDP_ERR,0x2534);
			    }
			    else if(size < (mem_rec[mr1].size-2)){
				    // to be completed.
				    // less than one packet, save incomplete packet , but
				    // dont' write it to buffer now.
				    prnt_msg(UDP_ERR,0x2535);
			    }
			    if (put_pckt_to_variable_size_buf(&(kt_udp_pt->netrx_st), size, &(mem_rec[mr1].parm[2])) == FAIL){
				    prnt_msg(UDP_ERR,0x2530);
			    }
			    mem_rec_rls(mr1);
			    break;
#endif
#if ((defined SYSCONUDP) && (defined SYSTEM_SIDE))
			  case KIATEL_UDP_UPPER_LAYER_SYSCONUDP_TYPE	    :
			    size=*((guint16 *)(&mem_rec[mr1].parm[0]));
			    if(size > (mem_rec[mr1].size-2)){
				    // to be completed.
				    // more than one packet, save incomplete packet , but
				    // dont' write it to buffer now.
				    prnt_msg(UDP_ERR,0x2534);
			    }
			    else if(size < (mem_rec[mr1].size-2)){
				    // to be completed.
				    // less than one packet, save incomplete packet , but
				    // dont' write it to buffer now.
				    prnt_msg(UDP_ERR,0x2535);
			    }
			    if (put_pckt_to_variable_size_buf(&(kt_udp_pt->netrx_st), size, &(mem_rec[mr1].parm[2])) == FAIL){
				    prnt_msg(UDP_ERR,0x2530);
			    }
			    mem_rec_rls(mr1);
			    break;
#endif
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
			  case KIATEL_UDP_UPPER_LAYER_OUR_OAM_TERMINAL_TYPE	   :
			  case KIATEL_UDP_UPPER_LAYER_HTBY_TYPE        :
			    {
				unsigned int n=0;
#if (defined MY_UDP_FOR_OAM)
				if(kt_udp_pt->upper_layer_type == KIATEL_UDP_UPPER_LAYER_OUR_OAM_TERMINAL_TYPE)
				    n=FIRST_UDP_HDLC_CH+kt_udp_pt->upper_layer_no; // oam_no+
#endif
#if((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE))
				if(kt_udp_pt->upper_layer_type == KIATEL_UDP_UPPER_LAYER_HTBY_TYPE)
				    n= HTBY_HDLC_CH;
#endif
				size=mem_rec[mr1].parm[0];
				if ( checkfree_buf(&hwrx_st[n]) >= (size+1) ){
				    unsigned int j;
				    put_buf(&hwrx_st[n],size); /* packet size */
				    for(j=1;j<size+1;j++)
					put_buf(&hwrx_st[n],mem_rec[mr1].parm[j]);
				}
				else
				    prnt_msg(UDP_ERR,0x2529);
			    }
#ifdef DEBUG_ACT
			    prnt_string("recv_udp:");
			    mem_rec[mr1].size=size+1;
			    print_mr(mr1);
#endif
			    mem_rec_rls(mr1);
			    break;
#endif
#if ((defined TFTPD_INSTALLED) && (defined SYSTEM_SIDE))
			  case KIATEL_UDP_UPPER_LAYER_TFTPD_TYPE  :
			    tftpd_recv2_rtn(kt_udp_pt->upper_layer_no, mr1);
			    break;
#endif
			  default :
			    prnt_msg(UDP_ERR,0x2537);
			    break;
			}
			goto again;
		    }
		}

#endif
		switch(ucb_st[ucb_no].socket.port){
#ifdef H225_GATEKEEPER_SIDE
		  case GKPR_UDP_PORT_RAS1    :
#ifdef DEBUG_ACT
			prnt_string("UDP_PORT_RAS1:\r\n");
			prnt_int(ucb_no);prnt_char('-');
			prnt_int(ucb_st[ucb_no].socket.port);prnt_char('-');
			prnt_int(fsocket.port);prnt_char(CR);
#endif
//			break;
		  case GKPR_UDP_PORT_RAS2    :
#ifdef DEBUG_ACT
			prnt_string("UDP_PORT_RAS2:\r\n");
			prnt_int(ucb_no);prnt_char('-');
			prnt_int(ucb_st[ucb_no].socket.port);prnt_char('-');
			prnt_int(fsocket.port);prnt_char(CR);
//			prnt_string("UDP data\r\n");
//			good_print(mem_rec[mr1].parm,mem_rec[mr1].size);
#endif
			my_ras2_rtn(ucb_no,mr1,fsocket);
			break;
#endif
// or checking fsocket if is 1718 or 1719??????

#ifdef H225_GATEWAY_SIDE
		  case GW_UDP_PORT_RAS	  :
#ifdef DEBUG_ACT
			  prnt_string("UDP_PORT_RAS:\r\n");
			  prnt_int(ucb_no);prnt_char('-');
			  prnt_int(ucb_st[ucb_no].socket.port);prnt_char('-');
			  prnt_int(fsocket.port);prnt_char(CR);
//			  prnt_string("UDP data\r\n");
//			  good_print(mem_rec[mr1].parm,mem_rec[mr1].size);
#endif
			my_ras2_rtn(ucb_no,mr1,fsocket);
			break;
#endif

/*
#if ((defined MY_UDP_FOR_OAM) || (defined HTBY_LINK_ON_MY_UDP))
		  case 3490:
		  case 3491:
		  case 3495:
		    {
			unsigned int n=0,size,j;

#ifdef MY_UDP_FOR_OAM
			if(ucb_st[ucb_no].socket.port == 3490)
				n=FIRST_UDP_HDLC_CH;
			if(ucb_st[ucb_no].socket.port == 3491)
				n=FIRST_UDP_HDLC_CH+1;
#endif
#ifdef HTBY_LINK_ON_MY_UDP
			if(ucb_st[ucb_no].socket.port == 3495)
				n=HTBY_HDLC_CH;
#endif
#ifdef DEBUG_ACT
			prnt_string("UDP_PORT OAM:\r\n");
			prnt_int(n);prnt_char(CR);
			print_mr(mr1);
#endif
			size=mem_rec[mr1].parm[0];
			if ( checkfree_buf(&hwrx_st[n]) >= (size+1) ){
				put_buf(&hwrx_st[n],size); // packet size
				for(j=1;j<size+1;j++)
				    put_buf(&hwrx_st[n],mem_rec[mr1].parm[j]);
			}
			else
			    prnt_msg(UDP_ERR,0x2529);
#ifdef DEBUG_ACT
			prnt_string("recv_udp:");
			mem_rec[mr1].size=size+1;
			print_mr(mr1);
#endif
			mem_rec_rls(mr1);
			break;
		    }
#endif
*/

#if ((defined TFTPD_INSTALLED) && (defined SYSTEM_SIDE))
		case TFTPD_UDP_PORT :
			tftpd_recv1_rtn(mr1, fsocket);
			break;
#endif
		default 	:
			  prnt_msg(UDP_ERR,0x2520);
			  prnt_int(ucb_st[ucb_no].socket.port);prnt_char('-');
			  prnt_int(fsocket.port);prnt_char(CR);
			  mem_rec_rls(mr1);
			  break;
		}
/*
		if( (myfile=fopen("datfile","a+b")) == NULL){
			prnt_string(\r\n"cannot create file!\r\n");
			exit(0);
		}
		if(mr1 < NO_MEM_REC){
			cnt=mem_rec[mr1].size;
			fprintf(myfile,"\nUDP len=%d\n",cnt);
			for(i=0;i<cnt;i++){
				fprintf(myfile,"\ndat[%04d]=0x%02x;",i,mem_rec[mr1].parm[i]);
			}
			fprintf(myfile,"\n");
			mem_rec_rls(mr1);
		}
		fclose(myfile);
*/
		goto again;
	}
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Delete a UDP control block						    */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int del_udp(unsigned int this_ucb){

	WORD_MR mr1;
	UCB_STRUCT *ucb_pt;
	unsigned int last_ucb_no=NULL_UCB;
	unsigned int ucb_no;

	for(ucb_no=Udps;ucb_no<MAX_UCB;last_ucb_no=ucb_no,ucb_no=ucb_st[ucb_no].next){
		if(ucb_no == this_ucb)
			break;
	}
	if(ucb_no >= MAX_UCB){
		/* Either conn was NULL or not found on list */
		Net_error = NET_ERROR_INVALID;
		prnt_msg(UDP_ERR,0x2508);
		return -1;
	}
	ucb_pt=&ucb_st[ucb_no];
	/* Get rid of any pending packets */
#if 0
	while(up->rcvcnt != 0){
		bp = up->rcvq;
		up->rcvq = up->rcvq->anext;
		free_p(&bp);
		up->rcvcnt--;
	}
#endif // 0
	while(ucb_pt->rcvcnt != 0){
		mr1=ucb_pt->rcvq;
		if(mr1 < NO_MEM_REC){
			ucb_pt->rcvq = mem_rec[ucb_pt->rcvq].rp;
			mem_rec_rls(mr1);
		}
		ucb_pt->rcvcnt--;
	}
	/* Remove from list */
	if(last_ucb_no < MAX_UCB)
		ucb_st[last_ucb_no].next=ucb_st[ucb_no].next;
	else
		Udps=ucb_st[ucb_no].next;	 /* was first on list */
	rls_ucb_st(ucb_no);
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Process an incoming UDP datagram					    */
/* ======================================================================== */
#ifdef UDP_INSTALLED
void udp_input(
unsigned int iface_no,	/* Input interface */
IP_STRUCT *ip,		/* IP header */
WORD_MR mr1,		/* UDP header and data */
int rxbroadcast,	/* The only protocol that accepts 'em */
gint32 said _U_
){
	PSEUDO_HEADER_STRUCT ph;
	UDP_STRUCT udp;
	UCB_STRUCT *ucb_pt;
	SOCKET_STRUCT lsocket;
	SOCKET_STRUCT fsocket;
	unsigned int length;
	IFACE_STRUCT *iface_pt;
	unsigned int ucb_no;

#ifdef DEBUG_ACT
	prnt_string("udp_input\r\n");
	print_mr(mr1);
#endif
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2509);
		return;
	}
	if(iface_no >= MAX_IFACE){
		prnt_msg(UDP_ERR,0x2510);
		return;
	}
	iface_pt=&iface_st[iface_no];
	/* Create pseudo-header and verify checksum */
	ph.source = ip->source;
	ph.dest = ip->dest;
	ph.protocol = ip->protocol;
	length = ip->length - IPLEN - ip->optlen;
	ph.length = length;

	/* Peek at header checksum before we extract the header. This	*/
	/* allows us to bypass cksum() if the checksum field was not	*/
	/* set by the sender.						*/

	udp.checksum = udpcksum(mr1);
	if((udp.checksum != 0) && (cksum(&ph,mr1,length) != 0)){
		/* Checksum non-zero, and wrong */
		Udp_mib[udpInErrors].value++;
		mem_rec_rls(mr1);
		prnt_msg(UDP_ERR,0x2511);
		return;
	}
	/* Extract UDP header in host order */
	if(ntohudp(&udp,&mr1) != 0){
		/* Truncated header */
		Udp_mib[udpInErrors].value++;
		mem_rec_rls(mr1);
		prnt_msg(UDP_ERR,0x2512);
		return;
	}
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2526);
		return;
	}
	/* If this was a broadcast packet, pretend it was sent to us */
	if(rxbroadcast){
		lsocket.address = iface_pt->addr;
	} else
		lsocket.address = ip->dest;

	lsocket.port = udp.dest;
	/* See if there's somebody around to read it */
	if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
err:
/*
		prnt_msg(UDP_ERR,0x2513);
		prnt_long(lsocket.address);
		prnt_char(CR);
		prnt_int(lsocket.port);prnt_char(CR);
//		print_mr(mr1);
*/
		/* Nope, return an ICMP message */
		if(!rxbroadcast){
			htonudp(&udp,&mr1,&ph);
			if(mr1 >= NO_MEM_REC){
				prnt_msg(UDP_ERR,0x2527);
				return;
			}
			icmp_output(ip,mr1,ICMP_DEST_UNREACH,ICMP_PORT_UNREACH,NULL);
			mr1=NULL_MEM_REC;
		}
		Udp_mib[udpNoPorts].value++;
		if(mr1 < NO_MEM_REC){
			mem_rec_rls(mr1);
		}
		return;
	}
	/* Prepend the foreign socket info */
	fsocket.address = ip->source;
	fsocket.port = udp.source;
	ucb_pt=&ucb_st[ucb_no];
//	pushdown(bpp,&fsocket,sizeof(fsocket));
	mr1=my_pushdown_copy_data(mr1,(guint8 *)&fsocket,sizeof(fsocket));
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2528);
		return;
	}

	/* Queue it */
//	enqueue(&up->rcvq,bpp);
	enqueue(&ucb_pt->rcvq,mr1);
	ucb_pt->rcvcnt++;
	Udp_mib[udpInDatagrams].value++;
#if 0
	if(ucb_pt->r_upcall)
		(*ucb_pt->r_upcall)(iface_no,ucb_no,ucb_pt->rcvcnt);
#endif // 0
//	s_urcall(iface_no,ucb_no,ucb_pt->rcvcnt);
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Look up UDP socket.							    */
/* Return control block pointer or NULL if nonexistant			    */
/* As side effect, move control block to top of list to speed future	    */
/* searches.								    */
/* ======================================================================== */
#ifdef UDP_INSTALLED
unsigned int lookup_ucb(SOCKET_STRUCT *socket){

	unsigned int ucb_no;
	unsigned int last_ucb_no=NULL_UCB;

#ifdef DEBUG_ACT
	prnt_string("lookup_ucb\r\n");
	prnt_long(socket->address);prnt_char('-');
	prnt_int(socket->port);prnt_char('-');
	prnt_char(CR);
#endif

	for(ucb_no=Udps;ucb_no<MAX_UCB;last_ucb_no=ucb_no,ucb_no=ucb_st[ucb_no].next){
		if((socket->port == ucb_st[ucb_no].socket.port) &&
		   ((socket->address == ucb_st[ucb_no].socket.address) ||
		    (ucb_st[ucb_no].socket.address == INADDR_ANY))
		){
			if(last_ucb_no < MAX_UCB){
				/* Move to top of list */
				ucb_st[last_ucb_no].next=ucb_st[ucb_no].next;
				ucb_st[ucb_no].next = Udps;
				Udps = ucb_no;
			}
			return ucb_no;
		}
	}
	return NULL_UCB;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Attempt to reclaim unused space in UDP receive queues */
/* ======================================================================== */
#ifdef UDP_INSTALLED
void
udp_garbage(int red _U_)
{
#if 0
	register struct udp_cb *udp;

	for(udp = Udps;udp != NULL; udp = udp->next){
		mbuf_crunch(&udp->rcvq);
	}
#endif // 0
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Convert UDP header in internal format to an mbuf in external format */
/* ======================================================================== */
#ifdef UDP_INSTALLED
void htonudp(
UDP_STRUCT *udp_pt,
WORD_MR *mr3,
PSEUDO_HEADER_STRUCT *ph
){
	guint8 *cp;
	guint checksum;
	WORD_MR mr1;

	mr1=*mr3;
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2524);
		*mr3=NULL_MEM_REC;
		return;
	}
	/* Allocate UDP protocol header and fill it in */
//	pushdown(bpp,NULL,UDPHDR);
	mr1=my_pushdown(mr1,UDPHDR);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2521);
		*mr3=NULL_MEM_REC;
		return;
	}
//	cp = (*bpp)->data;
	cp = mem_rec[mr1].parm;
	cp = put16(cp,udp_pt->source);	   /* Source port */
	cp = put16(cp,udp_pt->dest);	   /* Destination port */
	cp = put16(cp,udp_pt->length);	   /* Length */
	*cp++ = 0;			   /* Clear checksum */
	*cp-- = 0;

	/* All zeros and all ones is equivalent in one's complement arithmetic;
	 * the spec requires us to change zeros into ones to distinguish an
	 * all-zero checksum from no checksum at all
	 */
	if((checksum = cksum(ph,mr1,ph->length)) == 0)
		checksum = 0xffff;
	put16(cp,checksum);
	*mr3=mr1;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Convert UDP header in mbuf to internal structure */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int
ntohudp(
UDP_STRUCT *udp_pt,
WORD_MR *mr3
){
	guint8 udpbuf[UDPHDR];
	WORD_MR mr1;

#if 0
	if(pullup(bpp,udpbuf,UDPHDR) != UDPHDR)
		return -1;
#endif // 0
	mr1=*mr3;
	mr1=my_pullup_copy_data(mr1,udpbuf,UDPHDR);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2514);
		*mr3=NULL_MEM_REC;
		return -1;
	}
	udp_pt->source = get16(&udpbuf[0]);
	udp_pt->dest = get16(&udpbuf[2]);
	udp_pt->length = get16(&udpbuf[4]);
	udp_pt->checksum = get16(&udpbuf[6]);
	*mr3=mr1;
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Extract UDP checksum value from a network-format header without	    */
/* disturbing the header						    */
/* ======================================================================== */
#ifdef UDP_INSTALLED
guint udpcksum(WORD_MR mr1){

#if 0
	WORD_MR mr2;

	mr2=mem_rec_req(2);
	if(mr2 >= NO_MEM_REC){
		prnt_msg(UDP_ERR,0x2515);
		return 0;
	}
	mem_cpy(mem_rec[mr2].parm,&(mem_rec[mr1].parm[6]),2);
//	if(dup_p(&dup,bp,6,2) != 2)
//		return 0;
	return pull16(&mr2);
#endif // 0
	return(mem_rec[mr1].parm[6] | (mem_rec[mr1].parm[7] << 8));
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* Close a socket, freeing it for reuse. Try to do a graceful close on a    */
/* TCP socket, if possible						    */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int close_s_udp(int s){ 	/* Socket index */

	USOCK_STRUCT *usock_pt;

	if((unsigned int)s >= Nsock){
		errno = EBADF_ERROR;
		prnt_msg(UDP_ERR,0x2516);
		return -1;
	}
	usock_pt=&usock_st[s];
	if(--usock_pt->refcnt > 0)
		return 0;	/* Others are still using it */
	so_udp_close(usock_pt);
	rls_usock_st(s);
	return 0;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
void init_ucb_st(void){

	unsigned int i;
	UCB_STRUCT *ucb_pt;

	for(i=0,ucb_pt=&ucb_st[0];i < MAX_UCB;i++,ucb_pt++){
		ucb_pt->state=IN_QUE;
		ucb_pt->next=i+1;
		ucb_pt->prev=i-1;
//		ucb_pt->socket=
		ucb_pt->rcvq=NULL_MEM_REC;
		ucb_pt->rcvcnt=0;
		ucb_pt->user=NULL_USOCK;
		ucb_pt->ucb_no=i;
	}
	fq_ucb_st=0;
	eq_ucb_st=MAX_UCB-1;
	ucb_st[MAX_UCB-1].next=NULL_UCB;
	ucb_st[0].prev=NULL_UCB;
	return;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
unsigned int find_free_ucb_st(void){

	unsigned int ucb_no;
	UCB_STRUCT *ucb_pt;

	if(fq_ucb_st >= MAX_UCB){
		prnt_msg(UDP_ERR,0x2517);
		return NULL_UCB;
	}
	ucb_no=fq_ucb_st;
	ucb_pt=&ucb_st[ucb_no];
	fq_ucb_st=ucb_pt->next;
	if(fq_ucb_st < MAX_UCB)
		ucb_st[fq_ucb_st].prev=NULL_UCB;
	else
		eq_ucb_st=NULL_UCB;
	ucb_pt->next=NULL_UCB;
	ucb_pt->state=OUT_OF_QUE;
	return ucb_no;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
void rls_ucb_st(unsigned int ucb_no){

	UCB_STRUCT *ucb_pt;
	if(ucb_no >= MAX_UCB){
		prnt_msg(UDP_ERR,0x2518);
		prnt_int(ucb_no);prnt_char(CR);
		return;
	}
	ucb_pt=&ucb_st[ucb_no];
	if(ucb_pt->state == IN_QUE){
		prnt_msg(UDP_ERR,0x2519);
		prnt_int(ucb_no);prnt_char(CR);
		return;
	}
	ucb_pt->state=IN_QUE;
//	ucb_pt->socket=
	ucb_pt->rcvq=NULL_MEM_REC;
	ucb_pt->rcvcnt=0;
//
	ucb_pt->user=NULL_USOCK;
//	ucb_pt->ucb_no
	ucb_pt->prev=eq_ucb_st;
	if(eq_ucb_st < MAX_UCB)
		ucb_st[eq_ucb_st].next=ucb_no;
	else
		fq_ucb_st=ucb_no;
	eq_ucb_st=ucb_no;
	ucb_pt->next=NULL_UCB;
	return;
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef UDP_INSTALLED
int xmit_udp(unsigned int ucb_no,SOCKET_STRUCT *fsocket_pt,WORD_MR mr1){

	SOCKADDR_IN_STRUCT to;
	unsigned int s;
	USOCK_STRUCT *up;
#ifdef DEBUG_ACT
	prnt_string("xmit_udp:");
	prnt_int(ucb_no);prnt_char(CR);
	prnt_long(fsocket_pt->address);prnt_char('-');
	prnt_int(fsocket_pt->port);prnt_char('-');
	prnt_long(ucb_st[ucb_no].socket.address);prnt_char('-');
	prnt_int(ucb_st[ucb_no].socket.port);prnt_char(CR);
	print_mr(mr1);
#endif
	s=ucb_st[ucb_no].user;
	up=&usock_st[s];
#ifdef DEBUG_ACT
	prnt_int(s);prnt_char(CR);
#endif
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = fsocket_pt->address;
	to.sin_port	   = fsocket_pt->port;

	return(so_udp_send(up,mr1,(SOCKADDR_STRUCT*)&to));
}
#endif // UDP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef H225_INSTALLED
void my_ras2_rtn(unsigned int ucb_no,unsigned int mr1,SOCKET_STRUCT fsocket){

	unsigned char **dato;
	unsigned char *dati;

	dati=mem_rec[mr1].parm;

#ifdef DEBUG_ACT
/*
	prnt_string("Ras data:\r\n");
	good_print(dati,mem_rec[mr1].size);
*/
#endif
	dato=(unsigned char **)kt_g_malloc(POINTER_SIZE);
	*dato=NULL_POINTER;
	dissect_h225_RasMessage(dati,dato);
#ifdef H225_GATEKEEPER_SIDE
	if(dato != NULL_POINTER){
		to_h225_ras_q(ucb_no,fsocket,dato);
	}
#endif
#ifdef H225_GATEWAY_SIDE
	if(dato != NULL_POINTER){
		send_from_h225_udp_to_ras_sdl(ucb_no,fsocket,dato);
	}
#endif
	mem_rec_rls(mr1);
}
#endif // H225_INSTALLED
