

			/****  operator network ****/
//LAST TCP_ERR : 0x2301

#include "omtfp.h"
#include "opr_net.h"

#include "/ec/work/sys/lintcp.ext"

#define BEHNAMIR_16BIT_PMAMFTX_OPR_TO_SYS
#define BEHNAMIR_16BIT_PCPMFTX_OPR_TO_SYS

TX_RCRD tx[MAX_NO_OPR_CARD];
RX_RCRD rx[MAX_NO_OPR_CARD];

//#define DEBUG_ACT
//#define DEBUG_ACTIVE

//#define NET_DEBUG

#define MT8972_ERROR_ACTIVE

//extern TX_RCRD tx[];
//extern RX_RCRD rx[];
extern OPR_RCRD opr[];
extern TRW w[];
extern unsigned int f;
extern int rst_sgntr;
extern OPR_CALL_REC ocr;
extern BYTE mb[];
extern BYTE link_state[];
extern FPNH olrtn_idx[][NO_LINK_EVENTS];

#ifdef OPERATOR_PC
extern FILE *fer;
extern FILE *flog;
#ifdef LOG_FILE
extern DATTIM sdt;  /* system date time */
#endif
extern int base_addr[7];
extern int card;
extern BYTE sync_led[];
extern BYTE link_up_led[];

#endif

#ifdef NET_DEBUG
extern FILE *net_fp;
#endif

#ifdef PRE_INSTALL
extern BYTE org;
#endif

#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
extern BUF nettx_st[];
extern BUF netrx_st[];
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
extern BYTE hot_link;
#endif

#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef OPR_UPLOAD_WITH_TIMER
extern unsigned int upload_count;
#endif
#endif

/*========================================================================*/
/*========================================================================*/
#ifdef DAY_NIGHT_PARAMETERS_FILE
extern char *pfna[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE];
#else
extern char *pfna[MAX_NO_OPR_CARD][1];
#endif
extern unsigned short parmb[];

/*========================================================================*/
/*========================================================================*/

#if 0

#define FTP_T	3   // like CR_T
#define FTP_BLOCK_SIZE	16384
/*========================================================================*/
/*========================================================================*/

#define   FLAG_Copied	      0x80
#define   FLAG_Compress       0x40
typedef   signed char	      kt_byte;
typedef   signed short	      kt_word;
typedef   unsigned char       kt_ubyte;
typedef   unsigned short      kt_uword;
typedef   unsigned long       kt_ulong;
typedef   unsigned char       kt_bool;

kt_bool      GetMatch(Source,X,SourceSize,Hash,Size,Pos)
kt_ubyte     *Source;
kt_uword     X;
kt_uword     SourceSize;
kt_word      *Hash;
kt_uword     *Size;
kt_word      *Pos;
{    kt_uword	  HashValue  = (40543L*((((Source[X] << 4) ^
			      Source[X+1]) << 4) ^ Source[X+2]) >> 4) & 0xfff;
     *Pos = Hash[HashValue];
     Hash[HashValue] = X;
     if ((*Pos != -1) && ((X-*Pos) < 4096))
	  {    for (*Size = 0; ((*Size < 18)
			 && (Source[X+*Size] == Source[*Pos+*Size])
			 && ((X+*Size) < SourceSize)); (*Size)++);
	       return(*Size >= 3);
	  }
     return(FALSE);
}

/*========================================================================*/
/*========================================================================*/

kt_uword     Compression(Source,Dest,SourceSize)
kt_ubyte     *Source,*Dest;
kt_uword     SourceSize;
{    kt_word	  Hash[4096];
     kt_uword	  Key,Size,Pos;
     kt_ubyte	  Bit			   = 0;
     kt_uword	  Command=0;
     kt_uword	  X			   = 0;
     kt_uword	  Y			   = 3;
     kt_uword	  Z			   = 1;
     for (Key = 0; Key < 4096; Key++)
	  Hash[Key] = -1;
     Dest[0] = FLAG_Compress;
     for (; (X < SourceSize) && (Y <= SourceSize);)
	 {     print_error_report("X = %i Y = %i Z = %i",X,Y,Z);
	       if (Bit > 15)
		    {	 Dest[Z++] = (Command >> 8) & 0x00ff;
			 Dest[Z] = Command & 0x00ff;
			 Z = Y;
			 Bit = 0;
			 Y += 2;
		    }
	       for (Size = 1; (Source[X] == Source[X+Size]) && (Size < 0x0fff)
				   && (X+Size < SourceSize); Size++);
	       if (Size >= 16)
		    {	 Dest[Y++] = 0;
			 Dest[Y++] = ((Size - 16) >> 8) & 0x00ff;
			 Dest[Y++] = (Size - 16) & 0x00ff;
			 Dest[Y++] = Source[X];
			 X += Size;
			 Command = (Command << 1) + 1;
		    }
	       else if (GetMatch(Source,X,SourceSize,Hash,&Size,&Pos))
			 {    Key = ((X-Pos) << 4) + (Size - 3);
			      Dest[Y++] = (Key >> 8) & 0x00ff;
			      Dest[Y++] = Key & 0x00ff;
			      X += Size;
			      Command = (Command << 1) + 1;
			 }
		    else {    Dest[Y++] = Source[X++];
			      Command = (Command << 1);
			 }
	       Bit++;
	  }
     Command <<= (16-Bit);
     Dest[Z++] = (Command >> 8) & 0x00ff;
     Dest[Z] = Command & 0x00ff;
     if (Y > SourceSize)
	  {    for(Y = 0; Y < SourceSize; Dest[Y+1] = Source[Y++]);
	       Dest[0] = FLAG_Copied;
	       return(SourceSize+1);
	  }
     return(Y);
}

/*========================================================================*/
/*========================================================================*/

kt_uword     Decompression(Source,Dest,SourceSize)
kt_ubyte     *Source,*Dest;
kt_uword     SourceSize;
{    kt_uword	  X			   = 3;
     kt_uword	  Y			   = 0;
     kt_uword	  Pos,Size,K;
     kt_uword	  Command		   = (Source[1] << 8) + Source[2];
     kt_ubyte	  Bit			   = 16;
     if (Source[0] == FLAG_Copied)
	  {    for (Y = 1; Y < SourceSize; Dest[Y-1] = Source[Y++]);
	       return(SourceSize-1);
	  }
     for (; X < SourceSize;)
	  {    if (Bit == 0)
		    {	 Command = (Source[X++] << 8);
			 Command += Source[X++];
			 Bit = 16;
		    }
	       if (Command & 0x8000)
		    {	 Pos = (Source[X++] << 4);
			 Pos += (Source[X] >> 4);
			 if (Pos)
			      {    Size = (Source[X++] & 0x0f)+3;
				   for (K = 0; K < Size; K++)
					Dest[Y+K] = Dest[Y-Pos+K];
				   Y += Size;
			      }
			 else {    Size = (Source[X++] << 8);
				   Size += Source[X++] + 16;
				   for (K = 0; K < Size; Dest[Y+K++]=Source[X]);
				   X++;
				   Y += Size;
			      }
		    }
	       else Dest[Y++] = Source[X++];
	       Command <<= 1;
	       Bit--;
	  }
     return(Y);
}
/* ========================================================================== */
/* ========================================================================== */
BYTE rcv_ftp_buf[100000];
unsigned short this_ftp_packet_ptr=0;
unsigned short this_ftp_packet_size;
unsigned long ftp_buf_ptr=0L;
BYTE source[FTP_BLOCK_SIZE+4];
/* ========================================================================== */
/* ========================================================================== */
void handle_ftp(BYTE size){
	BYTE i;
	if(this_ftp_packet_ptr == 0){
		this_ftp_packet_size=(mb[1]<<8) | mb[2];
		i=3;
	}
	else
		i=1;
	for(;i<size;i++,this_ftp_packet_ptr++)
		source[this_ftp_packet_ptr]=mb[i];
	if(this_ftp_packet_ptr >= this_ftp_packet_size){
		ftp_buf_ptr += Decompression(source,&rcv_ftp_buf[ftp_buf_ptr],this_ftp_packet_size);
		this_ftp_packet_ptr=0;
		if(ftp_buf_ptr >= (2*NO_LOAD_PARM)){
			memcpy((BYTE *)parmb,rcv_ftp_buf,2*NO_LOAD_PARM);
			cpf(pfna[0][0],0);
			ftp_buf_ptr=0L;
		}
	}
}
#endif //0
/* ========================================================================== */
/* ========================================================================== */


#ifdef HDLC_8993
int miftx(BYTE n){  /* make I frame for transmit */
	int s,f,e;
	BYTE *a,*b;
	TX_RCRD *xp;

//#ifdef MT8972_ERROR_ACTIVE
	BYTE c;  /* for correcting MT8972 error */
//#endif

	xp=&tx[n];

	f=xp->qf;
	e=xp->qe;
	s=e-f;
	if(s < 0)
		s += TX_QUE_LEN;  /* size of queue */
	if(s >= MIN_I_PKT){  /* if I available */
		b=xp->ob;
		a=xp->q;
#ifdef BATCH_PROGRAMMING
		s=a[++f & (TX_QUE_LEN-1)];
		b[0]=0;
#else
		b[0]=opr[n].na;
#endif

#ifdef BATCH_PROGRAMMING
		/* control byte will be updated at the time of transmission */
		b[2]=s+3;
		xp->obs=s+3;
		for(e=3,c=0x01;s;s--,e++,c <<=1){
			f= (f + 1) & (TX_QUE_LEN-1);
			b[e]=a[f];
#ifdef MT8972_ERROR_ACTIVE
			if( (b[e] & 0xf0) == 0xf0){
			/* if higher nibble is 0xf0 */
				b[0] |= c;
				/* mark in size byte */
				b[e] &= 0x0f;
				/* reset higher nibble to zero */
			}
#endif
		}
		xp->qf=f;

#else // BATCH_PROGRAMMING
		/* control byte will be updated at the time of transmission */
		if(s >= FIFO_SIZE){
			s=FIFO_SIZE;
			b[2]=FIFO_SIZE+3;
			for(e=3,c=0x10;s;s--,e++,c <<=1){
				f= ++f & (TX_QUE_LEN-1);
				b[e]=a[f];
#ifdef MT8972_ERROR_ACTIVE
				if( (b[e] & 0xf0) == 0xf0){
				/* if higher nibble is 0xf0 */
					b[2] |= c;
					/* mark in size byte */
					b[e] &= 0x0f;
					/* reset higher nibble to zero */
				}
#endif
			}
			xp->obs=FIFO_SIZE+3;
			xp->qf=f;
		}
		else{
			b[2]=s+3;
			for(s=3,c=0x10;f != e;s++){
				f= ++f & (TX_QUE_LEN-1);
				b[s]=a[f];
#ifdef MT8972_ERROR_ACTIVE
				if( (b[s] & 0xf0) == 0xf0){
				/* if higher nibble is 0xf0 */
					b[2] |= c;
					/* mark in size byte */
					b[s] &= 0x0f;
					/* reset higher nibble to zero */
				}
#endif
			}
			xp->obs=s;
			xp->qf=e;
		}
#endif // BATCH_PROGRAMMING
		if(w[n].nfts)	/* inc. nfts */
			w[n].nfts=0;
		else
			w[n].nfts=FRM1_TS;
		xp->n2=0;
		return OK;
	}
	/* I not available */
	xp->obs=0;
	return RESET;
}
#endif


void fec_pc(void){   /*find existing cards and their address in pc*/
#ifdef OPERATOR_PC
//DOOD start: some changes
	card=0;
#ifdef HDLC_8993
		{
			int tmp;
			int i;

			tmp=MT8993_PORT;
			for(i=0;i<7;i++){
// DOOD : outportb->kt_outport8
				kt_outport8(tmp | TCR,0x87);
// DOOD : inportb->kt_inport8
				if(kt_inport8(tmp | TCR) == 0x87){
					base_addr[card]=tmp;
					card++;
				}
				tmp+=0x2000;
			}
			if(card==0){
				ocni();    /* if operator card not installed */
				return;
			}
		}
//DOOD end
#endif
	f &= ~OPR_CARD_OUT;  /* operator card installed */
#ifndef LIM
	card=1; // force extra cards to be ignored
#endif
	return;
#else
	card = 1;
	return;
#endif
}


#ifdef HDLC_8993
void ihc(BYTE n){  /* initialize HDLC controller */

#ifdef OPERATOR_PC
//DOOD start: #ifdef added
#ifdef HDLC_8993
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | TCR,0x87);	/* ch1=c_ch,ch2=HDLC,ch3=PCM */
// DOOD : inportb->kt_inport8
	if(kt_inport8(base_addr[n] | TCR) != 0x87){
		ocni();
		return;
	}
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | C_CHAN,0x28);    /* attack disable */
// DOOD : inportb->kt_inport8
	while(kt_inport8(base_addr[n] | HDLC_CSR) & 0x03)
// DOOD : inportb->kt_inport8
		kt_inport8(base_addr[n] | HDLC_XR);  /* read until fifo empty */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x02);  /* rx,tx enable,idle,64 kbps */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x02);  /* rx,tx enable,idle,64 kbps */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x02);  /* rx,tx enable,idle,64 kbps */
// DOOD : outportb->kt_outport8
//	  kt_outport8(base_addr[n] | SD_DIR,0);
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | TEST_REG,ALL_TEST_DISABLE);
/* added 8/2/76 */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_ISR,0xdd);  /* Enable all interrupts except EOPR and Rx8/11 */
#endif
//DOOD end
#else
	BYTE a;
	XBYTE[TCR]=0x87;  /* ch1=c_ch,ch2=HDLC,ch3=PCM */
	if( (a=XBYTE[TCR]) != 0x87){  /* if operator card defected */
		str2lcd("OPR. DEFECTED.");
		while(1)
			lcds();
	}
	XBYTE[C_CHAN]=0x28;  /* attack disable */
	while(XBYTE[HDLC_CSR] & 0x03)
		a=XBYTE[HDLC_XR];  /* read until fifo empty */
	XBYTE[HDLC_CSR]=0x02;  /* rx,tx enable,idle,64 kbps */
	XBYTE[HDLC_CSR]=0x02;  /* rx,tx enable,idle,64 kbps */
	XBYTE[HDLC_CSR]=0x02;  /* rx,tx enable,idle,64 kbps */
	XBYTE[SD_DIR]=0;
	XBYTE[TEST_REG]=ALL_TEST_DISABLE;
/* added 8/2/76 */
	XBYTE[HDLC_ISR]=0xdd;  /* Enable all interrupts except EOPR and Rx8/11 */

#endif

#ifdef NET_DEBUG
	fputs("ihc:\n",net_fp);
#endif
/*
	str2lcd(" ihc: ");
*/
}
#endif

#ifdef HDLC_8993
void ihp(BYTE n){  /* initialize H-phone */

#ifndef OPERATOR_PC
	BYTE a;
#endif
	OPR_RCRD *op;
	op=&opr[n];

	rx[n].ibs=0;
	rx[n].qe=0;
	rx[n].qf=0;
	w[n].fe=0;
	w[n].nfts=0;
	op->na=OPRT_ADDR;
	op->sspc=0;
	op->srpc=0;
	op->snipc=0;

#ifdef OPERATOR_PC

#ifdef PRE_INSTALL
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | RGCR,org);
#else
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | RGCR,OPR_RCVR_GAIN);
#endif
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | TRNS_CR,0);
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | TR1,FREQ_1);
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | TR2,FREQ_2);
	ihc(n);
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | DSP_CR,0x61);
#else
	PWR_RST_8993=0;
	for(a=0;a<0x80;a++);
	PWR_RST_8993=1;
	for(a=0;a<0xff;a++);
	for(a=0;a<0xff;a++);

#ifdef PRE_INSTALL
	XBYTE[RGCR]=org;
#else
	XBYTE[RGCR]=OPR_RCVR_GAIN;
#endif
	XBYTE[TRNS_CR]=0;
	XBYTE[TR1]=FREQ_1;
	XBYTE[TR2]=FREQ_2;
	ihc(0);
	XBYTE[DSP_CR]=0x61;
#endif

#ifdef NET_DEBUG
	fputs("ihp:\n",net_fp);
#endif
/*	str2lcd(" ihp: ");
*/
}
#endif

#ifdef HDLC_8993
void roni(BYTE n){  /* reset operator network initialize */

#ifndef OPERATOR_PC
	BYTE a;
#endif
	OPR_RCRD *op;
	TX_RCRD *xp;
	op=&opr[n];
	xp=&tx[n];

	xp->obs=xp->qe=xp->qf=0;
	xp->n2=0;
	rx[n].ibs=rx[n].qe=rx[n].qf=0;
	w[n].fe=w[n].nfts=0;
	op->na=OPRT_ADDR;
	op->sspc=op->srpc=op->snipc=0;
	link_state[n]=RESET_NOT_SYNC_LINK_NOT_AVAIL;

#ifdef	OPERATOR_PC

#ifdef PRE_INSTALL
// DOOD : outportb->kt_outport8
	     kt_outport8(base_addr[n] | RGCR,org);
#else
// DOOD : outportb->kt_outport8
	     kt_outport8(base_addr[n] | RGCR,OPR_RCVR_GAIN);
#endif
// DOOD : outportb->kt_outport8
	     kt_outport8(base_addr[n] | TRNS_CR,0);
// DOOD : outportb->kt_outport8
	     kt_outport8(base_addr[n] | TR1,FREQ_1);
// DOOD : outportb->kt_outport8
	     kt_outport8(base_addr[n] | TR2,FREQ_2);
	     ihc(n);
// DOOD : outportb->kt_outport8
	     kt_outport8(base_addr[n] | DSP_CR,0x61);
	     rst_sgntr=RESET_SIGNATURE;

#else
	PWR_RST_8993=0;
	for(a=0;a<0x80;a++);
	PWR_RST_8993=1;
	for(a=0;a<0xff;a++);
	for(a=0;a<0xff;a++);

#ifdef PRE_INSTALL
	XBYTE[RGCR]=org;
#else
	XBYTE[RGCR]=OPR_RCVR_GAIN;
#endif
	XBYTE[TRNS_CR]=0;
	XBYTE[TR1]=FREQ_1;
	XBYTE[TR2]=FREQ_2;
	ihc(0);
	XBYTE[DSP_CR]=0x61;
	rst_sgntr=RESET_SIGNATURE;
#endif
	led_reset(sync_led[n]);
	led_reset(link_up_led[n]);
	led_set(OPR_RESET_LED);
	olrtn_init();
}
#endif

#ifdef HDLC_8993
void txsuf(BYTE pb,BYTE n){  /* transmit S or U frame */
	OPR_RCRD *op;
#ifdef CHECKSUM
	BYTE chksum=0;
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		return;

	op=&opr[n];
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | TCR,0x87);  /* ch1=c_ch,ch2=HDLC,ch3=PCM */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_XR,op->na);
#ifdef CHECKSUM
	chksum+=op->na;
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_XR,pb);
	chksum+=pb;
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x12);  /* set EOP */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_XR,~chksum);
#else
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x12);  /* set EOP */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_XR,pb);
#endif
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x02);  /* reset EOP,enable tx */
#else
	XBYTE[TCR]=0x87;  /* ch1=c_ch,ch2=HDLC,ch3=PCM */
	XBYTE[HDLC_XR]=opr.na;
	chksum+=opr.na;
#ifdef CHECKSUM
	XBYTE[HDLC_XR]=pb;
	chksum+=pb;
	XBYTE[HDLC_CSR]=0x12;  /* set EOP */
	XBYTE[HDLC_XR]=~chksum;
#else
	XBYTE[HDLC_CSR]=0x12;  /* set EOP */
	XBYTE[HDLC_XR]=pb;
#endif
	XBYTE[HDLC_CSR]=0x02;  /* reset EOP,enable tx */
#endif

#ifdef NET_DEBUG
	fprintf(net_fp,"txsu: %x %x\n",pb,n);
#endif

#ifdef NET_DEBUG
	fprintf(net_fp," w.nfts=%x,w.fe=%x\n",w[n].nfts,w[n].fe);
#endif

#ifdef DEBUG_ACT
	str2lcd(" txsu: ");hex2lcd(pb);tolcdq(' ',TO_LCD_DD);
#endif
}
#endif

#ifdef HDLC_8993
void txf(BYTE n){  /* transmit frame */
	BYTE i,e;
	BYTE *b;
#ifdef CHECKSUM
	BYTE chksum=0;
#endif
	e=tx[n].obs-1;
	if(e == 0xff){	/* if buffer empty */

#ifdef NET_DEBUG
		fprintf(net_fp,"txf:2 %x\n",n);
#endif

#ifdef DEBUG_ACT
		str2lcd(" txf:2 ");
#endif
		return;
	}
	b=tx[n].ob;

#ifdef OPERATOR_PC
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | TCR,0x87);  /* ch1=c_ch,ch2=HDLC,ch3=PCM */
#else
	XBYTE[TCR]=0x87;  /* C ch,ch 3=PCM,ch 2=HDLC */
#endif

#ifdef NET_DEBUG
	fprintf(net_fp,"t=%x ",tx[n].obs);
#endif

#ifdef DEBUG_ACT
	str2lcd("t=");hex2lcd(tx[n].obs);
#endif
	/* update control byte,nfts or N(S) is not updated */
	if(w[n].nfts)  /* if nfts=1,N(S)= 0 */
		b[1]=I_C | P_F | w[n].fe;
	else  /* if nfts=0,N(S)=1 */
		b[1]=I_C | P_F | w[n].fe | FRM1_TS;
	/* because link layer is stop & wait,tx window size is 1 */

	for(i=0;i<e;i++){

#ifdef OPERATOR_PC
// DOOD : outportb->kt_outport8
		kt_outport8(base_addr[n] | HDLC_XR,b[i]);
#else
		XBYTE[HDLC_XR]=b[i];
#endif
#ifdef CHECKSUM
		chksum+=b[i];
#endif

#ifdef NET_DEBUG
		fprintf(net_fp,"%x %x ",b[i],n);
#endif

#ifdef DEBUG_ACT

		tolcdq(' ',TO_LCD_DD);hex2lcd(b[i]);

#endif
	}

#ifdef OPERATOR_PC

#ifdef CHECKSUM
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_XR,b[i]);
	chksum+=b[i];
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x12);  /* set EOP */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_XR,~chksum);
#else
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x12);  /* set EOP */
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_XR,b[i]);
#endif

#else

#ifdef CHECKSUM
	XBYTE[HDLC_XR]=b[i];
	chksum+=b[i];
	XBYTE[HDLC_CSR]=0x12;  /* set EOP */
	XBYTE[HDLC_XR]=~chksum;
#else

	XBYTE[HDLC_CSR]=0x12;  /* set EOP */
	XBYTE[HDLC_XR]=b[i];
#endif

#endif

#ifdef NET_DEBUG
	fprintf(net_fp,"%x ,w.nfts=%x,w.fe=%x\n",b[i],w[n].nfts,w[n].fe);
#endif

#ifdef DEBUG_ACT

	tolcdq(' ',TO_LCD_DD);hex2lcd(b[i]);
#endif

#ifdef OPERATOR_PC
// DOOD : outportb->kt_outport8
	kt_outport8(base_addr[n] | HDLC_CSR,0x02);  /* reset EOP,enable tx */
#else
	XBYTE[HDLC_CSR]=0x02;  /* reset EOP,enable tx */
#endif
}
#endif

#ifdef HDLC_8993
int ofrx(BYTE n){  /* operator frame received */
	BYTE a,i;
	BYTE *b;
	RX_RCRD *xp;
#ifdef CHECKSUM
	BYTE chksum=0;
#endif
	xp=&rx[n];
	xp->ibs=0;

#ifdef OPERATOR_PC
// DOOD : inportb->kt_inport8
	a=kt_inport8(base_addr[n] | HDLC_CSR);
#else
	a=XBYTE[HDLC_CSR];
#endif
	if( (a & 0x03) == 0){  /* if rx fifo 8993 empty */

#ifdef NET_DEBUG
		fprintf(net_fp,"ofrx:0 %x\n",n);
#endif

#ifdef DEBUG_ACT
		str2lcd(" ofrx:0 ");
#endif
		ihc(n);
		return FRMERR;
	}
	if( (a & 0x30) != 0x20){  /* if not first byte */

#ifdef NET_DEBUG
		fprintf(net_fp,"ofrx:1 %x\n",n);
#endif

#ifdef DEBUG_ACT
		str2lcd(" ofrx:1 ");
#endif
		ihc(n);
		return FRMERR;
	}
	i=0;
	b=xp->ib;
	do{

#ifdef OPERATOR_PC
// DOOD : inportb->kt_inport8
		b[i]=kt_inport8(base_addr[n] | HDLC_XR);
#else
		b[i]=XBYTE[HDLC_XR];
#endif
#ifdef CHECKSUM
		chksum+=b[i];
		if( (++i) >= (FIFO_SIZE+4) ){
#else
		if( (++i) >= (FIFO_SIZE+3) ){
#endif

#ifdef NET_DEBUG
			fprintf(net_fp,"ofrx:2 %x\n",n);
#endif

#ifdef DEBUG_ACT
			str2lcd(" ofrx:2 ");
#endif
			ihc(n);
			return FRMERR;
		}

#ifdef OPERATOR_PC
// DOOD : inportb->kt_inport8
		a=kt_inport8(base_addr[n] | HDLC_CSR) & 0x30;
#else
		a=XBYTE[HDLC_CSR] & 0x30;
#endif
	}while(a == 0);  /* read packet byte */
	if(a == 0x10){	/* if good FCS */

#ifdef OPERATOR_PC

#ifdef CHECKSUM
// DOOD : inportb->kt_inport8
// DOOD : inportb->kt_inport8
		chksum+=kt_inport8(base_addr[n] | HDLC_XR);
		if(chksum != 0xff){
			ihc(n);
			return FRMERR;
		}
#else
// DOOD : inportb->kt_inport8
		b[i]=kt_inport8(base_addr[n] | HDLC_XR);
#endif

#else

#ifdef CHECKSUM
		chksum+=XBYTE[HDLC_XR];
		if(chksum != 0xff){
			ihc(n);
			return FRMERR;
		}
#else
		b[i]=XBYTE[HDLC_XR];
#endif

#endif
		if(b[0] == opr[n].na){
#ifdef CHECKSUM
			xp->ibs=i;
#else
			xp->ibs=i+1;
#endif
#ifdef NET_DEBUG
			disp_rxib(n);
#endif
			/* test if fifo empty */
#ifdef OPERATOR_PC
// DOOD : inportb->kt_inport8
			a=kt_inport8(base_addr[n] | HDLC_CSR);
#else
			a=XBYTE[HDLC_CSR];
#endif
			if(a & 0x03)  /* if rx fifo not empty */
				ihc(n);
			return FRMGD;
		}

#ifdef NET_DEBUG
		fprintf(net_fp,"ofrx:3 %x\n",n);
#endif

#ifdef DEBUG_ACT
		str2lcd(" ofrx:3 ");
#endif
		ihc(n);
		return FRMERR;
	}
	/* if bad FCS */

#ifdef NET_DEBUG
	fprintf(net_fp,"ofrx:4 %x\n",n);
#endif

#ifdef DEBUG_ACT
	str2lcd(" ofrx:4 ");
#endif
	ihc(n);
	return FRMERR;
}
#endif

void soon(BYTE n){  /* sync out operator night */

#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
#ifdef NEW_DEP
	fpos_t pos;
#endif
#endif
#endif
	OPR_RCRD *op;

#ifdef OPERATOR_PC
	if(n >= card)
		return;
#endif

	op=&opr[n];
	++op->soon;
	if(op->soon >= SYNC_OUT_OPR_NIGHT){
		op->soon=0;
		rst_sgntr=FORCE_RESET_SIGNATURE;
		if(ocr.state != OPRSIDE_OPR_NIGHT){

#ifdef OPERATOR_PC

#ifdef LOG_FILE
			sdt=*get_dt(&sdt);
			fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  network error:101 %x\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se,n);
#else
			fprintf(fer,"network error:101 %x\n",n);
#endif
			initialize();
#ifdef HDLC_8993
			roni(n);
#endif
			init_layo();
#else
			lj0();
#endif
		}

	}
#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
	if(op->soon == SYNC_OUT_ALARM){
		alarm();
#ifdef NEW_DEP
		fgetpos(flog,&pos);
		fseek(flog,0,SEEK_END);
#endif
		fprintf(flog,"O&M Lost Connection with Shelf %d",n+1);
#ifdef NEW_DEP
		fsetpos(flog,&pos);
#endif
	}
#endif
#endif
}

#ifdef HDLC_8993
void onh(BYTE n){  /* operator network handler */
	BYTE a,c;
	int s;

#ifdef OPERATOR_PC
	if(n >= card)
		return;

// DOOD : inportb->kt_inport8
	a=kt_inport8(base_addr[n] | C_CHAN);
#else
	a=XBYTE[C_CHAN];
#endif
	if( (f & DSF) == RESET){  /* if not display status flag set */
		if(a & 0x01)
			led_set(sync_led[n]);
		else
			led_reset(sync_led[n]);
	}

	s=link_state[n];
	/* check state validity */
	if(s >= NO_LINK_STATES){
		rst_sgntr=FORCE_RESET_SIGNATURE;

#ifdef OPERATOR_PC
#ifdef LOG_FILE
		sdt=*get_dt(&sdt);
		fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  network error:106 %x\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se,n);
#else
		fprintf(fer,"network error:106 %x\n",n);
#endif
		initialize();
		roni(n);
		init_layo();
#else
		lj0();
#endif
		return;
	}

	if( (a & 0x01) == RESET){  /* if not sync */
		olrtn_idx[s][NOT_SYNC](n);
		return;
	}
	/* if sync */
	if(olrtn_idx[s][SYNC] != olrtn_error){
		olrtn_idx[s][SYNC](n);
		return;
	}

#ifdef OPERATOR_PC
// DOOD : inportb->kt_inport8
	c=kt_inport8(base_addr[n] | HDLC_CSR);
#else
	c=XBYTE[HDLC_CSR];
#endif
/* 8/2/76, 0x08 seems incorrect, changed to 0x80, also enabling interrupts in ihc() */
	if(c & 0x80){	/* if interrupt */

#ifdef OPERATOR_PC
// DOOD : inportb->kt_inport8
		c=kt_inport8(base_addr[n] | HDLC_ISR);
#else
		c=XBYTE[HDLC_ISR];
#endif
		if(c & 0x40){  /* if EOPD */
			tx[n].n2=0;
			if(ofrx(n) == FRMGD){	/* if good frame */
				a=rx[n].ib[1];
				if( (a & 0x01) == I_C){  /* if I_frame */
					olrtn_idx[s][I_FRAME](n);
					return;
				}
				if( (a & 0x03) == S_C){  /* if S_frame */
					if( (a & 0x0f) == RR)
						olrtn_idx[s][RR_FRAME](n);
					else
#ifdef OPERATOR_PC
						fprintf(fer,"onh error:%x %x\n",s,a)
#endif
						;
					return;
				}
				if( (a & 0x03) == U_C){  /* if U_frame */
					a &= ~P_F;
					/* discard P bit */
					if(a == SABM){
						olrtn_idx[s][SABM_FRAME](n);
						return;
					}
					if(a == UA){
						olrtn_idx[s][UA_FRAME](n);
						return;
					}
#ifdef OPERATOR_PC
					fprintf(fer,"onh error:%x %x\n",s,rx[n].ib[1]);
#endif
					return;
				}
#ifdef OPERATOR_PC
				fprintf(fer,"onh error:%x %x\n",s,a);
#endif
				return;
			}
		}
		else{  /* if not EOPD */
			if(c & 0x05)  /* if TxFun or RxFull */
				ihc(n);
		}
	}
	/* not interrupt,or bad frame */
	olrtn_idx[s][NULL_EVENT](n);
}
#endif

#ifdef HDLC_8993
int ttxq(BYTE *p,BYTE s,BYTE n){  /* put s bytes from p array to tx que */
			   /* if fail return ERR_CODE */
	int i;
	BYTE *a;
	TX_RCRD *xp;
#ifdef OPERATOR_PC
	if(n >= card)
		return ERR_CODE;
#endif
	xp=&tx[n];
	i=xp->qe-xp->qf;
	if(i<0)
		i += TX_QUE_LEN;
#ifdef BATCH_PROGRAMMING
	if( (i+(s+1)) >= (TX_QUE_LEN-1) )  // "size" byte
		return ERR_CODE;
#else
	if( (i+s) >= (TX_QUE_LEN-1) )
		return ERR_CODE;
#endif
	a=xp->q;
#ifdef BATCH_PROGRAMMING
	i=xp->qe;
	a[++i & (TX_QUE_LEN-1)]= s;

	for(;s;s--,p++){
		i= (i + 1) & (TX_QUE_LEN-1);
		a[i]= *p;
	}
#else
	for(i=xp->qe;s;s--,p++){
		i= ++i & (TX_QUE_LEN-1);
		a[i]= *p;
	}
#endif
	xp->qe=i;
	return OK;
}
#endif

int trxq(BYTE n){  /* put from input buffer to rx que */
	     /* if fail return ERR_CODE */
	int i;
	BYTE s;
	BYTE *a,*p;
	RX_RCRD *xp;
	xp=&rx[n];
#ifdef BATCH_PROGRAMMING
	s=xp->ibs-2; /* for address,control & size */
	xp->ib[2]-=3; /* pcktsize=ib[2]-address,control & size */
#else
	s=xp->ibs-3; /* for address,control & size */
#endif
	p=xp->ib;
	if(s <= 0)
		return ERR_CODE;
	i=xp->qe-xp->qf;
	if(i<0)
		i += RX_QUE_LEN;
	if( (i+s) >= (RX_QUE_LEN-1) )
		return ERR_CODE;
	a=xp->q;
	++p;	/* for address byte */
	++p;	/* for control byte */
#ifndef BATCH_PROGRAMMING
	++p;	/* for size byte */
#endif
	for(i=xp->qe;s;s--,p++){
		i= (i + 1) & (RX_QUE_LEN-1);
		a[i]= *p;
	}
	xp->qe=i;
	xp->ibs=0;
	return OK;
}

int pcpmftx(int s,int i,BYTE t){	/* always sent to system A */
/* put cp message for tx,stim. s for index i & type t, */
/* if fail return ERR_CODE */

#ifdef DEBUG_ACTIVE
	pc(2,21);
	print_error_report("\ns=%x  ",s);
	print_error_report("i=%x  ",i);
	print_error_report("t=%x  ",t);
#endif
	s &= 0xff;	/* for system to identify different operators */
	s |=TON<<8;
#ifdef BEHNAMIR_16BIT_PCPMFTX_OPR_TO_SYS
	mb[0]=t | (s<<4);
	mb[1]=s>>4;
	mb[2]=i;
	mb[3]=i>>8;
#else
	mb[0]=t | (i<<4);
	mb[1]=i>>4;
	mb[2]=s;
	mb[3]=s>>8;
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#ifdef CPU_EC512
	return(to_nettx_que(mb,MIN_I_PKT,0));
#else
	return(to_nettx_que(mb,MIN_I_PKT,hot_link));
#endif
#endif
#ifdef HDLC_8993
	return(ttxq(mb,MIN_I_PKT,0));
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	if((hot_link < max_sys_oam_tcp_link) && (tcp_link_st[hot_link].tcp_no < max_tcp_connection)){
		return(put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st),MIN_I_PKT,mb));
	}
	else{
		prnt_string("put_pckt_to_variable_size_buf failed1\n");
		prnt_int(hot_link);prnt_char('-');
		prnt_int(max_sys_oam_tcp_link);prnt_char('-');
		if(hot_link < max_sys_oam_tcp_link){
			prnt_int(tcp_link_st[hot_link].tcp_no);
		}
		prnt_char(CR);
		return FAIL;
	}
#endif

#ifdef MY_TCPIP_FOR_OAM
	return(send_oam_pckt_to_endpoint(hot_link,MIN_I_PKT,mb));
#endif

}

#ifdef LIM
#if(NO_EXTENSION_C != 0)
int pcpmftx2(int s,int i,BYTE t){	 /* always sent to system A */
/* put cp message for tx,stim. s for index i & type t, */
/* if fail return ERR_CODE */

	s &= 0xff;	/* for system to identify different operators */
	s |=TON<<8;
	mb[0]=t | (i<<4);
	mb[1]=i>>4;
	mb[2]=s;
	mb[3]=s>>8;
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#ifdef CPU_EC512
	return(to_nettx_que(mb,MIN_I_PKT,2));
#endif
#endif
#ifdef HDLC_8993
	return(ttxq(mb,MIN_I_PKT,2));
#endif
#if 0
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	return(put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_2].tcp_no].nettx_st),MIN_I_PKT,mb));
#endif
#endif // 0

}
#endif
int pcpmftx1(int s,int i,BYTE t){	 /* always sent to system A */
/* put cp message for tx,stim. s for index i & type t, */
/* if fail return ERR_CODE */

	s &= 0xff;	/* for system to identify different operators */
	s |=TON<<8;
	mb[0]=t | (i<<4);
	mb[1]=i>>4;
	mb[2]=s;
	mb[3]=s>>8;
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#ifdef CPU_EC512
	return(to_nettx_que(mb,MIN_I_PKT,1));
#endif
#endif
#ifdef HDLC_8993
	return(ttxq(mb,MIN_I_PKT,1));
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	if((tcp_link_1 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_1].tcp_no < max_tcp_connection)){
		return(put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_1].tcp_no].nettx_st),MIN_I_PKT,mb));
	}
	else{
		prnt_string("put_pckt_to_variable_size_buf failed2\n");
		prnt_int(hot_link);prnt_char('-');
		prnt_int(max_sys_oam_tcp_link);prnt_char('-');
		if(hot_link < max_sys_oam_tcp_link){
			prnt_int(tcp_link_st[hot_link].tcp_no);
		}
		prnt_char(CR);
		return FAIL;
	}
#endif
}
#endif
/*==========================================================================*/
/*==========================================================================*/
int pcpmftx_both_sys(int s,int i,BYTE t){	 /* always sent to system A */
/* put cp message for tx,stim. s for index i & type t, */
/* if fail return ERR_CODE */

#ifdef DEBUG_ACTIVE
	pc(2,21);
	print_error_report("\ns=%x  ",s);
	print_error_report("i=%x  ",i);
	print_error_report("t=%x  ",t);
#endif
	s &= 0xff;	/* for system to identify different operators */
	s |=TON<<8;
#ifdef BEHNAMIR_16BIT_PCPMFTX_OPR_TO_SYS
	mb[0]=t | (s<<4);
	mb[1]=s>>4;
	mb[2]=i;
	mb[3]=i>>8;
#else
	mb[0]=t | (i<<4);
	mb[1]=i>>4;
	mb[2]=s;
	mb[3]=s>>8;
#endif
#ifdef HOT_STANDBY
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
	to_nettx_que(mb,MIN_I_PKT,LAPB_LINK0);
	to_nettx_que(mb,MIN_I_PKT,LAPB_LINK1);
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	if((tcp_link_0 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_0].tcp_no < max_tcp_connection)){
		put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_0].tcp_no].nettx_st),MIN_I_PKT,mb);
	}
	else{
		prnt_string("put_pckt_to_variable_size_buf failed3\n");
		prnt_int(hot_link);prnt_char('-');
		prnt_int(max_sys_oam_tcp_link);prnt_char('-');
		if(hot_link < max_sys_oam_tcp_link){
			prnt_int(tcp_link_st[hot_link].tcp_no);
		}
		prnt_char(CR);
	}
	if((tcp_link_1 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_1].tcp_no < max_tcp_connection)){
		put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_1].tcp_no].nettx_st),MIN_I_PKT,mb);
	}
	else{
		prnt_string("put_pckt_to_variable_size_buf failed4\n");
		prnt_int(hot_link);prnt_char('-');
		prnt_int(max_sys_oam_tcp_link);prnt_char('-');
		if(hot_link < max_sys_oam_tcp_link){
			prnt_int(tcp_link_st[hot_link].tcp_no);
		}
		prnt_char(CR);
	}
#endif

#ifdef MY_TCPIP_FOR_OAM
	send_oam_pckt_to_endpoint(0,MIN_I_PKT,mb);
	send_oam_pckt_to_endpoint(1,MIN_I_PKT,mb);
#endif
	return OK;
#else
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#ifdef CPU_EC512
	return(to_nettx_que(mb,MIN_I_PKT,0));
#else
	return(to_nettx_que(mb,MIN_I_PKT,hot_link));
#endif
#endif
#ifdef HDLC_8993
	return(ttxq(mb,MIN_I_PKT,0));
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	if((hot_link < max_sys_oam_tcp_link) && (tcp_link_st[hot_link].tcp_no < max_tcp_connection)){
		return(put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st),MIN_I_PKT,mb));
	}
	else{
		prnt_string("put_pckt_to_variable_size_buf failed5\n");
		prnt_int(hot_link);prnt_char('-');
		prnt_int(max_sys_oam_tcp_link);prnt_char('-');
		if(hot_link < max_sys_oam_tcp_link){
			prnt_int(tcp_link_st[hot_link].tcp_no);
		}
		prnt_char(CR);
		return FAIL;
	}
#endif

#ifdef MY_TCPIP_FOR_OAM
	return(send_oam_pckt_to_endpoint(hot_link,MIN_I_PKT,mb));
#endif

#endif
}
/*==========================================================================*/
/*==========================================================================*/
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
int pmamftx_to_direct_link(int j,int i,int t,BYTE n){
#ifdef BEHNAMIR_16BIT_PMAMFTX_OPR_TO_SYS
	j=(j & (FEATURE_ACTIVATION | 0x3f)) | ((t & 0x0f) << 7);
	mb[0]=MA_T | (j<<4);
	mb[1]=j>>4;
	mb[2]=i;
	mb[3]=i>>8;
#else
	mb[0]=MA_T | (j<<4);
	mb[1]=j>>4;
	mb[2]=t | (i<<4);
	mb[3]=i>>4;
#endif
	return(to_nettx_que(mb,MIN_I_PKT,n));
}
#endif
/*==========================================================================*/
/*==========================================================================*/
//----------------
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#pragma warn -par
#endif
//----------------
int pmamftx(int j,int i,int t,BYTE n){

#ifdef DEBUG_ACTIVE
	pc(2,21);
	print_error_report("\nMAC=%x  ",j);
	print_error_report("indx=%d  ",i);
	print_error_report("t=%d  ",t);
	print_error_report("n=%d  ",n);
#endif
	/* put ma message for tx,work j for index i */
	/* & type t to system.If fail return ERR_CODE */
#ifdef BEHNAMIR_16BIT_PMAMFTX_OPR_TO_SYS
	j=(j & (FEATURE_ACTIVATION | 0x3f)) | ((t & 0x0f) << 7);
	mb[0]=MA_T | (j<<4);
	mb[1]=j>>4;
	mb[2]=i;
	mb[3]=i>>8;
#else
	mb[0]=MA_T | (j<<4);
	mb[1]=j>>4;
	mb[2]=t | (i<<4);
	mb[3]=i>>4;
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#ifdef CPU_EC512
	return(to_nettx_que(mb,MIN_I_PKT,n));
#else
	return(to_nettx_que(mb,MIN_I_PKT,hot_link));
#endif
#endif
#ifdef HDLC_8993
	return(ttxq(mb,MIN_I_PKT,n));
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	if((hot_link < max_sys_oam_tcp_link) && (tcp_link_st[hot_link].tcp_no < max_tcp_connection)){

#ifdef DEBUG_ACTIVE
	    {
		unsigned int t;
		struct sockaddr_in rmt_addr;	// connector's address information

		t=tcp_link_st[hot_link].tcp_no;
		rmt_addr=tcp_st[t].remote_addr;

		print_error_report("hot_link=%d ,tcp_no=%d ",hot_link,t);
		print_error_report("remote_ip=%d ",rmt_addr.sin_addr.s_addr);
		print_error_report("remote_port=%d \n",ntohs(rmt_addr.sin_port));
	    }
#endif
	    return(put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st),MIN_I_PKT,mb));
	}
	else{
	    prnt_string("put_pckt_to_variable_size_buf failed6\n");
	    prnt_int(hot_link);prnt_char('-');
	    prnt_int(max_sys_oam_tcp_link);prnt_char('-');
	    if(hot_link < max_sys_oam_tcp_link){
		    prnt_int(tcp_link_st[hot_link].tcp_no);
	    }
	    prnt_char(CR);
	    return FAIL;
	}
#endif

#ifdef MY_TCPIP_FOR_OAM
	return(send_oam_pckt_to_endpoint(hot_link,MIN_I_PKT,mb));
#endif

}
//----------------
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#pragma warn +par
#endif
//----------------
/*==========================================================================*/
//----------------
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#pragma warn -par
#endif
//----------------
int pmamftx_both_sys(int j,int i,int t,BYTE n){

#ifdef DEBUG_ACTIVE
	pc(2,21);
//DOOD : printf->print_error_report
	print_error_report("\nMAC_BOTH=%x  ",j);
//DOOD : printf->print_error_report
	print_error_report("indx=%d  ",i);
//DOOD : printf->print_error_report
	print_error_report("t=%d  ",t);
//DOOD : printf->print_error_report
	print_error_report("n=%d  ",n);
#endif

#ifdef BEHNAMIR_16BIT_PMAMFTX_OPR_TO_SYS
	j=(j & (FEATURE_ACTIVATION | 0x3f)) | ((t & 0x0f) << 7);
	mb[0]=MA_T | (j<<4);
	mb[1]=j>>4;
	mb[2]=i;
	mb[3]=i>>8;
#else
	mb[0]=MA_T | (j<<4);
	mb[1]=j>>4;
	mb[2]=t | (i<<4);
	mb[3]=i>>4;
#endif

#ifdef HOT_STANDBY
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
	to_nettx_que(mb,MIN_I_PKT,LAPB_LINK0);
	to_nettx_que(mb,MIN_I_PKT,LAPB_LINK1);
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	if((tcp_link_0 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_0].tcp_no < max_tcp_connection)){
#ifdef DEBUG_ACTIVE
	    {
		unsigned int t;
		struct sockaddr_in rmt_addr;	// connector's address information

		t=tcp_link_st[tcp_link_0].tcp_no;
		rmt_addr=tcp_st[t].remote_addr;

		print_error_report("tcp_link_0=%d ,tcp_no=%d ",tcp_link_0,t);
		print_error_report("remote_ip=%x ",rmt_addr.sin_addr.s_addr);
		print_error_report("remote_port=%d \n",ntohs(rmt_addr.sin_port));
	    }
#endif
	    put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_0].tcp_no].nettx_st),MIN_I_PKT,mb);
	}
	else{
	    prnt_string("put_pckt_to_variable_size_buf failed7\n");
	    prnt_int(hot_link);prnt_char('-');
	    prnt_int(max_sys_oam_tcp_link);prnt_char('-');
	    if(hot_link < max_sys_oam_tcp_link){
		    prnt_int(tcp_link_st[hot_link].tcp_no);
	    }
	    prnt_char(CR);
	}
	if((tcp_link_1 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_1].tcp_no < max_tcp_connection)){
#ifdef DEBUG_ACTIVE
	    {
		unsigned int t;
		struct sockaddr_in rmt_addr;	// connector's address information

		t=tcp_link_st[tcp_link_1].tcp_no;
		rmt_addr=tcp_st[t].remote_addr;

		print_error_report("tcp_link_1=%d ,tcp_no=%d ",tcp_link_1,t);
		print_error_report("remote_ip=%x ",rmt_addr.sin_addr.s_addr);
		print_error_report("remote_port=%d \n",ntohs(rmt_addr.sin_port));
	    }
#endif
	    put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_1].tcp_no].nettx_st),MIN_I_PKT,mb);
	}
	else{
	    prnt_string("put_pckt_to_variable_size_buf failed8\n");
	    prnt_int(hot_link);prnt_char('-');
	    prnt_int(max_sys_oam_tcp_link);prnt_char('-');
	    if(hot_link < max_sys_oam_tcp_link){
		    prnt_int(tcp_link_st[hot_link].tcp_no);
	    }
	    prnt_char(CR);
	}
#endif

#ifdef MY_TCPIP_FOR_OAM
	send_oam_pckt_to_endpoint(0,MIN_I_PKT,mb);
	send_oam_pckt_to_endpoint(1,MIN_I_PKT,mb);
#endif

	return OK; // to prevent warning
#else // HOT_STANDBY
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#ifdef CPU_EC512
	return(to_nettx_que(mb,MIN_I_PKT,n));
#else
	return(to_nettx_que(mb,MIN_I_PKT,hot_link));
#endif
#endif
#ifdef HDLC_8993
	return(ttxq(mb,MIN_I_PKT,n));
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	if((hot_link < max_sys_oam_tcp_link) && (tcp_link_st[hot_link].tcp_no < max_tcp_connection)){
		return(put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st),MIN_I_PKT,mb));
	}
	else{
		prnt_string("put_pckt_to_variable_size_buf failed9\n");
		prnt_int(hot_link);prnt_char('-');
		prnt_int(max_sys_oam_tcp_link);prnt_char('-');
		if(hot_link < max_sys_oam_tcp_link){
			prnt_int(tcp_link_st[hot_link].tcp_no);
		}
		prnt_char(CR);
		return FAIL;
	}
#endif

#ifdef MY_TCPIP_FOR_OAM
	return(send_oam_pckt_to_endpoint(hot_link,MIN_I_PKT,mb));
#endif

#endif //HOT_STANDBY
}
//----------------
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#pragma warn +par
#endif
//----------------
/*==========================================================================*/

#ifdef HDLC_8993
int tmfrxq(BYTE n){	/* take message from rx que to cp or ma que */
			/* if fail return ERR_CODE */
	int e;
	BYTE f;
	BYTE *a;
	RX_RCRD *xp;
#ifdef BATCH_PROGRAMMING
	BYTE size;
#endif
	xp=&rx[n];
	f=xp->qf;
	e=xp->qe;
	e -= f;
	if(e<0)
		e += RX_QUE_LEN;
	if(e < MIN_I_PKT)
		return ERR_CODE;
	a=xp->q;
#ifdef BATCH_PROGRAMMING
	size=a[++f & (RX_QUE_LEN-1)];
	for(e=0;e < size;e++){
		f= (f + 1) & (RX_QUE_LEN-1);
		mb[e]=a[f];
	}
#else
	for(e=0;e < MIN_I_PKT;e++){
		f= ++f & (RX_QUE_LEN-1);
		mb[e]=a[f];
	}
#endif
	xp->qf=f;
#ifdef NET_DEBUG
	fprintf(net_fp,"\nmb: %x %x %x %x\n",mb[0],mb[1],mb[2],mb[3]);
#endif
	if( (mb[0] & 0x0f) == MA_T)
		mtmaq(n);
	else
#ifdef LIM
		mtcpq(n);
#else
		mtcpq();
#endif
	return OK;
}
#endif

#ifdef LIM
void mtcpq(BYTE n){   /* message from mb to cpq */
#else
void mtcpq(void){   /* message from mb to cpq */
#endif
	INDEX_TYPE a;
	int b;
	a.t=mb[0] & 0x0f;
#ifdef BEHNAMIR
	b=(mb[0]>>4) | (mb[1]<<4);
#else
	a.i=(mb[0]>>4) | (mb[1]<<4);
#endif
#ifdef LIM
#if(NO_EXTENSION_C != 0)
	if(n==2){
		if(a.t==XST_T)
			a.i+=(NO_EXTENSION_A+NO_EXTENSION_B);
	}
	else
#endif
	if(n==1)
		if(a.t==XST_T)
			a.i+=NO_EXTENSION_A;
#endif
#ifdef BEHNAMIR
	a.i=mb[2] | (mb[3]<<8);
#else
	b=mb[2] | (mb[3]<<8);
#endif
	tocpq(a,b);
}

void mtmaq(BYTE n){  /* message from mb to maq */
	INDEX_TYPE w;
	unsigned int p;
	w.t=mb[0] & 0x0f;
	w.i=(mb[0]>>4) | (mb[1]<<4);
	p=mb[2] | (mb[3] << 8);
	tomaq(w,p,n);
}

/* ========================================================================== */
/* ========================================================================== */
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
int to_nettx_que(BYTE *p,BYTE s,BYTE n){  /* put s bytes from p array to tx que */
	BYTE i;
	if ( checkfree_buf(&nettx_st[n]) >= (s+1) ){
		put_buf(&nettx_st[n],s);
#ifdef DEBUG_ACTIVE
		int2lcd(s);
		tolcdq(' ',TO_LCD_DD);
#endif
		for(i=0;i<s;i++){
			put_buf(&nettx_st[n],p[i]);
#ifdef DEBUG_ACTIVE
			int2lcd(p[i]);
			tolcdq(' ',TO_LCD_DD);
#endif
		}
	}
	else{
		 str2lcd(" Err. buf full. ");
	}
	return OK;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
int from_netrx_que(BYTE n){  /* put s bytes from p array to tx que */

	BYTE i,size;
	/* take message from netrx que of nth operator to cp or ma que */
	/* if fail return NULL */

	if  (get_buf(&netrx_st[n],&size) != OK) return FAIL;
#ifdef DEBUG_ACTIVE
		print_error_report("\nn=%x ",n);
		print_error_report("\nsize=%x ",size);
#endif
//	if(size < MIN_I_PKT)
//		return FAIL;
//	  for(i=0;i < MIN_I_PKT;i++){
	if(size < 1)
		return FAIL;
/* never happens
	if(size >= 256){
	    BYTE data;
	    get_buf(&netrx_st[n],&data]);
	    print_error_report("\nfrom_netrx_que n=%d size=%d data=%x",n,size,data);
	    for(i=1;i < size;i++){
		if(get_buf(&netrx_st[n],&data) == FAIL) break;
	    }
	    return FAIL;
	}
*/
	for(i=0;i < size;i++){
		if  (get_buf(&netrx_st[n],&mb[i]) == FAIL)
			return FAIL;
#ifdef DEBUG_ACTIVE
		print_error_report("\nmb=%x ",mb[i]);
#endif
	}
	if( (mb[0] & 0x0f) == MA_T)
		mtmaq(n);
	else
	if( (mb[0] & 0x0f) == TERM_T)
		message_to_term_q(size);
	else
#define AUTH_T	12
	if( (mb[0] & 0x0f) == AUTH_T){
		// ignore it
	}
	else
	if( (mb[0] & 0x0f) == ALARM_T){
#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
#ifdef NEW_DEP
		fpos_t pos;
#endif

		mb[size]=0;
#ifdef NEW_DEP
		fgetpos(flog,&pos);
		fseek(flog,0,SEEK_END);
#endif
		fprintf(flog,"%s",&mb[1]);
#ifdef DISPLAY_ALARM
		show_alarm_without_date("%s",&mb[1]);
#endif
#ifdef NEW_DEP
		fsetpos(flog,&pos);
#endif
#endif
#endif
	}
	else
#if 0
	if( (mb[0] & 0x0f) == FTP_T)
		handle_ftp(size);
	else
#endif //0
#ifdef LIM
		mtcpq(n);
#else
		mtcpq();
#endif
	return OK;
}
#endif

/* ========================================================================== */
/* ========================================================================== */
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
void oam_opr_rcv_hndlr(void){

	BYTE i;
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	unsigned int tcp_no,size;
	BYTE dat[1024];
#endif
#ifdef MY_UDP_FOR_OAM
	for(i=FIRST_UDP_HDLC_CH;i<LAST_UDP_HDLC_CH;i++){
		while(from_netrx_que(i) == OK);
	}
#endif
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
	for(i=FIRST_RS232_HDLC_CH;i<LAST_RS232_HDLC_CH;i++){
		from_netrx_que(i);
	}
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
	for(i=0;i<max_sys_oam_tcp_link;i++){
		tcp_no=tcp_link_st[i].tcp_no;
		if(tcp_no < max_tcp_connection){
			while(get_pckt_from_variable_size_buf(&(tcp_st[tcp_no].netrx_st),&size,&dat[0]) == OK){
				if(size >= MAX_OPR_SYS_PCKT_SIZE+1){
					prnt_msg(TCP_ERR,0x2301);
					prnt_int(size);prnt_char(CR);
					continue;
				}
				memcpy(mb,dat,size);
				if( (mb[0] & 0x0f) == MA_T)
					mtmaq(i);
				else
					if( (mb[0] & 0x0f) == TERM_T)
						message_to_term_q(size);
					else
						if( (mb[0] & 0x0f) == FTP_T)
							handle_ftp(size);
						else
#ifdef LIM
							mtcpq(i);
#else
				mtcpq();
#endif
			}
		}
	}
#endif
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef HDLC_8993
#ifdef NET_DEBUG
void disp_rxib(BYTE n){
	BYTE i,s;
	RX_RCRD *xp;
	xp=&rx[n];
	s=xp->ibs;

#ifdef OPERATOR_PC
	fprintf(net_fp,"r=%x ",s);
#endif

#ifdef DEBUG_ACT
	tolcdq('r',TO_LCD_DD);
	tolcdq('=',TO_LCD_DD);
	hex2lcd(s);
	tolcdq(0x20,TO_LCD_DD);
#endif
	for(i=0;i<s;i++){

#ifdef DEBUG_ACT
		hex2lcd(xp->ib[i]);
#endif

#ifdef OPERATOR_PC
		fprintf(net_fp,"%x ",xp->ib[i]);
#endif
	}

#ifdef OPERATOR_PC
	fprintf(net_fp," w.nfts=%x,w.fe=%x\n",w[n].nfts,w[n].fe);
#endif
}
#endif
#endif

int smaf(void){  /* send maintenance & administration frame to system */
	int j;
	int h;
	BYTE t;
#ifdef HOT_STANDBY
	BYTE both_sys_flag;
	BYTE	err;
#endif

	if( (ocr.s.t == NULL_T) || (ocr.d.t == NULL_T) )
		return ERR_CODE;
	j=ocr.s.i & 0x3f;
	t=ocr.d.t;
#ifdef HOT_STANDBY
	both_sys_flag=RESET;
#endif
	switch(t){
		case	XST_T:	j |= MA_X_P;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
				if (ocr.d.i >= (NO_EXTENSION_A+NO_EXTENSION_B)){
					ocr.d.i -= (NO_EXTENSION_A+NO_EXTENSION_B);
					h=2;
				}
				else
#endif
				if (ocr.d.i >= NO_EXTENSION_A){
					ocr.d.i -= NO_EXTENSION_A;
					h=1;
				}
				else
#endif
					h=0;
				break;
		case	 CO_T:	j |= MA_COT_P;
				h=0;
				break;
		case	TIE_T:	j |= MA_TIET_P;
				h=0;
				break;
		case	EM_T:	j |= MA_EM_CAS_P;
				h=0;
				break;
#ifdef CAS1_INSTALLED
		case	CAS1_T: j |= MA_EM_CAS_P;
				h=0;
				break;
#endif
#ifdef CAS3_INSTALLED
		case	CAS3_T: j |= MA_EM_CAS_P;
				h=0;
				break;
#endif
		case	OPR_T:	j |= MA_SYS_P;
				switch(ocr.s.i){
#ifdef ON_LINE_BILLING_ACT
					case	MAC_ONLINE_BILLING_ACT:
#ifdef HOT_STANDBY
						both_sys_flag=SET;
#endif
						h=3;
						break;
#endif
#ifdef PARAMETER_DOWN_UP_LOAD
					case	MAC_DOWN_LOAD_PARAMETERS_1:
#ifdef HOT_STANDBY
						both_sys_flag=RESET;
#endif
						h=0;
						break;
//it seems that it is not needed here.
					case	MAC_UP_LOAD_PARAMETERS_1:
#ifdef HOT_STANDBY
						both_sys_flag=SET;
#endif
						h=0;
						break;
#endif
#ifdef PRE_INSTALL
					case	MAC_ANALOG_TRUNK_GAIN:
					case	MAC_EM_TRUNK_GAIN:
#ifdef HOT_STANDBY
						both_sys_flag=SET;
#endif
						h=0;
						break;
#endif
						/* system A related */


					case	MAC_TRUNK_STATE_PRINT:
#ifdef HOT_STANDBY
//if swch over is happenend,standby system is also aware.
						both_sys_flag=SET;
#endif
						h=3;
						break;
					case	MAC_EXT_STATE_PRINT:
#ifdef HOT_STANDBY
//if swch over is happenend,standby sytem is also aware.
						both_sys_flag=SET;
#endif
						h=3;
						break;
#ifdef CONFERENCE_ACT
					case	MAC_CONFERENCE_STATE_PRINT:
#ifdef HOT_STANDBY
//if swch over is happenend,standby sytem is also aware.
						both_sys_flag=SET;
#endif
						h=3;
						break;
#endif
#ifdef PRE_INSTALL
					case	MAC_EXT_CODEC_GAIN:
#ifdef HOT_STANDBY
						both_sys_flag=SET;
#endif
						h=3;
						break;
#endif
#ifdef EEPROM
					case	MAC_SET_DEFAULT_PARAM:
#ifdef HOT_STANDBY
						both_sys_flag=RESET;
#endif
						h=3;
						break;
#endif

#ifdef PARAMETER_DOWN_UP_LOAD
#if(NO_EXTENSION_C != 0)
					case	MAC_DOWN_LOAD_PARAMETERS_3:
						/* system C related */
						j=MAC_DOWN_LOAD_PARAMETERS | MA_SYS_P;
						h=2;
						break;
#endif
					case	MAC_DOWN_LOAD_PARAMETERS_2:
						/* system B related */
						j=MAC_DOWN_LOAD_PARAMETERS | MA_SYS_P;
						h=1;
						break;
#endif

#ifdef SYSTEM_MANUAL_TEST
					case	MAC_EXT_MANUAL_TEST:
#ifdef HOT_STANDBY
						both_sys_flag=SET;
#endif
#ifdef LIM
#if(NO_EXTENSION_C != 0)
						if (ocr.d.i >= (NO_EXTENSION_A+NO_EXTENSION_B)){
							ocr.d.i -= (NO_EXTENSION_A+NO_EXTENSION_B);
							h=2;
						}
						else
#endif
						if(ocr.d.i >= NO_EXTENSION_A){
							ocr.d.i -= NO_EXTENSION_A;
							h=1;
						}
						else
#endif
							h=0;
						j |= ((ocr.s.t & 0x08) << 6);
						break;
#endif

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

					case	MAC_CAS3_HISTORICAL_LOOSE_REQUEST:
					case	MAC_CAS3_HISTORICAL_REQUEST:
#ifdef HOT_STANDBY
						both_sys_flag=RESET;
#endif
						h=0;
						break;
#endif
#endif
#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

					case	MAC_EM_HISTORICAL_REQUEST:
#ifdef HOT_STANDBY
						both_sys_flag=RESET;
#endif
						h=0;
						break;
#endif
#endif
#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

					case	MAC_CAS1_HISTORICAL_REQUEST:
#ifdef HOT_STANDBY
						both_sys_flag=RESET;
#endif
						h=0;
						break;
#endif
#endif


#ifdef R1_SIGNALINGP

					case	MAC_OFFICE_CODE_PART1:
					case	MAC_OFFICE_CODE_PART2:
#ifdef HOT_STANDBY
						both_sys_flag=SET;
#endif
						h=3;
						break;
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
					case	MAC_EXIT_PROGRAM:
#ifdef HOT_STANDBY
						if (ocr.d.i == 0){ // STANDBY
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
							hot_link=(hot_link == LAPB_LINK0) ? LAPB_LINK1 : LAPB_LINK0;
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
							hot_link=(hot_link == tcp_link_0) ? tcp_link_1 : tcp_link_0;
#endif
#ifdef MY_TCPIP_FOR_OAM
							hot_link=(hot_link == 0) ? 1 : 0;
#endif
							err=pmamftx(MA_SYS_P | MAC_EXIT_PROGRAM,TON,OPR_T,0);
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
							hot_link=(hot_link == LAPB_LINK0) ? LAPB_LINK1 : LAPB_LINK0;
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
							hot_link=(hot_link == tcp_link_0) ? tcp_link_1 : tcp_link_0;
#endif
#ifdef MY_TCPIP_FOR_OAM
							hot_link=(hot_link == 0) ? 1 : 0;
#endif
							return(err);
						}
						else if (ocr.d.i == 1){ // HOT
							return(pmamftx(MA_SYS_P | MAC_EXIT_PROGRAM,TON,OPR_T,0));
						}
						else return ERR_CODE;
#else
						if (ocr.d.i == 1){ // HOT
							return(pmamftx(MA_SYS_P | MAC_EXIT_PROGRAM,TON,OPR_T,0));
						}
						else return ERR_CODE;
#endif
#endif
					case MAC_SS7_PACKET_PRINT   :
#ifdef HOT_STANDBY
						both_sys_flag=RESET;
#endif
						h=0;
						break;
#if ((defined EC2K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PU2K_SYSTEM))
					case MAC_SWITCHOVER_PROGRAM :
						return(pmamftx(MA_SYS_P | ocr.s.i,TON,OPR_T,0));
#endif
					default: return ERR_CODE;
				}
				break;
		default: return ERR_CODE;
	}
	if(ocr.s.t == SET)
		j |= 0x800;

#ifdef PC_COMPATIBLE
#ifdef HOT_STANDBY
	if (both_sys_flag == SET){
		return(pmamftx_both_sys(j,ocr.d.i,t,0));
	}
	else
#endif // HOT_STANDBY
	{
		return(pmamftx(j,ocr.d.i,t,0));
	}
#else

	if(h==3){
		if (pmamftx(j,ocr.d.i,t,0) == OK){
#ifdef LIM
			if (pmamftx(j,ocr.d.i,t,1) == OK){
#if(NO_EXTENSION_C != 0)
				if (pmamftx(j,ocr.d.i,t,2) == OK)
					return OK;
				else
					return ERR_CODE;
#else
				return OK;
#endif
			}
			else{
#if(NO_EXTENSION_C != 0)
				pmamftx(j,ocr.d.i,t,2);
#endif
				return ERR_CODE;
			}
#else	// LIM
			return OK;
#endif	// LIM
		}
		else{
#ifdef LIM
			pmamftx(j,ocr.d.i,t,1);
#if(NO_EXTENSION_C != 0)
			pmamftx(j,ocr.d.i,t,2);
#endif
#endif
			return ERR_CODE;
		}
	}
	else{
		return(pmamftx(j,ocr.d.i,t,h));
	}
#endif
}

/**** operator link routines ****/

#ifdef HDLC_8993
void olrtn_init(void){
	int i,j;
	for(i=0;i < NO_LINK_STATES;i++)
		for(j=0;j < NO_LINK_EVENTS;j++)
			olrtn_idx[i][j]=olrtn_error;

	olrtn_idx[RESET_NOT_SYNC_LINK_NOT_AVAIL][NOT_SYNC]=olrtn1;
	olrtn_idx[RESET_NOT_SYNC_LINK_NOT_AVAIL][SYNC]=olrtn2;
	olrtn_idx[RESET_NOT_SYNC_LINK_NOT_AVAIL][NULL_EVENT]=(FPNH)olrtn0;

	olrtn_idx[RESET_SYNC_LINK_NOT_AVAIL][SYNC]=olrtn3;
	olrtn_idx[RESET_SYNC_LINK_NOT_AVAIL][NOT_SYNC]=olrtn4;
	olrtn_idx[RESET_SYNC_LINK_NOT_AVAIL][NULL_EVENT]=(FPNH)olrtn0;

	olrtn_idx[RESET_SYNC_LINK_AVAIL][NULL_EVENT]=olrtn5;
	olrtn_idx[RESET_SYNC_LINK_AVAIL][NOT_SYNC]=olrtn4;
	olrtn_idx[RESET_SYNC_LINK_AVAIL][SABM_FRAME]=olrtn6;
	olrtn_idx[RESET_SYNC_LINK_AVAIL][UA_FRAME]=olrtn7;
	olrtn_idx[RESET_SYNC_LINK_AVAIL][I_FRAME]=(FPNH)olrtn0;
	olrtn_idx[RESET_SYNC_LINK_AVAIL][RR_FRAME]=(FPNH)olrtn0;

	olrtn_idx[INFORMATION_TRANSFER][NOT_SYNC]=olrtn8;
	olrtn_idx[INFORMATION_TRANSFER][NULL_EVENT]=olrtn9;
	olrtn_idx[INFORMATION_TRANSFER][UA_FRAME]=(FPNH)olrtn0;
	olrtn_idx[INFORMATION_TRANSFER][I_FRAME]=olrtn10;
	olrtn_idx[INFORMATION_TRANSFER][RR_FRAME]=olrtn11;
	olrtn_idx[INFORMATION_TRANSFER][SABM_FRAME]=olrtn6;

	olrtn_idx[NOT_SYNC_INFORMATION_TRANSFER][SYNC]=olrtn12;
	olrtn_idx[NOT_SYNC_INFORMATION_TRANSFER][NOT_SYNC]=olrtn13;
	olrtn_idx[NOT_SYNC_INFORMATION_TRANSFER][NULL_EVENT]=(FPNH)olrtn0;
}
#endif

#ifdef HDLC_8993
void olrtn_error(BYTE n){

#ifdef OPERATOR_PC
#ifdef LOG_FILE
	sdt=*get_dt(&sdt);
	fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  network error:105 %x\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se,link_state[n]);
#else
	fprintf(fer,"network error:105,%x\n",link_state[n]);
#endif
#endif
}
#endif

#ifdef HDLC_8993
void olrtn0(){
}
#endif

#ifdef HDLC_8993
void olrtn1(BYTE n){
	OPR_RCRD *op;

#ifdef DEBUG_ACT
/*
	str2lcd(" olr1 ");
*/
#endif
#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif
	op=&opr[n];
	if( (++op->srpc) >= SYNC_RESET_THRESHOLD){
		op->srpc=0;
		if( (++op->snipc) >= SYS_NETWORK_INIT_THRESHOLD){

#ifdef OPERATOR_PC
#ifdef LOG_FILE
			sdt=*get_dt(&sdt);
			fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  network error:100 %x\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se,n);
#else
			fprintf(fer,"network error:100\n");
#endif
#endif
			ihp(n);
		}
		else
			ihc(n);
	}
	soon(n);
	led_set(OPR_RESET_LED);
}
#endif

#ifdef HDLC_8993
void olrtn2(BYTE n){

#ifdef DEBUG_ACT
	str2lcd(" olr2 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr2\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	link_state[n]=RESET_SYNC_LINK_NOT_AVAIL;
	led_set(OPR_RESET_LED);
}
#endif

#ifdef HDLC_8993
void olrtn3(BYTE n){
	OPR_RCRD *op;

#ifdef DEBUG_ACT
/*
	str2lcd(" olr3 ");
*/
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	op=&opr[n];
	if( (++op->sspc) >= SYNC_SET_THRESHOLD){
		op->sspc=0;
		link_state[n]=RESET_SYNC_LINK_AVAIL;
	}
	led_set(OPR_RESET_LED);
}
#endif

#ifdef HDLC_8993
void olrtn4(BYTE n){

#ifdef DEBUG_ACT
	str2lcd(" olr4 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr4\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	opr[n].soon=0;
	link_state[n]=RESET_NOT_SYNC_LINK_NOT_AVAIL;
	led_set(OPR_RESET_LED);
}
#endif

#ifdef HDLC_8993
void olrtn5(BYTE n){
	  OPR_RCRD *op;

#ifdef DEBUG_ACT
	str2lcd(" olr5 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr5\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	op=&opr[n];
	if( (++op->sspc) >= LINK_DOWN_THRESHOLD){
		rst_sgntr=FORCE_RESET_SIGNATURE;
		op->sspc=0;
		if(ocr.state != OPRSIDE_OPR_NIGHT){

#ifdef OPERATOR_PC
#ifdef LOG_FILE
			sdt=*get_dt(&sdt);
			fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  network error:102\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se);
#else
			fprintf(fer,"network error:102\n");
#endif
			initialize();
			roni(n);
			init_layo();
#else
			lj0();
#endif
		}
	}
	else
		txsuf(SABM | P_F,n);
	led_reset(OPR_RESET_LED);
}
#endif

#ifdef HDLC_8993
void olrtn6(BYTE n){


#ifdef DEBUG_ACT
	str2lcd(" olr6 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr6\n");
#endif

	rst_sgntr=FORCE_RESET_SIGNATURE;

#ifdef OPERATOR_PC
	if(n >= card)
		  return;

	initialize();
	roni(n);
	init_layo();
	send_fdn();
#else
	lj0();
#endif
	txsuf(UA | P_F,n);
	link_state[n]=INFORMATION_TRANSFER;

#ifdef OPERATOR_FARSI
	if(f & OPR_LANG_FLAG)
		str2lcd(" –¨ ¤¤ì¤“ ô—¨þ¨ ‘“ ¯‘“—¤ ");
	else
#endif
		str2lcd("LINK UP ");
	led_reset(OPR_RESET_LED);
	if( (f & DSF) == RESET)  /* if display status inactive */
		led_set(link_up_led[n]);
}
#endif

#ifdef HDLC_8993
void olrtn7(BYTE n){

#ifdef DEBUG_ACT
	str2lcd(" olr7 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr7\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	ihp(n); /* reset HDLC controller */

#ifdef OPERATOR_FARSI
	if(f & OPR_LANG_FLAG)
		str2lcd(" –¨ ¤¤ì¤“ ô—¨þ¨ ‘“ ¯‘“—¤ ");
	else
#endif
		str2lcd("LINK UP ");
	send_fdn();
	link_state[n]=INFORMATION_TRANSFER;
	led_reset(OPR_RESET_LED);
	if( (f & DSF) == RESET)  /* if display status inactive */
		led_set(link_up_led[n]);
}
#endif

#ifdef HDLC_8993
void olrtn8(BYTE n){

#ifdef DEBUG_ACT
	str2lcd(" olr8 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr8\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	opr[n].soon=0;
	link_state[n]=NOT_SYNC_INFORMATION_TRANSFER;
	if( (f & DSF) == RESET)  /* if display status inactive */
		led_set(link_up_led[n]);
}
#endif

#ifdef HDLC_8993
void olrtn9(BYTE n){
	TX_RCRD *xp;

#ifdef DEBUG_ACT
/*
	str2lcd(" olr9 ");
*/
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr9\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif
	xp=&tx[n];
	if(xp->obs){  /* if tx. output buffer not empty */
		if( (++xp->n2) >= N2_THRESHOLD){
			xp->n2=0;
// probably better do s.th. more than ihc and remove current packet from buffer
// as it seems it can not be sent!
			ihc(n);
#ifdef OPERATOR_PC
#ifdef LOG_FILE
			sdt=*get_dt(&sdt);
			fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  network error:103\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se);
#else
			fprintf(fer,"network error:103\n");
#endif
#endif
		}
		txf(n);
	}
	else  /* if tx. output buffer empty */
		if(miftx(n) == OK)
			txf(n);
	if( (f & DSF) == RESET)  /* if display status inactive */
		led_set(link_up_led[n]);
}
#endif

#ifdef HDLC_8993
void olrtn10(BYTE n){
	BYTE a;
	RX_RCRD *rxp;

#ifdef DEBUG_ACT
	str2lcd(" olr10 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr10\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	rxp=&rx[n];
	if( (f & DSF) == RESET)  /* if display status inactive */
		led_set(link_up_led[n]);

	if(rxp->ib[2] != rxp->ibs){  /* if size not correct */
		ihc(n);
		if(tx[n].obs)  /* if tx. output buffer not empty */
			txf(n);
		else
			if(miftx(n) == OK)
				txf(n);
		return;
	}
	a=rxp->ib[1];
	if( ( (a & 0xe0) >> 4) == w[n].nfts)  /* if valid N(R) */
		tx[n].obs=0;  /* release tx. output buffer */

	if( ( (a & 0x0e) << 4) == w[n].fe){  /* if valid N(S) */
		if(trxq(n) != OK){  /* if rx. queue full */
			while(tmfrxq(n) == OK);
			trxq(n);
		}

		if(w[n].fe)  /* inc. fe */
			w[n].fe=0;
		else
			w[n].fe=FRM1_EX;
	}
	if(tx[n].obs)  /* if tx. output buffer not empty */
		txf(n);
	else{
		if(miftx(n) == OK)
			txf(n);
	       else
			txsuf(RR | w[n].fe | P_F,n);
	}
	while(tmfrxq(n) == OK);
}
#endif

#ifdef HDLC_8993
void olrtn11(BYTE n){
	BYTE a;

#ifdef DEBUG_ACT
	str2lcd(" olr11 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr11\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	a=rx[n].ib[1];
	if( ( (a & 0xe0) >> 4) == w[n].nfts){  /* if valid N(R) */
		tx[n].obs=0;
		if(miftx(n) == OK)
			txf(n);
	}
	else{  /* invalid N(R) */
		if(tx[n].obs)
			txf(n);
	       else
#ifdef OPERATOR_PC
#ifdef LOG_FILE
			sdt=*get_dt(&sdt);
			fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  network error:104\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se);
#else
			fprintf(fer,"network error:104\n")
#endif
#endif
			;
	}
	if( (f & DSF) == RESET)  /* if display status inactive */
		led_set(link_up_led[n]);
}
#endif

#ifdef HDLC_8993
void olrtn12(BYTE n){
	OPR_RCRD *op;

#ifdef DEBUG_ACT
/*
	str2lcd(" olr12 ");
*/
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	op=&opr[n];
	if( (++op->sspc) >= SYNC_SET_THRESHOLD){
		op->sspc=0;
		link_state[n]=INFORMATION_TRANSFER;
	}
	if( (f & DSF) == RESET)  /* if display status inactive */
		led_set(link_up_led[n]);
}
#endif

#ifdef HDLC_8993
void olrtn13(BYTE n){

#ifdef DEBUG_ACT
	str2lcd(" olr13 ");
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"\nolr13\n");
#endif

#ifdef OPERATOR_PC
	if(n >= card)
		  return;
#endif

	if( (f & DSF) == RESET)  /* if display status inactive */
		led_set(link_up_led[n]);
	soon(n);
}
#endif
