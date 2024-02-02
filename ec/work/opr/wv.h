			/****  window variables header	 ****/

struct{  /* array of border character set */
	int nw,ne,se,sw,side,line;
}wcs[]={
	{218,191,217,192,179,196},  /* single line */
	{201,187,188,200,186,205},  /* double line */
	{214,183,189,211,186,196},  /* single top,double side */
	{213,184,190,212,179,205},  /* double top,single side */
	{194,194,217,192,179,196},  /* pop_down menu */
//NEW_DEP start : only in opr
	{' ',' ',' ',' ',' ',' '}  /* no border */
//NEW_DEP end
};

WINDOW *wllh;  /* window linked list head */
WINDOW *wllt;  /* window linked list tail */

//DOOD start: #ifdef added
#if (OS == DOS)
union REGS r;
#endif
//DOOD end
char ca;  /* clear attribute */

int vsa;  /* video segment address */

WINDOW *ewnd;  /* error window pointer */

int im;  /* insert mode */

char mask2[]="__";
char mask10[]="__,__,__,__,__,__,__,__,__,__";
char mask15[]="__,__,__,__,__,__,__,__,__,__,__,__,__,__,__";

//NEW_DEP start : only in opr
char mask18[]="__________________";
//NEW_DEP end

//NEW_DEP start :
//MYCHNG start
WINDOW *saved_win;
WINDOW *saved_hwin;
long saved_h;
int saved_b;
int saved_a;
unsigned char exp_part_flag;
unsigned char ep_2nd_menu_flag;
unsigned char data_entry_flag;
unsigned char read_field_flag;
unsigned char cqw_flag;
unsigned char getchar_hndlr_flag;
WINDOW *cqw_win;
unsigned char error_message_flag;
unsigned char dep_quit_flag;
unsigned char non_error_message_flag;
WINDOW *ktbg_w;
//MYCHNG end
//NEW_DEP end

//MESR start
#ifdef DISPLAY_BILLING
WINDOW *bill_w,*sync_bill_w;
#endif
//MESR end
#ifdef DISPLAY_ALARM
WINDOW *alarm_w,*sync_alarm_w;
#endif
