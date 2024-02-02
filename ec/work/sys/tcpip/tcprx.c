
/*************	Time-stamp: <2010-02-25 10:45:39 savoj>   ************/
// ERROR : 0x2300-0x2321

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
/* TCP receive upcall routine						    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void s_trcall(unsigned int tcb_no _U_,guint32 cnt _U_){
#ifdef DEBUG_ACT
	prnt_string("s_trcall:");prnt_int(tcb_no);toprnq(CR);
#endif
	switch(tcb_st[tcb_no].type){
#ifdef KFTPD_INSTALLED
	case KFTPD_DATA_TCP_TYPE:
	{
		WORD_MR mr1;
		long result;

		result=recv_tcp(tcb_no,&mr1,0);
		if (result == 0){
			break;
		}
		if (result == -1){
			prnt_msg(TCP_ERR,0x2314);
			prnt_int(tcb_no);toprnq('-');
			prnt_long(cnt);toprnq(CR);
			if(tcb_st[tcb_no].rec_no < MAX_KFTPD_REC){
				to_aux_kftpd_q(tcb_st[tcb_no].rec_no, AUX_KFTPD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
			}
			break;
		}
		if(tcb_st[tcb_no].rec_no < MAX_KFTPD_REC){
			to_aux_kftpd_q(tcb_st[tcb_no].rec_no, AUX_KFTPD_L2_TCP_DATA_READY_STIM, mr1);
		}
		if(tcb_st[tcb_no].my_reset_flag == SET){
			tcp_output(tcb_no);
		}
		break;
	}
	case KFTPD_CONTROL_TCP_TYPE:
	{
		WORD_MR mr1;
		long result;

		result=recv_tcp(tcb_no,&mr1,0);
		if (result == 0){
			break;
		}
		if (result == -1){
			prnt_msg(TCP_ERR,0x2315);
			prnt_int(tcb_no);toprnq('-');
			prnt_long(cnt);toprnq(CR);
			if(tcb_st[tcb_no].rec_no < MAX_KFTPD_REC){
				to_kftpd_q(tcb_st[tcb_no].rec_no, KFTPD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
			}
			break;
		}
		kftpd_control_port_analysis(tcb_no, mr1);
		if(tcb_st[tcb_no].my_reset_flag == SET){
			tcp_output(tcb_no);
		}
		break;
	}
#endif
#ifdef KFTP_INSTALLED
	case KFTP_DATA_TCP_TYPE:
	{
		WORD_MR mr1;
		long result;

		result=recv_tcp(tcb_no,&mr1,0);
		if (result == 0){
			break;
		}
		if (result == -1){
			prnt_msg(TCP_ERR,0x2316);
			prnt_int(tcb_no);toprnq('-');
			prnt_long(cnt);toprnq(CR);
			if(tcb_st[tcb_no].rec_no < MAX_KFTP_REC){
				to_aux_kftp_q(tcb_st[tcb_no].rec_no, AUX_KFTP_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
			}
			break;
		}
		if(tcb_st[tcb_no].rec_no < MAX_KFTP_REC){
			to_aux_kftp_q(tcb_st[tcb_no].rec_no, AUX_KFTP_L2_TCP_DATA_READY_STIM, mr1);
		}
		if(tcb_st[tcb_no].my_reset_flag == SET){
			tcp_output(tcb_no);
		}
		break;
	}
	case KFTP_CONTROL_TCP_TYPE:
	{
		WORD_MR mr1;
		long result;

		result=recv_tcp(tcb_no,&mr1,0);
		if (result == 0){
			break;
		}
		if (result == -1){
			prnt_msg(TCP_ERR,0x2317);
			prnt_int(tcb_no);toprnq('-');
			prnt_long(cnt);toprnq(CR);
			if(tcb_st[tcb_no].rec_no < MAX_KFTP_REC){
				to_kftp_q(tcb_st[tcb_no].rec_no, KFTP_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
			}
			break;
		}
		kftp_control_port_analysis(tcb_no, mr1);
		if(tcb_st[tcb_no].my_reset_flag == SET){
			tcp_output(tcb_no);
		}
		break;
	}
#endif
#ifdef ROS_FOR_LI_INSTALLED
	case LI_ROS_TCP_TYPE:
	{
		WORD_MR mr1;
		long result;

		result=recv_tcp(tcb_no,&mr1,0);
		if (result == 0){
			break;
		}
		if (result == -1){
			prnt_msg(TCP_ERR,0x2318);
			prnt_int(tcb_no);toprnq('-');
			prnt_long(cnt);toprnq(CR);
			if(tcb_st[tcb_no].rec_no < MAX_LI_XPORT_REC){
				to_li_xport_que(tcb_st[tcb_no].rec_no, LI_XPORT_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
			}
			break;
		}
		to_li_xport_que(tcb_st[tcb_no].rec_no, LI_XPORT_L2_TCP_DATA_READY_STIM, mr1);
		if(tcb_st[tcb_no].my_reset_flag == SET){
			tcp_output(tcb_no);
		}
		break;
	}

#endif
#ifdef TELNETD_INSTALLED
	case TELNETD_TCP_TYPE		    :
	{
		WORD_MR mr1;
		long result;

		result=recv_tcp(tcb_no,&mr1,0);
		if (result == 0){
//PEACEPEACE start
			goto check_flag;
//			break;
//PEACEPEACE end
		}
		if (result == -1){
			prnt_msg(TCP_ERR,0x2319);
			prnt_int(tcb_no);toprnq('-');
			prnt_long(cnt);toprnq(CR);
			if(tcb_st[tcb_no].rec_no < MAX_TELNETD_REC){
				to_telnetd_q(tcb_st[tcb_no].rec_no, TELNETD_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
			}
			break;
		}
		telnetd_port_analysis(tcb_no, mr1);
//PEACEPEACE start
check_flag:
		// if we exit from telnet dos command by closing the window(without sending EXITTEL)
		// the computer only sends FIN to our system and tcp changes its state to TCP_CLOSE_WAIT
		// sending FIN means that remote party has not any data to send us anymore.
		// but local user(our system) should disconnect this link.
		if((tcb_st[tcb_no].rec_no < MAX_TELNETD_REC) && (tcb_st[tcb_no].state == TCP_CLOSE_WAIT)){
			to_telnetd_q(tcb_st[tcb_no].rec_no, TELNETD_L2_TCP_CLOSE_WAIT_STIM, NULL_MEM_REC);
			break;
		}
		if(tcb_st[tcb_no].my_reset_flag == SET){
			tcp_output(tcb_no);
			break;
		}
//PEACEPEACE end
		break;
	}
#endif
#ifdef HI1_INSTALLED
	case HI1_TCP_TYPE		:
	{
		WORD_MR mr1;
		long result;

		result=recv_tcp(tcb_no,&mr1,0);
		if (result == 0){
//PEACEPEACE start
			goto check_flag2;
//			break;
//PEACEPEACE end
		}
		if (result == -1){
			prnt_msg(TCP_ERR,0x2320);
			prnt_int(tcb_no);toprnq('-');
			prnt_long(cnt);toprnq(CR);
			if(tcb_st[tcb_no].rec_no < MAX_HI1_REC){
				to_hi1_q(tcb_st[tcb_no].rec_no, HI1_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
			}
			break;
		}
		hi1_port_analysis(tcb_no, mr1);
//PEACEPEACE start
check_flag2:
		// if we exit from telnet dos command by closing the window(without sending EXITTEL)
		// the computer only sends FIN to our system and tcp changes its state to TCP_CLOSE_WAIT
		// sending FIN means that remote party has not any data to send us anymore.
		// but local user(our system) should disconnect this link.
		if((tcb_st[tcb_no].rec_no < MAX_HI1_REC) && (tcb_st[tcb_no].state == TCP_CLOSE_WAIT)){
			to_hi1_q(tcb_st[tcb_no].rec_no, HI1_L2_TCP_CLOSE_WAIT_STIM, NULL_MEM_REC);
			break;
		}
		if(tcb_st[tcb_no].my_reset_flag == SET){
			tcp_output(tcb_no);
			break;
		}
//PEACEPEACE end
		break;
	}
#endif
#ifdef HI1_DISTRIBUTOR
	case HI1_DISTRIBUTOR_TCP_TYPE	:
	{
	    WORD_MR mr1;
	    long result;
	    result=recv_tcp(tcb_no,&mr1,0);
	    if (result == 0){
		goto check_flag3;
//		break;
	    }
	    if (result == -1){
		prnt_msg(TCP_ERR,0x2321);
		prnt_int(tcb_no);toprnq('-');
		prnt_long(cnt);toprnq(CR);
		if(tcb_st[tcb_no].rec_no < no_of_subscriber_groups){
		    //to_hi1_q(tcb_st[tcb_no].rec_no, HI1_L2_TCP_FAILURE_STIM, NULL_MEM_REC);
		}
		break;
	    }
	    hi1_distributor_tcp_port_analysis(tcb_no,mr1);
check_flag3:
	    // if we exit from telnet dos command by closing the window(without sending EXITTEL)
	    // the computer only sends FIN to our system and tcp changes its state to TCP_CLOSE_WAIT
	    // sending FIN means that remote party has not any data to send us anymore.
	    // but local user(our system) should disconnect this link.
	    if((tcb_st[tcb_no].rec_no < no_of_subscriber_groups) && (tcb_st[tcb_no].state == TCP_CLOSE_WAIT)){
		//to_hi1_q(tcb_st[tcb_no].rec_no, HI1_L2_TCP_CLOSE_WAIT_STIM, NULL_MEM_REC);
		break;
	    }
	    if(tcb_st[tcb_no].my_reset_flag == SET){
		tcp_output(tcb_no);
		break;
	    }
	    break;
	}
#endif
#ifdef SYSCON_SERVER
	case SYSCON_SERVER_TCP_TYPE	:
	{
	    WORD_MR mr1;
	    long result;

	    result=recv_tcp(tcb_no,&mr1,0);
	    if (result == 0){
		goto check_flag4;
	    }
	    if (result == -1){
		prnt_msg(TCP_ERR,0x2322);
		prnt_int(tcb_no);toprnq('-');
		prnt_long(cnt);toprnq(CR);
		if(tcb_st[tcb_no].rec_no < MAX_SYSCON_CLIENT){
		    guint syscon_client_no=tcb_st[tcb_no].rec_no;
		    SYSCON_SERVER_STRUCT *sssp=&syscon_server_st[syscon_client_no];
		    sssp->tcp_no=NULL_TCB;
		    do_syscon_server_finish(syscon_client_no);
		}
		break;
	    }
	    syscon_server_port_analysis(tcb_no, mr1);
check_flag4:
	    if((tcb_st[tcb_no].rec_no < MAX_SYSCON_CLIENT) && (tcb_st[tcb_no].state == TCP_CLOSE_WAIT)){
		guint syscon_client_no=tcb_st[tcb_no].rec_no;
		SYSCON_SERVER_STRUCT *sssp=&syscon_server_st[syscon_client_no];
		sssp->tcp_no=NULL_TCB;
		do_syscon_server_finish(syscon_client_no);
		break;
	    }
	    if(tcb_st[tcb_no].my_reset_flag == SET){
		tcp_output(tcb_no);
		break;
	    }
	    break;
	}
#endif
#ifdef SYSCON_CLIENT
	case SYSCON_CLIENT_TCP_TYPE   :
	{
	    WORD_MR mr1;
	    long result;
	    result=recv_tcp(tcb_no,&mr1,0);
	    if (result == 0){
		goto check_flag5;
	    }
	    if (result == -1){
		prnt_msg(TCP_ERR,0x2323);
		prnt_int(tcb_no);toprnq('-');
		prnt_long(cnt);toprnq(CR);
		if(tcb_st[tcb_no].rec_no < MAX_SYSCON_SERVER){
		    // L2_TCP_FAILURE
		}
		break;
	    }
	    syscon_client_port_analysis(tcb_no,mr1);
check_flag5:
	    if((tcb_st[tcb_no].rec_no < MAX_SYSCON_SERVER) && (tcb_st[tcb_no].state == TCP_CLOSE_WAIT)){
		// L2_TCP_CLOSE_WAIT
		break;
	    }
	    if(tcb_st[tcb_no].my_reset_flag == SET){
		tcp_output(tcb_no);
		break;
	    }
	    break;
	}
#endif
	default 		  :
		prnt_msg(TCP_ERR,0x2313);
		prnt_byte(tcb_st[tcb_no].type);prnt_char(CR);
		break;
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void so_tcp_recv_hndlr(void){
	WORD_MR mr1;
	unsigned int tcb_no;
	long cnt;

	for(tcb_no=Tcbs;tcb_no<MAX_TCB;tcb_no=tcb_st[tcb_no].next){
		if (tcb_st[tcb_no].trdiscard_flag == SET){
			continue;
		}
		if ((cnt = recv_tcp(tcb_no,&mr1,0)) == -1){
			continue;
		}
		if(cnt == 0){
			continue;
		}
		switch(tcb_st[tcb_no].type){
#ifdef Q931_FOR_H225
		    case H225_Q931_TCP_TYPE	  :
			h225_call_signaling_tcp_port_analysis(tcb_no,mr1);
			if(tcb_st[tcb_no].my_reset_flag == SET){
			    tcp_output(tcb_no);
			}
			continue;
#endif
#ifdef H245_GATEWAY_SIDE
		    case H245_TCP_TYPE		  :
			h245_tcp_port_analysis(tcb_no,mr1);
			if(tcb_st[tcb_no].my_reset_flag == SET){
			    tcp_output(tcb_no);
			}
			continue;
#endif
#ifdef MY_TCPIP_FOR_OAM
		    case OAM_TCP_TYPE		  :
			oam_tcp_port_analysis(tcb_no,mr1);
			if(tcb_st[tcb_no].my_reset_flag == SET){
			    tcp_output(tcb_no);
			}
			continue;
#endif
#ifdef KFTPD_INSTALLED
		    case KFTPD_CONTROL_TCP_TYPE 	  :
			continue;
		    case KFTPD_DATA_TCP_TYPE	  :
			continue;
#endif
/*
#ifdef KFTP_INSTALLED
		    case KFTP_CONTROL_TCP_TYPE	 :
			continue;
		    case KFTP_DATA_TCP_TYPE	 :
			continue;
#endif
*/
#ifdef TELNETD_INSTALLED
		    case TELNETD_TCP_TYPE	    :
			continue;
#endif
#ifdef HI1_INSTALLED
		    case HI1_TCP_TYPE		:
			continue;
#endif
#ifdef HI1_DISTRIBUTOR
		    case HI1_DISTRIBUTOR_TCP_TYPE	:
/* is not happening
			hi1_distributor_tcp_port_analysis(tcb_no,mr1);
			if(tcb_st[tcb_no].my_reset_flag == SET){
			    tcp_output(tcb_no);
			}
*/
			continue;
#endif
#ifdef SYSCON_SERVER
		    case SYSCON_SERVER_TCP_TYPE :
			continue;
#endif
#ifdef SYSCON_CLIENT
		    case SYSCON_CLIENT_TCP_TYPE :
// is not happening
			continue;
#endif
		    default			  :
			prnt_msg(TCP_ERR,0x2310);
			int2prnt(tcb_no);toprnq('-');
			byte2prnt(tcb_st[tcb_no].type);toprnq(CR);
			int2prnt(cnt);toprnq(CR);
			continue;
		}
/*
		if( (myfile=fopen("datfile","a+b")) == NULL){
			prnt_string("cannot create file!\r\n");
			exit(0);
		}
		if(mr1 < NO_MEM_REC){
			cnt=mem_rec[mr1].size;
			fprintf(myfile,"\nlen=%ld\n",cnt);
			for(i=0;i<cnt;i++){
				fprintf(myfile,"\ndat[%04d]=0x%02x;",i,mem_rec[mr1].parm[i]);
			}
			fprintf(myfile,"\n");
			mem_rec_rls(mr1);
		}
		fclose(myfile);
*/
	}
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int so_tcp_recv(USOCK_STRUCT *up,WORD_MR *mr3,SOCKADDR_STRUCT *from _U_, int *fromlen _U_){

	long cnt;
	unsigned int tcb_no;
	WORD_MR mr1;

	mr1=*mr3;
#if 0
	while(
	       ((tcb_no = up->cb.tcb_no) < MAX_TCB) &&
	       (tcb_st[tcb_no].trdiscard_flag != SET) &&
	       (cnt = recv_tcp(tcb_no,&mr1,0)) == -1){
		if(up->noblock){
			errno = EWOULDBLOCK_ERROR;
			return -1;
		}
//		else if((errno = kwait(up)) != 0){
//			return -1;
//		}
	}
#endif // 0
	tcb_no=up->cb.tcb_no;
	if(tcb_no >= MAX_TCB){
		/* Connection went away */
		errno = ENOTCONN_ERROR;
		prnt_msg(TCP_ERR,0x2300);
		return -1;
	}
	if (tcb_st[tcb_no].trdiscard_flag == SET){
		errno = ENOTCONN_ERROR; /* CHANGE */
		prnt_msg(TCP_ERR,0x2301);
		return -1;
	}
	if ((cnt = recv_tcp(tcb_no,&mr1,0)) == -1){
		if(up->noblock){
			errno = EWOULDBLOCK_ERROR;
		}
		return -1;
	}
#if 0
//	else if(tcb->r_upcall == trdiscard){
	else if(tcb_st[tcb_no].trdiscard_flag == SET){
		/* Receive shutdown has been done */
		errno = ENOTCONN_ERROR; /* CHANGE */
		return -1;
	}
#endif // 0
	*mr3=mr1;
	return cnt;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* User receive routine 						    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
long recv_tcp(
unsigned int tcb_no,
WORD_MR *mr3,
long cnt
){
	TCB_STRUCT *tcb_pt;
	WORD_MR mr1;
	if(tcb_no >= MAX_TCB){
		Net_error = NET_ERROR_INVALID;
		prnt_msg(TCP_ERR,0x2302);
		return -1;
	}
	mr1=*mr3;
	tcb_pt=&tcb_st[tcb_no];
	if(tcb_pt->rcvcnt == 0){
		/* If there's nothing on the queue, our action depends on what state */
		/* we're in (i.e., whether or not we're expecting any more data).    */
		/* If no more data is expected, then simply return 0; this is	     */
		/* interpreted as "end of file". Otherwise return -1.                */

		switch(tcb_pt->state){
		case TCP_LISTEN:
		case TCP_SYN_SENT:
		case TCP_SYN_RECEIVED:
		case TCP_ESTABLISHED:
		case TCP_FINWAIT1:
		case TCP_FINWAIT2:
			Net_error = NET_ERROR_WOULDBLK;
			mr1=NULL_MEM_REC;
			*mr3=mr1;
			return -1;
		case TCP_CLOSED:
		case TCP_CLOSE_WAIT:
		case TCP_CLOSING:
		case TCP_LAST_ACK:
		case TCP_TIME_WAIT:
			mr1=NULL_MEM_REC;
			*mr3=mr1;
//SHARABSHARAB : return -1->return 0
			return 0;
		}
	}
	/* cnt == 0 means "I want it all" */
	if(cnt == 0)
		cnt = tcb_pt->rcvcnt;
	/* See if the user can take all of it */
#if 0
	if(tcb_pt->rcvcnt <= cnt){
		cnt = tcb->rcvcnt;
		*bpp = tcb->rcvq;
		tcb->rcvq = NULL;
	} else {
		*bpp = ambufw(cnt);
		pullup(&tcb_pt->rcvq,(*bpp)->data,cnt);
		(*bpp)->cnt = cnt;
	}
#endif // 0
	if(tcb_pt->rcvcnt <= (unsigned long)cnt){
		cnt = tcb_pt->rcvcnt;
	}
	mr1=mem_rec_req(cnt);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2309);
		mr1=NULL_MEM_REC;
		*mr3=mr1;
		return -1;
	}
	my_pullup_linklist_copy_data(&(tcb_pt->rcvq),mem_rec[mr1].parm,cnt);
//	tcb_pt->rcvq=mr1;

//SHARAB
#ifdef DEBUG_ACT
	prnt_string("recv_tcp:");
	prnt_int(tcb_no);prnt_char(CR);
#endif
	tcb_pt->rcvcnt -= cnt;
	tcb_pt->rcv_wnd += cnt;
	/* Do a window update if it was less than one packet and now it's more */
	if((tcb_pt->rcv_wnd > tcb_pt->mss) && ((tcb_pt->rcv_wnd - cnt) < tcb_pt->mss)){
		tcb_pt->force_flag = 1;
		tcp_output(tcb_no);
	}
	*mr3=mr1;
	return cnt;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* This function is called from IP with the IP header in machine byte order,*/
/* along with a mbuf chain pointing to the TCP header.			    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void tcp_input(
unsigned int iface_no _U_,   /* Incoming interface (ignored) */
IP_STRUCT *ip,		/* IP header */
WORD_MR mr1,		/* Data field, if any */
int rxbroadcast,	/* Incoming broadcast - discard if true */
gint32 said _U_ 	/* Authenticated packet */
){
	unsigned int tcb_no,ntcb_no;
	TCB_STRUCT *tcb_pt,*ntcb_pt;	/* TCP Protocol control block */
	TCP_STRUCT seg; 		/* Local copy of segment header */
	CONNECTION_STRUCT conn; 	/* Local copy of addresses */
	PSEUDO_HEADER_STRUCT ph;	/* Pseudo-header for checksumming */
	int hdrlen;			/* Length of TCP header */
	guint length;
	gint32 t;

//SHARAB
#ifdef DEBUG_ACT
	prnt_string("tcp_input:\r\n");
	print_mr(mr1);
#endif
	if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2303);
		return;
	}
	Tcp_mib[tcpInSegs].value++;
	if(rxbroadcast){
		/* Any TCP packet arriving as a broadcast is */
		/* to be completely IGNORED!!		     */

		mem_rec_rls(mr1);
		return;
	}
	length = ip->length - IPLEN - ip->optlen;
	ph.source = ip->source;
	ph.dest = ip->dest;
	ph.protocol = ip->protocol;
	ph.length = length;
	if(cksum(&ph,mr1,length) != 0){
		/* Checksum failed, ignore segment completely */
		Tcp_mib[tcpInErrs].value++;
		mem_rec_rls(mr1);
		prnt_msg(TCP_ERR,0x2304);
		return;
	}
	/* Form local copy of TCP header in host byte order */
	if((hdrlen = ntohtcp(&seg,&mr1)) < 0){
		/* TCP header is too small */
//SHARAB start
		prnt_msg(TCP_ERR,0x2305);
		if(mr1 < NO_MEM_REC){
			print_mr(mr1);
			mem_rec_rls(mr1);
		}
//SHARAB end
		return;
	}
	length -= hdrlen;
	/* Fill in connection structure and find TCB */
	conn.local.address = ip->dest;
	conn.local.port = seg.dest;
	conn.remote.address = ip->source;
	conn.remote.port = seg.source;

	if((tcb_no = lookup_tcb(&conn)) >= MAX_TCB){
		/* If this segment doesn't carry a SYN, reject it */
		if(!seg.syn_flag){
//SHARAB
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			reset(ip,&seg);
			return;
		}
		/* See if there's a TCP_LISTEN on this socket with
		 * unspecified remote address and port
		 */
		conn.remote.address = 0;
		conn.remote.port = 0;
		if((tcb_no = lookup_tcb(&conn)) >= MAX_TCB){
			/* Nope, try unspecified local address too */
			conn.local.address = 0;
			if((tcb_no = lookup_tcb(&conn)) >= MAX_TCB){
				/* No LISTENs, so reject */
//SHARAB
				if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
				reset(ip,&seg);
				return;
			}
		}
		tcb_pt=&tcb_st[tcb_no];
		/* We've found a server listen socket, so clone the TCB */
		if(tcb_pt->clone_flag){
			// the old tcb is remained as listen socket
//			ntcb = (struct tcb *)mallocw(sizeof (struct tcb));
//			ASSIGN(*ntcb_pt,*tcb);
			ntcb_no=find_free_tcb_st();
			ntcb_pt=&tcb_st[ntcb_no];
//			mem_cpy((guint8 *)ntcb_pt,(guint8 *)tcb_pt,sizeof(TCB_STRUCT));
			copy_tcb_fields_to_new_one(tcb_no,ntcb_no);

//			tcb = ntcb_pt;
			tcb_pt = ntcb_pt;
			tcb_no=ntcb_no;

//			tcb_pt->timer.arg = tcb;

			/* Put on list */
			tcb_pt->next = Tcbs;
			Tcbs = tcb_no;
		}
		/* Put all the socket info into the TCB */
		tcb_pt->conn.local.address = ip->dest;
		tcb_pt->conn.remote.address = ip->source;
		tcb_pt->conn.remote.port = seg.source;
	}
	tcb_pt=&tcb_st[tcb_no];
	tcb_pt->congest_flag = ip->congest;
	/* Do unsynchronized-state processing (p. 65-68) */
#ifdef DEBUG_ACT
	prnt_string("tcp_input2::::::::::::::::::::::::");
	prnt_int(tcb_no);prnt_char('-');
	prnt_byte(tcb_pt->state);prnt_char(CR);
	print_mr(mr1);
#endif
	switch(tcb_pt->state){
	  case TCP_CLOSED:
//SHARAB
		if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
		reset(ip,&seg);
		return;
	  case TCP_LISTEN:
		if(seg.rst_flag){
//SHARAB
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			return;
		}
		if(seg.ack_flag){
//SHARAB
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			reset(ip,&seg);
			return;
		}
		if(seg.syn_flag){
			/* (Security check is bypassed) */
			/* page 66 */
			proc_syn(tcb_no,ip->tos,&seg);
			send_syn(tcb_no);
			settcpstate(tcb_no,TCP_SYN_RECEIVED);
			tcp_output(tcb_no);
		}
		if(mr1 < NO_MEM_REC){ // if tcp header is removed and no more data remained.
			mem_rec_rls(mr1);/* Unlikely to get here directly */
		}
		return;
	  case TCP_SYN_SENT:
		if(seg.ack_flag){
			if(!seq_within(seg.ack,tcb_pt->iss+1,tcb_pt->snd_nxt)){
//SHARAB
				if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
				reset(ip,&seg);
				return;
			}
		}
		if(seg.rst_flag){      /* p 67 */
			if(seg.ack_flag){
				/* The ack must be acceptable since we just checked it.
				 * This is how the remote side refuses connect requests.
				 */
//HALAB : RESET->
				close_self(tcb_no,RESET_CLOSE_REASON);
			}
//SHARAB
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			return;
		}
		/* (Security check skipped here) */
#ifdef	PREC_CHECK	/* Turned off for compatibility with BSD */
		/* Check incoming precedence; it must match if there's an ACK */
		if((seg.ack_flag) && (PREC(ip->tos) != PREC(tcb_pt->tos))){
//SHARAB
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			reset(ip,&seg);
			return;
		}
#endif
		if(seg.syn_flag){
			proc_syn(tcb_no,ip->tos,&seg);
			if(seg.ack_flag){
				/* Our SYN has been acked, otherwise the ACK
				 * wouldn't have been valid.
				 */
				update(tcb_no,&seg,length);
				settcpstate(tcb_no,TCP_ESTABLISHED);
			} else {
				settcpstate(tcb_no,TCP_SYN_RECEIVED);
			}
			if((length != 0) || (seg.fin_flag)) {
				break;		/* Continue processing if there's more */
			}
			tcp_output(tcb_no);
		} else {
//SHARAB
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);/* Ignore if neither SYN or RST is set */
		}
		return;
//alaki
	   default		:
		break;
	}
	/* We reach this point directly in any synchronized state. Note that
	 * if we fell through from LISTEN or SYN_SENT processing because of a
	 * data-bearing SYN, window trimming and sequence testing "cannot fail".
	 *
	 * Begin by trimming segment to fit receive window.
	 */
	if(trim(tcb_no,&seg,&mr1,&length) == -1){
		/* Segment is unacceptable */
		if(!seg.rst_flag){     /* NEVER answer RSTs */
			/* In SYN_RECEIVED state, answer a retransmitted SYN
			 * with a retransmitted SYN/ACK.
			 */
			if(tcb_pt->state == TCP_SYN_RECEIVED)
				tcb_pt->snd_ptr = tcb_pt->snd_una;
			tcb_pt->force_flag = 1;
			tcp_output(tcb_no);
		}
		return;
	}
	/* If segment isn't the next one expected, and there's data
	 * or flags associated with it, put it on the resequencing
	 * queue and remind ourselves to ACK it. Then strip off
	 * the SYN/data/FIN and continue to process the ACK (or RST)
	 */
	if( (seg.seq != tcb_pt->rcv_nxt) &&
	    ((length != 0) || (seg.syn_flag) || (seg.fin_flag))
	){
//SHARABSHARAB
#ifdef DEBUG_ACT
		prnt_string("bad seq.seq=");prnt_long(seg.seq);prnt_char('-');
		prnt_string("bad tcb_pt->rcv_nxt=");prnt_long(tcb_pt->rcv_nxt);prnt_char(CR);
#endif
		add_reseq(tcb_no,ip->tos,&seg,&mr1,length);
		if((seg.ack_flag) && (!seg.rst_flag))
			tcb_pt->force_flag = 1;
		seg.syn_flag=seg.fin_flag=0;
		length = 0;
	}
	/* This loop first processes the current segment, and then
	 * repeats if it can process the resequencing queue.
	 */
/////	for(;;){
		/* We reach this point with an acceptable segment; data and flags
		 * (if any) are in the window, and if there's data, syn or fin,
		 * the starting sequence number equals rcv.nxt
		 * (p. 70)
		 */
		if(seg.rst_flag){
			if( (tcb_pt->state == TCP_SYN_RECEIVED) &&
			    (!tcb_pt->clone_flag) && (!tcb_pt->active_flag)
			){
//#ifdef DEBUG_ACT
				prnt_string("seg.rst_flag is set-1\r\n");
//#endif
				/* Go back to listen state only if this was
				 * not a cloned or active server TCB
				 */
				settcpstate(tcb_no,TCP_LISTEN);
			} else {
//#ifdef DEBUG_ACT
				prnt_string("seg.rst_flag is set-2\r\n");
//#endif
//HALAB : RESET->
				close_self(tcb_no,RESET_CLOSE_REASON);
			}
//SHARAB : NULL_MEM_REC->NO_MEM_REC
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			return;
		}
		/* (Security check skipped here) p. 71 */
#ifdef	PREC_CHECK
		/* Check for precedence mismatch */
		if(PREC(ip->tos) != PREC(tcb_pt->tos)){
//SHARAB : NULL_MEM_REC->NO_MEM_REC
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			reset(ip,&seg);
			return;
		}
#endif
		/* Check for erroneous extra SYN */
		if(seg.syn_flag){
//SHARAB : NULL_MEM_REC->NO_MEM_REC
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			reset(ip,&seg);
			return;
		}
		/* Update timestamp field */
		if((seg.tstamp_flag) &&
		   (seq_within(tcb_pt->last_ack_sent,seg.seq,seg.seq+length))
		)
			tcb_pt->ts_recent = seg.tsval;
		/* Check ack field p. 72 */
		if(!seg.ack_flag){
//SHARAB : NULL_MEM_REC->NO_MEM_REC
			if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);/* All segments after synchronization must have ACK */
			return;
		}
		/* Process ACK */
		switch(tcb_pt->state){
		  case TCP_SYN_RECEIVED:
			if(seq_within(seg.ack,tcb_pt->snd_una+1,tcb_pt->snd_nxt)){
				update(tcb_no,&seg,length);
				settcpstate(tcb_no,TCP_ESTABLISHED);
			} else {
//SHARAB : NULL_MEM_REC->NO_MEM_REC
			       if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
			       reset(ip,&seg);
			       return;
			}
			break;
		  case TCP_ESTABLISHED:
		  case TCP_CLOSE_WAIT:
		  case TCP_FINWAIT2:
			update(tcb_no,&seg,length);
			break;
		  case TCP_FINWAIT1:	  /* p. 73 */
			update(tcb_no,&seg,length);
			if(tcb_pt->sndcnt == 0){
				/* Our FIN is acknowledged */
				settcpstate(tcb_no,TCP_FINWAIT2);
			}
			break;
		  case TCP_CLOSING:
			update(tcb_no,&seg,length);
			if(tcb_pt->sndcnt == 0){
				/* Our FIN is acknowledged */
				settcpstate(tcb_no,TCP_TIME_WAIT);
#if 0
				set_timer(&tcb_pt->timer,MSL2*1000L);
				start_timer(&tcb_pt->timer);
#endif // 0
				tcb_pt->next_timer_count=change_to_kiatel_count(MSL2*1000L);
				start_tcp_timer(tcb_no,tcb_pt->next_timer_count);
			}
			break;
		  case TCP_LAST_ACK:
			update(tcb_no,&seg,length);
			if(tcb_pt->sndcnt == 0){
				/* Our FIN is acknowledged, close connection */
				close_self(tcb_no,NORMAL_CLOSE_REASON);
				return;
			}
			break;
		  case TCP_TIME_WAIT:

//			start_timer(&tcb_pt->timer);
			start_tcp_timer(tcb_no,tcb_pt->next_timer_count);
			break;
		  default		:
//alaki
			prnt_msg(TCP_ERR,0x2307);
			return;
		}

		/* (URGent bit processing skipped here) */

		/* Process the segment text, if any, beginning at rcv.nxt (p. 74) */
		if(length != 0){
			switch(tcb_pt->state){
			case TCP_SYN_RECEIVED:
			case TCP_ESTABLISHED:
			case TCP_FINWAIT1:
			case TCP_FINWAIT2:
				/* Update performance figures */
				t = msclock();
				tcb_pt->inrate = (7*tcb_pt->inrate + t - tcb_pt->lastrx)/8;
				tcb_pt->lastrx = t;
				tcb_pt->inlen = (7*tcb_pt->inlen + length)/8;
				/* Place on receive queue */
				append(&tcb_pt->rcvq,mr1); // here mr1 may be NULL_MEM_REC, append should handle that
				tcb_pt->rcvcnt += length;
				tcb_pt->rcv_nxt += length;
				tcb_pt->rcv_wnd -= length;
				tcb_pt->force_flag = 1;
				/* Notify user */
#if 0
				if(tcb_pt->r_upcall)
					(*tcb_pt->r_upcall)(tcb,tcb_pt->rcvcnt);
#endif // 0
				if(tcb_pt->trdiscard_flag == SET){
					trdiscard(tcb_no,tcb_pt->rcvcnt);
				}
				else{
					s_trcall(tcb_no,tcb_pt->rcvcnt);
				}
				break;
			default:
				/* Ignore segment text */
//SHARAB : NULL_MEM_REC->NO_MEM_REC
				if(mr1 < NO_MEM_REC) mem_rec_rls(mr1);
				break;
			}
		}
		/* process FIN bit (p 75) */
		if(seg.fin_flag){
			tcb_pt->force_flag = 1;   /* Always respond with an ACK */

			switch(tcb_pt->state){
			  case TCP_SYN_RECEIVED:
			  case TCP_ESTABLISHED:
				tcb_pt->rcv_nxt++;
				settcpstate(tcb_no,TCP_CLOSE_WAIT);
				break;
			  case TCP_FINWAIT1:
				tcb_pt->rcv_nxt++;
				if(tcb_pt->sndcnt == 0){
					/* Our FIN has been acked; bypass TCP_CLOSING state */
					settcpstate(tcb_no,TCP_TIME_WAIT);
#if 0
					set_timer(&tcb_pt->timer,MSL2*1000L);
					start_timer(&tcb_pt->timer);
#endif // 0
					tcb_pt->next_timer_count=change_to_kiatel_count(MSL2*1000L);
					start_tcp_timer(tcb_no,tcb_pt->next_timer_count);
				} else {
					settcpstate(tcb_no,TCP_CLOSING);
				}
				break;
			  case TCP_FINWAIT2:
				tcb_pt->rcv_nxt++;
				settcpstate(tcb_no,TCP_TIME_WAIT);
#if 0
				set_timer(&tcb_pt->timer,MSL2*1000L);
				start_timer(&tcb_pt->timer);
#endif // 0
				tcb_pt->next_timer_count=change_to_kiatel_count(MSL2*1000L);
				start_tcp_timer(tcb_no,tcb_pt->next_timer_count);
				break;
			  case TCP_CLOSE_WAIT:
			  case TCP_CLOSING:
			  case TCP_LAST_ACK:
				break;		/* Ignore */
			  case TCP_TIME_WAIT:	  /* p 76 */
//				start_timer(&tcb_pt->timer);
				start_tcp_timer(tcb_no,tcb_pt->next_timer_count);
				break;
			  default		:
//alaki
				prnt_msg(TCP_ERR,0x2308);
				return;
			}
			/* Call the client again so he can see EOF */
#if 0
			if(tcb_pt->r_upcall)
				(*tcb_pt->r_upcall)(tcb,tcb_pt->rcvcnt);
#endif // 0
			if(tcb_pt->trdiscard_flag == SET){
				trdiscard(tcb_no,tcb_pt->rcvcnt);
			}
			else{
				s_trcall(tcb_no,tcb_pt->rcvcnt);
			}
		}
		/* Scan the resequencing queue, looking for a segment we can handle,
		 * and freeing all those that are now obsolete.
		 */
		while((tcb_pt->reseq != NULL) && seq_ge(tcb_pt->rcv_nxt,tcb_pt->reseq->seg.seq)){
			get_reseq(tcb_no,&ip->tos,&seg,&mr1,&length);
			if(trim(tcb_no,&seg,&mr1,&length) == 0)
				goto gotone;
			/* Segment is an old one; trim has freed it */
		}
/////		break;
gotone: ;
/////	}
	tcp_output(tcb_no);	   /* Send any necessary ack */
}
#endif // TCP_INSTALLED
