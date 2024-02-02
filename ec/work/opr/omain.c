
                     /****   omain.c   ****/

#include "omtfp.h"
#include "opr_net.h"

#ifdef OPERATOR_PC
#include "wg.h"
extern char mask18[];
#endif

//#define DEBUG_ACTIVE

//#define NET_DEBUG

#define AUDIT_ACT

#ifdef OPERATOR_PC
#include "ov.h"
#else
/*#include "ov_31.h"*/
#include "ov.h"
#pragma REGISTERBANK (0)        /* use Register Bank 0 for coding */
#endif

#ifdef IP_INSTALLED
extern unsigned int Routes[32][HASHMOD];       /* Routing table */
#endif

#include "/ec/work/sys/lintcp.ext"
extern CPR cpq[CP_QUE_LEN];
extern FPCP rtn_idx[OPRSIDE_NO_STATES][OPRSIDE_NO_STIMS];

extern TX_RCRD tx[];
extern RX_RCRD rx[];
#ifdef NEW_DEP
extern BYTE dep_flag;
#endif

#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
extern unsigned int tcp_link_0;
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
extern BUF nettx_st[];
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
extern BYTE link_flag[];
extern BYTE hot_link;
#ifdef HOT_STANDBY
extern BYTE htby_link_flag;
#endif
#endif

#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef OPR_UPLOAD_WITH_TIMER

extern unsigned int upload_count;
#if ((NO_OPRTR > 1) && (defined HOT_STANDBY))
extern unsigned short upload_reject_flag;
#endif

#endif // OPR_UPLOAD_WITH_TIMER
#endif // PARAMETER_DOWN_UP_LOAD
#ifdef DEP_MANUAL_TEST
extern TEST_ST tst;
extern unsigned int subscriber_state_block[];
extern BYTE subscriber_state_block_ptr;
#endif

#ifdef DEP_MANUAL_TEST
extern WINDOW *saved_win2;
#endif

#ifdef CB_INSTALLED
extern unsigned long first_cb_number;
#endif

//#if ((defined CALL_PROCESSOR) && (defined EXT_INSTALLED))
extern unsigned int no_of_subscriber_groups;
extern SUBSCRIBER_GROUP subscriber_group[MAX_NO_OF_SUBSCRIBER_GROUPS];
//#endif
// OLAGH start: block added
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
pthread_t etc_thread,init_thread;
pthread_mutex_t  f_intr_mutex_flag;
pthread_cond_t   f_intr_cond_flag;
pthread_mutex_t  start_mutex_flag,init_mutex_flag;
pthread_cond_t   start_cond_flag,init_cond_flag;
int kurtdev;
#endif
#endif
// OLAGH end

/* ============================================================================ */
/* ============================================================================ */
void read_site_parameters(void){

    site_data.no_office_code_dgts=0;
    site_data.no_of_subscriber_groups=0;

    {
        FILE *fp;
        fp=fopen("sitedata.dat","rb");
        if(fp != NULL_POINTER){     // COMPILER_NULL
            char line[255];
            BYTE subscriber_group=0;
            while(fgets(line,255,fp) != NULL_POINTER){ // COMPILER_NULL
                char *p;
                if(line[0] == '*') continue;
                p=strtok(line, " \t\r\n");
                if(p != NULL_POINTER){
                    if(strcmp(p,"OFFICE_CODE") == 0){
                        p=strtok(NULL_POINTER, " \t\r\n");
                        if(p != NULL_POINTER){
                            if(!isdigit(p[0]))
                                site_data.no_office_code_dgts=0;
                            else{
                                unsigned long office_code;
                                int i,k;
                                office_code=strtol(p,0,16);
                                for(i=0;i<6;i++)
                                    if((office_code>>((5-i)*4)) != 0)
                                        break;
                                site_data.no_office_code_dgts=6-i;
                                for(k=0;k<site_data.no_office_code_dgts;k++){
                                    site_data.office_code_dgts[k]=(office_code>>((site_data.no_office_code_dgts-k-1)*4))&0xf;
                                }
                            }
                        }
                    }
                    else
                    if(strcmp(p,"FDN") == 0){
                        p=strtok(NULL_POINTER, " \t\r\n");
                        if(p != NULL_POINTER){
                            site_data.directory_start[subscriber_group]=atosdnst(p);
                            subscriber_group++;
                        }
                    }
                }
            }
            fclose(fp);
            site_data.no_of_subscriber_groups=subscriber_group;
        }
    }
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
    if((site_data.no_office_code_dgts == 0) ||
       (site_data.no_of_subscriber_groups == 0)){
        print_error_report("\nPlease include a proper sitedata.dat file!\n");
        quit(1);
    }
#endif
#endif
}
/* =========================================================================== */
/* =========================================================================== */
#if (OS == DOS)
int main(void){
    kt_all_initialize();
    kt_forever_loop();
    return 0;
}
#endif
/* ============================================================================ */
/* ============================================================================ */
int kt_all_initialize(void){
#ifdef SI_DEBUG_ON_U
        char s[10];
#endif
        unsigned long l;int i,j;
#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
#ifdef NEW_DEP
        fpos_t pos;
#endif
#endif
#endif


#ifdef OPERATOR_PC
        FILE *fp;
// to be done also for linux
#if (OS == DOS)
        freopen("stdout.dat","wb",stdout);
#endif
        opr_gonz_fp=fopen("gonzales","a+b");
        opr_log_fp=fopen("log.txt","a+b");
        init_debug();
#endif


#ifdef KIATEL_DYNAMIC_MEM
        init_mem_rec();
        dynamic_mem_init();
#endif
#ifdef SPEED_DIALLING
        first_sp_node_pt=NULL_POINTER;
        read_sp_file();
#endif
#ifdef TIMERS_FOR_OPR
        {
            TMR *tmp;
            for(i=0,tmp=&timer[0];i < TIMER_NO;i++,tmp++){
                tmp->i=KT_NULL;  /* timer is in the queue */
                tmp->rptr=i+1;
                tmp->lptr=i-1;
            }
        }
        timer[TIMER_NO-1].rptr=KT_NULL; /*last timer */
        ftp=0;
        ltp=TIMER_NO-1;
        for(i=0;i < NO_TMR_QPTR;i++)
                tpt[i]=KT_NULL;
        tpti=0;
#endif

#if (((defined NEW_CC_STRATEGY)||(defined CALL_PROCESSOR)) && (defined EXT_INSTALLED))
#ifdef OPERATOR_PC
//              FILE *fp;
        for(i=0;i<(90+10);i++)
                buf110[i][0]='\0';
        for(i=0;i<10;i++){
                buf1819_1[i]=(unsigned int)(-1);
                buf1819_2[i][0]='\0';
        }
        if( (fp=fopen("fdn","r+b")) == NULL){ /* file does not exist */
                f_d_n=F_D_N;
                if( (fp=fopen("fdn","wb")) == NULL){
                        print_error_report("\nOPPC: cannot create directory number file!\n");
                        exit(0);
                }
                else{
                        fprintf(fp,"%lx",f_d_n);
                        ss[0]=0xffff;
                        fclose(fp);
                }
        }
        else{
                fscanf(fp,"%lx",&f_d_n);
                i=0;
                while(((j=fscanf(fp,"%d",&ss[i])) != 0) && (j != EOF)){
                        if((ss[i] >= (110-10)) && (ss[i] <= 199))
                                fscanf(fp,"%s",buf110[ss[i]-(110-10)]);
                        else{
                                if(i < 10){
                                        buf1819_1[i]=ss[i];
                                        fscanf(fp,"%s",buf1819_2[i]);
                                        i++;
                                }
                        }
                }
                ss[i]=0xffff;
                fclose(fp);
        }
#else
        f_d_n=F_D_N;
#endif
        for(l=f_d_n,no_intrnl_dgt=0;l != 0L; l>>=4,no_intrnl_dgt++);
        intrnl_1st_dgt=(f_d_n >> ((no_intrnl_dgt-1) * 4) );
        for(l=f_d_n,i=0;l > 0xfL; l>>=4,i++);           // find most significant nibble
        j=(int)l;
        l=hex2l(f_d_n) + MAX_EXISTING_NO; // NO_TOTAL_EXTENSION;
        l=l2hex(l);
        for(i=0;l > 0xfL; l>>=4,i++);           // find most significant nibble
        intrnl_1st_dgt_2=0xff;
        intrnl_1st_dgt_3=0xff;
        intrnl_1st_dgt_4=0xff;
        intrnl_1st_dgt_5=0xff;
        if(j != (int) l){
            intrnl_1st_dgt_2=(intrnl_1st_dgt + 1);
            if( (j+1) != (int) l){
                intrnl_1st_dgt_3=(intrnl_1st_dgt + 2);
                if( (j+2) != (int) l){
                    intrnl_1st_dgt_4=(intrnl_1st_dgt + 3);
                    if( (j+3) != (int) l){
                        intrnl_1st_dgt_5=(intrnl_1st_dgt + 4);
                    }
                }
            }
        }
        first_directory_decimal=hex2l(f_d_n);
#endif
#if (((defined NEW_CC_STRATEGY)||(defined CALL_PROCESSOR)) && (defined CB_INSTALLED))
#ifdef FIRST_CB_NUMBER
        first_cb_number=hex2l(FIRST_CB_NUMBER);
#else
        for(i=0,first_cb_number=1L;i<(no_intrnl_dgt-1);i++)
                first_cb_number=10L*first_cb_number;
#endif
#endif

        dep_fill_params();

        init_array();

#if ((defined CALL_PROCESSOR) && (defined EXT_INSTALLED))
        init_subscriber_groups_data();
#endif
#if (  (defined LAPB_ON_OPERATOR_SIDE) || (defined HDLC_RS232_ON_OPERATOR_SIDE)\
     || (defined TCP_ON_CLIENT_SIDE)    || (defined TCP_ON_SERVER_SIDE) \
     || (defined ARP_INSTALLED)                                         \
     || (defined IP_INSTALLED)                                          \
     || (defined TCP_INSTALLED)                                         \
     || (defined UDP_INSTALLED)                                         \
     || (defined ETHERNET_INSTALLED)                                    \
    )
        init_mess();
#endif
#ifdef DYNAMIC_IP_ALLOCATION
#ifdef MY_UDP_FOR_OAM
{
        unsigned int n;
        for(n=FIRST_UDP_HDLC_CH;n<LAST_UDP_HDLC_CH;n++){
                 ms_button[n]=n;
        }
}
#endif
#endif

#ifdef NET_DEBUG
        if( (net_fp=fopen("net.dbg","a+t")) == NULL)
                print_error_report("\nOPPC: cannot create network debug file!\n");
#endif


#ifdef OPERATOR_PC
        if( (fer=fopen("err.dbg","a+t")) == NULL)
                print_error_report("\nOPPC: cannot create error debug file!\n");
#endif

#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
#ifdef NEW_DEP
        if( (flog=fopen(ALARM_FILE,"r+t")) == NULL){
#else
        if( (flog=fopen(ALARM_FILE,"rt")) == NULL){
#endif
#ifdef NEW_DEP
                if((flog=fopen(ALARM_FILE,"w+t")) == NULL)
#else
                if((flog=fopen(ALARM_FILE,"wt")) == NULL)
#endif
                        print_error_report("\nOPPC: cannot create alarm log file!\n");
                else{
                        date_to_log();
#ifdef NEW_DEP
                        fgetpos(flog,&pos);
                        fseek(flog,0,SEEK_END);
#endif
                        fprintf(flog,"ALARM LOG FILE CREATED");
#ifdef NEW_DEP
                        fsetpos(flog,&pos);
#endif
                }
        }
        else{
                fseek(flog,0,SEEK_END);
                logsize=ftell(flog);
                if(logsize >= MAX_LOG_FILE_SIZE){
                        kt_system(log_file_backup_string);
#ifdef NEW_DEP
                        if((flog=fopen(ALARM_FILE,"w+t")) == NULL)
#else
                        if((flog=fopen(ALARM_FILE,"wt")) == NULL)
#endif
                                print_error_report("\nOPPC: cannot create alarm log file!\n");
                }
                else{
#ifndef NEW_DEP
                        fclose(flog);
                        if( (flog=fopen(ALARM_FILE,"a+t")) == NULL)
                                print_error_report("\nOPPC: cannot access alarm log file!\n");
#endif
                }
        }

#endif
#endif

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

        init_cas3_hst_file();
#endif
#endif
#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

        init_em_hst_file();
#endif
#endif
#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

        init_cas1_hst_file();
#endif
#endif


#ifdef OPERATOR_PC
        f=KBRF;  /* f used in init_file(),default is English language */
                 /* init_file() must be called before initialize(), */
                 /* because in initialize(),toal() is called. */
#endif


#ifdef OPERATOR_PC
#ifndef HDLC_RS232_ON_OPERATOR_SIDE
        if(!reset_mouse()){
//              print_error_report("\nSorry, no mouse driver installed.");
//              exit(1);

        }
#endif


        init_file();
        init_layo();
        display_mouse_ptr();
#ifdef UPDATE_DATE_TIME
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        ddt_ad_always();
#endif
#endif

#endif

#ifdef MY_TCPIP_FOR_OAM
        nettx_st0.arr=&tx0[0];
        nettx_st0.front=nettx_st0.rear=0;
        nettx_st0.size=MY_TCP_TX_NETBUF_SIZE;

        nettx_st1.arr=&tx1[0];
        nettx_st1.front=nettx_st1.rear=0;
        nettx_st1.size=MY_TCP_TX_NETBUF_SIZE;
#endif

#ifdef ARP_INSTALLED
        init_arp_type_array();
#endif
#ifdef IP_INSTALLED
        init_iface_struct();
#endif
#ifdef IP_INSTALLED
        init_hopper_q();
        ip_init();
#endif
#if ((defined TCP_INSTALLED) || (defined UDP_INSTALLED))
        tcp_udp_init();
#endif
#ifdef ETHERNET_INSTALLED
        init_pktdrvr_st();
/*
        for(i=0;i<PK_MAX;i++){
                pktdrvr_st[i].iface_no=NULL_IFACE;
                init_pktdrvr_buf(i);
        }
*/
#endif
#ifdef IP_INSTALLED
        for(i=0;i<MAX_IFACE;i++){
                init_ip_buf(i);
        }
        for(i=0;i<32;i++){
                for(j=0;j<HASHMOD;j++){
                        Routes[i][j]=NULL_IP_ROUTE;
                }
        }
#endif

#ifdef LINKLAYER_DEBUG_ACT
        open_lapb_err_file();
#endif

#ifdef HDLC_RS232_ON_OPERATOR_SIDE
//should be here,before init_lapb().
        init_hdlc_ports();
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
        init_link_handler();
#endif
#ifdef LAPB_ON_OPERATOR_SIDE
        init_lapb();
#endif
#ifdef KIATEL_UDP_INSTALLED
        read_kiatel_udp_config_file();
#endif
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
        init_tcp();
#endif
#ifdef NEW_DEP
        dep_flag = RESET;
#endif
        init_terminal();
        initialize();

#ifdef MALICIOUS_PRINT_ON_U
#if ((defined MY_UDP_FOR_OAM) || (TON == MALICIOUS_PRINT_OPR))
#ifdef NEW_DEP
        if(cmpf() == FAIL){
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("!ñ®˛˜ ˝§¯ç‚ıõ ÒìëÏ ¸ìë˛ Ùüê•ı");
                 else
#endif
                        str2lcd("MALICIOUS INFORMATION CANNOT BE COLLECTED!");
#else
                str2lcd("NO MALC. INF.");
#endif
        }
#endif // NEW_DEP
#endif /* TON == MALICIOUS_PRINT_OPR */
#endif // MALICIOUS_PRINT_ON_U

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        wvi();  /* windows variables initialization */
#endif
        fec_pc();
#endif

#ifdef PARAMETER_DOWN_UP_LOAD
        validate_parameters();
#endif

#ifdef SI_DEBUG_ON_U
        for(i=0;i<card;i++)
                if( (fsi[i]=fopen(strcat(kt_itoa(i+1,s,10),"sid.dbg"),"a+t")) == NULL)
                        print_error_report("\nOPPC: cannot create system debug file!\n");
#endif

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        if(card!=0)     /* this line not needed */
                   for(i=0;i<card;i++)
                                    roni(i);
#endif
        init_intr();
#else
        roni(0);
        init_t0_srl();
#endif

#ifdef PC_KEYBOARD
        init_keyboard();
#endif

#ifdef AUTO_UPLOAD
        auto_upload(RESET,0,0);
#ifdef LIM
        auto_upload(RESET,1,0);
#if(NO_EXTENSION_C != 0)
        auto_upload(RESET,2,0);
#endif

#endif
#endif
#if (((defined NEW_CC_STRATEGY)||(defined CALL_PROCESSOR)) && (defined EXT_INSTALLED))
        send_fdn();
#endif

#ifdef NEW_DEP
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        dep_init();
#endif
#endif
#ifdef HOT_STANDBY
        htby_link_flag=RESET;
        pmamftx_both_sys(MA_SYS_P | MAC_HOT_SYSTEM_CHECK,TON,OPR_T,0);
        set_stm_for_htby_check();
        htby_wait_win();
        hot_link=UNKNOWN_LINK;
#ifdef PARAMETER_DOWN_UP_LOAD
#if (NO_OPRTR > 1)
        upload_reject_flag=RESET;
#endif
#endif
#else  // HOT_STANDBY
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
        hot_link=LAPB_LINK0;
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
        hot_link=tcp_link_0;
#endif

#ifdef MY_TCPIP_FOR_OAM
        hot_link=0;
#endif
#endif //HOT_STANDBY

#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        init_led_attr();
#endif
        init_prntr();

#ifdef ETHERNET_INSTALLED
        all_pk_attach();
#endif
#ifdef MY_TCPIP_FOR_OAM
        start_oam_tcp_active_port(0);
#ifdef HOT_STANDBY
        start_oam_tcp_active_port(1);
#endif
#endif
        {
            FILE *fp;
            strcpy(exchange_name,"KIATEL Generic Site");
            strcpy(exchange_office_code,"KIATEL Generic Office Code");
            strcpy(exchange_area_code,"KIATEL Generic Area Code");
            fp=fopen("exchange.dat","rb");
            if(fp != NULL_POINTER){     // COMPILER_NULL
                char line[255];
                while(fgets(line,255,fp) != NULL_POINTER){ // COMPILER_NULL
                    char *p;
                    if(line[0] == '*') continue;
                    p=strtok(line, ":");
                    if(p != NULL_POINTER){
                        if(strcmp(p,"Name") == 0){
                            p=strtok(NULL_POINTER, "\r\n");
                            if(p != NULL_POINTER){
                                strncpy(exchange_name,p,99);
                            }
                        }
                        else
                        if(strcmp(p,"Office Code") == 0){
                            p=strtok(NULL_POINTER, "\r\n");
                            if(p != NULL_POINTER){
                                strncpy(exchange_office_code,p,99);
                            }
                        }
                        else
                        if(strcmp(p,"Area Code") == 0){
                            p=strtok(NULL_POINTER, "\r\n");
                            if(p != NULL_POINTER){
                                strncpy(exchange_area_code,p,99);
                            }
                        }
                    }
                }
                fclose(fp);
            }

        }
        read_site_parameters();
        return 0;
}
/* ============================================================================ */
/* ============================================================================ */
int kt_forever_loop(void){
        while(1){
#ifdef HOT_STANDBY
#if (OS == DOS)
                if (hot_link == UNKNOWN_LINK){
                        if (get_c() == ESC) quit(2);
                }
#endif
#endif
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
#ifdef NEW_DEP
                if (dep_flag == SET){
                        dep_main();
                }
#endif
                if(term_flag == SET){
                        terminal();
                }

#ifdef OPERATOR_PC
                if(f & OPR_CARD_OUT)
                        quit(2);
#endif
#endif
                scheduler();

#ifdef DISPLAY_EM_STATUS
                if(dtsf){
                        omap();
                        disp_trunk();
                }
#endif
#ifdef DISPLAY_CAS3_STATUS
                if(dcsf){
                        omap();
                        disp_cas3();
                }
#endif
#ifdef DISPLAY_CAS1_STATUS
                if(d1sf){
                        omap();
                        disp_cas1();
                }
#endif
                cp();
//              if(f & INTRF) continue;
                if(f_intr()) continue;
#ifdef OPERATOR_PC
#if ( (PARAMETER_DOWN_UP_LOAD == DEFINE_VALUE) || (DETAIL_BILLING == DEFINE_VALUE) || (defined SI_DEBUG_ON_U) )
#ifdef DISPLAY_EM_STATUS
                if(!dtsf)
#endif
#ifdef DISPLAY_CAS3_STATUS
                if(!dcsf)
#endif
#ifdef DISPLAY_CAS1_STATUS
                if(!d1sf)
#endif
                omap();
//              if(f & INTRF) continue;
                if(f_intr()) continue;
#endif
#endif


#ifdef OPERATOR_PC

#ifdef DISPLAY_STATUS_ACT
                if(dxsf){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
#ifndef NEW_DISPLAY_STATUS
                        chng_layo(ds);
#else
                        chng_layo();
#endif
#endif
                }
#endif
//              if(f & INTRF) continue;
                if(f_intr()) continue;

#ifdef DETAIL_BILLING

#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
                dbrc();  /* detail billing records collection */
//              if(f & INTRF) continue;
                if(f_intr()) continue;

#endif

#endif


#ifdef SI_FOR_BULK_BILLING
                sdt=*get_dt(&sdt);
                if( (sdt.hr == HOUR)  &&  (sdt.mi == MINUTE) )
                        quit(1);
//              if(f & INTRF) continue;
                if(f_intr()) continue;
#endif

#ifdef SCHEDULED_UPLOAD
                sdt=*get_dt(&sdt);
                if( (sdt.hr == DAY_HOUR)  &&  (sdt.mi == DAY_MINUTE) && ((unsigned)(sdt.se-DAY_SECOND)<10) ){
                        auto_upload(SET,0,0);
#ifdef LIM
                        auto_upload(SET,1,0);
#if(NO_EXTENSION_C != 0)
                        auto_upload(SET,2,0);
#endif
#endif
                }
                else
                if( (sdt.hr == NITE_HOUR)  &&  (sdt.mi == NITE_MINUTE) && ((unsigned)(sdt.se-NITE_SECOND)<10) ){
                        auto_upload(SET,0,1);
#ifdef LIM
                        auto_upload(SET,1,1);
#if(NO_EXTENSION_C != 0)
                        auto_upload(SET,2,1);
#endif
#endif
                }
//              if(f & INTRF) continue;
                if(f_intr()) continue;
#endif


#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
                ud();  /* update date */
                if(f_intr()) continue;
#endif
#endif  // DETAIL_BILLING

#ifdef TCP_INSTALLED
                if(f_intr()) continue;
                so_tcp_recv_hndlr();
#endif
#endif  // OPERATOR_PC


#ifdef AUDIT_ACT
//              if( (f & INTRF) == RESET)
                if(f_intr() == RESET)
                        oam();
#endif
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
        // this routine should be called while mutex is locked,and will automatically release
        // the mutex while it waits.
        pthread_mutex_lock(&f_intr_mutex_flag);
        pthread_cond_wait (&f_intr_cond_flag, &f_intr_mutex_flag);
        // when wake up,mutex is automatically locked.
        pthread_mutex_unlock(&f_intr_mutex_flag);
#endif
#endif
#if (COMPILER == GCC)
#if (OS == DOS)
                asm("\nxxx:\tmovb\t_f,%al\n\ttestl\t$1,%eax\n\tje\txxx");
#endif
#endif
#if (COMPILER == BCC)
//s1:           if(f & INTRF) continue;
s1:             if(f_intr()) continue;
                goto s1;
#endif
//              while( (f & INTRF) == RESET);
//              while(f_intr() == RESET);
        }
        return 0;
}
/* ============================================================================ */
/* ============================================================================ */

#ifdef PARAMETER_DOWN_UP_LOAD
void validate_parameters(void){

#if 0
      int i;

        for(i=0;i<card;i++){
#ifdef DAY_NIGHT_PARAMETERS_FILE
                pfp[i]=fopen(pfna[i][0],"rb");
                if( (pfp[i] == NULL) || (vpf(pfp[i],i) != OK) ){
                /* if parameters file does not exists or not valid */
                        clear_screen();
                        remove_mouse_ptr();
                        print_error_report("\nCan not open parameters file.\n\n");
                        exit(1);
                }

                pfp[i]=fopen(pfna[i][1],"rb");
                if( (pfp[i] == NULL) || (vpf(pfp[i],i) != OK) ){
                /* if parameters file does not exists or not valid */
                        clear_screen();
                        remove_mouse_ptr();
                        print_error_report("\nCan not open parameters file.\n\n");
                        exit(1);
                }
#else
                pfp[i]=fopen(pfna[i][0],"rb");
                if( (pfp[i] == NULL) || (vpf(pfp[i],i) != OK) ){
                /* if parameters file does not exists or not valid */
                        clear_screen();
                        remove_mouse_ptr();
                        print_error_report("\nCan not open parameters file.\n\n");
                        exit(1);
                }
#endif
        }

#endif // 0

#ifdef PARAMETER_DOWN_UP_LOAD
//      if(card!=0)
                gcp();  /* get current password */
#else
        pswd_d2d1=PSWD_D2D1;
        pswd_d4d3=PSWD_D4D3;
#endif

}
#endif

void initialize(){
        int i;
#ifdef NEW_DEP
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
//it is necessary when this routine is called after detecting LOS.
        if (dep_flag == SET ) dep_quit();
#endif
#endif
        cpfq=cpeq=0;
        mafq=maeq=0;
        time_tab[0] =0x8b03;      /* 1 0 0 0 1 0 1 1  0 0 0 0 0 0 1 1 */
        time_tab[1] =0x0d06;      /* 0 0 0 0 1 1 0 1  0 0 0 0 0 1 1 0 */
        time_tab[2] =0x0b03;      /* 0 0 0 0 1 0 1 1  0 0 0 0 0 0 1 1 */
        time_tab[3] =0x0d8a;      /* 0 0 0 0 1 1 0 1  1 0 0 0 1 0 1 0 */
        time_tab[4] =0x0b03;      /* 0 0 0 0 1 0 1 1  0 0 0 0 0 0 1 1 */
        time_tab[5] =0x0d16;      /* 0 0 0 0 1 1 0 1  0 0 0 1 0 1 1 0 */
        time_tab[6] =0x0b03;      /* 0 0 0 0 1 0 1 1  0 0 0 0 0 0 1 1 */
        time_tab[7] =0x0d22;      /* 0 0 0 0 1 1 0 1  0 0 1 0 0 0 1 0 */
        time_tab[8] =0x0b03;      /* 0 0 0 0 1 0 1 1  0 0 0 0 0 0 1 1 */
        time_tab[9] =0x0d06;      /* 0 0 0 0 1 1 0 1  0 0 0 0 0 1 1 0 */
        time_tab[10]=0x0b03;      /* 0 0 0 0 1 0 1 1  0 0 0 0 0 0 1 1 */
        time_tab[11]=0x0d42;      /* 0 0 0 0 1 1 0 1  0 1 0 0 0 0 1 0 */
        time_tab[12]=0x0b03;      /* 0 0 0 0 1 0 1 1  0 0 0 0 0 0 1 1 */
        time_tab[13]=0x0d16;      /* 0 0 0 0 1 1 0 1  0 0 0 1 0 1 1 0 */
        time_tab[14]=0x0b03;      /* 0 0 0 0 1 0 1 1  0 0 0 0 0 0 1 1 */
        time_tab[15]=0x0d22;      /* 0 0 0 0 1 1 0 1  0 0 1 0 0 0 1 0 */
        pph=0;
        tptw=time_tab[0];

        no_iqr=IQR_NO;
        {
            FILE *fp=fopen("iqrno.dat","rb");
            if(fp != NULL_POINTER){     // COMPILER_NULL
                char line[255];
                while(fgets(line,255,fp) != NULL_POINTER){ // COMPILER_NULL
                    if(line[0] == '*') continue;
                    unsigned int i1;
                    sscanf(line,"%d",&i1);
                    if((i1 > 0) && (i1 <= IQR_NO))
                        no_iqr=i1;
                }
                fclose(fp);
            }
        }

        //for(i=0;i<IQR_NO;i++){
        for(i=0;i<no_iqr;i++){
                iqr[i].rp=i+1;
                iqr[i].lp=i-1;
//              iqr[i].i=0xff;
                iqr[i].i=(int)(-1);
        }
        //iqr[IQR_NO-1].rp=0xff;
        iqr[no_iqr-1].rp=0xff;
        firf=0;
        //fire=IQR_NO-1;
        fire=no_iqr-1;
        for(i=0;i<(RXQR_NO);i++){
                rxqr[i].rp=i+1;
                rxqr[i].lp=i-1;
                rxqr[i].pt.t=NULL_T;
        }
        rxqr[RXQR_NO-1].rp=0xff;
        frxf=0;
        frxe=RXQR_NO-1;
        fqi=eqi=0xff;
        fqr=eqr=0xff;
        fqx=eqx=0xff;
        pqi=fqi;
        shown_pqi=0xff;
        pqx=fqx;
        shown_pqx=0xff;
        last_shown_pq=0;
#ifdef EM_CONT_SEP_OPR_Q
        fqem=eqem=0xff;
#endif

/* default operator state is NIGHT */
        ocr.state=OPRSIDE_OPR_NIGHT;
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        ocr.fs=0xff;

        for(i=0;i < NO_EXTENSION;i++){

                xst[i].s=IDLE;
#ifdef OPR_PRI_CONNECTION
                xst[i].dgt=NULL_POINTER;
                xst[i].no_dgt=0xff;
                xst[i].userside_pri=RESET;
                xst[i].og=RESET;
#endif
        }


#if(NO_CO != 0)
        for(i=0;i < NO_CO;i++)
                ctst[i].s=IDLE;
#endif


#ifndef NO_TIE_INIT
        for(i=0;i < NO_TIE;i++)
                ttst[i].s=IDLE;
#endif


#ifdef OPR_EM_CONNECTION
        for(i=0;i < NO_EM;i++)
                emtst[i].s=IDLE;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
//DYNAMIC : NO_CAS3_CHANNEL --> no_cas3_channel[0]
        for(i=0;i < no_cas3_channel[0];i++)
                dtcst[i].s=IDLE;
#endif

        for(i=0;i<10;i++)
                pl[i].t=NULL_T;
        hld.t=NULL_T;
        for(i=0;i<NO_HOLD_KEY;i++)
                hldl[i].t=NULL_T;

        if(rst_sgntr == FORCE_RESET_SIGNATURE)
        /* if reset by network functions,do not change other flags */
                f |=KBRF;
        else{
                f=KBRF;
                rst_sgntr=RESET_SIGNATURE;
        }
        lkp=0xff;

#ifdef TIME_DATE_DISPLAY
        csra=0;
#endif

        flcdq=elcdq=0;
        lcdf=0;
        clc=0;
        lf=0;
        trf=TONE_RNG_ACT;       /* tone ringer active */
        trs=0xff;
        trb=1;
        tr_pat=XQ_TR_PAT;
        ohs=ON_HOOK;

        cprti();  /* call processing routine table initializtion */

        trg=TONE_RINGER_GAIN;

#ifdef OPERATOR_PC
        hkl[0]=HOLD1_LED;
        hkl[1]=HOLD2_LED;
        hkl[2]=HOLD3_LED;
        hkl[3]=HOLD4_LED;
#else
        hkl=HOLD1_LED;
#endif

        iqs=0;
        rqs=xqs=0;
#ifdef EM_CONT_SEP_OPR_Q
        emqs=0;
#endif

        lsg1=0;
        lpg1=NLS_BIT_POS;
        /* NIGHT LED programmed */

//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
        oprnight=0;
        oprday=0;
#endif
//end night upload

        xlc=rlc=ilc=0;
#ifdef EM_CONT_SEP_OPR_Q
        emlc=0;
#endif
        plc=nlc=0;


#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("           ...ñ®ê Òë„Í §¯óê§ïê í™ ¸Û°ê¢");
        else
#endif
                str2lcd("OPERATOR NIGHT ACTIVE... ");
#else
        init_lcd();
        str2lcd("OPR. NIGHT ACT..");
#endif


#ifdef DISPLAY_STATUS_ACT
        for(i=0;i < NO_TOTAL_EXTENSION;i++)
                ds[i]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
        current_page=0;
#endif
#endif
#ifdef DISPLAY_CAS3_STATUS
        current_cas3_page=0;
#endif

        unmask_stm();

#ifdef PARAMETER_DOWN_UP_LOAD
        fill_params();
        make_param_files();
#endif


#ifndef OPERATOR_PC
        lpb=0;
        lpnb=0;
#endif

#ifdef DETAIL_BILLING
/* if this operator is detail billing collector */
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
        for(i=0;i < NO_DETAIL_BILLING_RECORDS;i++){
                dbr[i].lp=i-1;
                dbr[i].rp=i+1;
                dbr[i].s=0;
        }
        dbr[NO_DETAIL_BILLING_RECORDS - 1].rp=NIL;
        fdbrqf=0;
        fdbrqe=NO_DETAIL_BILLING_RECORDS - 1;
        for(i=0;i<MAX_NO_OPR_CARD;i++){
                complete_rdbs[i]=0;
                complete_rdbi[i]=0;
        }
#endif
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
        for(i=0;i<MAX_NO_OPR_CARD;i++){
                rdbs[i]=0;
                rdbi[i]=0;
        }
        adbrqf=adbrqe=NIL;
        sdt=*get_dt(&sdt);
        cd=sdt.da;
        if(cdbfn() == FAIL){

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("!ñ®˛˜ ˝§¯ç‚ıõ ÒìëÏ ¸é•õ íë®ü ñ§¯¨");
                 else
#endif
                        str2lcd("DETAIL BILLING CANNOT BE COLLECTED!");
#else
                str2lcd("NO DETAIL BILL");
#endif
        }

#ifdef BILLING_BACKUP
        setdisk(3);
        if(chdir("d:\\opr") == 0){
                if(cdbfn() == FAIL){
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG)
                                str2lcd("!ñ®˛˜ ˝§¯ç‚ıõ ÒìëÏ ¸é•õ íë®ü ñ§¯¨");
                        else
#endif
                                str2lcd("DETAIL BILLING BACKUP CANNOT BE COLLECTED!");
                }
        }
        setdisk(2);
#endif  // BILLING_BACKUP
#endif  // TON == DETAIL_BILLING_OPR
#endif  // DETAIL_BILLING


#ifdef REPORT_BULK_BILLING

#if( (TON == BULK_BILLING_OPR) || ((defined SECOND_BULK_BILLING_OPR) && (TON == SECOND_BULK_BILLING_OPR)) )

        for(i=0;i<MAX_NO_OPR_CARD;i++){
                rbbs[i]=0;
                rbbi[i]=0;
        }

        if(cbbf() == NIL){
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("!ñ®˛˜ ˝§¯ç‚ıõ ÒìëÏ ¸ÛÓ íë®ü ñ§¯¨");
                 else
#endif
                        str2lcd("BULK BILLING CANNOT BE COLLECTED!");
#else
                str2lcd("NO BULK BILL");
#endif
        }

#endif
/* TON == BULK_BILLING_OPR */

#endif

#ifdef MALICIOUS_PRINT_ON_U
#if (TON == MALICIOUS_PRINT_OPR)
        for(i=0;i<MAX_NO_OPR_CARD;i++){
                rmps[i]=0;
                rmpi[i]=0;
        }
#endif
#if ((defined MY_UDP_FOR_OAM) || (TON == MALICIOUS_PRINT_OPR))
        for(i=0;i<MAX_NO_OPR_CARD;i++){
                complete_rmps[i]=0;
                complete_rmpi[i]=0;
        }
#ifndef NEW_DEP
        if(cmpf() == FAIL){
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("!ñ®˛˜ ˝§¯ç‚ıõ ÒìëÏ ¸ìë˛ Ùüê•ı");
                 else
#endif
                        str2lcd("MALICIOUS INFORMATION CANNOT BE COLLECTED!");
#else
                str2lcd("NO MALC. INF.");
#endif
        }
#endif // NEW_DEP
#endif /* TON == MALICIOUS_PRINT_OPR */
#endif // MALICIOUS_PRINT_ON_U



#ifdef PARAMETER_DOWN_UP_LOAD
        f &= ~PARM_DOWN;
        f &= ~PARM_UP_1;
        f &= ~PARM_UP_2;
        f &= ~PARM_UP_3;
#endif

#ifdef DISPLAY_STATUS_ACT
        f &= ~DSF;
        dxsf=RESET;
#endif
#ifdef DISPLAY_EM_STATUS
        dtsf=RESET;
#endif
#ifdef DISPLAY_CAS1_STATUS
        d1sf=RESET;
#endif
#ifdef SYSTEM_MANUAL_TEST
        mtf=RESET;
#endif
#ifdef DISPLAY_BILLING
        bill_display_flag=RESET;
#endif
#ifdef DISPLAY_ALARM
        alarm_display_flag=RESET;
#endif

#ifdef OPERATOR_PC
        toal();  /* trun off all LEDs */
#endif

#ifdef PRE_INSTALL

#ifdef OPERATOR_PC
        if( (flp=fopen(LOCAL_PARAM_FILE,"r+b")) == NULL){ /* file does not exist */
                org=OPR_RCVR_GAIN;
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
                op_can_dial_on_co=OP_FIRST_ZERO_ON_CO_DEFAULT;
#endif
                if( (flp=fopen(LOCAL_PARAM_FILE,"wb")) == NULL){
                        print_error_report("\nOPPC: cannot create local parameters file!\n");
                }
                else{
                        putc(org,flp);
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
                        putc(op_can_dial_on_co,flp);
#endif
                        fclose(flp);
                }
        }
        else{
                org=getc(flp);
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
                op_can_dial_on_co=getc(flp);
#endif
                fclose(flp);
        }
#else
        org=OPR_RCVR_GAIN;
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
        op_can_dial_on_co=OP_FIRST_ZERO_ON_CO_DEFAULT;
#endif
#endif

#else

#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
        op_can_dial_on_co=OP_FIRST_ZERO_ON_CO_DEFAULT;
#endif

#endif
}

void scheduler(){
        unsigned int b;
#ifdef HDLC_8993
        int p;
#endif
//      f &= ~INTRF;
        reset_f_intr();
        f |=SCHLF;
        b=1;
        pph= (pph + 1) & 0x0f;
        tptw=time_tab[pph] & stm;
//      tptw &= stm;
        while(tptw){
//              if(f & INTRF){
//                      f &= ~INTRF;
                        if(f_intr()){
                                        reset_f_intr();
                        b=1;
                        tptw &= stm;
                }
                if(b & tptw){
                        tptw &= ~b;
                        switch (b){

// if decided to add some handler beware of set_stm_for...(), 0x7f00 time slots are safe

                                case       1:
#ifdef HDLC_8993
                                              for(p=0;p<card;p++)
                                                onh(p);
#endif
                                              break;
                                case       2: lcds(); /* 10 msec */



                                              break;
                                case       4:
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
#ifndef HDLC_RS232_ON_OPERATOR_SIDE
#ifdef OPERATOR_PC
                                              kbs_m();
#endif
#endif
#endif
                                              break;
                                case       8: trscn(); break;
                                case    0x10:
#ifdef OPERATOR_PC
                                              pc_timer_init();
#endif
                                              break;
                                case    0x20:
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
/* every 8*10=80 msec */
#ifdef OPERATOR_PC
                                              kbs_t();
#else
                                              kbs();
#endif
#endif
                                              break;
                                case    0x40: leds();
                                              break;
                                case    0x80:
#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
                                              alarm_reset();
#endif
#endif

#ifdef OPERATOR_PC
                                              update_time();

#ifdef UPDATE_DATE_TIME
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
                                              ddt_ad_always();
#endif
#endif

#ifdef REPORT_BULK_BILLING
#if( (TON == BULK_BILLING_OPR) || ((defined SECOND_BULK_BILLING_OPR) && (TON == SECOND_BULK_BILLING_OPR)) )
                                              bbf_backup();
#endif
#endif

#endif
                                              break;
                                case    0x100:   /* 10 msec */
#ifdef MY_TCPIP_FOR_OAM
                                                my_tcpip_oam_tx_hndlr();
#endif
#ifdef TCP_INSTALLED
                                                so_tcp_recv_hndlr();
#endif
#ifdef ETHERNET_INSTALLED
                                                pktdrvr_rx_hndlr();
#endif
#ifdef IP_INSTALLED
                                                ip_route_hndlr();
#endif
#ifdef UDP_INSTALLED
                                                udp_recv_hndlr();
#endif
#ifdef LAPB_ON_OPERATOR_SIDE
                                                linklayer_fast_handler();
                                                hwlayer_tx_fast_handler();
#endif
#ifdef IP_INSTALLED
                                                ip_route_hndlr();
                                                iface_outq_hndlr();
#endif
#ifdef ETHERNET_INSTALLED
                                                pktdrvr_tx_hndlr();
#endif
#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef OPR_UPLOAD_WITH_TIMER
                                              upload_handler();
#endif
#endif
#ifdef LAPB_ON_OPERATOR_SIDE
                                              hwlayer_rx_handler();
#endif
#ifdef TCP_ON_CLIENT_SIDE
                                              tcp_tx_client_hndlr();
#endif
#ifdef TCP_ON_SERVER_SIDE
                                              tcp_tx_server_hndlr();
#endif
                                              prndrv();
                                              break;
                                case    0x200:   /* 20 msec */
#ifdef TIMERS_FOR_OPR
                                                timeman();
#endif
#ifdef IP_INSTALLED
                                                ttldec_all_iface();
#endif
#ifdef LAPB_ON_OPERATOR_SIDE
                                              linklayer_handler();
                                              hwlayer_tx_handler();
#endif
                                              break;
                                case    0x400:   /* 20 msec */
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                                              oam_opr_rcv_hndlr();
#endif
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
                                              com_uart_tx_fifo_handler();
#endif
                                              break;
                                case    0x800:   /* 10 msec */
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
                                              com_uart_rx_fifo_handler();
                                              hdlc_rx_hndlr();
#endif
                                              break;
                                case    0x1000:
                                              break;
                                case    0x2000:
                                              break;
                                case    0x4000:
                                              break;
                                case    0x8000: /* 160 msec */
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                                              check_link_handler();
#endif
#ifdef HOT_STANDBY
                                              check_htby_link_handler();
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                                              check_tcp_link_handler();
#endif
#ifdef TCP_DEBUG_ACT
                                              reopen_tcp_err_file();
#endif
                                              break;
                        }
                }
                b<<=1;
        }
        f &= ~SCHLF;
}

void cp(void){
        CPR a;
        BYTE s;
        int b;
        f |= CPF;
//      while( !(f & INTRF) ){
        while( !(f_intr()) ){
            a=fromcpq();
            if(a.stim == 0xff){     /* if queue is empty */
                f &= ~CPF;
                return;
            }

#ifdef TIMERS_FOR_OPR
            if(a.pt.t == TIMER_T){
                TMR *tp;
                tp=&timer[a.pt.i];
                if(tp->tdiv == 0xfffe){ // already stopped, ignore it
                        rlstimer(a.pt.i);
                        return;
                }
                switch(tp->it.t){
#ifdef IP_INSTALLED
                  case IP_REASSEMBLE_SDL_T :
                    ip_timeout(a.pt.i);
                    rlstimer(a.pt.i);
                    return;
#endif
#ifdef TCP_INSTALLED
                  case TCP_SDL_T           :
                    tcp_timeout(a.pt.i);
                    rlstimer(a.pt.i);
                    return;
#endif
#ifdef ARP_INSTALLED
                  case ARP_SDL_T           :
                    arp_timeout(a.pt.i);
                    rlstimer(a.pt.i);
                    return;
#endif
                  default                       :
                    rlstimer(a.pt.i);
                    return;
                }
            }
#endif
                b=a.pt.i;
#ifdef OPR_OPR_TALK
            if(a.stim == SRC_CNNCTED)
                s=ocr.state;
            else
#endif
            {
                switch(a.pt.t){
                  case XST_T:     s=xst[b].s; break;
#if(NO_CO != 0)
                  case  CO_T:     s=ctst[b].s; break;
#endif


#ifndef NO_TIE_INIT
                  case TIE_T:     s=ttst[b].s; break;
#endif

                  case OPR_T:     s=ocr.state; break;

#ifdef EM_INSTALLED
                  case  EM_T:
#ifndef OPR_EM_CONNECTION
                    s=IDLE;
        /* all STD links from operator point of view are in IDLE state */
#else
                    s=emtst[b].s;
#endif
                    break;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                  case  CAS3_T:
//#ifndef OPR_EM_CONNECTION
//                  s=IDLE;
//        /* all STD links from operator point of view are in IDLE state */
//#else
                    s=dtcst[b].s;
//#endif
                    break;
#endif

                  default:        s=0xff; break;
                }
                if(s == IN_OCR)
                    s=ocr.state;
            }
            if( (s >= OPRSIDE_NO_STATES) || (a.stim >= OPRSIDE_NO_STIMS) ){

#ifdef DEBUG_ACTIVE
                str2lcd(" ERROR ");
                int2lcd(s);tolcdq(' ',TO_LCD_DD);int2lcd(a.stim);
#endif

#ifdef OPERATOR_PC
#ifdef LOG_FILE
                sdt=*get_dt(&sdt);
                fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  CODE  %x %x %x %x\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se,s,a.stim,a.pt.i,a.pt.t);
#else
                fprintf(fer,"CODE  %x %x %x %x\n",s,a.stim,a.pt.i,a.pt.t);
#endif
#endif
            }
            else{
#ifdef OPERATOR_PC
                if(rtn_idx[s][a.stim] == (FPCP)errorcp){
#ifdef LOG_FILE
                    sdt=*get_dt(&sdt);
                    fprintf(fer,"%4x-%02x-%02x %02x:%02x:%02x  CODE CP %x %x %x %x\n",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se,s,a.stim,a.pt.i,a.pt.t);
#else
                    fprintf(fer,"CODE CP %x %x %x %x\n",s,a.stim,a.pt.i,a.pt.t);
#endif
#ifdef DEBUG_ACTIVE
                    str2lcd("ERRORCP ");
                    int2lcd(s);
                    tolcdq(' ',TO_LCD_DD);
                    int2lcd(a.stim);
#endif
                }
                rtn_idx[s][a.stim](a.pt);
#else
                rtnexe(rtn_idx[s][a.stim],a.pt);
#endif
            }
        }
        f &= ~CPF;
}

#ifndef OPERATOR_PC

void rtnexe(BYTE rn,INDEX_TYPE pt){  /* execute routine number rn */

#ifdef DEBUG_ACTIVE
        str2lcd("rtnx:");hex2lcd(rn);
#endif

        switch(rn){
                case     0:     rtn0();break;
                case     1:     rtn1(pt); break;
                case     2:     rtn2(pt); break;
/*              case     3:     rtn3(pt); break;
                case     4:     rtn4(pt); break;
unused routines */
                case     5:     rtn5(pt); break;
                case     6:     rtn6(pt); break;
                case     7:     rtn7(pt); break;
                case     8:     rtn8(pt); break;
/*              case     9:     rtn9(pt); break;
unused routine */
                case    10:     rtn10(pt); break;
                case    11:     rtn11(pt); break;
                case    12:     rtn12(pt); break;
                case    13:     rtn13(pt); break;
                case    14:     rtn14(pt); break;
                case    15:     rtn15(pt); break;
                case    16:     rtn16(pt); break;
                case    17:     rtn17(pt); break;
                case    18:     rtn18(pt); break;
                case    19:     rtn19(pt); break;
                case    20:     rtn20(pt); break;
                case    21:     rtn21(pt); break;
                case    22:     rtn22(pt); break;
                case    23:     rtn23(pt); break;
                case    24:     rtn24(pt); break;
                case    25:     rtn25(pt); break;
                case    26:     rtn26(pt); break;
                case    27:     rtn27(pt); break;
                case    28:     rtn28(pt); break;
                case    29:     rtn29(pt); break;
                case    30:     rtn30(pt); break;
                case    31:     rtn31(pt); break;
                case    32:     rtn32(pt); break;
                case    33:     rtn33(pt); break;
                case    34:     rtn34(pt); break;
                case    35:     rtn35(pt); break;
                case    36:     rtn36(pt); break;
                case    37:     rtn37(pt); break;
                case    38:     rtn38(pt); break;
                case    39:     rtn39(pt); break;
                case    40:     rtn40(pt); break;
                case    41:     rtn41(pt); break;
                case    42:     rtn42(pt); break;
                case    43:     rtn43(pt); break;
                case    44:     rtn44(pt); break;
                case    45:     rtn45(pt); break;
                case    46:     rtn46(pt); break;
                case    47:     rtn47(pt); break;
                case    48:     rtn48(pt); break;
                case    49:     rtn49(pt); break;
                case    50:     rtn50(pt); break;
                case    51:     rtn51(pt); break;
                case    52:     rtn52(pt); break;
                case    53:     rtn53(pt); break;
                case    54:     rtn54(pt); break;
                case    55:     rtn55(pt); break;
                case    56:     rtn56(pt); break;
                case    57:     rtn57(pt); break;
                case    58:     rtn58(pt); break;
                case    59:     rtn59(pt); break;
                case    60:     rtn60(pt); break;
                case    61:     rtn61(pt); break;
                case    62:     rtn62(pt); break;
                case    63:     rtn63(pt); break;
                case    64:     rtn64(pt); break;
                case    65:     rtn65(pt); break;
                case    66:     rtn66(pt); break;
                case    67:     rtn67(pt); break;
                case    68:     rtn68(pt); break;
                case    69:     rtn69(pt); break;
                case    70:     rtn70(pt); break;
                case    71:     rtn71(pt); break;
                case    72:     rtn72(pt); break;
                case    73:     rtn73(pt); break;
                case    74:     rtn74(pt); break;
                case    75:     rtn75(pt); break;
                case    76:     rtn76(pt); break;
                case    77:     rtn77(pt); break;
                case    78:     rtn78(pt); break;
                case    79:     rtn79(pt); break;
                case    80:     rtn80(pt); break;
                case    81:     rtn81(pt); break;
                case    82:     rtn82(pt); break;
                case    83:     rtn83(pt); break;
                case    84:     rtn84(pt); break;
                case    85:     rtn85(pt); break;
                case    86:     rtn86(pt); break;
                case    87:     rtn87(pt); break;
                case    88:     rtn88(pt); break;
                case    89:     rtn89(pt); break;
                case    90:     rtn90(pt); break;
                case    91:     rtn91(pt); break;
                case    92:     rtn92(pt); break;

#ifdef OPR_MALICIOUS_DTCT_ACT
                case    93:     rtn93(pt); break;
#endif
                case    94:     rtn94(pt); break;
                case    95:     rtn95(pt); break;
                case    96:     rtn96(); break;
/*              case    97:     rtn97(pt); break;
*/
/*              case    98:     rtn98(pt); break;
*/
/*              case    99:     rtn99(pt); break;
*/
                case   100:     rtn100(pt); break;
                case   101:     rtn101(pt); break;
                case   102:     rtn102(pt); break;
                case   103:     rtn103(pt); break;
                case   104:     rtn104(pt); break;
                case   105:     rtn105(pt); break;

#ifdef DISPLAY_STATUS_ACT
                case   106:     rtn106(pt); break;
                case   107:     rtn107(pt); break;
#endif
                case   108:     rtn108(pt); break;
                case   109:     rtn109(pt); break;
                case   110:     rtn110(pt); break;
                case   111:     rtn111(pt); break;
                case   124:     rtn124(pt); break;

#ifdef OPR_EM_CONNECTION
                case    132:    rtn132(pt); break;
#endif
#ifdef EM_CONT_SEP_OPR_Q
                case    139:    rtn139(pt); break;
#endif
                default:        errorcp();break;
        }
}

#endif

int tocpq(INDEX_TYPE pt,int stim){
        if( ( (cpeq+1) & (CP_QUE_LEN-1) ) == cpfq)
                return ERR_CODE;  /* queue is full */
        cpeq= (cpeq + 1) & (CP_QUE_LEN-1);
        cpq[cpeq].pt=pt;
        cpq[cpeq].stim=stim;

#ifdef DEBUG_ACTIVE
        str2lcd("tocpq:");int2lcd(pt.i);tolcdq(' ',TO_LCD_DD);
        hex2lcd(pt.t);tolcdq(' ',TO_LCD_DD);int2lcd(stim);
#endif

        return OK;
}

CPR     fromcpq(){
        CPR     a;
        if(cpfq == cpeq){
                memset(&a,0,sizeof(a));
                a.stim=0xff; /* If queue is empty */
        }
        else{
                cpfq= (cpfq + 1) & (CP_QUE_LEN-1);
                a=cpq[cpfq];
        }
        return a;
}

int tomaq(INDEX_TYPE j,unsigned int p,BYTE n){
        if( ( (maeq+1) & (MA_QUE_LEN-1) ) == mafq){
                fprintf(fer,"\nMA queue full");
                return ERR_CODE;
                  /* queue is full */
        }
        maeq= (maeq + 1) & (MA_QUE_LEN-1);
        maq[maeq].wt=j;
        maq[maeq].parm=p;
#ifdef HOT_STANDBY
        // because of its usage in csp(),billing and malicious... packets in omap(),
        // "sys" field can not be used.
        maq[maeq].sys=0;
        maq[maeq].htby=n;
#else
        maq[maeq].sys=n;
#endif
        return OK;
}

#ifdef OPERATOR_PC
#if ( (PARAMETER_DOWN_UP_LOAD == DEFINE_VALUE) || (DETAIL_BILLING == DEFINE_VALUE) )

OMAR frommaq(){
        OMAR a;
        if(mafq==maeq){
                memset(&a,0,sizeof(a));
                a.wt.t=NULL_T;  /* If queue is empty */
        }
        else{
                mafq= (mafq + 1) & (MA_QUE_LEN-1);
                a=maq[mafq];
        }
        return a;
}
#endif
#endif

int ffiqr(){    /* find free internal queue record */
        BYTE a;
        if( (a=firf) >= IQR_NO)         return ERR_CODE;
        firf=iqr[a].rp;
        if(firf == 0xff)  /* if last record */
                fire=0xff;
        else
                iqr[firf].lp=0xff;
        return a;
}

void riqr(BYTE r){       /* release internal queue record */
        iqr[r].rp=0xff;
        iqr[r].lp=fire;
        if(fire < IQR_NO)
                iqr[fire].rp=r;
        else
                firf=r;
        fire=r;
}

int ffrxqr(){  /* find free recall or external queue record */
        BYTE a;
        if( (a=frxf) >= RXQR_NO )       return ERR_CODE;
        frxf=rxqr[a].rp;
        if(frxf == 0xff)     /* if last record */
                frxe=0xff;
        else
                rxqr[frxf].lp=0xff;
        return a;
}

void rrxqr(BYTE r){  /* release recall or external queue record */
        rxqr[r].rp=0xff;
        rxqr[r].lp=frxe;
        if(frxe < RXQR_NO)
                rxqr[frxe].rp=r;
        else
                frxf=r;
        frxe=r;
}

void rfiq(BYTE r){   /* remove from internal queue */
        if(r == pqi){
            pqi=iqr[r].rp;
            if(pqi >= IQR_NO)
                pqi=fqi;
            if(pqi == r)
                pqi=0xff;
        }
        if(r == fqi){   /* if 1st record */
                fqi=iqr[r].rp;
                if(fqi < IQR_NO)
                        iqr[fqi].lp=0xff;
                else
                        eqi=0xff;
        }
        else
                if(r == eqi){   /* if last record */
                        eqi=iqr[r].lp;
                        if(eqi < IQR_NO)
                                iqr[eqi].rp=0xff;
                        else
                                fqi=0xff;
                }
                else{  /* middle record */
                        iqr[ iqr[r].lp ].rp=iqr[r].rp;
                        iqr[ iqr[r].rp ].lp=iqr[r].lp;
                }
        iqs--; /* decrement internal queue size */
        riqr(r);
        if( (fqi==0xff) || (eqi==0xff) ){
                trf &= ~INTRNL_QUE_ACT;
                if( (trf & QUE_ACT) == RESET){   /* disable tone ringer */
#ifdef OPERATOR_PC
                        DIS_SPKR;
#else
                        XBYTE[DSP_CR]=0x62;
#endif
/*                      kt_outport8(DSP_CR,0x72);
*/
                        trs=0xff;
                }
        }
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\nrfiq-fqi:%02x-eqi:%02x-trf:%02x-trs:%02x-iqs:%02x\r\n",fqi,eqi,trf,trs,iqs);
#endif
}

void rfxq(BYTE r){  /* remove from external queue */
        if(r == pqx){
            pqx=rxqr[r].rp;
            if(pqx >= RXQR_NO)
                pqx=fqx;
            if(pqx == r)
                pqx=0xff;
        }
        if(r == fqx){   /* if 1st record */
                fqx=rxqr[r].rp;
                if(fqx < RXQR_NO)
                        rxqr[fqx].lp=0xff;
                else
                        eqx=0xff;
        }
        else
                if(r == eqx){   /* if last record */
                        eqx=rxqr[r].lp;
                        if(eqx < RXQR_NO)
                                rxqr[eqx].rp=0xff;
                        else
                                fqx=0xff;
                }
                else{   /* middle record */
                        rxqr[ rxqr[r].lp ].rp=rxqr[r].rp;
                        rxqr[ rxqr[r].rp ].lp=rxqr[r].lp;
                }
        xqs--; /* decrement external queue size */
        rrxqr(r);
        if( (fqx==0xff) || (eqx==0xff) ){
                trf &= ~EXTRNL_QUE_ACT;
                if( (trf & QUE_ACT) == RESET){   /* disable tone ringer */
#ifdef OPERATOR_PC
                        DIS_SPKR;
#else
                        XBYTE[DSP_CR]=0x62;
#endif
/*                      kt_outport8(DSP_CR,0x72);
*/
                        trs=0xff;
                }
        }
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\nrfxq-fqx:%02x-eqx:%02x-trf:%02x-trs:%02x-xqs:%02x-pqx:%02x\r\n",fqx,eqx,trf,trs,xqs,pqx);
#endif
}

#ifdef EM_CONT_SEP_OPR_Q
void rfemq(BYTE r){  /* remove from e&m queue */
        if(r == fqem){   /* if 1st record */
                fqem=rxqr[r].rp;
                if(fqem < RXQR_NO)
                        rxqr[fqem].lp=0xff;
                else
                        eqem=0xff;
        }
        else
                if(r == eqem){   /* if last record */
                        eqem=rxqr[r].lp;
                        if(eqem < RXQR_NO)
                                rxqr[eqem].rp=0xff;
                        else
                                fqem=0xff;
                }
                else{   /* middle record */
                        rxqr[ rxqr[r].lp ].rp=rxqr[r].rp;
                        rxqr[ rxqr[r].rp ].lp=rxqr[r].lp;
                }
        emqs--; /* decrement external queue size */
        rrxqr(r);
        if( (fqem==0xff) || (eqem==0xff) ){
                trf &= ~EM_QUE_ACT;
                if( (trf & QUE_ACT) == RESET){   /* disable tone ringer */
#ifdef OPERATOR_PC
                        DIS_SPKR;
#else
                        XBYTE[DSP_CR]=0x62;
#endif
/*                      kt_outport8(DSP_CR,0x72);
*/
                        trs=0xff;
                }
        }
}
#endif

void rfrq(BYTE r){   /* remove from recall queue */
        if(r == fqr){    /* if 1st record */
                fqr=rxqr[r].rp;
                if(fqr < RXQR_NO)
                        rxqr[fqr].lp=0xff;
                else
                        eqr=0xff;
        }
        else
                if(r == eqr){   /* if last record */
                        eqr=rxqr[r].lp;
                        if(eqr < RXQR_NO)
                                rxqr[eqr].rp=0xff;
                        else
                                fqr=0xff;
                }
                else{   /* middle record */
                        rxqr[ rxqr[r].lp ].rp=rxqr[r].rp;
                        rxqr[ rxqr[r].rp ].lp=rxqr[r].lp;
                }
        rqs--;  /* decrement recall queue size */
        rrxqr(r);
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\nrfrq-fqr:%02x-eqr:%02x-trf:%02x-trs:%02x\r\n",fqr,eqr,trf,trs);
#endif
        if( (fqr==0xff) || (eqr==0xff) ){
                trf &= ~RECALL_QUE_ACT;
                if( (trf & QUE_ACT) == RESET){  /* disable tone ringer */
#ifdef OPERATOR_PC
                        DIS_SPKR;
#else
                        XBYTE[DSP_CR]=0x62;
#endif
/*                      kt_outport8(DSP_CR,0x72);
*/
                        trs=0xff;
                }
        }
}

int fromiq(){  /* from internal queue */
        BYTE a;
        if(fqi >= IQR_NO)
                return ERR_CODE;
        a=fqi;

        if(a == pqi){
            pqi=iqr[a].rp;
            if(pqi >= IQR_NO)
                pqi=fqi;
            if(pqi == a)
                pqi=0xff;
        }

        fqi=iqr[a].rp;
        if(fqi >= IQR_NO)
                eqi=0xff;
        else
                iqr[fqi].lp=0xff;
        if( (fqi==0xff) || (eqi==0xff) ){
                trf &= ~INTRNL_QUE_ACT;
                if( (trf & QUE_ACT) == RESET){   /* disable tone ringer */
#ifdef OPERATOR_PC
                        DIS_SPKR;
#else
                        XBYTE[DSP_CR]=0x62;
#endif
/*                      kt_outport8(DSP_CR,0x72);
*/
                        trs=0xff;
                }
        }
        iqs--; /* decrement internal queue size */
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\nfromiq-fqi:%02x-eqi:%02x-trf:%02x-trs:%02x-iqs:%02x\r\n",fqi,eqi,trf,trs,iqs);
#endif
        return a;
}

int fromxq(){ /* from external queue */
        BYTE a;
        if(fqx >= RXQR_NO)
                return ERR_CODE;
        a=fqx;

        if(a == pqx){
            pqx=rxqr[a].rp;
            if(pqx >= RXQR_NO)
                pqx=fqx;
            if(pqx == a)
                pqx=0xff;
        }

        fqx=rxqr[a].rp;
        if(fqx >= RXQR_NO)
                eqx=0xff;
        else
                rxqr[fqx].lp=0xff;
        if( (fqx==0xff) || (eqx==0xff) ){
                trf &= ~EXTRNL_QUE_ACT;
                if( (trf & QUE_ACT) == RESET){  /* disable tone ringer */
#ifdef OPERATOR_PC
                        DIS_SPKR;
#else
                        XBYTE[DSP_CR]=0x62;
#endif
/*                      kt_outport8(DSP_CR,0x72);
*/
                        trs=0xff;
                }
        }
        xqs--; /* decrement external queue size */
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\nfromxq-fqx:%02x-eqx:%02x-trf:%02x-trs:%02x-xqs:%02x-pqx:%02x\r\n",fqx,eqx,trf,trs,xqs,pqx);
#endif
        return a;
}

#ifdef EM_CONT_SEP_OPR_Q
int fromemq(void){ /* from e&m queue */
        BYTE a;
        if(fqem >= RXQR_NO)
                return ERR_CODE;
        a=fqem;
        fqem=rxqr[a].rp;
        if(fqem >= RXQR_NO)
                eqem=0xff;
        else
                rxqr[fqem].lp=0xff;
        if( (fqem==0xff) || (eqem==0xff) ){
                trf &= ~EM_QUE_ACT;
                if( (trf & QUE_ACT) == RESET){  /* disable tone ringer */
#ifdef OPERATOR_PC
                        DIS_SPKR;
#else
                        XBYTE[DSP_CR]=0x62;
#endif
/*                      kt_outport8(DSP_CR,0x72);
*/
                        trs=0xff;
                }
        }
        emqs--; /* decrement external queue size */
        return a;
}
#endif

int fromrq(){  /* from recall queue */
        BYTE a;
        if(fqr >= RXQR_NO)
                return ERR_CODE;
        a=fqr;
        fqr=rxqr[a].rp;
        if(fqr >= RXQR_NO)
                eqr=0xff;
        else
                rxqr[fqx].lp=0xff;
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\nfromrq-fqr:%02x-eqr:%02x-trf:%02x-trs:%02x\r\n",fqr,eqr,trf,trs);
#endif
        if( (fqr==0xff) || (eqr==0xff) ){
                trf &= ~RECALL_QUE_ACT;
                if( (trf & QUE_ACT) == RESET){  /* disable tone ringer */
#ifdef OPERATOR_PC
                        DIS_SPKR;
#else
                        XBYTE[DSP_CR]=0x62;
#endif
/*                      kt_outport8(DSP_CR,0x72);
*/
                        trs=0xff;
                }
        }
        rqs--;  /* decrement recall queue size */
        return a;
}

void rtpns(INDEX_TYPE a){  /* release this party notify system */
        int b;
        if(a.t == NULL_T)       return;
        b=a.i;
        switch(a.t){
                case XST_T:
#if (defined LIM)
#if  (defined OPR_VOICE_THRU_LIMB)
#if(NO_EXTENSION_C != 0)
                                if(b >= (NO_EXTENSION_A+NO_EXTENSION_B))
                                        pcpmftx2(OPR_REJECT,b-(NO_EXTENSION_A+NO_EXTENSION_B),XST_T);
                                else
#endif
                                if(b >= NO_EXTENSION_A)
                                        pcpmftx1(OPR_REJECT,b-NO_EXTENSION_A,XST_T);
                                else
#else
                                if(b >= NO_EXTENSION_A)
                                        pcpmftx(OPR_REJECT,TON,OPR_T);
                                else
#endif
#endif
                                pcpmftx(OPR_REJECT,b,XST_T);
                                xst[b].s=IDLE;
#ifdef OPR_PRI_CONNECTION
                                if(xst[b].dgt != NULL_POINTER){
                                        kt_g_free(xst[b].dgt);
                                        xst[b].dgt=NULL_POINTER;
                                }
                                xst[b].no_dgt=0xff;
                                xst[b].userside_pri=RESET;
                                xst[b].og=RESET;
#endif
                                break;
#if(NO_CO != 0)
                case  CO_T:     ctst[b].s=IN_RCL_QU;
                                ctst[b].i=torq(a);
                                pcpmftx(RECALL,b,CO_T);
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     ttst[b].s=IN_RCL_QU;
                                ttst[b].i=torq(a);
                                pcpmftx(RECALL,b,TIE_T);
                                break;
#endif


#ifdef OPR_EM_CONNECTION
                case EM_T:      emtst[b].s=IDLE;
                                pcpmftx(TRNK_RLS,b,EM_T);
                                break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:    dtcst[b].s=IDLE;
                                pcpmftx(TRNK_RLS,b,CAS3_T);
                                break;
#endif
#ifdef OPR_OPR_TALK
                case OPR_T:     oprt[b].s=IDLE;
                                pcpmftx(OPR_REJECT,b,OPR_T);
                                break;
#endif

                default:        break;
        }
}

void rtp(INDEX_TYPE a){ /* release this party */
        int b;
        if(a.t == NULL_T)       return;
        b=a.i;
        switch(a.t){
                case XST_T:     xst[b].s=IDLE;
#ifdef OPR_PRI_CONNECTION
                                if(xst[b].dgt != NULL_POINTER){
                                        kt_g_free(xst[b].dgt);
                                        xst[b].dgt=NULL_POINTER;
                                }
                                xst[b].no_dgt=0xff;
                                xst[b].userside_pri=RESET;
                                xst[b].og=RESET;
#endif
                                break;
#if(NO_CO != 0)
                case  CO_T:     ctst[b].s=IDLE;
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     ttst[b].s=IDLE;
                                break;
#endif


#ifdef OPR_EM_CONNECTION
                case EM_T:      emtst[b].s=IDLE;
                                break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:    dtcst[b].s=IDLE;
                                break;
#endif
#ifdef OPR_OPR_TALK
                case OPR_T:     oprt[b].s=IDLE;
                                break;
#endif
                default:        break;
        }
}

int tolcdq(BYTE d,BYTE w){      /* to LCD queue data d,work w */
#ifdef OPERATOR_SIDE_DEBUG_ACT
        if(( w == CLEAR_LCD) && (d == 0xff)){
                putc(CR, opr_gonz_fp);
                putc(LF, opr_gonz_fp);
        }
        else{
                putc(d, opr_gonz_fp);
        }
#endif
        if ((d == CR) || (d == LF))
                return OK;
        if(((elcdq+1) & (LCD_QUE_LEN-1)) == flcdq)
                return ERR_CODE;    /* queue is full */
        elcdq= (elcdq + 1) & (LCD_QUE_LEN-1);
        lcdq[elcdq].info=d;
        lcdq[elcdq].op=w;
        return OK;
}

int fromlcdq(){
        if(flcdq == elcdq)
                return ERR_CODE;      /* if queue is empty */
        else{
                flcdq =(flcdq + 1) & (LCD_QUE_LEN-1);
                return flcdq;
        }
}

void flush_lcdq(){
        while(flcdq != elcdq)
                lcds();
}

void lcds(){
        LCD_RCRD a;

#ifdef OPERATOR_PC
        if(lcdsts() & 0x80)  /* if LCD busy */
#else
        if(XBYTE[LCD_STATUS] & 0x80)  /* if LCD busy */
#endif
                return;

#ifdef TIME_DATE_DISPLAY

        if(lcdf){  /* if save or restore in progress */
                switch(lcdf){
                        case SLCD_1L: if(csra == (END_ADDR_1L+1 )){
                                        lcdcmd(SET_DD_RAM_ADDR | START_ADDR_2L);
                                        lcdf &= ~SLCD_1L;
                                        lcdf |= SLCD_2L;
                                      }
                                      else
#ifdef OPERATOR_PC
                                        putlcd(sb[csra++]);
#else
                                        XBYTE[LCD_DATA]=sb[csra++];
#endif
                                      break;
                        case SLCD_2L: if(csra >= TOTAL_LCD_CHAR){
                                        lcdf=0;
                                        csra=0xff;
                                      }
                                      else
#ifdef OPERATOR_PC
                                        putlcd(sb[csra++]);
#else
                                        XBYTE[LCD_DATA]=sb[csra++];
#endif
                                      break;
                        case RLCD_1L: if(csra == (END_ADDR_1L+1) ){
                                        lcdcmd(SET_DD_RAM_ADDR | START_ADDR_2L);
                                        lcdf &= ~RLCD_1L;
                                        lcdf |= RLCD_2L;
                                      }
                                      else
#ifdef OPERATOR_PC
/*                                      sb[csra++]=getlcd()*/;
#else
                                        sb[csra++]=XBYTE[LCD_DATA];
#endif
                                      break;
                        case RLCD_2L: if(csra >= TOTAL_LCD_CHAR){
                                        lcdf=0;
                                        csra=0xff;
                                      }
                                      else
#ifdef OPERATOR_PC
/*                                      sb[csra++]=getlcd()*/;
#else
                                        sb[csra++]=XBYTE[LCD_DATA];
#endif
                                      break;
                        default: lcdf=0; break;
                }
                return;
        }
#endif
/* endif for TIME_DATE_DISPLAY */

        if(fromlcdq() == ERR_CODE)        return;
        a=lcdq[flcdq];
        switch(a.op){

#ifdef TIME_DATE_DISPLAY
                case SAVE_DISPL:
                        csra=0;
                        lcdf=SLCD_1L;
#ifdef OPERATOR_PC
                        lcdcmd(SET_DD_RAM_ADDR | START_ADDR_1L);
#else
                        XBYTE[LCD_CMD]=SET_DD_RAM_ADDR | START_ADDR_1L;
#endif
                        break;
                case RSTR_DISPL:
                        csra=0;
                        lcdf=RLCD_1L;
#ifdef OPERATOR_PC
                        lcdcmd(SET_DD_RAM_ADDR | START_ADDR_1L);
#else
                        XBYTE[LCD_CMD]=SET_DD_RAM_ADDR | START_ADDR_1L;
#endif
                        break;
#endif
/* endif TIME_DATE_DISPLAY */
                case TO_LCD_IR:
#ifdef OPERATOR_PC
                        lcdcmd(a.info);
#else
                        XBYTE[LCD_CMD]=a.info;
#endif
                        if(a.info==0x01){
                              clc=0;
                              lf=0;
                        }
                        break;
                case TO_LCD_DD:
                        if( (lf==0) && (clc>=NO_1L_CH) ){
#ifdef OPERATOR_PC
                                lcdcmd(SET_DD_RAM_ADDR | START_ADDR_2L);
#else
                                XBYTE[LCD_CMD]=SET_DD_RAM_ADDR | START_ADDR_2L;
#endif
                                lf=1;
                                clc=16;
                        }
                        else
                                if(clc>=TOTAL_LCD_CHAR){
#ifdef OPERATOR_PC
                                        lcdcmd(SET_DD_RAM_ADDR | START_ADDR_1L);
#else
                                        XBYTE[LCD_CMD]=SET_DD_RAM_ADDR | START_ADDR_1L;
#endif
                                        lf=0;
                                        clc=0;
                                }
#ifdef OPERATOR_PC
                        while(lcdsts() & 0x80);
                        /* if LCD busy */
                        putlcd(a.info);
#else
                        while(XBYTE[LCD_STATUS] & 0x80);
                        /* if LCD busy */
                        XBYTE[LCD_DATA]=a.info;
#endif
                        ++clc;
                        break;
#ifdef OPERATOR_PC
                case CLEAR_LCD:
                        init_lcd();
                        break;
#endif
                default: break;
        }
}

void str2lcd(char *s){
/*
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\n%s",s);
#endif
*/
        if(*s != '\0')
                while(*s != '\0'){
                        tolcdq(*s,TO_LCD_DD);
                        s++;
                }
}

void int2lcd(long a){
        unsigned long l;
        BYTE no_dgt;
        int b;
        for(l=a,no_dgt=0;l != 0L; l>>=4,no_dgt++);
        b=(int)(a>>16);
        if (no_dgt >= 8) tolcdq(h2c((b>>12)&0x0f),TO_LCD_DD);
        if (no_dgt >= 7) tolcdq(h2c((b>> 8)&0x0f),TO_LCD_DD);
        if (no_dgt >= 6) tolcdq(h2c((b>> 4)&0x0f),TO_LCD_DD);
        if (no_dgt >= 5) tolcdq(h2c( b     &0x0f),TO_LCD_DD);
        b=(int)(a&0xffff);
        if (no_dgt >= 4) tolcdq(h2c((b>>12)&0x0f),TO_LCD_DD);
        if (no_dgt >= 3) tolcdq(h2c((b>> 8)&0x0f),TO_LCD_DD);
        if (no_dgt >= 2) tolcdq(h2c((b>> 4)&0x0f),TO_LCD_DD);
        if (no_dgt >= 1) tolcdq(h2c( b     &0x0f),TO_LCD_DD);
}
void long2lcd(long a){

        int b;
        b=(int)(a>>16);
        tolcdq(h2c((b>>12)&0x0f),TO_LCD_DD);
        tolcdq(h2c((b>> 8)&0x0f),TO_LCD_DD);
        tolcdq(h2c((b>> 4)&0x0f),TO_LCD_DD);
        tolcdq(h2c( b     &0x0f),TO_LCD_DD);
        b=(int)(a&0xffff);
        tolcdq(h2c((b>>12)&0x0f),TO_LCD_DD);
        tolcdq(h2c((b>> 8)&0x0f),TO_LCD_DD);
        tolcdq(h2c((b>> 4)&0x0f),TO_LCD_DD);
        tolcdq(h2c( b     &0x0f),TO_LCD_DD);
}

void hex2lcd(BYTE a){
        tolcdq(h2c((a>>4) & 0x0f),TO_LCD_DD);
        tolcdq(h2c( a     & 0x0f),TO_LCD_DD);
}

char h2c(BYTE b){
        if(b <= 9)
                b += 0x30;
        else
                b += 0x57;
        return b;
}

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void kbs_m(){     /* key board scan by mouse */
        INDEX_TYPE a;
        BYTE kh;        /* key hit */
        kh=fkp();
        if(f & KBRF)
            if(kh == 0xff){
                if(f & KBHF){
                    f &= ~KBHF;
                    lkp=0xff;
                }
                return;
            }
            else    /* kh != 0xff */
            if(f & KBHF){
                if(kh == lkp){
                    a.t=OPR_T;
                    a.i=TON;
                    tocpq(a,kctt[kh]);
#ifdef DEBUG_ACTIVE
                    str2lcd(" kbs_m:");
                    hex2lcd(kctt[kh]);
#endif
                    if(kctt[kh] == NO_OPERATION){
                        return;
                    }
                    if(kctt[kh] == 0xff){
                        fcloseall();
#ifdef OPR_EXIT_PASSWORD
                        if(vox() == 1)
#endif
                        {
#ifdef REPORT_BULK_BILLING
#if( (TON == BULK_BILLING_OPR) || ((defined SECOND_BULK_BILLING_OPR) && (TON == SECOND_BULK_BILLING_OPR)) )
                            force_bbf_backup();
#endif
#endif
                            quit(0);
                        }
                    }
                    f &= ~KBRF;
                }
                else
                    lkp=kh;
                return;
            }
            else{   /* KBHF reset */
                lkp=kh;
                f |= KBHF;
                return;
            }
        else{   /* KBRF reset */
            if(kh == 0xff){
                f |= KBRF;
                f &= ~KBHF;
                lkp=0xff;
            }
            return;
        }
}
#endif
#endif


#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void kbs_t(){     /* key board scan by typing */
        INDEX_TYPE a;
        BYTE kh;        /* key hit */

        kh=key_fkp();
        if(kh == 0xff)
                return;

#ifdef DEBUG_ACTIVE
        str2lcd(" kbs_t:");
        hex2lcd(kctt[kh]);
#endif

        if(kctt[kh] == NO_OPERATION){
                return;
        }
        if(kctt[kh] == 0xff){
            fcloseall();
#ifdef OPR_EXIT_PASSWORD
            if(vox() == 1)
#endif
            {
#ifdef REPORT_BULK_BILLING
#if( (TON == BULK_BILLING_OPR) || ((defined SECOND_BULK_BILLING_OPR) && (TON == SECOND_BULK_BILLING_OPR)) )
                force_bbf_backup();
#endif
#endif
                quit(0);
            }
        }

        a.t=OPR_T;
        a.i=TON;
#ifdef DISPLAY_EM_STATUS
        if(dtsf){
                rtn105a(a);
//              tocpq(a,ATT_RLS);
        }
        else
#endif
#ifdef DISPLAY_CAS3_STATUS
        if(dcsf){
                if(kctt[kh] == DISP_CAS3_STATUS){
                        current_cas3_page++;
                        if(current_cas3_page == ((NO_CAS3_DISPLAY-1)/NO_CAS3_DISPLAYED_PER_PAGE) + 1)
                                current_cas3_page=0;
                        clear_screen();
                        disp_cas3_no();
/*
#ifdef HOT_STANDBY
                        pcpmftx_both_sys(CAS3_STATUS_REQ_ACT,current_cas3_page,OPR_T);
#else
                        pcpmftx(CAS3_STATUS_REQ_ACT,current_cas3_page,OPR_T);
#endif
*/
                }
                else
                if(kctt[kh] == RLS){
#ifdef HOT_STANDBY
                        pcpmftx_both_sys(CAS3_STATUS_REQ_ACT,current_cas3_page,OPR_T);
#else
                        pcpmftx(CAS3_STATUS_REQ_ACT,current_cas3_page,OPR_T);
#endif
                }
                else
                        rtn105c(a);
//              tocpq(a,ATT_RLS);
        }
        else
#endif
#ifdef DISPLAY_CAS1_STATUS
        if(d1sf){
                rtn105d(a);
//              tocpq(a,ATT_RLS);
        }
        else
#endif
#ifdef SYSTEM_MANUAL_TEST
        if(mtf)
                tocpq(a,kctt[kh]);
        else
#endif
#ifdef DISPLAY_BILLING
        if(bill_display_flag)
                rtn105e(a);
        else
#endif
#ifdef DISPLAY_ALARM
        if(alarm_display_flag)
                rtn105f(a);
        else
#endif
#ifdef DISPLAY_STATUS_ACT
        if(dxsf){
                if(kctt[kh] == DISP_STATUS){
                        current_page++;
                        if(current_page == ((NO_TOTAL_EXTENSION-1)/1000) + 1)
                                current_page=0;
                        clear_large_screen();
                        kt_print_char(0,0, (unsigned int) ((((find_subscriber_no(current_page*1000)%10000)/1000)+'0')), MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_GREEN));
                }
                else
                if(kctt[kh] == RLS){
#ifdef HOT_STANDBY
                        pcpmftx_both_sys(X_STATUS_REQ_ACT,current_page,OPR_T);
#else
                        pcpmftx(X_STATUS_REQ_ACT,current_page,OPR_T);
#endif
#ifdef LIM
                        pcpmftx1(X_STATUS_REQ_ACT,current_page,OPR_T);
#if(NO_EXTENSION_C != 0)
                        pcpmftx2(X_STATUS_REQ_ACT,current_page,OPR_T);
#endif
#endif
                }
                else
                        tocpq(a,RLS);
        }
        else
#endif
                tocpq(a,kctt[kh]);

}
#endif
#endif


#ifndef OPERATOR_PC
#ifndef PC_KEYBOARD
void kbs(){     /* key board scan */
        INDEX_TYPE a;
        BYTE kh;        /* key hit */
        kh=fkp();
        if(f & KBRF)
                if(kh == 0xff){
                        if(f & KBHF){
                                f &= ~KBHF;
                                lkp=NULL;
                        }
                        return;
                }
                else    /* kh != NULL */
                        if(f & KBHF){
                                if(kh == lkp){
                                        a.t=OPR_T;
                                        a.i=TON;
                                        tocpq(a,kctt[kh]);
#ifdef DEBUG_ACTIVE
                                        str2lcd("kbs:");
                                        hex2lcd(kctt[kh]);
#endif
                                        f &= ~KBRF;
                                }
                                else
                                        lkp=kh;
                                return;
                        }
                        else{   /* KBHF reset */
                                lkp=kh;
                                f |= KBHF;
                                return;
                        }
        else{   /* KBRF reset */
                if(kh == 0xff){
                        f |= KBRF;
                        f &= ~KBHF;
                        lkp=NULL;
                }
                return;
        }
}
#endif
#endif


void trscn(){

#ifdef OPERATOR_PC
        if ( trf & BELL_ACT ) {
                if( (++trs) >= BELL_STP){
                        DIS_SPKR;
                        trs=0;
                        trf &= ~BELL_ACT;
                }
                return;
        }
#endif

#ifndef OPERATOR_PC
        if( (ohs & ON_HOOK) == OFF_HOOK)        return;
#endif
        if( (trf & TONE_RNG_ACT) == RESET)      return;
        if( (trf & QUE_ACT) == RESET)           return;
                /* if all queues empty */
        if     ( trf & EXTRNL_QUE_ACT ) tr_pat=XQ_TR_PAT;
        else if( trf & EM_QUE_ACT     ) tr_pat=MQ_TR_PAT;
        else if( trf & INTRNL_QUE_ACT ) tr_pat=IQ_TR_PAT;
        else                            tr_pat=RQ_TR_PAT;

#ifdef OPERATOR_PC
/*      kt_outport8(RGCR,trg);
*/
#else
        XBYTE[RGCR]=trg;
#endif
        if( (++trs) >= TOTAL_RNG_STP){
                trb=1;
                trs=0;
                /* initialize tone ringer */

#ifdef OPERATOR_PC
#ifdef HDLC_8993
/*              kt_outport8(DSP_CR,0x72);
*/
                /* CPCMEM,DPCMEM,tone ringer,warble=16 Hz,DSP disable */
/*              kt_outport8(TR1,FREQ_1);
                kt_outport8(TR2,FREQ_2);
*/
                kt_outport8(TCR,0x87);         /* C ch,ch 3=PCM,ch 2=HDLC */
/*              kt_outport8(TRNS_CR,SPSKR);
*/
/*              kt_outport8(RGCR,trg);
                kt_outport8(DSP_CR,0x77);
*/
#endif

                EN_SPKR;
#else
                XBYTE[DSP_CR]=0x72;
                   /* CPCMEM,DPCMEM,tone ringer,warble=16 Hz,DSP disable */
                XBYTE[TR1]=FREQ_1;
                XBYTE[TR2]=FREQ_2;
                XBYTE[TCR]=0x87;         /* C ch,ch 3=PCM,ch 2=HDLC */
                XBYTE[TRNS_CR]=SPSKR;
                XBYTE[RGCR]=trg;
                XBYTE[DSP_CR]=0x77;
#endif
        }
        else
                if(trs >= TONE_RNG_STP){        /* tone ringer off */

#ifdef OPERATOR_PC
                        DIS_SPKR;
/*                      kt_outport8(DSP_CR,0x72);
*/
#else
                        XBYTE[DSP_CR]=0x72;
                        XBYTE[TRNS_CR]=0;
#endif
                }
                else{
                        trb<<=1;
                        if(trb & tr_pat){       /* tone ringer on */
#ifdef OPERATOR_PC
                                EN_SPKR;
/*                              kt_outport8(DSP_CR,0x77);
                                kt_outport8(TRNS_CR,SPSKR);
*/
#else
                                XBYTE[DSP_CR]=0x77;
                                XBYTE[TRNS_CR]=SPSKR;
#endif
                        }
                        else{   /* tone ringer off */
#ifdef OPERATOR_PC
                                DIS_SPKR;
/*                              kt_outport8(DSP_CR,0x72);
                                kt_outport8(TRNS_CR,0);
*/
#else
                                XBYTE[DSP_CR]=0x72;
                                XBYTE[TRNS_CR]=0;
#endif
                        }
                }
}


#ifdef OPERATOR_PC
#if (COMPILER == GCC)
void rtc(){
#else
void interrupt rtc(){
#endif
//      f |=INTRF;
        set_f_intr();
#if 0
        pph= (pph + 1) & 0x0f;
        tptw=time_tab[pph];
#endif // 0
}
#else
void init_t0_srl(){
               /* initialize the serial interface */
  SCON = 0xf0; /* SM0,SM1=11 mode #3, 9-bit UART with variable baud rate
                  SM2=1 enable the multiprocessor communication feature
                  REN=1 enable serial reception
                  TB8=0 the to be transmitted 9th data bit should always be zero
                  RB8=0 the received 9th data bit is don't care on write
                  TI=0 the transmit interrupt flag is cleared
                  RI=0 the receive interrupt flag is cleared  */
  PCON = 0x80; /* SMOD=1 to double the baud rate (1/32 fosc -> 1/16 fosc) */
               /* setup the timers */
  TH0  = 0xd8;
  TL0  = 0xf0; /* for 10 msec period (0-0xd8f0) x 1usec = 10 msec */
  TH1  =  250; /* (256-250)x16x1usec=96usec i.e. 10416.6 Bpsec */
  TMOD = 0xa1; /* for timer 1, 8-bit auto-reload timer mode working with
                  internal clock divided by a divide-by-32 prescaler and TL1,
                  which is loaded from TH1 when overflows .
                  for timer 0, 16-bit timer with no prescaling */
  TCON = 0xd0; /* TF1 don't care, set/reset by hardware,
                  TR1=1 to turn timer 1 on.
                  TF0 don't care, set/reset by hardware,
                  TR0=1 to turn on timer 0.
                  IEx, ITx for external interrupts, don't care */
  PS=1;        /* program the serial port interrupt priority to higher level */
  IE=0x92;     /* the interrupt enable register initialization
                  EA=1 enable interrupts , xx=00 don't care,
                  ES=1 enable the serial port interrupt
                  ET1=0 disable the timer 1 overflow interrupt
                  EX1=0 disable external interrupt 1
                  ET0=1 enable the timer 0 overflow interrupt
                  EX0=0 disable external interrupt 0 */
}

serial() interrupt 4 using 2 { BYTE data dat,sum; int data ptr;

        if(TI==1)  { TI=0; return;}   /* check if is xmit intr, clear the intrflag and return */
        if(SM2==0) { RI=0; return;}   /* if the sm2 bit is already cleared, it is a data byte */
        if(SBUF!=ID) { RI=0; return;} /* get the received byte, check if ID is matched
                                         if not, return */
        TI=1;
        sum=0;
        for( ptr=0; ptr<RAM_SIZE; ptr++ )  {
                 while (TI==0); TI=0;
                 dat = XBYTE [ptr];
/*               XBYTE[LED_PORT]=SBUF = dat;
*/
                 sum+=dat;
                 }
        while(TI==0); TI=0;
        SBUF=sum;
        RI=0;
}

rtc() interrupt 1 using 1 { /* Int Vector at 000BH, Reg Bank 1  */
        TR0=0;
        TH0=0xd8;
        TL0=0xf0;
        TR0=1;
//      f |=INTRF;
        set_f_intr();
        pph= ++pph & 0x0f;
        tptw=time_tab[pph];
}
#endif

#ifndef OPERATOR_PC
#pragma OPTIMIZE (4)
#endif

int  toiq(int a){
      /* xst a to internal queue,if fail return 0xff */
        int i;
        if( (i=ffiqr()) == ERR_CODE)    return ERR_CODE;
        if(pqi == 0xff)
            pqi=i;
        iqr[i].rp=0xff;
        iqr[i].i=a;
        iqr[i].no_dgt=0xff;
        if(eqi == 0xff)
                fqi=i;
        else
                iqr[eqi].rp=i;
        iqr[i].lp=eqi;
        eqi=i;
        trf |= INTRNL_QUE_ACT;
        iqs++; /* incerment internal queue size */
        lpg1 |=ILS_BIT_POS; /* active LED program */
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\ntoiq-fqi:%02x-eqi:%02x-trf:%02x-trs:%02x-iqs:%02x\r\n",fqi,eqi,trf,trs,iqs);
#endif
        return i;
}

#ifndef OPERATOR_PC
#pragma OPTIMIZE (5)
#endif


void  ptiq(BYTE r){  /* push record r to internal queue */
        if(r >= IQR_NO) return;
        iqr[r].lp=0xff;
        iqr[r].rp=fqi;
        if(fqi == 0xff)
                eqi=r;
        else
                iqr[fqi].lp=r;
        fqi=r;
        trf |= INTRNL_QUE_ACT;
        iqs++; /* incerment internal queue size */
        lpg1 |=ILS_BIT_POS; /* active LED program */
}

int  toxq(INDEX_TYPE pt){
 /* trunk pt to external queue,if fail return ERR_CODE */
        int i;
        if( (i=ffrxqr()) == ERR_CODE)        return ERR_CODE;
        if(pqx == 0xff)
            pqx=i;
        rxqr[i].rp=0xff;
        rxqr[i].pt=pt;
        rxqr[i].no_dgt=0xff;
        if(eqx == 0xff)
                fqx=i;
        else
                rxqr[eqx].rp=i;
        rxqr[i].lp=eqx;
        eqx=i;
        trf |= EXTRNL_QUE_ACT;
        xqs++; /* incerment external queue size */
        lpg1 |=XLS_BIT_POS; /* active LED program */
#ifdef OPERATOR_SIDE_DEBUG_ACT
        fprintf(opr_gonz_fp,"\r\ntoxq-fqx:%02x-eqx:%02x-trf:%02x-trs:%02x-xqs:%02x-pqx:%02x\r\n",fqx,eqx,trf,trs,xqs,pqx);
#endif
        return i;
}

#ifdef EM_CONT_SEP_OPR_Q
int  toemq(INDEX_TYPE pt){
 /* e&m trunk pt to e&m queue,if fail return ERR_CODE */
        int i;
        if( (i=ffrxqr()) == ERR_CODE)        return ERR_CODE;
        rxqr[i].rp=0xff;
        rxqr[i].pt=pt;
        if(eqem == 0xff)
                fqem=i;
        else
                rxqr[eqem].rp=i;
        rxqr[i].lp=eqem;
        eqem=i;
        trf |= EM_QUE_ACT;
        emqs++; /* incerment external queue size */
        lpg1 |=EMLS_BIT_POS; /* active LED program */
        return i;
}
#endif

void  ptxq(BYTE r){  /* push record r to external queue */
        if(r >= RXQR_NO) return;
        rxqr[r].lp=0xff;
        rxqr[r].rp=fqx;
        if(fqx == 0xff)
                eqx=r;
        else
                rxqr[fqx].lp=r;
        fqx=r;
        trf |= EXTRNL_QUE_ACT;
        xqs++; /* incerment external queue size */
        lpg1 |=XLS_BIT_POS; /* active LED program */
}

#ifdef EM_CONT_SEP_OPR_Q
void  ptemq(BYTE r){  /* push record r to e&m queue */
        if(r >= RXQR_NO) return;
        rxqr[r].lp=0xff;
        rxqr[r].rp=fqx;
        if(fqem == 0xff)
                eqem=r;
        else
                rxqr[fqem].lp=r;
        fqem=r;
        trf |= EM_QUE_ACT;
        emqs++; /* incerment external queue size */
        lpg1 |=EMLS_BIT_POS; /* active LED program */
}
#endif

int torq(INDEX_TYPE a){  /* trunk a to recall queue */
        int r;
        if( (r=ffrxqr()) == ERR_CODE)       return ERR_CODE;
        rxqr[r].rp=0xff;
        rxqr[r].pt=a;
        if(eqr == 0xff)
                fqr=r;
        else
                rxqr[eqr].rp=r;
        rxqr[r].lp=eqr;
        eqr=r;
        trf |= RECALL_QUE_ACT;
        rqs++; /* incerment recall queue size */
        lpg1 |=RLS_BIT_POS; /* active LED program */
        return r;
}

void  ptrq(BYTE r){  /* push record r to recall queue */
        if(r >= RXQR_NO) return;
        rxqr[r].lp=0xff;
        rxqr[r].rp=fqr;
        if(fqr == 0xff)
                eqr=r;
        else
                rxqr[fqr].lp=r;
        fqr=r;
        trf |= RECALL_QUE_ACT;
        rqs++; /* incerment recall queue size */
        lpg1 |=RLS_BIT_POS; /* active LED program */
}

#ifdef NEW_NUMBERING_PLAN
int fcp(){
        SDNST i;
        unsigned long j;
        BYTE a;
        unsigned int number_of_subscriber_digits;

        number_of_subscriber_digits=ocr.scdc;
        i=0;
        if(number_of_subscriber_digits >= 1){
                a=ocr.d2d1 & 0x0f;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 2){
                a=ocr.d2d1 >> 4;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 3){
                a=ocr.d4d3 & 0x0f;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 4){
                a=ocr.d4d3 >> 4;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 5){
                a=ocr.d6d5 & 0x0f;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 6){
                a=ocr.d6d5 >> 4;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 7){
                a=ocr.d8d7 & 0x0f;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 8){
                a=ocr.d8d7 >> 4;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 9){
                a=ocr.d10d9 & 0x0f;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 10){
                a=ocr.d10d9 >> 4;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
        if(number_of_subscriber_digits >= 11){
                a=ocr.d12d11 & 0x0f;
                if(a > 9)       a=0;
                i*=10;
                i +=a;
        }
#ifdef AHAR
        if((ocr.d2d1 & 0x0f) == 9){
                i/=10;
                i-=900;
                if(i >= 40) return NULL;
                i+=216;
                return (unsigned int) i;
        }
#endif
        if(no_of_subscriber_groups != 0){
            SDNST k;
            for(a=0;a<no_of_subscriber_groups;a++){
                if((k=(i-subscriber_group[a].directory_start)) < subscriber_group[a].no_of_ports){
                    i=k+subscriber_group[a].port_start;
                    if(i >= NO_EXTENSION) return ERR_CODE;
                    return (unsigned int)i;
                }
            }
            return ERR_CODE;
        }
        j=i-first_directory_decimal;     /* opr file(s) always include "A" header file
                                           so they may use ...DECIMAL instead of F_D_N */
#ifdef WE_DO_HAVE_TWO_FDN
        if(j >= NO_EXTENSION_FOR_F_D_N_A){
                j = (i-hex2l(F_D_N_B))+NO_EXTENSION_FOR_F_D_N_A;  /*  FIRST_DIRECTORY_DECIMAL;*/
        }
#endif
        if(j >= MAX_EXISTING_NO) return ERR_CODE;
        i=from_dialled_to_internal_no[(unsigned int)j];

#ifdef SIREN_INSTALLED
        if(i == SIREN_EXTENSION)
        /* siren can not be accessed by dialling its directory number */
                return ERR_CODE;
#endif


#ifdef AMPLIFIER_INSTALLED
        if(i == AMPLIFIER_EXTENSION)
        /* amplifier can not be accessed by dialling its directory number */
                return ERR_CODE;
#endif

        if(i >= NO_TOTAL_EXTENSION)
                return ERR_CODE;

#ifdef MUSIC_TAPE_RECORDER
        if(i == TAPE_RECORDER_EXT)
                return ERR_CODE;
#endif

        return (unsigned int)i;
}
#endif

void errorcp(){

#ifdef DEBUG_ACTIVE
        str2lcd(" ERROR ");
#endif
#ifdef OPERATOR_PC
/*
        trs=0;
        trf |= BELL_ACT;
        EN_SPKR;
*/
        activate_bell();
#endif
}


#ifdef  TIME_DATE_DISPLAY

void ddt_ad(){  /* display date time ,A.D. */

#ifndef UPDATE_DATE_TIME
        BYTE a;
        sdt=*get_dt(&sdt);
#endif

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);

#ifdef UPDATE_DATE_TIME
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("                   ...ñ®ê ˘¢ëıç §¯óê§ïê");
        else
#endif
                str2lcd(" OPERATOR READY...  ");
#endif

#else
        init_lcd();
#ifdef UPDATE_DATE_TIME
        str2lcd("OPR. READY...");
#endif
#endif

#ifndef UPDATE_DATE_TIME
        for(a=0;a<4;a++)
                tolcdq(0x20,TO_LCD_DD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("†˛§ëó:");
        else
#endif
                str2lcd("date: ");
        hex2lcd(sdt.mo);
        tolcdq('/',TO_LCD_DD);
        hex2lcd(sdt.da);
        tolcdq('/',TO_LCD_DD);
        hex2lcd(sdt.yr>>8);
        hex2lcd(sdt.yr);
        for(a=0;a<4;a++)
                tolcdq(0x20,TO_LCD_DD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2lcd("ˆëı•: ");
        else
#endif
                str2lcd("time: ");
        hex2lcd(sdt.hr);
        tolcdq(':',TO_LCD_DD);
        hex2lcd(sdt.mi);
        for(a=0;a<8;a++)
                tolcdq(0x20,TO_LCD_DD);
#endif
}
#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
//void str2scrn(BYTE *x,BYTE y,char *s,BYTE attr){
void str2scrn(BYTE *x,BYTE y,char *s,CHAT_TYPE attr){
        if(*s != '\0')
                while(*s != '\0'){
                        displscrn(*x,y,*s,attr);
                        s++;
                        (*x)++;
                }
}
//void hex2scrn(BYTE *x,BYTE y,BYTE a,BYTE attr){
void hex2scrn(BYTE *x,BYTE y,BYTE a,CHAT_TYPE attr){
        BYTE b;
        b=(a>>4) & 0x0f;
        if(b){ /* if 1st digit not 0 */
                displscrn(*x,y,h2c(b),attr);
                (*x)++;
        }
        else{
                displscrn(*x,y,' ',attr);
                (*x)++;
        }
        b=a & 0x0f;
        displscrn(*x,y,h2c(b),attr);
        (*x)++;
}
void ddt_ad_always(void){  /* display date time ,A.D. always */
        BYTE a;

        sdt=*get_dt(&sdt);
        a=19;
#if 0
        str2scrn(&a,0,"    ",7);
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2scrn(&a,0,"†˛§ëó:",7);
        else
#endif
                str2scrn(&a,0,"Date: ",7);
        hex2scrn(&a,0,sdt.mo,7);
        str2scrn(&a,0,"/",7);
        hex2scrn(&a,0,sdt.da,7);
        str2scrn(&a,0,"/",7);
        hex2scrn(&a,0,sdt.yr>>8,7);
        hex2scrn(&a,0,sdt.yr,7);
        str2scrn(&a,0,"    ",7);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2scrn(&a,0,"ˆëı•: ",7);
        else
#endif
                str2scrn(&a,0,"Time: ",7);
        hex2scrn(&a,0,sdt.hr,7);
        str2scrn(&a,0,":",7);
        hex2scrn(&a,0,sdt.mi,7);
        str2scrn(&a,0,":",7);
        hex2scrn(&a,0,sdt.se,7);
        str2scrn(&a,0,"     ",7);
#endif // 0
        str2scrn(&a,0,"    ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2scrn(&a,0,"†˛§ëó:",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        else
#endif
                str2scrn(&a,0,"Date: ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        hex2scrn(&a,0,sdt.mo,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        str2scrn(&a,0,"/",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        hex2scrn(&a,0,sdt.da,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        str2scrn(&a,0,"/",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        hex2scrn(&a,0,sdt.yr>>8,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        hex2scrn(&a,0,sdt.yr,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        str2scrn(&a,0,"    ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)
                str2scrn(&a,0,"ˆëı•: ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        else
#endif
                str2scrn(&a,0,"Time: ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        hex2scrn(&a,0,sdt.hr,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        str2scrn(&a,0,":",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        hex2scrn(&a,0,sdt.mi,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        str2scrn(&a,0,":",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        hex2scrn(&a,0,sdt.se,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        str2scrn(&a,0,"     ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
}
#endif
#endif
#endif

void leds(){
        BYTE a,s=0;
        BYTE *b=(BYTE *)0;

#ifdef OPERATOR_PC
        BYTE n;
#else
        unsigned int n;
#endif
        if(lpg1 == RESET)       return;
        for(a=0x01;a;a<<=1){
                if( (lpg1 & a) == RESET)        continue;
                switch(a){
                        case  XLS_BIT_POS: b= &xlc; n=EXTRNL_LED; s=xqs;
                                           break;
                        case  ILS_BIT_POS: b= &ilc; n=INTERNAL_LED; s=iqs;
                                           break;
                        case  RLS_BIT_POS: b= &rlc; n=RECALL_LED; s=rqs;
                                           break;
                        case  PLS_BIT_POS: b= &plc; n=PROG_LED;
                                           s=ICQ_1ST_THRESHOLD-1;
                                           /* for using BLINK1_CNTR */
                                           break;
                        case  NLS_BIT_POS: b= &nlc; n=NIGHT_LED;
                                           s=ICQ_1ST_THRESHOLD;
                                           /* for using BLINK2_CNTR */
                                           break;
#ifdef EM_CONT_SEP_OPR_Q
                        case EMLS_BIT_POS: b= &emlc; n=EM_LED; s=emqs;
                                           break;
#endif
#ifdef OPERATOR_PC
                        default: n=(BYTE)KT_NULL; break;
#else
                        default: n=(unsigned int)KT_NULL; break;
#endif
                }
#ifdef OPERATOR_PC
                if(n == (BYTE)KT_NULL)      continue;
#else
                if(n == (unsigned int)KT_NULL)      continue;
#endif
                if(*b){  /* if counter not zero */
                        --(*b);
                        continue;
                }
                if(lsg1 & a){ /* if LED on */
                        led_reset(n);
                        lsg1 &= ~a;  /* update LED status */
                        if(s){  /* if queue size != 0; new program needed */
                                if(s < ICQ_1ST_THRESHOLD)
                                        *b=BLINK1_CNTR;
                                else
                                        *b=BLINK2_CNTR;
                        }
                        else    /* new program not needed */
                                lpg1 &= ~a;  /* reset program bit for LED */
                }
                else{ /* LED off */
                        if(s){  /* if queue size != 0; new program needed */
                                led_set(n);
                                lsg1 |=a;
                                if(s < ICQ_1ST_THRESHOLD)
                                        *b=BLINK1_CNTR;
                                else
                                        *b=BLINK2_CNTR;
                        }
                        else    /* new program not needed */
                                lpg1 &= ~a;  /* reset program bit for LED */
                }
        }
}

#ifndef OPERATOR_PC
#pragma OPTIMIZE (4)
#endif


long hex2bcd(long a){
        unsigned long l;
        long b;
        BYTE no_dgt;
        if(a < 0x0a) return a;
        for(l=a,no_dgt=0;l != 0L; l/=10,no_dgt++);
        b=0;
        if (no_dgt >= 8)  b |= ((a/10000000L)%10) << 28;
        if (no_dgt >= 7)  b |= ((a/1000000L) %10) << 24;
        if (no_dgt >= 6)  b |= ((a/100000L)  %10) << 20;
        if (no_dgt >= 5)  b |= ((a/10000L)   %10) << 16;
        if (no_dgt >= 4)  b |= ((a/1000)     %10) << 12;
        if (no_dgt >= 3)  b |= ((a/100)      %10) << 8;
        if (no_dgt >= 2)  b |= ((a/10)       %10) << 4;
        if (no_dgt >= 1)  b |= ((a)          %10);
        return b;
}

#ifndef OPERATOR_PC
#pragma OPTIMIZE (5)
#endif


#ifdef OPERATOR_PC
#if ( (PARAMETER_DOWN_UP_LOAD == DEFINE_VALUE) || (DETAIL_BILLING == DEFINE_VALUE) )

void omap(){  /* operator's maintenance & administration processing */
        OMAR a;
        int b,h;
// night upload
#if ((defined DAY_NIGHT_PARAMETERS_FILE) || (NO_OPRTR > 1))
        INDEX_TYPE p;
#endif
// end night upload
#ifdef SYSTEM_MANUAL_TEST
        int c;
#endif
#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
#ifdef NEW_DEP
        fpos_t pos;
#endif
#endif
#endif
        f |= MAF;
//      while( (f & INTRF) == RESET){
        while( f_intr() == RESET){


#ifdef PARAMETER_DOWN_UP_LOAD
#ifndef OPR_UPLOAD_WITH_TIMER
                if(f & PARM_UP_1){
#ifdef HDLC_8993
                        /* find tx queue size */
                        b=tx[0].qe - tx[0].qf;
                        if(b < 0)
                                b += TX_QUE_LEN;  /* size of queue */
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#ifdef CPU_EC512
                        b=checkfree_buf(&nettx_st[0]) - (nettx_st[0].size - 1);
#else
                        b=checkfree_buf(&nettx_st[hot_link]) - (nettx_st[hot_link].size - 1);
#endif
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                        b=check_used_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st));
#endif
                        if(b == 0){  /* if tx queue empty,upload finished */
                                f &= ~PARM_UP_1;
                                unmask_stm();  /* unmask all tasks */
                                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("        .ñ®ê ˘óÍë˛ ˘ıóë° ë˚§óıê§ëï Òë®§ê");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                                        if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                                        if(ocr.state==OPR_PROG)
                                                str2lcd("           :¢¯™˛ı •ëËç ˝¢˛¢õ ¸•˛§ ˘ıë˜§ì");
                                }
                                else{
#endif

                                        str2lcd("UP LOAD FINISHED.                       ");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                                        if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                                        if(ocr.state==OPR_PROG)
                                                str2lcd("START NEW PROGRAM:");
#ifdef OPERATOR_FARSI
                                }
#endif




//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
#ifdef LIM
                                if(oprnight == 1){
                                        p.t=OPR_T;
                                        rtn101_2(p);
                                }
                                else if(oprday == 1){
                                        p.t=OPR_T;
                                        rtn102_2(p);
                                }
#else

                                if(oprnight == 1){
                                        p.t=OPR_T;
                                        rtn101_1(p);
                                }
                                else if(oprday == 1){
                                        p.t=OPR_T;
                                        rtn102_1(p);
                                }
#endif
#endif
//end night upload

                                break;
                        }
                        if( (b & 0x0f) == 0)
                                str2lcd("A ");
                        break;
                }
                if(f & PARM_UP_2){
#ifdef HDLC_8993
                        /* find tx queue size */
                        b=tx[1].qe - tx[1].qf;
                        if(b < 0)
                                b += TX_QUE_LEN;  /* size of queue */
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#ifdef CPU_EC512
                        b=checkfree_buf(&nettx_st[1]) - (nettx_st[1].size - 1);
#else
                        b=checkfree_buf(&nettx_st[hot_link]) - (nettx_st[hot_link].size - 1);
#endif
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                        b=check_used_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st));
#endif
                        if(b == 0){  /* if tx queue empty,upload finished */
                                f &= ~PARM_UP_2;
                                unmask_stm();  /* unmask all tasks */
                                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("        .ñ®ê ˘óÍë˛ ˘ıóë° ë˚§óıê§ëï Òë®§ê");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                                        if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                                        if(ocr.state==OPR_PROG)
                                                str2lcd("           :¢¯™˛ı •ëËç ˝¢˛¢õ ¸•˛§ ˘ıë˜§ì");
                                }
                                else{
#endif

                                        str2lcd("UP LOAD FINISHED.                       ");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                                        if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                                        if(ocr.state==OPR_PROG)
                                                str2lcd("START NEW PROGRAM:");
#ifdef OPERATOR_FARSI
                                }
#endif

//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                                if(oprnight == 2){
                                        p.t=OPR_T;
                                        rtn101_2(p);
                                }
                                else if(oprday == 2){
                                        p.t=OPR_T;
                                        rtn102_2(p);
                                }
#else
                                if(oprnight == 2){
                                        p.t=OPR_T;
                                        rtn101_1(p);
                                }
                                else if(oprday == 2){
                                        p.t=OPR_T;
                                        rtn102_1(p);
                                }
#endif
#endif
#endif
//end night upload

                                break;
                        }
                        if( (b & 0x0f) == 0)
                                str2lcd("B ");
                        break;
                }

                if(f & PARM_UP_3){
#ifdef HDLC_8993
                        /* find tx queue size */
                        b=tx[2].qe - tx[2].qf;
                        if(b < 0)
                                b += TX_QUE_LEN;  /* size of queue */
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                        b=checkfree_buf(&nettx_st[hot_link]) - (nettx_st[hot_link].size - 1);
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                        b=check_used_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st));
#endif
                        if(b == 0){  /* if tx queue empty,upload finished */
                                f &= ~PARM_UP_3;
                                unmask_stm();  /* unmask all tasks */
                                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("        .ñ®ê ˘óÍë˛ ˘ıóë° ë˚§óıê§ëï Òë®§ê");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                                        if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                                        if(ocr.state==OPR_PROG)
                                                str2lcd("           :¢¯™˛ı •ëËç ˝¢˛¢õ ¸•˛§ ˘ıë˜§ì");
                                }
                                else{
#endif

                                        str2lcd("UP LOAD FINISHED.                       ");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                                        if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                                        if(ocr.state==OPR_PROG)
                                                str2lcd("START NEW PROGRAM:");
#ifdef OPERATOR_FARSI
                                }
#endif
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                                if(oprnight == 3){
                                        p.t=OPR_T;
                                        rtn101_1(p);
                                }
                                else if(oprday == 3){
                                        p.t=OPR_T;
                                        rtn102_1(p);
                                }
#endif
#endif
#endif
//end night upload
                                break;
                        }
                        if( (b & 0x0f) == 0)
                                str2lcd("C ");
                        break;
                }
#endif

#endif // OPR_UPLOAD_WITH_TIMER

                a=frommaq();
                if(a.wt.t == NULL_T)  /* queue is empty */
                        break;

#ifdef PARAMETER_DOWN_UP_LOAD
                if(f & PARM_DOWN){  /* if parameters down load in progress */
                        csp(a);  /* collect system's parameters */
                        continue;
                }
#endif
                b=a.wt.i;
                switch(b){
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
                        case (MA_SYS_P | MAC_CLOSE_DBRF):
                          {
                            if(a.parm & 0x8000){
                                write_date_record(a.parm&0x7fff);
                            }
                            else{
                                create_new_dbrf_file_with_sys_time(a.parm);
                            }
                          }
                          break;
#endif
#endif
#endif

#ifdef OPR_UPLOAD_WITH_TIMER
                        case (MA_SYS_P | MAC_UPLOAD_FINISHED):
                                mac_upload_finished_rtn(a);
                                break;
#endif
#ifdef HOT_STANDBY
                        case (MA_SYS_P | MAC_THIS_SYSTEM_IS_HOT):
                                mac_this_sys_is_hot_rtn(a);
                                break;
                        case (MA_SYS_P | MAC_HOT_SYSTEM_ANSWER):
                                // MAC_HOT_SYSTEM_CHECK is also sent to standby system
                                // system.but its LAPB layer in system side is not
                                // active.so it will remain in nettx[] buffer in
                                // oprerator side.if switch over is happened,this
                                // packet is sent to new hot system,but its answer
                                // should be ignored.
                                if (hot_link == UNKNOWN_LINK){
                                        unmask_stm();
#ifdef DYNAMIC_IP_ALLOCATION
                                        {
                                                unsigned int old_button;

                                                if(a.parm == FROM_MASTER_SYSTEM) {
                                                        ms_button[a.htby]=0;old_button=1; // master-button=0, slave-button=1
                                                }
                                                else{
                                                        ms_button[a.htby]=1;old_button=0;
                                                }
                                                if(a.htby == LAPB_LINK0)
                                                        ms_button[LAPB_LINK1]=old_button; // master-button=0, slave-button=1
                                                else
                                                        ms_button[LAPB_LINK0]=old_button; // master-button=0, slave-button=1
                                        }
#endif
                                        hot_link=a.htby;
#ifdef DYNAMIC_IP_ALLOCATION
                                        set_led_attr(sync_led[ms_button[hot_link]],KT_RED,KT_WHITE,KT_NO_BLINKING);
                                        set_led_attr(link_up_led[ms_button[hot_link]],KT_RED,KT_WHITE,KT_NO_BLINKING);
#else

                                        set_led_attr(sync_led[hot_link],KT_RED,KT_WHITE,KT_NO_BLINKING);
                                        set_led_attr(link_up_led[hot_link],KT_RED,KT_WHITE,KT_NO_BLINKING);
#endif
                                        deletw(htby_win);
                                }
                                break;
                        case (MA_SYS_P | MAC_HTBY_LINK_IS_OK):

                                htby_link_flag=SET;
                                break;
                        case (MA_SYS_P | MAC_HTBY_LINK_IS_NOT_OK):

                                htby_link_flag=RESET;
                                break;
#endif
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                        case (MA_SYS_P | MAC_HDLC_RS232_CHECK_LINK):
                        // if the state of LAPB layer in hot system has been INFO_TRX
                        // before swchovr,with receiving MAC_THIS_SYSTEM_IS_HOT from the
                        // other system,link_flag[prv hot] has changed to RESET. LAPB layer do not
                        // update it after switching to this system again,because its
                        // state has not changed.note that LAPB layer in
                        // system side in standby system is not active and
                        // in operator side pmamftx will not push any data to
                        // standby system.So I should set it again.
//                              if (hot_link != UNKNOWN_LINK)
//                                      link_flag[hot_link]=SET;
                                break;
#endif
#ifdef DISPLAY_EM_STATUS
                        case (MA_SYS_P | MAC_DISPLAY_EM_STATUS):
                                b=a.parm>>8;
                                if(dtsf)
                                        if(b<NO_EM_DISPLAY){
                                                dts[b]=a.parm&0xff;
                                                dtschng[b]=SET;
                                        }
                                break;
#endif
#ifdef DISPLAY_CAS3_STATUS
                        case (MA_SYS_P | MAC_DISPLAY_CAS3_STATUS):
                                if((a.parm>>13) != (current_cas3_page & 0x07))
                                        break;
                                b=(a.parm>>8) & 0x01f;
                                if(dcsf){
                                    b += current_cas3_page * NO_CAS3_DISPLAYED_PER_PAGE;
                                    if(b<NO_CAS3_DISPLAY){
                                        BYTE state=a.parm&0xff;
                                        if((state == ICT) || (state == OGT)){
                                            dcsict[b]=state;
                                            dcsictchng[b]=SET;
                                        }
                                        else{
                                            dcs[b]=state;
                                            dcschng[b]=SET;
                                        }
                                    }
                                }
                                break;

#ifdef DISPLAY_CAS3_LINE_STATUS
                        case (MA_SYS_P | MAC_DISPLAY_3BIT_STATUS):
                                if((a.parm>>13) != (current_cas3_page & 0x07))
                                        break;
                                b=(a.parm>>8) & 0x01f;
                                if(dcsf){
                                        b += current_cas3_page * NO_CAS3_DISPLAYED_PER_PAGE;
                                        if(b<NO_CAS3_DISPLAY){
                                                dcsb[b]=a.parm&0xff;
                                                dcsbchng[b]=SET;
                                        }
                                }
                                break;
#endif

#endif

#ifdef DISPLAY_CAS1_STATUS
                        case (MA_SYS_P | MAC_DISPLAY_CAS1_STATUS):
                                b=a.parm>>8;
                                if(d1sf)
                                        if(b<NO_CAS1_DISPLAY){
                                                d1s[b]=a.parm&0xff;
                                                d1schng[b]=SET;
                                        }
                                break;

#ifdef DISPLAY_CAS1_LINE_STATUS
                        case (MA_SYS_P | MAC_DISPLAY_1BIT_STATUS):
                                b=a.parm>>8;
                                if(d1sf)
                                        if(b<NO_CAS1_DISPLAY){
                                                d1sb[b]=a.parm&0xff;
                                                d1sbchng[b]=SET;
                                        }
                                break;
#endif

#endif


#ifdef PARAMETER_DOWN_UP_LOAD
                        case (MA_SYS_P | MAC_DOWN_LOAD_PARAMETERS):

#ifdef OPERATOR_PC
                                tolcdq(0xff,CLEAR_LCD);
#else
                                init_lcd();
#endif


#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("...ñ®ê ˘¢™ •ëËç ë˚§óıê§ëï ñÍë˛§¢");
                                else
#endif
                                        str2lcd("DOWN LOAD STARTED...");
                                f |= PARM_DOWN;
                                pbs=0;
                                set_stm_for_up_down_load();
                                /* only leds(),pc_timer_init(),lcds() & */
                                /* onh() are unmasked */
                                DIS_SPKR;  /* disable tone ringer */
                                csp(a);
                                break;
#endif

#ifdef DETAIL_BILLING
                        case (MA_SYS_P | MAC_BILLING_PACKET_SIZE):
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
#ifdef HOT_STANDBY
                                h=a.htby;
#else
                                h=a.sys;
#endif
#ifdef BEHNAMIR
                                rdbs[h]=a.parm & 0xff;
                                db[h][MAX_DETAIL_BILLING_INFO_SIZE]=((a.parm&0xff00) >> 8);
#else
                                rdbs[h]=a.parm;
#endif
                                if( (rdbs[h] > MAX_DETAIL_BILLING_INFO_SIZE) ||
                                    (rdbs[h] < MIN_DETAIL_BILLING_INFO_SIZE) )
                                        rdbs[h]=0;
                                else
                                        rdbi[h]=0;
#endif
                                break;
                        case (MA_SYS_P | MAC_BILLING_RECORD):
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
#ifdef HOT_STANDBY
                                h=a.htby;
#else
                                h=a.sys;
#endif
                                if(rdbs[h] == 0)
                                        break;
                                db[h][rdbi[h]]=a.parm;
                                ++rdbi[h];
                                if(rdbi[h] >= rdbs[h]){
                                    unsigned int a=ffdbr();
                                    if(a == (unsigned int)(-1)){
                                        /* if detail billing record not available */
                                        /* report will be lost or output to the */
                                        /* err.dbg file */
#ifdef OPERATOR_PC
                                        fprintf(fer,"DETAIL BILLING RECORD NOT AVAILABLE:\n");
                                        {
                                            BYTE t;
                                            for(t=0;t < rdbs[h];t++)
                                                fprintf(fer,"%x ",db[h][t]);
                                        }
                                        fprintf(fer,"\n");
#endif
                                    }
                                    if(rdb(h,a) == OK)
                                        toadbq(a);
                                    else
                                        rdbr(a);
                                    rdbs[h]=0;
                                    rdbi[h]=0;
                                }
#endif
                                break;
                        case (MA_SYS_P | MAC_SET_DATE_FOR_COMPLETE_BILLING):
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
#define decimal2bcd(i) ((((i)/10) << 4) | ((i)%10))
                                billing_sdt.yr=decimal2bcd((a.parm & 0x7e00) >> 9);
                                billing_sdt.mo=decimal2bcd((a.parm & 0x01e0) >> 5);
                                billing_sdt.da=decimal2bcd (a.parm & 0x001f);
#endif
#endif
                                break;
                        case (MA_SYS_P | MAC_COMPLETE_BILLING_PACKET_SIZE):
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
#ifdef HOT_STANDBY
                                h=a.htby;
#else
                                h=a.sys;
#endif
                                complete_rdbs[h]=a.parm;
                                if(complete_rdbs[h] > MAX_DETAIL_BILLING_RECORD)
                                        complete_rdbs[h]=0;
                                else
                                        complete_rdbi[h]=0;
#endif
                                break;
                        case (MA_SYS_P | MAC_COMPLETE_BILLING_RECORD):
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
#ifdef HOT_STANDBY
                                h=a.htby;
#else
                                h=a.sys;
#endif
                                if(complete_rdbs[h] == 0)
                                        break;
                                complete_dbr[h][complete_rdbi[h]]=a.parm&0xff;
                                ++complete_rdbi[h];
                                complete_dbr[h][complete_rdbi[h]]=a.parm>>8;
                                ++complete_rdbi[h];
                                if(complete_rdbi[h] >= complete_rdbs[h]){
                                    unsigned int i,a=ffdbr();
                                    DBR *bp;
                                    if(a == (unsigned int)(-1)){
#ifdef OPERATOR_PC
                                        fprintf(fer,"DETAIL BILLING RECORD NOT AVAILABLE:\n");
#endif
                                    }
                                    bp=&dbr[a];
                                    bp->s=complete_rdbs[h];
                                    for(i=0;i<complete_rdbs[h];i++){
                                        bp->ri[i]=complete_dbr[h][i];
                                    }
#ifndef SEND_BILLING_TIME_FROM_SYS_TO_OPR
                                    adjust_billing_time(a);
#endif
#ifdef CPU_EC512
                                    adjust_local_pulse(a);
#endif
#if ((!defined SEND_BILLING_TIME_FROM_SYS_TO_OPR) || (defined CPU_EC512))
                                    calculate_checksum(a);
#endif
#ifdef DISPLAY_BILLING
                                    display_billing(a);
#endif
                                    write_billing_record_to_file(a);
                                    rdbr(a);
                                    complete_rdbs[h]=0;
                                    complete_rdbi[h]=0;
                                }
#endif
                                break;
#endif // DETAIL_BILLING

#ifdef REPORT_BULK_BILLING
                        case (MA_SYS_P | MAC_BULK_BILLING_PACKET_SIZE):
#if( (TON == BULK_BILLING_OPR) || ((defined SECOND_BULK_BILLING_OPR) && (TON == SECOND_BULK_BILLING_OPR)) )
                                h=a.sys;
                                rbbs[h]=a.parm;
                                if (rbbs[h] != BULK_BILLING_RECORD_SIZE)
                                        rbbs[h]=0;
                                else
                                        rbbi[h]=0;
#endif
                                break;
                        case (MA_SYS_P | MAC_BULK_BILLING_RECORD):
#if( (TON == BULK_BILLING_OPR) || ((defined SECOND_BULK_BILLING_OPR) && (TON == SECOND_BULK_BILLING_OPR)) )
                                h=a.sys;
                                if(rbbs[h] == 0)
                                        break;
                                if(rbbi[h] == 0){
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                                        if(h==2)
                                                bbxst[h]=a.parm+NO_EXTENSION_A+NO_EXTENSION_B;
                                        else
#endif
                                        if(h==1)
                                                bbxst[h]=a.parm+NO_EXTENSION_A;
                                        else
#endif
                                                bbxst[h]=a.parm;
                                        rbbi[h]++;
                                        break;
                                }
                                if(rbbi[h] == 1){
                                        wbbr(bbxst[h],a.parm);
                                        rbbs[h]=0;
                                        rbbi[h]=0;
                                }
#endif
                                break;
#endif


#ifdef MALICIOUS_PRINT_ON_U
                        case (MA_SYS_P | MAC_COMPLETE_MALICIOUS_PACKET_SIZE):
#if ((defined MY_UDP_FOR_OAM) || (TON == MALICIOUS_PRINT_OPR))
#ifdef HOT_STANDBY
                                h=a.htby;
#else
                                h=a.sys;
#endif
                                complete_rmps[h]=a.parm;
                                if(complete_rmps[h] > 150)
                                        complete_rmps[h]=0;
                                else
                                        complete_rmpi[h]=0;
#endif
                                break;
                        case (MA_SYS_P | MAC_COMPLETE_MALICIOUS_RECORD):
#if ((defined MY_UDP_FOR_OAM) || (TON == MALICIOUS_PRINT_OPR))
#ifdef HOT_STANDBY
                                h=a.htby;
#else
                                h=a.sys;
#endif
                                if(complete_rmps[h] == 0)
                                        break;
                                complete_rmp[h][complete_rmpi[h]]=a.parm&0xff;
                                ++complete_rmpi[h];
                                complete_rmp[h][complete_rmpi[h]]=a.parm>>8;
                                ++complete_rmpi[h];
                                if(complete_rmpi[h] >= complete_rmps[h]){
                                    if(complete_rmp[h][0] == 0xff)
                                        handle_malicious_info((char *)&complete_rmp[h][1],(char *)&complete_rmp[h][12]);
                                    else
                                        handle_malicious_info((char *)&complete_rmp[h][0],(char *)&complete_rmp[h][10]);
                                    complete_rmps[h]=0;
                                    complete_rmpi[h]=0;
                                }
#endif
                                break;
                        case (MA_SYS_P | MAC_MALICIOUS_PACKET_SIZE):
#if (TON == MALICIOUS_PRINT_OPR)
#ifdef HOT_STANDBY
                                h=a.htby;
#else
                                h=a.sys;
#endif
                                rmps[h]=a.parm;
                                if (rmps[h] >= MAX_MALICIOUS_RECORD_SIZE)
                                        rmps[h]=0;
                                else
                                        rmpi[h]=0;
#endif
                                break;
                        case (MA_SYS_P | MAC_MALICIOUS_RECORD):
#if (TON == MALICIOUS_PRINT_OPR)
#ifdef HOT_STANDBY
                                h=a.htby;
#else
                                h=a.sys;
#endif
                                if(rmps[h] != 0){
                                    rmp_data[h][rmpi[h]]=a.parm;
                                    rmpi[h]++;
                                    if(rmpi[h] >= rmps[h]){
                                        char mlc_buf[120],mlced_buf[11];
                                        char date_time[70];
                                        sdt=*get_dt(&sdt);
                                        sprintf(date_time,", %02x:%02x:%02x %02x:%02x:%02x",
                                                       sdt.yr&0xff,
                                                       sdt.mo,
                                                       sdt.da,
                                                       sdt.hr,
                                                       sdt.mi,
                                                       sdt.se
                                                    );
                                        strcat(mlc_buf,date_time);
                                        if(rmps[h] == 3){
                                            wmpr(h,mlc_buf);
                                        }
                                        else{
                                            wmpr_isup(h,mlc_buf);
                                        }
                                        strcat(mlc_buf,date_time);
                                        find_real_mlced(h,mlced_buf);
                                        handle_malicious_info(mlced_buf,mlc_buf);
                                        rmps[h]=0;
                                        rmpi[h]=0;
                                    }
                                }
#endif
                                break;
#endif // MALICIOUS_PRINT_ON_U


#ifdef SI_DEBUG_ON_U
                        case (MA_SYS_P | MAC_SI_DEBUG_ON_U):
#if(TON == SI_DEBUG_OPR)
                                h=a.sys;
                                putc(a.parm,fsi[h]);
#endif
                                break;
#endif

#ifdef AUTO_UPLOAD
                        case (MA_SYS_P | MAC_UPLOAD_REQUEST):
                                auto_upload(SET,0,0);
#ifdef LIM
/* 12/7/76: this is not correct, we should have two types of UPLOAD_REQUEST
   MACros from system side, and upon reception of each do auto_upload
   only for that */
                                auto_upload(SET,1,0);
#if(NO_EXTENSION_C != 0)
                                auto_upload(SET,2,0);
#endif
#endif
                                break;
#endif

#ifdef EEPROM
                        case (MA_SYS_P | MAC_EEPROM_LOAD_START):
                                led_set(RLS_LED);
                                break;

                        case (MA_SYS_P | MAC_EEPROM_LOAD_FINISHED):
                                led_reset(RLS_LED);
                                break;
#endif

#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
                        case (MA_SYS_P | MAC_ALARM):
                                if((a.parm & 0x8000) == 0) goto subscriber;
                                b=a.parm & 0xf800;
                                switch(b){
                                        case CONNECTION_ALARM:
                                                b=a.parm & 0x07ff;
                                                date_to_log();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                switch(b & 0x0700){
                                                  case RSU_CONNECTION_LOST:
                                                    b&=0x00ff;
                                                    fprintf(flog,"RSU#:%d has lost connection",b);
#ifdef DISPLAY_ALARM
                                                    show_alarm("RSU#:%d has lost connection",b);
#endif
                                                    break;
                                                  case RSU_CONNECTION_ESTABLISHED:
                                                    b&=0x00ff;
                                                    fprintf(flog,"RSU#:%d has established connection",b);
#ifdef DISPLAY_ALARM
                                                    show_alarm("RSU#:%d has established connection",b);
#endif
                                                    break;
                                                  default:
                                                    break;
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case HOT_STANDBY_ALARM:
                                                b=a.parm & 0x07ff;
                                                date_to_log();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                switch(b){
                                                  case MASTER_STARTED_AS_HOT            :
                                                    fprintf(flog,"Master Started as Hot");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Master Started as Hot");
#endif
                                                    break;
                                                  case MASTER_STARTED_AS_STANDBY        :
                                                    fprintf(flog,"Master Started as Standby");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Master Started as Standby");
#endif
                                                    break;
                                                  case SLAVE_STARTED_AS_HOT             :
                                                    fprintf(flog,"Slave Started as Hot");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Slave Started as Hot");
#endif
                                                    break;
                                                  case SLAVE_STARTED_AS_STANDBY         :
                                                    fprintf(flog,"Slave Started as Standby");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Slave Started as Standby");
#endif
                                                    break;
                                                  case MASTER_CHANGED_TO_HOT            :
                                                    fprintf(flog,"Master Changed to Hot");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Master Changed to Hot");
#endif
                                                    break;
                                                  case MASTER_CHANGED_TO_STANDBY        :
                                                    fprintf(flog,"Master Changed to Standby");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Master Changed to Standby");
#endif
                                                    break;
                                                  case SLAVE_CHANGED_TO_HOT             :
                                                    fprintf(flog,"Slave Changed to Hot");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Slave Changed to Hot");
#endif
                                                    break;
                                                  case SLAVE_CHANGED_TO_STANDBY         :
                                                    fprintf(flog,"Slave Changed to Standby");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Slave Changed to Standby");
#endif
                                                    break;
                                                  case MASTER_LOST_CONNECTION_WITH_SLAVE:
                                                    fprintf(flog,"Master Lost Connection with Slave");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Master Lost Connection with Slave");
#endif
                                                    break;
                                                  case SLAVE_LOST_CONNECTION_WITH_MASTER:
                                                    fprintf(flog,"Slave Lost Connection with Master");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Slave Lost Connection with Master");
#endif
                                                    break;
                                                  case MASTER_HAS_NO_CONNECTION_WITH_SLAVE:
                                                    fprintf(flog,"Master has no Connection with Slave");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Master has no Connection with Slave");
#endif
                                                    break;
                                                  case SLAVE_HAS_NO_CONNECTION_WITH_MASTER:
                                                    fprintf(flog,"Slave has no Connection with Master");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Slave has no Connection with Master");
#endif
                                                    break;
                                                  case MASTER_ESTABLISHED_CONNECTION_WITH_SLAVE:
                                                    fprintf(flog,"Master established Connection with Slave");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Master established Connection with Slave");
#endif
                                                    break;
                                                  case SLAVE_ESTABLISHED_CONNECTION_WITH_MASTER:
                                                    fprintf(flog,"Slave established Connection with Master");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Slave established Connection with Master");
#endif
                                                    break;
                                                  default:
                                                    {
                                                      char buf[255];
                                                      buf[0]=0;
                                                      if(b & POWER0_FAULT){
                                                        if((b&0xff) == 0xff)
                                                          sprintf(buf ,"Power#1 is faulty.");
                                                        else
                                                          sprintf(buf ,"Power#1 is faulty for RSU#:%d",b&0xff);
                                                        break;
                                                      }
                                                      else
                                                      if(b & POWER1_FAULT){
                                                        if((b&0xff) == 0xff)
                                                          sprintf(buf ,"Power#2 is faulty.");
                                                        else
                                                          sprintf(buf ,"Power#2 is faulty for RSU#:%d",b&0xff);
                                                        break;
                                                      }
                                                      else
                                                      if(b & POWER_FAULT_REMOVED){
                                                        if((b&0xff) == 0xff)
                                                          sprintf(buf ,"Power fault is removed.");
                                                        else
                                                          sprintf(buf ,"Power fault is removed for RSU#:%d",b&0xff);
                                                      }
                                                      if(buf[0] != 0){
                                                        fprintf(flog,buf);
#ifdef DISPLAY_ALARM
                                                        show_alarm(buf);
#endif
                                                      }
                                                    }
                                                    break;
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case OOS:
                                                b=a.parm & 0x07ff;
                                                alarm();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                if(b & 0x400){
                                                        b&=0x3ff;
                                                        fprintf(flog,"Digital Trunk for RSU#:%d is not Connected",b);
#ifdef DISPLAY_ALARM
                                                        show_alarm("Digital Trunk for RSU#:%d is not Connected",b);
#endif
                                                }
                                                else{
                                                        fprintf(flog,"Digital Trunk Number %d Has LOS",b+1);
#ifdef DISPLAY_ALARM
                                                        show_alarm("Digital Trunk Number %d Has LOS",b+1);
#endif
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case OOS_REMOVED:
                                                b=a.parm & 0x07ff;
                                                date_to_log();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                if(b & 0x400){
                                                        b&=0x3ff;
                                                        fprintf(flog,"Digital Trunk for RSU#:%d is Connected",b);
#ifdef DISPLAY_ALARM
                                                        show_alarm("Digital Trunk for RSU#:%d is Connected",b);
#endif
                                                }
                                                else{
                                                        fprintf(flog,"Digital Trunk Number %d Has SYNC",b+1);
#ifdef DISPLAY_ALARM
                                                        show_alarm("Digital Trunk Number %d Has SYNC",b+1);
#endif
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case REMOTE_OOS:
                                                b=a.parm & 0x07ff;
                                                alarm();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                if(b & 0x400){
                                                        b&=0x3ff;
                                                        fprintf(flog,"Digital Trunk for RSU#:%d Has Remote Alarm",b);
#ifdef DISPLAY_ALARM
                                                        show_alarm("Digital Trunk for RSU#:%d Has Remote Alarm",b);
#endif
                                                }
                                                else{
                                                        fprintf(flog,"Digital Trunk Number %d Has Remote Alarm",b+1);
#ifdef DISPLAY_ALARM
                                                        show_alarm("Digital Trunk Number %d Has Remote Alarm",b+1);
#endif
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case REMOTE_OOS_REMOVED:
                                                b=a.parm & 0x07ff;
                                                date_to_log();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                if(b & 0x400){
                                                        b&=0x3ff;
                                                        fprintf(flog,"Digital Trunk for RSU#:%d Remote Alarm Removed",b);
#ifdef DISPLAY_ALARM
                                                        show_alarm("Digital Trunk for RSU#:%d Remote Alarm Removed",b);
#endif
                                                }
                                                else{
                                                        fprintf(flog,"Digital Trunk Number %d Remote Alarm Removed",b+1);
#ifdef DISPLAY_ALARM
                                                        show_alarm("Digital Trunk Number %d Remote Alarm Removed",b+1);
#endif
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case LINE_CARD_FAULT:
                                        case LINE_CARD_FAULT|0x0800:
                                                b=a.parm & 0x0fff;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                                                if(a.sys==2)
                                                        b+= (((NO_EXTENSION_A+NO_EXTENSION_B) - 1) >> 4) + 1;
                                                else
#endif
                                                if(a.sys==1)
                                                        b+= ((NO_EXTENSION_A - 1) >> 4) + 1;
#endif
                                                alarm();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
#ifdef RMT_EXT
                                                if((b & 0x0fc0) != 0x0fc0){
                                                  fprintf(flog,"Line Card Number %d / P.U. Number %d Is Faulty/Out of Slot",(b&0x3f)+1,(b>>6)+1);
#ifdef DISPLAY_ALARM
                                                  show_alarm("Line Card Number %d / P.U. Number %d Is Faulty/Out of Slot",(b&0x3f)+1,(b>>6)+1);
#endif
                                                }
                                                else
#endif
                                                {
                                                  fprintf(flog,"Line Card Number %d Is Faulty/Out of Slot",(b&0x3f)+1);
#ifdef DISPLAY_ALARM
                                                  show_alarm("Line Card Number %d Is Faulty/Out of Slot",(b&0x3f)+1);
#endif
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case LINE_CARD_FAULT_REMOVED:
                                        case LINE_CARD_FAULT_REMOVED|0x0800:
                                                b=a.parm & 0x0fff;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                                                if(a.sys==2)
                                                        b+= (((NO_EXTENSION_A+NO_EXTENSION_B) - 1) >> 4) + 1;
                                                else
#endif
                                                if(a.sys==1)
                                                        b+= ((NO_EXTENSION_A - 1) >> 4) + 1;
#endif
                                                date_to_log();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
#ifdef RMT_EXT
                                                if((b & 0x0fc0) != 0x0fc0){
                                                  fprintf(flog,"Fault Removed From Line Card Number %d / P.U. Number %d",(b&0x3f)+1,(b>>6)+1);
#ifdef DISPLAY_ALARM
                                                  show_alarm("Fault Removed From Line Card Number %d / P.U. Number %d",(b&0x3f)+1,(b>>6)+1);
#endif
                                                }
                                                else
#endif
                                                {
                                                  fprintf(flog,"Fault Removed From Line Card Number %d",(b&0x3f)+1);
#ifdef DISPLAY_ALARM
                                                  show_alarm("Fault Removed From Line Card Number %d",(b&0x3f)+1);
#endif
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case OM_SYNC_LOSS:
                                                b=a.parm & 0x07ff;
                                                alarm();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                fprintf(flog,"Shelf Number %d Has Lost Connection with O&M Number %d",a.sys+1,b+1);
#ifdef DISPLAY_ALARM
                                                show_alarm("Shelf Number %d Has Lost Connection with O&M Number %d",a.sys+1,b+1);
#endif
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;


#ifdef EM_INSTALLED
                                        case EM_FAULT:
                                          b=a.parm & 0xfc00;
                                          switch(b){
                                            case EM_FAULT:
                                              b=a.parm & 0x03ff;
                                              alarm();
#ifdef NEW_DEP
                                              fgetpos(flog,&pos);
                                              fseek(flog,0,SEEK_END);
#endif
                                              fprintf(flog,"Periodic Test Result: E&M Channel Number %d Is Faulty",b+1);
#ifdef DISPLAY_ALARM
                                              show_alarm("Periodic Test Result: E&M Channel Number %d Is Faulty",b+1);
#endif
#ifdef NEW_DEP
                                              fsetpos(flog,&pos);
#endif
                                              break;
                                            case EM_FAULT_REMOVED:
                                              b=a.parm & 0x03ff;
                                              date_to_log();
#ifdef NEW_DEP
                                              fgetpos(flog,&pos);
                                              fseek(flog,0,SEEK_END);
#endif
                                              fprintf(flog,"Periodic Test Result: Fault Removed from E&M Channel Number %d",b+1);
#ifdef DISPLAY_ALARM
                                              show_alarm("Periodic Test Result: Fault Removed from E&M Channel Number %d",b+1);
#endif
#ifdef NEW_DEP
                                              fsetpos(flog,&pos);
#endif
                                              break;
                                          }
                                          break;
#endif
#ifdef LE_V52_INSTALLED
                                        case V52_FAULT:
                                          b=a.parm & 0xfc00;
                                          switch(b){
                                            case V52_FAULT:
                                              b=a.parm & 0x03ff;
                                              alarm();
#ifdef NEW_DEP
                                              fgetpos(flog,&pos);
                                              fseek(flog,0,SEEK_END);
#endif
                                              fprintf(flog,"V5 Interface Number %d is Out of Service.",b+1);
#ifdef DISPLAY_ALARM
                                              show_alarm("V5 Interface Number %d is Out of Service.",b+1);
#endif
#ifdef NEW_DEP
                                              fsetpos(flog,&pos);
#endif
                                              break;
                                            case V52_FAULT_REMOVED:
                                              b=a.parm & 0x03ff;
                                              date_to_log();
#ifdef NEW_DEP
                                              fgetpos(flog,&pos);
                                              fseek(flog,0,SEEK_END);
#endif
                                              fprintf(flog,"V5 Interface Number %d is In Service.",b+1);
#ifdef DISPLAY_ALARM
                                              show_alarm("V5 Interface Number %d is In Service.",b+1);
#endif
#ifdef NEW_DEP
                                              fsetpos(flog,&pos);
#endif
                                              break;
                                          }
                                          break;
#endif
#ifdef AN_V52_INSTALLED
                                        case V52_FAULT:
                                          b=a.parm & 0xfc00;
                                          switch(b){
                                            case V52_FAULT:
                                              //b=a.parm & 0x03ff;
                                              alarm();
#ifdef NEW_DEP
                                              fgetpos(flog,&pos);
                                              fseek(flog,0,SEEK_END);
#endif
                                              fprintf(flog,"V5 Interface is Out of Service.");
#ifdef DISPLAY_ALARM
                                              show_alarm("V5 Interface Number is Out of Service.");
#endif
#ifdef NEW_DEP
                                              fsetpos(flog,&pos);
#endif
                                              break;
                                            case V52_FAULT_REMOVED:
                                              //b=a.parm & 0x03ff;
                                              date_to_log();
#ifdef NEW_DEP
                                              fgetpos(flog,&pos);
                                              fseek(flog,0,SEEK_END);
#endif
                                              fprintf(flog,"V5 Interface is In Service.");
#ifdef DISPLAY_ALARM
                                              show_alarm("V5 Interface is In Service.");
#endif
#ifdef NEW_DEP
                                              fsetpos(flog,&pos);
#endif
                                              break;
                                          }
                                          break;
#endif
#if (NO_CB != 0)
                                        case CB_FAULT:
                                                b=a.parm & 0x07ff;
                                                alarm();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                fprintf(flog,"Periodic Test Result: Coin Box Number %d Is Faulty",b+1);
#ifdef DISPLAY_ALARM
                                                show_alarm("Periodic Test Result: Coin Box Number %d Is Faulty",b+1);
#endif
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case CB_FAULT_REMOVED:
                                                b=a.parm & 0x07ff;
                                                date_to_log();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                fprintf(flog,"Periodic Test Result: Fault Removed from Coin Box Number %d",b+1);
#ifdef DISPLAY_ALARM
                                                show_alarm("Periodic Test Result: Fault Removed from Coin Box Number %d",b+1);
#endif
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
#endif // (NO_CB != 0)
                                        case BILLING_RECORD_PROBLEM:
                                            if((a.parm & 0xfc00) == ALARM_PANEL_REPORT){
                                                b=a.parm & 0x03ff;
                                                alarm();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                if(b == 0){
                                                    fprintf(flog,"Connection with alarm panel established.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Connection with alarm panel established.");
#endif
                                                }
                                                else if(b == 1){
                                                    fprintf(flog,"Connection with alarm panel lost.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Connection with alarm panel lost.");
#endif
                                                }
                                                else if(b == 2){
                                                    fprintf(flog,"Smoke detector alarm removed.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Smoke detector alarm removed.");
#endif
                                                }
                                                else if(b == 3){
                                                    fprintf(flog,"Smoke detector alarm detected.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Smoke detector alarm detected.");
#endif
                                                }
                                                else if(b == 4){
                                                    fprintf(flog,"220 voltage loss alarm removed.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("220 voltage loss alarm removed.");
#endif
                                                }
                                                else if(b == 5){
                                                    fprintf(flog,"220 voltage loss alarm detected.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("220 voltage loss alarm detected.");
#endif
                                                }
                                                else if(b == 6){
                                                    fprintf(flog,"Door open alarm removed.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Door open alarm removed.");
#endif
                                                }
                                                else if(b == 7){
                                                    fprintf(flog,"Door open alarm detected.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Door open alarm detected.");
#endif
                                                }
                                                else if(b == 8){
                                                    fprintf(flog,"Temperature high alarm removed.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Temperature high alarm removed.");
#endif
                                                }
                                                else if(b == 9){
                                                    fprintf(flog,"Temperature high alarm detected.");
#ifdef DISPLAY_ALARM
                                                    show_alarm("Temperature high alarm detected.");
#endif
                                                }
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                            }
                                            else{
                                                b=a.parm & 0x07ff;
                                                alarm();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                fprintf(flog,"Shelf Number %d Has Billing Problem Code %d",a.sys+1,b+1);
#ifdef DISPLAY_ALARM
                                                show_alarm("Shelf Number %d Has Billing Problem Code %d",a.sys+1,b+1);
#endif
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                            }
                                            break;

                                        case MTP_L2_FAULT:
                                          b=a.parm & 0xfc00;
                                          switch(b){
                                            case MTP_L2_FAULT:
                                              b=a.parm & 0x03ff;
                                              alarm();
#ifdef NEW_DEP
                                              fgetpos(flog,&pos);
                                              fseek(flog,0,SEEK_END);
#endif
                                              fprintf(flog,"MTP Channel Number %d Has Been Stopped.",b+1);
#ifdef DISPLAY_ALARM
                                              show_alarm("MTP Channel Number %d Has Been Stopped.",b+1);
#endif
#ifdef NEW_DEP
                                              fsetpos(flog,&pos);
#endif
                                              break;
                                            case MTP_L2_FAULT_REMOVED:
                                              b=a.parm & 0x03ff;
                                              date_to_log();
#ifdef NEW_DEP
                                              fgetpos(flog,&pos);
                                              fseek(flog,0,SEEK_END);
#endif
                                              fprintf(flog,"MTP Channel Number %d Has Been Started.",b+1);
#ifdef DISPLAY_ALARM
                                              show_alarm("MTP Channel Number %d Has Been Started.",b+1);
#endif
#ifdef NEW_DEP
                                              fsetpos(flog,&pos);
#endif
                                              break;
                                          }
                                          break;
                                        default: break;
                                }
                                break;
subscriber:
                                b=a.parm & 0xc000;
                                switch(b){
                                        case SUBSCRIBER_FAULT:
                                                b=a.parm & 0x3fff;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                                                if(a.sys==2)
                                                        b+= (NO_EXTENSION_A+NO_EXTENSION_B);
                                                else
#endif
                                                if(a.sys==1)
                                                        b+= NO_EXTENSION_A;
#endif
                                                alarm();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                fprintf(flog,"Periodic Test Result: Subscriber Number "SDNS_FS" Is Faulty",find_subscriber_no(b));
#ifdef DISPLAY_ALARM
                                                show_alarm("Periodic Test Result: Subscriber Number "SDNS_FS" Is Faulty",find_subscriber_no(b));
#endif
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        case SUBSCRIBER_FAULT_REMOVED:
                                                b=a.parm & 0x3fff;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                                                if(a.sys==2)
                                                        b+= (NO_EXTENSION_A+NO_EXTENSION_B);
                                                else
#endif
                                                if(a.sys==1)
                                                        b+= NO_EXTENSION_A;
#endif
                                                date_to_log();
#ifdef NEW_DEP
                                                fgetpos(flog,&pos);
                                                fseek(flog,0,SEEK_END);
#endif
                                                fprintf(flog,"Periodic Test Result: Fault Removed from Subscriber Number "SDNS_FS,find_subscriber_no(b));
#ifdef DISPLAY_ALARM
                                                show_alarm("Periodic Test Result: Fault Removed from Subscriber Number "SDNS_FS,find_subscriber_no(b));
#endif
#ifdef NEW_DEP
                                                fsetpos(flog,&pos);
#endif
                                                break;
                                        default: break;

                                }
                                break;
#endif
#endif

#ifdef SYSTEM_MANUAL_TEST
                        case (MA_SYS_P | MAC_EXT_MANUAL_TEST_ANS):
                                c=a.parm & 0xf000;
                                b=a.parm & 0x0fff;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
                                if(a.sys==2)
                                        b+= (NO_EXTENSION_A+NO_EXTENSION_B);
                                else
#endif
                                if(a.sys==1)
                                        b+= NO_EXTENSION_A;
#endif

                                switch(c){
#if 0
                                        case TEST_FAIL:
                                                poke_s(69,6,"FAILED",RED<<8);
                                                break;
                                        case TEST_OK:
                                                poke_s(69,6,"PASSED",WHITE<<8);
                                                break;
                                        case TEST_BUSY:
                                                poke_s(69,6,"BUSY  ",YELLOW<<8);
                                                break;
                                        default:
                                                poke_s(69,6,"ERROR",RED<<8);
                                                break;
#endif // 0
                                        case TEST_FAIL:
                                                poke_s(69,6,"FAILED",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                                                break;
                                        case TEST_OK:
                                                poke_s(69,6,"PASSED",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                                                break;
                                        case TEST_BUSY:
                                                poke_s(69,6,"BUSY  ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW));
                                                break;
                                        default:
                                                poke_s(69,6,"ERROR",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                                                break;
                                }

                                break;
#endif

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

                        case (MA_SYS_P | MAC_CAS3_HISTORICAL_COUNT_REPORT):
                                fprintf(fhst,"\n   %5u",a.parm); /* count */
                                break;
                        case (MA_SYS_P | MAC_CAS3_HISTORICAL_SIGNAL_REPORT):
                                cas3_hst_sgnl_process(a.parm);
                                break;
#endif
#endif
#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

                        case (MA_SYS_P | MAC_EM_HISTORICAL_COUNT_REPORT):
                                fprintf(fhst,"\n   %5u",a.parm); /* count */
                                break;
                        case (MA_SYS_P | MAC_EM_HISTORICAL_SIGNAL_REPORT):
                                em_hst_sgnl_process(a.parm);
                                break;
#endif
#endif
#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

                        case (MA_SYS_P | MAC_CAS1_HISTORICAL_COUNT_REPORT):
                                fprintf(fhst,"\n   %5u",a.parm); /* count */
                                break;
                        case (MA_SYS_P | MAC_CAS1_HISTORICAL_SIGNAL_REPORT):
                                cas1_hst_sgnl_process(a.parm);
                                break;
#endif
#endif

#if (NO_OPRTR > 1)
                        case (MA_SYS_P | MAC_UPLOAD_REJECT):
                                if( (ocr.state == AWAIT_SYS_UPLOAD_ACCEPTANCE_1)
                                  ||(ocr.state == AWAIT_SYS_UPLOAD_ACCEPTANCE_2)
                                  ||(ocr.state == AWAIT_SYS_UPLOAD_ACCEPTANCE_3)
                                  ){
                                        p.t=OPR_T;
#ifdef HOT_STANDBY
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                                        if (a.htby == LAPB_LINK0)
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                                        if (a.htby == tcp_link_0)
#endif
#ifdef MY_TCPIP_FOR_OAM
                                        if (a.htby == 0)
#endif
                                        {
                                                rtn131_sys1(p);
                                        }
                                        else{
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                                                if (a.htby == LAPB_LINK1)
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                                                if (a.htby == tcp_link_1)
#endif
#ifdef MY_TCPIP_FOR_OAM
                                                if (a.htby == 1)
#endif

                                                {
                                                        rtn131_sys2(p);
                                                }
                                        }
#else
                                        rtn131(p);
#endif
                                }
                                break;

#endif

#if (NO_OPRTR > 1)
                        case (MA_SYS_P | MAC_UPLOAD_ACCEPT):
                                p.t=OPR_T;
                                if (ocr.state == AWAIT_SYS_UPLOAD_ACCEPTANCE_1){
#ifdef HOT_STANDBY
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                                        if (a.htby == LAPB_LINK0)
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                                        if (a.htby == tcp_link_0)
#endif
#ifdef MY_TCPIP_FOR_OAM
                                        if (a.htby == 0)
#endif
                                        {
                                                rtn130_1_sys1(p);
                                        }
                                        else{
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                                                if (a.htby == LAPB_LINK1)
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                                                if (a.htby == tcp_link_1)
#endif
#ifdef MY_TCPIP_FOR_OAM
                                                if (a.htby == 1)
#endif

                                                {
                                                        rtn130_1_sys2(p);
                                                }
                                        }
#else
                                        rtn130_1(p);
#endif

                                }
                                else
                                if (ocr.state == AWAIT_SYS_UPLOAD_ACCEPTANCE_2)
                                        rtn130_2(p);
                                else
                                if (ocr.state == AWAIT_SYS_UPLOAD_ACCEPTANCE_3)
                                        rtn130_3(p);
                                break;

#endif
#ifdef DEP_MANUAL_TEST
                        case (MA_SYS_P | MAC_EXT_STATE_REQUEST_ANS):
                                if(subscriber_state_block_ptr< SUBSCRIBER_STATE_BLOCK_SIZE)
                                        subscriber_state_block[subscriber_state_block_ptr++]=a.parm;

                                break;
                        case (MA_SYS_P | MAC_R1_TEST_REQUEST_ANS):
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
                                        switch(a.parm){
                                                case DIGIT1:
                                                        wprintf(saved_win2,"1");
                                                        break;
                                                case DIGIT2:
                                                        wprintf(saved_win2,"2");
                                                        break;
                                                case DIGIT3:
                                                        wprintf(saved_win2,"3");
                                                        break;
                                                case DIGIT4:
                                                        wprintf(saved_win2,"4");
                                                        break;
                                                case DIGIT5:
                                                        wprintf(saved_win2,"5");
                                                        break;
                                                case DIGIT6:
                                                        wprintf(saved_win2,"6");
                                                        break;
                                                case DIGIT7:
                                                        wprintf(saved_win2,"7");
                                                        break;
                                                case DIGIT8:
                                                        wprintf(saved_win2,"8");
                                                        break;
                                                case DIGIT9:
                                                        wprintf(saved_win2,"9");
                                                        break;
                                                case DIGIT10:
                                                        wprintf(saved_win2,"0");
                                                        break;
                                                case DIGITF:
                                                        wprintf(saved_win2,"F");
                                                        break;
                                                case KP:
                                                        wprintf(saved_win2,"K");
                                                        break;
                                                case ST:
                                                        wprintf(saved_win2,"S");
                                                        break;
                                                default:
                                                        wprintf(saved_win2,"X");
                                                        break;
                                        }
#endif
                                break;
                        case (MA_SYS_P | MAC_EXT_MANUAL_TEST_ANS):
                                if ((tst.prty_type == TEST_XST_T) && (tst.prty_indx == (a.parm&0x0fff)) )
                                        tst.result=a.parm&0xf000;
                                break;
#ifdef EM_INSTALLED
                        case (MA_SYS_P | MAC_EM_MANUAL_TEST_ANS):
                                if ((tst.prty_type == TEST_EM_T) && (tst.prty_indx == (a.parm&0x0fff)) )
                                        tst.result=a.parm&0xf000;
                                break;
#endif
#ifdef DTMF
                        case (MA_SYS_P | MAC_DTMF_MANUAL_TEST_ANS):
                                if ((tst.prty_type == TEST_DTMF_T) && (tst.prty_indx == (a.parm&0x0fff)) )
                                        tst.result=a.parm&0xf000;
                                break;
#endif
//DYNAMIC : #if ((NO_CAS3 != 0) || (NO_CAS1 != 0)) -->
#if ((defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED))
                        case (MA_SYS_P | MAC_DIGITAL_TRUNK_MANUAL_TEST_ANS):
                                if ((tst.prty_type == TEST_DGTL_TRNK_T) && (tst.prty_indx == (a.parm&0x0fff)) )
                                        tst.result=a.parm&0xf000;
                                break;
#endif
                        case (MA_SYS_P | MAC_SWCH_MANUAL_TEST_ANS):
                                if ((tst.prty_type == TEST_SWCH_T) && (tst.prty_indx == (a.parm&0x0fff)) )
                                        tst.result=a.parm&0xf000;
                                break;
#endif //DEP_MANUAL_TEST
                        default: break;
                }
        }
        f &= ~MAF;
}
#endif
#endif


#ifdef OPR_EXIT_PASSWORD
int vox(){  /* verify operator exit */
#ifdef NEW_DEP
        caw();
        return 1;
#else

        WINDOW *a;
        FIELD *c;
        char xpw[19];  /* exit password */
        int b;
        a=enw(30,10,3,20);
        if(a == NULL)
                return 0;
        activate_bell();
        dw(a);
        swb(a,DOUBLE_LINE);
#if 0
        swc(a,ALL,BLUE,YELLOW,BRIGHT);
        swc(a,BORDER,RED,YELLOW,BRIGHT);
        swc(a,TITLE,RED,YELLOW,BRIGHT);
#endif // 0
        swc(a,ALL,KT_BLUE,KT_YELLOW,KT_HIGH_INTENSITY);
        swc(a,BORDER,KT_RED,KT_YELLOW,KT_HIGH_INTENSITY);
        swc(a,TITLE,KT_RED,KT_YELLOW,KT_HIGH_INTENSITY);
        swt(a,"EXIT PASSWORD");
        draw_window(a);
        idew(a);
        c=enf(a,0,0,mask18,xpw,PASSWORD);
        if(c == NULL){
                caw();
                return 0;
        }
        xpw[18]='\0';
        caf(a);
        old_data_entry(a);
        xpw[EXIT_PASSWORD_LENGHT]='\0';
        if(strcmp(EXIT_PASSWORD,xpw) != 0)
                b=0;
        else
                b=1;
        caw();
        return b;

#endif
}
#endif

void cprti(){  /* call processing routine table initialization */
        int i,j;
        for(i=0;i<OPRSIDE_NO_STATES;i++)
                for(j=0;j<OPRSIDE_NO_STIMS;j++)

#ifdef OPERATOR_PC
                        rtn_idx[i][j]=(FPCP)errorcp;
#else
                        rtn_idx[i][j]=0xff;
#endif

#ifdef OPERATOR_PC
        for(i=OPRSIDE_CLID;i<OPRSIDE_CLID+10;i++){
            rtn_idx[IN_INTR_QU][i]=rtn149;
        }
        for(i=OPRSIDE_CLID;i<OPRSIDE_CLID+10;i++){
            rtn_idx[IN_EXTRNL_QU][i]=rtn152;
        }
        for(i=OPRSIDE_CLID;i<OPRSIDE_CLID+10;i++){
            rtn_idx[IN_RCL_QU][i]=rtn152;
        }
        rtn_idx[IDLE][ON_HOOK]=(FPCP)rtn0;
        rtn_idx[IDLE][OPR_REQ]=rtn1;
#ifdef OPR_OPR_TALK
        for(i=0;i<OPRSIDE_NO_STATES;i++){
                if(i == IDLE)
                        continue;
                rtn_idx[i][OPR_REQ]=rtn1_1;
        }
        rtn_idx[OPR_AT_HOLD][OPR_DEST_READY]=rtn1_2;
#endif
        rtn_idx[IDLE][TIME_OUT]=rtn110;
        rtn_idx[IDLE][OPR_REJECT]=(FPCP)rtn0;
#else
        rtn_idx[IDLE][ON_HOOK]=0;
        rtn_idx[IDLE][OPR_REQ]=1;
        rtn_idx[IDLE][TIME_OUT]=110;
        rtn_idx[IDLE][OPR_REJECT]=0;
#endif

#ifdef OPERATOR_PC
        rtn_idx[IN_INTR_QU][ON_HOOK]=rtn2;
#ifdef OPR_OPR_TALK
        for(i=0;i<OPRSIDE_NO_STATES;i++)
                rtn_idx[i][OPR_ON_HOOK]=rtn2_1;
#endif
        rtn_idx[IN_INTR_QU][OFF_HOOK]=rtn103;
#else
        rtn_idx[IN_INTR_QU][ON_HOOK]=2;
        rtn_idx[IN_INTR_QU][OFF_HOOK]=103;
#endif

        for(i=0;i<OPRSIDE_NO_STATES;i++){

#ifdef OPERATOR_PC
                rtn_idx[i][TONE_RNGR]=rtn6;
#else
                rtn_idx[i][TONE_RNGR]=6;
#endif
        }

/*      rtn_idx[OPR_IDLE][VOLUME]=rtn5;
VOLUME = FRC_RLS if volume needed check the confilect btwn them
*/

#ifdef OPERATOR_PC
        rtn_idx[OPR_IDLE][OPR_HOOK]=rtn7;
        rtn_idx[OPR_IDLE][NIGHT]=rtn101;
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
        rtn_idx[NIGHT_UPLOAD][N_0]=rtn101_1;
        rtn_idx[NIGHT_UPLOAD][N_1]=rtn101_2;
#endif
//end night upload
        rtn_idx[OPR_IDLE][DISP_STATUS]=rtn104;

#ifdef DISPLAY_EM_STATUS
        rtn_idx[OPR_IDLE][DISP_TRUNK_STATUS]=rtn104a;
#endif

#ifdef DISPLAY_CAS3_STATUS
        rtn_idx[OPR_IDLE][DISP_CAS3_STATUS]=rtn104c;
#endif

#ifdef DISPLAY_CAS1_STATUS
        //rtn_idx[OPR_IDLE][DISP_CAS1_STATUS]=rtn104d;
#endif
        rtn_idx[OPR_READY][DISP_CAS1_STATUS]=rtn17c;    // F8, ENTER (CR)

#ifdef SYSTEM_MANUAL_TEST
        rtn_idx[OPR_IDLE][MANUAL_TEST]=rtn104b;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_IDLE][i]=rtn97;

        rtn_idx[OPR_IDLE][EXIT_MANUAL_TEST]=rtn105b;
#endif
#ifdef NEW_DEP
      rtn_idx[OPR_IDLE][DEP_ACT]=rtn113;
#endif
      rtn_idx[OPR_IDLE][TERM_ACT]=rtn114;
#ifdef DISPLAY_BILLING
        rtn_idx[OPR_IDLE][BILLING_ACT]=rtn148;
#endif
#ifdef DISPLAY_ALARM
        rtn_idx[OPR_IDLE][ALARM_ACT]=rtn148f;
#endif
        rtn_idx[OPR_IDLE][RLS]=rtn105;
        rtn_idx[OPR_IDLE][ATT_RLS]=rtn105a;
#else
        rtn_idx[OPR_IDLE][OPR_HOOK]=7;
        rtn_idx[OPR_IDLE][NIGHT]=101;
        rtn_idx[OPR_IDLE][DISP_STATUS]=104;
//      rtn_idx[OPR_IDLE][DISP_TRUNK_STATUS]=???;
        rtn_idx[OPR_IDLE][RLS]=105;
//      rtn_idx[OPR_IDLE][ATT_RLS]=???;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_NIGHT][NIGHT]=rtn102;
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
        rtn_idx[DAY_UPLOAD][N_0]=rtn102_1;
        rtn_idx[DAY_UPLOAD][N_1]=rtn102_2;
#endif
//end night upload
#else
        rtn_idx[OPRSIDE_OPR_NIGHT][NIGHT]=102;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][OFF_HOOK]=rtn8;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][RLS]=rtn10;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][OPR_HOOK]=rtn11;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][ON_HOOK]=rtn98;
#ifdef OPR_OPR_TALK
        rtn_idx[OPR_OPR_RINGING][OPR_DEST_READY]=rtn8_1;
        rtn_idx[OPR_OPR_RINGING][RLS]=rtn10_1;
        rtn_idx[OPR_OPR_RINGING][OPR_HOOK]=rtn11_1;
#endif

#else
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][OFF_HOOK]=8;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][RLS]=10;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][OPR_HOOK]=11;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][ON_HOOK]=98;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][OPR_REJECT]=rtn12;
#ifdef LIM
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][EXT_BUSY]=rtn44a;
#endif
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][HOLD1]=rtn13;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][HOLD2]=rtn13;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][HOLD3]=rtn13;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][HOLD4]=rtn13;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][ATT_RLS]=rtn29;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][SRL_TRNSFR]=rtn111;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][FRC_RLS]=rtn111a;
#ifdef OPR_OPR_TALK
        rtn_idx[OPR_OPR_RINGING][OPR_REJECT]=rtn12_1;
        rtn_idx[OPRSIDE_OPR_TALK][SRC_CNNCTED]=rtn17_2;
        rtn_idx[OPR_OPR_RINGING][ATT_RLS]=rtn29;
#endif

#else
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][OPR_REJECT]=12;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][HOLD1]=13;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][HOLD2]=13;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][HOLD3]=13;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][HOLD4]=13;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][ATT_RLS]=29;
        rtn_idx[OPRSIDE_OPR_EXT_RINGING][SRL_TRNSFR]=111;
#endif


#ifdef OPERATOR_PC
        rtn_idx[IDLE][EXT_FORWARDED]=rtn14;
        rtn_idx[IDLE][OPR_REJECT]=rtn15;
        rtn_idx[IDLE][EXT_BUSY]=rtn16;
        rtn_idx[IDLE][AT_RING]=rtn103;
        rtn_idx[IDLE][AT_LOCK]=rtn103;
        rtn_idx[IDLE][OFF_HOOK]=rtn103;
        rtn_idx[IDLE][ON_HOOK]=rtn103;
        rtn_idx[IDLE][EXT_PICKED_UP]=rtn95;
#else
        rtn_idx[IDLE][EXT_FORWARDED]=14;
        rtn_idx[IDLE][OPR_REJECT]=15;
        rtn_idx[IDLE][EXT_BUSY]=16;
        rtn_idx[IDLE][AT_RING]=103;
        rtn_idx[IDLE][OFF_HOOK]=103;
        rtn_idx[IDLE][ON_HOOK]=103;
        rtn_idx[IDLE][EXT_PICKED_UP]=95;
#endif

#ifdef OPERATOR_PC
        //rtn_idx[OPR_READY][INTERNAL]=rtn17;
        rtn_idx[OPR_READY][INTERNAL]=rtn17a;
        rtn_idx[OPR_READY][SRC]=rtn18;
        rtn_idx[OPR_READY][OPR_HOOK]=rtn19;
#else
        rtn_idx[OPR_READY][INTERNAL]=17;
        rtn_idx[OPR_READY][SRC]=18;
        rtn_idx[OPR_READY][OPR_HOOK]=19;
#endif

#ifdef OPERATOR_PC
        //rtn_idx[OPR_READY][EXTRNL]=rtn20;
        rtn_idx[OPR_READY][EXTRNL]=rtn20a;
#ifdef EM_CONT_SEP_OPR_Q
        rtn_idx[OPR_READY][EM_Q]=rtn139;
#endif
        rtn_idx[OPR_READY][RECALL]=rtn21;
        rtn_idx[OPR_READY][PARK_XT]=rtn60;
#else
        rtn_idx[OPR_READY][EXTRNL]=20;
#ifdef EM_CONT_SEP_OPR_Q
        rtn_idx[OPR_READY][EM_Q]=139;
#endif
        rtn_idx[OPR_READY][RECALL]=21;
        rtn_idx[OPR_READY][PARK_XT]=60;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPR_READY][HOLD1]=rtn22;
        rtn_idx[OPR_READY][HOLD2]=rtn22;
        rtn_idx[OPR_READY][HOLD3]=rtn22;
        rtn_idx[OPR_READY][HOLD4]=rtn22;
        rtn_idx[OPR_READY][CO_GRP1]=rtn23;
        rtn_idx[OPR_READY][CO_GRP2]=rtn23;
        rtn_idx[OPR_READY][CO_GRP3]=rtn23;
        rtn_idx[OPR_READY][CO_GRP4]=rtn23;
        rtn_idx[OPR_READY][CO_GRP5]=rtn23;
        rtn_idx[OPR_READY][CO_GRP6]=rtn23;
        rtn_idx[OPR_READY][TIE_GRP1]=rtn108;
        rtn_idx[OPR_READY][TIE_GRP2]=rtn108;
        rtn_idx[OPR_READY][TIE_GRP3]=rtn108;
        rtn_idx[OPR_READY][TIE_GRP4]=rtn108;
        rtn_idx[OPR_READY][TIE_GRP5]=rtn108;
        rtn_idx[OPR_READY][TIE_GRP6]=rtn108;

#ifdef TRUNK_TRUNK_CONNECTION
        rtn_idx[OPR_READY][CO_TRUNK_RLS]=rtn115;
        rtn_idx[OPR_READY][TIE_TRUNK_RLS]=rtn116;
#endif

#else
        rtn_idx[OPR_READY][HOLD1]=22;
        rtn_idx[OPR_READY][HOLD2]=22;
        rtn_idx[OPR_READY][HOLD3]=22;
        rtn_idx[OPR_READY][HOLD4]=22;
        rtn_idx[OPR_READY][CO_GRP1]=23;
        rtn_idx[OPR_READY][CO_GRP2]=23;
        rtn_idx[OPR_READY][CO_GRP3]=23;
        rtn_idx[OPR_READY][CO_GRP4]=23;
        rtn_idx[OPR_READY][CO_GRP5]=23;
        rtn_idx[OPR_READY][CO_GRP6]=23;
        rtn_idx[OPR_READY][TIE_GRP1]=108;
        rtn_idx[OPR_READY][TIE_GRP2]=108;
        rtn_idx[OPR_READY][TIE_GRP3]=108;
        rtn_idx[OPR_READY][TIE_GRP4]=108;
        rtn_idx[OPR_READY][TIE_GRP5]=108;
        rtn_idx[OPR_READY][TIE_GRP6]=108;

#ifdef TRUNK_TRUNK_CONNECTION
        rtn_idx[OPR_READY][CO_TRUNK_RLS]=115;
        rtn_idx[OPR_READY][TIE_TRUNK_RLS]=116;
#endif

#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_TALK][ON_HOOK]=rtn24;
#ifdef OPR_OPR_TALK
        rtn_idx[OPRSIDE_OPR_TALK][OPR_ON_HOOK]=rtn24_1;
        rtn_idx[OPRSIDE_OPR_TALK][OPR_HOLD]=rtn24_2;
        rtn_idx[OPR_AT_HOLD][RLS]=rtn25_1;
        rtn_idx[OPR_AT_HOLD][OPR_HOOK]=rtn26;
#endif
        rtn_idx[OPRSIDE_OPR_TALK][RLS]=rtn25;
        rtn_idx[OPRSIDE_OPR_TALK][OPR_HOOK]=rtn26;
#else
        rtn_idx[OPRSIDE_OPR_TALK][ON_HOOK]=24;
        rtn_idx[OPRSIDE_OPR_TALK][RLS]=25;
        rtn_idx[OPRSIDE_OPR_TALK][OPR_HOOK]=26;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_TALK][DEST]=rtn27;
        rtn_idx[OPRSIDE_OPR_TALK][SRC]=rtn28;
        rtn_idx[OPRSIDE_OPR_TALK][PARK_XT]=rtn61;
#else
        rtn_idx[OPRSIDE_OPR_TALK][DEST]=27;
        rtn_idx[OPRSIDE_OPR_TALK][SRC]=28;
        rtn_idx[OPRSIDE_OPR_TALK][PARK_XT]=61;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_TALK][ATT_RLS]=rtn29;
        rtn_idx[OPRSIDE_OPR_TALK][OPR_REJECT]=rtn30;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPRSIDE_OPR_TALK][i]=rtn31;
#else
        rtn_idx[OPRSIDE_OPR_TALK][ATT_RLS]=29;
        rtn_idx[OPRSIDE_OPR_TALK][OPR_REJECT]=30;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPRSIDE_OPR_TALK][i]=31;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_TALK][SRL_TRNSFR]=rtn111;
        rtn_idx[OPRSIDE_OPR_TALK][FRC_RLS]=rtn111a;
        rtn_idx[OPRSIDE_OPR_TALK][RECALL]=rtn91;
#else
        rtn_idx[OPRSIDE_OPR_TALK][SRL_TRNSFR]=111;
        rtn_idx[OPRSIDE_OPR_TALK][RECALL]=91;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_TALK][HOLD1]=rtn32;
        rtn_idx[OPRSIDE_OPR_TALK][HOLD2]=rtn32;
        rtn_idx[OPRSIDE_OPR_TALK][HOLD3]=rtn32;
        rtn_idx[OPRSIDE_OPR_TALK][HOLD4]=rtn32;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP1]=rtn33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP2]=rtn33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP3]=rtn33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP4]=rtn33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP5]=rtn33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP6]=rtn33;
        rtn_idx[OPRSIDE_OPR_TALK][OFF_HOOK]=rtn8;

#ifdef DISPLAY_STATUS_ACT
        rtn_idx[OPRSIDE_OPR_TALK][AT_RING]=rtn106;
#endif
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP1]=rtn109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP2]=rtn109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP3]=rtn109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP4]=rtn109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP5]=rtn109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP6]=rtn109;
#else
        rtn_idx[OPRSIDE_OPR_TALK][HOLD1]=32;
        rtn_idx[OPRSIDE_OPR_TALK][HOLD2]=32;
        rtn_idx[OPRSIDE_OPR_TALK][HOLD3]=32;
        rtn_idx[OPRSIDE_OPR_TALK][HOLD4]=32;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP1]=33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP2]=33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP3]=33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP4]=33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP5]=33;
        rtn_idx[OPRSIDE_OPR_TALK][CO_GRP6]=33;
        rtn_idx[OPRSIDE_OPR_TALK][OFF_HOOK]=8;

#ifdef DISPLAY_STATUS_ACT
        rtn_idx[OPRSIDE_OPR_TALK][AT_RING]=106;
#endif
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP1]=109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP2]=109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP3]=109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP4]=109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP5]=109;
        rtn_idx[OPRSIDE_OPR_TALK][TIE_GRP6]=109;
#endif

#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_START][i]=rtn35;
        rtn_idx[OPR_START][RLS]=rtn35;
//        rtn_idx[OPR_START][INTRNL_1D_STIM]=rtn34;
#ifdef X_RAILWAY
        rtn_idx[OPR_START][N_9]=rtn34a;
#endif
#if ((NO_CO != 0) && (CO_REQ <= 0x0a))
        rtn_idx[OPR_START][CO_REQ+N_1-1]=rtn34b;
#endif
//#ifdef INTRNL_1ST_DGT_2
//        rtn_idx[OPR_START][INTRNL_2D_STIM]=rtn34;
//#endif

#ifdef OPR_OPR_TALK
        rtn_idx[OPR_START][OPRTR_NUM]=rtn34_1;
#endif
        rtn_idx[OPR_START][OPR_HOOK]=rtn36;

#ifdef DISPLAY_STATUS_ACT
        rtn_idx[OPR_START][AT_RING]=rtn107;
#endif

#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_START][i]=35;
        rtn_idx[OPR_START][RLS]=35;
        rtn_idx[OPR_START][INTRNL_1D_STIM]=34;
        rtn_idx[OPR_START][OPR_HOOK]=36;

#ifdef DISPLAY_STATUS_ACT
        rtn_idx[OPR_START][AT_RING]=107;
#endif

#endif

#ifdef OPERATOR_PC
        rtn_idx[IDLE][RECALL]=rtn85;
        rtn_idx[IDLE][RING_DTCT]=rtn37;
        rtn_idx[IDLE][TRNK_FREE]=rtn38;
#else
        rtn_idx[IDLE][RECALL]=85;
        rtn_idx[IDLE][RING_DTCT]=37;
        rtn_idx[IDLE][TRNK_FREE]=38;
#endif

#ifdef OPERATOR_PC
        rtn_idx[IN_EXTRNL_QU][RING_DISCNT]=rtn39;
        rtn_idx[IN_EXTRNL_QU][RING_DTCT]=(FPCP)rtn96;
#else
        rtn_idx[IN_EXTRNL_QU][RING_DISCNT]=39;
        rtn_idx[IN_EXTRNL_QU][RING_DTCT]=96;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPR_DIAL_INTRNL][OPR_HOOK]=rtn40;
        rtn_idx[OPR_DIAL_INTRNL][RLS]=rtn35;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_DIAL_INTRNL][i]=rtn41;

#ifdef OPR_OPR_TALK
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_DIAL_OPR][i]=rtn41_1;
#endif

#ifdef X_RAILWAY
        rtn_idx[OPR_DIAL_EM][OPR_HOOK]=rtn40;
        rtn_idx[OPR_DIAL_EM][RLS]=rtn35;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_DIAL_EM][i]=rtn41a;
#endif

#if ((NO_CO != 0) && (CO_REQ <= 0x0a))
        rtn_idx[OPR_DIAL_CO][OPR_HOOK]=rtn40;
        rtn_idx[OPR_DIAL_CO][RLS]=rtn35;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_DIAL_CO][i]=rtn41b;
#endif

#ifdef DISPLAY_STATUS_ACT
        rtn_idx[OPR_DIAL_INTRNL][AT_RING]=rtn107;
#endif

#else
        rtn_idx[OPR_DIAL_INTRNL][OPR_HOOK]=40;
        rtn_idx[OPR_DIAL_INTRNL][RLS]=35;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_DIAL_INTRNL][i]=41;

#ifdef OPR_MALICIOUS_DTCT_ACT
        rtn_idx[OPR_DIAL_INTRNL][AT_RING]=107;
#endif

#endif

#ifdef OPERATOR_PC
        rtn_idx[AWAIT_EXT][EXT_FREE]=rtn42;
        rtn_idx[AWAIT_EXT][OPR_REJECT]=rtn43;
        rtn_idx[AWAIT_EXT][EXT_BUSY]=rtn44;
#else
        rtn_idx[AWAIT_EXT][EXT_FREE]=42;
        rtn_idx[AWAIT_EXT][OPR_REJECT]=43;
        rtn_idx[AWAIT_EXT][EXT_BUSY]=44;
#endif

#ifdef OPERATOR_PC
        rtn_idx[IN_RCL_QU][TIME_OUT]=rtn100;
        rtn_idx[IN_RCL_QU][RECALL]=(FPCP)rtn0;
#else
        rtn_idx[IN_RCL_QU][TIME_OUT]=100;
        rtn_idx[IN_RCL_QU][RECALL]=0;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPR_EXT_BUSY][RLS]=rtn45;
        rtn_idx[OPR_EXT_BUSY][OPR_HOOK]=rtn46;
        rtn_idx[OPR_EXT_BUSY][OPRSIDE_OVRD]=rtn47;
        rtn_idx[OPR_EXT_BUSY][FRC_RLS]=rtn92;
#else
        rtn_idx[OPR_EXT_BUSY][RLS]=45;
        rtn_idx[OPR_EXT_BUSY][OPR_HOOK]=46;
        rtn_idx[OPR_EXT_BUSY][OPRSIDE_OVRD]=47;
        rtn_idx[OPR_EXT_BUSY][FRC_RLS]=92;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPR_EXT_BUSY][OPR_REJECT]=rtn48;
        rtn_idx[OPR_EXT_BUSY][CNNCT_TO_OPR]=rtn49;
#else
        rtn_idx[OPR_EXT_BUSY][OPR_REJECT]=48;
        rtn_idx[OPR_EXT_BUSY][CNNCT_TO_OPR]=49;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPR_EXT_OVRD][RLS]=rtn50;
        rtn_idx[OPR_EXT_OVRD][OPR_HOOK]=rtn51;
        rtn_idx[OPR_EXT_OVRD][ATT_RLS]=rtn94;
#else
        rtn_idx[OPR_EXT_OVRD][RLS]=50;
        rtn_idx[OPR_EXT_OVRD][OPR_HOOK]=51;
        rtn_idx[OPR_EXT_OVRD][ATT_RLS]=94;
#endif

#ifdef OPERATOR_PC
        rtn_idx[HOLD][ON_HOOK]=rtn52;
        rtn_idx[HOLD][RECALL]=rtn53;
        rtn_idx[HOLD][TIME_OUT]=rtn54;
        rtn_idx[HOLD][AT_RING]=rtn103;
        rtn_idx[HOLD][OFF_HOOK]=rtn103;

#if( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
        rtn_idx[HOLD][EM_OPEN]=rtn135;
        rtn_idx[HOLD][OPR_REJECT]=rtn135;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        rtn_idx[HOLD][DT_CLF_TO_OPR]=rtn135;
        rtn_idx[HOLD][OPR_REJECT]=rtn135;
#endif

#else
        rtn_idx[HOLD][ON_HOOK]=52;
        rtn_idx[HOLD][RECALL]=53;
        rtn_idx[HOLD][TIME_OUT]=54;
        rtn_idx[HOLD][AT_RING]=103;
        rtn_idx[HOLD][OFF_HOOK]=103;

#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
        rtn_idx[HOLD][EM_OPEN]=135;
        rtn_idx[HOLD][OPR_REJECT]=135;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        rtn_idx[HOLD][DT_CLF_TO_OPR]=135;
        rtn_idx[HOLD][OPR_REJECT]=135;
#endif

#endif

#ifdef OPERATOR_PC
        rtn_idx[AWAIT_TRNK][OPR_REJECT]=rtn55;
        rtn_idx[AWAIT_TRNK][GROUP_BUSY]=rtn56;
        rtn_idx[AWAIT_TRNK][RLS]=rtn57;
        rtn_idx[AWAIT_TRNK][OPR_HOOK]=rtn58;
        rtn_idx[AWAIT_TRNK][ON_HOOK]=rtn59;
#else
        rtn_idx[AWAIT_TRNK][OPR_REJECT]=55;
        rtn_idx[AWAIT_TRNK][GROUP_BUSY]=56;
        rtn_idx[AWAIT_TRNK][RLS]=57;
        rtn_idx[AWAIT_TRNK][OPR_HOOK]=58;
        rtn_idx[AWAIT_TRNK][ON_HOOK]=59;
#endif

#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[PARK_IN_DIAL][i]=rtn62;
        rtn_idx[PARK_IN_DIAL][RLS]=rtn63;
        rtn_idx[PARK_IN_DIAL][OPR_HOOK]=rtn64;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[PARK_IN_DIAL][i]=62;
        rtn_idx[PARK_IN_DIAL][RLS]=63;
        rtn_idx[PARK_IN_DIAL][OPR_HOOK]=64;
#endif

#ifdef OPERATOR_PC
        rtn_idx[PARK_IN_DIAL][ON_HOOK]=rtn65;
#else
        rtn_idx[PARK_IN_DIAL][ON_HOOK]=65;
#endif

#ifdef OPR_MALICIOUS_DTCT_ACT

#ifdef OPERATOR_PC
        rtn_idx[IDLE][OPRSIDE_MALICIOUS_PARTY]=rtn93;
#else
        rtn_idx[IDLE][OPRSIDE_MALICIOUS_PARTY]=93;
#endif

#endif


#ifdef OPERATOR_PC
        rtn_idx[OPR_EXT_BUSY][ATT_RLS]=rtn94;
#else
        rtn_idx[OPR_EXT_BUSY][ATT_RLS]=94;
#endif


#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[PARK_OUT_DIAL][i]=rtn66;
        rtn_idx[PARK_OUT_DIAL][RLS]=rtn67;
        rtn_idx[PARK_OUT_DIAL][OPR_HOOK]=rtn68;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[PARK_OUT_DIAL][i]=66;
        rtn_idx[PARK_OUT_DIAL][RLS]=67;
        rtn_idx[PARK_OUT_DIAL][OPR_HOOK]=68;
#endif


#ifdef OPERATOR_PC
        rtn_idx[PARK][ON_HOOK]=rtn69;
        rtn_idx[PARK][RECALL]=rtn70;
        rtn_idx[PARK][TIME_OUT]=rtn71;
        rtn_idx[PARK][AT_RING]=rtn103;
        rtn_idx[PARK][OFF_HOOK]=rtn103;

#if( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
        rtn_idx[PARK][EM_OPEN]=rtn136;
        rtn_idx[PARK][OPR_REJECT]=rtn136;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        rtn_idx[PARK][DT_CLF_TO_OPR]=rtn136;
        rtn_idx[PARK][OPR_REJECT]=rtn136;
#endif

#else
        rtn_idx[PARK][ON_HOOK]=69;
        rtn_idx[PARK][RECALL]=70;
        rtn_idx[PARK][TIME_OUT]=71;
        rtn_idx[PARK][AT_RING]=103;
        rtn_idx[PARK][OFF_HOOK]=103;

#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
        rtn_idx[PARK][EM_OPEN]=136;
        rtn_idx[PARK][OPR_REJECT]=136;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
        rtn_idx[PARK][DT_CLF_TO_OPR]=136;
        rtn_idx[PARK][OPR_REJECT]=136;
#endif

#endif

#ifdef OPERATOR_PC
        rtn_idx[OPRSIDE_OPR_TALK][HASH]=rtn72;
        rtn_idx[OPRSIDE_OPR_TALK][STAR]=rtn73;
#else
        rtn_idx[OPRSIDE_OPR_TALK][HASH]=72;
        rtn_idx[OPRSIDE_OPR_TALK][STAR]=73;
#endif

#ifdef OPERATOR_PC
        rtn_idx[OPR_READY][PROG]=rtn74;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_READY][i]=rtn75;
        rtn_idx[OPR_READY][RLS]=rtn99;
        rtn_idx[OPR_PROG][RLS]=rtn76;
        rtn_idx[OPR_PROG][OPR_HOOK]=rtn77;
        rtn_idx[OPR_PROG][ATT_RLS]=rtn78;
        rtn_idx[OPR_PROG][SYS_PROG_KEY]=rtn88;
#else
        rtn_idx[OPR_READY][PROG]=74;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[OPR_READY][i]=75;
        rtn_idx[OPR_READY][RLS]=99;
        rtn_idx[OPR_PROG][RLS]=76;
        rtn_idx[OPR_PROG][OPR_HOOK]=77;
        rtn_idx[OPR_PROG][ATT_RLS]=78;
        rtn_idx[OPR_PROG][SYS_PROG_KEY]=88;
#endif


#ifdef OPERATOR_PC
        rtn_idx[OPR_PROG][HASH]=rtn79;
        rtn_idx[OPR_PROG][STAR]=rtn80;
#if (((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM)) || ((defined EM_INSTALLED) && (defined OPR_EM_PROGRAM)) || ((defined CAS1_INSTALLED) && (defined OPR_CAS1_PROGRAM)))
         rtn_idx[OPR_PROG][EMCAS_PROG_KEY]=rtn79a;
#endif
#else
        rtn_idx[OPR_PROG][HASH]=79;
        rtn_idx[OPR_PROG][STAR]=80;
#if (((defined CAS3_INSTALLED) && (defined OPR_CAS3_PROGRAM)) || ((defined EM_INSTALLED) && (defined OPR_EM_PROGRAM)) )
//        rtn_idx[OPR_PROG][EMCAS_PROG_KEY]=79a;
#endif
#endif

#ifdef OPERATOR_PC
        rtn_idx[AWAIT_PROG_DEST][RLS]=rtn76;
        rtn_idx[AWAIT_PROG_DEST][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_PROG_DEST][ATT_RLS]=rtn78;
#else
        rtn_idx[AWAIT_PROG_DEST][RLS]=76;
        rtn_idx[AWAIT_PROG_DEST][OPR_HOOK]=77;
        rtn_idx[AWAIT_PROG_DEST][ATT_RLS]=78;
#endif


#ifdef OPERATOR_PC
        rtn_idx[AWAIT_PROG_FUNC][RLS]=rtn76;
        rtn_idx[AWAIT_PROG_FUNC][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_PROG_FUNC][ATT_RLS]=rtn78;
#else
        rtn_idx[AWAIT_PROG_FUNC][RLS]=76;
        rtn_idx[AWAIT_PROG_FUNC][OPR_HOOK]=77;
        rtn_idx[AWAIT_PROG_FUNC][ATT_RLS]=78;
#endif


#ifdef OPERATOR_PC
        rtn_idx[AWAIT_ACT_INACT][RLS]=rtn76;
        rtn_idx[AWAIT_ACT_INACT][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_ACT_INACT][ATT_RLS]=rtn78;
#else
        rtn_idx[AWAIT_ACT_INACT][RLS]=76;
        rtn_idx[AWAIT_ACT_INACT][OPR_HOOK]=77;
        rtn_idx[AWAIT_ACT_INACT][ATT_RLS]=78;
#endif


#ifdef OPERATOR_PC
        rtn_idx[AWAIT_SYS_PROG][RLS]=rtn76;
        rtn_idx[AWAIT_SYS_PROG][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_SYS_PROG][ATT_RLS]=rtn78;
#else
        rtn_idx[AWAIT_SYS_PROG][RLS]=76;
        rtn_idx[AWAIT_SYS_PROG][OPR_HOOK]=77;
        rtn_idx[AWAIT_SYS_PROG][ATT_RLS]=78;
#endif


#ifdef OPERATOR_PC
        rtn_idx[AWAIT_PASSWORD][RLS]=rtn76;
        rtn_idx[AWAIT_PASSWORD][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_PASSWORD][ATT_RLS]=rtn78;
#else
        rtn_idx[AWAIT_PASSWORD][RLS]=76;
        rtn_idx[AWAIT_PASSWORD][OPR_HOOK]=77;
        rtn_idx[AWAIT_PASSWORD][ATT_RLS]=78;
#endif


#ifdef OPERATOR_PC
        rtn_idx[OPR_PROG][CO_ID_KEY]=rtn81;
        rtn_idx[OPR_PROG][TIE_ID_KEY]=rtn81;
#else
        rtn_idx[OPR_PROG][CO_ID_KEY]=81;
        rtn_idx[OPR_PROG][TIE_ID_KEY]=81;
#endif


#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_PROG_DEST][i]=rtn82;
        rtn_idx[AWAIT_PROG_DEST][EOD_ID_KEY]=rtn83;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_PROG_DEST][i]=82;
        rtn_idx[AWAIT_PROG_DEST][EOD_ID_KEY]=83;
#endif


#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_PROG_FUNC][i]=rtn84;
        rtn_idx[AWAIT_PROG_FUNC][EOD_ID_KEY]=rtn86;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_PROG_FUNC][i]=84;
        rtn_idx[AWAIT_PROG_FUNC][EOD_ID_KEY]=86;
#endif


#ifdef OPERATOR_PC
        rtn_idx[AWAIT_ACT_INACT][N_1]=rtn87;
        rtn_idx[AWAIT_ACT_INACT][N_0]=rtn87;
#else
        rtn_idx[AWAIT_ACT_INACT][N_1]=87;
        rtn_idx[AWAIT_ACT_INACT][N_0]=87;
#endif


#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_SYS_PROG][i]=rtn89;
        rtn_idx[AWAIT_SYS_PROG][EOD_ID_KEY]=rtn124;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_SYS_PROG][i]=89;
        rtn_idx[AWAIT_SYS_PROG][EOD_ID_KEY]=124;
#endif


#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_PASSWORD][i]=rtn90;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_PASSWORD][i]=90;
#endif


#ifdef DAY_NIGHT_PARAMETERS_FILE

        rtn_idx[UPLOAD_FILE_SELECTION_1][RLS]=rtn76;
        rtn_idx[UPLOAD_FILE_SELECTION_1][OPR_HOOK]=rtn77;
        rtn_idx[UPLOAD_FILE_SELECTION_1][ATT_RLS]=rtn78;
        rtn_idx[UPLOAD_FILE_SELECTION_1][DAY_FILE_ID]=rtn97_1;
        rtn_idx[UPLOAD_FILE_SELECTION_1][NIGHT_FILE_ID]=rtn97_1;

        rtn_idx[UPLOAD_FILE_SELECTION_2][RLS]=rtn76;
        rtn_idx[UPLOAD_FILE_SELECTION_2][OPR_HOOK]=rtn77;
        rtn_idx[UPLOAD_FILE_SELECTION_2][ATT_RLS]=rtn78;
        rtn_idx[UPLOAD_FILE_SELECTION_2][DAY_FILE_ID]=rtn97_2;
        rtn_idx[UPLOAD_FILE_SELECTION_2][NIGHT_FILE_ID]=rtn97_2;

        rtn_idx[UPLOAD_FILE_SELECTION_3][RLS]=rtn76;
        rtn_idx[UPLOAD_FILE_SELECTION_3][OPR_HOOK]=rtn77;
        rtn_idx[UPLOAD_FILE_SELECTION_3][ATT_RLS]=rtn78;
        rtn_idx[UPLOAD_FILE_SELECTION_3][DAY_FILE_ID]=rtn97_3;
        rtn_idx[UPLOAD_FILE_SELECTION_3][NIGHT_FILE_ID]=rtn97_3;
#endif


#ifdef TRUNK_TRUNK_CONNECTION

#ifdef OPERATOR_PC

        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_TRNK_NO][i]=rtn117;
        rtn_idx[AWAIT_TRNK_NO][RLS]=rtn99;
        rtn_idx[AWAIT_TRNK_NO][EOD_ID_KEY]=rtn118;
        rtn_idx[AWAIT_TRNK_NO][OPR_HOOK]=rtn19;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_TRNK_NO][i]=117;
        rtn_idx[AWAIT_TRNK_NO][RLS]=99;
        rtn_idx[AWAIT_TRNK_NO][EOD_ID_KEY]=118;
        rtn_idx[AWAIT_TRNK_NO][OPR_HOOK]=19;
#endif

#endif


#ifdef TRUNK_TRUNK_CONNECTION

#ifdef OPERATOR_PC

        rtn_idx[OPR_TRUNK_OVRD][OPR_REJECT]=rtn119;
        rtn_idx[OPR_TRUNK_OVRD][RLS]=rtn120;
        rtn_idx[OPR_TRUNK_OVRD][FRC_RLS]=rtn121;
        rtn_idx[OPR_TRUNK_OVRD][OPR_HOOK]=rtn46;
#else
        rtn_idx[OPR_TRUNK_OVRD][OPR_REJECT]=119;
        rtn_idx[OPR_TRUNK_OVRD][RLS]=120;
        rtn_idx[OPR_TRUNK_OVRD][FRC_RLS]=121;
        rtn_idx[OPR_TRUNK_OVRD][OPR_HOOK]=46;
#endif

#endif


#ifdef TRUNK_TRUNK_CONNECTION
#ifdef OPERATOR_PC

        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][TRNK_RLS]=rtn122;
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][RING_DTCT]=rtn37;
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][RECALL]=rtn85;
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][TRNK_FREE]=rtn38;
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][CNNCT_TO_OPR]=rtn123;
#else
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][TRNK_RLS]=122;
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][RING_DTCT]=37;
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][RECALL]=85;
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][TRNK_FREE]=38;
        rtn_idx[OPRSIDE_TRUNK_TRUNK_TALK][CNNCT_TO_OPR]=123;
#endif
#endif


#ifdef PRE_INSTALL

#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_GAIN][i]=rtn125;
        rtn_idx[AWAIT_GAIN][RLS]=rtn76;
        rtn_idx[AWAIT_GAIN][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_GAIN][ATT_RLS]=rtn78;
        rtn_idx[AWAIT_GAIN][EOD_ID_KEY]=rtn126;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_GAIN][i]=125;
        rtn_idx[AWAIT_GAIN][RLS]=76;
        rtn_idx[AWAIT_GAIN][OPR_HOOK]=77;
        rtn_idx[AWAIT_GAIN][ATT_RLS]=78;
        rtn_idx[AWAIT_GAIN][EOD_ID_KEY]=126;
#endif

#endif


#ifdef OPERATOR_PC
        for(i=0;i < OPRSIDE_NO_STATES;i++)
                rtn_idx[i][TOG_LANGUAGE]=(FPCP)rtn0;

#ifdef OPERATOR_FARSI
        rtn_idx[OPR_IDLE][TOG_LANGUAGE]=rtn112;
#endif

#endif


#if(NO_OPRTR > 1)

#ifdef PARAMETER_DOWN_UP_LOAD

#ifdef OPERATOR_PC

#ifndef HOT_STANDBY
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_1][RLS]=rtn127;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_1][ATT_RLS]=rtn128;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_1][OPR_HOOK]=rtn129;
#endif
// not used.pcpmftx() for upload has changed to pmamftx(),
// so it is handled in omap().
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_1][UPLOAD_ACCEPT]=rtn130_1;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_1][UPLOAD_REJECT]=rtn131;

        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_2][RLS]=rtn127;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_2][ATT_RLS]=rtn128;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_2][OPR_HOOK]=rtn129;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_2][UPLOAD_ACCEPT]=rtn130_2;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_2][UPLOAD_REJECT]=rtn131;

        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_3][RLS]=rtn127;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_3][ATT_RLS]=rtn128;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_3][OPR_HOOK]=rtn129;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_3][UPLOAD_ACCEPT]=rtn130_3;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE_3][UPLOAD_REJECT]=rtn131;
#else
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE][RLS]=127;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE][ATT_RLS]=128;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE][OPR_HOOK]=129;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE][UPLOAD_ACCEPT]=130;
        rtn_idx[AWAIT_SYS_UPLOAD_ACCEPTANCE][UPLOAD_REJECT]=131;
#endif


#endif

#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
#ifdef OPERATOR_PC
        rtn_idx[IDLE][CAS3_OPR_REQ]=rtn37;
        rtn_idx[IN_EXTRNL_QU][DT_CLF_TO_OPR]=rtn39;
        rtn_idx[OPRSIDE_OPR_TALK][DT_CLF_TO_OPR]=rtn137;
#else
        rtn_idx[IDLE][CAS3_OPR_REQ]=37;
        rtn_idx[IN_EXTRNL_QU][DT_CLF_TO_OPR]=39;
        rtn_idx[OPRSIDE_OPR_TALK][DT_CLF_TO_OPR]=137;
#endif
#endif

#ifdef OPR_EM_CONNECTION
#ifdef OPERATOR_PC
        rtn_idx[OPR_READY][EM_GRP1]=rtn132;
        rtn_idx[OPR_READY][EM_GRP2]=rtn132;
        rtn_idx[OPR_READY][EM_GRP3]=rtn132;
        rtn_idx[OPR_READY][EM_GRP4]=rtn132;
        rtn_idx[OPR_READY][EM_GRP5]=rtn132;
        rtn_idx[OPR_READY][EM_GRP6]=rtn132;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP1]=rtn133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP2]=rtn133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP3]=rtn133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP4]=rtn133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP5]=rtn133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP6]=rtn133;
        rtn_idx[IDLE][EM_CONT_OPR_REQ]=rtn37;
        rtn_idx[IN_EXTRNL_QU][EM_OPEN]=rtn39;
        rtn_idx[OPRSIDE_OPR_TALK][EM_OPEN]=rtn137;
#else
        rtn_idx[OPR_READY][EM_GRP1]=132;
        rtn_idx[OPR_READY][EM_GRP2]=132;
        rtn_idx[OPR_READY][EM_GRP3]=132;
        rtn_idx[OPR_READY][EM_GRP4]=132;
        rtn_idx[OPR_READY][EM_GRP5]=132;
        rtn_idx[OPR_READY][EM_GRP6]=132;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP1]=133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP2]=133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP3]=133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP4]=133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP5]=133;
        rtn_idx[OPRSIDE_OPR_TALK][EM_GRP6]=133;
        rtn_idx[IDLE][EM_CONT_OPR_REQ]=37;
        rtn_idx[IN_EXTRNL_QU][EM_OPEN]=39;
        rtn_idx[OPRSIDE_OPR_TALK][EM_OPEN]=137;
#endif
#endif
#ifdef OPR_PRI_CONNECTION
#ifdef OPERATOR_PC
        rtn_idx[OPR_READY][PRI_GRP1]=rtn150;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_TRNK][i]=rtn31;
//      rtn_idx[IDLE][TRNK_FREE]=rtn38;
        rtn_idx[OPRSIDE_OPR_TALK][PRI_GRP1]=rtn151;
        rtn_idx[OPRSIDE_OPR_TALK][ON_HOOK]=rtn137;

        rtn_idx[HOLD][OPR_REJECT]=rtn135;
#else
        rtn_idx[OPR_READY][PRI_GRP1]=150;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_TRNK][i]=31;
//      rtn_idx[IDLE][TRNK_FREE]=38;
        rtn_idx[OPRSIDE_OPR_TALK][PRI_GRP1]=151;
        rtn_idx[OPRSIDE_OPR_TALK][ON_HOOK]=137;

        rtn_idx[HOLD][OPR_REJECT]=135;
#endif
#endif

#ifdef OPR_SIP_CONNECTION
#ifdef OPERATOR_PC
        rtn_idx[OPR_READY][SIP_REQ]=rtn153;
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_TRNK][i]=rtn31;
        rtn_idx[OPRSIDE_OPR_TALK][SIP_REQ]=rtn154;
        rtn_idx[OPRSIDE_OPR_TALK][ON_HOOK]=rtn137;

        rtn_idx[HOLD][OPR_REJECT]=rtn135;
#endif
#endif

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_CAS3_HST_CH][i]=rtn140;
        rtn_idx[AWAIT_CAS3_HST_CH][RLS]=rtn76;
        rtn_idx[AWAIT_CAS3_HST_CH][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_CAS3_HST_CH][ATT_RLS]=rtn78;
        rtn_idx[AWAIT_CAS3_HST_CH][EOD_ID_KEY]=rtn141;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_CAS3_HST_CH][i]=140;
        rtn_idx[AWAIT_CAS3_HST_CH][RLS]=76;
        rtn_idx[AWAIT_CAS3_HST_CH][OPR_HOOK]=77;
        rtn_idx[AWAIT_CAS3_HST_CH][ATT_RLS]=78;
        rtn_idx[AWAIT_CAS3_HST_CH][EOD_ID_KEY]=141;
#endif
#endif
#endif

#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_EM_HST_CH][i]=rtn142;
        rtn_idx[AWAIT_EM_HST_CH][RLS]=rtn76;
        rtn_idx[AWAIT_EM_HST_CH][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_EM_HST_CH][ATT_RLS]=rtn78;
        rtn_idx[AWAIT_EM_HST_CH][EOD_ID_KEY]=rtn143;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_EM_HST_CH][i]=142;
        rtn_idx[AWAIT_EM_HST_CH][RLS]=76;
        rtn_idx[AWAIT_EM_HST_CH][OPR_HOOK]=77;
        rtn_idx[AWAIT_EM_HST_CH][ATT_RLS]=78;
        rtn_idx[AWAIT_EM_HST_CH][EOD_ID_KEY]=143;
#endif
#endif
#endif

#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_CAS1_HST_CH][i]=rtn140a;
        rtn_idx[AWAIT_CAS1_HST_CH][RLS]=rtn76;
        rtn_idx[AWAIT_CAS1_HST_CH][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_CAS1_HST_CH][ATT_RLS]=rtn78;
        rtn_idx[AWAIT_CAS1_HST_CH][EOD_ID_KEY]=rtn141a;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_CAS1_HST_CH][i]=140a;
        rtn_idx[AWAIT_CAS1_HST_CH][RLS]=76;
        rtn_idx[AWAIT_CAS1_HST_CH][OPR_HOOK]=77;
        rtn_idx[AWAIT_CAS1_HST_CH][ATT_RLS]=78;
        rtn_idx[AWAIT_CAS1_HST_CH][EOD_ID_KEY]=141a;
#endif
#endif
#endif

#ifdef OPERATOR_PC
        for(i=0;i < OPRSIDE_NO_STATES;i++)
                rtn_idx[i][SHELL]=(FPCP)rtn0;

        rtn_idx[OPR_IDLE][SHELL]=(FPCP)rtn138;
#endif
#ifdef PC_COMPATIBLE

#ifdef OPERATOR_PC
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_SYSTEM_NO][i]=rtn146;
        rtn_idx[AWAIT_SYSTEM_NO][RLS]=rtn76;
        rtn_idx[AWAIT_SYSTEM_NO][OPR_HOOK]=rtn77;
        rtn_idx[AWAIT_SYSTEM_NO][ATT_RLS]=rtn78;
        rtn_idx[AWAIT_SYSTEM_NO][EOD_ID_KEY]=rtn147;
#else
        for(i=N_1;i<=N_0;i++)
                rtn_idx[AWAIT_SYSTEM_NO][i]=146;
        rtn_idx[AWAIT_SYSTEM_NO][RLS]=76;
        rtn_idx[AWAIT_SYSTEM_NO][OPR_HOOK]=77;
        rtn_idx[AWAIT_SYSTEM_NO][ATT_RLS]=78;
        rtn_idx[AWAIT_SYSTEM_NO][EOD_ID_KEY]=147;
#endif
#endif

}


#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void ocni(){  /* oprator card not installed */
        WINDOW *a;
#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
#ifdef NEW_DEP
        fpos_t pos;
#endif
#endif
#endif
        a=enw(28,10,5,34);
        if(a == NULL)
                return;
        activate_bell();
        dw(a);
        swb(a,DOUBLE_LINE);
//        swc(a,ALL,RED,YELLOW,BRIGHT);
        swc(a,ALL,KT_RED,KT_YELLOW,KT_HIGH_INTENSITY);
        swt(a,"ERROR");
        draw_window(a);
        wprompt(a,1,0,"Operator card not installed.");
        wprompt(a,1,1,"Turn off computer immediately.");
        wprompt(a,1,2,"Call KIATEL for repair.");

#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
        alarm();
#ifdef NEW_DEP
        fgetpos(flog,&pos);
        fseek(flog,0,SEEK_END);
#endif
        fprintf(flog,"O&M Hardware Fault");
#ifdef NEW_DEP
        fsetpos(flog,&pos);
#endif
#endif
#endif

        while(get_c() != NO_OPR_CARD_EXIT_KEY);
        caw();
        f |= OPR_CARD_OUT;
}
#endif
#endif

void sys_func(){  /* system function programming */
        int n;

#if ( (defined PARAMETER_DOWN_UP_LOAD) && (!defined DAY_NIGHT_PARAMETERS_FILE ) )
        unsigned int a;

#if ( (NO_OPRTR == 1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) )
        int b;
        unsigned int c;
#endif

#endif


#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif
        n=fd();
        switch(n){

#ifdef ON_LINE_BILLING_ACT
                case MAC_ONLINE_BILLING_ACT:
                                ocr.s.i=n;
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
                                else
#endif
                                        str2lcd(" 1:act.  0:inact ");
                                ocr.state=AWAIT_ACT_INACT;
                                break;
#endif

#ifdef PARAMETER_DOWN_UP_LOAD
                case MAC_DOWN_LOAD_PARAMETERS_1:
                case MAC_DOWN_LOAD_PARAMETERS_2:
                case MAC_DOWN_LOAD_PARAMETERS_3:
                                ocr.s.i=n;
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
                                else
#endif
                                        str2lcd(" 1:act.  0:inact ");
                                ocr.state=AWAIT_ACT_INACT;
                                break;
#endif

#ifdef PARAMETER_DOWN_UP_LOAD

#ifdef DAY_NIGHT_PARAMETERS_FILE
                case MAC_UP_LOAD_PARAMETERS_1:
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("       :¢˛éëı˜ íë°ó˜ê ê§ ë˚§óıê§ëï Ò˛ëÍ ");
                                        str2lcd("        1:•¯§ Ò˛ëÍ      2:í™ Ò˛ëÍ       ");
                                }
                                else{
#endif
                                        str2lcd("SELECT PARAMETERS FILE                  ");
                                        str2lcd("1:DAY FILE   2:NIGHT FILE");
#ifdef OPERATOR_FARSI
                                }
#endif
                                ocr.state=UPLOAD_FILE_SELECTION_1;
                                break;

                case MAC_UP_LOAD_PARAMETERS_2:
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("       :¢˛éëı˜ íë°ó˜ê ê§ ë˚§óıê§ëï Ò˛ëÍ ");
                                        str2lcd("        1:•¯§ Ò˛ëÍ      2:í™ Ò˛ëÍ       ");
                                }
                                else{
#endif
                                        str2lcd("SELECT PARAMETERS FILE                  ");
                                        str2lcd("1:DAY FILE   2:NIGHT FILE");
#ifdef OPERATOR_FARSI
                                }
#endif
                                ocr.state=UPLOAD_FILE_SELECTION_2;
                                break;
                case MAC_UP_LOAD_PARAMETERS_3:
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("       :¢˛éëı˜ íë°ó˜ê ê§ ë˚§óıê§ëï Ò˛ëÍ ");
                                        str2lcd("        1:•¯§ Ò˛ëÍ      2:í™ Ò˛ëÍ       ");
                                }
                                else{
#endif
                                        str2lcd("SELECT PARAMETERS FILE                  ");
                                        str2lcd("1:DAY FILE   2:NIGHT FILE");
#ifdef OPERATOR_FARSI
                                }
#endif
                                ocr.state=UPLOAD_FILE_SELECTION_3;
                                break;
#endif


#ifndef DAY_NIGHT_PARAMETERS_FILE
                case MAC_UP_LOAD_PARAMETERS_1:
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
                                        break;
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
#ifdef OPR_UPLOAD_WITH_TIMER
                                        upload_count=UPLOAD_PARM_TICK;
#endif
#ifdef HOT_STANDBY
                                        f |= (PARM_UP_1 | PARM_UP_2);
#else
                                        f |= PARM_UP_1;
#endif
                                        set_stm_for_up_down_load();
                                        /* only leds(),pc_timer_init(),lcds() & */
                                        /* onh() are unmasked */
                                        DIS_SPKR;  /* disable tone ringer */
#endif
/* NO_OPRTR == 1 */

                                }
                                else{
#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                                                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                                        }
                                        else{
#endif
                                                str2lcd("INVALID PARAMETERS FILE.                ");
                                                str2lcd("START NEW PROGRAMMING:");
#ifdef OPERATOR_FARSI
                                        }
#endif
                                }
                                ocr.d.t=NULL_T;
                                ocr.s.t=NULL_T;
                                ocr.mf=PROG_ACT;
                                ocr.state=OPR_PROG;
                                break;

                case MAC_UP_LOAD_PARAMETERS_2:
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
                                        break;
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
#endif
/* NO_OPRTR == 1 */

                                }
                                else{
#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                                                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                                        }
                                        else{
#endif
                                                str2lcd("INVALID PARAMETERS FILE.                ");
                                                str2lcd("START NEW PROGRAMMING:");
#ifdef OPERATOR_FARSI
                                        }
#endif
                                }
                                ocr.d.t=NULL_T;
                                ocr.s.t=NULL_T;
                                ocr.mf=PROG_ACT;
                                ocr.state=OPR_PROG;
                                break;
                case MAC_UP_LOAD_PARAMETERS_3:
                                pfp[2]=fopen(pfna[2][0],"rb");
                                if( (pfp[2] != NULL) && (vpf(pfp[2],2) == OK) ){
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
                                        break;
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
#endif
/* NO_OPRTR == 1 */

                                }
                                else{
#ifdef OPERATOR_FARSI
                                        if(f & OPR_LANG_FLAG){
                                                str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                                                str2lcd("             :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì");
                                        }
                                        else{
#endif
                                                str2lcd("INVALID PARAMETERS FILE.                ");
                                                str2lcd("START NEW PROGRAMMING:");
#ifdef OPERATOR_FARSI
                                        }
#endif
                                }
                                ocr.d.t=NULL_T;
                                ocr.s.t=NULL_T;
                                ocr.mf=PROG_ACT;
                                ocr.state=OPR_PROG;
                                break;
#endif    /* ndef DAY_NIGHT_PARAMETERS_FILE */


#endif
                case MAC_SS7_PACKET_PRINT:
                case MAC_TRUNK_STATE_PRINT:
                case MAC_EXT_STATE_PRINT:
#ifdef CONFERENCE_ACT
                case MAC_CONFERENCE_STATE_PRINT:
#endif
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
                case OP_FIRST_ZERO_ON_CO:
#endif
                                ocr.s.i=n;
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG)
                                        str2lcd("        1:Òë„Í            0:Òë„Í§˛Ë     ");
                                else
#endif
                                        str2lcd(" 1:act.  0:inact ");
                                ocr.state=AWAIT_ACT_INACT;
                                break;

                case CHANGE_PASSWORD:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("         :¢˛éëı˜ ¢§ê¯ ê§ ¢˛¢õ §¯ì‰ ˘ıÛÓ ");
                                        str2lcd(lcd_space_line);
                                }
                                else
#endif
                                        str2lcd(" ENTER NEW PASSWORD:");
#else
                                str2lcd(" ENTER NEW       PASSWORD:");
#endif
                                ocr.scdc=0;
                                ocr.xd=NO_PASSWORD_DGT;
                                ocr.state=AWAIT_PASSWORD;
                                break;

#ifdef PRE_INSTALL
                case MAC_OPR_RCVR_GAIN:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("   :(˝§ë™‰ê) ¢˛éëı˜ ¢§ê¯ ê§ §¯óê§ïê gain");
                                        str2lcd(lcd_space_line);
                                }
                                else
#endif
                                        str2lcd("Enter opr. rcvr gain in decimal:");
#else
                                str2lcd("opr. rx gain in  dec.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=3;
                                /* because gain is 1 byte & entered in */
                                /* decimal 3 digits are needed */
                                ocr.state=AWAIT_GAIN;
                                break;
#endif

#ifdef PRE_INSTALL
                case MAC_ANALOG_TRUNK_GAIN:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(":(¸§ë™‰ê) ¢˛éëı˜ ¢§ê¯ ê§ TIE ¯ ˝§˙™ gain");
                                        str2lcd("                    ");
                                }
                                else
#endif
                                        str2lcd("Enter CO & TIE trunks gain in decimal:   ");
#else
                                str2lcd("CO & TIE gain in  dec.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=3;
                                /* because gain is 1 byte & entered in */
                                /* decimal 3 digits are needed */
                                ocr.state=AWAIT_GAIN;
                                break;
#endif

#ifdef PRE_INSTALL
                case MAC_EXT_CODEC_GAIN:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("     :(˝§ë™‰ê) ¢˛éëı˜ ¢§ê¯ ê§ ¸Û°ê¢ gain");
                                        str2lcd(lcd_space_line);
                                }
                                else
#endif
                                        str2lcd("Enter extensions gain in decimal:");
#else
                                str2lcd("ext. gain in    dec.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=3;
                                /* because gain is 1 byte & entered in */
                                /* decimal 3 digits are needed */
                                ocr.state=AWAIT_GAIN;
                                break;
#endif


#ifdef EM_INSTALLED

#ifdef PRE_INSTALL
                case MAC_EM_TRUNK_GAIN:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("(˝§ë™‰ê)¢˛éëı˜ ¢§ê¯ ê§ E&M ˝ë˙Ó˜ê§ó gain");
                                        str2lcd("                    ");
                                 }
                                else
#endif
                                        str2lcd("Enter E & M trunks gain in decimal:");
#else
                                str2lcd("E & M gain in   dec.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=3;
                                /* because gain is 1 byte & entered in */
                                /* decimal 3 digits are needed */
                                ocr.state=AWAIT_GAIN;
                                break;
#endif

#endif

#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
                case MAC_CAS3_HISTORICAL_LOOSE_REQUEST:
                case MAC_CAS3_HISTORICAL_REQUEST:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("(˝§ë™‰ê)¢˛éëı˜ ¢§ê¯ ê§  CAS3   Òë˜ëÓ ˘§ëı™");
                                        str2lcd("                    ");
                                 }
                                else
#endif
                                        str2lcd("Enter CAS3 ch. no. :");
#else
                                str2lcd("CAS3 ch. no.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=NO_CAS3_HST_CH_DGT;
                                ocr.state=AWAIT_CAS3_HST_CH;
                                break;
#endif
#endif
#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
                case MAC_EM_HISTORICAL_REQUEST:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("(˝§ë™‰ê)¢˛éëı˜ ¢§ê¯ ê§  E&M   Òë˜ëÓ ˘§ëı™");
                                        str2lcd("                    ");
                                 }
                                else
#endif
                                        str2lcd("Enter E&M ch. no. :");
#else
                                str2lcd("E&M ch. no.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=NO_EM_HST_CH_DGT;
                                ocr.state=AWAIT_EM_HST_CH;
                                break;
#endif
#endif

#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
                case MAC_CAS1_HISTORICAL_REQUEST:
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("(˝§ë™‰ê)¢˛éëı˜ ¢§ê¯ ê§  CAS1   Òë˜ëÓ ˘§ëı™");
                                        str2lcd("                    ");
                                 }
                                else
#endif
                                        str2lcd("Enter CAS1 ch. no. :");
#else
                                str2lcd("CAS1 ch. no.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=NO_CAS1_HST_CH_DGT;
                                ocr.state=AWAIT_CAS1_HST_CH;
                                break;
#endif
#endif

#ifdef EEPROM
                case MAC_SET_DEFAULT_PARAM:
                                str2lcd("Default Parameters are set.");
                                ocr.s.i=n;
                                ocr.s.t=SET;
                                ocr.d.t=OPR_T;
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

                                break;
#endif

#ifdef PC_COMPATIBLE
                case MAC_EXIT_PROGRAM :
#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("(˝§ë™‰ê)¢˛éëı˜ ¢§ê¯ ê§  Ùó®˛® ˘§ëı™");
                                        str2lcd("                    ");
                                 }
                                else
#endif
                                        str2lcd("Enter System no. :");
#else
                                str2lcd("System no.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=1;  //No. of digits
                                ocr.state=AWAIT_SYSTEM_NO;
                                break;
                case MAC_SWITCHOVER_PROGRAM :
                                ocr.s.i=n;
                                ocr.s.t=SET;
                                ocr.d.t=OPR_T;
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
                                break;
/*
#ifdef OPERATOR_PC
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("(˝§ë™‰ê)¢˛éëı˜ ¢§ê¯ ê§  Ùó®˛® ˘§ëı™");
                                        str2lcd("                    ");
                                 }
                                else
#endif
                                        str2lcd("Enter System no. :");
#else
                                str2lcd("System no.:");
#endif
                                ocr.s.i=n;
                                ocr.scdc=0;
                                ocr.xd=1;  //No. of digits
                                ocr.state=AWAIT_SYSTEM_NO;
                                break;
*/
#endif


                default:

#ifdef OPERATOR_PC

#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd("                 .ñ®˛˜ §ìó„ı ˘¢™ ¢§ê¯ ¢Ó");
                                        str2lcd("                   .¢¯™˛ı ‚ØÏ ¸•˛§˘ıë˜§ì");
                                }
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
                                        str2lcd("                     ...ñ®ê˘¢ëıç §¯óê§ïê");
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
                                break;
        }
}


#ifdef AUDIT_ACT
void oam(){  /* operator audit manager */
        ohsa();  /* operator handset audit */
}
#endif


#ifdef AUDIT_ACT
void ohsa(){  /* operator handset audit */
        int a;
        a=ocr.state;
        if( (a == OPR_IDLE) || (a == OPRSIDE_OPR_NIGHT) )
                return;

#ifdef OPERATOR_PC
#ifdef HDLC_8993
        kt_outport8(TRNS_CR,HSMIC | HSSKR);
#endif
#else
        XBYTE[TRNS_CR]=HSMIC | HSSKR;
#endif
}
#endif


void rlstrnk(INDEX_TYPE a){
        int i,t;
        t=a.t;
        if( (t & TRUNK_T) == RESET)     return;
        i=a.i;
        pcpmftx(TRNK_RLS,i,t);

        switch(t){
#if(NO_CO != 0)
                case  CO_T:     ctst[i].s=IDLE;
                                break;
#endif


#ifndef NO_TIE_INIT
                case TIE_T:     ttst[i].s=IDLE;
                                break;
#endif


#ifdef OPR_EM_CONNECTION
                case EM_T:      emtst[i].s=IDLE;
                                break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:    dtcst[i].s=IDLE;
                                break;
#endif
                default:        break;
        }
}

void opoh(){    /* operator on hook */

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#ifdef HDLC_8993
        kt_outport8(TRNS_CR,0);
#endif
#else
        init_lcd();
        XBYTE[TRNS_CR]=0;
#endif
        ocr.state=OPR_IDLE;
        ocr.s.t=NULL_T;
        ocr.d.t=NULL_T;
        hld.t=NULL_T;
        trf |= OPR_HOOK_STAT;
        trs=0xff;
        ocr.mf=0;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
}

#if  ((defined AUTO_UPLOAD) || (defined SCHEDULED_UPLOAD))
void auto_upload(BYTE unconditional,BYTE n,BYTE day_nite){      // 0, day; 1, nite
/*
        INDEX_TYPE a;
        a.t=OPR_T;
        a.i=TON;
        tocpq(a,NIGHT);
        tocpq(a,OPR_HOOK);
        tocpq(a,PROG);
        tocpq(a,(pswd_d2d1&0x0f)+N_1-1);
        tocpq(a,((pswd_d2d1&0xf0)>>4)+N_1-1);
        tocpq(a,(pswd_d4d3&0x0f)+N_1-1);
        tocpq(a,((pswd_d4d3&0xf0)>>4)+N_1-1);
        tocpq(a,OPRSIDE_OVRD);
        tocpq(a,N_0);
        tocpq(a,N_2);
        tocpq(a,N_1);
        tocpq(a,OPR_HOOK);
        tocpq(a,NIGHT);
*/
        int a,b;
        unsigned int c;
        unsigned int file_changed;
        INDEX_TYPE pt;

        pt.t=OPR_T;
        pt.i=TON;

        rtn102(pt);

#ifdef OPERATOR_PC
        tolcdq(0xff,CLEAR_LCD);
#else
        init_lcd();
#endif

#ifdef DAY_NIGHT_PARAMETERS_FILE
        pfp[n]=fopen(pfna[n][day_nite],"rb");
#else
        pfp[n]=fopen(pfna[n][0],"rb");
#endif

        fseek(pfp[n],MAX_NAME_LENGHT-2,SEEK_SET);
        file_changed=my_getw(pfp[n]);

        if((unconditional==SET) || (file_changed==SET))

        if( (pfp[n] != NULL) && (vpf(pfp[n],n) == OK) ){
        /* if file is valid,the parameters is in parmb[]. */
                a=MA_SYS_P | MAC_UP_LOAD_PARAMETERS_1;
                /* feature activation bit in command frame is 0 */


#ifdef OPR_UPLOAD_WITH_TIMER
                upload_count=UPLOAD_PARM_TICK;
#endif
#ifdef HOT_STANDBY
                pmamftx_both_sys(a,TON,OPR_T,n);  /* send upload command to system */
#else
                pmamftx(a,TON,OPR_T,n);  /* send upload command to system */
#endif

                a |= FEATURE_ACTIVATION;
                /* feature activation bit in parameters frames is 1 */
                for(b=0;b < no_load_parm[n];b++){
                        c=parmb[b];
#ifdef HOT_STANDBY
                        pmamftx_both_sys(a,c >> 4,c & 0x0f,n);
#else
                        pmamftx(a,c >> 4,c & 0x0f,n);
#endif
                }

#ifdef DAY_NIGHT_PARAMETERS_FILE
                pfp[n]=fopen(pfna[n][day_nite],"r+b");
#else
                pfp[n]=fopen(pfna[n][0],"r+b");
#endif

                fseek(pfp[n],MAX_NAME_LENGHT-2,SEEK_SET);
                my_putw(RESET,pfp[n]);
                cfcs(pfp[n]);
                fclose(pfp[n]);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)
                        str2lcd("                 ...Ùó®˛® ˘ì Òë®§ê ·¯§™ ");
                else
#endif
                        str2lcd("UPLOAD STARTED...");
                if(n==0)
#ifdef HOT_STANDBY
                        f |= (PARM_UP_1 | PARM_UP_2);
#else
                        f |= PARM_UP_1;
#endif
                else if(n==1)
                        f |= PARM_UP_2;
                else if(n==2)
                        f |= PARM_UP_3;
                set_stm_for_up_down_load();
                /* only leds(),pc_timer_init(),lcds() & onh() are unmasked */
                DIS_SPKR;  /* disable tone ringer */
        }
        else{

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("              .ñ®˛˜ §ìó„ı ë˚§óıê§ëï Ò˛ëÍ");
                }
                else{
#endif
                        str2lcd("INVALID PARAMETERS FILE.               ");
#ifdef OPERATOR_FARSI
                }
#endif
        }

        rtn101(pt);
}
#endif

#ifdef PC_KEYBOARD
void init_keyboard(void){
        int i;
        /* this routine is called AFTER init_t0_srl */
        /* we should either not use TXD, or disable serial in init_t0_srl */

        for(i=0;i<0x100;i++)
                kctt[i]=NULL;

        kctt[0x15]=CO_GRP1;
        kctt[0x1d]=CO_GRP2;
        kctt[0x24]=CO_GRP3;
        kctt[0x2d]=CO_GRP4;
        kctt[0x2c]=CO_GRP5;
        kctt[0x1c]=TIE_GRP1;
        kctt[0x1b]=TIE_GRP2;
        kctt[0x23]=TIE_GRP3;
        kctt[0x2b]=TIE_GRP4;
        kctt[0x34]=TIE_GRP5;
        kctt[0x1a]=NIGHT;
        kctt[0x22]=PROG;
        kctt[0x21]=OPR_HOOK;
        kctt[0x2a]=VOLUME;
        kctt[0x32]=TIME;
        kctt[0x43]=CO_GRP6;
        kctt[0x44]=RECALL;
        kctt[0x4d]=EXTRNL;
        kctt[0x54]=INTERNAL;
        kctt[0x5b]=ATT_RLS;
        kctt[0x3b]=TIE_GRP6;
        kctt[0x42]=SRC;
        kctt[0x4b]=DEST;
        kctt[0x4c]=OVRD_KEY;
        kctt[0x52]=TONE_RNGR;
        kctt[0x31]=HOLD1;
        kctt[0x3a]=HOLD2;
        kctt[0x41]=HOLD3;
        kctt[0x49]=HOLD4;
        kctt[0x4a]=PARK_XT;
        kctt[0x35]=DISP_STATUS;
        kctt[0x29]=RLS;
        kctt[0x69]=N_1;
        kctt[0x72]=N_2;
        kctt[0x7a]=N_3;
        kctt[0x6b]=N_4;
        kctt[0x73]=N_5;
        kctt[0x74]=N_6;
        kctt[0x6c]=N_7;
        kctt[0x75]=N_8;
        kctt[0x7d]=N_9;
        kctt[0x16]=N_1;
        kctt[0x1e]=N_2;
        kctt[0x26]=N_3;
        kctt[0x25]=N_4;
        kctt[0x2e]=N_5;
        kctt[0x36]=N_6;
        kctt[0x3d]=N_7;
        kctt[0x3e]=N_8;
        kctt[0x46]=N_9;
        kctt[0x45]=N_0;
        kctt[0x0d]=TOG_LANGUAGE;
        kctt[0x0e]=SHELL;


        KEYBOARD_CLOCK=0;               /* keyboard clock */
        KEYBOARD_DATA=1;                /* keyboard data */
        SELECT_KEYBOARD_CLOCK=1;        /* invert keyboard clock, isr with rising edge  */

        rxflag=1;       /* on recieve mode in initialization */
        break_flag=0;
        tx_delay_flag=0;
        bi=0;

        TCON |= 4;      /* define falling-edge trigger for external interrupt 1 */

        IE |= 4;        /* enable EX1 interrupt */

        KEYBOARD_CLOCK=1;          /* enable the keyboard to send data */
}
BYTE bit2byte(void){    /* Makes the character from the received bits */

/* din[0], extra bit generated by us making KEYBOARD_CLOCK a transition from 0 to 1 */
/* din[1], start bit, should be zero */
/* din[2]..din[9], data bits */
/* din[10], odd parity bit */
/* din[11], stop bit, should be one */

        BYTE ch,i,j;
        ch=0;
        j=1;
        for(i=2;i<10;i++){
                if(din[i]) ch |= j;
                j <<= 1;
        }
        return ch;
}


BYTE parity(void){      /* Checks the parity bit and stop bit */
                        /* returns zero if stop bit is zero or if parity
                           computation is even, otherwise returns 1 */
        BYTE t,i;
        t=0;
        for(i=2;i<11;i++)
               t += din[i];
        return ( (t&0x01) & din[11] );
}
void s_r_bit(void) interrupt 2 {
        /* Interrupt service routine for external interrupt 1 */
        /* activated by falling edge of input */

        if(rxflag){             /* receiving bits */
                din[bi]=KEYBOARD_DATA;
                bi++;
                if(bi==12) KEYBOARD_CLOCK=0;       /* to inhibit data transmition by keyboard */
        }
        else{                   /* sending bits */
                KEYBOARD_DATA=din[bi];
                bi++;
                if (bi==11) {   /* to receive ack bit from keyboard */
                        bi=13;
                        rxflag=1;
                }
        }
}

void kbs(void){
        BYTE rb;        /* received byte */
        INDEX_TYPE a;

        if(bi==12){    /* byte received (in rx mode) */
                if(parity()){
                        rb=bit2byte();
                        bi=0;
                        KEYBOARD_CLOCK=1;       /* again enable the keyboard to send data */
                        if(rb == 0xf0) {        /* next code will be break */
                                break_flag=1;
                                return;
                        }
                        else if(break_flag){    /* this is a break, ignore */
                                break_flag=0;
                                return;
                        }
                        else if(rb==oldrb) return;      /* this is a typematic repeat, ignore */
                        else {
                                oldrb=rb;               /* save it for checking against next code (typematic) */
                                rb=kctt[rb];            /* decypher it */
                                if(rb != NULL)          /* if valid */
                                        a.t=OPR_T;
                                        a.i=TON;
                                        tocpq(a,rb);
                                return;                 /* else ignore */
                        }
                }
                else{
                        sys2key(0xfe);          /* request resend */
                        tx_delay_flag=1;        /* to implement one kbs() tick (80msec) delay */
                        return;
                }
        }
        else if(bi==14){        /* ack bit received (in tx mode) */
                bi=0;
                KEYBOARD_CLOCK=1;       /* should already be one */
                SELECT_KEYBOARD_CLOCK=1;        /* do it here or in isr after sending the 10th bit, before setting bi to 13 */
                KEYBOARD_DATA=1;        /* for "etminaan!" */
        }
        else if(tx_delay_flag){         /* one kbs() tick has passed from sys2key() */
                tx_delay_flag=0;        /* reset flag */
                rxflag=0;               /* isr in transmit mode */
                KEYBOARD_CLOCK=1;       /* allow keyboard to clock and receive data */
                SELECT_KEYBOARD_CLOCK=0;  /* do not invert keyboard clock, isr with falling edge */
        }
}

void sys2key(BYTE cmd){       /* Prepares for sending cmd to keyboard */
        BYTE t,i,j;
        KEYBOARD_CLOCK=0;
        KEYBOARD_DATA=0;
        din[0]=0;               /* start bit */
        t=0;                    /* parity bit */
        j=1;                    /* is shifted left to check bit position */
        for(i=1;i<9;i++){
                if(cmd & j) {
                        din[i]=1;
                        t++;
                }
                else
                        din[i]=0;
                j <<= 1;
        }
        din[9]= (~t) & 0x01 ;   /* odd parity, lsb */
        din[10]=1;              /* stop bit */
        bi=0;                   /* is now used as a pointer to currently to be transmitted bit */
}
#endif

#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
void my_alarm(void){
#ifdef HDLC_8993
        kt_outport8(base_addr[0] | SD_DIR,0x02);    /* only SD1 output */
        kt_outport8(base_addr[0] | SD_DATA,0x00);   /* SD1=0, enable alarm  */
#endif
        date_to_log();
}
void date_to_log(void){

#ifdef NEW_DEP
        fpos_t pos;
#endif
        sdt=*get_dt(&sdt);
#ifdef NEW_DEP
        fgetpos(flog,&pos);
        fseek(flog,0,SEEK_END);
#endif
        fprintf(flog,"\n%4x-%02x-%02x %02x:%02x:%02x: ",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se);
#ifdef NEW_DEP
        fsetpos(flog,&pos);
#endif
}
void alarm_reset(void){
#ifdef HDLC_8993
        kt_outport8(base_addr[0] | SD_DIR,0x02);    /* only SD1 output */
        if( (kt_inport8(base_addr[0] | SD_DATA) & 0x01) != 0 ) {  /* SD0=0, key pressed */
                kt_outport8(base_addr[0] | SD_DATA,0x02);   /* SD1=0, disable alarm */
        }
#endif
}
#endif
#endif

// we may use long version also for int version, however there shall be
// warning messages where hex2bcd is calling find_..., may be hex2bcd
// also should always be the long version
//#if (NO_INTRNL_DGT >= 5)
SDNST find_subscriber_no(unsigned int i){
        {
            SDNST k;unsigned int a;
            for(a=0;a<no_of_subscriber_groups;a++){
                if((k=(i-subscriber_group[a].port_start)) < subscriber_group[a].no_of_ports)
                    return (k+subscriber_group[a].directory_start);
            }
        }
#ifdef WE_DO_HAVE_TWO_FDN
        if(i >= NO_EXTENSION_FOR_F_D_N_A)
                return (from_internal_to_dialled_no[i]-NO_EXTENSION_FOR_F_D_N_A)+hex2l(F_D_N_B);
        else
#endif
#ifdef AHAR
        if(i >= 216)
                return (long)((i-216)+900);
        else
#endif
        return ((unsigned long)from_internal_to_dialled_no[i]+first_directory_decimal);
}

void init_array(void){
        int i;
#ifdef TCI_AZ
        BYTE j;
#endif
#ifdef OLOOM_PEZESHKI
        int  j;
#endif
#ifdef DARAI_ZANJAN
        int  j;
#endif
#ifdef PETRO_GAS
        int  j;
#endif
#ifdef TCI_AZ3005
        BYTE k,j;
#endif
#ifdef IRAN_NEWSPAPER
        int  j;
#endif
#ifdef NEHZAT
        int  j;
#endif
#ifdef BAGHER_ABAD
        int  j;
#endif
        for(i=0;i<NO_TOTAL_EXTENSION;i++)
                from_internal_to_dialled_no[i]=i;

#ifdef TCI_AZ
        from_internal_to_dialled_no[ 0]=11;
        from_internal_to_dialled_no[ 1]=12;
        from_internal_to_dialled_no[ 2]=13;

        from_internal_to_dialled_no[ 3]=21;
        from_internal_to_dialled_no[ 4]=22;
        from_internal_to_dialled_no[ 5]=23;

        from_internal_to_dialled_no[ 6]=31;
        from_internal_to_dialled_no[ 7]=32;
        from_internal_to_dialled_no[ 8]=33;
        from_internal_to_dialled_no[ 9]=34;
        from_internal_to_dialled_no[10]=35;
        from_internal_to_dialled_no[11]=36;

        from_internal_to_dialled_no[12]=41;
        from_internal_to_dialled_no[13]=42;
        from_internal_to_dialled_no[14]=43;
        from_internal_to_dialled_no[15]=44;
        from_internal_to_dialled_no[16]=45;
        from_internal_to_dialled_no[17]=46;
        from_internal_to_dialled_no[18]=47;

        from_internal_to_dialled_no[19]=51;
        from_internal_to_dialled_no[20]=52;
        from_internal_to_dialled_no[21]=53;
        from_internal_to_dialled_no[22]=54;
        from_internal_to_dialled_no[23]=55;

        from_internal_to_dialled_no[24]=61;
        from_internal_to_dialled_no[25]=62;
        from_internal_to_dialled_no[26]=63;
        from_internal_to_dialled_no[27]=64;

        from_internal_to_dialled_no[28]=71;
        from_internal_to_dialled_no[29]=72;
        from_internal_to_dialled_no[30]=73;
        from_internal_to_dialled_no[31]=74;

        from_internal_to_dialled_no[32]=81;
        from_internal_to_dialled_no[33]=82;
        from_internal_to_dialled_no[34]=83;
        from_internal_to_dialled_no[35]=84;

        for(i=36;i<2*36;i++)
                from_internal_to_dialled_no[i]=from_internal_to_dialled_no[i-36] + 100;
        for(i=2*36;i<3*36;i++)
                from_internal_to_dialled_no[i]=from_internal_to_dialled_no[i-36] + 100;
        for(i=3*36;i<4*36;i++)
                from_internal_to_dialled_no[i]=from_internal_to_dialled_no[i-36] + 100;
        for(i=4*36;i<5*36;i++)
                from_internal_to_dialled_no[i]=from_internal_to_dialled_no[i-36] + 100;
        for(i=5*36;i<6*36;i++)
                from_internal_to_dialled_no[i]=from_internal_to_dialled_no[i-36] + 100;

        for(i=6*36,j=0;j<30;i++,j++)
                from_internal_to_dialled_no[i]=611+j;

        for(i=(6*36)+30,j=0;j<26+64;i++,j++)
                from_internal_to_dialled_no[i]=711+j;
#endif

#ifdef TCI_AZ3005
        i=0;
        for(k=1;k<=6;k++)
                for(j=0;j<=51;j++,i++)
                        from_internal_to_dialled_no[i]=j+(k*100);
        for(j=0;j<=55;j++,i++)
                from_internal_to_dialled_no[i]=j+700;
#endif

#ifdef OLOOM_PEZESHKI
        i=0;
        for(j=0;j<=15;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=100;j<=147;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=200;j<=247;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=300;j<=363;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=400;j<=463;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=500;j<=547;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=600;j<=631;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=700;j<=779;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=780;j<(780+112);j++,i++)
                from_internal_to_dialled_no[i]=j;
#endif

#ifdef DARAI_ZANJAN
        i=0;
        for(j=1;j<=40;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=101;j<=140;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=201;j<=240;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=301;j<=340;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=401;j<=432;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=501;j<=530;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=601;j<=625;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=701;j<=709;j++,i++)
                from_internal_to_dialled_no[i]=j;
#endif

#ifdef PETRO_GAS
        i=0;
        for(j=1;j<=50;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=101;j<=150;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=201;j<=250;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=301;j<=350;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=401;j<=450;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=501;j<=550;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=601;j<=650;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=701;j<=750;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=801;j<=850;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=901;j<=950;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=1001;j<=1050;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=1101;j<=1158;j++,i++)
                from_internal_to_dialled_no[i]=j;
#endif

#ifdef NAFT100
        for(i=0;i<NO_TOTAL_EXTENSION;i++)
                from_internal_to_dialled_no[i]=i+1;
#endif

#ifdef AMOL
        from_internal_to_dialled_no[ 9]= 32+15;    // 3232-3185 <-> 3185+ 9
        from_internal_to_dialled_no[10]= 33+15;    // 3233-3185 <-> 3185+10
        from_internal_to_dialled_no[11]= 34+15;    // 3234-3185 <-> 3185+11
        from_internal_to_dialled_no[12]= 35+15;    // 3235-3185 <-> 3185+12
        from_internal_to_dialled_no[13]= 42+15;    // 3242-3185 <-> 3185+13
        from_internal_to_dialled_no[14]=133+15;    // 3333-3185 <-> 3185+14

        from_internal_to_dialled_no[ 32+15]= 9;    // 3232
        from_internal_to_dialled_no[ 33+15]=10;    // 3233
        from_internal_to_dialled_no[ 34+15]=11;    // 3234
        from_internal_to_dialled_no[ 35+15]=12;    // 3235
        from_internal_to_dialled_no[ 42+15]=13;    // 3242
        from_internal_to_dialled_no[133+15]=14;    // 3333
#endif
#ifdef IRAN_NEWSPAPER
        i=0;
        for(j=10;j<=46;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=110;j<=160;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=210;j<=260;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=310;j<=330;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=410;j<=425;j++,i++)
                from_internal_to_dialled_no[i]=j;
#endif
#ifdef NEHZAT
        i=0;
        for(j=0;j<=19;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=100;j<=119;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=200;j<=219;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=300;j<=319;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=400;j<=419;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=500;j<=519;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=600;j<=619;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=700;j<=719;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=800;j<=819;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=900;j<=959;j++,i++)
                from_internal_to_dialled_no[i]=j;
#endif
#ifdef BAGHER_ABAD
        i=0;
        for(j=0;j<2000;j++,i++)
                from_internal_to_dialled_no[i]=j;
        for(j=5000;j<6000;j++,i++)
                from_internal_to_dialled_no[i]=j;
#ifdef LAST_PHASE
        for(j=2000;j<5000;j++,i++)
                from_internal_to_dialled_no[i]=j;
#endif
        for(j=6000;j<MAX_EXISTING_NO;j++,i++)
                from_internal_to_dialled_no[i]=j;
#endif
#ifdef ORUM_NUMBERING_PLAN
        {
                unsigned int j,k;
                i=0;
                for(k=0,j=FIRST_OFFSET_IN_DIRECTORY_NUMBER;k<NO_EXTENSION_IN_FIRST_SEGMENT;i++,j++,k++)
                        from_internal_to_dialled_no[i]=j;
#ifdef NO_EXTENSION_IN_SECOND_SEGMENT
                for(k=0,j=SECOND_OFFSET_IN_DIRECTORY_NUMBER;k<NO_EXTENSION_IN_SECOND_SEGMENT;i++,j++,k++)
                        from_internal_to_dialled_no[i]=j;
#endif
#ifdef NO_EXTENSION_IN_THIRD_SEGMENT
                for(k=0,j=THIRD_OFFSET_IN_DIRECTORY_NUMBER;k<NO_EXTENSION_IN_THIRD_SEGMENT;i++,j++,k++)
                        from_internal_to_dialled_no[i]=j;
#endif
#ifdef NO_EXTENSION_IN_FOURTH_SEGMENT
                for(k=0,j=FOURTH_OFFSET_IN_DIRECTORY_NUMBER;k<NO_EXTENSION_IN_FOURTH_SEGMENT;i++,j++,k++)
                        from_internal_to_dialled_no[i]=j;
#endif
#ifdef NO_EXTENSION_IN_FIFTH_SEGMENT
                for(k=0,j=FIFTH_OFFSET_IN_DIRECTORY_NUMBER;k<NO_EXTENSION_IN_FIFTH_SEGMENT;i++,j++,k++)
                        from_internal_to_dialled_no[i]=j;
#endif
                for(;i<NO_TOTAL_EXTENSION;j++,i++)
                        from_internal_to_dialled_no[i]=j;
        }
#endif

        for(i=0;i<MAX_EXISTING_NO;i++)
                from_dialled_to_internal_no[i]=0xffff;

        for(i=0;i<NO_TOTAL_EXTENSION;i++)
                from_dialled_to_internal_no[from_internal_to_dialled_no[i]]=i;
}
/* =======================================================================*/
#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void cas3_hst_sgnl_process(unsigned int parm){
/*
  sys --> opr
  param=count (MAC_CAS3_HISTORICAL_COUNT_REPORT)
  param= TX_REPOT|ABCD bits or RX_REPORT|ABCD bits(MAC_CAS3_HISTORICAL_SIGNAL_REPORT)

  -------------------------------------------------------
 | param | param  | MA_SYS_P | MSB_NIB | LSB_NIB | MA_T  |
 |       |        |          |   MAC   |   MAC   |       |
  -------------------------------------------------------
   8bits   8bits     6bits            6bits       4bits
 <---- parm -----><-------- wt.i ----------------><-wt.t->

   read data from "frommaq" routine :
   INDEX_TYPE wt,unsigned int parm,BYTE sys (which system)
*/
        if ( (parm & 0xc0) == TX_REPORT )
//                fprintf(fhst,"%11.1d%d%d%d",(parm&0x08)>>3,(parm&0x04)>>2,(parm&0x02)>>1,(parm&0x01));
                fprintf(fhst,"----------%d%d%d%d",(parm&0x08)>>3,(parm&0x04)>>2,(parm&0x02)>>1,(parm&0x01));
        else   /* RX_REPORT */
//                fprintf(fhst,"%34.1d%d%d%d",(parm&0x08)>>3,(parm&0x04)>>2,(parm&0x02)>>1,(parm&0x01));
                fprintf(fhst,"---------------------------------%d%d%d%d",(parm&0x08)>>3,(parm&0x04)>>2,(parm&0x02)>>1,(parm&0x01));

}
#endif
#endif
/* =======================================================================*/
#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void em_hst_sgnl_process(unsigned int parm){

        char *gnd="GND";
        char *opn="OPN";

        char *dat;
/*
  sys --> opr
  param=count (MAC_EM_HISTORICAL_COUNT_REPORT)
  param= TX_REPOT|E&M signal or RX_REPORT|E&M signal(MAC_EM_HISTORICAL_SIGNAL_REPORT)

  -------------------------------------------------------
 | param | param  | MA_SYS_P | MSB_NIB | LSB_NIB | MA_T  |
 |       |        |          |   MAC   |   MAC   |       |
  -------------------------------------------------------
   8bits   8bits     6bits            6bits       4bits
 <---- parm -----><-------- wt.i ----------------><-wt.t->

   read data from "frommaq" routine :
   INDEX_TYPE wt,unsigned int parm,BYTE sys (which system)
*/
        if ((parm & 0x0f) == EM_GND_RPRT){
                dat=gnd;
        }
        else{ /* EM_OPN_RPRT */
                dat=opn;
        }

        if ( (parm & 0xc0) == TX_REPORT )
                fprintf(fhst,"%11.3s",dat);
        else   /* RX_REPORT */
                fprintf(fhst,"%34.3s",dat);

}
#endif
#endif

/* =======================================================================*/
#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)

void cas1_hst_sgnl_process(unsigned int parm){
/*
  sys --> opr
  param=count (MAC_CAS1_HISTORICAL_COUNT_REPORT)
  param= TX_REPOT|ABCD bits or RX_REPORT|ABCD bits(MAC_CAS1_HISTORICAL_SIGNAL_REPORT)

  -------------------------------------------------------
 | param | param  | MA_SYS_P | MSB_NIB | LSB_NIB | MA_T  |
 |       |        |          |   MAC   |   MAC   |       |
  -------------------------------------------------------
   8bits   8bits     6bits            6bits       4bits
 <---- parm -----><-------- wt.i ----------------><-wt.t->

   read data from "frommaq" routine :
   INDEX_TYPE wt,unsigned int parm,BYTE sys (which system)
*/
        if ( (parm & 0xc0) == TX_REPORT )
//                fprintf(fhst,"%11.1d%d%d%d",(parm&0x08)>>3,(parm&0x04)>>2,(parm&0x02)>>1,(parm&0x01));
                fprintf(fhst,"----------%d%d%d%d",(parm&0x08)>>3,(parm&0x04)>>2,(parm&0x02)>>1,(parm&0x01));
        else   /* RX_REPORT */
//                fprintf(fhst,"%34.1d%d%d%d",(parm&0x08)>>3,(parm&0x04)>>2,(parm&0x02)>>1,(parm&0x01));
                fprintf(fhst,"---------------------------------%d%d%d%d",(parm&0x08)>>3,(parm&0x04)>>2,(parm&0x02)>>1,(parm&0x01));

}
#endif
#endif
/* =======================================================================*/
#ifdef CAS3_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
void init_cas3_hst_file(void){

        if( (fhst=fopen(CAS3_HSTRCL_FILE,"rt")) == NULL){
                if((fhst=fopen(CAS3_HSTRCL_FILE,"wt")) == NULL){
                        print_error_report("\nOPPC: cannot create historical report file!\n");
                }
                else{
                        fprintf(fhst,"\n                           3-bit CAS HISTORICAL REPORT ");
                        fprintf(fhst,"\n                           ---------------------------\n ");
                        fprintf(fhst,"\n   Count       TX ABCD Bits           RX ABCD Bits");
                        fprintf(fhst,"\n   -----       ------------           ------------");
                }
        }
        else{
                fseek(fhst,0,SEEK_END);
                hstsize=ftell(fhst);
                if(hstsize >= MAX_HST_FILE_SIZE){
                        kt_system(cas3_hst_file_backup_string);
                        if((fhst=fopen(CAS3_HSTRCL_FILE,"wt")) == NULL){
                                print_error_report("\nOPPC: cannot create CAS3 historical report file!\n");
                        }

                        else{
                                fprintf(fhst,"\n                           3-bit CAS HISTORICAL REPORT ");
                                fprintf(fhst,"\n                           ---------------------------\n ");
                                fprintf(fhst,"   Count       TX ABCD Bits           RX ABCD Bits");
                                fprintf(fhst,"   -----       ------------           ------------");
                        }
                }
                else{
                        fclose(fhst);
                        if( (fhst=fopen(CAS3_HSTRCL_FILE,"a+t")) == NULL){
                                print_error_report("\nOPPC: cannot access historical report file!\n");
                        }

                }
        }
}
#endif
#endif
/* =======================================================================*/
#ifdef EM_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
void init_em_hst_file(void){

        if( (fhst=fopen(EM_HSTRCL_FILE,"rt")) == NULL){
                if((fhst=fopen(EM_HSTRCL_FILE,"wt")) == NULL){
                        print_error_report("\nOPPC: cannot create E&M historical report file!\n");
                }
                else{
                        fprintf(fhst,"\n                           E&M HISTORICAL REPORT ");
                        fprintf(fhst,"\n                           ----------------------\n ");
                        fprintf(fhst,"\n   Count       SGNL ON M LEAD         SGNL ON E LEAD");
                        fprintf(fhst,"\n   -----       --------------         --------------");
                }
        }
        else{
                fseek(fhst,0,SEEK_END);
                hstsize=ftell(fhst);
                if(hstsize >= MAX_HST_FILE_SIZE){
                        kt_system(em_hst_file_backup_string);
                        if((fhst=fopen(EM_HSTRCL_FILE,"wt")) == NULL){
                                print_error_report("\nOPPC: cannot create E&M historical report file!\n");
                        }

                        else{
                                fprintf(fhst,"\n                           E&M HISTORICAL REPORT ");
                                fprintf(fhst,"\n                           ----------------------\n ");
                                fprintf(fhst,"   Count       SGNL ON M LEAD         SGNL ON E LEAD");
                                fprintf(fhst,"   -----       --------------         --------------");
                        }
                }
                else{
                        fclose(fhst);
                        if( (fhst=fopen(EM_HSTRCL_FILE,"a+t")) == NULL){
                                print_error_report("\nOPPC: cannot access E&M historical report file!\n");
                        }

                }
        }
}
#endif
#endif
/* =======================================================================*/
#ifdef CAS1_HISTORICAL_REPORT
#if (TON == HISTORICAL_OPR)
void init_cas1_hst_file(void){

        if( (fhst=fopen(CAS1_HSTRCL_FILE,"rt")) == NULL){
                if((fhst=fopen(CAS1_HSTRCL_FILE,"wt")) == NULL){
                        print_error_report("\nOPPC: cannot create historical report file!\n");
                }
                else{
                        fprintf(fhst,"\n                           1-bit CAS HISTORICAL REPORT ");
                        fprintf(fhst,"\n                           ---------------------------\n ");
                        fprintf(fhst,"\n   Count       TX ABCD Bits           RX ABCD Bits");
                        fprintf(fhst,"\n   -----       ------------           ------------");
                }
        }
        else{
                fseek(fhst,0,SEEK_END);
                hstsize=ftell(fhst);
                if(hstsize >= MAX_HST_FILE_SIZE){
                        kt_system(cas1_hst_file_backup_string);
                        if((fhst=fopen(CAS1_HSTRCL_FILE,"wt")) == NULL){
                                print_error_report("\nOPPC: cannot create CAS1 historical report file!\n");
                        }

                        else{
                                fprintf(fhst,"\n                           1-bit CAS HISTORICAL REPORT ");
                                fprintf(fhst,"\n                           ---------------------------\n ");
                                fprintf(fhst,"   Count       TX ABCD Bits           RX ABCD Bits");
                                fprintf(fhst,"   -----       ------------           ------------");
                        }
                }
                else{
                        fclose(fhst);
                        if( (fhst=fopen(CAS1_HSTRCL_FILE,"a+t")) == NULL){
                                print_error_report("\nOPPC: cannot access historical report file!\n");
                        }

                }
        }
}
#endif
#endif
/* =======================================================================*/


/* =======================================================================*/
/* =======================================================================*/
#if (  (defined LAPB_ON_OPERATOR_SIDE) || (defined HDLC_RS232_ON_OPERATOR_SIDE)\
     || (defined TCP_ON_CLIENT_SIDE)    || (defined TCP_ON_SERVER_SIDE) \
     || (defined ARP_INSTALLED)                                         \
     || (defined IP_INSTALLED)                                          \
     || (defined TCP_INSTALLED)                                         \
     || (defined UDP_INSTALLED)                                         \
     || (defined ETHERNET_INSTALLED)                                    \
    )
void init_mess(void){
#ifdef LAPB_ON_OPERATOR_SIDE
        mess[LAPB_ERROR]="LAPB_ERROR";
        mess[LAPB_RTN]="LAPB_RTN";
        mess[ICERROR]="ICERROR";
#endif

#ifdef HDLC_RS232_ON_OPERATOR_SIDE
        mess[SERIAL_ERROR]="SERIAL_ERROR";
        mess[SERIAL_RTN]="SERIAL_RTN";
#endif
#if ((defined LAPB_ON_OPERATOR_SIDE) || (defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined FIFO_RD_WR))
        mess[BUF_OVERFLOW]="BUF FULL";
#endif
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
        mess[TCP_ERR]="TCP ERR";
#endif

#ifdef ARP_INSTALLED
        mess[ARP_ERR]="ARP_ERROR";
#endif
#ifdef IP_INSTALLED
        mess[IP_ERR]="IP_ERROR";
#endif
#ifdef TCP_INSTALLED
        mess[TCP_ERR]="TCP_ERROR";
#endif
#ifdef UDP_INSTALLED
        mess[UDP_ERR]="UDP_ERROR";
#endif
#ifdef ETHERNET_INSTALLED
        mess[ETHERNET_ERR]="ETHERNET_ERROR";
#endif
        mess[ERROR]="ERROR";
}
#endif
/* =======================================================================*/
/* =======================================================================*/
void set_stm_for_up_down_load(void){
        stm= 0xff00 | 0x40 | 0x10 | 2 | 1;
}
/* =======================================================================*/
/* =======================================================================*/
void set_stm_for_display(void){
        stm = 0xff00 | OHH_BIT_POS | KBS_T_BIT_POS;
}
/* =======================================================================*/
/* =======================================================================*/
#ifdef HOT_STANDBY
void set_stm_for_htby_check(void){
        stm =  0x7f00 | 0x40 | 0x10;
//      stm = 0x7f50;
}
#endif
/* =======================================================================*/
/* =======================================================================*/
#ifdef NEW_DEP
void set_stm_for_dep(void){

        stm = 0xff00 | 0x80 | 0x40 | 0x10 | 0x8 | 0x4 | 0x1;
//      stm = 0xffdd;
}
#endif
/* =======================================================================*/
/* =======================================================================*/
void set_stm_for_term(void){
        stm = 0x7f00 | 0x80 | 0x10 | 0x8 | 0x4 | 0x1;
}
/* =======================================================================*/
/* =======================================================================*/
void unmask_stm(void){
        stm = 0xffff;
}
/* =======================================================================*/
/* =======================================================================*/
#ifdef HOT_STANDBY
void htby_wait_win(void){

        htby_win=enw(27,8,9,26);
        if(htby_win == NULL)
                return ;
        dw(htby_win);
        swb(htby_win,DOUBLE_LINE);
//      swc(htby_win,ALL,BLUE,WHITE,BRIGHT);
        swc(htby_win,ALL,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
        draw_window(htby_win);
        wprompt(htby_win,6,1,"Please Wait!");
        wprompt(htby_win,6,2,"ƒƒƒƒƒƒƒƒƒƒƒƒ");
        wprompt(htby_win,3,4,"Press ESC to quit.");
        pc(80,25);
        return;
}
#endif
/* =======================================================================*/
/* =======================================================================*/

#ifdef HOT_STANDBY
void mac_this_sys_is_hot_rtn(OMAR a){

#ifdef DYNAMIC_IP_ALLOCATION
        unsigned int old_button;
#endif
       // if "opr.exe" is executed before "s.exe",
       // this packet is received before MAC_HOT_SYSTEM_ANSWER
       // ignore it.
        if (hot_link != UNKNOWN_LINK){
//              link_flag[hot_link]=RESET;

#ifdef DYNAMIC_IP_ALLOCATION
                if(a.parm == FROM_MASTER_SYSTEM){
                        ms_button[a.htby]=0;old_button=1;
                }
                else{
                        ms_button[a.htby]=1;old_button=0;
                }
                if(a.htby == LAPB_LINK0)
                        ms_button[LAPB_LINK1]=old_button;
                else
                        ms_button[LAPB_LINK0]=old_button;
                set_led_attr(sync_led[old_button],KT_YELLOW,KT_WHITE,KT_NO_BLINKING);
                set_led_attr(link_up_led[old_button],KT_YELLOW,KT_WHITE,KT_NO_BLINKING);
                hot_link=a.htby;
                set_led_attr(sync_led[ms_button[a.htby]],KT_RED,KT_WHITE,KT_NO_BLINKING);
                set_led_attr(link_up_led[ms_button[a.htby]],KT_RED,KT_WHITE,KT_NO_BLINKING);
#else
                set_led_attr(sync_led[hot_link],KT_YELLOW,KT_WHITE,KT_NO_BLINKING);
                set_led_attr(link_up_led[hot_link],KT_YELLOW,KT_WHITE,KT_NO_BLINKING);
                hot_link=a.htby;
                set_led_attr(sync_led[hot_link],KT_RED,KT_WHITE,KT_NO_BLINKING);
                set_led_attr(link_up_led[hot_link],KT_RED,KT_WHITE,KT_NO_BLINKING);
#endif

#if 0
#ifdef DISPLAY_STATUS_ACT
               if (dxsf == SET)
                       pcpmftx(X_STATUS_REQ_ACT,TON,OPR_T);
                       /* if opr has been in "H" page during
                          swchover ,the new hot system should
                          be informed.
                       */
#endif

#endif // 0
//PIANO
               reset_calls_report_status();
       }
}
#endif
/* =======================================================================*/
/* =======================================================================*/
#ifdef HOT_STANDBY
void check_htby_link_handler(void){

        if (htby_link_flag == SET){
                led_set(SYNC_LED3);
                led_set(LINK_UP_LED3);
        }
        else{
                led_reset(SYNC_LED3);
                led_reset(LINK_UP_LED3);
        }
}
#endif
/* =======================================================================*/
/* =======================================================================*/
#ifdef PARAMETER_DOWN_UP_LOAD
#ifdef OPR_UPLOAD_WITH_TIMER
void mac_upload_finished_rtn(OMAR a){

#ifdef DAY_NIGHT_PARAMETERS_FILE
        INDEX_TYPE p;
#endif
#ifdef HOT_STANDBY
//---------------
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
        if (a.htby == LAPB_LINK0)
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
        if (a.htby == tcp_link_0)
#endif
#ifdef MY_TCPIP_FOR_OAM
        if (a.htby == 0)
#endif
        { // LINK0
                if(f & PARM_UP_1){
                        f &= ~PARM_UP_1;
#if (NO_OPRTR > 1)
                        upload_reject_flag &=~PARM_UP_1;
#endif
                        if (f & PARM_UP_2){ // upload of LINK1 has not finished yet.
                                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(".ñ®ê˘óÍë˛˘ıóë° Ò¯êÙó®˛® ¸ë˚§óıê§ëï Òë®§ê");
                                }
                                else
#endif
                                {
                                        str2lcd("UP LOAD OF SYSTEM 1 FINISHED.           ");
                                }
                                return;
                        }
                }
                else return;
        }
        else{    // STANDBY
                if(f & PARM_UP_2){
                        f &= ~PARM_UP_2;
#if (NO_OPRTR > 1)
                        upload_reject_flag &=~PARM_UP_2;
#endif
                        if (f & PARM_UP_1){ // upload of LINK0 has not finished yet.
                                tolcdq(0xff,CLEAR_LCD);
#ifdef OPERATOR_FARSI
                                if(f & OPR_LANG_FLAG){
                                        str2lcd(".ñ®ê˘óÍë˛˘ıóë° Ù¯¢Ùó®˛® ¸ë˚§óıê§ëï Òë®§ê");
                                }
                                else
#endif
                                {
                                        str2lcd("UP LOAD OF SYSTEM 2 FINISHED.           ");
                                }
                                return;
                        }
                }
                else return;
        }
#if (NO_OPRTR > 1)
        upload_reject_flag=RESET;
#endif
//---------------
#else  // HOT_STANDBY
        f &= ~PARM_UP_1;
//---------------
#endif // HOT_STANDBY
        unmask_stm();  /* unmask all tasks */
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("        .ñ®ê ˘óÍë˛ ˘ıóë° ë˚§óıê§ëï Òë®§ê");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                        if(ocr.state==OPR_PROG)
                                str2lcd("           :¢¯™˛ı •ëËç ˝¢˛¢õ ¸•˛§ ˘ıë˜§ì");
        }
        else{
#endif
                str2lcd("UP LOAD FINISHED .                      ");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                        if(ocr.state==OPR_PROG)
                                str2lcd("START NEW PROGRAM:");
#ifdef OPERATOR_FARSI
       }
#endif

//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
#ifdef LIM
        if(oprnight == 1){
                p.t=OPR_T;
                rtn101_2(p);
        }
        else if(oprday == 1){
                p.t=OPR_T;
                rtn102_2(p);
        }
#else
        if(oprnight == 1){
                p.t=OPR_T;
                rtn101_1(p);
        }
        else if(oprday == 1){
                p.t=OPR_T;
                rtn102_1(p);
        }
#endif
#endif
//end night upload
}
#endif
#endif
/* =======================================================================*/
/* =======================================================================*/
#ifdef OPR_UPLOAD_WITH_TIMER
#ifdef PARAMETER_DOWN_UP_LOAD
void upload_handler(void){

#ifdef DAY_NIGHT_PARAMETERS_FILE
        INDEX_TYPE p;
#endif


#if (NO_OPRTR > 1)
        if( (ocr.state == AWAIT_SYS_UPLOAD_ACCEPTANCE_1)){
                if (upload_count-- == 0){
#ifdef HOT_STANDBY
                        upload_reject_flag=RESET;
                        if ((f&PARM_UP_1) || (f&PARM_UP_2))
                                upload_count=UPLOAD_PARM_TICK;
#endif
                        ocr.d.t=NULL_T;
                        ocr.s.t=NULL_T;
                        ocr.mf=PROG_ACT;
                        ocr.state=OPR_PROG;
#ifdef OPERATOR_FARSI
                        if(f & OPR_LANG_FLAG){
                                str2lcd("  .¢™ ¸§ï® Ùó®˛® ñı®•ê †®ëï ñÍë˛§¢ ˆëı• " );
                                str2lcd("            :¢¯™˛ı •ëËç ¢˛¢õ ¸•˛§˘ıë˜§ì ");
                        }
                        else
                        {
                                str2lcd("UPLOAD ACCEPT TIME OUT.                 ");
#ifdef HOT_STANDBY
                                if ( (f&(PARM_UP_1 | PARM_UP_2)) == 0 )
#endif
                                         str2lcd("START NEW PROGRAM:                     ");
                        }
#else
                        init_lcd();
                        str2lcd("UPLOAD ACCEPT TIME OUT.                 ");
#ifdef HOT_STANDBY
                        if ( (f&(PARM_UP_1 | PARM_UP_2)) == 0 )
#endif
                                str2lcd("START NEW PROG.:                       ");
#endif
                }

                return;
        }
#endif
#ifdef HOT_STANDBY
        if ( (f&(PARM_UP_1 | PARM_UP_2)) == 0 )
#else
        if (( f & PARM_UP_1) == 0 )
#endif
                return;    /* upload is not active.*/

        if (upload_count-- != 0){
                if ((upload_count % 50) == 0)
                       str2lcd("A ");
        }
        else{
                if ( f & PARM_UP_1 ){ // unsuccessful upload on LINK0
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                        initialize_network_buf(LAPB_LINK0);
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                        if (tcp_link_0 < max_sys_oam_tcp_link){
                                initialize_tcp_network_buf(tcp_link_st[tcp_link_0].tcp_no);
                        }
#endif
                        f &= ~PARM_UP_1;
                }
#ifdef HOT_STANDBY
                if ( f & PARM_UP_2 ){ // unsuccessful upload on LINK1
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
                       initialize_network_buf(LAPB_LINK1);
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                        if (tcp_link_1 < max_sys_oam_tcp_link){
                                initialize_tcp_network_buf(tcp_link_st[tcp_link_1].tcp_no);
                        }
#endif
                        f &= ~PARM_UP_2;
                }
#if (NO_OPRTR > 1)
                upload_reject_flag=RESET;
#endif
#endif // HOT_STANDBY
                unmask_stm();  /* unmask all tasks */
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd(" .¢™ ¸§ï® Ùó®˛® ˘ì ë˚§óıê§ëï Òë®§ê ˆëı• " );
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                        if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                                if(ocr.state==OPR_PROG)
                                        str2lcd("           :¢¯™˛ı •ëËç ˝¢˛¢õ ¸•˛§ ˘ıë˜§ì");
                }
                else{
#endif
                         str2lcd("UP LOAD TIME OUT.                       ");
//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
                         if((oprnight == 0) && (oprday == 0))
#endif
//end night upload
                                 if(ocr.state==OPR_PROG)
                                          str2lcd("START NEW PROGRAM:");
#ifdef OPERATOR_FARSI
                }
#endif

//night upload
#ifdef DAY_NIGHT_PARAMETERS_FILE
#ifdef LIM
                if(oprnight == 1){
                        p.t=OPR_T;
                        rtn101_2(p);
                }
                else if(oprday == 1){
                        p.t=OPR_T;
                        rtn102_2(p);
                }
#else
                if(oprnight == 1){
                       p.t=OPR_T;
                       rtn101_1(p);
                }
                else if(oprday == 1){
                       p.t=OPR_T;
                       rtn102_1(p);
                }
#endif
#endif
//end night upload
        }
}
#endif
#endif

/* =======================================================================*/
/* =======================================================================*/
void send_fdn(void){
        int i,j,k;

        i= (unsigned int)(f_d_n & 0xffffL);
#ifdef HOT_STANDBY
        pmamftx_both_sys(MA_SYS_P | MAC_FDN1,i,OPR_T,0);
#else
        pmamftx(MA_SYS_P | MAC_FDN1,i,OPR_T,0);
#ifdef LIM
        pmamftx(MA_SYS_P | MAC_FDN1,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
        pmamftx(MA_SYS_P | MAC_FDN1,i,OPR_T,2);
#endif
#endif
#endif
        i= (unsigned int)((f_d_n >> 16) & 0xffffL);
#ifdef HOT_STANDBY
        pmamftx_both_sys(MA_SYS_P | MAC_FDN2,i,OPR_T,0);
#else
        pmamftx(MA_SYS_P | MAC_FDN2,i,OPR_T,0);
#ifdef LIM
        pmamftx(MA_SYS_P | MAC_FDN2,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
        pmamftx(MA_SYS_P | MAC_FDN2,i,OPR_T,2);
#endif
#endif
#endif

/*
        i= (unsigned int)((f_d_n & 0x3ffL) | 0x400);
#ifdef HOT_STANDBY
        pmamftx_both_sys(MA_SYS_P | MAC_FDN,i,OPR_T,0);
#else
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,0);
#ifdef LIM
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,2);
#endif
#endif
#endif
        i= (unsigned int)(((f_d_n >> 10)& 0x3ffL) | 0x800);
#ifdef HOT_STANDBY
        pmamftx_both_sys(MA_SYS_P | MAC_FDN,i,OPR_T,0);
#else
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,0);
#ifdef LIM
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,2);
#endif
#endif
#endif
        i= (unsigned int)(((f_d_n >> 20)& 0x3ffL) | 0xc00);
#ifdef HOT_STANDBY
        pmamftx_both_sys(MA_SYS_P | MAC_FDN,i,OPR_T,0);
#else
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,0);
#ifdef LIM
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
        pmamftx(MA_SYS_P | MAC_FDN,i,OPR_T,2);
#endif
#endif
#endif
*/

/*
        for(i=0;ss[i] != 0xffff;i++){
#ifdef HOT_STANDBY
                pmamftx_both_sys(MA_SYS_P | MAC_SS,ss[i],OPR_T,0);
#else
                pmamftx(MA_SYS_P | MAC_SS,ss[i],OPR_T,0);
#ifdef LIM
                pmamftx(MA_SYS_P | MAC_SS,ss[i],OPR_T,1);
#if(NO_EXTENSION_C != 0)
                pmamftx(MA_SYS_P | MAC_SS,ss[i],OPR_T,2);
#endif
#endif
#endif
        }
*/

        for(k=0;k<=(89+10);k++){
                if(buf110[k][0]=='\0')       continue;
                i=k+(110-10);
#ifdef HOT_STANDBY
                pmamftx_both_sys(MA_SYS_P | MAC_SS,i,OPR_T,0);
#else
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,0);
#ifdef LIM
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,2);
#endif
#endif
#endif
                for(j=0;;j++){
                        if(buf110[k][j] == '\0') break;
                        i=buf110[k][j]-'0';
#ifdef HOT_STANDBY
                        pmamftx_both_sys(MA_SYS_P | MAC_SS,i,OPR_T,0);
#else
                        pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,0);
#ifdef LIM
                        pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
                        pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,2);
#endif
#endif
#endif
                }
                i=15;   // End of Digits
#ifdef HOT_STANDBY
                pmamftx_both_sys(MA_SYS_P | MAC_SS,i,OPR_T,0);
#else
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,0);
#ifdef LIM
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,2);
#endif
#endif
#endif
        }

        for(k=0;k<10;k++){
                if(buf1819_1[k] == (unsigned int)(-1)) break;;
                if(buf1819_2[k][0]=='\0')       continue;
#ifdef HOT_STANDBY
                pmamftx_both_sys(MA_SYS_P | MAC_SS,buf1819_1[k],OPR_T,0);
#else
                pmamftx(MA_SYS_P | MAC_SS,buf1819_1[k],OPR_T,0);
#ifdef LIM
                pmamftx(MA_SYS_P | MAC_SS,buf1819_1[k],OPR_T,1);
#if(NO_EXTENSION_C != 0)
                pmamftx(MA_SYS_P | MAC_SS,buf1819_1[k],OPR_T,2);
#endif
#endif
#endif
                for(j=0;;j++){
                        if(buf1819_2[k][j] == '\0') break;
                        i=buf1819_2[k][j]-'0';
#ifdef HOT_STANDBY
                        pmamftx_both_sys(MA_SYS_P | MAC_SS,i,OPR_T,0);
#else
                        pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,0);
#ifdef LIM
                        pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
                        pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,2);
#endif
#endif
#endif
                }
                i=14;   // End of Digits
#ifdef HOT_STANDBY
                pmamftx_both_sys(MA_SYS_P | MAC_SS,i,OPR_T,0);
#else
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,0);
#ifdef LIM
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,1);
#if(NO_EXTENSION_C != 0)
                pmamftx(MA_SYS_P | MAC_SS,i,OPR_T,2);
#endif
#endif
#endif
        }
}

/* =======================================================================*/
/* =======================================================================*/
//extern BYTE scrn_save[256][80];
extern char command_buf[120];
extern BYTE buf_ptr;
extern BYTE scrn_save[256][80];
extern BYTE scrn_save_line;
extern BYTE scrn_save_column;
extern BYTE command_buf_save[120];
extern BYTE buf_ptr_save;
extern BYTE scroll_pointer;
extern BYTE *dos_key[32];
extern BYTE dos_key_ptr;
extern BYTE dos_key_current_ptr;
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void terminal(void){
        BYTE a,h,i,j,k,*ptr;
#ifdef OPERATOR_PC
        a=get_c();
#endif
        if(a == NO_KEY_PRESSED)
                return;
        if(a == PGUP){
                if(scroll_pointer < (256-25)){
                        scroll_pointer+=25;
                        k=scrn_save_line-scroll_pointer;
                        for(j=0;j<25;j++,k++){
                                for(i=0;i<80;i++){
                                        displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                                }
                        }
                        cursor(0,24);
                }
                return;
        }
        else if(a == PGDN){
                if(scroll_pointer >= (24+1)){
                        scroll_pointer-=25;
                        if(scroll_pointer <= 24)
                                restore_terminal();
                        else{
                                k=scrn_save_line-scroll_pointer;
                                for(j=0;j<25;j++,k++){
                                        for(i=0;i<80;i++){
                                                displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                                        }
                                }
                                cursor(0,24);
                        }
                }
                return;
        }
        else if(a == UP){
                if(scroll_pointer < (256-1)){
                        scroll_pointer++;
                        k=scrn_save_line-scroll_pointer;
                        for(j=0;j<25;j++,k++){
                                for(i=0;i<80;i++){
                                        displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                                }
                        }
                        cursor(0,24);
                }
                return;
        }
        else if(a == DN){
                if(scroll_pointer >= (24+1)){
                        scroll_pointer--;
                        if(scroll_pointer == 24)
                                restore_terminal();
                        else{
                                k=scrn_save_line-scroll_pointer;
                                for(j=0;j<25;j++,k++){
                                        for(i=0;i<80;i++){
                                                displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                                        }
                                }
                                cursor(0,24);
                        }
                }
                return;
        }
        if(scroll_pointer != 24){
                restore_terminal();
        }
        if(a == BACKSPACE){
                if(buf_ptr > 0){
                        buf_ptr--;
                        print_char(BACKSPACE);
                        print_char(' ');
                        print_char(BACKSPACE);
                }
                return;
        }
        else if((a == F5) || (a == ESC)){
                for(i=0;i<buf_ptr;i++)
                        print_char(BACKSPACE);
                for(i=0;i<buf_ptr;i++)
                        print_char(' ');
                for(i=0;i<buf_ptr;i++)
                        print_char(BACKSPACE);
                buf_ptr=0;
                return;
        }
        else if(a == F3){
                for(i=buf_ptr;i<buf_ptr_save;i++){
                        print_char(command_buf[i]=command_buf_save[i]);
                }
                buf_ptr=i;
                return;
        }
        else if(a == F1){
                if(buf_ptr<buf_ptr_save){
                        print_char(command_buf[buf_ptr]=command_buf_save[buf_ptr]);
                        buf_ptr++;
                }
                return;
        }
        else if(a == BS){
                if(dos_key_current_ptr<31){
                        for(i=0;i<buf_ptr;i++)
                                print_char(BACKSPACE);
                        for(i=0;i<buf_ptr;i++)
                                print_char(' ');
                        for(i=0;i<buf_ptr;i++)
                                print_char(BACKSPACE);
                        dos_key_current_ptr++;
                        j=(dos_key_ptr-dos_key_current_ptr)&0x1f;
                        if(dos_key[j] != (BYTE *)0){
                                for(i=0;;i++){
                                        command_buf[i]=dos_key[j][i];
                                        if(command_buf[i]=='\0')
                                                break;
                                        else
                                                print_char(command_buf[i]);
                                }
                                buf_ptr=i;
                        }
                        else
                                buf_ptr=0;
                }
                return;
        }
        else if(a == FWD){
                if(dos_key_current_ptr>0){
                        for(i=0;i<buf_ptr;i++)
                                print_char(BACKSPACE);
                        for(i=0;i<buf_ptr;i++)
                                print_char(' ');
                        for(i=0;i<buf_ptr;i++)
                                print_char(BACKSPACE);
                        dos_key_current_ptr--;
                        j=(dos_key_ptr-dos_key_current_ptr)&0x1f;
                        if(dos_key[j] != (BYTE *)0){
                                for(i=0;;i++){
                                        command_buf[i]=dos_key[j][i];
                                        if(command_buf[i]=='\0')
                                                break;
                                        else
                                                print_char(command_buf[i]);
                                }
                                buf_ptr=i;
                        }
                        else
                                buf_ptr=0;
                }
                else{
                        for(i=0;i<buf_ptr;i++)
                                print_char(BACKSPACE);
                        for(i=0;i<buf_ptr;i++)
                                print_char(' ');
                        for(i=0;i<buf_ptr;i++)
                                print_char(BACKSPACE);
                        buf_ptr=0;
                }
                return;
        }
        else if( a == '\\'){
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
                ncurses_attrset(KT_COLOR_PAIR((KT_BLACK<<3|KT_WHITE)+1)|KT_A_ALTCHARSET);
                ncurses_scrollok(0);
#endif
#endif
                term_flag=RESET;
#if (OS == DOS)
                textattr(0x07);
#endif
                unmask_stm();
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
                for(h=0;h<10;h++)
                        if(pl[h].t != NULL_T)
                                turn_on_park_led(h);

                if( (trf & TONE_RNG_ACT) == RESET)  /* tone ringer disable */
                        led_set(TONE_RNGR_LED);
                else
                        led_reset(TONE_RNGR_LED);
                return;
        }
        else if(a == F2){
            FILE *fp=fopen("terminal.log","wb");
            unsigned int jj;
            for(jj=0;jj<256;jj++){
                for(i=0;i<80;i++){
                    putc(scrn_save[jj][i],fp);
                }
                putc('\r',fp);
                putc('\n',fp);
            }
            fclose(fp);
        }
        else{
                print_char(a);
#if (OS == DOS)
                if(a == CR)
                        print_char(LF);
#endif
                if(buf_ptr < 118) // '\0' should also be added.
                        command_buf[buf_ptr++]=a;
        }
        if (a == CR) {
                kt_send();
                ptr=malloc(buf_ptr);
                for(i=0;i<buf_ptr-1;i++){       // do not copy the ending CR
                        ptr[i]=command_buf_save[i]=command_buf[i];
                }
                execute();      // as execute uses strtok(command_buf,...), and it makes command_buf corrupted, hence we should call execute AFTER saving command_buf
                ptr[i]='\0';
                if(dos_key[dos_key_ptr] != (BYTE *)0)
                        free(dos_key[dos_key_ptr]);
                dos_key[dos_key_ptr]=ptr;
                dos_key_ptr=(dos_key_ptr+1) & 0x1f;
                dos_key_current_ptr=0;
                buf_ptr_save=buf_ptr-1;
                buf_ptr=0;
        }
}
#endif
void print_char_to_log_file(BYTE c){
#ifdef OPERATOR_PC
        putc(c,opr_log_fp);
        if(c == CR)
                putc(LF,opr_log_fp);
#endif
}
void kt_send(void){
        BYTE i,j;
        BYTE send_buf[MAX_OPR_SYS_PCKT_SIZE];
        send_buf[0]=TERM_T;
        for(j=0;j<buf_ptr;){
                for(i=1;(i<MAX_OPR_SYS_PCKT_SIZE) && (j<buf_ptr);i++){
                        send_buf[i]=command_buf[j++];
                        print_char_to_log_file(send_buf[i]);
                }
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#ifdef HOT_STANDBY
                to_nettx_que(send_buf,i,LAPB_LINK0);
                to_nettx_que(send_buf,i,LAPB_LINK1);
#else
                to_nettx_que(send_buf,i,hot_link);
#endif
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
#ifdef HOT_STANDBY
                if((tcp_link_0 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_0].tcp_no < max_tcp_connection)){
                        put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_0].tcp_no].nettx_st),i,send_buf);
                }
                else{
                        print_error_report("put_pckt_to_variable_size_buf failed11\n");
                }
                if((tcp_link_1 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_1].tcp_no < max_tcp_connection)){
                        put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_1].tcp_no].nettx_st),i,send_buf);
                }
                else{
                        print_error_report("put_pckt_to_variable_size_buf failed12\n");
                }
#else
                if((hot_link < max_sys_oam_tcp_link) && (tcp_link_st[hot_link].tcp_no < max_tcp_connection)){
                        put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st),i,send_buf);
                }
                else{
                        print_error_report("put_pckt_to_variable_size_buf failed\n");
                }
#endif
#endif
#ifdef MY_TCPIP_FOR_OAM
#ifdef HOT_STANDBY
                send_oam_pckt_to_endpoint(0,i,send_buf);
                send_oam_pckt_to_endpoint(1,i,send_buf);
#else
                send_oam_pckt_to_endpoint(hot_link,i,send_buf);
#endif
#endif
        }
}
void message_to_term_q(BYTE size){
        BYTE i;
        if(term_flag == RESET)
                return;
        for(i=1;i<size;i++){
                print_char(mb[i]);
                print_char_to_log_file(mb[i]);
#if (OS == DOS)
                if(mb[i]==CR)
                        print_char(LF);
#endif
        }
}
void init_terminal(void){
        int i,j;
        for(i=0;i<256;i++)
                for(j=0;j<80;j++)
                        scrn_save[i][j]=' ';
        buf_ptr=0;
        term_flag=RESET;
        scrn_save_line=0;
        scrn_save_column=0;
        buf_ptr_save=0;
        scroll_pointer=24;
        dos_key_ptr=0;
        dos_key_current_ptr=0;
        for(i=0;i<31;i++)
                dos_key[i]=(BYTE *)0;
}
void print_char(BYTE c){
#if (OS == DOS)
        putch(c);
#endif
#if(OS ==  LINUX_COMPATIBLE)
        if(c == CR)
                ncurses_addch('\n' | TERM_ATTR);
        else
                ncurses_addch(c | TERM_ATTR);
#endif
        if(c==LF){
                return;
        }
        if(c==CR){
                while(scrn_save_column<80){
                        scrn_save[scrn_save_line][scrn_save_column++]=' ';
                }
                scrn_save_column=0;
                scrn_save_line++;
                return;
        }
        if(c==BACKSPACE){
                if(scrn_save_column > 0)
                        scrn_save_column--;
                return;
        }
        scrn_save[scrn_save_line][scrn_save_column++]=c;
        if(scrn_save_column==80){
                scrn_save_column=0;
                scrn_save_line++;
        }
}
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void restore_terminal(void){
#ifdef OPERATOR_PC
        BYTE i,j,k;

#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
        ncurses_attrset(TERM_ATTR);
        ncurses_bkgdset(TERM_ATTR);
        ncurses_scrollok(1);
#endif
#endif
        scroll_pointer=24;
        k=scrn_save_line-scroll_pointer;
        for(j=0;j<25;j++,k++){
                for(i=0;i<80;i++){
                        displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                }
        }
        cursor(scrn_save_column,24);
#endif
}
#endif
void execute(void){
        char *p;
        command_buf[buf_ptr]='\0';
        if(buf_ptr==0) return;
        p = strtok(command_buf, " ");
        if(p == NULL){
                goto end;
        }
#if ((defined EXT_INSTALLED) && (defined CALL_PROCESSOR))
        if(strcmp(p,"SETSUBNO") == 0){
                unsigned int i,j,dat;
                p=strtok(NULL," ");
                if(p == NULL){
                        goto end;
                }
                i=atoi(p);
                i-= hex2l(f_d_n);
                if (i >= MAX_EXISTING_NO){
                        goto end;
                }
                if(from_dialled_to_internal_no[i] >= NO_TOTAL_EXTENSION){
                        goto end;
                }
                p=strtok(NULL," ");
                if(p == NULL){
                        goto end;
                }
                j=atoi(p);
                j-= hex2l(f_d_n);
                if (j >= MAX_EXISTING_NO){
                        goto end;
                }
                if(from_dialled_to_internal_no[j] < NO_TOTAL_EXTENSION){
                        goto end;
                }
                dat=from_dialled_to_internal_no[j]=from_dialled_to_internal_no[i];
                from_dialled_to_internal_no[i]=0xffff;
                from_internal_to_dialled_no[dat]=j;
                goto end;
        }
        else
#endif
        {
        }
end:
        return;
}
#ifdef OPERATOR_PC
void send_time_date_to_sys(DATTIM *dt){
        int i[6],j;
        // pmamftx can send total 16 bits, however if in system side
        // BEHNAMIR is not defined, pt.i shall be only 12 bits in martn.c
        i[0] = (unsigned int)(dt->se | 0x0000);
        i[1] = (unsigned int)(dt->mi | 0x0200);
        i[2] = (unsigned int)(dt->hr | 0x0400);
        i[3] = (unsigned int)(dt->da | 0x0600);
        i[4] = (unsigned int)(dt->mo | 0x0800);
        i[5] = (unsigned int)(dt->yr | 0x0a00);
        for(j=0;j<6;j++)
#ifdef HOT_STANDBY
                pmamftx_both_sys(MA_SYS_P | MAC_TIME,i[j],OPR_T,0);
#else
                pmamftx(MA_SYS_P | MAC_TIME,i[j],OPR_T,0);
#endif
}
void update_time(void){
        DATTIM dt;
        if( (++update_time_index) >= UPDATE_TIME_INTERVAL){
                if(get_and_check_dt(&dt) == FAIL)
                        return;
#ifdef SEND_TIME_DATE_FROM_OPR_TO_SYS
                send_time_date_to_sys(&dt);
#endif
                correct_given_time(&dt);
                update_time_index=0;
        }
}
#endif

#if (COMPILER == GCC)
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void fcloseall(void){
#ifdef NET_DEBUG
        fclose(net_fp);
#endif
#ifdef OPERATOR_PC
        fclose(fer);
        fclose(opr_gonz_fp);
        fclose(opr_log_fp);
#endif
#ifdef LINKLAYER_DEBUG_ACT
        close_lapb_err_file();
#endif
#ifdef TCP_DEBUG_ACT
        close_tcp_err_file();
#endif
}
#endif
#endif

#if ((defined CALL_PROCESSOR) && (defined EXT_INSTALLED))
void init_subscriber_groups_data(void){
        no_of_subscriber_groups=0;
#ifdef PC_COMPATIBLE
        {
            FILE *fp;
            char line[50];
            unsigned int group_no,i;
            SDNST directory_start;
            unsigned long port_start=0L,no_of_ports;
            if ((fp = fopen ("subs.dat", "rb")) == NULL)
                goto next1;
            while(fgets(line,50,fp) != NULL){
                if(line[0]=='*') continue;
                if(sscanf(line,"%d "SDNS_FS" %ld",&group_no,&directory_start,&no_of_ports) == 3){
                    subscriber_group[no_of_subscriber_groups].group_no=group_no;
                    subscriber_group[no_of_subscriber_groups].directory_start=directory_start;
                    subscriber_group[no_of_subscriber_groups].no_of_ports=no_of_ports;
                    subscriber_group[no_of_subscriber_groups].port_start=port_start;
                    port_start+=no_of_ports;
                    for(i=0;directory_start != 0L; directory_start/=10L,i++);
                    subscriber_group[no_of_subscriber_groups].no_of_digits=i;
                    no_of_subscriber_groups++;
                    if(no_of_subscriber_groups >= MAX_NO_OF_SUBSCRIBER_GROUPS)
                        break;
                }
            }
            fclose(fp);
next1:
            if ((fp = fopen ("numbers.dat", "rb")) == NULL)
                goto next2;
            no_of_subscriber_groups=0;  // override possible subs.dat
            while(fgets(line,50,fp) != NULL){
                unsigned int internal_no,directory_no;
                if(line[0]=='*') continue;
                if(sscanf(line,"%d %d",&internal_no,&directory_no) == 2){
                    if((internal_no < NO_STATIC_EXTENSION) && (directory_no < MAX_EXISTING_NO)){
                        from_internal_to_dialled_no[internal_no]=directory_no;
                        from_dialled_to_internal_no[directory_no]=internal_no;
                    }
                }
            }
            fclose(fp);
next2:
            return;
        }
#endif
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
#define KT_KURT_EXPLICIT
//#define KT_KURT_ANYTIME
#define TARGET_RT_ID            1
int main(void)
{
    struct rtparams rt_param;
    pthread_attr_t attr1,attr2;

    fprintf(stderr,"kurt main() is called\n");
    kurtdev=kurt_open();
    if (kurtdev == -1){
        fprintf(stderr,"kurt_open failed\n");
        exit(1);
    }
    rt_param.rt_id = TARGET_RT_ID/*ASSIGN_RT_ID*/;
    rt_param.period = 0;
    rt_param.exec_time = 300000000;
#ifdef KT_KURT_EXPLICIT
    rt_param.rt_mode = KURT_EXPLICIT | KURT_EPISODIC;
#endif
#ifdef KT_KURT_ANYTIME
    rt_param.rt_mode = KURT_ANYTIME | KURT_CONTINUOUS;
#endif
        //rt_param.processor = 0;
    rt_param.rt_name[0] = '\0';

    init_all_pthread_sync_device();

    /* if a thread requires joining ,consider explicitly creating it as joinable */
    /* this provides portability as not all implementations may create threads as*/
    /* joinable by default */

    iopl(3);
// for my tests I have redirected  print_error_report to tcp_fer file.
// and if I put this block in kt_all_initialize routine,kt_dynamic does not
// work.
#ifdef TCP_DEBUG_ACT
    open_tcp_err_file();
#endif

    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
    pthread_create(&init_thread, &attr1, init_thread_program, NULL);

    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);
    pthread_create(&etc_thread, &attr2, etc_thread_program, NULL);

    sleep(5);
    if(set_rtparams(kurtdev,0, RT_REGISTER, &rt_param) < 0){
        fprintf(stderr,"set_rtparams failed\n");
        close(kurtdev);
        exit(1);
    }
    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2);

    kt_ncurses_init();

    pthread_cond_signal (&init_cond_flag);
    print_error_report("Real-time process %u entering work loop...\n",rt_param.rt_id);
#if (OS == LINUX_COMPATIBLE)
#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
    //open_user_space_isa_mem();
#endif
#endif
    while(1){
        rt_suspend(kurtdev, SUSPEND_IF_NRT /*| START_SCHED*/);
        rtc_program();
    }
    destroy_all_pthread_sync_device();
    pthread_cancel(etc_thread);
    pthread_cancel(init_thread);
    close(kurtdev);
    pthread_exit(NULL);
#if (OS == LINUX_COMPATIBLE)
#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
    //close_user_space_isa_mem();
#endif
#endif
    return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
void rtc_program(void)
{
    pthread_mutex_lock(&f_intr_mutex_flag);
    f |=INTRF;
    pthread_cond_signal (&f_intr_cond_flag);
    pthread_mutex_unlock(&f_intr_mutex_flag);
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
void *init_thread_program(void *q)
{
    pthread_mutex_lock(&init_mutex_flag);
    pthread_cond_wait (&init_cond_flag, &init_mutex_flag);
    pthread_mutex_unlock(&init_mutex_flag);

    //ioperm(0x60,5,1);
    print_error_report("Beginning init thread program\n");
    kt_all_initialize();
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
    init_tcp_threads();
#endif
#if (OS == LINUX_COMPATIBLE)
#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
    //open_user_space_isa_mem();
#endif
#endif
    pthread_cond_broadcast (&start_cond_flag);
    pthread_exit(NULL);
    return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
void *etc_thread_program(void *q)
{

    pthread_mutex_lock(&start_mutex_flag);
    pthread_cond_wait (&start_cond_flag, &start_mutex_flag);
    pthread_mutex_unlock(&start_mutex_flag);

    //ioperm(0x60,5,1);
    print_error_report("Beginning etc thread program\n");
    kt_forever_loop();
    return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
void init_all_pthread_sync_device(void)
{
    pthread_mutex_init (&f_intr_mutex_flag, NULL);
    pthread_mutex_init (&start_mutex_flag, NULL);
    pthread_mutex_init (&init_mutex_flag, NULL);

    pthread_cond_init (&f_intr_cond_flag, NULL);
    pthread_cond_init (&start_cond_flag, NULL);
    pthread_cond_init (&init_cond_flag, NULL);
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
void destroy_all_pthread_sync_device(void)
{
    pthread_mutex_destroy (&f_intr_mutex_flag);
    pthread_mutex_destroy (&start_mutex_flag);
    pthread_mutex_destroy (&init_mutex_flag);

    pthread_cond_destroy (&f_intr_cond_flag);
    pthread_cond_destroy (&start_cond_flag);
    pthread_cond_destroy (&init_cond_flag);
}
#endif
#endif
/* =========================================================================== */
/* =========================================================================== */
unsigned int f_intr(void)
{
#if (OS == DOS) || (OS == WINDOWS)
        return f & INTRF;
#endif
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
{
        unsigned int tmp_f_intr;

        pthread_mutex_lock(&f_intr_mutex_flag);
        tmp_f_intr=f & INTRF;
        pthread_mutex_unlock(&f_intr_mutex_flag);
        return(tmp_f_intr);
}
#endif
#endif
}
/* =========================================================================== */
/* =========================================================================== */
void set_f_intr(void)
{
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
    pthread_mutex_lock(&f_intr_mutex_flag);
#endif
#endif
    f |=INTRF;
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
    pthread_mutex_unlock(&f_intr_mutex_flag);
#endif
#endif
}
/* =========================================================================== */
/* =========================================================================== */
void reset_f_intr(void)
{
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
    pthread_mutex_lock(&f_intr_mutex_flag);
#endif
#endif
        f &= ~INTRF;
#if (OS == LINUX_COMPATIBLE)
#if (LINUX_OS == KURT)
    pthread_mutex_unlock(&f_intr_mutex_flag);
#endif
#endif
}
/* =========================================================================== */
/* =========================================================================== */
void activate_bell(void)
{
//#if (OS == DOS)
    putchar(BELL);
//#endif
//#if (OS == LINUX_COMPATIBLE)
//#ifdef NCURSES_USED
//      beep();
//#endif
//#endif
}
/* =========================================================================== */
/* =========================================================================== */
#ifdef TIMERS_FOR_OPR
unsigned int getimer(void){
        unsigned int t;
        BYTE local_flag=RESET;
again:
        if(ftp >= TIMER_NO){
                if(local_flag == RESET){
                        prnt_msg(ERROR,0x5006);
                        local_flag=SET;
//                      crash_dump_analysis_timer();
                        goto again;
                }
                return KT_NULL;
        }
        t=ftp;
        if(ltp == t)
                ltp=KT_NULL;
        ftp=timer[t].rptr;
        if(ftp < TIMER_NO)
                timer[ftp].lptr=KT_NULL;
        timer[t].state=OUT_OF_QUE;
        return t;
}

void rlstimer(unsigned int tmr_no){
        TMR *tp;
        if((unsigned int)tmr_no >= TIMER_NO) {
                prnt_msg(ERROR,0x5001);
                return;
        }
        tp=&timer[tmr_no];
        if(tp->state == IN_QUE){
                prnt_msg(ERROR,0x5005);
//              prnt_timer(tmr_no);
                return;
        }
        tp->i=KT_NULL;
        tp->rptr=KT_NULL;
        tp->lptr=ltp;
        if(ltp < TIMER_NO)
                timer[ltp].rptr=tmr_no;
        else
                ftp=tmr_no;
        ltp=tmr_no;
        tp->state=IN_QUE;
}
unsigned int setimer(int tick,int indx,BYTE type){
        unsigned int t;
        unsigned int i;
        TMR *tp;
        INDEX_TYPE a;

        if( (t=getimer()) >= TIMER_NO){
        /* if timer not available,send TIME OUT for requesting source to cpq */
                a.i=indx;
                a.t=type;
                tocpq(a,TIME_OUT);
                prnt_msg(ERROR,0x5000);
                prnt_int(indx); prnt_char('-');
                prnt_byte(type); prnt_char(CR);
//              for(i=0;i<TIMER_NO;i++)
//                      prnt_timer(i);
                return t;
        }
        tp=&timer[t];
        tp->tdiv= tick / NO_TMR_QPTR;
        tp->tick=tick;
#if 0
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
        save_addr(tp->addr);
#endif
#endif
#endif //0
        tp->it.i=indx;
        tp->it.t=type;
#if 0
#ifdef NEW_DEBUG
        tp->stim=cpq[cpfq].stim;  // some default
        switch(type){
#ifdef IP_INSTALLED
/*
          case IP_REASSEMBLE_SDL_T    :
                tp->state=((REASM_STRUCT *)(indx))->state;
                break;
*/
#endif
#ifdef TCP_INSTALLED
          case TCP_SDL_T              :
                tp->state=tcb_st[indx].state;
                break;
#endif
#ifdef ARP_INSTALLED
          case ARP_SDL_T              :
                tp->state=((ARP_TAB_STRUCT *)(indx))->state;
                break;
#endif
          default               :
//              tp->state=0xff;
                break;
        }
#endif // NEW_DEBUG
#endif //0
        tp->lptr=KT_NULL;
        i=(tick+tpti) & (NO_TMR_QPTR - 1);
        /* i=(tick+index) % NO_TMR_QPTR */
        tp->i=i;
        tp->rptr=tpt[i];
        if(tpt[i] < TIMER_NO)
                timer[tpt[i]].lptr=t;
        tpt[i]=t;
        return t;
}

unsigned int stoptimer(int indx,BYTE type){
        unsigned int a;
        TMR *tp;

        a=(unsigned int)KT_NULL;
        switch(type){  /* switch on type */

#ifdef IP_INSTALLED
                case IP_REASSEMBLE_SDL_T    :
                                a=((REASM_STRUCT *)(indx))->timer_no;
//                              ((REASM_STRUCT *)(indx))->timer_no=NULL_TIMER;
                                indx=timer[a].it.i;
                                break;
#endif
#ifdef TCP_INSTALLED
                case TCP_SDL_T              :
                                a=indx;
                                indx=timer[a].it.i;
                                break;
#endif
#ifdef ARP_INSTALLED
                case ARP_SDL_T              :
                                a=((ARP_TAB_STRUCT *)(indx))->timer_no;
//                              ((ARP_TAB_STRUCT *)(indx))->timer_no=NULL_TIMER;
                                indx=timer[a].it.i;
                                break;
#endif
                default:        a=(unsigned int)KT_NULL;break;
        }
        if(a >= TIMER_NO) return a;
        tp=&timer[a];
        if(tp->i >= NO_TMR_QPTR) return a;
        if( (tp->it.t != type) || (tp->it.i != indx) ){
                prnt_msg(ERROR,0x5003);
//              prnt_timer(a);
                prnt_int(indx);prnt_char('-');
                prnt_byte(type);prnt_char('-');
                prnt_char(CR);
                print_call_frame();
                return KT_NULL;
        }
        if(tp->tdiv != 0xffff){     // not already timed out
                if(tp->lptr >= TIMER_NO){
                        if( (tpt[tp->i]=tp->rptr) < TIMER_NO)
                                timer[tp->rptr].lptr=KT_NULL;
                }
                else{
                        if(tp->rptr < TIMER_NO){
                        /* if not last timer */
                                timer[tp->rptr].lptr=tp->lptr;
                                timer[tp->lptr].rptr=tp->rptr;
                        }
                        else  /* last timer */
                                timer[ tp->lptr ].rptr=KT_NULL;
                }
                rlstimer(a);
        }
        tp->tdiv=0xfffe;        // flag as already stopped
        return a;
}
void timeman(void){
        unsigned int a,b;
        TMR *tp;
        INDEX_TYPE c;
        tpti= (tpti + 1) & (NO_TMR_QPTR-1);
        a=tpt[tpti];
        if(a < TIMER_NO){
            while(a < TIMER_NO){
                tp=&timer[a];
                if(tp->tdiv == 0){
                    c.i=a;
                    c.t=TIMER_T;
                    tocpq(c,TIME_OUT);
                    tp->tdiv=0xffff;    // flag as already timed out;
                    if(tp->lptr >= TIMER_NO){    /* if 1st timer */
                            if( (tpt[tp->i]=tp->rptr) < TIMER_NO)
                                    timer[tp->rptr].lptr=KT_NULL;
                    }
                    else{
                            if(tp->rptr < TIMER_NO){
                            /* if not last timer */
                                    timer[ tp->rptr ].lptr=tp->lptr;
                                    timer[ tp->lptr ].rptr=tp->rptr;
                            }
                            else  /* last timer */
                                    timer[ tp->lptr ].rptr=KT_NULL;
                    }
                    b=tp->rptr;
                    a=b;
                }
                else{
                    --tp->tdiv;
                    a=tp->rptr;
                }
            }
        }
}
#endif
/* =========================================================================== */
/* =========================================================================== */
void turn_on_park_led(BYTE n){
        led_set(PARK_LED);
}
/* =========================================================================== */
/* =========================================================================== */
void turn_off_park_led(BYTE n){
        BYTE h;
        for(h=0;h<10;h++)
                if(pl[h].t != NULL_T)
                        return;
        led_reset(PARK_LED);
}
/* =========================================================================== */
/* =========================================================================== */
#ifdef OPR_PRI_CONNECTION
void save_og_pri_digits(unsigned int ext_no)
{
        BYTE *dgt_pt;
        BYTE dgt_arr[32];
        BYTE no_dgt, a, i;

#ifdef DEBUG_ACTIVE
        str2lcd("save_og_pri_digits:");int2lcd(ext_no);str2lcd("\r\n");
#endif
        if(ext_no >= NO_EXTENSION){
                str2lcd("ERROR,0x5007:");int2lcd(ext_no);str2lcd("\r\n");
                return;
        }
        if(ocr.scdc == 0){
                str2lcd("ERROR,0x5011:");int2lcd(ext_no);str2lcd("\r\n");
                return;
        }
        no_dgt=0;
        dgt_pt=&ocr.d2d1;
        while(no_dgt < ocr.scdc){
                {
                        a=*dgt_pt & 0x0f;
                        if(a > 9)       a=0;
                        dgt_arr[no_dgt]=a;
                        no_dgt++;
                }
                if(no_dgt < ocr.scdc){
                        a=*dgt_pt >> 4;
                        if(a > 9)       a=0;
                        dgt_arr[no_dgt]=a;
                        no_dgt++;
                }
                dgt_pt++;
        }
        if(xst[ext_no].dgt != NULL_POINTER){
                str2lcd("ERROR,0x5008:");int2lcd(ext_no);str2lcd("\r\n");
                kt_g_free(xst[ext_no].dgt);
        }
        xst[ext_no].dgt=(BYTE *)kt_g_malloc(no_dgt);
        if(xst[ext_no].dgt == NULL_POINTER){
                prnt_msg(ERROR,0x5008);
                prnt_int(ext_no);prnt_char(CR);
                return;
        }
        for(i=0;i<no_dgt;i++){
                xst[ext_no].dgt[i]=dgt_arr[i];
        }
        xst[ext_no].no_dgt=no_dgt;
}
#endif
/* =========================================================================== */
/* =========================================================================== */
#ifdef OPR_PRI_CONNECTION
void save_ic_pri_digits(unsigned int ext_no)
{
        int r;
        BYTE i;

#ifdef DEBUG_ACTIVE
        str2lcd("save_ic_pri_digits:");int2lcd(ext_no);str2lcd("\r\n");
#endif
        if(ext_no >= NO_EXTENSION){
                str2lcd("ERROR,0x5009:");int2lcd(ext_no);str2lcd("\r\n");
                return;
        }
        r=xst[ext_no].i;
        if(r >= IQR_NO){
                str2lcd("ERROR,0x5010:");int2lcd(r);str2lcd("\r\n");
                return;
        }

        if(iqr[r].no_dgt == 0xff)  return;
        if(xst[ext_no].dgt != NULL_POINTER){
#ifdef DEBUG_ACTIVE
                str2lcd("ERROR,0x5011:");int2lcd(ext_no);str2lcd("\r\n");
#endif
                kt_g_free(xst[ext_no].dgt);
        }
        xst[ext_no].dgt=(BYTE *)kt_g_malloc(iqr[r].no_dgt);
        if(xst[ext_no].dgt == NULL_POINTER){
                str2lcd("ERROR,0x5012:");int2lcd(ext_no);str2lcd("\r\n");
                return;
        }
        for(i=0;i<iqr[r].no_dgt;i++){
                xst[ext_no].dgt[i]=iqr[r].src_dgt[i];
        }
        xst[ext_no].no_dgt=iqr[r].no_dgt;
}
#endif
/* =========================================================================== */
/* =========================================================================== */
#ifdef OPR_PRI_CONNECTION
void show_ext_digits(unsigned int ext_no)
{
        BYTE i;
        char buf[128];

#ifdef DEBUG_ACTIVE
        str2lcd("show_ext_digits:");int2lcd(ext_no);str2lcd("\r\n");
#endif
        if(xst[ext_no].no_dgt == 0xff)
                return;
        for(i=0;i<xst[ext_no].no_dgt;i++){
                buf[i]=xst[ext_no].dgt[i] +'0';
        }
        buf[i]=0;
        str2lcd(buf);
}
#endif
/* =========================================================================== */
/* =========================================================================== */
void rtp_oprside(unsigned int indx, unsigned char type){

        INDEX_TYPE pt;

        pt.i=indx;
        pt.t=type;
        rtp(pt);
}
/* =========================================================================== */
/* =========================================================================== */
//PIANO start
void reset_calls_report_status(void) {

        unsigned int i, indx;
        INDEX_TYPE pt;
        BYTE type;
        int r;

//SHADOW
#ifdef DEBUG_ACTIVE
        str2lcd("reset_calls_report_status:");int2lcd(ocr.state);str2lcd("\r\n");
#endif
        switch(ocr.state){
        case OPRSIDE_OPR_NIGHT  :
                pcpmftx(NIGHT_ACT, TON, OPR_T);
                return;
        case OPR_IDLE           :
        case OPR_READY          :
        default:
                pcpmftx(NIGHT_INACT, TON, OPR_T);
                break;
        }
        for(i=0;i<NO_HOLD_KEY;i++){
                indx=hldl[i].i;
                type=hldl[i].t;
                switch(type){
                case XST_T:
                        rtp(hldl[i]);
#ifdef DISPLAY_STATUS_ACT
                        ds[indx]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
                        if(dxsf)
                                opr_update(indx);
#endif
#endif
                        break;
#if(NO_CO != 0)
                case  CO_T:
                        rtp(hldl[i]);
                        break;
#endif
#ifndef NO_TIE_INIT
                case TIE_T:
                        rtp(hldl[i]);
                        break;
#endif
#if ( (defined EM_INSTALLED) && (defined OPR_EM_CONNECTION) )
                case EM_T:
                        rtp(hldl[i]);
                        break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:
                        rtp(hldl[i]);
                        break;
#endif
                case NULL_T:
                        continue;
                default:
                        break;
                }
                hldl[i].t=NULL_T;
#ifdef OPERATOR_PC
                led_reset(hkl[i]);  /* hold LED off */
#else
                led_reset(hkl);  /* hold LED off */
#endif
        }
        for(i=0;i<10;i++){
                indx=pl[i].i;
                type=pl[i].t;
                switch(type){
                case XST_T:
                        rtp(pl[i]);
#ifdef DISPLAY_STATUS_ACT
                        ds[indx]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
                        if(dxsf)
                                opr_update(indx);
#endif
#endif
                        break;
#if(NO_CO != 0)
                case  CO_T:
                        rtp(pl[i]);
                        break;
#endif
#ifndef NO_TIE_INIT
                case TIE_T:
                        rtp(pl[i]);
                        break;
#endif
#if ( (defined EM_INSTALLED) && (defined OPR_EM_CONNECTION) )
                case EM_T:
                        rtp(pl[i]);
                        break;
#endif
#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:
                        rtp(pl[i]);
                        break;
#endif
                case NULL_T:
                        continue;
                default:
                        break;
                }
                pl[i].t=NULL_T;
                turn_off_park_led(i);
        }
        while( (r=fromiq()) != ERR_CODE){
                pt.i=iqr[r].i;
                pt.t=XST_T;
                riqr(r);
                rtp(pt);
#ifdef DISPLAY_STATUS_ACT
                ds[pt.i]=ON_HOOK;
#ifdef NEW_DISPLAY_STATUS
                if(dxsf)
                        opr_update(pt.i);
#endif
#endif
        }
        while( (r=fromxq()) != ERR_CODE){
                pt=rxqr[r].pt;
                rrxqr(r);
                switch(pt.t){
#ifndef NO_TIE_INIT
                case TIE_T:
                        rtp(pt);
                        break;
#endif
#if ( (defined EM_CONTINUOUS_SIGNALING) && (defined OPR_EM_CONNECTION) )
                case EM_T:
                        rtp(pt);
                        break;
#endif

#if ((defined LIM) || (defined OPR_DT_CONNECTION))
                case CAS3_T:
                        rtp(pt);
                        break;
#endif
#if(NO_CO != 0)
                case CO_T:
                        rtp(pt);
                        break;
#endif
                default:
                        break;
                }
        }
        while( (r=fromrq()) != ERR_CODE){
                pt=rxqr[r].pt;
                rrxqr(r);
                switch(pt.t){
#ifndef NO_TIE_INIT
                case TIE_T:
                        rtp(pt);
                        break;
#endif
#if(NO_CO != 0)
                case CO_T:
                        rtp(pt);
                        break;
#endif
                default:
                        break;
                }
        }
#ifdef EM_CONT_SEP_OPR_Q
        while( (r=fromemq()) != ERR_CODE){
                pt=rxqr[r].pt;
                rrxqr(r);
                switch(pt.t){
                case EM_T:
                        rtp(pt);
                        break;
                default:
                        break;
                }
        }
#endif
        if( ocr.s.t != NULL_T){
                rtp(ocr.s);
                ocr.s.t=NULL_T;
        }

        if(ocr.d.t != NULL_T) {
                rtp(ocr.d);
                ocr.d.t=NULL_T;
        }

        switch(ocr.state){
//      case OPRSIDE_OPR_NIGHT  :
        case OPR_IDLE           :
                break;
        case OPR_READY          :
        default:
                ocr.state=OPR_READY;
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
                break;
        }
        ocr.scdc=0;
        ocr.mf=RESET;
        ocr.fs=0xff;
        hld.t=NULL_T;
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
//PIANOPIANO start: to be asked
/*
        ohs=ON_HOOK;
        trf |= OPR_HOOK_STAT;
        trs=0xff;
*/
        lpg1 &= ~PLS_BIT_POS;
        lsg1 &= ~PLS_BIT_POS;
        led_reset(PROG_LED);
//PIANOPIANO end
}
//PIANO end
/* =========================================================================== */
/* =========================================================================== */
