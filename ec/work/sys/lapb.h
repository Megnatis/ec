
//.....................................................
#ifdef RSU_CCSV5_INSTALLED
#ifndef CCSV5_TOWARDS_RSU_TRUNK_INDEX_START
#define CCSV5_TOWARDS_RSU_TRUNK_INDEX_START	CCSV5_TRUNK_INDEX_START
#endif
#ifndef NO_CCSV5_TOWARDS_RSU_GROUP
#define NO_CCSV5_TOWARDS_RSU_GROUP	NO_RSU
#endif
#ifndef NO_CCSV5_TOWARDS_RSU_DIGITAL_TRUNK
#define NO_CCSV5_TOWARDS_RSU_DIGITAL_TRUNK     NO_CCSV5_DIGITAL_TRUNK
#endif
#define NO_CCSV5_TOWARDS_RSU_CHANNEL (NO_CCSV5_TOWARDS_RSU_DIGITAL_TRUNK * 30)
#ifndef TOTAL_NO_VIRTUAL_RSU
#define TOTAL_NO_VIRTUAL_RSU  0
#endif
#define TOTAL_NO_RSU	(NO_RSU+TOTAL_NO_VIRTUAL_RSU)
#else
#define NO_CCSV5_TOWARDS_RSU_GROUP	0
#define NO_CCSV5_TOWARDS_RSU_DIGITAL_TRUNK     0
#endif

#ifdef RSUSIDE
#ifndef CCSV5_TOWARDS_HOST_TRUNK_INDEX_START
#define CCSV5_TOWARDS_HOST_TRUNK_INDEX_START	CCSV5_TRUNK_INDEX_START
#endif
#ifndef NO_CCSV5_TOWARDS_HOST_GROUP
#define NO_CCSV5_TOWARDS_HOST_GROUP	1
#endif
#ifndef NO_CCSV5_TOWARDS_HOST_DIGITAL_TRUNK
#define NO_CCSV5_TOWARDS_HOST_DIGITAL_TRUNK	NO_CCSV5_DIGITAL_TRUNK
#endif
#ifndef CCSV5_DEFAULT_GROUP
#define CCSV5_DEFAULT_GROUP	NO_CCSV5_TOWARDS_RSU_GROUP
#endif
#define CCSV5_TOWARDS_HOST_GROUP_START	NO_CCSV5_TOWARDS_RSU_GROUP
#define CCSV5_TOWARDS_HOST_CHANNEL_INDEX_START	(NO_CCSV5_TOWARDS_RSU_DIGITAL_TRUNK * 30)  // NO_CCSV5_TOWARDS_RSU_CHANNEL
#define NO_CCSV5_TOWARDS_HOST_CHANNEL (NO_CCSV5_TOWARDS_HOST_DIGITAL_TRUNK * 30)
#else
#define NO_CCSV5_TOWARDS_HOST_GROUP	0
#define NO_CCSV5_TOWARDS_HOST_DIGITAL_TRUNK	0
#endif

#define TOTAL_NO_CCSV5_GROUP (NO_CCSV5_TOWARDS_HOST_GROUP+NO_CCSV5_TOWARDS_RSU_GROUP)
#define TOTAL_NO_CCSV5_DIGITAL_TRUNK (NO_CCSV5_TOWARDS_HOST_DIGITAL_TRUNK+NO_CCSV5_TOWARDS_RSU_DIGITAL_TRUNK)
#undef NO_CCSV5_CHANNEL
#define NO_CCSV5_CHANNEL  (TOTAL_NO_CCSV5_DIGITAL_TRUNK * 30)
//.....................................................

#ifdef LAPB
#ifdef HDLC_RS232
#ifdef ALARM_PANEL_ON_HDLC_RS232
#define 	FIRST_RS232_OAM_HDLC_CH 	(FIRST_RS232_HDLC_CH+1) // first ch: Alarm Panel
#define 	FIRST_RS232_OPR_HDLC_CH 	(FIRST_RS232_HDLC_CH+1) // first ch: Alarm Panel
#else
#define 	FIRST_RS232_OAM_HDLC_CH 	FIRST_RS232_HDLC_CH
#define 	FIRST_RS232_OPR_HDLC_CH 	FIRST_RS232_HDLC_CH
#endif
#endif

//CHEEN : seems unnecessary : FIRST_HDLC_RS232_CH is not correct(FIRST_RS232_HDLC_CH)
//#define	NO_HDLC_RS232_CH		(LAST_HDLC_RS232_CH - FIRST_HDLC_RS232_CH )

#ifdef HDLC_8952
#define 		IC_HDLC_8952	0
#endif

#ifdef HDLC_9075
#define 	IC_HDLC_9075			1
#endif

#ifdef HDLC_RS232
#define 	IC_HDLC_RS232			2
#endif
#ifdef HDLC_9076
#define 	IC_HDLC_9076			3
#endif
#ifdef HDLC_QUADE1_PEF22554
#define 	IC_HDLC_QUADE1_PEF22554 	 4
#endif
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
#define 	IC_HDLC_ON_UDP			5
#endif

#define IC_HDLC_NOT_INSTALLED			7  // ic_type is only 3 bits!

#ifdef SYSTEM_SIDE
#define NO_LAPB_CH (TOTAL_NO_CCSV5_GROUP+NO_NEW_HDLC_COM_PORT+NO_HDLC_UDP_PORT+NO_HDLC_HTBY_UDP_PORT)
#endif

#ifdef OPERATOR_SIDE
#ifdef HDLC_RS232
#define NO_LAPB_CH	 (NO_NEW_HDLC_COM_PORT)
#endif
#ifdef MY_UDP_FOR_OAM
#define NO_LAPB_CH	 (NO_HDLC_UDP_PORT)
#endif
#endif

#ifdef SYSTEM_SIDE
#ifdef RSU_CCSV5_INSTALLED
#define  MAX_LINK_LAPB_SCAN_PER_LINK	10
#endif
#ifdef RSUSIDE
#ifdef CPU_EC512
#ifdef V52_PSTN_ACCESS_ON_RSUSIDE
#define  MAX_LINK_LAPB_SCAN_PER_LINK	2
#else
#define  MAX_LINK_LAPB_SCAN_PER_LINK	1
#endif
#else
#define  MAX_LINK_LAPB_SCAN_PER_LINK	10
#endif
#endif
#if ((!defined RSU_CCSV5_INSTALLED) && (!defined RSUSIDE))
#define  MAX_LINK_LAPB_SCAN_PER_LINK	2
#endif
#endif // SYSTEM_SIDE
#ifdef OPERATOR_SIDE
#ifdef MY_UDP_FOR_OAM
#define  MAX_LINK_LAPB_SCAN_PER_LINK	10
#else
#define  MAX_LINK_LAPB_SCAN_PER_LINK	2
#endif
#endif // OPERATOR_SIDE

// if NO_LAPB_CH is pretty big, this is a run time headache, however
// what else can we do? otherwise, we may lose data, handlers shall be slow
// and handler tick (10 or 20 msec) shall not be counted upon
//#if (NO_LAPB_CH > 16)
//#define	  MAX_HWRX_LAPB_SCAN	  16
//#define	  MAX_HWTX_LAPB_SCAN	  16
//#define	  MAX_LINK_LAPB_SCAN	  16
//#else
#define 	MAX_HWRX_LAPB_SCAN	NO_LAPB_CH
#define 	MAX_HWTX_LAPB_SCAN	NO_LAPB_CH
#define 	MAX_LINK_LAPB_SCAN	NO_LAPB_CH
//#endif
//common.h

/* should be here ,because of defining variables in only one file */

///#define		MAX_DOWNLOAD_STATE	     3
///#define		MAX_DOWNLOAD_EVENT	     7
///typedef void (*DWNLD_RTN)(void);

//#define	NULL		-1

//#define OK		  TRUE

//#define FAIL		  FALSE

#define WORD_LAPB	unsigned int

//#define BYTE		  unsigned char

#if ((defined RSU_CCSV5_INSTALLED) || (defined RSUSIDE))
#define RSU_NETBUF_SIZE   8192 //2048 //1024
#endif

#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
#ifdef CPU_EC512
#define OAM_NETBUF_SIZE     (16384)
#elif (defined EC1K_SYSTEM)
#ifdef EC1K_LOW_MEMORY
#define OAM_NETBUF_SIZE     16384
#else
#define OAM_NETBUF_SIZE     32768
#endif
#else
#ifdef EC2K_LOW_MEMORY
#define OAM_NETBUF_SIZE     32768
#else
#define OAM_NETBUF_SIZE     131072 // ((8192*4)-1)
#endif
#endif
#endif
#ifdef HDLC_RS232
#ifdef ALARM_PANEL_ON_HDLC_RS232
#define ALARM_NETBUF_SIZE 1024
#endif
#endif
#ifdef HTBY_LINK_ON_MY_UDP
#define HTBY_NETBUF_SIZE     1000000   //500000  //131072
#endif

//#define RX_NETBUF_SIZE    1024
#if (/*(defined OPERATOR_SIDE) &&*/ (defined MY_UDP_FOR_OAM))
// due to compilation in "wbill", we can not use DETAIL_BILLING... in ifdef
#if (defined OPERATOR_SIDE) /*&& (defined DETAIL_BILLING) && ((TON == DETAIL_BILLING_OPR) || ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)))*/
#define OAM_RX_NETBUF_SIZE    65536
#else
#define OAM_RX_NETBUF_SIZE    8192
#endif
#else
#define OAM_RX_NETBUF_SIZE    1024
#endif
#define RSU_RX_NETBUF_SIZE    1024
#define ALARM_RX_NETBUF_SIZE  1024
#ifdef HTBY_LINK_ON_MY_UDP
#define HTBY_RX_NETBUF_SIZE    8192
#else
#define HTBY_RX_NETBUF_SIZE    1024
#endif

/*
#define NETBUF_SIZE	  (8192)
*/


/* max size of FIFO buffers between link layer and network layer.
   since the size of data frames are not  fixed, first byte of each packet
   is used to store size of packet.
*/

#ifdef HDLC_8952
#define MAX_PACKET_SIZE    18
#else
#define MAX_PACKET_SIZE    256
#endif
/* There is no limitation for packet size in linklayer. It is limited to max
   size of FIFO buffers in 8993 */

//#define HWBUF_SIZE	    1024
#define HWBUF_SIZE	  8192

/* max size of common buffers between link layer and physical layer .There
   are two FIFO buffers (hwtx[] and hwrx[]) for communicating data between
   these two layers. */

/* common structure used for reading and writing data to these FIFOs :	  */
/* nettx[],netrx[],hwtx[],hwrx[]					  */
/* it is also used for checking size of all  buffers ?!!!!		  */
/* (with different types) */


/* timer structure , used for publictmr and timers of retransmission window*/

typedef struct{
	BYTE active; /* set if timer is active */
	WORD_LAPB count;
}TIMER_ST;

typedef struct{
	WORD_LAPB seg;
	WORD_LAPB off;
}FLASH_ST;

//ec52hw.h

/* #define ACK_ON_REQ */

/* I am not sure if it should send an ACK for each received frame ,or after
   receiving a request from other side. so both of them are supported.
   when ACK_ON_REQ is selected , receiver need not immediately acknowledge
   the receipt of a frame if it doesn't have any I_frame to send with updated
   sequence counter.in this situation the transmitter will set its P/F bit
   with the last I_frame before closing its window.So the receiver is obliged
   to send an acknowledgement,using a supervisory frame */

#define  CS_CORRECTION TRUE


#ifdef	 HDLC_8952

//#define  CS8952	  0x9000
/*
#define  FIFO_STAT	0
#define  RXTX_DATA	1
#define  CNTRL_REG	2
#define  RX_ADDRS	3
#define  C_CH_TX	4
#define  TIMING_CNTRL	5
#define  INTF_WDT	6
#define  INT_EN 	7
#define  GEN_STAT	8
#define  C_CH_RX	9
*/
#define FIFO_SIZE_8952	19


#endif

/* max count of sync detection for reporting Loss of Sync */
#define SYNC_LOSS_LIMIT 	10
/* max count of sync detection for reporting Sync OK */
#define SYNC_OK_LIMIT		10

void reset_linklayer(BYTE n);
void hwlayer_rx_handler(void);
void hwlayer_tx_handler(void);
void hwlayer_tx_fast_handler(void);
BYTE initialize_hwlayer(BYTE n);
void make_rx_fifo_empty(BYTE n);
//void make_tx_fifo_empty(BYTE n);
void check_8972_sync(BYTE n);

#ifdef HDLC_8952
BYTE init_HDLC_and_8972(BYTE n);
BYTE transmit_packet_8952(BYTE n);
BYTE receive_packet_8952(BYTE n);
BYTE receive_packet_8952_ver2(BYTE n);
#endif

#ifdef HDLC_9075
BYTE init_hdlc_9075(BYTE n);
BYTE transmit_packet_9075(BYTE n);
BYTE receive_packet_9075(BYTE n);
BYTE receive_packet_9075_ver2(BYTE n);
#endif

#ifdef HDLC_9076
BYTE init_hdlc_9076(BYTE n);
BYTE transmit_packet_9076(BYTE n);
BYTE receive_packet_9076_ver2(BYTE n);
#endif
#ifdef HDLC_QUADE1_PEF22554
BYTE init_hdlc_pef22554(unsigned int n);
BYTE transmit_packet_pef22554(unsigned int n);
BYTE receive_packet_pef22554_ver2(unsigned int n);
#endif // HDLC_QUADE1_PEF22554
#if ((defined MY_UDP_FOR_OAM) || ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
BYTE init_hdlc_on_udp(BYTE n);
BYTE transmit_packet_on_udp(BYTE n);
#endif

BYTE save_packet_hdlc(BYTE n);
BYTE init_hdlc_ch(BYTE n);
BYTE transmit_packet(BYTE n);
BYTE receive_packet(BYTE n);

extern void error_report(BYTE state,BYTE event,WORD_LAPB errcode);

#ifdef HDLC_8952
/*
BYTE rfifo_stat(int addr);
BYTE rrx_data(int addr);

void wtx_data(int addr,BYTE a);
BYTE rcntrl(int addr);
void wcntrl(int addr,BYTE a);
BYTE rrx_addrs(int addr);
void wrx_addrs(int addr,BYTE a);
BYTE rcch_tx(int addr);
void wcch_tx(int addr,BYTE a);
BYTE rcch_rx(int addr);
BYTE rtime_cntrl(int addr);
void wtime_cntrl(int addr,BYTE a);
BYTE rint_flag(int addr);
BYTE rint_en(int addr);
void wint_en(int addr,BYTE a);
BYTE rgen_stat(int addr);
*/
#endif


//ec52link.h

/* #define ACK_ON_REQ */

/* I am not sure if it should send an ACK for each received frame ,or after
   receiving a request from other side. so both of them are supported.
   when ACK_ON_REQ is selected , receiver need not immediately acknowledge
   the receipt of a frame if it doesn't have any I_frame to send with updated
   sequence counter.in this situation the transmitter will set its P/F bit
   with the last I_frame before closing its window.So the receiver is obliged
   to send an acknowledgement,using a supervisory frame */


#define FAIL_NO_DATA	   2
#define FAIL_BUF_FULL	   3
#define FAIL_WIN_CLOSE	   4

#define CONTINUE	   TRUE
#define FINISH		   FALSE

#define NO_TMR		   0
/* timer should not be activated. */

#define RETX_WIN_SIZE_1 	1
#define RETX_WIN_SIZE_7 	7

#define RETX_WIN_BUF_SIZE_1	RETX_WIN_SIZE_1+1
#define RETX_WIN_BUF_SIZE_7	RETX_WIN_SIZE_7+1

#define RETX_WIN_SIZE		7
#define RETX_WIN_BUF_SIZE	RETX_WIN_SIZE+1


/*
   max size of retransmission window (the max no. of permitted unacknowledged
   frames).since in get_retxwin() and put_retxwin() routines one WIN_ST is not used for
   storing data,buffer size should be one more than window size.
*/

#define MAX_RETRY_NO	   3
/* Maximum no of I_frame retransmission */

#define SU_FRAME_SIZE	   2
/* size of S_packets and U_packets.( address byte +  control byte) */

//#define COUNT_ADJUST_VALUE  MAX_LINK_LAPB_SCAN_PER_LINK
#define COUNT_ADJUST_VALUE  (hdlc_ch[n].fast_link ? 2:1)   // with new strategy of check_expired_timers
// now if we write some count as (x*COUNT_ADJUST_VALUE), it shall be x*20msec (linklayer_handler tick)

#define IFRAME_ACK_COUNT   (/*200*/600*COUNT_ADJUST_VALUE)  //20
/* max time waiting for receiving acknowledgement */

#define RX_UA_COUNT	   (50*COUNT_ADJUST_VALUE)   //10
/* max time waiting to rx UA after sending SABM or DISC */

#define MAX_TX_PEND_COUNT  (1000*COUNT_ADJUST_VALUE)
/* max time waiting for hw buffers to be empty for transmitting new frame*/

#define MAX_RX_PEND_COUNT  (100*COUNT_ADJUST_VALUE)
/* max time waiting for network buffers to become empty for saving rx_ed frame*/
#define WAIT_BEFORE_RESET_COUNT   (10*COUNT_ADJUST_VALUE)
/* max time waiting before link reset,to be sure that last frame (FRMR,...)
   has been sent.*/
#define MAX_LINK_SETUP_COUNT	  ((RX_UA_COUNT*3)*COUNT_ADJUST_VALUE)	//5

/* since both stations at either end of the link can start transmission,
   both of them can send SABM.The station which has received it first will
   send UA and wait to receive its own response.MAX_LINK_SETUP_COUNT is the
   max time waiting to receive UA,after sending "UA".possibly UA will not
   be received,if station 2 is powered up later than station 1.
*/

/*
  frame format
 _____________________________________________________
| flag | address | control | information | FCS	| flag |
|______|_________|_________|_____________|______|______|
 1 byte  1 byte    1 byte    0to n byte    1byte  1byte
 flag bytes and FCS byte are supported by hardware layer.

			  control byte
		     lsb		   msb
		     _1__2__3__4__5__6__7__8__
      Information    | 0 |  N(S) |P/F|	N(R)   |
       (I_Frame)     |___|_______|___|_________|
						     SS :
		      _1__2__3__4__5__6__7__8__ 	RR  : 00 P/F Receiver Ready
      Supervisory    | 1 |0 | SS |P/F|	N(R)   |	REJ : 01 P/F Reject
       (S_Frame)     |___|__|____|___|________ |	RNR : 10 P/F Receiver Not Ready
							SREJ: 11 -----------
		      _1__2__3__4__5__6__7__8__
      Unnumbered     | 1 |1 | MM |P/F|	MMM    |
       (U_Frame)     |___|__|____|___|_________|     MM P/F MMM :
							SABM : 11 P/F 100  Set Asynchronous Balanced Mode
							DISC : 00 P   010  Disconnect
							DM   : 11 F   000  Disconnect Mode
							UA   : 00 F   110  Unnumbered Acknowledgement
							FRMR : 10 F   001  Frame Reject
							---------------------
							CMDR : 10 F   001
							RESET: 11 P   001
							etc.
			  control byte

		      _7__6__5__4__3__2__1__0_
      Information    |	N(R)  |P/F| N(S)   |0 |
       (I_Frame)     |________|___|________|__|

		      _7__6__5__4__3__2__1__0_	     SS :
      Supervisory    |	N(R)  |P/F| SS	|0 |1 | 	RR  : 00 P/F Receiver Ready
       (S_Frame)     |________|___|_____|__|__| 	REJ : 10 P/F Reject
							RNR : 01 P/F Receiver Not Ready
							SREJ: 11 ------------
		      _7__6__5__4__3__2__1__0_
      Unnumbered     |	MMM   |P/F|  MM |1 |1 |      MMM P/F MM :
       (U_Frame)     |________|___|_____|__|__| 	SABM : 001 P/F 11 Set Asynchronous Balanced Mode
							DISC : 010 P   00 Disconnect
							DM   : 000 F   11 Disconnect Mode
							UA   : 011 F   00 Unnumbered Acknowledgement
							FRMR : 100 F   01 Frame Reject
							---------------------
							CMDR : 100 F   01
							RESET: 100 P   11
							etc.

*/
#define NR_SHIFT	   5
#define NS_SHIFT	   1
/* no. of right shits necessary to put Nr and Ns in bits 2,1,0 */
#define NR_POS		   0xe0
/* position of N(R) bits in I_frame and S_frame */
#define NS_POS		   0x0e
/* position of N(S) bits in S_frame */
#define PF_POS		   0x10
/* position of P/F bit in I_frame ,S_frame and U_frame */
#define I_POS		   0x01
/* position of I_frame recognition bit.(bit no. 1 of I_frames should be one.) */
#define SU_POS		   0x03
/* position of S_frame and U_frame recognition bits. */
#define CMD_S_POS	   0x0c
/* position of S_frame command code bits */
#define CMD_U_POS	   0xec
/* position of U_frame command code bits */
#define LAPB_I_FRAME		0x00
/* I_frame recognition bit.( bit0 = 0 ) */
#define S_FRAME 	   0x01
/* S_frame recognition bits.( bit0 = 1,bit1 = 0 ) */
#define U_FRAME 	   0x03
/* U_frame recognition bits.( bit0 = 1,bit1 = 1 ) */
#define IRETX_FRAME	   0x04    /* dummy */

#define LAPB_RR 		0x00
/* RR command code */
#define REJ		   0x08
/* REJ command code */
#define RNR		   0x04
/* RNR command code */
#define LAPB_SABM		0x2c
/* SABM command code */
#define DISC		   0x40
/* DISC command code */
#define LAPB_UA 		0x60
/* UA response code */
#define DM		   0x0c
/* DM response code */
#define FRMR		   0x84
/* FRMR command code */
#define PF_SET		   0x10
/* Poll/Final bit is set*/
#define PF_RESET	   0x00
/* Poll/Final bit is reset */

/* frames with P=1 requires a response at earliest opportunity with F bit set to 1*/


#define TRI_BIT 	   0x07
/* Send and receive sequence counters contain the sequence number of
   the frame module 8,it means that frames no. will take the numbers :
   0 1 2 3 4 5 6 7 0 1 etc. TRI_BIT is used for comparison and addition
   of Vr and Vs in module 8 */

#define MAX_STATE	   8
#define MAX_EVENT	   13

/* link states */

#define IDLE			0
#define WAIT_FOR_UA		1
#define INFO_TRX		2
#define REMOTE_STN_BUSY 	3
#define TX_PEND 		4
#define RX_PEND 		5
#define WAIT_BEFORE_RESET	6
#define WAIT_FOR_LINK_SETUP	7
/* link events */

#define I_RX		   0
#define RR_RX		   1
#define RNR_RX		   2
#define REJ_RX		   3
#define SABM_RX 	   4
#define DISC_RX 	   5
#define DM_RX		   6
#define UA_RX		   7
#define FRMR_RX 	   8
#define LAPB_TIME_OUT	   9
#define PACKET_TIME_OUT    10
#define LAPB_NULL_EVENT    11
#define BAD_CMD 	   12

/************************************************************************/

typedef BYTE (*FUNC_PTR)(BYTE);


union union_rxframe{
      struct{
	     BYTE      Nr;	/* Next frame no. expected to receive */
	     BYTE      Ns;	/* Next frame no. to send */
	     BYTE      size;	/* data size of received frame */
	     BYTE      tmparr[MAX_PACKET_SIZE]; /* array for temp storage of rx_ed frame*/
      }i;
      struct{
	     BYTE Nr;	     /* Next frame to receive */
	     BYTE S_cmd;    /* Command code of S_frame : RR,RNR,REJ */
      }s;
      struct{
	     BYTE U_cmd;    /* Command type of U_frame : SABM,DISC,UA,FRMR,...*/
      }u;
};

/* structure used for temporary storage of received frame.(I,S or U frame)*/

typedef struct{
		union union_rxframe f;
		BYTE pf;		 /* P/F bit of pending frame*/
		BYTE ftype;		 /* type of received frame : I or S or U */
}RXFRAME_ST;


/* structure used for storage of pending frame information.(I,S or U frame)*/

typedef struct{
		BYTE	 type;	   /* ITX, IRETX,   S	,  U	*/
		BYTE	 subtype;  /* NULL, NULL, S_cmd,U_cmd */
		BYTE	 pf;	   /* P/F bit of pending frame	*/
		BYTE	 nxtstate; /* next state after exit from pending condition*/
		WORD_LAPB   count;    /* timer count of pending frame if needed*/
}TXFRAME_ST;


/* structure of unacked frames in retransmission window */

typedef struct{
	     BYTE   pf;        /* P/F bit */
	     BYTE   Ns;        /* frame id */
	     WORD_LAPB netindex;  /* index of frame in nettx[] */
	     BYTE   retry_no;  /* no of retransmissions */
	     WORD_LAPB	 count;     /* timer for receiving ACK */
}WIN_ST;


/* structure for reading and writing data to and from retxwin[] 	 */

typedef struct {
		WIN_ST *arr;
		WORD_LAPB   front;
		WORD_LAPB   rear;
		WORD_LAPB   size;
		BYTE   win_size;
}WINBUF;

typedef struct{
	unsigned long cs;
#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
	BYTE oam_no;
#endif
#ifdef HDLC_QUADE1_PEF22554
	BYTE e1_no;
#endif
	unsigned int dl_ch:3;
	unsigned int ic_type:3;
	unsigned int fast_link:1;
}HDLC_ST;

/* #define MAX_PACKET_SIZE    19					   */

/* I have not checked it in link layer ,to be checked in hw_layer.	   */

BYTE link_rtn1(BYTE n);
BYTE link_rtn2(BYTE n);
BYTE link_rtn3(BYTE n);
BYTE link_rtn4(BYTE n);
BYTE link_rtn5(BYTE n);
BYTE link_rtn6(BYTE n);
BYTE link_rtn7(BYTE n);
BYTE link_rtn8(BYTE n);
BYTE link_rtn9(BYTE n);
BYTE link_rtn10(BYTE n);
BYTE link_rtn11(BYTE n);
BYTE link_rtn12(BYTE n);
BYTE link_rtn13(BYTE n);
BYTE link_rtn14(BYTE n);
BYTE link_rtn15(BYTE n);
BYTE link_rtn16(BYTE n);
BYTE link_rtn17(BYTE n);
BYTE link_rtn18(BYTE n);
BYTE link_rtn19(BYTE n);
BYTE link_rtn20(BYTE n);

BYTE link_rtn_null(BYTE n);
BYTE link_reset(BYTE n);
BYTE link_hw_reset(BYTE n);

BYTE read_received_frame(BYTE n);
BYTE check_NR_remove_ACKedframes(BYTE n,BYTE nr);
BYTE save_received_frame(BYTE n);
BYTE send_Sframe(BYTE n,BYTE scmd,BYTE pf,WORD_LAPB tmrcount);
BYTE send_Uframe(BYTE n,BYTE ucmd,BYTE pf,WORD_LAPB tmrcount);
BYTE send_Iframe(BYTE n,BYTE pf);
void save_tx_information(BYTE n,BYTE type,BYTE subtype,BYTE pf,BYTE nxtstate,WORD_LAPB count);
void check_transmission_link(BYTE n);
BYTE release_retxwin(BYTE n);

BYTE retx_Iframe(BYTE n);
BYTE put_retxwin(WINBUF *buf,WIN_ST *dat);
BYTE get_retxwin(WINBUF *buf,WIN_ST *dat);
void linklayer_handler(void);
void linklayer_fast_handler(void);
BYTE check_expired_timers(BYTE n);
void tx_pend (BYTE n,WORD_LAPB count);
void rx_pend (BYTE n,WORD_LAPB count);
void initialize_linklayer(BYTE n);
void initialize_network_buf(BYTE n);
void init_link_rtns(void);
void init_lapb(void);
void init_lapb_for_reloadrsu(void);

void check_8972_sync(BYTE n);
BYTE initialize_hwlayer(BYTE n);

void error_report(BYTE state,BYTE event,WORD_LAPB errcode);

#ifdef HDLC_RS232
BYTE transmit_packet_rs232(BYTE n);
BYTE receive_packet_rs232(BYTE n);
BYTE init_hdlc_rs232(BYTE n);
void make_rs232_rx_fifo_empty(BYTE n);
#endif

char lapb_link_ready_for_check(BYTE n);
//VIOLET
//char check_buf_empty(BUF *buf);
/*
extern BYTE put_buf(BUF *buf,BYTE dat);
extern BYTE get_buf(BUF *buf,BYTE *dat);
extern BYTE read_buf(BUF *buf,BYTE *dat);
extern WORD_LAPB checkfree_buf(BUF *buf);
*/
char lapb_link_ready(BYTE n);
#define 	MAX_9075_BYTES_RD_PER_TICK	64
#define 	MAX_9076_BYTES_RD_PER_TICK	128 //64

void close_lapb_err_file(void);
void open_lapb_err_file(void);

#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
int to_nettx_que(BYTE *p,BYTE s,BYTE n); /* put s bytes from p array to tx que */
int from_netrx_que(BYTE n); /* put s bytes from p array to tx que */
#endif

#endif // LAPB

#if ((defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
#define 	MAX_CHECK_LINK_COUNT		8
void init_link_handler(void);
void check_link_handler(void);
#endif

#ifdef LAPB
#define 	SYSTEM_ID		0
#define 	RSU_ID			1
#define 	THIS_RSU_ID		1

#define 	OPR0_ID 		21
#define 	OPR1_ID 		22
#define 	OPR2_ID 		23
#define 	OPR3_ID 		24
#define 	OPR4_ID 		25

#ifdef ALARM_PANEL_ON_HDLC_RS232
#define 	ALARM_ID		31
#endif
#endif
