
#include "omtfp.h"
#include "opr_net.h"

#ifndef NO_CAS3
#define NO_CAS3         0
#endif

#ifndef NO_CAS1
#define NO_CAS1         0
#endif


#ifdef EM_GROUP_PROGRAM
#include "/ec/work/sys/emmacro.h"
#endif

#ifdef PARAMETER_DOWN_UP_LOAD

extern char in[];
extern FILE *pfp[2];
//extern unsigned int parmb[];
extern unsigned short parmb[];
extern int pbs;

#ifdef DAY_NIGHT_PARAMETERS_FILE
extern char *pfna[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE];
#else
extern char *pfna[MAX_NO_OPR_CARD][1];
#endif
//DYNAMIC start
extern int no_cas3[];
extern int no_cas1[];
//DYNAMIC end


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
#if ((defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
int emsgnd[NO_EM_GROUP];
#endif
/* e&m group send group number digits */
#endif

//DYNAMIC : #if (NO_CAS3 != 0) -->
#ifdef EXTERNAL_CAS3_INSTALLED
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

#ifdef CAS1_INSTALLED
int c1ai[NO_CAS1];
#endif


#ifdef R1_SIGNALINGP
char office_code[7]="021";
#endif


#endif



#ifdef PARAMETER_DOWN_UP_LOAD
void make_param_files(void){

        int i;
        int j;

        init_dpi();
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
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
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
#ifdef TCI
                parmb[j] |= (DTMF_ALLOWED);
#endif
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

//DYNAMIC : #if (NO_CAS3 != 0) -->
#ifdef EXTERNAL_CAS3_INSTALLED
//DYNAMIC : NO_CAS3 --> no_cas3[0]
        for(i=0;i < no_cas3[0];i++){
                parmb[j]=dtai[i];  /* activation int */
                j++;
        }
#endif

//DYNAMIC : to be corrected.
//DYNAMIC : #if ((NO_CAS1 != 0) && (NO_CAS3 == 0)) -->
#if ((defined CAS1_INSTALLED) && (!defined EXTERNAL_CAS3_INSTALLED))
// for the time being we have either NO_CAS3 or NO_CAS1, not both
//DYNAMIC : NO_CAS1 --> no_cas1[0]
        for(i=0;i < no_cas1[0];i++){
                parmb[j]=c1ai[i];  /* activation int */
                j++;
        }
#endif

#if (NO_ACTIVE_OPR != 0)

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

#endif  // (NO_ACTIVE_OPR != 0)

#ifdef HOT_LINE_PROG
        parmb[j]=(DESTINATION_HOT_LINE_0_I << 4) | DESTINATION_HOT_LINE_0_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_1_I << 4) | DESTINATION_HOT_LINE_1_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_2_I << 4) | DESTINATION_HOT_LINE_2_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_3_I << 4) | DESTINATION_HOT_LINE_3_T;
        j++;
#else
        for(i=0;i < NO_HOT_LINE;i++){
                parmb[j]=NULL_T;
                j++;
        }
#endif

#ifdef EM_GROUP_PROGRAM
        for(i=0;i < NO_EM_GROUP;i++){
#if ((defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                parmb[j]=emsgnd[i];
#else
                parmb[j]=0;
#endif
                j++;
        }
#endif

#ifdef CB_INSTALLED
        for(i=0;i < NO_CB;i++){
                parmb[j]= /*INTERNATIONAL |*/ (STD | DTMF_ALLOWED);
                j++;
        }
#endif

#ifdef R1_SIGNALINGP
        parmb[j]=(office_code[1] << 8) | office_code[0];
        j++;
        parmb[j]=(office_code[3] << 8) | office_code[2];
        j++;
        parmb[j]=(office_code[5] << 8) | office_code[4];
        j++;
#endif
#ifdef READ_OLD_PARM_FILE
#include <io.h>
    {
        FILE *fp;
        int a;
        unsigned int parmb2[NO_LOAD_PARM],size,j2;
        fp=fopen("parm1.old","rb");
        if(fp == NULL) goto end;
        fseek(fp,MAX_NAME_LENGHT+2,SEEK_SET);
        size=filelength(fileno(fp));
        size-=MAX_NAME_LENGHT+2;
        size-=2;                    // checksum
        size/=2;
        for(a=0;a < size;a++){
            parmb2[a]=my_getw(fp);
        }
//################################################3
        j=j2=0;
        if(NO_LOAD_PARM <= size) return;
        {
            unsigned int no_old_extension;
#ifdef X_RAILWAY
            if(((NO_LOAD_PARM-size)/5)*5 != (NO_LOAD_PARM-size))    return;
            no_old_extension=NO_EXTENSION-((NO_LOAD_PARM-size)/5);
#else
            if(((NO_LOAD_PARM-size)/4)*4 != (NO_LOAD_PARM-size))    return;
            no_old_extension=NO_EXTENSION-((NO_LOAD_PARM-size)/4);
#endif
            for(i=0;i < no_old_extension;i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
#ifdef X_RAILWAY
                parmb[j]= parmb2[j2];
                j++;j2++;
#endif
                parmb[j]= parmb2[j2];
                j++;j2++;
                parmb[j]= parmb2[j2];
                j++;j2++;
                parmb[j]= parmb2[j2];
                j++;j2++;
            }
        }
        for(;i < NO_EXTENSION;i++){
                j++;
#ifdef X_RAILWAY
                j++;
#endif
                j++;
                j++;
                j++;
        }
#ifdef EM_GROUP_PROGRAM
        for(i=0;i < NO_EM;i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
                parmb[j]= parmb2[j2];
                j++;j2++;
                parmb[j]= parmb2[j2];
                j++;j2++;
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#elif (!(defined NO_TIE_INIT))
        for(i=0;i < NO_TIE;i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#endif

#if (NO_CO != 0)
        for(i=0;i < NO_CO;i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#endif

#ifdef EXTERNAL_CAS3_INSTALLED
        for(i=0;i < no_cas3[0];i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#endif

#if ((defined CAS1_INSTALLED) && (!defined EXTERNAL_CAS3_INSTALLED))
        for(i=0;i < no_cas1[0];i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#endif

#if (NO_ACTIVE_OPR != 0)

#ifdef OPR_NIGHT_PROG

        parmb[j]= parmb2[j2];
        j++;j2++;
        parmb[j]= parmb2[j2];
        j++;j2++;
        parmb[j]= parmb2[j2];
        j++;j2++;
        parmb[j]= parmb2[j2];
        j++;j2++;

#else
        for(i=0;i < NO_OPR_NITX;i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#endif

#endif  // (NO_ACTIVE_OPR != 0)

#ifdef HOT_LINE_PROG
        parmb[j]= parmb2[j2];
        j++;j2++;
        parmb[j]= parmb2[j2];
        j++;j2++;
        parmb[j]= parmb2[j2];
        j++;j2++;
        parmb[j]= parmb2[j2];
        j++;j2++;
#else
        for(i=0;i < NO_HOT_LINE;i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#endif

#ifdef EM_GROUP_PROGRAM
        for(i=0;i < NO_EM_GROUP;i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#endif

#ifdef CB_INSTALLED
        for(i=0;i < NO_CB;i++){
                parmb[j]= parmb2[j2];
                j++;j2++;
        }
#endif

#ifdef R1_SIGNALINGP
        parmb[j]= parmb2[j2];
        j++;j2++;
        parmb[j]= parmb2[j2];
        j++;j2++;
        parmb[j]= parmb2[j2];
        j++;j2++;
#endif
//################################################3
    }
end:
#endif

#ifdef DAY_NIGHT_PARAMETERS_FILE
                pfp[0]=fopen(pfna[0][0],"rb");
                if( (pfp[0] == NULL) || (vpf(pfp[0],0) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[0][0],0);  /* creat parameters file */

                pfp[0]=fopen(pfna[0][1],"rb");
                if( (pfp[0] == NULL) || (vpf(pfp[0],0) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[0][1],0); /* creat parameters file */
#else
                pfp[0]=fopen(pfna[0][0],"rb");
                if( (pfp[0] == NULL) || (vpf(pfp[0],0) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[0][0],0); /* creat parameters file */
#endif

#undef NO_LOAD_PARM
#undef NO_EXTENSION
#undef CTG_DEFAULT
//#undef EM_GROUP_DEFAULT  // em_group_default can not be different for limb
#undef TTG_DEFAULT
#undef CST_DEFAULT
#undef AST_DEFAULT
#undef NO_EM
#undef EM_CONT_RESOLVED_TIMER_DEFAULT
#undef EM_CONT_RESOLVED_TIMER_FOR_SEND_DEFAULT
#undef NO_TIE
#undef NO_CO
#undef OPR_NIGHT_0
#undef OPR_NIGHT_1
#undef OPR_NIGHT_2
#undef OPR_NIGHT_3
#undef DESTINATION_HOT_LINE_0_I
#undef DESTINATION_HOT_LINE_1_I
#undef DESTINATION_HOT_LINE_2_I
#undef DESTINATION_HOT_LINE_3_I
#undef NO_EM_GROUP
#undef NO_LINE_CARD_IN_SYS
#undef LINE_CARD_START_SLOT
#undef SELF_CENTER
#undef FIRST_DIRECTORY_NUM

#undef SELF_CENTER_GROUP_NUMBER
#undef SELF_CENTER_GROUP_NUMBER_D2D1
#undef NO_ANALOG_CARD
#undef NO_ANALOG_TRUNK
/* 16/9/76: following undef is very important, otherwise, the rest
   of code until end of this file will take the "EM_INSTALLED" in corresponding
   "a" file and erroneous results will happen.
    We should also take care of other similar cases (to be verified later)
*/
#undef EM_INSTALLED
#undef NO_EM
//#undef EM_GROUP_PROGRAM    // em_group_program should also be considered as defined in limb to do properly for xemtg
                           // also we should NOT undef X_RAILWAY
#undef DTMF_CARD_START_SLOT
#undef R1_SIGNALINGP
#undef NO_LOAD_PARM1
#undef GLOBAL_NIGHT_EXT

#undef NO_DIGITAL_TRUNK
#undef NO_CAS3_DIGITAL_TRUNK
#undef NO_CAS3_GROUP
#undef NO_CAS3_IC_CH
#undef NO_CAS3_OG_CH
#undef LIM_OGT_GROUP
#undef LIM2_OGT_GROUP
#undef ANALOG_CARD_START_SLOT
#undef NO_CAS3
#undef NO_DT_GROUP
#undef CAS1_INSTALLED
#undef NO_CAS1
#undef NO_CAS1_DIGITAL_TRUNK
#undef NO_CAS1_GROUP
#undef NO_CAS1_IC_CH
#undef NO_CAS1_OG_CH
#undef CONFERENCE_ST_BUS

#undef CB_INSTALLED
#undef NO_CB

#ifdef LIM


#include <generalb.h>

#ifndef NO_EM
#define NO_EM           0
#endif

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
#ifdef TCI
                parmb[j] |= (DTMF_ALLOWED);
#endif
                j++;
                parmb[j]=AST_DEFAULT;
                j++;
                parmb[j]=hli[i];
                j++;
        }

#include "cur_xpc.c"
/* include current extensions parameters change */

#if ((defined EM_GROUP_PROGRAM) && (NO_EM != 0))
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

//DYNAMIC : #if (NO_CAS3 != 0) -->
#ifdef EXTERNAL_CAS3_INSTALLED
//DYNAMIC : NO_CAS3 --> no_cas3[1]
        for(i=0;i < no_cas3[1];i++){
                parmb[j]=dtai[i];  /* activation int */
                j++;
        }
#endif

//DYNAMIC : to be corrected.
//DYNAMIC : #if ((NO_CAS1 != 0) && (NO_CAS3 == 0)) -->
#if ((defined CAS1_INSTALLED) && (!defined EXTERNAL_CAS3_INSTALLED))
// for the time being we have either NO_CAS3 or NO_CAS1, not both
//DYNAMIC : NO_CAS1 --> no_cas1[1]
        for(i=0;i < no_cas1[1];i++){
                parmb[j]=c1ai[i];  /* activation int */
                j++;
        }
#endif

#if (NO_ACTIVE_OPR != 0)

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

#endif // (NO_ACTIVE_OPR != 0)

#ifdef HOT_LINE_PROG
        parmb[j]=(DESTINATION_HOT_LINE_0_I << 4) | DESTINATION_HOT_LINE_0_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_1_I << 4) | DESTINATION_HOT_LINE_1_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_2_I << 4) | DESTINATION_HOT_LINE_2_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_3_I << 4) | DESTINATION_HOT_LINE_3_T;
        j++;
#else
        for(i=0;i < NO_HOT_LINE;i++){
                parmb[j]=NULL_T;
                j++;
        }
#endif

#if ((defined EM_GROUP_PROGRAM) && (NO_EM != 0))
        for(i=0;i < NO_EM_GROUP;i++){
#if ((defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                parmb[j]=emsgnd[i];
#else
                parmb[j]=0;
#endif
                j++;
        }
#endif

#ifdef CB_INSTALLED
        for(i=0;i < NO_CB;i++){
                parmb[j]= INTERNATIONAL | STD;
                j++;
        }
#endif

#ifdef R1_SIGNALINGP
        parmb[j]=(office_code[1] << 8) | office_code[0];
        j++;
        parmb[j]=(office_code[3] << 8) | office_code[2];
        j++;
        parmb[j]=(office_code[5] << 8) | office_code[4];
        j++;
#endif



#ifdef DAY_NIGHT_PARAMETERS_FILE
                pfp[1]=fopen(pfna[1][0],"rb");
                if( (pfp[1] == NULL) || (vpf(pfp[1],1) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[1][0],1);  /* creat parameters file */

                pfp[1]=fopen(pfna[1][1],"rb");
                if( (pfp[1] == NULL) || (vpf(pfp[1],1) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[1][1],1); /* creat parameters file */
#else
                pfp[1]=fopen(pfna[1][0],"rb");
                if( (pfp[1] == NULL) || (vpf(pfp[1],1) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[1][0],1); /* creat parameters file */

#endif


#if(NO_EXTENSION_C != 0)

#undef NO_LOAD_PARM
#undef NO_EXTENSION
#undef CTG_DEFAULT
//#undef EM_GROUP_DEFAULT  // em_group_default can not be different for limb
#undef TTG_DEFAULT
#undef CST_DEFAULT
#undef AST_DEFAULT
#undef NO_EM
#undef EM_CONT_RESOLVED_TIMER_DEFAULT
#undef EM_CONT_RESOLVED_TIMER_FOR_SEND_DEFAULT
#undef NO_TIE
#undef NO_CO
#undef OPR_NIGHT_0
#undef OPR_NIGHT_1
#undef OPR_NIGHT_2
#undef OPR_NIGHT_3
#undef DESTINATION_HOT_LINE_0_I
#undef DESTINATION_HOT_LINE_1_I
#undef DESTINATION_HOT_LINE_2_I
#undef DESTINATION_HOT_LINE_3_I
#undef NO_EM_GROUP
#undef NO_LINE_CARD_IN_SYS
#undef LINE_CARD_START_SLOT
#undef SELF_CENTER
#undef FIRST_DIRECTORY_NUM

#undef SELF_CENTER_GROUP_NUMBER
#undef SELF_CENTER_GROUP_NUMBER_D2D1
#undef NO_ANALOG_CARD
#undef NO_ANALOG_TRUNK
/* 16/9/76: following undef is very important, otherwise, the rest
   of code until end of this file will take the "EM_INSTALLED" in corresponding
   "a" file and erroneous results will happen.
    We should also take care of other similar cases (to be verified later)
*/
#undef EM_INSTALLED
#undef NO_EM
//#undef EM_GROUP_PROGRAM    // em_group_program should also be considered as defined in limb to do properly for xemtg
                           // also we should NOT undef X_RAILWAY
#undef DTMF_CARD_START_SLOT
#undef R1_SIGNALINGP
#undef NO_LOAD_PARM1
#undef GLOBAL_NIGHT_EXT

#undef NO_DIGITAL_TRUNK
#undef NO_CAS3_DIGITAL_TRUNK
#undef NO_CAS3_GROUP
#undef NO_CAS3_IC_CH
#undef NO_CAS3_OG_CH
#undef LIM_OGT_GROUP
#undef LIM2_OGT_GROUP
#undef ANALOG_CARD_START_SLOT
#undef NO_CAS3
#undef NO_DT_GROUP
#undef CAS1_INSTALLED
#undef NO_CAS1
#undef NO_CAS1_DIGITAL_TRUNK
#undef NO_CAS1_GROUP
#undef NO_CAS1_IC_CH
#undef NO_CAS1_OG_CH
#undef CONFERENCE_ST_BUS

#undef CB_INSTALLED
#undef NO_CB

#include <generalc.h>

#ifndef NO_EM
#define NO_EM           0
#endif

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
#ifdef TCI
                parmb[j] |= (DTMF_ALLOWED);
#endif
                j++;
                parmb[j]=AST_DEFAULT;
                j++;
                parmb[j]=hli[i];
                j++;
        }

#include "cur_xpc.c"
/* include current extensions parameters change */

#if ((defined EM_GROUP_PROGRAM) && (NO_EM != 0))
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

//DYNAMIC : #if (NO_CAS3 != 0) -->
#ifdef EXTERNAL_CAS3_INSTALLED
//DYNAMIC : NO_CAS3 --> no_cas3[2]
        for(i=0;i < no_cas3[2];i++){
                parmb[j]=dtai[i];  /* activation int */
                j++;
        }
#endif


//DYNAMIC : to be corrected.
//DYNAMIC : #if ((NO_CAS1 != 0) && (NO_CAS3 == 0)) -->
#if ((defined CAS1_INSTALLED) && (!defined EXTERNAL_CAS3_INSTALLED))
// for the time being we have either NO_CAS3 or NO_CAS1, not both
//DYNAMIC : NO_CAS1 --> no_cas1[2]
        for(i=0;i < no_cas1[2];i++){
                parmb[j]=c1ai[i];  /* activation int */
                j++;
        }
#endif

#if (NO_ACTIVE_OPR != 0)

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

#endif // (NO_ACTIVE_OPR != 0)

#ifdef HOT_LINE_PROG
        parmb[j]=(DESTINATION_HOT_LINE_0_I << 4) | DESTINATION_HOT_LINE_0_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_1_I << 4) | DESTINATION_HOT_LINE_1_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_2_I << 4) | DESTINATION_HOT_LINE_2_T;
        j++;
        parmb[j]=(DESTINATION_HOT_LINE_3_I << 4) | DESTINATION_HOT_LINE_3_T;
        j++;
#else
        for(i=0;i < NO_HOT_LINE;i++){
                parmb[j]=NULL_T;
                j++;
        }
#endif

#if ((defined EM_GROUP_PROGRAM) && (NO_EM != 0))
        for(i=0;i < NO_EM_GROUP;i++){
#if ((defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
                parmb[j]=emsgnd[i];
#else
                parmb[j]=0;
#endif
                j++;
        }
#endif

#ifdef CB_INSTALLED
        for(i=0;i < NO_CB;i++){
                parmb[j]= INTERNATIONAL | STD;
                j++;
        }
#endif

#ifdef R1_SIGNALINGP
        parmb[j]=(office_code[1] << 8) | office_code[0];
        j++;
        parmb[j]=(office_code[3] << 8) | office_code[2];
        j++;
        parmb[j]=(office_code[5] << 8) | office_code[4];
        j++;
#endif



#ifdef DAY_NIGHT_PARAMETERS_FILE
                pfp[2]=fopen(pfna[2][0],"rb");
                if( (pfp[2] == NULL) || (vpf(pfp[2],2) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[2][0],2);  /* creat parameters file */

                pfp[2]=fopen(pfna[2][1],"rb");
                if( (pfp[2] == NULL) || (vpf(pfp[2],2) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[2][1],2); /* creat parameters file */
#else
                pfp[2]=fopen(pfna[2][0],"rb");
                if( (pfp[2] == NULL) || (vpf(pfp[2],2) != OK) )
                /* if parameters file does not exists or not valid */
                        cpf(pfna[2][0],2); /* creat parameters file */

#endif

#endif   // (NO_EXTENSION_C != 0)

#endif  // LIM

}
#endif


