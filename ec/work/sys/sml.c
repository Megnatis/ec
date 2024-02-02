

//#include "dsmtfp.h"
//#define	  DEBUG_ACT

//WAR start
/*
#ifdef SYSTEM_SIDE
#include "sys_net.h"
#endif
*/
//WAR end

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else  // CENTRAL_EXTERN
#include "hdr.h"
//DOOD
#if (OS == DOS)
#include <dos.h>
//DOOD
#endif

#ifdef HDLC_RS232
extern SRL_PORT_ST srl_port[];
extern BYTE hdlc_link_state[];			 // link state
extern BYTE hdlc_received_bcc[];		 // received BCC
extern BYTE rcv_st[];
extern BYTE rcv_size[]; 	       // pointer to received data array
extern BYTE rcv_dat[NO_NEW_HDLC_COM_PORT][MAX_HDLC_DATA_SIZE]; // where received data is put
extern BYTE bkup_rcv_st[];
extern BYTE bkup_rcv_size[];		    // pointer to received data array
extern BYTE bkup_rcv_dat[NO_NEW_HDLC_COM_PORT][MAX_HDLC_DATA_SIZE]; // where received data is put
extern HDLC_FUNC_PTR hdlc_rtn[]; // state/stim array
extern BUF com_hwrx_st[];
extern BUF com_hwtx_st[];
extern BYTE hdlc_dbg_flg[];
extern HDLC_ST hdlc_ch[];
extern char *mess[];
extern BUF hwrx_st[];
extern BUF hwtx_st[];
extern BYTE sync_flag[];
#ifdef SML_DEBUG
extern FILE *sml_fp;
#endif
#endif

#endif // CENTRAL_EXTERN
/***************************************************************************/
/* Link Handler Routines						   */
/***************************************************************************/

#ifdef HDLC_RS232
BYTE init_hdlc_rs232(BYTE n){

	unsigned int indx;

//LATCHUP : typecast added
	indx=(unsigned int)hdlc_ch[n].cs;

	hdlc_link_state[indx]=HDLC_LINK_IDLE;

	bkup_rcv_st[indx] = RESET;
	rcv_st[indx] = RESET;

	rcv_size[indx]=0;
	bkup_rcv_size[indx]=0;
	return OK;
}
#endif
/* ======================================================================= */
#ifdef HDLC_RS232
void init_hdlc_ports(void){

	BYTE indx,n;

#ifndef SERIAL_INTERFACE_ACTIVE 	// CPU_EC512
	init_com_regs();
#endif

	for (n=FIRST_NEW_HDLC_COM_PORT,indx=n-FIRST_NEW_HDLC_COM_PORT;n<LAST_NEW_HDLC_COM_PORT;n++,indx++){

#ifndef SERIAL_INTERFACE_ACTIVE 	// CPU_EC512
		init_com_port(n);     // initialize layer 1
#endif

		init_com_bufs(indx);
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
		init_com_trx_int(n);
#endif
	}
	// initializing layer 2

	hdlc_rtn[ HDLC_LINK_IDLE  ] = hdlc_rtn12;
	hdlc_rtn[ HDLC_WTFR_DATA  ] = hdlc_rtn42;
	hdlc_rtn[ HDLC_DLE_IN_DATA] = hdlc_rtn52;
	hdlc_rtn[ HDLC_WTFR_BCC   ] = hdlc_rtn62;
#ifdef SML_DEBUG
	sml_fp=fopen("sml.dbg","a+t");
#endif
}
#endif
/* ======================================================================= */
#ifdef HDLC_RS232
void hdlc_rx_hndlr(void){

	BYTE rec_data;
	BYTE n,indx;
	BYTE cnt;
	BUF *pt;
	BYTE state;
//FLOPPY_BUG : this routine is optimized for speed.

	for (n=FIRST_NEW_HDLC_COM_PORT,indx=n-FIRST_NEW_HDLC_COM_PORT;n<LAST_NEW_HDLC_COM_PORT;n++,indx++){

		cnt=MAX_RS232_RX_BYTE_COUNT;
		pt=&com_hwrx_st[indx];

		while( get_buf(pt,&rec_data) == OK){
			state=hdlc_link_state[indx];
			if (state >= NO_HDLC_STATES){
				prnt_msg(SERIAL_ERROR,0x1668);
				continue;
			}
			hdlc_rtn[state](indx,rec_data);
//GRPPRG_TEST start
#ifdef SML_DEBUG
			fprintf(sml_fp,"r:%x ",rec_data);
#endif
//GRPPRG_TEST end
			if(--cnt == 0) break;
		}
//GRPPRG_TEST start
#ifdef SML_DEBUG
		if (cnt != MAX_RS232_RX_BYTE_COUNT)
			fprintf(sml_fp,"\n");
#endif
//GRPPRG_TEST end
	}
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef HDLC_RS232
BYTE transmit_packet_rs232(BYTE n){

	BYTE dat_size,i,dat;
	BYTE block_check_character=0;
	BYTE indx;
	BUF *pt,*tx_pt;

	indx=(unsigned int)hdlc_ch[n].cs-FIRST_NEW_HDLC_COM_PORT;
	tx_pt=&hwtx_st[n];
	if (get_buf(tx_pt,&dat_size) != OK)
		return OK;
again:
#ifdef DEBUG_ACT
	prnt_char(CR);
	prnt_byte(dat_size);prnt_char('-');
#endif
#ifdef SML_DEBUG
//	fprintf(sml_fp,"tx_size:%x ",dat_size);
#endif
	pt=&com_hwtx_st[indx];
	if (put_buf(pt,STX_BYTE) != OK){
		prnt_msg(BUF_OVERFLOW,0x5080);
		return FAIL;
	}
	for(i=0;i<dat_size;i++){
		if (get_buf(tx_pt,&dat) != OK)
			return FAIL;
#ifdef DEBUG_ACT
		prnt_byte(dat);prnt_char('-');
#endif
#ifdef SML_DEBUG
//		fprintf(sml_fp,"tx_dat:%x ",dat);
#endif
		if( put_buf(pt,dat) != OK){
			prnt_msg(BUF_OVERFLOW,0x5081);
			return FAIL;
		}
		block_check_character ^= dat;	  // calculate all data bytes in BCC

		if(dat == DLE_BYTE)		 /* DLE insertion */
			if( put_buf(pt,DLE_BYTE) != OK){  // do not calculate extra DLE in BCC
				prnt_msg(BUF_OVERFLOW,0x5082);
				return FAIL;
			}
	}
#ifdef SML_DEBUG
	fprintf(sml_fp,"\n");
#endif
						// do not calculate any control character in BCC
	if( put_buf(pt,DLE_BYTE) != OK){	   /* DLE */
		prnt_msg(BUF_OVERFLOW,0x5083);
		return FAIL;
	}
	if( put_buf(pt,ETX_BYTE) != OK){	   /* ETX */
		prnt_msg(BUF_OVERFLOW,0x5084);
		return FAIL;
	}
							  /* BCC */
	if( put_buf(pt,block_check_character) != OK){
		prnt_msg(BUF_OVERFLOW,0x5085);
		return FAIL;
	}
/*
	if (get_buf(tx_pt,&dat_size) == OK)
		goto again;
*/
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
	enable_com_trx_int((unsigned int)hdlc_ch[n].cs);
#endif
	return OK;
}
#endif

/* ======================================================================= */
/* hdlc_rtn[ LINK_IDLE ][ DATA_REC ] = hdlc_rtn12			     */
/* ======================================================================= */
#ifdef HDLC_RS232
void hdlc_rtn12(BYTE n,BYTE rec_data){
#ifdef DEBUG_ACT
	if (hdlc_dbg_flg[n] == SET)
		prnt_msg(SERIAL_RTN,0x112);
#endif

	if(rec_data == STX_BYTE){
//added on 78/9/4
	      rcv_st[n]=RESET;
	      hdlc_received_bcc[n]=0;
	      rcv_size[n]=0;
	      hdlc_link_state[n]=HDLC_WTFR_DATA;
	}
	else{
		//ignore it!
	}
}
#endif

/* ======================================================================= */
/* hdlc_rtn[ WTFR_DATA ][ DATA_REC ] = hdlc_rtn42			     */
/* ======================================================================= */
#ifdef HDLC_RS232
void hdlc_rtn42(BYTE n,BYTE rec_data){

#ifdef DEBUG_ACT
	if (hdlc_dbg_flg[n] == SET)
		prnt_msg(SERIAL_RTN,0x142);
#endif

	if(rec_data == DLE_BYTE){
		hdlc_link_state[n] = HDLC_DLE_IN_DATA;
	}
	else{
		if(rcv_size[n] < MAX_HDLC_DATA_SIZE){
			rcv_dat[n][rcv_size[n]]=rec_data;
			rcv_size[n]++;
			hdlc_received_bcc[n] ^= rec_data;
		}
		else{
			rcv_st[n] |= RCV_OVERFLOW;
		}
	}
}
#endif

/* ======================================================================= */
/* hdlc_rtn[ DLE_IN_DATA ][ DATA_REC ] = hdlc_rtn52			     */
/* ======================================================================= */
#ifdef HDLC_RS232
void hdlc_rtn52(BYTE n,BYTE rec_data){

#ifdef DEBUG_ACT
	if (hdlc_dbg_flg[n] == SET)
		prnt_msg(SERIAL_RTN,0x152);
#endif

	if(rec_data == DLE_BYTE){
		if(rcv_size[n] < MAX_HDLC_DATA_SIZE){
			rcv_dat[n][rcv_size[n]]=rec_data;
			rcv_size[n]++;
			hdlc_received_bcc[n] ^= rec_data;
		}
		else{
			rcv_st[n] |= RCV_OVERFLOW;
		}
		hdlc_link_state[n] = HDLC_WTFR_DATA;
	}
	else{
		if(rec_data == ETX_BYTE){
			hdlc_link_state[n] = HDLC_WTFR_BCC;
		}
		else{
//???????????
		}
	}
}
#endif
/* ======================================================================= */
/* hdlc_rtn[ WTFR_BCC][ DATA_REC ] = hdlc_rtn62 			     */
/* ======================================================================= */
#ifdef HDLC_RS232
void hdlc_rtn62(BYTE n,BYTE rec_data){

	BYTE i;

#ifdef DEBUG_ACT
	if (hdlc_dbg_flg[n] == SET)
		prnt_msg(SERIAL_RTN,0x162);
#endif

	if((hdlc_received_bcc[n] ^ rec_data) != 0){
		rcv_st[n] |= BAD_BCC;
#ifdef DEBUG_ACT
		prnt_char('B');prnt_char('D');prnt_char(':');
		prnt_byte(hdlc_received_bcc[n]);prnt_char(' ');
		prnt_byte(rec_data);prnt_char(' ');
		prnt_char('s');prnt_char('z');prnt_char(':');
		prnt_byte(rcv_size[n]);prnt_char(' ');
		for(i=0;i<rcv_size[n];i++){
			prnt_byte(rcv_dat[n][i]);prnt_char('-');
		}
		prnt_char(CR);
#endif
	}
	else{
#ifdef DEBUG_ACT
		prnt_char('B');prnt_char('C');prnt_char('C');prnt_char(':');
		prnt_byte(hdlc_received_bcc[n]);prnt_char(' ');
		prnt_byte(rec_data);prnt_char(' ');
#endif
		bkup_rcv_size[n]=rcv_size[n];
#ifdef DEBUG_ACT
		prnt_char('s');prnt_char('z');prnt_char(':');
		prnt_byte(rcv_size[n]);prnt_char(' ');
#endif

		for(i=0;i<rcv_size[n];i++){
			bkup_rcv_dat[n][i]=rcv_dat[n][i];
#ifdef DEBUG_ACT
			prnt_byte(rcv_dat[n][i]);prnt_char('-');
#endif
		}
#ifdef DEBUG_ACT
//		prnt_char(CR);
#endif
#ifdef DEBUG_ACT
		prnt_char('r');prnt_char('x');prnt_char(' ');
		prnt_char('O');prnt_char('K');prnt_char(CR);
#endif
	}
	rcv_st[n] |= PACKET_READY;
	bkup_rcv_st[n] = rcv_st[n];
	hdlc_link_state[n]=HDLC_LINK_IDLE;
}
#endif

/* ======================================================================= */
/* ======================================================================= */
#ifdef HDLC_RS232
void hdlc_error(BYTE error_code){

	prnt_msg(SERIAL_ERROR,0xee);
	prnt_byte(error_code);
	prnt_char(CR);

}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef HDLC_RS232
BYTE receive_packet_rs232(BYTE n){

//FLOPPY_BUG
//this routine is optimized for speed.

	unsigned int indx;
	BYTE i;
	BYTE *st,*size,*dat;
	BUF *arr;

//LATCHUP : typecast added
	indx=(unsigned int)hdlc_ch[n].cs-FIRST_NEW_HDLC_COM_PORT;
	st=&bkup_rcv_st[indx];

	if ( (*st & RCV_OVERFLOW ) != RESET ){
		prnt_msg(LAPB_ERROR,0x9100);
		make_rs232_rx_fifo_empty(n);
		return FAIL;
	}
	if ( (*st & PACKET_READY ) == RESET ){
		return OK;
	}
	*st &= ~PACKET_READY;

	if ( (*st & BAD_BCC ) == RESET ){
#ifdef DEBUG_ACT
		prnt_char('n');prnt_char(':');
		prnt_byte(n);prnt_char(' ');
		prnt_char('d');prnt_char('x');prnt_char(':');
		prnt_byte(indx);prnt_char(' ');
		prnt_char('c');prnt_char('s');prnt_char(':');
//LATCHUP : typecast added
		prnt_byte((BYTE)hdlc_ch[n].cs);prnt_char(' ');
		prnt_char('s');prnt_char('z');prnt_char(':');
		prnt_byte(bkup_rcv_size[indx]);prnt_char(' ');
		prnt_char('s');prnt_char('t');prnt_char(':');
		prnt_byte(bkup_rcv_st[indx]);prnt_char(' ');
		prnt_char('d');prnt_char('t');prnt_char(':');
		for(i=0;i<bkup_rcv_size[indx];i++){
			 prnt_byte(bkup_rcv_dat[indx][i]);prnt_char(' ');
		}
		prnt_char(CR);
#endif

		size=&bkup_rcv_size[indx];
		arr=&hwrx_st[n];
		dat=&bkup_rcv_dat[indx][0];
		if ( checkfree_buf(arr) >= (*size+1) ){
			put_buf(arr,*size); /* packet size */
			for(i=0;i<*size;i++,dat++){
				put_buf(arr,*dat);
#ifdef DEBUG_ACT
				prnt_char('s');prnt_char('z');prnt_char(':');
				prnt_byte(bkup_rcv_size[indx]);prnt_char(' ');
#endif

			}
			*st = RESET;
			return OK;
		}
		else{
			prnt_msg(BUF_OVERFLOW,0x5086);
			*st = RESET;
			return FAIL;
		}
	}
	else{
		prnt_msg(LAPB_ERROR,0x9102);
		prnt_byte(n);prnt_char(CR);
#ifdef DEBUG_ACT
		prnt_char('n');prnt_char(':');
		prnt_byte(n);prnt_char(' ');
		prnt_char('d');prnt_char('x');prnt_char(':');
		prnt_byte(indx);prnt_char(' ');
		prnt_char('c');prnt_char('s');prnt_char(':');
//LATCHUP : typecast added
		prnt_byte((BYTE)hdlc_ch[n].cs);prnt_char(' ');
		prnt_char('s');prnt_char('z');prnt_char(':');
		prnt_byte(bkup_rcv_size[indx]);prnt_char(' ');
		prnt_char('s');prnt_char('t');prnt_char(':');
		prnt_byte(bkup_rcv_st[indx]);prnt_char(' ');
		prnt_char('d');prnt_char('t');prnt_char(':');
		for(i=0;i<bkup_rcv_size[indx];i++){
			 prnt_byte(bkup_rcv_dat[indx][i]);prnt_char(' ');
		}
		prnt_char(CR);
#endif
		*st = RESET;
		return FAIL;
	}
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef HDLC_RS232
void make_rs232_rx_fifo_empty(BYTE n){

	unsigned int indx;

//LATCHUP : typecast added
	indx=(unsigned int)hdlc_ch[n].cs-FIRST_NEW_HDLC_COM_PORT;
	bkup_rcv_st[indx] = RESET;
	bkup_rcv_size[indx]=0;
//added on 78/9/4
	hdlc_link_state[n]=HDLC_LINK_IDLE;
	rcv_st[indx] = RESET;
	rcv_size[indx]=0;
//added end
}
#endif
/* ======================================================================= */
/* ======================================================================= */
