                      /*****   OV.H   *****/
unsigned int f;

//CPR cpq[CP_QUE_LEN];
unsigned short cpfq,cpeq;

OMAR maq[MA_QUE_LEN];
unsigned short mafq,maeq;

unsigned int time_tab[16];
          /* time table used by scheduler */
BYTE pph;
          /* present phase is used to index time table */
//FPCP rtn_idx[OPRSIDE_NO_STATES][OPRSIDE_NO_STIMS];
/* routine index table contains the pointer of call processing routines */

unsigned int tptw; /* This phase task word. */

OPR_RCRD opr[MAX_NO_OPR_CARD];
//TX_RCRD tx[MAX_NO_OPR_CARD];
//RX_RCRD rx[MAX_NO_OPR_CARD];
TRW w[MAX_NO_OPR_CARD];
//BYTE mb[MIN_I_PKT];  /* message buffer for tx & rx HDLC */
//BYTE mb[MAX_OPR_SYS_PCKT_SIZE+1];  /* message buffer for tx & rx HDLC */
// in from_netrx_que, for cases of AUTH_T and ALARM_T, size may be big
BYTE mb[256];  /* message buffer for tx & rx HDLC */


STAT_IN_OPR xst[NO_TOTAL_EXTENSION];

#ifdef OPR_OPR_TALK
STAT_IN_OPR oprt[NO_OPRTR];
#endif

#if (NO_CO != 0)
STAT_IN_OPR ctst[NO_CO];  /* CO trunk status table */
#endif


#ifndef NO_TIE_INIT
STAT_IN_OPR ttst[NO_TIE];  /* TIE trunk status table */
#endif


#ifdef OPR_EM_CONNECTION
STAT_IN_OPR emtst[NO_EM];  /* E&M trunk status table */
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
STAT_IN_OPR dtcst[NO_CAS3_CHANNEL]; /* CAS3 channel status table */
#endif


OPR_CALL_REC ocr;

IN_QUE_REC iqr[IQR_NO];  /* internal queue records */

RE_EX_QUE_REC rxqr[RXQR_NO];  /* recall & external queue records */


INDEX_TYPE pl[10];  /* park location */
INDEX_TYPE hld,hldl[NO_HOLD_KEY];

#ifdef OPERATOR_PC
BYTE hkl[NO_HOLD_KEY];  /* hold key's LED */
#ifdef HDLC_8993
BYTE sync_led[MAX_NO_OPR_CARD]={SYNC_LED1,SYNC_LED2,SYNC_LED3};
BYTE link_up_led[MAX_NO_OPR_CARD]={LINK_UP_LED1,LINK_UP_LED2,LINK_UP_LED3};
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
//oam<->hot,oam<->standby,hot_standby
//#ifdef HOT_STANDBY
BYTE sync_led[/*NO_LAPB_CH+1*/ 3]={SYNC_LED1,SYNC_LED2,SYNC_LED3};
BYTE link_up_led[/*NO_LAPB_CH+1*/ 3]={LINK_UP_LED1,LINK_UP_LED2,LINK_UP_LED3};

#ifdef DYNAMIC_IP_ALLOCATION
unsigned int ms_button[NO_HDLC_UDP_PORT]; // master-button=0, slave-button=1 it has changed to array because of wbill.exe
#endif

//#else
//BYTE sync_led[1]={SYNC_LED1};
//BYTE link_up_led[1]={LINK_UP_LED1};

//#endif
#endif
#else // OPERATOR_PC
BYTE hkl; /* hold key's LED */
#endif

BYTE fqi,eqi,fqr,eqr,fqx,eqx;
BYTE pqi,shown_pqi,pqx,shown_pqx,last_shown_pq,no_iqr;
BYTE firf,fire;  /* free internal records front & end */
BYTE frxf,frxe;  /* free recall & external records front & end */
#ifdef EM_CONT_SEP_OPR_Q
BYTE fqem,eqem;
#endif

#define NO_OPERATION 0xfe
#ifdef OPERATOR_PC
#ifndef OPR_EM_CONNECTION
#ifdef OPR_PRI_CONNECTION
#ifdef OPR_SIP_CONNECTION
BYTE kctt[NO_KEYS]={
          CO_GRP1,CO_GRP2,CO_GRP3,CO_GRP4,CO_GRP5,TIE_GRP1,TIE_GRP2,TIE_GRP3,
          TIE_GRP4,SIP_REQ,NIGHT,PROG,OPR_HOOK,VOLUME,TIME,CO_GRP6,
          RECALL,EXTRNL,INTERNAL,ATT_RLS,PRI_GRP1,SRC,DEST,OVRD_KEY,
          TONE_RNGR,HOLD1,HOLD2,HOLD3,HOLD4,PARK_XT,0xff,NO_OPERATION,
          DISP_STATUS,RLS,N_1,N_2,N_3,N_4,N_5,N_6,
          N_7,N_8,N_9,STAR,N_0,HASH,TOG_LANGUAGE,SHELL,
          NO_OPERATION,NO_OPERATION,NO_OPERATION,DISP_CAS1_STATUS,DISP_CAS3_STATUS,EXIT_MANUAL_TEST,MANUAL_TEST,DISP_TRUNK_STATUS
          ,DEP_ACT,BILLING_ACT,ALARM_ACT,TERM_ACT
          };
#else
BYTE kctt[NO_KEYS]={
          CO_GRP1,CO_GRP2,CO_GRP3,CO_GRP4,CO_GRP5,TIE_GRP1,TIE_GRP2,TIE_GRP3,
          TIE_GRP4,TIE_GRP5,NIGHT,PROG,OPR_HOOK,VOLUME,TIME,CO_GRP6,
          RECALL,EXTRNL,INTERNAL,ATT_RLS,PRI_GRP1,SRC,DEST,OVRD_KEY,
          TONE_RNGR,HOLD1,HOLD2,HOLD3,HOLD4,PARK_XT,0xff,NO_OPERATION,
          DISP_STATUS,RLS,N_1,N_2,N_3,N_4,N_5,N_6,
          N_7,N_8,N_9,STAR,N_0,HASH,TOG_LANGUAGE,SHELL,
          NO_OPERATION,NO_OPERATION,NO_OPERATION,DISP_CAS1_STATUS,DISP_CAS3_STATUS,EXIT_MANUAL_TEST,MANUAL_TEST,DISP_TRUNK_STATUS
          ,DEP_ACT,BILLING_ACT,ALARM_ACT,TERM_ACT
          };
#endif // OPR_SIP_CONNECTION
#else // OPR_PRI_CONNECTION
BYTE kctt[NO_KEYS]={
          CO_GRP1,CO_GRP2,CO_GRP3,CO_GRP4,CO_GRP5,TIE_GRP1,TIE_GRP2,TIE_GRP3,
          TIE_GRP4,TIE_GRP5,NIGHT,PROG,OPR_HOOK,VOLUME,TIME,CO_GRP6,
          RECALL,EXTRNL,INTERNAL,ATT_RLS,CAS3_GRP1,SRC,DEST,OVRD_KEY,
          TONE_RNGR,HOLD1,HOLD2,HOLD3,HOLD4,PARK_XT,0xff,NO_OPERATION,
          DISP_STATUS,RLS,N_1,N_2,N_3,N_4,N_5,N_6,
          N_7,N_8,N_9,STAR,N_0,HASH,TOG_LANGUAGE,SHELL,
          NO_OPERATION,NO_OPERATION,NO_OPERATION,DISP_CAS1_STATUS,DISP_CAS3_STATUS,EXIT_MANUAL_TEST,MANUAL_TEST,DISP_TRUNK_STATUS
          ,DEP_ACT,BILLING_ACT,ALARM_ACT,TERM_ACT
          };
#endif // OPR_PRI_CONNECTION
#else   /* OPR_EM_CONNECTION */
#ifdef EM_CONT_SEP_OPR_Q
BYTE kctt[NO_KEYS]={
/* 00 */  CO_GRP1,CO_GRP2,CO_GRP3,CO_GRP4,CO_GRP5,EM_GRP1,EM_GRP2,EM_GRP3,
/* 08 */  EM_GRP4,EM_GRP5,NIGHT,PROG,OPR_HOOK,VOLUME,TIME,EM_Q,
/* 16 */  RECALL,EXTRNL,INTERNAL,ATT_RLS,EM_GRP6,SRC,DEST,OVRD_KEY,
/* 24 */  TONE_RNGR,HOLD1,HOLD2,HOLD3,HOLD4,PARK_XT,0xff,NO_OPERATION,
/* 32 */  DISP_STATUS,RLS,N_1,N_2,N_3,N_4,N_5,N_6,
/* 40 */  N_7,N_8,N_9,STAR,N_0,HASH,TOG_LANGUAGE,SHELL,
/* 48 */  EM_GRP7,EM_GRP8,EM_GRP9,DISP_CAS1_STATUS,DISP_CAS3_STATUS,EXIT_MANUAL_TEST,MANUAL_TEST,DISP_TRUNK_STATUS
/* 56 */  ,DEP_ACT,BILLING_ACT,ALARM_ACT,TERM_ACT
          };
#else   /* EM_CONT_SEP_OPR_Q */
BYTE kctt[NO_KEYS]={
          CO_GRP1,CO_GRP2,CO_GRP3,CO_GRP4,CO_GRP5,EM_GRP1,EM_GRP2,EM_GRP3,
          EM_GRP4,EM_GRP5,NIGHT,PROG,OPR_HOOK,VOLUME,TIME,TIE_GRP1,
          RECALL,EXTRNL,INTERNAL,ATT_RLS,EM_GRP6,SRC,DEST,OVRD_KEY,
          TONE_RNGR,HOLD1,HOLD2,HOLD3,HOLD4,PARK_XT,0xff,NO_OPERATION,
          DISP_STATUS,RLS,N_1,N_2,N_3,N_4,N_5,N_6,
          N_7,N_8,N_9,STAR,N_0,HASH,TOG_LANGUAGE,SHELL,
          NO_OPERATION,NO_OPERATION,NO_OPERATION,DISP_CAS1_STATUS,DISP_CAS3_STATUS,EXIT_MANUAL_TEST,MANUAL_TEST,DISP_TRUNK_STATUS
          ,DEP_ACT,BILLING_ACT,ALARM_ACT,TERM_ACT
          };
#endif  /* EM_CONT_SEP_OPR_Q */
#endif  /* OPR_EM_CONNECTION */

#else   /* OPERATOR_PC */

#ifdef PC_KEYBOARD


BYTE kctt[0x100];


#else              /* PC_KEYBOARD */


#ifndef OPR_EM_CONNECTION
BYTE kctt[NO_KEYS]={
          NULL,
          N_1     ,N_4     ,N_7      ,STAR      ,NULL,NULL,NULL,
          N_2     ,N_5     ,N_8      ,N_0       ,NULL,NULL,NULL,
          N_3     ,N_6     ,N_9      ,HASH      ,NULL,NULL,NULL,
          CO_GRP1 ,TIE_GRP1,NIGHT    ,ATT_RLS   ,NULL,NULL,NULL,
          CO_GRP2 ,TIE_GRP2,PROG     ,RLS       ,NULL,NULL,NULL,
          RECALL  ,SRC     ,HOLD1    ,OPR_HOOK  ,NULL,NULL,NULL,
          EXTRNL  ,DEST    ,TONE_RNGR,SRL_TRNSFR,NULL,NULL,NULL,
//?????
          INTERNAL,OPRSIDE_OVRD    ,PARK     ,NULL      ,NULL,NULL
          };
#else   /* OPR_EM_CONNECTION */

#ifdef EM_CONT_SEP_OPR_Q
BYTE kctt[NO_KEYS]={
          NULL,
          N_1     ,N_4     ,N_7      ,STAR      ,NULL,NULL,NULL,
          N_2     ,N_5     ,N_8      ,N_0       ,NULL,NULL,NULL,
          N_3     ,N_6     ,N_9      ,HASH      ,NULL,NULL,NULL,
          CO_GRP1 ,EM_GRP1 ,NIGHT    ,ATT_RLS   ,NULL,NULL,NULL,
          CO_GRP2 ,EM_GRP2 ,PROG     ,RLS       ,NULL,NULL,NULL,
          RECALL  ,SRC     ,HOLD1    ,OPR_HOOK  ,NULL,NULL,NULL,
          EXTRNL  ,DEST    ,TONE_RNGR,SRL_TRNSFR,NULL,NULL,NULL,
          INTERNAL,EM_Q    ,PARK     ,NULL      ,NULL,NULL
          };
#else   /* EM_CONT_SEP_OPR_Q */
BYTE kctt[NO_KEYS]={
          NULL,
          N_1     ,N_4     ,N_7      ,STAR      ,NULL,NULL,NULL,
          N_2     ,N_5     ,N_8      ,N_0       ,NULL,NULL,NULL,
          N_3     ,N_6     ,N_9      ,HASH      ,NULL,NULL,NULL,
          CO_GRP1 ,EM_GRP1 ,NIGHT    ,ATT_RLS   ,NULL,NULL,NULL,
          CO_GRP2 ,EM_GRP2 ,PROG     ,RLS       ,NULL,NULL,NULL,
          RECALL  ,SRC     ,HOLD1    ,OPR_HOOK  ,NULL,NULL,NULL,
          EXTRNL  ,DEST    ,TONE_RNGR,SRL_TRNSFR,NULL,NULL,NULL,
//?????
          INTERNAL,OPRSIDE_OVRD    ,PARK     ,NULL      ,NULL,NULL
          };
#endif  /* EM_CONT_SEP_OPR_Q */

#endif  /* OPR_EM_CONNECTION */

#endif  /* PC_KEYBOARD */

#endif  /* OPERATOR_PC */



BYTE sp[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
  /* search pattern */
BYTE lkp;  /* last key pressed */

#ifdef TIME_DATE_DISPLAY
BYTE csra;  /* current save restore address for LCD */
BYTE sb[TOTAL_LCD_CHAR];  /* save block for LCD display */
#endif

LCD_RCRD lcdq[LCD_QUE_LEN];

/*
#ifndef OPERATOR_DEMO
BYTE flcdq,elcdq;
#else
*/
int flcdq,elcdq;
//#endif

BYTE lcdf;  /* LCD flag */
BYTE clc;  /* current LCD character */
BYTE lf;  /* line flag */

BYTE trf;  /* tone ringer flag */
BYTE trs;  /* tone ringer step */
int trb;  /* tone ringer bit */
unsigned int tr_pat;    /* tone ringer pattern */

BYTE ohs;  /* operator hook status */
BYTE trg;  /* tone ringer gain */

int rst_sgntr;  /* reset signature */

BYTE pswd_d2d1;
BYTE pswd_d4d3;

//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
int oprnight,oprday;
#endif
//end night upload

#ifdef OPERATOR_PC
#include "ovpc.h"

#if ((defined CAS3_HISTORICAL_REPORT) || (defined EM_HISTORICAL_REPORT) || (defined CAS1_HISTORICAL_REPORT))
#if (TON == HISTORICAL_OPR)

FILE *fhst;
long hstsize;

#endif
#endif

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
//char *cas3_hst_file_backup_string="copy cas3_hst.rpt cas3_hst.bak /Y >NUL:";
char *cas3_hst_file_backup_string="copy cas3_hst.rpt cas3_hst.bak /Y";
#endif
#endif

#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
//char *em_hst_file_backup_string="copy em_hst.rpt em_hst.bak /Y >NUL:";
char *em_hst_file_backup_string="copy em_hst.rpt em_hst.bak /Y";
#endif
#endif

#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
//char *cas1_hst_file_backup_string="copy cas1_hst.rpt cas1_hst.bak /Y >NUL:";
char *cas1_hst_file_backup_string="copy cas1_hst.rpt cas1_hst.bak /Y";
#endif
#endif


#endif

int iqs;  /* internal queue size */
BYTE rqs,xqs;  /* recall queue size & external queue size */
#ifdef EM_CONT_SEP_OPR_Q
BYTE emqs;  /* e&m queue size */
#endif

BYTE lsg1;  /* LED status group 1 */
BYTE lpg1;  /* LED program group 1 */

BYTE xlc,rlc,ilc;  /* EXTRNL,RECALL & INTERNAL LED counter */
BYTE plc,nlc;  /* PROG & NIGHT LED counter */
#ifdef EM_CONT_SEP_OPR_Q
BYTE emlc;      /* E&M LED counter */
#endif

#ifdef DISPLAY_STATUS_ACT
BYTE ds[NO_TOTAL_EXTENSION];  /* display status */
#endif

#ifdef DISPLAY_EM_STATUS
BYTE dts[NO_EM_DISPLAY];
BYTE dtschng[NO_EM_DISPLAY];
#endif

#ifdef DISPLAY_CAS3_STATUS
BYTE dcs[NO_CAS3_DISPLAY];
BYTE dcschng[NO_CAS3_DISPLAY];

BYTE dcsict[NO_CAS3_DISPLAY];
BYTE dcsictchng[NO_CAS3_DISPLAY];

#ifdef DISPLAY_CAS3_LINE_STATUS
BYTE dcsb[NO_CAS3_DISPLAY];
BYTE dcsbchng[NO_CAS3_DISPLAY];
#endif

#endif

#ifdef DISPLAY_CAS1_STATUS
BYTE d1s[NO_CAS1_DISPLAY];
BYTE d1schng[NO_CAS1_DISPLAY];

#ifdef DISPLAY_CAS1_LINE_STATUS
BYTE d1sb[NO_CAS1_DISPLAY];
BYTE d1sbchng[NO_CAS1_DISPLAY];
#endif

#endif


unsigned int stm;  /* scheduler's tasks mask */

#ifdef NET_DEBUG
FILE *net_fp;
#endif

#ifdef OPERATOR_PC
FILE *fer;
FILE *flp;

#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
FILE *flog;
long logsize;
//char *log_file_backup_string="copy alarm.log alarm.bak /Y >NUL:";
char *log_file_backup_string="copy alarm.log alarm.bak /Y";
#endif
#endif

#endif

#ifdef SI_DEBUG_ON_U
FILE *fsi[MAX_NO_OPR_CARD];
#endif


#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
BYTE rdbs[MAX_NO_OPR_CARD];  /* receive detail billing size in int */
BYTE rdbi[MAX_NO_OPR_CARD];  /* receive detail billing index,used for rdbr */
unsigned int adbrqf,adbrqe;  /* active detail billing records queue front & end */
#ifdef BEHNAMIR
unsigned int db[MAX_NO_OPR_CARD][MAX_DETAIL_BILLING_INFO_SIZE+1];  /* detail billing */
#else
unsigned int db[MAX_NO_OPR_CARD][MAX_DETAIL_BILLING_INFO_SIZE];  /* detail billing */
#endif
/* db[0]=source in INDEX_TYPE format,db[1]=destination in INDEX_TYPE format */
/* db[2]=time in sec,db[3]=number of digits,db[4] to db[8]=numbers in */
/* packed BCD,4 numbers in each d4d3d2d1,d8d7d6d5,...*/
BYTE cd;  /* current date */
#endif
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
DBR dbr[NO_DETAIL_BILLING_RECORDS];  /* detail billing records */
unsigned int fdbrqf,fdbrqe;  /* free detail billing records queue front & end */
BYTE complete_rdbs[MAX_NO_OPR_CARD],complete_rdbi[MAX_NO_OPR_CARD],complete_dbr[MAX_NO_OPR_CARD][MAX_DETAIL_BILLING_RECORD];
#endif
#endif

#ifdef REPORT_BULK_BILLING
#if( (TON == BULK_BILLING_OPR) || ((defined SECOND_BULK_BILLING_OPR) && (TON == SECOND_BULK_BILLING_OPR)) )
unsigned int bbxst[MAX_NO_OPR_CARD];     /* bulk billing extension number */
char bbfbs[80]; /* bulk billing file backup string */
unsigned int backup_index;
BYTE rbbs[MAX_NO_OPR_CARD];  /* receive bulk billing size in int */
BYTE rbbi[MAX_NO_OPR_CARD];  /* receive bulk billing index,used for ... */
#endif
#endif

#ifdef MALICIOUS_PRINT_ON_U
#if (TON == MALICIOUS_PRINT_OPR)
BYTE rmps[MAX_NO_OPR_CARD];  /* receive malicious print size in int */
BYTE rmpi[MAX_NO_OPR_CARD];  /* receive malicious print index,used for ... */
unsigned int rmp_data[MAX_NO_OPR_CARD][MAX_MALICIOUS_RECORD_SIZE];
#endif
#if ((defined MY_UDP_FOR_OAM) || (TON == MALICIOUS_PRINT_OPR))
BYTE complete_rmps[MAX_NO_OPR_CARD],complete_rmpi[MAX_NO_OPR_CARD],complete_rmp[MAX_NO_OPR_CARD][150];
#endif
#endif


//BYTE link_state[MAX_NO_OPR_CARD];
//FPNH olrtn_idx[NO_LINK_STATES][NO_LINK_EVENTS];
/* operator link routine index table contains the pointer of */
/* network handler routines */


#ifdef PRE_INSTALL
BYTE org;  /* operator receive gain */
#endif


#ifdef OPERATOR_FARSI
char lcd_1l_buf[LCD_CHAR_PER_LINE + 1]="                                        ";
char lcd_space_line[LCD_CHAR_PER_LINE + 1]="                                        ";
char lcd_3_space[4]="   ";
#endif

#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
BYTE op_can_dial_on_co;
#endif

#ifndef OPERATOR_PC
BYTE lpb;  /* LED port buffer */
BYTE lpnb;  /* LED port next buffer */
#endif

#ifdef PC_KEYBOARD
BYTE din[14];
BYTE rxflag;
BYTE break_flag;
BYTE tx_delay_flag;
BYTE bi;
BYTE oldrb;
#endif

/* parameter file format in unit of integer
MAX_NAME_LENGHT
PASSWORD

per ext.:       TTG (EM_GROUP) , CTG    may become 2 if X_RAILWAY
                CST
                AST
                hli

per e&m:        sp
                ai
                t1
                t2

per tie:        did
                ai

per co:         did
                ai

per cas3:       ai

nite extensions         4

hot lines               4
em group sgnds          NO_EM_GROUP
*/

int from_internal_to_dialled_no[NO_TOTAL_EXTENSION];

int from_dialled_to_internal_no[MAX_EXISTING_NO];
#if (  (defined LAPB_ON_OPERATOR_SIDE) || (defined HDLC_RS232_ON_OPERATOR_SIDE)\
     || (defined TCP_ON_CLIENT_SIDE)    || (defined TCP_ON_SERVER_SIDE) \
     || (defined ARP_INSTALLED)                                         \
     || (defined IP_INSTALLED)                                          \
     || (defined TCP_INSTALLED)                                         \
     || (defined UDP_INSTALLED)                                         \
     || (defined ETHERNET_INSTALLED)                                    \
     || (defined FIFO_RD_WR) || (defined KIATEL_DYNAMIC_MEM)            \
    )
char *mess[NO_PRINTER_MESSAGES];
#endif
// from depv.h start
int no_extension[3];
int no_my_extension[3];
int no_co[3];
int no_tie[3];
int no_em[3];
int no_cas3[3];
int no_cas1[3];
int no_em_group[3];
int no_cb[3];
//from depv.h end
unsigned int no_cas3_channel[3];
unsigned int no_cas1_channel[3];
GRP_TYPE no_cas3_programmable_group[3];
GRP_TYPE no_cas1_group[3];

BYTE no_intrnl_dgt;
BYTE intrnl_1st_dgt;
BYTE intrnl_1st_dgt_2;
BYTE intrnl_1st_dgt_3;
BYTE intrnl_1st_dgt_4;
BYTE intrnl_1st_dgt_5;
unsigned long f_d_n;
unsigned long first_directory_decimal;
unsigned int ss[180]; // (110-10)-199
char buf110[90+10][20];
unsigned int buf1819_1[10];
char buf1819_2[10][20];

BYTE term_flag;

#ifdef TIMERS_FOR_OPR
TMR timer[TIMER_NO];
unsigned int ftp,ltp;  /* first timer pointer,last timer pointer */
unsigned int tpt[NO_TMR_QPTR];  /* timer pointer table */
unsigned int tpti;  /* timer pointer table index, used for indexing in tpt */
#endif

#ifdef KIATEL_DYNAMIC_MEM
MEM_REC_ST mem_rec[NO_MEM_REC];
unsigned int eq_mem;
unsigned int fq_mem;
BLOCK *my_slop;
BLOCK *my_freelist[30];

#if NUMSMALL
BLOCK *my_smallblocks[NUMSMALL];
#endif

unsigned char dynamic_mem_arr[MAX_DYNAMIC_MEM_SIZE];
unsigned long curr_mem_indx;
BLOCK *expected_sbrk;
BYTE dump_memory_done;
#endif

#ifdef MY_TCPIP_FOR_OAM
#define MY_TCP_TX_NETBUF_SIZE   (4096)
unsigned int tcb0,tcb1;
BUF nettx_st0,nettx_st1;
BYTE tx0[MY_TCP_TX_NETBUF_SIZE];
BYTE tx1[MY_TCP_TX_NETBUF_SIZE];
#endif
#ifdef OPERATOR_SIDE
#ifdef OPERATOR_PC
FILE *opr_gonz_fp;
FILE *opr_log_fp;
#endif
#endif

#ifdef SPEED_DIALLING
SP_NODE_STRUCT *first_sp_node_pt;
#endif
