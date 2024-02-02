                /****  display & edit parameters variables  ****/
FILE * fp[3];
char n[MAX_NAME_LENGHT];
int p;


char xtgm7[]  ="_______";  /* ext. trunk group mask */
char xtgm6[]  ="______";  /* ext. trunk group mask */
char xtgm[]  ="_ ______";  /* ext. trunk group mask */
//char xemtgm[]="_ __________";   /* ext. e&m trunk group mask */
char xemtgm[]="_ _______________";   /* ext. e&m trunk group mask */
#ifdef X_RAILWAY
/* class of service table mask */
char cstm[]="_ _ _ _ _ _ _ _ _ _ _ _ _ _";
char astm[]="_ _ _";    /* active service table mask */
#else
/* class of service table mask */
char cstm[]="_  _  _  _  _  _  _  _  _  _  _  _  _  _";
char astm[]="_  _  _";  /* active service table mask */
#endif
char hlim[]="__";  /* hot line index mask */
char ddm[]="__________";  /* destination DID mask */
char pwb[15];  /* password buffer */
char pwm[]="______________";  /* password mask */

EXT_EDIT_LINE xel[NO_DISPLAY_ON_PAGE];  /* extensions edit line */
TRUNK_EDIT_LINE tel[NO_DISPLAY_ON_PAGE];  /* trunk edit line */
#ifdef ONLINE_EDIT
TRUNK_EDIT_LINE prv_tel[NO_DISPLAY_ON_PAGE];  /* trunk edit line */
#endif
DIGITAL_TRUNK_EDIT_LINE dtel[NO_DISPLAY_ON_PAGE];  /* digital trunk edit line */
DIGITAL_TRUNK_EDIT_LINE prv_dtel[NO_DISPLAY_ON_PAGE];  /* digital trunk edit line */

char nel[NO_OPR_NITX][11];  /* night edit line */
#ifdef ONLINE_EDIT
long prv_nelv[NO_OPR_NITX];  /* night edit line */
long nelv[NO_OPR_NITX];  /* night edit line */
#endif
HOT_LINE_EDIT_LINE hlel[NO_HOT_LINE];  /* hot line edit line */

#ifdef ONLINE_EDIT
HOT_LINE_EDIT_LINE prv_hlel[NO_HOT_LINE];  /* hot line edit line */
#endif
#ifdef AUTO_UPLOAD
unsigned int file_changed;
#endif

#ifdef EM_GROUP_PROGRAM
char emgpel[NO_EM_GROUP][3];  /* e&m group program edit line */
unsigned int emgpelv[NO_EM_GROUP];
unsigned int prv_emgpelv[NO_EM_GROUP];
#endif
int fdb=0;

int last_s;
WINDOW *saved_a1;
BYTE dep_flag;
BYTE ep_flag;
BYTE saved_n;
long *file_line_ptr;
long last_line;
long line_no;
BYTE file_viewer_flag;
FILE *view_fp;
char *file_title;
//long alarm_line_ptr[MAX_FILE_LINE];
long alarm_last_line;
//long malicious_line_ptr[MAX_FILE_LINE];
long malicious_last_line;
//FILE *alarm_fp;
FILE *malicious_fp;
BYTE cpfoq_flag;
FILE *fp1;
char ocl[7];
#ifdef ONLINE_EDIT
unsigned long oclv;
unsigned long prv_oclv;
#endif

#ifdef ONLINE_EDIT
EXT_EDIT_LINE prv_xel[NO_DISPLAY_ON_PAGE];  /* extensions edit line */
#endif
BYTE cpw_flag;
BYTE dep_1st_entry_flag;
BYTE mac_nite[MAX_NITE_MAC];

#ifdef DEP_MANUAL_TEST
BYTE test_2nd_menu_flag;
BYTE test_flag;
char test_ext[8];
char test_ext_mask[]="________";  /* destination DID mask */
#ifdef EM_INSTALLED
char test_em[5];
char test_em_mask[]="____";
#endif
#if( (NO_CAS3 != 0) || (NO_CAS1 != 0) )
char test_dgtl_trnk[5];
char test_dgtl_trnk_mask[]="____";
#endif
#ifdef DTMF
char test_dtmf[5];
char test_dtmf_mask[]="____";
#endif
char test_swch[3];
char test_swch_mask[]="__";

char test_port_unit[5];
char test_port_unit_mask[]="____";

TEST_ST tst;
WINDOW *saved_win2;
WINDOW *saved_win3;
BYTE menu_item;
BYTE test_menu_flag;
#endif

char srch_ext_no[11];
unsigned long srch_ext_no_v;
#ifdef BATCH_PROGRAMMING
BATCH_EXT_EDIT_LINE batch_xel;  /* extensions edit line */
BYTE batch_ep_flag;
unsigned char batch_ep_2nd_menu_flag;
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
BATCH_DIGITAL_TRUNK_EDIT_LINE batch_dtel;
#endif
#endif
#ifdef CB_INSTALLED
unsigned int first_cb_no;
unsigned long first_cb_number;
#endif

#ifdef DEP_MANUAL_TEST
unsigned int subscriber_state_block[SUBSCRIBER_STATE_BLOCK_SIZE];
BYTE subscriber_state_block_ptr;
#endif
#ifdef DAY_NIGHT_PARAMETERS_FILE
BYTE saved_day_file_no;
#endif
