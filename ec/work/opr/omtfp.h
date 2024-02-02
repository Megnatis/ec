
#define STORE_SUBSCRIBER_DIRECTORY_NUMBER_IN_BILLING_FILE

#define CLOSE_DBRF_ON_DAY
//#define CLOSE_DBRF_ON_SIZE

#include "/ec/work/sys/osdef.h"

#if (OS == WINDOWS)
#include <go32.h>
#define _SIZE_T
#include <sys/types.h>         // c:/rsxntdj/include/sys/stat.h needs this
#endif

#if (COMPILER == GCC)
#include <stdio.h>
#if (OS == DOS)
#include <go32.h>
#endif
#if (OS == DOS) || (OS == WINDOWS)
#include <pc.h>
#include <dpmi.h>
#define LOCK_VARIABLE(x)        _go32_dpmi_lock_data((void *)&x,(long)sizeof(x));
#define LOCK_FUNCTION(x)        _go32_dpmi_lock_code(x,(long)sizeof(x));
#endif
#if (OS == LINUX_COMPATIBLE)
//#include <asm/io.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/io.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/vt.h>
#include <string.h>
#include <stdarg.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#if (LINUX_OS == KURT)
#include <linux/rt.h>
#endif
#endif
#endif

#define OPERATOR_SIDE
                      /*****   OMTFP.H   *****/



                      /***    class of service macros   ***/
#define  CALL_BACK              0x01
#define  CALLER_ID_RESTRICTION  0x01
#define  CAMP_ON                0x02
#define  FOLLOW_ME              0x04
#define  STD                    0x08
#define  INTERNATIONAL          0x10
#define  DONT_DIST              0x20
#define  RING_AGAIN_COT         0x40
#define  B3                     0x40
#define  CONFERENCE_SERVICE     0x80
#define  PRI_ALLOWED            0x100
#define  TIE_0                  0x100
#define  EM_ALLOWED             0x100
#define  B2                     0x100
#define  CALL_PICKUP_USE        0x200
#define  WAKE_UP_IN_CST         0x200
#define  RING_AGAIN_TIET        0x400
#define  DTMF_ALLOWED           0x400
#define  HOT_LINE_ACCESS        0x800
#define  HOT_LINE_ACTIVE        0x800
#define  CLID_ACTIVE            0x800
//#define  CEPT_ALLOWED           0x1000
#define  SIREN_ACCESS           0x2000
#define  B1                     0x2000
#define  DECT_DATA_ALLOWED      0x2000
#define  AMPLIFIER_ACCESS       0x4000
#define  HOME_METERING_ACTIVE   0x4000


#define  CALL_GEN               0x80
#define  FOLLOW_ME_ALWAYS       0x04
#define  FOLLOW_ME_BUSY         0x08
#define  FOLLOW_ME_NO_ANS       0x10
#define  CO_ALLOW               0x80
#define  TIE_ALLOW              0x80
#define  MALICIOUS_CALL_DTCT    0x100
#define  DONT_ALLOW_ACD         0x200
#define  FEATURE_ACTIVE         0x8000

/**** include installation macro file here  ****/

#define NEW_DEP

#include "/ec/work/spec/general.h"

#ifdef DISPLAY_STATUS_ACT
#define NEW_DISPLAY_STATUS
#endif

#ifndef OPERATOR_PC
#pragma large
#endif

#include "key.h"
#define CHAT_TYPE       unsigned long           // char + attribute
#define CHAT_SIZE       sizeof(CHAT_TYPE)
#ifdef DYNAMIC_EXT_INSTALLED
#ifdef BASIC_NET_ISDN_INSTALLED
#define BASIC_NET_ISDN_NO_DYNAMIC_EXTENSION     (NO_ISDN_USER*2)
#else
#define BASIC_NET_ISDN_NO_DYNAMIC_EXTENSION     0
#endif
#ifdef PRIMARY_NET_ISDN_INSTALLED
#define PRIMARY_NET_ISDN_NO_DYNAMIC_EXTENSION   (NO_ISDN_USER*30)
#else
#define PRIMARY_NET_ISDN_NO_DYNAMIC_EXTENSION   0
#endif
#ifdef USERSIDE_PRIMARY_ISDN_INSTALLED
#define USERSIDE_ISDN_NO_DYNAMIC_EXTENSION      (USERSIDE_NO_ISDN_USER*30)
#else
#define USERSIDE_ISDN_NO_DYNAMIC_EXTENSION      0
#endif
#define NO_DYNAMIC_EXTENSION            (BASIC_NET_ISDN_NO_DYNAMIC_EXTENSION+\
                                         PRIMARY_NET_ISDN_NO_DYNAMIC_EXTENSION+\
                                         USERSIDE_ISDN_NO_DYNAMIC_EXTENSION)

#define NO_EXTENSION                    (NO_STATIC_EXTENSION+NO_DYNAMIC_EXTENSION)
#endif // DYNAMIC_EXT_INSTALLED

#if (NO_EXTENSION != 0)
#define EXT_INSTALLED
#endif

#ifndef NO_STATIC_EXTENSION
#define NO_STATIC_EXTENSION     NO_TOTAL_EXTENSION
#endif

#ifdef LIM  // assuming in LIM we do not have DYNAMIC_EXTENSION
#define NO_MY_EXTENSION     NO_EXTENSION
#else
#define NO_MY_EXTENSION     NO_STATIC_EXTENSION
#endif

#ifdef DSP_DTMF
#define NO_DTMF                 16
#endif

#ifndef NEW_HEADER
#define CPU_EC512
#endif

#ifdef CENTRAL_PC
#define EC2K_SYSTEM
#define PC_COMPATIBLE
#endif

#ifndef NEW_HEADER
#define CALL_PROCESSOR
#endif

#ifdef DETAIL_BILLING
#define DISPLAY_BILLING
#endif

#if ((defined CALL_PROCESSOR) && (defined PC_COMPATIBLE))
#define SOFTWARE_SECURITY
#endif

#define BATCH_PROGRAMMING
#define OPR_CAS3_PROGRAM
#define OPR_CAS1_PROGRAM
#define OPR_EM_PROGRAM
#ifdef BATCH_PROGRAMMING
#define NEW_ANALOG_TRUNK_GROUPING
#endif

#if ((!defined HDLC_RS232_ON_OPERATOR_SIDE) && (!defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) && (!defined MY_TCPIP_FOR_OAM) && (!defined MY_UDP_FOR_OAM))
#ifndef DO_NOT_DEFINE_HDLC_8993
#define HDLC_8993
#endif
#endif

#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#define LAPB_ON_OPERATOR_SIDE
#endif
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#define HDLC_RS232              //      for sml.c and smllyr1.c
#endif

#ifdef   LD_INSTALLED
#define  LD_ALLOWED             0x2000 /* its position in cst[], same as siren access */
#endif

#ifdef CAS3_INSTALLED
#define NO_CAS3_CHANNEL         (NO_CAS3_DIGITAL_TRUNK * 30)
#endif

#ifdef CAS1_INSTALLED
#define NO_CAS1_CHANNEL         (NO_CAS1_DIGITAL_TRUNK * 30)
#endif


/* to support older "general.h" files which do not define NO_CAS3 */
#ifndef NO_CAS3
#define NO_CAS3         0
#endif

#ifndef NO_CAS1
#define NO_CAS1         0
#endif

#if (NO_CAS3 != 0)
#define         EXTERNAL_CAS3_INSTALLED
#endif

#ifdef PC_COMPATIBLE
#define SEND_BILLING_TIME_FROM_SYS_TO_OPR
#endif

#define  KT_NULL                -1
#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == WINDOWS)
#include <dos.h>
#include <conio.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#if (COMPILER == GCC)
#include <sys/stat.h>
#if (OS == DOS) || (OS == WINDOWS)
#include <sys/farptr.h>
#include <dir.h>        // for setdisk
#endif
#include <unistd.h>     // for chdir
#else
#include <dir.h>
#endif
#define alarm() my_alarm()

#if ( (PARAMETER_DOWN_UP_LOAD == DEFINE_VALUE) || (DETAIL_BILLING == DEFINE_VALUE) )

#define MAX_NAME_LENGHT         MAX_NAME_LENGTH

#endif

#ifdef PARAMETER_DOWN_UP_LOAD

#define READ_BLOCK_SIZE         128
#define VALID_FILE              1
#define INVALID_FILE            0
#define FILE_CHECK_SUM_SIZE     1
/* file check sum is an unsigned int */

/*
#ifdef EM_GROUP_PROGRAM
#define VALID_FILE_SIZE  (MAX_NAME_LENGHT+2+NO_EXTENSION*8+NO_ANALOG_TRUNK*4+NO_EM*4+2)
#else
#define VALID_FILE_SIZE  (MAX_NAME_LENGHT+2+NO_EXTENSION*8+NO_ANALOG_TRUNK*4+2)
#endif
*/
/* valid file size=20 bytes for installation name,2 bytes for password, */
/* 8 bytes for each extension,4 bytes for each analog (or e&m) trunk,2 bytes for fcs */

#define FILE_CREATION_OK        1
#define FILE_CREATION_FAIL      0

#endif


#ifdef SI_FOR_BULK_BILLING
#define  HOUR                   3
#define  MINUTE                 0
#endif

#define EN_SPKR  kt_outport8(0x61,kt_inport8(0x61)|0x03)
#define DIS_SPKR kt_outport8(0x61,kt_inport8(0x61)&0xfc)

#define FREQ1_LSB 169
#define FREQ1_MSB   4       /* 1.19318 Mhz /  1 Khz = 1193 */
#define FREQ2_LSB 121
#define FREQ2_MSB  10       /* 1.19318 Mhz / 445 Hz = 2681 */

#ifdef HDLC_8993
#define MT8993_PORT             0x1700
#define C_CHAN   0x0000
#define HDLC_XR  0x0002
#define HDLC_CSR 0x0003
#define HDLC_ISR 0x0004
#define TCR      0x0005
#define TR1      0x0007
#define TR2      0x0008
#define DSP_CR   0x0009
#define TRNS_CR  0x000a
#define RGCR     0x000b
#define SD_DATA  0x000c
#define SD_DIR   0x000d
#define TEST_REG 0x000e
#endif


#define  NO_KEYS                60
#define  NO_HOLD_KEY            4

#define vad(x,y)                ( (y) * 160 + (x) * 2)

#define  TONE_RNGR_LED          24
#define  HOLD1_LED              25
#define  HOLD2_LED              26
#define  HOLD3_LED              27
#define  HOLD4_LED              28
#define  PARK_LED               29

#define  EXTRNL_LED             17
#define  RECALL_LED             16
#define  INTERNAL_LED           18
#ifdef EM_CONT_SEP_OPR_Q
#define  EM_LED                 15
#endif

#define  NIGHT_LED              10
#define  PROG_LED               11

#define  SOURCE_LED             34
#define  DEST_LED               35

#define  SYNC_LED1              30
#define  LINK_UP_LED1           31
#define  OPR_RESET_LED          32
#define  RLS_LED                33

#define  SYNC_LED2              36
#define  LINK_UP_LED2           37

#define  SYNC_LED3              38
#define  LINK_UP_LED3           39

#define  MAX_NO_LED             40

#else

#include <absacc.h>  /* direct access to RAM locations */
#include <reg51.h>  /* special function register 8051 */


#define MT8993_PORT             0xa000
#define C_CHAN                  0xa000
#define HDLC_XR                 0xa002
#define HDLC_CSR                0xa003
#define HDLC_ISR                0xa004
#define TCR                     0xa005
#define TR1                     0xa007
#define TR2                     0xa008
#define DSP_CR                  0xa009
#define TRNS_CR                 0xa00a
#define RGCR                    0xa00b
#define SD_DATA                 0xa00c
#define SD_DIR                  0xa00d
#define TEST_REG                0xa00e

//#define  NULL                 -1

#define  NO_KEYS        56
#define  NO_HOLD_KEY    1

#define  CO_GRP1_LED    0x01
#define  CO_GRP2_LED    0x02
#define  RECALL_LED     0x04
#define  EXTRNL_LED     0x08
#define  INTERNAL_LED   0x10
#define  TIE_GRP1_LED   0x20
#define  TIE_GRP2_LED   0x40
#define  SOURCE_LED     0x80
#define  DEST_LED       0x0100
#define  OVRD_LED       0x0200
#define  NIGHT_LED      0x0400
#define  PROG_LED       0x0800
#define  HOLD1_LED      0x1000
#define  TONE_RNGR_LED  0x2000
#define  PARK_LED       0x4000
#define  RESET_LED      0x8000
#ifdef EM_CONT_SEP_OPR_Q
#define  EM_LED         RESET_LED
#endif

#define  SYNC_LED       OVRD_LED
#define  LINK_UP_LED    PARK_LED
#define  OPR_RESET_LED  RESET_LED

#define LED_PORT                0xe000
#define LED_PORT_NEXT           0x8000

#define LCD_CMD                 0xc000
#define LCD_DATA                0xc001
#define LCD_STATUS              0xc000
#define ID                      0x23
#define RAM_SIZE                0x8000     /* for 6116 */

#endif

#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
#define  CP_QUE_LEN             (16384)         // that makes also MA_QUE_LEN longer which is necessary
#else
#define  CP_QUE_LEN             128
#endif

#define  NO_OPR_NITX            4
#define  NO_HOT_LINE            4

#define RESET_SIGNATURE         0x1234
#define FORCE_RESET_SIGNATURE   0x5678

#define ALL_TEST_DISABLE        0x40

#define TWO_LINE_INIT           0x38
#define ONE_LINE_INIT           0x30
#define NO_1L_CH                16

#define  OK              1
#define  FAIL            0

#define  MA_QUE_LEN      (CP_QUE_LEN / 2)


#ifdef PARAMETER_DOWN_UP_LOAD

#define  RX_QUE_LEN     256
        /* receiver queue length */
#ifdef X_RAILWAY
#define  TX_QUE_LEN     16384
#else
#ifdef BATCH_PROGRAMMING
#define  TX_QUE_LEN     16384           // size of each ordinary ma packet now is 5 bytes instead of 4, so uploading e.g. 1672 words needs 1672*5=8360 bytes!
#else
#define  TX_QUE_LEN     8192
#endif
#endif
        /* transmitter queue length */
#else

#define  RX_QUE_LEN     128
        /* receiver queue length */
#define  TX_QUE_LEN     128
        /* transmitter queue length */
#endif

#define  MIN_I_PKT      4

#ifdef BATCH_PROGRAMMING
#define FIFO_SIZE       7
#else
#define FIFO_SIZE       4
        /* maximum 1 4_byte packets can be send or received */
#endif

#define CHECKSUM

                /**** flag macros ****/

#define  INTRF          0x01
#define  SCHLF          0x02
#define  CPF            0x04
#define  MAF            0x08
#define  KBHF           0x10
        /* key board hit flag */
#define  KBRF           0x20
        /* key board release flag */
#define  DSF            0x40
        /* display status flag */

#ifdef PARAMETER_DOWN_UP_LOAD
#define  PARM_DOWN      0x80
#define  PARM_UP_1      0x100
#define  PARM_UP_2      0x800
#define  PARM_UP_3      0x1000
#endif

#ifdef OPERATOR_PC
#define OPR_CARD_OUT    0x200
#endif

#ifdef OPERATOR_PC
#define OPR_LANG_FLAG   0x400
/* oprerator language flag 1: Farsi,0: English */
#endif

                /*****          states definitions              *****/
#define  IDLE                           0
#define  PARK                           1
#define  HOLD                           2
#define  IN_INTR_QU                     3
        /* in internal queue */
#define  IN_RCL_QU                      4
        /* in recall queue */
#define  IN_EXTRNL_QU                   5
        /* in external queue */
#define  IN_OCR                         6
        /* in operator call record */
#define  OPRSIDE_INTRNL_CALL            7
#define  PARK_OUT_DIAL                  8
#define  PARK_IN_DIAL                   9
////#define  OPRSIDE_RINGING                0xa
////#define  OPRSIDE_CONFERENCE             0xb
////#define  OPRSIDE_CONVERSATION           0xc
////#define  OPRSIDE_AWAIT_PARK_LOC         0xd
////#define  OPRSIDE_EXTRNL_CALL            0xe
#define  OPR_READY                      0xf
#define  OPR_IDLE                       0x10
#define  OPRSIDE_OPR_TALK               0x11
#define  OPR_START                      0x12
#define  OPR_DIAL_INTRNL                0x13
#define  OPRSIDE_OPR_EXT_RINGING        0x14
#define  AWAIT_EXT                      0x15
#define  OPR_EXT_BUSY                   0x16
#define  OPR_EXT_OVRD                   0x17
#define  AWAIT_TRNK                     0x18
#define  OPR_PROG                       0x19
#define  AWAIT_PROG_DEST                0x1a
#define  AWAIT_PROG_FUNC                0x1b
#define  AWAIT_ACT_INACT                0x1c
#define  OPRSIDE_OPR_NIGHT              0x1d
#define  AWAIT_SYS_PROG                 0x1e
#define  AWAIT_PASSWORD                 0x1f
#define  UPLOAD_FILE_SELECTION_1        0x20
#define  AWAIT_TRNK_NO                  0x21
#define  OPRSIDE_TRUNK_TRUNK_TALK       0x22
#define  OPR_TRUNK_OVRD                 0x23
#define  AWAIT_SYS_UPLOAD_ACCEPTANCE_1     0x24
#define  AWAIT_SYS_UPLOAD_ACCEPTANCE_2     0x25
#define  AWAIT_SYS_UPLOAD_ACCEPTANCE_3     0x2a
#define  UPLOAD_FILE_SELECTION_2        0x26
#define  UPLOAD_FILE_SELECTION_3        0x2b

/*
#ifdef PRE_INSTALL
#define  AWAIT_GAIN                     0x27
#endif
#ifdef PRE_INSTALL
#define  OPRSIDE_NO_STATES              0x28
#else
#define  OPRSIDE_NO_STATES              0x27
#endif
*/

/* Miss Savoj has changed this block */

#define  AWAIT_GAIN                     0x27

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#define  AWAIT_CAS3_HST_CH              0x28
#endif
#endif

#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#define  AWAIT_EM_HST_CH                0x29
#endif
#endif

#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#define  AWAIT_CAS1_HST_CH              0x35
#endif
#endif
#ifdef PC_COMPATIBLE
#define  AWAIT_SYSTEM_NO                0x36
#endif

#ifdef OPR_OPR_TALK
#define  OPR_DIAL_OPR                   0x2c
#define  AWAIT_OPR                      0x2d
#define  OPR_OPR_RINGING                0x2e
#define  OPR_OPR_BUSY                   0x2f
#define  OPR_OPR_OVRD                   0x30
#define  OPR_AT_HOLD                    0x31
#endif

//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
#define  NIGHT_UPLOAD                   0x32
#define  DAY_UPLOAD                     0x33
#endif
//night upload finish.

#ifdef X_RAILWAY
#define OPR_DIAL_EM                     0x34
#endif
#define OPR_DIAL_CO                     0x37
#define  OPRSIDE_NO_STATES              0x38

#ifdef DISPLAY_CAS3_STATUS
#ifdef SHOW_LIM_STATUS
#define NO_CAS3_DISPLAY        (NO_CAS3_DIGITAL_TRUNK * 30)
#else
#define NO_CAS3_DISPLAY        (NO_CAS3)
#endif
#endif

#ifdef DISPLAY_CAS1_STATUS
#define NO_CAS1_DISPLAY        (NO_CAS1_DIGITAL_TRUNK * 30)
#endif

#ifdef DISPLAY_EM_STATUS
#define NO_EM_DISPLAY        NO_EM
#endif

#define UNKNOWN                 0xff
#define OUT_OF_SERVICE          0xff
#define OGT                     0xfe
#define ICT                     0xfd


        /*****          stimulus definition             *****/
#define  OFF_HOOK               0
#define  ON_HOOK                1
#define  ABONDON                2
#define  AT_RING                ABONDON
/* because ABONDON is not used for ext. at operator,I use it for */
/* status display instead of defining new stimulus for RING state */
#define  TIME_OUT               3
#define  OPR_REJECT             4         /* if resources unavailable */
#define  CNNCT_TO_OPR           5
#define  NO_MATCH               6
#define  EXT_PICKED_UP          NO_MATCH
#define  INTERNAL               7
#define  OPR_REQ                8
#define  RING_DTCT              9
#define  EM_CONT_OPR_REQ        RING_DTCT
#define  CAS3_OPR_REQ           RING_DTCT
#define  RECALL                 0xa
#define  TRNK_FREE              0xb
#define  GROUP_BUSY             0xc
#define  CO_GROUP_REQ           0xd
#define  SRC_EXT_REQ            0xe
#define  DST_EXT_REQ            0xf
#define  ATT_RLS                0x10
/* also used for camp on connected party of operator to a busy ext. */

#define  RLS                    0x11
#define  OVRD_KEY               0x12
#ifdef NO_OP_OVRD
#define  OPRSIDE_OVRD           0xfe    /* non-existent code */
#else
#define  OPRSIDE_OVRD           OVRD_KEY
#endif
#define  SYS_PROG_KEY           OVRD_KEY
#define  OPRSIDE_MALICIOUS_PARTY        OVRD_KEY
#define  HOLD_XT                0x13     /* hold extension,trunk */
#define  PARK_XT                0x14     /* park extension,trunk */
#define  TIE_GROUP_REQ          0x15
#define  RING_DISCNT            0x16
#define  EXT_FREE               0x17
#define  EXT_FORWARDED          EXT_FREE
#define  EXT_BUSY               0x18
#define  TRNK_RLS               0x19
#define  MUSIC                  0x1a
#define  TONE_TO_OPR            0x1b
#define  AT_LOCK                TONE_TO_OPR
/* because TONE_TO_OPR is not used for ext. at operator as a stimulus,I use it for */
/* status display instead of defining new stimulus for LOCK state */
#define  STAR                   0x1c
#define  HASH                   0x1d
#define  NIGHT                  0x1e
#define  SRC                    0x1f
#define  DEST                   0x20

#define  N_1                    0x21
#define  N_2                    0x22
#define  N_3                    0x23
#define  N_4                    0x24
#define  N_5                    0x25
#define  N_6                    0x26
#define  N_7                    0x27
#define  N_8                    0x28
#define  N_9                    0x29
#define  N_0                    0x2a

#define  HOLD1                  0x2b
#define  HOLD2                  0x2c
#define  HOLD3                  0x2d
#define  HOLD4                  0x2e

#define  CO_GRP1                0x2f
#define  CO_GRP2                0x30
#define  CO_GRP3                0x31
#define  CO_GRP4                0x32
#define  CO_GRP5                0x33
#define  CO_GRP6                0x34

#define  TIE_GRP1               0x35
#define  TIE_GRP2               0x36
#define  TIE_GRP3               0x37
#define  TIE_GRP4               0x38
#define  TIE_GRP5               0x39
#define  TIE_GRP6               0x3a

#define  EMCAS_PROG_KEY         TIE_GRP1

#ifdef NO_TONE_RNGR
#define  TONE_RNGR              0xfe    /* non-existent code */
#else
#define  TONE_RNGR              0x3b
#endif
#define  TIME                   0x3c
#define  SRL_TRNSFR             TIME
#define  PROG                   0x3d
#define  OPR_HOOK               0x3e
#define  EXTRNL                 0x3f
#define  VOLUME                 0x40
#define  FRC_RLS                VOLUME
#define  DISP_STATUS            0x41
#define  TOG_LANGUAGE           0x42

#define  UPLOAD_ACCEPT          0x43
#define  UPLOAD_REJECT          0x44

/* if above two changed update dsmtfp.h */

#define  EM_GRP1                0x45
#define  EM_GRP2                0x46
#define  EM_GRP3                0x47
#define  EM_GRP4                0x48
#define  EM_GRP5                0x49
#define  EM_GRP6                0x4a
#define  EM_GRP7                0x4b
#define  EM_GRP8                0x4c
#define  EM_GRP9                0x4d
#define  EM_GRP10               0x4e

#define  EM_OPEN                0x4f
#define  DT_CLF_TO_OPR          0x52
/* if above two changed update dsmtfp.h */

#define  EM_Q                   0x50
#define  SHELL                  0x51

#define DISP_TRUNK_STATUS       0x53
#define MANUAL_TEST             0x54
#define EXIT_MANUAL_TEST        0x55
#define DISP_CAS3_STATUS        0x56
#define DISP_CAS1_STATUS        0x57

#ifdef OPR_OPR_TALK             // followings may be sent to system side
#define SRC_OPR_REQ             0x58
#define DST_OPR_REQ             0x59
#define OPR_FREE                0x5a
#define OPR_BUSY                0x5b
#define OPR_HOLD                0x5c     /* hold extension,trunk */
#define OPR_DEST_READY          0x5d
#define OPR_ON_HOOK             0x5e
#define OPR_CNNCT_TO_OPR        0x5f
#define CNNCT_TO_HOLD_OPR       0x60
#define SRC_CNNCTED             0x61
#endif
#define DEP_ACT                 0x62
#define  CAS3_GRP1              0x63
#define  BILLING_ACT            0x64
#define  ALARM_ACT              0x65
#define  TERM_ACT               0x66
#define  OPRSIDE_CLID           0x67    // reserve 10 spaces
#define  PRI_GRP1               EM_GRP6
#ifdef OPR_SIP_CONNECTION
#define  SIP_REQ                EM_GRP5
#endif

#define  OPRSIDE_NO_STIMS       0x71

// following are those defined in dsmtfp.h and needed here
#define  NIGHT_ACT              0x1e
#define  NIGHT_INACT            0x1f
#define  EM_GROUP_REQ           0x2c
#define  NUM                    0x2d
#define  X_STATUS_REQ_ACT       0x2e
#define  X_STATUS_REQ_INACT     0x2f
#define  SERIAL_TRANSFER        0x30
#define  FORCE_RELEASE          0x31
#define  CNNCT_TO_OPR_EM_CONT   0x33
#define  CNNCT_TO_OPR_CAS3      0x35
#define  EM_STATUS_REQ_ACT      0x37
#define  EM_STATUS_REQ_INACT    0x38
#define  CAS3_STATUS_REQ_ACT    0x39
#define  CAS3_STATUS_REQ_INACT  0x3a
#define  CAS1_STATUS_REQ_ACT    0x3b
#define  CAS1_STATUS_REQ_INACT  0x3c
#define  CAS3_GROUP_REQ         0x3f
#ifdef OPR_PRI_CONNECTION
#define  PRI_GROUP_REQ          0x3f
#endif
#ifdef OPR_SIP_CONNECTION
#define  SIP_GROUP_REQ          EM_GROUP_REQ
#endif

#define  NO_SERVICE_DGT         2

#if (NO_ANALOG_TRUNK >= 100)
#define  NO_TRUNK_ID_DGT        3
#else
#define  NO_TRUNK_ID_DGT        2
#endif

#if ((NO_CAS3 >= 100) || (NO_EM >= 100))
#define  NO_EMCAS_ID_DGT        3
#else
#define  NO_EMCAS_ID_DGT        2
#endif

#define  SET                    1
#define  RESET                  0

//#define  DIAL_TICK              500
                                         /* Dial timer = 500 * 20 msec */
//#define  HOLD_TICK              6000
                                                                /* hold timer = 6000*20 msec */
//#define  PARK_TICK              (2*HOLD_TICK)

#ifndef NO_COT_GROUP
#if(NO_CO != 0)
#define  NO_COT_GROUP           16 //6
                        /* number of CO trunk group */
#else
#define  NO_COT_GROUP           0
#endif
#endif

#ifndef NO_TIE_INIT
#define  NO_TIET_GROUP           6
                        /* number of TIE trunk group */
#else
#define NO_TIET_GROUP           0
#endif


#ifdef OPR_EM_CONNECTION
#ifdef X_RAILWAY
//#define NO_EMT_GROUP            10
#define NO_EMT_GROUP            15
#else
#define NO_EMT_GROUP            6
#endif
#else
#define NO_EMT_GROUP            0
#endif
#ifdef OPR_DT_CONNECTION
#define NO_CAS3_GROUP_IN_OPR    1
#else
#define NO_CAS3_GROUP_IN_OPR    0
#endif
#ifdef OPR_PRI_CONNECTION
#define NO_PRI_GROUP_IN_OPR    1
#else
#define NO_PRI_GROUP_IN_OPR    0
#endif

#ifdef CB_INSTALLED
#define  CB_T                   0x07
//#define FIRST_CB_NO             1000
#endif

#define  MA_T                   0
        /* maintenance & administration type */
#define  OPR_T                  0x01
#define  XST_T                  0x02
#define  NULL_T                 0x04
/* if we had saved originators
#define  CONF_T                 0x06
*/
#define  TIE_T                  0x0a
#define  DOD_T                  0x0e
#ifdef LIM
#define  LIM_T                  0x09
#endif
#define  EM_T                   0x0b
#ifdef CAS3_INSTALLED
#define  CAS3_T                 0x0f
#endif
#ifdef CAS1_INSTALLED
#define  CAS1_T                 0x0c
#endif
#define SERVICE_T               0x0d    // only in billing
#define NEWCC_T                 0x0b    // only in billing
#define PRI_T                   0x05    // only in billing
#define PRI_NETWORK_T           0x06    // only in billing
#ifdef ISUP
#define ISUP_T                  0x08
#endif
#define  TRUNK_T  0x08 /*(TIE_T & DOD_T & EM_T & CAS3_T & CAS1_T & ~XST_T)*/
/*#define  TRUNK_T                (TIE_T & DOD_T & ~XST_T)*/
#define  XDT_T                  (TIE_T & DOD_T & XST_T)
#define  XCT_T                  XDT_T

#define  CO_T                   DOD_T
#ifdef BATCH_PROGRAMMING
#define BATCH_MA_T              0x05  // DR_T
#endif

// TERM_T, and ALARM_T, are only used in sys<->opr link, along with MA_T
#define TERM_T                  0x06
#define ALARM_T                 0x07

#ifdef SIP_INSTALLED
#define SIP_T                   EM_T
#endif

#ifdef NO_OPR_INTRNL_QU
#define  IQR_NO                 1
#else
#ifdef SA_RAILWAY
#define  IQR_NO                 5
#else
#define  IQR_NO                 100 //2
#endif
        /* internal queue records number */
#endif

#ifdef OPR_EM_CONNECTION
#define  RQR_NO                 (NO_CO+NO_TIE+NO_EM)
        /* recall queue records number */
#else
#define  RQR_NO                 (NO_CO+NO_TIE)
        /* recall queue records number */
#endif

#define  XQR_NO                 RQR_NO
        /* external queue records number */

#define  RXQR_NO                (RQR_NO+XQR_NO)

#if (RXQR_NO > 255)
#undef RXQR_NO
#define  RXQR_NO                255
#endif


#define  TOTAL_LCD_CHAR         32

#ifdef OPERATOR_PC
#define LCD_CHAR_PER_LINE       40
#endif

#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#define  LCD_QUE_LEN            2048
#else //HDLC_RS232
#ifndef OPERATOR_DEMO
#define  LCD_QUE_LEN            256
#else
#define  LCD_QUE_LEN            512
#endif
#endif //HDLC_RS232

#define  TO_LCD_IR              1
#define  TO_LCD_DD              2
#define  SAVE_DISPL             4
#define  RSTR_DISPL             8

#define  CLEAR_LCD              0x10

#define  SLCD_1L                1
        /* save LCD 1st line */
#define  SLCD_2L                2
        /* save LCD 2nd line */
#define  RLCD_1L                4
        /* save LCD 1st line */
#define  RLCD_2L                8
        /* save LCD 2st line */

#define  START_ADDR_1L          0
#define  END_ADDR_1L            0x27
#define  START_ADDR_2L          0x40

#define  SET_DD_RAM_ADDR        0x80

        /*****          TONE RINGER MACROS              *****/
#define  TONE_RNG_ACT           0x80
#define  INTRNL_QUE_ACT         0x01
#define  EXTRNL_QUE_ACT         0x02
#define  RECALL_QUE_ACT         0x04
#define  OPR_HOOK_STAT          0x08
#define  EM_QUE_ACT             0x10
#define  BELL_ACT               0x20
#define  BELL_STP               4       /* 4*160msec = 0.64 sec */
#define  QUE_ACT        (INTRNL_QUE_ACT | EXTRNL_QUE_ACT | RECALL_QUE_ACT | EM_QUE_ACT)
#define  TONE_RNG_STP           12      /* 12*160msec = 1.92 sec */
#define  TONE_RNG_PHASE         2    /* 3 */  /* 2*1.92sec = 3.84 sec */
#define  TOTAL_RNG_STP          (TONE_RNG_STP * TONE_RNG_PHASE)
/*#define       TR_PAT          0x00ff*/
#define  XQ_TR_PAT              0x0fff  /* 12*160msec = 1.92 sec on, ...off */
#define  MQ_TR_PAT              0x0c63  /* 2*160msec = 0.32 sec on, 0.48 sec off, three times */
#define  IQ_TR_PAT              0x0f0f  /* 4bits * 160 msec = 0.64 sec on, 0.64sec off, 0.64sec on*/
#define  RQ_TR_PAT              0x003f  /* 6*160msec = 0.96 sec on, ...off */
                /* tone ringer pattern */
#define  FREQ_1         0x03     /* 4=800 Hz,3=1 KHz,2=1.3 KHz,1=2 KHz */
#define  FREQ_2         0x08     /* 5=667 Hz,6=571 Hz,7=500 Hz,8=445 Hz */
#define  TONE_RINGER_GAIN       0x02
        /* +12 db */

#define  HSMIC                  0x08
        /* handset microphone */
#define  SPMIC                  0x04
        /* speaker phone microphone */
#define  SPSKR                  0x02
        /* speaker phone speaker */
#define  HSSKR                  0x01
        /* handset speaker */

#define  NHN                    0x20
        /* numbers high nibble */
#define  INTRNL_1D_STIM         (NHN | (int)INTRNL_1ST_DGT)
        /* internal 1st digit stimulus */
#ifdef INTRNL_1ST_DGT_2
#define  INTRNL_2D_STIM         (NHN | (int)INTRNL_1ST_DGT_2)
#endif

#ifdef OPR_OPR_TALK
#define  OPRTR_NUM              (NHN | (int)OPERATOR_NUM)
#define  NO_OPR_DGT              2
#endif

#define  ACT_FAC1       3
#define  INACT_FAC1     1
#define  HOLD_FAC1      HOLD
#define  ACT_FAC2       (ACT_FAC1 << 4)
#define  INACT_FAC2     (INACT_FAC1 << 4)
#define  HOLD_FAC2      (HOLD << 4)

#define  NO_TONE                0
#define  DIALT                  1
#define  BUSYT                  2
#define  RBT                    3
#define  ICPTT                  4
#define  CMPOT                  5
#define  CNFRMT                 6
#define  SPECIALT               7
#define  ROT                    8

#define  REDIAL_ACT             0x01
#define  PROG_ACT               0x02
#define  XST_PROG               0x04
#define  TRNK_PROG              0x08
#define  DID_PROG               0x10
#define  SYSTEM_PROG            0x20
#define  PARK_IN_DIALLED        0x40
#define  PARK_OUT_DIALLED       0x80
#define  DIAL_ACTIVE            0x0100
#define  EMCAS_PROG             0x0200
#define  PRI_EXT_IN_TALK        0x0400
#define  IS_PRIMARY             0x0800
#ifdef SPEED_DIALLING
#define  SPEED_DIALLING_ACT     0x1000
#endif


#define  NO_PASSWORD_DGT        0x04
#ifdef NAFT
#define  PSWD_D2D1              0x2a
#define  PSWD_D4D3              0xaa
#else
#define  PSWD_D2D1              0x21
#define  PSWD_D4D3              0x43
#endif

#define  EOD_ID_KEY             PROG
        /* end of digit identifier key,when entering prog. destination */
#define  CO_ID_KEY              N_1
        /* CO trunk identifier key */
#define  TIE_ID_KEY             N_2
        /* TIE trunk identifier key */
#define  MAX_DEST_DGT           3
        /* maximum destination digits for programming */

#define  MAX_FUNC_DGT           3
        /* maximum function digits for programming */

#ifdef DAY_NIGHT_PARAMETERS_FILE

#define DAY_FILE_ID             N_1
#define NIGHT_FILE_ID           N_2

#endif

   /*****    maintenance & administration commands macros  *****/
                /***    extensions related      ***/

#define  MAC_CO_ACCESS          0x0
#define  MAC_CO_1ST0            0x1
#define  MAC_CO_2ST0            0x2
#define  MAC_TIE_ACCESS         0x3
#define  MAC_TIE_1ST0           0x4

#define  MAC_CO_GRP_ACCESS      0x5
#define  MAC_TIE_GRP_ACCESS     0x6

#define  MAC_CO_GRP0            0x7
#define  MAC_CO_GRP1            0x8
#define  MAC_CO_GRP2            0x9
#define  MAC_CO_GRP3            0xa
#define  MAC_CO_GRP4            0xb
#define  MAC_CO_GRP5            0xc

#define  MAC_TIE_GRP0           0xd
#define  MAC_TIE_GRP1           0xe
#define  MAC_TIE_GRP2           0xf
#define  MAC_TIE_GRP3           0x10
#define  MAC_TIE_GRP4           0x11
#define  MAC_TIE_GRP5           0x12

#define  MAC_EM_GRP0             0x13
#define  MAC_EM_GRP1             0x14
#define  MAC_EM_GRP2             0x15
#define  MAC_EM_GRP3             0x16
#define  MAC_EM_GRP4             0x17
#define  MAC_EM_GRP5             0x18
#define  MAC_EM_GRP6             0x19
#define  MAC_EM_GRP7             0x1a
#define  MAC_EM_GRP8             0x1b
#define  MAC_EM_GRP9             0x1c
#define  MAC_EM_GRP10            0x1d
#define  MAC_EM_GRP11            0x1e
#define  MAC_EM_GRP12            0x1f
#define  MAC_EM_GRP13            0x20
#define  MAC_EM_GRP_ACCESS       0x21   /* packet is sent, no use for now */

#define  MAC_NITE_OPR0          0x22
#define  MAC_NITE_OPR1          0x23
#define  MAC_NITE_OPR2          0x24
#define  MAC_NITE_OPR3          0x25

#define  MAC_CO_RSRV            0x26
#define  MAC_TIE_RSRV           0x27
        /* trunk reservation */

#define  MAC_CPU_ACCESS         0x28
        /* call pick up */

#define  MAC_CB_ACCESS          0x29
        /* call back */

#define  MAC_CAMPON_ACCESS      0x2a
#define  MAC_FM_ACCESS          0x2b
        /* follow me */

#define  MAC_DONTD_ACCESS       0x2c
        /* do not disturb active */

#define  MAC_CONF_ACCESS         0x2d
        /* conference */

#define  MAC_CALL_GEN            0x2e
#define  MAC_MALICIOUS_DTCT      0x2f

#define  MAC_HOT_LINE_ACCESS     0x30

#define  MAC_TO_HOT_LINE0        0x31
#define  MAC_TO_HOT_LINE1        0x32
#define  MAC_TO_HOT_LINE2        0x33
#define  MAC_TO_HOT_LINE3        0x34

#define  MAC_X_HOT_LINE0         0x35
#define  MAC_X_HOT_LINE1         0x36
#define  MAC_X_HOT_LINE2         0x37
#define  MAC_X_HOT_LINE3         0x38

#define  MAC_EXT_DID             0x39

#define  MAC_EXT_FEATURE         0x3a

#define  MAC_EXT_RELEASE         0x3b

#define  MAC_SIREN_ACCESS        0x3c

#define  MAC_AMPLIFIER_ACCESS    0x3d

#define  MAC_HOT_LINE            0x3e


#define  MA_XF_NO                0x3f


                /***    trunks related    ***/
#define  MAC_TRNK_OPR_ONLY      0x0
#define  MAC_TRNK_SERVICE       0x1

#define  MAC_TRNK_DID           0x8
#define  MAC_DID_INACTIVE       0x9

#define  MAC_T_HOT_LINE0        0xa
#define  MAC_T_HOT_LINE1        0xb
#define  MAC_T_HOT_LINE2        0xc
#define  MAC_T_HOT_LINE3        0xd

#define  MAC_T_TIMING_ACT       0x0e
#define  MAC_TRUNK_RELEASE      0x0f

#define  MAC_TRNK_GRP0          0x10

#define  MAC_TOLL_ACCESS        0x20    // leave space for 16 MAC_TRNK_GRP
#define  MA_TF_NO               0x21

                   /***  e&m related  ***/
#define   MAC_EM_OGT               0x0
#define   MAC_EM_ICT               0x1
#define   MAC_INTER_SIG_DGT_SEND   0x2
#define   MAC_ANSWER_TX_IC         0x3
#define   MAC_ANSWER_RX_OG         0x4
#define   MAC_IMMEDIATE_ACK        0x5
#define   MAC_WAIT_FOR_ACK         0x6
#define   MAC_EOS_NEEDED           0x7
#define   MAC_EM_TRNK_GRP0         0x8
#define   MAC_EM_TRNK_GRP1         0x9
#define   MAC_EM_TRNK_GRP2         0xa
#define   MAC_EM_TRNK_GRP3         0xb
#define   MAC_EM_TRNK_GRP4         0xc
#define   MAC_EM_TRNK_GRP5         0xd
#define   MAC_EM_TRNK_GRP6         0xe
#define   MAC_EM_TRNK_GRP7         0xf
#define   MAC_EM_TRNK_GRP8         0x10
#define   MAC_EM_TRNK_GRP9         0x11
#define   MAC_EM_IN_SERVICE        0x12
#define   MAC_EM_SEND9             0x13
#define   MAC_EM_DEST_TRNST        0x14
#define   MAC_EM_LPBACK            0x15
#define   MAC_EM_TIME1             0x16
#define   MAC_EM_TIME2             0x17
#define   MAC_EM_GRP               0x18 /* dummy, no packet is sent */
#define   MAC_EM_GRP0_DIGIT        0x19
#define   MAC_EM_GRP1_DIGIT        0x1a
#define   MAC_EM_GRP2_DIGIT        0x1b
#define   MAC_EM_GRP3_DIGIT        0x1c
#define   MAC_EM_GRP4_DIGIT        0x1d
#define   MAC_EM_GRP5_DIGIT        0x1e
#define   MAC_EM_GRP6_DIGIT        0x1f
#define   MAC_EM_GRP7_DIGIT        0x20
#define   MAC_EM_GRP8_DIGIT        0x21
#define   MAC_EM_GRP9_DIGIT        0x22

#define   MA_EF_NO                 0x23


                  /***  digital trunk related  ***/

#define   MAC_IN_SERVICE           (0x0 + MA_EF_NO)
#define   MAC_INCOMING_CH          (0x1 + MA_EF_NO)
#define   MAC_OGC                  (0x2 + MA_EF_NO)
#define   MAC_PTS                  (0x3 + MA_EF_NO)
#define   MAC_EOS                  (0x4 + MA_EF_NO)
#define   MAC_METERING             (0x5 + MA_EF_NO)
#define   MAC_OPR_RCL              (0x6 + MA_EF_NO)
#define   MAC_MAL_CALL             (0x7 + MA_EF_NO)
#define   MAC_IMM_ACK              (0x8 + MA_EF_NO)
#define   MAC_WTFR_ACK             (0x9 + MA_EF_NO)
#define   MAC_INTR_SIGNAL_DGT_SEND (0xa + MA_EF_NO)
#define   MAC_DGTL_TRNK_GRP        (0xb + MA_EF_NO)
#define   MAC_DGTL_TRNK_RLS        (0xc + MA_EF_NO)
#define   MAC_DGTL_TRNK_GRP0       (0xd + MA_EF_NO)

#define   MA_CF_NO                  0x1d        // leave space for 16 MAC_DGTL_TRNK_GRP

#define   MA_EF_CF_NO               (MA_EF_NO + MA_CF_NO)
                                    // 0x23 + 0x1d=0x40 no more space for another MAC_... !

                /***  system's related  ***/

#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
#define  OP_FIRST_ZERO_ON_CO            8       // opr -> opr  sys_prog code-8
#endif
#define  CHANGE_PASSWORD                9       // opr -> opr  sys_prog code-9
#ifdef PRE_INSTALL
#define  MAC_OPR_RCVR_GAIN              11      // opr -> opr  sys_prog code-11
#endif
#ifdef PARAMETER_DOWN_UP_LOAD
#define  MAC_DOWN_LOAD_PARAMETERS_2     21      // opr -> opr  sys_prog code-21, for opr->sys always ..._1 is sent
#define  MAC_UP_LOAD_PARAMETERS_2       22      // opr -> opr  sys_prog code-22, for opr->sys always ..._1 is sent
#define  MAC_DOWN_LOAD_PARAMETERS_3     41      // opr -> opr  sys_prog code-41, for opr->sys always ..._1 is sent
#define  MAC_UP_LOAD_PARAMETERS_3       42      // opr -> opr  sys_prog code-42, for opr->sys always ..._1 is sent
#endif

#define  MAC_ONLINE_BILLING_ACT                 0x00    // opr -> sys  sys_prog code-0

#ifdef PARAMETER_DOWN_UP_LOAD
#define  MAC_DOWN_LOAD_PARAMETERS               0x01    // sys -> opr
#define  MAC_DOWN_LOAD_PARAMETERS_1             0x01    // opr -> sys  sys_prog code-1
#define  MAC_UP_LOAD_PARAMETERS_1               0x02    // opr -> sys  sys_prog code-2
#endif

#define  MAC_TRUNK_STATE_PRINT                  0x03    // opr -> sys  sys_prog code-3
#define  MAC_EXT_STATE_PRINT                    0x04    // opr -> sys  sys_prog code-4
#define  MAC_SET_DATE_FOR_COMPLETE_BILLING      0x02    // sys -> opr
#define  MAC_COMPLETE_BILLING_PACKET_SIZE       0x03    // sys -> opr
#define  MAC_COMPLETE_BILLING_RECORD            0x04    // sys -> opr
#define  MAC_BILLING_PACKET_SIZE                0x05    // sys -> opr
#define  MAC_BILLING_RECORD                     0x06    // sys -> opr

#define  MAC_COMPLETE_MALICIOUS_PACKET_SIZE     0x00    // sys -> opr
#define  MAC_COMPLETE_MALICIOUS_RECORD          0x07    // sys -> opr

#ifdef CONFERENCE_ACT
#define  MAC_CONFERENCE_STATE_PRINT             0x07    // opr -> sys  sys_prog code-7
#endif

#define MAC_FDN1                                0x08    // opr -> sys
#define MAC_FDN2                                0x14    // opr -> sys

#ifdef SI_DEBUG_ON_U
#define  MAC_SI_DEBUG_ON_U                      0x09
#endif

#ifdef AUTO_UPLOAD
#define  MAC_UPLOAD_REQUEST                     0x0a
#endif

#define MAC_SS                                  0x0b

#ifdef PRE_INSTALL
#define  MAC_ANALOG_TRUNK_GAIN                  0x0c    // opr -> sys  sys_prog code-12
#define  MAC_EXT_CODEC_GAIN                     0x0d    // opr -> sys  sys_prog code-13
#define  MAC_EM_TRUNK_GAIN                      0x0e    // opr -> sys  sys_prog code-14
#endif

#ifdef REPORT_BULK_BILLING
#define  MAC_BULK_BILLING_PACKET_SIZE           0x0f
#define  MAC_BULK_BILLING_RECORD                0x10
#endif

#ifdef MALICIOUS_PRINT_ON_U
#define  MAC_MALICIOUS_PACKET_SIZE              0x11
#define  MAC_MALICIOUS_RECORD                   0x12
#endif

#ifdef DISPLAY_EM_STATUS
#define MAC_DISPLAY_EM_STATUS                   0x13
#endif

#if(NO_OPRTR > 1)
#define  MAC_UPLOAD_ACCEPT                      0x14    // sys -> opr
#define  MAC_UPLOAD_REJECT                      0x15    // sys -> opr
#endif

#ifdef ALARM_ENABLE
#define MAC_ALARM                               0x16    // sys -> opr


#if ((defined MANUAL_TEST) || (defined DEP_MANUAL_TEST))
#define MAC_EXT_MANUAL_TEST                     0x17    // opr -> sys
#define MAC_EXT_MANUAL_TEST_ANS                 0x17    // sys -> opr
#define MAC_EXT_STATE_REQUEST                   0x18    // opr -> sys
#define MAC_EXT_STATE_REQUEST_ANS               0x18    // sys -> opr
#define MAC_R1_TEST_REQUEST                     0x19    // opr -> sys
#define MAC_R1_TEST_REQUEST_ANS                 0x19    // sys -> opr
#endif

#define MAC_MALICIOUS_FILE_VIEWED               0x1a    // opr -> sys
#endif

#define MAC_CLOSE_DBRF                          0x1a    // sys -> opr

#ifdef EEPROM
#define MAC_SET_DEFAULT_PARAM                   0x1b
#define MAC_EEPROM_LOAD_START                   0x1c
#define MAC_EEPROM_LOAD_FINISHED                0x1d
#endif

#ifdef DISPLAY_CAS3_STATUS
#define MAC_DISPLAY_CAS3_STATUS                 0x1e
#define MAC_DISPLAY_3BIT_STATUS                 0x1f
#endif

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
#define MAC_CAS3_HISTORICAL_SIGNAL_REPORT       0x21    // sys -> opr
#define MAC_CAS3_HISTORICAL_COUNT_REPORT        0x22    // sys -> opr
#define MAC_CAS3_HISTORICAL_LOOSE_REQUEST       0x21    // opr -> sys  sys_prog code-33
#define MAC_CAS3_HISTORICAL_REQUEST             0x22    // opr -> sys  sys_prog code-34
#endif
#endif

#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
#define MAC_EM_HISTORICAL_SIGNAL_REPORT         0x24    // sys -> opr
#define MAC_EM_HISTORICAL_COUNT_REPORT          0x25    // sys -> opr
#define MAC_EM_HISTORICAL_REQUEST               0x25    // opr -> sys  sys_prog code-37
#endif
#endif

#ifdef DISPLAY_CAS1_STATUS
#define MAC_DISPLAY_CAS1_STATUS                 0x26
#define MAC_DISPLAY_1BIT_STATUS                 0x27
#endif

#define  MAC_HDLC_RS232_CHECK_LINK              0x2b

#define  MAC_THIS_SYSTEM_IS_HOT                 0x2c
#define  MAC_HOT_SYSTEM_CHECK                   0x2d
#define  MAC_HOT_SYSTEM_ANSWER                  0x2e
#define  MAC_HTBY_LINK_IS_OK                    0x2f
#define  MAC_HTBY_LINK_IS_NOT_OK                0x30

#ifdef OPR_UPLOAD_WITH_TIMER
#define  MAC_UPLOAD_FINISHED                    0x31
#endif

#ifdef DEP_MANUAL_TEST
#define MAC_EM_MANUAL_TEST                      0x33    // opr -> sys
#define MAC_EM_MANUAL_TEST_ANS                  0x33    // sys -> opr
#define MAC_DTMF_MANUAL_TEST                    0x34    // opr -> sys
#define MAC_DTMF_MANUAL_TEST_ANS                0x34    // sys -> opr
#define MAC_SWCH_MANUAL_TEST                    0x35    // opr -> sys
#define MAC_SWCH_MANUAL_TEST_ANS                0x35    // sys -> opr
#define MAC_DIGITAL_TRUNK_MANUAL_TEST           0x36    // opr -> sys
#define MAC_DIGITAL_TRUNK_MANUAL_TEST_ANS       0x36    // sys -> opr
#endif //DEP_MANUAL_TEST

#define MAC_SWITCHOVER_PROGRAM                  0x37    // opr -> sys  sys_prog code-56

#define MAC_OFFICE_CODE_PART1                   0x3a    // opr -> sys
#define MAC_OFFICE_CODE_PART2                   0x3b    // opr -> sys
#define MAC_SS7_PACKET_PRINT                    0x3c    // opr -> sys  sys_prog code-60
#define MAC_TIME                                0x3d    // opr -> sys

#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
#define MAC_CAS1_HISTORICAL_SIGNAL_REPORT       0x3d    // sys -> opr
#define MAC_CAS1_HISTORICAL_COUNT_REPORT        0x3e    // sys -> opr
#define MAC_CAS1_HISTORICAL_REQUEST             0x3e    // opr -> sys  sys_prog code-62
#endif
#endif

#define MAC_EXIT_PROGRAM                        0x3f    // opr -> sys  sys_prog code-63

#define MA_SF_NO                                0x40

#define  MA_X_P                 0x040        /* MA extension programming */
#define  MA_COT_P               0x080        /* MA CO trunk programming */
#define  MA_TIET_P              0x100        /* MA TIE trunk programming */
#define  MA_SYS_P               0x200        /* MA system programming */
#define  MA_EM_CAS_P            0x400        /* E&M and CAS3 programming */
#define FEATURE_ACTIVATION      0x800        /* feature activation bit */

#ifdef ALARM_ENABLE
#ifdef NAFT
#define ALARM_OPR                       1
#else
#define ALARM_OPR                       0
#endif
#define SUBSCRIBER_FAULT                0x0000  // msb 0 indicates subscriber-oriented message, subscriber number is lsb 14 bits
#define SUBSCRIBER_FAULT_REMOVED        0x4000

#define EM_FAULT                        0x8000  // msb 1 indicates non-subscriber-oriented message, combinations of next 4 bits used for now, remaining 11 bits used for "port" number
#define EM_FAULT_REMOVED                0x8400
#define OOS                             0x8800
#define LINE_CARD_FAULT                 0x9000
#define LINE_CARD_FAULT_REMOVED         0xa000
#define REMOTE_OOS                      0xb000
#define BILLING_RECORD_PROBLEM          0xb800
#define ALARM_PANEL_REPORT              0xbc00
#define CB_FAULT                        0xc000
#define CB_FAULT_REMOVED                0xc800
#define OOS_REMOVED                     0xd000
#define OM_SYNC_LOSS                    0xd800
#define MTP_L2_FAULT                    0xe000
#define MTP_L2_FAULT_REMOVED            0xe400
#define REMOTE_OOS_REMOVED              0xe800
#define HOT_STANDBY_ALARM               0xf000
#define CONNECTION_ALARM                0xf800
#define V52_FAULT                       0x8000
#define V52_FAULT_REMOVED               0x8400

// sub macros for HOT_STANDBY_ALARM
#define MASTER_STARTED_AS_HOT                           0x0000
#define MASTER_STARTED_AS_STANDBY                       0x0001
#define SLAVE_STARTED_AS_HOT                            0x0002
#define SLAVE_STARTED_AS_STANDBY                        0x0003
#define MASTER_CHANGED_TO_HOT                           0x0004
#define MASTER_CHANGED_TO_STANDBY                       0x0005
#define SLAVE_CHANGED_TO_HOT                            0x0006
#define SLAVE_CHANGED_TO_STANDBY                        0x0007
#define MASTER_LOST_CONNECTION_WITH_SLAVE               0x0008
#define SLAVE_LOST_CONNECTION_WITH_MASTER               0x0009
#define MASTER_HAS_NO_CONNECTION_WITH_SLAVE             0x000a
#define SLAVE_HAS_NO_CONNECTION_WITH_MASTER             0x000b
#define MASTER_ESTABLISHED_CONNECTION_WITH_SLAVE        0x000c
#define SLAVE_ESTABLISHED_CONNECTION_WITH_MASTER        0x000d
#define POWER0_FAULT                                    0x0400
#define POWER1_FAULT                                    0x0200
#define POWER_FAULT_REMOVED                             0x0100

// sub macros for CONNECTION_ALARM
#define RSU_CONNECTION_LOST                             0x0000
#define RSU_CONNECTION_ESTABLISHED                      0x0400

#define ALARM_FILE                      "alarm.log"
#define MAX_LOG_FILE_SIZE               1000000L
#endif // ALARM_ENABLE

#define  ERR_CODE               0x0fff

#define  ICQ_1ST_THRESHOLD      5
        /* incoming queue 1st threshold */
#define  ICQ_2ND_THRESHOLD      10
        /* incoming queue 2nd threshold */

#define  XLS_BIT_POS            0x01
        /* EXTRNL LED status bit position */

#define  ILS_BIT_POS            0x02
        /* INTERNAL LED status bit position */

#define  RLS_BIT_POS            0x04
        /* RECALL LED status bit position */

#define  PLS_BIT_POS            0x08
        /* PROG LED status bit position */

#define  NLS_BIT_POS            0x10
        /* NIGHT LED status bit position */
#ifdef EM_CONT_SEP_OPR_Q
#define  EMLS_BIT_POS             0x20
#endif

#define  BLINK1_CNTR            5
#define  BLINK2_CNTR            1


#ifdef PRE_INSTALL
#define  SYS_PROG_DGTS          2
#else
#define  SYS_PROG_DGTS          1
#endif


#define  OHH_BIT_POS                    0x01
#define  KBS_T_BIT_POS                  0x20


#ifdef DETAIL_BILLING

#ifdef LONG_BILLING_FILENAME
#define BILLING_FILE_NUMBER_SIZE_STRING "6"
#define BILLING_FILE_NAME_SIZE  10
#define FIRST_DETAIL_BILLING_FILE       "dbrf000001"
#else
#define BILLING_FILE_NUMBER_SIZE_STRING "4"
#define BILLING_FILE_NAME_SIZE  8
#define FIRST_DETAIL_BILLING_FILE       "dbrf0001"
#endif
//#define BILLING_FILE_NUMBER_SIZE  (BILLING_FILE_NAME_SIZE-FILE_NUMBER_IN_FILE_NAME)

#define INSTALLATION_NAME_RECORD        0x11
#define DATE_RECORD                     0x22
#define EXT_EXT_RECORD                  0x33
#define EXT_TRUNK_RECORD                0x44
#define CB_EXT_RECORD                   0x66
#define CB_TRUNK_RECORD                 0x77

#define TRUNK_EXT_RECORD                0x35
#define TRUNK_TRUNK_RECORD              0x46

#define OPR_EXT_RECORD                  0x80
#define OPR_TRUNK_RECORD                0x86    // 0x85

#define NO_DETAIL_BILLING_RECORDS       512

#define DETAIL_BILLING_NAME_FILE        "dbfn.bfn"
#define FILE_NUMBER_IN_FILE_NAME        4
/* start of file number in file name string,0001 starts at position 4 in */
/* dbrf0001. */

#define RECORD_TYPE_POSITION            0
#define RECORD_SIZE_POSITION            1
#define RECORD_FIRST_BYTE               2

#define DATE_RECORD_SIZE                3
/* year BCD: 1 byte,month BCD: 1 byte,day BCD: 1 byte */

#define POSITION_NUMBER_DIGIT_IN_DB     3
/* position of the number of digits in db[] */

#define POSITION_D4D3_D2D1              4
/* position of d4d3d2d1 in db[] */

// maximum theoretical billing record size written to file (in bytes)
#define MAX_DETAIL_BILLING_RECORD       52
// record's type:1,
// record's size:1,
// source:2,
// destination:2,
// start time:3,
// time of conversation:2,
// number of digits:1,
// digits:18,  36 digits max
// number of source digits:1,
// source digits:10,  20 digits max
// pulse count:2,
// src directory number:4,
// dst directory number:4,
// check sum:1

// maximum theoretical billing information size rcved from system (in unsigned short)
#define MAX_DETAIL_BILLING_INFO_SIZE         23
// source 1,
// destination 1,
// duration 1,
// REPORT_BULK_IN_DETAIL(adds 1 for pulse count),
// SEND_BILLING_TIME_FROM_SYS_TO_OPR(adds 3 for date & time),
// number of digits 1,
// 9 for digits, each packing 4 digits for a total of 36 digits
// number of source digits 1,
// 5 for source digits, each packing 4 digits for a total of 20 digits

// minimum theoretical billing information size rcved from system (in unsigned short)
#define MIN_DETAIL_BILLING_INFO_SIZE          3
// source 1,
// destination 1,
// duration 1,
// no REPORT_BULK_IN_DETAIL(adds 1)
// no SEND_BILLING_TIME_FROM_SYS_TO_OPR(adds 3)
// ext_ext record, so no destination digit info

#endif // DETAIL_BILLING

#ifdef REPORT_BULK_BILLING

#define BULK_BILLING_RECORD_SIZE        2
/* bulk billing records size in int, first is extension number (with flag as
   the msb), second is the counter (either std or local) */

#define WHICH_COUNT_MASK        0xc000
#define LOCAL_COUNT_FLAG        0x0000
#define LOCAL_EXTRA_COUNT_FLAG  0x4000
#define STD_COUNT_FLAG          0x8000
#define SERVICE_COUNT_FLAG      0xc000
#ifdef R1_SIGNALING
#define INTERNATIONAL_COUNT_FLAG      0xc000
#endif

#define BULK_BILLING_FILE               "/ktbill/kiatel.bil"
#define BULK_BILLING_BACKUP_FILE        "/ktbill/kiatel.bak"
#define BULK_BILLING_DIRECTORY          "/ktbill"
void wbbr(unsigned int,unsigned int);
unsigned int cbbf(void);
void bbf_backup(void);
void force_bbf_backup(void);
#define  BACKUP_INTERVAL                2000
        /* backup time interval = 160msec * 2000 = 320 sec =5min,20sec */

#endif

#define  UPDATE_TIME_INTERVAL   30      // 160msec * 30=4.8 sec

#ifdef MALICIOUS_PRINT_ON_U

#define MAX_MALICIOUS_RECORD_SIZE  25
/* bulk billing record's size in integer */
/* maliciouser and malicioused */
#define MALICIOUS_FILE               "/ktbill/kiatel.mlc"
#define MALICIOUS_DIRECTORY          "/ktbill"
#endif



#define NIL     0xffff

#define NO_LOAD_PARM_FILE       2       // day file and night file
/* number of load parameters files */

#if(NO_CO < 10)
#define NO_DGT_FOR_CO           1
#elif(NO_CO < 100)
#define NO_DGT_FOR_CO           2
#else
#define NO_DGT_FOR_CO           3
#endif

#if(NO_TIE < 10)
#define NO_DGT_FOR_TIE          1
#elif(NO_TIE < 100)
#define NO_DGT_FOR_TIE          2
#else
#define NO_DGT_FOR_TIE          3
#endif

#define NO_DGT_FOR_OPR_DIAL_CO  2       // 2 for for example dialling 01,02,... 3 for for example dialling 001,002,...

#ifdef TRUNK_TRUNK_CONNECTION
#define CO_TRUNK_RLS            CO_GRP6
/* for releasing CO trunk engaged in trunk trunk connection */
#define TIE_TRUNK_RLS           TIE_GRP6
/* for releasing TIE trunk engaged in trunk trunk connection */
#endif

                        /****  types definition  ****/

#if COMPILER == GCC
#define SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
#endif
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
typedef unsigned long long SDNST;       // Subscriber Directory Number Start Type
#define SDNS_FS         "%lld"          // SDNS Format String
#define SDNS_NBB        5               // SDNS No of Bytes in Billing
#define SDNS_BF         "lld"           // SDNS Billing Format
#define atosdnst(p)     strtoll(p,0,10) // atoll(p)
#else
typedef unsigned long SDNST;            // Subscriber Directory Number Start Type
#define SDNS_FS         "%ld"           // SDNS Format String
#define SDNS_NBB        4               // SDNS No of Bytes in Billing
#define SDNS_BF         "ld"            // SDNS Billing Format
#define atosdnst(p)     atol(p)
#endif

typedef unsigned char BYTE;

typedef struct{
        unsigned int t:4;
#ifdef BEHNAMIR
        unsigned int i:16;
#else
        unsigned int i:12;
#endif
}INDEX_TYPE;

#if 0
typedef struct{
        BYTE    lptr;
        BYTE    rptr;
        INDEX_TYPE   it;
        int     tdiv;
        int     i;      /* index to timer pointer table */
}TMR;
#endif //0

typedef struct{
        INDEX_TYPE pt;
        int stim;
}CPR;  /* call processing record */

typedef struct{
        INDEX_TYPE wt;  /* work,type */
        INDEX_TYPE pt;  /* place,type */
}MAR;  /* maintenance & administration record */

typedef struct{
        INDEX_TYPE wt;  /* work,type */
        unsigned int parm;  /* parameter */
        BYTE sys;       /* which system */
        BYTE htby;      /* which system hot or standby ?*/
}OMAR;  /* operator's MA record */

typedef struct{
        BYTE q[TX_QUE_LEN];

#ifdef PARAMETER_DOWN_UP_LOAD
        int qe;
        int qf;
#else
        BYTE qe;
        BYTE qf;
#endif

        BYTE ob[FIFO_SIZE+3];
        /* 1 byte for address,1 byte for control & 1 byte for size */
        BYTE obs;
        BYTE n2;       /* number of retransmition */
}TX_RCRD;

typedef struct{
        BYTE q[RX_QUE_LEN];

#ifdef PARAMETER_DOWN_UP_LOAD
        int qe;
        int qf;
#else
        BYTE qe;
        BYTE qf;
#endif
#ifdef CHECKSUM
        BYTE ib[FIFO_SIZE+4];
        // one extra byte for checksum
#else
        BYTE ib[FIFO_SIZE+3];
        /* 1 byte for address,1 byte for control & 1 byte for size */
#endif
        BYTE ibs;
}RX_RCRD;

typedef struct{
        BYTE fe;  /* frame expected */
        BYTE nfts;  /* next frame to send */
}TRW;  /* tx,rx window */

typedef struct{
        BYTE sspc;  /* sync set peg counter */
        BYTE srpc;  /* sync reset peg counter */
        BYTE snipc;  /* system network initialization peg counter */
        BYTE na;  /* network address */
        unsigned int soon;  /* sync out operator night */
}OPR_RCRD;

typedef struct{
        BYTE s; /* section */
        int i; /* index */
#ifdef OPR_PRI_CONNECTION
        BYTE *dgt;
        BYTE no_dgt;
        unsigned int userside_pri:1;
        unsigned int og:1;
#endif
}STAT_IN_OPR;   /* status in operator */

typedef struct{
        BYTE info;      /* information */
        BYTE op;        /* operation */
}LCD_RCRD;

typedef struct{
        BYTE state;
        BYTE tidx;
        INDEX_TYPE s;   /* source */
        INDEX_TYPE d;   /* destination */
        BYTE fs;        /* facilities status */
                        /* b0-b3:fs soure,b4-b7:fs destination */
                        /* active=OFF_HOOK or trunk seized, */
                        /* inactive=ON_HOOK or trunk idle */
                        /* HOLD */
        BYTE scdc;      /* send count,dialed digits count */
        BYTE xd;        /* expected digits */
        unsigned int mf; /* mark field,b0=redial active */
        /* b1=program active,b2=ext. programming,b3=trunk programming */
        /* b4=DID programming,b5=system programming,b6=park in dialled */
        /* b7=park out dialled,b8=dial active b9=  b10=pri ext in talk */
        BYTE d2d1;
        BYTE d4d3;
        BYTE d6d5;
        BYTE d8d7;
        BYTE d10d9;
        BYTE d12d11;
        BYTE d14d13;
        BYTE d16d15;
        BYTE d18d17;
        BYTE d20d19;
}OPR_CALL_REC;

typedef struct{
        BYTE rp;        /* right pointer */
        BYTE lp;        /* left pointer */
        INDEX_TYPE pt;
        BYTE no_dgt;
        BYTE src_dgt[20];
}RE_EX_QUE_REC; /* recall & external queue records */

typedef struct{
        BYTE rp;        /* right pointer */
        BYTE lp;        /* left pointer */
        int i;          /* index */
        BYTE no_dgt;
        BYTE src_dgt[20];
}IN_QUE_REC;    /* internal queue record */

typedef struct dattim{
        unsigned int yr;
        BYTE mo;
        BYTE da;
        BYTE hr;
        BYTE mi;
        BYTE se;
} DATTIM;

struct LED_POS {
        BYTE x1;
        BYTE y1;
        BYTE x2;
        BYTE y2;
        CHAT_TYPE attr;
};

typedef void (*FPCP)(INDEX_TYPE);
       /* FPCP is function pointer type for call processing */

#ifdef DETAIL_BILLING

typedef struct{
        BYTE s;  /* size */
        unsigned int rp;  /* right pointer */
        unsigned int lp;  /* left pointer */
        BYTE ri[MAX_DETAIL_BILLING_RECORD+1];  /* record's information */
        // add one extra byte to allow for one possible added prev_nibble
        // when copying digits from db[][] to ri[]
}DBR;  /* detail billing record */
#endif

                /****  functions' prototypes  ****/

#ifdef OPERATOR_PC
#if (COMPILER == GCC)
void rtc(void);
#else
void interrupt rtc(void);
#endif
void kbs_m(void);void kbs_t(void);
void cfcs(FILE *);void led_set(BYTE);void led_reset(BYTE);
void intrrset(unsigned int);

#else

void rtc(void);void kbs(void);
void led_set(unsigned int);void led_reset(unsigned int);
#endif


#if ( (PARAMETER_DOWN_UP_LOAD == DEFINE_VALUE) || (DETAIL_BILLING == DEFINE_VALUE) )
OMAR frommaq(void);
#endif
#ifdef PARAMETER_DOWN_UP_LOAD
void csp(OMAR);void unp(void);
#endif


#ifdef OPERATOR_PC

#if ( (PARAMETER_DOWN_UP_LOAD == DEFINE_VALUE) || (DETAIL_BILLING == DEFINE_VALUE) )
void omap(void);
#endif

/*
void wdr(FILE *);
void winr(FILE *);
FILE *cdbrf(void);
*/
void wdr(int handle);
void winr(int handle);
BYTE cdbrf(int *handle);
unsigned int kt_file_close(int handle);

int vfcs(FILE *);
int vpf(FILE *,BYTE);

#endif

void initialize(void);void scheduler(void);void error(void);void cp(void);
CPR fromcpq(void);void lcds(void);void rfiq(BYTE);int fromiq(void);
int torq(INDEX_TYPE);int fkp(void);int fromxq(void);
long hex2bcd(long a);
void int2lcd(long);

int tocpq(INDEX_TYPE,int);void ddt_ad(void);void leds(void);int fromrq(void);
void init_lcd(void);void str2lcd(char *);void init_t0_srl(void);
void rtnexe(BYTE,INDEX_TYPE);int tolcdq(BYTE,BYTE);void trscn(void);
void tron(void);void troff(void);void init_tr(void);void rtp(INDEX_TYPE);
int fcp(void);void opoh(void);void rlstrnk(INDEX_TYPE);void dtr(void);
void ptiq(BYTE);int toiq(int);void ptxq(BYTE);void ptrq(BYTE);
int toxq(INDEX_TYPE);void rtpns(INDEX_TYPE);void riqr(BYTE);void rfrq(BYTE);
void hex2lcd(BYTE);void rrxqr(BYTE);void rfxq(BYTE);char h2c(BYTE);
void lj0(void);int fd(void);
void displscrn(BYTE,BYTE,CHAT_TYPE,CHAT_TYPE);
void ud(void);void clear_screen(void);
int vmode(void);void cursor(int,int);void curr_cursor(int *,int *);
int cursor_type(void);void set_cursor_type(int);int get_char(void);
int scroll_lock(void);void vpoke(unsigned,unsigned,unsigned);
int vpeek(unsigned,unsigned);void init_intr(void);void quit(int);
void errorcp(void);void b2led(BYTE);void pc_timer_init(void);
BYTE get_c(void);DATTIM *get_dt(DATTIM *);
BYTE get_and_check_dt(DATTIM *);
unsigned int ffdbr(void);
void rdbr(unsigned int a);
void toadbq(unsigned int a);
BYTE rdb(BYTE,unsigned int a);
void display_billing(unsigned int a);
void write_billing_record_to_file(unsigned int a);
unsigned int cdbfn(void);
void dbrc(void);
int cpf(char *,BYTE);
void toal(void);void cprti(void);void ocni(void);
void flush_lcdq(void);void sys_func(void);void oam(void);void ohsa(void);
int reset_mouse(void);void init_file(void);void init_layo(void);
void remove_mouse_ptr(void);
void display_mouse_ptr(void);BYTE lcdsts(void);
void lcdcmd(BYTE);void putlcd(BYTE);int fromlcdq(void);int vox(void);
BYTE key_fkp(void);int ffiqr(void);int ffrxqr(void);
int tomaq(INDEX_TYPE,unsigned int,BYTE);void gcp(void);


void rtn0(void);
void rtn1(INDEX_TYPE);void rtn2(INDEX_TYPE);
void rtn3(INDEX_TYPE);void rtn4(INDEX_TYPE);
void rtn5(INDEX_TYPE);void rtn6(INDEX_TYPE);
void rtn7(INDEX_TYPE);void rtn8(INDEX_TYPE);
void rtn9(INDEX_TYPE);void rtn10(INDEX_TYPE);
void rtn11(INDEX_TYPE);void rtn12(INDEX_TYPE);
void rtn13(INDEX_TYPE);void rtn14(INDEX_TYPE);
void rtn15(INDEX_TYPE);void rtn16(INDEX_TYPE);
void rtn17(INDEX_TYPE);void rtn18(INDEX_TYPE);
void rtn17c(INDEX_TYPE);
void rtn17a(INDEX_TYPE);void rtn17b(INDEX_TYPE);
void rtn20a(INDEX_TYPE);void rtn20b(INDEX_TYPE);
void rtn19(INDEX_TYPE);void rtn20(INDEX_TYPE);
void rtn21(INDEX_TYPE);void rtn22(INDEX_TYPE);
void rtn23(INDEX_TYPE);void rtn24(INDEX_TYPE);
void rtn25(INDEX_TYPE);void rtn26(INDEX_TYPE);
void rtn27(INDEX_TYPE);void rtn28(INDEX_TYPE);
void rtn29(INDEX_TYPE);void rtn30(INDEX_TYPE);
void rtn31(INDEX_TYPE);void rtn32(INDEX_TYPE);
void rtn33(INDEX_TYPE);void rtn34(INDEX_TYPE);
void rtn34a(INDEX_TYPE);
void rtn34b(INDEX_TYPE);
#ifdef OPR_OPR_TALK
void rtn1_1(INDEX_TYPE);void rtn1_2(INDEX_TYPE);
void rtn2_1(INDEX_TYPE);void rtn8_1(INDEX_TYPE);
void rtn10_1(INDEX_TYPE);void rtn11_1(INDEX_TYPE);
void rtn12_1(INDEX_TYPE);
void rtn17_1(int,int);void rtn17_2(INDEX_TYPE);
void rtn24_1(INDEX_TYPE);void rtn24_2(INDEX_TYPE);
void rtn25_1(INDEX_TYPE);void rtn34_1(INDEX_TYPE);
void rtn41_1(INDEX_TYPE);
#endif
void rtn35(INDEX_TYPE);void rtn36(INDEX_TYPE);
void rtn37(INDEX_TYPE);void rtn38(INDEX_TYPE);
void rtn39(INDEX_TYPE);
void rtn40(INDEX_TYPE);void rtn41(INDEX_TYPE);
void rtn41a(INDEX_TYPE);
void rtn41b(INDEX_TYPE);
void rtn42(INDEX_TYPE);void rtn43(INDEX_TYPE);
void rtn44(INDEX_TYPE);void rtn45(INDEX_TYPE);
void rtn46(INDEX_TYPE);void rtn47(INDEX_TYPE);
void rtn48(INDEX_TYPE);void rtn49(INDEX_TYPE);
void rtn50(INDEX_TYPE);void rtn51(INDEX_TYPE);
void rtn52(INDEX_TYPE);void rtn53(INDEX_TYPE);
void rtn54(INDEX_TYPE);void rtn55(INDEX_TYPE);
void rtn56(INDEX_TYPE);void rtn57(INDEX_TYPE);
void rtn58(INDEX_TYPE);void rtn59(INDEX_TYPE);
void rtn60(INDEX_TYPE);void rtn61(INDEX_TYPE);
void rtn62(INDEX_TYPE);void rtn63(INDEX_TYPE);
void rtn64(INDEX_TYPE);void rtn65(INDEX_TYPE);
void rtn66(INDEX_TYPE);void rtn67(INDEX_TYPE);
void rtn68(INDEX_TYPE);void rtn69(INDEX_TYPE);
void rtn70(INDEX_TYPE);void rtn71(INDEX_TYPE);
void rtn72(INDEX_TYPE);void rtn73(INDEX_TYPE);
void rtn74(INDEX_TYPE);void rtn75(INDEX_TYPE);
void rtn76(INDEX_TYPE);void rtn77(INDEX_TYPE);
void rtn78(INDEX_TYPE);void rtn79(INDEX_TYPE);
void rtn79a(INDEX_TYPE);
void rtn80(INDEX_TYPE);void rtn81(INDEX_TYPE);
void rtn82(INDEX_TYPE);void rtn83(INDEX_TYPE);
void rtn84(INDEX_TYPE);void rtn85(INDEX_TYPE);
void rtn86(INDEX_TYPE);void rtn87(INDEX_TYPE);
void rtn88(INDEX_TYPE);void rtn89(INDEX_TYPE);
void rtn90(INDEX_TYPE);void rtn91(INDEX_TYPE);
void rtn92(INDEX_TYPE);void rtn93(INDEX_TYPE);
void rtn94(INDEX_TYPE);void rtn95(INDEX_TYPE);
void rtn96(void);
void rtn97(INDEX_TYPE);
void rtn97_1(INDEX_TYPE);
void rtn97_2(INDEX_TYPE);
void rtn97_3(INDEX_TYPE);
void rtn98(INDEX_TYPE);void rtn99(INDEX_TYPE);
void rtn100(INDEX_TYPE);void rtn101(INDEX_TYPE);

//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
void rtn101_1(INDEX_TYPE);void rtn101_2(INDEX_TYPE);
void rtn102_1(INDEX_TYPE);void rtn102_2(INDEX_TYPE);
#endif
//night upload finish

void rtn102(INDEX_TYPE);void rtn103(INDEX_TYPE);
void rtn104(INDEX_TYPE);void rtn105(INDEX_TYPE);
void rtn104a(INDEX_TYPE);
void rtn105a(INDEX_TYPE);void rtn105c(INDEX_TYPE);
void rtn105d(INDEX_TYPE);
void rtn104b(INDEX_TYPE);
void rtn105b(INDEX_TYPE);
void rtn104c(INDEX_TYPE);
void rtn104d(INDEX_TYPE);
void rtn106(INDEX_TYPE);void rtn107(INDEX_TYPE);
void rtn108(INDEX_TYPE);void rtn109(INDEX_TYPE);
void rtn110(INDEX_TYPE);void rtn111(INDEX_TYPE);void rtn111a(INDEX_TYPE);
void rtn112(INDEX_TYPE);
void rtn115(INDEX_TYPE);void rtn116(INDEX_TYPE);
void rtn117(INDEX_TYPE);void rtn118(INDEX_TYPE);
void rtn119(INDEX_TYPE);void rtn120(INDEX_TYPE);
void rtn121(INDEX_TYPE);void rtn122(INDEX_TYPE);
void rtn123(INDEX_TYPE);void rtn124(INDEX_TYPE);
void rtn125(INDEX_TYPE);void rtn126(INDEX_TYPE);
void rtn127(INDEX_TYPE);void rtn128(INDEX_TYPE);
void rtn129(INDEX_TYPE);void rtn130_1(INDEX_TYPE);
void rtn130_2(INDEX_TYPE);
void rtn130_3(INDEX_TYPE);
void rtn131(INDEX_TYPE);void rtn132(INDEX_TYPE);
void rtn133(INDEX_TYPE);void rtn134(INDEX_TYPE);
void rtn135(INDEX_TYPE);void rtn136(INDEX_TYPE);
void rtn137(INDEX_TYPE);
void rtn138(void);
void rtn139(INDEX_TYPE);
#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
void rtn140(INDEX_TYPE);
void rtn141(INDEX_TYPE);
#endif
#endif
#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
void rtn142(INDEX_TYPE);
void rtn143(INDEX_TYPE);
#endif
#endif
#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
void rtn140a(INDEX_TYPE);
void rtn141a(INDEX_TYPE);
#endif
#endif
#ifdef NEW_DEP
void rtn113(INDEX_TYPE pt);
#endif
void rtn114(INDEX_TYPE pt);
void rtn148(INDEX_TYPE pt);
void rtn149(INDEX_TYPE pt);
void rtn150(INDEX_TYPE pt);
void rtn151(INDEX_TYPE pt);
void rtn152(INDEX_TYPE pt);
void rtn153(INDEX_TYPE pt);
void rtn154(INDEX_TYPE pt);

#if(NO_OPRTR > 1)
#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef HOT_STANDBY
void rtn131_sys1(INDEX_TYPE pt);
void rtn131_sys2(INDEX_TYPE pt);
void rtn130_1_sys1(INDEX_TYPE pt);
void rtn130_1_sys2(INDEX_TYPE pt);
#endif
#endif
#endif
#ifdef PC_COMPATIBLE
void rtn146(INDEX_TYPE);
void rtn147(INDEX_TYPE);
#endif


void save_screen(void);
void restore_screen(void);
#if 0
void str2scrn(BYTE *,BYTE,char *,BYTE);
void hex2scrn(BYTE *,BYTE,BYTE,BYTE);
#endif // 0
void str2scrn(BYTE *,BYTE,char *,CHAT_TYPE);
void hex2scrn(BYTE *,BYTE,BYTE,CHAT_TYPE);
void ddt_ad_always(void);
void auto_upload(BYTE,BYTE,BYTE);
#ifdef EM_CONT_SEP_OPR_Q
int  toemq(INDEX_TYPE);
void rfemq(BYTE);
int fromemq(void);
#endif

#ifdef PC_KEYBOARD
void s_r_bit(void);
BYTE bit2byte(void);
BYTE parity(void);
void init_keyboard(void);
void sys2key(BYTE);
#endif

#ifdef PARAMETER_DOWN_UP_LOAD
void validate_parameters(void);
void fill_params(void);
void make_param_files(void);
#endif


#ifdef MALICIOUS_PRINT_ON_U
void find_real_mlced(BYTE,char *);
void wmpr(BYTE,char *);
void wmpr_isup(BYTE,char *);
#ifdef SIP_INSTALLED
void wmpr_sip(BYTE,char *);
#endif
void handle_malicious_info(char *mlced_buf,char *mlc_buf);
unsigned int cmpf(void);
#endif

#ifdef ALARM_ENABLE
void my_alarm(void);
void alarm_reset(void);
void date_to_log(void);
#endif

SDNST find_subscriber_no(unsigned int i);

#ifndef OPERATOR_PC
#define PWR_RST_8993            T1      /* P1.0 */
#define KEYBOARD_DATA           T0      /* P1.1 */
#define KEYBOARD_CLOCK          TXD     /* P1.2 */
#define SELECT_KEYBOARD_CLOCK   RXD     /* P1.3 */
#endif

#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
/* macros for defining dtai[] in ???_dpi.c */
#define CAS3_GROUP_MASK 0x000f
#define IS              0x0010     /* 1:in service, 0:out of service. */
#define ICC             0x0020     /* incoming channel */
#define OGC             0x0040     /* outgoing channel */
#define PTS             0x0080     /* signaling with PTS */
#define EOS             0x0100     /* signaling with EOS */
#define CAS_METERING    0x0200     /* signaling with METERING pulse */
#define OPR_RCL         0x0400     /* signaling with operator recall */
#define MAL_CALL        0x0800     /* signaling with malicious call */
#define IMM_ACK         0x1000     /* 1:immediate acknowlege should be sent after
                                      receiving active edge of seize, otherwise
                                      ack sent after complete reception of
                                      seize pulse. This bit is only for CAS1
                                      ICT  */
#define WTFR_ACK        0x2000      /* 1:should wait for acknowlege after
                                       sending sieze pulse
                                       This bit is only for CAS1, OGT */
#define INTER_SIGNAL_DGT_SEND  0x4000
#endif


#if (TON == 0)
#define LOCAL_PARAM_FILE        "loc1.prm"
#elif (TON == 1)
#define LOCAL_PARAM_FILE        "loc2.prm"
#elif (TON == 2)
#define LOCAL_PARAM_FILE        "loc3.prm"
#elif (TON == 3)
#define LOCAL_PARAM_FILE        "loc4.prm"
#else
#define LOCAL_PARAM_FILE        "loc.prm"
#endif

#define MAX_NO_OPR_CARD         3

void init_array(void);

#ifndef MAX_EXISTING_NO
#define MAX_EXISTING_NO NO_TOTAL_EXTENSION
#endif

void disp_trunk(void);
void disp_trunk_no(void);
void disp_cas3_no(void);
void disp_cas3(void);
void disp_cas1_no(void);
void disp_cas1(void);
void poke_s(BYTE x,BYTE y, char *s,CHAT_TYPE color);

void test_menu(void);

#ifdef DISPLAY_EM_STATUS
#include "/ec/work/sys/emstates.h"

#include "/ec/work/sys/csstates.h"
#include "/ec/work/sys/ststim.h"

#ifdef LIM
#include "/ec/work/sys/csstates.h"
#endif
#endif
#ifdef DISPLAY_CAS3_STATUS
#include "/ec/work/sys/emstates.h"
#include "/ec/work/sys/csstates.h"
#ifdef ISUP
#include "/ec/work/sys/ststim.h"
#endif
#endif
#ifdef DISPLAY_CAS1_STATUS
#include "/ec/work/sys/emstates.h"
#ifdef LIM
#include "/ec/work/sys/csstates.h"
#endif
#endif

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#define CAS3_HSTRCL_FILE                    "cas3_hst.rpt"
#define  NO_CAS3_HST_CH_DGT     3
void init_cas3_hst_file(void);
void cas3_hst_sgnl_process(unsigned int parm);

#endif
#endif



#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#define EM_HSTRCL_FILE                    "em_hst.rpt"
#define MAX_HST_FILE_SIZE                    1000000L

#define  NO_EM_HST_CH_DGT     3
void init_em_hst_file(void);
void em_hst_sgnl_process(unsigned int parm);

#endif
#endif

#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#define CAS1_HSTRCL_FILE                    "cas1_hst.rpt"
#define  NO_CAS1_HST_CH_DGT     3
void init_cas1_hst_file(void);
void cas1_hst_sgnl_process(unsigned int parm);

#endif
#endif


#if ((defined CAS3_HISTORICAL_REPORT) || (defined EM_HISTORICAL_REPORT) || (defined CAS1_HISTORICAL_REPORT))
#if (TON == HISTORICAL_OPR)

#define HISTORICAL_OPR                       0
#define MAX_HST_FILE_SIZE                    1000000L
#define TX_REPORT                            0x80
#define RX_REPORT                            0x40

#endif
#endif

#ifdef EM_HISTORICAL_REPORT
#define EM_GND_RPRT             0
#define EM_OPN_RPRT             1
#endif

#ifdef NEW_DISPLAY_STATUS
void opr_update(int kext);
void chng_layo(void);
#else
void chng_layo(BYTE *);
#endif
#ifdef X_RAILWAY
unsigned int femgroup(void);
#endif
unsigned int fcogroup(void);
#ifdef NEW_DEP
void dep_hndlr_init(void);
void open_param_files(int a);
void dep_quit(void);
void dep_init(void);
void dep_main(void);
#endif

#define TRUE               1
#define FALSE              0

#if ((defined LAPB_ON_OPERATOR_SIDE)||(defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
#define FIFO_RD_WR
#endif

#ifdef FIFO_RD_WR
#ifdef OPERATOR_SIDE
typedef struct {
        BYTE *arr;
        unsigned int front;
        unsigned int rear;
        unsigned int size;
}BUF;
#endif
BYTE put_buf(BUF *buf,BYTE dat);
BYTE get_buf(BUF *buf,BYTE *dat);
BYTE read_buf(BUF *buf,BYTE *dat);
unsigned int checkfree_buf(BUF *buf);
void dump_buf(BUF *);
unsigned int check_size_of_next_packet(BUF *buf);

BYTE put_pckt_to_variable_size_buf(BUF *buf,unsigned int size,BYTE *dat);
BYTE get_pckt_from_variable_size_buf(BUF *buf,unsigned int *size,BYTE *dat);
BYTE check_variable_size_buf_is_empty(BUF *buf);
unsigned int check_free_variable_size_buf(BUF *buf);
unsigned int check_used_variable_size_buf(BUF *buf);
BYTE write_and_check_size_buffer(BUF *buf,unsigned int size,BYTE *dat);
BYTE read_and_check_size_buffer(BUF *buf,unsigned int *size,BYTE *dat,unsigned int max_dat_size);

// should be same as dsmtfp.h
#define FIFO_2_BYTES_FOR_SIZE           0x80
#define FIFO_2_BYTES_FOR_SIZE_POS       0x80
#endif // FIFO_RD_WR


#ifdef LAPB_ON_OPERATOR_SIDE
#include "/ec/work/sys/lapb.h"
#endif
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#include "/ec/work/sys/sml.h"
#endif

#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
#define TCP_ON_CLIENT_SIDE
#endif

#include "/ec/work/sys/lintcp.h"
#ifdef LAPB_ON_OPERATOR_SIDE
#define LAPB_ERROR      0
#define LAPB_RTN        1
#define ICERROR         2
#endif
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#define SERIAL_ERROR    3
#endif
#if ((defined LAPB_ON_OPERATOR_SIDE) || (defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined FIFO_RD_WR))
#define BUF_OVERFLOW    4
#endif
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#define TCP_ERR         5
#endif

#ifdef ARP_INSTALLED
#define ARP_ERR         6
#endif
#ifdef IP_INSTALLED
#define IP_ERR          7
#endif
#ifdef TCP_INSTALLED  // shall not be defined with TCP_ON_CLIENT/SERVER_SIDE
#define TCP_ERR         8
#endif
#ifdef UDP_INSTALLED
#define UDP_ERR         9
#endif
#ifdef ETHERNET_INSTALLED
#define ETHERNET_ERR    10
#endif

#define ERROR           11
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#define SERIAL_RTN      12
#endif

#define NO_PRINTER_MESSAGES     13

void init_mess(void);
//#endif

#define LAPB_LINK0      0
#define LAPB_LINK1      1
#define UNKNOWN_LINK    2

#define CR              0x0d
#define LF              0x0a

void set_stm_for_up_down_load(void);
void set_stm_for_display(void);
#ifdef NEW_DEP
void set_stm_for_dep(void);
#endif
#ifdef HOT_STANDBY
void set_stm_for_htby_check(void);
#endif
void set_stm_for_term(void);
void unmask_stm(void);
#ifdef HOT_STANDBY
void htby_wait_win(void);
#endif
void init_led_attr(void);
#ifdef HOT_STANDBY
void mac_this_sys_is_hot_rtn(OMAR a);
#endif
#ifdef HOT_STANDBY
void check_htby_link_handler(void);
#endif


#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef OPR_UPLOAD_WITH_TIMER

#define UPLOAD_PARM_TICK                        12000
#if (NO_OPRTR > 1)
#define UPLOAD_ACCEPT_REJECT_TICK               100
#endif
void mac_upload_finished_rtn(OMAR a);
void upload_handler(void);

#endif
#endif

#if 0
#define         COLOR_BLACK            0x0
#define         COLOR_BLUE             0x1
#define         COLOR_GREEN            0x2
#define         COLOR_CYAN             0x3
#define         COLOR_RED              0x4
#define         COLOR_PURPLE           0x5
#define         COLOR_BROWN            0x6
#define         COLOR_LIGHTGRAY     0x7
#define         COLOR_DARKGRAY     0x8
#define         COLOR_LIGHTBLUE     0x9
#define         COLOR_LIGHTGREEN    0xa
#define         COLOR_LIGHTCYAN     0xb
#define         COLOR_LIGHTRED     0xc
#define         COLOR_LIGHTPURPLE   0xd
#define         COLOR_YELLOW           0xe
#define         COLOR_WHITE            0xf
#define         BLINK_OFF              0x00
#define         BLINK_ON               0x80
#endif // 0

void set_led_attr(BYTE led_no,CHAT_TYPE bkcolor,CHAT_TYPE forcolor,CHAT_TYPE blink);
//void set_led_attr(BYTE led_no,BYTE bkcolor,BYTE forcolor,BYTE blink);

#if ((defined DEP_MANUAL_TEST) || (defined SYSTEM_MANUAL_TEST))
#define         TEST_FAIL               0x0000
#define         TEST_OK                 0x1000
#define         TEST_BUSY               0x2000
#endif
#ifdef DEP_MANUAL_TEST
#define         TEST_NO_ANSWER          0x3000

#define         TEST_NULL_T            0
#define         TEST_XST_T             1
#define         TEST_EM_T              2
#define         TEST_DTMF_T            3
#define         TEST_SWCH_T            4
#define         TEST_DGTL_TRNK_T       5

typedef struct{
        BYTE prty_type;  /* but the types are different */
        unsigned int prty_indx;
        unsigned int result;
        /*
           2 bits are enough,but only to prevent << or >> or
           defining two type of macros for TEST_FAIL,.....
           "result" is defined as unsigned int.
        */
}TEST_ST;
#endif

void rtn144(INDEX_TYPE pt);
void rtn145(INDEX_TYPE pt);

#define         EM_CAS3_STATES_SIZE           NO_EM_STATES

#ifdef LIM
#define         EMSTATE_NAME_ARR_SIZE         NO_EM_STATES+NO_CAS3_STATES
#else
#ifdef RMT_EM
#define         EMSTATE_NAME_ARR_SIZE         NO_EM_STATES+NO_RMT_EM_STATES
#else
#define         EMSTATE_NAME_ARR_SIZE         NO_EM_STATES
#endif
#endif


void rtn44a(INDEX_TYPE);


#if (COMPILER == GCC)
char *ltoa(long value, char *string, int radix);
#endif
int my_getw(FILE *stream);
int my_putw(int w, FILE *stream);

#define         GRP_TYPE        unsigned char
void dep_fill_params(void);
void init_dpi(void);

#define NO_CAS3_DISPLAYED_PER_PAGE      30

void clear_large_screen(void);

#ifdef BATCH_PROGRAMMING

                /*** batch of extensions related      ***/

#define  MAC_BATCH_EXT_AMPLIFIER_ACCESS  MAC_AMPLIFIER_ACCESS     // AA
#define  MAC_BATCH_EXT_SIREN_ACCESS      MAC_SIREN_ACCESS         // SA
#define  MAC_BATCH_EXT_HOT_LINE_ACCESS   MAC_HOT_LINE_ACCESS      // HL
#define  MAC_BATCH_EXT_TIE_RSRV          MAC_TIE_RSRV             // TR
#define  MAC_BATCH_EXT_CPU_ACCESS        MAC_CPU_ACCESS           // PU  call pick up

#define  MAC_BATCH_EXT_TIE_1ST0          MAC_TIE_1ST0             // T0
#define  MAC_BATCH_EXT_CONF_ACCESS       MAC_CONF_ACCESS          // CF conference
#define  MAC_BATCH_EXT_CO_RSRV           MAC_CO_RSRV              // CR trunk reservation
#define  MAC_BATCH_EXT_DONTD_ACCESS      MAC_DONTD_ACCESS         // DD do not disturb active
#define  MAC_BATCH_EXT_CO_2ST0           MAC_CO_2ST0              // IN
#define  MAC_BATCH_EXT_CO_1ST0           MAC_CO_1ST0              // ST
#define  MAC_BATCH_EXT_FM_ACCESS         MAC_FM_ACCESS            // FM follow me
#define  MAC_BATCH_EXT_CAMPON_ACCESS     MAC_CAMPON_ACCESS        // CP
#define  MAC_BATCH_EXT_CB_ACCESS         MAC_CB_ACCESS            // CB call back
#define  MAC_BATCH_EXT_FEATURE           MAC_EXT_FEATURE          // FA
#define  MAC_BATCH_EXT_MALICIOUS_DTCT    MAC_MALICIOUS_DTCT       // MC
#define  MAC_BATCH_EXT_CALL_GEN          MAC_CALL_GEN             // CG

#define  MAC_BATCH_EXT_CO_GRP0           MAC_CO_GRP0
#define  MAC_BATCH_EXT_CO_GRP1           MAC_CO_GRP1
#define  MAC_BATCH_EXT_CO_GRP2           MAC_CO_GRP2
#define  MAC_BATCH_EXT_CO_GRP3           MAC_CO_GRP3
#define  MAC_BATCH_EXT_CO_GRP4           MAC_CO_GRP4
#define  MAC_BATCH_EXT_CO_GRP5           MAC_CO_GRP5

#define  MAC_BATCH_EXT_TIE_GRP0          MAC_TIE_GRP0
#define  MAC_BATCH_EXT_TIE_GRP1          MAC_TIE_GRP1
#define  MAC_BATCH_EXT_TIE_GRP2          MAC_TIE_GRP2
#define  MAC_BATCH_EXT_TIE_GRP3          MAC_TIE_GRP3
#define  MAC_BATCH_EXT_TIE_GRP4          MAC_TIE_GRP4
#define  MAC_BATCH_EXT_TIE_GRP5          MAC_TIE_GRP5

#define  MAC_BATCH_EXT_CO_GRP11          MAC_BATCH_EXT_TIE_GRP5
#define  MAC_BATCH_EXT_CO_GRP15          MAC_BATCH_EXT_EM_GRP3

#define  MAC_BATCH_EXT_EM_GRP0           MAC_EM_GRP0
#define  MAC_BATCH_EXT_EM_GRP1           MAC_EM_GRP1
#define  MAC_BATCH_EXT_EM_GRP2           MAC_EM_GRP2
#define  MAC_BATCH_EXT_EM_GRP3           MAC_EM_GRP3
#define  MAC_BATCH_EXT_EM_GRP4           MAC_EM_GRP4
#define  MAC_BATCH_EXT_EM_GRP5           MAC_EM_GRP5
#define  MAC_BATCH_EXT_EM_GRP6           MAC_EM_GRP6
#define  MAC_BATCH_EXT_EM_GRP7           MAC_EM_GRP7
#define  MAC_BATCH_EXT_EM_GRP8           MAC_EM_GRP8
#define  MAC_BATCH_EXT_EM_GRP9           MAC_EM_GRP9
#define  MAC_BATCH_EXT_EM_GRP10          MAC_EM_GRP10
#define  MAC_BATCH_EXT_EM_GRP11          MAC_EM_GRP11
#define  MAC_BATCH_EXT_EM_GRP12          MAC_EM_GRP12
#define  MAC_BATCH_EXT_EM_GRP13          MAC_EM_GRP13

#endif // BATCH_PROGRAMMING

#ifdef DISPLAY_BILLING
void rtn105e(INDEX_TYPE);
#endif


//#define hex2l(x) ( ((x>>24) * 1000000L) + (((x>>20)%0x10) * 100000L) + (((x>>16)%0x10) * 10000L) + ((x>>12)%0x10) * 1000 + ((x>>8)%0x10) * 100 + ((x>>4)%0x10) * 10 + (x%0x10))
//#define l2hex(x) ( ((x/1000000L)<<24) + (((x/100000L)%10)<<20) + (((x/10000)%10)<<16) + (((x/1000)%10)<<12) + (((x/100)%10)<<8) + (((x/10)%10)<<4) + (x%10))

#define hex2l(x) ( (((x>>28)%0x10) * 10000000L) +  (((x>>24)%0x10) * 1000000L) + (((x>>20)%0x10) * 100000L) + (((x>>16)%0x10) * 10000L) + ((x>>12)%0x10) * 1000 + ((x>>8)%0x10) * 100 + ((x>>4)%0x10) * 10 + (x%0x10))
#define l2hex(x) ( (((x/10000000L)%10)<<28) +  (((x/1000000L)%10)<<24) + (((x/100000L)%10)<<20) + (((x/10000)%10)<<16) + (((x/1000)%10)<<12) + (((x/100)%10)<<8) + (((x/10)%10)<<4) + (x%10))

#ifdef DEP_MANUAL_TEST
#define SUBSCRIBER_STATE_BLOCK_SIZE     35
#define  CR_T                   0x03
#define  DR_T                   0x05
#include "/ec/work/sys/ststim.h"
#endif

#ifdef DISPLAY_ALARM
void rtn105f(INDEX_TYPE);
void rtn148f(INDEX_TYPE pt);
void show_alarm(char *fmt, ...);
void show_alarm_without_date(char *fmt, ...);
#endif

#ifdef DEP_MANUAL_TEST
#if ((defined CPU_EC512) || (defined EC2K_SYSTEM))
#define  DIGIT1         0x28     /* 700Hz,900Hz   */
#define  DIGIT2         0x30     /* 700Hz,1100Hz  */
#define  DIGIT3         0x38     /* 900Hz,1100Hz  */
#define  DIGIT4         0x40     /* 700Hz,1300Hz  */
#define  DIGIT5         0x48     /* 900Hz,1300Hz  */
#define  DIGIT6         0x50     /* 1100Hz,1300Hz */
#define  DIGIT7         0x58     /* 700Hz,1500Hz  */
#define  DIGIT8         0x60     /* 900Hz,1500Hz  */
#define  DIGIT9         0x68     /* 1100Hz,1500Hz */
#define  DIGIT10        0x70     /* 1300Hz,1500Hz */
#define  ST             0x78
#define  KP             0x20
#define  DIGITF         0xff
#endif
#if ((defined EC1K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PU2K_SYSTEM))
#define  DIGIT1         0x01     /* 700Hz,900Hz   */
#define  DIGIT2         0x02     /* 700Hz,1100Hz  */
#define  DIGIT3         0x03     /* 900Hz,1100Hz  */
#define  DIGIT4         0x04     /* 700Hz,1300Hz  */
#define  DIGIT5         0x05     /* 900Hz,1300Hz  */
#define  DIGIT6         0x06     /* 1100Hz,1300Hz */
#define  DIGIT7         0x07     /* 700Hz,1500Hz  */
#define  DIGIT8         0x08     /* 900Hz,1500Hz  */
#define  DIGIT9         0x09     /* 1100Hz,1500Hz */
#define  DIGIT10        0x0a     /* 1300Hz,1500Hz */
#define  ST             0x0b
#define  KP             0x00
#define  DIGITF         0x0f
#endif
#endif // DEP_MANUAL_TEST
void send_fdn(void);
void terminal(void);
#define MAX_OPR_SYS_PCKT_SIZE   16
void message_to_term_q(BYTE);
void kt_send(void);
void execute(void);
void init_terminal(void);
void print_char(BYTE c);
void restore_terminal(void);

#ifdef FIFO_RD_WR
void prnt_msg(BYTE msg_type,unsigned int err_code);
#endif
void prnt_int(int msg);
void prnt_long(long msg);
void prnt_char(BYTE msg);
void prnt_byte(BYTE msg);
void prnt_string(char *msg);

void prndrv(void);
void init_prntr(void);
void ws2p(char *p);
void correct_time(void);
void correct_given_time(DATTIM *);
void backup_pfna(int i);

int pmamftx_both_sys(int,int,int,BYTE);
int pmamftx(int,int,int,BYTE);
int pmamftx_to_direct_link(int,int,int,BYTE);
int pcpmftx_both_sys(int s,int i,BYTE t);        /* always sent to system A */
void send_time_date_to_sys(DATTIM *dt);
void update_time(void);

#if (COMPILER == GCC)
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void fcloseall(void);
#endif
#endif

void init_subscriber_groups_data(void);
#ifndef MAX_NO_OF_SUBSCRIBER_GROUPS
#define MAX_NO_OF_SUBSCRIBER_GROUPS     100
#endif
typedef struct{
        SDNST directory_start;
        unsigned long no_of_ports;
        unsigned long port_start;
        unsigned int no_of_digits;
        unsigned int group_no;
}SUBSCRIBER_GROUP;

#if (COMPILER == GCC)
#if (OS == DOS) || (OS == WINDOWS)
#define kt_outport8(port,data)                  outportb(port,data)
#define kt_outport16(port,data)                 outportw(port,data)
#define kt_inport8(port)                                inportb(port)
#define kt_inport16(port)                               inportw(port)
#endif
#if (OS == DOS)
#define kt_farpoke8(seg,off,data)               _farpokeb(_dos_ds,(seg<<4)+off,data)
#define kt_farpoke16(seg,off,data)              _farpokew(_dos_ds,(seg<<4)+off,data)
#define kt_farpeek8(seg,off)                    _farpeekb(_dos_ds,(seg<<4)+off)
#define kt_farpeek16(seg,off)                   _farpeekw(_dos_ds,(seg<<4)+off)
#endif
#if (OS == LINUX_COMPATIBLE)
#define kt_outport8(port,data)          outb(data,port)
#define kt_inport8(port)                        inb(port)
#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
#define kt_farpoke8(seg,off,data)       {isa_mem[(seg<<4)+off-ISA_MEMORY_BASE]=data;}
#define kt_farpoke16(seg,off,data)      {(unsigned short)isa_mem[(seg<<4)+off-ISA_MEMORY_BASE]=data;}
#define kt_farpeek8(seg,off)            isa_mem[(seg<<4)+off-ISA_MEMORY_BASE]
#define kt_farpeek16(seg,off)           (unsigned short)isa_mem[(seg<<4)+off-ISA_MEMORY_BASE]
#endif
#endif
#endif
#if (COMPILER == BCC)
#define kt_outport8(port,data)                  outportb(port,data)
#define kt_inport8(port)                                inportb(port)
#define kt_farpoke8(seg,off,data)               pokeb(seg,off,data);
#define kt_farpoke16(seg,off,data)              poke(seg,off,data)
#define kt_farpeek8(seg,off)                    peekb(seg,off)
#define kt_farpeek16(seg,off)                   peek(seg,off)
#endif

#if (OS == LINUX_COMPATIBLE)
struct kt_time {
    unsigned char ti_min;
    unsigned char ti_hour;
    unsigned char ti_hund;
    unsigned char ti_sec;
};
struct kt_date {
    short da_year;
    char da_day;
    char da_mon;
};
#endif

#if (OS == DOS) || (OS == WINDOWS)
struct kt_time{
    unsigned char ti_min;
    unsigned char ti_hour;
    unsigned char ti_hund;
    unsigned char ti_sec;
};
struct kt_date{
    short da_year;
    char da_day;
    char da_mon;
};
/* nice try, funny!!
typedef struct time struct kt_time;
typedef struct date struct kt_date;
*/
#endif


void kt_settime(struct kt_time *tm);
void kt_setdate(struct kt_date *dt);
int kt_system(const char *cmd);
char *kt_itoa(int value,char *string,int radix);
unsigned int f_intr(void);
void set_f_intr(void);
void reset_f_intr(void);
int print_error_report(const char *fmt, ...);
void activate_bell(void);
#ifdef OPERATOR_PC
void kt_print_char(unsigned int x, unsigned int y, CHAT_TYPE ch,CHAT_TYPE attr);
CHAT_TYPE kt_read_char(unsigned int x, unsigned int y);
int kt_forever_loop(void);
int kt_all_initialize(void);
#endif

#if (OS == LINUX_COMPATIBLE)

#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
void open_user_space_isa_mem(void);
void close_user_space_isa_mem(void);
#endif

#ifdef NCURSES_USED
void kt_ncurses_init(void);
int ncurses_getch(void);
int ncurses_mvaddch(unsigned int y,unsigned int x,unsigned long ch);
int ncurses_addch(unsigned long ch);
unsigned long ncurses_mvinch(unsigned int y,unsigned int x);
int ncurses_printw(const char *fmt,...);
int ncurses_move(int y,int );
void ncurses_getyx(int y,int x);
int ncurses_curs_set(int visibility);
void ncurses_clear(void);
int ncurses_def_prog_mode(void);
int ncurses_endwin(void);
int ncurses_refresh(void);
int ncurses_attrset(int attrs);
int ncurses_scrollok(unsigned char dat);
void ncurses_bkgdset(int attrs);
#endif

#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
#define ISA_MEMORY_SIZE         0x50000L
#define ISA_MEMORY_BASE         0xa0000L
#endif

#if (LINUX_OS == KURT)
void rtc_program(void);
void *etc_thread_program(void *q);
void *init_thread_program(void *q);
void init_all_pthread_sync_device(void);
void destroy_all_pthread_sync_device(void);
#endif

#endif // (OS == LINUX_COMPATIBLE)

#define NULL_POINTER    0
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
void oam_opr_rcv_hndlr(void);
#endif

void write_date_record(unsigned short packed_date);
void create_new_dbrf_file_with_sys_time(unsigned short packed_date);

//....


#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM) || (defined OPR_PRI_CONNECTION))

#define KIATEL_DYNAMIC_MEM

int max(int value1, int value2);        // defined in wg.h
int min(int value1, int value2);

#define IN_USE                  1

#define _U_     __attribute__((unused))

#define WORD_MR         unsigned int

typedef char                    gchar  ;
typedef unsigned char           guchar ;

typedef short                   gshort ;
typedef unsigned short          gushort;

typedef int                     gint   ;
typedef unsigned int            guint  ;

typedef long                    glong  ;
typedef unsigned long           gulong ;

typedef short                   gint16 ;
typedef unsigned short          guint16;

typedef char                    gint8  ;
typedef unsigned char           guint8 ;

//typedef int                     gint32 ;
//typedef unsigned int            guint32;
typedef long                    gint32 ;
typedef unsigned long           guint32;

typedef long long               gint64;
typedef unsigned long long      guint64;
typedef guint8                  gboolean;

typedef guint                   gsize;

//typedef long                    gptr;

#if (COMPILER == GCC)
typedef void                    *gpointer;
typedef const void              *gconstpointer;
#endif
#if (COMPILER == BCC)
typedef unsigned char           *gpointer;
typedef const unsigned char     *gconstpointer;
#endif


#define G_MAXINT                G_MAXINT32 // peyda nakardam
#define G_MININT                G_MININT32 // peyda nakardam
typedef struct _GString         GString;

struct _GString
{
  gchar  *str;
  gsize len;
  gsize allocated_len;
};

#define GPOINTER_TO_INT(p)      ((gint)   (p))
#define GPOINTER_TO_UINT(p)     ((guint)  (p))

#define GINT_TO_POINTER(i)      ((gpointer)  (i))
#define GUINT_TO_POINTER(u)     ((gpointer)  (u))
#endif // ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM) || (defined OPR_PRI_CONNECTION))

typedef long                    gptr;

#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
#define TIMERS_FOR_OPR
#include "/ec/work/sys/tcpip/tcpip.h"
#endif

#ifdef MY_TCPIP_FOR_OAM
void start_oam_tcp_active_port(unsigned int rec_no);
BYTE send_oam_pckt_to_endpoint(BYTE n,unsigned int size,BYTE *dat);
BYTE oam_tcp_port_analysis(unsigned int tcb_no,unsigned int mr1);
void my_tcpip_oam_tx_hndlr(void);
#endif

#ifdef IN_DEPTH_BACKTRACK
#define BACKTRACK_DEPTH   10   // should be enough!
#else
#define BACKTRACK_DEPTH   1
#endif


#ifdef KIATEL_DYNAMIC_MEM

#define long2prnt(w2p) prnt_int(((guint32)(w2p))/0x10000L);prnt_int(((guint32)(w2p))%0x10000L);
#define  IN_QUE                 0xff
#define  OUT_OF_QUE             0xfe
#define NULL_MEM_REC            (WORD_MR)(-1)  //0xffff
#define MEM_NULL_PARAM          (BYTE *)0

#define NO_MEM_REC              (8192)


typedef struct{
        BYTE *parm;
        BYTE state;
        WORD_MR size;
        WORD_MR rp;
        WORD_MR lp;
/*
        unsigned int size;
        unsigned int rp;
        unsigned int lp;
*/
#if (COMPILER == GCC)
        gptr addr[BACKTRACK_DEPTH];
#endif
}MEM_REC_ST;

#define NUMSMALL        0
#define KT_SIZE_T               unsigned int
#define MAX_DYNAMIC_MEM_SIZE    0xfffffL
typedef struct BLOCK {
        KT_SIZE_T size;
#if (COMPILER == BCC)
        unsigned int dummy1;
#endif
        struct BLOCK *next;
        int bucket;
#if (COMPILER == BCC)
        unsigned int dummy2;
#endif
}BLOCK;

BYTE *mem_req(unsigned int size);
void mem_rls(BYTE *addr);
void mem_cpy(BYTE *dst,BYTE *src,unsigned int size);
int mem_cmp(BYTE *dst,BYTE *src,unsigned int size);
unsigned int mem_rec_req(unsigned int size);
void mem_rec_rls(unsigned int i);
unsigned int find_free_mem_rec(void);
void init_mem_rec(void);
void print_mr(unsigned int mr1);
void *my_malloc(KT_SIZE_T size);
void my_free(void *ptr);
void my_assert(int a);
void *my_sbrk(unsigned int size);
void dynamic_mem_init(void);
void my_check(BLOCK *b);
void my_consistency(void);
int my_size2bucket(KT_SIZE_T size);
int my_b2bucket(BLOCK *b);
BLOCK *my_split_block(BLOCK *b, KT_SIZE_T size);
BLOCK *my_merge(BLOCK *a, BLOCK *b, BLOCK *c);
void *my_realloc(void *ptr, KT_SIZE_T size);
void dump_memory(void);
void dump_all_memory(void);
void dump_mem_rec(void);
void *mem_set(void *buffer, int ch, size_t num);
gpointer kt_g_malloc (gulong n_bytes);
void kt_g_free (gpointer mem);
gpointer kt_g_calloc(size_t num_elements, size_t size);
WORD_MR find_mr1_from_g_pointer(gpointer mem);
gpointer find_g_pointer_from_mr1 (WORD_MR mr1);

#endif // KIATEL_DYNAMIC_MEM

#ifdef TIMERS_FOR_OPR

#define NULL_TIMER      0xffff
#define  TIMER_T        0x0d
#define  NO_TMR_QPTR    128
#ifndef TIMER_NO   // may be it is better to have OPR_TIMER_NO
#define  TIMER_NO       250
#endif
#ifdef IP_INSTALLED
#define IP_REASSEMBLE_SDL_T             0x57
#endif
#ifdef TCP_INSTALLED
#define TCP_SDL_T                       0x58
#endif
#ifdef ARP_INSTALLED
#define ARP_SDL_T                       0x59
#endif

#ifdef ARP_INSTALLED
#define INDEX_32_BITS
#endif
typedef struct{
        unsigned int t:8;
#ifdef INDEX_32_BITS
        unsigned int i;
#else
        unsigned int i:16;
#endif
}INDEX_TYPE_VER2;
typedef struct{
        unsigned int lptr;
        unsigned int rptr;
        INDEX_TYPE_VER2  it;
        unsigned int tdiv;
        int tick;
        BYTE state;
#ifdef NEW_DEBUG
        BYTE stim;
#if (COMPILER == GCC)
        gptr addr[BACKTRACK_DEPTH];
#endif
#endif
        unsigned int     i;  /* index to timer pointer table */
        BYTE subtype;
}TMR;

unsigned int getimer(void);void rlstimer(unsigned int tmr_no);
unsigned int setimer(int,int,BYTE);unsigned int stoptimer(int,BYTE);
void timeman(void);

#endif // TIMERS_FOR_OPR

#ifdef IN_DEPTH_BACKTRACK
#define BACKTRACK_DEPTH   10   // should be enough!
#else
#define BACKTRACK_DEPTH   1
#endif

void print_call_frame(void);
void print_addr(gptr *addr);
void save_addr(gptr *addr);
void copy_addr(gptr *addr1,gptr *addr2);
void store_call_frame(gptr *addr);
void print_stored_call_frame(gptr *addr);
void init_debug(void);

void turn_on_park_led(BYTE n);
void turn_off_park_led(BYTE n);
void adjust_local_pulse(unsigned int a);
void adjust_billing_time(unsigned int a);
void calculate_checksum(unsigned int a);

//#define MAX_NO_OF_SUBSCRIBER_GROUPS     50
typedef struct{
    BYTE no_office_code_dgts;
    BYTE office_code_dgts[6];
    BYTE no_of_subscriber_groups;
    SDNST directory_start[MAX_NO_OF_SUBSCRIBER_GROUPS];
}SITE_PARAMETERS;

// also exist in dsmtfp.h
#define FROM_MASTER_SYSTEM      0
#define FROM_SLAVE_SYSTEM       1
//

#ifdef OPR_PRI_CONNECTION
void save_og_pri_digits(unsigned int ext_no);
void save_ic_pri_digits(unsigned int ext_no);
void show_ext_digits(unsigned int ext_no);
#endif
void rtp_oprside(unsigned int indx, unsigned char type);
void reset_calls_report_status(void);

#ifdef SPEED_DIALLING
#define NULL_SP_NODE                    (SP_NODE_STRUCT *)(0)
#define LAST_VALID_SP_DIGIT             9    // DIGIT 0 should be kept as 0 not 0x0a
#define NO_SP_VALID_DIGIT               (LAST_VALID_SP_DIGIT+1)
#define MAX_NO_SP_DIGIT1                5
#define MAX_NO_SP_DIGIT2                20
typedef struct SP_NODE_STRUCT{
        struct SP_NODE_STRUCT *nxt_node_pt[NO_SP_VALID_DIGIT];
        guchar *number_ptr;
        WORD_MR mr1;
}SP_NODE_STRUCT;
void read_sp_file(void);
void find_sp_from_tree(guchar *dgt,guchar no_of_digits,guchar **number_ptr);
#endif
