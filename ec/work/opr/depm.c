            /****  display & edit parameters file  ****/

#include "omtfp.h"


#ifdef NEW_DEP

#if (OS == DOS)
#include <conio.h>
#include <process.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "wg.h"
#include "depmtfp.h"
#include "depv.h"

#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
extern BYTE exp_part_flag;
extern BYTE ep_2nd_menu_flag;
extern BYTE data_entry_flag;
extern BYTE read_field_flag;
extern BYTE error_message_flag;
extern BYTE dep_quit_flag;
extern BYTE non_error_message_flag;
extern int saved_b;
extern BYTE getchar_hndlr_flag;
extern WINDOW *cqw_win;
extern WINDOW *this_win;
extern WINDOW *saved_win;
extern int saved_b;
extern int saved_a;
extern WINDOW *ewnd;  /* error window pointer */
extern long alarm_line_ptr[];
extern long malicious_line_ptr[];

extern int from_internal_to_dialled_no[];
extern int from_dialled_to_internal_no[];
#ifdef DAY_NIGHT_PARAMETERS_FILE
extern char *pfna[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE];
extern char *pfna_bak[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE];
extern char *pfna_bak2[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE];
#else
extern char *pfna[MAX_NO_OPR_CARD][1];
extern char *pfna_bak[MAX_NO_OPR_CARD][1];
extern char *pfna_bak2[MAX_NO_OPR_CARD][1];
#endif
extern BYTE dep_flag;
extern BYTE stm;
extern WINDOW *ktbg_w;
#if ((defined ALARM_ENABLE) && (TON == ALARM_OPR))
extern FILE *flog;
#endif
extern INDEX_TYPE hldl[],pl[];
extern BYTE hkl[];
extern int no_extension[];
extern int no_my_extension[];
extern int no_co[];
extern int no_tie[];
extern int no_em[];
extern int no_cas3[];
extern int no_cas1[];
extern int no_em_group[];
extern int no_cb[];
#ifdef BATCH_PROGRAMMING
extern unsigned char batch_ep_2nd_menu_flag;
extern BYTE batch_ep_flag;
#endif
extern unsigned long first_directory_decimal;
#ifdef DAY_NIGHT_PARAMETERS_FILE
extern BYTE saved_day_file_no;
#endif

//#if ((defined CALL_PROCESSOR) && (defined EXT_INSTALLED))
extern unsigned int no_of_subscriber_groups;
extern SUBSCRIBER_GROUP subscriber_group[MAX_NO_OF_SUBSCRIBER_GROUPS];
//#endif

/*===========================================================================*/
/*===========================================================================*/
void eeph(void){
        switch (get_c()){
           case 'c':
           case 'C':
           case 'q':
           case 'Q':
                 getchar_hndlr_flag=RESET;
                 deletw(saved_win);
                 deletw(cqw_win);
                 ep_2nd_menu_flag = RESET;
                 ep_1st_menu_init();
                 break;
           default : break;
        }
}
void dep_init(void){
        int i,a;
        dep_flag=RESET;
#ifdef BATCH_PROGRAMMING
        batch_ep_flag=RESET;
#endif
        init_mac_nite_arr();
        for(a=0;a<NO_DISPLAY_ON_PAGE;a++){
                for(i=0;i<8;i++)
                        xel[a].ctg[i]=0;
                for(i=0;i<8;i++)
                        xel[a].ttg[i]=0;
                for(i=0;i<17;i++)
                        xel[a].emtg[i]=0;
                for(i=0;i<15;i++)
                        xel[a].cst[i]=0;
                for(i=0;i<4;i++)
                        xel[a].ast[i]=0;
                for(i=0;i<3;i++)
                        xel[a].hli[i]=0;
                for(i=0;i<11;i++)
                        tel[a].ddb[i]=0;
                for(i=0;i<5;i++)
                        tel[a].t1[i]=0;
                for(i=0;i<5;i++)
                        tel[a].t2[i]=0;
#ifdef X_RAILWAY
                for(i=0;i<3;i++)
#else
                for(i=0;i<2;i++)
#endif
                        tel[a].gn[i]=0;
                for(i=0;i<3;i++)
                        dtel[a].gn[i]=0;
        }
        for(a=0;a<NO_OPR_NITX;a++){
                for(i=0;i<11;i++)
                        nel[a][i]=0;
        }
        for(a=0;a<NO_HOT_LINE;a++){
                for(i=0;i<4;i++)
                        hlel[a].dht[i]=0;
                for(i=0;i<11;i++)
                        hlel[a].dhi[i]=0;
        }
        wvi();  /* window variable initialization */
}
/* ========================================================================*/
void dep_hndlr_init(void){
        int i;
        exp_part_flag=0;
        saved_b=0;
        saved_n=0;
        ep_2nd_menu_flag=RESET;
        data_entry_flag=RESET;
        read_field_flag=RESET;
        ep_flag=RESET;
#ifdef BATCH_PROGRAMMING
        batch_ep_flag=RESET;
        batch_ep_2nd_menu_flag=RESET;
#endif
        getchar_hndlr_flag=RESET;
        error_message_flag=RESET;
        dep_quit_flag=RESET;
        last_s=RESET;
        cpfoq_flag=RESET;
        cpw_flag=RESET;
        dep_1st_entry_flag=SET;

        dep_flag=SET;
#ifdef DEP_MANUAL_TEST
        test_flag=RESET;
        test_2nd_menu_flag=RESET;
        tst.prty_type=TEST_NULL_T;
        tst.result=TEST_NO_ANSWER;
        test_menu_flag=RESET;
#endif
        for(i=0;i< 3 ;i++){
#ifndef LIM
            if(i==1) break;
#elif (NO_EXTENSION_C == 0)
            if(i==2) break;
#endif
            fp[i]=NULL;
        }

#ifdef AUTO_UPLOAD
        file_changed=RESET;
#endif
        ktbg();
#ifdef DAY_NIGHT_PARAMETERS_FILE
        cpfoq();  /* choose parameters file or quit */
#else
        open_param_files(1);
        if(dep_quit_flag == RESET)
                cpw_init_display();
#endif
}
/*===========================================================================*/
/*===========================================================================*/
extern DATTIM sdt;  /* system date time */
void backup_pfna(int i){
        char buf[80],buf2[10];
        strcpy(buf,"copy ");
        strcat(buf,pfna[i][0]); // assuming only day file
        strcat(buf," ");

//      strcat(buf,pfna_bak[i][0]); // assuming only day file

        sdt=*get_dt(&sdt);
        sprintf(buf2,"%02x%02x%02x%02x",sdt.mo,sdt.da,sdt.hr,sdt.mi);
        strcat(buf,buf2);
        strcat(buf,pfna_bak2[i][0]); // assuming only day file

        strcat(buf," /Y > p");
//DOOD : system->kt_system
        kt_system(buf);
}
/*===========================================================================*/
/*===========================================================================*/
void open_param_files(int a){
        int b,s;
        int i;
        FILE *afp;  /* auxiliary file pointer */
        a--;
        if(a >= NO_LOAD_PARM_FILE)
                return ;
        for(i=0;i< 3 ;i++){
#ifndef LIM
                if(i==1) break;
#elif (NO_EXTENSION_C == 0)
                if(i==2) break;
#endif
                if (fp[i] != NULL){
                        fclose(fp[i]);
                        fp[i]=NULL;
                }
                afp=fopen(pfna[i][a],"r+b");
                if(afp == NULL){
                        error_message("cannot open parameters file");
                        dep_quit_flag=SET;
                        return;
                }
                fp[i]=afp;
        }
        /* installation name */
        fseek(fp[0],0,SEEK_SET);
        for(b=0;(a=fgetc(fp[0])) != 0x1a;b++)
                n[b]=a;
        n[b]='\0';
        /* password */
        fseek(fp[0],MAX_NAME_LENGHT,SEEK_SET);
        a=my_getw(fp[0]);
        for(p=0,s=12,b=0;b < 4;b++,s -=4){
                if((a & 0x0f) != 0x0a)
                        p |= (a & 0x0f) << s;
                a >>= 4;
        }

}
/*===========================================================================*/
/*===========================================================================*/
void dep_main(void){
        int b=0,s;
#ifdef DAY_NIGHT_PARAMETERS_FILE
        BYTE a;
#endif
        if (dep_flag == RESET) return;

        if (cpw_flag == SET ){  // check password
                cpw();
                return;
        }
        if (dep_quit_flag == SET){
                if (get_c() == NO_KEY_PRESSED){
                        return;
                }
                if (ewnd != NULL) // error window pointer
                        clear_message(); // if error
                dep_quit_flag=RESET;
                dep_quit();
                return;
        }
        if ((error_message_flag == SET) || (non_error_message_flag == SET)){
                if (get_c() == NO_KEY_PRESSED){
                        return;
                }
                if (ewnd != NULL) // error window pointer
                        clear_message(); // if error
                error_message_flag=RESET;
                if (non_error_message_flag == SET){
                        saved_a1=dmmrc();       // display main menu
                        non_error_message_flag=RESET;
                }
                return;
        }
#ifdef DAY_NIGHT_PARAMETERS_FILE
        if ( cpfoq_flag == SET){
                a=get_selection(saved_win,3,"DNQ");
                if ( a == NO_KEY_PRESSED){
                        return;
                }
                else{
                        cpfoq_flag=RESET;
                        deletw(saved_win);
                        if (dep_1st_entry_flag == SET){
                                if ( a >= 3 ){
                                        dep_quit();
                                        return;
                                }
                                cpw_init_display();
                                dep_1st_entry_flag=RESET;
                                open_param_files(a);
                                saved_day_file_no=a;
                        }
                        else{
                                saved_a1=dmmrc();
                                saved_a1->sp=1;
                                if(a < 3){
                                        open_param_files(a);
                                        saved_day_file_no=a;
                                }
                        }
//                      open_param_files(a);
//                      saved_day_file_no=a;
                        return;
                }
        }
#endif
        if (file_viewer_flag == SET){
                file_viewer();
                if (file_viewer_flag == RESET)
                         saved_a1=dmmrc();
                return;
        }
        if (getchar_hndlr_flag == SET){
                switch (last_s){
                  case 8 : /* ddhl()*/
                  case 1 :    // din()
                  case 2 :    // dpw() or mnul_tst
                  case 7 :    // donx()
                  case 9 :    // deph()
                  case 15:    // v_office_code()
                          switch (get_c()){
                             case 'c':
                             case 'C':
                             case 'q':
                             case 'Q':
                                   getchar_hndlr_flag=RESET;
                                   deletw(saved_win);
                                   deletw(cqw_win);
                                   saved_a1=dmmrc();
                                   break;
                             default : break;
                           }
                           break;
                  case 13 : if (get_c() != NO_KEY_PRESSED){
                                   if (ewnd != NULL)
                                          clear_message(); // if error
                                   ktbg();
                                   getchar_hndlr_flag=RESET;
                                   saved_a1=dmmrc();
                             }
                             break;
                  case 3  :  if ((dxp(saved_n) == ESC)){
                                    saved_a1=dmmrc();
                             }
                            break;
#ifdef EM_GROUP_PROGRAM
                  case 4:    if ((demtp(saved_n) == ESC)){
                                    saved_a1=dmmrc();
                             }
                             break;
#endif
#ifndef NO_TIE_INIT
                  case 4:    if ((dttp(saved_n) == ESC)){
                                    saved_a1=dmmrc();
                             }
                            break;
#endif
#if (NO_CO != 0)
                  case 5  :  if ((dctp(saved_n) == ESC)){
                                    saved_a1=dmmrc();
                             }
                            break;
#endif
#ifdef REPORT_BULK_BILLING
                  case 12  :  if ((vbbf() == ESC)){
                                    fclose(fp1);
                                    saved_a1=dmmrc();
                             }
                            break;
#endif
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
                  case 6  :  if ((ddtp(saved_n) == ESC)){
                                    saved_a1=dmmrc();
                             }
                            break;
#endif
#ifdef CB_INSTALLED
                  case 18 :  if ((dcbp(saved_n) == ESC)){
                                    saved_a1=dmmrc();
                             }
                            break;
#endif
                  case 10:  // ep(), deph()
                            switch (get_c()){
                               case 'c':
                               case 'C':
                               case 'q':
                               case 'Q':
                                     getchar_hndlr_flag=RESET;
                                     deletw(saved_win);
                                     deletw(cqw_win);
                                     ep_2nd_menu_flag = RESET;
                                     ep_1st_menu_init();
                                     break;
                               default : break;
                            }
                            break;
                  default : break;
                }
                return;
        }
        if (ep_flag != RESET){
                ep();
                /* exit from ep */
                if (ep_flag == RESET){
                         saved_a1=dmmrc();
                }
                return;
        }
#ifdef BATCH_PROGRAMMING
        if (batch_ep_flag != RESET){
                batch_ep();
                /* exit from ep */
                if (batch_ep_flag == RESET){
                         saved_a1=dmmrc();
                }
                return;
        }
#endif
#ifdef DEP_MANUAL_TEST
        if (test_flag != RESET){
                test_menu();
                /* exit from ep */
                if (test_flag == RESET){
                         saved_a1=dmmrc();
                }
                return;
        }
#endif
#ifdef DEP_MANUAL_TEST
        s=get_selection(saved_a1,1,"IUEMCGNHLDABVFOWTXQ");
#else
        s=get_selection(saved_a1,1,"IPEMCGNHLDABVFOWTXQ");
#endif
        if (s == NO_KEY_PRESSED){
            return;
        }
        else{
                deletw(saved_a1);
        }
        last_s=s;
        switch(s){
                case 1: b=din();getchar_hndlr_flag=SET;
                        return;
                case 2:
#ifdef DEP_MANUAL_TEST
                        b=test_1st_menu();
                        test_flag=SET;
#else
                        b=dpw();getchar_hndlr_flag=SET;
#endif
                        return;
                case 3:
                        {fdb=0;
                         saved_n=0;
                         getchar_hndlr_flag=SET;
                         dxp_init(saved_n);
                         return;
                         }
#ifdef EM_GROUP_PROGRAM
                case 4:
                        {fdb=0;
                         saved_n=0;
                         getchar_hndlr_flag=SET;
                         demtp_init(saved_n);
                         return;
                         }
#else
                case 4:
#ifndef NO_TIE_INIT
                        {
                         saved_n=0;
                         getchar_hndlr_flag=SET;
                         dttp_init(saved_n);
                         return;
                         }
#else
                        break;
#endif

#endif
                case 5:
#if (NO_CO != 0)
                        {fdb=0;
                         saved_n=0;
                         getchar_hndlr_flag=SET;
                         dctp_init(saved_n);
                         return;
                         }
#else
                        break;
#endif
                case 6:

#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
                        {fdb=0;
                         saved_n=0;
                         getchar_hndlr_flag=SET;
                         ddtp_init(saved_n);
                         return;
                         }
#else
                        break;
#endif
                case 7:
#if (NO_ACTIVE_OPR != 0)
                        b=donx(0);getchar_hndlr_flag=SET;
                        return;
#else
                        break;
#endif
                case 8:
#ifdef HOT_LINE_SERVICE_ACTIVE
                        b=ddhl(0);
                        saved_n=0;
                        getchar_hndlr_flag=SET;
                        return;
#else
                        break;
#endif
                case 9: b=deph();getchar_hndlr_flag=SET;
                        return;
                case 10:
                         b=ep_1st_menu();
                         ep_flag=SET;
                         return;
                case 11:
#ifdef DAY_NIGHT_PARAMETERS_FILE
                         b=cpfoq();
                         return;
#else
                         break;
#endif
                case 12:
#ifdef REPORT_BULK_BILLING
                         {
                         getchar_hndlr_flag=SET;
                         vbbf_init();
                         return;
                         }
#else
                         break;
#endif
                case 13:
#ifdef MALICIOUS_PRINT_ON_U
                         if (malicious_fp != NULL){
                                 init_malicious_file_data();
                                 file_view_init();
                         }
                         else
                                  saved_a1=dmmrc();
                         return;
#else
                         break;
#endif
                case 14:
#ifdef REPORT_BULK_BILLING
                         b=cnvrtbbf();
                         return;
#else
                         break;
#endif
                case 15: b=v_office_code();
                         getchar_hndlr_flag=SET;
                         return;
                case 16:
#if ((defined ALARM_ENABLE) && (TON == ALARM_OPR))
                         if (flog != NULL){
                                 init_alarm_file_data();
                                 file_view_init();
                         }
                         else
                                  saved_a1=dmmrc();
                         return;
#else
                         break;
#endif
                case 17:
#ifdef BATCH_PROGRAMMING
                         b=batch_ep_1st_menu();
                         batch_ep_flag=SET;
                         return;
#else
                         break;
#endif
                case 18:
#ifdef CB_INSTALLED
                        first_cb_no=0;
                        saved_n=0;
                        getchar_hndlr_flag=SET;
                        dcbp_init(saved_n);
                        return;

#else
                        break;
#endif
                case 19: b='q';break;
                default: /*b='q';*/ break;
        }
        saved_a1=dmmrc();
        if( (b == 'q') || (b == 'Q') ){
                dep_quit();
        }
}

WINDOW *dmmrc(){  /* display main menu & return choice */
        WINDOW *a;

        a=enw(22,0,21,39);
        if(a == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,dmmrc:error in memory allocation!");
                quit(1);
        }
        dw(a);
        swb(a,DOUBLE_LINE);

//DOOD start
#if 0
        swc(a,ALL,BLUE,WHITE,BRIGHT);
        swc(a,ACCENT,RED,WHITE,BRIGHT);
#endif
        swc(a,ALL,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
        swc(a,ACCENT,KT_RED,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(a,"PARAMETERS FILE DISPLAY & EDIT");
        draw_window(a);
//DOOD start
//      swucf(a,GREEN);
        swucf(a,KT_GREEN);
//DOOD end
        wprompt(a,1,0,"1-Installation name.");
#ifdef DEP_MANUAL_TEST
        wprompt(a,1,1,"2-manUal test.");
#else
        wprompt(a,1,1,"2-Password.");
#endif
        wprompt(a,1,2,"3-display Extensions parameters.");
#ifdef EM_GROUP_PROGRAM
        wprompt(a,1,3,"4-display e&M trunk parameters.");
#else
#ifndef NO_TIE_INIT
        wprompt(a,1,3,"4-display tie trunk paraMeters.");
#else
        wprompt(a,1,3,"4-not used.");
#endif
#endif
#if (NO_CO!=0)
        wprompt(a,1,4,"5-display Co trunk parameters.");
#else
        wprompt(a,1,4,"5-not used.");
#endif
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
        wprompt(a,1,5,"6-display diGital trunk parameters.");
#else
        wprompt(a,1,5,"6-not used.");
#endif
#if (NO_ACTIVE_OPR != 0)
        wprompt(a,1,6,"7-operator's Night extensions.");
#else
        wprompt(a,1,6,"7-not used.");
#endif
#ifdef HOT_LINE_SERVICE_ACTIVE
        wprompt(a,1,7,"8-destination Hot lines.");
#else
        wprompt(a,1,7,"8-not used.");
#endif
        wprompt(a,1,8,"9-heLp on extensions parameters.");
        wprompt(a,1,9,"10-eDit parameters.");
#ifdef DAY_NIGHT_PARAMETERS_FILE
        wprompt(a,1,10,"11-chAnge parameters file.");
#else
        wprompt(a,1,10,"11-not used.");
#endif
#ifdef REPORT_BULK_BILLING
        wprompt(a,1,11,"12-view Bulk billing file.");
#else
        wprompt(a,1,11,"12-not used.");
#endif
#ifdef MALICIOUS_PRINT_ON_U
        wprompt(a,1,12,"13-View malicious info. file.");
#else
        wprompt(a,1,12,"13-not used.");
#endif
#ifdef REPORT_BULK_BILLING
        wprompt(a,1,13,"14-convert bulk billing File.");
#else
        wprompt(a,1,13,"14-not used.");
#endif
#ifdef R1_SIGNALINGP
        wprompt(a,1,14,"15-Office code.");
#else
        wprompt(a,1,14,"15-not used.");
#endif
#if ((defined ALARM_ENABLE) && (TON == ALARM_OPR))
        wprompt(a,1,15,"16-vieW alarm log file.");
#else
        wprompt(a,1,15,"16-not used.");
#endif

#ifdef BATCH_PROGRAMMING
        wprompt(a,1,16,"17-ediT parameters in batch mode.");
#else
#ifdef DETAIL_BILLING
        wprompt(a,1,16,"17-view deTail billing file.");
#else
        wprompt(a,1,16,"17-not used.");
#endif
#endif // BATCH_PROGRAMMING
#ifdef CB_INSTALLED
        wprompt(a,1,17,"18-display coin boX parameters.");
#else
        wprompt(a,1,17,"18-not used.");
#endif
        wprompt(a,1,18,"19-Quit.");
        return a;
}


int din(){  /* display installation name */
        WINDOW *a;
        int b;
        a=enw(28,8,3,24);
        if(a == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,din:error in memory allocation!");
                quit(1);
        }
        dw(a);
        swb(a,DOUBLE_LINE);
//DOOD start
//        swc(a,ALL,YELLOW,WHITE,BRIGHT);
        swc(a,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(a,"INSTALLATION NAME");
        draw_window(a);
        b=22 - strlen(n);
        b /= 2;
        for(;b;b--)
                wputchar(a,' ');
        wprintf(a,"%s",n);
        saved_win=a;
        getchar_hndlr_flag=SET;
        cqw();
        return b;
}

int dpw(){  /* display password */
        WINDOW *a;
        a=enw(32,11,3,12);
        if(a == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,dpw:error in memory allocation!");
                quit(1);
        }
        dw(a);
        swb(a,DOUBLE_LINE);
//DOOD start
//      swc(a,ALL,RED,WHITE,BRIGHT);
        swc(a,ALL,KT_RED,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(a,"PASSWORD");
        draw_window(a);
        wputchar(a,'*');
        wputchar(a,'*');
        wputchar(a,' ');
        wprintf(a,"%x",p);
        wputchar(a,' ');
        wputchar(a,'*');
        wputchar(a,'*');
        saved_win=a;
        getchar_hndlr_flag=SET;
        cqw();
        return OK;
}

void cqw(void){  /* continue or quit window */
        WINDOW *a;
        a=enw(45,22,3,23);
        if(a == NULL)
                return;
        dw(a);
        swb(a,DOUBLE_LINE);
//DOOD start
//      swc(a,ALL,AQUA,WHITE,BRIGHT);
        swc(a,ALL,KT_CYAN,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        draw_window(a);
        wprompt(a,1,0,"cont. or quit:c\\q?");
        wcursor(a,22,0);
        getchar_hndlr_flag = SET;
        cqw_win=a;
        return;
}

#if (NO_ACTIVE_OPR != 0)
int donx(BYTE n){  /* display operator's night extensions */
        WINDOW *w;
        int a;
        unsigned int b;

#ifdef EM_GROUP_PROGRAM

        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#elif (defined NO_TIE_INIT)

        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2);
#endif
        fseek(fp[n],a,SEEK_SET);
        w=enw(16,0,NO_OPR_NITX+4,47);
        if(w == NULL){
                caw();  cs();
                print_error_report("\nDEPM,donx:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swt(w,"OPERATOR'S NIGHT");
        draw_window(w);
        wprompt(w,0,0,"operator's night ext.\t\text. #");
        wprompt(w,0,1,"---------------------------------------------");
        for(a=0;a < NO_OPR_NITX;a++){
                wprintf(w,"\n\t\t%1.1d\t\t",a+1);
                b=my_getw(fp[n]);
                if( (b & 0x0f) != XST_T)
                        wprintf(w,"     \tno night programmed");
                else{
                        b >>= 4;
//                      wprintf(w,"\t\t\t %-6.0ld",dep_find_subscriber_no(b));
                        wprintf(w,"\t\t\t %-10.0ld",dep_find_subscriber_no(b));
                }
        }


        saved_win=w;
        getchar_hndlr_flag=SET;
        cqw();
        return 'l';
}
#endif

#ifdef HOT_LINE_SERVICE_ACTIVE
int ddhl(BYTE n){  /* display destination hot line */
        WINDOW *w;
        int a,c;
        long b;

#ifdef EM_GROUP_PROGRAM

        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (no_cas3[n] * 2) + (no_cas1[n] * 2) + (NO_OPR_NITX_PARAMS * 2);
#endif
        fseek(fp[n],a,SEEK_SET);
        w=enw(17,0,NO_HOT_LINE+4,46);
        if(w == NULL){
                caw();  cs();
                print_error_report("\nDEPM,ddhl:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
        swt(w,"HOT LINES");
        draw_window(w);

        wprompt(w,0,0,"dest. hot line\t\tdest. type\t  dest. #");
        wprompt(w,0,1,"--------------------------------------------");
        for(a=0;a < NO_HOT_LINE ;a++){
                wprintf(w,"\n\t   %1.1d\t\t",a+1);
                b=my_getw(fp[n]);
                c=(unsigned)b & 0x0f;
                b >>= 4;
                if(c == NULL_T){
                        wprintf(w,"\tno hot line programmed");
                        continue;
                }
                if(c == XST_T){
                        wprintf(w,"  \t\tEXT \t");
                        b = dep_find_subscriber_no(b);
//                      wprintf(w,"\t%-6.0ld",b);
                        wprintf(w,"\t%-10.0ld",b);
                        continue;
                }
                if(c == CO_T){
                        wprintf(w,"   \t\tCO \t\t");
                        b++;
                        /* for displaying trunk # from 1 instead of 0 */
                }
                else
                        if(c == TIE_T){
                                wprintf(w,"    \tTIE\t\t");
                                b++;
                                /* for displaying trunk # from 1 instead of 0 */
                        }
                        else
                                wprintf(w,"   \t\t%1.4x\t",c);
                wprintf(w,"\t%1.3d",b);
        }
        cqw();
        saved_win=w;
        getchar_hndlr_flag=SET;
        return 'l';
}
#endif

int deph(void){  /* display extensions parameters help */
        WINDOW *w;
//      w=enw(21,0,19,37);
        w=enw(21,0,19,46);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,deph:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,MAGENTA,WHITE,BRIGHT);
        swc(w,ALL,KT_MAGENTA,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"PARAMETERS HELP");
        draw_window(w);
        wprompt(w,1,0,"AA: home metering/paging");
#ifdef BAD_PAYER
        wprompt(w,1,1,"B1: bad payer warning");
#else
        wprompt(w,1,1,"SA: LD trunk/siren access");
#endif
//        wprompt(w,1,2,"HL: clip/hot line");
        wprompt(w,1,2,"HL: clip");
//        wprompt(w,1,3,"TR: TIE trunk reservation");
        wprompt(w,1,3,"TR: DTMF dialling allowed");
        wprompt(w,1,4,"PU: wake up/call pick up");
#ifdef BAD_PAYER
//      wprompt(w,1,5,"B2: bad payer one-way block");
        wprompt(w,1,5,"B2: bad payer one-way block/primary access");
#else
//      wprompt(w,1,5,"T0: E&M Trunk/dial 0 on TIE");
        wprompt(w,1,5,"T0: E&M Trunk/dial 0 on TIE/primary access");
#endif
        wprompt(w,1,6,"CF: conference");
#ifdef BAD_PAYER
        wprompt(w,1,7,"B3: bad payer bothway block");
#else
        wprompt(w,1,7,"CR: CO trunk reservation");
#endif
        wprompt(w,1,8,"DD: do not disturb");
        wprompt(w,1,9,"IN: international call access");
        wprompt(w,1,10,"ST: national call access");
        wprompt(w,1,11,"FM: call forward/follow me");
        wprompt(w,1,12,"CP: call waiting/camp on");
        wprompt(w,1,13,"CB: clir");
        wprompt(w,1,14,"FA: features active");
        wprompt(w,1,15,"MC: malicious call detection");
        wprompt(w,1,16,"CG: call generation");
        saved_win=w;
        getchar_hndlr_flag=SET;
        cqw();
        return 'c';
}

int cpfoq(){  /* choose parameters file or quit */
        WINDOW *w;
        w=enw(26,8,5,27);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,cpf:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,MAGENTA,WHITE,BRIGHT);
        swc(w,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
        swc(w,ALL,KT_MAGENTA,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"CHOOSE PARAMETERS FILE");
        draw_window(w);
//DOOD start
//      swucf(w,GREEN);
        swucf(w,KT_GREEN);
//DOOD end
        wprompt(w,1,0,"Day parameters file.");
        wprompt(w,1,1,"Night parameters file.");
        wprompt(w,1,2,"Quit.");
        saved_win=w;
        cpfoq_flag=SET;
        return 0;

}

int cnvrtbbf(void){ /* convert bulk billing file (to ASCII) */
#ifdef REPORT_BULK_BILLING
        unsigned int s;
        unsigned int counts[2];
        unsigned int extra_counts[2];
        FILE *i,*o;
        i=fopen(BULK_BILLING_FILE,"rb");
        if(i == NULL){
                error_message("Cannot open bulk billing file.");
                return 'c';
        }
        o=fopen(BULK_BILLING_ASCII_FILE,"wt");
        if(o == NULL){
                fclose(i);
                error_message("Cannot convert bulk billing file.");
                return 'c';
        }
        fprintf(o,"\nSubscriber   STD      Local    Extra      L+E      S+L+E     Intern.   ");
        fprintf(o,"\n-----------------------------------------------------------------------");
        for(s=0;s<NO_TOTAL_EXTENSION;s++){
                fseek(i,(s << 2),SEEK_SET);
                if(fread(counts,2,2,i) != 2) break;
                fseek(i,(s << 2) + (NO_TOTAL_EXTENSION << 2),SEEK_SET);
                if(fread(extra_counts,2,2,i) != 2) break;

                fprintf(o,"\n   %-6.0ld: %6.6u    %6.6u",dep_find_subscriber_no(s),counts[0],counts[1]);
                fprintf(o,"    %6.6u    %6.6u",extra_counts[0],counts[1]+extra_counts[0]);
                fprintf(o,"    %6.6u    %6.6u",counts[0]+counts[1]+extra_counts[0],extra_counts[1]);
        }
        fprintf(o,"\n\n");
        fclose(i);
        fclose(o);
        non_error_message("Conversion Done.");
#endif
        return 'c';
}

int vdbf(void){
        char dbfvs[80];
        char s[10];
        unsigned long i;

        i=first_directory_decimal;
        ltoa(i,s,10);
        strcpy(dbfvs,DETAIL_BILLING_VIEWER);
        strcat(dbfvs," ");
        strcat(dbfvs,s);
        strcat(dbfvs," ");
//DOOD : itoa -> kt_itoa
        kt_itoa(NO_TOTAL_EXTENSION,s,10);
        strcat(dbfvs,s);
//DOOD : system->kt_system
        if(kt_system(dbfvs) != 0){
                error_message("Cannot run detail billing viewer!");
                get_char();
                clear_message();
        }
        ktbg();
        return 'c';
}

int vmcf(void){ /* view malicious calls file */
#ifdef MALICIOUS_PRINT_ON_U
        char mcfvs[80]; /* malicious call file view string */
        strcpy(mcfvs,"wpview ");
        strcat(mcfvs,MALICIOUS_FILE);
//DOOD : system->kt_system
        if(kt_system(mcfvs) != 0){
                error_message("Cannot open malicious info. file.");
                getchar_hndlr_flag=SET;
                return 'c';
        }
#endif
        return 'c';
}

int v_office_code(void){ /* view office code */
#ifdef R1_SIGNALINGP
/* size of office code block in file is 6 bytes, 3 words */
        int a;
        WINDOW *w;
        int b;
        char office_code[7];

#ifdef EM_GROUP_PROGRAM
        a=MAX_NAME_LENGHT + 2 + (no_extension[0] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[0] * 8) + (no_co[0] * 4) + (no_cas3[0] * 2) + (no_cas1[0] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_em_group[0] * 2) + (no_cb[0] * 2) ;

#elif (defined  NO_TIE_INIT)
        a=MAX_NAME_LENGHT + 2 + (no_extension[0] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[0] * 4) + (no_cas3[0] * 2) + (no_cas1[0] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_cb[0] * 2) ;
#else
        a=MAX_NAME_LENGHT + 2 + (no_extension[0] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[0] * 4) + (no_co[0] * 4) + (no_cas3[0] * 2) + (no_cas1[0] * 2) + (NO_OPR_NITX_PARAMS * 2) + 8 + (no_cb[0] * 2) ;
#endif
        fseek(fp[0],a,SEEK_SET);

        w=enw(32,8,3,15);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,v_office_code:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"OFFICE CODE");
        draw_window(w);

        b=my_getw(fp[0]);
        office_code[0]=b&0xff;
        office_code[1]=b>>8;
        b=my_getw(fp[0]);
        office_code[2]=b&0xff;
        office_code[3]=b>>8;
        b=my_getw(fp[0]);
        office_code[4]=b&0xff;
        office_code[5]=b>>8;
        office_code[6]=0;

        b=13 - strlen(office_code);
        b /= 2;
        for(;b;b--)
                wputchar(w,' ');
        wprintf(w,"%s",office_code);
        saved_win=w;
        getchar_hndlr_flag=SET;
        cqw();
        return b;
#else
        return 'c';
#endif
}

SDNST dep_find_subscriber_no(unsigned int i){
        {
            SDNST k;unsigned int a;
            for(a=0;a<no_of_subscriber_groups;a++){
                if((k=(i-subscriber_group[a].port_start)) < subscriber_group[a].no_of_ports)
                    return (k+subscriber_group[a].directory_start);
            }
        }
#ifdef WE_DO_HAVE_TWO_FDN
        if(i >= NO_EXTENSION_FOR_F_D_N_A)
                return (((from_internal_to_dialled_no[i]-NO_EXTENSION_FOR_F_D_N_A)+hex2l(F_D_N_B))%10000000L);
        else
#endif
#ifdef AHAR
        if(i >= 216)
                return (long)((i-216)+900);
        else
#endif
        return (((long)from_internal_to_dialled_no[i]+first_directory_decimal)%100000000L);
}
unsigned int dep_fcp(SDNST i){
        unsigned long j;
        {
            SDNST k;unsigned int a;
            for(a=0;a<no_of_subscriber_groups;a++){
                if((k=(i-subscriber_group[a].directory_start)) < subscriber_group[a].no_of_ports)
                    return (k+subscriber_group[a].port_start);
            }
        }
#ifdef AHAR
        if(i < 940L)
                return (unsigned int)(i-900L);
#endif
        j=i-first_directory_decimal;
#ifdef WE_DO_HAVE_TWO_FDN
        if(j >= NO_EXTENSION_FOR_F_D_N_A){
                j = (i-hex2l(F_D_N_B))+NO_EXTENSION_FOR_F_D_N_A;  /*  FIRST_DIRECTORY_DECIMAL;*/
        }
#endif
        if(j >= MAX_EXISTING_NO) return OPR_ERROR;
        i=from_dialled_to_internal_no[(unsigned int)j];
        if(i >= NO_TOTAL_EXTENSION)
                return OPR_ERROR;
        return (unsigned int) i;
}

int dxp(BYTE n){  /* display extensions parameters */

        BYTE  code;

        code=get_c();
        switch (code){
          case ESC :
                getchar_hndlr_flag=RESET;
                deletw(saved_win);
                saved_b=0;
                saved_n=0;
                return code;
          case PGDN :
                saved_b++;
                if (saved_b < (no_my_extension[n] / NO_DISPLAY_ON_PAGE)){
                        dxp_part1(n);
                        return code;
                }
                if (saved_b == (no_my_extension[n] / NO_DISPLAY_ON_PAGE)){
                        if ((no_my_extension[n] % NO_DISPLAY_ON_PAGE) > 0){
                                deletw(saved_win);
                                dxp_init_display(n);
                                dxp_part1(n);
                                return code;
                        }

                }
#ifdef LIM
                /* saved_b > */
                if ( saved_n == 0){
                        saved_b=0;
                        saved_n=1;
                        deletw(saved_win);
                        dxp_init_display(saved_n);
                        dxp_part1(saved_n);
                        return code;
                }
#if(NO_EXTENSION_C != 0)
                if ( saved_n == 1){
                        saved_b=0;
                        saved_n=2;
                        deletw(saved_win);
                        dxp_init_display(saved_n);
                        dxp_part1(saved_n);
                        return code;
                }
#endif
#endif
                saved_b--;
                return code;
          case PGUP :
                return code;
          default   :
                return code;

        }
}

void dxp_init_display(BYTE n){  /* display extensions parameters */

        WINDOW *w;
        int b,s,p;
        s=no_my_extension[n] / NO_DISPLAY_ON_PAGE;
        p=no_my_extension[n] % NO_DISPLAY_ON_PAGE;
        b=saved_b;
        if ( saved_b == 0 ){
                saved_a=fdb;
                fseek(fp[n],MAX_NAME_LENGHT + 2,SEEK_SET);
                fdb+=no_my_extension[n];
        }
        /* display extensions directory numbers */

        if (b != s)
#ifdef X_RAILWAY
                w=enw(0,0,22,80);
#else
                w=enw(0,0,21,80);
#endif
        else{
                if (p > 0){
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
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,dxp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"EXTENSIONS PARAMETERS");
        draw_window(w);
#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
        wprompt(w,0,0,"ext.   CO        TIE         E&M             cst                     ast   hli");
        wprompt(w,0,1,"                                           SA  TR  T0  CR  IN  FM  CB  MC  ");
#else
        wprompt(w,0,0,"ext.       CO     E&M                        cst                     ast   hli");
#endif
#else
        wprompt(w,0,0,"ext.       CO     TIE                        cst                     ast   hli");
#endif

#ifdef X_RAILWAY
        wprompt(w,0,2,"     gp654321 gp654321 gp543210987654321 AA  HL  PU  CF  DD  ST  CP  FA  CG");
        wprompt(w,0,3,"------------------------------------------------------------------------------");
#else
#ifdef BAD_PAYER
        wprompt(w,0,1,"           654321 654321 AA B1 HL TR PU B2 CF B3 DD IN ST FM CP CB FA MC CG");
#else
        wprompt(w,0,1,"           654321 654321 AA SA HL TR PU T0 CF CR DD IN ST FM CP CB FA MC CG");
#endif
        wprompt(w,0,2,"------------------------------------------------------------------------------");
#endif
        saved_win=w;
}

void dxp_part1(BYTE n){  /* display extensions parameters */

        WINDOW *w;
        int b,c,s,p;
        unsigned int a;
        unsigned int e,d;
        int max_dsply;
        b=saved_b;
        w=saved_win;
        a=saved_a;
        s=no_my_extension[n] / NO_DISPLAY_ON_PAGE;
        p=no_my_extension[n] % NO_DISPLAY_ON_PAGE;
#ifdef X_RAILWAY
                wcursor(w,76,3);
#else
                wcursor(w,76,2);
#endif
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                max_dsply=p;
                for(c=0;c < max_dsply;c++,a++){
#ifdef X_RAILWAY
                        wprintf(w,"\n%-6.0ld",dep_find_subscriber_no(a));
#else
                        wprintf(w,"\n%-10.0"SDNS_BF,dep_find_subscriber_no(a));
#endif
                        d=my_getw(fp[n]);
#ifdef NEW_ANALOG_TRUNK_GROUPING
                        wprintf(w," ");
                        for(e=0x20;e;e >>= 1)
                                if(d & e)
                                        wputchar(w,'|');
                                 else
                                         wputchar(w,'-');
#else // NEW_ANALOG_TRUNK_GROUPING
                        if(d & 0x80){  /* if CO allowed */
                                if(d & 0x40){  /* CO group allowed */
                                        wprintf(w,"|");
                                        for(e=0x20;e;e >>= 1)
                                                if(d & e)
                                                        wputchar(w,'|');
                                                else
                                                        wputchar(w,'-');
                                }
                                else{  /* CO group not allowed */
                                        wprintf(w,"-");
                                        for(e=6;e;e--)
                                                if(e == (d & 0x3f) )
                                                        wputchar(w,'|');
                                                else
                                                        wputchar(w,'-');
                                }
                        }
                        else  /* CO not allowed */
                                wprintf(w,"-------");
#endif // NEW_ANALOG_TRUNK_GROUPING
#if ((!defined NO_TIE_INIT) || (defined EM_GROUP_PROGRAM))
                        d >>= 8;
#else
                        d >>= 6;  // in order to allow b11-b6 corresponding to co groups 6-12 be shown on tie locations
#endif
#ifndef X_RAILWAY
//                      wputchar(w,' ');
#endif
#ifdef NEW_ANALOG_TRUNK_GROUPING
//                      wprintf(w,"   ");
                        wprintf(w," ");
                        for(e=0x20;e;e >>= 1)
                                if(d & e)
                                        wputchar(w,'|');
                                else
                                        wputchar(w,'-');
#else // NEW_ANALOG_TRUNK_GROUPING
                        if(d & 0x80){  /* if TIE allowed */
                                if(d & 0x40){  /* TIE group allowed */
                                        wprintf(w," | ");
                                        for(e=0x20;e;e >>= 1)
                                                if(d & e)
                                                        wputchar(w,'|');
                                                else
                                                        wputchar(w,'-');
                                }
                                else{  /* TIE group not allowed */
                                        wprintf(w," - ");
#if ( (defined EM_GROUP_PROGRAM) && (!defined X_RAILWAY) )
                                        for(e=0x20;e;e >>= 1)
                                                if(d & e)
#else
                                        for(e=6;e;e--)
                                                if(e == (d & 0x3f) )
#endif
                                                        wputchar(w,'|');
                                                else
                                                        wputchar(w,'-');
                                }
                        }
                        else  /* TIE not allowed */
                                wprintf(w," - ------");
#endif // NEW_ANALOG_TRUNK_GROUPING
#ifdef X_RAILWAY
                        d=my_getw(fp[n]);

#ifdef NEW_ANALOG_TRUNK_GROUPING
                        wprintf(w,"   ");
#else // NEW_ANALOG_TRUNK_GROUPING
                        if(d & 0x8000)   /* if E&M allowed */
                                wprintf(w," | ");
                        else
                                wprintf(w," - ");
#endif // NEW_ANALOG_TRUNK_GROUPING
                        for(e=0x4000;e;e >>= 1)
                                if(d & e)
                                        wputchar(w,'|');
                                else
                                        wputchar(w,'-');
#endif
#ifdef X_RAILWAY
                        wprintf(w," ");
#else
                        wprintf(w," ");
#endif
                        d=my_getw(fp[n]);
                        if(d & AMPLIFIER_ACCESS)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & SIREN_ACCESS)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & HOT_LINE_ACCESS)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & RING_AGAIN_TIET)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & CALL_PICKUP_USE)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & TIE_0)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & CONFERENCE_SERVICE)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & RING_AGAIN_COT)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & DONT_DIST)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & INTERNATIONAL)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & STD)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & FOLLOW_ME)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & CAMP_ON)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & CALL_BACK)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        d=my_getw(fp[n]);
                        if(d & FEATURE_ACTIVE)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & MALICIOUS_CALL_DTCT)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        if(d & CALL_GEN)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
#ifndef X_RAILWAY
                        wputchar(w,' ');
#endif
                        d=my_getw(fp[n]);
                        if(d < NO_HOT_LINE)
                                wprintf(w,"%1.2d",d+1);
                                /* for displaying index 0 as 1 */
                        else
                                wprintf(w,"--",d);
                }
        saved_a=a;
}

void dxp_init(BYTE n){  /* display extensions parameters */
        dxp_init_display(n);
        dxp_part1(n);
        return;
}

#if (NO_CO != 0)
int dctp(BYTE n){  /* display co parameters */

        BYTE  code;
        int s;

        s=(no_co[n] / NO_DISPLAY_ON_PAGE);
        code=get_c();
        switch (code){
          case ESC :
                getchar_hndlr_flag=RESET;
                deletw(saved_win);
                saved_b=0;
                saved_n=0;
                return code;
          case PGDN :
                saved_b++;
                if (saved_b < s){
                        dctp_part1(n);
                        return code;
                }
                if (saved_b == s){
                        if ((no_co[n] % NO_DISPLAY_ON_PAGE) > 0){
                                deletw(saved_win);
                                dctp_init_display(n);
                                dctp_part1(n);
                                return code;
                        }
                }

#ifdef LIM
                /* saved_b > */
                if ( saved_n == 0){
                        saved_n=1;
                        if ((no_co[1] / NO_DISPLAY_ON_PAGE) == 0){
                                saved_b--;
                                return code;
                        }
                        saved_b=0;
                        deletw(saved_win);
                        dctp_init_display(saved_n);
                        dctp_part1(saved_n);
                        return code;
                }
#if(NO_EXTENSION_C != 0)
                if ( saved_n == 1){
                        saved_n=2;
                        if ((no_co[2] / NO_DISPLAY_ON_PAGE) == 0){
                                saved_b--;
                                return code;
                        }
                        saved_b=0;
                        deletw(saved_win);
                        dctp_init_display(saved_n);
                        dctp_part1(saved_n);
                        return code;
                }
#endif
#endif
                saved_b--;
                return code;
          case PGUP :
                return code;
          default   :
                return code;

        }
}
#endif

#if (NO_CO != 0)
void dctp_init(BYTE n){
        dctp_init_display(n);
        dctp_part1(n);
}
#endif

#if (NO_CO != 0)
void dctp_init_display(BYTE n){  /* display CO trunk parameters */
        WINDOW *w;
        int a,b,s,p;
        s=no_co[n] / NO_DISPLAY_ON_PAGE;
        p=no_co[n] % NO_DISPLAY_ON_PAGE;
        b=saved_b;
        if (saved_b == 0){
#ifdef EM_GROUP_PROGRAM
                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8);
#elif (defined  NO_TIE_INIT)
                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH);
#else
                a=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4);
#endif
                fseek(fp[n],a,SEEK_SET);
                saved_a=1;
        }
        a=saved_a;
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
                print_error_report("\nDEPM,dctp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"CO TRUNKS PARAMETERS ");
        //swt(w);
        draw_window(w);
#ifdef TOLL_BARRING_ON_COs
        wprompt(w,0,0,"     CO#   toll  DDI        group #   in service   DDI active   timing active");
#else
        wprompt(w,0,0," CO trunk #    dest. DDI    group #   in service   DDI active   timing active");
#endif
        wprompt(w,0,1,"-------------------------------------------------------------------------------");
        saved_win=w;
}
#endif

#if (NO_CO != 0)
void dctp_part1(BYTE n){  /* display CO trunk parameters */
        WINDOW *w;
        int a,b,c,s,p;unsigned int d;
        int e;
        int max_dsply;
        b=saved_b;
        w=saved_win;
        a=saved_a;
        s=no_co[n] / NO_DISPLAY_ON_PAGE;
        p=no_co[n] % NO_DISPLAY_ON_PAGE;
                wcursor(w,76,1);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++,a++){
#ifdef TOLL_BARRING_ON_COs
                        wprintf(w,"\n     %1.3d\t",a);
#else
                        wprintf(w,"\n     %1.3d\t\t ",a);
#endif
                        e=my_getw(fp[n]);  /* destination DDI */
                        d=my_getw(fp[n]);  /* activation int */
#ifdef TOLL_BARRING_ON_COs
                        if(d & TOLL_ACCESS_MASK)
                                wprintf(w,"|\t ");
                        else
                                wprintf(w,"-\t ");
#endif
                        if(((unsigned int) e < NO_TOTAL_EXTENSION) && (d & DA_MASK))
                                wprintf(w,"%-6.0ld\t\t",dep_find_subscriber_no(e));
                        else
                                wprintf(w,"------\t\t");
                        wprintf(w,"%.2d\t   ",((d & 0xf000) >> 12) + 1 );
                        /* for displaying trunk group # from 1 */

                        if(d & IS_MASK)
                                wprintf(w,"        |\t\t");
                        else
                                wprintf(w,"        -\t\t");
                        if(d & DA_MASK)
                                wprintf(w,"    |\t\t");
                        else
                                wprintf(w,"    -\t\t");
                        if(d & TA_MASK)
                                wprintf(w,"      |");
                        else
                                wprintf(w,"      -");
                }
        saved_a=a;
        return ;
}
#endif

#ifndef NO_TIE_INIT
int dttp(BYTE n){  /* display extensions parameters */

        BYTE  code;
        int s;

        s=(no_tie[n] / NO_DISPLAY_ON_PAGE);
        code=get_c();
        switch (code){
          case ESC :
                getchar_hndlr_flag=RESET;
                deletw(saved_win);
                saved_b=0;
                saved_n=0;
                return code;
          case PGDN :
                saved_b++;
                if (saved_b < s){
                        dttp_part1(n);
                        return code;
                }
                if (saved_b == s){
                        if ((no_co[n] % NO_DISPLAY_ON_PAGE) > 0){
                                deletw(saved_win);
                                dttp_init_display(n);
                                dttp_part1(n);
                                return code;
                        }
                }

#ifdef LIM
                /* saved_b > */
                if ( saved_n == 0){
                        saved_n=1;
                        if ((no_tie[1] / NO_DISPLAY_ON_PAGE) == 0){
                                saved_b--;
                                return code;
                        }
                        saved_b=0;
                        deletw(saved_win);
                        dttp_init_display(saved_n);
                        dttp_part1(saved_n);
                        return code;
                }
#if(NO_EXTENSION_C != 0)
                if ( saved_n == 1){
                        saved_n=2;
                        if ((no_tie[2] / NO_DISPLAY_ON_PAGE) == 0){
                                saved_b--;
                                return code;
                        }
                        saved_b=0;
                        deletw(saved_win);
                        dttp_init_display(saved_n);
                        dttp_part1(saved_n);
                        return code;
                }
#endif
#endif
                saved_b--;
                return code;
          case PGUP :
                return code;
          default   :
                return code;

        }
}
#endif

#ifndef NO_TIE_INIT
void dttp_init(BYTE n){
        dttp_init_display(n);
        dttp_part1(n);
}
#endif

#ifndef NO_TIE_INIT
void dttp_init_display(BYTE n){  /* dispaly TIE trunk parameters */
        WINDOW *w;
        int b,s,p;
        s=no_tie[n] / NO_DISPLAY_ON_PAGE;
        p=no_tie[n] % NO_DISPLAY_ON_PAGE;
        b=saved_b;
        if (saved_b == 0){
                fseek(fp[n],MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH),SEEK_SET);
                saved_a=1;  /* for display first trunk is number 1 */
        }
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
                print_error_report("\nDEPM,dttp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"TIE TRUNKS PARAMETERS");
        draw_window(w);
        wprompt(w,0,0,"TIE trunk #    dest. DDI    group #   in service   DDI active   timing active");
        wprompt(w,0,1,"-------------------------------------------------------------------------------");
        saved_win=w;
        return ;
}
#endif

#ifndef NO_TIE_INIT
void dttp_part1(BYTE n){  /* dispaly TIE trunk parameters */
        WINDOW *w;
        int a,b,c,d,s,p;
        int e;
        int max_dsply;

        b=saved_b;
        w=saved_win;
        a=saved_a;
        s=no_tie[n] / NO_DISPLAY_ON_PAGE;
        p=no_tie[n] % NO_DISPLAY_ON_PAGE;

                wcursor(w,76,1);
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
                                wprintf(w,"%-6.0ld\t\t",dep_find_subscriber_no(e));
                        else
                                wprintf(w,"------\t\t");
                        wprintf(w,"%.1d\t   ",(d & TRUNK_GROUP_MASK) + 1);
                        /* for displaying trunk group # from 1 */

                        if(d & IS_MASK)
                                wprintf(w,"        |\t\t");
                        else
                                wprintf(w,"        -\t\t");
                        if(d & DA_MASK)
                                wprintf(w,"    |\t\t");
                        else
                                wprintf(w,"    -\t\t");
                        if(d & TA_MASK)
                                wprintf(w,"      |");
                        else
                                wprintf(w,"      -");
                }
        saved_a=a;
        return ;
}
#endif


#ifdef EM_GROUP_PROGRAM
int demtp(BYTE n){  /* display extensions parameters */
        BYTE  code;
        int s;
        s=(no_em[n] / NO_DISPLAY_ON_PAGE);
        code=get_c();
        switch (code){
          case ESC :
                getchar_hndlr_flag=RESET;
                deletw(saved_win);
                saved_b=0;
                saved_n=0;
                return code;
          case PGDN :
                saved_b++;
                if (saved_b < s){
                        demtp_part1(n);
                        return code;
                }
                if (saved_b == s){
                        if ((no_em[n] % NO_DISPLAY_ON_PAGE) > 0){
                                deletw(saved_win);
                                demtp_init_display(n);
                                demtp_part1(n);
                                return code;
                        }
                }

#ifdef LIM
                /* saved_b > */
                if ( saved_n == 0){
                        saved_n=1;
                        if ((no_em[1] / NO_DISPLAY_ON_PAGE) == 0){
                                saved_b--;
                                return code;
                        }
                        saved_b=0;
                        deletw(saved_win);
                        demtp_init_display(saved_n);
                        demtp_part1(saved_n);
                        return code;
                }
#if(NO_EXTENSION_C != 0)
                if ( saved_n == 1){
                        saved_n=2;
                        if ((no_em[2] / NO_DISPLAY_ON_PAGE) == 0){
                                saved_b--;
                                return code;
                        }
                        saved_b=0;
                        deletw(saved_win);
                        demtp_init_display(saved_n);
                        demtp_part1(saved_n);
                        return code;
                }
#endif
#endif

                saved_b--;
                return code;
          case PGUP :
                return code;
          default   :
                return code;

        }
}
#endif

#ifdef EM_GROUP_PROGRAM
void demtp_init(BYTE n){
        demtp_init_display(n);
        demtp_part1(n);
}
#endif

#ifdef EM_GROUP_PROGRAM
void demtp_init_display(BYTE n){  /* dispaly EM trunk parameters */
        WINDOW *w;
        int b,s,p;
        s=no_em[n] / NO_DISPLAY_ON_PAGE;
        p=no_em[n] % NO_DISPLAY_ON_PAGE;
        b=saved_b;
        if (saved_b == 0){
                fseek(fp[n],MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH),SEEK_SET);
                saved_a=1;
        }
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
                print_error_report("\nDEPM,demtp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"E&M TRUNKS PARAMETERS");
        draw_window(w);
        wprompt(w,0,0,"E&M trunk #    sig. prot    group #       i.s    s.9   d.t   lpbk   t1   t2  ");
        wprompt(w,0,1,"-------------------------------------------------------------------------------");
        saved_win=w;
}
#endif

#ifdef EM_GROUP_PROGRAM
void demtp_part1(BYTE n){  /* dispaly EM trunk parameters */
        WINDOW *w;
        int a,b,c,d,s,p,e;
        int max_dsply;
        b=saved_b;
        w=saved_win;
        a=saved_a;
        s=no_em[n] / NO_DISPLAY_ON_PAGE;
        p=no_em[n] % NO_DISPLAY_ON_PAGE;

        wcursor(w,76,1);
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
                        wprintf(w,"%1.2x  \t\t",e);
#ifdef X_RAILWAY
                        wprintf(w,"  %1.2d   ",((d & EM_TRUNK_GROUP_MASK)>>EMTGM_LOC) + 1);
#else
                        wprintf(w,"  %.1d\t   ",((d & EM_TRUNK_GROUP_MASK)>>EMTGM_LOC) + 1);
#endif
                        /* for displaying trunk group # from 1 */

                        if(d & IS_MASK)
                                wprintf(w,"        |");
                        else
                                wprintf(w,"        -");
                        if(d & S9_MASK)
                                wprintf(w,"      |");
                        else
                                wprintf(w,"      -");
                        if(d & DA_MASK)
                                wprintf(w,"     |\t");
                        else
                                wprintf(w,"     -\t");
                        if(d & TA_MASK)
                                wprintf(w,"   |");
                        else
                                wprintf(w,"   -");
                        e=my_getw(fp[n])&0xff;
                        wprintf(w,"    %1.3d ",e);
                        e=my_getw(fp[n])&0xff;
                        wprintf(w," %1.3d ",e);
                }
        saved_a=a;
        return ;
}
#endif

#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
int ddtp(BYTE n){  /* display extensions parameters */

        BYTE  code;
        int s,p;

#ifdef EXTERNAL_CAS3_INSTALLED
        s=no_cas3[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas3[n] % NO_DISPLAY_ON_PAGE;
#else   // (NO_CAS1 != 0)
        s=no_cas1[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas1[n] % NO_DISPLAY_ON_PAGE;
#endif
        code=get_c();
        switch (code){
          case ESC :
                getchar_hndlr_flag=RESET;
                deletw(saved_win);
                saved_b=0;
                saved_n=0;
                return code;
          case PGDN :
                saved_b++;
                if (saved_b < s){
                        ddtp_part1(n);
                        return code;
                }
                if (saved_b == s){
                        if (p > 0){
                                deletw(saved_win);
                                ddtp_init_display(n);
                                ddtp_part1(n);
                                return code;
                        }
                }

#ifdef LIM
                /* saved_b > */
                if ( saved_n == 0){
                        saved_n=1;
#ifdef EXTERNAL_CAS3_INSTALLED
                        if ((no_cas3[1] / NO_DISPLAY_ON_PAGE) == 0){
#else //CAS1
                        if ((no_cas1[1] / NO_DISPLAY_ON_PAGE) == 0){
#endif
                                saved_b--;
                                return code;
                        }
                        saved_b=0;
                        deletw(saved_win);
                        ddtp_init_display(saved_n);
                        ddtp_part1(saved_n);
                        return code;
                }
#if(NO_EXTENSION_C != 0)
                if ( saved_n == 1){
                        saved_n=2;
#ifdef EXTERNAL_CAS3_INSTALLED
                        if ((no_cas3[2] / NO_DISPLAY_ON_PAGE) == 0){
#else //CAS1
                        if ((no_cas1[2] / NO_DISPLAY_ON_PAGE) == 0){
#endif
                                saved_b--;
                                return code;
                        }
                        saved_b=0;
                        deletw(saved_win);
                        ddtp_init_display(saved_n);
                        ddtp_part1(saved_n);
                        return code;
                }
#endif
#endif

                saved_b--;
                return code;
          case PGUP :
                return code;
          default   :
                return code;

        }
}
#endif

#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void ddtp_init(BYTE n){
        ddtp_init_display(n);
        ddtp_part1(n);
}
#endif

#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void ddtp_init_display(BYTE n){  /* display digital trunk parameters */
        WINDOW *w;
        int a,b,s,p;

#ifdef EXTERNAL_CAS3_INSTALLED
        s=no_cas3[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas3[n] % NO_DISPLAY_ON_PAGE;
#else   // (NO_CAS1 != 0)
        s=no_cas1[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas1[n] % NO_DISPLAY_ON_PAGE;
#endif
        b=saved_b;
        if (saved_b == 0){
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
                print_error_report("\nDEPM,ddtp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"DIGITAL TRUNKS PARAMETERS");
        draw_window(w);
        wprompt(w,0,0,"dig.trunk #  group #  is  icc  ogc  pts  eos  mtr  opr  mal  imm  wfa  ids   ");
        wprompt(w,0,1,"-------------------------------------------------------------------------------");

        saved_win=w;
}
#endif


#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void ddtp_part1(BYTE n){  /* display digital trunk parameters */
        WINDOW *w;
        int a,b,c,d,s,p;
        int max_dsply;

        b=saved_b;
        w=saved_win;
        a=saved_a;

#ifdef EXTERNAL_CAS3_INSTALLED
        s=no_cas3[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas3[n] % NO_DISPLAY_ON_PAGE;
#else   // (NO_CAS1 != 0)
        s=no_cas1[n] / NO_DISPLAY_ON_PAGE;
        p=no_cas1[n] % NO_DISPLAY_ON_PAGE;
#endif
                wcursor(w,76,1);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++,a++){
                        wprintf(w,"\n     %1.3d\t ",a);
                        d=my_getw(fp[n]);  /* activation int */
//                      wprintf(w,"  %2.2d ",(d & CAS3_GROUP_MASK) + 1);
                        wprintf(w,"  %2.2d ",(d & CAS3_GROUP_MASK));
                        /* for displaying trunk group # from 1 */
                        if(d & IS)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & ICC)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & OGC)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & PTS)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & EOS)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & CAS_METERING)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & OPR_RCL)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & MAL_CALL)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & IMM_ACK)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & WTFR_ACK)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                        if(d & INTER_SIGNAL_DGT_SEND)
                                wprintf(w,"    |");
                        else
                                wprintf(w,"    -");
                }
        saved_a=a;
        return;
}
#endif

#ifdef REPORT_BULK_BILLING
int vbbf(void){  /* display extensions parameters */

        BYTE  code;
        int s,p;

        s=(NO_TOTAL_EXTENSION) / NO_DISPLAY_ON_PAGE;
        p=(NO_TOTAL_EXTENSION) % NO_DISPLAY_ON_PAGE;
        code=get_c();
        switch (code){
          case ESC :
                getchar_hndlr_flag=RESET;
                deletw(saved_win);
                saved_b=0;
                return code;
          case PGDN :
                saved_b++;
                if (saved_b < s){
                        vbbf_part1();
                        return code;
                }
                if (saved_b == s){
                        if (p > 0){
                                deletw(saved_win);
                                vbbf_init_display();
                                vbbf_part1();
                                return code;
                        }
                }

                saved_b--;
                return code;
          case PGUP :
                        if (saved_b == 0){
                                return code;
                        }
                        if(saved_b == s){
                                saved_a -= (p+NO_DISPLAY_ON_PAGE);
                                saved_b--;
                                vbbf_part1();
                                return code;
                        }
                        if(saved_b < s){
                                saved_a -= 2*NO_DISPLAY_ON_PAGE;
                                saved_b--;
                                vbbf_part1();
                                return code;
                        }
          default   :
                return code;

        }
}
#endif


#ifdef REPORT_BULK_BILLING
void vbbf_init(void){
        fp1=fopen(BULK_BILLING_FILE,"rb");
        if(fp1 == NULL){
                error_message("Cannot open bulk billing file.");
                get_char();
                clear_message();
                return 'c';
        }
        vbbf_init_display();
        vbbf_part1();
}
#endif


#ifdef REPORT_BULK_BILLING
void vbbf_init_display(void){ /* view bulk billing file */
        WINDOW *w;
        int b,c,s,p;
        int a;
        unsigned int d1,d2,d3;
        s=(NO_TOTAL_EXTENSION) / NO_DISPLAY_ON_PAGE;
        p=(NO_TOTAL_EXTENSION) % NO_DISPLAY_ON_PAGE;
        b=saved_b;
        if (saved_b == 0){
                saved_a=0;
                fseek(fp1,0,SEEK_SET);
        }
        a=saved_a;
        /* display extensions directory numbers */
        if (b != s){
                w=enw(0,0,21,80);
        }
        else{
                if (p > 0)
                        w=enw(0,0,p+5,80);
                else
                        w=enw(0,0,21,80);
        }
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,vbbf0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"SUBSCRIBERS PULSE COUNTERS");
        draw_window(w);
        wprompt(w,0,0,"                                                                               ");
        wprompt(w,0,1,"Subscr.    STD    LOCAL   EXTRA    L+E    S+L+E  INTERN.                    ");
        wprompt(w,0,2,"-------------------------------------------------------------------------------");
        saved_win=w;
        return 'c';
}
#endif




#ifdef REPORT_BULK_BILLING
void vbbf_part1(void){ /* view bulk billing file */
        WINDOW *w;
        int b,c,s,p;
        int a;
        unsigned int d1,d2,d3;
        int max_dsply;

        b=saved_b;
        w=saved_win;
        a=saved_a;
        s=(NO_TOTAL_EXTENSION) / NO_DISPLAY_ON_PAGE;
        p=(NO_TOTAL_EXTENSION) % NO_DISPLAY_ON_PAGE;
                wcursor(w,76,2);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                if ( p > 0)
                        max_dsply=p;
                else
                        max_dsply=NO_DISPLAY_ON_PAGE;
                for(c=0;c < max_dsply;c++,a++){
                        fseek(fp1,(a << 2),SEEK_SET);
                        wprintf(w,"\n%-6.0ld",dep_find_subscriber_no(a));
                        d1=my_getw(fp1);    // std
                        wprintf(w,"\t%6.6u",d1);
                        d2=my_getw(fp1);    // local
                        wprintf(w,"\t%6.6u",d2);
                        fseek(fp1,(a << 2) + (NO_TOTAL_EXTENSION << 2),SEEK_SET);
                        d3      =     my_getw(fp1);         // extra
                        wprintf(w,"\t%6.6u",d3);
                        wprintf(w,"\t%6.6u",d2+d3);
                        wprintf(w,"\t%6.6u",d1+d2+d3);
                        d1=my_getw(fp1);                    // service/international
                        wprintf(w,"\t%6.6u",d1);
                }
        saved_a=a;
        return 'c';
}
#endif




//MYCHNG end
void file_view_init(void){

        char line[80];
        WINDOW *w;
        BYTE c;

        w=enw(0,0,20,80);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,dxp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD start
        swt(w,file_title);
        draw_window(w);

        line_no=0;
        for(c=0;c < NO_DISPLAY_ON_PAGE;c++){
                if (line_no >= last_line){
                        line_no--;
                        break;
                }
                fgets(line,80,view_fp);
                wprintf(w,line);
                line_no++;
       }
        pc(80,25);
        saved_win=w;
        file_viewer_flag=SET;
}


void file_viewer(void){ /* view alarm log file */

        char line[80];
        WINDOW *w;
        BYTE c;
        BYTE key;
        BYTE last_page_line_no;

        w=saved_win;
        last_page_line_no=(last_line % NO_DISPLAY_ON_PAGE)+1;
        key=get_c();
        switch (key){
          case NO_KEY_PRESSED :
                      break;
          case ESC  : file_viewer_flag=RESET;
                      deletw(saved_win);
                      break;
          case PGDN :
                      if ( last_line <= (NO_DISPLAY_ON_PAGE)) break;
                      if (line_no >= last_line) break;
                      wcursor(w,0,0);
                      cw(w);
                      if ((line_no+NO_DISPLAY_ON_PAGE) > last_line){
                              line_no=last_line-NO_DISPLAY_ON_PAGE;
                      }
                      fseek(view_fp, file_line_ptr[(unsigned int)line_no], SEEK_SET);
                      for(c=0;c < NO_DISPLAY_ON_PAGE;c++){
                              fgets(line,80,view_fp);
                              wprintf(w,line);
                              line_no++;
                      }
                      pc(80,25);
                      break;
          case PGUP :
                      if ( last_line <= (NO_DISPLAY_ON_PAGE)) break;
                      if (line_no == NO_DISPLAY_ON_PAGE) break;
                      wcursor(w,0,0);
                      cw(w);
                      if (line_no == last_line)
                              line_no-=(NO_DISPLAY_ON_PAGE+last_page_line_no);
                      else
                              line_no-=2*NO_DISPLAY_ON_PAGE;

                      if (line_no < 0){
                              line_no=0;
                      }
                      fseek(view_fp, file_line_ptr[(unsigned int)line_no], SEEK_SET);
                      for(c=0;c < NO_DISPLAY_ON_PAGE;c++,line_no++){
                              fgets(line,80,view_fp);
                              wprintf(w,line);
                      }
                      pc(80,25);
                      break;
          case DN :
                      if ( last_line <= (NO_DISPLAY_ON_PAGE)) break;
                      if (line_no >= last_line) break;
                      line_no++;
                      line_no-=NO_DISPLAY_ON_PAGE;
                      if ((line_no+NO_DISPLAY_ON_PAGE) > last_line){
                              line_no=last_line-NO_DISPLAY_ON_PAGE;
                      }
                      wcursor(w,0,0);
                      cw(w);
                      fseek(view_fp, file_line_ptr[(unsigned int)line_no], SEEK_SET);
                      for(c=0;c < NO_DISPLAY_ON_PAGE;c++,line_no++){
                              fgets(line,80,view_fp);
                              wprintf(w,line);
                      }
                      pc(80,25);
                      break;
          case UP :
                      if ( last_line <= (NO_DISPLAY_ON_PAGE)) break;
                      if (line_no == NO_DISPLAY_ON_PAGE) break;
                      wcursor(w,0,0);
                      cw(w);
                      line_no-=NO_DISPLAY_ON_PAGE;
                      line_no--;
                      if (line_no < 0){
                              line_no=0;
                      }
                      fseek(view_fp, file_line_ptr[(unsigned int)line_no], SEEK_SET);
                      for(c=0;c < NO_DISPLAY_ON_PAGE;c++,line_no++){
                              fgets(line,80,view_fp);
                              wprintf(w,line);
                      }
                      pc(80,25);
                      break;
          default :   break;
        }
}


#if ((defined ALARM_ENABLE) && (TON == ALARM_OPR))
void init_alarm_pointer_array(void){

        long ptr;
        char line[80];

        if (flog == NULL) return;
        alarm_last_line=0;
        while(!feof(flog)){
                ptr=ftell(flog);
                alarm_line_ptr[(unsigned int)alarm_last_line]=ptr;
                fgets(line,80,flog);
                alarm_last_line++;
                if (alarm_last_line >= MAX_FILE_LINE) break;
        }
        /* to remove last line of binary file (possibly eof char)*/
//      alarm_last_line--;
        fseek(flog,0L,SEEK_SET);
}
#endif

void init_malicious_pointer_array(void){

        long ptr;
        char line[80];

        if (malicious_fp == NULL) return;
        malicious_last_line=0;
        while(!feof(malicious_fp)){
                ptr=ftell(malicious_fp);
                malicious_line_ptr[(unsigned int)malicious_last_line]=ptr;
                fgets(line,80,malicious_fp);
                malicious_last_line++;
                if (malicious_last_line >= MAX_FILE_LINE) break;
        }
        /* to remove last line of binary file (possibly eof char)*/
//        malicious_last_line--;
        fseek(malicious_fp,0L,SEEK_SET);
}

#if ((defined ALARM_ENABLE) && (TON == ALARM_OPR))
void init_alarm_file_data(void){

        if(flog == NULL){
        }
        else{
                /* seek to the start of the file */
                fseek(flog, 0, SEEK_SET);
                init_alarm_pointer_array();
                last_line=alarm_last_line;
                view_fp=flog;
                file_line_ptr=&alarm_line_ptr[0];
                file_title=" ** ALARM LOG FILE ** ";
                fseek(flog, 0, SEEK_SET);
        }

}
#endif

void init_malicious_file_data(void){

#ifdef MALICIOUS_PRINT_ON_U
        if(malicious_fp == NULL){
        }
        else{
// following close and then again open added just to make this view include
// also any added data from the last open time. we may had chosen not to open
// malicious file in the initialization at all and instead every time for every
// view open it and after view close it.
                fclose(malicious_fp);
#if (COMPILER == BCC)
                malicious_fp=_fsopen(MALICIOUS_FILE,"r+t",0);  // read and write, 0 is SH_COMPAT
#endif
#if (COMPILER == GCC)
                malicious_fp=fopen(MALICIOUS_FILE,"r+t");  // read and write
#endif
                /* seek to the start of the file */
                fseek(malicious_fp, 0, SEEK_SET);
                init_malicious_pointer_array();
                last_line=malicious_last_line;
                view_fp=malicious_fp;
                file_line_ptr=&malicious_line_ptr[0];
                file_title=" ** MALICIOUS LOG FILE ** ";
                fseek(malicious_fp, 0, SEEK_SET);
#ifdef ALARM_ENABLE
                pmamftx_both_sys(MAC_MALICIOUS_FILE_VIEWED|MA_SYS_P|FEATURE_ACTIVATION,TON,OPR_T,saved_n);
#endif
        }
#endif
}

void dep_quit(void){

        BYTE h;
        unmask_stm();
        dep_flag=RESET;

#ifdef AUTO_UPLOAD
        fseek(fp[0],MAX_NAME_LENGHT-2,SEEK_SET);
        my_putw(file_changed,fp[0]);
#endif
        if(fp[0] != NULL){
                cfcs(fp[0]);  /* update file check sum */
                fclose(fp[0]);
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
        deletw(ktbg_w);
        caw();
        for(h=0;h<NO_HOLD_KEY;h++)
                if(hldl[h].t != NULL_T)

#ifdef OPERATOR_PC
                        led_set(hkl[h]);  /* hold LED on */
#else
                        led_set(hkl);  /* hold LED on */
#endif
        for(h=0;h<10;h++)
                if(pl[h].t != NULL_T)
                        turn_on_park_led(h);

        pc(25,80);

#ifdef AUTO_UPLOAD
        auto_upload(RESET,0,0);
#ifdef LIM
        auto_upload(RESET,1,0);
#if(NO_EXTENSION_C != 0)
        auto_upload(RESET,2,0);
#endif

#endif
#endif
}



int cpw(void){

        int code;
        if (data_entry_flag == SET){
                code=data_entry(saved_win);
                switch (code){
                  case ESC :
                        deletw(saved_win);
                        data_entry_flag=RESET;
                        cpw_flag=RESET;
                        if (cpw_part2() == OK){
                                saved_a1=dmmrc();
                                saved_a1->sp=1;
                        }
                        else{
                                dep_quit();
                        }
                        return code;
                  default :
                                return code;
                }
        }
        return NO_KEY_PRESSED;
}


int cpw_init_display(void){

        WINDOW *w;
        FIELD *f;

        w=enw(32,11,3,16);
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,cpw:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
#if 0
        swc(w,ALL,AQUA,WHITE,BRIGHT);
        swc(w,BORDER,RED,YELLOW,BRIGHT);
        swc(w,TITLE,RED,YELLOW,BRIGHT);
#endif // 0
        swc(w,ALL,KT_CYAN,KT_WHITE,KT_HIGH_INTENSITY);
        swc(w,BORDER,KT_RED,KT_YELLOW,KT_HIGH_INTENSITY);
        swc(w,TITLE,KT_RED,KT_YELLOW,KT_HIGH_INTENSITY);
//DOOD end
        swt(w,"ENTER PASSWORD");
        draw_window(w);
        idew(w);
        f=enf(w,0,0,pwm,pwb,PASSWORD);
        if(f == NULL){
                caw(); cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,cpw2:error in memory allocation!");
                quit(1);
        }
        caf(w);
        cpw_flag=SET;
        saved_win=w;
        data_entry_init(saved_win);
        return 0;
}


int cpw_part2(void){
        int a;
        sscanf(pwb,"%x",&a);
        if(a == p)
                return OK;
        else
                return 0;
}


void init_mac_nite_arr(void){
        mac_nite[0]=MAC_NITE_OPR0;
        mac_nite[1]=MAC_NITE_OPR1;
        mac_nite[2]=MAC_NITE_OPR2;
        mac_nite[3]=MAC_NITE_OPR3;
}
#ifdef CB_INSTALLED
int dcbp(BYTE n){  /* display extensions parameters */

        BYTE  code;

        code=get_c();
        switch (code){
          case ESC :
                getchar_hndlr_flag=RESET;
                deletw(saved_win);
                saved_b=0;
                saved_n=0;
                return code;
          case PGDN :
                saved_b++;
                if (saved_b < (no_cb[n] / NO_DISPLAY_ON_PAGE)){
                        dcbp_part1(n);
                        return code;
                }
                if (saved_b == (no_cb[n] / NO_DISPLAY_ON_PAGE)){
                        if ((no_cb[n] % NO_DISPLAY_ON_PAGE) > 0){
                                deletw(saved_win);
                                dcbp_init_display(n);
                                dcbp_part1(n);
                                return code;
                        }

                }
#ifdef LIM
                /* saved_b > */
                if ( saved_n == 0){
                        saved_b=0;
                        saved_n=1;
                        deletw(saved_win);
                        dcbp_init_display(saved_n);
                        dcbp_part1(saved_n);
                        return code;
                }
#if(NO_EXTENSION_C != 0)
                if ( saved_n == 1){
                        saved_b=0;
                        saved_n=2;
                        deletw(saved_win);
                        dcbp_init_display(saved_n);
                        dcbp_part1(saved_n);
                        return code;
                }
#endif
#endif
                saved_b--;
                return code;
          case PGUP :
                return code;
          default   :
                return code;

        }
}
#endif



#ifdef CB_INSTALLED
void dcbp_init_display(BYTE n){  /* display extensions parameters */

        WINDOW *w;
        int b,s,p;
        unsigned long h;

        s=no_cb[n] / NO_DISPLAY_ON_PAGE;
        p=no_cb[n] % NO_DISPLAY_ON_PAGE;
        b=saved_b;
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
        /* display extensions directory numbers */

        if (b != s)
                w=enw(0,0,21,80);
        else{
                if (p > 0){
                        w=enw(0,0,p+5,80);
                }
                else{
                        w=enw(0,0,21,80);
                }
        }
        if(w == NULL){
                caw();  cs();
//DOOD : printf->print_error_report
                print_error_report("\nDEPM,dcbp0:error in memory allocation!");
                quit(1);
        }
        dw(w);
        swb(w,DOUBLE_LINE);
//DOOD start
//      swc(w,ALL,YELLOW,WHITE,BRIGHT);
        swc(w,ALL,KT_YELLOW,KT_WHITE,KT_HIGH_INTENSITY);
//DOOD end
        swt(w," COIN BOX PARAMETERS ");
        draw_window(w);
        wprompt(w,0,0,"  Coin box                                cst                              ");

        wprompt(w,0,1,"                         AA SA HL TR PU T0 CF CR DD IN ST FM CP CB            ");
        wprompt(w,0,2,"------------------------------------------------------------------------------");
        saved_win=w;
}
#endif


#ifdef CB_INSTALLED
void dcbp_part1(BYTE n){  /* display extensions parameters */

        WINDOW *w;
        int b,c,s,p;
        unsigned int a;
        unsigned int d;
        int max_dsply;

        b=saved_b;
        w=saved_win;
        a=saved_a;
        s=no_cb[n] / NO_DISPLAY_ON_PAGE;
        p=no_cb[n] % NO_DISPLAY_ON_PAGE;
        wcursor(w,76,2);
        if (b != s )
                max_dsply=NO_DISPLAY_ON_PAGE;
        else
                max_dsply=p;
                for(c=0;c < max_dsply;c++,a++){
//                      wprintf(w,"\n%6.0d",a+FIRST_CB_NO);
                        wprintf(w,"\n%-7.0ld",a+first_cb_number);

                        wprintf(w,"        ");
                        wputchar(w,' ');
                        wprintf(w,"        ");
                        wprintf(w," ");

                        d=my_getw(fp[n]);
                        if(d & AMPLIFIER_ACCESS)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & SIREN_ACCESS)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & HOT_LINE_ACCESS)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & RING_AGAIN_TIET)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & CALL_PICKUP_USE)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & TIE_0)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & CONFERENCE_SERVICE)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & RING_AGAIN_COT)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & DONT_DIST)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & INTERNATIONAL)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & STD)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & FOLLOW_ME)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & CAMP_ON)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                        if(d & CALL_BACK)
                                wprintf(w," |");
                        else
                                wprintf(w," -");
                        wputchar(w,' ');
                                wprintf(w,"  ");
                        wputchar(w,' ');
                                wprintf(w,"  ");
                        wputchar(w,' ');
                                wprintf(w,"  ");
                        wputchar(w,' ');
                                wprintf(w,"   ");

                }
        saved_a=a;
}
#endif

#ifdef CB_INSTALLED
void dcbp_init(BYTE n){  /* display extensions parameters */
        dcbp_init_display(n);
        dcbp_part1(n);
        return;
}
#endif

#endif // (OS == DOS) || (OS == LINUX_COMPATIBLE)
#endif //NEW_DEP
