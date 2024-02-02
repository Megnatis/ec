                          /***    oprtn.c ***/

#include "omtfp.h"
#include "opr_net.h"

#ifdef OPERATOR_PC
#include "wg.h"
#endif

extern DATTIM sdt;  /* system date time */

CPR cpq[CP_QUE_LEN];

FPCP rtn_idx[OPRSIDE_NO_STATES][OPRSIDE_NO_STIMS];
/* routine index table contains the pointer of call processing routines */

BYTE link_state[MAX_NO_OPR_CARD];
FPNH olrtn_idx[NO_LINK_STATES][NO_LINK_EVENTS];
/* operator link routine index table contains the pointer of */
/* network handler routines */


#if ((defined CAS3_HISTORICAL_REPORT) || (defined EM_HISTORICAL_REPORT) || (defined CAS1_HISTORICAL_REPORT))
#if (TON == HISTORICAL_OPR)
extern FILE *fhst;
extern unsigned int no_cas3_channel[];
extern unsigned int no_cas1_channel[];
#endif
#endif
#if ((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM))
extern int no_cas3[];
#endif
#if ((defined CAS1_INSTALLED) && (defined OPR_CAS1_PROGRAM))
extern int no_cas1[];
#endif


#ifdef OPERATOR_PC
#if (OS == DOS)
#include <process.h>
#include <dir.h>
#endif
#endif

#ifdef OPERATOR_SIDE_DEBUG_ACT
#define DEBUG_ACT
#endif


#ifdef OPERATOR_PC
extern OPR_CALL_REC ocr;
extern int base_addr[];         /* voice connection of opr is always handled thru card # 0 */

#ifndef NO_TIE_INIT
extern STAT_IN_OPR ttst[];
#endif


#if(NO_CO != 0)
extern STAT_IN_OPR ctst[];
#endif


#ifdef OPR_EM_CONNECTION
extern STAT_IN_OPR emtst[];
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
extern STAT_IN_OPR dtcst[];
#endif

extern INDEX_TYPE hld,hldl[],pl[];
extern STAT_IN_OPR xst[];
#ifdef OPR_OPR_TALK
extern STAT_IN_OPR oprt[];
#endif
extern BYTE ohs,trs,trf,trg,pswd_d2d1,pswd_d4d3;
extern unsigned short cpfq;
//extern CPR cpq[];
extern RE_EX_QUE_REC rxqr[];
extern IN_QUE_REC iqr[];
extern BYTE hkl[],lpg1,lsg1,ds[];
extern unsigned int stm;
//extern FPCP rtn_idx[][OPRSIDE_NO_STIMS];
//extern unsigned int parmb[];
extern unsigned short parmb[];
extern FILE *fer;
extern FILE *flp;

#ifdef DISPLAY_EM_STATUS
extern BYTE dts[];
extern BYTE dtschng[];
#endif

#ifdef DISPLAY_CAS3_STATUS
extern BYTE dcs[];
extern BYTE dcschng[];

#ifdef DISPLAY_CAS3_LINE_STATUS
extern BYTE dcsb[];
extern BYTE dcsbchng[];
#endif

#endif

#ifdef DISPLAY_CAS1_STATUS
extern BYTE d1s[];
extern BYTE d1schng[];

#ifdef DISPLAY_CAS1_LINE_STATUS
extern BYTE d1sb[];
extern BYTE d1sbchng[];
#endif

#endif

#endif

//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
extern int oprnight,oprday;
#endif
//end night upload


#ifndef OPERATOR_PC
extern xdata OPR_CALL_REC ocr;

#ifndef NO_TIE_INIT
extern xdata STAT_IN_OPR ttst[];
#endif


#if(NO_CO != 0)
extern xdata STAT_IN_OPR ctst[];
#endif


#ifdef OPR_EM_CONNECTION
extern xdata STAT_IN_OPR emtst[];
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
extern xdata STAT_IN_OPR dtcst[];
#endif

extern xdata INDEX_TYPE hld,hldl[];
extern xdata STAT_IN_OPR xst[];
#ifdef OPR_OPR_TALK
extern xdata STAT_IN_OPR oprt[];
#endif
extern xdata BYTE ohs,trs,trf,trg;
extern xdata BYTE cpfq;
extern xdata CPR cpq[];
extern xdata RE_EX_QUE_REC rxqr[];
extern xdata IN_QUE_REC iqr[];
extern xdata INDEX_TYPE pl[];
extern xdata BYTE pswd_d2d1,pswd_d4d3;
extern xdata BYTE lpg1,lsg1,ds[],stm;
extern xdata BYTE rtn_idx[][OPRSIDE_NO_STIMS];
extern xdata unsigned int hkl;

#ifdef PARAMETER_DOWN_UP_LOAD
extern xdata unsigned int parmb[];
#endif

#endif

extern unsigned int f;

#ifdef DAY_NIGHT_PARAMETERS_FILE
extern char *pfna[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE];
#endif

#ifdef PARAMETER_DOWN_UP_LOAD
extern FILE *pfp[];
extern int no_load_parm[MAX_NO_OPR_CARD];

#endif

#ifdef PRE_INSTALL
extern BYTE org;
#endif

#ifdef OPERATOR_FARSI
extern char lcd_1l_buf[],lcd_space_line[],lcd_3_space[];
#endif

#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
extern BYTE op_can_dial_on_co;
#endif


#ifdef DISPLAY_STATUS_ACT
extern BYTE dxsf;
#endif
#ifdef DISPLAY_EM_STATUS
extern BYTE dtsf;
#endif
#ifdef DISPLAY_CAS3_STATUS
extern BYTE dcsf;
#endif
#ifdef DISPLAY_CAS1_STATUS
extern BYTE d1sf;
#endif

#ifdef SYSTEM_MANUAL_TEST
extern BYTE mtf;
#endif

#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef OPR_UPLOAD_WITH_TIMER
extern unsigned int upload_count;
#if ((NO_OPRTR > 1) && (defined HOT_STANDBY))
extern unsigned short upload_reject_flag;
#endif
#endif
#endif

#ifdef HOT_STANDBY
extern BYTE hot_link;
#endif

#ifdef NEW_DISPLAY_STATUS
extern BYTE current_page;
extern int VSG;
#endif

#ifdef DISPLAY_CAS3_STATUS
extern BYTE current_cas3_page;
#endif
extern BYTE no_intrnl_dgt;
extern BYTE intrnl_1st_dgt;
extern BYTE intrnl_1st_dgt_2;
extern BYTE intrnl_1st_dgt_3;
extern BYTE intrnl_1st_dgt_4;
extern BYTE intrnl_1st_dgt_5;
#ifdef DISPLAY_BILLING
extern BYTE bill_display_flag;
extern BYTE bill_line,scroll_flag;
extern WINDOW *bill_w,*sync_bill_w;
#endif

#ifdef DISPLAY_ALARM
extern BYTE alarm_display_flag;
extern BYTE alarm_line,alarm_scroll_flag;
extern WINDOW *alarm_w,*sync_alarm_w;
#endif
extern BYTE term_flag;
extern BYTE fqi,fqx;
extern BYTE pqi,shown_pqi,pqx,shown_pqx,last_shown_pq;

void rtn0(){
}

void rtn1(INDEX_TYPE pt){
        int a,r;

#ifdef DEBUG_ACT
        str2lcd(" r1 ");
#endif
        if((pt.t != XST_T) && (pt.t != OPR_T)){ /* OPR_OPR_TALK */

#ifdef DEBUG_ACT
                str2lcd(" ERR:1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:1\n");
#endif
                return;
        }
#ifdef OPR_OPR_TALK
        if(pt.t == OPR_T){
                rtn1_1(pt);
                return;
        }
#endif
        a=pt.i;

#ifdef NO_OPR_INTRNL_QU
        if( (ocr.state != OPR_READY) && (ocr.state != OPR_IDLE) ){
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                if(a >= (NO_EXTENSION_A+NO_EXTENSION_B))
                        pcpmftx2(OPR_REJECT,a-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T);
                else
#endif
                if(a >= NO_EXTENSION_A)
                        pcpmftx1(OPR_REJECT,a-NO_EXTENSION_A,XST_T);
                else
#endif
                pcpmftx(OPR_REJECT,a,XST_T);
                return;
        }
#endif
        if( (r=toiq(a)) == ERR_CODE){
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                if(a >= (NO_EXTENSION_A+NO_EXTENSION_B))
                        pcpmftx2(OPR_REJECT,a-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T);
                else
#endif
                if(a >= NO_EXTENSION_A)
                        pcpmftx1(OPR_REJECT,a-NO_EXTENSION_A,XST_T);
                else
#endif
                pcpmftx(OPR_REJECT,a,XST_T);
                return;
        }
        xst[a].s=IN_INTR_QU;
        xst[a].i=r;
}

#ifdef OPR_OPR_TALK
void rtn1_1(INDEX_TYPE pt){
        int a,a1,r;

#ifdef DEBUG_ACT
        str2lcd(" r1_1 ");
#endif
        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:1_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:1_1\n");
#endif
                return;
        }

        a=pt.i;

#ifdef NO_OPR_INTRNL_QU
        if( (ocr.state != OPR_READY) && (ocr.state != OPR_IDLE) ){
                pcpmftx(OPR_REJECT,a,OPR_T);
                return;
        }
#endif
        a1=a | 0x4000;
        if( (r=toiq(a1)) == ERR_CODE){
                pcpmftx(OPR_REJECT,a,OPR_T);
                return;
        }
        oprt[a].i=r;
}

void rtn1_2(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r1_2 ");
#endif
        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:1_2 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:1_2\n");
#endif
                return;
        }

        a=pt.i;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(lcd_3_space);
                int2lcd(hex2bcd((OPERATOR_NUM * 10) + a));
                str2lcd(" :¸Û°ê¢ ê¢ìı");
        }
        else
#endif
                str2lcd("source opr:");

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                tolcdq(h2c(OPERATOR_NUM),TO_LCD_DD);
                tolcdq(h2c(a+1),TO_LCD_DD);



/*
        if( (ocr.s.i == a) && (ocr.s.t == OPR_T) ){
                ocr.fs &= 0xf0;
                ocr.fs |= ACT_FAC1;
        }
        else{
                ocr.fs &= 0x0f;
                ocr.fs |= ACT_FAC2;
        }
*/
        ocr.state=OPRSIDE_OPR_TALK;
}


void rtn2_1(INDEX_TYPE pt){
        int a;
        BYTE i;
#ifdef DEBUG_ACT
        str2lcd(" r2_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:2_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:2_1\n");
#endif
                return;
        }
        a=pt.i;
        i=oprt[a].i;
        if(hldl[i].t == OPR_T){
                hldl[i].t=NULL_T;
                oprt[a].s=IDLE;

#ifdef OPERATOR_PC
                led_reset(hkl[i]);  /* hold LED off */
#else
                led_reset(hkl);  /* hold LED off */
#endif
                return;
        }
        rfiq(oprt[a].i);
}
#endif

void rtn2(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r2 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:2 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:2\n");
#endif
                return;
        }
        a=pt.i;
        rfiq(xst[a].i);

        if(xst[a].i == shown_pqi){
          shown_pqi=0xff;
        }

//      xst[a].s=IDLE;
        rtp(pt);

#ifdef DISPLAY_STATUS_ACT
        ds[a]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif

}

void rtn5(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r5 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:5 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:5\n");
#endif
                return;
        }
        if(trf & QUE_ACT)
                trg= (trg + 1) & 0x0f;
}

void rtn6(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r6 ");
#endif

         if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                 str2lcd(" ERR:6 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:6\n");
#endif
                 return;
         }
         trf ^= TONE_RNG_ACT;
         if( (trf & TONE_RNG_ACT) == RESET){    /* disable tone ringer */

#ifdef OPERATOR_PC
                DIS_SPKR;
#ifdef HDLC_8993
                kt_outport8(base_addr[0] | DSP_CR,0x72);
#endif
#else
                XBYTE[DSP_CR]=0x62;
#endif
                trs=0xff;
                led_set(TONE_RNGR_LED);
         }
         else
                led_reset(TONE_RNGR_LED);
}

void rtn7(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r7 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:7 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:7\n");
#endif
                 return;
         }
        if( (++ohs) & ON_HOOK){
                opoh();
                return;
        }

#ifdef OPERATOR_PC

#ifdef HDLC_8993
#ifdef PRE_INSTALL
        kt_outport8(base_addr[0] | RGCR,org);
#else
        kt_outport8(base_addr[0] | RGCR,OPR_RCVR_GAIN);
#endif
        kt_outport8(base_addr[0] | DSP_CR,0x62);
        kt_outport8(base_addr[0] | TRNS_CR,HSMIC | HSSKR);
#endif
        DIS_SPKR;   /* disable tone ringer */
#else

#ifdef PRE_INSTALL
        XBYTE[RGCR]=org;
#else
        XBYTE[RGCR]=OPR_RCVR_GAIN;
#endif
        XBYTE[DSP_CR]=0x62;
        XBYTE[TRNS_CR]=HSMIC | HSSKR;
#endif
        trf &= ~OPR_HOOK_STAT;
        trs=0xff;

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                   ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.fs=0xff;
        ocr.mf=RESET;
}

void rtn8(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r8 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:8 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:8\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                                   †®ëï ");

        else
#endif
                str2lcd(" answer ");
        a=pt.i;
        if( (ocr.s.i == a) && (ocr.s.t == XST_T) ){
                ocr.fs &= 0xf0;
                ocr.fs |= ACT_FAC1;
#ifdef OPR_PRI_CONNECTION
                if(xst[a].userside_pri == SET){
                        ocr.mf |= PRI_EXT_IN_TALK;
                        save_og_pri_digits(a);
                }
#endif
        }
        else{
                ocr.fs &= 0x0f;
                ocr.fs |= ACT_FAC2;
        }
        ocr.state=OPRSIDE_OPR_TALK;

#ifdef DISPLAY_STATUS_ACT
        ds[a]=OFF_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif
}

void rtn10(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r10 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:10 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:10\n");
#endif
                return;
        }
        if( ( (ocr.fs & 0xf0)==INACT_FAC2) && (ocr.d.t==XST_T) ){
                rtpns(ocr.d);
                ocr.d.t=NULL_T;
        }
        else
                if( ((ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t == XST_T) ){
                        rtpns(ocr.s);
                        ocr.s.t=NULL_T;
                }
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");

                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
        else{
                ocr.state=OPRSIDE_OPR_TALK;

                led_reset(DEST_LED);

#ifdef OPERATOR_PC

                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" READY...(Having a Source!) ");
#else
                init_lcd();
                str2lcd("READY..(SOURCE)");
#endif
        }
}

void rtn11(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r11 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:11 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:11\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                if(ocr.s.t != NULL_T)
                        rtpns(ocr.s);
                if(ocr.d.t != NULL_T);
                        rtpns(ocr.d);
                opoh();
        }
}

void rtn12(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r12 ");
#endif

        if((pt.t != XST_T) && (pt.t != EM_T)){

#ifdef DEBUG_ACT
                str2lcd(" ERR:12 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:12\n");
#endif
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ˘¢™˜ Ò¯ìÏ");
        else
#endif
                str2lcd(" REJECT ");
        a=pt.i;
        if(pt.t==XST_T)
//              xst[a].s=IDLE;
                rtp(pt);
#ifdef OPR_EM_CONNECTION
        else if(pt.t==EM_T)
                emtst[a].s=IDLE;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        else if(pt.t==CAS3_T)
                dtcst[a].s=IDLE;
#endif
        if( (ocr.s.i==a) && (ocr.s.t==pt.t) )
                ocr.s.t=NULL_T;
        else
                ocr.d.t=NULL_T;
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
/*      else
                  ocr.state=OPRSIDE_OPR_TALK;
*/
        /* state should remain OPRSIDE_OPR_EXT_RINGING anyway unless REJECT
           came from the same extension who was ringing which I am
           afraid is not possible. Also it seems that is not possible
           that REJECT come from the other (if) extension i.e. here
           REJECT always comes from an E&M who is the other party
           and has gone OPEN or TIME_OUT after OPEN for ANSWER
        */
}

void rtn13(INDEX_TYPE pt){
        BYTE h;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r13 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:13 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:13\n");
#endif
                return;
        }
        h=(cpq[cpfq].stim & 0xff) - HOLD1;
        if(h >= NO_HOLD_KEY)    return;
        if(hldl[h].t != NULL_T)  return;
        if( ( (ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t==XST_T) ){
                a=ocr.s.i;
                ocr.s.t=NULL_T;
        }
        else{
                a=ocr.d.i;
                ocr.d.t=NULL_T;
        }
        pcpmftx(HOLD_XT,a,XST_T);
        hldl[h].i=a;
        hldl[h].t=XST_T;

#ifdef OPERATOR_PC
        led_set(hkl[h]);  /* hold LED on */
#else
        led_set(hkl);  /* hold LED on */
#endif
        xst[a].s=HOLD;
        xst[a].i=h;
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
        else
                ocr.state=OPRSIDE_OPR_TALK;
}

#ifdef OPR_OPR_TALK

void rtn8_1(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r8_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:8_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:8_1\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                                   †®ëï ");

        else
#endif
                str2lcd(" answer ");
        a=pt.i;
        if( (ocr.s.i == a) && (ocr.s.t == OPR_T) ){
                ocr.fs &= 0xf0;
                ocr.fs |= ACT_FAC1;
        }
        else{
                ocr.fs &= 0x0f;
                ocr.fs |= ACT_FAC2;
        }
        ocr.state=OPRSIDE_OPR_TALK;
/*
#ifdef DISPLAY_STATUS_ACT
        ds[a]=OFF_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif
*/
}

void rtn10_1(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r10_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:10_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:10_1\n");
#endif
                return;
        }
        if( ( (ocr.fs & 0xf0)==INACT_FAC2) && (ocr.d.t==OPR_T) ){
                rtpns(ocr.d);
                ocr.d.t=NULL_T;
        }
        else
                if( ((ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t == OPR_T) ){
                        rtpns(ocr.s);
                        ocr.s.t=NULL_T;
                }
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");

                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
        else{
                ocr.state=OPRSIDE_OPR_TALK;

                led_reset(DEST_LED);

#ifdef OPERATOR_PC

                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" READY...(Having a Source!) ");
#else
                init_lcd();
                str2lcd("READY..(SOURCE)");
#endif
        }
}

void rtn11_1(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r11_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:11_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:11_1\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                if(ocr.s.t != NULL_T)
                        rtpns(ocr.s);
                if(ocr.d.t != NULL_T);
                        rtpns(ocr.d);
                opoh();
        }
}

void rtn12_1(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r12_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:12_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:12_1\n");
#endif
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ˘¢™˜ Ò¯ìÏ");
        else
#endif
                str2lcd(" REJECT ");
        a=pt.i;
        if(pt.t==OPR_T)
                oprt[a].s=IDLE;

        if( (ocr.s.i==a) && (ocr.s.t==pt.t) )
                ocr.s.t=NULL_T;
        else
                ocr.d.t=NULL_T;
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
/*      else
                  ocr.state=OPRSIDE_OPR_TALK;
*/
        /* state should remain OPRSIDE_OPR_EXT_RINGING anyway unless REJECT
           came from the same extension who was ringing which I am
           afraid is not possible. Also it seems that is not possible
           that REJECT come from the other (if) extension i.e. here
           REJECT always comes from an E&M who is the other party
           and has gone OPEN or TIME_OUT after OPEN for ANSWER
        */
}

#endif

void rtn14(INDEX_TYPE pt){
        int a;

#ifdef OPERATOR_FARSI
        char *b;
#endif


#ifdef DEBUG_ACT
        str2lcd(" r14 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:14 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:14\n");
#endif
                return;
        }
        if( ( (ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t == XST_T) ){
                a=ocr.s.i;
                ocr.s=pt;
        }
        else
                if( ( (ocr.fs & 0xf0)==INACT_FAC2) && (ocr.d.t==XST_T) ){
                        a=ocr.d.i;
                        ocr.d=pt;
                }
                else
                        return;
//      xst[a].s=IDLE;
        rtp_oprside(a, XST_T);

#ifdef DISPLAY_STATUS_ACT
        ds[a]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif
        a=pt.i;
        xst[a].s=IN_OCR;

#ifdef DISPLAY_STATUS_ACT
        ds[a]=AT_RING;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                b=strchr(lcd_1l_buf,':');
                --b;
                str2lcd(b);
                str2lcd("     ");
                int2lcd(hex2bcd(find_subscriber_no(a)));
                str2lcd(" :¸Û°ê¢ ˘ì ñ®ê ˘¢™ ÒÏó˜ı ˘ıÛëÓı");
        }
        else
#endif
                str2lcd(" follow me ext.:");

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                int2lcd(hex2bcd(find_subscriber_no(a)));
        ocr.state=OPRSIDE_OPR_EXT_RINGING;
}

void rtn15(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r15 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:15 ");int2lcd(pt.i);
                tolcdq(' ',TO_LCD_DD);int2lcd(pt.t);
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:15 ,%d %x\n",pt.i,pt.t);
#endif
                return;
        }
        a=pt.i;
        if( (ocr.s.i==a) && (ocr.s.t==XST_T) )
                ocr.s.t=NULL_T;
        else
                if( (ocr.d.i==a) && (ocr.d.t==XST_T) )
                        ocr.d.t=NULL_T;
                else    return;
//      xst[a].s=IDLE;
        rtp(pt);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ˘¢™˜ Ò¯ìÏ");

        else
#endif
                str2lcd(" REJECT ");
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else
                ocr.state=OPRSIDE_OPR_TALK;
}

void rtn16(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r16 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:16 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:16\n");
#endif
                return;
        }
        if( ( (ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t==XST_T) ){
                a=ocr.s.i;
                ocr.s.t=NULL_T;
        }
        else
                if( ( (ocr.fs & 0xf0)==INACT_FAC2) && (ocr.d.t==XST_T) ){
                        a=ocr.d.i;
                        ocr.d.t=NULL_T;
                }
                else
                        return;
//      xst[a].s=IDLE;
        rtp_oprside(a, XST_T);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd(":¸Û°ê¢ ˘ì ¢™ ÒÏó˜ı ñ®ê ÒëÁ™ê ¸Û°ê¢");
        else
#endif
                str2lcd(" follow me busy:");
        if( (ocr.s.t==NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                     ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else
                ocr.state=OPRSIDE_OPR_TALK;
}

void rtn17(INDEX_TYPE pt){
        int a,r;
#ifdef OPR_OPR_TALK
        int a1,a2;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r17 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:17 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:17\n");
#endif
                return;
        }
        if( (r=fromiq()) == ERR_CODE)       return;
        a=iqr[r].i;

#ifdef OPR_OPR_TALK
        if(a & 0x4000){
                a1=a & 0xfff;
                a2=r;
                rtn17_1(a1,a2);
                return;
        }
#endif

#if(NO_EXTENSION_C != 0)
                if(a >= (NO_EXTENSION_A+NO_EXTENSION_B))
                        pcpmftx2(OPR_REJECT,a-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T);
                else
#endif
#if  (defined LIM)
#if(NO_EXTENSION_C != 0)
        if(a >= (NO_EXTENSION_A+NO_EXTENSION_B)){
                if(pcpmftx2(CNNCT_TO_OPR,a-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T) == ERR_CODE){
                        ptiq(r);
                        return;
                }
        }
        else
#endif
        if(a >= NO_EXTENSION_A){
                if(pcpmftx1(CNNCT_TO_OPR,a-NO_EXTENSION_A,XST_T) == ERR_CODE){
                        ptiq(r);
                        return;
                }
        }
        else
#endif
        if(pcpmftx(CNNCT_TO_OPR,a,XST_T) == ERR_CODE){
                ptiq(r);
                return;
        }
        riqr(r);
        led_set(SOURCE_LED);  /* source LED on */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
            str2lcd(lcd_3_space);
            if(iqr[r].no_dgt == 0xff)
                int2lcd(hex2bcd(find_subscriber_no(a)));
            else{
                int i;char buf[256];
                for(i=0;i<iqr[r].no_dgt;i++){
                    buf[i]=iqr[r].src_dgt[i]+'0';
                }
                buf[i]=0;
                str2lcd(buf);
            }
            str2lcd(" :¸Û°ê¢ ê¢ìı");
        }
        else
#endif
                str2lcd("source ext:");

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
        {
            if(iqr[r].no_dgt == 0xff)
                int2lcd(hex2bcd(find_subscriber_no(a)));
            else{
                int i;char buf[256];
                for(i=0;i<iqr[r].no_dgt;i++){
                    buf[i]=iqr[r].src_dgt[i]+'0';
                }
                buf[i]=0;
                str2lcd(buf);
            }
        }
        xst[a].s=IN_OCR;
        ocr.s.i=a;
        ocr.s.t=XST_T;
        ocr.fs=ACT_FAC1;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
#ifdef OPR_PRI_CONNECTION
        if(xst[a].userside_pri == SET){
                ocr.mf |= PRI_EXT_IN_TALK;
                save_ic_pri_digits(a);
        }
#endif
        ocr.state=OPRSIDE_OPR_TALK;
}

void rtn17a(INDEX_TYPE pt){
        int a,r;
#ifdef DEBUG_ACT
        str2lcd(" r17a ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:17a ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:17a\n");
#endif
                return;
        }
        tolcdq(0xff,CLEAR_LCD);
        if(pqi >= IQR_NO){
          str2lcd("Internal Queue Empty");
          return;
        }
        r=pqi;
        a=iqr[r].i;
        if(r == fqi)
          str2lcd("First In que ext:");
        else
          str2lcd("      In que ext:");
        if(iqr[r].no_dgt == 0xff)
            int2lcd(hex2bcd(find_subscriber_no(a)));
        else{
            int i;char buf[256];
            for(i=0;i<iqr[r].no_dgt;i++){
                buf[i]=iqr[r].src_dgt[i]+'0';
            }
            buf[i]=0;
            str2lcd(buf);
        }
        shown_pqi=r;
        last_shown_pq=1;
        pqi=iqr[r].rp;
        if(pqi >= IQR_NO)
          pqi=fqi;
}

void rtn17c(INDEX_TYPE pt){
#ifdef DEBUG_ACT
        str2lcd(" r17c ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:17c ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:17c\n");
#endif
                return;
        }
        if(last_shown_pq == 0)
            rtn20b(pt);
        else
            rtn17b(pt);
}


void rtn17b(INDEX_TYPE pt){
        int a,r;
#ifdef OPR_OPR_TALK
        int a1,a2;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r17b ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:17b ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:17b\n");
#endif
                return;
        }

        r=shown_pqi;
        if(r >= IQR_NO){
            rtn17(pt);
            return;
        }
        shown_pqi=0xff;

        a=iqr[r].i;

#ifdef OPR_OPR_TALK
        if(a & 0x4000){
                a1=a & 0xfff;
                a2=r;
                rtn17_1(a1,a2);    // this routine should now do rfiq() instead of riqr()
                return;
        }
#endif

#if(NO_EXTENSION_C != 0)
                if(a >= (NO_EXTENSION_A+NO_EXTENSION_B))
                        pcpmftx2(OPR_REJECT,a-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T);
                else
#endif
#if  (defined LIM)
#if(NO_EXTENSION_C != 0)
        if(a >= (NO_EXTENSION_A+NO_EXTENSION_B)){
                if(pcpmftx2(CNNCT_TO_OPR,a-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T) == ERR_CODE){
                        ptiq(r);
                        return;
                }
        }
        else
#endif
        if(a >= NO_EXTENSION_A){
                if(pcpmftx1(CNNCT_TO_OPR,a-NO_EXTENSION_A,XST_T) == ERR_CODE){
                        ptiq(r);
                        return;
                }
        }
        else
#endif
        if(pcpmftx(CNNCT_TO_OPR,a,XST_T) == ERR_CODE){
                ptiq(r);
                return;
        }

        //riqr(r);
        rfiq(r);

        led_set(SOURCE_LED);  /* source LED on */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
            str2lcd(lcd_3_space);
            if(iqr[r].no_dgt == 0xff)
                int2lcd(hex2bcd(find_subscriber_no(a)));
            else{
                int i;char buf[256];
                for(i=0;i<iqr[r].no_dgt;i++){
                    buf[i]=iqr[r].src_dgt[i]+'0';
                }
                buf[i]=0;
                str2lcd(buf);
            }
            str2lcd(" :¸Û°ê¢ ê¢ìı");
        }
        else
#endif
                str2lcd("source ext:");

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
        {
            if(iqr[r].no_dgt == 0xff)
                int2lcd(hex2bcd(find_subscriber_no(a)));
            else{
                int i;char buf[256];
                for(i=0;i<iqr[r].no_dgt;i++){
                    buf[i]=iqr[r].src_dgt[i]+'0';
                }
                buf[i]=0;
                str2lcd(buf);
            }
        }
        xst[a].s=IN_OCR;
        ocr.s.i=a;
        ocr.s.t=XST_T;
        ocr.fs=ACT_FAC1;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
#ifdef OPR_PRI_CONNECTION
        if(xst[a].userside_pri == SET){
                ocr.mf |= PRI_EXT_IN_TALK;
                save_ic_pri_digits(a);
        }
#endif
        ocr.state=OPRSIDE_OPR_TALK;
}

#ifdef OPR_OPR_TALK
void rtn17_1(int d1,int d2){

#ifdef DEBUG_ACT
        str2lcd(" r17_1 ");
#endif
/*
        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:17_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:17_1\n");
#endif
                return;
        }
*/
        if(d1 >= NO_OPRTR){
                pcpmftx(OPR_REJECT,d1,OPR_T);
                return;
        }

        if(pcpmftx(OPR_CNNCT_TO_OPR,d1,OPR_T) == ERR_CODE){
                ptiq(d2);
                return;
        }
        riqr(d2);
        led_set(SOURCE_LED);  /* source LED on */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(lcd_3_space);
                int2lcd(hex2bcd((OPERATOR_NUM * 10) + d1));
                str2lcd(" :¸Û°ê¢ ê¢ìı");
        }
        else
#endif
                str2lcd("source opr:");

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                //int2lcd(hex2bcd((OPERATOR_NUM * 10) + d1 + 1));
                tolcdq(h2c(OPERATOR_NUM),TO_LCD_DD);
                tolcdq(h2c(d1+1),TO_LCD_DD);
        oprt[d1].s=IN_OCR;
        ocr.s.i=d1;
        ocr.s.t=OPR_T;
        ocr.fs=ACT_FAC1;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.state=OPRSIDE_OPR_TALK;
}

void rtn17_2(INDEX_TYPE pt){
        int a;
//      int r;

#ifdef DEBUG_ACT
        str2lcd(" r17_2 ");
#endif
/*
        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:17_2 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:17_2\n");
#endif
                return;
        }
*/

        a=pt.i;
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        switch(pt.t){
                case XST_T:
#if(NO_EXTENSION_C != 0)
                        if(a >= (NO_EXTENSION_A+NO_EXTENSION_B))
                                pcpmftx2(OPR_REJECT,a-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T);
#endif
                        led_set(SOURCE_LED);  /* source LED on */

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd(lcd_3_space);
                                int2lcd(hex2bcd(find_subscriber_no(a)));
                                str2lcd(" :¸Û°ê¢ ê¢ìı");
                        }
                        else
#endif
                                str2lcd("source ext:");

#ifdef OPERATOR_FARSI
                        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                int2lcd(hex2bcd(find_subscriber_no(a)));
                        xst[a].s=IN_OCR;
                        break;
#ifndef NO_TIE_INIT
                case TIE_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(":TIE Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source TIE trunk:");
#else
                                str2lcd("source TIE:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                ttst[a].s=IN_OCR;

                                //pcpmftx(CNNCT_TO_OPR,r,TIE_T);
                                break;
#endif

#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
                case EM_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(":E&M Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source E&M trunk:");
#else
                                str2lcd("source E&M:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                emtst[a].s=IN_OCR;
//                              pcpmftx(CNNCT_TO_OPR_EM_CONT,r,EM_T);
                                break;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(":DT  Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source DT  trunk:");
#else
                                str2lcd("source DT :");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                dtcst[a].s=IN_OCR;
//                              pcpmftx(CNNCT_TO_OPR_CAS3,r,CAS3_T);
                                break;
#endif


#if(NO_CO != 0)
                case CO_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :¸§˙™ Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source CO trunk:");
#else
                                str2lcd("source  CO:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                {
#if (NO_CO >= 100)
                                        hex2lcd(hex2bcd(a + 1)>>8);
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                }
                                ctst[a].s=IN_OCR;
                                //pcpmftx(CNNCT_TO_OPR,r,CO_T);
                                break;
#endif
                default:        return;

        }
        ocr.s.i=a;
        ocr.s.t=pt.t;
        ocr.fs=ACT_FAC1;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
}

#endif

void rtn18(INDEX_TYPE pt){
        int i;

#ifdef DEBUG_ACT
        str2lcd(" r18 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:18 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:18\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        if( (ocr.fs & 0x0f) == HOLD_FAC1){
                i=(int)hex2bcd(ocr.s.i);
                switch(ocr.s.t){
                        case XST_T:
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        int2lcd(hex2bcd(find_subscriber_no(ocr.s.i)));
                                        str2lcd(" :¸Û°ê¢ ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source ext.:");
#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        int2lcd(hex2bcd(find_subscriber_no(ocr.s.i)));
                                break;
#if(NO_CO != 0)
                        case  CO_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(i + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :˝§˙™ Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source CO trunk:");
#else
                                str2lcd("source  CO:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                {
#if (NO_CO >= 100)
                                        hex2lcd(hex2bcd(i + 1)>>8);
#endif
                                        hex2lcd(i + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                }
                                break;
#endif


#ifndef NO_TIE_INIT
                        case TIE_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(i + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :TIE Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source TIE trunk:");
#else
                                str2lcd("source TIE:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(i + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                break;
#endif
                        default:        return;
                }
                led_set(SOURCE_LED);
                /* source LED on */

                pcpmftx(CNNCT_TO_OPR,ocr.s.i,ocr.s.t);
                ocr.state=OPRSIDE_OPR_TALK;
                hld.t=NULL_T;
        }
        else{

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        strcpy(lcd_1l_buf,"                  §¯óê§ïê Ø®¯ó ¸§˛˘§ëı™");
                        str2lcd(lcd_1l_buf);
                }
                 else
#endif
                        str2lcd(" opr. dial");
#else
                str2lcd("OPR. DIAL");
#endif
                ocr.state=OPR_START;
        }
}

void rtn19(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r19 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:19 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:19\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK)
                opoh();
}

void rtn20(INDEX_TYPE pt){
        int r,r1;
        INDEX_TYPE a;

#ifdef DEBUG_ACT
        str2lcd(" r20 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:20 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:20\n");
#endif
                return;
        }
        if( (r1=fromxq()) == ERR_CODE)       return;
        a=rxqr[r1].pt;
        rrxqr(r1);
        r=a.i;
        led_set(SOURCE_LED);  /* source LED on */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        switch(a.t){
#ifndef NO_TIE_INIT
            case TIE_T:
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_3_space);
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(":TIE Ø° ê¢ìı");
                }
                else
#endif
                        str2lcd("source TIE trunk:");
#else
                str2lcd("source TIE:");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                ttst[r].s=IN_OCR;

                pcpmftx(CNNCT_TO_OPR,r,TIE_T);
                break;
#endif
#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
            case EM_T:
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_3_space);
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(":E&M Ø° ê¢ìı");
                }
                else
#endif
                        str2lcd("source E&M trunk:");
#else
                str2lcd("source E&M:");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                emtst[r].s=IN_OCR;
                pcpmftx(CNNCT_TO_OPR_EM_CONT,r,EM_T);
                break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
            case CAS3_T:
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_3_space);
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(":DT  Ø° ê¢ìı");
                }
                else
#endif
                        str2lcd("source DT  trunk:");
#else
                str2lcd("source DT :");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                dtcst[r].s=IN_OCR;
                pcpmftx(CNNCT_TO_OPR_CAS3,r,CAS3_T);
                break;
#endif
#if(NO_CO != 0)
            case CO_T:
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                    str2lcd(lcd_3_space);
                    if(rxqr[r1].no_dgt == 0xff){
#if (NO_CO >= 100)
                        hex2lcd(hex2bcd(r + 1)>>8);
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                    }
                    else{
                        int i;char buf[256];
                        for(i=0;i<rxqr[r1].no_dgt;i++){
                            buf[i]=rxqr[r1].src_dgt[i]+'0';
                        }
                        buf[i]=0;
                        str2lcd(buf);
                    }
                    str2lcd(" :¸§˙™ Ø° ê¢ìı");
                }
                else
#endif
                    str2lcd("source CO trunk:");
#else
                str2lcd("source  CO:");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                {
                    if(rxqr[r1].no_dgt == 0xff){
#if (NO_CO >= 100)
                        hex2lcd(hex2bcd(r + 1)>>8);
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                    }
                    else{
                        int i;char buf[256];
                        for(i=0;i<rxqr[r1].no_dgt;i++){
                            buf[i]=rxqr[r1].src_dgt[i]+'0';
                        }
                        buf[i]=0;
                        str2lcd(buf);
                    }
                }
                ctst[r].s=IN_OCR;
                pcpmftx(CNNCT_TO_OPR,r,CO_T);
                break;
#endif
            default:        return;

        }
        ocr.s.i=r;
        ocr.s.t=a.t;
        ocr.fs=ACT_FAC1;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.state=OPRSIDE_OPR_TALK;
}

void rtn20a(INDEX_TYPE pt){
        int r;
        INDEX_TYPE a;
#ifdef DEBUG_ACT
        str2lcd(" r20a ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:20a ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:20a\n");
#endif
                return;
        }
        tolcdq(0xff,CLEAR_LCD);
        if(pqx >= RXQR_NO){
          str2lcd("External Queue Empty");
          return;
        }
        r=pqx;
        a=rxqr[r].pt;
#if(NO_CO != 0)
        if(a.t == CO_T){
          if(r == fqx)
            str2lcd("First In que CO:");
          else
            str2lcd("      In que CO:");


          if(rxqr[r].no_dgt == 0xff){
#if (NO_CO >= 100)
            hex2lcd(hex2bcd(a.i + 1)>>8);
#endif
            hex2lcd(hex2bcd(a.i + 1));
            /* for displaying first trunk as 1 not 0 */
          }
          else{
            int i;char buf[256];
            for(i=0;i<rxqr[r].no_dgt;i++){
                buf[i]=rxqr[r].src_dgt[i]+'0';
            }
            buf[i]=0;
            str2lcd(buf);
          }
          shown_pqx=r;
          last_shown_pq=0;
        }
#endif

        pqx=rxqr[r].rp;
        if(pqx >= RXQR_NO)
          pqx=fqx;
}

void rtn20b(INDEX_TYPE pt){
        int r,r1;
        INDEX_TYPE a;

#ifdef DEBUG_ACT
        str2lcd(" r20b ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:20b ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:20b\n");
#endif
                return;
        }

        r1=shown_pqx;
        if(r1 >= RXQR_NO){
            rtn20(pt);
            return;
        }
        shown_pqx=0xff;

        a=rxqr[r1].pt;

        //rrxqr(r1);
        rfxq(r1);

        r=a.i;
        led_set(SOURCE_LED);  /* source LED on */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        switch(a.t){
#ifndef NO_TIE_INIT
            case TIE_T:
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_3_space);
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(":TIE Ø° ê¢ìı");
                }
                else
#endif
                        str2lcd("source TIE trunk:");
#else
                str2lcd("source TIE:");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                ttst[r].s=IN_OCR;

                pcpmftx(CNNCT_TO_OPR,r,TIE_T);
                break;
#endif
#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
            case EM_T:
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_3_space);
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(":E&M Ø° ê¢ìı");
                }
                else
#endif
                        str2lcd("source E&M trunk:");
#else
                str2lcd("source E&M:");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                emtst[r].s=IN_OCR;
                pcpmftx(CNNCT_TO_OPR_EM_CONT,r,EM_T);
                break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
            case CAS3_T:
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_3_space);
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(":DT  Ø° ê¢ìı");
                }
                else
#endif
                        str2lcd("source DT  trunk:");
#else
                str2lcd("source DT :");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                dtcst[r].s=IN_OCR;
                pcpmftx(CNNCT_TO_OPR_CAS3,r,CAS3_T);
                break;
#endif
#if(NO_CO != 0)
            case CO_T:
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                    str2lcd(lcd_3_space);
                    if(rxqr[r1].no_dgt == 0xff){
#if (NO_CO >= 100)
                        hex2lcd(hex2bcd(r + 1)>>8);
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                    }
                    else{
                        int i;char buf[256];
                        for(i=0;i<rxqr[r1].no_dgt;i++){
                            buf[i]=rxqr[r1].src_dgt[i]+'0';
                        }
                        buf[i]=0;
                        str2lcd(buf);
                    }
                    str2lcd(" :¸§˙™ Ø° ê¢ìı");
                }
                else
#endif
                    str2lcd("source CO trunk:");
#else
                str2lcd("source  CO:");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                {
                    if(rxqr[r1].no_dgt == 0xff){
#if (NO_CO >= 100)
                        hex2lcd(hex2bcd(r + 1)>>8);
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                    }
                    else{
                        int i;char buf[256];
                        for(i=0;i<rxqr[r1].no_dgt;i++){
                            buf[i]=rxqr[r1].src_dgt[i]+'0';
                        }
                        buf[i]=0;
                        str2lcd(buf);
                    }
                }
                ctst[r].s=IN_OCR;
                pcpmftx(CNNCT_TO_OPR,r,CO_T);
                break;
#endif
            default:        return;

        }
        ocr.s.i=r;
        ocr.s.t=a.t;
        ocr.fs=ACT_FAC1;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.state=OPRSIDE_OPR_TALK;
}

void rtn21(INDEX_TYPE pt){
        int r,r1;
        INDEX_TYPE a;
#ifdef DEBUG_ACT
        str2lcd(" r21 ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:21 ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:21,%d,%d\n",pt.i,pt.t);
#endif
                return;
        }
        if( (r1=fromrq()) == ERR_CODE)       return;
        a=rxqr[r1].pt;
        if(pcpmftx(CNNCT_TO_OPR,a.i,a.t) == ERR_CODE){
                ptrq(r1);
                return;
        }
        rrxqr(r1);
        r=a.i;
        led_set(SOURCE_LED);  /* source LED on */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        if(a.t == CO_T){
#if(NO_CO != 0)
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                    str2lcd(lcd_3_space);
                    if(rxqr[r1].no_dgt == 0xff){
#if (NO_CO >= 100)
                        hex2lcd(hex2bcd(r + 1)>>8);
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                    }
                    else{
                        int i;char buf[256];
                        for(i=0;i<rxqr[r1].no_dgt;i++){
                            buf[i]=rxqr[r1].src_dgt[i]+'0';
                        }
                        buf[i]=0;
                        str2lcd(buf);
                    }
                    str2lcd(" :¸§˙™ Ø° ê¢ìı");
                }
                else
#endif
                    str2lcd("source CO trunk:");
#else
                str2lcd("source  CO:");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                {
                    if(rxqr[r1].no_dgt == 0xff){
#if (NO_CO >= 100)
                        hex2lcd(hex2bcd(r + 1)>>8);
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                    }
                    else{
                        int i;char buf[256];
                        for(i=0;i<rxqr[r1].no_dgt;i++){
                            buf[i]=rxqr[r1].src_dgt[i]+'0';
                        }
                        buf[i]=0;
                        str2lcd(buf);
                    }
                }
                ctst[r].s=IN_OCR;
#endif
        }
        else{
#ifndef NO_TIE_INIT
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_3_space);
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(" :TIE Ø° ê¢ìı");
                }
                else
#endif
                        str2lcd("source TIE trunk:");
#else
                str2lcd("source TIE:");
#endif
#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                        hex2lcd(hex2bcd(r + 1));
                        /* for displaying first trunk as 1 not 0 */
                ttst[r].s=IN_OCR;
#endif
        }
        ocr.s.i=r;
        ocr.s.t=a.t;
        ocr.fs=ACT_FAC1;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.state=OPRSIDE_OPR_TALK;
}

void rtn22(INDEX_TYPE pt){
        BYTE h,t;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r22 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:22 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:22\n");
#endif
                return;
        }
        h=(cpq[cpfq].stim & 0xff) - HOLD1;
        if(h >= NO_HOLD_KEY)    return;
        if(hldl[h].t == NULL_T)  return;
        a=hldl[h].i;
        t=hldl[h].t;
        pcpmftx(CNNCT_TO_OPR,a,t);
        ocr.s=hldl[h];
        ocr.d.t=NULL_T;
        ocr.fs=ACT_FAC1;
        ocr.state=OPRSIDE_OPR_TALK;
        hldl[h].t=NULL_T;

#ifdef OPERATOR_PC
        led_reset(hkl[h]);  /* hold LED off */
#else
        led_reset(hkl);  /* hold LED off */
#endif
        led_set(SOURCE_LED);
        led_reset(DEST_LED);

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        switch(t){
#ifdef OPR_OPR_TALK
                case OPR_T:
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        int2lcd(hex2bcd((OPERATOR_NUM * 10) + (a+1)));
                                        str2lcd(" :¸Û°ê¢ ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source opr:");

#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        tolcdq(h2c(OPERATOR_NUM),TO_LCD_DD);
                                        tolcdq(h2c(a+1),TO_LCD_DD);
                                oprt[a].s=IN_OCR;
                                break;
#endif
                case XST_T:

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
#ifdef OPR_PRI_CONNECTION
                                        if(xst[a].userside_pri == SET)
                                                show_ext_digits(a);
                                        else
#endif
                                                int2lcd(hex2bcd(find_subscriber_no(a)));
                                        str2lcd(" :¸Û°ê¢ ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source ext.:");
#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                {
#ifdef OPR_PRI_CONNECTION
                                        if(xst[a].userside_pri == SET){
                                                ocr.mf |= PRI_EXT_IN_TALK;
                                                show_ext_digits(a);
                                        }
                                        else
#endif
                                                int2lcd(hex2bcd(find_subscriber_no(a)));
                                }

                                xst[a].s=IN_OCR;
                                break;

#ifndef NO_TIE_INIT
                case TIE_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :TIE Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source TIE trunk:");
#else
                                str2lcd("source TIE:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                ttst[a].s=IN_OCR;
                                break;
#endif


#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
                case EM_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :E&M Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source E&M trunk:");
#else
                                str2lcd("source E&M:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                emtst[a].s=IN_OCR;
                                break;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :DT  Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source DT  trunk:");
#else
                                str2lcd("source DT :");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                dtcst[a].s=IN_OCR;
                                break;
#endif


#if(NO_CO != 0)
                case  CO_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :¸§˙™ Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source CO trunk:");
#else
                                str2lcd("source  CO:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                {
#if (NO_CO >= 100)
                                        hex2lcd(hex2bcd(a + 1)>>8);
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                }
                                ctst[a].s=IN_OCR;
                                break;
#endif
                        default:    return;
                }
}

void rtn23(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r23 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:23 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:23\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - CO_GRP1;
#if (NO_COT_GROUP != 0)
        if(n >= NO_COT_GROUP)
#endif
                return;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("              ");
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
                str2lcd(" :˘¯§ •ê ¸§˙™ Ø° ¸ëÆëÏó");
        }
        else
#endif
                str2lcd("CO trunk req. group:");
#else
        init_lcd();
        str2lcd(" CO GRP");
#endif


#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
        pcpmftx(CO_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        n=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,n & 0xbf);  /* disable operator's codec */
#endif
#else
        n=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=n & 0xbf;  /* disable operator's codec */
#endif
}

void rtn24(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r24 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:24 ");
#endif

#ifdef OPERATOR_PC
        fprintf(fer,"ERR:24\n");
#endif
                return;
        }
        a=pt.i;
//      xst[a].s=IDLE;
        rtp(pt);

#ifdef DISPLAY_STATUS_ACT
        ds[a]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif
        if( (ocr.s.i==a) && (ocr.s.t==XST_T) ){
                ocr.s.t=NULL_T;  /* source LED off */
                led_reset(SOURCE_LED);
        }
        else{
                ocr.d.t=NULL_T;
                led_reset(DEST_LED);  /* destination LED off */
        }
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.fs=0xff;
                ocr.mf=RESET;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
}

#ifdef OPR_OPR_TALK

void rtn24_1(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r24_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:24_1 ");
#endif

#ifdef OPERATOR_PC
        fprintf(fer,"ERR:24_1\n");
#endif
                return;
        }
        a=pt.i;

        if( (ocr.s.i==a) && (ocr.s.t==OPR_T) ){
                ocr.s.t=NULL_T;  /* source LED off */
                led_reset(SOURCE_LED);
        }
        else{
                ocr.d.t=NULL_T;
                led_reset(DEST_LED);  /* destination LED off */
        }
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.fs=0xff;
                ocr.mf=RESET;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
}

void rtn24_2(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r24_2 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:24_2 ");
#endif

#ifdef OPERATOR_PC
        fprintf(fer,"ERR:24_2\n");
#endif
                return;
        }

        ocr.state=OPR_AT_HOLD;
        ocr.scdc=0;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR AT HOLD...  ");
#else
        init_lcd();
        str2lcd("OPR. HOLD...");
#endif
}

void rtn25_1(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r25_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:25_1 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:25_1\n");
#endif
                return;
        }
        pcpmftx(RLS,TON,OPR_T);
        ocr.mf &= ~DIAL_ACTIVE;
        if( ((ocr.fs & 0x0f) == ACT_FAC1) && (ocr.s.t == OPR_T) ){
                rtp(ocr.s);
                ocr.s.t=NULL_T;
                ocr.fs &= 0xf0;
                led_reset(SOURCE_LED);
        }
        else{
                if( ((ocr.fs & 0xf0) == ACT_FAC2) && (ocr.d.t == OPR_T) ){
                        rtp(ocr.d);
                        ocr.d.t=NULL_T;
                        ocr.fs &= 0x0f;
                        led_reset(DEST_LED);
                }
        }

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
}

#endif

void rtn25(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r25 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:25 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:25\n");
#endif
                return;
        }
        pcpmftx(RLS,TON,OPR_T);
        ocr.mf &= ~DIAL_ACTIVE;
        if( (ocr.fs & 0x0f) == ACT_FAC1){
                rtp(ocr.s);
                ocr.s.t=NULL_T;
                led_reset(SOURCE_LED);
        }
        else{
                if( (ocr.fs & 0xf0) == ACT_FAC2){
                        rtp(ocr.d);
                        ocr.d.t=NULL_T;
                        led_reset(DEST_LED);
                }
        }
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
        else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
}

void rtn26(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r26 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:26 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:26\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                if(ocr.s.t & TRUNK_T)
                        rlstrnk(ocr.s);
                else
                        rtpns(ocr.s);
                ocr.s.t=NULL_T;

                if(ocr.d.t & TRUNK_T)
                        rlstrnk(ocr.d);
                else
                        rtpns(ocr.d);
                ocr.d.t=NULL_T;

                hld.t=NULL_T;
                opoh();
        }
}

void rtn27(INDEX_TYPE pt){
        INDEX_TYPE a;
        int i;

#ifdef DEBUG_ACT
        str2lcd(" r27 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:27 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:27\n");
#endif
                return;
        }
        if( (ocr.d.t != NULL_T) && ( (ocr.fs & 0xf0) == ACT_FAC2) )
        /* if destination already active */
                return;
#ifdef OPR_PRI_CONNECTION
        if((ocr.s.t == XST_T) && (xst[ocr.s.i].userside_pri == SET)) { // only pri subscribers come to this routine.
                if((ocr.mf & PRI_EXT_IN_TALK) == RESET)
                        return;
        }
#endif
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        if(ocr.s.t != NULL_T){
                a=ocr.s;
                pcpmftx(MUSIC,a.i,a.t);
                ocr.fs &= 0xf0;
                ocr.fs |= HOLD_FAC1;
                hld=a;
        }
        /* source LED off,destination LED on */
        led_reset(SOURCE_LED);
        led_set(DEST_LED);
        ocr.mf &= ~DIAL_ACTIVE;
        if( (ocr.fs & 0xf0) == HOLD_FAC2){  /* if destination at hold */
                a=ocr.d;
                pcpmftx(CNNCT_TO_OPR,a.i,a.t);
                ocr.fs &=0x0f;
                ocr.fs |=ACT_FAC2;
                ocr.state=OPRSIDE_OPR_TALK;
                i=(int)hex2bcd(a.i);
                switch(ocr.d.t){
                        case XST_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd("                       ");
                                                int2lcd(hex2bcd(find_subscriber_no(a.i)));
                                                str2lcd(" :¸Û°ê¢ ¢¨Ïı");
                                        }
                                        else
#endif
                                                str2lcd("destination ext.:");
#else
                                        str2lcd("dest. ext.:");
#endif


#ifdef OPERATOR_FARSI
                                        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                                int2lcd(hex2bcd(find_subscriber_no(a.i)));
                                        break;
#if(NO_CO != 0)
                        case  CO_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd("                       ");
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                                str2lcd(" :¸§˙™ Ø° ¢¨Ïı");
                                        }
                                        else
#endif
                                                str2lcd("destination CO trunk:");
#else
                                        str2lcd("dest.  CO:");
#endif


#ifdef OPERATOR_FARSI
                                        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        {
#if (NO_CO >= 100)
                                                hex2lcd(hex2bcd(i + 1)>>8);
#endif
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                        }
                                        break;
#endif


#ifndef NO_TIE_INIT
                        case TIE_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd("                       ");
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                                str2lcd(" :TIE Ø° ¢¨Ïı");
                                        }
                                        else
#endif
                                                str2lcd("destination TIE trunk:");
#else
                                        str2lcd("dest. TIE:");
#endif


#ifdef OPERATOR_FARSI
                                        if( (f & OPR_LANG_FLAG) == RESET)
#endif

                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                        break;
#endif


#ifdef OPR_EM_CONNECTION
                        case  EM_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd("                      ");
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                                str2lcd(" :E & M Ø° ¢¨Ïı");
                                        }
                                        else
#endif
                                                str2lcd("destination EM trunk:");
#else
                                        str2lcd("dest.  EM:");
#endif


#ifdef OPERATOR_FARSI
                                        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                        break;
#endif
                        default:    return;
                }
        }
        else{
                ocr.scdc=0;
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        strcpy(lcd_1l_buf,"                  §¯óê§ïê Ø®¯ó ¸§˛˘§ëı™");
                        str2lcd(lcd_1l_buf);
                }
                else
#endif
                        str2lcd(" opr. dial:");
#else
                str2lcd("OPR. DIAL");
#endif

#ifdef TRUNK_TRUNK_CONNECTION
                if(ocr.d.t != NULL_T)
                        rtpns(ocr.d);
#endif
                ocr.d.t=NULL_T;
                ocr.state=OPR_START;
        }
}

void rtn28(INDEX_TYPE pt){
        INDEX_TYPE a;
        int i;

#ifdef DEBUG_ACT
        str2lcd(" r28 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:28 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:28\n");
#endif
                return;
        }
        if( (ocr.fs & 0x0f) != HOLD_FAC1)    return;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        if(ocr.d.t != NULL_T){
                a=ocr.d;
                pcpmftx(MUSIC,a.i,a.t);
                ocr.fs &= 0x0f;
                ocr.fs |= HOLD_FAC2;
                hld=a;
        }
        /* source LED on,destination LED off */
        led_set(SOURCE_LED);
        led_reset(DEST_LED);
        ocr.mf &= ~DIAL_ACTIVE;
        a=ocr.s;
        pcpmftx(CNNCT_TO_OPR,a.i,a.t);
        ocr.fs &=0xf0;
        ocr.fs |=ACT_FAC1;
        i=(int)hex2bcd(a.i);
        switch(ocr.s.t){
                        case XST_T:
#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd(lcd_3_space);
#ifdef OPR_PRI_CONNECTION
                                                if(xst[a.i].userside_pri == SET)
                                                        show_ext_digits(a.i);
                                                else
#endif
                                                        int2lcd(hex2bcd(find_subscriber_no(a.i)));
                                                str2lcd(" :¸Û°ê¢ ê¢ìı");
                                        }
                                        else
#endif
                                                str2lcd("source ext.:");
#ifdef OPERATOR_FARSI
                                        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        {

#ifdef OPR_PRI_CONNECTION
                                                if(xst[a.i].userside_pri == SET)
                                                        show_ext_digits(a.i);
                                                else
#endif
                                                        int2lcd(hex2bcd(find_subscriber_no(a.i)));
                                        }

                                        break;
#if(NO_CO != 0)
                        case  CO_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd(lcd_3_space);
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                                str2lcd(" :˝§˙™ Ø° ê¢ìı");
                                        }
                                        else
#endif
                                                str2lcd("source CO trunk:");
#else
                                        str2lcd("source  CO:");
#endif

#ifdef OPERATOR_FARSI
                                        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        {
#if (NO_CO >= 100)
                                                hex2lcd(hex2bcd(i + 1)>>8);
#endif
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                        }
                                        break;
#endif


#ifndef NO_TIE_INIT
                        case TIE_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd(lcd_3_space);
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                                str2lcd(" :TIE Ø° ê¢ìı");
                                        }
                                        else
#endif
                                                str2lcd("source TIE trunk:");
#else
                                        str2lcd("source TIE:");
#endif


#ifdef OPERATOR_FARSI
                                        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                        break;
#endif


#ifdef OPR_EM_CONNECTION
                        case  EM_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd("  ");
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                                str2lcd(" :E & M Ø° ê¢ìı");
                                        }
                                        else
#endif
                                                str2lcd("source EM trunk:");
#else
                                        str2lcd("source  EM:");
#endif

#ifdef OPERATOR_FARSI
                                        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                                hex2lcd(i + 1);
                                                /* for displaying first trunk as 1 not 0 */
                                        break;
#endif
                        default:    return;
                }
}

void rtn29(INDEX_TYPE pt){

#ifdef TRUNK_TRUNK_CONNECTION
        int t;
#endif


#ifdef DEBUG_ACT
        str2lcd(" r29 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:29 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:29\n");
#endif
                return;
        }
        if(hld.t != NULL_T){
                pcpmftx(ATT_RLS,TON,OPR_T);

#ifdef TRUNK_TRUNK_CONNECTION
                if( (ocr.s.t & ocr.d.t & TRUNK_T) == TRUNK_T){
                        t=ocr.s.t;
                        if(t == CO_T)
#if(NO_CO != 0)
                                ctst[ocr.s.i].s=OPRSIDE_TRUNK_TRUNK_TALK
#endif
                                ;
                        else
                                if(t == TIE_T)
#ifndef NO_TIE_INIT
                                        ttst[ocr.s.i].s=OPRSIDE_TRUNK_TRUNK_TALK
#endif
                                        ;
                        t=ocr.d.t;
                        if(t == CO_T)
#if(NO_CO != 0)
                                ctst[ocr.d.i].s=OPRSIDE_TRUNK_TRUNK_TALK
#endif
                                ;
                        else
                                if(t == TIE_T)
#ifndef NO_TIE_INIT
                                        ttst[ocr.d.i].s=OPRSIDE_TRUNK_TRUNK_TALK
#endif
                                        ;
                }
                else{
                        rtp(ocr.s);
                        rtp(ocr.d);
                }
#else
                rtp(ocr.s);
                rtp(ocr.d);
#endif
        }
        else{
                rtpns(ocr.s);
                rtpns(ocr.d);
        }
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.fs=0xff;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);


#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}

void rtn30(INDEX_TYPE pt){
        int a;
        BYTE t;

#ifdef DEBUG_ACT
        str2lcd(" r30 ");
#endif

        if( (pt.t & XCT_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:30 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:30\n");
#endif
                return;
        }
        a=pt.i;
        t=pt.t;
        if( (ocr.s.t == t) && (ocr.s.i == a) )
                ocr.s.t=NULL_T;
        else
                if( (ocr.d.t == t) && (ocr.d.i == a) )
                        ocr.d.t=NULL_T;
                else
                        return;
        rtp(pt);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ˘¢™˜ Ò¯ìÏ");
        else
#endif
                str2lcd(" REJECT ");
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                     ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else
                ocr.mf &= ~DIAL_ACTIVE;
}

void rtn31(INDEX_TYPE pt){
        BYTE d,n;

#ifdef DEBUG_ACT
        str2lcd(" r31 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:31 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:31\n");
#endif
                return;
        }
        if( (ocr.mf & DIAL_ACTIVE) == RESET)    return;
        d=(cpq[cpfq].stim & 0xff) - N_1 +1;
        n=d;

#ifdef SPEED_DIALLING
        if((ocr.mf & SPEED_DIALLING_ACT) == 0)
#endif
        {
            pcpmftx(NUM,d,OPR_T);
        }
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                d <<= 4;
        switch(ocr.scdc){
                case  1:ocr.d2d1=d;

#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
#ifdef SPEED_DIALLING
                        if((ocr.mf & SPEED_DIALLING_ACT) == 0)
#endif
                        {
                            if ( (d==0xa) &&
                                 (op_can_dial_on_co == RESET) &&
                                 (
                                   (
                                     ((ocr.fs & 0x0f) == ACT_FAC1) && (ocr.s.t == CO_T)
                                   ) ||
                                   (
                                     ((ocr.fs & 0xf0) == ACT_FAC2) && (ocr.d.t == CO_T)
                                   )
                                 )
                               ){
                                    rtn25(pt);
                                    return;
                            }
                        }
#endif
                        break;
                case  2:ocr.d2d1 |=d;break;
                case  3:ocr.d4d3=d;break;
                case  4:ocr.d4d3 |=d;break;
                case  5:ocr.d6d5=d;break;
                case  6:ocr.d6d5 |=d;break;
                case  7:ocr.d8d7=d;break;
                case  8:ocr.d8d7 |=d;break;
                case  9:ocr.d10d9=d;break;
                case 10:ocr.d10d9 |=d;break;
                case 11:ocr.d12d11=d;break;
                case 12:ocr.d12d11 |=d;break;
                case 13:ocr.d14d13=d;break;
                case 14:ocr.d14d13 |=d;break;
                case 15:ocr.d16d15=d;break;
                case 16:ocr.d16d15 |=d;break;
                default: break;
        }
        if(n > 9)
                n=0;
#ifdef OLOOM_PEZESHKI
        if(ocr.mf & REDIAL_ACT){
                ocr.mf &= ~REDIAL_ACT;
#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
                str2lcd("dial on trunk:");
#else
                init_lcd();
                str2lcd("dial:");
#endif
        }
#endif
        tolcdq(h2c(n),TO_LCD_DD);
#ifdef KIATEL
        if (
               (
                 ((ocr.fs & 0x0f) == ACT_FAC1) && (ocr.s.t == CO_T)
               ) ||
               (
                 ((ocr.fs & 0xf0) == ACT_FAC2) && (ocr.d.t == CO_T)
               )
           ){
                fprintf(fer,"%d",n);
        }
#endif
}

void rtn32(INDEX_TYPE pt){
        BYTE h,t;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r32 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:32 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:32\n");
#endif
                return;
        }
        ocr.mf &= ~DIAL_ACTIVE;
        h=(cpq[cpfq].stim & 0xff) - HOLD1;
        if(h >= NO_HOLD_KEY)    return;
        if(hldl[h].t == NULL_T){   /* hold location free */
                if( ( (ocr.fs & 0x0f) == ACT_FAC1) && (ocr.s.t != NULL_T) ){
                        a=ocr.s.i;
                        t=ocr.s.t;
                        ocr.s.t=NULL_T;
                }
                else
                        if( ( (ocr.fs & 0xf0) == ACT_FAC2) && (ocr.d.t != NULL_T) ){
                                a=ocr.d.i;
                                t=ocr.d.t;
                                ocr.d.t=NULL_T;
                        }
                        else    return;
#ifdef OPR_PRI_CONNECTION
                // opr->pri ringing
                if ((t == XST_T) && (xst[a].userside_pri == SET) && (xst[a].og == SET)){
                        if(xst[a].dgt == NULL_POINTER){
                                save_og_pri_digits(a);
                        }
                }
#endif
                pcpmftx(HOLD_XT,a,t);
                hldl[h].i=a;
                hldl[h].t=t;

#ifdef OPERATOR_PC
                led_set(hkl[h]);  /* hold LED on */
#else
                led_set(hkl);  /* hold LED on */
#endif

                switch(t){
#ifdef OPR_OPR_TALK
                        case OPR_T:     oprt[a].s=HOLD;
                                        oprt[a].i=h;
                                        break;
#endif
                        case XST_T:     xst[a].s=HOLD;
                                        xst[a].i=h;
                                        break;
#ifndef NO_TIE_INIT
                        case TIE_T:     ttst[a].s=HOLD;
                                        ttst[a].i=h;
                                        break;
#endif


#if(NO_CO != 0)
                        case CO_T:      ctst[a].s=HOLD;
                                        ctst[a].i=h;
                                        break;
#endif


#ifdef OPR_EM_CONNECTION
                        case EM_T:      emtst[a].s=HOLD;
                                        emtst[a].i=h;
                                        break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                        case CAS3_T:    dtcst[a].s=HOLD;
                                        dtcst[a].i=h;
                                        break;
#endif
                        default:        break;

                }

                if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                        ocr.state=OPR_READY;
                        ocr.scdc=0;
                        ocr.mf=RESET;
                        ocr.fs=0xff;
                        hld.t=NULL_T;
                        led_reset(SOURCE_LED);
                        led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                        ddt_ad();
#else

#ifdef OPERATOR_PC
                        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("                     ...ñ®ê ˘¢ëıç §¯óê§ïê");
                        else
#endif
                                str2lcd(" OPERATOR READY...  ");
#else
                        init_lcd();
                        str2lcd("OPR. READY...");
#endif

#endif
                }
                else

#ifdef OPERATOR_PC
                        tolcdq(0xff,CLEAR_LCD);
#else
                        init_lcd();
#endif
                return;
        }
        /* hold location busy */
        if( (ocr.s.t != NULL_T) && (ocr.d.t != NULL_T) )   return;

#if ( (!(defined TRUNK_TRUNK_CONNECTION)) && (!(defined EM_TRUNK_CONNECTION)) )
        if( ( (ocr.s.t & hldl[h].t & TRUNK_T) == TRUNK_T) ||
            ( (ocr.d.t & hldl[h].t & TRUNK_T) == TRUNK_T) )
        /* if source or destination and hold party are trunk */
        /* for avoiding 2 analog trunk connection */
                return;
#endif

#if ( (defined EM_TRUNK_CONNECTION) && (!(defined TRUNK_TRUNK_CONNECTION)) )
        if(  (ocr.s.t!=EM_T) && (hldl[h].t!=EM_T) && (ocr.d.t!=EM_T))
        /* source or destination or hold party should be E&M */
        /* for again avoiding 2 analog trunk connection */
                return;
#endif


#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        if(ocr.d.t == NULL_T){
                pcpmftx(MUSIC,ocr.s.i,ocr.s.t);
                ocr.fs= HOLD | ACT_FAC2;
                hld=ocr.s;
                ocr.d=hldl[h];
                led_reset(SOURCE_LED);
                led_set(DEST_LED);

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("                       ");
                        str2lcd("¢¨Ïı");
                }
                else
#endif
                        str2lcd("destination ");
#else
                str2lcd("dest. ");
#endif
        }
        else
                if(ocr.s.t == NULL_T){
                        pcpmftx(MUSIC,ocr.d.i,ocr.d.t);
                        ocr.fs=HOLD_FAC2 | ACT_FAC1;
                        hld=ocr.d;
                        ocr.s=hldl[h];
                        led_set(SOURCE_LED);
                        led_reset(DEST_LED);
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("               ê¢ìı");
                                str2lcd("                     ");
                                str2lcd(lcd_space_line);
                                /* cursor to the beginnig of the 1st line */
                        }
                        else
#endif
                                str2lcd("source ");
                }
                else    return;
        t=hldl[h].t;
        a=hldl[h].i;
        hldl[h].t=NULL_T;

#ifdef OPERATOR_PC
        led_reset(hkl[h]);  /* hold LED off */
#else
        led_reset(hkl);  /* hold LED off */
#endif
        switch(t){
#ifdef OPR_OPR_TALK
                case OPR_T:
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        int2lcd(hex2bcd((OPERATOR_NUM * 10) + (a+1)));
                                        str2lcd(" :¸Û°ê¢ ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source opr:");

#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        tolcdq(h2c(OPERATOR_NUM),TO_LCD_DD);
                                        tolcdq(h2c(a+1),TO_LCD_DD);
                                oprt[a].s=IN_OCR;
                                break;
#endif
                case XST_T:     xst[a].s=IN_OCR;

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
#ifdef OPR_PRI_CONNECTION
                                        if(xst[a].userside_pri == SET)
                                                show_ext_digits(a);
                                        else
#endif
                                                int2lcd(hex2bcd(find_subscriber_no(a)));
                                        str2lcd(" :¸Û°ê¢ ");
                                }
                                else
#endif
                                        str2lcd("ext.:");
#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                {
#ifdef OPR_PRI_CONNECTION
                                        if(xst[a].userside_pri == SET)
                                                show_ext_digits(a);
                                        else
#endif
                                                int2lcd(hex2bcd(find_subscriber_no(a)));
                                }

                                break;

#if(NO_CO != 0)
                case  CO_T:     ctst[a].s=IN_OCR;

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :¸§˙™ Ø° ");
                                }
                                else
#endif
                                        str2lcd("CO trunk:");
#else
                                str2lcd(" CO:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                {
#if (NO_CO >= 100)
                                        hex2lcd(hex2bcd(a + 1)>>8);
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                }
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     ttst[a].s=IN_OCR;

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("    ");
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :TIE Ø° ");
                                }
                                else
#endif
                                        str2lcd("TIE trunk:");
#else
                                str2lcd("TIE:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                break;
#endif


#ifdef OPR_EM_CONNECTION
                case  EM_T:     emtst[a].s=IN_OCR;

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("    ");
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :E&M Ø° ");
                                }
                                else
#endif
                                        str2lcd("E&M trunk:");
#else
                                str2lcd("E&M:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                break;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case  CAS3_T:   dtcst[a].s=IN_OCR;

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("    ");
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :DT  Ø° ");
                                }
                                else
#endif
                                        str2lcd("DT  trunk:");
#else
                                str2lcd("DT :");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                break;
#endif
                default:        return;
        }
        pcpmftx(CNNCT_TO_OPR,a,t);
}

void rtn33(INDEX_TYPE pt){
        BYTE n;
        int a;
        BYTE t;

#ifdef DEBUG_ACT
        str2lcd(" r33 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:33 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:33\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - CO_GRP1;
#if (NO_COT_GROUP != 0)
        if(n >= NO_COT_GROUP)
#endif
                return;
        if( ((t=ocr.s.t) == XST_T) && (ocr.d.t == NULL_T) ){
                a=ocr.s.i;
                ocr.fs &= 0xf0;
                ocr.fs |= HOLD_FAC1;
        }
        else
                if( ((t=ocr.d.t) == XST_T) && (ocr.s.t == NULL_T) ){
                        a=ocr.d.i;
                        ocr.fs &= 0x0f;
                        ocr.fs |= HOLD_FAC2;
                }
                else
#ifdef LIM
        if( ((t=ocr.s.t) == CAS3_T) && (ocr.d.t == NULL_T) ){
                a=ocr.s.i;
                ocr.fs &= 0xf0;
                ocr.fs |= HOLD_FAC1;
        }
        else
                if( ((t=ocr.d.t) == CAS3_T) && (ocr.s.t == NULL_T) ){
                        a=ocr.d.i;
                        ocr.fs &= 0x0f;
                        ocr.fs |= HOLD_FAC2;
                }
                else
#endif
                        return;
        pcpmftx(MUSIC,a,t);
        hld.i=a;
        hld.t=t;
        pcpmftx(CO_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("              ");
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
                str2lcd(" :˘¯§ •ê ¸§˙™ Ø° ¸ëÆëÏó");
        }
        else
#endif
                str2lcd("CO trunk req. group:");
#else
        init_lcd();
        str2lcd(" CO GRP");
#endif


#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
}

void rtn34(INDEX_TYPE pt){
        BYTE n;

#ifdef OPERATOR_FARSI
        char *a;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r34 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:34 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:34\n");
#endif
                return;
        }

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                a=strchr(lcd_1l_buf,'§');
                /* a points to the last character of the previous message */
                a -= 7;
                /* a points to the start of the new appended message */
                memcpy(a,":¸Û°ê¢",6);
                tolcdq(0xff,CLEAR_LCD);
                str2lcd(lcd_1l_buf);
        }
        else
#endif
                str2lcd(" ext:");
#else
        str2lcd(" ext.: ");
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(lcd_space_line);
                str2lcd("      ");
        }
#endif
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        ocr.d2d1=n;
        ocr.state=OPR_DIAL_INTRNL;
        ocr.scdc=0x01;
        ocr.xd=0x04;
}
#ifdef X_RAILWAY
void rtn34a(INDEX_TYPE pt){
        BYTE n;

#ifdef OPERATOR_FARSI
        char *a;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r34a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:34a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:34a\n");
#endif
                return;
        }

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                a=strchr(lcd_1l_buf,'§');
                /* a points to the last character of the previous message */
                a -= 7;
                /* a points to the start of the new appended message */
                memcpy(a,": e&m ",6);
                tolcdq(0xff,CLEAR_LCD);
                str2lcd(lcd_1l_buf);
        }
        else
#endif
                str2lcd(" e&m:");
#else
        str2lcd(" e&m.: ");
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(lcd_space_line);
                str2lcd("      ");
        }
#endif
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        ocr.d2d1=n;
        ocr.state=OPR_DIAL_EM;
        ocr.scdc=0x01;
        ocr.xd=0x03;
}
#endif

#if ((NO_CO != 0) && (CO_REQ <= 0x0a))
void rtn34b(INDEX_TYPE pt){
        BYTE n;

#ifdef OPERATOR_FARSI
        char *a;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r34b ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:34b ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:34b\n");
#endif
                return;
        }

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                a=strchr(lcd_1l_buf,'§');
                /* a points to the last character of the previous message */
                a -= 7;
                /* a points to the start of the new appended message */
                memcpy(a,": DOD ",6);
                tolcdq(0xff,CLEAR_LCD);
                str2lcd(lcd_1l_buf);
        }
        else
#endif
                str2lcd(" DOD:");
#else
        str2lcd(" DOD.: ");
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(lcd_space_line);
                str2lcd("      ");
        }
#endif
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        ocr.d2d1=n;
        ocr.state=OPR_DIAL_CO;
        ocr.scdc=0x01;
        ocr.xd=NO_DGT_FOR_OPR_DIAL_CO;
}
#endif

#ifdef OPR_OPR_TALK
void rtn34_1(INDEX_TYPE pt){
        BYTE n;

#ifdef OPERATOR_FARSI
        char *a;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r34_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:34_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:34_1\n");
#endif
                return;
        }

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                a=strchr(lcd_1l_buf,'§');
                /* a points to the last character of the previous message */
                a -= 7;
                /* a points to the start of the new appended message */
                memcpy(a,":¸Û°ê¢",6);
                tolcdq(0xff,CLEAR_LCD);
                str2lcd(lcd_1l_buf);
        }
        else
#endif
                str2lcd(" opr:");
#else
        str2lcd(" opr.: ");
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(lcd_space_line);
                str2lcd("      ");
        }
#endif
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        ocr.d2d1=n;
        ocr.state=OPR_DIAL_OPR;
        ocr.scdc=0x01;
        ocr.xd=0x02;
}

#endif

void rtn35(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r35 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:35 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:35\n");
#endif
                return;
        }

        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(
/*
           (n == intrnl_1st_dgt)   ||
           (n == intrnl_1st_dgt_2) ||
           (n == intrnl_1st_dgt_3) ||
           (n == intrnl_1st_dgt_4) ||
           (n == intrnl_1st_dgt_5)
*/
           (n >= 1) && (n <= 10)
        ){
                rtn34(pt);
                return;
        }

        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                hld.t=NULL_T;

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
        else{

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
                ocr.state=OPRSIDE_OPR_TALK;
        }
}

void rtn36(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r36 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:36 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:36\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                if(ocr.s.t != NULL_T)
                        rtpns(ocr.s);
                if(ocr.d.t != NULL_T)
                        rtpns(ocr.d);
                opoh();
        }
}

void rtn37(INDEX_TYPE pt){
        BYTE a,t,i;

#ifdef DEBUG_ACT
        str2lcd(" r37 ");
#endif

        t=pt.t;
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:37 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:37\n");
#endif
                return;
        }
        a=pt.i;
#ifdef EM_CONT_SEP_OPR_Q
        if( t == EM_T )
                i=toemq(pt);
        else
                i=toxq(pt);
        if(i < RXQR_NO){
#else
        if( (i=toxq(pt)) < RXQR_NO){
#endif

                switch(t){

#ifndef NO_TIE_INIT
                        case TIE_T:     ttst[a].s=IN_EXTRNL_QU;
                                        ttst[a].i=i;
                                        return;
#endif


#if(NO_CO != 0)
                        case CO_T:      ctst[a].s=IN_EXTRNL_QU;
                                        ctst[a].i=i;
                                        return;
#endif


#if ( (defined EM_INSTALLED) && (defined OPR_EM_CONNECTION) )
                        case EM_T:      emtst[a].s=IN_EXTRNL_QU;
                                        emtst[a].i=i;
                                        return;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                        case CAS3_T:
                                        dtcst[a].s=IN_EXTRNL_QU;
                                        dtcst[a].i=i;
                                        return;
#endif
                        default:
#ifdef OPERATOR_PC
                                        fprintf(fer,"ERR:37.1\n");
#endif
                                        return;
                }
        }

#ifdef OPERATOR_PC
        fprintf(fer,"ERR:37.2\n");
#endif
}

void rtn38(INDEX_TYPE pt){
//      BYTE a,t;
        BYTE t;
        unsigned int a;

#ifdef DEBUG_ACT
        str2lcd(" r38 ");
#endif

        t=pt.t;
#if 0
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:38 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:38\n");
#endif
                return;
        }
#endif // 0
        if(ocr.state != AWAIT_TRNK)
        /* If operator not waiting for trunk, i.e. operator request trunk */
        /* but before TRNK_FREE come from system operator release it. */
        /* Coming TRNK_FREE at other states,displays "free < " message */
        /* which is not appropriate & also changes operator state which */
        /* may cause problems. */
                return;
        a=pt.i;
        switch(t){

#ifndef NO_TIE_INIT
                case TIE_T:     ttst[a].s=IN_OCR;
                                break;
#endif


#if(NO_CO != 0)
                case CO_T:      ctst[a].s=IN_OCR;
#ifdef KIATEL
                                sdt=*get_dt(&sdt);
                                fprintf(fer,"\n%4x-%02x-%02x %02x:%02x:%02x: ",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se);
                                fprintf(fer,"DoD No.%d: ",a+1);
#endif
                                break;
#endif


#ifdef OPR_EM_CONNECTION
                case EM_T:      emtst[a].s=IN_OCR;
                                break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:    dtcst[a].s=IN_OCR;
                                break;
#endif
#ifdef OPR_PRI_CONNECTION
                case XST_T:     xst[a].s=IN_OCR;
                                xst[a].userside_pri=SET;
                                xst[a].og=SET;
                                ocr.mf &= ~PRI_EXT_IN_TALK;
                                break;
#endif
                default:        return;
        }

        if(ocr.s.t == NULL_T){
                ocr.s=pt;
                ocr.fs &= 0xf0;
                ocr.fs |= ACT_FAC1;
                led_set(SOURCE_LED);  /* source LED on */
                led_reset(DEST_LED);  /* destination LED off */
        }
        else{
                ocr.d=pt;
                ocr.fs &= 0x0f;
                ocr.fs |= ACT_FAC2;
                led_reset(SOURCE_LED);  /* source LED off */
                led_set(DEST_LED);  /* destination LED on */
        }
#ifdef OPR_PRI_CONNECTION
        if(t != XST_T)  // it is in the middle of dialling
#endif
        {

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("¢ê•ç");
                else
#endif
                        str2lcd(" free ");
#else
                str2lcd(" free");
#endif

#if (NO_CO >= 100)
                hex2lcd(hex2bcd(a + 1)>>8);
#endif
                hex2lcd(hex2bcd(a + 1));
                /* for displaying trunk number from 1 instead of 0 */

#ifdef OPERATOR_PC
                str2lcd(" < ");
#else
                str2lcd("<");
#endif
        }

        ocr.state=OPRSIDE_OPR_TALK;
        ocr.mf |= DIAL_ACTIVE;

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        a=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,a | 0x40);  /* enable operator's codec */
#endif
#else
        a=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=a | 0x40;  /* enable operator's codec */

#endif
}

void rtn39(INDEX_TYPE pt){
        int a,t;

#ifdef DEBUG_ACT
        str2lcd(" r39 ");
#endif

        t=pt.t;
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:39 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:39\n");
#endif
                return;
        }
        a=pt.i;

        switch(t){

#ifndef NO_TIE_INIT
                case TIE_T:     rfxq(ttst[a].i);
                                ttst[a].s=IDLE;
                                break;
#endif


#if(NO_CO != 0)
                case CO_T:      rfxq(ctst[a].i);

                                if(ctst[a].i == shown_pqx){
                                  shown_pqx=0xff;
                                }

                                ctst[a].s=IDLE;
                                break;
#endif


#if ( (defined EM_INSTALLED) && (defined OPR_EM_CONNECTION) )
                case EM_T:
#ifdef EM_CONT_SEP_OPR_Q
                                rfemq(emtst[a].i);
#else
                                rfxq(emtst[a].i);
#endif
                                emtst[a].s=IDLE;
                                break;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:
//#ifdef EM_CONT_SEP_OPR_Q
//                                rfemq(emtst[a].i);
//#else
                                rfxq(dtcst[a].i);
//#endif
                                dtcst[a].s=IDLE;
                                break;
#endif

                default:        break;
        }
}

void rtn40(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r40 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:40 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:40\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                rtpns(hld);
                opoh();
        }
}

void rtn41(INDEX_TYPE pt){
        int n;

#ifdef DEBUG_ACT
        str2lcd(" r41 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:41 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:41\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)    /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5 =n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
#if 0
#ifdef AHAR  new changes make this block unnecessary
        if((ocr.d2d1 & 0x0f) == 9){
            if( (ocr.scdc & 0x0f) >= 3 ){
                goto cont1;
            }
        }
        else
#endif
#endif //0
        if( (n=fcp()) != ERR_CODE){
            /* valid number */
            if(xst[n].s != IDLE){
#ifdef OPERATOR_PC
              tolcdq(0xff,CLEAR_LCD);
#else
              init_lcd();
#endif
              str2lcd(" ext. busy ");
              if((ocr.s.t == NULL_T) && (ocr.d.t == NULL_T)){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
              }
              else{
                pcpmftx(TONE_TO_OPR,ICPTT,OPR_T);
                ocr.state=OPRSIDE_OPR_TALK;
              }
              return;
            }
            if(ocr.s.t == NULL_T){
#if  ((defined LIM) && (defined OPR_VOICE_THRU_LIMB))
#if(NO_EXTENSION_C != 0)
                if(n >= (NO_EXTENSION_A+NO_EXTENSION_B))
                    pcpmftx2(SRC_EXT_REQ,n-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T);
                else
#endif
                if(n >= NO_EXTENSION_A)
                    pcpmftx1(SRC_EXT_REQ,n-NO_EXTENSION_A,XST_T);
                else
#endif
                pcpmftx(SRC_EXT_REQ,n,XST_T);
                ocr.s.i=n;
                ocr.s.t=XST_T;
                ocr.fs &= 0xf0;
                ocr.fs |= INACT_FAC1;
            }
            else{
#if  ((defined LIM) && (defined OPR_VOICE_THRU_LIMB))
#if(NO_EXTENSION_C != 0)
                if(n >= (NO_EXTENSION_A+NO_EXTENSION_B))
                    pcpmftx2(DST_EXT_REQ,n-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T);
                else
#endif
                if(n >= NO_EXTENSION_A)
                    pcpmftx1(DST_EXT_REQ,n-NO_EXTENSION_A,XST_T);
                else
#endif
                pcpmftx(DST_EXT_REQ,n,XST_T);
                ocr.d.i=n;
                ocr.d.t=XST_T;
                ocr.fs &= 0x0f;
                ocr.fs |= INACT_FAC2;
            }
            ocr.state=AWAIT_EXT;
            xst[n].s=IN_OCR;
            return;
        }
        if( (ocr.scdc & 0x0f) >= 9 ){   /* some maximum */
            if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                hld.t=NULL_T;
#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else
#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                    str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                    str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif
#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
            }
            else{
#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                    str2lcd("                        ñ®ê ñ®§¢ë˜ ˘§ëı™");
                else
#endif
                    str2lcd(" invalid number ");
                pcpmftx(TONE_TO_OPR,ICPTT,OPR_T);
                ocr.state=OPRSIDE_OPR_TALK;
            }
            return;
        }
}

#ifdef X_RAILWAY
unsigned int femgroup(void){  /* find group number from numbers dialled */
        unsigned int a,d;
// for now assuming 2 digits dialled for group number
        a=(ocr.d2d1 & 0xf0) >> 4;
        if(a > 9)
                a=0;
        d=ocr.d4d3 & 0x0f;
        if(d > 9)
                d=0;
        a=(a * 10) + d;
        a--;
        if(a >= NO_EM_GROUP)
                return ERR_CODE;
        else
                return a;
}

void rtn41a(INDEX_TYPE pt){
        int n;

#ifdef DEBUG_ACT
        str2lcd(" r41a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:41a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:41a\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)    /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5 =n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) >= 3 ){   /* if last digit */
                if( (n=femgroup()) == ERR_CODE){
                        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                                hld.t=NULL_T;
#ifdef TIME_DATE_DISPLAY
                                ddt_ad();
#else

#ifdef OPERATOR_PC
                                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                                else
#endif
                                        str2lcd(" OPERATOR READY...  ");
#else
                                init_lcd();
                                str2lcd("OPR. READY...");
#endif

#endif
                                ocr.state=OPR_READY;
                                ocr.scdc=0;
                                ocr.mf=RESET;
                                ocr.fs=0xff;
                                led_reset(SOURCE_LED);
                                led_reset(DEST_LED);
                        }
                        else{
#ifdef OPERATOR_PC
                                tolcdq(0xff,CLEAR_LCD);
#else
                                init_lcd();
#endif


#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("                        ñ®ê ñ®§¢ë˜ ˘§ëı™");
                                else
#endif
                                        str2lcd(" invalid number ");
                                pcpmftx(TONE_TO_OPR,ICPTT,OPR_T);
                                ocr.state=OPRSIDE_OPR_TALK;
                        }
                        return;
                }
                /* valid group number */
                cpq[cpfq].stim=EM_GRP1+n;    // simulate appropriate stimulus
                if(ocr.s.t == NULL_T)
                        rtn132(pt);
                else
                        rtn133(pt);
        }
}
#endif

#if ((NO_CO != 0) && (CO_REQ <= 0x0a))
unsigned int fcogroup(void){  /* find group number from numbers dialled */
        unsigned int a;
// for now assuming 1 or 2 digits dialled for group number
        a=(ocr.d2d1 & 0xf0) >> 4;
        if(a > 9)
                a=0;
#if (NO_DGT_FOR_OPR_DIAL_CO == 3)
        {
        unsigned int d;
        d=ocr.d4d3 & 0x0f;
        if(d > 9)
                d=0;
        a=(a * 10) + d;
        }
#endif
        a--;
        if(a >= NO_COT_GROUP)
                return ERR_CODE;
        else
                return a;
}

void rtn41b(INDEX_TYPE pt){
        int n;

#ifdef DEBUG_ACT
        str2lcd(" r41b ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:41b ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:41b\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)    /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5 =n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) >= NO_DGT_FOR_OPR_DIAL_CO ){   /* if last digit */
                if( (n=fcogroup()) == ERR_CODE){
                        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                                hld.t=NULL_T;
#ifdef TIME_DATE_DISPLAY
                                ddt_ad();
#else

#ifdef OPERATOR_PC
                                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                                else
#endif
                                        str2lcd(" OPERATOR READY...  ");
#else
                                init_lcd();
                                str2lcd("OPR. READY...");
#endif

#endif
                                ocr.state=OPR_READY;
                                ocr.scdc=0;
                                ocr.mf=RESET;
                                ocr.fs=0xff;
                                led_reset(SOURCE_LED);
                                led_reset(DEST_LED);
                        }
                        else{
#ifdef OPERATOR_PC
                                tolcdq(0xff,CLEAR_LCD);
#else
                                init_lcd();
#endif


#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("                        ñ®ê ñ®§¢ë˜ ˘§ëı™");
                                else
#endif
                                        str2lcd(" invalid number ");
                                pcpmftx(TONE_TO_OPR,ICPTT,OPR_T);
                                ocr.state=OPRSIDE_OPR_TALK;
                        }
                        return;
                }
                /* valid group number */
                cpq[cpfq].stim=CO_GRP1+n;    // simulate appropriate stimulus
                if(ocr.s.t == NULL_T)
                        rtn23(pt);
                else
                        rtn33(pt);
        }
}
#endif

void rtn42(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r42 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:42 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:42\n");
#endif
                return;
        }

#ifdef KARAJ_RAILWAY

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("    ................          ");
        else
#endif
                str2lcd(" .......");

#else

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("    ˆ¢§¯° Ô˜• Òëü §¢          ");
        else
#endif
                str2lcd(" ringing");

#endif
        ocr.state=OPRSIDE_OPR_EXT_RINGING;

#ifdef DISPLAY_STATUS_ACT
        ds[ pt.i ]=AT_RING;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(pt.i);
#endif
#endif
}

void rtn43(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r43 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:43 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:43\n");
#endif
                return;
        }
        a=pt.i;
//      xst[a].s=IDLE;
        rtp(pt);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ˘¢™˜ Ò¯ìÏ               ");
        else
#endif
                str2lcd(" reject ");
        if( (ocr.s.i==a) && (ocr.s.t==XST_T) )
                ocr.s.t=NULL_T;
        else
                ocr.d.t=NULL_T;
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                     ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else
                ocr.state=OPRSIDE_OPR_TALK;
}

void rtn44(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r44 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:44 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:44\n");
#endif
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("   ñ®ê ÒëÁ™ê ¸Û°ê¢            ");
        else
#endif
                str2lcd(" ext. busy ");
        ocr.state=OPR_EXT_BUSY;
}

#ifdef LIM
void rtn44a(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r44a ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:44a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:44a\n");
#endif
                return;
        }
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("   ñ®ê ÒëÁ™ê ¸Û°ê¢            ");
        else
#endif
                str2lcd(" ext. busy ");
        ocr.state=OPR_EXT_BUSY;

#ifdef DISPLAY_STATUS_ACT
        ds[ pt.i ]=OFF_HOOK;    // assuming the destination ext. in other lim is busy, to override previously written "falsely-assumed" AT_RING in rtn42
                                // this may have the problem that if other lim has send congestion to us (hence compiled to ext_busy), may be this congestion
                                // is not due to ext. being busy, be due to other reasons, however what else could we do?!
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(pt.i);
#endif
#endif
}
#endif

void rtn45(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r45 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:45 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:45\n");
#endif
                return;
        }
        pcpmftx(TONE_TO_OPR,NO_TONE,OPR_T);
        if( (ocr.s.t==XST_T) && ( (ocr.fs & 0x0f)==INACT_FAC1) ){
                a=ocr.s.i;
                ocr.s.t=NULL_T;
        }
        else
                if( (ocr.d.t==XST_T) && ( (ocr.fs & 0xf0)==INACT_FAC2) ){
                        a=ocr.d.i;
                        ocr.d.t=NULL_T;
                }
                else    return;
//      xst[a].s=IDLE;
        rtp_oprside(a, XST_T);
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else{
                ocr.state=OPRSIDE_OPR_TALK;

                led_reset(DEST_LED);

#ifdef OPERATOR_PC

                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" READY...(Having a Source!) ");
#else
                init_lcd();
                str2lcd("READY..(SOURCE)");
#endif
        }
}

void rtn46(INDEX_TYPE pt){
        BYTE t;

#ifdef DEBUG_ACT
        str2lcd(" r46 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:46 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:46\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                pcpmftx(TONE_TO_OPR,NO_TONE,OPR_T);
                t=ocr.s.t;
                if(t & TRUNK_T)
                        rlstrnk(ocr.s);
                else
                        if(t == XST_T){
                                if( (ocr.fs & 0x0f) == INACT_FAC1)
//                                      xst[ocr.s.i].s=IDLE;
                                        rtp_oprside(ocr.s.i, XST_T);
                                else
                                        rtpns(ocr.s);
                        }
                t=ocr.d.t;
                if(t & TRUNK_T)
                        rlstrnk(ocr.d);
                else
                        if(t == XST_T){
                                if( (ocr.fs & 0xf0) == INACT_FAC2 )
//                                      xst[ocr.d.i].s=IDLE;
                                        rtp_oprside(ocr.d.i, XST_T);
                                else
                                        rtpns(ocr.d);
                        }
                opoh();
        }
}

void rtn47(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r47 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:47 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:47\n");
#endif
                return;
        }
        if( ( (ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t==XST_T) )
                a=ocr.s.i;
        else
                if( ( (ocr.fs & 0xf0)==INACT_FAC2) && (ocr.d.t==XST_T) )
                        a=ocr.d.i;
                else    return;
        pcpmftx(OPRSIDE_OVRD,a,XST_T);

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("                ");
                int2lcd(hex2bcd(find_subscriber_no(a)));
                str2lcd(" :¸Û°ê¢ Ø° ¸¯§ ˆóÍ§");
        }
        else
#endif
                str2lcd("ext. override:");

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                int2lcd(hex2bcd(find_subscriber_no(a)));
}

void rtn48(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r48 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:48 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:48\n");
#endif
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ˘¢™˜ Ò¯ìÏ ");
        else
#endif
                str2lcd(" REJECT ");
        if( ( (ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t==XST_T) ){
                a=ocr.s.i;
                ocr.s.t=NULL_T;
        }
        else
                if( ( (ocr.fs & 0xf0)==INACT_FAC2) && (ocr.d.t==XST_T) ){
                        a=ocr.d.i;
                        ocr.d.t=NULL_T;
                }
                else    return;
//      xst[a].s=IDLE;
        rtp_oprside(a, XST_T);
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else
                ocr.state=OPRSIDE_OPR_TALK;
}

void rtn49(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r49 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:49 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:49\n");
#endif
        }
        else
                ocr.state=OPR_EXT_OVRD;
}

void rtn50(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r50 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:50 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:50\n");
#endif
                return;
        }
        if( ( (ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t==XST_T) ){
                a=ocr.s.i;
                ocr.s.t=NULL_T;
        }
        else
                if( ( (ocr.fs & 0xf0)==INACT_FAC2) && (ocr.d.t==XST_T) ){
                        a=ocr.d.i;
                        ocr.d.t=NULL_T;
                }
                else    return;
        pcpmftx(RLS,a,XST_T);
//      xst[a].s=IDLE;
        rtp_oprside(a, XST_T);
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else{
                ocr.state=OPRSIDE_OPR_TALK;

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
        }
}

void rtn51(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r51 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:51 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:51\n");
#endif
                return;
        }
        if( ((++ohs) & ON_HOOK) ==RESET)        return;
        if( ( (ocr.fs & 0x0f)==INACT_FAC1) && (ocr.s.t==XST_T) ){
                a=ocr.s.i;
                rtpns(ocr.d);
        }
        else
                if( ( (ocr.fs & 0xf0)==INACT_FAC2) && (ocr.d.t==XST_T) ){
                        a=ocr.d.i;
                        rtpns(ocr.s);
                }
                else    return;
        pcpmftx(RLS,a,XST_T);
//      xst[a].s=IDLE;
        rtp_oprside(a, XST_T);
        opoh();
}

#ifdef OPR_OPR_TALK

void rtn41_1(INDEX_TYPE pt){
        int n,n1;

#ifdef DEBUG_ACT
        str2lcd(" r41_1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:41_1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:41_1\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)    /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5 =n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) >= NO_OPR_DGT ){   /* if last digit */
                n1=fd();

#if (OPERATOR_NUM < 10)
                n=n1 - (OPERATOR_NUM * 10) - 1;
#else
                n=n1 - 1;
#endif

                if( (n1 == ERR_CODE) || ((unsigned)n >= NO_OPRTR) ){

                        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                                hld.t=NULL_T;
#ifdef TIME_DATE_DISPLAY
                                ddt_ad();
#else

#ifdef OPERATOR_PC
                                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                                else
#endif
                                        str2lcd(" OPERATOR READY...  ");
#else
                                init_lcd();
                                str2lcd("OPR. READY...");
#endif

#endif
                                ocr.state=OPR_READY;
                                ocr.scdc=0;
                                ocr.mf=RESET;
                                ocr.fs=0xff;
                                led_reset(SOURCE_LED);
                                led_reset(DEST_LED);
                        }
                        else{
#ifdef OPERATOR_PC
                                tolcdq(0xff,CLEAR_LCD);
#else
                                init_lcd();
#endif


#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("                        ñ®ê ñ®§¢ë˜ ˘§ëı™");
                                else
#endif
                                        str2lcd(" invalid number ");
                                pcpmftx(TONE_TO_OPR,ICPTT,OPR_T);
                                ocr.state=OPRSIDE_OPR_TALK;
                        }
                        return;
                }
                /* valid number */
                if(ocr.s.t == NULL_T){
                        pcpmftx(SRC_OPR_REQ,n,OPR_T);
                        ocr.s.i=n;
                        ocr.s.t=OPR_T;
                        ocr.fs &= 0xf0;
                        ocr.fs |= INACT_FAC1;
                }
                else{
                        pcpmftx(DST_OPR_REQ,n,OPR_T);

                        ocr.d.i=n;
                        ocr.d.t=OPR_T;
                        ocr.fs &= 0x0f;
                        ocr.fs |= INACT_FAC2;
                }
                ocr.state=OPR_OPR_RINGING;
        }
}

#endif

void rtn52(INDEX_TYPE pt){
        int a;
        BYTE i;

#ifdef DEBUG_ACT
        str2lcd(" r52 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:52 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:52\n");
#endif
                return;
        }
        a=pt.i;
        i=xst[a].i;
//      xst[a].s=IDLE;
        rtp(pt);

#ifdef DISPLAY_STATUS_ACT
        ds[a]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif
        hldl[i].t=NULL_T;

#ifdef OPERATOR_PC
        led_reset(hkl[i]);  /* hold LED off */
#else
        led_reset(hkl);  /* hold LED off */
#endif
}

void rtn53(INDEX_TYPE pt){
        int a;
        BYTE t;
#if ((NO_CO != 0) || (!defined NO_TIE_INIT))
        BYTE i=0;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r53 ");
#endif

        t=pt.t;
        if( (t & TRUNK_T) ==RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:53 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:53\n");
#endif
                return;
        }
        a=pt.i;
        if(t == CO_T){
#if(NO_CO != 0)
                i=ctst[a].i;
                ctst[a].s=IN_RCL_QU;
                ctst[a].i=torq(pt);
#endif
        }
        else{

#ifndef NO_TIE_INIT
                i=ttst[a].i;
                ttst[a].s=IN_RCL_QU;
                ttst[a].i=torq(pt);
#endif
        }
#if ((NO_CO != 0) || (!defined NO_TIE_INIT))
        hldl[i].t=NULL_T;
#endif

#ifdef OPERATOR_PC
#if ((NO_CO != 0) || (!defined NO_TIE_INIT))
        led_reset(hkl[i]);  /* hold LED off */
#endif
#else
        led_reset(hkl);  /* hold LED off */
#endif
}

void rtn54(INDEX_TYPE pt){
        int a;
        BYTE i,t;

#ifdef DEBUG_ACT
        str2lcd(" r54 ");
#endif

        t=pt.t;
        if( (t & XCT_T) ==RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:54 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:54\n");
#endif
                return;
        }
        a=pt.i;
        switch(t){
                case XST_T:     i=xst[a].i;
//                              xst[a].s=IDLE;
                                rtp_oprside(a, XST_T);
                                break;
#if(NO_CO != 0)
                case  CO_T:     i=ctst[a].i;
                                ctst[a].i=torq(pt);
                                ctst[a].s=IN_RCL_QU;
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     i=ttst[a].i;
                                ttst[a].i=torq(pt);
                                ttst[a].s=IN_RCL_QU;
                                break;
#endif
                default:        return;
        }
        hldl[i].t=NULL_T;

#ifdef OPERATOR_PC
        led_reset(hkl[i]);  /* hold LED off */
#else
        led_reset(hkl);  /* hold LED off */
#endif
}

void rtn55(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r55 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:55 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:55\n");
#endif
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ˘¢™˜ Ò¯ìÏ ");
        else
#endif
                str2lcd(" REJECT ");
}

void rtn56(INDEX_TYPE pt){
#if ((!defined OPERATOR_PC) || (defined HDLC_8993))
        BYTE a;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r56 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:56 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:56\n");
#endif
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ÒëÁ™ê ");
        else
#endif
                str2lcd(" busy ");

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        a=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,a | 0x40);  /* enable operator's codec */
#endif
#else
        a=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=a | 0x40;  /* enable operator's codec */
#endif
}

void rtn57(INDEX_TYPE pt){
#if ((!defined OPERATOR_PC) || (defined HDLC_8993))
        BYTE a;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r57 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:57 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:57\n");
#endif
                return;
        }
        pcpmftx(RLS,TON,OPR_T);

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        a=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,a | 0x40); /* enable operator's codec */
#endif
#else
        a=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=a | 0x40;  /* enable operator's codec */
#endif
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else{
                ocr.state=OPRSIDE_OPR_TALK;

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
        }
}

void rtn58(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r58 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:58 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:58\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                pcpmftx(RLS,TON,OPR_T);
                if(ocr.s.t != NULL_T)
                        rtpns(ocr.s);
                if(ocr.d.t != NULL_T)
                        rtpns(ocr.d);
                opoh();
        }
}

void rtn59(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r59 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:59 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:59\n");
#endif
                return;
        }
        pcpmftx(RLS,TON,OPR_T);
        a=pt.i;
//      xst[a].s=IDLE;
        rtp(pt);

#ifdef DISPLAY_STATUS_ACT
        ds[a]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ˘ıÛëÓı ‚ØÏ ");
        else
#endif
                str2lcd(" on hook ");

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}

void rtn60(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r60 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:60 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:60\n");
#endif
                return;
        }
        ocr.state=PARK_OUT_DIAL;
        ocr.mf &= ~PARK_OUT_DIALLED;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                strcpy(lcd_1l_buf," :Ì§ëï •ê ö¯§° ˘§ëı™ §‡ó˜ı");
                str2lcd("            ");
                str2lcd(lcd_1l_buf);
                str2lcd(lcd_space_line);
                str2lcd("          ");
        }
        else
#endif
                str2lcd("await park out:");
}

void rtn61(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r61 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:61 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:61\n");
#endif
                return;
        }
        ocr.mf &= ~DIAL_ACTIVE;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        if( (ocr.s.t != NULL_T) && ( (ocr.fs & 0x0f) == ACT_FAC1) )
                pcpmftx(MUSIC,ocr.s.i,ocr.s.t);
        else
                if( (ocr.d.t != NULL_T) && ( (ocr.fs & 0xf0) == ACT_FAC2) )
                        pcpmftx(MUSIC,ocr.d.i,ocr.d.t);
                else    return;

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                strcpy(lcd_1l_buf," :Ì§ëï Òüı ˘§ëı™ §‡ó˜ı");
                str2lcd("                ");
                str2lcd(lcd_1l_buf);
                str2lcd(lcd_space_line);
                str2lcd("              ");
        }
        else
#endif
                str2lcd("await park in:");
        ocr.state=PARK_IN_DIAL;
        ocr.mf &= ~PARK_IN_DIALLED;
}

void rtn62(INDEX_TYPE pt){
        BYTE n,t;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r62 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:62 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:62\n");
#endif
                return;
        }
        if(ocr.mf & PARK_IN_DIALLED)
        /* if park # already dialled */
                return;
        n=(cpq[cpfq].stim & 0xff) - N_1;
        if(n > 9)
                hex2lcd(0);
        else
                hex2lcd(n+1);
        if(pl[n].t != NULL_T){

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_1l_buf);
                        str2lcd("                      ñ®ê ÒëÁ™ê Ì§ëï Òüı");
                }
                else
#endif
                        str2lcd(" park busy ");
                pcpmftx(TONE_TO_OPR,BUSYT,OPR_T);
                ocr.mf |= PARK_IN_DIALLED;
                return;
        }
        if( (ocr.s.t != NULL_T) && ( (ocr.fs & 0x0f) == ACT_FAC1) ){
                a=ocr.s.i;
                t=ocr.s.t;
                pl[n]=ocr.s;
                ocr.s.t=NULL_T;
                turn_on_park_led(n);
        }
        else
                if( (ocr.d.t != NULL_T) && ( (ocr.fs & 0xf0)==ACT_FAC2) ){
                        a=ocr.d.i;
                        t=ocr.d.t;
                        pl[n]=ocr.d;
                        ocr.d.t=NULL_T;
                        turn_on_park_led(n);
                }
                else    return;
        pcpmftx(PARK_XT,a,t);
        if(t == XST_T){
                xst[a].s=PARK;
                xst[a].i=n;
        }
        else
                if(t == CO_T){
#if(NO_CO != 0)
                        ctst[a].s=PARK;
                        ctst[a].i=n;
#endif
                }
                else
                        if(t == TIE_T){
#ifndef NO_TIE_INIT
                                ttst[a].s=PARK;
                                ttst[a].i=n;
#endif
                        }
                        else
                                if(t == EM_T){
#ifdef OPR_EM_CONNECTION
                                        emtst[a].s=PARK;
                                        emtst[a].i=n;
#endif
                                }
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                        else
                                if(t == CAS3_T){
                                        dtcst[a].s=PARK;
                                        dtcst[a].i=n;
                                }
#endif

        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else{

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
                ocr.state=OPRSIDE_OPR_TALK;
        }
}

void rtn63(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r63 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:63 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:63\n");
#endif
                return;
        }
        pcpmftx(TONE_TO_OPR,NO_TONE,OPR_T);

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                str2lcd("OPR. READY...");
#endif

#endif
                return;
        }
        if( (ocr.fs & 0x0f) == ACT_FAC1)
                pcpmftx(CNNCT_TO_OPR,ocr.s.i,ocr.s.t);
        else
                if( (ocr.fs & 0xf0) == ACT_FAC2)
                        pcpmftx(CNNCT_TO_OPR,ocr.d.i,ocr.d.t);
        ocr.state=OPRSIDE_OPR_TALK;
}

void rtn64(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r64 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:64 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:64\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                if(ocr.s.t != NULL_T)
                        rtpns(ocr.s);
                if(ocr.d.t != NULL_T)
                        rtpns(ocr.d);
                opoh();
        }
}

void rtn65(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r65 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:65 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:65\n");
#endif
                return;
        }
        a=pt.i;
//      xst[a].s=IDLE;
        rtp(pt);
        if( (ocr.s.i==a) && (ocr.s.t==XST_T) )
                ocr.s.t=NULL_T;
        else
                if( (ocr.d.i==a) && (ocr.d.t==XST_T) )
                        ocr.d.t=NULL_T;
                else    return;
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else
                ocr.state=OPRSIDE_OPR_TALK;
}

void rtn66(INDEX_TYPE pt){
        BYTE n,t;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r66 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:66 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:66\n");
#endif
                return;
        }
        if(ocr.mf & PARK_OUT_DIALLED)
        /* if park out # already dialled */
                return;
        n=(cpq[cpfq].stim & 0xff) - N_1;
        if(n >= 9)
                hex2lcd(0);
        else
                hex2lcd(n+1);
        if(pl[n].t == NULL_T){

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(lcd_1l_buf);
                        str2lcd("                       ñ®ê ¢ê•ç Ì§ëï Òüı");
                }
                else
#endif
                        str2lcd(" park free ");
                pcpmftx(TONE_TO_OPR,ICPTT,OPR_T);
                ocr.mf |= PARK_OUT_DIALLED;
                return;
        }
        a=pl[n].i;
        t=pl[n].t;
        ocr.s=pl[n];
        pl[n].t=NULL_T;
        turn_off_park_led(n);
        pcpmftx(CNNCT_TO_OPR,a,t);

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        switch(t){
                        case XST_T:
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        int2lcd(hex2bcd(find_subscriber_no(a)));
                                        str2lcd(" :¸Û°ê¢ ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source ext.:");
#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG)  == RESET)
#endif
                                        int2lcd(hex2bcd(find_subscriber_no(a)));
                                xst[a].s=IN_OCR;
                                break;

#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
                        case EM_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :E&M Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source E&M trunk:");
#else
                                str2lcd("source E&M:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                emtst[a].s=IN_OCR;
                                break;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                        case CAS3_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :DT  Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source DT  trunk:");
#else
                                str2lcd("source DT :");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                dtcst[a].s=IN_OCR;
                                break;
#endif

#if(NO_CO != 0)
                        case  CO_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :˝§˙™ Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source CO trunk:");
#else
                                str2lcd("source CO:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG)  == RESET)
#endif
                                {
#if (NO_CO >= 100)
                                        hex2lcd(hex2bcd(a + 1)>>8);
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                }
                                ctst[a].s=IN_OCR;
                                break;
#endif


#ifndef NO_TIE_INIT
                        case TIE_T:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(" :TIE Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source TIE trunk:");
#else
                                str2lcd("source TIE:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG)  == RESET)
#endif
                                        hex2lcd(hex2bcd(a + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                ttst[a].s=IN_OCR;
                                break;
#endif
                        default:        return;
                }
        led_set(SOURCE_LED);  /* source LED on */
        ocr.fs=ACT_FAC1;
        ocr.state=OPRSIDE_OPR_TALK;
}

void rtn67(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r67 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:67 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:67\n");
#endif
                return;
        }
        pcpmftx(RLS,TON,OPR_T);

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        ocr.fs=0xff;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}

void rtn68(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r68 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:68 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:68\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK)
                opoh();
}

void rtn69(INDEX_TYPE pt){
        int a,i;

#ifdef DEBUG_ACT
        str2lcd(" r69 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:69 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:69\n");
#endif
                return;
        }
        a=pt.i;
        i=xst[a].i;
//      xst[a].s=IDLE;
        rtp(pt);

#ifdef DISPLAY_STATUS_ACT
        ds[a]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif
        pl[i].t=NULL_T;
        turn_off_park_led(i);
}

void rtn70(INDEX_TYPE pt){
        int a;
        BYTE t;
#if ((NO_CO != 0) || (!defined NO_TIE_INIT))
        BYTE i=0;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r70 ");
#endif

        t=pt.t;
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:70 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:70\n");
#endif
                return;
        }
        a=pt.i;
        if(t == CO_T){
#if(NO_CO != 0)
                i=ctst[a].i;
                ctst[a].s=IN_RCL_QU;
                ctst[a].i=torq(pt);
#endif
        }
        else{
#ifndef NO_TIE_INIT
                i=ttst[a].i;
                ttst[a].s=IN_RCL_QU;
                ttst[a].i=torq(pt);
#endif
        }
#if ((NO_CO != 0) || (!defined NO_TIE_INIT))
        pl[i].t=NULL_T;
        turn_off_park_led(i);
#endif
        /* park LED adjusted */
}

void rtn71(INDEX_TYPE pt){
        int a;
        BYTE i,t;

#ifdef DEBUG_ACT
        str2lcd(" r71 ");
#endif

        t=pt.t;
        if( (t & XCT_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:71 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:71\n");
#endif
                return;
        }
        a=pt.i;
        switch(t){
                case XST_T:     i=xst[a].i;
//                              xst[a].s=IDLE;
                                rtp_oprside(a, XST_T);
                                break;
#if(NO_CO != 0)
                case  CO_T:     i=ctst[a].i;
                                ctst[a].i=torq(pt);
                                ctst[a].s=IN_RCL_QU;
                                break;
#endif

#ifndef NO_TIE_INIT
                case TIE_T:     i=ttst[a].i;
                                ttst[a].i=torq(pt);
                                ttst[a].s=IN_RCL_QU;
                                break;
#endif
                default:        return;
        }
        pl[i].t=NULL_T;
        turn_off_park_led(i);
        /* park LED adjusted */
}

void rtn72(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r72 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:72 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:72\n");
#endif
                return;
        }
        if( ((ocr.s.t & TRUNK_T) && (ocr.fs & 0x0f)==ACT_FAC1)  ||
            ((ocr.d.t & TRUNK_T) && (ocr.fs & 0xf0)==ACT_FAC2)    ){
#ifdef SPEED_DIALLING
            if(ocr.mf & SPEED_DIALLING_ACT){
              tolcdq('#',TO_LCD_DD);
              if(ocr.scdc != 0){
/*
#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
*/
                tolcdq('-',TO_LCD_DD);
                tolcdq('>',TO_LCD_DD);

                guchar code1[MAX_NO_SP_DIGIT1];
                guint i,j;
                guchar *c1=&ocr.d2d1;
                for(i=0,j=0;i<ocr.scdc;i+=2,c1++){
                  code1[j++]=(*c1)&0x0f;
                  code1[j++]=((*c1)&0xf0) >> 4;
                }
                guchar no_dgt=ocr.scdc;
                for(i=0;i<no_dgt;i++){
                  if(code1[i] == 0xa) code1[i]=0;
                }
                guchar *number_ptr;
                find_sp_from_tree(code1,no_dgt,&number_ptr);
                if(number_ptr != NULL_POINTER){
                  no_dgt=number_ptr[0];
                  if(no_dgt > 20) no_dgt=20;
                  for(i=0,c1=&ocr.d2d1;i<no_dgt;i+=2,c1++){
                    *c1=(number_ptr[i+2] << 4) | number_ptr[i+1];
                  }
                  ocr.scdc=no_dgt;
                  for(i=0;i<no_dgt;i++){
                    guchar digit=number_ptr[i+1];
                    pcpmftx(NUM,digit,OPR_T);
                    if(digit > 9) digit=0;
                    tolcdq(h2c(digit),TO_LCD_DD);
                  }
                }
                ocr.mf &= ~SPEED_DIALLING_ACT;
              }
              return;
            }
#endif
            pcpmftx(HASH,TON,OPR_T);
#ifndef OLOOM_PEZESHKI
            ocr.mf &= ~DIAL_ACTIVE;
#endif
            ocr.mf |= REDIAL_ACT;
        }
}

void rtn73(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r73 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:73 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:73\n");
#endif
                return;
        }
        if(ocr.mf & REDIAL_ACT){
                pcpmftx(STAR,TON,OPR_T);
                ocr.mf &= ~REDIAL_ACT;
        }
#ifdef SPEED_DIALLING
        else{
            tolcdq('*',TO_LCD_DD);
            if(ocr.scdc == 0){
                ocr.mf |= SPEED_DIALLING_ACT;
            }
        }
#endif
}

void rtn74(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r74 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:74 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:74\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("               :¢˛éëı˜ ¢§ê¯ ê§ §¯ì‰ ˘ıÛÓ");
        else
#endif
                str2lcd(" ENTER PASSWORD:");
#else
        init_lcd();
        str2lcd("ENTER PASSWORD:");
#endif

        ocr.mf |=PROG_ACT;
        ocr.scdc=0;
}

void rtn75(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r75 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:75 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:75\n");
#endif
                return;
        }
        if( (ocr.mf & PROG_ACT) == RESET)       return;
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if( (++ocr.scdc & 0x01) == RESET)    /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1:ocr.d2d1=n;break;
                case  2:ocr.d2d1 |=n;break;
                case  3:ocr.d4d3=n;break;
                case  4:ocr.d4d3 |=n;break;
                case  5:ocr.d6d5=n; break;
                case  6:ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < NO_PASSWORD_DGT)        return;

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif

        if( (ocr.d2d1==pswd_d2d1) && (ocr.d4d3==pswd_d4d3) ){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("                       ¢™ Ò¯ìÏ §¯ì‰ ˘ıÛÓ");
                        str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                }
                else{
#endif
                        str2lcd(" PASSWORD OK ");
                        str2lcd(" START PROGRAMMING:");
#ifdef OPERATOR_FARSI
                }
#endif

#else
                str2lcd("PASSWORD OK     ");
                str2lcd("START PROG:");
#endif

                ocr.state=OPR_PROG;
                ocr.mf=PROG_ACT;
                ocr.d.t=NULL_T;
                lpg1 |= PLS_BIT_POS;
        }
        else{

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                ñ®˛˜ Ò¯ìÏ ÒìëÏ §¯ì‰ ˘ıÛÓ");

                else
#endif
                        str2lcd(" BAD PASSWORD ");

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else


#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
}

void rtn76(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r76 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:76 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:76\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
        else
#endif
                str2lcd(" START NEW PROG.:");
#else
        init_lcd();
        str2lcd("START NEW PROG.:");
#endif

        ocr.d.t=NULL_T;
                  ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}

void rtn77(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r77 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:77 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:77\n");
#endif
                return;
                  }
        if( (++ohs) & ON_HOOK){
                                         opoh();
                lpg1 &= ~PLS_BIT_POS;
                                         lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                  }
}

void rtn78(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r78 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:78 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:78\n");
#endif
                return;
        }

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.s.t=ocr.d.t=NULL_T;
        ocr.mf=RESET;
        lpg1 &= ~PLS_BIT_POS;
        lsg1 &= ~PLS_BIT_POS;
        led_reset(PROG_LED);
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}

void rtn79(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r79 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:79 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:79\n");
#endif
                return;
        }
        if(ocr.mf & PROG_ACT){
                ocr.mf |= XST_PROG;

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif


#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("         :¸Û°ê¢ ˘§ëı™ - ¸Û°ê¢ ¸•˛§˘ıë˜§ì");
                        str2lcd(lcd_space_line);
                        str2lcd(lcd_3_space);
                }
                else
#endif
                        str2lcd(" EXT. PROG,ENTER EXT. #:");
                ocr.d.t=XST_T;
                ocr.scdc=0;
                ocr.xd=no_intrnl_dgt;
                ocr.state=AWAIT_PROG_DEST;
        }
}

#if (((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM)) || ((defined EM_INSTALLED) && (defined OPR_EM_PROGRAM)) || ((defined CAS1_INSTALLED) && (defined OPR_CAS1_PROGRAM)))
void rtn79a(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r79a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:79a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:79a\n");
#endif
                return;
        }
        if(ocr.mf & PROG_ACT){
                ocr.mf |= EMCAS_PROG;

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif


#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("         :Ì˜ê§ó ˘§ëı™ - e&m/cas ¸•˛§˘ıë˜§ì");
                        str2lcd(lcd_space_line);
                        str2lcd(lcd_3_space);
                }
                else
#endif
                        str2lcd(" EM/CAS. PROG,ENTER trnk. #:");
#if ((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM))
                ocr.d.t=CAS3_T;
#endif
#if ((defined EM_INSTALLED) && (defined OPR_EM_PROGRAM))
                ocr.d.t=EM_T;
#endif
#if ((defined CAS1_INSTALLED) && (defined OPR_CAS1_PROGRAM))
                ocr.d.t=CAS1_T;
#endif
                ocr.scdc=0;
                ocr.xd=NO_EMCAS_ID_DGT;
                ocr.state=AWAIT_PROG_DEST;
        }
}
#endif
void rtn80(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r80 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:80 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:80\n");
#endif
                return;
        }
        if(ocr.mf & PROG_ACT){
                ocr.mf |= TRNK_PROG;
                ocr.xd=2;  /* expected digits */

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        strcpy(lcd_1l_buf,"      :Ì˜ê§ó ·¯˜ íë°ó˜ê,Ì˜ê§ó ¸•˛§˘ıë˜§ì");
                        str2lcd(lcd_1l_buf);
                }
                else
#endif
                        str2lcd(" TRUNK PROG,ENTER TRUNK TYPE:");
#else
                init_lcd();
                str2lcd("TRUNK PROG,ENTERTRUNK TYPE:");
#endif

        }
}


void rtn81(INDEX_TYPE pt){
        BYTE t;

#ifdef DEBUG_ACT
        str2lcd(" r81 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:81 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:81\n");
#endif
                return;
        }
        if(ocr.mf & TRNK_PROG){
                t=cpq[cpfq].stim & 0xff;
                if(t == CO_ID_KEY){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("     :¢˛éëı˜ ¢§ê¯ ê§ Ì˜ê§ó ˘§ëı™,¸§˙™ Ø°");
                        else
#endif
                                str2lcd(" CO TRUNK ");
#else
                        init_lcd();
                        str2lcd(" CO TRUNK");
#endif
                        ocr.d.t=CO_T;
                }
                else
                        if(t == TIE_ID_KEY){
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("      :¢˛éëı˜ ¢§ê¯ ê§ Ì˜ê§ó ˘§ëı™,TIE Ø°");
                                else
#endif
                                        str2lcd(" TIE TRUNK ");
#else
                                init_lcd();
                                str2lcd("TIE TRUNK");
#endif
                                ocr.d.t=TIE_T;
                        }
                        else    return;
                ocr.scdc=0;
                ocr.xd=NO_TRUNK_ID_DGT;

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd(lcd_1l_buf);
                        /* put cursor to the beginning of the 2nd line */
                else
#endif
                        str2lcd(" ENTER TRUNK #:");
                ocr.state=AWAIT_PROG_DEST;
        }
}

void rtn82(INDEX_TYPE pt){
        BYTE n;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r82 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:82 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:82\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)       /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5=n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.mf & XST_PROG) && ( (ocr.scdc & 0x0f)==1) )
                if( ( (ocr.d2d1 & 0x0f) != intrnl_1st_dgt)
                 && ( (ocr.d2d1 & 0x0f) != intrnl_1st_dgt_2)
                 && ( (ocr.d2d1 & 0x0f) != intrnl_1st_dgt_3)
                 && ( (ocr.d2d1 & 0x0f) != intrnl_1st_dgt_4)
                 && ( (ocr.d2d1 & 0x0f) != intrnl_1st_dgt_5)
                        ){

#ifdef OPERATOR_PC
                        tolcdq(0xff,CLEAR_LCD);
#else
                        init_lcd();
#endif


#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("    ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì,ñ®˛˜ §ìó„ı ¢¨Ïı");
                        else{
#endif
                                str2lcd(" INVD DEST. ");
                                str2lcd(" PROG. EXIT ");
#ifdef OPERATOR_FARSI
                        }
#endif


#ifdef TIME_DATE_DISPLAY
                        ddt_ad();
#else

#ifdef OPERATOR_PC
                        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                        else
#endif
                                str2lcd(" OPERATOR READY...  ");
#else
                        init_lcd();
                        str2lcd("OPR. READY...");
#endif

#endif
                        ocr.state=OPR_READY;
                        ocr.scdc=0;
                        ocr.mf=RESET;
                        ocr.s.t=ocr.d.t=NULL_T;
                        lpg1 &= ~PLS_BIT_POS;
                        lsg1 &= ~PLS_BIT_POS;
                        led_reset(PROG_LED);
                        led_reset(SOURCE_LED);
                        led_reset(DEST_LED);
                        return;
                }
        if( (ocr.scdc & 0x0f) < ocr.xd) return;
        switch(ocr.d.t){
                case XST_T:     a=fcp();
                                break;
#if(NO_CO != 0)
                case  CO_T:     a=fd();
//DYNAMIC : ">" --> ">="
                                if( (a >= NO_CO) || (a == 0) )
                                        a=ERR_CODE;
                                else
                                        a--;
                                /* acting on trunks starting from trunk 0 */
                                /* user enter trunks starting from 1 */
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     a=fd();
//DYNAMIC : ">" --> ">="
                                if( (a >= NO_TIE) || (a == 0) )
                                        a=ERR_CODE;
                                else
                                        a--;
                                /* acting on trunks starting from trunk 0 */
                                /* user enter trunks starting from 1 */
                                break;
#endif
#if ((defined EM_INSTALLED) && (defined OPR_EM_PROGRAM))
                case EM_T:
                                a=fd();
                                if( (a >= NO_EM) || (a == 0) )
                                        a=ERR_CODE;
                                else
                                        a--;
                                /* acting on trunks starting from trunk 0 */
                                /* user enter trunks starting from 1 */
                                break;
#endif
#if ((defined CAS1_INSTALLED) && (defined OPR_CAS1_PROGRAM))
                case CAS1_T:
                                a=fd();
                                if( (a >= no_cas1[0]) || (a == 0) )
                                        a=ERR_CODE;
                                else
                                        a--;
                                /* acting on trunks starting from trunk 0 */
                                /* user enter trunks starting from 1 */
                                break;
#endif
#if ((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM))
                case CAS3_T:
                                a=fd();
                                if( (a >= no_cas3[0]) || (a == 0) )
                                        a=ERR_CODE;
                                else
                                        a--;
                                /* acting on trunks starting from trunk 0 */
                                /* user enter trunks starting from 1 */
                                break;
#endif
                default:        a=ERR_CODE; break;
        }
        if(a == ERR_CODE){      /* if destination not valid */

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("    ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì,ñ®˛˜ §ìó„ı ¢¨Ïı");
                else{
#endif
                        str2lcd(" INVALID DESTINATION                    ");
                        str2lcd(" EXIT PROGRAMMING  ");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                init_lcd();
                str2lcd("INVALID DEST.   ");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }
        ocr.d.i=a;
        ocr.scdc=0;
        if( (ocr.mf & DID_PROG)  &&  (ocr.d.t == XST_T) ){
                ocr.s.i=MAC_EXT_DID;
                ocr.s.t=SET;

#ifdef OPERATOR_PC
                        tolcdq(0xff,CLEAR_LCD);
#else
                        init_lcd();
#endif

                if(smaf() == ERR_CODE){

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("            !ñ®˛˜ ¢¯õ¯ı ¸•˛§˘ıë˜§ì ˆëÓıê");
                        else
#endif
                                str2lcd(" prog. failed!                          ");
                }

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                else
#endif
                        str2lcd(" START NEW PROG.:");
                ocr.d.t=NULL_T; ocr.s.t=NULL_T;
                ocr.mf=PROG_ACT; ocr.state=OPR_PROG;
                return;
        }
        ocr.xd=MAX_FUNC_DGT;

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
                str2lcd("           :¢˛éëı˜ ¢§ê¯ ê§ ¸•˛§˘ıë˜§ì ¢Ó");
                str2lcd(lcd_space_line);
                str2lcd(lcd_3_space);
        }
        else
#endif
                str2lcd(" enter func. code:");
#else
        init_lcd();
        str2lcd("ENTER FUNCTION  CODE:");
#endif
        ocr.state=AWAIT_PROG_FUNC;
}

void rtn83(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r83 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:83 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:83\n");
#endif
                return;
        }
        a=fd();
        a--;  /* for saving trunk # from 0 instead of 1 */
        switch(ocr.d.t){

#if(NO_CO != 0)
                case  CO_T:     if(a >= NO_CO)  a=ERR_CODE;
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     if(a >= NO_TIE) a=ERR_CODE;
                                break;
#endif

#if ((defined EM_INSTALLED) && (defined OPR_EM_PROGRAM))
                case EM_T:
                                if (a >= NO_EM)  a=ERR_CODE;
                                break;
#endif
#if ((defined CAS1_INSTALLED) && (defined OPR_CAS1_PROGRAM))
                case CAS1_T:
                                if (a >= no_cas1[0])  a=ERR_CODE;
                                break;
#endif
#if ((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM))
                case CAS3_T:
                                if (a >= no_cas3[0])  a=ERR_CODE;
                                break;
#endif

                default:        a=ERR_CODE; break;
        }
        if(a == ERR_CODE){  /* if destination not valid */

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("    ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì,ñ®˛˜ §ìó„ı ¢¨Ïı");
                else{
#endif
                        str2lcd(" INVALID DESTINATION                    ");
                        str2lcd(" EXIT PROGRAMMING  ");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                init_lcd();
                str2lcd("INVALID DEST.   ");
                str2lcd("EXIT PROGRAMMING");
#endif

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
        }
        else{
                ocr.d.i=a;
                ocr.scdc=0;
                ocr.xd=MAX_FUNC_DGT;

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
#ifdef OPERATOR_PC
                        tolcdq(0xff,CLEAR_LCD);
#else
                        init_lcd();
#endif
                        str2lcd("           :¢˛éëı˜ ¢§ê¯ ê§ ¸•˛§˘ıë˜§ì ¢Ó");
                        str2lcd(lcd_space_line);
                        str2lcd(lcd_3_space);
                }
                else
#endif
                        str2lcd(" enter func. code:");
#else
                init_lcd();
                str2lcd("ENTER FUNCTION  CODE:");
#endif
                ocr.state=AWAIT_PROG_FUNC;
        }
}

int fd(){  /* find destination from numbers dialled */
        int i;
        BYTE a,c,w;
        c=ocr.scdc & 0x0f;
        if(c == 0)      return ERR_CODE;
        if(c == 3)
                w=100;
        else
                if(c == 2)
                        w=10;
                else
                        w=1;
        a=ocr.d2d1 & 0x0f;
        if(a > 9)     a=0;
        i=a*w;
        if( (--c) == 0)    return i;

        a=ocr.d2d1 >> 4;
        if(a > 9)     a=0;
        w /= 10;
        i +=a*w;
        if( (--c) == 0)    return i;

        a=ocr.d4d3 & 0x0f;
        if(a > 9)     a=0;
        i +=a;
        return i;
}

void rtn84(INDEX_TYPE pt){
        BYTE n;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r84 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:84 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:84\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1:ocr.d2d1=n; break;
                case  2:ocr.d2d1 |=n; break;
                case  3:ocr.d4d3=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < MAX_FUNC_DGT)   return;
        a=fd();
        switch(ocr.d.t){
                case XST_T:     if(a >= MA_XF_NO)
                                        a=ERR_CODE;
                                break;
#if(NO_CO != 0)
                case  CO_T:     if(a >= MA_TF_NO)
                                        a=ERR_CODE;
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     if(a >= MA_TF_NO)
                                        a=ERR_CODE;
                                break;
#endif

#if ((defined EM_INSTALLED) && (defined OPR_EM_PROGRAM))
                case EM_T:
                                if(a >= MA_EF_CF_NO)
                                        a=ERR_CODE;
                                break;
#endif
#if ((defined CAS1_INSTALLED) && (defined OPR_CAS1_PROGRAM))
                case CAS1_T:
                                if(a >= MA_EF_CF_NO)
                                        a=ERR_CODE;
                                break;
#endif
#if ((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM))
                case CAS3_T:
                                if(a >= MA_EF_CF_NO)
                                        a=ERR_CODE;
                                break;
#endif

                default:        a=ERR_CODE; break;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        if(a == ERR_CODE){  /* if function not valid */

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("      ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì,ñ®˛˜ §ìó„ı ¢Ó");
                else{
#endif
                        str2lcd(" INVALID FUNCTION CODE                   ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID FUNCTION");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }
        ocr.s.i=a;
        ocr.scdc=0;

        if( (a == MAC_TRNK_DID)  &&  (ocr.mf & TRNK_PROG) ){
                ocr.s.t=SET;
                if(smaf() == ERR_CODE){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("                 !¢™˜ ˘óÍ§˛£ï ¸•˛§˘ıë˜§ì");
                                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                        }
                        else{
#endif
                                str2lcd(" PROGRAMMING FAILED!                    ");
                                str2lcd(" START NEW PROGRAM:");
#ifdef OPERATOR_FARSI
                        }
#endif


#else
                        str2lcd("PROGRAMMING FAIL");
                        str2lcd("START NEW PROG.:");
#endif
                        ocr.d.t=NULL_T; ocr.s.t=NULL_T;
                        ocr.mf=PROG_ACT; ocr.state=OPR_PROG;
                        return;
                }

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    :¢˛éëı˜ ¢§ê¯ ê§ ¢¨Ïı");
                else
#endif
                        str2lcd(" enter dest.:");
                ocr.xd=no_intrnl_dgt;
                ocr.d.t=XST_T;
                ocr.mf=PROG_ACT | XST_PROG | DID_PROG;
                ocr.state=AWAIT_PROG_DEST;
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
        else
#endif
                str2lcd(" 1:act.  0:inact ");
        ocr.state=AWAIT_ACT_INACT;
}

void rtn85(INDEX_TYPE pt){
        BYTE t,a;

#ifdef DEBUG_ACT
        str2lcd(" r85 ");
#endif

        t=pt.t;
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:85 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:85\n");
#endif
                return;
        }
        a=pt.i;
        if(t == CO_T){
#if(NO_CO != 0)
                ctst[a].s=IN_RCL_QU;
                ctst[a].i=torq(pt);
#endif
        }
        else{
                if(t == TIE_T){
#ifndef NO_TIE_INIT
                        ttst[a].s=IN_RCL_QU;
                        ttst[a].i=torq(pt);
#endif
                }
                else    return;
        }
}

void rtn86(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r86 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:86 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:86\n");
#endif
                return;
        }
        a=fd();
        switch(ocr.d.t){
                case XST_T:     if(a >= MA_XF_NO)
                                        a=ERR_CODE;
                                break;
#if(NO_CO != 0)
                case  CO_T:     if(a >= MA_TF_NO)
                                        a=ERR_CODE;
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     if(a >= MA_TF_NO)
                                        a=ERR_CODE;
                                break;
#endif

#if ((defined EM_INSTALLED) && (defined OPR_EM_PROGRAM))
                case EM_T:
                                if(a >= MA_EF_CF_NO)
                                        a=ERR_CODE;
                                break;

#endif
#if ((defined CAS1_INSTALLED) && (defined OPR_CAS1_PROGRAM))
                case CAS1_T:
                                if(a >= MA_EF_CF_NO)
                                        a=ERR_CODE;
                                break;
#endif
#if ((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM))
                case CAS3_T:
                                if(a >= MA_EF_CF_NO)
                                        a=ERR_CODE;
                                break;
#endif

                case OPR_T:     if(a >= MA_SF_NO)
                                        a=ERR_CODE;
                                break;
                default:        a=ERR_CODE; break;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        if(a == ERR_CODE){      /* if function not valid */

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı ¸•˛§˘ıë˜§ì ¢Ó ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID FUNCTION CODE                  ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID FUNCTION");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }
        ocr.s.i=a;
        ocr.scdc=0;

        if( (a==MAC_TRNK_DID)  &&  (ocr.mf & TRNK_PROG) ){
                ocr.s.t=SET;
                if(smaf() == ERR_CODE){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("                !¢™˜ ˘óÍ§˛£ï ¸•˛§˘ıë˜§ì ");
                                str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
                        }
                        else{
#endif
                                str2lcd(" PROGRAMMING FAILED!                    ");
                                str2lcd(" START NEW PROGRAM:");
#ifdef OPERATOR_FARSI
                        }
#endif


#else
                        str2lcd("PROGRAMMING FAIL");
                        str2lcd("START NEW PROG.:");
#endif
                        ocr.d.t=NULL_T; ocr.s.t=NULL_T;
                        ocr.mf=PROG_ACT; ocr.state=OPR_PROG;
                        return;
                }

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                   :¢˛éëı˜ ¢§ê¯ ê§ ¢¨Ïı ");
                else
#endif
                        str2lcd(" enter dest.:");
                ocr.xd=no_intrnl_dgt;
                ocr.d.t=XST_T;
                ocr.mf=PROG_ACT | XST_PROG | DID_PROG;
                ocr.state=AWAIT_PROG_DEST;
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
        else
#endif
                str2lcd(" 1:act.  0:inact ");
        ocr.state=AWAIT_ACT_INACT;
}

void rtn87(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r87 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:87 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:87\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n != 1)
                n=0;
        ocr.s.t=n;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
        if(ocr.s.i == OP_FIRST_ZERO_ON_CO){
                if(ocr.s.t==1)  op_can_dial_on_co=SET;
                else            op_can_dial_on_co=RESET;
#ifdef OPERATOR_PC
                flp=fopen(LOCAL_PARAM_FILE,"r+b");
                fseek(flp,1L,SEEK_SET);
                putc(op_can_dial_on_co,flp);
                fclose(flp);
#endif
        }
        else
#endif
        if(smaf() == ERR_CODE){  /* send MA packet to system */

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                !¢™˜ ˘óÍ§˛£ï ¸•˛§˘ıë˜§ì ");
                else
#endif
                        str2lcd(" PROGRAMMING FAILED!                    ");
#else
                str2lcd("PROGRAMMING FAIL");
#endif
        }
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
        else
#endif
                str2lcd(" START NEW PROGRAM:");
#else
        str2lcd("START NEW PROG.:");
#endif
#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

        if((ocr.s.t==1) && (ocr.d.t == OPR_T) && ((ocr.s.i == MAC_CAS3_HISTORICAL_REQUEST) || (ocr.s.i == MAC_CAS3_HISTORICAL_LOOSE_REQUEST))){
                fprintf(fhst,"\n\n    3-bit CAS Historical report of ch. no. %u   ",ocr.d.i+1);
                fprintf(fhst,  "\n    .........................................\n");
        }
#endif
#endif

#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

        if((ocr.s.t==1) && (ocr.d.t == OPR_T) && (ocr.s.i == MAC_EM_HISTORICAL_REQUEST)){
                fprintf(fhst,"\n\n    E&M Historical report of ch. no. %u   ",ocr.d.i+1);
                fprintf(fhst,  "\n    ...................................\n");
        }
#endif
#endif

#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

        if((ocr.s.t==1) && (ocr.d.t == OPR_T) && (ocr.s.i == MAC_CAS1_HISTORICAL_REQUEST)){
                fprintf(fhst,"\n\n    1-bit CAS Historical report of ch. no. %u   ",ocr.d.i+1);
                fprintf(fhst,  "\n    .........................................\n");
        }
#endif
#endif

        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}

void rtn88(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r88 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:88 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:88\n");
#endif
                return;
        }
        if(ocr.mf & PROG_ACT){
                ocr.mf |= SYSTEM_PROG;

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("    :¢˛éëı˜ ¢§ê¯ ê§ ¸ıó®˛® ¸•˛§˘ıë˜§ì ¢Ó");
                        str2lcd(lcd_space_line);
                }
                else
#endif
                        str2lcd(" SYSTEM PROG,ENTER FUNCTION #:");
#else
                init_lcd();
                str2lcd("SYS. PROG ENTER FUNCTION #:");
#endif
                ocr.d.t=OPR_T;
                ocr.d.i=TON;
                ocr.scdc=0;
                ocr.xd=SYS_PROG_DGTS;
                ocr.state=AWAIT_SYS_PROG;
        }
}

void rtn89(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r89 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:89 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:89\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5=n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < ocr.xd)
                return;
        sys_func();
}

void rtn90(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r90 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:90 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:90\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5=n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < ocr.xd)
                return;
        pswd_d2d1=ocr.d2d1;
        pswd_d4d3=ocr.d4d3;

#ifdef PARAMETER_DOWN_UP_LOAD
        unp();
#endif

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("                .ñ®ê Òë„Í ¢˛¢õ §¯ì‰ ˘ıÛÓ");
                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
        }
        else{
#endif
                str2lcd("NEW PASSWORD IS ACTIVE.                 ");
                str2lcd("START NEW PROGRAMMING:");

#ifdef OPERATOR_FARSI
        }
#endif


#else
        init_lcd();
        str2lcd("NEW PASSWORD ACT");
        str2lcd("START NEW PROG.:");
#endif

        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}

void rtn91(INDEX_TYPE pt){
        BYTE t,a;

#ifdef DEBUG_ACT
        str2lcd(" r91 ");
#endif

        t=pt.t;
        if(t == OPR_T)  return;
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:91 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:91,%x\n",t);
#endif
                return;
        }
        a=pt.i;
        if(t == CO_T){
#if(NO_CO != 0)
                ctst[a].s=IN_RCL_QU;
                ctst[a].i=torq(pt);
#endif
        }
        else{
                if(t == TIE_T){
#ifndef NO_TIE_INIT
                        ttst[a].s=IN_RCL_QU;
                        ttst[a].i=torq(pt);
#endif
                }
                else    return;
        }
        if( (ocr.s.t == t) && (ocr.s.i == a) )
                ocr.s.t=NULL_T;
        else
                if( (ocr.d.t == t) && (ocr.d.i == a) )
                        ocr.d.t=NULL_T;
                else
                        return;

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd(" ñ™§ì ");
        else
#endif
                str2lcd(" RECALL ");
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else
                ocr.mf &= ~DIAL_ACTIVE;
}

void rtn92(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r92 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:92 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:92\n");
#endif
                return;
        }
        if( (ocr.s.t==XST_T) && ( (ocr.fs & 0x0f)==INACT_FAC1) ){
                a=ocr.s.i;
                ocr.s.t=NULL_T;
        }
        else
                if( (ocr.d.t==XST_T) && ( (ocr.fs & 0xf0)==INACT_FAC2) ){
                        a=ocr.d.i;
                        ocr.d.t=NULL_T;
                }
                else    return;
//      xst[a].s=IDLE;
        rtp_oprside(a, XST_T);
        pcpmftx(FORCE_RELEASE,a,XST_T);
        if( (ocr.s.t==NULL_T) && (ocr.d.t==NULL_T) ){
                hld.t=NULL_T;
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else{
                ocr.state=OPRSIDE_OPR_TALK;

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
        }
}

#ifdef OPR_MALICIOUS_DTCT_ACT

void rtn93(INDEX_TYPE pt){
        int a;
        BYTE t;

#ifdef DEBUG_ACT
        str2lcd(" r93 ");
#endif
        t=pt.t;
        if( (t != XST_T) && ( (t & TRUNK_T) == RESET) ){
#ifdef DEBUG_ACT
                str2lcd(" ERR:93 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:93\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        a=hex2bcd(pt.i);
        switch(t){
                case XST_T:

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("      ");
                                        int2lcd(hex2bcd(find_subscriber_no(pt.i)));
                                        str2lcd(":Ùüê•ı ¸Û°ê¢");
                                }
                                else
#endif
                                        str2lcd(" malicious ext.:");
#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        int2lcd(hex2bcd(find_subscriber_no(pt.i)));
                                break;
#if(NO_CO != 0)
                case  CO_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("      ");
                                        hex2lcd(a + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(":Ùüê•ı ¸§˙™ Ø°");
                                }
                                else
#endif
                                        str2lcd("malicious CO trunk:");
#else
                                str2lcd("malicious  CO:");
#endif

#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                {
#if (NO_CO >= 100)
                                        hex2lcd((a + 1)>>8);
#endif
                                        hex2lcd(a + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                }
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2ld("      ");
                                        hex2lcd(a + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(":Ùüê•ı TIE Ø°");
                                }
                                else
#endif
                                        str2lcd("malicious TIE trunk:");
#else
                                str2lcd("malicious TIE:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(a + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                break;
#endif


#ifdef EM_INSTALLED
                case  EM_T:

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("      ");
                                        hex2lcd(a + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(":Ùüê•ı ¸§˙™ ˆ˛ì Ø°");
                                }
                                else
#endif
                                        str2lcd("malicious STD trunk:");
#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(a + 1);
                                        /* for displaying first trunk as 1 not 0 */
                                break;
#endif
                default:    break;
        }
}
#endif


void rtn94(INDEX_TYPE pt){
        int a,b;

#ifdef DEBUG_ACT
        str2lcd(" r94 ");
#endif
        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:94 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:94\n");
#endif
                return;
        }
        a=ocr.s.i;
        b=ocr.d.i;
        if( (ocr.fs & 0x0f) == HOLD_FAC1){
                if( (ocr.fs & 0xf0) == INACT_FAC2)
                        pcpmftx(ATT_RLS,b,ocr.d.t);
        }
        else
                if( (ocr.fs & 0xf0) == HOLD_FAC2){
                        if( (ocr.fs & 0x0f) == INACT_FAC1)
                                pcpmftx(ATT_RLS,a,ocr.s.t);
                }
                else{
                        if(ocr.state == OPR_EXT_OVRD){
                                if( ( (ocr.fs & 0x0f) == INACT_FAC1) &&
                                        (ocr.s.t == XST_T) )
                                        pcpmftx(RLS,a,XST_T);
                                else
                                        if( ( (ocr.fs & 0xf0) == INACT_FAC2) &&
                                                (ocr.d.t == XST_T) )
                                                pcpmftx(RLS,b,XST_T);
                        }
                }
        rtp(ocr.s);
        rtp(ocr.d);
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.fs=0xff;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
}

void rtn95(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r95 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:95 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:95\n");
#endif
                return;
        }
        if( ( (ocr.fs & 0x0f) == INACT_FAC1) && (ocr.s.t == XST_T) ){
                a=ocr.s.i;
                ocr.s=pt;
                ocr.fs &= 0xf0;
                ocr.fs |= ACT_FAC1;
        }
        else
                if( ( (ocr.fs & 0xf0) == INACT_FAC2) && (ocr.d.t == XST_T) ){
                        a=ocr.d.i;
                        ocr.d=pt;
                        ocr.fs &= 0x0f;
                        ocr.fs |= ACT_FAC2;
                }
                else
                        return;
//      xst[a].s=IDLE;
        rtp_oprside(a, XST_T);

#ifdef DISPLAY_STATUS_ACT
        ds[a]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif
        a=pt.i;
        xst[a].s=IN_OCR;

#ifdef DISPLAY_STATUS_ACT
        ds[a]=OFF_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);
#else
                init_lcd();
#endif
                str2lcd(lcd_3_space);
                int2lcd(hex2bcd(find_subscriber_no(a)));
                str2lcd(":¸Û°ê¢ Ø®¯ó ˘ıÛëÓı ˆóÍ§         ");
        }
        else
#endif
                str2lcd(" picked up by ext.:");
#else
        init_lcd();
        str2lcd("PICKED UP BY    EXT.:");
#endif

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                int2lcd(hex2bcd(find_subscriber_no(a)));
        ocr.state=OPRSIDE_OPR_TALK;
}

void rtn96(){
#ifdef HDLC_8993
        ihc(0);
#endif
}

#if  (defined (DAY_NIGHT_PARAMETERS_FILE) )

#if  ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )

void rtn97_1(INDEX_TYPE pt){
        int a,b;
        unsigned int c;

#ifdef DEBUG_ACT
        str2lcd(" r97 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:97 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:97\n");
#endif
                return;
        }
        a=(cpq[cpfq].stim & 0xff) - N_1;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        pfp[0]=fopen(pfna[0][a],"rb");
        if( (pfp != NULL) && (vpf(pfp[0],0) == OK) ){
        /* if file is valid,the parameters is in parmb[]. */
                a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                /* feature activation bit in command frame is 0 */
#ifdef HOT_STANDBY
                pmamftx_both_sys(a,TON,OPR_T,0);
#else
                pmamftx(a,TON,OPR_T,0);
#endif
                a |= FEATURE_ACTIVATION;
                /* feature activation bit in parameters frames is 1 */
                for(b=0;b < no_load_parm[0];b++){
                        c=parmb[b];
#ifdef HOT_STANDBY
                        pmamftx_both_sys(a,c >> 4,c & 0x0f,0);
#else
                        pmamftx(a,c >> 4,c & 0x0f,0);
#endif
                }

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                 ...Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
                else
#endif
                        str2lcd("UPLOAD STARTED...");
#ifdef HOT_STANDBY
                f |= ( PARM_UP_1 | PARM_UP_2 );
#else
                f |= PARM_UP_1;
#endif
#ifdef OPR_UPLOAD_WITH_TIMER
                upload_count=UPLOAD_PARM_TICK;
#endif
                set_stm_for_up_down_load();
                /* only leds(),pc_timer_init(),lcds() & ohh() are unmasked */
                DIS_SPKR;  /* disable tone ringer */
        }
        else{

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                }
                else{
#endif
                        str2lcd("INVALID PARAMETERS FILE.               ");
                        str2lcd(" START NEW PROGRAMMING:");
#ifdef OPERATOR_FARSI
                }
#endif
        }
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
void rtn97_2(INDEX_TYPE pt){
        int a,b;
        unsigned int c;

#ifdef DEBUG_ACT
        str2lcd(" r97 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:97 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:97\n");
#endif
                return;
        }
        a=(cpq[cpfq].stim & 0xff) - N_1;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        pfp[1]=fopen(pfna[1][a],"rb");
        if( (pfp[1] != NULL) && (vpf(pfp[1],1) == OK) ){
        /* if file is valid,the parameters is in parmb[]. */
                a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                /* feature activation bit in command frame is 0 */
                pmamftx(a,TON,OPR_T,1);

                a |= FEATURE_ACTIVATION;
                /* feature activation bit in parameters frames is 1 */
                for(b=0;b < no_load_parm[1];b++){
                        c=parmb[b];
                        pmamftx(a,c >> 4,c & 0x0f,1);
                }

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                 ...Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
                else
#endif
                        str2lcd("UPLOAD STARTED...");
                f |= PARM_UP_2;
                set_stm_for_up_down_load();
                /* only leds(),pc_timer_init(),lcds() & ohh() are unmasked */
                DIS_SPKR;  /* disable tone ringer */
        }
        else{

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                }
                else{
#endif
                        str2lcd("INVALID PARAMETERS FILE.               ");
                        str2lcd(" START NEW PROGRAMMING:");
#ifdef OPERATOR_FARSI
                }
#endif
        }
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
void rtn97_3(INDEX_TYPE pt){
        int a,b;
        unsigned int c;

#ifdef DEBUG_ACT
        str2lcd(" r97 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:97 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:97\n");
#endif
                return;
        }
        a=(cpq[cpfq].stim & 0xff) - N_1;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        pfp[2]=fopen(pfna[2][a],"rb");
        if( (pfp[2] != NULL) && (vpf(pfp[2],2) == OK) ){
        /* if file is valid,the parameters is in parmb[]. */
                a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                /* feature activation bit in command frame is 0 */
                pmamftx(a,TON,OPR_T,2);

                a |= FEATURE_ACTIVATION;
                /* feature activation bit in parameters frames is 1 */
                for(b=0;b < no_load_parm[2];b++){
                        c=parmb[b];
                        pmamftx(a,c >> 4,c & 0x0f,2);
                }

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                 ...Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
                else
#endif
                        str2lcd("UPLOAD STARTED...");
                f |= PARM_UP_3;
                set_stm_for_up_down_load();
                /* only leds(),pc_timer_init(),lcds() & ohh() are unmasked */
                DIS_SPKR;  /* disable tone ringer */
        }
        else{

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                }
                else{
#endif
                        str2lcd("INVALID PARAMETERS FILE.               ");
                        str2lcd(" START NEW PROGRAMMING:");
#ifdef OPERATOR_FARSI
                }
#endif
        }
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
#endif


#if ( (NO_OPRTR > 1) && (!defined ONLY_THIS_OPRTR_CAN_UPLOAD) )

void rtn97_1(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r97 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:97 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:97\n");
#endif
                return;
        }
        a=(cpq[cpfq].stim & 0xff) - N_1;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        pfp[0]=fopen(pfna[0][a],"rb");
        if( (pfp[0] != NULL) && (vpf(pfp[0],0) == OK) ){
        /* if file is valid,the parameters is in parmb[]. */
#ifdef HOT_STANDBY
                pmamftx_both_sys(MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1,TON,OPR_T,0);
#else
                pmamftx(MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1,TON,OPR_T,0);
#endif
                /* send upload command to system,with feature activation */
                /* bit to 0 */

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("....¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı");
                else
#endif
                        str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...    ");
                ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_1;

#ifdef OPR_UPLOAD_WITH_TIMER
                upload_count=UPLOAD_ACCEPT_REJECT_TICK;
#endif
                return;
        }
        /* invalid file */

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
        }
        else{
#endif
                str2lcd("INVALID PARAMETERS FILE.               ");
                str2lcd(" START NEW PROGRAMMING:");

#ifdef OPERATOR_FARSI
        }
#endif
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
void rtn97_2(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r97 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:97 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:97\n");
#endif
                return;
        }
        a=(cpq[cpfq].stim & 0xff) - N_1;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        pfp[1]=fopen(pfna[1][a],"rb");
        if( (pfp[1] != NULL) && (vpf(pfp[1],1) == OK) ){
        /* if file is valid,the parameters is in parmb[]. */

                pmamftx(MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1,TON,OPR_T,1);
                /* send upload command to system,with feature activation */
                /* bit to 0 */

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("....¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı");
                else
#endif
                        str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...   ");
                ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_2;
                return;
        }
        /* invalid file */

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
        }
        else{
#endif
                str2lcd("INVALID PARAMETERS FILE.               ");
                str2lcd(" START NEW PROGRAMMING:");

#ifdef OPERATOR_FARSI
        }
#endif
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
void rtn97_3(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r97 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:97 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:97\n");
#endif
                return;
        }
        a=(cpq[cpfq].stim & 0xff) - N_1;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        pfp[2]=fopen(pfna[2][a],"rb");
        if( (pfp[2] != NULL) && (vpf(pfp[2],2) == OK) ){
        /* if file is valid,the parameters is in parmb[]. */

                pmamftx(MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1,TON,OPR_T,2);
                /* send upload command to system,with feature activation */
                /* bit to 0 */

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("....¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı");
                else
#endif
                        str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...   ");
                ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_3;
                return;
        }
        /* invalid file */

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
        }
        else{
#endif
                str2lcd("INVALID PARAMETERS FILE.               ");
                str2lcd(" START NEW PROGRAMMING:");

#ifdef OPERATOR_FARSI
        }
#endif
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
#endif

#endif

#ifdef SYSTEM_MANUAL_TEST
void rtn97(INDEX_TYPE pt){
        int a,j,i;
        char s[4];
#ifdef DEBUG_ACT
        str2lcd(" r97 ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:97 ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:97\n");
#endif
                return;
        }
        if(!mtf)        return;
        j=26+ocr.scdc;
        i=6;
        a=(cpq[cpfq].stim & 0xff) - N_1 + 1;
        if(a > 9)
                a=0;
        if(ocr.scdc==0)
//              poke_s(25,8,"                         ",RED<<8);
                poke_s(25,8,"                         ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
        sprintf(s,"%d",a);
//      poke_s(j,i,s,WHITE<<8);
        poke_s(j,i,s,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        j++;
        cursor(j,i);
        if( (++ocr.scdc & 0x01) == RESET)    /* even number of digits */
                a <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=a; break;
                case  2: ocr.d2d1 |=a; break;
                case  3: ocr.d4d3=a; break;
                case  4: ocr.d4d3 |=a; break;
                case  5: ocr.d6d5=a; break;
                case  6: ocr.d6d5 |=a; break;
                case  7: ocr.d8d7 =a; break;
                case  8: ocr.d8d7 |=a; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) >= no_intrnl_dgt ){   /* if last digit */
                a=fcp();
                if( (a = fcp()) == ERR_CODE){
//                      poke_s(25,8,"INVALID EXTENSION NUMBER!",RED<<8);
                        poke_s(25,8,"INVALID EXTENSION NUMBER!",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                }
                else{
                        ocr.s.i=MAC_EXT_MANUAL_TEST;
                        ocr.s.t=TON;
                        ocr.d.i=a;
                        ocr.d.t=OPR_T;

                        if(smaf() == ERR_CODE){
//                              poke_s(25,8,"SYSTEM ERROR!",RED<<8);
                                poke_s(25,8,"SYSTEM ERROR!",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                        }
                        else
                                cursor(26,6);
                }
                ocr.scdc=0;
        }
}
#endif


void rtn98(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r98 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:98 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:98\n");
#endif
                return;
        }
        a=pt.i;
//      xst[a].s=IDLE;
        rtp(pt);

#ifdef DISPLAY_STATUS_ACT
        ds[a]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(a);
#endif
#endif
        if( (ocr.s.i == a) && (ocr.s.t == XST_T) )
                ocr.s.t=NULL_T;
        else
                if( (ocr.d.i == a) && (ocr.d.t == XST_T) )
                        ocr.d.t=NULL_T;
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.fs=0xff;
                ocr.mf=RESET;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(".¢§Ó ‚ØÏ ");
                int2lcd(hex2bcd(find_subscriber_no(a)));
                str2lcd(" :¸Û°ê¢");
        }
        else
#endif
                str2lcd("ext. ");

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET){
#endif
                int2lcd(hex2bcd(find_subscriber_no(a)));
                str2lcd(" ON HOOK");

#ifdef OPERATOR_FARSI
        }
#endif
}

void rtn99(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r99 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:99 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:99\n");
#endif
                return;
        }
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.fs=0xff;
        ocr.mf=RESET;
        hld.t=NULL_T;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
        pcpmftx(RLS,TON,OPR_T);

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
}

void rtn100(INDEX_TYPE pt){
        BYTE a,t;

#ifdef DEBUG_ACT
        str2lcd(" r100 ");
#endif

        t=pt.t;
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:100 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:100\n");
#endif
                return;
        }
        a=pt.i;
        if(t == CO_T){
#if(NO_CO != 0)
                rfrq(ctst[a].i);
                ctst[a].s=IDLE;
#endif
        }
        else{
#ifndef NO_TIE_INIT
                rfrq(ttst[a].i);
                ttst[a].s=IDLE;
#endif
        }
}




// night upload

#if (defined DAY_NIGHT_PARAMETERS_FILE)
void rtn101_1(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r101 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
//PIANO
                str2lcd(" ERR:101_1 ");
#endif

#ifdef OPERATOR_PC
//PIANO
                fprintf(fer,"ERR:101_1\n");
#endif
                return;
        }

        //if(oprnight == 0 ){
        pcpmftx(NIGHT_ACT,TON,OPR_T);
#ifdef LIM
        pcpmftx1(NIGHT_ACT,TON,OPR_T);
#if(NO_EXTENSION_C != 0)
        pcpmftx2(NIGHT_ACT,TON,OPR_T);
#endif
#endif
        //}
        //else
        oprnight=0;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("            ...ñ®ê Òë„Í §¯óê§ïê í™ ¸Û°ê¢");
        else
#endif
                str2lcd(" OPERATOR NIGHT ACTIVE...  ");
#else
        init_lcd();
        str2lcd("OPR. NIGHT ACT..");
#endif
        ocr.state=OPRSIDE_OPR_NIGHT;
        lpg1 |= NLS_BIT_POS;
}

void rtn101_2(INDEX_TYPE pt){

        unsigned int a;
#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
        int b;
        unsigned int c;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r101 ");
#endif

        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:101 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:101\n");
#endif
                return;
        }

        if(oprnight == 0){
                //pcpmftx(NIGHT_INACT,TON,OPR_T);
                pfp[0]=fopen(pfna[0][1],"rb");
                if( (pfp[0] != NULL) && (vpf(pfp[0],0) == OK) ){
                                /* if file is valid,the parameters */
                                /* is in parmb[]. */
                        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                          /* activation feature in command frame */
                                       /* is 0 */
#ifdef HOT_STANDBY
                        pmamftx_both_sys(a,TON,OPR_T,0);  /* send upload command to system */
#else
                        pmamftx(a,TON,OPR_T,0);  /* send upload command to system */
#endif

#if ( (NO_OPRTR > 1) && (!defined ONLY_THIS_OPRTR_CAN_UPLOAD) )

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("   ¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı ");
                        else
#endif
                        str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...    ");
                        ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_1;
#ifdef OPR_UPLOAD_WITH_TIMER
                        upload_count=UPLOAD_ACCEPT_REJECT_TICK;
#endif
                        oprnight++;
                        return;
#endif


#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
                        a |= FEATURE_ACTIVATION;
                                        /* activation feature is parameters */
                                        /* frame is 1 */
                        for(b=0;b < no_load_parm[0];b++){
                                c=parmb[b];
#ifdef HOT_STANDBY
                                pmamftx_both_sys(a,c >> 4,c & 0x0f,0);
#else
                                pmamftx(a,c >> 4,c & 0x0f,0);
#endif
                        }
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("...ñ®ê ˘¢™ •ëËç Ùó®˛® ˘ì ë˚§óıê§ëï Òë®§ê");
                        else
#endif
                        str2lcd("UPLOAD STARTED...");
#ifdef HOT_STANDBY
                        f |= ( PARM_UP_1 | PARM_UP_2 );
#else
                        f |= PARM_UP_1;
#endif
#ifdef OPR_UPLOAD_WITH_TIMER
                        upload_count=UPLOAD_PARM_TICK;
#endif
                        set_stm_for_up_down_load();
                                        /* only leds(),pc_timer_init(),lcds() & */
                                        /* onh() are unmasked */
                        DIS_SPKR;  /* disable tone ringer */
                        oprnight++;
                        return;
#endif
                }
                else{
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        }
                        else{
#endif
                                str2lcd("INVALID PARAMETERS FILE.                ");
#ifdef OPERATOR_FARSI
                        }
#endif
                        rtn101_1(pt);
                        return;
                }
        }

#ifdef LIM
        else if(oprnight == 1){
                //pcpmftx1(NIGHT_INACT,TON,OPR_T);
                pfp[1]=fopen(pfna[1][1],"rb");
                if( (pfp[1] != NULL) && (vpf(pfp[1],1) == OK) ){
                                /* if file is valid,the parameters */
                                /* is in parmb[]. */
                        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                          /* activation feature in command frame */
                                       /* is 0 */
                        pmamftx(a,TON,OPR_T,1);  /* send upload command to system */

#if ( (NO_OPRTR > 1) && (!defined ONLY_THIS_OPRTR_CAN_UPLOAD) )

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("   ¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı ");
                        else
#endif
                        str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...   ");
                        ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_2;
                        oprnight++;
                        return;
#endif


#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
                        a |= FEATURE_ACTIVATION;
                                        /* activation feature is parameters */
                                        /* frame is 1 */
                        for(b=0;b < no_load_parm[1];b++){
                                c=parmb[b];
                                pmamftx(a,c >> 4,c & 0x0f,1);
                        }
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("...ñ®ê ˘¢™ •ëËç Ùó®˛® ˘ì ë˚§óıê§ëï Òë®§ê");
                        else
#endif
                                str2lcd("UPLOAD STARTED...");
                        f |= PARM_UP_2;
                        set_stm_for_up_down_load();
                                        /* only leds(),pc_timer_init(),lcds() & */
                                        /* onh() are unmasked */
                        DIS_SPKR;  /* disable tone ringer */
                        oprnight++;
                        return;
#endif
                }
                else{
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        }
                        else{
#endif
                                str2lcd("INVALID PARAMETERS FILE.                ");
#ifdef OPERATOR_FARSI
                        }
#endif
                        rtn101_1(pt);
                        return;
                }
        }

#if(NO_EXTENSION_C != 0)
        else if(oprnight == 2){
                //pcpmftx2(NIGHT_INACT,TON,OPR_T);
                pfp[2]=fopen(pfna[2][1],"rb");
                if( (pfp[2] != NULL) && (vpf(pfp[2],0) == OK) ){
                                /* if file is valid,the parameters */
                                /* is in parmb[]. */
                        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                          /* activation feature in command frame */
                                       /* is 0 */
                        pmamftx(a,TON,OPR_T,2);  /* send upload command to system */

#if ( (NO_OPRTR > 1) && (!defined ONLY_THIS_OPRTR_CAN_UPLOAD) )

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("   ¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı ");
                        else
#endif
                        str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...   ");
                        ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_3;
                        oprnight++;
                        return;
#endif


#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
                        a |= FEATURE_ACTIVATION;
                                        /* activation feature is parameters */
                                        /* frame is 1 */
                        for(b=0;b < no_load_parm[2];b++){
                                c=parmb[b];
                                pmamftx(a,c >> 4,c & 0x0f,2);
                        }
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("...ñ®ê ˘¢™ •ëËç Ùó®˛® ˘ì ë˚§óıê§ëï Òë®§ê");
                        else
#endif
                        str2lcd("UPLOAD STARTED...");
                        f |= PARM_UP_3;
                        set_stm_for_up_down_load();
                                        /* only leds(),pc_timer_init(),lcds() & */
                                        /* onh() are unmasked */
                        DIS_SPKR;  /* disable tone ringer */
                        oprnight++;
                        return;
#endif
                }
                else{
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        }
                        else{
#endif
                                str2lcd("INVALID PARAMETERS FILE.                ");
#ifdef OPERATOR_FARSI
                        }
#endif
                        rtn101_1(pt);
                        return;
                }
        }
#endif
#endif

        else    return;
}

void rtn102_1(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r102 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:102 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:102\n");
#endif
                return;
        }

        if(oprday == 0 ){
                pcpmftx(NIGHT_INACT,TON,OPR_T);
#ifdef LIM
                pcpmftx1(NIGHT_INACT,TON,OPR_T);
#if(NO_EXTENSION_C != 0)
                pcpmftx2(NIGHT_INACT,TON,OPR_T);
#endif
#endif
        }
        else
                oprday=0;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif
        ocr.state=OPR_IDLE;
        lpg1 &= ~NLS_BIT_POS;
        lsg1 &= ~NLS_BIT_POS;
        led_reset(NIGHT_LED);
}

void rtn102_2(INDEX_TYPE pt){

        unsigned int a;
#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
        int b;
        unsigned int c;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r102 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:102 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:102\n");
#endif
                return;
        }

        if(oprday == 0){
                pcpmftx(NIGHT_INACT,TON,OPR_T);
                pfp[0]=fopen(pfna[0][0],"rb");
                if( (pfp[0] != NULL) && (vpf(pfp[0],0) == OK) ){
                                /* if file is valid,the parameters */
                                /* is in parmb[]. */
                        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                          /* activation feature in command frame */
                                       /* is 0 */
#ifdef HOT_STANDBY
                        pmamftx_both_sys(a,TON,OPR_T,0);  /* send upload command to system */
#else
                        pmamftx(a,TON,OPR_T,0);  /* send upload command to system */
#endif

#if ( (NO_OPRTR > 1) && (!defined ONLY_THIS_OPRTR_CAN_UPLOAD) )

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("   ¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı ");
                        else
#endif
                        str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...    ");
                        ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_1;
#ifdef OPR_UPLOAD_WITH_TIMER
                        upload_count=UPLOAD_ACCEPT_REJECT_TICK;
#endif
                        oprday++;
                        return;
#endif


#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
                        a |= FEATURE_ACTIVATION;
                                        /* activation feature is parameters */
                                        /* frame is 1 */
                        for(b=0;b < no_load_parm[0];b++){
                                c=parmb[b];
#ifdef HOT_STANDBY
                                pmamftx_both_sys(a,c >> 4,c & 0x0f,0);
#else
                                pmamftx(a,c >> 4,c & 0x0f,0);
#endif
                        }
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("...ñ®ê ˘¢™ •ëËç Ùó®˛® ˘ì ë˚§óıê§ëï Òë®§ê");
                        else
#endif
                        str2lcd("UPLOAD STARTED...");
#ifdef HOT_STANDBY
                        f |= (PARM_UP_1 | PARM_UP_2);
#else
                        f |= PARM_UP_1;
#endif
#ifdef OPR_UPLOAD_WITH_TIMER
                        upload_count=UPLOAD_PARM_TICK;
#endif
                        set_stm_for_up_down_load();
                                        /* only leds(),pc_timer_init(),lcds() & */
                                        /* onh() are unmasked */
                        DIS_SPKR;  /* disable tone ringer */
                        oprday++;
                        return;
#endif
                }
                else{
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        }
                        else{
#endif
                                str2lcd("INVALID PARAMETERS FILE.                ");
#ifdef OPERATOR_FARSI
                        }
#endif
                        rtn102_1(pt);
                        return;
                }
        }

#ifdef LIM
        else if(oprday == 1){
                pcpmftx1(NIGHT_INACT,TON,OPR_T);
                pfp[1]=fopen(pfna[1][0],"rb");
                if( (pfp[1] != NULL) && (vpf(pfp[1],1) == OK) ){
                                /* if file is valid,the parameters */
                                /* is in parmb[]. */
                        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                          /* activation feature in command frame */
                                       /* is 0 */
                        pmamftx(a,TON,OPR_T,1);  /* send upload command to system */

#if ( (NO_OPRTR > 1) && (!defined ONLY_THIS_OPRTR_CAN_UPLOAD) )

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("   ¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı ");
                        else
#endif
                        str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...");
                        ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_2;
                        oprday++;
                        return;
#endif


#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
                        a |= FEATURE_ACTIVATION;
                                        /* activation feature is parameters */
                                        /* frame is 1 */
                        for(b=0;b < no_load_parm[1];b++){
                                c=parmb[b];
                                pmamftx(a,c >> 4,c & 0x0f,1);
                        }
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("...ñ®ê ˘¢™ •ëËç Ùó®˛® ˘ì ë˚§óıê§ëï Òë®§ê");
                        else
#endif
                                str2lcd("UPLOAD STARTED...");
                        f |= PARM_UP_2;
                        set_stm_for_up_down_load();
                                        /* only leds(),pc_timer_init(),lcds() & */
                                        /* onh() are unmasked */
                        DIS_SPKR;  /* disable tone ringer */
                        oprday++;
                        return;
#endif
                }
                else{
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        }
                        else{
#endif
                                str2lcd("INVALID PARAMETERS FILE.                ");
#ifdef OPERATOR_FARSI
                        }
#endif
                        rtn102_1(pt);
                        return;
                }
        }

#if(NO_EXTENSION_C != 0)
        else if(oprday == 2){
                pcpmftx2(NIGHT_INACT,TON,OPR_T);
                pfp[2]=fopen(pfna[2][0],"rb");
                if( (pfp[2] != NULL) && (vpf(pfp[2],0) == OK) ){
                                /* if file is valid,the parameters */
                                /* is in parmb[]. */
                        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                          /* activation feature in command frame */
                                       /* is 0 */
                        pmamftx(a,TON,OPR_T,2);  /* send upload command to system */

#if ( (NO_OPRTR > 1) && (!defined ONLY_THIS_OPRTR_CAN_UPLOAD) )

#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("   ¢˛™ëì Ùó®˛® È§Ø •ê ñÍë˛§¢ Ò¯ìÏ §‡ó˜ı ");
                        else
#endif
                                str2lcd("WAIT FOR SYSTEM UPLOAD ACCEPTANCE...");
                        ocr.state=AWAIT_SYS_UPLOAD_ACCEPTANCE_3;
                        oprday++;
                        return;
#endif


#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
                        a |= FEATURE_ACTIVATION;
                                        /* activation feature is parameters */
                                        /* frame is 1 */
                        for(b=0;b < no_load_parm[2];b++){
                                c=parmb[b];
                                pmamftx(a,c >> 4,c & 0x0f,2);
                        }
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("...ñ®ê ˘¢™ •ëËç Ùó®˛® ˘ì ë˚§óıê§ëï Òë®§ê");
                        else
#endif
                                str2lcd("UPLOAD STARTED...");
                        f |= PARM_UP_3;
                        set_stm_for_up_down_load();
                                        /* only leds(),pc_timer_init(),lcds() & */
                                        /* onh() are unmasked */
                        DIS_SPKR;  /* disable tone ringer */
                        oprday++;
                        return;
#endif
                }
                else{
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                        }
                        else{
#endif
                                str2lcd("INVALID PARAMETERS FILE.                ");
#ifdef OPERATOR_FARSI
                        }
#endif
                        rtn102_1(pt);
                        return;
                }
        }
#endif
#endif
        else    return;

}
#endif
//end night upload

#if ((defined DAY_NIGHT_PARAMETERS_FILE) && (!defined NO_NIGHT_UPLOAD))
void rtn101(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r101 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:101 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:101\n");
#endif
                return;
        }
        if(pt.i == 0xff){       // coming from system
            pcpmftx(NIGHT_INACT,TON,OPR_T);
#ifdef LIM
            pcpmftx1(NIGHT_INACT,TON,OPR_T);
#if(NO_EXTENSION_C != 0)
            pcpmftx2(NIGHT_INACT,TON,OPR_T);
#endif
#endif
            return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("            ...ñ®ê Òë„Í §¯óê§ïê í™ ¸Û°ê¢");
        else
#endif
                str2lcd("Do You Want To Upload Night Parameters? ");
                str2lcd("        1:YES      0:NO                 ");
#else
        init_lcd();
        str2lcd("Upload Night Param?");
#endif
        ocr.state=NIGHT_UPLOAD;
}

void rtn102(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r102 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:102 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:102\n");
#endif
                return;
        }

        if(pt.i == 0xff){       // coming from system
                return;
        }
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("            ...ñ®ê Òë„Í §¯óê§ïê í™ ¸Û°ê¢");
        else
#endif
                str2lcd("Do You Want To Upload Day Parameters?   ");
                str2lcd("        1:YES      0:NO                 ");
#else
        init_lcd();
        str2lcd("Upload Day Param?");
#endif
        ocr.state=DAY_UPLOAD;
}

#else

/* rtn101 and rtn102 were like this before "night upload" */
void rtn101(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r101 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:101 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:101\n");
#endif
                return;
        }
        if(pt.i == 0xff){       // coming from system
            pcpmftx(NIGHT_INACT,TON,OPR_T);
#ifdef LIM
            pcpmftx1(NIGHT_INACT,TON,OPR_T);
#if(NO_EXTENSION_C != 0)
            pcpmftx2(NIGHT_INACT,TON,OPR_T);
#endif
#endif
            return;
        }
        pcpmftx(NIGHT_ACT,TON,OPR_T);
#ifdef LIM
        pcpmftx1(NIGHT_ACT,TON,OPR_T);
#if(NO_EXTENSION_C != 0)
        pcpmftx2(NIGHT_ACT,TON,OPR_T);
#endif
#endif

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("            ...ñ®ê Òë„Í §¯óê§ïê í™ ¸Û°ê¢");
        else
#endif
                str2lcd(" OPERATOR NIGHT ACTIVE...  ");
#else
        init_lcd();
        str2lcd("OPR. NIGHT ACT..");
#endif
        ocr.state=OPRSIDE_OPR_NIGHT;
        lpg1 |= NLS_BIT_POS;
}

void rtn102(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r102 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:102 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:102\n");
#endif
                return;
        }
        if(pt.i == 0xff){       // coming from system
                return;
        }

        pcpmftx(NIGHT_INACT,TON,OPR_T);
#ifdef LIM
        pcpmftx1(NIGHT_INACT,TON,OPR_T);
#if(NO_EXTENSION_C != 0)
        pcpmftx2(NIGHT_INACT,TON,OPR_T);
#endif
#endif

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif
        ocr.state=OPR_IDLE;
        lpg1 &= ~NLS_BIT_POS;
        lsg1 &= ~NLS_BIT_POS;
        led_reset(NIGHT_LED);
}
#endif


void rtn103(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r103 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:103 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:103\n");
#endif
                return;
        }

#ifdef DISPLAY_STATUS_ACT
        ds[ pt.i ]=cpq[cpfq].stim & 0xff;
#ifdef NEW_DISPLAY_STATUS
        if(dxsf)
                opr_update(pt.i);
#endif
#endif
}

void rtn104(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r104 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:104 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:104\n");
#endif
                return;
        }
        set_stm_for_display();
        /* only ohh() & kbs_t() is unmasked */

#ifdef OPERATOR_PC
        DIS_SPKR;
        /* kt_outport8(DSP_CR,0x72); */  /* tone ringer off */
#else
        XBYTE[DSP_CR]=0x62; /* tone ringer off */
#endif
        f |= DSF;  /* set display status flag */
#ifdef DISPLAY_STATUS_ACT
        dxsf=SET;
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
#if (OS == DOS)
        textmode(C4350);
#endif
        clear_screen();
#if (NO_TOTAL_EXTENSION > 1000)
//      vpoke(VSG,vad(0,0),(((find_subscriber_no(current_page*1000)%10000)/1000)+'0')|0x700);

        kt_print_char(0,0,(((find_subscriber_no(current_page*1000)%10000)/1000)+'0'),MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
#endif
#endif
#endif


#ifdef DISPLAY_STATUS_ACT
        for(a=0;a<NO_TOTAL_EXTENSION;a++)
                ds[a]=ON_HOOK;
#endif
/*
#ifdef HOT_STANDBY
        pcpmftx_both_sys(X_STATUS_REQ_ACT,TON,OPR_T);
#else
        pcpmftx(X_STATUS_REQ_ACT,TON,OPR_T);
#endif

#ifdef LIM
        pcpmftx1(X_STATUS_REQ_ACT,TON,OPR_T);
#if(NO_EXTENSION_C != 0)
        pcpmftx2(X_STATUS_REQ_ACT,TON,OPR_T);
#endif
#endif
*/
}

#ifdef DISPLAY_EM_STATUS
void rtn104a(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r104a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:104a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:104a\n");
#endif
                return;
        }
        set_stm_for_display();
        /* only ohh() & kbs_t() is unmasked */

#ifdef OPERATOR_PC
        DIS_SPKR;
        /* kt_outport8(DSP_CR,0x72); */  /* tone ringer off */
#else
        XBYTE[DSP_CR]=0x62; /* tone ringer off */
#endif
        f |= DSF;  /* set display status flag */
        dtsf=SET;  /* set display trunk status flag */

#ifdef OPERATOR_PC
//        textmode(C4350);
        clear_screen();
#endif


        disp_trunk_no();
        for(a=0;a<NO_EM_DISPLAY;a++){
                dts[a]=UNKNOWN;
                dtschng[a]=SET;
        }
#ifdef HOT_STANDBY
        pcpmftx_both_sys(EM_STATUS_REQ_ACT,TON,OPR_T);
#else
        pcpmftx(EM_STATUS_REQ_ACT,TON,OPR_T);
#endif
}
#endif

#ifdef DISPLAY_CAS3_STATUS
void rtn104c(INDEX_TYPE pt){
#ifdef DEBUG_ACT
        str2lcd(" r104c ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:104c ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:104c\n");
#endif
                return;
        }
        set_stm_for_display();
        /* only ohh() & kbs_t() is unmasked */
#ifdef OPERATOR_PC
        DIS_SPKR;
        /* kt_outport8(DSP_CR,0x72); */  /* tone ringer off */
#else
        XBYTE[DSP_CR]=0x62; /* tone ringer off */
#endif
        f |= DSF;  /* set display status flag */
        dcsf=SET;  /* set display cas3 status flag */
#ifdef OPERATOR_PC
//      textmode(C4350);
        clear_screen();
#endif
        disp_cas3_no();
/*
#ifdef HOT_STANDBY
        pcpmftx_both_sys(CAS3_STATUS_REQ_ACT,current_cas3_page,OPR_T);
#else
        pcpmftx(CAS3_STATUS_REQ_ACT,current_cas3_page,OPR_T);
#endif
*/
}
#endif

#ifdef DISPLAY_CAS1_STATUS
void rtn104d(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r104d ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:104d ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:104d\n");
#endif
                return;
        }
        set_stm_for_display();
        /* only ohh() & kbs_t() is unmasked */

#ifdef OPERATOR_PC
        DIS_SPKR;
        /* kt_outport8(DSP_CR,0x72); */  /* tone ringer off */
#else
        XBYTE[DSP_CR]=0x62; /* tone ringer off */
#endif
        f |= DSF;  /* set display status flag */
        d1sf=SET;  /* set display cas3 status flag */

#ifdef OPERATOR_PC
        textmode(C4350);
        clear_screen();
#endif


        disp_cas1_no();
        for(a=0;a<NO_CAS1_DISPLAY;a++){
                d1s[a]=UNKNOWN;
                d1schng[a]=SET;
#ifdef DISPLAY_CAS1_LINE_STATUS
                d1sb[a]=UNKNOWN;
                d1sbchng[a]=SET;
#endif
        }
#ifdef HOT_STANDBY
        pcpmftx_both_sys(CAS1_STATUS_REQ_ACT,TON,OPR_T);
#else
        pcpmftx(CAS1_STATUS_REQ_ACT,TON,OPR_T);
#endif
}
#endif

#ifdef SYSTEM_MANUAL_TEST
void rtn104b(INDEX_TYPE pt){
#ifdef DEBUG_ACT
        str2lcd(" r104b ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:104b ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:104b\n");
#endif
                return;
        }
        set_stm_for_display();
        /* only ohh() & kbs_t() is unmasked */
#ifdef OPERATOR_PC
        DIS_SPKR;
        /* kt_outport8(DSP_CR,0x72); */  /* tone ringer off */
#else
        XBYTE[DSP_CR]=0x62; /* tone ringer off */
#endif
        f |= DSF;  /* set display status flag */
        mtf=SET;  /* set manual test flag */
#ifdef OPERATOR_PC
//        textmode(C4350);
        clear_screen();
#endif
        test_menu();
//      pcpmftx(MANUAL_TEST_ACT,TON,OPR_T);
}
#endif


void rtn105(INDEX_TYPE pt){
        BYTE h;

#ifdef DEBUG_ACT
        str2lcd(" r105 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:105 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:105\n");
#endif
                return;
        }
//      stm = 0xffff;  /* unmask all tasks */
        unmask_stm();
        if(f & DSF){  /* if display status flag active */
                f &= ~DSF;
#ifdef DISPLAY_STATUS_ACT
                dxsf=RESET;
#endif

#ifdef OPERATOR_PC
                init_layo();
#endif
                for(h=0;h<NO_HOLD_KEY;h++)
                        if(hldl[h].t != NULL_T)

#ifdef OPERATOR_PC
                                led_set(hkl[h]);  /* hold LED on */
#else
                                led_set(hkl);  /* hold LED on */
#endif
#ifdef HOT_STANDBY
                pcpmftx_both_sys(X_STATUS_REQ_INACT,TON,OPR_T);
#else
                pcpmftx(X_STATUS_REQ_INACT,TON,OPR_T);
#endif
#ifdef LIM
                pcpmftx1(X_STATUS_REQ_INACT,TON,OPR_T);
#if(NO_EXTENSION_C != 0)
                pcpmftx2(X_STATUS_REQ_INACT,TON,OPR_T);
#endif
#endif
                if( (trf & TONE_RNG_ACT) == RESET)  /* tone ringer disable */
                        led_set(TONE_RNGR_LED);
                else
                        led_reset(TONE_RNGR_LED);
        }
}

void rtn105a(INDEX_TYPE pt){
#ifdef DISPLAY_EM_STATUS
        BYTE h;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r105a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:105a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:105a\n");
#endif
                return;
        }
//      stm = 0xffff;  /* unmask all tasks */
        unmask_stm();
#ifdef DISPLAY_EM_STATUS
        if(dtsf){  /* if display status flag active */
                f &= ~DSF;
                dtsf=RESET;

#ifdef OPERATOR_PC
                init_layo();
#endif
                for(h=0;h<NO_HOLD_KEY;h++)
                        if(hldl[h].t != NULL_T)

#ifdef OPERATOR_PC
                                led_set(hkl[h]);  /* hold LED on */
#else
                                led_set(hkl);  /* hold LED on */
#endif
#ifdef HOT_STANDBY
                pcpmftx_both_sys(EM_STATUS_REQ_INACT,TON,OPR_T);
#else
                pcpmftx(EM_STATUS_REQ_INACT,TON,OPR_T);
#endif
                if( (trf & TONE_RNG_ACT) == RESET)  /* tone ringer disable */
                        led_set(TONE_RNGR_LED);
                else
                        led_reset(TONE_RNGR_LED);
        }
#endif
}

void rtn105c(INDEX_TYPE pt){
#ifdef DISPLAY_CAS3_STATUS
        BYTE h;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r105c ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:105c ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:105c\n");
#endif
                return;
        }
//      stm = 0xffff;  /* unmask all tasks */
        unmask_stm();
#ifdef DISPLAY_CAS3_STATUS
        if(dcsf){  /* if display status flag active */
                dcsf=RESET;
                f &= ~DSF;

#ifdef OPERATOR_PC
                init_layo();
#endif
                for(h=0;h<NO_HOLD_KEY;h++)
                        if(hldl[h].t != NULL_T)

#ifdef OPERATOR_PC
                                led_set(hkl[h]);  /* hold LED on */
#else
                                led_set(hkl);  /* hold LED on */
#endif
#ifdef HOT_STANDBY
                pcpmftx_both_sys(CAS3_STATUS_REQ_INACT,TON,OPR_T);
#else
                pcpmftx(CAS3_STATUS_REQ_INACT,TON,OPR_T);
#endif
                if( (trf & TONE_RNG_ACT) == RESET)  /* tone ringer disable */
                        led_set(TONE_RNGR_LED);
                else
                        led_reset(TONE_RNGR_LED);
        }
#endif
}

void rtn105d(INDEX_TYPE pt){
#ifdef DISPLAY_CAS1_STATUS
        BYTE h;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r1051 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:1051 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:1051\n");
#endif
                return;
        }
//      stm = 0xffff;  /* unmask all tasks */
        unmask_stm();
#ifdef DISPLAY_CAS1_STATUS
        if(d1sf){  /* if display status flag active */
                d1sf=RESET;
                f &= ~DSF;

#ifdef OPERATOR_PC
                init_layo();
#endif
                for(h=0;h<NO_HOLD_KEY;h++)
                        if(hldl[h].t != NULL_T)

#ifdef OPERATOR_PC
                                led_set(hkl[h]);  /* hold LED on */
#else
                                led_set(hkl);  /* hold LED on */
#endif
#ifdef HOT_STANDBY
                pcpmftx_both_sys(CAS1_STATUS_REQ_INACT,TON,OPR_T);
#else
                pcpmftx(CAS1_STATUS_REQ_INACT,TON,OPR_T);
#endif
                if( (trf & TONE_RNG_ACT) == RESET)  /* tone ringer disable */
                        led_set(TONE_RNGR_LED);
                else
                        led_reset(TONE_RNGR_LED);
        }
#endif
}

void rtn105b(INDEX_TYPE pt){
#ifdef SYSTEM_MANUAL_TEST
        BYTE h;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r105b ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:105b ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:105b\n");
#endif
                return;
        }
//      stm = 0xffff;  /* unmask all tasks */
        unmask_stm();
#ifdef SYSTEM_MANUAL_TEST
        if(mtf){  /* if display status flag active */
                f &= ~DSF;
                mtf=RESET;

#ifdef OPERATOR_PC
                init_layo();
#endif
                for(h=0;h<NO_HOLD_KEY;h++)
                        if(hldl[h].t != NULL_T)

#ifdef OPERATOR_PC
                                led_set(hkl[h]);  /* hold LED on */
#else
                                led_set(hkl);  /* hold LED on */
#endif
                if( (trf & TONE_RNG_ACT) == RESET)  /* tone ringer disable */
                        led_set(TONE_RNGR_LED);
                else
                        led_reset(TONE_RNGR_LED);
        }
#endif
}


#ifdef DISPLAY_STATUS_ACT

void rtn106(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r106 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:106 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:106\n");
#endif
                return;
        }
        ocr.s=pt;
        ocr.fs &= 0xf0;
        ocr.fs |= INACT_FAC1;
        ocr.state=OPRSIDE_OPR_EXT_RINGING;
}
#endif

#ifdef DISPLAY_STATUS_ACT

void rtn107(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r107 ");
#endif

        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:107 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:107\n");
#endif
                return;
        }
        a=pt.i;
        ocr.s=pt;
        ocr.fs &= 0xf0;
        ocr.fs |= INACT_FAC1;
        ocr.state=OPRSIDE_OPR_EXT_RINGING;
        ocr.d.t=NULL_T;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif


#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("          ˆ¢§¯° Ô˜• Òëü §¢ ");
                int2lcd(hex2bcd(find_subscriber_no(a)));
                str2lcd(" :¸Û°ê¢ ");
        }
        else
#endif
                str2lcd("ext: ");

#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET){
#endif
                int2lcd(hex2bcd(find_subscriber_no(a)));
                str2lcd(" ringing              ");

#ifdef OPERATOR_FARSI
        }
#endif
}
#endif

void rtn108(INDEX_TYPE pt){

#ifndef NO_TIE_INIT
        BYTE n;
#endif


#ifdef DEBUG_ACT
        str2lcd(" r108 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:108 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:108\n");
#endif
                return;
        }

#ifndef NO_TIE_INIT
        n=(cpq[cpfq].stim & 0xff) - TIE_GRP1;
        if(n >= NO_TIET_GROUP)  return;
        pcpmftx(TIE_GROUP_REQ,n,OPR_T);

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("              ");
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
                str2lcd(" :˘¯§ •ê TIE Ø° ¸ëÆëÏó ");
        }
        else
#endif
                str2lcd("TIE trunk req. group:");
#else
        init_lcd();
        str2lcd("TIE GRP");
#endif


#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        n=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,n & 0xbf); /* disable operator's codec */
#endif
#else
        n=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=n & 0xbf;  /* disable operator's codec */
#endif

#endif
/* ndef NO_TIE_INIT */
}

void rtn109(INDEX_TYPE pt){

#ifndef NO_TIE_INIT
        BYTE n;
        int a;
#endif

#ifdef DEBUG_ACT
        str2lcd(" r109 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:109 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:109\n");
#endif
                return;
        }

#ifndef NO_TIE_INIT
        n=(cpq[cpfq].stim & 0xff) - TIE_GRP1;
        if(n >= NO_TIET_GROUP)    return;
        if(ocr.s.t == XST_T && ocr.d.t == NULL_T){
                a=ocr.s.i;
                ocr.fs &= 0xf0;
                ocr.fs |= HOLD_FAC1;
        }
        else
                if( (ocr.d.t==XST_T) && (ocr.s.t==NULL_T) ){
                        a=ocr.d.i;
                        ocr.fs &= 0x0f;
                        ocr.fs |= HOLD_FAC2;
                }
                else    return;
        pcpmftx(MUSIC,a,XST_T);
        hld.i=a;
        hld.t=XST_T;
        pcpmftx(TIE_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("              ");
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
                str2lcd(" :˘¯§ •ê TIE Ø° ¸ëÆëÏó ");
        }
        else
#endif
                str2lcd("TIE trunk req. group:");
#else
        init_lcd();
        str2lcd("TIE GRP");
#endif


#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
#endif
/* ndef NO_TIE_INIT */
}

void rtn110(INDEX_TYPE pt){
        int a;
        BYTE i,t;

#ifdef DEBUG_ACT
        str2lcd(" r110 ");
#endif

        t=pt.t;
        if(t & TRUNK_T) return;
        if(t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:110 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:110,%x %x\n",pt.i,pt.t);
#endif
                return;
        }
        a=pt.i;
        i=0;
        while(i < NO_HOLD_KEY){
                if( (hldl[i].t == XST_T)  &&  (hldl[i].i == a) ){

#ifdef OPERATOR_PC
                        rtn_idx[HOLD][TIME_OUT](pt);
#else
                        rtn54(pt);
                        /* state: HOLD,stimulus: TIME_OUT -> rtn54 */
#endif
                        return;
                }
                ++i;
        }
        /* ext. not in a hold location */
        i=0;
        while(i <= 10){ /* number of park locations */
                if( (pl[i].t == XST_T)  &&  (pl[i].i == a) ){

#ifdef OPERATOR_PC
                        rtn_idx[PARK][TIME_OUT](pt);
#else
                        rtn71(pt);
                        /* state: PARK,stimulus: TIME_OUT -> rtn71 */
#endif
                        return;
                }
                ++i;
        }

#ifdef DEBUG_ACT
        str2lcd(" ERR:110.1 ");
#endif

#ifdef OPERATOR_PC
        fprintf(fer,"ERR:110.1\n");
#endif
}

void rtn111(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r111 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:111 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:111\n");
#endif
                return;
        }

#ifdef OPR_EM_CONNECTION
        if( (ocr.s.t == EM_T) || (ocr.d.t == EM_T) )
                return;
#endif

        if(hld.t != NULL_T){
                pcpmftx(SERIAL_TRANSFER,TON,OPR_T);
                rtp(ocr.s);
                rtp(ocr.d);
        }
        else{
                rtpns(ocr.s);
                rtpns(ocr.d);
        }
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.fs=0xff;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}
void rtn111a(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r111a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:111a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:111a\n");
#endif
                return;
        }

#ifdef OPR_EM_CONNECTION
        if( (ocr.s.t == EM_T) || (ocr.d.t == EM_T) )
                return;
#endif

        if(hld.t != NULL_T){
                pcpmftx(FORCE_RELEASE,TON,OPR_T);
                rtp(ocr.s);
                rtp(ocr.d);
        }
        else{
                rtpns(ocr.s);
                rtpns(ocr.d);
        }
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.fs=0xff;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}


#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
void rtn112(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r112 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:112 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:112\n");
#endif
                return;
        }
        f ^= OPR_LANG_FLAG;  /* toggle operator language flag */
        /* clear LCD queue if needed */
        init_file();
        init_layo();

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif
}
#endif

#endif


#ifdef TRUNK_TRUNK_CONNECTION

void rtn115(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r115 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:115 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:115\n");
#endif
                return;
        }
        ocr.s.t=CO_T;
        ocr.scdc=0;
        ocr.xd=NO_DGT_FOR_CO;
        ocr.state=AWAIT_TRNK_NO;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("          :¢˛éëı˜ ¢§ê¯ ê§ ¸§˙™ Ø° ˘§ëı™ ");
                str2lcd(lcd_space_line);
                str2lcd("     ");
        }
        else
#endif
                str2lcd(" CO trunk,enter trunk no:");
#else
        init_lcd();
        str2lcd("CO trunk no:");
#endif
}
#endif


#ifdef TRUNK_TRUNK_CONNECTION

void rtn116(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r116 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:116 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:116\n");
#endif
                return;
        }
        ocr.s.t=TIE_T;
        ocr.scdc=0;
        ocr.xd=NO_DGT_FOR_TIE;
        ocr.state=AWAIT_TRNK_NO;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("           :¢˛éëı˜ ¢§ê¯ ê§ TIE Ø° ˘§ëı™ ");
                str2lcd(lcd_space_line);
                str2lcd("      ");
        }
        else
#endif
                str2lcd(" TIE trunk,enter trunk no:");
#else
        init_lcd();
        str2lcd("TIE trunk no:");
#endif
}
#endif


#ifdef TRUNK_TRUNK_CONNECTION

void rtn117(INDEX_TYPE pt){
        int n,t;

#ifdef DEBUG_ACT
        str2lcd(" r117 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:117 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:117\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)  /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5=n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < ocr.xd) return;
        n=fd();
        t=ocr.s.t;
        if(t == CO_T){
#if(NO_CO != 0)
                if(n >= NO_CO)
                        n=ERR_CODE;
#endif
        }
        else
                if(t == TIE_T){
#ifndef NO_TIE_INIT
                        if(n >= NO_TIE)
                                n=ERR_CODE;
#endif
                }
                else
                        n=ERR_CODE;

        if(n == ERR_CODE){  /* if destination not valid */

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                        ñ®˛˜ §ìó„ı ¢¨Ïı ");
                else
#endif
                        str2lcd(" INVALID DESTINATION                    ");
#else
                init_lcd();
                str2lcd("INVALID DEST.   ");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }
        pcpmftx(OPRSIDE_OVRD,n,t);
        ocr.s.i=n;
        ocr.scdc=0;
        ocr.state=OPR_TRUNK_OVRD;
}
#endif


#ifdef TRUNK_TRUNK_CONNECTION

void rtn118(INDEX_TYPE pt){
        int n,t;

#ifdef DEBUG_ACT
        str2lcd(" r118 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:118 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:118\n");
#endif
                return;
        }
        n=fd();
        t=ocr.s.t;
        if(t == CO_T){
#if(NO_CO != 0)
                if(n >= NO_CO)
                        n=ERR_CODE;
#endif
        }
        else
                if(t == TIE_T){
#ifndef NO_TIE_INIT
                        if(n >= NO_TIE)
                                n=ERR_CODE;
#endif
                }
                else
                        n=ERR_CODE;

        if(n == ERR_CODE){  /* if destination not valid */

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                        ñ®˛˜ §ìó„ı ¢¨Ïı ");
                else
#endif
                        str2lcd(" INVALID DESTINATION                    ");
#else
                init_lcd();
                str2lcd("INVALID DEST.   ");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }
        pcpmftx(OPRSIDE_OVRD,n,t);
        ocr.s.i=n;
        ocr.scdc=0;
        ocr.state=OPR_TRUNK_OVRD;
}
#endif


#ifdef TRUNK_TRUNK_CONNECTION

void rtn119(INDEX_TYPE pt){
        int a,t;

#ifdef DEBUG_ACT
        str2lcd(" r119 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:119 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:119\n");
#endif
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd(" ñ®ê ˘¢™˜ Ò¯ìÏ ");
        else
#endif
                str2lcd(" REJECT ");
        a=ocr.s.i;
        t=ocr.s.t;
        if(t == CO_T)
#if(NO_CO != 0)
                ctst[a].s=IDLE
#endif
                ;
        else
                if(t == TIE_T)
#ifndef NO_TIE_INIT
                        ttst[a].s=IDLE
#endif
                        ;

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        hld.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        ocr.fs=0xff;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}
#endif

#ifdef TRUNK_TRUNK_CONNECTION

void rtn120(INDEX_TYPE pt){
        int t;

#ifdef DEBUG_ACT
        str2lcd(" r120 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:120 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:120\n");
#endif
                return;
        }
        t=ocr.s.t;
        if(t & TRUNK_T)
                pcpmftx(RLS,ocr.s.i,t);

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                              ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        hld.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        ocr.fs=0xff;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}
#endif


#ifdef TRUNK_TRUNK_CONNECTION

void rtn121(INDEX_TYPE pt){
        int t;

#ifdef DEBUG_ACT
        str2lcd(" r121 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:121 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:121\n");
#endif
                return;
        }
        t=ocr.s.t;
        if(t & TRUNK_T)
                pcpmftx(FORCE_RELEASE,ocr.s.i,t);

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif

#endif
        hld.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.mf=RESET;
        ocr.fs=0xff;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}
#endif


#ifdef TRUNK_TRUNK_CONNECTION

void rtn122(INDEX_TYPE pt){
        int t;

#ifdef DEBUG_ACT
        str2lcd(" r122 ");
#endif
        t=pt.t;
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:122 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:122\n");
#endif
                return;
        }
        if(t == CO_T)
#if(NO_CO != 0)
                ctst[ pt.i ].s=IDLE
#endif
                ;
        else
                if(t == TIE_T)
#ifndef NO_TIE_INIT
                        ttst[ pt.i ].s=IDLE
#endif
                        ;
}
#endif

#ifdef TRUNK_TRUNK_CONNECTION

void rtn123(INDEX_TYPE pt){
        int a,t;

#ifdef DEBUG_ACT
        str2lcd(" r123 ");
#endif
        t=pt.t;
        if( (t & TRUNK_T) == RESET){

#ifdef DEBUG_ACT
                str2lcd(" ERR:123 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:123\n");
#endif
                return;
        }
        a=pt.i;
        if( (ocr.s.t != t) || (ocr.s.i != a) ){
                rtp(ocr.s);
                ocr.s=pt;
        }
        a=hex2bcd(a);

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        if(t == CO_T){
#if(NO_CO != 0)

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("        ");
                        hex2lcd(a + 1);
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(" :¸§˙™ Ø° Î˛§Ø •ê ˘ıÛëÓı ¢§ê¯ ");
                }
                else
#endif
                        str2lcd("override CO trunk:");
#else
                str2lcd("ovrd CO:");
#endif


#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                {
#if (NO_CO >= 100)
                        hex2lcd((a + 1)>>8);
#endif
                        hex2lcd(a + 1);
                        /* for displaying first trunk as 1 not 0 */
                }
#endif
                return;
        }
        if(t == TIE_T){
#ifndef NO_TIE_INIT

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("        ");
                        hex2lcd(a + 1);
                        /* for displaying first trunk as 1 not 0 */
                        str2lcd(" :TIE Ø° Î˛§Ø •ê ˘ıÛëÓı ¢§ê¯ ");
                }
                else
#endif
                        str2lcd("override TIE trunk:");
#else
                str2lcd("ovrd TIE:");
#endif


#ifdef OPERATOR_FARSI
                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                        hex2lcd(a + 1);
                        /* for displaying first trunk as 1 not 0 */
#endif
                return;
        }
}
#endif

void rtn124(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r124 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:124 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:124\n");
#endif
                return;
        }
        sys_func();
}


#ifdef PRE_INSTALL

void rtn125(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r125 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:125 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:125\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                case  5: ocr.d6d5=n; break;
                case  6: ocr.d6d5 |=n; break;
                case  7: ocr.d8d7 =n; break;
                case  8: ocr.d8d7 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < ocr.xd)
                return;
        n=fd();  /* find enter gain in hex */
        if(ocr.s.i == MAC_OPR_RCVR_GAIN){
        /* if operator receiver gain */
                org=n;

#ifdef OPERATOR_PC
#ifdef HDLC_8993
                kt_outport8(base_addr[0] | RGCR,org);
#endif
                flp=fopen(LOCAL_PARAM_FILE,"r+b");
                putc(org,flp);
                fclose(flp);
#else
                XBYTE[RGCR]=org;
#endif
        }
        else{
                ocr.s.t=SET;
                ocr.d.i=n;
                ocr.d.t=OPR_T;
                smaf();  /* send MA frame */
        }

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
                else
#endif
                        str2lcd(" START NEW PROGRAM:");
#else
                init_lcd();
                str2lcd("START NEW PROG.:");
#endif
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;

}
#endif


#ifdef PRE_INSTALL

void rtn126(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r126 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:126 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:126\n");
#endif
                return;
        }
        n=fd();  /* find enter gain in hex */
        if(ocr.s.i == MAC_OPR_RCVR_GAIN){
        /* if operator receiver gain */
                org=n;

#ifdef OPERATOR_PC
#ifdef HDLC_8993
                kt_outport8(base_addr[0] | RGCR,org);
#endif
                flp=fopen(LOCAL_PARAM_FILE,"r+b");
                putc(org,flp);
                fclose(flp);
#else
                XBYTE[RGCR]=org;
#endif
        }
        else{
                ocr.s.t=SET;
                ocr.d.i=n;
                ocr.d.t=OPR_T;
                smaf();  /* send MA frame */
        }

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
                else
#endif
                        str2lcd(" START NEW PROGRAM:");
#else
                init_lcd();
                str2lcd("START NEW PROG.:");
#endif
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;

}
#endif

#ifdef PARAMETER_DOWN_UP_LOAD

#if(NO_OPRTR > 1)
void rtn127(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r127 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:127 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:127\n");
#endif
                return;
        }
        pcpmftx(RLS,TON,OPR_T);  /* send upload release to system */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
        else
#endif
                str2lcd(" START NEW PROG.:");
#else
        init_lcd();
        str2lcd("START NEW PROG.:");
#endif

        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
#endif


#if(NO_OPRTR > 1)
void rtn128(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r128 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:128 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:128\n");
#endif
                return;
        }
        pcpmftx(RLS,TON,OPR_T);  /* send upload release to system */

#ifdef TIME_DATE_DISPLAY
        ddt_ad();
#else

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#else
        init_lcd();
        str2lcd("OPR. READY...");
#endif


#endif
        ocr.state=OPR_READY;
        ocr.scdc=0;
        ocr.s.t=ocr.d.t=NULL_T;
        ocr.mf=RESET;
        lpg1 &= ~PLS_BIT_POS;
        lsg1 &= ~PLS_BIT_POS;
        led_reset(PROG_LED);
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}
#endif


#if(NO_OPRTR > 1)
void rtn129(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r129 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:129 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:129\n");
#endif
                return;
        }
        if( (++ohs) & ON_HOOK){
                pcpmftx(RLS,TON,OPR_T);  /* send upload release to system */
                opoh();
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
        }
}
#endif


#if(NO_OPRTR > 1)
void rtn130_1(INDEX_TYPE pt){
        int a,b;
        unsigned int c;

#ifdef DEBUG_ACT
        str2lcd(" r130 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:130 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:130\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1 | FEATURE_ACTIVATION;
        /* feature activation bit in parameters frames is 1 */

        for(b=0;b < no_load_parm[0];b++){
                c=parmb[b];
#ifdef HOT_STANDBY
                pmamftx_both_sys(a,c >> 4,c & 0x0f,0);
#else
                pmamftx(a,c >> 4,c & 0x0f,0);
#endif
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                 ...Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
        else
#endif
                str2lcd("UPLOAD STARTED...");
#ifdef HOT_STANDBY
        f |= ( PARM_UP_1 | PARM_UP_2 );
#else
        f |= PARM_UP_1;
#endif

#ifdef OPR_UPLOAD_WITH_TIMER
        upload_count=UPLOAD_PARM_TICK;
#endif
        set_stm_for_up_down_load();
        /* only leds(),pc_timer_init(),lcds() & ohh() are unmasked */

        DIS_SPKR;  /* disable tone ringer */

        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
void rtn130_2(INDEX_TYPE pt){
        int a,b;
        unsigned int c;

#ifdef DEBUG_ACT
        str2lcd(" r130 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:130 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:130\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1 | FEATURE_ACTIVATION;
        /* feature activation bit in parameters frames is 1 */

        for(b=0;b < no_load_parm[1];b++){
                c=parmb[b];
                pmamftx(a,c >> 4,c & 0x0f,1);
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                 ...Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
        else
#endif
                str2lcd("UPLOAD STARTED...");
        f |= PARM_UP_2;

        set_stm_for_up_down_load();
        /* only leds(),pc_timer_init(),lcds() & ohh() are unmasked */

        DIS_SPKR;  /* disable tone ringer */

        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
void rtn130_3(INDEX_TYPE pt){
        int a,b;
        unsigned int c;

#ifdef DEBUG_ACT
        str2lcd(" r130 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:130 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:130\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1 | FEATURE_ACTIVATION;
        /* feature activation bit in parameters frames is 1 */

        for(b=0;b < no_load_parm[2];b++){
                c=parmb[b];
                pmamftx(a,c >> 4,c & 0x0f,2);
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                 ...Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
        else
#endif
                str2lcd("UPLOAD STARTED...");
        f |= PARM_UP_3;

        set_stm_for_up_down_load();
        /* only leds(),pc_timer_init(),lcds() & ohh() are unmasked */

        DIS_SPKR;  /* disable tone ringer */

        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
#endif



#if(NO_OPRTR > 1)
void rtn131(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r131 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:131 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:131\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("              ¢¯™˛ı ‚ØÏ ë˚§óıê§ëï Òë®§ê ");
                str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
        }
        else{
#endif
                str2lcd("UPLOAD REJECTED                         ");
                str2lcd("START NEW PROGRAM:");
#ifdef OPERATOR_FARSI
        }
#endif


#else
        init_lcd();
        str2lcd("UPLOAD REJECTED");
        str2lcd("START NEW PROG.:");
#endif

        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
#endif

#endif

#ifdef OPR_EM_CONNECTION
void rtn132(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r132 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:132 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:132\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - EM_GRP1;
        if(n >= NO_EMT_GROUP)  return;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("             ");
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
                str2lcd(" :˘¯§ •ê E & M Ø° ¸ëÆëÏó");
        }
        else
#endif

                str2lcd("EM trunk req. group:");
#else
        init_lcd();
        str2lcd(" EM GRP");
#endif



#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
        pcpmftx(EM_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        n=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,n & 0xbf);  /* disable operator's codec */
#endif
#else
        n=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=n & 0xbf;  /* disable operator's codec */
#endif
}
#endif


#ifdef OPR_EM_CONNECTION
void rtn133(INDEX_TYPE pt){
        BYTE n;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r133 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:133 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:133\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - EM_GRP1;
        if(n >= NO_EMT_GROUP)
                return;
        if( (ocr.s.t == XST_T) && (ocr.d.t == NULL_T) ){
                a=ocr.s.i;
                ocr.fs &= 0xf0;
                ocr.fs |= HOLD_FAC1;
        }
        else
                if( (ocr.d.t == XST_T) && (ocr.s.t == NULL_T) ){
                        a=ocr.d.i;
                        ocr.fs &= 0x0f;
                        ocr.fs |= HOLD_FAC2;
                }
                else    return;
        pcpmftx(MUSIC,a,XST_T);
        hld.i=a;
        hld.t=XST_T;
        pcpmftx(EM_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("             ");
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
                str2lcd(" :˘¯§ •ê E & M Ø° ¸ëÆëÏó");
        }
        else
#endif
                str2lcd("EM trunk req. group:");
#else
        init_lcd();
        str2lcd(" EM GRP");
#endif


#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
}
#endif

/*
#ifdef OPR_EM_CONNECTION
void rtn134(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r134 ");
#endif

        if(pt.t != EM_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:134 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:134\n");
#endif
                return;
        }
        a=pt.i;
        if( (ocr.s.t == EM_T) && (ocr.s.i == a) )
                ocr.s.t=NULL_T;
        else
                if( (ocr.d.t == EM_T) && (ocr.d.i == a) )
                        ocr.d.t=NULL_T;
                else
                        return;

        emtst[a].s=IDLE;

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("  ñ®ê ˘¢™˜ Ò¯ìÏ");
        else
#endif
                str2lcd(" EM trunk released ");

        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.fs=0xff;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                     ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
        else
                ocr.mf &= ~DIAL_ACTIVE;
}
#endif
*/
#if ( ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) ) || ((defined LIM) || (defined OPR_DT_CONNECTION))  || (defined OPR_PRI_CONNECTION))
void rtn135(INDEX_TYPE pt){
        unsigned int a;
        unsigned int i=(unsigned int)(-1);
        BYTE t;

#ifdef DEBUG_ACT
        str2lcd(" r135 ");
#endif

        t=pt.t;
        if( (t != EM_T)
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                         && (t != CAS3_T)
#endif
#ifdef OPR_PRI_CONNECTION
                          && (t != XST_T)       // it is in the middle of dialling
#endif
        ){

#ifdef DEBUG_ACT
                str2lcd(" ERR:135 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:135\n");
#endif
                return;
        }
        a=pt.i;

#if ( (defined EM_INSTALLED) && (defined OPR_EM_CONNECTION) )
        if(t==EM_T){
                i=emtst[a].i;
                emtst[a].s=IDLE;
        }
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        if(t==CAS3_T){
                i=dtcst[a].i;
                dtcst[a].s=IDLE;
        }
#endif
#ifdef OPR_PRI_CONNECTION
        if(t == XST_T){
                i=xst[a].i;
                rtp(pt);
        }
#endif
        if(i < NO_HOLD_KEY){
                hldl[i].t=NULL_T;
#ifdef OPERATOR_PC
                led_reset(hkl[i]);  /* hold LED off */
#else
                led_reset(hkl);  /* hold LED off */
#endif
        }
}
#endif



#if ( ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) ) || ((defined LIM) || (defined OPR_DT_CONNECTION)) )
void rtn136(INDEX_TYPE pt){
        int a,i;
        BYTE t;

#ifdef DEBUG_ACT
        str2lcd(" r136 ");
#endif

        t=pt.t;
        if( (t != EM_T)
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                         && (t != CAS3_T)
#endif
        ){

#ifdef DEBUG_ACT
                str2lcd(" ERR:136 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:136\n");
#endif
                return;
        }
        a=pt.i;

#if ( (defined EM_INSTALLED) && (defined OPR_EM_CONNECTION) )
        if(t==EM_T){
                i=emtst[a].i;
                emtst[a].s=IDLE;
        }
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        if(t==CAS3_T){
                i=dtcst[a].i;
                dtcst[a].s=IDLE;
        }
#endif

        pl[i].t=NULL_T;
        turn_off_park_led(i);
}
#endif

/*#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )*/
#if ( (defined OPR_EM_CONNECTION) || ((defined LIM) || (defined OPR_DT_CONNECTION)) || (defined OPR_PRI_CONNECTION))
void rtn137(INDEX_TYPE pt){
        int a;
        BYTE t;

#ifdef DEBUG_ACT
        str2lcd(" r137 ");
#endif

        t=pt.t;
        if( (t != EM_T)
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                         && (t != CAS3_T)
#endif
#ifdef OPR_PRI_CONNECTION
                         && (t != XST_T)
#endif

        ){

#ifdef DEBUG_ACT
                str2lcd(" ERR:137 ");
#endif

#ifdef OPERATOR_PC
        fprintf(fer,"ERR:137\n");
#endif
                return;
        }
        a=pt.i;
#if ( (defined EM_INSTALLED) && (defined OPR_EM_CONNECTION) )
        if(t==EM_T){
                emtst[a].s=IDLE;
        }
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        if(t==CAS3_T){
                dtcst[a].s=IDLE;
        }
#endif
#ifdef OPR_PRI_CONNECTION
        if(t == XST_T){
//              xst[a].s=IDLE;
                rtp_oprside(a, XST_T);
        }
#endif

#if ( (defined EM_INSTALLED) && (defined OPR_EM_CONNECTION) )
        if( (ocr.s.i==a) && (ocr.s.t==EM_T) ){
                ocr.s.t=NULL_T;  /* source LED off */
                led_reset(SOURCE_LED);
        }
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        if( (ocr.s.i==a) && (ocr.s.t==CAS3_T) ){
                ocr.s.t=NULL_T;  /* source LED off */
                led_reset(SOURCE_LED);
        }
#endif
#ifdef OPR_PRI_CONNECTION
        if( (ocr.s.i == a) && (ocr.s.t == XST_T) ){
                ocr.s.t=NULL_T;  /* source LED off */
                led_reset(SOURCE_LED);
        }
#endif
        else{
                ocr.d.t=NULL_T;
                led_reset(DEST_LED);  /* destination LED off */
        }
        if( (ocr.s.t == NULL_T) && (ocr.d.t == NULL_T) ){
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.fs=0xff;
                ocr.mf=RESET;
                hld.t=NULL_T;
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
        }
}
#endif

#ifdef OPERATOR_PC
void rtn138(void){
#if (OS == DOS)
        char *dospath;
        if( (dospath=searchpath("command.com")) == NULL) return;
        save_screen();
/*      clrscr();*/
        clear_screen();
        print_error_report("\nType Exit to return to OPR program ...");
        if (spawnl(P_WAIT,dospath,NULL)==-1)
                print_error_report("\nCommand.com could not be loaded ...");
        getch();
/*      init_layo();*/
        restore_screen();
/* here we should get back screen */
#endif
}
#endif


#ifdef EM_CONT_SEP_OPR_Q
void rtn139(INDEX_TYPE pt){
        int r;
        INDEX_TYPE a;

#ifdef DEBUG_ACT
        str2lcd(" r20a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:20a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:20a\n");
#endif
                return;
        }
        if( (r=fromemq()) == ERR_CODE)       return;
        a=rxqr[r].pt;
        rrxqr(r);
        r=a.i;
        led_set(SOURCE_LED);  /* source LED on */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        switch(a.t){

                case EM_T:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(lcd_3_space);
                                        hex2lcd(hex2bcd(r + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                        str2lcd(":E&M Ø° ê¢ìı");
                                }
                                else
#endif
                                        str2lcd("source E&M trunk:");
#else
                                str2lcd("source E&M:");
#endif


#ifdef OPERATOR_FARSI
                                if( (f & OPR_LANG_FLAG) == RESET)
#endif
                                        hex2lcd(hex2bcd(r + 1));
                                        /* for displaying first trunk as 1 not 0 */
                                emtst[r].s=IN_OCR;
                                pcpmftx(CNNCT_TO_OPR_EM_CONT,r,EM_T);
                                break;

                default:        return;

        }
        ocr.s.i=r;
        ocr.s.t=a.t;
        ocr.fs=ACT_FAC1;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        ocr.state=OPRSIDE_OPR_TALK;
}
#endif

/* ======================================================================*/
/* rtn_idx[AWAIT_CAS3_HST_CH][i]                                         */
/* ======================================================================*/

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void rtn140(INDEX_TYPE pt){
        unsigned int n;

#ifdef DEBUG_ACT
        str2lcd(" r140 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:140 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:140\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < ocr.xd)
                return;
        n=fd();
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

//      ocr.s.t=SET;
//DYNAMIC : NO_CAS3_CHANNEL --> no_cas3_channel[0]
        if(n >= no_cas3_channel[0]){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı  Òë˜ëÓ ˘§ëı™ ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID CAS3 CH. NO.               ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID CAS3 CH. NO.");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }
        n--;

        ocr.d.i=n;
        ocr.d.t=OPR_T;

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
        else
#endif
        str2lcd(" 1:act.  0:inact ");
        ocr.state=AWAIT_ACT_INACT;
}
#endif
#endif
/* ======================================================================*/
/* rtn_idx[AWAIT_CAS3_HST_CH][EOD_ID_KEY]                                */
/* ======================================================================*/
#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void rtn141(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r141 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:141 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:141\n");
#endif
                return;
        }
        a=fd();

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

//DYNAMIC : NO_CAS3_CHANNEL -->
        if(a >= no_cas3_channel[0]){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı  Òë˜ëÓ ˘§ëı™ ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID CAS3 CH. NO.               ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID CAS3 CH. NO.");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
        else
#endif
                str2lcd(" 1:act.  0:inact ");
        a--;
        ocr.d.i=a;
        ocr.d.t=OPR_T;
        ocr.state=AWAIT_ACT_INACT;
}
#endif
#endif

/* ======================================================================*/
/* rtn_idx[AWAIT_EM_HST_CH][i]                                   */
/* ======================================================================*/

#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void rtn142(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r142 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:142 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:142\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < ocr.xd)
                return;
        n=fd();
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

//      ocr.s.t=SET;
        if(n >= NO_EM){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı  Òë˜ëÓ ˘§ëı™ ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID E&M CH. NO.               ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID E&M CH. NO.");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }

        n--;
        ocr.d.i=n;
        ocr.d.t=OPR_T;

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
        else
#endif
        str2lcd(" 1:act.  0:inact ");
        ocr.state=AWAIT_ACT_INACT;
}
#endif
#endif
/* ======================================================================*/
/* rtn_idx[AWAIT_EM_HST_CH][EOD_ID_KEY]                          */
/* ======================================================================*/
#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void rtn143(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r143 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:143 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:143\n");
#endif
                return;
        }
        a=fd();

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

        if(a >= NO_EM){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı  Òë˜ëÓ ˘§ëı™ ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID E&M CH. NO.               ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID E&M CH. NO.");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
        else
#endif
                str2lcd(" 1:act.  0:inact ");

        a--;
        ocr.d.i=a;
        ocr.d.t=OPR_T;
        ocr.state=AWAIT_ACT_INACT;
}
#endif
#endif

/* ======================================================================*/
/* rtn_idx[AWAIT_CAS1_HST_CH][i]                                         */
/* ======================================================================*/

#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void rtn140a(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r140a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:140a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:140a\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < ocr.xd)
                return;
        n=fd();
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

//      ocr.s.t=SET;
//DYNAMIC : NO_CAS1_CHANNEL --> no_cas1_channel[0]
        if(n >= no_cas1_channel[0]){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı  Òë˜ëÓ ˘§ëı™ ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID CAS1 CH. NO.               ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID CAS1 CH. NO.");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }
        n--;

        ocr.d.i=n;
        ocr.d.t=OPR_T;

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
        else
#endif
        str2lcd(" 1:act.  0:inact ");
        ocr.state=AWAIT_ACT_INACT;
}
#endif
#endif
/* ======================================================================*/
/* rtn_idx[AWAIT_CAS1_HST_CH][EOD_ID_KEY]                                */
/* ======================================================================*/
#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void rtn141a(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r141a ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:141a ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:141a\n");
#endif
                return;
        }
        a=fd();

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

//DYNAMIC : NO_CAS1_CHANNEL --> no_cas1_channel[0]
        if(a >= no_cas1_channel[0]){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı  Òë˜ëÓ ˘§ëı™ ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID CAS1 CH. NO.               ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID CAS1 CH. NO.");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
        else
#endif
                str2lcd(" 1:act.  0:inact ");
        a--;
        ocr.d.i=a;
        ocr.d.t=OPR_T;
        ocr.state=AWAIT_ACT_INACT;
}
#endif
#endif


/* ========================================================================*/
/* rtn_idx[OPR_IDLE][DEP_ACT]                                              */
/* ========================================================================*/
#ifdef NEW_DEP
void rtn113(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r113 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:113 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:113\n");
#endif
                return;
        }
//      stm = 0xffdd;
        set_stm_for_dep();
        /*  kbs_t() is masked */

#if 0
#ifdef OPERATOR_PC
        DIS_SPKR;
        /* kt_outport8(DSP_CR,0x72); */  /* tone ringer off */
#else
        XBYTE[DSP_CR]=0x62; /* tone ringer off */
#endif

#endif // 0
//      f |= DSF;  /* set display status flag */

#ifdef OPERATOR_PC
//      textmode(C4350);
//      clear_screen();
#endif
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        dep_hndlr_init();
#endif
}
#endif




/*============================================================================*/
/*============================================================================*/
#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef HOT_STANDBY
#if(NO_OPRTR > 1)
void rtn130_1_sys1(INDEX_TYPE pt){
        int a,b;
        unsigned int c;
        BYTE saved_link;

#ifdef DEBUG_ACT
        str2lcd(" r130_1_sys1 ");
#endif
        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:130_1_sys1 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:130_1_sys1\n");
#endif
                return;
        }
/*
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
*/
        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1 | FEATURE_ACTIVATION;
        /* feature activation bit in parameters frames is 1 */

        saved_link=hot_link;
#ifdef LAPB_ON_OPERATOR_SIDE
        hot_link=LAPB_LINK0;
#endif
        for(b=0;b < no_load_parm[0];b++){
                c=parmb[b];
                pmamftx(a,c >> 4,c & 0x0f,0);
        }
        hot_link=saved_link;

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("            ... Ò¯ê Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
        else
#endif
                str2lcd("UPLOAD OF SYSTEM 1 STARTED...           ");
        f |= PARM_UP_1;
        set_stm_for_up_down_load();
        /* only leds(),pc_timer_init(),lcds() & ohh() are unmasked */

        DIS_SPKR;  /* disable tone ringer */

        if ((f & PARM_UP_2) || (upload_reject_flag & PARM_UP_2)){

#ifdef OPR_UPLOAD_WITH_TIMER
                upload_count=UPLOAD_PARM_TICK;
#endif
                ocr.d.t=NULL_T;
                ocr.s.t=NULL_T;
                ocr.mf=PROG_ACT;
                ocr.state=OPR_PROG;
        }
}
#endif
#endif
#endif // PARAMETER_DOWN_UP_LOAD

/*============================================================================*/
/*============================================================================*/
#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef HOT_STANDBY
#if(NO_OPRTR > 1)
void rtn130_1_sys2(INDEX_TYPE pt){
        int a,b;
        unsigned int c;
        BYTE saved_link;

#ifdef DEBUG_ACT
        str2lcd(" r130_1_sys2 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:130_1_sys2 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:130_1_sys2\n");
#endif
                return;
        }
/*
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
*/
        a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1 | FEATURE_ACTIVATION;
        /* feature activation bit in parameters frames is 1 */

        saved_link=hot_link;
#ifdef LAPB_ON_OPERATOR_SIDE
        hot_link=LAPB_LINK1;
#endif
        for(b=0;b < no_load_parm[0];b++){
                c=parmb[b];
                pmamftx(a,c >> 4,c & 0x0f,0);
        }
        hot_link=saved_link;

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("          ...   Ù¯¢ Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
        else
#endif
                str2lcd("UPLOAD OF SYSTEM 2 STARTED...           ");
        f |= PARM_UP_2;

        set_stm_for_up_down_load();
        /* only leds(),pc_timer_init(),lcds() & ohh() are unmasked */

        DIS_SPKR;  /* disable tone ringer */


        if (( f & PARM_UP_1) || (upload_reject_flag & PARM_UP_1)) {

#ifdef OPR_UPLOAD_WITH_TIMER
                upload_count=UPLOAD_PARM_TICK;
#endif
                ocr.d.t=NULL_T;
                ocr.s.t=NULL_T;
                ocr.mf=PROG_ACT;
                ocr.state=OPR_PROG;
        }
}
#endif
#endif
#endif // PARAMETER_DOWN_UP_LOAD
/*============================================================================*/
/*============================================================================*/
#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef HOT_STANDBY
#if(NO_OPRTR > 1)
void rtn131_sys1(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r131_sys1 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:131_sys1 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:131_sys1\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("            ¢¯™˛ı ‚ØÏ Ò¯ê Ùó®˛®  ¸ë˚§óıê§ëï Òë®§ê ");
                if (upload_reject_flag & PARM_UP_2){
                        str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
                }
        }
        else{
#endif
                str2lcd("UPLOAD OF SYSTEM 1 REJECTED.            ");
                if (upload_reject_flag & PARM_UP_2){
                        str2lcd("START NEW PROGRAM:");
                }
#ifdef OPERATOR_FARSI
        }
#endif


#else
        init_lcd();
        str2lcd("UPLOAD OF SYSTEM 1 REJECTED.               ");
        if (upload_reject_flag & PARM_UP_2){
                str2lcd("START NEW PROG.:");
        }
#endif
        upload_reject_flag &= PARM_UP_1;
        if ((f & PARM_UP_2) || (upload_reject_flag & PARM_UP_2)){

                ocr.d.t=NULL_T;
                ocr.s.t=NULL_T;
                ocr.mf=PROG_ACT;
                ocr.state=OPR_PROG;
        }
}
#endif
#endif
#endif // PARAMETER_DOWN_UP_LOAD

/*============================================================================*/
/*============================================================================*/
#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef HOT_STANDBY
#if(NO_OPRTR > 1)
void rtn131_sys2(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r131_sys2 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:131_sys2 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:131_sys2\n");
#endif
                return;
        }

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("            ¢¯™˛ı ‚ØÏ  Ù¯¢ Ùó®˛®  ¸ë˚§óıê§ëï Òë®§ê ");
                if (upload_reject_flag & PARM_UP_2){
                        str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
                }
        }
        else{
#endif
                str2lcd("UPLOAD OF SYSTEM 2 REJECTED.            ");
                if (upload_reject_flag & PARM_UP_1){
                        str2lcd("START NEW PROGRAM:");
                }
#ifdef OPERATOR_FARSI
        }
#endif


#else
        init_lcd();
        str2lcd("UPLOAD OF SYSTEM 2 REJECTED.               ");
        if (upload_reject_flag & PARM_UP_1){
                str2lcd("START NEW PROG.:");
        }
#endif
        upload_reject_flag &= PARM_UP_2;
        if (( f & PARM_UP_1) || (upload_reject_flag & PARM_UP_1)) {

                ocr.d.t=NULL_T;
                ocr.s.t=NULL_T;
                ocr.mf=PROG_ACT;
                ocr.state=OPR_PROG;
        }
}
#endif
#endif
#endif // PARAMETER_DOWN_UP_LOAD


#ifdef OPR_DT_CONNECTION
void rtn144(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r144 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:144 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:144\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - CAS3_GRP1;
        if(n >= NO_CAS3_GROUP_IN_OPR)  return;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("             ");
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
                str2lcd(" :˘¯§ •ê D.T. Ø° ¸ëÆëÏó");
        }
        else
#endif

                str2lcd("DT trunk req. group:");
#else
        init_lcd();
        str2lcd(" DT GRP");
#endif



#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
        pcpmftx(CAS3_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        n=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,n & 0xbf);  /* disable operator's codec */
#endif
#else
        n=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=n & 0xbf;  /* disable operator's codec */
#endif
}
#endif


#ifdef OPR_DT_CONNECTION
void rtn145(INDEX_TYPE pt){
        BYTE n;
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r145 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:145 ");
#endif


#ifdef OPERATOR_PC
                fprintf(fer,"ERR:145\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - CAS3_GRP1;
        if(n >= NO_CAS3_GROUP_IN_OPR)
                return;
        if( (ocr.s.t == XST_T) && (ocr.d.t == NULL_T) ){
                a=ocr.s.i;
                ocr.fs &= 0xf0;
                ocr.fs |= HOLD_FAC1;
        }
        else
                if( (ocr.d.t == XST_T) && (ocr.s.t == NULL_T) ){
                        a=ocr.d.i;
                        ocr.fs &= 0x0f;
                        ocr.fs |= HOLD_FAC2;
                }
                else    return;
        pcpmftx(MUSIC,a,XST_T);
        hld.i=a;
        hld.t=XST_T;
        pcpmftx(CAS3_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("             ");
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
                str2lcd(" :˘¯§ •ê D.T. Ø° ¸ëÆëÏó");
        }
        else
#endif
                str2lcd("DT trunk req. group:");
#else
        init_lcd();
        str2lcd(" DT GRP");
#endif


#ifdef OPERATOR_FARSI
        if( (f & OPR_LANG_FLAG) == RESET)
#endif
                hex2lcd(n+1);
                /* for displaying trunk group # from 1 instead of 0 */
}
#endif
/* ======================================================================*/
/* rtn_idx[AWAIT_SYSTEM_NO][i]                                           */
/* ======================================================================*/
#ifdef PC_COMPATIBLE

void rtn146(INDEX_TYPE pt){
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r146 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:146 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:146\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - N_1 +1;
        if(n > 9)
                n=0;
        tolcdq(h2c(n),TO_LCD_DD);
        if( (++ocr.scdc & 0x01) == RESET)   /* even number of digits */
                n <<= 4;
        switch(ocr.scdc){
                case  1: ocr.d2d1=n; break;
                case  2: ocr.d2d1 |=n; break;
                case  3: ocr.d4d3=n; break;
                case  4: ocr.d4d3 |=n; break;
                default: break;
        }
        if( (ocr.scdc & 0x0f) < ocr.xd)
                return;
        n=fd();
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        ocr.s.t=SET;
        if(n >= 2){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı  Ùó®˛® ˘§ëı™ ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID SYSTEM. NO.               ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif


#else
                str2lcd("INVALID SYSTEM NO.");
                str2lcd("EXIT PROGRAMMING");
#endif


#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }

        ocr.d.i=n;
        ocr.d.t=OPR_T;

        if(smaf() == ERR_CODE){  /* send MA packet to system */

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                !¢™˜ ˘óÍ§˛£ï ¸•˛§˘ıë˜§ì ");
                else
#endif
                        str2lcd(" PROGRAMMING FAILED!                      ");
#else
                str2lcd("PROGRAMMING FAIL");
#endif
        }

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
        else
#endif
                str2lcd(" START NEW PROGRAM:");
#else
        str2lcd("START NEW PROG.:");
#endif

        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
#endif
/* ======================================================================*/
/* rtn_idx[AWAIT_SYSTEM_NO][EOD_ID_KEY]                                  */
/* ======================================================================*/
#ifdef PC_COMPATIBLE
void rtn147(INDEX_TYPE pt){
        int a;

#ifdef DEBUG_ACT
        str2lcd(" r147 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:147 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:147\n");
#endif
                return;
        }
        a=fd();

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        ocr.s.t=SET;
        if(a >= 2){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("               ñ®˛˜ §ìó„ı  Ùó®˛® ˘§ëı™ ");
                        str2lcd("                   ¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì ");
                }
                else{
#endif
                        str2lcd(" INVALID SYSTEM NO.                 ");
                        str2lcd(" EXIT PROGRAMMING");
#ifdef OPERATOR_FARSI
                }
#endif
#else
                str2lcd("INVALID SYSTEM NO.              ");
                str2lcd("EXIT PROGRAMMING");
#endif

#ifdef TIME_DATE_DISPLAY
                ddt_ad();
#else

#ifdef OPERATOR_PC
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                    ...ñ®ê ˘¢ëıç §¯óê§ïê");
                else
#endif
                        str2lcd(" OPERATOR READY...  ");
#else
                init_lcd();
                str2lcd("OPR. READY...");
#endif

#endif
                ocr.state=OPR_READY;
                ocr.scdc=0;
                ocr.mf=RESET;
                ocr.s.t=ocr.d.t=NULL_T;
                lpg1 &= ~PLS_BIT_POS;
                lsg1 &= ~PLS_BIT_POS;
                led_reset(PROG_LED);
                led_reset(SOURCE_LED);
                led_reset(DEST_LED);
                return;
        }

        ocr.d.i=a;
        ocr.d.t=OPR_T;
        if(smaf() == ERR_CODE){  /* send MA packet to system */

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                !¢™˜ ˘óÍ§˛£ï ¸•˛§˘ıë˜§ì ");
                else
#endif
                        str2lcd(" PROGRAMMING FAILED!                    ");
#else
                str2lcd("PROGRAMMING FAIL");
#endif
        }

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
        else
#endif
                str2lcd(" START NEW PROGRAM:");
#else
        str2lcd("START NEW PROG.:");
#endif
        ocr.d.t=NULL_T;
        ocr.s.t=NULL_T;
        ocr.mf=PROG_ACT;
        ocr.state=OPR_PROG;
}
#endif
/* ========================================================================*/
/* rtn_idx[OPR_IDLE][BILLING_ACT]                                      */
/* ========================================================================*/
#ifdef DISPLAY_BILLING
void rtn148(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r148 ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:148 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:148\n");
#endif
                return;
        }
        set_stm_for_display();
        /* only ohh() & kbs_t() is unmasked */

#ifdef OPERATOR_PC
        DIS_SPKR;
        /* kt_outport8(DSP_CR,0x72); */  /* tone ringer off */
#else
        XBYTE[DSP_CR]=0x62; /* tone ringer off */
#endif
        bill_display_flag=SET;
        bill_line=-1;
        scroll_flag=RESET;

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        bill_bg();
#endif
#endif
}
#endif
/* ========================================================================*/
/* ========================================================================*/
#ifdef DISPLAY_BILLING
void rtn105e(INDEX_TYPE pt){

        BYTE h;

#ifdef DEBUG_ACT
        str2lcd(" r105e ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:105e ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:105e\n");
#endif
                return;
        }
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        deletw(bill_w);
        deletw(sync_bill_w);
#endif
        unmask_stm();
        if(bill_display_flag == SET){  /* if display status flag active */
                bill_display_flag=RESET;
#ifdef OPERATOR_PC
                init_layo();
#endif
                for(h=0;h<NO_HOLD_KEY;h++)
                        if(hldl[h].t != NULL_T)
#ifdef OPERATOR_PC
                                led_set(hkl[h]);  /* hold LED on */
#else
                                led_set(hkl);  /* hold LED on */
#endif
                if( (trf & TONE_RNG_ACT) == RESET)  /* tone ringer disable */
                        led_set(TONE_RNGR_LED);
                else
                        led_reset(TONE_RNGR_LED);
        }
}
#endif
/* ========================================================================*/
/* rtn_idx[OPR_IDLE][ALARM_ACT]                                            */
/* ========================================================================*/
#ifdef DISPLAY_ALARM
void rtn148f(INDEX_TYPE pt){

#ifdef DEBUG_ACT
        str2lcd(" r148f ");
#endif

        if(pt.t != OPR_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:148f ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:148f\n");
#endif
                return;
        }
        set_stm_for_display();
        /* only ohh() & kbs_t() is unmasked */

#ifdef OPERATOR_PC
        DIS_SPKR;
        /* kt_outport8(DSP_CR,0x72); */  /* tone ringer off */
#else
        XBYTE[DSP_CR]=0x62; /* tone ringer off */
#endif
        alarm_display_flag=SET;
        alarm_line=-1;
        alarm_scroll_flag=RESET;

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        alarm_bg();
#endif
#endif
}
#endif
/* ========================================================================*/
/* ========================================================================*/
#ifdef DISPLAY_ALARM
void rtn105f(INDEX_TYPE pt){

        BYTE h;

#ifdef DEBUG_ACT
        str2lcd(" r105f ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:105f ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:105f\n");
#endif
                return;
        }
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        deletw(alarm_w);
        deletw(sync_alarm_w);
#endif
        unmask_stm();
        if(alarm_display_flag == SET){  /* if display status flag active */
                alarm_display_flag=RESET;
#ifdef OPERATOR_PC
                init_layo();
#endif
                for(h=0;h<NO_HOLD_KEY;h++)
                        if(hldl[h].t != NULL_T)
#ifdef OPERATOR_PC
                                led_set(hkl[h]);  /* hold LED on */
#else
                                led_set(hkl);  /* hold LED on */
#endif
                if( (trf & TONE_RNG_ACT) == RESET)  /* tone ringer disable */
                        led_set(TONE_RNGR_LED);
                else
                        led_reset(TONE_RNGR_LED);
        }
}
#endif

/* ========================================================================*/
/* rtn_idx[OPR_IDLE][TERM_ACT]                                             */
/* ========================================================================*/
void rtn114(INDEX_TYPE pt){
#ifdef DEBUG_ACT
        str2lcd(" r114 ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:114 ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:114\n");
#endif
                return;
        }
        set_stm_for_term();
        term_flag=SET;
#ifdef OPERATOR_PC
#if (OS == DOS)
        textattr(TEXTATTR_TERM_ATTR);
#endif
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        restore_terminal();
#endif
#endif
}

void rtn149(INDEX_TYPE pt){
        int a,r;
        BYTE n;

#ifdef DEBUG_ACT
        str2lcd(" r149 ");
#endif
        if(pt.t != XST_T){

#ifdef DEBUG_ACT
                str2lcd(" ERR:149 ");
#endif

#ifdef OPERATOR_PC
                fprintf(fer,"ERR:149\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - OPRSIDE_CLID;
        a=pt.i;
        r=xst[a].i;
        if(iqr[r].no_dgt == 0xff){
                iqr[r].no_dgt=0;
//to be asked ?????
#ifdef OPR_PRI_CONNECTION
                xst[a].userside_pri=SET;
                xst[a].og=RESET;
#endif
        }
        iqr[r].src_dgt[iqr[r].no_dgt]=n;
        iqr[r].no_dgt++;
}
/* ============================================================================================================ */
/* rtn_idx[OPR_READY][PRI_GRP1]                                                                                 */
/* ============================================================================================================ */
#ifdef OPR_PRI_CONNECTION
void rtn150(INDEX_TYPE pt){
        BYTE n;
#ifdef DEBUG_ACT
        str2lcd(" r150 ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:150 ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:150\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - PRI_GRP1;
        if(n >= NO_PRI_GROUP_IN_OPR){
                str2lcd(" ERR:1501 ");
                int2lcd(n);str2lcd(" ");
                int2lcd(cpq[cpfq].stim);str2lcd("\r\n");
                return;
        }
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(" : PRI Ø° ¸ëÆëÏó");
        }
        else
#endif

                str2lcd("PRI Dialling:");
#else
        init_lcd();
        str2lcd(" PRI DIAL");
#endif
                /* for displaying trunk group # from 1 instead of 0 */
        pcpmftx(PRI_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;
        ocr.mf |= DIAL_ACTIVE;
#ifdef OPERATOR_PC
#ifdef HDLC_8993
        n=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,n & 0xbf);  /* disable operator's codec */
#endif
#else
        n=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=n & 0xbf;  /* disable operator's codec */
#endif
}
#endif
/* ============================================================================================================ */
/* rtn_idx[OPRSIDE_OPR_TALK][PRI_GRP1]                                                                          */
/* ============================================================================================================ */
#ifdef OPR_PRI_CONNECTION
void rtn151(INDEX_TYPE pt){
        BYTE n;
        int a;
        BYTE t;
#ifdef DEBUG_ACT
        str2lcd(" r151 ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:151 ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:151\n");
#endif
                return;
        }
        n=(cpq[cpfq].stim & 0xff) - PRI_GRP1;
        if(n >= NO_PRI_GROUP_IN_OPR)
                return;
        if( (ocr.s.t == XST_T) && (ocr.d.t == NULL_T) ){
                a=ocr.s.i;
                t=ocr.s.t;
                ocr.fs &= 0xf0;
                ocr.fs |= HOLD_FAC1;
        }
        else
                if( (ocr.d.t == XST_T) && (ocr.s.t == NULL_T) ){
                        a=ocr.d.i;
                        t=ocr.d.t;
                        ocr.fs &= 0x0f;
                        ocr.fs |= HOLD_FAC2;
                }
                else    return;
        if((t == XST_T) && (xst[a].userside_pri == SET)) {
                if((ocr.mf & PRI_EXT_IN_TALK) == RESET)
                        return;
        }
        pcpmftx(MUSIC,a,XST_T);
        hld.i=a;
        hld.t=XST_T;
        pcpmftx(PRI_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;
        ocr.mf |= DIAL_ACTIVE;

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(" : PRI Ø° ¸ëÆëÏó");
        }
        else
#endif
                str2lcd("PRI Dialling:");
#else
        init_lcd();
        str2lcd(" PRI DIAL");
#endif
}
#endif
/* ============================================================================================================ */
/* rtn_idx[IN_EXTRNL_QU][OPRSIDE_CLID+i]                                                                        */
/* rtn_idx[IN_RCL_QU][OPRSIDE_CLID+i]                                                                           */
/* ============================================================================================================ */
void rtn152(INDEX_TYPE pt){
        int a,r;
        BYTE n;
#ifdef DEBUG_ACT
        str2lcd(" r152 ");
#endif
        if(pt.t != CO_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:152 ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:152\n");
#endif
                return;
        }
#if(NO_CO != 0)
        n=(cpq[cpfq].stim & 0xff) - OPRSIDE_CLID;
        a=pt.i;
        r=ctst[a].i;
        if(rxqr[r].no_dgt == 0xff){
                rxqr[r].no_dgt=0;
        }
        rxqr[r].src_dgt[rxqr[r].no_dgt]=n;
        rxqr[r].no_dgt++;
#endif
}
/* ============================================================================================================ */
/* rtn_idx[OPR_READY][SIP_REQ]                                                                                  */
/* ============================================================================================================ */
#ifdef OPR_SIP_CONNECTION
void rtn153(INDEX_TYPE pt){
        BYTE n;
#ifdef DEBUG_ACT
        str2lcd(" r153 ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:153 ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:153\n");
#endif
                return;
        }
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(" : SIP Ø° ¸ëÆëÏó");
        }
        else
#endif
                str2lcd("SIP Dialling:");
#else
        init_lcd();
        str2lcd(" SIP DIAL");
#endif
        n=0;
        pcpmftx(SIP_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;
        ocr.mf |= DIAL_ACTIVE;
#ifdef OPERATOR_PC
#ifdef HDLC_8993
        n=kt_inport8(base_addr[0] | DSP_CR);
        kt_outport8(base_addr[0] | DSP_CR,n & 0xbf);  /* disable operator's codec */
#endif
#else
        n=XBYTE[DSP_CR];
        XBYTE[DSP_CR]=n & 0xbf;  /* disable operator's codec */
#endif
}
#endif
/* ============================================================================================================ */
/* rtn_idx[OPRSIDE_OPR_TALK][SIP_REQ]                                                                           */
/* ============================================================================================================ */
#ifdef OPR_SIP_CONNECTION
void rtn154(INDEX_TYPE pt){
        BYTE n;
        int a;
        BYTE t;
#ifdef DEBUG_ACT
        str2lcd(" r154 ");
#endif
        if(pt.t != OPR_T){
#ifdef DEBUG_ACT
                str2lcd(" ERR:154 ");
#endif
#ifdef OPERATOR_PC
                fprintf(fer,"ERR:154\n");
#endif
                return;
        }
        if( (ocr.s.t == XST_T) && (ocr.d.t == NULL_T) ){
                a=ocr.s.i;
                t=ocr.s.t;
                ocr.fs &= 0xf0;
                ocr.fs |= HOLD_FAC1;
        }
        else
                if( (ocr.d.t == XST_T) && (ocr.s.t == NULL_T) ){
                        a=ocr.d.i;
                        t=ocr.d.t;
                        ocr.fs &= 0x0f;
                        ocr.fs |= HOLD_FAC2;
                }
                else    return;
/*
        if((t == XST_T) && (xst[a].userside_pri == SET)) {
                if((ocr.mf & PRI_EXT_IN_TALK) == RESET)
                        return;
        }
*/
        pcpmftx(MUSIC,a,XST_T);
        hld.i=a;
        hld.t=XST_T;
        n=0;
        pcpmftx(SIP_GROUP_REQ,n,OPR_T);
        ocr.state=AWAIT_TRNK;
        ocr.scdc=0;
        ocr.mf |= DIAL_ACTIVE;
#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd(" : SIP Ø° ¸ëÆëÏó");
        }
        else
#endif
                str2lcd("SIP Dialling:");
#else
        init_lcd();
        str2lcd(" SIP DIAL");
#endif
}
#endif
/* ============================================================================================================ */
/* ============================================================================================================ */
