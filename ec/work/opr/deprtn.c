
//NEW_DEP
//#include "..\omtfp.h"
#include "omtfp.h"
#ifdef NEW_DEP
#include "opr_net.h"
//DOOD : #ifdef added
#if (OS == DOS)
#include <conio.h>
//DOOD : #endif added
#endif
#include <string.h>
#include <stdlib.h>
#include "wg.h"
#include "depmtfp.h"

#ifdef ONLINE_EDIT
extern TRUNK_EDIT_LINE tel[];
extern TRUNK_EDIT_LINE prv_tel[];
extern EXT_EDIT_LINE xel[];
extern EXT_EDIT_LINE prv_xel[];
extern BYTE saved_n;
extern long nelv[];
extern long prv_nelv[];
extern HOT_LINE_EDIT_LINE hlel[];
extern HOT_LINE_EDIT_LINE prv_hlel[];
extern unsigned long oclv;
extern unsigned long prv_oclv;
#ifdef EM_GROUP_PROGRAM
extern unsigned int emgpelv[];
extern unsigned int prv_emgpelv[];
#endif

//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
extern DIGITAL_TRUNK_EDIT_LINE dtel[];  /* digital trunk edit line */
extern DIGITAL_TRUNK_EDIT_LINE prv_dtel[];  /* digital trunk edit line */
#endif
#endif
//CPULOAD : BYTE is added
extern BYTE mac_nite[];
#ifdef CB_INSTALLED
extern int no_cb[];
#endif
//NEW_OPR start
/* ====================================================================== */

#if 0
#ifdef ONLINE_EDIT
int pmamftx(int j,int i,int t,BYTE n){
                /* put ma message for tx,work j for index i */
                /* & type t to system.If fail return ERR_CODE */

        pc(0,0);
//DOOD : printf->print_error_report
        print_error_report("\nMAC=%x  ",j);
//DOOD : printf->print_error_report
        print_error_report("indx=%d  ",i);
        getch();

/*
        mb[0]=MA_T | (j<<4);
        mb[1]=j>>4;
        mb[2]=t | (i<<4);
        mb[3]=i>>4;
        return(ttxq(mb,MIN_I_PKT,n));
*/
}
#endif

#endif // 0
//NEW_OPR end
/* ====================================================================== */
/* ====================================================================== */
#ifdef ONLINE_EDIT
void send_ext_ctgv_pckt(unsigned int ext_indx,BYTE c){

        BYTE s;
        unsigned int val;
//GRPPRG start
#ifndef NEW_ANALOG_TRUNK_GROUPING
        unsigned int prv_val;
#endif
//GRPPRG end
        unsigned int mac,pos;

#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (ext_indx >= (NO_EXTENSION_A+NO_EXTENSION_B))
                ext_indx -= (NO_EXTENSION_A+NO_EXTENSION_B);
        else
#endif
        if (ext_indx >= NO_EXTENSION_A)
                ext_indx -= NO_EXTENSION_A;
#endif

        val=xel[c].ctgv;
        s=xel[c].ctgv ^ prv_xel[c].ctgv;

//CPULOAD start : "{" and "}" are added.
//GRPPRG : the following line added
#ifndef NEW_ANALOG_TRUNK_GROUPING
        if ( s & 0x80 ){
                if (xel[c].ctgv & 0x80 ){
                        pmamftx_both_sys(MAC_CO_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
                }
                else{
                        pmamftx_both_sys(MAC_CO_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
                        return;
                }
        }
//CPULOAD end
        if ( s & 0x40 ){
               if (xel[c].ctgv & 0x40 ){
                        pmamftx_both_sys(MAC_CO_GRP_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CO_GRP_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
        if (!(xel[c].ctgv & 0x40) ){
                val=1<< ((xel[c].ctgv&0x3f)-1);
                prv_val=1<<((prv_xel[c].ctgv&0x3f)-1);
                s=val ^ prv_val;
        }
//GRPPRG : the following line added
#endif // ANALOG_TRUNK_GROUPING
        for (pos=0x01,mac=MAC_CO_GRP0;mac<=MAC_CO_GRP5;pos<<=1,mac++){


                if ( s & pos ){
                        if (val & pos )
                                pmamftx_both_sys(mac|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
                        else
                                pmamftx_both_sys(mac|MA_X_P,ext_indx,XST_T,saved_n);
                }
       }
}
#endif
/* ====================================================================== */
/* ========================================================================== */
#ifdef ONLINE_EDIT
#if (  (!defined NO_TIE_INIT) || (defined EM_GROUP_PROGRAM) )

void send_ext_ttgv_pckt(unsigned int ext_indx,BYTE c){

        BYTE s;
#ifndef NEW_ANALOG_TRUNK_GROUPING
        unsigned int prv_val;
#endif
        unsigned int val;
        unsigned int mac,pos;

#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (ext_indx >= (NO_EXTENSION_A+NO_EXTENSION_B))
                ext_indx -= (NO_EXTENSION_A+NO_EXTENSION_B);
        else
#endif
        if (ext_indx >= NO_EXTENSION_A)
                ext_indx -= NO_EXTENSION_A;
#endif

        s=xel[c].ttgv ^ prv_xel[c].ttgv;
        val=xel[c].ttgv;
//...........................................................................
//GRPPRG : the following line added
#ifndef NEW_ANALOG_TRUNK_GROUPING
        if ( s & 0x80 ){
                if (xel[c].ttgv & 0x80 ){

#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
                         pmamftx_both_sys(MAC_TIE_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#else
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                        // do nothing
#else /* EM_PULSE_SIGNALING */
                        // do nothing
#endif

#endif
#else
#ifndef NO_TIE_INIT
                        pmamftx_both_sys(MAC_TIE_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#endif
#endif
                }
                else{
#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
                         pmamftx_both_sys(MAC_TIE_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
#else
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                        // do nothing
#else /* EM_PULSE_SIGNALING */
                        // do nothing
#endif

#endif
#else
#ifndef NO_TIE_INIT
                        pmamftx_both_sys(MAC_TIE_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
#endif
#endif
                }
        }
//...........................................................................
//...........................................................................
        if ( s & 0x40 ){
                if (xel[c].ttgv & 0x40 ){
#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
                         pmamftx_both_sys(MAC_TIE_GRP_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#else
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                         pmamftx_both_sys(MAC_EM_GRP_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#else /* EM_PULSE_SIGNALING */
                         pmamftx_both_sys(MAC_EM_GRP_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#endif

#endif
#else
#ifndef NO_TIE_INIT
                        pmamftx_both_sys(MAC_TIE_GRP_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#endif
#endif
                }
                else{
#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
                         pmamftx_both_sys(MAC_TIE_GRP_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
#else
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                         pmamftx_both_sys(MAC_EM_GRP_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#else /* EM_PULSE_SIGNALING */
                         pmamftx_both_sys(MAC_EM_GRP_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#endif

#endif
#else
#ifndef NO_TIE_INIT
                        pmamftx_both_sys(MAC_TIE_GRP_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
#endif
#endif
                }
        }
//...........................................................................
        if (!(xel[c].ttgv & 0x40) ){
                val=1<< ((xel[c].ttgv&0x3f)-1);
                prv_val=1<<((prv_xel[c].ttgv&0x3f)-1);
                s=val ^ prv_val;
        }
//...........................................................................
//GRPPRG : the following line added
#endif // NEW_ANALOG_TRUNK_GROUPING

#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
        for (pos=0x01,mac=MAC_TIE_GRP0;mac<=MAC_TIE_GRP5;pos<<=1,mac++)
#else
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
        for (pos=0x01,mac=MAC_EM_GRP0;mac<=MAC_EM_GRP5;pos<<=1,mac++)
#else /* EM_PULSE_SIGNALING */
        for (pos=0x01,mac=MAC_EM_GRP0;mac<=MAC_EM_GRP5;pos<<=1,mac++)
#endif

#endif
#else
#ifndef NO_TIE_INIT
        for (pos=0x01,mac=MAC_TIE_GRP0;mac<=MAC_TIE_GRP5;pos<<=1,mac++)
#endif
#endif
        {
            if ( s & pos ){
                if (val & pos ){
#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
                         pmamftx_both_sys(mac|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#else
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                         pmamftx_both_sys(mac|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#else /* EM_PULSE_SIGNALING */
                         pmamftx_both_sys(mac|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#endif

#endif
#else
#ifndef NO_TIE_INIT
                        pmamftx_both_sys(mac|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#endif
#endif
                }
                else{
#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
                         pmamftx_both_sys(mac|MA_X_P,ext_indx,XST_T,saved_n);
#else
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                         pmamftx_both_sys(mac|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#else /* EM_PULSE_SIGNALING */
                         pmamftx_both_sys(mac|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
#endif

#endif
#else
#ifndef NO_TIE_INIT
                        pmamftx_both_sys(mac|MA_X_P,ext_indx,XST_T,saved_n);
#endif
#endif
                }
            }
        }

}
#endif
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef ONLINE_EDIT
#ifdef EM_GROUP_PROGRAM
#ifdef EM_GROUP_DIALLED
void send_ext_emtgv_pckt(unsigned int ext_indx,BYTE c){

        unsigned int s;
        unsigned int mac,pos;

#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (ext_indx >= (NO_EXTENSION_A+NO_EXTENSION_B))
                ext_indx -= (NO_EXTENSION_A+NO_EXTENSION_B);
        else
#endif
        if (ext_indx >= NO_EXTENSION_A)
                ext_indx -= NO_EXTENSION_A;
#endif

        s=xel[c].emtgv ^ prv_xel[c].emtgv;
/*
        if ( s & 0x8000 )
                if (xel[c].emtgv & 0x8000 )
                        pmamftx_both_sys(MAC_EM|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
                else{
                        pmamftx_both_sys(MAC_EM|MA_X_P,ext_indx,XST_T,saved_n);
                        return;
                }
*/
//GRPPRG : the following line added
#ifndef NEW_ANALOG_TRUNK_GROUPING
        if ( s & 0x4000 ){
               if (xel[c].emtgv & 0x4000 ){
                        pmamftx_both_sys(MAC_EM_GRP_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_EM_GRP_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//GRPPRG : the following line added
#endif // NEW_ANALOG_TRUNK_GROUPING

        for (pos=0x0001,mac=MAC_EM_GRP0;mac<=MAC_EM_GRP13;pos<<=1,mac++){
                if ( s & pos ){
                        if (xel[c].emtgv & pos )
                                pmamftx_both_sys(mac|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
                        else
                                pmamftx_both_sys(mac|MA_X_P,ext_indx,XST_T,saved_n);
                }
        }

}
#endif
#endif
#endif
/* ====================================================================== */
/* ========================================================================== */
#ifdef ONLINE_EDIT
void send_ext_cstv_pckt(unsigned int ext_indx,BYTE c){

        unsigned int s;

#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (ext_indx >= (NO_EXTENSION_A+NO_EXTENSION_B))
                ext_indx -= (NO_EXTENSION_A+NO_EXTENSION_B);
        else
#endif
        if (ext_indx >= NO_EXTENSION_A)
                ext_indx -= NO_EXTENSION_A;
#endif

        s=xel[c].cstv ^ prv_xel[c].cstv;
//CB
//CPULOAD start : "{" and "}" are added.
        if ( s & CALL_BACK ){
                if (xel[c].cstv & CALL_BACK ){
                        pmamftx_both_sys(MAC_CB_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
                }
                else{
                        pmamftx_both_sys(MAC_CB_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
                }
        }
//CPULOAD end
//CP
        if ( s & CAMP_ON ){
               if (xel[c].cstv & CAMP_ON ){
                        pmamftx_both_sys(MAC_CAMPON_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CAMPON_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//FM
        if ( s & FOLLOW_ME ){
               if (xel[c].cstv & FOLLOW_ME ){
                        pmamftx_both_sys(MAC_FM_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_FM_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//ST
//#if (NO_CO != 0)
        if ( s & STD ){
               if (xel[c].cstv & STD ){
                        pmamftx_both_sys(MAC_CO_1ST0|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CO_1ST0|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//#endif
//IN
//#if (NO_CO != 0)
        if ( s & INTERNATIONAL ){
               if (xel[c].cstv & INTERNATIONAL ){
                        pmamftx_both_sys(MAC_CO_2ST0|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CO_2ST0|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//#endif
//DD
        if ( s & DONT_DIST ){
               if (xel[c].cstv & DONT_DIST ){
                        pmamftx_both_sys(MAC_DONTD_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_DONTD_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//CR
//#if ((NO_CO != 0) || (defined BAD_PAYER))
        if ( s & RING_AGAIN_COT ){
               if (xel[c].cstv & RING_AGAIN_COT ){
                        pmamftx_both_sys(MAC_CO_RSRV|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CO_RSRV|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//#endif
//CF
#ifdef CONFERENCE_ACT
        if ( s & CONFERENCE_SERVICE ){
               if (xel[c].cstv & CONFERENCE_SERVICE ){
                        pmamftx_both_sys(MAC_CONF_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CONF_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
#endif
//T0
//#if ((defined BAD_PAYER) || (defined EM_INSTALLED) || (!defined NO_TIE_INIT))
        if ( s & TIE_0 ){
               if (xel[c].cstv & TIE_0 ){
                        pmamftx_both_sys(MAC_TIE_1ST0|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_TIE_1ST0|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//#endif
//PU
        if ( s & CALL_PICKUP_USE ){
               if (xel[c].cstv & CALL_PICKUP_USE ){
                        pmamftx_both_sys(MAC_CPU_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CPU_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//TR
#ifdef DTMF
        if ( s & RING_AGAIN_TIET ){
               if (xel[c].cstv & RING_AGAIN_TIET ){
                        pmamftx_both_sys(MAC_TIE_RSRV|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_TIE_RSRV|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
#endif
//HL
        if ( s & HOT_LINE_ACCESS ){
               if (xel[c].cstv & HOT_LINE_ACCESS ){
                        pmamftx_both_sys(MAC_HOT_LINE_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_HOT_LINE_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//SA
//#if ((defined BAD_PAYER) || (defined SIREN_INSTALLED ))
        if ( s & SIREN_ACCESS ){
               if (xel[c].cstv & SIREN_ACCESS ){
                        pmamftx_both_sys(MAC_SIREN_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_SIREN_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//#endif
//AA
//#if ((defined AMPLIFIER_INSTALLED) || (defined HOME_METERING))
        if ( s & AMPLIFIER_ACCESS ){
               if (xel[c].cstv & AMPLIFIER_ACCESS ){
                        pmamftx_both_sys(MAC_AMPLIFIER_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_AMPLIFIER_ACCESS|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//#endif
}
#endif
/* ====================================================================== */
/* ========================================================================== */
#ifdef ONLINE_EDIT
void send_ext_astv_pckt(unsigned int ext_indx,BYTE c){

        unsigned int s;

#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (ext_indx >= (NO_EXTENSION_A+NO_EXTENSION_B))
                ext_indx -= (NO_EXTENSION_A+NO_EXTENSION_B);
        else
#endif
        if (ext_indx >= NO_EXTENSION_A)
                ext_indx -= NO_EXTENSION_A;
#endif

        s=xel[c].astv ^ prv_xel[c].astv;
//FA
//CPULOAD start : "{" and "}" are added.
        if ( s & FEATURE_ACTIVE ){
                if (xel[c].astv & FEATURE_ACTIVE ){
                        pmamftx_both_sys(MAC_EXT_FEATURE|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
                }
                else{
                        pmamftx_both_sys(MAC_EXT_FEATURE|MA_X_P,ext_indx,XST_T,saved_n);
                }
        }
//CPULOAD end
//MC
        if ( s & MALICIOUS_CALL_DTCT ){
               if (xel[c].astv & MALICIOUS_CALL_DTCT ){
                        pmamftx_both_sys(MAC_MALICIOUS_DTCT|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_MALICIOUS_DTCT|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }
//CG
        if ( s & CALL_GEN ){
               if (xel[c].astv & CALL_GEN ){
                        pmamftx_both_sys(MAC_CALL_GEN|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CALL_GEN|MA_X_P,ext_indx,XST_T,saved_n);
               }
        }

}
#endif
/* ====================================================================== */
/* ========================================================================== */
#ifdef ONLINE_EDIT
void send_ext_hliv_pckt(unsigned int ext_indx,BYTE c){

        unsigned int s;

#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (ext_indx >= (NO_EXTENSION_A+NO_EXTENSION_B))
                ext_indx -= (NO_EXTENSION_A+NO_EXTENSION_B);
        else
#endif
        if (ext_indx >= NO_EXTENSION_A)
                ext_indx -= NO_EXTENSION_A;
#endif

        s=xel[c].hliv ^ prv_xel[c].hliv;
        if ( s )
                pmamftx_both_sys((MAC_TO_HOT_LINE0+xel[c].hliv)|MA_X_P|FEATURE_ACTIVATION,ext_indx,XST_T,saved_n);
        else
                pmamftx_both_sys((MAC_TO_HOT_LINE0+xel[c].hliv)|MA_X_P,ext_indx,XST_T,saved_n);
}
#endif
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
#if (NO_CO != 0)
void send_co_ai_pckt(BYTE co_indx,BYTE c){

        unsigned int s;

        s=tel[c].ai ^ prv_tel[c].ai;

        if ( s & 0xf000 ){
                pmamftx_both_sys((((tel[c].ai>>12)+MAC_TRNK_GRP0)|MA_COT_P|FEATURE_ACTIVATION),co_indx,CO_T,saved_n);
        }
        if ( s & IS_MASK ){
               if (tel[c].ai & IS_MASK ){
                        pmamftx_both_sys(MAC_TRNK_SERVICE|MA_COT_P|FEATURE_ACTIVATION,co_indx,CO_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_TRNK_SERVICE|MA_COT_P,co_indx,CO_T,saved_n);
               }
        }
        if ( s & DA_MASK ){
//??????????????????????????????
               if (tel[c].ai & DA_MASK ){
//                      pmamftx_both_sys(MAC_DID_INACTIVE|MA_COT_P|FEATURE_ACTIVATION,co_indx,CO_T,saved_n);
               }
               else{
                        pmamftx_both_sys((MAC_DID_INACTIVE|MA_COT_P|FEATURE_ACTIVATION),co_indx,CO_T,saved_n);
               }
        }
        if ( s & TA_MASK ){
               if (tel[c].ai & TA_MASK ){
                        pmamftx_both_sys(MAC_T_TIMING_ACT|MA_COT_P|FEATURE_ACTIVATION,co_indx,CO_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_T_TIMING_ACT|MA_COT_P,co_indx,CO_T,saved_n);
               }
        }
#ifdef TOLL_BARRING_ON_COs
        if ( s & TOLL_ACCESS_MASK ){
               if (tel[c].ai & TOLL_ACCESS_MASK ){
                        pmamftx_both_sys(MAC_TOLL_ACCESS|MA_COT_P|FEATURE_ACTIVATION,co_indx,CO_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_TOLL_ACCESS|MA_COT_P,co_indx,CO_T,saved_n);
               }
        }
#endif
}
#endif

/* ====================================================================== */
#if (NO_CO != 0)
void send_co_ddbv_pckt(BYTE co_indx,BYTE c){

        unsigned int s;
        unsigned int ext_indx;

        ext_indx=tel[c].ddbv;

        s=tel[c].ddbv ^ prv_tel[c].ddbv;

        if ( s ){
                pmamftx_both_sys((MAC_TRNK_DID|MA_COT_P|FEATURE_ACTIVATION),co_indx,CO_T,saved_n);
                pmamftx_both_sys((MAC_EXT_DID|MA_X_P|FEATURE_ACTIVATION),ext_indx,XST_T,saved_n);
        }

}
#endif

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
#ifndef NO_TIE_INIT
void send_tie_ai_pckt(BYTE tie_indx,BYTE c){

        unsigned int s;

        s=tel[c].ai ^ prv_tel[c].ai;

        if ( s & TRUNK_GROUP_MASK ){
                pmamftx_both_sys((((tel[c].ai & TRUNK_GROUP_MASK)+MAC_TRNK_GRP0)|MA_TIET_P|FEATURE_ACTIVATION),tie_indx,TIE_T,saved_n);
        }
        if ( s & IS_MASK ){
               if (tel[c].ai & IS_MASK ){
                        pmamftx_both_sys(MAC_TRNK_SERVICE|MA_TIET_P|FEATURE_ACTIVATION,tie_indx,TIE_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_TRNK_SERVICE|MA_TIET_P,tie_indx,TIE_T,saved_n);
               }
        }
        if ( s & DA_MASK ){
               if (tel[c].ai & DA_MASK ){
//                      pmamftx_both_sys(MAC_DID_INACTIVE|MA_TIET_P|FEATURE_ACTIVATION,tie_indx,TIE_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_DID_INACTIVE|MA_TIET_P|FEATURE_ACTIVATION,tie_indx,TIE_T,saved_n);
               }
        }
        if ( s & TA_MASK ){
               if (tel[c].ai & TA_MASK ){
                        pmamftx_both_sys(MAC_T_TIMING_ACT|MA_TIET_P|FEATURE_ACTIVATION,tie_indx,TIE_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_T_TIMING_ACT|MA_TIET_P,tie_indx,TIE_T,saved_n);
               }
        }


}
#endif

/* ====================================================================== */
#ifndef NO_TIE_INIT
void send_tie_ddbv_pckt(BYTE tie_indx,BYTE c){

        unsigned int s;
        unsigned int ext_indx;

        ext_indx=tel[c].ddbv;

        s=tel[c].ddbv ^ prv_tel[c].ddbv;

        if ( s ){
                pmamftx_both_sys((MAC_TRNK_DID|MA_TIET_P|FEATURE_ACTIVATION),tie_indx,TIE_T,saved_n);
                pmamftx_both_sys((MAC_EXT_DID|MA_X_P|FEATURE_ACTIVATION),ext_indx,XST_T,saved_n);
        }

}
#endif
/* ====================================================================== */
void send_eonx_nel_pckt(BYTE nel_indx){

        unsigned int s;
        unsigned int i,prv_i;
        BYTE t,prv_t;


        prv_i=(unsigned int)prv_nelv[nel_indx];
        prv_t=(int)(prv_i & 0x0f);
        prv_i >>= 4;

        i=(unsigned int)nelv[nel_indx];
        t=(int)(i & 0x0f);
        i >>= 4;

        s=(unsigned int )(i ^ prv_i);
        if ( (prv_t == XST_T) && (t == NULL_T)){
                pmamftx_both_sys(((/*nel_indx+*/mac_nite[TON])|MA_X_P),prv_i,XST_T,saved_n);
                return;
        }
        if ( (prv_t == NULL_T) && (t == XST_T)){
                pmamftx_both_sys(((/*nel_indx+*/mac_nite[TON])|MA_X_P|FEATURE_ACTIVATION),i,XST_T,saved_n);
                return;
        }
        if ((prv_t == XST_T) && (t == XST_T)){
                if (s){
                        pmamftx_both_sys(((/*nel_indx+*/mac_nite[TON])|MA_X_P),prv_i,XST_T,saved_n);
                        pmamftx_both_sys(((/*nel_indx+*/mac_nite[TON])|MA_X_P|FEATURE_ACTIVATION),i,XST_T,saved_n);
                }
        }

}
/* ====================================================================== */
#ifdef HOT_LINE_SERVICE_ACTIVE
void send_edhl_hlel_pckt(BYTE hlel_indx){

        unsigned int s,i,prv_i;
        BYTE t,prv_t;

        s=(unsigned int)(hlel[hlel_indx].dhv ^ prv_hlel[hlel_indx].dhv);

        i=(unsigned int)hlel[hlel_indx].dhv;
        t=(int)(i & 0x0f);
        i >>= 4;

        prv_i=(unsigned int)prv_hlel[hlel_indx].dhv;
        prv_t=(int)(prv_i & 0x0f);
        prv_i >>= 4;

        if (s){
                /* removing previous hot line*/
                switch(prv_t){
                  case XST_T: pmamftx_both_sys(((hlel_indx+MAC_X_HOT_LINE0)|MA_X_P),prv_i,XST_T,saved_n);
                              break;
                  case CO_T:  pmamftx_both_sys(((hlel_indx+MAC_T_HOT_LINE0)|MA_COT_P),prv_i,CO_T,saved_n);
                              break;
                  case TIE_T: pmamftx_both_sys(((hlel_indx+MAC_T_HOT_LINE0)|MA_TIET_P),prv_i,TIE_T,saved_n);
                              break;
                  default:    break;
                }
                switch(t){
                  case XST_T: pmamftx_both_sys(((hlel_indx+MAC_X_HOT_LINE0)|MA_X_P|FEATURE_ACTIVATION),i,XST_T,saved_n);
                              break;
                  case CO_T:  pmamftx_both_sys(((hlel_indx+MAC_T_HOT_LINE0)|MA_COT_P|FEATURE_ACTIVATION),i,CO_T,saved_n);
                              break;
                  case TIE_T: pmamftx_both_sys(((hlel_indx+MAC_T_HOT_LINE0)|MA_TIET_P|FEATURE_ACTIVATION),i,TIE_T,saved_n);
                              break;
                  default:    break;
                }
        }

}
#endif
/* ====================================================================== */
#ifdef R1_SIGNALINGP
void send_office_code_ocl_pckt(void){
/*
//DOOD : printf->print_error_report
        print_error_report("\noclv=%ld",oclv);
//DOOD : printf->print_error_report
        print_error_report("\nprv_oclv=%ld",prv_oclv);
*/
        pmamftx_both_sys(MAC_OFFICE_CODE_PART1 | MA_SYS_P | FEATURE_ACTIVATION, (int)(oclv%1000L), OPR_T,0);
        pmamftx_both_sys(MAC_OFFICE_CODE_PART2 | MA_SYS_P | FEATURE_ACTIVATION, (int)(oclv/1000L), OPR_T,0);
#ifdef LIM
        pmamftx_both_sys(MAC_OFFICE_CODE_PART1 | MA_SYS_P | FEATURE_ACTIVATION, (int)(oclv%1000L), OPR_T,1);
        pmamftx_both_sys(MAC_OFFICE_CODE_PART2 | MA_SYS_P | FEATURE_ACTIVATION, (int)(oclv/1000L), OPR_T,1);
#if (NO_EXTENSION_C != 0)
        pmamftx_both_sys(MAC_OFFICE_CODE_PART1 | MA_SYS_P | FEATURE_ACTIVATION, (int)(oclv%1000L), OPR_T,2);
        pmamftx_both_sys(MAC_OFFICE_CODE_PART2 | MA_SYS_P | FEATURE_ACTIVATION, (int)(oclv/1000L), OPR_T,2);
#endif
#endif
}
#endif
/* ====================================================================== */

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

#ifdef EM_GROUP_PROGRAM
void send_em_ai_pckt(BYTE em_indx,BYTE c){

        unsigned int s;

        s=tel[c].ai ^ prv_tel[c].ai;

        if ( s & EM_TRUNK_GROUP_MASK ){
                pmamftx_both_sys(( ((tel[c].ai & EM_TRUNK_GROUP_MASK) >> EMTGM_LOC) +
                            MAC_EM_TRNK_GRP0)|MA_EM_CAS_P|FEATURE_ACTIVATION,em_indx,EM_T,saved_n);
        }
        if ( s & IS_MASK ){
               if (tel[c].ai & IS_MASK ){
                        pmamftx_both_sys(MAC_EM_IN_SERVICE|MA_EM_CAS_P|FEATURE_ACTIVATION,em_indx,EM_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_EM_IN_SERVICE|MA_EM_CAS_P,em_indx,EM_T,saved_n);
               }
        }
#ifdef EM_CONTINUOUS_SIGNALING
        if ( s & S9_MASK ){
               if (tel[c].ai & S9_MASK ){
                        pmamftx_both_sys(MAC_EM_SEND9|MA_EM_CAS_P|FEATURE_ACTIVATION,em_indx,EM_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_EM_SEND9|MA_EM_CAS_P,em_indx,EM_T,saved_n);
               }
        }
        if ( s & DA_MASK ){
               if (tel[c].ai & DA_MASK ){
                        pmamftx_both_sys(MAC_EM_DEST_TRNST|MA_EM_CAS_P|FEATURE_ACTIVATION,em_indx,EM_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_EM_DEST_TRNST|MA_EM_CAS_P,em_indx,EM_T,saved_n);
               }
        }
        if ( s & TA_MASK ){
               if (tel[c].ai & TA_MASK ){
                        pmamftx_both_sys(MAC_EM_LPBACK|MA_EM_CAS_P|FEATURE_ACTIVATION,em_indx,EM_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_EM_LPBACK|MA_EM_CAS_P,em_indx,EM_T,saved_n);
               }
        }
#endif
}
#endif

/* ====================================================================== */
/* ====================================================================== */
#ifdef EM_GROUP_PROGRAM
#ifdef EM_CONTINUOUS_SIGNALING
void send_em_t1_pckt(BYTE em_indx,BYTE c){

        unsigned long s;

        s=tel[c].t1v^ prv_tel[c].t1v;
        if (s){
                pmamftx_both_sys((MAC_EM_TIME1|MA_EM_CAS_P),em_indx,EM_T,saved_n);
//second macro ?????????
//              pmamftx_both_sys((MAC_EM_TIME1|MA_EM_CAS_P),tel[c].t1v,EM_T,saved_n);
        }
}
#endif
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef EM_GROUP_PROGRAM
#ifdef EM_CONTINUOUS_SIGNALING
void send_em_t2_pckt(BYTE em_indx,BYTE c){

        unsigned long s;

        s=tel[c].t2v ^ prv_tel[c].t2v;
        if (s){
                pmamftx_both_sys((MAC_EM_TIME2|MA_EM_CAS_P),em_indx,EM_T,saved_n);
//second macro ?????????
//              pmamftx_both_sys((MAC_EM_TIME2|MA_EM_CAS_P),tel[c].t2v,EM_T,saved_n);
        }
}
#endif
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef EM_GROUP_PROGRAM
void send_em_ddbv_pckt(BYTE em_indx,BYTE c){

        unsigned int s;
        BYTE val;

        s=tel[c].ddbv ^ prv_tel[c].ddbv;
        val=tel[c].ddbv;
        if ( s ){
                if ( s & 0x01){
                        if (val & 0x01){
                                pmamftx_both_sys((MAC_EM_OGT|MA_EM_CAS_P|FEATURE_ACTIVATION),em_indx,EM_T,saved_n);
                        }
                        else{
                                pmamftx_both_sys((MAC_EM_OGT|MA_EM_CAS_P),em_indx,EM_T,saved_n);
                        }
                }
                if ( s & 0x02){
                        if (val & 0x02){
                                pmamftx_both_sys((MAC_EM_ICT|MA_EM_CAS_P|FEATURE_ACTIVATION),em_indx,EM_T,saved_n);
                        }
                        else{
                                pmamftx_both_sys((MAC_EM_ICT|MA_EM_CAS_P),em_indx,EM_T,saved_n);
                        }
                }
                if ( s & 0x04){
                        if (val & 0x04){
                                pmamftx_both_sys((MAC_INTER_SIG_DGT_SEND|MA_EM_CAS_P|FEATURE_ACTIVATION),em_indx,EM_T,saved_n);
                        }
                        else{
                                pmamftx_both_sys((MAC_INTER_SIG_DGT_SEND|MA_EM_CAS_P),em_indx,EM_T,saved_n);
                        }
                }
                if ( s & 0x08){
                        if (val & 0x08){
                                pmamftx_both_sys((MAC_ANSWER_TX_IC|MA_EM_CAS_P|FEATURE_ACTIVATION),em_indx,EM_T,saved_n);
                        }
                        else{
                                pmamftx_both_sys((MAC_ANSWER_TX_IC|MA_EM_CAS_P),em_indx,EM_T,saved_n);
                        }
                }
                if ( s & 0x10){
                        if (val & 0x10){
                                pmamftx_both_sys((MAC_ANSWER_RX_OG|MA_EM_CAS_P|FEATURE_ACTIVATION),em_indx,EM_T,saved_n);
                        }
                        else{
                                pmamftx_both_sys((MAC_ANSWER_RX_OG|MA_EM_CAS_P),em_indx,EM_T,saved_n);
                        }
                }
                if ( s & 0x20){
                        if (val & 0x20){
                                pmamftx_both_sys((MAC_IMMEDIATE_ACK|MA_EM_CAS_P|FEATURE_ACTIVATION),em_indx,EM_T,saved_n);
                        }
                        else{
                                pmamftx_both_sys((MAC_IMMEDIATE_ACK|MA_EM_CAS_P),em_indx,EM_T,saved_n);
                        }
                }
                if ( s & 0x40){
                        if (val & 0x40){
                                pmamftx_both_sys((MAC_WAIT_FOR_ACK|MA_EM_CAS_P|FEATURE_ACTIVATION),em_indx,EM_T,saved_n);
                        }
                        else{
                                pmamftx_both_sys((MAC_WAIT_FOR_ACK|MA_EM_CAS_P),em_indx,EM_T,saved_n);
                        }
                }
                if ( s & 0x80){
                        if (val & 0x80){
                                pmamftx_both_sys((MAC_EOS_NEEDED|MA_EM_CAS_P|FEATURE_ACTIVATION),em_indx,EM_T,saved_n);
                        }
                        else{
                                pmamftx_both_sys((MAC_EOS_NEEDED|MA_EM_CAS_P),em_indx,EM_T,saved_n);
                        }
                }
        }

}
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef EM_GROUP_PROGRAM
void send_em_emgpel_pckt(BYTE grp_indx){

        unsigned long s;

        s=emgpelv[grp_indx]^ prv_emgpelv[grp_indx];
//??????????
        if (s){
                pmamftx_both_sys(((MAC_EM_GRP0_DIGIT+grp_indx)|MA_EM_CAS_P),(unsigned int)emgpelv[grp_indx],EM_T,saved_n);
        }
}
#endif
/* ====================================================================== */
//DYNAMIC : to be corrected.
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void send_cas_ai_pckt(unsigned int cas_indx,BYTE c){

        unsigned int s;
        BYTE cas_type;
        unsigned int ai;

        s=dtel[c].ai ^ prv_dtel[c].ai;
        ai=dtel[c].ai;

//DYNAMIC : #if (NO_CAS3 != 0) -->
#ifdef EXTERNAL_CAS3_INSTALLED
        cas_type=CAS3_T;
#else
//DYNAMIC : #if (NO_CAS1 != 0) -->
#ifdef CAS1_INSTALLED
        cas_type=CAS1_T;
#endif
#endif
        if ( s & CAS3_GROUP_MASK ){
                pmamftx_both_sys(((ai & CAS3_GROUP_MASK)+MAC_DGTL_TRNK_GRP0)|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
        }
        if ( s & IS ){
               if (ai & IS ){
                        pmamftx_both_sys(MAC_IN_SERVICE|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_IN_SERVICE|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & ICC ){
               if (ai & ICC ){
                        pmamftx_both_sys(MAC_INCOMING_CH|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_INCOMING_CH|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & OGC ){
               if (ai & OGC ){
                        pmamftx_both_sys(MAC_OGC|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_OGC|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & PTS ){
               if (ai & PTS ){
                        pmamftx_both_sys(MAC_PTS|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_PTS|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & EOS ){
               if (ai & EOS ){
                        pmamftx_both_sys(MAC_EOS|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_EOS|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & CAS_METERING ){
               if (ai & CAS_METERING ){
                        pmamftx_both_sys(MAC_METERING|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_METERING|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & OPR_RCL ){
               if (ai & OPR_RCL ){
                        pmamftx_both_sys(MAC_OPR_RCL|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_OPR_RCL|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & MAL_CALL ){
               if (ai & MAL_CALL ){
                        pmamftx_both_sys(MAC_MAL_CALL|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_MAL_CALL|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & IMM_ACK ){
               if (ai & IMM_ACK ){
                        pmamftx_both_sys(MAC_IMM_ACK|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_IMM_ACK|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & WTFR_ACK ){
               if (ai & WTFR_ACK ){
                        pmamftx_both_sys(MAC_WTFR_ACK|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_WTFR_ACK|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
        if ( s & INTER_SIGNAL_DGT_SEND ){
               if (ai & INTER_SIGNAL_DGT_SEND ){
                        pmamftx_both_sys(MAC_INTR_SIGNAL_DGT_SEND|MA_EM_CAS_P|FEATURE_ACTIVATION,cas_indx,cas_type,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_INTR_SIGNAL_DGT_SEND|MA_EM_CAS_P,cas_indx,cas_type,saved_n);
               }
        }
}
#endif

/* ====================================================================== */
//CBADDED start
/* ====================================================================== */
/* ========================================================================== */
#ifdef ONLINE_EDIT
#ifdef CB_INSTALLED
void send_cb_cstv_pckt(unsigned int ext_indx,BYTE c){

        unsigned int s;

#ifdef LIM
#if(NO_EXTENSION_C != 0)
        if (ext_indx >= (no_cb[0]+no_cb[1]))
                ext_indx -= (no_cb[0]+no_cb[1]);
        else
#endif
        if (ext_indx >= no_cb[0])
                ext_indx -= no_cb[0];
#endif

        s=xel[c].cstv ^ prv_xel[c].cstv;
//CB
        if ( s & CALL_BACK ){
                if (xel[c].cstv & CALL_BACK ){
                        pmamftx_both_sys(MAC_CB_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
                }
                else{
                        pmamftx_both_sys(MAC_CB_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
                }
        }
//CP
        if ( s & CAMP_ON ){
               if (xel[c].cstv & CAMP_ON ){
                        pmamftx_both_sys(MAC_CAMPON_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CAMPON_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//FM
        if ( s & FOLLOW_ME ){
               if (xel[c].cstv & FOLLOW_ME ){
                        pmamftx_both_sys(MAC_FM_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_FM_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//ST
//#if (NO_CO != 0)
        if ( s & STD ){
               if (xel[c].cstv & STD ){
                        pmamftx_both_sys(MAC_CO_1ST0|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CO_1ST0|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//#endif
//IN
//#if (NO_CO != 0)
        if ( s & INTERNATIONAL ){
               if (xel[c].cstv & INTERNATIONAL ){
                        pmamftx_both_sys(MAC_CO_2ST0|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CO_2ST0|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//#endif
//DD
        if ( s & DONT_DIST ){
               if (xel[c].cstv & DONT_DIST ){
                        pmamftx_both_sys(MAC_DONTD_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_DONTD_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//CR
//#if ((NO_CO != 0) || (defined BAD_PAYER))
        if ( s & RING_AGAIN_COT ){
               if (xel[c].cstv & RING_AGAIN_COT ){
                        pmamftx_both_sys(MAC_CO_RSRV|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CO_RSRV|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//#endif
//CF
#ifdef CONFERENCE_ACT
        if ( s & CONFERENCE_SERVICE ){
               if (xel[c].cstv & CONFERENCE_SERVICE ){
                        pmamftx_both_sys(MAC_CONF_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CONF_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
#endif
//T0
//#if ((defined BAD_PAYER) || (defined EM_INSTALLED) || (!defined NO_TIE_INIT))
        if ( s & TIE_0 ){
               if (xel[c].cstv & TIE_0 ){
                        pmamftx_both_sys(MAC_TIE_1ST0|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_TIE_1ST0|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//#endif
//PU
        if ( s & CALL_PICKUP_USE ){
               if (xel[c].cstv & CALL_PICKUP_USE ){
                        pmamftx_both_sys(MAC_CPU_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_CPU_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//TR
#ifdef DTMF
        if ( s & RING_AGAIN_TIET ){
               if (xel[c].cstv & RING_AGAIN_TIET ){
                        pmamftx_both_sys(MAC_TIE_RSRV|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_TIE_RSRV|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
#endif
//HL
        if ( s & HOT_LINE_ACCESS ){
               if (xel[c].cstv & HOT_LINE_ACCESS ){
                        pmamftx_both_sys(MAC_HOT_LINE_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_HOT_LINE_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//SA
//#if ((defined BAD_PAYER) || (defined SIREN_INSTALLED ))
        if ( s & SIREN_ACCESS ){
               if (xel[c].cstv & SIREN_ACCESS ){
                        pmamftx_both_sys(MAC_SIREN_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_SIREN_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//#endif
//AA
//#if ((defined AMPLIFIER_INSTALLED) || (defined HOME_METERING))
        if ( s & AMPLIFIER_ACCESS ){
               if (xel[c].cstv & AMPLIFIER_ACCESS ){
                        pmamftx_both_sys(MAC_AMPLIFIER_ACCESS|MA_X_P|FEATURE_ACTIVATION,ext_indx,CB_T,saved_n);
               }
               else{
                        pmamftx_both_sys(MAC_AMPLIFIER_ACCESS|MA_X_P,ext_indx,CB_T,saved_n);
               }
        }
//#endif
}
#endif
#endif
//CBADDED end
#endif // NEW_DEP
