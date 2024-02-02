                              /**** opr_net.h  ****/

#define  SYNC_RESET_THRESHOLD   25
        /* 25*20 msec=500 msec for accepting out of sync */

#define  SYNC_SET_THRESHOLD     25
        /* 25*20 msec=500 msec for accepting sync */

#define  SYS_NETWORK_INIT_THRESHOLD     100
        /* 2 sec before initializing network */

#define  LINK_DOWN_THRESHOLD            250
        /* 250*20 msec=5 sec for link dwon before initializtion */

#define  SYNC_OUT_OPR_NIGHT             6000
        /* 6000 * 20 msec = 2 min out of sync then corresponding operator */
        /* goes on night */

#define  SYNC_OUT_ALARM                 100
        /* 100 * 20 msec = 2 sec out of sync then alarm */

//GRPPRG : commeted
//#define FIFO_SIZE       4
        /* maximum 1 4_byte packets can be send or received */

#define I_C             0
                /* information control */
#define I_C_NF          0x02
                /* information control for next frame */
#define S_C             0x01
                /* supervisory control */
#define U_C             0x03
                /* unnumbered control */
#define P_F             0x10
                /* poll / final */
#define RR              0x01
                /* receiver ready */
#define SABM            0x2f
                /* set asynchronous balance mode */
#define UA              0x63
                /* unumbered ACK.,response to SABM */

#define FRM1_EX         0x20
                /* frame 1 expected */
#define FRM1_TS         0x02
                /* frame 1 to send */
#define  FRMGD          0x0c00
#define  FRMERR         0x0300


#define  N2_THRESHOLD   100

                        /****  states definitions  ****/

#define RESET_NOT_SYNC_LINK_NOT_AVAIL   0
#define RESET_SYNC_LINK_NOT_AVAIL       1
#define RESET_SYNC_LINK_AVAIL           2
#define INFORMATION_TRANSFER            3
#define NOT_SYNC_INFORMATION_TRANSFER   4

#define  NO_LINK_STATES                 0x05

                        /****  events definition  ****/
#define NOT_SYNC                0
#define SYNC                    1
#define NULL_EVENT              2
#define SABM_FRAME              3
#define UA_FRAME                4
#define I_FRAME                 5
#define RR_FRAME                6

#define  NO_LINK_EVENTS         0x07


typedef void (*FPNH)(BYTE);
       /* FPNH is function pointer type for network handler */

                /****  functions' prototypes  ****/

int miftx(BYTE);void fec_pc(void);void ihc(BYTE);void ihp(BYTE);void roni(BYTE);
void txsuf(BYTE,BYTE);void txf(BYTE);int ofrx(BYTE);void onh(BYTE);
void soon(BYTE);int ttxq(BYTE *,BYTE,BYTE);int trxq(BYTE);
int pcpmftx(int,int,BYTE);
int pcpmftx1(int,int,BYTE);
int pcpmftx2(int,int,BYTE);
int tmfrxq(BYTE);void mtmaq(BYTE);void disp_rxib(BYTE);

#ifdef LIM
void mtcpq(BYTE);
#else
void mtcpq(void);
#endif

int smaf(void);void olrtn_init(void);void olrtn_error(BYTE);

void olrtn0(void);
void olrtn1(BYTE);void olrtn2(BYTE);void olrtn3(BYTE);void olrtn4(BYTE);
void olrtn5(BYTE);void olrtn6(BYTE);void olrtn7(BYTE);void olrtn8(BYTE);
void olrtn9(BYTE);void olrtn10(BYTE);void olrtn11(BYTE);void olrtn12(BYTE);
void olrtn13(BYTE);

