
#include "omtfp.h"
#include "opr_net.h"


#ifdef EM_GROUP_PROGRAM
#include <em.h>
#endif

#include "ov.h"



#ifdef PARAMETER_DOWN_UP_LOAD


#ifndef NO_TIE_INIT
int ttdd[NO_TIE];  /* TIE trunk destination DID */
int ttai[NO_TIE];  /* TIE trunk activation int */
#endif


#if (NO_CO != 0)
int ctdd[NO_CO];  /* CO trunk destination DID */
int ctai[NO_CO];  /* CO trunk activation int */
/* activation int: b0-b2=tgn,b3=is,b4=dda,b5=ta */
#endif


#ifdef EM_GROUP_PROGRAM
int emtsp[NO_EM];  /* EM trunk signaling protocol */
int emtai[NO_EM];  /* EM trunk activation int */
/* activation int: b7-b10,or b0-b2=tgn,b3=is,b4=dest.transit,b5=lpbk.test,b6=send_9*/
#ifdef EM_CONTINUOUS_SIGNALING
int emsgnd[NO_EM_GROUP];
#endif
/* e&m group send group number digits */
#endif

#if (NO_CAS3 != 0)
int dtai[NO_CAS3];

#if 0
group # b0-b3: gn;
is      b4: is        /* 1:in service, 0:out of service. */
icc     b5: icc       /* incoming channel */
ogc     b6: ogc       /* outgoing channel */
                      /* for both ways channel icc & ogc are set */
pts     b7: s_pts:1;  /* signaling with PTS */
eos     b8: s_eos:1;  /* signaling with EOS */
mtr     b9: s_metering:1;  /* signaling with METERING pulse */
opr     b10: s_opr_rcl:1;  /* signaling with operator recall */
mal     b11: s_mal_call:1;  /* signaling with malicious call */

imm     b12: imm_ack:1; /* 1:immediate acknowlege should be sent after
                        receiving active edge of seize, otherwise
                        ack sent after complete reception of
                        seize pulse. This bit is only for CAS1
                        ICT  */
wfa     b13: wtfr_ack:1; /* 1:should wait for acknowlege after
                        sending sieze pulse
                        This bit is only for CAS1, OGT */
ids     b14: inter_signal_dgt_send:1;

#endif // 0
#endif

int hli[NO_EXTENSION];  /* hot line index */

#endif

int tolcdq(BYTE d,BYTE w){      /* to LCD queue data d,work w */
        return OK;
}

void str2lcd(char *s){
        if(*s != '\0')
                while(*s != '\0'){
                        tolcdq(*s,TO_LCD_DD);
                        s++;
                }
}




void main(void){

#ifdef PARAMETER_DOWN_UP_LOAD
        int i;
        int j;

#if (CARD_NO == 0)
        no_load_parm[0]=NO_LOAD_PARM;
        valid_parm_file_size[0]=VALID_PARM_FILE_SIZE;
#endif
#if (CARD_NO == 1)
        no_load_parm[1]=NO_LOAD_PARM;
        valid_parm_file_size[1]=VALID_PARM_FILE_SIZE;
#endif



        strcpy(in,INSTALLATION_NAME);

#include "cur_dpi.c"
/* include current default parameters initialization */
        pbs=0;
        j=0;
        for(i=0;i < NO_EXTENSION;i++){


#ifndef CTG_DEFAULT
#define CTG_DEFAULT     0x00
#endif

#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
                parmb[j]= CTG_DEFAULT;
                j++;
                parmb[j]=EM_GROUP_DEFAULT;
#else
#ifdef EM_CONTINUOUS_SIGNALING
                parmb[j]=((EM_GROUP_DEFAULT&0x3f)<< 8) | CTG_DEFAULT | (EM_GROUP_DEFAULT & 0x8000);
#else /* EM_PULSE_SIGNALING */
                parmb[j]= CTG_DEFAULT;
#endif

#endif
#else
                parmb[j]=(TTG_DEFAULT << 8) | CTG_DEFAULT;
#endif
                j++;
                parmb[j]=CST_DEFAULT;
                j++;
                parmb[j]=AST_DEFAULT;
                j++;
                parmb[j]=hli[i];
                j++;
        }

#include "cur_xpc.c"
/* include current extensions parameters change */

#ifdef EM_GROUP_PROGRAM
        for(i=0;i < NO_EM;i++){
                parmb[j]=emtsp[i];  /* DID destination */
                j++;
                parmb[j]=emtai[i];  /* activation int */
                j++;
#ifdef EM_CONTINUOUS_SIGNALING
                parmb[j]=EM_CONT_RESOLVED_TIMER_DEFAULT;
#else /* EM_PULSE_SIGNALING */
                parmb[j]=0;
#endif
                j++;
#ifdef EM_CONTINUOUS_SIGNALING
                parmb[j]=EM_CONT_RESOLVED_TIMER_FOR_SEND_DEFAULT;
#else /* EM_PULSE_SIGNALING */
                parmb[j]=0;
#endif
                j++;
        }
#elif (!(defined NO_TIE_INIT))
        for(i=0;i < NO_TIE;i++){
                parmb[j]=ttdd[i];  /* DID destination */
                j++;
                parmb[j]=ttai[i];  /* activation int */
                j++;
        }
#endif

#if (NO_CO != 0)
        for(i=0;i < NO_CO;i++){
                parmb[j]=ctdd[i];  /* DID destination */
                j++;
                parmb[j]=ctai[i];  /* activation int */
                j++;
        }
#endif

#if (NO_CAS3 != 0)
        for(i=0;i < NO_CAS3;i++){
                parmb[j]=dtai[i];  /* activation int */
                j++;
        }
#endif

#ifdef OPR_NIGHT_PROG

#if(OPR_NIGHT_0 < NO_EXTENSION)
        parmb[j]=( (OPR_NIGHT_0 << 4) | XST_T);
#else
        parmb[j]=NULL_T;
#endif
        j++;

#if(OPR_NIGHT_1 < NO_EXTENSION)
        parmb[j]=( (OPR_NIGHT_1 << 4) | XST_T);
#else
        parmb[j]=NULL_T;
#endif
        j++;

#if(OPR_NIGHT_2 < NO_EXTENSION)
        parmb[j]=( (OPR_NIGHT_2 << 4) | XST_T);
#else
        parmb[j]=NULL_T;
#endif
        j++;

#if(OPR_NIGHT_3 < NO_EXTENSION)
        parmb[j]=( (OPR_NIGHT_3 << 4) | XST_T);
#else
        parmb[j]=NULL_T;
#endif
        j++;

#else
        for(i=0;i < NO_OPR_NITX;i++){
                parmb[j]=NULL_T;
                j++;
        }
#endif

#ifdef HOT_LINE_PROG
        parmb[j]=(DESTINATION_HOT_LINE_0_I << 4) | DESTINATION_HOT_LINE_0_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_1_I << 4) | DESTINATION_HOT_LINE__1_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_2_I << 4) | DESTINATION_HOT_LINE__2_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_3_I << 4) | DESTINATION_HOT_LINE__3_T;
        j++;
#else
        for(i=0;i < NO_HOT_LINE;i++){
                parmb[j]=NULL_T;
                j++;
        }
#endif

#ifdef EM_GROUP_PROGRAM
        for(i=0;i < NO_EM_GROUP;i++){
#ifdef EM_CONTINUOUS_SIGNALING
                parmb[j]=emsgnd[i];
#else
                parmb[j]=0;
#endif
                j++;
        }
#endif

        i=CARD_NO;
#ifdef DAY_NIGHT_PARAMETERS_FILE
                pfp[i]=fopen(pfna[i][0],"rb");
                if( (pfp[i] == NULL) || (vpf(pfp[i],i) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[i][0],i);  /* creat parameters file */

                pfp[i]=fopen(pfna[i][1],"rb");
                if( (pfp[i] == NULL) || (vpf(pfp[i],i) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[i][1],i); /* creat parameters file */
#else
                pfp[i]=fopen(pfna[i][0],"rb");
                if( (pfp[i] == NULL) || (vpf(pfp[i],i) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[i][0],i); /* creat parameters file */
#endif

#endif
}


