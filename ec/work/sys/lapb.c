
// tmprx can be a local variable.to be corrected.
// event can be local too !

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else  // CENTRAL_EXTERN
#include "hdr.h"
#if (OS == DOS)
#include <dos.h>
#endif

#ifdef FIFO_RD_WR
extern char *mess[];
#endif

#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#ifdef TCP_DEBUG_ACT
extern FILE *tcp_fer;
extern unsigned int	tcp_reopen_cnt;
#endif
#endif

#if(((defined OPERATOR_SIDE) && (defined LAPB_ON_OPERATOR_SIDE)) || (defined SYSTEM_SIDE))
#ifdef LAPB

#ifdef KIATEL_DYNAMIC_MEM
extern MEM_REC_ST mem_rec[];
#endif

extern BYTE hwtx[NO_LAPB_CH][HWBUF_SIZE];
extern BYTE hwrx[NO_LAPB_CH][HWBUF_SIZE];

extern BUF hwrx_st[];
extern BUF hwtx_st[];

extern BYTE sync_ok_count[];
extern BYTE sync_loss_count[];
extern BYTE sync_flag[];
extern BYTE lapb_link_flag[];

extern BYTE lapb_dbg_flg;
extern HDLC_ST hdlc_ch[];
#ifdef HDLC_RS232
extern SRL_PORT_ST srl_port[];
#endif
extern BYTE tst_flg;
/* it is necessary to have a temp receive array for counting receiving	   */
/* bytes because size of packet is the first byte that should be written   */
/* to hwrx() */

extern BYTE partial_packet[NO_LAPB_CH][MAX_PACKET_SIZE];
extern BYTE partial_packet_indx[];

#ifdef SYSTEM_SIDE
#if  (defined CEPT_INSTALLED)
extern DIGITAL_TRUNK_RECORD dtr[];
#endif

#ifdef RSU_CCSV5_INSTALLED
extern RSU_STAT_TAB rsu_st[];
#endif
#endif

extern BYTE hwrx_lapb_scn_no;
extern BYTE hwtx_lapb_scn_no;
extern BYTE link_lapb_scn_no;
#endif // LAPB
#endif // (((OPERATOR_SIDE) && (LAPB_ON_OPERATOR_SIDE)) || (SYSTEM_SIDE))

#endif // CENTRAL_EXTERN

#ifdef LAPB_DEBUG_ACT
#define DEBUG_ACT
#endif

#ifdef LAPB_INTERFRAME_TIME_FILL_WITH_FLAG
#define INTERFRAME_TIME_FILL_FOR_9076 0x04
#define INTERFRAME_TIME_FILL_FOR_9075 0x04
#define INTERFRAME_TIME_FILL_FOR_8952 0x04
#else
#define INTERFRAME_TIME_FILL_FOR_9076 0x00
#define INTERFRAME_TIME_FILL_FOR_9075 0x00
#define INTERFRAME_TIME_FILL_FOR_8952 0x00
#endif

#if(((defined OPERATOR_SIDE) && (defined LAPB_ON_OPERATOR_SIDE)) || (defined SYSTEM_SIDE))
#ifdef LAPB
/***************************************************************************/

BYTE initialize_hwlayer(BYTE n){

#ifdef HDLC_RS232
	if (hdlc_ch[n].ic_type == IC_HDLC_RS232){
#ifndef SERIAL_INTERFACE_ACTIVE 	// CPU_EC512
		if (srl_port[(unsigned int)hdlc_ch[n].cs].act == RESET)
			sync_flag[n]=RESET;
		else
#endif
			sync_flag[n] = SET;
	}
	else
#endif
	{
		sync_flag[n] = RESET;
		sync_loss_count[n] = sync_ok_count[n] = 0;
	}
#ifdef LINKLAYER_DEBUG_ACT
	if (lapb_dbg_flg == SET){
	    prnt_char('h');prnt_char('w');prnt_char('l');prnt_char('a');prnt_char('y');
	    prnt_char('e');prnt_char('r');prnt_char(':');prnt_byte(n);
	    prnt_char('-');prnt_byte(hdlc_ch[n].ic_type);
	    prnt_char('-');prnt_byte(hdlc_ch[n].cs);
	    prnt_char('-');prnt_byte(sync_flag[n]);
	    prnt_char(CR);
	}
#endif
	return	(init_hdlc_ch(n));
}
/***************************************************************************/

void make_rx_fifo_empty(BYTE n){

#if ((defined HDLC_8952) || (defined HDLC_9075))
	BYTE i;
#endif
#ifdef DEBUG_ACT
//	prnt_msg(LAPB_ERROR,0x402b);
#endif

	switch (hdlc_ch[n].ic_type){
#ifdef HDLC_8952
	  case IC_HDLC_8952 :
			while (( rfifo_stat(hdlc_ch[n].cs) & 0x30) != 0){
#pragma warn -eff
				rrx_data(hdlc_ch[n].cs);
#pragma warn +eff
				for(i=0;i<10;i++);
				partial_packet_indx[n]=0;
			}
			break;
#endif
#ifdef HDLC_9075
	  case IC_HDLC_9075 :
			while( (rd_hdlc_st_9075(hdlc_ch[n].cs,hdlc_ch[n].dl_ch)&0x03) != 0){
				rd_hdlc_rx_fifo_9075(hdlc_ch[n].cs,hdlc_ch[n].dl_ch);
				for(i=0;i<10;i++);
			}
// alaki
			wr_hdlc_cntrl1_9075(hdlc_ch[n].cs,hdlc_ch[n].dl_ch,0x60);
			/* Adrec=0,RxEN=1,TxEN=1,EOP=0,FA=0,Mark/Idle\=0,RSV=0 */

//alaki
			wr_hdlc_cntrl2_9075(hdlc_ch[n].cs,hdlc_ch[n].dl_ch,0x82);
			/* Insel=1,Cycle=0,Tcrci=0,Seven=0,RSV=0,RSV=0,Rxfrst=1,Txfrst=0*/
			/* Rxfrst = Rx FIFO Reset=1 */
			partial_packet_indx[n]=0;
			break;
#endif
#ifdef HDLC_RS232
	  case IC_HDLC_RS232:
			make_rs232_rx_fifo_empty(n);
			break;
#endif
#ifdef HDLC_9076
	  case IC_HDLC_9076 :
			wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_2_ADDR_9076,hdlc_ch[n].cs,hdlc_ch[n].dl_ch,0x82);
			/* Insel=1,Cycle=0,Tcrci=0,Seven=0,RSV=0,RSV=0,Rxfrst=1,Txfrst=0*/
			partial_packet_indx[n]=0;
			break;
#endif
#ifdef HDLC_QUADE1_PEF22554
	  case IC_HDLC_QUADE1_PEF22554 :
			partial_packet_indx[n]=0;
			break;
#endif
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
	  case IC_HDLC_ON_UDP : // is not called for this case
			break;
#endif
	  default	    :
			prnt_msg(LAPB_ERROR,0x1002);
			break;
	}
}
/***************************************************************************/
#if 0
#if ((defined HDLC_9075) || (defined HDLC_9076) || (defined HDLC_QUADE1_PEF22554))
void make_tx_fifo_empty(BYTE n){

#ifdef HDLC_QUADE1_PEF22554
	unsigned int i;
#endif

	switch (hdlc_ch[n].ic_type){
#ifdef HDLC_9075
	  case IC_HDLC_9075	:
		wr_hdlc_cntrl2_9075(hdlc_ch[n].cs,hdlc_ch[n].dl_ch,0x81);
		/* Insel=1,Cycle=0,Tcrci=0,Seven=0,RSV=0,RSV=0,Rxfrst=0,Txfrst=1*/
		/* Rxfrst = Rx FIFO Reset=1 */
		return;
#endif
#ifdef HDLC_9076
	  case IC_HDLC_9076	:
		wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_2_ADDR_9076,hdlc_ch[n].cs,hdlc_ch[n].dl_ch,0x81);
		/* Insel=1,Cycle=0,Tcrci=0,Seven=0,RSV=0,RSV=0,Rxfrst=1,Txfrst=0*/
		return;
#endif
#ifdef HDLC_QUADE1_PEF22554
	  case IC_HDLC_QUADE1_PEF22554 :
//		tx_partial_packet_indx[n]=0;
		/* hdlc */
		wr_reg_quad_e1_pef22554(hdlc_ch[n].cs,hdlc_ch[n].e1_no,QUADE1_PEF22554_CMDR_CNTRL_REG,
			CMDR_RMC_RCV_MESSAGE_COMPLETE_D7_RESET	 |
			CMDR_RRES_RCVR_RESET_D6_RESET		 |
			CMDR_XREP_TRANSMISSION_REPEAT_D5_RESET	 |
			CMDR_XRES_XMITTER_RESET_D4_SET		 |
			CMDR_XHF_XMIT_HDLC_FRAME_D3_RESET	 |
			CMDR_XTF_XMIT_TRANSPARENT_FRAME_D2_RESET |
			CMDR_XME_XMIT_MESSAGE_END_D1_RESET	 |
			CMDR_SRES_SIGNALING_XMITTER_RESET_D0_RESET);
		for(i=0;i<1000;i++);
		wr_reg_quad_e1_pef22554(hdlc_ch[n].cs,hdlc_ch[n].e1_no,QUADE1_PEF22554_CMDR_CNTRL_REG,
			CMDR_RMC_RCV_MESSAGE_COMPLETE_D7_RESET	 |
			CMDR_RRES_RCVR_RESET_D6_RESET		 |
			CMDR_XREP_TRANSMISSION_REPEAT_D5_RESET	 |
			CMDR_XRES_XMITTER_RESET_D4_RESET	 |
			CMDR_XHF_XMIT_HDLC_FRAME_D3_RESET	 |
			CMDR_XTF_XMIT_TRANSPARENT_FRAME_D2_RESET |
			CMDR_XME_XMIT_MESSAGE_END_D1_RESET	 |
			CMDR_SRES_SIGNALING_XMITTER_RESET_D0_RESET);
		return;
#endif
	  default		:
		prnt_msg(LAPB_ERROR,0x1008);
		return;
	}
}
#endif
#endif //0
/***************************************************************************/

/***************************************************************************/

#ifdef HDLC_8952

void check_8972_sync(BYTE n){

	BYTE status;

	status = rcch_rx(hdlc_ch[n].cs);
	if(status & 0x01){	    /* SYNC detected */
		if (!sync_flag[n]){
			if (sync_ok_count[n]++ == SYNC_OK_LIMIT){
				sync_flag[n] = TRUE;
				sync_ok_count[n] = 0;
			}
		}
		return ;
	}
	else{			    /* SYNC not detected */
		if (sync_flag[n]){
			if (sync_loss_count[n]++ == SYNC_LOSS_LIMIT){
				sync_flag[n] = FALSE;
				sync_loss_count[n] = 0;
			}
		}
		return ;
	}
}
#endif
/***************************************************************************/
BYTE init_hdlc_ch(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
	if (lapb_dbg_flg == SET){
	    prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(':');prnt_byte(n);prnt_char(CR);
	}
#endif

	switch ( hdlc_ch[n].ic_type){
#ifdef HDLC_8952
	  case IC_HDLC_8952 :
				return(init_HDLC_and_8972(n));
#endif
#ifdef HDLC_9075
	  case IC_HDLC_9075 :
				return(init_hdlc_9075(n));
#endif
#ifdef HDLC_RS232
	  case IC_HDLC_RS232:
				return(init_hdlc_rs232(n));
#endif
#ifdef HDLC_9076
	  case IC_HDLC_9076 :
				return(init_hdlc_9076(n));
#endif
#ifdef HDLC_QUADE1_PEF22554
	  case IC_HDLC_QUADE1_PEF22554 :
				return(init_hdlc_pef22554(n));
#endif
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
	  case IC_HDLC_ON_UDP :
				return(init_hdlc_on_udp(n));
#endif
	  case IC_HDLC_NOT_INSTALLED:
				return OK;
	  default	    :
				prnt_msg(LAPB_ERROR,0x1003);
				prnt_byte(n);prnt_char('-');
				prnt_byte(hdlc_ch[n].ic_type);prnt_char(CR);
#ifdef SYSTEM_SIDE
				print_call_frame();
#endif
				return FAIL;
	}

}
/***************************************************************************/
#ifdef HDLC_8952
BYTE init_HDLC_and_8972(BYTE n){

	BYTE i;
	wtime_cntrl(hdlc_ch[n].cs,0x80);  /* RST active */
	for(i=0;i<3;i++);
	wtime_cntrl(hdlc_ch[n].cs,0x80);  /* RST active */
	for(i=0;i<3;i++);

	/* RST inactive,internal control,c_ch enable,ch 2 HDLC */

	wtime_cntrl(hdlc_ch[n].cs,0x63);
	for(i=0;i<3;i++);
	wtime_cntrl(hdlc_ch[n].cs,0x63);
	for(i=0;i<3;i++);
	wtime_cntrl(hdlc_ch[n].cs,0x63);
	for(i=0;i<3;i++);

	wint_en(hdlc_ch[n].cs,0x41);	 /* Enable EOPD and Rxfull interrupts*/
	for(i=0;i<3;i++);
	wcch_tx(hdlc_ch[n].cs,0x28);	 /* 8972 : select control register,diagnostic */
			      /* register reset,160Kbit/s,attack=0,HK=0    */

	for(i=0;i<3;i++);
	wcntrl(hdlc_ch[n].cs,0xc0 | INTERFRAME_TIME_FILL_FOR_8952);	 /* RxAD disable,idle, rcvr,xmtr enable */
	for(i=0;i<3;i++);
	make_rx_fifo_empty(n);
#if ((NO_OPRTR == 1) || (NO_ACTIVE_OPR == 1))
	wtime_cntrl(CS8952_1,0x80);
	wtime_cntrl(CS8952_1,0x80);
	wtime_cntrl(CS8952_1,0x63);  /* RST inactive,internal timing */
	wtime_cntrl(CS8952_1,0x63);  /* 4096 kbps,c_ch enable,ch 2 HDLC */
	wtime_cntrl(CS8952_1,0x63);
	wint_en(CS8952_1,0xfd);  /* Enable all interrupts except RX 15/19 */
//	wrx_addrs(CS8952_1,opr[n].na);
	wcch_tx(CS8952_1,0x28);  /* attack disable */
	wcntrl(CS8952_1,0xe0);	 /* RxAD,idle, rcvr,xmtr enable */
#endif
	partial_packet_indx[n]=0;
	return OK;
}
#endif
/****************************************************************************/
#ifdef HDLC_9075
BYTE init_hdlc_9075(BYTE n){

	 HDLC_ST  *pt;
	 BYTE i;

	 pt=&hdlc_ch[n];

	if ( rd_ms1_id_9075(pt->cs) != ID_CODE_9075){
		return FAIL;
	}

	 wr_mc1_mf_nbit_dl_slct_9075(pt->cs,0x00);
	/* ASEL=0,MFSEL=0,NBTB=0,Sa4-Sa8=11111 */
	/* Sa4-Sa8=11111 selects the Sa bits of the NFA signal for 4,8,12,16
	   or 20 kbit/sec data link channel. */
	if (pt->dl_ch == HDLC0_9075)
		wr_mc1_hdlc_slct_9075(pt->cs,HDLC0_9075_SEL|0x00);
		/* HDLC0=1,HDLC1=0,RxTRSP=0,UNUSED=00000*/
	else
		wr_mc1_hdlc_slct_9075(pt->cs,HDLC1_9075_SEL|0x00);
		/* HDLC0=0,HDLC1=1,RxTRSP=0,UNUSED=00000*/

	wr_mc1_int_sgnl_bert_9075(pt->cs,0xd8);
	/* ODE=1,SPND=1,INTA=0,TxCCS=1,RPSIG=1,CNTCLR=0,MSN=0,64KCpt->cs=0*/

	/* HDLC RESET */

	wr_hdlc_test_cntrl_9075(pt->cs,pt->dl_ch,0x80);
	/* HDLC Reset=1,RTloop=0,RSV=0,RSV=0,RSV=0,Ftst=0,RSV=0,Hloop=0*/
	for(i=0;i<50;i++);
	wr_hdlc_test_cntrl_9075(pt->cs,pt->dl_ch,0x00);
	/* HDLC Reset=0,RTloop=0,RSV=0,RSV=0,RSV=0,Ftst=0,RSV=0,Hloop=0*/

	/* HDLC RESET END */

	wr_hdlc_addr_rcgn1_9075(pt->cs,pt->dl_ch,0x00);
	/* Adr16-AdR11=000000,Adr10=0,A1en=0 */

	wr_hdlc_addr_rcgn2_9075(pt->cs,pt->dl_ch,0x00);
	/* Adr26-Adr20=0000000,A2en=0 */

	wr_hdlc_cntrl1_9075(pt->cs,pt->dl_ch,0x60 | INTERFRAME_TIME_FILL_FOR_9075);
	/* Adrec=0,RxEN=1,TxEN=1,EOP=0,FA=0,Mark/Idle\=0,RSV=0 */


	wr_hdlc_cntrl2_9075(pt->cs,pt->dl_ch,0x83);
	/*INTsel=1,Cycle=1,TCRCi=0,Seven=1,RSV=0,RSV=0,RXfRST=0,TXfRST=0*/

	wr_hdlc_int_msk_9075(pt->cs,pt->dl_ch,0x41);
	/*Ga=0,EOPD=1,TEOP=0,EOPR=0,TxFI=0,FA=0,TxUNDER=0,rXFf=0,rXOvfl=1*/

	wr_hdlc_tx_byte_cnt_9075(pt->cs,pt->dl_ch,0x0);
	/* Cnt7-0 = 0 (count is not used) */

	wr_hdlc_test_cntrl_9075(pt->cs,pt->dl_ch,0x00);
	/* HDLC Reset=0,RTloop=0,RSV=0,RSV=0,RSV=0,Ftst=0,RSV=0,Hloop=0*/

	wr_hdlc_cntrl3_9075(pt->cs,pt->dl_ch,0x77);
	/* BIT7:unsed=0 ,RFD2-0 :111 ,BIT3:unused=0,TFD2-0:111 */

	wr_hdlc_cntrl4_9075(pt->cs,pt->dl_ch,0x77);
	/* BIT7:unsed ,RFFS2-0 :111 ,BIT3:unused,TFLS2-0: */


	partial_packet_indx[n]=0;
	return OK;
}
#endif
/****************************************************************************/
#ifdef HDLC_9076
BYTE init_hdlc_9076(BYTE n){
	 HDLC_ST  *pt;
	 BYTE i;

	 pt=&hdlc_ch[n];

	if ( rd_ms1_9076(MS1_IDENTIFICATION_REGISTER_ADDR_9076,pt->cs) != IDENTIFICATION_WORD_9076){
		return FAIL;
	}
	wr_mc1_9076(MC1_MULTIFRAME_AND_DATA_LINK_SELECTION_ADDR_9076,pt->cs,0x00);
	/* bit7:unused,MFSEL=0,NBTB=0,SA4-SA8=00000			*/

//????? wr_mc2_9076(MC2_HDLC0_SELECT_ADDR_9076,pt->cs,0xd0|related_ts_to_this_hdlc[0]);

//????? wr_mc1_int_sgnl_bert_9076(pt->cs,0xd8);
	/* ODE=1,SPND=1,INTA=0,TxCCS=1,RPSIG=1,CNTCLR=0,MSN=0,64KCpt->cs=0*/

	/* HDLC RESET */
	wr_hdlc_cntrl_9076(HDLC_CNTRL_TEST_CONTROL_ADDR_9076,pt->cs,pt->dl_ch,0x80);
	/* Hdlc ReSeT=1,RTloop=0,RSV=0,RSV=0,CRCTST=0,FTST=0,ARTST=0,Hloop=0*/
	for(i=0;i<50;i++);
	wr_hdlc_cntrl_9076(HDLC_CNTRL_TEST_CONTROL_ADDR_9076,pt->cs,pt->dl_ch,0x00);
	/* Hdlc ReSeT=1,RTloop=0,RSV=0,RSV=0,CRCTST=0,FTST=0,ARTST=0,Hloop=0*/
	/* HDLC RESET END */

	wr_hdlc_cntrl_9076(HDLC_CNTRL_ADDR_RECOGNITION_1_ADDR_9076,pt->cs,pt->dl_ch,0x00);
	/* Adr16-AdR11=000000,Adr10=0,A1en=0 */

	wr_hdlc_cntrl_9076(HDLC_CNTRL_ADDR_RECOGNITION_2_ADDR_9076,pt->cs,pt->dl_ch,0x00);
	/* Adr26-Adr20=0000000,A2en=0 */

	wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_1_ADDR_9076,pt->cs,pt->dl_ch,0x60 | INTERFRAME_TIME_FILL_FOR_9076);
	/* Adrec=0,RxEN=1,TxEN=1,EOP=0,FA=0,Mark/Idle\=0,TR=0,FRUN=0	*/

	wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_2_ADDR_9076,pt->cs,pt->dl_ch,0x83);
	/*INTsel=1,Cycle=1,TCRCi=0,Seven=1,RSV=0,RSV=0,RXFRST=0,TXFRST=0*/

	wr_hdlc_cntrl_9076(HDLC_CNTRL_INTERRUPT_MASK_ADDR_9076,pt->cs,pt->dl_ch,0x41);
	/*Ga=0,EOPD=1,TEOP=0,EOPR=0,TxFI=0,FA=0,TxUNDER=0,rXFf=0,rXOvfl=1*/

	wr_hdlc_cntrl_9076(HDLC_CNTRL_LOW_TX_BYTE_COUNT_ADDR_9076,pt->cs,pt->dl_ch,0x00);
	/* Cnt7-0 = 0 (count is not used) */
//chand baar ???
	wr_hdlc_cntrl_9076(HDLC_CNTRL_TEST_CONTROL_ADDR_9076,pt->cs,pt->dl_ch,0x00);
	/* Hdlc ReSeT=1,RTloop=0,RSV=0,RSV=0,CRCTST=0,FTST=0,ARTST=0,Hloop=0*/

	wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_3_ADDR_9076,pt->cs,pt->dl_ch,0x77);
	/* BIT7:unsed=0 ,RFD2-0 :111 ,BIT3:unused=0,TFD2-0:111 */

	wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_4_ADDR_9076,pt->cs,pt->dl_ch,0x77);
	/* BIT7:unsed ,RFFS2-0 :111 ,BIT3:unused,TFLS2-0: */

	partial_packet_indx[n]=0;
	return OK;
}
#endif
/****************************************************************************/
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
BYTE init_hdlc_on_udp(BYTE n){
/*
	HDLC_ST  *pt;
	SOCKET_STRUCT lsocket;
	guint ucb_no;

	pt=&hdlc_ch[n];

#ifdef SYSTEM_SIDE
#ifdef HOT_STANDBY
	if (rd_master_swch() == RESET)
	    lsocket.address=
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 92 <<  0);
	else
#endif
	    lsocket.address=
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 91 <<  0);
#endif
#ifdef OPERATOR_SIDE
#if (TON == 0)
	lsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 95 <<  0);
#endif
#if (TON == 1)
	lsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 96 <<  0);
#endif
#if (TON == 2)
	lsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 97 <<  0);
#endif
#if (TON == 3)
	lsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 98 <<  0);
#endif
#endif
	lsocket.port=3490+pt->dl_ch;

	if((ucb_no = lookup_ucb(&lsocket)) < MAX_UCB){
		// already exist, just ignore
		return OK;
	}

	ucb_no=start_udp(&lsocket);
	if(ucb_no >= MAX_UCB){
		prnt_msg(LAPB_ERROR,0x5807);
	}
	else{
		pt->cs=ucb_no;
	}
*/
	return OK;
}
#endif
/****************************************************************************/
BYTE transmit_packet(BYTE n){

	switch (hdlc_ch[n].ic_type){
#ifdef HDLC_8952
	  case IC_HDLC_8952 :
				return(transmit_packet_8952(n));
#endif
#ifdef HDLC_9075
	  case IC_HDLC_9075 :
				return(transmit_packet_9075(n));
#endif
#ifdef HDLC_RS232
	  case IC_HDLC_RS232:
				return(transmit_packet_rs232(n));
#endif
#ifdef HDLC_9076
	  case IC_HDLC_9076 :
				return(transmit_packet_9076(n));
#endif
#ifdef HDLC_QUADE1_PEF22554
	  case IC_HDLC_QUADE1_PEF22554 :
				return(transmit_packet_pef22554(n));
#endif
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
	  case IC_HDLC_ON_UDP :
				return(transmit_packet_on_udp(n));
#endif
	  case IC_HDLC_NOT_INSTALLED:
				return OK;
	  default	    :
				prnt_msg(LAPB_ERROR,0x1004);
				return FAIL;
	}
}
/****************************************************************************/
#ifdef HDLC_8952
BYTE transmit_packet_8952(BYTE n){
/* note : only one packet is sent with each timer interrupt.*/
	BYTE status,dat,size,j;
	HDLC_ST *pt;
	BYTE total_size=0;
#ifdef DEBUG_ACT
//	prnt_msg(LAPB_RTN,0x100);
#endif
	pt=&hdlc_ch[n];
	status = (BYTE)rfifo_stat(pt->cs);
	if( (status & 0x0c) != 0x08){  /* if Tx FIFO 8952 not empty */
		prnt_msg(LAPB_ERROR,0x4011);
		prnt_byte(n);prnt_char('-');
		prnt_int((int)pt->cs);prnt_char('-');
		prnt_int(pt->dl_ch);prnt_char('-');
		prnt_byte(status);prnt_char(CR);
		return FAIL;
	}

send_again:

#ifdef CS_CORRECTION
	if (get_buf(&hwtx_st[n],&size) == OK){
		BYTE dati[100],cs_byte1,bit_pos1,cs_byte2,bit_pos2,chksum;
		cs_byte1=0;
		bit_pos1=0x01;
		cs_byte2=0;
		bit_pos2=0x01;
		chksum=0;
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
		prnt_char('T'); prnt_char('x');prnt_char('-');prnt_byte(n);
		prnt_char(':');
#endif
		for(j=0;j<size;j++){  /* last byte after setting EOP */
			if(j >= 100){
				prnt_msg(LAPB_ERROR,0x4016);
				return FAIL;
			}
			if ( get_buf(&hwtx_st[n],&dati[j]) != OK){
				prnt_msg(LAPB_ERROR,0x4015);
				return FAIL;
			}
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
			prnt_byte(dati[j]);prnt_char('-');
#endif
			dat=dati[j];
			chksum=chksum+dat;
			if (j>2){
				if((dat & 0xf0) == 0xf0){
					dat = dat & 0x0f;
					cs_byte1 = cs_byte1 | bit_pos1;
				}
				bit_pos1=bit_pos1<<1;
				if((dat & 0x0f) == 0x0f){
					dat = dat & 0xf0;
					cs_byte2 = cs_byte2 | bit_pos2;
				}
				bit_pos2=bit_pos2<<1;
			}
		}
		chksum = chksum + cs_byte1 + cs_byte2;
		if ( (chksum & 0xf0) == 0xf0){
			chksum = chksum & 0x0f;
			cs_byte1 = cs_byte1 | bit_pos1;
		}
		if ( (chksum & 0x0f) == 0x0f){
			chksum = chksum & 0xf0;
			cs_byte2 = cs_byte2 | bit_pos2;
		}
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
//		prnt_byte(chksum);prnt_char('-');
//		prnt_byte(cs_byte1);prnt_char('-');
//		prnt_byte(cs_byte2);
		prnt_char(CR);
#endif
		for(j=0;j<size;j++){  /* last byte after setting EOP */
			wtx_data(pt->cs,dati[j]);
		}
		wtx_data(pt->cs,chksum);
		wtx_data(pt->cs,cs_byte1);
		wcntrl(pt->cs,0xc1 | INTERFRAME_TIME_FILL_FOR_8952);  /* set EOP */
		wtx_data(pt->cs,cs_byte2);
		wcntrl(pt->cs,0xc0 | INTERFRAME_TIME_FILL_FOR_8952);/* reset EOP,enable tx */
		size+=3;
		goto return_ok;
	}
	else{
		return OK; /* no data for transmission */
	}

#else  /* #ifdef CS_CORRECTION */
	if (get_buf(&hwtx_st[n],&size) == OK){

		for(j=0;j<size-1;j++){	/* last byte after setting EOP */
			if ( get_buf(&hwtx_st[n],&dat) == OK){
				wtx_data(pt->cs,dat);
			}
			else{
				prnt_msg(LAPB_ERROR,0x4016);
				/* transmitted anyway.receiver will ignore it.(bad frame)*/
				return FAIL;
			}
		}
		wcntrl(pt->cs,0xc1 | INTERFRAME_TIME_FILL_FOR_8952);  /* set EOP */
		get_buf(&hwtx_st[n],&dat);

		wtx_data(pt->cs,dat);
		wcntrl(pt->cs,0xc0 | INTERFRAME_TIME_FILL_FOR_8952);  /* reset EOP,enable tx */
		goto return_ok;
	}
	else{
		return OK; /* no data for transmission */
	}
#endif	 /* #ifdef CS_CORRECTION */
return_ok:

#ifdef RSU_CCSV5_INSTALLED
#ifdef RSUSIDE
	if(n < NO_CCSV5_TOWARDS_RSU_GROUP)
#endif
	{
	    if(rsu_st[n].has_fast_lapb == RESET)
		return OK;
	}
#endif
	total_size+=size;
	if((total_size+check_size_of_next_packet(&hwtx_st[n])) <= 16) // leave space for possible 3 bytes (CS_CORRECTION)
	    goto send_again;
	else
	    return OK;
}
#endif // HDLC_8952
/****************************************************************************/
/****************************************************************************/
#ifdef HDLC_9075
BYTE transmit_packet_9075(BYTE n){

/* note : only one packet is sent with each timer interrupt.*/
	BYTE status,dat,size,j;
#ifdef CS_CORRECTION
	BYTE cs_byte1,bit_pos1;
	BYTE cs_byte2,bit_pos2;
	BYTE chksum;
#endif
	HDLC_ST *pt;

#ifdef DEBUG_ACT
	prnt_msg(LAPB_RTN,0x100);
#endif


	pt=&hdlc_ch[n];
///	status = (BYTE)rfifo_stat(pt->cs);
	status = rd_hdlc_st_9075(pt->cs,pt->dl_ch);

	if( (status & 0x0c) != 0x08){  /* if Tx FIFO 9075 not empty */

		prnt_msg(LAPB_ERROR,0x4011);
		prnt_byte(status);prnt_char(CR);
#ifdef DEBUG_ACT
//		prnt_byte(n);prnt_char('-');
//		prnt_int(pt->cs);
#endif
		return FAIL;
	}

#ifdef CS_CORRECTION

	if (get_buf(&hwtx_st[n],&size) == OK){
#ifdef DEBUG_ACT
//		prnt_char('S'); /*prnt_char('i'); prnt_char('z');prnt_char('e');*/
//		prnt_char(':'); prnt_byte(size); /*prnt_char(CR);*/
#endif

		cs_byte1=0;
		bit_pos1=0x01;
		cs_byte2=0;
		bit_pos2=0x01;
		chksum=0;
		for(j=0;j<size;j++){  /* last byte after setting EOP */
			if ( get_buf(&hwtx_st[n],&dat) == OK){
#ifdef DEBUG_ACT
//				prnt_char('T'); prnt_char('x');
//				prnt_char(':'); prnt_byte(dat);
#endif
				chksum=chksum+dat;
				if (j>2){
					if((dat & 0xf0) == 0xf0){
						dat = dat & 0x0f;
						cs_byte1 = cs_byte1 | bit_pos1;
					}
					bit_pos1=bit_pos1<<1;
					if((dat & 0x0f) == 0x0f){
						dat = dat & 0xf0;
						cs_byte2 = cs_byte2 | bit_pos2;
					}
					bit_pos2=bit_pos2<<1;
				}

///				wtx_data(pt->cs,dat);
				wr_hdlc_tx_fifo_9075(pt->cs,pt->dl_ch,dat);

			}
			else{
				prnt_msg(LAPB_ERROR,0x4012);
				/* transmitted anyway.receiver will ignore it.(bad frame)*/

				return FAIL;
			}
		}
		chksum = chksum + cs_byte1 + cs_byte2;
		if ( (chksum & 0xf0) == 0xf0){
			chksum = chksum & 0x0f;
			cs_byte1 = cs_byte1 | bit_pos1;
		}
		if ( (chksum & 0x0f) == 0x0f){
			chksum = chksum & 0xf0;
			cs_byte2 = cs_byte2 | bit_pos2;
		}
#ifdef DEBUG_ACT
//		prnt_char('T'); prnt_char('x'); prnt_char('D');prnt_char('t');
//		prnt_char(':'); prnt_byte(chksum); prnt_char(CR);
#endif

///		wtx_data(pt->cs,chksum);
		wr_hdlc_tx_fifo_9075(pt->cs,pt->dl_ch,chksum);
#ifdef DEBUG_ACT
//		prnt_char('T'); prnt_char('x'); prnt_char('C');prnt_char('1');
//		prnt_char(':'); prnt_byte(cs_byte1); prnt_char(CR);
#endif

///		wtx_data(pt->cs,cs_byte1);
		wr_hdlc_tx_fifo_9075(pt->cs,pt->dl_ch,cs_byte1);

///		wcntrl(pt->cs,0xc1);  /* set EOP */
		wr_hdlc_cntrl1_9075(pt->cs,pt->dl_ch,0x70 | INTERFRAME_TIME_FILL_FOR_9075); /* set EOP */
#ifdef DEBUG_ACT
//		prnt_char('T'); prnt_char('X'); prnt_char('C');prnt_char('2');
//		prnt_char(':'); prnt_byte(cs_byte2); prnt_char(CR);
#endif

///		wtx_data(pt->cs,cs_byte2);
		wr_hdlc_tx_fifo_9075(pt->cs,pt->dl_ch,cs_byte2);

///		wcntrl(pt->cs,0xc0);/* reset EOP,enable tx */
		wr_hdlc_cntrl1_9075(pt->cs,pt->dl_ch,0x60 | INTERFRAME_TIME_FILL_FOR_9075); /* reset EOP */

		return OK;
	}
	else{
		return OK; /* no data for transmission */
	}


#else  /* #ifdef CS_CORRECTION */
	if (get_buf(&hwtx_st[n],&size) == OK){

		for(j=0;j<size-1;j++){	/* last byte after setting EOP */
			if ( get_buf(&hwtx_st[n],&dat) == OK){
///				wtx_data(pt->cs,dat);
				wr_hdlc_tx_fifo_9075(pt->cs,pt->dl_ch,dat);
			}
			else{
				/* transmitted anyway.receiver will ignore it.(bad frame)*/
				prnt_msg(LAPB_ERROR,0x4013);
				return FAIL;
			}
		}
///		wcntrl(pt->cs,0xc1);  /* set EOP */
		wr_hdlc_cntrl1_9075(pt->cs,pt->dl_ch,0x70 | INTERFRAME_TIME_FILL_FOR_9075); /* set EOP */
		get_buf(&hwtx_st[n],&dat);

///		wtx_data(pt->cs,dat);
		wr_hdlc_tx_fifo_9075(pt->cs,pt->dl_ch,dat);

///		wcntrl(pt->cs,0xc0);  /* reset EOP,enable tx */
		wr_hdlc_cntrl1_9075(pt->cs,pt->dl_ch,0x60 | INTERFRAME_TIME_FILL_FOR_9075); /* reset EOP */

		return OK;
	}
	else{
		return OK; /* no data for transmission */
	}
#endif	 /* #ifdef CS_CORRECTION */
}
#endif // HDLC_9075
/****************************************************************************/
#ifdef HDLC_9076
BYTE transmit_packet_9076(BYTE n){

	BYTE status,dat,size,j;
	BYTE total_size=0;
#ifdef CS_CORRECTION
	BYTE cs_byte1,bit_pos1;
	BYTE cs_byte2,bit_pos2;
	BYTE chksum;
#endif
	HDLC_ST *pt;

#ifdef DEBUG_ACT
//	prnt_msg(LAPB_RTN,0x100);
#endif
	pt=&hdlc_ch[n];
	status = rd_hdlc_status_9076(HDLC_STATUS_HDLC_STATUS_ADDR_9076,pt->cs,pt->dl_ch);
	if( (status & 0x0c) != 0x08){  /* if Tx FIFO 9076 not empty */
		prnt_msg(LAPB_ERROR,0x4011);
		prnt_byte(n);prnt_char('-');
		prnt_int(pt->cs);prnt_char('-');
		prnt_int(pt->dl_ch);prnt_char('-');
		prnt_byte(status);prnt_char(CR);
		return FAIL;
	}
send_again:
#ifdef CS_CORRECTION
	if (get_buf(&hwtx_st[n],&size) == OK){
#ifdef DEBUG_ACT
//		prnt_char('S'); /*prnt_char('i'); prnt_char('z');prnt_char('e');*/
//		prnt_char(':'); prnt_byte(size); /*prnt_char(CR);*/
#endif
		cs_byte1=0;
		bit_pos1=0x01;
		cs_byte2=0;
		bit_pos2=0x01;
		chksum=0;
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
		prnt_char('T'); prnt_char('x');prnt_char('-');prnt_byte(n);
		prnt_char(':');
#endif
		for(j=0;j<size;j++){  /* last byte after setting EOP */
			if ( get_buf(&hwtx_st[n],&dat) == OK){
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
				prnt_byte(dat);prnt_char('-');
#endif
				chksum=chksum+dat;
				if (j>2){
					if((dat & 0xf0) == 0xf0){
						dat = dat & 0x0f;
						cs_byte1 = cs_byte1 | bit_pos1;
					}
					bit_pos1=bit_pos1<<1;
					if((dat & 0x0f) == 0x0f){
						dat = dat & 0xf0;
						cs_byte2 = cs_byte2 | bit_pos2;
					}
					bit_pos2=bit_pos2<<1;
				}
				wr_hdlc_cntrl_9076(HDLC_CNTRL_TX_FIFO_ADDR_9076,pt->cs,pt->dl_ch,dat);
			}
			else{
				prnt_msg(LAPB_ERROR,0x4012);
				/* transmitted anyway.receiver will ignore it.(bad frame)*/
				return FAIL;
			}
		}
		chksum = chksum + cs_byte1 + cs_byte2;
		if ( (chksum & 0xf0) == 0xf0){
			chksum = chksum & 0x0f;
			cs_byte1 = cs_byte1 | bit_pos1;
		}
		if ( (chksum & 0x0f) == 0x0f){
			chksum = chksum & 0xf0;
			cs_byte2 = cs_byte2 | bit_pos2;
		}
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
//		prnt_byte(chksum);prnt_char('-');
#endif
		wr_hdlc_cntrl_9076(HDLC_CNTRL_TX_FIFO_ADDR_9076,pt->cs,pt->dl_ch,chksum);
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
//		prnt_byte(cs_byte1);prnt_char('-');
#endif
		wr_hdlc_cntrl_9076(HDLC_CNTRL_TX_FIFO_ADDR_9076,pt->cs,pt->dl_ch,cs_byte1);
		wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_1_ADDR_9076,pt->cs,pt->dl_ch,0x70 | INTERFRAME_TIME_FILL_FOR_9076); /* set EOP */
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
//		prnt_byte(cs_byte2);
		prnt_char(CR);
#endif
		wr_hdlc_cntrl_9076(HDLC_CNTRL_TX_FIFO_ADDR_9076,pt->cs,pt->dl_ch,cs_byte2);
		wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_1_ADDR_9076,pt->cs,pt->dl_ch,0x60 | INTERFRAME_TIME_FILL_FOR_9076); /* reset EOP */
		size+=3;
		goto return_ok;
	}
	else{
		return OK; /* no data for transmission */
	}
#else  /* #ifdef CS_CORRECTION */
	if (get_buf(&hwtx_st[n],&size) == OK){
		for(j=0;j<size-1;j++){	/* last byte after setting EOP */
			if ( get_buf(&hwtx_st[n],&dat) == OK){
				wr_hdlc_cntrl_9076(HDLC_CNTRL_TX_FIFO_ADDR_9076,pt->cs,pt->dl_ch,dat);
			}
			else{
				/* transmitted anyway.receiver will ignore it.(bad frame)*/
				prnt_msg(LAPB_ERROR,0x4013);
				return FAIL;
			}
		}
		wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_1_ADDR_9076,pt->cs,pt->dl_ch,0x70 | INTERFRAME_TIME_FILL_FOR_9076); /* set EOP */
		get_buf(&hwtx_st[n],&dat);
		wr_hdlc_cntrl_9076(HDLC_CNTRL_TX_FIFO_ADDR_9076,pt->cs,pt->dl_ch,dat);
		wr_hdlc_cntrl_9076(HDLC_CNTRL_HDLC_CONTROL_1_ADDR_9076,pt->cs,pt->dl_ch,0x60 | INTERFRAME_TIME_FILL_FOR_9076); /* reset EOP */
		goto return_ok;
	}
	else{
		return OK; /* no data for transmission */
	}
#endif	 /* #ifdef CS_CORRECTION */

return_ok:

#ifdef RSU_CCSV5_INSTALLED
#ifdef RSUSIDE
	if(n < NO_CCSV5_TOWARDS_RSU_GROUP)
#endif
	{
	    if(rsu_st[n].has_fast_lapb == RESET)
		return OK;
	}
#endif
	total_size+=size;
	if((total_size+check_size_of_next_packet(&hwtx_st[n])) < 100) //125, leave space for possible 3 bytes (CS_CORRECTION)
	    goto send_again;
	else
	    return OK;
}
#endif // HDLC_9076
/****************************************************************************/
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
#if ((defined OPERATOR_SIDE) && (defined KIATEL_UDP_INSTALLED))
extern KIATEL_UDP_STRUCT	kt_udp_rec[MAX_KIATEL_UDP_REC];
#endif
BYTE transmit_packet_on_udp(BYTE n){
#if ((defined OPERATOR_SIDE) && (OS == WINDOWS))
	BYTE size;
	while(get_buf(&hwtx_st[n],&size) == OK){
	    BYTE j;char send_buf[2048];int send_buf_len;
	    send_buf[0]=size;
	    for(j=1;j<size+1;j++){  /* last byte after setting EOP */
		    if(get_buf(&hwtx_st[n],&send_buf[j]) != OK){
			    prnt_msg(LAPB_ERROR,0x4013);
			    return FAIL;
		    }
	    }
	    send_buf_len=size+1;
	    send_win_packet(send_buf, send_buf_len, n);
	}
	return OK;
#endif
#if ((defined SYSTEM_SIDE) || ((defined OPERATOR_SIDE) && (defined KIATEL_UDP_INSTALLED)))
	BYTE size;
	unsigned short j;	// size can be 255, then 'j' should handle size+1
	BYTE total_size=0;
	unsigned int mr1;
	HDLC_ST  *pt;
	unsigned int kt_udp_no;
	pt=&hdlc_ch[n];

#if 0
#ifdef OPERATOR_SIDE
	if(pt->dl_ch == 0)
	    fsocket.address=
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 91 <<  0);
	if(pt->dl_ch == 1)
	    fsocket.address=
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 92 <<  0);
#if (TON == 0)
	    fsocket.port=3490+0;
#endif
#if (TON == 1)
	    fsocket.port=3490+1;
#endif
#if (TON == 2)
	    fsocket.port=3490+2;
#endif
#if (TON == 3)
	    fsocket.port=3490+3;
#endif
#endif
#ifdef SYSTEM_SIDE
	if(pt->dl_ch == 0)
	    fsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 95 <<  0);
	if(pt->dl_ch == 1)
	    fsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 96 <<  0);
	if(pt->dl_ch == 2)
	    fsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 97 <<  0);
	if(pt->dl_ch == 3)
	    fsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 98 <<  0);
#ifdef HTBY_LINK_ON_MY_UDP
	if(pt->dl_ch == 5){
	    if (rd_master_swch() == RESET)
	      fsocket.address=
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 91 <<  0);
	    else
	      fsocket.address=
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 92 <<  0);
	    fsocket.port=3490+5;
	}
	else
#endif
#ifdef HOT_STANDBY
	if (rd_master_swch() == RESET)
	    fsocket.port=3490+1;
	else
#endif
	    fsocket.port=3490+0;
#endif
	ucb_no=pt->cs;
#endif // 0

	kt_udp_no=pt->cs;
#ifdef KIATEL_UDP_INSTALLED
	if(kt_udp_no >= MAX_KIATEL_UDP_REC)
#endif
#if ((defined UDP_ON_CLIENT_SIDE) && (defined SYSTEM_SIDE))
	if(kt_udp_no >= MAX_UDP_REC)
#endif
	{
		prnt_msg(LAPB_ERROR,0x4301);
		prnt_byte(n);prnt_char('-');
		prnt_byte(FIRST_UDP_HDLC_CH);prnt_char('-');
		prnt_byte(kt_udp_no);prnt_char('-');
		prnt_byte(hdlc_ch[n].ic_type);prnt_char(CR);
		return FAIL;
	}
#ifdef KIATEL_UDP_INSTALLED
	SOCKET_STRUCT fsocket;
	guint ucb_no;
	KIATEL_UDP_STRUCT *kt_udp_pt;
	kt_udp_pt=&kt_udp_rec[kt_udp_no];
	ucb_no=kt_udp_pt->ucb_no;
	if(ucb_no >= MAX_UCB){
		prnt_msg(LAPB_ERROR,0x4302);
		return FAIL;
	}
	fsocket.address=kt_udp_pt->remote_address;
	fsocket.port=kt_udp_pt->remote_port;
#endif
#if ((defined UDP_ON_CLIENT_SIDE) && (defined SYSTEM_SIDE))
	UDP_STRUCT *udp_pt;
	udp_pt=&udp_st[kt_udp_no];
#endif

send_again:
	if (get_buf(&hwtx_st[n],&size) != OK)
	    return OK; /* no data for transmission */

	else{
	    mr1=mem_rec_req(size+1);
	    if(mr1 >= NO_MEM_REC)
		return FAIL;
	    mem_rec[mr1].parm[0]=size;
	    for(j=1;j<size+1;j++){  /* last byte after setting EOP */
		if(get_buf(&hwtx_st[n],&mem_rec[mr1].parm[j]) != OK){
		    prnt_msg(LAPB_ERROR,0x4013);
		    prnt_byte(n);prnt_char('-');
		    prnt_int(size);prnt_char(CR);
		    mem_rec_rls(mr1);
		    return FAIL;
		}
	    }
#ifdef PRINT_LAPB_XMT_SENDING_PACKET
	    prnt_char('T'); prnt_char('x');prnt_char('-');prnt_byte(n);
	    prnt_char(':');
	    print_mr(mr1);
#endif
#ifdef KIATEL_UDP_INSTALLED
	    xmit_udp(ucb_no,&fsocket,mr1); // this routine is stupid enough to return 0 on success and -1 on failure!!
#endif
#if ((defined UDP_ON_CLIENT_SIDE) && (defined SYSTEM_SIDE))
	    guint mem_size=mem_rec[mr1].size;
//	    put_pckt_to_variable_size_buf(&(udp_pt->nettx_st),mem_rec[mr1].parm,mem_size));
// try to send it now
	    struct sockaddr_in their_addr;
	    int sent_size;
	    memcpy(udp_pt->tx_pckt_arr,mem_rec[mr1].parm,mem_size);
	    mem_rec_rls(mr1);
	    if(set_sockaddr_in_fields(&their_addr,udp_pt) == FAIL){
		prnt_msg(LAPB_ERROR,0x4013);
		return FAIL;
	    }
	    sent_size=sendto(udp_pt->tx_sockfd,udp_pt->tx_pckt_arr,mem_size,
		       0,(struct sockaddr *)&their_addr,sizeof(their_addr));
#endif
	    total_size+=size;
	    if((total_size+check_size_of_next_packet(&hwtx_st[n])) < 1000)
		goto send_again;
	    else
		return OK;
	}
#endif // ((defined SYSTEM_SIDE) || ((defined OPERATOR_SIDE) && (defined KIATEL_UDP_INSTALLED)))
}
#endif // ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
/****************************************************************************/
BYTE receive_packet(BYTE n){

	switch (hdlc_ch[n].ic_type){
#ifdef HDLC_8952
	  case IC_HDLC_8952 :
				return(receive_packet_8952_ver2(n));
#endif
#ifdef HDLC_9075
	  case IC_HDLC_9075 :
				return(receive_packet_9075_ver2(n));
#endif
#ifdef HDLC_RS232
	  case IC_HDLC_RS232:
				return(receive_packet_rs232(n));
#endif
#ifdef HDLC_9076
	  case IC_HDLC_9076 :
				return(receive_packet_9076_ver2(n));
#endif
#ifdef HDLC_QUADE1_PEF22554
	  case IC_HDLC_QUADE1_PEF22554 :
				return(receive_packet_pef22554_ver2(n));
#endif
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
	  case IC_HDLC_ON_UDP :
// receive_packet_on_udp_ver2 is handled by udp routines whenever a packet is received
				return OK;
#endif
	  case IC_HDLC_NOT_INSTALLED:
				return OK;
	  default	    :
				prnt_msg(LAPB_ERROR,0x1005);
				return FAIL;
	}
}
/****************************************************************************/
#ifdef HDLC_8952
BYTE receive_packet_8952(BYTE n){

	BYTE indx,j;
	BYTE status,int_status;
	BYTE tmprx[MAX_PACKET_SIZE];
	HDLC_ST *pt;


#ifdef CS_CORRECTION
	BYTE bit_pos1,cs_byte1;
	BYTE bit_pos2,cs_byte2;
	BYTE chksum,cal_chksum;
#endif

	pt=&hdlc_ch[n];
	status = rgen_stat(pt->cs);
	if((status & 0x08) != 0x08){   /* if interrupt not detected */
#ifdef DEBUG_ACT
//		prnt_char('K'); prnt_char('1');prnt_char('*');
//		prnt_byte(status);
#endif

		return OK;
	}
	int_status = rint_flag(pt->cs);

	if ( (int_status & 0x01) == 0x01){  /* if Rx overflow detected */
		prnt_msg(LAPB_ERROR,0x4006);
		make_rx_fifo_empty(n);
		return FAIL;
	}

	if ( (int_status & 0x40) != 0x40){  /* if EOPD not detected */
#ifdef DEBUG_ACT
//		prnt_char('K'); prnt_char('2');prnt_char('#');
#endif

		return OK;
	}
	status = rfifo_stat(pt->cs);
	if ( (status & 0x30) == 0){    /* RxFIFO Empty */
		prnt_msg(LAPB_ERROR,0x4007);
		return FAIL;
	}
	else{
		if( (status & 0xc0) != 0x40){  /* if not first byte */
			prnt_msg(LAPB_ERROR,0x4008);
			make_rx_fifo_empty(n);
			return	FAIL;	  /* should read until FIFO empty */
		}
		else{
			indx = 0;
			do{
#if (MAX_PACKET_SIZE < 256)
				if (indx >= MAX_PACKET_SIZE){

					prnt_msg(LAPB_ERROR,0x4009);
					init_HDLC_and_8972(n);
//					make_rx_fifo_empty(n);
					return	FAIL;	  /* should read until FIFO empty */

				}
#endif

				tmprx[indx] = rrx_data(pt->cs);
#ifdef DEBUG_ACT
				prnt_char('R'); prnt_char('x');
				prnt_char(':'); prnt_byte(tmprx[indx]); prnt_char(CR);
#endif

				indx++;
				status = rfifo_stat(pt->cs) & 0xc0; /* check the status of Rx FIFO */
			}while( status == 0 );	/* read packet byte */
#ifdef CS_CORRECTION
			indx--;
			cs_byte1=tmprx[indx];
			if( status == 0x80 ){  /* last byte good FCS */
				bit_pos1=0x01;
				bit_pos2=0x01;

				cs_byte2 = rrx_data(pt->cs); /* read last byte */
#ifdef DEBUG_ACT
//				prnt_char('R'); prnt_char('X'); prnt_char('C');prnt_char('2');
//				prnt_char(':'); prnt_byte(cs_byte2); prnt_char(CR);
#endif
				indx--;
				if ( checkfree_buf(&hwrx_st[n]) >= indx ){
					cal_chksum=0;
					for(j=0;j<indx;j++){
						if(j>2){
							if((tmprx[j]&0xf0)==0x00){
								if ((cs_byte1&bit_pos1)!=0)
									tmprx[j]=tmprx[j]|0xf0;
							}
							bit_pos1=bit_pos1<<1;
							if((tmprx[j]&0x0f)==0x00){
								if ((cs_byte2&bit_pos2)!=0)
									tmprx[j]=tmprx[j]|0x0f;
							}
							bit_pos2=bit_pos2<<1;
						}
						cal_chksum=cal_chksum+tmprx[j];
					}
					chksum=tmprx[indx];
					if((chksum&0xf0)==0x00){
						if ((cs_byte1&bit_pos1)!=0){
							chksum=chksum|0xf0;
							cs_byte1=cs_byte1 &(~bit_pos1);
						}
					}
					if((chksum&0x0f)==0x00){
						if ((cs_byte2&bit_pos2)!=0){
							chksum=chksum|0x0f;
							cs_byte2=cs_byte2 &(~bit_pos2);
						}
					}
					cal_chksum = cal_chksum + cs_byte1 + cs_byte2;
					if (chksum != cal_chksum){
						prnt_msg(LAPB_ERROR,0x400b);
						make_rx_fifo_empty(n);
#ifdef DEBUG_ACT
/*
						prnt_byte(0xdd);
						prnt_byte(bit_pos1);
						prnt_byte(bit_pos2);
						prnt_byte(cs_byte1);
						prnt_byte(cs_byte2);
						prnt_byte(chksum);
						prnt_byte(cal_chksum);
*/
#endif
						return	FAIL;	  /* should read until FIFO empty */
					}
					else{
#ifdef DEBUG_ACT
						prnt_char('S'); prnt_char('I'); prnt_char('Z');prnt_char('E');
						prnt_char(':'); prnt_byte(indx); prnt_char(CR);
#endif

						put_buf(&hwrx_st[n],indx); /* packet size */
						for(j=0;j<indx;j++){
#ifdef DEBUG_ACT
							prnt_char('R'); prnt_char('X');
							prnt_char(':'); prnt_byte(tmprx[j]); prnt_char(CR);
#endif

							put_buf(&hwrx_st[n],tmprx[j]);
						}
						return OK;
					}
				}
				else{
					prnt_msg(LAPB_ERROR,0x400c);
					return FAIL;
				}
			}

#else	/* #ifdef CS_CORRECTION */
			if( status == 0x80 ){  /* last byte good FCS */
				tmprx[indx] = rrx_data(pt->cs); /* read last byte */
#ifdef DEBUG_ACT
		   /*		prnt_byte(tmprx[indx]);*/
#endif
				if ( checkfree_buf(&hwrx_st[n]) >= indx+1 ){
					put_buf(&hwrx_st[n],(indx+1)); /* packet size */
					for(j=0;j<=indx;j++){
						put_buf(&hwrx_st[n],tmprx[j]);

					}
					return OK;
				}
				else{
					prnt_msg(LAPB_ERROR,0x400d);
					return FAIL;
				}
			}
#endif /*#ifdef CS_CORRECTION */
			else{
				prnt_msg(LAPB_ERROR,0x400e);
				make_rx_fifo_empty(n);
				return	FAIL;	  /* should read until FIFO empty */
			}
		}
	}
}
#endif // HDLC_8952
/****************************************************************************/
#ifdef HDLC_9075
BYTE receive_packet_9075(BYTE n){

	BYTE indx,j;
	BYTE status,int_status;
	BYTE tmprx[MAX_PACKET_SIZE];
	HDLC_ST *pt;
#ifdef CS_CORRECTION
	BYTE bit_pos1,cs_byte1;
	BYTE bit_pos2,cs_byte2;
	BYTE chksum,cal_chksum;
#endif

	pt=&hdlc_ch[n];

	status = rd_hdlc_st_9075(pt->cs,pt->dl_ch);
	if((status & 0x80) != 0x80){   /* if interrupt not detected */
#ifdef DEBUG_ACT
		prnt_byte(status);prnt_char('-');
		prnt_char('N'); prnt_char('o');prnt_char(' ');
		prnt_char('i'); prnt_char('n');prnt_char('t');
		prnt_char(CR);
#endif
		return OK;
	}
//it should be here.int_status should not be read if interrupt has not been
//detected.It is possible that during this period EOPD is detected,but int
//has not been generated yet.with reading int_status EOPD is cleared and
//received packet will remain in fifo.
	int_status=rd_hdlc_int_st_9075(pt->cs,pt->dl_ch);
	if ((int_status & 0x03) != 0){

		prnt_msg(LAPB_ERROR,0x4040);
		make_rx_fifo_empty(n);
		return FAIL;
	}
#ifdef DEBUG_ACT
	prnt_byte(status);prnt_char('-');
	prnt_char('i'); prnt_char('n');prnt_char('t');
	prnt_char(CR);
#endif

#if 0
	if ( (int_status & 0x02) == 0x02){  /* if Rx FIFO is filled above the threshold */
		prnt_msg(LAPB_ERROR,0x4000);
		make_rx_fifo_empty(n);
		return FAIL;
	}
#endif // 0
	if ( (int_status & 0x40) != 0x40){  /* if EOPD not detected */
#ifdef DEBUG_ACT

		prnt_byte(status);prnt_char('-');
		prnt_char('N'); prnt_char('o');prnt_char(' ');
		prnt_char('E'); prnt_char('O');prnt_char('P');
		prnt_char(CR);
#endif

		return OK;
	}
	if ( (status & 0x03) == 0){    /* RxFIFO Empty */
		prnt_msg(LAPB_ERROR,0x4001);
		return FAIL;
	}
	else{
		if( (status & 0x30) != 0x10){  /* if not first byte */

//			prnt_msg(LAPB_ERROR,0x4002);
			make_rx_fifo_empty(n);
			return	FAIL;	  /* should read until FIFO empty */
		}
		else{
			indx = 0;
			do{
				if (indx >= MAX_PACKET_SIZE){

					prnt_msg(LAPB_ERROR,0x4003);
					init_hdlc_9075(n);
//					make_rx_fifo_empty(n);
					return	FAIL;	  /* should read until FIFO empty */

				}

				tmprx[indx] = rd_hdlc_rx_fifo_9075(pt->cs,pt->dl_ch);

#ifdef DEBUG_ACT
if (tst_flg == SET){
				prnt_char(' '); prnt_char('R');
				prnt_char(':'); prnt_byte(tmprx[indx]); //prnt_char(CR);
}
#endif
				indx++;
				status=rd_hdlc_st_9075(pt->cs,pt->dl_ch)&0x30;
			}while( status == 0 );	/* read packet byte */
#ifdef DEBUG_ACT
if (tst_flg == SET){
//				prnt_char(CR);
}
#endif
#ifdef CS_CORRECTION
			indx--;
			cs_byte1=tmprx[indx];
			if( status == 0x20 ){  /* last byte good FCS */
				bit_pos1=0x01;
				bit_pos2=0x01;

				cs_byte2 = rd_hdlc_rx_fifo_9075(pt->cs,pt->dl_ch);
#ifdef DEBUG_ACT
//				prnt_char('R'); prnt_char('x'); prnt_char('C');prnt_char('2');
//				prnt_char(':'); prnt_byte(cs_byte2); prnt_char(CR);
#endif
#ifdef DEBUG_ACT
if (tst_flg == SET){
				prnt_char(' '); prnt_char('R');
				prnt_char(':'); prnt_byte(cs_byte2);prnt_char(' ');
				prnt_char('n'); prnt_byte(n); //prnt_char(CR);
				prnt_char(CR);
}
#endif
				indx--;
				if ( checkfree_buf(&hwrx_st[n]) >= indx ){
					cal_chksum=0;
					for(j=0;j<indx;j++){
						if(j>2){
							if((tmprx[j]&0xf0)==0x00){
								if ((cs_byte1&bit_pos1)!=0)
									tmprx[j]=tmprx[j]|0xf0;
							}
							bit_pos1=bit_pos1<<1;
							if((tmprx[j]&0x0f)==0x00){
								if ((cs_byte2&bit_pos2)!=0)
									tmprx[j]=tmprx[j]|0x0f;
							}
							bit_pos2=bit_pos2<<1;
						}
						cal_chksum=cal_chksum+tmprx[j];
					}
					chksum=tmprx[indx];
					if((chksum&0xf0)==0x00){
						if ((cs_byte1&bit_pos1)!=0){
							chksum=chksum|0xf0;
							cs_byte1=cs_byte1 &(~bit_pos1);
						}
					}
					if((chksum&0x0f)==0x00){
						if ((cs_byte2&bit_pos2)!=0){
							chksum=chksum|0x0f;
							cs_byte2=cs_byte2 &(~bit_pos2);
						}
					}
					cal_chksum = cal_chksum + cs_byte1 + cs_byte2;
					if (chksum != cal_chksum){
						prnt_msg(LAPB_ERROR,0x4004);
						prnt_byte(n);prnt_char(CR);
						make_rx_fifo_empty(n);
#ifdef DEBUG_ACT
/*
						prnt_byte(bit_pos1);
						prnt_byte(bit_pos2);
						prnt_byte(cs_byte1);
						prnt_byte(cs_byte2);
						prnt_byte(chksum);
						prnt_byte(cal_chksum);
*/
#endif
						return	FAIL;	  /* should read until FIFO empty */
					}
					else{
#ifdef DEBUG_ACT
						prnt_char('S'); prnt_char('I'); prnt_char('Z');prnt_char('E');
						prnt_char(':'); prnt_byte(indx); prnt_char(CR);
#endif
						put_buf(&hwrx_st[n],indx); /* packet size */
						for(j=0;j<indx;j++){
#ifdef DEBUG_ACT
								prnt_char('R'); prnt_char('X');
								prnt_char(':'); prnt_byte(tmprx[j]); prnt_char(CR);
#endif
							put_buf(&hwrx_st[n],tmprx[j]);
						}
						return OK;
					}
				}
				else{
					prnt_msg(LAPB_ERROR,0x4005);
					return FAIL;
				}
			}

#else	/* #ifdef CS_CORRECTION */
			if( status == 0x80 ){  /* last byte good FCS */
///				tmprx[indx] = rrx_data(pt->cs); /* read last byte */
				tmprx[indx] = rd_hdlc_rx_fifo(pt->cs,pt->dl_ch);

				if ( checkfree_buf(&hwrx_st[n]) >= indx+1 ){
					put_buf(&hwrx_st[n],(indx+1)); /* packet size */
					for(j=0;j<=indx;j++){
						put_buf(&hwrx_st[n],tmprx[j]);

					}
					return OK;
				}
				else{
					prnt_msg(LAPB_ERROR,0x400f);
					return FAIL;
				}
			}
#endif /*#ifdef CS_CORRECTION */
			else{
				prnt_msg(LAPB_ERROR,0x4010);
				make_rx_fifo_empty(n);
				return	FAIL;	  /* should read until FIFO empty */
			}
		}
	}
}
#endif // HDLC_9075

/*****************************************************************************/
void hwlayer_rx_handler(void){

	BYTE n;
/* if no sync, packet is read,but if corrupted,will be rejected by this layer */
/* or link layer */

#ifdef DEBUG_ACT
//	prnt_msg(LAPB_RTN,0x101);
#endif
	for (n=0;n<MAX_HWRX_LAPB_SCAN;n++){
		if( (++hwrx_lapb_scn_no) >= NO_LAPB_CH)
			hwrx_lapb_scn_no=0;
		if (sync_flag[hwrx_lapb_scn_no] != SET){
//			prnt_msg(LAPB_ERROR,0x1010);
			continue;
		}
		if (receive_packet(hwrx_lapb_scn_no) == FAIL){
//			prnt_msg(LAPB_ERROR,0x1012);
		}
	}
	return;
}
/*****************************************************************************/
void hwlayer_tx_handler(void){
	BYTE n;
	for (n=0;n<MAX_HWTX_LAPB_SCAN;n++){
		if( (++hwtx_lapb_scn_no) >= NO_LAPB_CH) hwtx_lapb_scn_no=0;
		if (sync_flag[hwtx_lapb_scn_no] != SET) continue ;   /* if no sync ,transmission is stopped. */
		if(hdlc_ch[hwtx_lapb_scn_no].fast_link == SET) continue;
		if (transmit_packet(hwtx_lapb_scn_no) == FAIL){
#ifdef DEBUG_ACT
		    prnt_msg(LAPB_RTN,0x400);
#endif
		}
	}
	return;
}
void hwlayer_tx_fast_handler(void){
	BYTE n;
	for (n=0;n<MAX_HWTX_LAPB_SCAN;n++){
		if( (++hwtx_lapb_scn_no) >= NO_LAPB_CH) hwtx_lapb_scn_no=0;
		if (sync_flag[hwtx_lapb_scn_no] != SET) continue ;   /* if no sync ,transmission is stopped. */
		if(hdlc_ch[hwtx_lapb_scn_no].fast_link == RESET) continue;
		if (transmit_packet(hwtx_lapb_scn_no) == FAIL){
#ifdef DEBUG_ACT
		    prnt_msg(LAPB_RTN,0x400);
#endif
		}
	}
	return;
}
/**************************************************************************/

/* ======================================================================*/
/* remote_stn and local_stn id should be initialized. */
/* 8952 or 8993 should be selected.		      */
/* ======================================================================*/

/* ACK_ON_REQ has been deleted.to be added if necessary .		 */
/* if used,receiver should send ACK on reception of I_frame with PF_SET    */
/* transmitter will request ACK on the last I_frame before closing retx_win*/

/* I don't know when it shouLd send DM instead of UA in response to SABM */

/* sending PF_SET or PF_RESET is considered in sending frames.but in
   received  frames in most cases there is no check on it.		 */

/* Common FIFO buffers between physical layer and link layer. */

#ifndef CENTRAL_EXTERN

extern FUNC_PTR link_rtn[MAX_STATE][MAX_EVENT];

/* nettx_st2 is used for reading data,but not removing them  from nettx[]    */
/* front and rear of these variables are used for reading maximum 7 frames.  */
/* data for each frame is  removed from nettx[] array after receiving ACK.   */

#ifdef SYSTEM_SIDE
#if ((defined RSU_CCSV5_INSTALLED) || (defined RSUSIDE))
extern BYTE nettx_rsu[TOTAL_NO_CCSV5_GROUP][RSU_NETBUF_SIZE];
extern BYTE netrx_rsu[TOTAL_NO_CCSV5_GROUP][RSU_RX_NETBUF_SIZE];
#endif
#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
extern BYTE nettx_oam[NO_OPRTR][OAM_NETBUF_SIZE];
extern BYTE netrx_oam[NO_OPRTR][OAM_RX_NETBUF_SIZE];
#endif
#ifdef HTBY_LINK_ON_MY_UDP
extern BYTE nettx_htby[1][HTBY_NETBUF_SIZE];
extern BYTE netrx_htby[1][HTBY_RX_NETBUF_SIZE];
#endif
#endif //SYSTEM_SIDE

#ifdef OPERATOR_SIDE
extern BYTE nettx_oam[NO_LAPB_CH][OAM_NETBUF_SIZE];
extern BYTE netrx_oam[NO_LAPB_CH][OAM_RX_NETBUF_SIZE];
#endif

#ifdef HDLC_RS232
#ifdef SYSTEM_SIDE
#ifdef ALARM_PANEL_ON_HDLC_RS232
extern BYTE nettx_alarm[ALARM_NETBUF_SIZE];
#endif
#endif
#endif

extern BUF nettx_st[];
/* variable including &nettx[0],front,rear and size used for reading and
   writing to nettx[] FIFO buffer.*/
extern BUF nettx_st2[];
/* variable including &nettx[0],front,rear and size used for dummy
   reading and writing to nettx[] FIFO buffer.*/
extern BUF netrx_st[];
/* variable including &netrx[0],front,rear and size used for reading and
   writing to nettx[] FIFO buffer.*/

extern WIN_ST retxwin[NO_LAPB_CH][RETX_WIN_BUF_SIZE];

extern WINBUF retxwin_st[NO_LAPB_CH];

extern FUNC_PTR link_rtn[MAX_STATE][MAX_EVENT];

extern BYTE Vs[] ;  /* Send State Variable.The sequence number it will assign */
	   /* to the next I_FRAME to be transmitted .*/
extern BYTE Vr[] ;  /* Receive State Variable.The next in_sequence I_FRAME it */
	   /* expects to receive. */

extern RXFRAME_ST curr_rx[];
extern TXFRAME_ST curr_tx[];
extern TIMER_ST publictmr[];

extern BYTE remote_stn[] ;
extern BYTE local_stn[];

extern BYTE retx_index[]; /* index of the frame in retxwin which is ready for	      */
		 /* for retransmission.(because of TIMEOUT or receiving REJ) */
extern BYTE REJ_flag[];   /* Having sent one REJ frame, station does not send	 */
		/* any further REJ frames until a correct frame has    */
		/* been received. This flag is set if one REJ is sent. */

extern BYTE state[];
//extern BYTE event;
extern BYTE dbg_event;

extern BUF hwrx_st[];
extern BUF hwtx_st[];

extern BYTE hwtx[NO_LAPB_CH][HWBUF_SIZE];
extern BYTE hwrx[NO_LAPB_CH][HWBUF_SIZE];

#endif // ndef CENTRAL_EXTERN
/* ======================================================================*/
void tx_pend(BYTE n ,WORD_LAPB count){
	prnt_char('T');prnt_char('x');prnt_char('p');prnt_char('e');
	prnt_char('n');prnt_char('d');prnt_char(CR);
	state[n] = TX_PEND;
	publictmr[n].count = count;
	publictmr[n].active = TRUE;
	return;
}
/* ======================================================================*/
void rx_pend(BYTE n,WORD_LAPB count){
	prnt_char('R');prnt_char('x');prnt_char('p');prnt_char('e');
	prnt_char('n');prnt_char('d');prnt_char(CR);
	state[n] = RX_PEND;
	publictmr[n].count = count;
	publictmr[n].active = TRUE;
	return;
}
/* ======================================================================*/

void initialize_linklayer(BYTE n){

	nettx_st2[n].front = nettx_st[n].front;
	nettx_st2[n].rear = nettx_st[n].rear;

	Vr[n] = Vs[n] = 0;

	state[n] = IDLE;
	lapb_link_flag[n]=RESET;

	REJ_flag[n] = FALSE;

	publictmr[n].active = FALSE;

	hwtx_st[n].arr = &hwtx[n][0];
	hwtx_st[n].front = hwtx_st[n].rear = 0;
	hwtx_st[n].size = HWBUF_SIZE;

	hwrx_st[n].arr = &hwrx[n][0];
	hwrx_st[n].front = hwrx_st[n].rear = 0;
	hwrx_st[n].size = HWBUF_SIZE;

	retxwin_st[n].arr = &retxwin[n][0];
	retxwin_st[n].rear = retxwin_st[n].front = 0;
// moved to init_lapb
//	retxwin_st[n].size = RETX_WIN_BUF_SIZE;

}
/* ======================================================================*/
//LCS3_TEST start : this routine is added.
void reset_linklayer(BYTE n){

	nettx_st2[n].front = nettx_st[n].front;
	nettx_st2[n].rear = nettx_st[n].rear;

	Vr[n] = Vs[n] = 0;

	state[n] = IDLE;
	lapb_link_flag[n]=RESET;

	REJ_flag[n] = FALSE;

	publictmr[n].active = FALSE;

	hwtx_st[n].front = hwtx_st[n].rear = 0;

	hwrx_st[n].front = hwrx_st[n].rear = 0;

	retxwin_st[n].rear = retxwin_st[n].front = 0;
#ifdef SYSTEM_SIDE
#ifdef RSU_CCSV5_INSTALLED
#ifdef ALARM_ENABLE
	    if(n < NO_RSU){
		pmamftx(MA_SYS_P | MAC_ALARM, CONNECTION_ALARM |
			 RSU_CONNECTION_LOST | n,ALARM_OPR);
	    }
#endif
#endif
#endif

}
/* ======================================================================*/

void init_link_rtns(void){

	BYTE i,j;

	for (i=0;i<MAX_STATE;i++)
		for(j=0;j<MAX_EVENT;j++)
			link_rtn[i][j] =(FUNC_PTR)link_rtn_null;

	link_rtn[IDLE][I_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[IDLE][RR_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[IDLE][RNR_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[IDLE][REJ_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[IDLE][SABM_RX] = link_rtn1;
	link_rtn[IDLE][DISC_RX] = link_rtn5;
	link_rtn[IDLE][UA_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[IDLE][DM_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[IDLE][FRMR_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[IDLE][LAPB_TIME_OUT] = link_rtn2;/*link_reset;*/
	link_rtn[IDLE][PACKET_TIME_OUT] = link_reset;
	link_rtn[IDLE][LAPB_NULL_EVENT] = link_rtn2;

	link_rtn[WAIT_FOR_UA][I_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[WAIT_FOR_UA][RR_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[WAIT_FOR_UA][RNR_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[WAIT_FOR_UA][REJ_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[WAIT_FOR_UA][SABM_RX] = link_rtn4;
	link_rtn[WAIT_FOR_UA][DISC_RX] = link_rtn5;
	link_rtn[WAIT_FOR_UA][UA_RX] = link_rtn3;
	link_rtn[WAIT_FOR_UA][DM_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[WAIT_FOR_UA][FRMR_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[WAIT_FOR_UA][LAPB_TIME_OUT] = link_rtn6;
	link_rtn[WAIT_FOR_UA][PACKET_TIME_OUT] = link_reset;
	link_rtn[WAIT_FOR_UA][LAPB_NULL_EVENT] =(FUNC_PTR) link_rtn_null;

	link_rtn[INFO_TRX][I_RX] = link_rtn7;
	link_rtn[INFO_TRX][RR_RX] = link_rtn8;
	link_rtn[INFO_TRX][RNR_RX] = link_rtn9;
	link_rtn[INFO_TRX][REJ_RX] = link_rtn10;
//LAPB_BUG
//	link_rtn[INFO_TRX][SABM_RX] = link_reset;
	link_rtn[INFO_TRX][SABM_RX] = link_rtn19;

	link_rtn[INFO_TRX][DISC_RX] = link_rtn5;
//SYNC_BUG
//	link_rtn[INFO_TRX][UA_RX] = link_reset;
//	it is better to ignore it.if WAIT_FOR_LINK_SETUP_COUNT is not
//	long enough ! or there is a long delay(overload in one side)
//	before receiving UA,this side is reset again,and link_reset,
//	will continuosly happen in both sides.

	link_rtn[INFO_TRX][UA_RX] = (FUNC_PTR) link_rtn_null;
	link_rtn[INFO_TRX][DM_RX] = link_reset;
	link_rtn[INFO_TRX][FRMR_RX] = link_reset;
	link_rtn[INFO_TRX][PACKET_TIME_OUT] = link_rtn13;
	/* I don't know.to be checked later.*/
	link_rtn[INFO_TRX][LAPB_TIME_OUT] = link_reset;

	link_rtn[INFO_TRX][LAPB_NULL_EVENT] = link_rtn11;

	link_rtn[REMOTE_STN_BUSY][RR_RX] = link_rtn14;
	link_rtn[REMOTE_STN_BUSY][DISC_RX] = link_rtn5;
	link_rtn[REMOTE_STN_BUSY][LAPB_TIME_OUT] = link_hw_reset;
	link_rtn[REMOTE_STN_BUSY][PACKET_TIME_OUT] = (FUNC_PTR)link_rtn_null;
	link_rtn[REMOTE_STN_BUSY][UA_RX] = link_hw_reset;
	link_rtn[REMOTE_STN_BUSY][DM_RX] = link_hw_reset;
	link_rtn[REMOTE_STN_BUSY][FRMR_RX] = link_hw_reset;

	/* added 76/10/22. */

	link_rtn[REMOTE_STN_BUSY][SABM_RX] = link_reset;

	link_rtn[TX_PEND][LAPB_NULL_EVENT] = link_rtn12;
	link_rtn[TX_PEND][LAPB_TIME_OUT] = link_hw_reset;
	link_rtn[TX_PEND][PACKET_TIME_OUT] = (FUNC_PTR)link_rtn_null;

	link_rtn[RX_PEND][LAPB_NULL_EVENT] = link_rtn15;
	link_rtn[RX_PEND][LAPB_TIME_OUT] = link_hw_reset;
	link_rtn[RX_PEND][PACKET_TIME_OUT] = (FUNC_PTR)link_rtn_null;

	link_rtn[WAIT_BEFORE_RESET][LAPB_TIME_OUT]=link_rtn16;
	link_rtn[WAIT_BEFORE_RESET][PACKET_TIME_OUT]=(FUNC_PTR)link_rtn_null;

	link_rtn[WAIT_FOR_LINK_SETUP][LAPB_TIME_OUT]=link_rtn17;
	link_rtn[WAIT_FOR_LINK_SETUP][UA_RX]=link_rtn3;
//LAPB_BUG
	link_rtn[WAIT_FOR_LINK_SETUP][I_RX]=link_rtn18;
	link_rtn[WAIT_FOR_LINK_SETUP][SABM_RX]=link_rtn20;

	for (j=0;j<MAX_STATE;j++)
		link_rtn[j][BAD_CMD]=link_hw_reset;

	/* link_rtn[][RESET] */
	/* When a number of frames are lost or some kind of failure occurs */
	/* it may become necessary for both ends to  agree  to	reset the  */
	/* receive  sequence  counters. On  receiving  RESET , UA  command */
	/* should be sent.Now in this situation FRMR is sent .		   */
	/* to be completed later if necessary.				   */

	/* link_rtn[STN_BUSY][] */
	/* It is not the same as  TX_PEND  or  RX_PEND. in this condition */
	/* busy station sends RNR,which requires the other side to stop   */
	/* transmission.The station receiving RNR will usually poll the   */
	/* busy station by sending an RR with the P bit set.This requires */
	/* the other side to respond with either an RR or RNR.		  */
}
/* ======================================================================*/
/* if address is not correct or no.of data bytes existing in buf are less  */
/* than size,existing bytes should be removed for buffer correction.	   */
/* ======================================================================*/

BYTE read_received_frame(BYTE n){

	BYTE datsize;
	BYTE dat,j;
	BYTE addr_flag;
	BUF *buf_pt;
	RXFRAME_ST *curr_pt;
	BYTE size;
	BYTE tmpdat;

	buf_pt=&hwrx_st[n];
	curr_pt=&curr_rx[n];
	if ( get_buf(buf_pt,&datsize) == FAIL )  /* size byte */
		return FAIL;	/* buffer is empty. */

	if ( get_buf(buf_pt,&dat) == OK ){     /* address byte */
		addr_flag = (dat == local_stn[n]);
		tmpdat=dat;
		/* if address is not correct,should read packet completely */
		/* to remove it from buffer. */
	}
	else{
		prnt_msg(LAPB_ERROR,0x401c);
		return FAIL;
	}
	if ( get_buf( buf_pt,&dat ) == OK ){	 /* control byte */
		if ( (dat & I_POS) == LAPB_I_FRAME ){
			curr_pt->ftype = LAPB_I_FRAME;
			curr_pt->pf = dat & PF_POS;
			curr_pt->f.i.Nr = (dat >> NR_SHIFT) & TRI_BIT;
			curr_pt->f.i.Ns = (dat >> NS_SHIFT) & TRI_BIT;
			curr_pt->f.i.size = datsize - 2; /* information only */
			size=curr_rx[n].f.i.size;
			for ( j=0;j<size;j++ ){
				if ( get_buf(buf_pt,&dat) == OK ){
					curr_pt->f.i.tmparr[j]=dat;
				}
				else{
					prnt_msg(LAPB_ERROR,0x4017);
					return FAIL;
				}
			}
			if( addr_flag) return OK;
			else{
				prnt_msg(LAPB_ERROR,0x4018);
				prnt_byte(tmpdat);prnt_char(CR);
#ifdef SET_LAPB_LOCAL_STN_AUTOMATICALLY
				local_stn[n]=tmpdat;
				return OK;
#else
				return FAIL;
#endif
			}
		}
		else{
			if ( (dat & SU_POS) == S_FRAME ){
				curr_pt->ftype = S_FRAME;
				curr_pt->pf = dat & PF_POS;
				curr_pt->f.s.Nr = (dat >> NR_SHIFT) & TRI_BIT;
				curr_pt->f.s.S_cmd = dat & CMD_S_POS;
				if (addr_flag)
					return OK;
				else{
					prnt_msg(LAPB_ERROR,0x4019);
					prnt_byte(tmpdat);prnt_char(CR);
#ifdef SET_LAPB_LOCAL_STN_AUTOMATICALLY
					local_stn[n]=tmpdat;
					return OK;
#else
					return FAIL;
#endif
				}
			}
			else{	   /* U_FRAME */
				curr_pt->ftype = U_FRAME;
				curr_pt->pf = dat & PF_POS;
				curr_pt->f.u.U_cmd = dat & CMD_U_POS;
				if (addr_flag) return OK;
				else{
					prnt_msg(LAPB_ERROR,0x401a);
					prnt_byte(n);prnt_char('-');
					prnt_byte(local_stn[n]);prnt_char('-');
					prnt_byte(tmpdat);
					prnt_char(CR);
#ifdef SET_LAPB_LOCAL_STN_AUTOMATICALLY
					local_stn[n]=tmpdat;
					return OK;
#else
					return FAIL;
#endif
				}
			}
		}
	}
	else{
		prnt_msg(LAPB_ERROR,0x401b);
		return FAIL;
	}
}
/* ======================================================================*/
/*		if N(R) is correct, (it means that N(R)-1 is equal to	  */
/*		frame id of one of the exiting frames in retxwin.)	 */
/*		removes related frames from retxwin.			 */
/*		if N(R) is not correct ,(sequence is lost) FRMR should be */
/*		sent and state changes to IDLE.(regardless of N(S))	  */
/*		if N(S) = V(R) Frame id is correct.RR with V(R)++ should  */
/*		be sent.						  */
/*		if N(S) < V(R) repeated frame.ignored.RR with correct V(R)*/
/*		should be sent. 					  */
/*		if N(S) > V(R) one or more frame is lost.REJ frame with   */
/*		correct V(R) should be sent.				  */
/* ======================================================================*/

BYTE check_NR_remove_ACKedframes(BYTE n,BYTE nr){

	BYTE tot;
	char i,j;

	tot = retxwin_st[n].win_size - checkfree_buf((BUF *)&retxwin_st[n]);

		/* Check if Nr = frame id of one of the frames in retxwin */
		/* if yes removes that frame and frames before that from  */
		/* retxwin.suppose that ACK for those frames has been	  */
		/* corrupted. or possibly one ACK for more than one frame */
		/* is sent.						  */

	for (i=tot;i>= 0;i--){
		if (  ( (nr-1) & TRI_BIT ) == (( ((Vs[n]-1) & TRI_BIT) - i) & TRI_BIT) ){

			for ( j=0;j<tot-i;j++ ){
				release_retxwin(n);
			}
			return OK;
		}
	}
/*
	prnt_char('F');prnt_char('A');prnt_char('I');prnt_char('L');prnt_char(' ');
	prnt_char('N');prnt_char('R');prnt_char(CR);
*/
	return FAIL;   /* Nr != frame id of one of the frames in retxwin */
}		       /* Nr is out of sequence.*/
/* ======================================================================*/

BYTE save_received_frame(BYTE n){

	BYTE j;
	BUF *buf_pt;
	RXFRAME_ST *curr_pt;
	BYTE size;

	buf_pt=&netrx_st[n];
	curr_pt=&curr_rx[n];
	size=curr_pt->f.i.size;
	if ( checkfree_buf(buf_pt) >= size + 1){ /* size + information */
		put_buf(buf_pt,size); /* size of packet is saved.*/
#ifdef LINKLAYER_DEBUG_ACT
		if (lapb_dbg_flg == SET){
			prnt_char('s');prnt_char('v');prnt_char('e');prnt_char('-');prnt_byte(n);prnt_char(':');
		}
#endif
		for ( j=0;j<size;j++ ){
			put_buf(buf_pt,curr_pt->f.i.tmparr[j]);
#ifdef LINKLAYER_DEBUG_ACT
			if (lapb_dbg_flg == SET){
			       prnt_byte(curr_pt->f.i.tmparr[j]);prnt_char('-');
			}
#endif
		}
#ifdef LINKLAYER_DEBUG_ACT
		if (lapb_dbg_flg == SET){
			prnt_char(CR);
		}
#endif
		return OK;
	}
	else{
		prnt_msg(LAPB_ERROR,0x401d);
		prnt_byte(n);prnt_char(CR);
		return FAIL;
	}
}

/* ======================================================================*/

BYTE send_Sframe(BYTE n,BYTE scmd,BYTE pf,WORD_LAPB tmrcount){

	BUF *buf_pt;

	buf_pt=&hwtx_st[n];

	if ( checkfree_buf(buf_pt) < (SU_FRAME_SIZE + 1) ){
		prnt_msg(LAPB_ERROR,0x401e);
		return FAIL;
	}
	if (sync_flag[n] == SET){ // to prevent buffer overflow
		if ( tmrcount != NO_TMR){
			publictmr[n].active = TRUE;
			publictmr[n].count = tmrcount;
		}
		// this buffer will be initialized after SYNC detection.
#ifdef DEBUG_ACT
		if (tst_flg==SET){
			if ((n==0) || (n==1)){
				prnt_char('t');prnt_char('x');prnt_char('S');prnt_char(':');
				prnt_byte(scmd);prnt_char('-');
				prnt_byte(Vr[n]);prnt_char('-');
				prnt_char('n');prnt_char(':');prnt_byte(n);
				prnt_char(CR);
			}
		}
#endif
		put_buf(buf_pt,SU_FRAME_SIZE );  /* packet size */
		put_buf(buf_pt,remote_stn[n]);
		put_buf(buf_pt,S_FRAME | scmd | pf | ((Vr[n]<<NR_SHIFT)&NR_POS));
#ifdef LINKLAYER_DEBUG_ACT
		if (lapb_dbg_flg == SET){
			prnt_char('s');prnt_char('n');prnt_char('d');prnt_char('S');prnt_char(':');
			prnt_byte(n);prnt_char('V');prnt_char('r');
			prnt_byte(Vr[n]);prnt_char('c');prnt_char('m');prnt_char('d');
			prnt_byte(scmd);prnt_char(CR);
		}
#endif
	}
	return OK;
}

/* ======================================================================*/
BYTE send_Uframe(BYTE n,BYTE ucmd,BYTE pf,WORD_LAPB tmrcount){

	BUF *buf_pt;

	buf_pt=&hwtx_st[n];
	if ( checkfree_buf(buf_pt) < (SU_FRAME_SIZE + 1) ){
		prnt_msg(LAPB_ERROR,0x401f);
// MOOLY
		prnt_byte(n);prnt_char(CR);
		return FAIL;
	}

	if (sync_flag[n] == SET){  // to prevent buffer overflow
		if ( tmrcount != NO_TMR){
			publictmr[n].active = TRUE;
			publictmr[n].count = tmrcount;
		}
		// this buffer will be initialized after SYNC detection.
#ifdef DEBUG_ACT
		if (tst_flg==SET){
			if((n==0) || (n==1)){
				prnt_char('t');prnt_char('x');prnt_char('U');prnt_char(':');
				prnt_byte(ucmd);prnt_char('-');
				prnt_char('n');prnt_char(':');prnt_byte(n);
				prnt_char(CR);
			}
		}
#endif
		put_buf(buf_pt,SU_FRAME_SIZE );  /* packet size */
		put_buf(buf_pt,remote_stn[n]);
		put_buf(buf_pt,U_FRAME | ucmd | pf );
#ifdef LINKLAYER_DEBUG_ACT
		if (lapb_dbg_flg == SET){
			prnt_char('s');prnt_char('n');prnt_char('d');prnt_char('U');prnt_char(':');
			prnt_byte(n);prnt_char('-');
			prnt_byte(ucmd);prnt_char(CR);
	       }
#endif
	}
	return OK;
}

/* ======================================================================*/
BYTE send_Iframe(BYTE n,BYTE pf){

	BYTE j,dat,datsize;
	WIN_ST windat;
	BYTE nr,ns;
	BUF *buf_pt,*net2_pt;


	buf_pt=&hwtx_st[n];
	net2_pt=&nettx_st2[n];

	if (sync_flag[n] != SET) return FAIL_WIN_CLOSE;
	/* since only primitive reading is done by nettx_st2,and nettx_st2.rear */
	/* is never changed ,so it should be updated  before comparison. however*/
	/* in error  conditions for returning data to buffer , nettx_st2.rear	*/
	/*  is used. */
	/* note : nettx_st.rear is updated only by network layer .		*/

	net2_pt->rear=nettx_st[n].rear;
	/* During reading and writing to buffers,the last byte is not used.*/
	if (checkfree_buf(net2_pt) != (net2_pt->size -1)){   /* no data to tx*/
		if ( checkfree_buf((BUF *)&retxwin_st[n]) > 0 ) { /* retx window not closed */
			windat.netindex = net2_pt->front;
			read_buf(net2_pt,&datsize);
			/* since in get_buf() routine ,"front" is first added and then data */
			/* is read , so saving "front" for  retransmission ( if necessary ) */
			/* should be done before reading first byte .			    */
			if ( checkfree_buf(buf_pt) > (datsize + 3) ){  /* size,address,control*/
				get_buf(net2_pt,&datsize);
				windat.count = IFRAME_ACK_COUNT;
				windat.pf = pf;
				windat.Ns = Vs[n];
				windat.retry_no = 0;
				put_retxwin(&retxwin_st[n],&windat);
				put_buf(buf_pt,datsize + 2);
#ifdef DEBUG_ACT
				if (tst_flg==SET){
					prnt_char('t');prnt_char('x');prnt_char('I');prnt_char(':');
					prnt_byte(datsize);prnt_char('-');
				}
#endif
				put_buf(buf_pt,remote_stn[n] );
#ifdef DEBUG_ACT
				if (tst_flg==SET){
					prnt_byte(remote_stn[n]);prnt_char('-');
				}
#endif
				nr = (Vr[n] << NR_SHIFT) & NR_POS;
				ns = (Vs[n] << NS_SHIFT) & NS_POS;
#ifdef LINKLAYER_DEBUG_ACT
				if (lapb_dbg_flg == SET){
					prnt_char('s');prnt_char('n');prnt_char('d');prnt_char('I');prnt_char(':');
					prnt_byte(n);prnt_char('n');prnt_char('s');prnt_byte(Vs[n]);prnt_char('n');prnt_char('r');prnt_byte(Vr[n]);
					prnt_char(CR);

				}
#endif
				put_buf(buf_pt,LAPB_I_FRAME | nr | pf | ns);
#ifdef DEBUG_ACT
				if (tst_flg==SET){
					prnt_byte(LAPB_I_FRAME | nr | pf | ns);prnt_char('-');
				}
#endif
				for ( j=0;j<datsize;j++ ){
					if ( get_buf(net2_pt,&dat ) == OK ){
						put_buf(buf_pt,dat );
#ifdef DEBUG_ACT
						if (tst_flg==SET){
							prnt_byte(dat);prnt_char('-');
						}
#endif
					}
					else{
						prnt_msg(LAPB_ERROR,0x4025);
						return FAIL_NO_DATA;
						/* I don't know what should I do */
						/* bad frame should be rejected by network layer */
						/* of receiving side.*/
					}
				}
#ifdef DEBUG_ACT
				if (tst_flg==SET){
					prnt_char('n');prnt_char(':');prnt_byte(n);
					prnt_char(CR);
				}
#endif
				 //Vs[n] = ++Vs[n] & TRI_BIT;
				 Vs[n] = (Vs[n]+1) & TRI_BIT;
				 return OK;
			}
			else{
				prnt_msg(LAPB_ERROR,0x4020);
				prnt_byte(n);prnt_char('-');
				prnt_int(checkfree_buf(buf_pt));prnt_char('-');
				prnt_int(datsize);prnt_char(CR);
				return FAIL_BUF_FULL;	 /* HW buffer is full */
			}
		}
		else{
#ifdef LINKLAYER_DEBUG_ACT
			if (lapb_dbg_flg == SET){
			    prnt_msg(LAPB_ERROR,0x4021);
			    prnt_byte(n);prnt_char(CR);
			}
#endif
			return FAIL_WIN_CLOSE;	/* I don't know if we need more return bytes*/
		}
	}
	else{
		return FAIL_NO_DATA;  /* There is no data to transmit. */
	}
}

/* ======================================================================*/
BYTE release_retxwin(BYTE n){

/* Since releasing frames from retxwin[] and nettx[] is sequential,using   */
/* retx_index or netindex is not necessary here.it is used for retransmission.*/

	BYTE i,dat,datsize;
	WIN_ST windat;
	BUF *buf_pt;

	buf_pt=&nettx_st[n];
	if ( get_retxwin(&retxwin_st[n],&windat) == OK ){ /* release retx_win */

		if ( get_buf(buf_pt,&datsize) == OK){	/* release nettx[] */
			for (i=0;i<datsize;i++){
				get_buf(buf_pt,&dat);
			}
			return OK;
		}
		else{
			prnt_msg(LAPB_ERROR,0x4022);
			return FAIL;
		}
	}
	else{
		prnt_msg(LAPB_ERROR,0x4023);
		return FAIL;
	}
}
/* ======================================================================*/
BYTE retx_Iframe(BYTE n){

	BUF tmp_st;
	BYTE j,dat,datsize;
	BYTE nr,ns;

	if (sync_flag[n] != SET) return FAIL;
	retxwin[n][retx_index[n]].count = IFRAME_ACK_COUNT;
//	tmp_st.arr = &nettx[n][0];
	tmp_st.arr = nettx_st[n].arr;
//	tmp_st.size = NETBUF_SIZE;
	tmp_st.size = nettx_st[n].size;
	tmp_st.front = retxwin[n][retx_index[n]].netindex;
	tmp_st.rear = nettx_st[n].rear;
	get_buf( &tmp_st,&datsize );
	if ( checkfree_buf( &hwtx_st[n] ) > (datsize + 3) ){
		put_buf( &hwtx_st[n],datsize + 2 );
#ifdef DEBUG_ACT
		if (tst_flg==SET){
			prnt_char('r');prnt_char('t');prnt_char('x');prnt_char('I');prnt_char(':');
			prnt_byte(datsize);prnt_char('-');
		}
#endif
		put_buf( &hwtx_st[n],remote_stn[n] );
#ifdef DEBUG_ACT
		if (tst_flg==SET){
			prnt_byte(remote_stn[n]);prnt_char('-');
		}
#endif
		nr = (Vr[n] << NR_SHIFT) & NR_POS;
		ns = ( retxwin[n][retx_index[n]].Ns << NS_SHIFT ) & NS_POS;
		put_buf( &hwtx_st[n],LAPB_I_FRAME | ns | retxwin[n][retx_index[n]].pf | nr);
#ifdef DEBUG_ACT
		if (tst_flg==SET){
			prnt_byte(LAPB_I_FRAME | ns | retxwin[n][retx_index[n]].pf | nr);prnt_char('-');
		}
#endif
		for ( j=0;j<datsize;j++ ){
			get_buf( &tmp_st,&dat );
			put_buf( &hwtx_st[n],dat );
#ifdef DEBUG_ACT
			if (tst_flg==SET){
				prnt_byte(dat);prnt_char('-');
			}
#endif
		}
#ifdef DEBUG_ACT
		if (tst_flg==SET){
			prnt_char('n');prnt_char(':');prnt_byte(n);
			prnt_char(CR);
			prnt_char(CR);
		}
#endif
#ifdef LINKLAYER_DEBUG_ACT
		if (lapb_dbg_flg == SET){
			prnt_char('r');prnt_char('s');prnt_char('n');prnt_char('d');prnt_char('I');prnt_char(':');
			prnt_byte(n);prnt_char('n');prnt_char('s');prnt_byte(retxwin[n][retx_index[n]].Ns);prnt_char('n');prnt_char('r');prnt_byte(Vr[n]);
			prnt_char('-');
		}
#endif
		return OK;
	}
	prnt_msg(LAPB_ERROR,0x4024);
	return FAIL;
}

/* ======================================================================*/

void save_tx_information(BYTE n,BYTE type,BYTE subtype,BYTE pf,BYTE nxtstate,WORD_LAPB count){

/*
	 type	  : type of pending frame :  ITX,  IRETX,   S	  , U
	 subtype  : subtype of pending frame NULL, NULL ,   S_cmd , U_cmd
	 pf	  : P/F bit of pending frame.
	 nxtstate : next state after exit from pending condition.
	 count	  : timer count of pending frame if needed.
*/
	prnt_msg(LAPB_ERROR,0x4026);
	curr_tx[n].type = type;
	curr_tx[n].subtype = subtype;
	curr_tx[n].pf = pf;
	curr_tx[n].nxtstate = nxtstate;
	curr_tx[n].count = count;
}
/* ======================================================================*/

void check_transmission_link(BYTE n){
	switch (curr_tx[n].type){
	  case LAPB_I_FRAME  :
				if ( send_Iframe(n,curr_tx[n].pf) == OK ){
					state[n] = curr_tx[n].nxtstate;
					publictmr[n].active = FALSE;
				}
				return;
	  case IRETX_FRAME   :
				if ( retx_Iframe(n) == OK ){
					state[n] = curr_tx[n].nxtstate;
					publictmr[n].active = FALSE;
				}
				return;
	  case S_FRAME	     :
				if ( send_Sframe(n,curr_tx[n].subtype,curr_tx[n].pf,curr_tx[n].count) == OK ){
					state[n] = curr_tx[n].nxtstate;
					publictmr[n].active = FALSE;
				}
				return;
	  case U_FRAME	     :
				if ( send_Uframe(n,curr_tx[n].subtype,curr_tx[n].pf,curr_tx[n].count) == OK){
					state[n] = curr_tx[n].nxtstate;
					publictmr[n].active = FALSE;
				}
				return;
	}
}
/* ======================================================================*/

BYTE put_retxwin(WINBUF *buf,WIN_ST *dat){

	WORD_LAPB temp_rear;

	if (buf->rear == (buf->size -1))
		temp_rear=0;
	else
		temp_rear = buf->rear + 1;
	if (buf->front == temp_rear){
		prnt_msg(LAPB_ERROR,0x4029);
		return FAIL;  /* tx_buf is full*/
	}
	else{
		buf->rear = temp_rear;
	}
	buf->arr[buf->rear].pf = dat->pf;
	buf->arr[buf->rear].Ns = dat->Ns;
	buf->arr[buf->rear].netindex = dat->netindex;
	buf->arr[buf->rear].retry_no = dat->retry_no;
	buf->arr[buf->rear].count = dat->count;
	return OK;
}

/* ======================================================================*/

BYTE get_retxwin(WINBUF *buf,WIN_ST *dat){

	if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
	if (buf->front == (buf->size -1))
		buf->front=0;
	else{
		buf->front++ ;
	}
	dat->pf = buf->arr[buf->front].pf ;
	dat->Ns = buf->arr[buf->front].Ns ;
	dat->netindex = buf->arr[buf->front].netindex ;
	dat->retry_no = buf->arr[buf->front].retry_no ;
	dat->count = buf->arr[buf->front].count ;
	return OK;

}

/* ======================================================================*/
void generic_linklayer_handler(BYTE link_lapb_scn_no){
      BYTE stimulus;
      BYTE event;
      if (sync_flag[link_lapb_scn_no] != SET){
	      return;
      }

// this check_expired_timers() has become really a non-manageable issue
// not only now it differs for fast_handler() called every 10msec and "normal"
// handler() called every 20msec, but also what happens if link_rtn[][]()
// returns FINISH?! it shall not continue the MAX_LINK_LAPB_SCAN_PER_LINK loop
// till the end and COUNT_ADJUST_VALUE becomes irrelevant!

#if 0

      for(stimulus=0;stimulus<MAX_LINK_LAPB_SCAN_PER_LINK;stimulus++){
	if ((event=check_expired_timers(link_lapb_scn_no)) == FAIL) {
		if ((state[link_lapb_scn_no] != TX_PEND) && (state[link_lapb_scn_no] != RX_PEND)){
			if ( read_received_frame(link_lapb_scn_no) == OK){
				switch (curr_rx[link_lapb_scn_no].ftype){
				  case LAPB_I_FRAME : event = I_RX;
						 break;
				  case S_FRAME : switch (curr_rx[link_lapb_scn_no].f.s.S_cmd){
						   case LAPB_RR   : event = RR_RX;break;
						   case RNR  : event = RNR_RX;;break;
						   case REJ  : event = REJ_RX;break;
						   default   : event = BAD_CMD;break;
						 }
						 break;
				  case U_FRAME : switch (curr_rx[link_lapb_scn_no].f.u.U_cmd){
						   case LAPB_SABM : event = SABM_RX;break;
						   case DISC : event = DISC_RX;break;
						   case LAPB_UA   : event = UA_RX;break;
						   case FRMR : event = FRMR_RX;break;
						   case DM   : event = DM_RX;break;
						   default   : event = BAD_CMD;break;
						}

						 break;
				}
			}
			else{
				event = LAPB_NULL_EVENT;
			}
		}
		else{
			event = LAPB_NULL_EVENT;
	       }
	}
//#ifdef DEBUG_ACT
	dbg_event=event;
//#endif
	if ( link_rtn[state[link_lapb_scn_no]][event](link_lapb_scn_no) == FINISH ) {
#ifdef DEBUG_ACT
	   if (tst_flg == SET){
		prnt_byte(link_lapb_scn_no);prnt_char('-');
		prnt_byte(state[link_lapb_scn_no]);prnt_char('-');
		prnt_byte(event);prnt_char(CR);
	    }
#endif
//	    continue;
	    goto next;
	}
/*	link_rtn[state[link_lapb_scn_no]][event](link_lapb_scn_no);  */
      } // stimulus=
next:
      return;

#endif //0

// new strategy for check_expired_timers() as below

    event=check_expired_timers(link_lapb_scn_no);
    if(event != FAIL){
	dbg_event=event;
	if(link_rtn[state[link_lapb_scn_no]][event](link_lapb_scn_no) == FINISH)
	    return;
    }
    for(stimulus=0;stimulus<MAX_LINK_LAPB_SCAN_PER_LINK;stimulus++){
	if((state[link_lapb_scn_no] != TX_PEND) && (state[link_lapb_scn_no] != RX_PEND)){
	    if(read_received_frame(link_lapb_scn_no) == OK){
	      switch (curr_rx[link_lapb_scn_no].ftype){
		case LAPB_I_FRAME : event = I_RX;
			       break;
		case S_FRAME : switch (curr_rx[link_lapb_scn_no].f.s.S_cmd){
				 case LAPB_RR	: event = RR_RX;break;
				 case RNR  : event = RNR_RX;;break;
				 case REJ  : event = REJ_RX;break;
				 default   : event = BAD_CMD;break;
			       }
			       break;
		case U_FRAME : switch (curr_rx[link_lapb_scn_no].f.u.U_cmd){
				 case LAPB_SABM : event = SABM_RX;break;
				 case DISC : event = DISC_RX;break;
				 case LAPB_UA	: event = UA_RX;break;
				 case FRMR : event = FRMR_RX;break;
				 case DM   : event = DM_RX;break;
				 default   : event = BAD_CMD;break;
			       }

			       break;
	      }
	    }
	    else
		event = LAPB_NULL_EVENT;
	}
	else
	    event = LAPB_NULL_EVENT;
	dbg_event=event;
	if(link_rtn[state[link_lapb_scn_no]][event](link_lapb_scn_no) == FINISH)
	    return;
    }
}
void linklayer_handler(void){
    BYTE n;
    for (n=0;n<MAX_LINK_LAPB_SCAN;n++){
      if( (++link_lapb_scn_no) >= NO_LAPB_CH)
	      link_lapb_scn_no=0;
      if (hdlc_ch[link_lapb_scn_no].fast_link == SET){
	      continue;
      }
      generic_linklayer_handler(link_lapb_scn_no);
    }
}
void linklayer_fast_handler(void){
    BYTE n;
    for (n=0;n<MAX_LINK_LAPB_SCAN;n++){
      if( (++link_lapb_scn_no) >= NO_LAPB_CH)
	      link_lapb_scn_no=0;
      if (hdlc_ch[link_lapb_scn_no].fast_link == RESET){
	      continue;
      }
      generic_linklayer_handler(link_lapb_scn_no);
    }
}
/* ======================================================================*/
/**  Function : link_rtn_null() 					 */
/**									 */
/* ======================================================================*/
BYTE link_rtn_null(BYTE n){
//#ifdef DEBUG_ACT
if (dbg_event != LAPB_NULL_EVENT){
	prnt_msg(LAPB_RTN,0x600);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
	prnt_char('-');prnt_byte(state[n]);prnt_char('-');
	prnt_byte(dbg_event);
	prnt_char(CR);
}
//#endif
	return FINISH;
}
/**************************************************************************/
/**  Function : link_reset()						  */
/**		initialize link layer					  */
/**************************************************************************/
BYTE link_reset(BYTE n){

//#ifdef DEBUG_ACT
	prnt_msg(LAPB_RTN,0x500);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
	prnt_char('-');prnt_byte(state[n]);prnt_char('-');
	prnt_byte(dbg_event);
	prnt_char(CR);
//#endif

//	initialize_linklayer(n);
	reset_linklayer(n);
	state[n] = IDLE;
	return FINISH;
}
/**************************************************************************/
/**  Function : link_hw_reset() 					  */
/**		initialize link layer + physical layer			  */
/**************************************************************************/
BYTE link_hw_reset(BYTE n){
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
	if (hdlc_ch[n].ic_type == IC_HDLC_ON_UDP){
		// just ignore!
	}
	else
#endif
	{
//#ifdef DEBUG_ACT
	    prnt_char('l');prnt_char('i');prnt_char('n');prnt_char('k');prnt_char('-');prnt_char('h');
	    prnt_char('w');prnt_char('-');prnt_char('r');prnt_char('e');prnt_char('s');prnt_char('e');
	    prnt_char('t');prnt_char(CR);
	    prnt_byte(curr_rx[n].ftype);prnt_char('-');
	    prnt_byte(curr_rx[n].f.s.S_cmd);prnt_char('-');
	    prnt_byte(curr_rx[n].f.u.U_cmd);prnt_char(CR);
//#endif
//	    while (initialize_hwlayer(n) == FAIL);
	    init_hdlc_ch(n);
//	    initialize_hwlayer(n);
	    link_reset(n);
	}
	return FINISH;
}

/**************************************************************************/
/**************************************************************************/
void set_lapb_link_flag(BYTE n){
	if(lapb_link_flag[n] == RESET){
	    lapb_link_flag[n]=SET;
#ifdef SYSTEM_SIDE
#ifdef RSU_CCSV5_INSTALLED
#ifdef ALARM_ENABLE
	    if(n < NO_RSU){
		pmamftx(MA_SYS_P | MAC_ALARM, CONNECTION_ALARM |
			 RSU_CONNECTION_ESTABLISHED | n,ALARM_OPR);
	    }
#endif
#endif
#endif
//PIANO start
#ifdef OPERATOR_SIDE
#ifndef HOT_STANDBY
	    reset_calls_report_status();
#endif
#endif
//PIANO end
	}
}
/**************************************************************************/
/**  Function : link_rtn1() = link_rtn[IDLE][SABM_RX]			  */
/**		action : send UA_FRAME					  */
/* to be completed. I don't know when it should send DM instead of UA     */
/**************************************************************************/
BYTE link_rtn1(BYTE n){
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x501);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif


	if ( send_Uframe(n,LAPB_UA,PF_SET,NO_TMR) == OK ){
		state[n] = INFO_TRX;
		Vr[n]=Vs[n]=0;
		set_lapb_link_flag(n);
		return CONTINUE ;
	}
	else{
		save_tx_information(n,U_FRAME,LAPB_UA,PF_SET,INFO_TRX,NO_TMR);
		tx_pend(n,MAX_TX_PEND_COUNT);
		return FINISH;
	}
}

/**************************************************************************/
/**  Function : link_rtn2() = link_rtn[IDLE][NULL_EVENT]		  */
/**		action :  send SABM_FRAME				  */
/**************************************************************************/

BYTE link_rtn2(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x502);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

	if ( send_Uframe(n,LAPB_SABM,PF_SET,RX_UA_COUNT) == OK ){
		state[n] = WAIT_FOR_UA;
		return FINISH;
	}
	else{
		save_tx_information(n,U_FRAME,LAPB_SABM,PF_SET,WAIT_FOR_UA,RX_UA_COUNT);
		return FINISH;
	}
}
/**************************************************************************/
/**  Function : link_rtn3() = link_rtn[WAIT_FOR_UA][UA_RX]		  */
/**		action : release timer,new state = INFO_TRX		  */
/**************************************************************************/
BYTE link_rtn3(BYTE n){
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x503);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif


	publictmr[n].active = FALSE;
	state[n] = INFO_TRX;
	Vr[n]=Vs[n]=0;
	set_lapb_link_flag(n);
	return CONTINUE;
}

/**************************************************************************/
/**  Function : link_rtn4() = link_rtn[WAIT_FOR_UA][SABM_RX]		  */
/*		action : send UA , still waits for UA . 		  */
/**************************************************************************/
BYTE link_rtn4(BYTE n){
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x504);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

	if ( send_Uframe(n,LAPB_UA,PF_SET,MAX_LINK_SETUP_COUNT) == OK ){
		state[n] = WAIT_FOR_LINK_SETUP;
		return FINISH ;    /* should stay here for receiving UA */
	}
	else{
		save_tx_information(n,U_FRAME,LAPB_UA,PF_SET,INFO_TRX,MAX_LINK_SETUP_COUNT);
		tx_pend(n,MAX_TX_PEND_COUNT);
		return FINISH;
	}
}
/**************************************************************************/
/**  Function : link_rtn5() = link_rtn[WAIT_FOR_UA][DISC_RX]		  */
/*		action : send UA,go to IDLE state			  */
/**************************************************************************/
BYTE link_rtn5(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x505);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

	if ( send_Uframe(n,LAPB_UA,PF_SET,WAIT_BEFORE_RESET_COUNT) == OK ){
		state[n] = WAIT_BEFORE_RESET;
		return FINISH;
	/* should wait to be sure that UA has been sent */
	}
	else{
		save_tx_information(n,U_FRAME,LAPB_UA,PF_SET,WAIT_BEFORE_RESET,WAIT_BEFORE_RESET_COUNT);
		tx_pend(n,MAX_TX_PEND_COUNT);
		return FINISH;
       }
}

/**************************************************************************/
/**  Function : link_rtn6() = link_rtn[WAIT_FOR_UA][LAPB_TIME_OUT]	       */
/**		action : release timer ,goto IDLE state .		  */
/**************************************************************************/
BYTE link_rtn6(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x506);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

//	prnt_msg(LAPB_ERROR,0x5060);

	publictmr[n].active = FALSE;
	link_rtn2(n);
	return FINISH;
}
/**************************************************************************/
/**  Function : link_rtn7() = link_rtn[INFO_TRX][I_RX]			  */
/**		action : check rx_ed N(R),remove acked frames . 	  */
/**			 check rx_ed N(S),save frame if its id is correct */
/**			 if Iframe ready to tx,send it with new local N(R)*/
/**			 and N(S),else					  */
/**			if (ACK_ON_REQ) send RR if p/f=1 .		  */
/**			if !(ACK_ON_REQ) send RR.			  */
/**************************************************************************/

BYTE link_rtn7(BYTE n){
#ifdef LINKLAYER_DEBUG_ACT
	BYTE j;
#endif

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x507);prnt_char('-');
	prnt_byte(n);prnt_char('-');prnt_char(CR);
}
#endif
	if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.i.Nr ) == FAIL ){
		reset_linklayer(n);
		if ( send_Uframe(n,FRMR,PF_RESET,NO_TMR) == OK ){
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,LAPB_UA,PF_RESET,IDLE,NO_TMR);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
/*
	if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.i.Nr ) == FAIL ){
		if ( send_Uframe(n,FRMR,PF_RESET,WAIT_BEFORE_RESET_COUNT) == OK ){
			state[n] = WAIT_BEFORE_RESET;
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,LAPB_UA,PF_RESET,WAIT_BEFORE_RESET,WAIT_BEFORE_RESET_COUNT);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
*/
	/*
	   Check if received frame id is correct :
	   Ns = Vr : good frame.
	   Ns < Vr : bad frame ,(repetitive frame)
	   Ns > Vr : bad frame ,one or more frames are lost.
	*/
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_char('V');prnt_char('r');
	prnt_byte(Vr[n]);prnt_char('n');prnt_char('s');
	prnt_byte(curr_rx[n].f.i.Ns);prnt_char('n');prnt_char('r');
	prnt_byte(curr_rx[n].f.i.Nr);
	prnt_char(CR);
	for ( j=0;j<curr_rx[n].f.i.size;j++ ){
		prnt_byte(curr_rx[n].f.i.tmparr[j]);prnt_char('-');
	}
	prnt_char(CR);
}
#endif
	if ( Vr[n] == ( (curr_rx[n].f.i.Ns ) & TRI_BIT) ){
		REJ_flag[n] = FALSE;
		//Vr[n] = (++Vr[n]) & TRI_BIT;
		Vr[n] = (Vr[n]+1) & TRI_BIT;
		if (save_received_frame(n) == OK){
//			packet_no++;
			if (send_Iframe(n,PF_RESET)==OK){ /* note : following comment */
				return CONTINUE;
			}
			else{
				if (send_Sframe(n,LAPB_RR,PF_RESET,NO_TMR)==OK){ /* note : following comment */
					return CONTINUE;
				}
				else{
					save_tx_information(n,S_FRAME,LAPB_RR,PF_SET,INFO_TRX,NO_TMR);
					tx_pend(n,MAX_TX_PEND_COUNT);
					return FINISH;
				}
			}
		}
		else{
			publictmr[n].count = MAX_RX_PEND_COUNT;
			publictmr[n].active = TRUE;
			state[n] = RX_PEND;
			if (send_Sframe(n,RNR,PF_RESET,NO_TMR)==OK){
				return FINISH;
			}
			else{
				save_tx_information(n,S_FRAME,RNR,PF_RESET,RX_PEND,NO_TMR);
				tx_pend(n,MAX_TX_PEND_COUNT);
				return FINISH;
			}
		}
	}
	else{
		if (REJ_flag[n] == FALSE){
				       /* N(S) is not correct */
			if (send_Sframe(n,REJ,PF_RESET,NO_TMR)==OK){ /* note : following comment */
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
		prnt_msg(LAPB_RTN,0x5070);prnt_char('-');prnt_byte(n);
		prnt_char('-'); prnt_char('V');prnt_char('r');  prnt_byte(Vr[n]);
		prnt_char('n');prnt_char('s');
		prnt_byte(curr_rx[n].f.i.Ns & TRI_BIT);prnt_char('-');
		prnt_char(CR);
}
#endif
			/* Having sent one REJ frame, station does not send  */
			/* any further REJ frames until a correct frame has  */
			/* been received.*/

				REJ_flag[n] = TRUE;
				return CONTINUE;
			}
			else{
				save_tx_information(n,S_FRAME,REJ,PF_RESET,INFO_TRX,NO_TMR);
				tx_pend(n,MAX_TX_PEND_COUNT);
				return FINISH;
			}
		}
		else{	/* REJ should not be sent. */
			return CONTINUE;
		 }
	}
}
/**************************************************************************/

/* I don't know if I should send RR or REJ if frame id is not correct.two  */
/* conditions may occur. Consider the following situations :		   */

/*   transmitter	  receiver					   */
/* A:  3,4,5,6,7,0,1	  expected next frame to rx:3			   */
/* B:  1,2,3,4,5,6,7							   */
/* If frame 1 has been received instead of frame 3 :			   */
/* A : transmitter has sent 3,4,5,6,7,0,1 but receiver has not received    */
/*     3,4,5,6,7,0 .(frames are corrupted) only frame 1 is received.	   */
/*     receiver should send REJ(3).					   */
/* B : transmitter has sent frames 1,2 before,but has not received ACK.    */
/*     (ACK is corrupted.) so xmiter should send 1,2 again. but receiver   */
/*     has received it before,so should ignore it and send RR(3)	   */
/*     Now in receiver side I can not differ between these two conditions. */
/*     So for the time being , if received N(S) != V(R) REJ with V(R) is   */
/*     sent.								   */
/***************************************************************************/
/**  Function : link_rtn8() = link_rtn[INFO_TRX][RR_RX] 		   */
/**		action : if (ACK_ON_REQ) : remove acked frames if p/f=1    */
/**			 if !(ACK_ON_REQ): remove acked frames		   */
/***************************************************************************/
BYTE link_rtn8(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x508);prnt_char('-');
	prnt_byte(n);prnt_char('n');prnt_char('r');
	prnt_byte(curr_rx[n].f.s.Nr);prnt_char(CR);
}
#endif
	if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.s.Nr) == FAIL ){
		reset_linklayer(n);
		if ( send_Uframe(n,FRMR,PF_RESET,NO_TMR) == OK ){
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,FRMR,PF_RESET,IDLE,NO_TMR);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
/*
	if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.s.Nr) == FAIL ){
		if ( send_Uframe(n,FRMR,PF_RESET,WAIT_BEFORE_RESET_COUNT) == OK ){
			state[n] = WAIT_BEFORE_RESET;
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,FRMR,PF_RESET,WAIT_BEFORE_RESET,WAIT_BEFORE_RESET_COUNT);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
*/
	switch ( send_Iframe(n,PF_RESET) ){

	  case OK	      : return CONTINUE ;
	  case FAIL_NO_DATA   : return /*CONTINUE*/FINISH ;
	  case FAIL_WIN_CLOSE : return /*CONTINUE*/FINISH ;
	  case FAIL_BUF_FULL  :
#ifdef SYSTEM_SIDE
				save_tx_information(n,LAPB_I_FRAME,NULL,PF_RESET,INFO_TRX,NO_TMR);
#endif
#ifdef OPERATOR_SIDE
				save_tx_information(n,LAPB_I_FRAME,KT_NULL,PF_RESET,INFO_TRX,NO_TMR);
#endif
				tx_pend(n,MAX_TX_PEND_COUNT);
				return FINISH;
	  default	      : return FINISH;
	}

}
/***************************************************************************/
/**  Function : link_rtn9() = link_rtn[INFO_TRX][RNR_RX]		   */
/*		action : remove acked frames,goto REMOTE_STN_BUSY state    */
/***************************************************************************/

BYTE link_rtn9(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x509);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif
	if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.s.Nr) == FAIL ){
		reset_linklayer(n);
		if ( send_Uframe(n,FRMR,PF_RESET,NO_TMR) == OK ){
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,FRMR,PF_RESET,IDLE,NO_TMR);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
/*
	if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.s.Nr) == FAIL ){
		if ( send_Uframe(n,FRMR,PF_RESET,WAIT_BEFORE_RESET_COUNT) == OK ){
			state[n] = WAIT_BEFORE_RESET;
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,FRMR,PF_RESET,WAIT_BEFORE_RESET,WAIT_BEFORE_RESET_COUNT);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
*/
	/* added 76/10/22 */
	publictmr[n].count = MAX_RX_PEND_COUNT;
	publictmr[n].active = TRUE;
	/**/
	state[n] = REMOTE_STN_BUSY;
	return FINISH;
}
/***************************************************************************/
/**  Function : link_rtn10() = link_rtn[INFO_TRX][REJ_RX]		   */
/**		action : remove acked frames , make rejected frames ready  */
/**		for retransmission.(windat->count=0)			   */
/***************************************************************************/
BYTE link_rtn10(BYTE n){

	BYTE tot,j;
	WIN_ST windat;
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x510);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

      if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.s.Nr) == FAIL ){
		reset_linklayer(n);
		if ( send_Uframe(n,FRMR,PF_RESET,NO_TMR) == OK ){
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,FRMR,PF_RESET,IDLE,NO_TMR);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
/*
	if ( xxxx(n,curr_rx[n].f.s.Nr) == FAIL ){

		if ( send_Uframe(n,FRMR,PF_RESET,WAIT_BEFORE_RESET_COUNT) == OK ){

			state[n] = WAIT_BEFORE_RESET;
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,FRMR,PF_RESET,WAIT_BEFORE_RESET,WAIT_BEFORE_RESET_COUNT);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
*/
	/* action on receiving REJ is the same as LAPB_TIME_OUT event.so LAPB_TIME_OUT */
	/* is simulated for remaining frames in retxwin.*/

	tot = retxwin_st[n].win_size - checkfree_buf((BUF*)&retxwin_st[n]);
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_char('t');prnt_char('o');prnt_char('t');prnt_char(':');
	prnt_byte(tot);prnt_char('n');prnt_char('r');
	prnt_byte(curr_rx[n].f.s.Nr);prnt_char('-');prnt_char(CR);
}
#endif

	for (j=0;j<tot;j++){
	    if(get_retxwin(&retxwin_st[n],&windat) == OK){
		windat.count = 0;
		if ( windat.retry_no++ < MAX_RETRY_NO){
			put_retxwin(&retxwin_st[n],&windat);
		}
		else{
/* to be checked if it is necessary to send FRMR or RESET */
			link_reset(n);
			return FINISH;
		}
	    }
	    else
		break;
	}
	return CONTINUE;

	/* ambigious condition if there is no frame in retxwin.receiving */
	/* REJ is meaningless.continue anyway.*/
}
/***************************************************************************/
/**  Function : link_rtn11() = link_rtn[INFO_TRX][NULL_EVENT]		   */
/*		action : checks if Iframe ready to tx.			   */
/***************************************************************************/
BYTE link_rtn11(BYTE n){

	switch ( send_Iframe(n,PF_RESET) ){

	  case OK	      :
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x511);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif
				return CONTINUE ;

	  case FAIL_NO_DATA   : return FINISH ;
	  case FAIL_WIN_CLOSE : return FINISH ;
	  case FAIL_BUF_FULL  :
#ifdef SYSTEM_SIDE
				save_tx_information(n,LAPB_I_FRAME,NULL,PF_RESET,INFO_TRX,NO_TMR);
#endif
#ifdef OPERATOR_SIDE
				save_tx_information(n,LAPB_I_FRAME,KT_NULL,PF_RESET,INFO_TRX,NO_TMR);
#endif
				tx_pend(n,MAX_TX_PEND_COUNT);
				return FINISH;
	 default	      : return FINISH;
	}

}

/***************************************************************************/
/**  Function : link_rtn12() = link_rtn[TX_PEND][]			   */
/*		action : if hw tx buffer ready,sends pending frame is ready*/
/*			 goto saved state.				   */
/***************************************************************************/
BYTE link_rtn12(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x512);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

	check_transmission_link(n);
	return CONTINUE;
}
/***************************************************************************/
/**  Function : link_rtn13() = link_rtn[INFO_TRX][PACKET_TIME_OUT]		  */
/**		action : index of retransmit win (the frame which should   */
/**		be retransmitted) is a global variable "retx_index".    */
/**		it has been found in handler.				   */
/***************************************************************************/

BYTE link_rtn13(BYTE n){
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x513);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

	if (retxwin[n][retx_index[n]].retry_no++ < MAX_RETRY_NO){
		if ( retx_Iframe(n) == OK)
			return CONTINUE;
		else{
#ifdef SYSTEM_SIDE
			save_tx_information(n,IRETX_FRAME,NULL,PF_RESET,INFO_TRX,NO_TMR);
#endif
#ifdef OPERATOR_SIDE
			save_tx_information(n,IRETX_FRAME,KT_NULL,PF_RESET,INFO_TRX,NO_TMR);
#endif
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
	else{
/* sending RESET command to be added if necessary */
		link_reset(n);
		return FINISH;
	}
}
/***************************************************************************/
/**  Function : link_rtn14() = link_rtn[REMOTE_STN_BUSY][RR_RX] 	   */
/**		action : removes acked frames, goto INFO_TRX state.	   */
/***************************************************************************/
BYTE link_rtn14(BYTE n){
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x514);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif
	if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.s.Nr) == FAIL ){
		reset_linklayer(n);
		if ( send_Uframe(n,FRMR,PF_RESET,NO_TMR) == OK ){
			state[n] = WAIT_BEFORE_RESET;
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,FRMR,PF_RESET,IDLE,NO_TMR);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
/*
	if ( check_NR_remove_ACKedframes(n,curr_rx[n].f.s.Nr) == FAIL ){
		if ( send_Uframe(n,FRMR,PF_RESET,WAIT_BEFORE_RESET_COUNT) == OK ){
			state[n] = WAIT_BEFORE_RESET;
			return FINISH;
		}
		else{
			save_tx_information(n,U_FRAME,FRMR,PF_RESET,WAIT_BEFORE_RESET,WAIT_BEFORE_RESET_COUNT);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
*/
	/* added 76/10/22 */
	publictmr[n].active = FALSE;
	/**/
	state[n] = INFO_TRX;
	set_lapb_link_flag(n);
	return CONTINUE;
}
/****************************************************************************/
/**  Function : link_rtn15() = link_rtn[RX_PEND][]			    */
/**		save pending frame if possible,goto INFO_TRX state	    */
/****************************************************************************/
BYTE link_rtn15(BYTE n){
#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x515);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

	if (save_received_frame(n) == OK){
		publictmr[n].active = FALSE;
		if (send_Sframe(n,LAPB_RR,PF_RESET,NO_TMR)==OK){ /* note : following comment */
			state[n] = INFO_TRX;
			set_lapb_link_flag(n);
			return CONTINUE;
		}
		else{
			save_tx_information(n,S_FRAME,LAPB_RR,PF_SET,INFO_TRX,NO_TMR);
			tx_pend(n,MAX_TX_PEND_COUNT);
			return FINISH;
		}
	}
	else	return FINISH;

}
/****************************************************************************/
/**  Function : link_rtn16() = link_rtn[WAIT][LAPB_TIME_OUT]			 */
/**		acquired time before link_reset is passed.reset it now.     */
/****************************************************************************/
BYTE link_rtn16(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x516);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

	prnt_msg(LAPB_ERROR,0x5160);
	publictmr[n].active = FALSE;
	link_reset(n);
	return FINISH;
}
/****************************************************************************/
/**  Function : link_rtn17() = link_rtn[WAIT_FOR_SETUP][LAPB_TIME_OUT]	    */
/**		acquired time before link_reset is passed.reset it now.     */
/****************************************************************************/
BYTE link_rtn17(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x517);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif

//	prnt_msg(LAPB_ERROR,0x5170);

	publictmr[n].active = FALSE;
	state[n]=INFO_TRX;
	Vr[n]=Vs[n]=0;
	set_lapb_link_flag(n);
	return CONTINUE;
}



/****************************************************************************/
/**  Function : link_rtn18() = link_rtn[WAIT_FOR_SETUP][I_RX]		    */
/**		acquired time before link_reset is passed.reset it now.     */
/****************************************************************************/
BYTE link_rtn18(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x518);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif
	link_rtn3(n);
	link_rtn7(n);

	return CONTINUE;
}
/**************************************************************************/
/**  Function : link_rtn19() = link_rtn[INFO_TRX][SABM_RX]		  */
/**************************************************************************/
BYTE link_rtn19(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x519);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif
	reset_linklayer(n);
	if ( send_Uframe(n,LAPB_UA,PF_SET,NO_TMR) == OK ){
		state[n] = INFO_TRX;
		set_lapb_link_flag(n);
		return FINISH;
	/* should wait to be sure that UA has been sent */
	}
	else{
		save_tx_information(n,U_FRAME,LAPB_UA,PF_SET,INFO_TRX,NO_TMR);
		tx_pend(n,MAX_TX_PEND_COUNT);
		return FINISH;
       }
}
/**************************************************************************/
/**  Function : link_rtn20() = link_rtn[WAIT_FOR_LINK_SETUP][SABM_RX]	  */
/**************************************************************************/
BYTE link_rtn20(BYTE n){

#ifdef LINKLAYER_DEBUG_ACT
if (lapb_dbg_flg == SET){
	prnt_msg(LAPB_RTN,0x520);prnt_char('-');
	prnt_byte(n);prnt_char(CR);
}
#endif
	if ( send_Uframe(n,LAPB_UA,PF_SET,NO_TMR) == OK ){
		state[n] = INFO_TRX;
		set_lapb_link_flag(n);
		publictmr[n].active = FALSE;
		return FINISH;
	}
	else{
		save_tx_information(n,U_FRAME,LAPB_UA,PF_SET,INFO_TRX,NO_TMR);
		tx_pend(n,MAX_TX_PEND_COUNT);
		return FINISH;
       }
}
/***************************************************************************/

BYTE check_expired_timers(BYTE n){

/* During each call , only one timeout is reported.publictmr has higher
   priority than packet timeout. */

	WIN_ST windat;
	BYTE tot,i,found;

	found=FAIL;

	if ( publictmr[n].active == TRUE ){
		if ( publictmr[n].count == 0 ){
			found = LAPB_TIME_OUT;
		}
		else	publictmr[n].count--;
	}
	tot=retxwin_st[n].win_size - checkfree_buf((BUF *)&retxwin_st[n]);
	for ( i=0;i<tot;i++ ){
	    if(get_retxwin(&retxwin_st[n],&windat) == OK){
		if ( windat.count != 0){
			windat.count--;
			put_retxwin(&retxwin_st[n],&windat);
		}
		else{
			if (found == FAIL){
				put_retxwin(&retxwin_st[n],&windat);
				retx_index[n] = retxwin_st[n].rear;
				found = PACKET_TIME_OUT;
			}
			else {
				put_retxwin(&retxwin_st[n],&windat);
			}
		}
	    }
	    else
		break;
	}
	return (found);
}


/***************************************************************************/
void initialize_network_buf(BYTE n){

	nettx_st[n].front = nettx_st[n].rear = 0;
	nettx_st2[n].front = nettx_st2[n].rear = 0;
	netrx_st[n].front = netrx_st[n].rear = 0;

//	netrx_st[n].arr = &netrx[n][0];
//	netrx_st[n].size = RX_NETBUF_SIZE;
}
/* ====================================================================== */
/* ====================================================================== */

void init_lapb(void){

/*
		   LAPB
		     ³
	ÚÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
    ÚÄÄÄÁÄÄÄÄ¿(OPREATOR_SIDE) ÚÄÄÄÄÁÄÄÄ¿   (SYSTEM_SIDE)
    ³OPERATOR³		      ³ SYSTEM ³
    ÀÄÄÄÂÄÄÄÄÙ		      ÀÄÄÄÂÄÄÄÄÙ
    ÚÄÄÄÁÄÄÄÄÄÄ¿       ÚÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄ¿
    ³HDLC_RS232³       ³		      ³
    ÀÄÄÄÄÄÄÄÄÄÄÙ      9075 (EC2K_SYSTEM)      8952  !(EC2K_SYSTEM)
		       ³	     ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
		  ÚÄÄÄÄÁÄÄÄÄÄ¿	     ³ (RSUSIDE)	 ³(RSU_CCSV5_INSTALLED)
   (HDLC_9075)	  ³HDLC_9075 ³	    RSU 		MAIN SYS
		  ÃÄÄÄÄÄÄÄÄÄÄ´	     ³			 ³
   (HDLC_RS232)   ³HDLC_RS232³	ÚÄÄÄÄÁÄÄÄÄÄ¿	    ÚÄÄÄÄÁÄÄÄÄÄ¿
		  ÀÄÄÄÄÄÄÄÄÄÄÙ	³HDLC_8952 ³	    ³HDLC_8952 ³
				ÀÄÄÄÄÄÄÄÄÄÄÙ	    ÀÄÄÄÄÄÄÄÄÄÄÙ

*/

//LEMON : || (defined MGW672_SYSTEM) added
#if ((defined EC2K_SYSTEM) || (defined EC10K_SYSTEM) || (defined EC1K_SYSTEM) || (defined PU2K_SYSTEM) || (defined MGW672_SYSTEM))
#ifdef SYSTEM_SIDE
#ifdef RSU_CCSV5_INSTALLED
	BYTE i;
#endif
#endif
#endif
	BYTE n;

#ifdef LINKLAYER_DEBUG_ACT
	if (lapb_dbg_flg == SET){
	    prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char('-');
	    prnt_char('l');prnt_char('a');prnt_char('p');prnt_char('b');prnt_char(CR);
	}
#endif

	for (n=0;n<NO_LAPB_CH;n++){
		hdlc_ch[n].fast_link=RESET;
	}

// =================================================

#ifdef OPERATOR_SIDE
#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
#if (TON == 0)
	local_stn[0] = OPR0_ID;
#endif
#if (TON == 1)
	local_stn[0] = OPR1_ID;
#endif
#if (TON == 2)
	local_stn[0] = OPR2_ID;
#endif
#if (TON == 3)
	local_stn[0] = OPR3_ID;
#endif
#if (TON == 4)
	local_stn[0] = OPR4_ID;
#endif
#ifdef RSUSIDE
	remote_stn[0] = THIS_RSU_ID;
#else
	remote_stn[0] = SYSTEM_ID;
#endif
#ifdef HDLC_RS232
	hdlc_ch[0].ic_type=IC_HDLC_RS232;
	hdlc_ch[0].cs=(FIRST_NEW_HDLC_COM_PORT+0);
#endif
#ifdef MY_UDP_FOR_OAM
	hdlc_ch[0].ic_type=IC_HDLC_NOT_INSTALLED;   //IC_HDLC_ON_UDP;
	hdlc_ch[0].dl_ch=0;
#ifdef KIATEL_UDP_INSTALLED
	hdlc_ch[0].cs=NULL_KIATEL_UDP_REC;
#endif
	hdlc_ch[0].fast_link=SET;
#endif

	nettx_st[0].arr = &nettx_oam[0][0];
	nettx_st[0].size = OAM_NETBUF_SIZE;
	nettx_st2[0].arr = &nettx_oam[0][0];
	nettx_st2[0].size = OAM_NETBUF_SIZE;

#ifdef HDLC_RS232
	retxwin_st[0].size = RETX_WIN_BUF_SIZE_1;
	retxwin_st[0].win_size = RETX_WIN_SIZE_1;
#endif
#ifdef MY_UDP_FOR_OAM
	retxwin_st[0].size = RETX_WIN_BUF_SIZE_1;
	retxwin_st[0].win_size = RETX_WIN_SIZE_1;
#endif
	netrx_st[0].arr = &netrx_oam[0][0];
	netrx_st[0].size = OAM_RX_NETBUF_SIZE;

#if ((defined HOT_STANDBY) || ((defined CPU_EC512) && (defined LIM)))
#if (TON == 0)
	local_stn[1] = OPR0_ID;
#endif
#if (TON == 1)
	local_stn[1] = OPR1_ID;
#endif
#if (TON == 2)
	local_stn[1] = OPR2_ID;
#endif
#if (TON == 3)
	local_stn[1] = OPR3_ID;
#endif
#if (TON == 4)
	local_stn[1] = OPR4_ID;
#endif
	remote_stn[1] = SYSTEM_ID;

#ifdef HDLC_RS232
	hdlc_ch[1].ic_type=IC_HDLC_RS232;
	hdlc_ch[1].cs=(FIRST_NEW_HDLC_COM_PORT+1);
#endif
#ifdef MY_UDP_FOR_OAM
	hdlc_ch[1].ic_type=IC_HDLC_NOT_INSTALLED;   //IC_HDLC_ON_UDP;
	hdlc_ch[1].dl_ch=1;
#ifdef KIATEL_UDP_INSTALLED
	hdlc_ch[1].cs=NULL_KIATEL_UDP_REC;
#endif
	hdlc_ch[1].fast_link=SET;
#endif

	nettx_st[1].arr = &nettx_oam[1][0];
	nettx_st[1].size = OAM_NETBUF_SIZE;
	nettx_st2[1].arr = &nettx_oam[1][0];
	nettx_st2[1].size = OAM_NETBUF_SIZE;
#ifdef HDLC_RS232
	retxwin_st[1].size = RETX_WIN_BUF_SIZE_1;
	retxwin_st[1].win_size = RETX_WIN_SIZE_1;
#endif
#ifdef MY_UDP_FOR_OAM
	retxwin_st[1].size = RETX_WIN_BUF_SIZE_1;
	retxwin_st[1].win_size = RETX_WIN_SIZE_1;
#endif
	netrx_st[1].arr = &netrx_oam[1][0];
	netrx_st[1].size = OAM_RX_NETBUF_SIZE;


#endif
#endif
#endif

// =================================================

#ifdef SYSTEM_SIDE
//LEMON start
#ifdef MGW672_SYSTEM
#endif
//LEMON end

#if ((defined EC2K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PC_SYSTEM))

// ################

#ifdef RSUSIDE
	local_stn[CCSV5_DEFAULT_GROUP] = THIS_RSU_ID;
	remote_stn[CCSV5_DEFAULT_GROUP] = SYSTEM_ID;
	hdlc_ch[CCSV5_DEFAULT_GROUP].dl_ch=HDLC_0_9076;
	hdlc_ch[CCSV5_DEFAULT_GROUP].ic_type=IC_HDLC_9076;
	hdlc_ch[CCSV5_DEFAULT_GROUP].cs=dtr[0].cs;

	nettx_st[CCSV5_DEFAULT_GROUP].arr = &nettx_rsu[CCSV5_DEFAULT_GROUP][0];
	nettx_st[CCSV5_DEFAULT_GROUP].size = RSU_NETBUF_SIZE;
	nettx_st2[CCSV5_DEFAULT_GROUP].arr = &nettx_rsu[CCSV5_DEFAULT_GROUP][0];
	nettx_st2[CCSV5_DEFAULT_GROUP].size = RSU_NETBUF_SIZE;

	retxwin_st[CCSV5_DEFAULT_GROUP].size = RETX_WIN_BUF_SIZE_7;
	retxwin_st[CCSV5_DEFAULT_GROUP].win_size = RETX_WIN_SIZE_7;

	netrx_st[CCSV5_DEFAULT_GROUP].arr = &netrx_rsu[CCSV5_DEFAULT_GROUP][0];
	netrx_st[CCSV5_DEFAULT_GROUP].size = RSU_RX_NETBUF_SIZE;
#endif // RSUSIDE

// ################

#ifdef RSU_CCSV5_INSTALLED
#if ((defined HDLC_9075) || (defined HDLC_9076))
	for(i=0;i<NO_RSU;i++){
		//for my test only.to program one EPROM for all of the RSUs.
		if(rsu_st[i].ccs_trnk == 0xff){
		    hdlc_ch[i].ic_type=IC_HDLC_NOT_INSTALLED;
		    continue;
		}
		local_stn[i] = SYSTEM_ID;
		remote_stn[i] = RSU_ID;//+i;
		hdlc_ch[i].cs=dtr[rsu_st[i].ccs_trnk].cs;
#ifdef HDLC_9075
		hdlc_ch[i].dl_ch=HDLC1_9075;
		hdlc_ch[i].ic_type=IC_HDLC_9075;
#endif
#ifdef HDLC_9076
		hdlc_ch[i].dl_ch=HDLC_0_9076;
		hdlc_ch[i].ic_type=IC_HDLC_9076;
#endif

		nettx_st[i].arr = &nettx_rsu[i][0];
		nettx_st[i].size = RSU_NETBUF_SIZE;
		nettx_st2[i].arr = &nettx_rsu[i][0];
		nettx_st2[i].size = RSU_NETBUF_SIZE;

		retxwin_st[i].size = RETX_WIN_BUF_SIZE_7;
		retxwin_st[i].win_size = RETX_WIN_SIZE_7;

		netrx_st[i].arr = &netrx_rsu[i][0];
		netrx_st[i].size = RSU_RX_NETBUF_SIZE;
	}
#endif
#endif // RSU_CCSV5_INSTALLED

// ################

#ifdef HDLC_RS232
	{
	    int i=0,j;
#ifdef ALARM_PANEL_ON_HDLC_RS232
	    i++;
#ifdef RSUSIDE
	    local_stn[FIRST_RS232_HDLC_CH] = THIS_RSU_ID;
#else
	    local_stn[FIRST_RS232_HDLC_CH] = SYSTEM_ID;
#endif
	    remote_stn[FIRST_RS232_HDLC_CH] = ALARM_ID;
	    hdlc_ch[FIRST_RS232_HDLC_CH].ic_type=IC_HDLC_RS232;
	    hdlc_ch[FIRST_RS232_HDLC_CH].cs=(FIRST_NEW_HDLC_COM_PORT+0);
	    hdlc_ch[FIRST_RS232_HDLC_CH].oam_no=0xff;

	    nettx_st[FIRST_RS232_HDLC_CH].arr = &nettx_alarm[0];
	    nettx_st[FIRST_RS232_HDLC_CH].size = ALARM_NETBUF_SIZE;
	    nettx_st2[FIRST_RS232_HDLC_CH].arr = &nettx_alarm[0];
	    nettx_st2[FIRST_RS232_HDLC_CH].size =ALARM_NETBUF_SIZE;

	    retxwin_st[FIRST_RS232_HDLC_CH].size = RETX_WIN_BUF_SIZE_1;
	    retxwin_st[FIRST_RS232_HDLC_CH].win_size = RETX_WIN_SIZE_1;

	    netrx_st[FIRST_RS232_HDLC_CH].arr = &netrx_alarm[0];
	    netrx_st[FIRST_RS232_HDLC_CH].size = ALARM_RX_NETBUF_SIZE;
#endif
	    for(j=0;j<NO_NEW_HDLC_COM_PORT;j++,i++){
#ifdef RSUSIDE
		local_stn[FIRST_RS232_HDLC_CH+i] = THIS_RSU_ID;
#else
		local_stn[FIRST_RS232_HDLC_CH+i] = SYSTEM_ID;
#endif
		remote_stn[FIRST_RS232_HDLC_CH+i] = OPR0_ID+j;
		hdlc_ch[FIRST_RS232_HDLC_CH+i].ic_type=IC_HDLC_RS232;
		hdlc_ch[FIRST_RS232_HDLC_CH+i].cs=(FIRST_NEW_HDLC_COM_PORT+i);
		hdlc_ch[FIRST_RS232_HDLC_CH+i].oam_no=j;

		nettx_st[FIRST_RS232_HDLC_CH+i].arr = &nettx_oam[j][0];
		nettx_st[FIRST_RS232_HDLC_CH+i].size = OAM_NETBUF_SIZE;
		nettx_st2[FIRST_RS232_HDLC_CH+i].arr = &nettx_oam[j][0];
		nettx_st2[FIRST_RS232_HDLC_CH+i].size =OAM_NETBUF_SIZE;

		retxwin_st[FIRST_RS232_HDLC_CH+i].size = RETX_WIN_BUF_SIZE_1;
		retxwin_st[FIRST_RS232_HDLC_CH+i].win_size = RETX_WIN_SIZE_1;

		netrx_st[FIRST_RS232_HDLC_CH+i].arr = &netrx_oam[j][0];
		netrx_st[FIRST_RS232_HDLC_CH+i].size = OAM_RX_NETBUF_SIZE;
	    }
	}
#endif // HDLC_RS232

// ################

#ifdef MY_UDP_FOR_OAM
	{   int i;
	    for(i=0;i<NO_HDLC_UDP_PORT;i++){
#ifdef RSUSIDE
		local_stn[FIRST_UDP_HDLC_CH+i] = THIS_RSU_ID;
#else
		local_stn[FIRST_UDP_HDLC_CH+i] = SYSTEM_ID;
#endif
		remote_stn[FIRST_UDP_HDLC_CH+i] = OPR0_ID+i;
		hdlc_ch[FIRST_UDP_HDLC_CH+i].ic_type=IC_HDLC_NOT_INSTALLED;   //IC_HDLC_ON_UDP;
		hdlc_ch[FIRST_UDP_HDLC_CH+i].dl_ch=i;
#ifdef KIATEL_UDP_INSTALLED
		hdlc_ch[FIRST_UDP_HDLC_CH+i].cs=NULL_KIATEL_UDP_REC;
#endif
#ifdef UDP_ON_CLIENT_SIDE
		hdlc_ch[FIRST_UDP_HDLC_CH+i].cs=NULL_UDP_REC;
#endif
		hdlc_ch[FIRST_UDP_HDLC_CH+i].oam_no=i;
		hdlc_ch[FIRST_UDP_HDLC_CH+i].fast_link=SET;

		nettx_st[FIRST_UDP_HDLC_CH+i].arr = &nettx_oam[i][0];
		nettx_st[FIRST_UDP_HDLC_CH+i].size = OAM_NETBUF_SIZE;
		nettx_st2[FIRST_UDP_HDLC_CH+i].arr = &nettx_oam[i][0];
		nettx_st2[FIRST_UDP_HDLC_CH+i].size =OAM_NETBUF_SIZE;

		retxwin_st[FIRST_UDP_HDLC_CH+i].size = RETX_WIN_BUF_SIZE_1;
		retxwin_st[FIRST_UDP_HDLC_CH+i].win_size = RETX_WIN_SIZE_1;

		netrx_st[FIRST_UDP_HDLC_CH+i].arr = &netrx_oam[i][0];
		netrx_st[FIRST_UDP_HDLC_CH+i].size = OAM_RX_NETBUF_SIZE;
	    }
	}
#endif // MY_UDP_FOR_OAM

// ################

#ifdef HTBY_LINK_ON_MY_UDP

	local_stn[HTBY_HDLC_CH] = SYSTEM_ID;
	remote_stn[HTBY_HDLC_CH] = SYSTEM_ID;
	hdlc_ch[HTBY_HDLC_CH].ic_type=IC_HDLC_NOT_INSTALLED;   //IC_HDLC_ON_UDP;
	hdlc_ch[HTBY_HDLC_CH].dl_ch=5;	// reserve 0-4 for operators
	hdlc_ch[HTBY_HDLC_CH].cs=NULL_KIATEL_UDP_REC;
//	hdlc_ch[HTBY_HDLC_CH].oam_no=0;
	hdlc_ch[HTBY_HDLC_CH].fast_link=SET;

	nettx_st [HTBY_HDLC_CH].arr = &nettx_htby[0][0];
	nettx_st [HTBY_HDLC_CH].size = HTBY_NETBUF_SIZE;
	nettx_st2[HTBY_HDLC_CH].arr = &nettx_htby[0][0];
	nettx_st2[HTBY_HDLC_CH].size = HTBY_NETBUF_SIZE;

	retxwin_st[HTBY_HDLC_CH].size = RETX_WIN_BUF_SIZE_7;
	retxwin_st[HTBY_HDLC_CH].win_size = RETX_WIN_SIZE_7;

	netrx_st [HTBY_HDLC_CH].arr = &netrx_htby[0][0];
	netrx_st [HTBY_HDLC_CH].size = HTBY_RX_NETBUF_SIZE;
#endif // HTBY_LINK_ON_MY_UDP
#endif // EC2K_SYSTEM || EC10K_SYSTEM

// ################

#ifdef EC1K_SYSTEM

// ################

#ifdef MY_UDP_FOR_OAM
	{   int i;
	    for(i=0;i<NO_HDLC_UDP_PORT;i++){
#ifdef RSUSIDE
		local_stn[FIRST_UDP_HDLC_CH+i] = THIS_RSU_ID;
#else
		local_stn[FIRST_UDP_HDLC_CH+i] = SYSTEM_ID;
#endif
		remote_stn[FIRST_UDP_HDLC_CH+i] = OPR0_ID+i;
		hdlc_ch[FIRST_UDP_HDLC_CH+i].ic_type=IC_HDLC_NOT_INSTALLED;   //IC_HDLC_ON_UDP;
		hdlc_ch[FIRST_UDP_HDLC_CH+i].dl_ch=i;
#ifdef KIATEL_UDP_INSTALLED
		hdlc_ch[FIRST_UDP_HDLC_CH+i].cs=NULL_KIATEL_UDP_REC;
#endif
#ifdef UDP_ON_CLIENT_SIDE
		hdlc_ch[FIRST_UDP_HDLC_CH+i].cs=NULL_UDP_REC;
#endif
		hdlc_ch[FIRST_UDP_HDLC_CH+i].oam_no=i;
		hdlc_ch[FIRST_UDP_HDLC_CH+i].fast_link=SET;

		nettx_st[FIRST_UDP_HDLC_CH+i].arr = &nettx_oam[i][0];
		nettx_st[FIRST_UDP_HDLC_CH+i].size = OAM_NETBUF_SIZE;
		nettx_st2[FIRST_UDP_HDLC_CH+i].arr = &nettx_oam[i][0];
		nettx_st2[FIRST_UDP_HDLC_CH+i].size =OAM_NETBUF_SIZE;

		retxwin_st[FIRST_UDP_HDLC_CH+i].size = RETX_WIN_BUF_SIZE_1;
		retxwin_st[FIRST_UDP_HDLC_CH+i].win_size = RETX_WIN_SIZE_1;

		netrx_st[FIRST_UDP_HDLC_CH+i].arr = &netrx_oam[i][0];
		netrx_st[FIRST_UDP_HDLC_CH+i].size = OAM_RX_NETBUF_SIZE;
	    }
	}
#endif // MY_UDP_FOR_OAM

// ################

#ifdef RSUSIDE
	local_stn[0] = THIS_RSU_ID;
	remote_stn[0] = SYSTEM_ID;
	hdlc_ch[0].dl_ch=HDLC_0_9076;
	hdlc_ch[0].ic_type=IC_HDLC_9076;
//	hdlc_ch[0].cs=EC1K_9076_1_CS;
	hdlc_ch[0].cs=dtr[CCSV5_TOWARDS_HOST_TRUNK_INDEX_START].cs;

	nettx_st[0].arr = &nettx_rsu[0][0];
	nettx_st[0].size = RSU_NETBUF_SIZE;
	nettx_st2[0].arr = &nettx_rsu[0][0];
	nettx_st2[0].size = RSU_NETBUF_SIZE;

	retxwin_st[0].size = RETX_WIN_BUF_SIZE_7;
	retxwin_st[0].win_size = RETX_WIN_SIZE_7;

	netrx_st[0].arr = &netrx_rsu[0][0];
	netrx_st[0].size = RSU_RX_NETBUF_SIZE;
#endif

// ################

#ifdef	STAND_ALONE_EC1K

// ################

#ifdef RSU_CCSV5_INSTALLED
#ifdef HDLC_9076
	for(i=0;i<NO_RSU;i++){
		//for my test only.to program one EPROM for all of the RSUs.
		if(rsu_st[i].ccs_trnk == 0xff){
		    hdlc_ch[i].ic_type=IC_HDLC_NOT_INSTALLED;
		    continue;
		}
		local_stn[i] = SYSTEM_ID;
		remote_stn[i] = RSU_ID;//+i;
		hdlc_ch[i].cs=dtr[rsu_st[i].ccs_trnk].cs;
		hdlc_ch[i].dl_ch=HDLC_0_9076;
		hdlc_ch[i].ic_type=IC_HDLC_9076;

		nettx_st[i].arr = &nettx_rsu[i][0];
		nettx_st[i].size = RSU_NETBUF_SIZE;
		nettx_st2[i].arr = &nettx_rsu[i][0];
		nettx_st2[i].size = RSU_NETBUF_SIZE;
		retxwin_st[i].size = RETX_WIN_BUF_SIZE_7;
		retxwin_st[i].win_size = RETX_WIN_SIZE_7;

		netrx_st[i].arr = &netrx_rsu[i][0];
		netrx_st[i].size = RSU_RX_NETBUF_SIZE;
	}
#endif
#endif

// ################

#ifdef HDLC_RS232
	{   int i;
	    for(i=0;i<NO_NEW_HDLC_COM_PORT;i++){
		local_stn[FIRST_RS232_HDLC_CH+i] = SYSTEM_ID;
		remote_stn[FIRST_RS232_HDLC_CH+i] = OPR0_ID+i;
		hdlc_ch[FIRST_RS232_HDLC_CH+i].ic_type=IC_HDLC_RS232;
		hdlc_ch[FIRST_RS232_HDLC_CH+i].cs=(FIRST_NEW_HDLC_COM_PORT+i);
		hdlc_ch[FIRST_RS232_HDLC_CH+i].oam_no=i;

		nettx_st[FIRST_RS232_HDLC_CH+i].arr = &nettx_oam[i][0];
		nettx_st[FIRST_RS232_HDLC_CH+i].size = OAM_NETBUF_SIZE;
		nettx_st2[FIRST_RS232_HDLC_CH+i].arr = &nettx_oam[i][0];
		nettx_st2[FIRST_RS232_HDLC_CH+i].size =OAM_NETBUF_SIZE;

		retxwin_st[FIRST_RS232_HDLC_CH+i].size = RETX_WIN_BUF_SIZE_1;
		retxwin_st[FIRST_RS232_HDLC_CH+i].win_size = RETX_WIN_SIZE_1;

		netrx_st[FIRST_RS232_HDLC_CH+i].arr = &netrx_oam[i][0];
		netrx_st[FIRST_RS232_HDLC_CH+i].size = OAM_RX_NETBUF_SIZE;
	    }
	}
#endif // HDLC_RS232

// ################

#endif // STAND_ALONE_EC1K
#endif // EC1K_SYSTEM

#ifdef PU2K_SYSTEM

// ################

#ifdef RSUSIDE
	local_stn[0] = THIS_RSU_ID;
	remote_stn[0] = SYSTEM_ID;
	hdlc_ch[0].dl_ch=HDLC_0_PEF22554;
	hdlc_ch[0].ic_type=IC_HDLC_QUADE1_PEF22554;
	hdlc_ch[0].cs=PU2K_QUADE1_PEF22554_CS_0;
	hdlc_ch[0].e1_no=0;

	nettx_st[0].arr = &nettx_rsu[0][0];
	nettx_st[0].size = RSU_NETBUF_SIZE;
	nettx_st2[0].arr = &nettx_rsu[0][0];
	nettx_st2[0].size = RSU_NETBUF_SIZE;

	retxwin_st[0].size = RETX_WIN_BUF_SIZE_7;
	retxwin_st[0].win_size = RETX_WIN_SIZE_7;

	netrx_st[0].arr = &netrx_rsu[0][0];
	netrx_st[0].size = RSU_RX_NETBUF_SIZE;
#endif

// ################

#ifdef	STAND_ALONE_PU2K

// ################

#ifdef RSU_CCSV5_INSTALLED
#ifdef HDLC_QUADE1_PEF22554
	for(i=0;i<NO_RSU;i++){
		//for my test only.to program one EPROM for all of the RSUs.
		if(rsu_st[i].ccs_trnk == 0xff){
		    hdlc_ch[i].ic_type=IC_HDLC_NOT_INSTALLED;
		    continue;
		}
		local_stn[i] = SYSTEM_ID;
		remote_stn[i] = RSU_ID;//+i;
		hdlc_ch[i].cs=dtr[rsu_st[i].ccs_trnk].cs;
		hdlc_ch[i].dl_ch=HDLC_0_PEF22554;
		hdlc_ch[i].ic_type=IC_HDLC_QUADE1_PEF22554;
		hdlc_ch[i].e1_no=dtr[rsu_st[i].ccs_trnk].e1_no;

		nettx_st[i].arr = &nettx_rsu[i][0];
		nettx_st[i].size = RSU_NETBUF_SIZE;
		nettx_st2[i].arr = &nettx_rsu[i][0];
		nettx_st2[i].size = RSU_NETBUF_SIZE;
		retxwin_st[i].size = RETX_WIN_BUF_SIZE_7;
		retxwin_st[i].win_size = RETX_WIN_SIZE_7;

		netrx_st[i].arr = &netrx_rsu[i][0];
		netrx_st[i].size = RSU_RX_NETBUF_SIZE;
	}
#endif
#endif

// ################

#ifdef HDLC_RS232
	{
	    int i=0,j;
#ifdef ALARM_PANEL_ON_HDLC_RS232
	    i++;
#ifdef RSUSIDE
	    local_stn[FIRST_RS232_HDLC_CH] = THIS_RSU_ID;
#else
	    local_stn[FIRST_RS232_HDLC_CH] = SYSTEM_ID;
#endif
	    remote_stn[FIRST_RS232_HDLC_CH] = ALARM_ID;
	    hdlc_ch[FIRST_RS232_HDLC_CH].ic_type=IC_HDLC_RS232;
	    hdlc_ch[FIRST_RS232_HDLC_CH].cs=(FIRST_NEW_HDLC_COM_PORT+0);
	    hdlc_ch[FIRST_RS232_HDLC_CH].oam_no=0xff;

	    nettx_st[FIRST_RS232_HDLC_CH].arr = &nettx_alarm[0];
	    nettx_st[FIRST_RS232_HDLC_CH].size = ALARM_NETBUF_SIZE;
	    nettx_st2[FIRST_RS232_HDLC_CH].arr = &nettx_alarm[0];
	    nettx_st2[FIRST_RS232_HDLC_CH].size =ALARM_NETBUF_SIZE;

	    retxwin_st[FIRST_RS232_HDLC_CH].size = RETX_WIN_BUF_SIZE_1;
	    retxwin_st[FIRST_RS232_HDLC_CH].win_size = RETX_WIN_SIZE_1;

	    netrx_st[FIRST_RS232_HDLC_CH].arr = &netrx_alarm[0];
	    netrx_st[FIRST_RS232_HDLC_CH].size = ALARM_RX_NETBUF_SIZE;
#endif
	    for(j=0;j<NO_NEW_HDLC_COM_PORT;j++,i++){
#ifdef RSUSIDE
		local_stn[FIRST_RS232_HDLC_CH+i] = THIS_RSU_ID;
#else
		local_stn[FIRST_RS232_HDLC_CH+i] = SYSTEM_ID;
#endif
		remote_stn[FIRST_RS232_HDLC_CH+i] = OPR0_ID+j;
		hdlc_ch[FIRST_RS232_HDLC_CH+i].ic_type=IC_HDLC_RS232;
		hdlc_ch[FIRST_RS232_HDLC_CH+i].cs=(FIRST_NEW_HDLC_COM_PORT+i);
		hdlc_ch[FIRST_RS232_HDLC_CH+i].oam_no=j;

		nettx_st[FIRST_RS232_HDLC_CH+i].arr = &nettx_oam[j][0];
		nettx_st[FIRST_RS232_HDLC_CH+i].size = OAM_NETBUF_SIZE;
		nettx_st2[FIRST_RS232_HDLC_CH+i].arr = &nettx_oam[j][0];
		nettx_st2[FIRST_RS232_HDLC_CH+i].size =OAM_NETBUF_SIZE;

		retxwin_st[FIRST_RS232_HDLC_CH+i].size = RETX_WIN_BUF_SIZE_1;
		retxwin_st[FIRST_RS232_HDLC_CH+i].win_size = RETX_WIN_SIZE_1;

		netrx_st[FIRST_RS232_HDLC_CH+i].arr = &netrx_oam[j][0];
		netrx_st[FIRST_RS232_HDLC_CH+i].size = OAM_RX_NETBUF_SIZE;
	    }
	}
#endif // HDLC_RS232

// ################

#endif // STAND_ALONE_PU2K
#endif // PU2K_SYSTEM

// --------------------------------------
#ifdef CPU_EC512

// ################

#ifdef RSUSIDE
#ifdef HDLC_8952
	local_stn[0] = THIS_RSU_ID;
	remote_stn[0] = SYSTEM_ID;
	hdlc_ch[0].ic_type=IC_HDLC_8952;
	hdlc_ch[0].cs=0x9000;

	nettx_st[0].arr = &nettx_rsu[0][0];
	nettx_st[0].size = RSU_NETBUF_SIZE;
	nettx_st2[0].arr = &nettx_rsu[0][0];
	nettx_st2[0].size = RSU_NETBUF_SIZE;

	retxwin_st[0].size = RETX_WIN_BUF_SIZE_7;
	retxwin_st[0].win_size = RETX_WIN_SIZE_7;

	netrx_st[0].arr = &netrx_rsu[0][0];
	netrx_st[0].size = RSU_RX_NETBUF_SIZE;
#endif
#endif

// ################

#ifdef RSU_CCSV5_INSTALLED
#ifdef HDLC_8952
	{   int i;
	    for(i=0;i<NO_RSU;i++){
		local_stn[i] = SYSTEM_ID;
		remote_stn[i] = RSU_ID;
		hdlc_ch[i].ic_type=IC_HDLC_8952;
		hdlc_ch[i].cs=(i == 0) ? 0x9000 : ((i == 1) ? 0x9800:0xc000);

		nettx_st[i].arr = &nettx_rsu[i][0];
		nettx_st[i].size = RSU_NETBUF_SIZE;
		nettx_st2[i].arr = &nettx_rsu[i][0];
		nettx_st2[i].size = RSU_NETBUF_SIZE;

		retxwin_st[i].size = RETX_WIN_BUF_SIZE_7;
		retxwin_st[i].win_size = RETX_WIN_SIZE_7;

		netrx_st[i].arr = &netrx_rsu[i][0];
		netrx_st[i].size = RSU_RX_NETBUF_SIZE;
	    }
	}
#endif // HDLC_8952
#endif // RSU_CCSV5_INSTALLED

// ################

#ifdef	STAND_ALONE_EC512
	local_stn[FIRST_RS232_HDLC_CH] = SYSTEM_ID;
	remote_stn[FIRST_RS232_HDLC_CH] = OPR0_ID;
	hdlc_ch[FIRST_RS232_HDLC_CH].ic_type=IC_HDLC_RS232;
	hdlc_ch[FIRST_RS232_HDLC_CH].cs=(FIRST_NEW_HDLC_COM_PORT+0);
	hdlc_ch[FIRST_RS232_HDLC_CH].oam_no=0;

	nettx_st[FIRST_RS232_HDLC_CH].arr = &nettx_oam[0][0];
	nettx_st[FIRST_RS232_HDLC_CH].size = OAM_NETBUF_SIZE;
	nettx_st2[FIRST_RS232_HDLC_CH].arr = &nettx_oam[0][0];
	nettx_st2[FIRST_RS232_HDLC_CH].size =OAM_NETBUF_SIZE;

	retxwin_st[FIRST_RS232_HDLC_CH].size = RETX_WIN_BUF_SIZE_1;
	retxwin_st[FIRST_RS232_HDLC_CH].win_size = RETX_WIN_SIZE_1;

	netrx_st[FIRST_RS232_HDLC_CH].arr = &netrx_oam[0][0];
	netrx_st[FIRST_RS232_HDLC_CH].size = OAM_RX_NETBUF_SIZE;
#endif // STAND_ALONE_EC512

// ################

#endif // CPU_EC512
#endif // SYSTEM_SIDE

// ==================================================


	tst_flg=RESET;
	for (n=0;n<NO_LAPB_CH;n++){
		initialize_hwlayer(n);
		initialize_network_buf(n);
		initialize_linklayer(n);
	}
	init_link_rtns();
	hwrx_lapb_scn_no=NO_LAPB_CH-1;
	hwtx_lapb_scn_no=NO_LAPB_CH-1;
	link_lapb_scn_no=NO_LAPB_CH-1;

#ifdef LINKLAYER_DEBUG_ACT
	if (lapb_dbg_flg == SET){
	    for (n=0;n<NO_LAPB_CH;n++){
		prnt_byte(n);prnt_char(':');
		prnt_byte(local_stn[n]);prnt_char('-');
		prnt_byte(remote_stn[n]);prnt_char('-');
		prnt_byte(hdlc_ch[n].ic_type);prnt_char('-');
		prnt_byte(hdlc_ch[n].dl_ch);prnt_char('-');
		prnt_int((int)hdlc_ch[n].cs);prnt_char('-');
#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
		prnt_byte(hdlc_ch[n].oam_no);prnt_char('-');
#endif
#ifdef HDLC_QUADE1_PEF22554
		prnt_byte(hdlc_ch[n].e1_no);prnt_char('-');
#endif
		prnt_char(CR);
	    }
	}
#endif
}
/* ====================================================================== */
/* ====================================================================== */
void init_lapb_for_reloadrsu(void){

#if ((defined EC2K_SYSTEM) || (defined EC10K_SYSTEM) || (defined EC1K_SYSTEM) || (defined PU2K_SYSTEM) || (defined MGW672_SYSTEM))
#ifdef SYSTEM_SIDE
#ifdef RSU_CCSV5_INSTALLED
	BYTE i;
#endif
#endif
#endif
	BYTE n;

// ==================================================

#ifdef SYSTEM_SIDE
// --------------------------------------
#if ((defined EC2K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PC_SYSTEM))
#ifdef RSU_CCSV5_INSTALLED
#if ((defined HDLC_9075) || (defined HDLC_9076))
	for(i=0;i<NO_RSU;i++){
		//for my test only.to program one EPROM for all of the RSUs.
		if(rsu_st[i].ccs_trnk == 0xff){
		    hdlc_ch[i].ic_type=IC_HDLC_NOT_INSTALLED;
		    continue;
		}
		local_stn[i] = SYSTEM_ID;
		remote_stn[i] = RSU_ID;//+i;
		hdlc_ch[i].cs=dtr[rsu_st[i].ccs_trnk].cs;
#ifdef HDLC_9075
		hdlc_ch[i].dl_ch=HDLC1_9075;
		hdlc_ch[i].ic_type=IC_HDLC_9075;
#endif
#ifdef HDLC_9076
		hdlc_ch[i].dl_ch=HDLC_0_9076;
		hdlc_ch[i].ic_type=IC_HDLC_9076;
#endif

		nettx_st[i].arr = &nettx_rsu[i][0];
		nettx_st[i].size = RSU_NETBUF_SIZE;
		nettx_st2[i].arr = &nettx_rsu[i][0];
		nettx_st2[i].size = RSU_NETBUF_SIZE;

		retxwin_st[i].size = RETX_WIN_BUF_SIZE_7;
		retxwin_st[i].win_size = RETX_WIN_SIZE_7;

		netrx_st[i].arr = &netrx_rsu[i][0];
		netrx_st[i].size = RSU_RX_NETBUF_SIZE;
	}
#endif
#endif // RSU_CCSV5_INSTALLED
#endif // ((defined EC2K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PC_SYSTEM))
// --------------------------------------
#ifdef EC1K_SYSTEM
#ifdef STAND_ALONE_EC1K
#ifdef RSU_CCSV5_INSTALLED
#ifdef HDLC_9076
	for(i=0;i<NO_RSU;i++){
		//for my test only.to program one EPROM for all of the RSUs.
		if(rsu_st[i].ccs_trnk == 0xff){
		    hdlc_ch[i].ic_type=IC_HDLC_NOT_INSTALLED;
		    continue;
		}
		local_stn[i] = SYSTEM_ID;
		remote_stn[i] = RSU_ID;//+i;
		hdlc_ch[i].cs=dtr[rsu_st[i].ccs_trnk].cs;
		hdlc_ch[i].dl_ch=HDLC_0_9076;
		hdlc_ch[i].ic_type=IC_HDLC_9076;

		nettx_st[i].arr = &nettx_rsu[i][0];
		nettx_st[i].size = RSU_NETBUF_SIZE;
		nettx_st2[i].arr = &nettx_rsu[i][0];
		nettx_st2[i].size = RSU_NETBUF_SIZE;
		retxwin_st[i].size = RETX_WIN_BUF_SIZE_7;
		retxwin_st[i].win_size = RETX_WIN_SIZE_7;

		netrx_st[i].arr = &netrx_rsu[i][0];
		netrx_st[i].size = RSU_RX_NETBUF_SIZE;
	}
#endif
#endif
#endif // STAND_ALONE_EC1K
#endif // EC1K_SYSTEM
// --------------------------------------
#ifdef PU2K_SYSTEM
#ifdef STAND_ALONE_PU2K
#ifdef RSU_CCSV5_INSTALLED
#ifdef HDLC_QUADE1_PEF22554
	for(i=0;i<NO_RSU;i++){
		//for my test only.to program one EPROM for all of the RSUs.
		if(rsu_st[i].ccs_trnk == 0xff){
		    hdlc_ch[i].ic_type=IC_HDLC_NOT_INSTALLED;
		    continue;
		}
		local_stn[i] = SYSTEM_ID;
		remote_stn[i] = RSU_ID;//+i;
		hdlc_ch[i].cs=dtr[rsu_st[i].ccs_trnk].cs;
		hdlc_ch[i].dl_ch=HDLC_0_PEF22554;
		hdlc_ch[i].ic_type=IC_HDLC_QUADE1_PEF22554;
		hdlc_ch[i].e1_no=dtr[rsu_st[i].ccs_trnk].e1_no;

		nettx_st[i].arr = &nettx_rsu[i][0];
		nettx_st[i].size = RSU_NETBUF_SIZE;
		nettx_st2[i].arr = &nettx_rsu[i][0];
		nettx_st2[i].size = RSU_NETBUF_SIZE;
		retxwin_st[i].size = RETX_WIN_BUF_SIZE_7;
		retxwin_st[i].win_size = RETX_WIN_SIZE_7;

		netrx_st[i].arr = &netrx_rsu[i][0];
		netrx_st[i].size = RSU_RX_NETBUF_SIZE;
	}
#endif
#endif
#endif // STAND_ALONE_PU2K
#endif // PU2K_SYSTEM
// --------------------------------------
#ifdef CPU_EC512
#ifdef RSU_CCSV5_INSTALLED
#ifdef HDLC_8952
	{   int i;
	    for(i=0;i<NO_RSU;i++){
		local_stn[i] = SYSTEM_ID;
		remote_stn[i] = RSU_ID;
		hdlc_ch[i].ic_type=IC_HDLC_8952;
		hdlc_ch[i].cs=(i == 0) ? 0x9000 : ((i == 1) ? 0x9800:0xc000);

		nettx_st[i].arr = &nettx_rsu[i][0];
		nettx_st[i].size = RSU_NETBUF_SIZE;
		nettx_st2[i].arr = &nettx_rsu[i][0];
		nettx_st2[i].size = RSU_NETBUF_SIZE;

		retxwin_st[i].size = RETX_WIN_BUF_SIZE_7;
		retxwin_st[i].win_size = RETX_WIN_SIZE_7;

		netrx_st[i].arr = &netrx_rsu[i][0];
		netrx_st[i].size = RSU_RX_NETBUF_SIZE;
	    }
	}
#endif // HDLC_8952
#endif // RSU_CCSV5_INSTALLED
#endif // CPU_EC512
// --------------------------------------
#endif // SYSTEM_SIDE

// ==================================================

	for (n=0;n<NO_LAPB_CH;n++){

	  switch ( hdlc_ch[n].ic_type){
#ifdef HDLC_8952
	    case IC_HDLC_8952 :
#endif
#ifdef HDLC_9075
	    case IC_HDLC_9075 :
#endif
#ifdef HDLC_9076
	    case IC_HDLC_9076 :
#endif
#ifdef HDLC_QUADE1_PEF22554
	    case IC_HDLC_QUADE1_PEF22554 :
#endif

		initialize_hwlayer(n);
		initialize_network_buf(n);
		initialize_linklayer(n);
	    default:
		continue;
	  }
	}
}
/* ====================================================================== */
/* ====================================================================== */
char lapb_link_ready_for_check(BYTE n){

	if (state[n] != INFO_TRX){
		return FAIL;
	}
	if (checkfree_buf(&nettx_st[n]) != ((&nettx_st[n])->size -1)){
		return FAIL; /* tx buffer is not empty */
	}
	return OK;
}
/* ====================================================================== */
/* ====================================================================== */
char lapb_link_ready(BYTE n){

	if (state[n] == INFO_TRX)
		return SET;
	else
		return RESET;
}
/* ====================================================================== */
/* ====================================================================== */
//VIOLET start
#if 0
char check_buf_empty(BUF *buf){

	if (checkfree_buf(buf) != (buf->size -1)){
		return FAIL; /* tx buffer is not empty */
	}
	else
		return OK;
}
#endif // 0
//VIOLET end
/* ======================================================================*/
/* ======================================================================*/
#ifdef HDLC_9075
BYTE receive_packet_9075_ver2(BYTE n){

	BYTE indx;
	BYTE status,int_status;
	HDLC_ST *pt;
	BYTE *pckt_pt;
	BYTE cnt;

	pt=&hdlc_ch[n];

	int_status=rd_hdlc_int_st_9075(pt->cs,pt->dl_ch);
	if ((int_status & 0x01) != 0){ /* Rx overflow*/

		prnt_msg(LAPB_ERROR,0x4040);
		prnt_byte(n);prnt_char(CR);
		make_rx_fifo_empty(n);
		return FAIL;
	}
	pckt_pt=&partial_packet[n][0];
	indx=partial_packet_indx[n];

	cnt=0;
	/* while Rx fifo is not empty */
	while (( (status = rd_hdlc_st_9075(pt->cs,pt->dl_ch)) & 0x03) != 0){
#ifdef DEBUG_ACT
		prnt_char('s');prnt_char('t');prnt_char(':');
		prnt_byte(status);prnt_char('-');
		prnt_char(CR);
#endif
		switch (status & 0x30){
		  case 0x10 :	/* first byte */
				if (indx != 0){
					prnt_msg(LAPB_ERROR,0x4002);
					make_rx_fifo_empty(n);
					return FAIL;
				}
				pckt_pt[indx]=rd_hdlc_rx_fifo_9075(pt->cs,pt->dl_ch);indx++;
#ifdef DEBUG_ACT
				prnt_char('r');prnt_char('1');prnt_char(':');
				prnt_byte(indx-1);prnt_char('-');
				prnt_byte(pckt_pt[indx-1]);prnt_char('-');
				prnt_char(CR);
#endif

				break;
		  case 0x00 :	/* packet byte */
				if (indx >= MAX_PACKET_SIZE){
					prnt_msg(LAPB_ERROR,0x4003);
					prnt_byte(n);prnt_char(CR);
					init_hdlc_9075(n);
					return	FAIL;	  /* should read until FIFO empty */
				}
				pckt_pt[indx]=rd_hdlc_rx_fifo_9075(pt->cs,pt->dl_ch);indx++;
#ifdef DEBUG_ACT
				prnt_char('r');prnt_char('2');prnt_char(':');
				prnt_byte(indx-1);prnt_char('-');
				prnt_byte(pckt_pt[indx-1]);prnt_char('-');
				prnt_char(CR);
#endif
				break;
		  case 0x20 :	/* last byte good FCS */
				pckt_pt[indx]=rd_hdlc_rx_fifo_9075(pt->cs,pt->dl_ch);
				indx++;
#ifdef DEBUG_ACT
				prnt_char('r');prnt_char('3');prnt_char(':');
				prnt_byte(indx);prnt_char('-');
				prnt_byte(pckt_pt[indx]);prnt_char('-');
				prnt_char(CR);
#endif
				partial_packet_indx[n]=indx;
				save_packet_hdlc(n);
				indx=0;
				break;
		  case 0x30 :	/* last byte bad FCS */
#ifdef DEBUG_ACT
				prnt_char('B');prnt_char('A');prnt_char('D');prnt_char(' ');
				prnt_char('F');prnt_char('C');prnt_char('S');
				prnt_char(CR);
#endif
				rd_hdlc_rx_fifo_9075(pt->cs,pt->dl_ch);/* to remove it.*/
				indx=0;
				break;
		}
		if (++cnt == MAX_9075_BYTES_RD_PER_TICK){
			break;
		}
	}
	partial_packet_indx[n]=indx;
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef HDLC_9076
BYTE receive_packet_9076_ver2(BYTE n){

	BYTE indx;
	BYTE status,int_status;
	HDLC_ST *pt;
	BYTE *pckt_pt;
	BYTE cnt;

	pt=&hdlc_ch[n];

	int_status=rd_hdlc_status_9076(HDLC_STATUS_HDLC_INTERRUPT_STATUS_ADDR_9076,pt->cs,pt->dl_ch);
	if ((int_status & 0x01) != 0){ /* Rx overflow*/
		prnt_msg(LAPB_ERROR,0x4040);
		prnt_byte(n);prnt_char('-');
		prnt_int(pt->cs);prnt_char('-');
		prnt_int(pt->dl_ch);prnt_char('-');
		prnt_byte(int_status);prnt_char(CR);
		make_rx_fifo_empty(n);
		return FAIL;
	}
	pckt_pt=&partial_packet[n][0];
	indx=partial_packet_indx[n];

	cnt=0;
	/* while Rx fifo is not empty */
	while (( (status = rd_hdlc_status_9076(HDLC_STATUS_HDLC_STATUS_ADDR_9076,pt->cs,pt->dl_ch)) & 0x03) != 0){
#ifdef DEBUG_ACT
		if (tst_flg == SET){
		    prnt_char('s');prnt_char('t');prnt_char('-');prnt_byte(n);prnt_char(':');
		    prnt_byte(status);
		    prnt_char(CR);
		}
#endif
		switch (status & 0x30){
		  case 0x10 :	/* first byte */
				if (indx != 0){
					prnt_msg(LAPB_ERROR,0x4002);
					prnt_byte(n);prnt_char(':');
					{   BYTE j;
					    for(j=0;j<indx;j++){
						prnt_byte(pckt_pt[j]);prnt_char('-');
					    }
					    prnt_char(CR);
					}
					indx=0;
//					  make_rx_fifo_empty(n);
//					  return FAIL;
				}
				pckt_pt[indx]=rd_hdlc_status_9076(HDLC_STATUS_RX_FIFO_ADDR_9076,pt->cs,pt->dl_ch);indx++;
#ifdef DEBUG_ACT
				if (tst_flg == SET){
				    prnt_char('r');prnt_char('1');prnt_char('-');prnt_byte(n);prnt_char(':');
				    prnt_byte(indx-1);prnt_char('-');
				    prnt_byte(pckt_pt[indx-1]);prnt_char('-');
				    prnt_char(CR);
				}
#endif

				break;
		  case 0x00 :	/* packet byte */
#if (MAX_PACKET_SIZE < 256)
				if (indx >= MAX_PACKET_SIZE){
					prnt_msg(LAPB_ERROR,0x4003);
					prnt_byte(n);prnt_char(CR);
					init_hdlc_9076(n);
					return	FAIL;	  /* should read until FIFO empty */
				}
#endif
				pckt_pt[indx]=rd_hdlc_status_9076(HDLC_STATUS_RX_FIFO_ADDR_9076,pt->cs,pt->dl_ch);indx++;
#ifdef DEBUG_ACT
				if (tst_flg == SET){
				    prnt_char('r');prnt_char('2');prnt_char('-');prnt_byte(n);prnt_char(':');
				    prnt_byte(indx-1);prnt_char('-');
				    prnt_byte(pckt_pt[indx-1]);prnt_char('-');
				    prnt_char(CR);
				}
#endif
				break;
		  case 0x20 :	/* last byte good FCS */
				pckt_pt[indx]=rd_hdlc_status_9076(HDLC_STATUS_RX_FIFO_ADDR_9076,pt->cs,pt->dl_ch);
				indx++;
#ifdef DEBUG_ACT
				if (tst_flg == SET){
				    prnt_char('r');prnt_char('3');prnt_char('-');prnt_byte(n);prnt_char(':');
				    prnt_byte(indx);prnt_char('-');
				    prnt_byte(pckt_pt[indx]);prnt_char('-');
				    prnt_char(CR);
				}
#endif
				partial_packet_indx[n]=indx;
				save_packet_hdlc(n);
				indx=0;
				break;
		  case 0x30 :	/* last byte bad FCS */
#ifdef DEBUG_ACT
				if (tst_flg == SET){
				    prnt_char('B');prnt_char('A');prnt_char('D');prnt_char(' ');
				    prnt_char('F');prnt_char('C');prnt_char('S');prnt_char('-');prnt_byte(n);
				    prnt_char(CR);
				}
#endif
				rd_hdlc_status_9076(HDLC_STATUS_RX_FIFO_ADDR_9076,pt->cs,pt->dl_ch);
				indx=0;
				break;
		}
		if (++cnt == MAX_9076_BYTES_RD_PER_TICK){
			break;
		}
	}
	partial_packet_indx[n]=indx;
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef HDLC_8952
BYTE receive_packet_8952_ver2(BYTE n){

	BYTE indx;
	BYTE status,int_status;
	HDLC_ST *pt;
	BYTE *pckt_pt;

	pt=&hdlc_ch[n];

	int_status = rint_flag(pt->cs);
	if ((int_status & 0x01) == 0x01){ /* Rx overflow*/

		prnt_msg(LAPB_ERROR,0x4006);
		make_rx_fifo_empty(n);
		return FAIL;
	}
	pckt_pt=&partial_packet[n][0];
	indx=partial_packet_indx[n];
	/* while Rx fifo is not empty */
	while (( (status = rfifo_stat(pt->cs)) & 0x30) != 0){

#ifdef DEBUG_ACT
		if (tst_flg == SET){
		    prnt_char('s');prnt_char('t');prnt_char(':');
		    prnt_byte(status);prnt_char('-');
		    prnt_char(CR);
		}
#endif
		switch (status & 0xc0){
		  case 0x40 :	/* first byte */
				if (indx != 0){
					prnt_msg(LAPB_ERROR,0x4002);
					indx=0;
//					make_rx_fifo_empty(n);
//					return FAIL;
				}
				pckt_pt[indx]=rrx_data(pt->cs);indx++;
#ifdef DEBUG_ACT
				if (tst_flg == SET){
				    prnt_char('r');prnt_char('1');prnt_char(':');
				    prnt_byte(indx-1);prnt_char('-');
				    prnt_byte(pckt_pt[indx-1]);prnt_char('-');
				    prnt_char(CR);
				}
#endif

				break;
		  case 0x00 :	/* packet byte */
				if (indx >= MAX_PACKET_SIZE){
					prnt_msg(LAPB_ERROR,0x4003);
					init_HDLC_and_8972(n);
					return	FAIL;
				}
				pckt_pt[indx]=rrx_data(pt->cs);indx++;
#ifdef DEBUG_ACT
				if (tst_flg == SET){
				    prnt_char('r');prnt_char('2');prnt_char(':');
				    prnt_byte(indx-1);prnt_char('-');
				    prnt_byte(pckt_pt[indx-1]);prnt_char('-');
				    prnt_char(CR);
				}
#endif
				break;
		  case 0x80 :	/* last byte good FCS */
				pckt_pt[indx]=rrx_data(pt->cs);
				indx++;
#ifdef DEBUG_ACT
				if (tst_flg == SET){
				    prnt_char('r');prnt_char('3');prnt_char(':');
				    prnt_byte(indx);prnt_char('-');
				    prnt_byte(pckt_pt[indx]);prnt_char('-');
				    prnt_char(CR);
				}
#endif
				partial_packet_indx[n]=indx;
				save_packet_hdlc(n);
				indx=0;
				break;
		  case 0xc0 :	/* last byte bad FCS */
#ifdef DEBUG_ACT
				if (tst_flg == SET){
				    prnt_char('B');prnt_char('A');prnt_char('D');prnt_char(' ');
				    prnt_char('F');prnt_char('C');prnt_char('S');
				    prnt_char(CR);
				}
#endif
#pragma warn -eff
				rrx_data(pt->cs);/* to remove it.*/
#pragma warn +eff
				indx=0;
				break;
		}
	}
	partial_packet_indx[n]=indx;
	return OK;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#if ((defined HDLC_9075) || (defined HDLC_8952) || (defined HDLC_9076) || (defined HDLC_QUADE1_PEF22554) )
BYTE save_packet_hdlc(BYTE n){

	BYTE j,indx;
	BYTE *pckt_pt;

#ifdef CS_CORRECTION
	BYTE bit_pos1,cs_byte1;
	BYTE bit_pos2,cs_byte2;
	BYTE chksum,cal_chksum;
#endif

	pckt_pt=&partial_packet[n][0];
	indx=partial_packet_indx[n];
#if ((defined DEBUG_ACT) /*|| (defined PRINT_LAPB_RECEIVED_PACKET)*/)
	if (tst_flg == SET){
	    prnt_char('R'); prnt_char('x');prnt_char('-');prnt_byte(n);prnt_char(':');
	    for(j=0;j<indx;j++){
		prnt_byte(pckt_pt[j]);prnt_char('-');
	    }
	    prnt_char(CR);
	}
#endif

#ifdef CS_CORRECTION

	indx--;cs_byte2=pckt_pt[indx];
	indx--;cs_byte1=pckt_pt[indx];
	indx--;chksum=pckt_pt[indx];
	bit_pos1=0x01;
	bit_pos2=0x01;

#ifdef DEBUG_ACT
if (tst_flg == SET){
	prnt_char('R'); prnt_char('x');;prnt_char(':');
	prnt_byte(n);;prnt_char('-');
	prnt_byte(cs_byte2);prnt_char('-');
	prnt_byte(cs_byte1);prnt_char('-');
	prnt_byte(chksum);prnt_char('-');
	prnt_char(CR);
}
#endif
	if ( checkfree_buf(&hwrx_st[n]) >= (indx+1) ){
		cal_chksum=0;
		for(j=0;j<indx;j++){
			if(j>2){
				if((pckt_pt[j]&0xf0)==0x00){
					if ((cs_byte1&bit_pos1)!=0)
						pckt_pt[j]=pckt_pt[j]|0xf0;
				}
				bit_pos1=bit_pos1<<1;
				if((pckt_pt[j]&0x0f)==0x00){
					if ((cs_byte2&bit_pos2)!=0)
						pckt_pt[j]=pckt_pt[j]|0x0f;
				}
				bit_pos2=bit_pos2<<1;
			}
			cal_chksum=cal_chksum+pckt_pt[j];
		}
		if((chksum&0xf0)==0x00){
			if ((cs_byte1&bit_pos1)!=0){
				chksum=chksum|0xf0;
				cs_byte1=cs_byte1 &(~bit_pos1);
			}
		}
		if((chksum&0x0f)==0x00){
			if ((cs_byte2&bit_pos2)!=0){
				chksum=chksum|0x0f;
				cs_byte2=cs_byte2 &(~bit_pos2);
			}
		}
		cal_chksum = cal_chksum + cs_byte1 + cs_byte2;
		if (chksum != cal_chksum){
			prnt_msg(LAPB_ERROR,0x402c);
			prnt_byte(n);prnt_char(CR);
//???????????????????
//			make_rx_fifo_empty(n);
//#ifdef DEBUG_ACT
			prnt_byte(bit_pos1);prnt_char('-');
			prnt_byte(bit_pos2);prnt_char('-');
			prnt_byte(cs_byte1);prnt_char('-');
			prnt_byte(cs_byte2);prnt_char('-');
			prnt_byte(chksum);prnt_char('-');
			prnt_byte(cal_chksum);prnt_char(CR);
			for(j=0;j<partial_packet_indx[n];j++){
				prnt_byte(pckt_pt[j]);prnt_char('-');
			}
			prnt_char(CR);
//#endif
			return FAIL;	 /* should read until FIFO empty */
		}
		else{
#ifdef DEBUG_ACT
			if (tst_flg == SET){
			    prnt_char('S'); prnt_char('I'); prnt_char('Z');prnt_char('E');
			    prnt_char(':'); prnt_byte(indx); prnt_char(CR);
			}
#endif
			put_buf(&hwrx_st[n],indx); /* packet size */
			for(j=0;j<indx;j++){
				put_buf(&hwrx_st[n],pckt_pt[j]);
			}
#if ((defined DEBUG_ACT) || (defined PRINT_LAPB_RECEIVED_PACKET))
	prnt_char('R'); prnt_char('x');prnt_char('-');prnt_byte(n);prnt_char(':');
	for(j=0;j<indx;j++){
		prnt_byte(pckt_pt[j]);prnt_char('-');
	}
	prnt_char(CR);
#endif
			return OK;
		}
	}
	else{
		prnt_msg(LAPB_ERROR,0x4005);
		return FAIL;
	}
#else	/* #ifdef CS_CORRECTION */
	if ( checkfree_buf(&hwrx_st[n]) >= (indx+1) ){
		put_buf(&hwrx_st[n],indx); /* packet size */
		for(j=0;j<indx;j++){
			put_buf(&hwrx_st[n],pckt_pt[j]);
		}
		return OK;
	}
	else{
		prnt_msg(LAPB_ERROR,0x400f);
		return FAIL;
	}
#endif
}
#endif // HDLC_9075
#endif // LAPB
#endif // #if(((OPERATOR_SIDE) && (LAPB_ON_OPERATOR_SIDE)) || (SYSTEM_SIDE))
/* ====================================================================== */
/* ====================================================================== */


#ifndef CENTRAL_EXTERN

#if ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
extern FILE *opr_gonz_fp;
#endif

#if ((defined LINKLAYER_DEBUG_ACT) && (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE))
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
#include <stdio.h>
extern FILE *lapb_fer;
#endif
#endif

#endif

#if ((defined FIFO_RD_WR) || (defined KIATEL_DYNAMIC_MEM))
void prnt_msg(BYTE msg_type,unsigned int err_code){

#ifdef SYSTEM_SIDE
	mes2prnti(mess[msg_type],err_code);
#endif

#if ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(opr_gonz_fp,"%s,%04x\r\n",mess[msg_type],err_code);

#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#ifdef TCP_DEBUG_ACT
	fprintf(tcp_fer,"%s:%04x\n",mess[msg_type],err_code);
	fflush(tcp_fer);
#endif
#endif
#endif

#if ((defined LINKLAYER_DEBUG_ACT) && (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE))
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(lapb_fer,"%s,%04x",mess[msg_type],err_code);
#endif
#endif
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#if ((((defined FIFO_RD_WR) || (defined KIATEL_DYNAMIC_MEM)) && (defined SYSTEM_SIDE)) || (defined OPERATOR_SIDE))
void prnt_byte(BYTE msg){

#ifdef SYSTEM_SIDE
	byte2prnt(msg);
#endif

#if ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(opr_gonz_fp,"%02x",msg);
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#ifdef TCP_DEBUG_ACT
	fprintf(tcp_fer,"%02x",msg);
	fflush(tcp_fer);
#endif
#endif
#endif

#if ((defined LINKLAYER_DEBUG_ACT) && (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE))
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(lapb_fer,"%02x",msg);
#endif
#endif
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#if ((((defined FIFO_RD_WR) || (defined KIATEL_DYNAMIC_MEM)) && (defined SYSTEM_SIDE)) || (defined OPERATOR_SIDE))
void prnt_int(int msg){

#ifdef SYSTEM_SIDE
	int2prnt(msg);
#endif

#if ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(opr_gonz_fp,"%04x",msg&0xffff);
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#ifdef TCP_DEBUG_ACT
	fprintf(tcp_fer,"%04x",msg&0xffff);
	fflush(tcp_fer);
#endif
#endif
#endif

#if ((defined LINKLAYER_DEBUG_ACT) && (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE))
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(lapb_fer,"%04x",msg&0xffff);
#endif
#endif
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#if ((((defined FIFO_RD_WR) || (defined KIATEL_DYNAMIC_MEM)) && (defined SYSTEM_SIDE)) || (defined OPERATOR_SIDE))
void prnt_char(BYTE msg){

#ifdef SYSTEM_SIDE
	toprnq(msg);
#endif

#if ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(opr_gonz_fp,"%c",msg);
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#ifdef TCP_DEBUG_ACT
	if ((msg == CR) || (msg == LF))
		fprintf(tcp_fer,"\n");
	else
		fprintf(tcp_fer,"%c",msg);
		fflush(tcp_fer);
#endif
#endif
#endif

#if ((defined LINKLAYER_DEBUG_ACT) && (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE))
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	if ((msg == CR) || (msg == LF))
		fprintf(lapb_fer,"\n");
	else
		fprintf(lapb_fer,"%c",msg);
#endif
#endif
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#if ((((defined FIFO_RD_WR) || (defined KIATEL_DYNAMIC_MEM)) && (defined SYSTEM_SIDE)) || (defined OPERATOR_SIDE))
void prnt_string(char *msg){
#ifdef SYSTEM_SIDE
	ws2p(msg);
#endif

#if ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(opr_gonz_fp,"%s",msg);
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#ifdef TCP_DEBUG_ACT
	fprintf(tcp_fer,"%s",msg);
	fflush(tcp_fer);
#endif
#endif
#endif

#if ((defined LINKLAYER_DEBUG_ACT) && (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE))
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(lapb_fer,"%s",msg);
#endif
#endif

}
#endif

/* ====================================================================== */
/* ====================================================================== */
#if ((((defined FIFO_RD_WR) || (defined KIATEL_DYNAMIC_MEM)) && (defined SYSTEM_SIDE)) || (defined OPERATOR_SIDE))
void prnt_long(long msg){

#ifdef SYSTEM_SIDE
	long2prnt(msg);
#endif

#if ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(opr_gonz_fp,"%08lx",msg);
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#ifdef TCP_DEBUG_ACT
	fprintf(tcp_fer,"%08lx",msg);
	fflush(tcp_fer);
#endif
#endif
#endif

#if ((defined LINKLAYER_DEBUG_ACT) && (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE))
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fprintf(lapb_fer,"%08lx",msg);
#endif
#endif
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef LINKLAYER_DEBUG_ACT
void close_lapb_err_file(void){
#if (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE)
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	fclose(lapb_fer);
#endif
#endif
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef LINKLAYER_DEBUG_ACT
void open_lapb_err_file(void){
#if (!defined DO_NOT_WRITE_TO_LAPB_DEBUG_FILE)
#if ((defined SYSTEM_SIDE) && (defined PC_COMPATIBLE)) || \
    ((defined OPERATOR_SIDE) && (defined OPERATOR_PC))
	lapb_fer=fopen("lapb.dbg","w+t");
#endif
#endif
	lapb_dbg_flg=DEFAULT_LINKLAYER_DEBUG_VALUE;
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
#ifdef OPERATOR_SIDE
extern unsigned int check_link_count[];
extern BYTE sync_led[];
extern BYTE link_up_led[];
extern BYTE hot_link;
//JASHN start
#ifdef DYNAMIC_IP_ALLOCATION
extern unsigned int ms_button[];
#endif
//JASHN end
#endif
#endif
/* ====================================================================== */
/* ====================================================================== */
#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
void check_link_handler(void){

	BYTE n;
	/*
	   If we have RS232 link,there is no sync check so LAPB layer is
	   always active.It is not really important,only when the state
	   is INFO_TRX	but there is no data for transmission,
	   if the link is disconnected two sides will not be aware.
	   So a check packet is sent .if the packet could not be sent,
	   state of link is changed and link flag is reset.
	   I have to check state of LAPB layer to prevent sending
	   MAC_HDLC_RS232_LINK_STATUS_REQ , when the link is disconnected.
	   if not,buffer overflow will happen,and when the link is
	   re-estabilshed a lot of MAC_HDLC_RS232_LINK_STATUS_REQ is sent.
	*/

#ifdef HDLC_RS232
	for(n=FIRST_RS232_HDLC_CH;n<LAST_RS232_HDLC_CH;n++){
#endif
#ifdef MY_UDP_FOR_OAM
	for(n=FIRST_UDP_HDLC_CH;n<LAST_UDP_HDLC_CH;n++){
#endif

		if (check_link_count[n] == 0){
//JASHN start
#ifdef DYNAMIC_IP_ALLOCATION
#ifdef OPERATOR_SIDE
			if (lapb_link_flag[n] == SET){
				led_set(sync_led[ms_button[n]]);
				led_set(link_up_led[ms_button[n]]);
			}
			else{
				led_reset(sync_led[ms_button[n]]);
				led_reset(link_up_led[ms_button[n]]);
			}
#endif
#else
#ifdef OPERATOR_SIDE
			if (lapb_link_flag[n] == SET){
				led_set(sync_led[n]);
				led_set(link_up_led[n]);
			}
			else{
#ifdef CPU_EC512
//				initialize();
//				init_layo();
//				led_set(OPR_RESET_LED);
#endif
				led_reset(sync_led[n]);
				led_reset(link_up_led[n]);
			}
#endif
#endif
//JASHN end
			if (lapb_link_ready_for_check(n) == OK){
#ifdef OPERATOR_SIDE
			    // TON is not used here.
			    // if someday, we wanted to add some code in
			    // maf_s_check_link(), we shall use this_pckt_oam_no
			    pmamftx_to_direct_link(MA_SYS_P | MAC_HDLC_RS232_CHECK_LINK,/*TON*/0,OPR_T,n);
#endif
#ifdef SYSTEM_SIDE
#ifdef HDLC_RS232
			    pmamftx(MA_SYS_P | MAC_HDLC_RS232_CHECK_LINK, 0,n-FIRST_RS232_OAM_HDLC_CH);
#endif
#ifdef MY_UDP_FOR_OAM
			    pmamftx(MA_SYS_P | MAC_HDLC_RS232_CHECK_LINK, 0,n-FIRST_UDP_HDLC_CH);
#endif
#endif
			}
			check_link_count[n]=MAX_CHECK_LINK_COUNT;
		}
		else{
			check_link_count[n]--;
		}

	}
}
#endif

/* ======================================================================= */
/* ======================================================================= */
#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
void init_link_handler(void){
	BYTE n;
#ifdef HDLC_RS232
	for(n=FIRST_RS232_HDLC_CH;n<LAST_RS232_HDLC_CH;n++){
#endif
#ifdef MY_UDP_FOR_OAM
	for(n=FIRST_UDP_HDLC_CH;n<LAST_UDP_HDLC_CH;n++){
#endif
		check_link_count[n]=0;
	}
}
#endif
/* ======================================================================= */
/* ======================================================================= */
