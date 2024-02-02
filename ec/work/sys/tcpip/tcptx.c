
/*************	Time-stamp: <2009-09-23 17:27:19 savoj>   ************/
// ERROR : 0x2400-0x2412
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
#ifdef MY_TCPIP_FOR_OAM
#ifdef OPERATOR_SIDE
BYTE send_oam_pckt_to_tcp_port(BYTE n,unsigned int size,BYTE *dat);
extern BUF nettx_st0,nettx_st1;
#endif
#endif
#endif // CENTRAL_EXTERN

//SHARAB
#ifdef KA9Q_TCP_DEBUG_ACT
#define DEBUG_ACT
#endif

/* ======================================================================== */
/* TCP transmit upcall routine						    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void s_ttcall(unsigned int tcb_no _U_,gint32 cnt _U_){

#ifdef MY_TCPIP_FOR_OAM
//SHARABSHARAB : type should be checked
????
	unsigned int i,size;
	BYTE dat[4096];
	BUF *buf_ptr;
	i=tcb_st[tcb_no].rec_no;
#ifdef SYSTEM_SIDE
	buf_ptr=&oam[i].nettx_st;
#endif
#ifdef OPERATOR_SIDE
	buf_ptr=(i == 0) ? &nettx_st0 : &nettx_st1;
#endif
	if(get_pckt_from_variable_size_buf(buf_ptr,&size,dat) == OK)
	    send_oam_pckt_to_tcp_port(i,size,dat);
#endif
#if 0
	/* Wake up anybody waiting to send data, and let them run */
	ksignal(itop(tcb->user),1);
	kwait(NULL);
#endif // 0
	if(cnt < 0) return; // retx window is full, I need it to find link failure
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
/*
		if(tcb_st[tcb_no].rec_no < MAX_KFTPD_REC){
			to_kftpd_q(tcb_st[tcb_no].rec_no, KFTPD_L2_TCP_SEND_READY_STIM, NULL_MEM_REC);
		}
*/
		break;
	case KFTPD_DATA_TCP_TYPE	  :
		if(tcb_st[tcb_no].rec_no < MAX_KFTPD_REC){
			to_aux_kftpd_q(tcb_st[tcb_no].rec_no, AUX_KFTPD_L2_TCP_SEND_READY_STIM, NULL_MEM_REC);
		}
		break;
#endif
#ifdef KFTP_INSTALLED
	case KFTP_CONTROL_TCP_TYPE	 :
/*
		if(tcb_st[tcb_no].rec_no < MAX_KFTP_REC){
			to_kftp_q(tcb_st[tcb_no].rec_no, KFTP_L2_TCP_SEND_READY_STIM, NULL_MEM_REC);
		}
*/
		break;
	case KFTP_DATA_TCP_TYPE 	 :
		if(tcb_st[tcb_no].rec_no < MAX_KFTP_REC){
			to_aux_kftp_q(tcb_st[tcb_no].rec_no, AUX_KFTP_L2_TCP_SEND_READY_STIM, NULL_MEM_REC);
		}
		break;
#endif
#ifdef ROS_FOR_LI_INSTALLED
	case LI_ROS_TCP_TYPE	:
		if(tcb_st[tcb_no].rec_no < MAX_LI_XPORT_REC){
			to_li_xport_que(tcb_st[tcb_no].rec_no, LI_XPORT_L2_TCP_SEND_READY_STIM, NULL_MEM_REC);
		}
		break;
#endif
#ifdef TELNETD_INSTALLED
	case TELNETD_TCP_TYPE	    :
		break;
#endif
#ifdef HI1_INSTALLED
	case HI1_TCP_TYPE	:
		break;
#endif
#ifdef HI1_DISTRIBUTOR
	case HI1_DISTRIBUTOR_TCP_TYPE	    :
		break;
#endif
#ifdef SYSCON_SERVER
	case SYSCON_SERVER_TCP_TYPE	  :
		break;
#endif
#ifdef SYSCON_CLIENT
	case SYSCON_CLIENT_TCP_TYPE	  :
		break;
#endif
	default 		  :
		prnt_msg(TCP_ERR,0x2410);
		break;
	}
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void so_tcp_send_hndlr(void){
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef TCP_INSTALLED
int so_tcp_send(USOCK_STRUCT *up,WORD_MR mr1,SOCKADDR_STRUCT *to _U_){

	unsigned int tcb_no;
	long cnt;

	if((tcb_no = up->cb.tcb_no) >= MAX_TCB){
		mem_rec_rls(mr1);
		errno = ENOTCONN_ERROR;
		prnt_msg(TCP_ERR,0x2400);
		return -1;
	}
	cnt = send_tcp(tcb_no,mr1);
#if 0
	while(
		((tcb_no = up->cb.tcb_no) < MAX_TCB) &&
		 (tcb_st[tcb_no].sndcnt > tcb_st[tcb_no].window)
		){
		/* Send queue is full */
		if(up->noblock){
			errno = EWOULDBLOCK_ERROR;
			prnt_msg(TCP_ERR,0x2401);
			return -1;
		}
/*
		else if((errno = kwait(up)) != 0){
			return -1;
		}
*/
	}
#endif // 0
	if(tcb_no == NULL_TCB){
		errno = ENOTCONN_ERROR;
		prnt_msg(TCP_ERR,0x2402);
		return -1;
	}
	return cnt;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* User send routine							    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
long send_tcp(unsigned int tcb_no,WORD_MR mr1){

	gint32 cnt;
	TCB_STRUCT *tcb_pt;

	if(mr1 >= NO_MEM_REC){
		Net_error = NET_ERROR_INVALID;
		prnt_msg(TCP_ERR,0x2403);
		return -1;
	}
	if(tcb_no >= MAX_TCB){
		mem_rec_rls(mr1);
		Net_error = NET_ERROR_INVALID;
		prnt_msg(TCP_ERR,0x2404);
//HALAB
		prnt_int(tcb_no);prnt_char(CR);
		return -1;
	}
#ifdef DEBUG_ACT
	prnt_string("send_tcp:");prnt_int(tcb_no);prnt_char(CR);
	print_mr(mr1);
#endif
	tcb_pt=&tcb_st[tcb_no];
	cnt = mem_rec[mr1].size;
#ifdef HI1_INSTALLED
	if(tcb_pt->type == HI1_TCP_TYPE){
#ifdef DES_INSTALLED
	    if(use_des){
		guint hi1_no=tcb_pt->rec_no;
		int i,size;
		HI1_STRUCT  *hi1_pt;
		if(hi1_no >= MAX_HI1_REC){
		    prnt_msg(TCP_ERR,0x2411);
		    mem_rec_rls(mr1);
		    Net_error = NET_ERROR_INVALID;
		    return -1;
		}
		size=(cnt/8)*8;

		// zero pad it to make it multiple of 8
		if(cnt > size){
		    WORD_MR mr2;
		    size+=8;
		    mr2=mem_rec_req(size);
		    if(mr2 >= NO_MEM_REC){
			prnt_msg(TCP_ERR,0x2412);
			Net_error = NET_ERROR_NO_MEM;
			return -1;
		    }
		    memcpy(&mem_rec[mr2].parm[0],&mem_rec[mr1].parm[0],cnt);
		    for(i=cnt;i<size;i++){
			mem_rec[mr2].parm[i]=0;  // 0 pad, it is text!
		    }
		    mem_rec_rls(mr1);
		    cnt=size;
		    mr1=mr2;
		}

		hi1_pt=&hi1_rec[hi1_no];
		for(i=0;i<size;i+=8){
		    des(hi1_pt->kne,&mem_rec[mr1].parm[i]);
		}
/* should not happen
		size=cnt-size;
		if(size != 0){
		    unsigned char work[8];
		    memcpy(work,&mem_rec[mr1].parm[i],size);
		    des(hi1_pt->kne,work);
		    memcpy(&mem_rec[mr1].parm[i],work,size);
		}
*/
	    }
#endif
	}
#endif
	switch(tcb_pt->state){
	  case TCP_CLOSED:
		prnt_msg(TCP_ERR,0x2405);
//SABZ
		prnt_int(tcb_no);prnt_char(CR);
		mem_rec_rls(mr1);
		Net_error = NET_ERROR_NO_CONN;
		return -1;
	  case TCP_LISTEN:
#ifdef DEBUG_ACT
		prnt_string("TCP_LISTEN\r\n");
#endif
		if((tcb_pt->conn.remote.address == 0) && (tcb_pt->conn.remote.port == 0)){
			/* Save data for later */
			append(&tcb_pt->sndq,mr1);
			tcb_pt->sndcnt += cnt;
			break;
		}
		/* Change state from passive to active */
		tcb_pt->active_flag = 1;
		send_syn(tcb_no);
		settcpstate(tcb_no,TCP_SYN_SENT);  /* Note fall-thru */
		append(&tcb_pt->sndq,mr1);
		tcb_pt->sndcnt += cnt;
		tcp_output(tcb_no);
		break;
	  case TCP_SYN_SENT:
	  case TCP_SYN_RECEIVED:
	  case TCP_ESTABLISHED:
	  case TCP_CLOSE_WAIT:
//SHARAB
#ifdef DEBUG_ACT
		prnt_string("TCP_SYN_SENT:TCP_SYN_RECEIVED:TCP_ESTABLISHED:TCP_CLOSE_WAIT\r\n");
#endif
		append(&tcb_pt->sndq,mr1);
		tcb_pt->sndcnt += cnt;
		tcp_output(tcb_no);
		break;
	  case TCP_FINWAIT1:
	  case TCP_FINWAIT2:
	  case TCP_CLOSING:
	  case TCP_LAST_ACK:
	  case TCP_TIME_WAIT:
		prnt_msg(TCP_ERR,0x2406);
		mem_rec_rls(mr1);
		Net_error = NET_ERROR_CON_CLOS;
		return -1;
	}
	return (long)cnt;
}
#endif // TCP_INSTALLED
/* ======================================================================== */
/* Send a segment on the specified connection. One gets sent only	    */
/* if there is data to be sent or if "force" is non zero                    */
/* ======================================================================== */
#ifdef TCP_INSTALLED
void tcp_output(unsigned int tcb_no){

	TCB_STRUCT *tcb_pt;
	WORD_MR mr1;		/* Header and data buffer pointers */
	TCP_STRUCT seg; 	/* Local working copy of header */
	guint	ssize;		/* Size of current segment being sent */
				/* including SYN and FIN flags */
	guint	dsize;		/* Size of segment less SYN and FIN */
	gint32	usable; 	/* Usable window */
	gint32	sent;		/* Sequence count (incl SYN/FIN) already */
				/* in the pipe but not yet acked */
	gint32 rto;		/* Retransmit timeout setting */

	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2407);
		return;
	}
	tcb_pt=&tcb_st[tcb_no];
#ifdef DEBUG_ACT
	prnt_string("tcp_output start:");prnt_int(tcb_no);prnt_char(CR);
#endif
	switch(tcb_pt->state){
	  case TCP_LISTEN:
	  case TCP_CLOSED:
		prnt_msg(TCP_ERR,0x2408);
		print_call_frame();
#ifdef DEBUG_ACT
		prnt_string("tcp_output end\r\n");
#endif
		return; /* Don't send anything */
	  default	:
		break;
	}
	for(;;){
		mem_set(&seg,0,sizeof(seg));
		/* Compute data already in flight */
		sent = tcb_pt->snd_ptr - tcb_pt->snd_una;
//LION start : my added code
		if(tcb_pt->my_reset_flag == SET){
			if(/*(sent == 0) &&*/(tcb_pt->sndcnt == 0)){
				tcb_pt->force_flag = 1;
				tcb_pt->my_send_rst_flag=SET;
			}
		}
//LION end
#ifdef DEBUG_ACT
		prnt_string("tcp_out:102\r\n");
		prnt_int(tcb_pt->snd_ptr);prnt_char('-');
		prnt_int(tcb_pt->snd_una);prnt_char('-');
		prnt_int(sent);prnt_char(CR);
#endif
		/* Compute usable send window as minimum of offered
		 * and congestion windows, minus data already in flight.
		 * Be careful that the window hasn't shrunk --
		 * these are unsigned vars.
		 */
		usable = min(tcb_pt->snd_wnd,tcb_pt->cwind);
#ifdef DEBUG_ACT
		prnt_string("tcp_out:103\r\n");
		prnt_int(tcb_pt->snd_wnd);prnt_char('-');
		prnt_int(tcb_pt->cwind);prnt_char('-');
		prnt_int(usable);prnt_char(CR);
#endif
		if(usable > sent)
			usable -= sent; /* Most common case */
		else if((usable == 0) && (sent == 0))
			usable = 1;	/* Closed window probe */
		else
			usable = 0;	/* Window closed or shrunken */

		/* Compute size of segment we *could* send. This is the
		 * smallest of the usable window, the mss, or the amount
		 * we have on hand. (I don't like optimistic windows)
		 */
		ssize = min(tcb_pt->sndcnt - sent,usable);
#ifdef DEBUG_ACT
		prnt_string("tcp_out:104\r\n");
		prnt_int(tcb_pt->sndcnt);prnt_char('-');
		prnt_int(sent);prnt_char('-');
		prnt_int(ssize);prnt_char(CR);
#endif
		ssize = min(ssize,tcb_pt->mss);
#ifdef DEBUG_ACT
		prnt_string("tcp_out:105\r\n");
		prnt_int(tcb_pt->mss);prnt_char('-');
		prnt_int(ssize);prnt_char(CR);
#endif

		/* Now we decide if we actually want to send it.
		 * Apply John Nagle's "single outstanding segment" rule.
		 * If data is already in the pipeline, don't send
		 * more unless it is MSS-sized, the very last packet,
		 * or we're being forced to transmit anyway (e.g., to
		 * ack incoming data).
		 */
#ifdef DEBUG_ACT
		prnt_string("tcp_out:106\r\n");
		prnt_int(tcb_pt->state);prnt_char('-');
		prnt_int(tcb_pt->force_flag);prnt_char('-');
		prnt_int(tcb_pt->mss);prnt_char('-');
		prnt_int(tcb_pt->sndcnt);prnt_char('-');
		prnt_int(tcb_pt->synack_flag);prnt_char('-');
		prnt_int(tcb_pt->iss);prnt_char('-');
		prnt_int(tcb_pt->snd_ptr);prnt_char('-');
		prnt_int(tcb_pt->snd_nxt);prnt_char('-');
		prnt_int(sent);prnt_char('-');
		prnt_int(Tcp_syndata);prnt_char('-');
		prnt_int(ssize);prnt_char(CR);
#endif
		if(
		   (!tcb_pt->force_flag) && (sent != 0) && (ssize < tcb_pt->mss) &&
		   !((tcb_pt->state == TCP_FINWAIT1) && (ssize == tcb_pt->sndcnt-sent))
		  ){
			ssize = 0;
		}
#ifdef DEBUG_ACT
		prnt_string("tcp_out:107\r\n");
		prnt_int(ssize);prnt_char(CR);
#endif
		/* Unless the tcp syndata option is on, inhibit data until
		 * our SYN has been acked. This ought to be OK, but some
		 * old TCPs have problems with data piggybacked on SYNs.
		 */
		if((!tcb_pt->synack_flag) && (!Tcp_syndata)){
			if(tcb_pt->snd_ptr == tcb_pt->iss){
				ssize = min(1,ssize);	/* Send only SYN */
			}
			else{
				ssize = 0;	/* Don't send anything */
			}
		}
#ifdef DEBUG_ACT
		prnt_string("tcp_out:108\r\n");
		prnt_int(ssize);prnt_char(CR);
#endif
		/* If we're forced to send an ack while retransmitting,
		 * don't send any data. This will let us use the current
		 * sequence number, which may be necessary for the
		 * ack to be accepted by the receiver
		 */
		if((tcb_pt->force_flag) && (tcb_pt->snd_ptr != tcb_pt->snd_nxt)){
			ssize = 0;
		}
#ifdef DEBUG_ACT
		prnt_string("tcp_out:109\r\n");
		prnt_int(ssize);prnt_char(CR);
#endif
		if((ssize == 0) && (!tcb_pt->force_flag)){
			break;		/* No need to send anything */
		}
#ifdef DEBUG_ACT
		prnt_string("tcp_out:110\r\n");
		prnt_int(ssize);prnt_char(CR);
#endif
		tcb_pt->force_flag = 0;   /* Only one forced segment! */

		seg.source = tcb_pt->conn.local.port;
		seg.dest = tcb_pt->conn.remote.port;

		/* Set the flags according to the state we're in. It is
		 * assumed that if this segment is associated with a state
		 * transition, then the state change will already have been
		 * made. This allows this routine to be called from a
		 * retransmission timeout with force=1.
		 */
		seg.ack_flag = 1; /* Every state except TCP_SYN_SENT */
		seg.congest_flag = tcb_pt->congest_flag;

		if(tcb_pt->state == TCP_SYN_SENT)
			seg.ack_flag = 0; /* Haven't seen anything yet */

		dsize = ssize;
		if((!tcb_pt->synack_flag) && (tcb_pt->snd_ptr == tcb_pt->iss)){
			/* Send SYN */
			seg.syn_flag = 1;
			dsize--;	/* SYN isn't really in snd queue */
			/* Also send MSS, wscale and tstamp (if OK) */
			seg.mss = Tcp_mss;
			seg.mss_flag = 1;
			seg.wsopt = DEF_WSCALE;
			seg.wscale_flag = 1;
			if(Tcp_tstamps){
				seg.tstamp_flag = 1;
				seg.tsval = msclock();
			}
		}
		/* If there's no data, use snd_nxt rather than snd_ptr to
		 * ensure ack acceptance in case we were retransmitting
		 */
		if(ssize == 0)
			seg.seq = tcb_pt->snd_nxt;
		else
			seg.seq = tcb_pt->snd_ptr;
		tcb_pt->last_ack_sent = seg.ack = tcb_pt->rcv_nxt;
		if((seg.syn_flag) || (!tcb_pt->ws_ok_flag)){
			seg.wnd = tcb_pt->rcv_wnd;
		}
		else{
			seg.wnd = tcb_pt->rcv_wnd >> tcb_pt->rcv_wind_scale;
		}
		/* Now try to extract some data from the send queue. Since
		 * SYN and FIN occupy sequence space and are reflected in
		 * sndcnt but don't actually sit in the send queue, extract
		 * will return one less than dsize if a FIN needs to be sent.
		 */
#if 0
		dbp = ambufw(NET_HDR_PAD+dsize);
		dbp->data += NET_HDR_PAD;	/* Allow room for other hdrs */
#endif // 0
		if(dsize != 0){
			mr1=mem_rec_req(dsize);
		}
		else{
			mr1=NULL_MEM_REC;
		}
		if(dsize != 0){
			gint32 offset;

			/* SYN doesn't actually take up space on the sndq,
			 * so take it out of the sent count
			 */
			offset = sent;
			if((!tcb_pt->synack_flag) && (sent != 0))
				offset--;

//			dbp->cnt = extract(tcb_pt->sndq,(guint)offset,dbp->data,dsize);
			mem_rec[mr1].size=extract(tcb_pt->sndq,(guint)offset,&(mem_rec[mr1].parm[0]),dsize);
#ifdef DEBUG_ACT
			prnt_string("tcp_out:111\r\n");
			prnt_int(dsize);prnt_char('-');
			prnt_int(mem_rec[mr1].size);prnt_char('-');
			prnt_char(CR);
			print_mr(mr1);
#endif
			if(mem_rec[mr1].size != dsize){
				/* We ran past the end of the send queue;
				 * send a FIN
				 */
				seg.fin_flag = 1;
				dsize--;
			}
		}
		/* If the entire send queue will now be in the pipe, set the
		 * push flag
		 */
		if((dsize != 0) && (sent + ssize == tcb_pt->sndcnt))
			seg.psh_flag = 1;

		/* If this transmission includes previously transmitted data,
		 * snd_nxt will already be past snd_ptr. In this case,
		 * compute the amount of retransmitted data and keep score
		 */
		if(tcb_pt->snd_ptr < tcb_pt->snd_nxt)
			tcb_pt->resent += min(tcb_pt->snd_nxt - tcb_pt->snd_ptr,ssize);

		tcb_pt->snd_ptr += ssize;
		/* If this is the first transmission of a range of sequence
		 * numbers, record it so we'll accept acknowledgments
		 * for it later
		 */
		if(seq_gt(tcb_pt->snd_ptr,tcb_pt->snd_nxt))
			tcb_pt->snd_nxt = tcb_pt->snd_ptr;

		if((tcb_pt->ts_ok_flag) && (seg.ack_flag)){
			seg.tstamp_flag = 1;
			seg.tsval = msclock();
			seg.tsecr = tcb_pt->ts_recent;
		}
		/* Generate TCP header, compute checksum, and link in data */
//		htontcp(&seg,&dbp,tcb_pt->conn.local.address,tcb_pt->conn.remote.address);
//LION start : my added code
		if(tcb_pt->my_send_rst_flag == SET){
			seg.rst_flag=1;
		}
//LION end
		htontcp(&seg,&mr1,tcb_pt->conn.local.address,tcb_pt->conn.remote.address);
#ifdef DEBUG_ACT
		prnt_string("tcp_out:112\r\n");
		print_mr(mr1);
#endif
		/* If we're sending some data or flags, start retransmission
		 * and round trip timers if they aren't already running.
		 */
		if(ssize != 0){
			/* Set round trip timer. */
			rto = backoff(tcb_pt->backoff) * (4 * tcb_pt->mdev + tcb_pt->srtt);
#if 0
			set_timer(&tcb_pt->timer,max(MIN_RTO,rto));
			if(!run_timer(&tcb_pt->timer))
				start_timer(&tcb_pt->timer);
#endif // 0
//LION start
			tcb_pt->next_timer_count=change_to_kiatel_count(max(MIN_RTO,rto));
//SHARAB start
			if(tcb_pt->timer_no < TIMER_NO ){
				stop_tcp_timer(tcb_no);
			}
			start_tcp_timer(tcb_no,tcb_pt->next_timer_count);
//SHARAB end
//LION end

			/* If round trip timer isn't running, start it */
			if((tcb_pt->ts_ok_flag) || (!tcb_pt->rtt_run_flag)){
				tcb_pt->rtt_run_flag = 1;
				tcb_pt->rtt_time = msclock();
				tcb_pt->rttseq = tcb_pt->snd_ptr;
				tcb_pt->rttack = tcb_pt->snd_una;
			}
		}
		if(tcb_pt->retran_flag)
			Tcp_mib[tcpRetransSegs].value++;
		else
			Tcp_mib[tcpOutSegs].value++;

#ifdef DEBUG_ACT
		prnt_string("ip_send from tcp\r\n");
		prnt_int(tcb_pt->conn.local.address>>16);prnt_int(tcb_pt->conn.local.address);prnt_char('-');
		prnt_int(tcb_pt->conn.remote.address>>16);prnt_int(tcb_pt->conn.remote.address);prnt_char(CR);
		print_mr(mr1);
#endif
		ip_send(tcb_pt->conn.local.address,tcb_pt->conn.remote.address,
		 TCP_PTCL,tcb_pt->tos,0,mr1,0,0);
//LION start : my added code
		if(tcb_pt->my_send_rst_flag == SET){
			my_release_tcp(tcb_no);
		}
//LION end
	}
#ifdef DEBUG_ACT
	prnt_string("tcp_output end\r\n");
#endif
}
#endif // TCP_INSTALLED
