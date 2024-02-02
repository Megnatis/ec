
#include "omtfp.h"

#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

//DOOD : #ifdef added
#if (OS == DOS)
#include <conio.h>
//DOOD : #endif added
#endif
#include <string.h>
#include <stdlib.h>
#include "wg.h"
#include "depmtfp.h"
#include "opr_net.h"

#ifdef DEP_MANUAL_TEST
char *state_string[NO_STATES]={
 "Idle          "  //  IDLE                           0
,"Park          "  //  PARK                           1
,"Hold          "  //  HOLD                           2
,"In Internal Qu"  //  IN_INTRNL_QU                   3
,"In Recall Qu  "  //  IN_RCL_QU                      4
,"In External Qu"  //  IN_EXTRNL_QU                   5
,"Wtfr Hook     "  //  AWAIT_ON_HOOK                  6
,"Await Trnk Rls"  //  AWAIT_TRNK_RLS                 7
,"Wtfr 1st Dgt  "  //  AWAIT_1ST_DGT                  8
,"Path Analysis "  //  PATH_ANALYSIS                  9
,"Intrnl Dial   "  //  INTRNL                         0x0a
,"Service Analys"  //  SERVICE_ANALYSIS               0x0b
,"Call Waiting  "  //  CALL_WAITING                   0x0c
,"Ringing       "  //  RINGING                        0x0d
,"Flsh onn Ring "  //  FLASH_DTCT_RING                0x0e
,"Intrnl Talk   "  //  CONVERSATION                   0x0f
,"Flsh on Intrnl"  //  FLASH_DTCT_CONVERSATION        0x10
,"Wtfr Park Loc."  //  AWAIT_PARK_LOC                 0x11
,"External Call "  //  EXTRNL_CALL                    0x12
,"Ring Call Back"  //  RING_CALL_BACK                 0x13
,"Collect Dgts  "  //  COLLECT_DGTS                   0x14
,"Locked        "  //  LOCK                           0x15
,"Wtfr Trnk Szr "  //  AWAIT_TRNK_SZR                 0x16
,"Flsh on DOD   "  //  FLASH_DTCT_TRNK                0x17
,"CO Trnk Grp   "  //  CO_TRNK_GROUP                  0x18
,"Tie Trnk Grp  "  //  TIE_TRNK_GROUP                 0x19
,"Flsh Confirm  "  //  FLASH_CONFIRM                  0x1a
,"Dial Opr      "  //  DIAL_OPR                       0x1b
,"Opr Sub Ring  "  //  OPR_EXT_RINGING                0x1c
,"Opr Talk      "  //  OPR_TALK                       0x1d
,"Trnk Wtfr Sub "  //  TRNK_AWAIT_EXT                 0x1e
,"Opr Night     "  //  OPR_NIGHT                      0x1f
,"Trnk Trnk Talk"  //  TRUNK_TRUNK_TALK               0x20
,"E&M Trnk Grp  "  //  EM_TRNK_GROUP                  0x21
,"Ring to Night "  //  RINGING_TO_NITE                0x22
,"S.S. Analysis "  //  SPECIAL_SERVICE_ANALYSIS       0x23
,"Opr @ Hold    "  //  OPR_AT_HOLD                    0x24
,"Bad Payer B1  "  //  BAD_PAYER_IN_B1                0x25
,"Wake Up Ring  "  //  WAKE_UP_RINGING                0x26


#if ( (defined EM_PULSE_SIGNALING) || (defined CAS1_INSTALLED) )
,"Wtfr Ack      "  //  AWAIT_ACK              EM_PULSE_STATES_START
,"Ack Rcvd      "  //  ACK                    (EM_PULSE_STATES_START + 1)
,"E&M OGT Block "  //  OGT_BLOCK              (EM_PULSE_STATES_START + 2)
,"E&M ICT Block "  //  ICT_BLOCK              (EM_PULSE_STATES_START + 3)
,"Wtfr Seize    "  //  AWAIT_SEIZE            (EM_PULSE_STATES_START + 4)
,"E&M Guard     "  //  GAURD                  (EM_PULSE_STATES_START + 5)
,"Answer        "  //  ANSWER                 (EM_PULSE_STATES_START + 6)
,"Wtfr Dgt      "  //  AWAIT_DGT              (EM_PULSE_STATES_START + 7)
,"Wtfr Seize Ack"  //  AWAIT_SEIZE_ACK        (EM_PULSE_STATES_START + 8)
,"IC Sgnl Block "  //  IC_SGNL_BLOCK          (EM_PULSE_STATES_START + 9)
,"Congestion    "  //  CONGESTION             (EM_PULSE_STATES_START + 0x0a)
,"ICT Connect   "  //  STD_CONNECT            (EM_PULSE_STATES_START + 0x0b)
,"Clear Back    "  //  CLR_BACK               (EM_PULSE_STATES_START + 0x0c)
,"OGT Talk      "  //  TALK                   (EM_PULSE_STATES_START + 0x0d)
,"E&M Metering  "  //  METERING               (EM_PULSE_STATES_START + 0x0e)
,"Opr Wtfr Ack  "  //  OPR_AWAIT_ACK_EM       (EM_PULSE_STATES_START + 0x0f)
,"Opr Ack Rcvd  "  //  OPR_ACK_EM             (EM_PULSE_STATES_START + 0x10)
,"Opr Answer    "  //  OPR_ANSWER_EM          (EM_PULSE_STATES_START + 0x11)
,"Opr Talk      "  //  OPR_TALK_EM            (EM_PULSE_STATES_START + 0x12)
,"Opr Metering  "  //  OPR_METERING_EM        (EM_PULSE_STATES_START + 0x13)
,"Flsh on EM OGT"  //  FLASH_DTCT_OGT_EM_PULSE    (EM_PULSE_STATES_START + 0x14)
,"Flsh on EM ICT"  //  FLASH_DTCT_ICT_EM_PULSE    (EM_PULSE_STATES_START + 0x15)
,"E&M FAS       "  //  EM_FAS                 (EM_PULSE_STATES_START + 0x16)
,"Init Guard    "  //  INIT_GAURD             (EM_PULSE_STATES_START + 0x17)
,"E&M Local Bloc"  //  LOCAL_BLOCK            (EM_PULSE_STATES_START + 0x18)
,"Wtfr 1st Dgt  "  //  AWAIT_1ST_DGT_EM       (EM_PULSE_STATES_START + 0x19)
,"Ack Input Clf "  //  ACK_INPUT_CLF          (EM_PULSE_STATES_START + 0x1a)
,"Talk Input Clf"  //  TALK_INPUT_CLF         (EM_PULSE_STATES_START + 0x1b)
,"ICT Ringing   "  //  STD_CONNECT_RINGING    (EM_PULSE_STATES_START + 0x1c)
#endif

#ifdef EM_CONTINUOUS_SIGNALING
,"Wtfr Sz Ack   "  //  AWAIT_SEIZE_ACK_EM_CONT        EM_CONTINUOUS_STATES_START
,"Sz Ack        "  //  SEIZE_ACK_EM_CONT              (EM_CONTINUOUS_STATES_START + 1)
,"Wtfr Answer   "  //  AWAIT_ANSWER_EM_CONT           (EM_CONTINUOUS_STATES_START + 2)
,"Answer        "  //  ANSWER_EM_CONT                 (EM_CONTINUOUS_STATES_START + 3)
,"Sub. Talk EM  "  //  EXT_TALK_EM_CONT               (EM_CONTINUOUS_STATES_START + 4)
,"Flsh on EM    "  //  FLASH_DTCT_EM_CONT             (EM_CONTINUOUS_STATES_START + 5)
,"Wtfr EM 1st Dg"  //  AWAIT_EM_CONT_DEST_1ST_DGT     (EM_CONTINUOUS_STATES_START + 6)
,"Wtfr Trnk Grp "  //  AWAIT_EM_CONT_TRUNK_GROUP      (EM_CONTINUOUS_STATES_START + 7)
,"EM Block      "  //  EM_CONT_BLOCK                  (EM_CONTINUOUS_STATES_START + 8)
,"Wtfr Sub. Num "  //  AWAIT_EM_CONT_EXT_NUM          (EM_CONTINUOUS_STATES_START + 9)
,"Wtfr Opr. Num "  //  AWAIT_EM_CONT_OPR_NUM          (EM_CONTINUOUS_STATES_START + 0x0a)
,"Wtfr CLF      "  //  AWAIT_EM_CONT_CLF              (EM_CONTINUOUS_STATES_START + 0x0b)
,"EM Sub. Ring  "  //  EM_CONT_EXT_RINGING            (EM_CONTINUOUS_STATES_START + 0x0c)
,"EM Sub. Talk  "  //  EM_CONT_TALK_EXT               (EM_CONTINUOUS_STATES_START + 0x0d)
,"Opr Wtfr Ack  "  //  OPR_AWAIT_SEIZE_ACK_EM_CONT    (EM_CONTINUOUS_STATES_START + 0x0e)
,"Opr Ack Rcvd  "  //  OPR_SEIZE_ACK_EM_CONT          (EM_CONTINUOUS_STATES_START + 0x0f)
,"Opr Answer    "  //  OPR_ANSWER_EM_CONT             (EM_CONTINUOUS_STATES_START + 0x10)
,"Opr Talk EM   "  //  OPR_TALK_EM_CONT               (EM_CONTINUOUS_STATES_START + 0x11)
,"EM Trnk Talk  "  //  EM_TRUNK_TALK                  (EM_CONTINUOUS_STATES_START + 0x12)
,"EM Wtfr EM Ack"  //  EM_CONT_AWAIT_SEIZE_ACK_EM_CONT (EM_CONTINUOUS_STATES_START + 0x13)
,"EM Rcvd EM Ack"  //  EM_CONT_SEIZE_ACK_EM_CONT      (EM_CONTINUOUS_STATES_START + 0x14)
,"EM Wtfr EM Ans"  //  EM_CONT_AWAIT_ANSWER_EM_CONT   (EM_CONTINUOUS_STATES_START + 0x15)
,"EM Rcvd EM Ans"  //  EM_CONT_ANSWER_EM_CONT         (EM_CONTINUOUS_STATES_START + 0x16)
#endif

#ifdef CAS3_INSTALLED
,"Wtfr Seize Ack"  //  AWAIT_SEIZED_ACKN              CAS3_STATES_START
,"Ack Rcvd      "  //  SEIZED_ACKN                    (CAS3_STATES_START + 1)
,"Wtfr PTS      "  //  AWAIT_DT_PTS                   (CAS3_STATES_START + 2)
,"PTS           "  //  DT_PTS                         (CAS3_STATES_START + 3)
,"Addressing Dgt"  //  ADDRESSING_DGT                 (CAS3_STATES_START + 4)
,"Wtfr EOS      "  //  AWAIT_DT_EOS                   (CAS3_STATES_START + 5)
,"EOS           "  //  DT_EOS                         (CAS3_STATES_START + 6)
,"Wtfr Answer   "  //  AWAIT_DT_ANSWERED              (CAS3_STATES_START + 7)
,"DT Guard      "  //  DT_GUARD                       (CAS3_STATES_START + 8)
,"DT OGT Talk   "  //  EXT_DT_TALK                    (CAS3_STATES_START + 9)
,"Trnk Offr/CLB "  //  DT_OPR_RCL_CLB                 (CAS3_STATES_START + 0x0a)
,"DT Metering   "  //  DT_METERING                    (CAS3_STATES_START + 0x0b)
,"Flsh on DT OGT"  //  OGDT_FLASH_DTCT                (CAS3_STATES_START + 0x0c)
,"DT OGT Block  "  //  DT_OG_BLOCK                    (CAS3_STATES_START + 0x0d)
,"DT Local Block"  //  DT_LOCAL_BLOCK                 (CAS3_STATES_START + 0x0e)
,"Wtfr DT Dgt   "  //  AWAIT_DT_ADDRESSING            (CAS3_STATES_START + 0x0f)
,"Wtfr Dst. Addr"  //  AWAIT_DEST_ADDRESS             (CAS3_STATES_START + 0x10)
,"DT Ringing    "  //  DT_EXT_RINGING                 (CAS3_STATES_START + 0x11)
,"ICT Talk      "  //  DT_EXT_TALK                    (CAS3_STATES_START + 0x12)
,"Flsh on DT ICT"  //  ICDT_FLASH_DTCT                (CAS3_STATES_START + 0x13)
,"Wtfr DT CLF   "  //  AWAIT_DT_CLF                   (CAS3_STATES_START + 0x14)
,"Wtfr Sz Ack Dg"  //  AWAIT_SEIZED_ACKN_DGT_READY    (CAS3_STATES_START + 0x15)
,"Wtfr Dst Addr "  //  AWAIT_DEST_ADDRESSING          (CAS3_STATES_START + 0x16)
,"Wtfr Opr Num  "  //  AWAIT_CAS3_OPR_NUM             (CAS3_STATES_START + 0x17)
,"DT FAS        "  //  DT_FAS                         (CAS3_STATES_START + 0x18)
,"MLC on DT     "  //  EXT_DT_TALK_MALICIOUS          (CAS3_STATES_START + 0x19)
,"Opr Wtfr Ack  "  //  OPR_AWAIT_SEIZED_ACKN          (CAS3_STATES_START + 0x1a)
#endif

#ifdef CHANNEL_TEST
,"Wtfr Ch Tst Dg"  //  WTFR_CHANNEL_TEST_DGTS          CHANNEL_TEST_STATES_START+0
,"Ch. Tst Cnnctd"  //  CHANNEL_TEST_CONNECTED          CHANNEL_TEST_STATES_START+1
#endif

#ifdef CONFERENCE_ACT
,"Conference    "  //  CONFERENCE              CONFERENCE_STATES_START
,"Cnfrnce Fail  "  //  CONFERENCE_FAIL         (CONFERENCE_STATES_START + 1)
#ifdef CONFERENCE_N_PARTY
,"Flsh on Conf. "  //  FLASH_DTCT_CONFERENCE   (CONFERENCE_STATES_START + 2)
#endif
#endif

#ifdef AMPLIFIER_INSTALLED
,"Amplifier On  "  //  AMPLIFIER_ON            AMPLIFIER_STATE_START
#endif

#ifdef SIREN_INSTALLED
,"Siren On      "  //  SIREN_ON                SIREN_STATE_START
#endif

#ifdef LD_INSTALLED
,"Wtfr Rmt Bias "  //  AWAIT_RMT_BIAS_LD               LD_STATES_START
,"LD Send Dgts  "  //  SEND_DGTS_LD                    (LD_STATES_START+1)
,"LD Wtfr Answer"  //  AWAIT_ANSWER_LD                 (LD_STATES_START+2)
,"Sub. Talk LD  "  //  EXT_TALK_LD                     (LD_STATES_START+3)
,"Wtfr Fwd Bias "  //  AWAIT_LD_FORWARD_BIAS_CBK       (LD_STATES_START+4)
,"Sc Wtfr Rmt Bs"  //  SEC_AWAIT_RMT_BIAS_LD           (LD_STATES_START+5)
,"LD Wtfr 1st Dg"  //  AWAIT_LD_DEST_1ST_DGT           (LD_STATES_START+6)
,"LD wtfr Sub Dg"  //  AWAIT_LD_EXT_NUM                (LD_STATES_START+7)
,"LD Ringing    "  //  LD_EXT_RINGING                  (LD_STATES_START+8)
,"LD Talk Sub.  "  //  LD_TALK_EXT                     (LD_STATES_START+9)
,"Wtfr LD Hook  "  //  AWAIT_LD_ON_HOOK_CLF            (LD_STATES_START+10)
,"Wtfr LD Fwd   "  //  AWAIT_LD_LOOP_FORWARD_CBK       (LD_STATES_START+11)
#endif

#if ((defined RMT_EXT) || (defined RMT_CB))
,"Wtfr Dscnct Ak"  //  AWAIT_LINK_DISCONNECT_ACK       RMT_EXT_STATES_START
,"Not Accessible"  //  EXT_NOT_ACCESSIBLE              (RMT_EXT_STATES_START+1)
#endif

#ifdef EM_ON_RSUSIDE
,"              "  //  DTCCS_CONNECT_EM                (EM_RSUSIDE_STATES_START)
,"              "  //  DTCCS_CONNECT_EM_DS             (EM_RSUSIDE_STATES_START+1)
,"              "  //  EM_CONNECT_DTCCS                (EM_RSUSIDE_STATES_START+2)
,"              "  //  EM_CONNECT_DTCCS_DR             (EM_RSUSIDE_STATES_START+3)
#endif

#ifdef RMT_EM
,"Wtfr EM Dscnct"  //  AWAIT_EM_LINK_DISCONNECT_ACK            (RMT_EM_STATES_START)
,"Wtfr EM Lcl Ak"  //  AWAIT_EM_LCL_BLK_LINK_DISCONNECT_ACK    (RMT_EM_STATES_START+1)
,"Wtfr EM Rmt Ak"  //  AWAIT_EM_RMT_BLK_LINK_DISCONNECT_ACK    (RMT_EM_STATES_START+2)
#endif

#if ((defined EXT_ON_RSUSIDE) || (defined CB_ON_RSUSIDE))
,"              "  //  EXT_CONNECT_CCSV5               (EXT_RSUSIDE_STATES_START)
,"              "  //  CCSV5_CONNECT_EXT               (EXT_RSUSIDE_STATES_START+1)
,"              "  //  FLASH_CHECK                     (EXT_RSUSIDE_STATES_START+2)
,"              "  //  EXT_CONNECT_CCSV5_NO_DR         (EXT_RSUSIDE_STATES_START+3)
#endif

#if ((defined RMT_CAS3) || (defined CAS3_ON_E1_PROCESSOR_EXIST))
,"Wtfr DT Dscnct"  //  AWAIT_CAS3_LINK_DISCONNECT_ACK            (RMT_CAS3_STATES_START)
,"Wtfr DT Lcl Ak"  //  AWAIT_CAS3_LCL_BLK_LINK_DISCONNECT_ACK    (RMT_CAS3_STATES_START+1)
,"Wtfr DT Rmt Ak"  //  AWAIT_CAS3_RMT_BLK_LINK_DISCONNECT_ACK    (RMT_CAS3_STATES_START+2)
#endif

#if ((defined CAS3_ON_RSUSIDE) || (defined CAS3_ON_E1_PROCESSOR_SIDE))
,"              "  //  DTCCS_CONNECT_CAS3                (CAS3_RSUSIDE_STATES_START)
,"              "  //  DTCCS_CONNECT_CAS3_DS             (CAS3_RSUSIDE_STATES_START+1)
,"              "  //  CAS3_CONNECT_DTCCS                (CAS3_RSUSIDE_STATES_START+2)
,"              "  //  CAS3_CONNECT_DTCCS_DR             (CAS3_RSUSIDE_STATES_START+3)
#endif

#ifdef ISUP

,"Isupi Wtfr Dgt"  //  ISUPI_WAIT_FOR_FURTHER_DIGITS_STATE     (ISUP_STATES_START+0)
,"Isupi Ringing "  //  ISUPI_EXT_RINGING_STATE                 (ISUP_STATES_START+1)
,"Isupi Answered"  //  ISUPI_ANSWERED_STATE                    (ISUP_STATES_START+2)
,"Isupi Wtfr Rlc"  //  ISUPI_WAIT_FOR_RLC_STATE                (ISUP_STATES_START+3)
,"Isupi Wtfr Rea"  //  ISUPI_WAIT_FOR_REANSWER_STATE           (ISUP_STATES_START+4)
,"Isupo Wtfr Dgt"  //  ISUPO_WAIT_FOR_DIGITS_STATE             (ISUP_STATES_START+5)
,"Isupo Wtfr Acm"  //  ISUPO_WAIT_FOR_ACM_STATE                (ISUP_STATES_START+6)
,"Isupo Wtfr Anm"  //  ISUPO_WAIT_FOR_ANM_STATE                (ISUP_STATES_START+7)
,"Isupo Wtfr Rlc"  //  ISUPO_WAIT_FOR_RLC_STATE                (ISUP_STATES_START+8)
,"Isupo Answered"  //  ISUPO_ANSWERED_STATE                    (ISUP_STATES_START+9)
,"Isupi Wtfr Con"  //  ISUPI_WAIT_FOR_CONTINUITY_CHECK_STATE   (ISUP_STATES_START+0xa)
,"Isupo Wtfr M D"  //  ISUPO_WAIT_FOR_MORE_DIGITS_STATE        (ISUP_STATES_START+0xb)
,"Hear Annc.    "  //  HEAR_ANNOUNCEMENT                       (ISUP_STATES_START+0xc)
,"Isupi Flash   "  //  ISUPI_FLASH_DETECTED_STATE              (ISUP_STATES_START+0xd)
,"Isupo Flash   "  //  ISUPO_FLASH_DETECTED_STATE              (ISUP_STATES_START+0xe)
#endif

#ifdef LE_V51_INSTALLED
,"Await V51 Disc"  //  AWAIT_V51_LINK_DISCONNECT_COMPLETE      LE_V51_STATES_START
#endif

#ifdef LE_V52_INSTALLED
,"Await V52 Disc"  //  AWAIT_V52_LINK_DISCONNECT_COMPLETE      LE_V52_STATES_START
#endif
};
char *type_string[16]={
 "M&A   "  //  MA_T                   0
,"OPR   "  //  OPR_T                  0x01
,"SUB   "  //  XST_T                  0x02
,"C.R.  "  //  CR_T                   0x03
,"NULL  "  //  NULL_T                 0x04
,"D.R.  "  //  DR_T                   0x05
,"CONF  "  //  CONF_T                 0x06
,"Coin B"  //                         0x07
//,"LD    "  //  LD_T                   0x08
,"ISUP  "  //  ISUP_T                 0x08
,"CCS   "  //  LIM_T                  0x09    DTCCS_T                0x09
,"TIE   "  //  TIE_T                  0x0a
,"E&M   "  //  EM_T                   0x0b
,"CAS 1b"  //  CAS1_T                 0x0c
,"TIMER "  //  TIMER_T                0x0d
,"DOD   "  //  CO_T                   0x0e
,"CAS 3b"  //  CAS3_T                 0x0f
};
#endif

#ifdef DEP_MANUAL_TEST
//????????????????????????????????????????????????????????????????????????
//????????????????????????????????????????????????????????????????????????
//is defined in dsmtfp.h
#define         DTMF_PER_CARD   16

#define         MAX_SWITCH_NO   1

#ifdef LIM

#if(NO_EXTENSION_C != 0)
#define         MAX_PORT_NO      3
#else
#define         MAX_PORT_NO      2
#endif

#else
#define         MAX_PORT_NO      1
#endif

//#define         NO_DTMF_CARD_IN_SYS_A           NO_DTMF_CARD_IN_SYS

#ifdef LIM

#define         NO_DTMF_A       (NO_DTMF_CARD_IN_SYS_A * DTMF_PER_CARD)
#define         NO_DTMF_B       (NO_DTMF_CARD_IN_SYS_B * DTMF_PER_CARD)

#if(NO_EXTENSION_C != 0)
#define         NO_DTMF_C       (NO_DTMF_CARD_IN_SYS_C * DTMF_PER_CARD)
#endif

#else
#ifdef NO_DTMF_CARD_IN_SYS
#define         NO_DTMF_A       (NO_DTMF_CARD_IN_SYS * DTMF_PER_CARD)
#endif
#ifdef DSP_DTMF
#define         NO_DTMF_A       NO_DTMF
#endif
#endif

#if((defined DTMF) && (!defined NO_DTMF_A))
#ifdef RSU_CCSV5_INSTALLED
#define NO_DTMF_A       (NO_RSU * DTMF_PER_CARD)
#else
#define NO_DTMF_A       0
#endif
#endif

//????????????????????????????????????????????????????????????????????????
//????????????????????????????????????????????????????????????????????????

extern WINDOW *saved_win;
extern WINDOW *saved_win2;
extern WINDOW *saved_win3;
extern BYTE menu_item;
extern BYTE test_flag;
extern BYTE test_2nd_menu_flag;
extern char ddm[];
extern char test_ext[];
extern char test_ext_mask[];
#ifdef EM_INSTALLED
extern char test_em[];
extern char test_em_mask[];
extern int no_em[];
#endif
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
extern char test_dgtl_trnk[];
extern char test_dgtl_trnk_mask[];
extern int no_cas1[];
extern int no_cas3[];
#endif
#ifdef DTMF
extern char test_dtmf[];
extern char test_dtmf_mask[];
#endif
extern BYTE test_menu_flag;

extern char test_port_unit[];
extern char test_port_unit_mask[];

extern char test_swch[];
extern char test_swch_mask[];

extern unsigned char data_entry_flag;
extern TEST_ST tst;

extern unsigned int subscriber_state_block[];
extern BYTE subscriber_state_block_ptr;

#endif

#ifdef CB_INSTALLED
extern unsigned long first_cb_number;
#endif

#ifdef DEP_MANUAL_TEST
void test_menu(void){
        if (test_2nd_menu_flag == RESET) {
                test_1st_menu();
                return;
        }
        switch(test_2nd_menu_flag){
                case 1:                 // Subscriber Test
                          ext_test();
                          break;
                case 2:                 // Dtmf Test
#ifdef DTMF
                          dtmf_menu();
#endif
                          break;
                case 3:                 // E&M Test
#ifdef EM_INSTALLED
                          em_test();
#endif
                          break;
                case 4:                 // Switch Test
                          swch_menu();
                          break;
                case 5:                 //Digital Trunk Test
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
                          dgtl_trnk_test();
#endif
                          break;
                case 6:
                          test_flag=RESET;  //Quit
                          test_2nd_menu_flag=RESET;
                          deletw(saved_win);
                          break;
                case 7:                 // Subscriber State
                          ext_state();
                          break;
                case 8:
                          r1_test();
                          break;
                default : break;
        }
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef DEP_MANUAL_TEST
int test_1st_menu(void){  /* edit parameters */

        int b;
        if (test_flag == RESET){
                saved_win=display_test_menu();
        }
        b=get_selection(saved_win,1,"STMIGRU1");
        if (b == NO_KEY_PRESSED){
              return b;
        }
        switch(b){
                case 1:
                        test_2nd_menu_flag=b;
                        ext_test_init_display();
                        return b;
                case 2:
#ifdef DTMF
                        test_2nd_menu_flag=b;
                        dtmf_menu();
#endif
                        return b;
                case 3:
#ifdef EM_INSTALLED
                        // don't forget! test_2nd_menu_flag=RESET if EM_INSTALLED
                        // is not defined.
                        test_2nd_menu_flag=b;
                        em_test_init_display();
#endif
                        return b;
                case 4:
                        test_2nd_menu_flag=b;
                        swch_menu();
                        return b;
                case 5:
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
                        test_2nd_menu_flag=b;
                        dgtl_trnk_test_init_display();
#endif
                        return b;
                case 6 :
                        test_flag=RESET;  //Quit
                        test_2nd_menu_flag=RESET;
                        deletw(saved_win);
                        return b;
                case 7:
                        test_2nd_menu_flag=b;
                        ext_state_init_display();
                        return b;
                case 8:
                        test_2nd_menu_flag=b;
                        r1_test_init_display();
                        return b;
                default: return b;
        }

}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef DEP_MANUAL_TEST
WINDOW *display_test_menu(void){  /* display test menu & return choice */

        WINDOW *a;

        a=enw(21,4,10,32);
        if(a == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,display_test_menu:error in memory allocation!");
                quit(1);
        }
        dw(a);
        swb(a,DOUBLE_LINE);
//DOOD start
#if 0
        swc(a,ALL,MAGENTA,WHITE,BRIGHT);
        swc(a,ACCENT,AQUA,BLACK,BRIGHT);
#endif // 0
        swc(a,ALL,KT_MAGENTA,KT_WHITE,KT_HIGH_INTENSITY);
        swc(a,ACCENT,KT_CYAN,KT_BLACK,KT_HIGH_INTENSITY);
//DOOD end
        swt(a,"  SYSTEM TEST  ");
        draw_window(a);
//DOOD start
//      swucf(a,GREEN);
        swucf(a,KT_GREEN);
//DOOD end

        wprompt(a,1,0,"1-Subscriber test.");

#ifdef DTMF
        wprompt(a,1,1,"2-dTmf test.");
#else
        wprompt(a,1,1,"2-not used.");
#endif

#ifdef EM_INSTALLED
        wprompt(a,1,2,"3-e&M trunk test.");
#else
        wprompt(a,1,2,"3-not used.");
#endif

        wprompt(a,1,3,"4-swItch test.");

//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
        wprompt(a,1,4,"5-diGital trunk test.");
#else
        wprompt(a,1,4,"5-not used.");
#endif

        wprompt(a,1,5,"6-Return to main menu.");
        wprompt(a,1,6,"7-sUbscriber state.");
        wprompt(a,1,7,"8-r1 test.");
        return a;
}
#endif
/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
void ext_test(void){

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win2);
                switch (code){
                  case ESC :    exit_manual_test_with_1_menu();
                                return ;
                  case ENTER :  ext_test_part2();
                                return;
                  default :     return;
                }
        }
        else{    // ENTER key is pressed.

                switch (tst.result){
                  case TEST_NO_ANSWER : break;
                  case TEST_OK        :
                                        wprompt(saved_win2,0,4,"       *****  Test is Passed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_FAIL      :
                                        wprompt(saved_win2,0,4,"       *****  Test is Failed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_BUSY      :
                                        wprompt(saved_win2,0,4,"     *****  SUBSCRIBER is BUSY.  *****   ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  default             : break;
                }
                if (get_c() == ESC)
                        exit_manual_test_with_1_menu();
        }
        return ;
}
#define NO_ITEMS_PER_COLUMN     17
#define FIRST_ROW               1
#define FIRST_COLUMN            0
#define COLUMN_OFFSET           26
void ext_state(void){
        int code,j,i;
        unsigned int c;
        BYTE a,a3,dgt;
        BYTE row,col;
        if (data_entry_flag == SET){
                code=data_entry(saved_win2);
                switch (code){
                  case ESC :    exit_manual_test_with_1_menu();
                                return ;
                  case ENTER :  ext_state_part2();
                                return;
                  default :     return;
                }
        }
        else{    // ENTER key is pressed.

                if(subscriber_state_block_ptr == SUBSCRIBER_STATE_BLOCK_SIZE){
                        // 17 items per column
                        row=FIRST_ROW;
                        col=FIRST_COLUMN;
//........
                        c=subscriber_state_block[0];
                        a=c>>8;         // stape
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&0x80){      // rc
                                if(a == CR_T)
                                        wprintf(saved_win2,"State: Call Rcrd Cnnctd.");
                                else
                                if(a == DR_T)
                                        wprintf(saved_win2,"State: Dgt Rcvr Cnnctd.");
                                else
                                        wprintf(saved_win2,"State: --");
                        }
                        else
                                wprintf(saved_win2,"State: %s",state_string[a]);
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&0x40)       // rr
                                wprintf(saved_win2,"Receive Ring: Yes.");
                        else
                                wprintf(saved_win2,"Receive Ring: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&0x20)       // hhp
                                wprintf(saved_win2,"Have a Hold Party: Yes.");
                        else
                                wprintf(saved_win2,"Have a Hold Party: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&0x10)       // extstat
                                wprintf(saved_win2,"Hook Status: On Hook.");
                        else
                                wprintf(saved_win2,"Hook Status: Off Hook.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&0x08)       // icw
                                wprintf(saved_win2,"Active Call Waiting: Yes.");
                        else
                                wprintf(saved_win2,"Active Call Waiting: No.");
//........
                        c=subscriber_state_block[1];    // rsu_no
                        a = (c & 0xf800) >> 11;
                        j = (c & 0x07e0) >> 5;
                        i = (c & 0x001f);
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(a==0xff)
                                wprintf(saved_win2,"P.U. Number: No P.U.");
                        else
                                wprintf(saved_win2,"P.U.: %d/%d/%d",a+1,j+1,i+1);
//........
                        c=subscriber_state_block[2];    // ccsv5_ch
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c==0xffff)
                                wprintf(saved_win2,"CCS Ch. No: No Channel.");
                        else
                                wprintf(saved_win2,"CCS Ch. No: %d",c);
//........
                        c=subscriber_state_block[3];    // cst
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&CAMP_ON)
                                wprintf(saved_win2,"Call Wait Allwd: Yes.");
                        else
                                wprintf(saved_win2,"Call Wait Allwd: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&FOLLOW_ME)
                                wprintf(saved_win2,"Follow Me Allwd: Yes.");
                        else
                                wprintf(saved_win2,"Follow Me Allwd: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&STD)
                                wprintf(saved_win2,"Inter City Allwd: Yes.");
                        else
                                wprintf(saved_win2,"Inter City Allwd: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&INTERNATIONAL)
                                wprintf(saved_win2,"Interntnl Allwd: Yes.");
                        else
                                wprintf(saved_win2,"Interntnl Allwd: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&DONT_DIST)
                                wprintf(saved_win2,"DoN't Dstrb Allwd: Yes.");
                        else
                                wprintf(saved_win2,"DoN't Dstrb Allwd: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&CONFERENCE_SERVICE)
                                wprintf(saved_win2,"Conference Allwd: Yes.");
                        else
                                wprintf(saved_win2,"Conference Allwd: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&DTMF_ALLOWED)
                                wprintf(saved_win2,"DTMF dial Allwd: Yes.");
                        else
                                wprintf(saved_win2,"DTMF dial Allwd: No.");
//........
                        c=subscriber_state_block[4];    // ast
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&CALL_GEN)
                                wprintf(saved_win2,"Call Gen. Allwd: Yes.");
                        else
                                wprintf(saved_win2,"Call Gen. Allwd: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&FEATURE_ACTIVE)
                                wprintf(saved_win2,"Flash Allwd: Yes.");
                        else
                                wprintf(saved_win2,"Flash Allwd: No.");
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c&MALICIOUS_CALL_DTCT)
                                wprintf(saved_win2,"MLC Dtct. Allwd: Yes.");
                        else
                                wprintf(saved_win2,"MLC Dtct. Allwd: No.");

//........
                        c=subscriber_state_block[5];    // xp->tidx
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"Timer: No. TMR cnnctd.");
                        else
                                wprintf(saved_win2,"Timer No.: %d",c);
//........
                        c=subscriber_state_block[6];    // tp->tick
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"Tmr Val.: No. Value.");
                        else
                                wprintf(saved_win2,"Tmr Val.: %ld msec",c*20L);
//........
                        c=subscriber_state_block[7];    // tp->tdiv/tp->i
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"Expires: No. Value.");
                        else
                                wprintf(saved_win2,"Expires: %ld msec",c*20L);
//........
                        c=subscriber_state_block[8];    // cp->state/cp->f3.t
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        a3=c&0xff;       // f3.t
                        if(c == 0xffff)
                                wprintf(saved_win2,"CR State: No CR cnnctd.");
                        else
                                wprintf(saved_win2,"CR State: %s",state_string[c>>8]);
//........
                        c=subscriber_state_block[9];    // cp->tidx
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"CR TMR: No TMR cnnctd.");
                        else
                                wprintf(saved_win2,"CR TMR No.: %d",c);
//........
                        c=subscriber_state_block[10];    // tp->tick
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"Tmr Val.: No. Value.");
                        else
                                wprintf(saved_win2,"Tmr Val.: %ld msec",c*20L);
//........
                        c=subscriber_state_block[11];    // tp->tdiv/tp->i
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"Expires: No. Value.");
                        else
                                wprintf(saved_win2,"Expires: %ld msec",c*20L);
//........
                        c=subscriber_state_block[12];    // cp->f4
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"CR F4: No DR cnnctd.");
                        else
                                wprintf(saved_win2,"CR F4: DR no. %d",c);
//........
                        c=subscriber_state_block[13];    // cp->f1.i
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"CR 1st Fac.: No F1");
                        else
                                wprintf(saved_win2,"CR 1st Fac.: %d",c);
//........
                        c=subscriber_state_block[14];    // cp->f2.i
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"CR 2nd Fac.: No F2");
                        else
                                wprintf(saved_win2,"CR 2nd Fac.: %d",c);
//........
                        c=subscriber_state_block[15];    // cp->f3.i
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"CR 3rd Fac.: No F3");
                        else
                                wprintf(saved_win2,"CR 3rd Fac.: %d",c);
//........
                        c=subscriber_state_block[16];    // (cp->f1.t << 8) | (cp->f2.t);
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        a=c>>8;
                        if(a == 0xff)
                                wprintf(saved_win2,"CR 1st Fac Type: Empty");
                        else
                                wprintf(saved_win2,"CR 1st Fac Type: %s",type_string[a]);
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        a=c&0xff;
                        if(a == 0xff)
                                wprintf(saved_win2,"CR 2nd Fac Type: Empty");
                        else
                                wprintf(saved_win2,"CR 2nd Fac Type: %s",type_string[a]);
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        a=a3;
                        if(a == 0xff)
                                wprintf(saved_win2,"CR 3rd Fac Type: Empty");
                        else
                                wprintf(saved_win2,"CR 3rd Fac Type: %s",type_string[a]);
//........
                        c=subscriber_state_block[17];    // (dp->state << 8) | dp->f6;
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        a=c>>8;
                        if(a == 0xff)
                                wprintf(saved_win2,"DR State: No DR Cnnctd.");
                        else
                                wprintf(saved_win2,"DR State: %s",state_string[a]);
                        a=c&0xff;
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(a == 0xff){
                                a3=0;
                                wprintf(saved_win2,"No. Dialled Dgt: --");
                        }
                        else{
                                a3=a&0x1f;
                                wprintf(saved_win2,"No. Dialled Dgt: %d",a3);
                        }
//........
                        c=subscriber_state_block[18];   // dp->tidx
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"DR TMR: No TMR cnnctd.");
                        else
                                wprintf(saved_win2,"DR TMR No.: %d",c);
//........
                        c=subscriber_state_block[19];    // tp->tick
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"Tmr Val.: No. Value.");
                        else
                                wprintf(saved_win2,"Tmr Val.: %ld msec",c*20L);
//........
                        c=subscriber_state_block[20];    // tp->tdiv/tp->i
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"Expires: No. Value.");
                        else
                                wprintf(saved_win2,"Expires: %ld msec",c*20L);
//........
                        c=subscriber_state_block[21];   // dp->f1
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"DR 1st Fac.: No F1");
                        else
                                wprintf(saved_win2,"DR 1st Fac.: %d",c);
//........
                        c=subscriber_state_block[22];   // dp->f1t
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        a=c&0xff;
                        if(a == 0xff)
                                wprintf(saved_win2,"DR 1st Fac Type: Empty");
                        else
                                wprintf(saved_win2,"DR 1st Fac Type: %s",type_string[a]);
//........
                        c=subscriber_state_block[23];   // dp->d4d3d2d1
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        wprintf(saved_win2,"Dgts: ");
                        j=0;
                        for(i=0;i<4;i++,j++){
                                if(j<a3){
                                        dgt=c&0x0f;
                                        if(dgt>=0x0a) dgt=0;
                                        c=c>>4;
                                        wprintf(saved_win2,"%1d",dgt);
                                }
                                else
                                        wprintf(saved_win2,"-");
                        }
//........
                        c=subscriber_state_block[24];   // dp->d8d7d6d5
                        for(i=0;i<4;i++,j++){
                                if(j<a3){
                                        dgt=c&0x0f;
                                        if(dgt>=0x0a) dgt=0;
                                        c=c>>4;
                                        wprintf(saved_win2,"%1d",dgt);
                                }
                                else
                                        wprintf(saved_win2,"-");
                        }
//........
                        c=subscriber_state_block[25];   // dp->d12d11d10d9
                        for(i=0;i<4;i++,j++){
                                if(j<a3){
                                        dgt=c&0x0f;
                                        if(dgt>=0x0a) dgt=0;
                                        c=c>>4;
                                        wprintf(saved_win2,"%1d",dgt);
                                }
                                else
                                        wprintf(saved_win2,"-");
                        }
//........
                        c=subscriber_state_block[26];   // dp->d16d15d14d13
                        for(i=0;i<4;i++,j++){
                                if(j<a3){
                                        dgt=c&0x0f;
                                        if(dgt>=0x0a) dgt=0;
                                        c=c>>4;
                                        wprintf(saved_win2,"%1d",dgt);
                                }
                                else
                                        wprintf(saved_win2,"-");
                        }

//........
                        c=subscriber_state_block[27];    // (bp->state << 8) | bp->nd
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        a=c>>8;
                        if(a == 0xff)
                                wprintf(saved_win2,"BR State: No BR Cnnctd.");
                        else
                                wprintf(saved_win2,"BR State: %d",a);
                        a=c&0xff;
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(a == 0xff){
                                a3=0;
                                wprintf(saved_win2,"No. Dialled Dgt: --");
                        }
                        else{
                                a3=a&0x0f;
                                wprintf(saved_win2,"No. Dialled Dgt: %d",a3);
                        }
//........
                        c=subscriber_state_block[28];   // bp->time
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"BR Time: No Time Rcrded.");
                        else
                                wprintf(saved_win2,"BR Time: %d Sec.",c*2);
//........
                        c=subscriber_state_block[29];   // bp->count
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(c == 0xffff)
                                wprintf(saved_win2,"BR Pls Cnt.: No Pls.");
                        else
                                wprintf(saved_win2,"BR Pls Cnt.: %d",c);
//........
                        c=subscriber_state_block[30];   // bp->d4d3d2d1
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        wprintf(saved_win2,"Dgts: ");
                        j=0;
                        for(i=0;i<4;i++,j++){
                                if(j<a3){
                                        dgt=c&0x0f;
                                        if(dgt>=0x0a) dgt=0;
                                        c=c>>4;
                                        wprintf(saved_win2,"%1d",dgt);
                                }
                                else
                                        wprintf(saved_win2,"-");
                        }
//........
                        c=subscriber_state_block[31];   // bp->d8d7d6d5
                        for(i=0;i<4;i++,j++){
                                if(j<a3){
                                        dgt=c&0x0f;
                                        if(dgt>=0x0a) dgt=0;
                                        c=c>>4;
                                        wprintf(saved_win2,"%1d",dgt);
                                }
                                else
                                        wprintf(saved_win2,"-");
                        }
//........
                        c=subscriber_state_block[32];   // bp->d12d11d10d9
                        for(i=0;i<4;i++,j++){
                                if(j<a3){
                                        dgt=c&0x0f;
                                        if(dgt>=0x0a) dgt=0;
                                        c=c>>4;
                                        wprintf(saved_win2,"%1d",dgt);
                                }
                                else
                                        wprintf(saved_win2,"-");
                        }
//........
                        c=subscriber_state_block[33];   // bp->d16d15d14d13
                        for(i=0;i<4;i++,j++){
                                if(j<a3){
                                        dgt=c&0x0f;
                                        if(dgt>=0x0a) dgt=0;
                                        c=c>>4;
                                        wprintf(saved_win2,"%1d",dgt);
                                }
                                else
                                        wprintf(saved_win2,"-");
                        }
//........
                        c=subscriber_state_block[34];   // (v51_pt->pstn_state << 8) | sys_pt->state
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        a=c>>8;
                        if(a == 0xff)
                                wprintf(saved_win2,"PSTN State: Not Relevant.");
                        else
                                wprintf(saved_win2,"PSTN State: %d",a);
                        a=c&0xff;
                        wcursor(saved_win2,col,row);if(++row==FIRST_ROW+NO_ITEMS_PER_COLUMN) {row=FIRST_ROW;col+=COLUMN_OFFSET;}
                        if(a == 0xff)
                                wprintf(saved_win2,"Sys State:  Not Relevant.");
                        else
                                wprintf(saved_win2,"Sys State: %d",a);
//........
                        subscriber_state_block_ptr=0;
                }
                if (get_c() == ESC)
                        exit_manual_test_with_1_menu();
        }
        return ;
}
#endif


/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
void ext_test_init_display(void){

        WINDOW *w;
        FIELD *f;

        w=enw(30,8,10,47);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,ext_test_init:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  SUBSCRIBER MANUAL TEST  ");
        draw_window(w);
        wprompt(w,0,1," Please Enter Subscriber No. :");
        wprompt(w,0,2," ---------------------------     -------");
        wprompt(w,0,4,"  Press ENTER to start,ESC to quit. ");
        /* field allocation */
        idew(w);
        f=enf(w,33,1,test_ext_mask,test_ext,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,ext_test_init:error in memory allocation!");
                quit(1);
        }
        caf(w);
        saved_win2=w;
        data_entry_init(saved_win2);
        return ;
}
void ext_state_init_display(void){

        WINDOW *w;
        FIELD *f;

        w=enw(30,8,10,47);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,ext_state_init:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  SUBSCRIBER STATE REQUEST  ");
        draw_window(w);
        wprompt(w,0,1," Please Enter Subscriber No. :");
        wprompt(w,0,2," ---------------------------     -------");
        wprompt(w,0,4,"  Press ENTER to start,ESC to quit. ");
        /* field allocation */
        idew(w);
        f=enf(w,33,1,test_ext_mask,test_ext,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,ext_state_init:error in memory allocation!");
                quit(1);
        }
        caf(w);
        saved_win2=w;
        data_entry_init(saved_win2);
        return ;
}
#endif

/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
void ext_test_part2(void){

        BYTE n;
        unsigned long b;

        b=-1;
        data_entry_flag=RESET;
        sscanf(test_ext,"%ld",&b);
        b = dep_fcp(b);
        if (b == OPR_ERROR){
                wprompt(saved_win2,0,4,"           Invalid Subscriber No. !        ");
                wprompt(saved_win2,0,6,"              Press ESC to quit.           ");
                return;
        }
        wprompt(saved_win2,0,4,"        *****  Please Wait !  *****     ");
        wprompt(saved_win2,0,6,"             Press ESC to quit.         ");
#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (b >= (NO_EXTENSION_A+NO_EXTENSION_B)){
                b -= (NO_EXTENSION_A+NO_EXTENSION_B);
                n=2;
        }
        else
#endif
        if(b >= NO_EXTENSION_A){
                b -= NO_EXTENSION_A;
                n=1;
        }
        else
#endif
        n=0;
        pmamftx(MAC_EXT_MANUAL_TEST|MA_SYS_P|FEATURE_ACTIVATION,(unsigned int)b,OPR_T,n);
        tst.prty_type=TEST_XST_T;
        tst.prty_indx=(unsigned int)b;
        tst.result=TEST_NO_ANSWER;
        return ;
}
void ext_state_part2(void){
        WINDOW *w;
        BYTE n;
        unsigned long b,b1;
        b=-1;
        data_entry_flag=RESET;
        sscanf(test_ext,"%ld",&b);
        b1 = dep_fcp(b);
        if (b1 == OPR_ERROR){
#ifdef CB_INSTALLED
                b-=first_cb_number;
                if(b < NO_CB){
                        b |= 0x8000;
                        n=0;
                        goto cont1;
                }
#endif
                wprompt(saved_win2,0,4,"           Invalid Subscriber No. !        ");
                wprompt(saved_win2,0,6,"              Press ESC to quit.           ");
                return;
        }
        b=b1;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (b >= (NO_EXTENSION_A+NO_EXTENSION_B)){
                b -= (NO_EXTENSION_A+NO_EXTENSION_B);
                n=2;
        }
        else
#endif
        if(b >= NO_EXTENSION_A){
                b -= NO_EXTENSION_A;
                n=1;
        }
        else
#endif
        n=0;
cont1:
        deletw(saved_win2);
        w=enw(0,0,20,80);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,ext_state_part2:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  SUBSCRIBER STATE DISPLAY  ");
        draw_window(w);
        wcursor(w,0,0);
        wprintf(w,"Subscriber Number: %s.  Press ESC to quit.",test_ext);
        saved_win2=w;
        pmamftx(MAC_EXT_STATE_REQUEST|MA_SYS_P|FEATURE_ACTIVATION,(unsigned int)b,OPR_T,n);
        subscriber_state_block_ptr=0;
        return ;
}
#endif


/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
#ifdef EM_INSTALLED
void em_test(void){

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win2);
                switch (code){
                  case ESC :    exit_manual_test_with_1_menu();
                                return ;
                  case ENTER :  em_test_part2();
                                return;
                  default :     return;
                }
        }
        else{    // ENTER key is pressed.
                switch (tst.result){
                  case TEST_NO_ANSWER : break;
                  case TEST_OK        :
                                        wprompt(saved_win2,0,4,"       *****  Test is Passed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_FAIL      :
                                        wprompt(saved_win2,0,4,"       *****  Test is Failed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_BUSY      :
                                        wprompt(saved_win2,0,4,"     *****  E&M trunk is BUSY.  *****   ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  default             : break;
                }
                if (get_c() == ESC)
                        exit_manual_test_with_1_menu();
        }
        return ;
}
#endif
#endif

/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
#ifdef EM_INSTALLED
void em_test_init_display(void){

        WINDOW *w;
        FIELD *f;

        w=enw(30,8,10,47);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,em_test_init:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  E&M TRUNK MANUAL TEST  ");
        draw_window(w);
        wprompt(w,0,1," Please Enter E&M Trunk No. :");
        wprompt(w,0,2," ---------------------------     ----");
        wprompt(w,0,4,"  Press ENTER to start,ESC to quit. ");
        /* field allocation */
        idew(w);
        f=enf(w,33,1,test_em_mask,test_em,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,em_test_init:error in memory allocation!");
                quit(1);
        }
        caf(w);
        saved_win2=w;
        data_entry_init(saved_win2);
        return ;
}
#endif
#endif
/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
#ifdef EM_INSTALLED
void em_test_part2(void){

        unsigned int em_no;

        data_entry_flag=RESET;
        em_no=-1;
        sscanf(test_em,"%d",&em_no);
        em_no--;
        if (em_no >= NO_EM){
                wprompt(saved_win2,0,4,"           Invalid E&M trunk No. !      ");
                wprompt(saved_win2,0,6,"             Press ESC to quit.         ");
                return;
        }
        wprompt(saved_win2,0,4,"        *****  Please Wait !  *****     ");
        wprompt(saved_win2,0,6,"            Press ESC to quit.          ");
        pmamftx(MAC_EM_MANUAL_TEST|MA_SYS_P|FEATURE_ACTIVATION,em_no,OPR_T,0);
        tst.prty_type=TEST_EM_T;
        tst.prty_indx=em_no;
        tst.result=TEST_NO_ANSWER;
        return ;
}
#endif
#endif

/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void dgtl_trnk_test(void){

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win2);
                switch (code){
                  case ESC :    exit_manual_test_with_1_menu();
                                return ;
                  case ENTER :  dgtl_trnk_test_part2();
                                return;
                  default :     return;
                }
        }
        else{    // ENTER key is pressed.
                switch (tst.result){
                  case TEST_NO_ANSWER : break;
                  case TEST_OK        :
                                        wprompt(saved_win2,0,4,"       *****  Test is Passed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_FAIL      :
                                        wprompt(saved_win2,0,4,"       *****  Test is Failed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_BUSY      :
                                        wprompt(saved_win2,0,4,"     ***** DIGITAL TRUNK is BUSY.  ****");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  default             : break;
                }
                if (get_c() == ESC)
                        exit_manual_test_with_1_menu();
        }
        return ;
}
#endif
#endif

/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void dgtl_trnk_test_init_display(void){

        WINDOW *w;
        FIELD *f;

        w=enw(30,8,10,47);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,dgtl_trnk_test_init:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  DIGITAL TRUNK MANUAL TEST  ");
        draw_window(w);
        wprompt(w,0,1," Please Enter Digital Trunk Ch. No. :");
        wprompt(w,0,2," ----------------------------------   ----");
        wprompt(w,0,4,"  Press ENTER to start,ESC to quit. ");
        /* field allocation */
        idew(w);
        f=enf(w,38,1,test_dgtl_trnk_mask,test_dgtl_trnk,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,dgtl_trnk_test_init:error in memory allocation!");
                quit(1);
        }
        caf(w);
        saved_win2=w;
        data_entry_init(saved_win2);
        return ;
}
#endif
#endif
/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void dgtl_trnk_test_part2(void){

        unsigned int ch_no;

        data_entry_flag=RESET;
        ch_no=-1;
        sscanf(test_dgtl_trnk,"%d",&ch_no);
        ch_no--;
//DYNAMIC : #if (NO_CAS3 != 0) -->
#ifdef EXTERNAL_CAS3_INSTALLED
//DYNAMIC : NO_CAS3 --> no_cas3[0]
        if (ch_no >= no_cas3[0]){
#endif
//DYNAMIC : #if (NO_CAS1 != 0) -->
#ifdef CAS1_INSTALLED
//DYNAMIC : NO_CAS1 -->
        if (ch_no >= no_cas1[0]){
#endif
                wprompt(saved_win2,0,4,"     Invalid Digital trunk No. !          ");
                return;
        }
        wprompt(saved_win2,0,4,"        *****  Please Wait !  *****     ");
        wprompt(saved_win2,0,6,"             Press ESC to quit.         ");
//DYNAMIC : #if (NO_CAS3 != 0) -->
#ifdef EXTERNAL_CAS3_INSTALLED
        pmamftx(MAC_DIGITAL_TRUNK_MANUAL_TEST|MA_SYS_P|FEATURE_ACTIVATION,ch_no,OPR_T,0);
//      to be completed if (NO_CAS1 != 0)
#endif
        tst.prty_type=TEST_DGTL_TRNK_T;
        tst.prty_indx=ch_no;
        tst.result=TEST_NO_ANSWER;
        return ;
}
#endif
#endif

/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
#ifdef DTMF
void dtmf_menu_init_display(void){

        WINDOW *a;

        a=enw(25,8,5,32);
        if(a == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,display_test_menu:error in memory allocation!");
                quit(1);
        }
        dw(a);
        swb(a,DOUBLE_LINE);
//DOOD start
#if 0
        swc(a,ALL,MAGENTA,WHITE,BRIGHT);
        swc(a,ACCENT,AQUA,BLACK,BRIGHT);
#endif // 0
        swc(a,ALL,KT_MAGENTA,KT_WHITE,KT_HIGH_INTENSITY);
        swc(a,ACCENT,KT_CYAN,KT_BLACK,KT_HIGH_INTENSITY);
//DOOD end
        swt(a,"  DTMF PORT TEST  ");
        draw_window(a);
//DOOD start
//      swucf(a,GREEN);
        swucf(a,KT_GREEN);
//DOOD end
#ifdef RSU_CCSV5_INSTALLED
        wprompt(a,1,0,"1-Group switch unit.");
#else
        wprompt(a,1,0,"1-not used.");
#endif
        wprompt(a,1,1,"2-Port unit.");

        wprompt(a,1,2,"3-Return to test menu.");
        saved_win2=a;
}
#endif
#endif

/* ========================================================================= */
/* ========================================================================= */
#ifdef DEP_MANUAL_TEST
#ifdef DTMF
void dtmf_menu(void){  /* edit parameters */

        int b;
        if (test_menu_flag == RESET){
                dtmf_menu_init_display();
                test_menu_flag=FIRST_MENU;
                return;
        }
        if (test_menu_flag == FIRST_MENU){
                b=get_selection(saved_win2,1,"GPR");
                if (b == NO_KEY_PRESSED){
                      return;
                }
                switch(b){
                        case 1:
#ifdef RSU_CCSV5_INSTALLED
                                dtmf_test_init_display();
                                test_menu_flag=SECOND_MENU;
                                menu_item=GROUP_SWITCH_TEST;
#endif
                                return ;
                        case 2: dtmf_test_init_display();
                                test_menu_flag=SECOND_MENU;
                                menu_item=PORT_UNIT_TEST;
                                return ;
                        case 3 :
                                 deletw(saved_win2);
                                 test_menu_flag=RESET;
                                 test_2nd_menu_flag=RESET;
                                 return;
                       default: return;
                }
        }
        if (test_menu_flag == SECOND_MENU){
                dtmf_test();
        }
        return;

}
#endif
#endif
/* ========================================================================= */
/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
#ifdef DTMF
void dtmf_test(void){

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win3);
                switch (code){
                  case ESC :    exit_manual_test_with_2_menu();
                                return ;
                  case ENTER :  dtmf_test_part2();
                                return;
                  default :     return;
                }
        }
        else{    // ENTER key is pressed.

                switch (tst.result){
                  case TEST_NO_ANSWER : break;
                  case TEST_OK        :
                                        wprompt(saved_win3,0,5,"       *****  Test is Passed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_FAIL      :
                                        wprompt(saved_win3,0,5,"       *****  Test is Failed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_BUSY      :
                                        wprompt(saved_win3,0,5,"     *****  DTMF PORT is BUSY.  *****   ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  default             : break;
                }
                if (get_c() == ESC){
                        exit_manual_test_with_2_menu();
                }
        }
        return ;
}
#endif
#endif

/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
#ifdef DTMF
void dtmf_test_init_display(void){

        WINDOW *w;
        FIELD *f;

        w=enw(30,12,10,47);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,dtmf_test_init:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  DTMF PORT MANUAL TEST  ");
        draw_window(w);
        wprompt(w,0,1,"    Port Unit No.           :");
        wprompt(w,0,2,"                                 ----");
        wprompt(w,0,3,"    DTMF Port No.           :");
        wprompt(w,0,4,"                                 ----");
        wprompt(w,0,6,"      Press ENTER to start,ESC to quit. ");
        /* field allocation */
        idew(w);
        f=enf(w,33,1,test_port_unit_mask,test_port_unit,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,dtmf_test_init:error in memory allocation!");
                quit(1);
        }
        f=enf(w,33,3,test_dtmf_mask,test_dtmf,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,dtmf_test_init:error in memory allocation!");
                quit(1);
        }
        caf(w);
        saved_win3=w;
        data_entry_init(saved_win3);
        return ;
}
#endif
#endif
/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
#ifdef DTMF
void dtmf_test_part2(void){

        unsigned int dtmf_no,port_unit_no,max_dtmf;


        data_entry_flag=RESET;
        port_unit_no=-1;
        sscanf(test_port_unit,"%d",&port_unit_no);
        port_unit_no--;
        if (port_unit_no >= MAX_PORT_NO){
                wprompt(saved_win3,0,5,"          Invalid Port Unit No. !       ");
                wprompt(saved_win3,0,6,"             Press ESC to quit.         ");
                return;
        }

        wprompt(saved_win3,0,5,"        *****  Please Wait !  *****     ");
        wprompt(saved_win3,0,6,"             Press ESC to quit.         ");
#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (port_unit_no == 2){
                max_dtmf=NO_DTMF_C;
        }
        else
#endif
        if(port_unit_no == 1){
                max_dtmf=NO_DTMF_B;
        }
        else
#endif
        max_dtmf=NO_DTMF_A;

        dtmf_no=-1;
        sscanf(test_dtmf,"%d",&dtmf_no);
        dtmf_no--;
        if (dtmf_no >= max_dtmf){
                wprompt(saved_win3,0,5,"          Invalid DTMF Port No. !       ");
                wprompt(saved_win3,0,6,"             Press ESC to quit.         ");
                return;
        }

        wprompt(saved_win3,0,5,"        *****  Please Wait !  *****     ");
        wprompt(saved_win3,0,6,"             Press ESC to quit.         ");

        pmamftx(MAC_DTMF_MANUAL_TEST|MA_SYS_P|FEATURE_ACTIVATION,dtmf_no,OPR_T,port_unit_no);
        tst.prty_type=TEST_DTMF_T;
        tst.prty_indx=dtmf_no;
        tst.result=TEST_NO_ANSWER;
        return ;
}
#endif
#endif
/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
void swch_menu_init_display(void){

        WINDOW *a;

        a=enw(25,8,5,32);
        if(a == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,display_test_menu:error in memory allocation!");
                quit(1);
        }
        dw(a);
        swb(a,DOUBLE_LINE);
//DOOD start
#if 0
        swc(a,ALL,MAGENTA,WHITE,BRIGHT);
        swc(a,ACCENT,AQUA,BLACK,BRIGHT);
#endif // 0
        swc(a,ALL,KT_MAGENTA,KT_WHITE,KT_HIGH_INTENSITY);
        swc(a,ACCENT,KT_CYAN,KT_BLACK,KT_HIGH_INTENSITY);
//DOOD end
        swt(a,"  SWITCHING NETWORK TEST  ");
        draw_window(a);
//DOOD start
//      swucf(a,GREEN);
        swucf(a,KT_GREEN);
//DOOD end
#ifdef RSU_CCSV5_INSTALLED
        wprompt(a,1,0,"1-Group switch unit.");
#else
        wprompt(a,1,0,"1-not used.");
#endif
        wprompt(a,1,1,"2-Port unit.");

        wprompt(a,1,2,"3-Return to test menu.");
        saved_win2=a;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#ifdef DEP_MANUAL_TEST
void swch_menu(void){  /* edit parameters */

        int b;
        if (test_menu_flag == RESET){
                swch_menu_init_display();
                test_menu_flag=FIRST_MENU;
                return;
        }
        if (test_menu_flag == FIRST_MENU){
                b=get_selection(saved_win2,1,"GPR");
                if (b == NO_KEY_PRESSED){
                      return;
                }
                switch(b){
                        case 1:
#ifdef RSU_CCSV5_INSTALLED
                                swch_test_init_display();
                                test_menu_flag=SECOND_MENU;
                                menu_item=GROUP_SWITCH_TEST;
#endif
                                return ;
                        case 2: swch_test_init_display();
                                test_menu_flag=SECOND_MENU;
                                menu_item=PORT_UNIT_TEST;
                                return ;
                        case 3 :
                                 deletw(saved_win2);
                                 test_menu_flag=RESET;
                                 test_2nd_menu_flag=RESET;
                                 return;
                       default: return;
                }
        }
        if (test_menu_flag == SECOND_MENU){
                swch_test();
        }
        return;

}
#endif
/* ========================================================================= */


#ifdef DEP_MANUAL_TEST
void swch_test(void){

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win3);
                switch (code){
                  case ESC :    exit_manual_test_with_2_menu();
                                return ;
                  case ENTER :  swch_test_part2();
                                return;
                  default :     return;
                }
        }
        else{    // ENTER key is pressed.
                switch (tst.result){
                  case TEST_NO_ANSWER : break;
                  case TEST_OK        :
                                        wprompt(saved_win3,0,4,"       *****  Test is Passed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_FAIL      :
                                        wprompt(saved_win3,0,4,"       *****  Test is Failed. *****     ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  case TEST_BUSY      :
                                        wprompt(saved_win3,0,4,"   ***** Switching Network is BUSY. ***** ");
                                        tst.result=TEST_NO_ANSWER;
                                        break ;
                  default             : break;
                }
                if (get_c() == ESC){
                        exit_manual_test_with_2_menu();
                }
        }
        return ;
}
#endif

/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
void swch_test_init_display(void){

        WINDOW *w;
        FIELD *f;

        w=enw(30,12,10,47);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,swch_test_init:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  SWITCHING NETWORK MANUAL TEST  ");
        draw_window(w);
        wprompt(w,0,2,"    Port Unit No.           :");
        wprompt(w,0,3,"                                 ----");
/*
        wprompt(w,0,3,"    Switch No.              :");
        wprompt(w,0,4,"                                 ----");
*/
        wprompt(w,0,5,"      Press ENTER to start,ESC to quit. ");
        /* field allocation */
        idew(w);
        f=enf(w,33,2,test_port_unit_mask,test_port_unit,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,dtmf_test_init:error in memory allocation!");
                quit(1);
        }
/*
        f=enf(w,33,3,test_swch_mask,test_swch,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,swch_test_init:error in memory allocation!");
                quit(1);
        }
*/
        caf(w);
        saved_win3=w;
        data_entry_init(saved_win3);
        return ;
}
#endif
/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
void swch_test_part2(void){

//      unsigned int swch_no;
        unsigned int port_unit_no;

        data_entry_flag=RESET;
        port_unit_no=-1;
        sscanf(test_port_unit,"%d",&port_unit_no);
        port_unit_no--;
        if (port_unit_no >= MAX_PORT_NO){
                wprompt(saved_win3,0,4,"          Invalid Port Unit No. !       ");
                wprompt(saved_win3,0,5,"             Press ESC to quit.         ");
                return;
        }
/*
        swch_no=-1;
        sscanf(test_swch,"%d",&swch_no);
        swch_no--;
        if (swch_no >= MAX_SWITCH_NO){
                wprompt(saved_win3,0,5,"           Invalid  Switch No. !          ");
                wprompt(saved_win3,0,6,"             Press ESC to quit.         ");
                return;
        }
*/
        wprompt(saved_win3,0,4,"        *****  Please Wait !  *****     ");
        wprompt(saved_win3,0,5,"             Press ESC to quit.         ");

        pmamftx(MAC_SWCH_MANUAL_TEST|MA_SYS_P|FEATURE_ACTIVATION,TON,OPR_T,port_unit_no);
        tst.prty_type=TEST_SWCH_T;
        tst.prty_indx=port_unit_no;
        tst.result=TEST_NO_ANSWER;
        return ;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef DEP_MANUAL_TEST
void exit_manual_test_with_1_menu(void){

        tst.prty_type=TEST_NULL_T;
        tst.result=TEST_NO_ANSWER;
        data_entry_flag=RESET;
        pc(80,25);
        deletw(saved_win2);
        test_2nd_menu_flag = RESET;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef DEP_MANUAL_TEST
void exit_manual_test_with_2_menu(void){

        test_menu_flag=FIRST_MENU;
        tst.prty_type=TEST_NULL_T;
        tst.result=TEST_NO_ANSWER;
        data_entry_flag=RESET;
        pc(80,25);
        deletw(saved_win3);
}
#endif
/* ========================================================================= */
/* ========================================================================= */

#ifdef DEP_MANUAL_TEST
void r1_test_init_display(void){
        WINDOW *w;
        FIELD *f;
        w=enw(30,8,10,47);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,r1_test_init:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  R1 SIGNALING TST REQUEST  ");
        draw_window(w);
        wprompt(w,0,1," Please  Enter  Channel  No. :");
        wprompt(w,0,2," ---------------------------     -------");
        wprompt(w,0,4,"  Press ENTER to start,ESC to quit. ");
        /* field allocation */
        idew(w);
        f=enf(w,33,1,test_ext_mask,test_ext,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,r1_test_init:error in memory allocation!");
                quit(1);
        }
        caf(w);
        saved_win2=w;
        data_entry_init(saved_win2);
        return ;
}
void r1_test_part2(void){
        WINDOW *w;
        unsigned int b;
        b=-1;
        data_entry_flag=RESET;
        sscanf(test_ext,"%d",&b);
        b--;
#if (defined EXTERNAL_CAS3_INSTALLED)
        if (b >= no_cas3[0]){
#elif (defined CAS1_INSTALLED)
        if (b >= no_cas1[0]){
#elif (defined EM_INSTALLED)
        if (b >= no_em[0]){
#else
        if (b >= 0){    // just to avoid compiler syntax error when we do have none of above
#endif
                wprompt(saved_win2,0,4,"           Invalid   Channel  No. !        ");
                wprompt(saved_win2,0,6,"              Press ESC to quit.           ");
                return;
        }
        deletw(saved_win2);
        w=enw(0,0,20,80);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,r1_test_part2:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"  R1  CHANNEL TEST DISPLAY  ");
        draw_window(w);
        wcursor(w,0,0);
        wprintf(w,"Channel Number UT: %s.  Press ESC to quit.",test_ext);
        saved_win2=w;
        pmamftx(MAC_R1_TEST_REQUEST|MA_SYS_P|FEATURE_ACTIVATION,(unsigned int)b,OPR_T,0);
        // in future may be we should do something about the case of testing r1 in limb or limc
        // for now, only test lima

        wcursor(saved_win2,0,1);
}
void r1_test(void){
        int code;
        if (data_entry_flag == SET){
                code=data_entry(saved_win2);
                switch (code){
                  case ESC :
                                exit_manual_test_with_1_menu();
                                return ;
                  case ENTER :  r1_test_part2();
                                return;
                  default :     return;
                }
        }
        else{    // ENTER key is pressed.
                if (get_c() == ESC){
                        pmamftx(MAC_R1_TEST_REQUEST|MA_SYS_P|FEATURE_ACTIVATION,0xfff,OPR_T,0);
                        exit_manual_test_with_1_menu();
                }
        }
        return ;
}
#endif

/* ========================================================================= */
/* ========================================================================= */

#endif // (OS == DOS) || (OS == LINUX_COMPATIBLE)
