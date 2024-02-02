                     /****  edit parameters  ****/

#include "omtfp.h"


#ifdef NEW_DEP
#if (OS == DOS)
#include <conio.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "wg.h"
#include "depmtfp.h"
#ifdef LAPB_ON_OPERATOR_SIDE
#include "/ec/work/sys/lapb.var"
#endif
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
#include "/ec/work/sys/sml.var"
#endif

#include "/ec/work/sys/tcpip/tcpipvar.h"

#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
extern FILE *fp[];
extern EXT_EDIT_LINE xel[];
extern TRUNK_EDIT_LINE tel[];
#ifdef ONLINE_EDIT
extern TRUNK_EDIT_LINE prv_tel[];
#endif
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
extern DIGITAL_TRUNK_EDIT_LINE dtel[];
extern GRP_TYPE no_cas3_programmable_group[];
extern GRP_TYPE no_cas1_group[];

#ifdef ONLINE_EDIT
extern DIGITAL_TRUNK_EDIT_LINE prv_dtel[];  /* digital trunk edit line */
#endif
#endif
extern char xtgm[],cstm[],astm[],hlim[],ddm[];
extern char xtgm7[];
extern char xtgm6[];
extern char xemtgm[];
extern char nel[][11];
#ifdef ONLINE_EDIT
extern long nelv[];
extern long prv_nelv[];
#endif
extern HOT_LINE_EDIT_LINE hlel[];
#ifdef ONLINE_EDIT
extern HOT_LINE_EDIT_LINE prv_hlel[];
#endif
#ifdef AUTO_UPLOAD
extern BYTE file_changed;
#endif
#ifdef EM_GROUP_PROGRAM
extern char emgpel[][3];
#ifdef ONLINE_EDIT
extern unsigned int emgpelv[];
extern unsigned int prv_emgpelv[];
#endif
#endif

extern int fdb;

extern int no_extension[];
extern int no_my_extension[];
extern int no_cb[];
extern int no_co[];
extern int no_tie[];
extern int no_em[];
extern int no_cas3[];
extern int no_cas1[];
extern int no_em_group[];
extern BYTE ep_flag;
extern WINDOW *saved_win;
extern WINDOW *saved_hwin;
extern long saved_h;
extern int saved_b;
extern int saved_a;
extern unsigned char ep_2nd_menu_flag;
extern unsigned char data_entry_flag;
extern unsigned char saved_n;
extern char ocl[];
extern unsigned long oclv;
extern unsigned long prv_oclv;

#ifdef ONLINE_EDIT
extern EXT_EDIT_LINE prv_xel[];
#endif
extern char srch_ext_no[];
extern unsigned long srch_ext_no_v;

extern BYTE no_intrnl_dgt;
#ifdef CB_INSTALLED
extern unsigned int first_cb_no;
extern unsigned long first_cb_number;
#endif
#ifdef DAY_NIGHT_PARAMETERS_FILE
extern BYTE saved_day_file_no;
#endif

/* ========================================================================= */
/* ========================================================================= */
int ep(void){  /* edit parameters */

        if (ep_2nd_menu_flag == RESET) {
                ep_1st_menu();
                return 'c';
        }
        switch(ep_2nd_menu_flag){
                case 1:
                        kt_exp(saved_n);
                        break;
                case 2:
#if (NO_CO != 0)
                        ectp(saved_n);
#endif
                        break;
#ifdef EM_GROUP_PROGRAM
                case 3:
                        eemtp(saved_n);
                        break;

                case 4:
                        eemgp();
                        break;
#else
                case 3:
#ifndef NO_TIE_INIT
                        ettp(saved_n);
#endif
                        break;

                case 4: break;
#endif
                case 5:
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
                        edtp(saved_n);
#endif
                        break;

                case 6:
#if (NO_ACTIVE_OPR != 0)
                        eonx(0);

#endif
                        break;
                case 7:
#ifdef HOT_LINE_SERVICE_ACTIVE
                        edhl(0);
#endif
                        break;

                case 8:
#if ((defined R1_SIGNALINGP) && (!defined SOFTWARE_SECURITY))
                        e_office_code();
#endif
                        break;

                case 9:
                        break;
                case 10:
#ifdef CB_INSTALLED
                        ecbp(saved_n);
#endif
                        break;
                case 11:
                        ask_ext_no();
                        break;
                case 12:
                        ep_flag=RESET;
                        break;
                default : break;
        }

        if (ep_flag == RESET )
        {
#ifdef AUTO_UPLOAD
                file_changed=SET;
#endif
                if ( ep_2nd_menu_flag != 12)    //if return to main menu,saved_win has already been deleted.
                        deletw(saved_win);
                ep_2nd_menu_flag=RESET;
#ifdef AUTO_UPLOAD
                fseek(fp[0],MAX_NAME_LENGHT-2,SEEK_SET);
                my_putw(file_changed,fp[0]);
#endif
                if(fp[0] != NULL){
                        cfcs(fp[0]);  /* update file check sum */
                        fclose(fp[0]);
                        backup_pfna(0);
                        fp[0]=NULL;
                }
#ifdef LIM
                if(fp[1] != NULL){
                        cfcs(fp[1]);  /* update file check sum */
                        fclose(fp[1]);
                        fp[1]=NULL;
                }
#if(NO_EXTENSION_C != 0)
                if(fp[2] != NULL){
                        cfcs(fp[2]);  /* update file check sum */
                        fclose(fp[2]);
                        fp[2]=NULL;
                }
#endif
#endif
#ifdef DAY_NIGHT_PARAMETERS_FILE
                open_param_files(saved_day_file_no);
#else
                open_param_files(1);
#endif
        }
        return 'c';
}
/* ========================================================================= */
/* ========================================================================= */
void ep_1st_menu_init(void){  /* edit parameters */

        WINDOW *a1;
        a1=demrc();
        saved_win=a1;
}
/* ========================================================================= */
/* ========================================================================= */
int ep_1st_menu(void){  /* edit parameters */
        int b;
        WINDOW *a1;
                if (ep_flag == RESET){
                        a1=demrc();
                        saved_win=a1;
                }
                else{
                        a1=saved_win;
                }
#ifdef EM_GROUP_PROGRAM
                b=get_selection(a1,1,"ECMGDNHOLBAR");
#else
                b=get_selection(a1,1,"ECT~DNHOLBAR");
#endif
                if (b == NO_KEY_PRESSED){
                      return b;
                }

                switch(b){
                        case 1:
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                fdb=0;
                                saved_n=0;
                                exp_init_display(saved_n);
                                exp_part1(saved_n);
                                break;
                        case 2:
#if (NO_CO != 0)
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                fdb=0;
                                saved_n=0;
                                ectp_init_display(saved_n);
                                ectp_part1(saved_n);
#endif
                                break;
#ifdef EM_GROUP_PROGRAM
                        case 3:
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                saved_n=0;
                                eemtp_init_display(saved_n);
                                eemtp_part1(saved_n);
                                break;
                        case 4:
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                eemgp_init_display();
                                break;
#else
                        case 3:
#ifndef NO_TIE_INIT
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                saved_n=0;
                                ettp_init_display(saved_n);
                                ettp_part1(saved_n);
#endif
                                break;

                        case 4: break;
#endif
                        case 5:
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                saved_n=0;
                                edtp_init_display(saved_n);
                                edtp_part1(saved_n);
#endif
                                break;

                        case 6:
#if (NO_ACTIVE_OPR != 0)
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                eonx_init_display(0);

#endif
                                break;
                        case 7:
#ifdef HOT_LINE_SERVICE_ACTIVE
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                edhl_init_display(0);
#endif
                                break;

                        case 8:
#if ((defined R1_SIGNALINGP) && (!defined SOFTWARE_SECURITY))
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                e_office_code_init_display();
#endif
                                break;
                        case 9:
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                deph();
                                break;
                        case 10:
#ifdef CB_INSTALLED
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                first_cb_no=0;
                                saved_n=0;
                                ecbp_init_display(saved_n);
                                ecbp_part1(saved_n);
#endif
                                break;

                        case 11:
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                ask_ext_no_init_display();
                                break;
                        case 12 :
                                deletw(a1);
                                ep_2nd_menu_flag=b;
                                return b;
                        default: break;
                }
#ifdef AUTO_UPLOAD
//              file_changed=SET;
#endif

        return b;
}
/* ========================================================================= */
/* ========================================================================= */
WINDOW *demrc(void){  /* display edit menu & return choice */
        WINDOW *a;
        a=enw(21,6,14,39);
        if(a == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,demrc:error in memory allocation!");
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
        swt(a,"EDIT PARAMETERS");
        draw_window(a);
//DOOD start
//      swucf(a,GREEN);
        swucf(a,KT_GREEN);
//DOOD end
        wprompt(a,1,0,"1-Extension parameters.");
#if (NO_CO!=0)
        wprompt(a,1,1,"2-Co trunk parameters.");
#else
        wprompt(a,1,1,"2-not used.");
#endif
#ifdef EM_GROUP_PROGRAM
        wprompt(a,1,2,"3-e&M trunk parameters.");
        wprompt(a,1,3,"4-e&m Group digits.");
#else
#ifndef NO_TIE_INIT
        wprompt(a,1,2,"3-Tie trunk parameters.");
#else
        wprompt(a,1,2,"3-not used.");
#endif
        wprompt(a,1,3,"4-not used.");
#endif
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
        wprompt(a,1,4,"5-Digital trunk parameters.");
#else
        wprompt(a,1,4,"5-not used.");
#endif
#if (NO_ACTIVE_OPR != 0)
        wprompt(a,1,5,"6-Night extensions.");
#else
        wprompt(a,1,5,"6-not used.");
#endif
#ifdef HOT_LINE_SERVICE_ACTIVE
        wprompt(a,1,6,"7-Hot lines.");
#else
        wprompt(a,1,6,"7-not used.");
#endif
#ifdef R1_SIGNALINGP
#ifdef SOFTWARE_SECURITY
        wprompt(a,1,7,"8-can not change office code.");
#else
        wprompt(a,1,7,"8-Office code.");
#endif
#else
        wprompt(a,1,7,"8-not used.");
#endif
        wprompt(a,1,8,"9-heLp on extensions parameters.");
#ifdef CB_INSTALLED
        wprompt(a,1,9,"10-coin Box lines.");
#else
        wprompt(a,1,9,"10-not used.");
#endif
        wprompt(a,1,10,"11-specific extension pArameters");
        wprompt(a,1,11,"12-Return to main menu.");
        return a;
}
/* ========================================================================= */
/* ========================================================================= */
int kt_exp(BYTE n){  /* edit extensions' parameters */

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        exp_part2(n);
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        saved_b=0;
                        saved_n=0;
                        return code;
                  case PGDN :
                        exp_part2(saved_n);
                        saved_b++;
                        if (saved_b < (no_my_extension[saved_n] / NO_DISPLAY_ON_PAGE)){
                                exp_part1(saved_n);
                                return code;
                        }
                        if (saved_b == (no_my_extension[saved_n] / NO_DISPLAY_ON_PAGE)){
                                if ((no_my_extension[saved_n] % NO_DISPLAY_ON_PAGE) > 0){
                                        deletw(saved_win);
                                        exp_init_display(saved_n);
                                        exp_part1(saved_n);
                                        return code;
                                }
                        }
#ifdef LIM
                /* saved_b > */
                        if ( saved_n == 0){
                                saved_b=0;
                                saved_n=1;
                                deletw(saved_win);
                                exp_init_display(saved_n);
                                exp_part1(saved_n);
                                return code;
                        }
#if(NO_EXTENSION_C != 0)
                        if ( saved_n == 1){
                                saved_b=0;
                                saved_n=2;
                                deletw(saved_win);
                                exp_init_display(saved_n);
                                exp_part1(saved_n);
                                return code;
                        }
#endif
#endif
                        saved_b--;
                        return code;
                  case PGUP :
                                return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}
/* ========================================================================= */
/* ========================================================================= */
void exp_init_display(BYTE n){  /* edit extensions' parameters */

        WINDOW *w;
        int b,s,p;
        unsigned int a;
        int row_offset,no_field;
        unsigned int e,d;
        FIELD *f,*tf;
        int max_dsply;
        s=no_my_extension[n] / NO_DISPLAY_ON_PAGE;
        p=no_my_extension[n] % NO_DISPLAY_ON_PAGE;
        if ( saved_b == 0 ){
                saved_a=fdb;
                fseek(fp[n],MAX_NAME_LENGHT + 2,SEEK_SET);
                fdb+=no_my_extension[n];
        }
        a=saved_a; // current ext. no.
        b=saved_b; // current page no.
        /* extensions directory numbers are used for display */
        if (b != s)
#ifdef X_RAILWAY
                w=enw(0,0,22,80);
#else
                w=enw(0,0,21,80);
#endif
        else{
                if ( p > 0){
#ifdef X_RAILWAY
                        w=enw(0,0,p+6,80);
#else
                        w=enw(0,0,p+5,80);
#endif
                }
                else{
#ifdef X_RAILWAY
                        w=enw(0,0,22,80);
#else
                        w=enw(0,0,21,80);
#endif
                }
        }
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
        if(w == NULL){
                caw();  cs();
                print_error_report("\nDEP,exp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
        swt(w,"EDIT EXTENSIONS PARAMETERS");
        draw_window(w);
#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
        wprompt(w,0,0,"ext.   CO        TIE         E&M                       cst            ast  hli");
        wprompt(w,0,1,"                                           SA  TR  T0  CR  IN  FM  CB  MC  ");
#else
        wprompt(w,0,0,"ext.       CO     E&M                        cst                     ast   hli");
#endif
#else
//      wprompt(w,0,0,"ext.   CO        TIE                         cst                     ast   hli");
        wprompt(w,0,0,"ext.       CO     TIE                        cst                     ast   hli");
#endif

#ifdef X_RAILWAY
        wprompt(w,0,2,"     gp654321 gp654321 gp543210987654321 AA  HL  PU  CF  DD  ST  CP  FA  CG");
        wprompt(w,0,3,"------------------------------------------------------------------------------");
#else
#ifdef BAD_PAYER
//      wprompt(w,0,1,"       gp654321 gp654321 AA B1 HL TR PU B2 CF B3 DD IN ST FM CP CB FA MC CG");
        wprompt(w,0,1,"           654321 654321 AA B1 HL TR PU B2 CF B3 DD IN ST FM CP CB FA MC CG");
#else
//      wprompt(w,0,1,"       gp654321 gp654321 AA SA HL TR PU T0 CF CR DD IN ST FM CP CB FA MC CG");
        wprompt(w,0,1,"           654321 654321 AA SA HL TR PU T0 CF CR DD IN ST FM CP CB FA MC CG");
#endif
        wprompt(w,0,2,"------------------------------------------------------------------------------");
#endif

#ifdef X_RAILWAY
        row_offset=4;
#else
        row_offset=3;
#endif
        /* field allocation */
        idew(w);
        for(d=0;d < max_dsply;d++){
#ifdef X_RAILWAY
                f=enf(w,5,row_offset+d,xtgm,xel[d].ctg,BOOLEAN);
#else
//              f=enf(w,8,row_offset+d,xtgm7,xel[d].ctg,BOOLEAN);
                f=enf(w,11,row_offset+d,xtgm6,xel[d].ctg,BOOLEAN);
#endif
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,exp1:error in memory allocation!");
                        quit(1);
                }
#ifdef X_RAILWAY
                f=enf(w,14,row_offset+d,xtgm,xel[d].ttg,BOOLEAN);
#else
//              f=enf(w,16,row_offset+d,xtgm,xel[d].ttg,BOOLEAN);
                f=enf(w,18,row_offset+d,xtgm6,xel[d].ttg,BOOLEAN);
#endif
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,exp2:error in memory allocation!");
                        quit(1);
                }

#ifdef X_RAILWAY
                f=enf(w,23,row_offset+d,xemtgm,xel[d].emtg,BOOLEAN);
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,exp2a:error in memory allocation!");
                        quit(1);
                }

                f=enf(w,42,row_offset+d,cstm,xel[d].cst,BOOLEAN);
#else
                f=enf(w,26,row_offset+d,cstm,xel[d].cst,BOOLEAN);
#endif
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,exp3:error in memory allocation!");
                        quit(1);
                }
#ifdef X_RAILWAY
                f=enf(w,70,row_offset+d,astm,xel[d].ast,BOOLEAN);
#else
                f=enf(w,68,row_offset+d,astm,xel[d].ast,BOOLEAN);
#endif
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,exp4:error in memory allocation!");
                        quit(1);
                }
                f=enf(w,76,row_offset+d,hlim,xel[d].hli,DECIMAL);
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,exp5:error in memory allocation!");
                        quit(1);
                }
        }
#ifdef X_RAILWAY
        no_field=6;
#else
        no_field=5;
#endif

        /* linking up & down fields */
        for(d=0,f=w->ff;d < ( max_dsply * no_field );d++,f=f->fnxt){
                /* 5 = number of field in a line */

                for(e=0,tf=f;(e < no_field) && (tf != NULL);e++,tf=tf->fnxt);
                /* 5 = number of field in a line */
                f->fdn=tf;

                for(e=no_field,tf=f;e && (tf != NULL);e--,tf=tf->fprv);
                /* 5 = number of field in a line */
                f->fup=tf;
        }
        saved_win=w;
        saved_a=a;
}
/* ========================================================================= */
/* ========================================================================= */
void exp_part1(BYTE n){  /* edit extensions' parameters */
        WINDOW *w;
        int b,c,s,p;
        unsigned int a;
        long h;
        unsigned int e,d;
        char *g;
        int max_dsply;

        b=saved_b;
        w=saved_win;
        a=saved_a;
        s=no_my_extension[n] / NO_DISPLAY_ON_PAGE;
        p=no_my_extension[n] % NO_DISPLAY_ON_PAGE;
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
        caf(w);
#ifdef X_RAILWAY
        wcursor(w,76,3);
#else
        wcursor(w,76,2);
#endif
        h=ftell(fp[n]);
        fseek(fp[n],h,SEEK_SET);
        for(c=0;c < max_dsply;c++,a++){
#ifdef X_RAILWAY
                wprintf(w,"\n%-5.0ld",dep_find_subscriber_no(a));
#else
//              wprintf(w,"\n%-8.0"SDNS_BF,dep_find_subscriber_no(a));
                wprintf(w,"\n%-10.0"SDNS_BF,dep_find_subscriber_no(a));
#endif
                d=my_getw(fp[n]);
#ifdef ONLINE_EDIT
#if (NO_CO != 0)
                prv_xel[c].ctgv=(BYTE)(d&0xff);
                xel[c].ctgv=(BYTE)(d&0xff);
#endif
#endif
                g=xel[c].ctg;
#ifdef NEW_ANALOG_TRUNK_GROUPING
//              *g='-';
//              g++;
                for(e=0x20;e;e >>= 1,g++)
                        if(d & e)
                                *g='|';
                        else
                                *g='-';
#else // NEW_ANALOG_TRUNK_GROUPING
                if(d & 0x80){  /* if CO allowed */
                        if(d & 0x40){  /* CO group allowed */
                                *g='|';
                                g++;
                                for(e=0x20;e;e >>= 1,g++)
                                        if(d & e)
                                                *g='|';
                                        else
                                                *g='-';
                        }
                        else{  /* CO group not allowed */
                                *g='-';
                                g++;
                                for(e=6;e;e--,g++)
                                        if(e == (d & 0x3f) )
                                                *g='|';
                                        else
                                                *g='-';
                        }
                }
                else  /* CO not allowed */
                        strcpy(g,"-------");
#endif // NEW_ANALOG_TRUNK_GROUPING

                d >>= 8;
#ifdef ONLINE_EDIT
#if (  (!defined NO_TIE_INIT) || (defined EM_GROUP_PROGRAM) )
                prv_xel[c].ttgv=(BYTE)(d&0xff);
                xel[c].ttgv=(BYTE)(d&0xff);
#endif
#endif
                g=xel[c].ttg;
#ifdef NEW_ANALOG_TRUNK_GROUPING
//              *g='-';
//              g++;
                for(e=0x20;e;e >>= 1,g++)
                        if(d & e)
                                *g='|';
                        else
                                *g='-';
#else // NEW_ANALOG_TRUNK_GROUPING
                if(d & 0x80){  /* if TIE allowed */
                        if(d & 0x40){  /* TIE group allowed */
                                *g='|';
                                g++;
                                for(e=0x20;e;e >>= 1,g++)
                                        if(d & e)
                                                *g='|';
                                        else
                                                *g='-';
                        }
                        else{  /* TIE group not allowed */
                                *g='-';
                                g++;
#if ( (defined EM_GROUP_PROGRAM) && (!defined X_RAILWAY) )
                                for(e=0x20;e;e >>= 1,g++)
                                        if(d & e)
#else
                                for(e=6;e;e--,g++)
                                        if(e == (d & 0x3f) )
#endif
                                                *g='|';
                                        else
                                                *g='-';
                        }
                }
                else  /* TIE not allowed */
                        strcpy(g,"-------");
#endif // NEW_ANALOG_TRUNK_GROUPING
#ifdef X_RAILWAY
                d=my_getw(fp[n]);
#ifdef ONLINE_EDIT
#ifdef EM_CONTINUOUS_SIGNALING
                xel[c].emtgv=d;
                prv_xel[c].emtgv=d;
#endif
#endif
                g=xel[c].emtg;
#ifdef NEW_ANALOG_TRUNK_GROUPING
                *g='-';
#else // NEW_ANALOG_TRUNK_GROUPING
                if(d & 0x8000)   /* if E&M allowed */
                        *g='|';
                else
                        *g='-';
#endif // NEW_ANALOG_TRUNK_GROUPING
                g++;
                for(e=0x4000;e;e >>= 1,g++)
                        if(d & e)
                                *g='|';
                        else
                                *g='-';
#endif
                d=my_getw(fp[n]);
#ifdef ONLINE_EDIT
                prv_xel[c].cstv=d;
#endif
                xel[c].cstv=d;
                g=xel[c].cst;

                if(d & AMPLIFIER_ACCESS)        *g='|';
                else    *g='-';
                g++;

                if(d & SIREN_ACCESS)    *g='|';
                else    *g='-';
                g++;

                if(d & HOT_LINE_ACCESS) *g='|';
                else    *g='-';
                g++;

                if(d & RING_AGAIN_TIET) *g='|';
                else    *g='-';
                g++;

                if(d & CALL_PICKUP_USE) *g='|';
                else    *g='-';
                g++;

                if(d & TIE_0)   *g='|';
                else    *g='-';
                g++;

                if(d & CONFERENCE_SERVICE)      *g='|';
                else    *g='-';
                g++;

                if(d & RING_AGAIN_COT)  *g='|';
                else    *g='-';
                g++;

                if(d & DONT_DIST)       *g='|';
                else    *g='-';
                g++;

                if(d & INTERNATIONAL)   *g='|';
                else    *g='-';
                g++;

                if(d & STD)     *g='|';
                else    *g='-';
                g++;

                if(d & FOLLOW_ME)       *g='|';
                else    *g='-';
                g++;

                if(d & CAMP_ON) *g='|';
                else    *g='-';
                g++;

                if(d & CALL_BACK)       *g='|';
                else    *g='-';

                d=my_getw(fp[n]);
#ifdef ONLINE_EDIT
                prv_xel[c].astv=d;
#endif
                xel[c].astv=d;
                g=xel[c].ast;

                if(d & FEATURE_ACTIVE)  *g='|';
                else    *g='-';
                g++;

                if(d & MALICIOUS_CALL_DTCT)     *g='|';
                else    *g='-';
                g++;

                if(d & CALL_GEN)        *g='|';
                else    *g='-';

                d=my_getw(fp[n]);
#ifdef ONLINE_EDIT
                prv_xel[c].hliv=d;
                xel[c].hliv=d;
#endif
                /* for displaying hot line index 0 as 1 */
                if(d < NO_HOT_LINE)
//DOOD : itoa -> kt_itoa
                        kt_itoa(d+1,xel[c].hli,10);
        }

        data_entry_init(saved_win);
        saved_a=a;
        saved_h=h; // current file position
}
/* ========================================================================= */
/* ========================================================================= */
void exp_part2(BYTE n){  /* edit extensions' parameters */

        int b,c,s,p;
        long h;
        unsigned int e,d;
        char *g;
        int max_dsply;
#ifdef ONLINE_EDIT
        unsigned int ext_indx;
#endif

        b=saved_b;
        h=saved_h;
        s=no_my_extension[n] / NO_DISPLAY_ON_PAGE;
        p=no_my_extension[n] % NO_DISPLAY_ON_PAGE;
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
        fseek(fp[n],h,SEEK_SET);
        for(c=0;c <max_dsply;c++){
                g=xel[c].ctg;
#ifdef NEW_ANALOG_TRUNK_GROUPING
                d=0;
                for(/*g++,*/e=0x20;*g != '\0';g++,e >>= 1)
                        if(*g == '|')
                                d |= e;
#else // NEW_ANALOG_TRUNK_GROUPING
                if(strcmp(g,"-------") == 0)  /* if CO not allowed */
                        d=0;
                else{  /* CO allowed */
                        d=0x80;
                        if(*g == '|'){  /* if CO group allowed */
                                d |= 0x40;
                                for(g++,e=0x20;*g != '\0';g++,e >>= 1)
                                        if(*g == '|')
                                                d |= e;
                        }
                        else{  /* CO group not allowed */
                        /* when CO group is not allowed,user select */
                        /* the only 1 group for the ext.,because */
                        /* user tick the allowed group,I have to */
                        /* convert the ticked group to hex. for */
                        /* saving in ctg[] */
                                for(g++,e=0x20;*g != '\0';g++,e >>= 1)
                                        if(*g == '|'){
                                                for(;e;e >>= 1,d++);
                                                break;
                                        }
                        }
                }
#endif // NEW_ANALOG_TRUNK_GROUPING

#ifdef ONLINE_EDIT
#if (NO_CO != 0)
                xel[c].ctgv=d;
                if ( d != prv_xel[c].ctgv){
                        ext_indx=(saved_a-max_dsply)+c;
                        send_ext_ctgv_pckt(ext_indx,c);
                }
#endif
#endif
                fputc(d,fp[n]);

                g=xel[c].ttg;
#ifdef NEW_ANALOG_TRUNK_GROUPING
                d=0;
                for(/*g++,*/e=0x20;*g != '\0';g++,e >>= 1)
                        if(*g == '|')
                                d |= e;
#else // NEW_ANALOG_TRUNK_GROUPING
                if(strcmp(g,"-------") == 0)  /* if TIE not allowed */
                        d=0;
                else{  /* TIE allowed */
                        d=0x80;
                        if(*g == '|'){  /* if TIE group allowed */
                                d |= 0x40;
                                for(g++,e=0x20;*g != '\0';g++,e >>= 1)
                                        if(*g == '|')
                                                d |= e;
                        }
                        else{  /* TIE group not allowed */
                        /* when TIE group is not allowed,user select */
                        /* the only 1 group for the ext.,because */
                        /* user tick the allowed group,I have to */
                        /* convert the ticked group to hex. for */
                        /* saving in ttg[] */
                                for(g++,e=0x20;*g != '\0';g++,e >>= 1)
                                        if(*g == '|'){
#if ( (defined EM_GROUP_PROGRAM) && (!defined X_RAILWAY) )
                                                d |= e;
#else
                                                for(;e;e >>= 1,d++);
                                                break;
#endif
                                        }
                        }
                }
#endif // NEW_ANALOG_TRUNK_GROUPING

#ifdef ONLINE_EDIT
#if (  (!defined NO_TIE_INIT) || (defined EM_GROUP_PROGRAM) )
                xel[c].ttgv=d;
                if ( d != prv_xel[c].ttgv){
                        ext_indx=(saved_a-max_dsply)+c;
                        send_ext_ttgv_pckt(ext_indx,c);
                }
#endif
#endif
                fputc(d,fp[n]);

#ifdef X_RAILWAY
                g=xel[c].emtg;
#ifdef NEW_ANALOG_TRUNK_GROUPING
                d=0;
#else // NEW_ANALOG_TRUNK_GROUPING
                if(*g == '|')
                        d=0x8000;
                else
                        d=0;
#endif // NEW_ANALOG_TRUNK_GROUPING
                for(g++,e=0x4000;*g != '\0';g++,e >>= 1)
                        if(*g == '|'){
                                d |= e;
                        }
#ifdef ONLINE_EDIT
#ifdef EM_GROUP_PROGRAM
#ifdef EM_GROUP_DIALLED
                xel[c].emtgv=d;
                if ( d != prv_xel[c].ttgv){
                        ext_indx=(saved_a-max_dsply)+c;
                        send_ext_emtgv_pckt(ext_indx,c);
                }
#endif
#endif
#endif
                my_putw(d,fp[n]);
#endif

                d=xel[c].cstv;
                g=xel[c].cst;

                if(*g == '|')   d |= AMPLIFIER_ACCESS;
                else    d &= ~AMPLIFIER_ACCESS;
                g++;

                if(*g == '|')   d |= SIREN_ACCESS;
                else    d &= ~SIREN_ACCESS;
                g++;

                if(*g == '|')   d |= HOT_LINE_ACCESS;
                else    d &= ~HOT_LINE_ACCESS;
                g++;

                if(*g == '|')   d |= RING_AGAIN_TIET;
                else    d &= ~RING_AGAIN_TIET;
                g++;

                if(*g == '|')   d |= CALL_PICKUP_USE;
                else    d &= ~CALL_PICKUP_USE;
                g++;

                if(*g == '|')   d |= TIE_0;
                else    d &= ~TIE_0;
                g++;

                if(*g == '|')   d |= CONFERENCE_SERVICE;
                else    d &= ~CONFERENCE_SERVICE;
                g++;

                if(*g == '|')   d |= RING_AGAIN_COT;
                else    d &= ~RING_AGAIN_COT;
                g++;

                if(*g == '|')   d |= DONT_DIST;
                else    d &= ~DONT_DIST;
                g++;

                if(*g == '|')
                        d |= INTERNATIONAL;
                else    d &= ~INTERNATIONAL;
                g++;

                if(*g == '|')   d |= STD;
                else    d &= ~STD;
                g++;

                if(*g == '|')   d |= FOLLOW_ME;
                else    d &= ~FOLLOW_ME;
                g++;

                if(*g == '|')   d |= CAMP_ON;
                else    d &= ~CAMP_ON;
                g++;

                if(*g == '|')   d |= CALL_BACK;
                else    d &= ~CALL_BACK;

#ifdef ONLINE_EDIT
                xel[c].cstv=d;
                if ( d != prv_xel[c].cstv){
                        ext_indx=(saved_a-max_dsply)+c;
                        send_ext_cstv_pckt(ext_indx,c);
                }
#endif
                my_putw(d,fp[n]);

                d=xel[c].astv;
                g=xel[c].ast;

                if(*g == '|')   d |= FEATURE_ACTIVE;
                else    d &= ~FEATURE_ACTIVE;
                g++;

                if(*g == '|')   d |= MALICIOUS_CALL_DTCT;
                else    d &= ~MALICIOUS_CALL_DTCT;
                g++;

                if(*g == '|')   d |= CALL_GEN;
                else    d &= ~CALL_GEN;

#ifdef ONLINE_EDIT
                xel[c].astv=d;
                if ( d != prv_xel[c].astv){
                        ext_indx=(saved_a-max_dsply)+c;
                        send_ext_astv_pckt(ext_indx,c);
                }
#endif
                my_putw(d,fp[n]);
                d=0;
                sscanf(xel[c].hli,"%d",&d);
                --d;  /* for saving hot line index 1 as 0 */
                if(d >= NO_HOT_LINE)
                        my_putw(0x00ff,fp[n]);
                else{
                        my_putw(d,fp[n]);
#ifdef ONLINE_EDIT
                        xel[c].hliv=d;
#endif
                }
#ifdef ONLINE_EDIT
                if ( xel[c].hliv != prv_xel[c].hliv){
                        ext_indx=(saved_a-max_dsply)+c;
                        send_ext_hliv_pckt(ext_indx,c);
                }
#endif
        }

   return;
}
/* ========================================================================= */
/* ========================================================================= */
#if (NO_CO != 0)
int ectp(BYTE n){  /* edit extensions' parameters */

        int code;
        int p,s;

        s=no_co[n] / NO_DISPLAY_ON_PAGE;
        p=no_co[n] % NO_DISPLAY_ON_PAGE;
        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        ectp_part2(n);
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        saved_b=0;
                        saved_n=0;
                        return code;
                  case PGDN :
                        ectp_part2(n);
                        saved_b++;
                        if (saved_b < s){
                                ectp_part1(n);
                                return code;
                        }
                        if (saved_b == s){
                                if (p > 0){
                                        deletw(saved_win);
                                        ectp_init_display(n);
                                        ectp_part1(n);
                                        return code;
                                }
                        }
#ifdef LIM
                /* saved_b > */
                        if ( saved_n == 0){
                                saved_n=1;
                                if ((no_co[1] / NO_DISPLAY_ON_PAGE) != 0){
                                        saved_b=0;
                                        deletw(saved_win);
                                        ectp_init_display(saved_n);
                                        ectp_part1(saved_n);
                                        return code;
                                }
                        }
#if(NO_EXTENSION_C != 0)
                        if ( saved_n == 1){
                                saved_n=2;
                                if ((no_co[2] / NO_DISPLAY_ON_PAGE) != 0){
                                        saved_b=0;
                                        deletw(saved_win);
                                        ectp_init_display(saved_n);
                                        ectp_part1(saved_n);
                                        return code;
                                }
                        }
#endif
#endif
                        saved_n=n;
                        saved_b--;
                        return code;
                  case PGUP :
                                return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#if (NO_CO != 0)
void ectp_init_display(BYTE n){  /* edit CO trunk parameters */
        WINDOW *w;
        int a,b,c,s,p;
        unsigned int d;
        FIELD *f,*tf;
        int nf;
        int max_dsply;

        s=no_co[n] / NO_DISPLAY_ON_PAGE;
        p=no_co[n] % NO_DISPLAY_ON_PAGE;

        if ( saved_b == 0 ){
#ifdef EM_GROUP_PROGRAM
                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * /*4*/8);
#elif (defined  NO_TIE_INIT)
                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH);
#else
                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4);
#endif
                fseek(fp[n],a,SEEK_SET);
                saved_a=1;
        }
        a=saved_a;
        b=saved_b;
        if(b != s){
                w=enw(0,0,20,80);
        }
        else{
                if (p > 0)
                        w=enw(0,0,p+4,80);
                else
                        w=enw(0,0,20,80);
        }
                if(w == NULL){
                        caw();  cs();
//DOOD : printf->print_error_report
                        print_error_report("\nDEP,ectp0:error in memory allocation!");
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
                swt(w,"EDIT CO TRUNKS PARAMETERS");
                draw_window(w);
#ifdef TOLL_BARRING_ON_COs
                wprompt(w,0,0,"     CO#   toll  DDI        group #   in service   DDI active   timing active");
#else
                wprompt(w,0,0," CO trunk #    dest. DDI    group #   in service   DDI active   timing active");
#endif
                wprompt(w,0,1,"-------------------------------------------------------------------------------");

                /* field allocation */
                idew(w);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++){
#ifdef TOLL_BARRING_ON_COs
                        f=enf(w,13,2+c,"_",&tel[c].toll_access,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ectp0.5:error in memory allocation!");
                                quit(1);
                        }
#endif
                        f=enf(w,17,2+c,ddm,tel[c].ddb,DECIMAL);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ectp1:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,30,2+c,"__",tel[c].gn,DECIMAL);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ectp2:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,43,2+c,"_",&tel[c].is,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ectp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,56,2+c,"_",&tel[c].da,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ectp4:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,70,2+c,"_",&tel[c].ta,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ectp5:error in memory allocation!");
                                quit(1);
                        }
                }
                /* linking up & down fields */
#ifdef TOLL_BARRING_ON_COs
                nf=6;
#else
                nf=5;
#endif
                for(d=0,f=w->ff;d < ( max_dsply * nf );d++,f=f->fnxt){
                        /* nf = number of field in a line */

                        for(c=0,tf=f;(c < nf) && (tf != NULL);c++,tf=tf->fnxt);
                        /* nf = number of field in a line */
                        f->fdn=tf;

                        for(c=nf,tf=f;c && (tf != NULL);c--,tf=tf->fprv);
                        /* nf = number of field in a line */
                        f->fup=tf;
                }
        saved_win=w;
        saved_a=a;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#if (NO_CO != 0)
void ectp_part1(BYTE n){  /* edit CO trunk parameters */
        WINDOW *w;
        int a,b,c,s,p;
        unsigned int d,e;
        long h;
        int max_dsply;

        s=no_co[n] / NO_DISPLAY_ON_PAGE;
        p=no_co[n] % NO_DISPLAY_ON_PAGE;

        b=saved_b;
        w=saved_win;
        a=saved_a;
                caf(w);
                wcursor(w,76,1);
                h=ftell(fp[n]);
                fseek(fp[n],h,SEEK_SET);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++,a++){
                        wprintf(w,"\n     %1.3d\t\t ",a);
                        e=my_getw(fp[n]);  /* destination DDI */
                        d=my_getw(fp[n]);  /* activation int */
                        if(((unsigned int) e < NO_TOTAL_EXTENSION) && (d & DA_MASK))
                                ltoa(dep_find_subscriber_no(e),tel[c].ddb,10);
                        tel[c].ai=d&0x0fff;
#ifdef ONLINE_EDIT
                        tel[c].ddbv=e;
                        prv_tel[c].ai=d;
                        prv_tel[c].ddbv=e;
#endif
//DOOD : itoa -> kt_itoa
                        kt_itoa(((d & 0xf000) >> 12) + 1 ,tel[c].gn,10);
                        /* for displaying trunk group # from 1 */

                        if(d & IS_MASK) tel[c].is='|';
                        else    tel[c].is='-';

                        if(d & DA_MASK) tel[c].da='|';
                        else    tel[c].da='-';

                        if(d & TA_MASK) tel[c].ta='|';
                        else    tel[c].ta='-';

#ifdef TOLL_BARRING_ON_COs
                        if(d & TOLL_ACCESS_MASK) tel[c].toll_access='|';
                        else    tel[c].toll_access='-';
#endif
                }

        data_entry_init(saved_win);
        saved_a=a;
        saved_h=h;
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#if (NO_CO != 0)
void ectp_part2(BYTE n){  /* edit CO trunk parameters */
        int b,c,s,p;
        unsigned int d;
        unsigned long d1;
        long h;
        int max_dsply;
#ifdef ONLINE_EDIT
        BYTE co_indx;
#endif

        b=saved_b;
        h=saved_h;
        s=no_co[n] / NO_DISPLAY_ON_PAGE;
        p=no_co[n] % NO_DISPLAY_ON_PAGE;
                fseek(fp[n],h,SEEK_SET);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++){
                        d1=-1;
                        sscanf(tel[c].ddb,"%ld",&d1);
                        d1 = dep_fcp(d1);
                        if ((d1 == OPR_ERROR)||(tel[c].da == '-')) {
                                my_putw(OPR_ERROR,fp[n]);
                        }
                        else{
                                my_putw((unsigned int)d1,fp[n]);
#ifdef ONLINE_EDIT
                                tel[c].ddbv=(unsigned int)d1;
                                if (tel[c].ddbv  != prv_tel[c].ddbv){
                                        co_indx=(saved_a-max_dsply)+c-1;
                                        send_co_ddbv_pckt(co_indx,c);
                                }
#endif
                        }
                        d=0;
                        sscanf(tel[c].gn,"%d",&d);
                        d--;
                        /* for saving trunk group # from 0 */
                        if (d >= NO_COT_GROUP)
                                d=0;
                        d <<= 12;
                        d |= tel[c].ai;

                        if(tel[c].is == '|')    d |= IS_MASK;
                        else    d &= ~IS_MASK;

                        if(tel[c].da == '|')    d |= DA_MASK;
                        else    d &= ~DA_MASK;

                        if(tel[c].ta == '|')    d |= TA_MASK;
                        else    d &= ~TA_MASK;
#ifdef TOLL_BARRING_ON_COs
                        if(tel[c].toll_access == '|')    d |= TOLL_ACCESS_MASK;
                        else    d &= ~TOLL_ACCESS_MASK;
#endif
#ifdef ONLINE_EDIT

                tel[c].ai=d;
                if ( d != prv_tel[c].ai){
                        co_indx=(saved_a-max_dsply)+c-1;
                        send_co_ai_pckt(co_indx,c);
                }

#endif
                        my_putw(d,fp[n]);
                }
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef EM_GROUP_PROGRAM
int eemtp(BYTE n){  /* edit extensions' parameters */

        int code;
        int p,s;

        s=no_em[n] / NO_DISPLAY_ON_PAGE;
        p=no_em[n] % NO_DISPLAY_ON_PAGE;
        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        eemtp_part2(n);
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        saved_b=0;
                        saved_n=0;
                        return code;
                  case PGDN :
                        eemtp_part2(n);
                        saved_b++;
                        if (saved_b < s){
                                eemtp_part1(n);
                                return code;
                        }
                        if (saved_b == s){
                                if (p > 0){
                                        deletw(saved_win);
                                        eemtp_init_display(n);
                                        eemtp_part1(n);
                                        return code;
                                }
                        }
#ifdef LIM
                /* saved_b > */
                        if ( saved_n == 0){
                                saved_n=1;
                                if ((no_em[1] / NO_DISPLAY_ON_PAGE) != 0){
                                        saved_b=0;
                                        deletw(saved_win);
                                        eemtp_init_display(saved_n);
                                        eemtp_part1(saved_n);
                                        return code;
                                }
                        }
#if(NO_EXTENSION_C != 0)
                        if ( saved_n == 1){
                                saved_n=2;
                                if ((no_em[2] / NO_DISPLAY_ON_PAGE) != 0){
                                        saved_b=0;
                                        deletw(saved_win);
                                        eemtp_init_display(saved_n);
                                        eemtp_part1(saved_n);
                                        return code;
                                }
                        }
#endif
#endif
                        saved_n=n;
                        saved_b--;
                        return code;
                  case PGUP :
                                return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef EM_GROUP_PROGRAM
void eemtp_init_display(BYTE n){  /* edit E&M trunk parameters */
        WINDOW *w;
        int a,b,c,s,p;
        unsigned int d;
        FIELD *f,*tf;
        int max_dsply;

        s=no_em[n] / NO_DISPLAY_ON_PAGE;
        p=no_em[n] % NO_DISPLAY_ON_PAGE;
        if ( saved_b == 0 ){
                fseek(fp[n],MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH),SEEK_SET);
                saved_a=1;  /* for display first trunk is number 1 */
        }
        a=saved_a;
        b=saved_b;
        if(b != s){
                w=enw(0,0,20,80);
        }
        else{
                if (p > 0)
                        w=enw(0,0,p+4,80);
                else
                        w=enw(0,0,20,80);
        }
                if(w == NULL){
                        caw();  cs();
//DOOD : printf->print_error_report
                        print_error_report("\nDEP,eemtp0:error in memory allocation!");
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
                swt(w,"EDIT E&M TRUNKS PARAMETERS");
                draw_window(w);
                wprompt(w,0,0,"E&M trunk #    sig. prot    group #       i.s    s.9   d.t   lpbk   t1   t2  ");
                wprompt(w,0,1,"-------------------------------------------------------------------------------");

                /* field allocation */
                idew(w);
                if (b != s )
                        max_dsply=NO_DISPLAY_ON_PAGE;
                else
                        if ( p > 0)
                                max_dsply=p;
                        else
                                max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++){
                        f=enf(w,17,2+c,hlim,tel[c].ddb,HEX);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,eemtp1:error in memory allocation!");
                                quit(1);
                        }
#ifdef X_RAILWAY
                        f=enf(w,30,2+c,"__",tel[c].gn,DECIMAL);
#else
                        f=enf(w,30,2+c,"_",tel[c].gn,DECIMAL);
#endif
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,eemtp2:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,43,2+c,"_",&tel[c].is,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,eemtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,50,2+c,"_",&tel[c].s9,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,eemtp3a:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,56,2+c,"_",&tel[c].da,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,eemtp4:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,63,2+c,"_",&tel[c].ta,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,eemtp5:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,68,2+c,/*ddm*/"___",tel[c].t1,DECIMAL);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,eemtp1a:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,73,2+c,/*ddm*/"___",tel[c].t2,DECIMAL);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,eemtp1b:error in memory allocation!");
                                quit(1);
                        }
                }
                /* linking up & down fields */
                for(d=0,f=w->ff;d < ( max_dsply * 8 );d++,f=f->fnxt){
                        /* 8 = number of field in a line */

                        for(c=0,tf=f;(c < 8) && (tf != NULL);c++,tf=tf->fnxt);
                        /* 8 = number of field in a line */
                        f->fdn=tf;

                        for(c=8,tf=f;c && (tf != NULL);c--,tf=tf->fprv);
                        /* 8 = number of field in a line */
                        f->fup=tf;
                }
        saved_win=w;
        saved_a=a;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef EM_GROUP_PROGRAM
void eemtp_part1(BYTE n){  /* edit E&M trunk parameters */
        WINDOW *w;
        int a,b,c,s,p,e;
        unsigned int d;
        long h;
        int max_dsply;

        s=no_em[n] / NO_DISPLAY_ON_PAGE;
        p=no_em[n] % NO_DISPLAY_ON_PAGE;
        b=saved_b;
        w=saved_win;
        a=saved_a;
                caf(w);
                wcursor(w,76,1);
                h=ftell(fp[n]);
                fseek(fp[n],h,SEEK_SET);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++,a++){
                        wprintf(w,"\n     %1.3d\t\t ",a);
                        e=my_getw(fp[n])&0xff;  /* signaling protocol */
                        d=my_getw(fp[n]);  /* activation int */
                        sprintf(tel[c].ddb,"%1.2x",e);
                        tel[c].ai=d;
#ifdef ONLINE_EDIT
                        tel[c].ddbv=e;
                        prv_tel[c].ai=d;
                        prv_tel[c].ddbv=e;
#endif
#ifdef X_RAILWAY
                        sprintf(tel[c].gn,"%1.2d",((d & EM_TRUNK_GROUP_MASK)>>EMTGM_LOC) + 1);
#else
                        sprintf(tel[c].gn,"%.1d",((d & EM_TRUNK_GROUP_MASK)>>EMTGM_LOC) + 1);
#endif
                        /* for displaying trunk group # from 1 */

                        if(d & IS_MASK) tel[c].is='|';
                        else    tel[c].is='-';

                        if(d & S9_MASK) tel[c].s9='|';
                        else    tel[c].s9='-';

                        if(d & DA_MASK) tel[c].da='|';
                        else    tel[c].da='-';

                        if(d & TA_MASK) tel[c].ta='|';
                        else    tel[c].ta='-';

                        e=my_getw(fp[n])&0xff;
                        sprintf(tel[c].t1,"%1.3d",e);
#ifdef ONLINE_EDIT
                        prv_tel[c].t1v=e;
#endif
                        e=my_getw(fp[n])&0xff;
                        sprintf(tel[c].t2,"%1.3d",e);
#ifdef ONLINE_EDIT
                        prv_tel[c].t2v=e;
#endif
                }
        data_entry_init(saved_win);
        saved_a=a;
        saved_h=h;
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef EM_GROUP_PROGRAM
void eemtp_part2(BYTE n){  /* edit E&M trunk parameters */

        int b,c,s,p;
        unsigned int d;
        long h;
        int max_dsply;
#ifdef ONLINE_EDIT
        BYTE em_indx;
#endif

        b=saved_b;
        h=saved_h;
        s=no_em[n] / NO_DISPLAY_ON_PAGE;
        p=no_em[n] % NO_DISPLAY_ON_PAGE;
        fseek(fp[n],h,SEEK_SET);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++){
                        d=0xffff;
                        sscanf(tel[c].ddb,"%x",&d);
                        if ( d == 0xffff) d=prv_tel[c].ddbv;
                        my_putw(d,fp[n]);

#ifdef ONLINE_EDIT
                        tel[c].ddbv=d;
                        if (tel[c].ddbv != prv_tel[c].ddbv){
                                em_indx=(saved_a-max_dsply)+c-1;
                                send_em_ddbv_pckt(em_indx,c);
                        }
#endif
                        d=0;
                        sscanf(tel[c].gn,"%d",&d);
                        d--;
                        /* for saving trunk group # from 0 */
                        if ( d >= NO_EM_GROUP)
                                d=0;
                        d <<= EMTGM_LOC;
                        d &= EM_TRUNK_GROUP_MASK;
                        d |= (tel[c].ai & ~(EM_TRUNK_GROUP_MASK));

                        if(tel[c].is == '|')    d |= IS_MASK;
                        else    d &= ~IS_MASK;

                        if(tel[c].s9 == '|')    d |= S9_MASK;
                        else    d &= ~S9_MASK;

                        if(tel[c].da == '|')    d |= DA_MASK;
                        else    d &= ~DA_MASK;

                        if(tel[c].ta == '|')    d |= TA_MASK;
                        else    d &= ~TA_MASK;

                        my_putw(d,fp[n]);

#ifdef ONLINE_EDIT
                        tel[c].ai=d;
                        if (tel[c].ai != prv_tel[c].ai){
                                em_indx=(saved_a-max_dsply)+c-1;
                                send_em_ai_pckt(em_indx,c);
                        }
#endif
                        d=0xffff;
                        sscanf(tel[c].t1,"%d",&d);
                        if (d == 0xffff) d=(unsigned int)prv_tel[c].t1v;
                        my_putw(d,fp[n]);
#ifdef ONLINE_EDIT
#ifdef EM_CONTINUOUS_SIGNALING
                        tel[c].t1v=d;
                        if (tel[c].t1v != prv_tel[c].t1v){
                                em_indx=(saved_a-max_dsply)+c-1;
                                send_em_t1_pckt(em_indx,c);
                        }
#endif
#endif
                        d=0xffff;
                        sscanf(tel[c].t2,"%d",&d);
                        if (d == 0xffff) d=(unsigned int)prv_tel[c].t2v;
                        my_putw(d,fp[n]);

#ifdef ONLINE_EDIT
#ifdef EM_CONTINUOUS_SIGNALING
                        tel[c].t2v=d;
                        if (tel[c].t2v != prv_tel[c].t2v){
                                em_indx=(saved_a-max_dsply)+c-1;
                                send_em_t2_pckt(em_indx,c);
                        }
#endif
#endif
                }
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef EM_GROUP_PROGRAM
void eemgp(void){  /* edit extensions' parameters */

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        eemgp_part2();
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        return;
                  default :
                                return;
                }
        }
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef EM_GROUP_PROGRAM
void eemgp_init_display(void){  /* edit e&m group parameters */
        WINDOW *w;
        int a;
        unsigned int b,c;
        long h;
        FIELD *f;

        a=MAX_NAME_LENGHT + 2 + (no_extension[0] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[0] * 8) + (no_co[0] * 4) + (no_cas3[0] * 2) + (no_cas1[0] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8;
        fseek(fp[0],a,SEEK_SET);
        w=enw(16,0,NO_EM_GROUP+4,47);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEP,eemgp0:error in memory allocation!");
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
        swt(w,"EDIT E&M GROUP DIGITS");
        draw_window(w);
        wprompt(w,0,0,"e&m group number     \t\tdigits");
        wprompt(w,0,1,"---------------------------------------------");

        /* field allocation */
        idew(w);
        for(a=0;a < NO_EM_GROUP;a++){
                f=enf(w,29,2+a,hlim,emgpel[a],DECIMAL);
                if(f == NULL){
                        caw(); cs();
//DOOD : printf->print_error_report
                        print_error_report("\nDEP,eemgp1:error in memory allocation!");
                        quit(1);
                }
        }
        caf(w);
        wcursor(w,44,1);
        h=ftell(fp[0]);
        fseek(fp[0],h,SEEK_SET);
        for(a=0;a < NO_EM_GROUP;a++){
                wprintf(w,"\n\t\t%1.1d\t\t",a+1);
                b=my_getw(fp[0]);
                b&=0xff;
                c=(b&0xf0) >> 4;
                c=c >= 0xa ? 0 : c;
                b&=0x0f;
                b=b >= 0xa ? 0 : b;
                b=(b*10) + c;
                sprintf(emgpel[a],"%1.2d",b);
#ifdef ONLINE_EDIT
                emgpelv[a]=b;
                prv_emgpelv[a]=b;
#endif
        }
        saved_win=w;
        data_entry_init(saved_win);
        saved_a=a;
        saved_h=h;
        return;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#ifdef EM_GROUP_PROGRAM
void eemgp_part2(void){  /* edit e&m group parameters */

        int a;
        unsigned int b,c;
        long h;

        h=saved_h;
        a=saved_a;
        fseek(fp[0],h,SEEK_SET);
        for(a=0;a < NO_EM_GROUP;a++){
                b=0xffff;
                sscanf(emgpel[a],"%d",&b);
                if ( b == 0xffff) b=prv_emgpelv[a];

#ifdef ONLINE_EDIT
                emgpelv[a]=b;
                if (prv_emgpelv[a] != emgpelv[a]){
                        send_em_emgpel_pckt(a);
                }
#endif
                c=b/10;
                c=c == 0 ? 0xa : c;
                b=b%10;
                b=b == 0 ? 0xa : b;
                b=(b << 4) | c;
                my_putw(b,fp[0]);
        }
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef R1_SIGNALINGP
void e_office_code(void){  /* edit extensions' parameters */

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        e_office_code_part2();
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        return;
                  default :
                                return;
                }
        }
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef R1_SIGNALINGP
void e_office_code_init_display(void){  /* edit office code */
/* size of office code block in file is 6 bytes, 3 words */
        WINDOW *w;
        long h;
        FIELD *f=NULL;
        int a,b;

#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[0] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[0] * 8) + (no_co[0] * 4) + (no_cas3[0] * 2) + (no_cas1[0] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_em_group[0] * 2) + (no_cb[0] * 2) ;

#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[0] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[0] * 4) + (no_cas3[0] * 2) + (no_cas1[0] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_cb[0] * 2) ;
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[0] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[0] * 4) + (no_co[0] * 4) + (no_cas3[0] * 2) + (no_cas1[0] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_cb[0] * 2) ;
#endif

        fseek(fp[0],a,SEEK_SET);
        w=enw(30,11,3,19);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,e_office_code:error in memory allocation!");
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
        swt(w,"ENTER OFFICE CODE");
        draw_window(w);
        idew(w);
        if (no_intrnl_dgt == 8)
                f=enf(w,2,0,"__",ocl,DECIMAL);
        if (no_intrnl_dgt == 7)
                f=enf(w,3,0,"___",ocl,DECIMAL);
        if (no_intrnl_dgt == 6)
                f=enf(w,4,0,"____",ocl,DECIMAL);
        if (no_intrnl_dgt == 5)
                f=enf(w,5,0,"_____",ocl,DECIMAL);
        if (no_intrnl_dgt <= 4)
                f=enf(w,6,0,"______",ocl,DECIMAL);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,e_office_code2:error in memory allocation!");
                quit(1);
        }
        caf(w);

        h=ftell(fp[0]);
        fseek(fp[0],h,SEEK_SET);

        b=my_getw(fp[0]);
        ocl[0]=b&0xff;
        ocl[1]=b>>8;
        b=my_getw(fp[0]);
        ocl[2]=b&0xff;
        ocl[3]=b>>8;
        b=my_getw(fp[0]);
        ocl[4]=b&0xff;
        ocl[5]=b>>8;
        if (no_intrnl_dgt == 8)
                ocl[2]=0;
        if (no_intrnl_dgt == 7)
                ocl[3]=0;
        if (no_intrnl_dgt == 6)
                ocl[4]=0;
        if (no_intrnl_dgt == 5)
                ocl[5]=0;
        if (no_intrnl_dgt <= 4)
                ocl[6]=0;
#ifdef ONLINE_EDIT
        oclv=-1;
        sscanf(ocl,"%ld",&oclv);
        prv_oclv=oclv;
#endif
        saved_win=w;
        data_entry_init(saved_win);
        saved_h=h;
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef R1_SIGNALINGP
void e_office_code_part2(void){  /* edit office code */
/* size of office code block in file is 6 bytes, 3 words */

        long h;
        int a,b;

        h=saved_h;
        fseek(fp[0],h,SEEK_SET);

        for(a=0;a<6;a++){
                if(!isdigit(ocl[a])){
                        ocl[a]=0;
                        break;
                }
        }

#ifdef ONLINE_EDIT
        sscanf(ocl,"%ld",&oclv);
        if (oclv != prv_oclv){
                send_office_code_ocl_pckt();
        }
#endif
        b=(ocl[1] << 8) | ocl[0];
        my_putw(b,fp[0]);
        b=(ocl[3] << 8) | ocl[2];
        my_putw(b,fp[0]);
        b=(ocl[5] << 8) | ocl[4];
        my_putw(b,fp[0]);


#ifdef LIM
#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[1] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[1] * 8) + (no_co[1] * 4) + (no_cas3[1] * 2) + (no_cas1[1] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_em_group[1] * 2) + (no_cb[1] * 2) ;

#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[1] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[1] * 4) + (no_cas3[1] * 2) + (no_cas1[1] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_cb[1] * 2) ;
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[1] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[1] * 4) + (no_co[1] * 4) + (no_cas3[1] * 2) + (no_cas1[1] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_cb[1] * 2) ;
#endif
        fseek(fp[1],a,SEEK_SET);

        b=(ocl[1] << 8) | ocl[0];
        my_putw(b,fp[1]);
        b=(ocl[3] << 8) | ocl[2];
        my_putw(b,fp[1]);
        b=(ocl[5] << 8) | ocl[4];
        my_putw(b,fp[1]);

#if(NO_EXTENSION_C != 0)

#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[2] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[2] * 8) + (no_co[2] * 4) + (no_cas3[2] * 2) + (no_cas1[2] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_em_group[2] * 2) + (no_cb[2] * 2) ;
#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[2] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[2] * 4) + (no_cas3[2] * 2) + (no_cas1[2] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_cb[2] * 2) ;
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[2] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[2] * 4) + (no_co[2] * 4) + (no_cas3[2] * 2) + (no_cas1[2] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_cb[2] * 2) ;
#endif

        fseek(fp[2],a,SEEK_SET);

        b=(ocl[1] << 8) | ocl[0];
        my_putw(b,fp[2]);
        b=(ocl[3] << 8) | ocl[2];
        my_putw(b,fp[2]);
        b=(ocl[5] << 8) | ocl[4];
        my_putw(b,fp[2]);

#endif // (NO_EXTENSION_C != 0)
#endif  // LIM
        return;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#ifdef HOT_LINE_SERVICE_ACTIVE
int edhl(BYTE n){  /* edit extensions' parameters */

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        edhl_part2(n);
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        deletw(saved_hwin);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef HOT_LINE_SERVICE_ACTIVE
void edhl_init_display(BYTE n){  /* edit destination hot line */
        WINDOW *w,*wh;
        int a,c;
        long b;
        long h;
        FIELD *f;

#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2)  + (NO_OPR_NITX_PARAMS * 2);
#endif

        fseek(fp[n],a,SEEK_SET);
        w=enw(17,0,NO_HOT_LINE+4,46);
        if(w == NULL){
                caw();  cs();
                print_error_report("\nDEP,edhl0:error in memory allocation!");
                quit(1);
        }
        wh=enw(26,10,7,32);
        if(wh == NULL){
                caw();  cs();
                print_error_report("\nDEP,edhl1:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
        swt(w,"EDIT HOT LINES");
        draw_window(w);

        dw(wh);
        swb(wh,DOUBLE_LINE);
        swc(wh,ALL,KT_BLUE,KT_YELLOW,KT_HIGH_INTENSITY);
        swt(wh,"HOT LINES HELP");
        draw_window(wh);
        wprompt(wh,0,0,"   valid destination type");
        wprompt(wh,0,1,"-----------------------------");
        wprompt(wh,1,2,"CO : destination CO trunk.");
        wprompt(wh,1,3,"TIE: destination TIE trunk.");
        wprompt(wh,1,4,"EXT: destination extension.");

        wprompt(w,0,0,"dest. hot line\t\tdest. type\t  dest. #");
        wprompt(w,0,1,"--------------------------------------------");

        /* field allocation */
        idew(w);
        for(a=0;a < NO_HOT_LINE;a++){
                f=enf(w,24,2+a,"___",hlel[a].dht,ALPHANUMERIC);
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,edhl2:error in memory allocation!");
                        quit(1);
                }
//              f=enf(w,36,2+a,ddm,hlel[a].dhi,DECIMAL);
                f=enf(w,33,2+a,ddm,hlel[a].dhi,DECIMAL);
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,edhl3:error in memory allocation!");
                        quit(1);
                }
        }
        caf(w);
        wcursor(w,43,1);
        h=ftell(fp[n]);
        fseek(fp[n],h,SEEK_SET);
        for(a=0;a < NO_HOT_LINE;a++){
                wprintf(w,"\n\t   %1.1d",a+1);
                b=my_getw(fp[n]);
#ifdef ONLINE_EDIT

                hlel[a].dhv=b;
                prv_hlel[a].dhv=b;

#endif
                c=(int)(b & 0x0f);
                if(c == NULL_T)
                        continue;
                b >>= 4;

                switch(c){
                        case XST_T:     strcpy(hlel[a].dht,"EXT");
                                        b = dep_find_subscriber_no(b);
                                        break;
                        case CO_T:      strcpy(hlel[a].dht,"CO ");
                                        b++;
                                        /* for displaying trunk # from 1 instead of 0 */
                                        break;
                        case TIE_T:     strcpy(hlel[a].dht,"TIE");
                                        b++;
                                        /* for displaying trunk # from 1 instead of 0 */
                                        break;
                        default:        break;
                }
                ltoa(b,hlel[a].dhi,10);
        }
        saved_win=w;
        data_entry_init(saved_win);
        saved_hwin=wh;
        saved_h=h;
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef HOT_LINE_SERVICE_ACTIVE
void edhl_part2(BYTE n){  /* edit destination hot line */

        int a;
        unsigned long b;
        long h;

        h=saved_h;
        fseek(fp[n],h,SEEK_SET);
pc(0,0);
        for(a=0;a < NO_HOT_LINE;a++){
                if(strcmp(hlel[a].dht,"EXT") == 0){
                        b=-1;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b = dep_fcp(b);
                        if(b == OPR_ERROR)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= XST_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
                        goto endfor;
                }
                if(strcmp(hlel[a].dht,"CO ") == 0){
#if(NO_CO != 0)
                        b=0;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b--;
                        /* for saving trunk # from 0 instead of 1 */
                        if(b >= NO_CO)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= CO_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
#else
                        my_putw(NULL_T,fp[n]);
#endif
                        goto endfor;
                }
                if(strcmp(hlel[a].dht,"TIE") == 0){

#ifndef NO_TIE_INIT
                        b=0;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b--;
                        /* for saving trunk # from 0 instead of 1 */
                        if(b >= NO_TIE)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= TIE_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
#else
                        my_putw(NULL_T,fp[n]);
#endif
                        goto endfor;
                }
                b=NULL_T;
                my_putw(NULL_T,fp[n]);
endfor:
#ifdef ONLINE_EDIT
                hlel[a].dhv=b;
                if (hlel[a].dhv != prv_hlel[a].dhv){
                        send_edhl_hlel_pckt(a);
                }
#endif
        }
#ifdef LIM
        n=1;
#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2)  + (NO_OPR_NITX_PARAMS * 2);
#endif
        fseek(fp[n],a,SEEK_SET);
pc(0,0);
        for(a=0;a < NO_HOT_LINE;a++){
                if(strcmp(hlel[a].dht,"EXT") == 0){
                        b=-1;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b = dep_fcp(b);
                        if(b == OPR_ERROR)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= XST_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
                        goto endfor;
                }
                if(strcmp(hlel[a].dht,"CO ") == 0){
#if(NO_CO != 0)
                        b=0;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b--;
                        /* for saving trunk # from 0 instead of 1 */
                        if(b >= NO_CO)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= CO_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
#else
                        my_putw(NULL_T,fp[n]);
#endif
                        goto endfor;
                }
                if(strcmp(hlel[a].dht,"TIE") == 0){

#ifndef NO_TIE_INIT
                        b=0;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b--;
                        /* for saving trunk # from 0 instead of 1 */
                        if(b >= NO_TIE)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= TIE_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
#else
                        my_putw(NULL_T,fp[n]);
#endif
                        goto endfor;
                }
                b=NULL_T;
                my_putw(NULL_T,fp[n]);
endfor:
#ifdef ONLINE_EDIT
                hlel[a].dhv=b;
                if (hlel[a].dhv != prv_hlel[a].dhv){
                        send_edhl_hlel_pckt(a);
                }
#endif
        }

#if (NO_EXTENSION_C != 0)
        n=2;
#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2)  + (NO_OPR_NITX_PARAMS * 2);
#endif
        fseek(fp[n],a,SEEK_SET);
pc(0,0);
        for(a=0;a < NO_HOT_LINE;a++){
                if(strcmp(hlel[a].dht,"EXT") == 0){
                        b=-1;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b = dep_fcp(b);
                        if(b == OPR_ERROR)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= XST_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
                        goto endfor;
                }
                if(strcmp(hlel[a].dht,"CO ") == 0){
#if(NO_CO != 0)
                        b=0;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b--;
                        /* for saving trunk # from 0 instead of 1 */
                        if(b >= NO_CO)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= CO_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
#else
                        my_putw(NULL_T,fp[n]);
#endif
                        goto endfor;
                }
                if(strcmp(hlel[a].dht,"TIE") == 0){

#ifndef NO_TIE_INIT
                        b=0;
                        sscanf(hlel[a].dhi,"%ld",&b);
                        b--;
                        /* for saving trunk # from 0 instead of 1 */
                        if(b >= NO_TIE)
                                b=NULL_T;
                        else{
                                b <<= 4;
                                b |= TIE_T;
                        }
                        my_putw((unsigned int)b,fp[n]);
#else
                        my_putw(NULL_T,fp[n]);
#endif
                        goto endfor;
                }
                b=NULL_T;
                my_putw(NULL_T,fp[n]);
endfor:
#ifdef ONLINE_EDIT
                hlel[a].dhv=b;
                if (hlel[a].dhv != prv_hlel[a].dhv){
                        send_edhl_hlel_pckt(a);
                }
#endif
        }
#endif
#endif
        return;
}
#endif

/* ========================================================================= */
/* ========================================================================= */

#if (NO_ACTIVE_OPR != 0)
int eonx(BYTE n){  /* edit operator's night extensions */

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        eonx_part2(n);
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#if (NO_ACTIVE_OPR != 0)
void eonx_init_display(BYTE n){  /* edit operator's night extensions */
        WINDOW *w;
        int a;
        unsigned long b;
        long h;
        FIELD *f;

#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#endif

        fseek(fp[n],a,SEEK_SET);
        w=enw(16,0,NO_OPR_NITX+4,47);
        if(w == NULL){
                caw();  cs();
                print_error_report("\nDEP,eonx0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
        swt(w,"EDIT OPERATOR'S NIGHT");
        draw_window(w);
        wprompt(w,0,0,"operator's night ext.\t\text. #");
        wprompt(w,0,1,"---------------------------------------------");

        /* field allocation */
        idew(w);
        for(a=0;a < NO_OPR_NITX;a++){
//              f=enf(w,29,2+a,ddm,nel[a],DECIMAL);
                f=enf(w,27,2+a,ddm,nel[a],DECIMAL);
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,eonx1:error in memory allocation!");
                        quit(1);
                }
        }
        caf(w);
        wcursor(w,44,1);
        h=ftell(fp[n]);
        fseek(fp[n],h,SEEK_SET);
        for(a=0;a < NO_OPR_NITX;a++){
                wprintf(w,"\n\t\t%1.1d\t\t",a+1);
                b=my_getw(fp[n]);
#ifdef ONLINE_EDIT
                nelv[a]=b;
                prv_nelv[a]=b;
#endif
                if( (b & 0x0f) == XST_T){  /* if night is an extension */
                        b >>= 4;
                        b = dep_find_subscriber_no((unsigned int)b);
                        ltoa(b,nel[a],10);
                }
        }
        saved_win=w;
        data_entry_init(saved_win);
        saved_h=h;

        return ;
}
#endif


/* ========================================================================= */
/* ========================================================================= */
#if (NO_ACTIVE_OPR != 0)
void eonx_part2(BYTE n){  /* edit operator's night extensions */

        int a;
        unsigned long b;
        long h;

        h=saved_h;
        fseek(fp[n],h,SEEK_SET);
        for(a=0;a < NO_OPR_NITX;a++){
                b=-1;
                sscanf(nel[a],"%ld",&b);
                b = dep_fcp(b);
                if(b == OPR_ERROR){
                        b=NULL_T;
                }
                else{
                        b <<= 4;
                        b |= XST_T;
                }
                my_putw((unsigned)b,fp[n]);
#ifdef ONLINE_EDIT
                nelv[a]=b;
                if (nelv[a] != prv_nelv[a]){
                        send_eonx_nel_pckt(a);
                }
#endif
        }

#ifdef LIM
        n=1;
#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#endif
        fseek(fp[n],a,SEEK_SET);
        for(a=0;a < NO_OPR_NITX;a++){
                b=-1;
                sscanf(nel[a],"%ld",&b);
                b = dep_fcp(b);
                if(b == OPR_ERROR){
                        b=NULL_T;
                }
                else{
                        b <<= 4;
                        b |= XST_T;
                }
                my_putw((unsigned)b,fp[n]);
#ifdef ONLINE_EDIT
                nelv[a]=b;
                if (nelv[a] != prv_nelv[a]){
                        send_eonx_nel_pckt(a);
                }
#endif
        }

#if (NO_EXTENSION_C != 0)
        n=2;
#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#endif
        fseek(fp[n],a,SEEK_SET);
        for(a=0;a < NO_OPR_NITX;a++){
                b=-1;
                sscanf(nel[a],"%ld",&b);
                b = dep_fcp(b);
                if(b == OPR_ERROR){
                        b=NULL_T;
                }
                else{
                        b <<= 4;
                        b |= XST_T;
                }
                my_putw((unsigned)b,fp[n]);
#ifdef ONLINE_EDIT
                nelv[a]=b;
                if (nelv[a] != prv_nelv[a]){
                        send_eonx_nel_pckt(a);
                }
#endif
        }
#endif
#endif
        return ;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
int edtp(BYTE n){  /* edit digital trunk parameters */

        int code;
        int s,p;

#ifdef EXTERNAL_CAS3_INSTALLED
        s=no_cas3[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas3[n] % NO_DISPLAY_ON_PAGE;
#else   // (NO_CAS1 != 0)
        s=no_cas1[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas1[n] % NO_DISPLAY_ON_PAGE;
#endif
        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        edtp_part2(n);
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        saved_b=0;
                        saved_n=0;
                        return code;
                  case PGDN :
                        edtp_part2(n);
                        saved_b++;
                        if (saved_b < s){
                                edtp_part1(n);
                                return code;
                        }
                        if (saved_b == s){
                                if (p > 0){
                                        deletw(saved_win);
                                        edtp_init_display(n);
                                        edtp_part1(n);
                                        return code;
                                }
                        }

                        saved_n=n;
                        saved_b--;
                        return code;
                  case PGUP :
                                return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void edtp_init_display(BYTE n){  /* edit digital trunk parameters */

        WINDOW *w;
        int a,b,c,s,p;
        unsigned int d;
        FIELD *f,*tf;
        int max_dsply;

#ifdef EXTERNAL_CAS3_INSTALLED
        s=no_cas3[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas3[n] % NO_DISPLAY_ON_PAGE;
#else   // (NO_CAS1 != 0)
        s=no_cas1[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas1[n] % NO_DISPLAY_ON_PAGE;
#endif
        if ( saved_b == 0 ){
#ifdef EM_GROUP_PROGRAM

                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4);
#elif (defined NO_TIE_INIT)

                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4);
#else
                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4);
#endif
                fseek(fp[n],a,SEEK_SET);
                saved_a=1;
        }
        a=saved_a;
        b=saved_b;
        if(b != s){
                w=enw(0,0,20,80);
        }
        else{
                if (p > 0)
                        w=enw(0,0,p+4,80);
                else
                        w=enw(0,0,20,80);
        }
                if(w == NULL){
                        caw();  cs();
//DOOD : printf->print_error_report
                        print_error_report("\nDEP,edtp0:error in memory allocation!");
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
                swt(w,"EDIT DIGITAL TRUNKS PARAMETERS");
                draw_window(w);
                wprompt(w,0,0,"dig.trunk #  group #  is  icc  ogc  pts  eos  mtr  opr  mal  imm  wfa  ids   ");
                wprompt(w,0,1,"-------------------------------------------------------------------------------");

                /* field allocation */
                idew(w);
                if (b != s )
                        max_dsply=NO_DISPLAY_ON_PAGE;
                else
                        if ( p > 0)
                                max_dsply=p;
                        else
                                max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++){
                        f=enf(w,15,2+c,"__",dtel[c].gn,DECIMAL);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp1:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,22,2+c,"_",&dtel[c].is,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp2:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,27,2+c,"_",&dtel[c].icc,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,32,2+c,"_",&dtel[c].ogc,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,37,2+c,"_",&dtel[c].pts,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,42,2+c,"_",&dtel[c].eos,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,47,2+c,"_",&dtel[c].mtr,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,52,2+c,"_",&dtel[c].opr,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,57,2+c,"_",&dtel[c].mal,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,62,2+c,"_",&dtel[c].imm,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,67,2+c,"_",&dtel[c].wfa,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,72,2+c,"_",&dtel[c].ids,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,edtp3:error in memory allocation!");
                                quit(1);
                        }
                }
                /* linking up & down fields */
                for(d=0,f=w->ff;d < ( max_dsply * 12 );d++,f=f->fnxt){
                        /* 12 = number of field in a line */

                        for(c=0,tf=f;(c < 12) && (tf != NULL);c++,tf=tf->fnxt);
                        /* 12 = number of field in a line */
                        f->fdn=tf;

                        for(c=12,tf=f;c && (tf != NULL);c--,tf=tf->fprv);
                        /* 12 = number of field in a line */
                        f->fup=tf;
                }
        saved_win=w;
        saved_a=a;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void edtp_part1(BYTE n){  /* edit digital trunk parameters */
        WINDOW *w;
        int a,b,c,s,p;
        unsigned int d;
        long h;
        int max_dsply;

#ifdef EXTERNAL_CAS3_INSTALLED
        s=no_cas3[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas3[n] % NO_DISPLAY_ON_PAGE;
#else   // (NO_CAS1 != 0)
        s=no_cas1[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas1[n] % NO_DISPLAY_ON_PAGE;
#endif
        b=saved_b;
        w=saved_win;
        a=saved_a;
        caf(w);
        wcursor(w,76,1);
        h=ftell(fp[n]);
        fseek(fp[n],h,SEEK_SET);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++,a++){
                        wprintf(w,"\n     %1.3d",a);
                        d=my_getw(fp[n]);  /* activation int */
                        dtel[c].ai=d;
#ifdef ONLINE_EDIT

                        prv_dtel[c].ai=d;
#endif
//                      sprintf(dtel[c].gn,"%2.2d",(d & CAS3_GROUP_MASK) + 1);
                        sprintf(dtel[c].gn,"%2.2d",(d & CAS3_GROUP_MASK));
                        /* for displaying trunk group # from 1 */

                        if(d & IS) dtel[c].is='|';
                        else    dtel[c].is='-';

                        if(d & ICC) dtel[c].icc='|';
                        else    dtel[c].icc='-';

                        if(d & OGC) dtel[c].ogc='|';
                        else    dtel[c].ogc='-';

                        if(d & PTS) dtel[c].pts='|';
                        else    dtel[c].pts='-';

                        if(d & EOS) dtel[c].eos='|';
                        else    dtel[c].eos='-';

                        if(d & CAS_METERING) dtel[c].mtr='|';
                        else    dtel[c].mtr='-';

                        if(d & OPR_RCL) dtel[c].opr='|';
                        else    dtel[c].opr='-';

                        if(d & MAL_CALL) dtel[c].mal='|';
                        else    dtel[c].mal='-';

                        if(d & IMM_ACK) dtel[c].imm='|';
                        else    dtel[c].imm='-';

                        if(d & WTFR_ACK) dtel[c].wfa='|';
                        else    dtel[c].wfa='-';

                        if(d & INTER_SIGNAL_DGT_SEND) dtel[c].ids='|';
                        else    dtel[c].ids='-';
                }
        data_entry_init(saved_win);
        saved_a=a;
        saved_h=h;
        return;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void edtp_part2(BYTE n){  /* edit digital trunk parameters */

        int b,c,s,p;
        unsigned int d;
        long h;
        int max_dsply;
        unsigned int cas_indx;
        b=saved_b;
        h=saved_h;
#ifdef EXTERNAL_CAS3_INSTALLED
        s=no_cas3[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas3[n] % NO_DISPLAY_ON_PAGE;
#else   // (NO_CAS1 != 0)
        s=no_cas1[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas1[n] % NO_DISPLAY_ON_PAGE;
#endif
        fseek(fp[n],h,SEEK_SET);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++){

                        sscanf(dtel[c].gn,"%d",&d);
//                      d--;
                        /* for saving trunk group # from 0 */
#ifdef EXTERNAL_CAS3_INSTALLED
                        if(d >= no_cas3_programmable_group[0])
#else   // (NO_CAS1 != 0)
                        if(d >= no_cas1_group[0])
#endif
                                d=0;
                        d &= (CAS3_GROUP_MASK);
                        d |= (dtel[c].ai & ~(CAS3_GROUP_MASK));

                        if(dtel[c].is == '|')    d |= IS;
                        else    d &= ~IS;

                        if(dtel[c].icc == '|')    d |= ICC;
                        else    d &= ~ICC;

                        if(dtel[c].ogc == '|')    d |= OGC;
                        else    d &= ~OGC;

                        if(dtel[c].pts == '|')    d |= PTS;
                        else    d &= ~PTS;

                        if(dtel[c].eos == '|')    d |= EOS;
                        else    d &= ~EOS;

                        if(dtel[c].mtr == '|')    d |= CAS_METERING;
                        else    d &= ~CAS_METERING;

                        if(dtel[c].opr == '|')    d |= OPR_RCL;
                        else    d &= ~OPR_RCL;

                        if(dtel[c].mal == '|')    d |= MAL_CALL;
                        else    d &= ~MAL_CALL;

                        if(dtel[c].imm == '|')    d |= IMM_ACK;
                        else    d &= ~IMM_ACK;

                        if(dtel[c].wfa == '|')    d |= WTFR_ACK;
                        else    d &= ~WTFR_ACK;

                        if(dtel[c].ids == '|')    d |= INTER_SIGNAL_DGT_SEND;
                        else    d &= ~INTER_SIGNAL_DGT_SEND;

                        my_putw(d,fp[n]);
#ifdef ONLINE_EDIT
                        dtel[c].ai=d;
                        if (dtel[c].ai != prv_dtel[c].ai){
                                cas_indx=(saved_a-max_dsply)+c-1;
                                send_cas_ai_pckt(cas_indx,c);
                        }
#endif
                }
        return;
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#ifndef NO_TIE_INIT
int ettp(BYTE n){  /* edit extensions' parameters */

        int code;
        int p,s;

        s=no_tie[n] / NO_DISPLAY_ON_PAGE;
        p=no_tie[n] % NO_DISPLAY_ON_PAGE;
        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        ettp_part2(n);
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        saved_b=0;
                        saved_n=0;
                        return code;
                  case PGDN :
                        ettp_part2(n);
                        saved_b++;
                        if (saved_b < s){
                                ettp_part1(n);
                                return code;
                        }
                        if (saved_b == s){
                                if (p > 0){
                                        deletw(saved_win);
                                        ectp_init_display(n);
                                        ectp_part1(n);
                                        return code;
                                }
                        }
#ifdef LIM
                /* saved_b > */
                        if ( saved_n == 0){
                                saved_n=1;
                                if ((no_tie[1] / NO_DISPLAY_ON_PAGE) != 0){
                                        saved_b=0;
                                        deletw(saved_win);
                                        ettp_init_display(saved_n);
                                        ettp_part1(saved_n);
                                        return code;
                                }
                        }
#if(NO_EXTENSION_C != 0)
                        if ( saved_n == 1){
                                saved_n=2;
                                if ((no_tie[2] / NO_DISPLAY_ON_PAGE) == 0){
                                        saved_b=0;
                                        deletw(saved_win);
                                        ettp_init_display(saved_n);
                                        ettp_part1(saved_n);
                                        return code;
                                }
                        }
#endif
#endif
                        saved_n=n;
                        saved_b--;
                        return code;
                  case PGUP :
                                return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifndef NO_TIE_INIT
void ettp_init_display(BYTE n){  /* edit TIE trunk parameters */

        WINDOW *w;
        int a,b,c,s,p;
        unsigned int d;
        FIELD *f,*tf;
        int max_dsply;

        s=no_tie[n] / NO_DISPLAY_ON_PAGE;
        p=no_tie[n] % NO_DISPLAY_ON_PAGE;
        if (saved_b == 0){
                fseek(fp[n],MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH),SEEK_SET);
                saved_a=1;
        }
        a=saved_a;
        b=saved_b;
        if(b != s){
                w=enw(0,0,20,80);
        }
        else{
                if (p > 0)
                        w=enw(0,0,p+4,80);
                else
                        w=enw(0,0,20,80);
        }
                if(w == NULL){
                        caw();  cs();
//DOOD : printf->print_error_report
                        print_error_report("\nDEP,ettp0:error in memory allocation!");
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
                swt(w,"EDIT TIE TRUNKS PARAMETERS");
                draw_window(w);
                wprompt(w,0,0,"TIE trunk #    dest. DDI    group #   in service   DDI active   timing active");
                wprompt(w,0,1,"-------------------------------------------------------------------------------");

                /* field allocation */
                idew(w);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;

                for(c=0;c < max_dsply;c++){
                        f=enf(w,17,2+c,ddm,tel[c].ddb,DECIMAL);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ettp1:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,30,2+c,"_",tel[c].gn,DECIMAL);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ettp2:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,43,2+c,"_",&tel[c].is,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ettp3:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,56,2+c,"_",&tel[c].da,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ettp4:error in memory allocation!");
                                quit(1);
                        }
                        f=enf(w,70,2+c,"_",&tel[c].ta,BOOLEAN);
                        if(f == NULL){
                                caw(); cs();
//DOOD : printf->print_error_report
                                print_error_report("\nDEP,ettp5:error in memory allocation!");
                                quit(1);
                        }
                }
                /* linking up & down fields */
                for(d=0,f=w->ff;d < ( max_dsply * 5 );d++,f=f->fnxt){
                        /* 5 = number of field in a line */

                        for(c=0,tf=f;(c < 5) && (tf != NULL);c++,tf=tf->fnxt);
                        /* 5 = number of field in a line */
                        f->fdn=tf;

                        for(c=5,tf=f;c && (tf != NULL);c--,tf=tf->fprv);
                        /* 5 = number of field in a line */
                        f->fup=tf;
                }
        saved_win=w;
        saved_a=a;
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifndef NO_TIE_INIT
void ettp_part1(BYTE n){  /* edit TIE trunk parameters */

        WINDOW *w;
        int a,b,c,s,p;
        unsigned int d,e;
        long h;
        int max_dsply;

        s=no_tie[n] / NO_DISPLAY_ON_PAGE;
        p=no_tie[n] % NO_DISPLAY_ON_PAGE;

        b=saved_b;
        w=saved_win;
        a=saved_a;
        caf(w);
        wcursor(w,76,1);
        h=ftell(fp[n]);
        fseek(fp[n],h,SEEK_SET);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++,a++){
                        wprintf(w,"\n     %1.3d\t\t ",a);
                        e=my_getw(fp[n]);  /* destination DDI */
                        d=my_getw(fp[n]);  /* activation int */
                        if(((unsigned int) e < NO_TOTAL_EXTENSION) && (d & DA_MASK)){
                                ltoa(dep_find_subscriber_no(e),tel[c].ddb,10);
                        }
                        tel[c].ai=d;
#ifdef ONLINE_EDIT
                        tel[c].ddbv=e;
                        prv_tel[c].ai=d;
                        prv_tel[c].ddbv=e;
#endif
                        kt_itoa( (d & TRUNK_GROUP_MASK) + 1,tel[c].gn,10);
                        /* for displaying trunk group # from 1 */

                        if(d & IS_MASK) tel[c].is='|';
                        else    tel[c].is='-';

                        if(d & DA_MASK) tel[c].da='|';
                        else    tel[c].da='-';

                        if(d & TA_MASK) tel[c].ta='|';
                        else    tel[c].ta='-';
                }
        data_entry_init(saved_win);
        saved_a=a;
        saved_h=h;
        return;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifndef NO_TIE_INIT
void ettp_part2(BYTE n){  /* edit TIE trunk parameters */

        int b,c,s,p;
        unsigned int d;
        unsigned long d1;
        long h;
        int max_dsply;
        unsigned int tie_indx;

        b=saved_b;
        h=saved_h;
        s=no_tie[n] / NO_DISPLAY_ON_PAGE;
        p=no_tie[n] % NO_DISPLAY_ON_PAGE;

        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                fseek(fp[n],h,SEEK_SET);
                for(c=0;c < max_dsply;c++){
                        d1=-1;
                        sscanf(tel[c].ddb,"%ld",&d1);
                        d1 = dep_fcp(d1);
                        if(d1 == OPR_ERROR){
                                my_putw(OPR_ERROR,fp[n]);
                        }

                        else{
                                my_putw((unsigned int)d1,fp[n]);
#ifdef ONLINE_EDIT
                                tel[c].ddbv=d1;
                                if (tel[c].ddbv  != prv_tel[c].ddbv){
                                        tie_indx=(saved_a-max_dsply)+c-1;
                                        send_tie_ddbv_pckt(tie_indx,c);
                                }
#endif
                        }
                        d=0;
                        sscanf(tel[c].gn,"%d",&d);
                        d--;
                        /* for saving trunk group # from 0 */
                        if(d >= NO_TIET_GROUP)
                                d=0;
                        d &= TRUNK_GROUP_MASK;
                        d |= (tel[c].ai & ~TRUNK_GROUP_MASK);

                        if(tel[c].is == '|')    d |= IS_MASK;
                        else    d &= ~IS_MASK;

                        if(tel[c].da == '|')    d |= DA_MASK;
                        else    d &= ~DA_MASK;

                        if(tel[c].ta == '|')    d |= TA_MASK;
                        else    d &= ~TA_MASK;

#ifdef ONLINE_EDIT

                        tel[c].ai=d;
                        if ( d != prv_tel[c].ai){
                                tie_indx=(saved_a-max_dsply)+c-1;
                                send_tie_ai_pckt(tie_indx,c);
                        }

#endif
                        my_putw(d,fp[n]);
                }
        return;
}
#endif

//GRPPRG start : block added
/* ======================================================================== */
/* ======================================================================== */
void ask_ext_no(void){  /* edit extensions' parameters */

        int code;
        long ext_pos_in_file;

        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        return;
                  case ENTER :
                        ask_ext_no_part2();
                        data_entry_flag=RESET;
                        saved_a=(int)(srch_ext_no_v - (srch_ext_no_v % NO_DISPLAY_ON_PAGE));
                        ext_pos_in_file=saved_a;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                        if (srch_ext_no_v >= (NO_EXTENSION_A+NO_EXTENSION_B)){
                                saved_n=2;
                                fdb=NO_EXTENSION_A+NO_EXTENSION_B;
                                srch_ext_no_v-=(NO_EXTENSION_A+NO_EXTENSION_B);
                                ext_pos_in_file-=(NO_EXTENSION_A+NO_EXTENSION_B);
                                saved_b=(unsigned int)(srch_ext_no_v / NO_DISPLAY_ON_PAGE); // current page no.
                        }
                        else
#endif
                        if (srch_ext_no_v >= NO_EXTENSION_A){
                                saved_n=1;
                                fdb=NO_EXTENSION_A;
                                srch_ext_no_v-=NO_EXTENSION_A;
                                ext_pos_in_file-=NO_EXTENSION_A;
                                saved_b=(unsigned int)(srch_ext_no_v / NO_DISPLAY_ON_PAGE); // current page no.
                        }
                        else{
                                saved_n=0;
                                fdb=0;
                                saved_b=(unsigned int)(srch_ext_no_v / NO_DISPLAY_ON_PAGE); // current page no.
                        }
#else  // LIM
                        saved_n=0;
                        fdb=0;
                        saved_b=(int)(srch_ext_no_v / NO_DISPLAY_ON_PAGE); // current page no.
#endif // LIM
                        saved_h=MAX_NAME_LENGHT+2+(EXTENSION_PARAM_BLOCK_LENGTH*ext_pos_in_file);
                        fseek(fp[saved_n],saved_h,SEEK_SET);
                        deletw(saved_win);
                        ep_2nd_menu_flag=1;
                        exp_init_display(saved_n);
                        exp_part1(saved_n);
                        return;
                  default :
                        return;
                }
        }
}
/* ======================================================================== */
/* ======================================================================== */
void ask_ext_no_init_display(void){  /* edit office code */

        WINDOW *w;
        FIELD *f;

        w=enw(30,11,3,20);
        if(w == NULL){
                caw();  cs();
                print_error_report("\nDEPM,ext_no_ask:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
        swt(w," ENTER EXT. NO. ");
        draw_window(w);
        idew(w);
        f=enf(w,5,0,"__________",srch_ext_no,DECIMAL);
        if(f == NULL){
                caw(); cs();
                print_error_report("\nDEPM,ext_no_ask:error in memory allocation!");
                quit(1);
        }
        caf(w);

        saved_win=w;
        data_entry_init(saved_win);
        return;
}
/* ======================================================================== */
/* ======================================================================== */
void ask_ext_no_part2(void){  /* edit office code */

        int a;

        for(a=0;a<10;a++){
                if(!isdigit(srch_ext_no[a])){
                        srch_ext_no[a]=0;
                        break;
                }
        }
        srch_ext_no_v=-1;
        sscanf(srch_ext_no,"%ld",&srch_ext_no_v);
        srch_ext_no_v = dep_fcp(srch_ext_no_v);
        if (srch_ext_no_v >= NO_TOTAL_EXTENSION) srch_ext_no_v=0;
        return;
}
/* ========================================================================= */
/* ========================================================================= */
#ifdef CB_INSTALLED
int ecbp(BYTE n){  /* edit extensions' parameters */

        int code;

        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        ecbp_part2(n);
                        data_entry_flag=RESET;
                        deletw(saved_win);
                        ep_2nd_menu_flag = RESET;
                        ep_1st_menu_init();
                        saved_b=0;
                        saved_n=0;
                        return code;
                  case PGDN :
                        ecbp_part2(saved_n);
                        saved_b++;
                        if (saved_b < (no_cb[saved_n] / NO_DISPLAY_ON_PAGE)){
                                ecbp_part1(saved_n);
                                return code;
                        }
                        if (saved_b == (no_cb[saved_n] / NO_DISPLAY_ON_PAGE)){
                                if ((no_cb[saved_n] % NO_DISPLAY_ON_PAGE) > 0){
                                        deletw(saved_win);
                                        ecbp_init_display(saved_n);
                                        ecbp_part1(saved_n);
                                        return code;
                                }
                        }
#ifdef LIM
                /* saved_b > */
                        if ( saved_n == 0){
                                saved_b=0;
                                saved_n=1;
                                deletw(saved_win);
                                ecbp_init_display(saved_n);
                                ecbp_part1(saved_n);
                                return code;
                        }
#if(NO_EXTENSION_C != 0)
                        if ( saved_n == 1){
                                saved_b=0;
                                saved_n=2;
                                deletw(saved_win);
                                ecbp_init_display(saved_n);
                                ecbp_part1(saved_n);
                                return code;
                        }
#endif
#endif
                        saved_b--;
                        return code;
                  case PGUP :
                                return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef CB_INSTALLED
void ecbp_init_display(BYTE n){


        WINDOW *w;
        int b,s,p;
        unsigned int a;
        int row_offset,no_field;
        unsigned int e,d;
        unsigned long h;
        FIELD *f,*tf;
        int max_dsply;
        s=no_cb[n] / NO_DISPLAY_ON_PAGE;
        p=no_cb[n] % NO_DISPLAY_ON_PAGE;
        if ( saved_b == 0 ){
                saved_a=first_cb_no;
#ifdef EM_GROUP_PROGRAM
                h=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_em_group[n] * 2);

#elif (defined  NO_TIE_INIT)
                h=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8;
#else
                h=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8;
#endif
                fseek(fp[n],h,SEEK_SET);
                first_cb_no+=no_cb[n];
        }
        a=saved_a; // current ext. no.
        b=saved_b; // current page no.
        if (b != s)
                w=enw(0,0,21,80);
        else{
                if ( p > 0){
                        w=enw(0,0,p+5,80);
                }
                else{
                        w=enw(0,0,21,80);
                }
        }
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
        if(w == NULL){
                caw();  cs();
                print_error_report("\nDEP,exp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
        swt(w," EDIT COIN BOX PARAMETERS ");
        draw_window(w);
        wprompt(w,0,0,"     Coin box                                cst                              ");

        wprompt(w,0,1,"                         AA SA HL TR PU T0 CF CR DD IN ST FM CP CB            ");
        wprompt(w,0,2,"------------------------------------------------------------------------------");

        row_offset=3;

        /* field allocation */
        idew(w);
        for(d=0;d < max_dsply;d++){

                f=enf(w,26,row_offset+d,cstm,xel[d].cst,BOOLEAN);
                if(f == NULL){
                        caw(); cs();
                        print_error_report("\nDEP,exp3:error in memory allocation!");
                        quit(1);
                }
        }
        no_field=1;

        /* linking up & down fields */
        for(d=0,f=w->ff;d < ( max_dsply * no_field );d++,f=f->fnxt){
                /* 5 = number of field in a line */

                for(e=0,tf=f;(e < no_field) && (tf != NULL);e++,tf=tf->fnxt);
                /* 5 = number of field in a line */
                f->fdn=tf;

                for(e=no_field,tf=f;e && (tf != NULL);e--,tf=tf->fprv);
                /* 5 = number of field in a line */
                f->fup=tf;
        }
        saved_win=w;
        saved_a=a;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef CB_INSTALLED
void ecbp_part1(BYTE n){
        WINDOW *w;
        int b,c,s,p;
        unsigned int a;
        long h;
        unsigned int d;
        char *g;
        int max_dsply;

        b=saved_b;
        w=saved_win;
        a=saved_a;
        s=no_cb[n] / NO_DISPLAY_ON_PAGE;
        p=no_cb[n] % NO_DISPLAY_ON_PAGE;
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
        caf(w);
        wcursor(w,76,2);
        h=ftell(fp[n]);
        fseek(fp[n],h,SEEK_SET);
        for(c=0;c < max_dsply;c++,a++){
//              wprintf(w,"\n     %1.3d",a+FIRST_CB_NO);
                wprintf(w,"\n%-6.0ld",a+first_cb_number);
                d=my_getw(fp[n]);
                prv_xel[c].cstv=d;
                xel[c].cstv=d;
                g=xel[c].cst;

                if(d & AMPLIFIER_ACCESS)        *g='|';
                else    *g='-';
                g++;

                if(d & SIREN_ACCESS)    *g='|';
                else    *g='-';
                g++;

                if(d & HOT_LINE_ACCESS) *g='|';
                else    *g='-';
                g++;

                if(d & RING_AGAIN_TIET) *g='|';
                else    *g='-';
                g++;

                if(d & CALL_PICKUP_USE) *g='|';
                else    *g='-';
                g++;

                if(d & TIE_0)   *g='|';
                else    *g='-';
                g++;

                if(d & CONFERENCE_SERVICE)      *g='|';
                else    *g='-';
                g++;

                if(d & RING_AGAIN_COT)  *g='|';
                else    *g='-';
                g++;

                if(d & DONT_DIST)       *g='|';
                else    *g='-';
                g++;

                if(d & INTERNATIONAL)   *g='|';
                else    *g='-';
                g++;

                if(d & STD)     *g='|';
                else    *g='-';
                g++;

                if(d & FOLLOW_ME)       *g='|';
                else    *g='-';
                g++;

                if(d & CAMP_ON) *g='|';
                else    *g='-';
                g++;

                if(d & CALL_BACK)       *g='|';
                else    *g='-';
        }

        data_entry_init(saved_win);
        saved_a=a;
        saved_h=h; // current file position
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef CB_INSTALLED
void ecbp_part2(BYTE n){

        int b,c,s,p;
        long h;
        unsigned int d;
        char *g;
        int max_dsply;
        unsigned int ext_indx;

        b=saved_b;
        h=saved_h;
        s=no_cb[n] / NO_DISPLAY_ON_PAGE;
        p=no_cb[n] % NO_DISPLAY_ON_PAGE;
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
        fseek(fp[n],h,SEEK_SET);
        for(c=0;c <max_dsply;c++){

                d=xel[c].cstv;
                g=xel[c].cst;

                if(*g == '|')   d |= AMPLIFIER_ACCESS;
                else    d &= ~AMPLIFIER_ACCESS;
                g++;

                if(*g == '|')   d |= SIREN_ACCESS;
                else    d &= ~SIREN_ACCESS;
                g++;

                if(*g == '|')   d |= HOT_LINE_ACCESS;
                else    d &= ~HOT_LINE_ACCESS;
                g++;

                if(*g == '|')   d |= RING_AGAIN_TIET;
                else    d &= ~RING_AGAIN_TIET;
                g++;

                if(*g == '|')   d |= CALL_PICKUP_USE;
                else    d &= ~CALL_PICKUP_USE;
                g++;

                if(*g == '|')   d |= TIE_0;
                else    d &= ~TIE_0;
                g++;

                if(*g == '|')   d |= CONFERENCE_SERVICE;
                else    d &= ~CONFERENCE_SERVICE;
                g++;

                if(*g == '|')   d |= RING_AGAIN_COT;
                else    d &= ~RING_AGAIN_COT;
                g++;

                if(*g == '|')   d |= DONT_DIST;
                else    d &= ~DONT_DIST;
                g++;

                if(*g == '|')
                        d |= INTERNATIONAL;
                else    d &= ~INTERNATIONAL;
                g++;

                if(*g == '|')   d |= STD;
                else    d &= ~STD;
                g++;

                if(*g == '|')   d |= FOLLOW_ME;
                else    d &= ~FOLLOW_ME;
                g++;

                if(*g == '|')   d |= CAMP_ON;
                else    d &= ~CAMP_ON;
                g++;

                if(*g == '|')   d |= CALL_BACK;
                else    d &= ~CALL_BACK;

                xel[c].cstv=d;
                if ( d != prv_xel[c].cstv){
                        ext_indx=(saved_a-max_dsply)+c;
                        send_cb_cstv_pckt(ext_indx,c);
                }
                my_putw(d,fp[n]);

        }

   return;
}
#endif

#endif // (OS == DOS) || (OS == LINUX_COMPATIBLE)

#endif // NEW_DEP
