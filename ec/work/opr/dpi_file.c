
        /****  TCI CAS3(or EM) default parameters initialization  ****/
#include "omtfp.h"

#ifdef EM_GROUP_PROGRAM
#include "/ec/work/sys/emmacro.h"
#endif

#if (NO_CO != 0)
extern int ctdd[];  /* CO trunk destination DID */
extern int ctai[];  /* CO trunk activation int */
/* activation int: b0-b2=tgn,b3=is,b4=dda,b5=ta */
// tgn changed to b12-b15
#endif

#ifdef EM_GROUP_PROGRAM
extern int emtsp[];  /* EM trunk signaling protocol */
extern int emtai[];  /* EM trunk activation int */
#endif

#ifdef EXTERNAL_CAS3_INSTALLED
extern int dtai[];
#endif

#ifdef CAS1_INSTALLED
extern int c1ai[];
#endif

extern int hli[];  /* hot line index */

#ifdef R1_SIGNALINGP
extern char office_code[];
#endif

extern int no_cas3[];

#if ((defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
extern int emsgnd[];
#endif

void init_dpi(void){

//DOOD start : some changes
        unsigned int i;
#if ((defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED))
        unsigned int j,ch;
#endif
#ifdef EXTERNAL_CAS3_INSTALLED
        unsigned int max_trnk;
#endif
//DOOD end

#if (NO_CO != 0)
        for(i=0;i < NO_CO;i++)
                ctdd[i]=0;

        /* CO trunks in group 0,in service,destination DID not active */
        /* & timing active */
        for(i=0;i < NO_CO;i++)
                ctai[i]=0x0028;
#endif


#ifdef EXTERNAL_CAS3_INSTALLED
        ch=0;
        max_trnk=no_cas3[0]/30;  // only external cas3 trunks.
        for(j=0;j<max_trnk;j++){
                for(i=0;i<15;i++,ch++)  /* ogc */
                        dtai[ch]=IS | OGC | CAS_METERING | 0x00 /*(group #)*/;
                for(i=0;i<15;i++,ch++)      /* icc */
#ifdef CAS3_ICT_GROUP
                        dtai[ch]=IS | ICC | CAS_METERING | CAS3_ICT_GROUP;
#else
                        dtai[ch]=IS | ICC | CAS_METERING;
#endif
        }
#if 0
        for(i=0;i < NO_CAS3_IC_CH;i++)      /* icc */
                dtai[i]=IS | ICC | CAS_METERING;
        for(i=NO_CAS3/2;i<(NO_CAS3/2) + NO_CAS3_OG_CH;i++)  /* ogc */
                dtai[i]=IS | OGC | CAS_METERING | 0x00 /*(group #)*/;
#endif // 0
#endif

#ifdef CAS1_INSTALLED
        ch=0;
        for(j=0;j<NO_CAS1_DIGITAL_TRUNK;j++){
                for(i=0;i<15;i++,ch++)      /* icc */
                        c1ai[ch]=IS | ICC | CAS_METERING | IMM_ACK;
                for(i=0;i<15;i++,ch++)  /* ogc */
                        c1ai[ch]=IS | OGC | CAS_METERING | WTFR_ACK | 0x00 /*(group #)*/;
        }
#if 0
        for(i=0;i < NO_CAS1_IC_CH;i++)      /* icc */
                c1ai[i]=IS | ICC | CAS_METERING | IMM_ACK;
        for(i=NO_CAS1/2;i<(NO_CAS1/2) + NO_CAS1_OG_CH;i++)  /* ogc */
                c1ai[i]=IS | OGC | CAS_METERING | WTFR_ACK | 0x00 /*(group #)*/;
#endif // 0
#endif

#ifdef EM_INSTALLED
#ifdef EM_PULSE_SIGNALING
#ifdef EM_GROUP_PROGRAM
        /* odd  trunks 1,3,.. are in group 0(OGT) */
        /* even trunks 0,2,.. are in group 1(ICT) */
        /* trunks 12,13 out of service*/
        for(i=0;i <NO_EM ;i++){
                if(i & 0x01){  /* odd trunks OGT */
                        emtai[i]=0x0008 | EM_OGT_GROUP;
                        emtsp[i]=EM_OGT | WAIT_FOR_ACK | ANSWER_RX_OG;
                }
                else{ /* even trunks ICT */
                        emtai[i]=0x0008 | EM_ICT_GROUP;
                        emtsp[i]=IMMEDIATE_ACK | EM_ICT | ANSWER_TX_IC;
                }
        }
        for(i=NO_IS_EM;i<NO_EM;i++)
                emtai[i] &= 0xfff7;
#endif
#endif
#endif

#ifdef EM_INSTALLED
#ifdef EM_CONTINUOUS_SIGNALING
#ifdef EM_GROUP_PROGRAM
/* all trunks group 0, in service, dest_transit, no lpbk_test */
        for(i=0;i <NO_EM ;i++){
                emtsp[i]=EM_OGT | EM_ICT; // | ANSWER_TX_IC;
                emtai[i]=0x0018;
        }

#if (defined EM_GROUP_DIALLED)
        for(i=0;i <NO_EM_GROUP ;i++)
                emsgnd[i]=0x5a;
#endif
#endif
#endif
#endif


#ifdef R1_SIGNALINGP
//      strcpy(office_code,"021");
        office_code[0]='0'+0xf;
        office_code[1]='0';
        office_code[2]='0';
        office_code[3]='0';
        office_code[4]='0';
        office_code[5]='0';
        office_code[6]=0;
#endif

        /* no destination hot line programmed for extensions */
        for(i=0;i < NO_EXTENSION;i++)
                hli[i]=0xff;
}
