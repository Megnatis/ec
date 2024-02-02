
#define         ONLINE_EDIT
#define         MAX_NITE_MAC    4
  /****  display & edit parameters macros,types,& functions prototypes  ****/
#define NO_DISPLAY_ON_PAGE      16
/* 25 - 3 for continue or quit windwo - 2 for display window */
/* - 3 for each window caption - 1 btwn windows space */

#define TRUNK_GROUP_MASK        0x07
#ifdef X_RAILWAY
#define EM_TRUNK_GROUP_MASK    0x780    /* b10-b7 is group number for e&m */
#define EMTGM_LOC               7       /* BIT NUMBER OF LSBIT 'ONE' IN EM_TRUNK_GROUP_MASK */
#define EXTENSION_PARAM_BLOCK_LENGTH    10  /* in bytes */
#else
#define EM_TRUNK_GROUP_MASK     0x07
#define EMTGM_LOC               0
#define EXTENSION_PARAM_BLOCK_LENGTH    8
#endif
#define IS_MASK                 0x08
#define DA_MASK                 0x10
#define TA_MASK                 0x20
#define S9_MASK                 0x40
/* in activation int of a trunk,b0-b2:trunk group,b3:in service, */
/* b4:DID active,b5:timing active */
#ifdef TOLL_BARRING_ON_COs
#define TOLL_ACCESS_MASK        0x40
#endif



#if( (NO_CAS3 != 0) || (NO_CAS1 != 0) )
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

typedef struct{
        char ctg[17];  /* CO trunk group edit buffer */
#ifdef ONLINE_EDIT
        BYTE ctgv;
        BYTE ttgv;
        unsigned int emtgv;
        unsigned int  hliv;
#endif
        char ttg[8];  /* TIE trunk group edit buffer */
        char emtg[17];  /* E&M trunk group edit buffer */
        char cst[15];  /* class of service edit buffer */
        unsigned int cstv;  /* class of service table value */
        char ast[4];  /* active service table edit buffer */
        unsigned int astv;  /* active service table value */
        char hli[3];  /* hot line index */
}EXT_EDIT_LINE;

typedef struct{
#ifdef ONLINE_EDIT
        int ddbv;
        unsigned long t1v;
        unsigned long t2v;
#endif
        char ddb[11];  /* destination DID buffer */
        char t1[5];   /* timer 1 em_cont */
        char t2[5];   /* timer 2 em_cont */
        char gn[3];  /* group number buffer */
        char is;  /* in service buffer */
        char da;  /* DID active */
        char ta;  /* timing active */
        char s9;  /* send 9 */
        unsigned int ai;  /* activation int.,read from parameter's file */
#ifdef TOLL_BARRING_ON_COs
        char toll_access;
#endif
}TRUNK_EDIT_LINE;

typedef struct{
        char gn[3];  /* group number buffer */
        char is;
        char icc;
        char ogc;
        char pts;
        char eos;
        char mtr;
        char opr;
        char mal;
        char imm;
        char wfa;
        char ids;

        unsigned int ai;  /* activation int.,read from parameter's file */
}DIGITAL_TRUNK_EDIT_LINE;

typedef struct{
        char dht[4];  /* destination hot line type */
        char dhi[11];  /* destination hot line index */
#ifdef ONLINE_EDIT
        unsigned long dhv;
#endif

}HOT_LINE_EDIT_LINE;

int din(void);int dpw(void);

int dxp(BYTE);
void dxp_part1(BYTE n);  /* display extensions parameters */
void dxp_init_display(BYTE n);  /* display extensions parameters */
void dxp_init(BYTE n);  /* display extensions parameters */
#ifdef CB_INSTALLED
int dcbp(BYTE);
void dcbp_part1(BYTE n);
void dcbp_init_display(BYTE n);
void dcbp_init(BYTE n);
#endif
int dttp(BYTE);
void dttp_part1(BYTE n);  /* display extensions parameters */
void dttp_init_display(BYTE n);  /* display extensions parameters */
void dttp_init(BYTE n);  /* display extensions parameters */
int dctp(BYTE);
void dctp_init(BYTE);
void dctp_part1(BYTE);
void dctp_init_display(BYTE);
#if (NO_CO != 0)
int ectp(BYTE n); /* edit extensions' parameters */
void ectp_part1(BYTE n); /* edit extensions' parameters */
void ectp_part2(BYTE n); /* edit extensions' parameters */
void ectp_init_display(BYTE n); /* edit extensions' parameters */
#endif
int donx(BYTE);
int ddhl(BYTE);int deph(void);
//MOHAJER : exp->kt_exp
int kt_exp(BYTE);
void exp_part1(BYTE);
void exp_part2(BYTE);
void exp_init_display(BYTE n); /* edit extensions' parameters */
int ecbp(BYTE);
void ecbp_part1(BYTE);
void ecbp_part2(BYTE);
void ecbp_init_display(BYTE n);
int ettp(BYTE);
void ettp_init_display(BYTE);
void ettp_part1(BYTE);
void ettp_part2(BYTE);
int eonx(BYTE);
void eonx_init_display(BYTE);
void eonx_part2(BYTE);
int edhl(BYTE);
void edhl_init_display(BYTE);
void edhl_part2(BYTE);
int cpfoq(void);
int cnpf(int);

void cqw(void);
WINDOW *demrc(void);
int cpw(void);
WINDOW *dmmrc(void);
int ep(void);
int ep_1st_menu(void);
void ep_1st_menu_init(void);
#ifdef EM_GROUP_PROGRAM
void demtp_init_display(BYTE);
void demtp_init(BYTE);
void demtp_part1(BYTE);
int demtp(BYTE);
void eemtp_init_display(BYTE);
void eemtp_part1(BYTE);
void eemtp_part2(BYTE);
int eemtp(BYTE);
void eemgp_init_display(void);
void eemgp_part2(void);
void eemgp(void);
#endif

int vbbf(void);
void vbbf_init(void);
void vbbf_init_display(void);
int cnvrtbbf(void);

#ifdef REPORT_BULK_BILLING
#define BULK_BILLING_FILE               "\\ktbill\\kiatel.bil"
#define BULK_BILLING_ASCII_FILE         "\\bill.asc"
#endif

int ddtp(BYTE);
void ddtp_init(BYTE);
void ddtp_init_display(BYTE);
void ddtp_part1(BYTE);
int edtp(BYTE);
void edtp_init_display(BYTE);
void edtp_part1(BYTE);
void edtp_part2(BYTE);
int vmcf(void);
int valarmlogf(void);
int v_office_code(void);
void e_office_code(void);
void e_office_code_init_display(void);
void e_office_code_part2(void);
SDNST dep_find_subscriber_no(unsigned int i);
unsigned int dep_fcp(SDNST i);

void init_array(void);

int vdbf(void);
#define DETAIL_BILLING_VIEWER   "dosbill.exe"

#ifndef NO_OPR_NITX_PARAMS
#define NO_OPR_NITX_PARAMS NO_OPR_NITX          // to support older versions of "header files"
#endif
#define         MAX_FILE_LINE             16000
void file_view_init(void);
void file_viewer(void); /* view alarm log file */
void init_alarm_file_data(void);
void init_malicious_file_data(void);
void init_alarm_pointer_array(void);
void init_malicious_pointer_array(void);
#ifdef ONLINE_EDIT
void send_ext_ctgv_pckt(unsigned int ext_indx,BYTE c);
void send_ext_ttgv_pckt(unsigned int ext_indx,BYTE c);
void send_ext_emtgv_pckt(unsigned int ext_indx,BYTE c);
void send_ext_cstv_pckt(unsigned int ext_indx,BYTE c);
#ifdef CB_INSTALLED
void send_cb_cstv_pckt(unsigned int ext_indx,BYTE c);
#endif
void send_ext_astv_pckt(unsigned int ext_indx,BYTE c);
void send_ext_hliv_pckt(unsigned int ext_indx,BYTE c);
#if (NO_CO != 0)
void send_co_ai_pckt(BYTE co_indx,BYTE c);
void send_co_ddbv_pckt(BYTE co_indx,BYTE c);
#endif
#ifndef  NO_TIE_INIT
void send_tie_ai_pckt(BYTE tie_indx,BYTE c);
void send_tie_ddbv_pckt(BYTE tie_indx,BYTE c);
#endif
void send_eonx_nel_pckt(BYTE nel_indx);

#ifdef HOT_LINE_SERVICE_ACTIVE
void send_edhl_hlel_pckt(BYTE hlel_indx);
#endif
void send_office_code_ocl_pckt(void);

#ifdef EM_GROUP_PROGRAM
void send_em_ddbv_pckt(BYTE em_indx,BYTE c);
void send_em_ai_pckt(BYTE em_indx,BYTE c);
void send_em_t1_pckt(BYTE em_indx,BYTE c);
void send_em_t2_pckt(BYTE em_indx,BYTE c);
void send_em_emgpel_pckt(BYTE grp_indx);
#endif
#if( (NO_CAS3 != 0) || (NO_CAS1 != 0) )
void send_cas_ai_pckt(unsigned int cas_indx,BYTE c);
#endif
#endif
void init_mac_nite_arr(void);
int  cpw_init_display(void);
int  cpw_part2(void);


#ifdef DEP_MANUAL_TEST
void test_menu(void);
void test_1st_menu_init(void);
int test_1st_menu(void);
WINDOW *display_test_menu(void);
void ext_test_init_display(void);
void ext_test(void);
void ext_test_part2(void);
#ifdef EM_INSTALLED
void em_test_init_display(void);
void em_test(void);
void em_test_part2(void);
#endif
#if( (NO_CAS3 != 0) || (NO_CAS1 != 0) )
void dgtl_trnk_test_init_display(void);
void dgtl_trnk_test(void);
void dgtl_trnk_test_part2(void);
#endif
#ifdef DTMF
void dtmf_menu(void);
void dtmf_menu_init_display(void);
void dtmf_test_init_display(void);
void dtmf_test(void);
void dtmf_test_part2(void);
#endif
void swch_menu(void);
void swch_menu_init_display(void);
void swch_test_init_display(void);
void swch_test(void);
void swch_test_part2(void);
void exit_manual_test_with_1_menu(void);
void exit_manual_test_with_2_menu(void);
#define         FIRST_MENU      1
#define         SECOND_MENU     2

#define         GROUP_SWITCH_TEST          1
#define         PORT_UNIT_TEST             2
#endif

void ask_ext_no(void);  /* edit extensions' parameters */
void ask_ext_no_init_display(void);  /* edit office code */
void ask_ext_no_part2(void);  /* edit office code */
#ifdef BATCH_PROGRAMMING
int  batch_exp(void);
void batch_exp_part2(void);
void batch_exp_init_display(void); /* edit extensions' parameters */
WINDOW *batch_demrc(void);  /* display edit menu & return choice */
int batch_ep(void);  /* edit parameters */
void batch_ep_1st_menu_init(void);  /* edit parameters */
int batch_ep_1st_menu(void);  /* edit parameters */

typedef struct{
        char ctg[17];  /* CO trunk group edit buffer */
        char ttg[8];  /* TIE trunk group edit buffer */
#ifdef X_RAILWAY
        char emtg[17];  /* E&M trunk group edit buffer */
#endif
        char cst[15];  /* class of service edit buffer */
        char ast[4];  /* active service table edit buffer */
        char first_ext[11];
        char last_ext[11];
}BATCH_EXT_EDIT_LINE;

typedef struct{
        SDNST frst_prty;
        SDNST lst_prty;
        BYTE prty_type;
}BATCH_PARTY_ST;

BYTE send_batch_ma_pckt(BYTE msg_type,BYTE dat,BATCH_PARTY_ST *p,BYTE n);

#define MA_BATCH_EXT_PCKT_SIZE  7

#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
typedef struct{
        char gn[3];  /* group number buffer */
        char is;
        char icc;
        char ogc;
        char pts;
        char eos;
        char mtr;
        char opr;
        char mal;
        char imm;
        char wfa;
        char ids;
        char first_cas[5];
        char last_cas[5];
}BATCH_DIGITAL_TRUNK_EDIT_LINE;

int  batch_edtp(void);
void batch_edtp_part2(void);
void batch_edtp_init_display(void); /* edit extensions' parameters */
#endif
#endif // BATCH_PROGRAMMING

void ext_state_init_display(void);
void ext_state(void);
void ext_state_part2(void);

void r1_test_init_display(void);
void r1_test(void);
void r1_test_part2(void);
