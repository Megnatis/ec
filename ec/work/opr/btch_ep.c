//GRPPRG : file added

#include "omtfp.h"

#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

#include "opr_net.h"

#include <string.h>
#include <stdio.h>

//CHEEN : line added
#include "/ec/work/sys/lintcp.ext"

#ifdef BATCH_PROGRAMMING
#include "wg.h"
#include "depmtfp.h"

extern FILE *fp[];
extern int fdb;

extern WINDOW *saved_win;
extern unsigned char ep_2nd_menu_flag;
extern unsigned char data_entry_flag;
extern unsigned char saved_n;
extern int saved_b;

extern char srch_ext_no[];
extern unsigned long srch_ext_no_v;
extern BATCH_EXT_EDIT_LINE batch_xel;
extern unsigned char batch_ep_2nd_menu_flag;
extern BYTE batch_ep_flag;
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
extern BYTE hot_link;
#endif
#ifdef NET_DEBUG
extern FILE *net_fp;
#endif
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
extern BATCH_DIGITAL_TRUNK_EDIT_LINE batch_dtel;
#endif
extern int no_cas3[];
extern int no_extension[];
extern int no_co[];
extern GRP_TYPE no_cas3_programmable_group[];
extern int no_cas1[];
extern int no_em[];
extern GRP_TYPE no_cas1_group[];
#endif // BATCH_PROGRAMMING

#ifdef DAY_NIGHT_PARAMETERS_FILE
extern BYTE saved_day_file_no;
#endif

/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
WINDOW *batch_demrc(void){  /* display edit menu & return choice */
	WINDOW *a;
//DVD : 21,8,12,39 -->
	a=enw(21,7,8,41);
	if(a == NULL){
		caw();	cs();
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
	swt(a," EDIT PARAMETERS IN BATCH MODE ");
	draw_window(a);
//DOOD start
//	swucf(a,GREEN);
	swucf(a,KT_GREEN);
//DOOD end
	wprompt(a,1,0,"1-batch of Extension parameters.");
#if (NO_CO!=0)
	wprompt(a,1,1,"2-batch of Co trunk parameters.");
#else
	wprompt(a,1,1,"2-not used.");
#endif
#ifdef EM_GROUP_PROGRAM
	wprompt(a,1,2,"3-batch of e&M trunk parameters.");
	wprompt(a,1,3,"4-batch of e&m Group digits.");
#else
#ifndef NO_TIE_INIT
	wprompt(a,1,2,"3-batch of Tie trunk parameters.");
#else
	wprompt(a,1,2,"3-not used.");
#endif
	wprompt(a,1,3,"4-not used.");
#endif
//DYNAMIC : to be corrected.
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
	wprompt(a,1,4,"5-batch of Digital trunk parameters.");
#else
	wprompt(a,1,4,"5-not used.");
#endif
	wprompt(a,1,5,"6-Return to main menu.");
	return a;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
int batch_ep(void){  /* edit parameters */

	if (batch_ep_2nd_menu_flag == RESET) {
		batch_ep_1st_menu();
		return 'c';
	}
	switch(batch_ep_2nd_menu_flag){
		case 1:
			batch_exp();
			break;
		case 2:
/*
#if (NO_CO != 0)
			batch_ectp(saved_n);
#endif
*/
			break;
#ifdef EM_GROUP_PROGRAM
		case 3:
/*
			batch_eemtp(saved_n);
*/
			break;

		case 4:
/*
			batch_eemgp();
*/
			break;
#else
		case 3:
#ifndef NO_TIE_INIT
/*
			batch_ettp(saved_n);
*/
#endif
			break;

		case 4: break;
#endif
		case 5:
//DYNAMIC : to be corrected.
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )

			batch_edtp();

#endif
			break;

		case 6:
			batch_ep_flag=RESET;
			break;
		default : break;
	}

	if (batch_ep_flag == RESET )
	{
#ifdef AUTO_UPLOAD
		file_changed=SET;
#endif
		if ( batch_ep_2nd_menu_flag != 6)    //if return to main menu,saved_win has already been deleted.
			deletw(saved_win);
		batch_ep_2nd_menu_flag=RESET;
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
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
void batch_ep_1st_menu_init(void){  /* edit parameters */

	WINDOW *a1;
	a1=batch_demrc();
	saved_win=a1;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
int batch_ep_1st_menu(void){  /* edit parameters */

	int b;
	WINDOW *a1;

	if (batch_ep_flag == RESET){
		a1=batch_demrc();
		saved_win=a1;
	}
	else{
		a1=saved_win;
	}
#ifdef EM_GROUP_PROGRAM
	b=get_selection(a1,1,"ECMGDR");
#else
	b=get_selection(a1,1,"ECT~DR");
#endif
	if (b == NO_KEY_PRESSED){
	      return b;
	}
	switch(b){
		case 1:
			deletw(a1);
			batch_ep_2nd_menu_flag=b;
			fdb=0;
			batch_exp_init_display();

			break;
		case 2:
/*
#if (NO_CO != 0)
			deletw(a1);
			batch_ep_2nd_menu_flag=b;
			fdb=0;
			saved_n=0;
			batch_ectp_init_display(saved_n);
			batch_ectp_part1(saved_n);
#endif
*/
			break;
#ifdef EM_GROUP_PROGRAM
		case 3:
/*
			deletw(a1);
			batch_ep_2nd_menu_flag=b;
			saved_n=0;
			batch_eemtp_init_display(saved_n);
			batch_eemtp_part1(saved_n);
*/
			break;
		case 4:
/*
			deletw(a1);
			batch_ep_2nd_menu_flag=b;
			batch_eemgp_init_display();
*/
			break;
#else
		case 3:
#ifndef NO_TIE_INIT
/*
			deletw(a1);
			batch_ep_2nd_menu_flag=b;
			saved_n=0;
			batch_ettp_init_display(saved_n);
			batch_ettp_part1(saved_n);
*/
#endif
			break;

		case 4: break;
#endif
		case 5:
//DYNAMIC : to be corrected.
//DYNAMIC : #if( (NO_CAS3 != 0) || (NO_CAS1 != 0) ) -->
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )

			deletw(a1);
			batch_ep_2nd_menu_flag=b;
			batch_edtp_init_display();
#endif
			break;

		case 6 :
			deletw(a1);
			batch_ep_2nd_menu_flag=b;
			return b;
		default: break;
	}
#ifdef AUTO_UPLOAD
//	file_changed=SET;
#endif

	return b;
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
int batch_exp(void){  /* edit extensions' parameters */

	int code;

	if (data_entry_flag == SET){
		code=data_entry(saved_win);
		switch (code){
		  case ENTER :
			batch_exp_part2();
			data_entry_flag=RESET;
			deletw(saved_win);
			batch_ep_2nd_menu_flag = RESET;
			batch_ep_1st_menu_init();
			saved_b=0;
			return code;
		  case ESC :
			data_entry_flag=RESET;
			deletw(saved_win);
			batch_ep_2nd_menu_flag = RESET;
			batch_ep_1st_menu_init();
			saved_b=0;
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
#ifdef BATCH_PROGRAMMING
void batch_exp_init_display(void){  /* edit extensions' parameters */

	WINDOW *w;
	int no_field;
	unsigned int e,d;
	FIELD *f,*tf;
	int off=-2;
#ifdef X_RAILWAY
	w=enw(4,2,20,71);
#else
	w=enw(4,2,17,71);
#endif
	if(w == NULL){
		caw();	cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
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
	swt(w," EDIT BATCH OF EXTENSION PARAMETERS ");
	draw_window(w);

	wprompt(w,1,off+3, " Extension No.  ³   From:            To:                           ");
	wprompt(w,1,off+4, "ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ-  ");
#ifdef BAD_PAYER
	wprompt(w,1,off+5, "    Class of    ³   AA B1 HL TR PU B2 CF B3 DD IN ST FM CP CB       ");
#else
	wprompt(w,1,off+5, "    Class of    ³   AA SA HL TR PU T0 CF CR DD IN ST FM CP CB       ");
#endif
	wprompt(w,1,off+6, "  Service Table ³                 ");
	wprompt(w,1,off+7, "ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ");
	wprompt(w,1,off+8, "    Active      ³   FA MC CG     ");
	wprompt(w,1,off+9, "  Service Table ³                ");
	wprompt(w,1,off+10,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ");

	wprompt(w,1,off+11,"      CO        ³   grp6543210987654321 ");
	wprompt(w,1,off+12,"    Groups      ³                       ");
	wprompt(w,1,off+13,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ-  ");


#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
	wprompt(w,1,off+14, "      TIE       ³   grp654321 ");
	wprompt(w,1,off+15, "    Groups      ³             ");
	wprompt(w,1,off+16, "ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ-  ");

	wprompt(w,1,off+17, "      E&M       ³   grp543210987654321 ");
	wprompt(w,1,off+18, "    Groups      ³                      ");
#else
	wprompt(w,1,off+14, "      E&M       ³   grp654321 ");
	wprompt(w,1,off+15, "    Groups      ³             ");
#endif
#else
	wprompt(w,1,off+14, "      TIE       ³   grp654321 ");
	wprompt(w,1,off+15, "    Groups      ³             ");
#endif

	idew(w);

	f=enf(w,27,off+3,"__________",batch_xel.first_ext,DECIMAL);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.first_ext,"----------");

	f=enf(w,42,off+3,"__________",batch_xel.last_ext,DECIMAL);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.last_ext,"----------");

	f=enf(w,22,off+6,"_  _  _  _  _  _  _  _  _  _  _  _  _  _",batch_xel.cst,BOOLEAN);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.cst,"xxxxxxxxxxxxxx");

	f=enf(w,22,off+9,"_  _  _",batch_xel.ast,BOOLEAN);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.ast,"xxx");

	f=enf(w,24,off+12,"________________",batch_xel.ctg,BOOLEAN);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.ctg,"xxxxxxxxxxxxxxxx");

#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
	f=enf(w,24,off+15,"______",batch_xel.ttg,BOOLEAN);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.ttg,"xxxxxx");


	f=enf(w,24,off+18,"_______________",batch_xel.emtg,BOOLEAN);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.emtg,"xxxxxxxxxxxxxxx");

#else
	// ????????? it is really emtg ,but is saved in ttg field.
	f=enf(w,24,off+15,"______",batch_xel.ttg,BOOLEAN);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.ttg,"xxxxxx");

#endif
#else
	f=enf(w,24,off+15,"______",batch_xel.ttg,BOOLEAN);
	if(f == NULL){
		caw(); cs();
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_xel.ttg,"xxxxxx");
#endif

#ifdef X_RAILWAY
	no_field=7;
#else
	no_field=6;
#endif

	/* linking up & down fields */
	for(d=0,f=w->ff;d <  no_field ;d++,f=f->fnxt){
		/* 5 = number of field in a line */

		for(e=0,tf=f;(e < no_field) && (tf != NULL);e++,tf=tf->fnxt);
		/* 5 = number of field in a line */
		f->fdn=tf;

		for(e=no_field,tf=f;e && (tf != NULL);e--,tf=tf->fprv);
		/* 5 = number of field in a line */
		f->fup=tf;
	}
	saved_win=w;
	data_entry_init(saved_win);
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
void batch_exp_part2(void){  /* edit extensions' parameters */

	long h,i;
	unsigned int tot_ext,e;
#if ((defined EM_GROUP_PROGRAM) || (!defined NO_TIE_INIT))
	BYTE ttg_in_file;
#endif
	BYTE ttg_changed,ttgv;
	unsigned int ctg_in_file,ctg_changed,ctgv;
#ifdef X_RAILWAY
	unsigned int emtg_changed;
	unsigned int emtgv;
	unsigned int emtg_in_file;
#endif
	unsigned int ast_changed,cst_changed;
	unsigned int astv,cstv;  /* active service table value */
	unsigned int ast_in_file,cst_in_file;
	char *g;
	BATCH_PARTY_ST prty_info;
#ifdef LIM
	char max_ext[8];
	char dst[25];
#endif
	BYTE n,mac;

	prty_info.frst_prty=-1;
	prty_info.lst_prty=-1;

	sscanf(batch_xel.first_ext,SDNS_FS,&prty_info.frst_prty);
	sscanf(batch_xel.last_ext,SDNS_FS,&prty_info.lst_prty);

	prty_info.frst_prty=dep_fcp(prty_info.frst_prty);
	prty_info.lst_prty=dep_fcp(prty_info.lst_prty);
	prty_info.prty_type=XST_T;

	if ((prty_info.frst_prty >= NO_TOTAL_EXTENSION) || (prty_info.lst_prty >= NO_TOTAL_EXTENSION)){
		error_message("Invalid Numbers !");
		return;
	}
	if (prty_info.lst_prty < prty_info.frst_prty){
		error_message("Wrong Numbers !");
		return;
	}
#ifdef LIM
	strcpy(dst, "Max Ext. No. :");
#if(NO_EXTENSION_C != 0)
	if (prty_info.frst_prty >= (NO_EXTENSION_A+NO_EXTENSION_B)){ // is not in LIMC
		n=2;
		prty_info.frst_prty-=(NO_EXTENSION_A+NO_EXTENSION_B);
		prty_info.lst_prty-=(NO_EXTENSION_A+NO_EXTENSION_B);
	}
	else
#endif
	{
		if (prty_info.frst_prty >= NO_EXTENSION_A){ // LIMB
			if(prty_info.lst_prty >= (NO_EXTENSION_A+NO_EXTENSION_B)){
				ltoa((dep_find_subscriber_no(NO_EXTENSION_A+NO_EXTENSION_B-1)),max_ext,10);
				strcat(dst,max_ext );
				error_message(dst);
				return;
			}
			else{
				n=1;
				prty_info.frst_prty-=NO_EXTENSION_A;
				prty_info.lst_prty-=NO_EXTENSION_A;
			}
		}
		else{
			if(prty_info.lst_prty >= NO_EXTENSION_A){ // LIMA
				ltoa(dep_find_subscriber_no(NO_EXTENSION_A-1),max_ext,10);
				strcat(dst,max_ext );
				error_message(dst);
				return;
			}
			else{
				n=0;
			}
		}
	}
#else
	n=0;
#endif // LIM
	ctg_changed=0x0000;
	g=batch_xel.ctg;
	ctgv=0;
//	for(e=0x20,mac=MAC_BATCH_EXT_CO_GRP5;*g != '\0';g++,e >>= 1,mac--){
//	for(e=0x0800,mac=MAC_BATCH_EXT_CO_GRP11,g+=4;*g != '\0';g++,e >>= 1,mac--){
	for(e=0x8000,mac=MAC_BATCH_EXT_CO_GRP15;*g != '\0';g++,e >>= 1,mac--){
		if(*g != 'x'){
			ctg_changed |= e;
			if(*g == '|'){
				ctgv |= e;
				send_batch_ma_pckt(mac,SET,&prty_info,n);
			}
			else{
				send_batch_ma_pckt(mac,RESET,&prty_info,n);
			}
		}
	}

	ttg_changed=0x00;
	g=batch_xel.ttg;
	ttgv=0;
#if ((defined EM_GROUP_PROGRAM) || (!defined NO_TIE_INIT))

#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
	for(e=0x20,mac=MAC_BATCH_EXT_TIE_GRP5;*g != '\0';g++,e >>= 1,mac--){
#else
#if( (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED))
	for(e=0x20,mac=MAC_BATCH_EXT_EM_GRP5;*g != '\0';g++,e >>= 1,mac--){
#else /* EM_PULSE_SIGNALING */
	for(e=0x20,mac=MAC_BATCH_EXT_EM_GRP5;*g != '\0';g++,e >>= 1,mac--){
#endif

#endif
#else
#ifndef NO_TIE_INIT
	for(e=0x20,mac=MAC_BATCH_EXT_TIE_GRP5;*g != '\0';g++,e >>= 1,mac--){
#endif
#endif
		if(*g != 'x'){
			ttg_changed |= e;
			if(*g == '|'){
				send_batch_ma_pckt(mac,SET,&prty_info,n);
				ttgv |= e;
			}
			else{
				send_batch_ma_pckt(mac,RESET,&prty_info,n);
			}
		}
	}
#endif // ((defined EM_GROUP_PROGRAM) || (!defined NO_TIE_INIT))

#ifdef X_RAILWAY
	emtg_changed=0x0000;
	g=batch_xel.emtg;
	emtgv=0;
	for(e=0x2000,mac=MAC_BATCH_EXT_EM_GRP13;*g != '\0';g++,e >>= 1,mac--){
		if(*g != 'x'){
			emtg_changed |= e;
			if(*g == '|'){
				emtgv |= e;
				send_batch_ma_pckt(mac,SET,&prty_info,n);
			}
			else{
				send_batch_ma_pckt(mac,RESET,&prty_info,n);
			}
		}
	}
#endif // X_RAILWAY

	cst_changed=0x0000;
	g=batch_xel.cst;
	cstv=0;
#if ((defined AMPLIFIER_INSTALLED) || (defined HOME_METERING))
	if(*g != 'x'){
		cst_changed |= AMPLIFIER_ACCESS;
		if(*g == '|'){
			cstv |= AMPLIFIER_ACCESS;
			send_batch_ma_pckt(MAC_BATCH_EXT_AMPLIFIER_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_AMPLIFIER_ACCESS,RESET,&prty_info,n);
		}
	}
#endif
	g++;

//#if ((defined BAD_PAYER) || (defined SIREN_INSTALLED ))
	if(*g != 'x'){
		cst_changed |= SIREN_ACCESS;
		if(*g == '|'){
			cstv |= SIREN_ACCESS;
			send_batch_ma_pckt(MAC_BATCH_EXT_SIREN_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_SIREN_ACCESS,RESET,&prty_info,n);
		}
	}
//#endif
	g++;

	if(*g != 'x'){
		cst_changed |= HOT_LINE_ACCESS;
		if(*g == '|'){
			cstv |= HOT_LINE_ACCESS;
			send_batch_ma_pckt(MAC_BATCH_EXT_HOT_LINE_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_HOT_LINE_ACCESS,RESET,&prty_info,n);
		}
	}
	g++;

#ifdef DTMF
	if(*g != 'x'){
		cst_changed |= RING_AGAIN_TIET;
		if(*g == '|'){
			cstv |= RING_AGAIN_TIET;
			send_batch_ma_pckt(MAC_BATCH_EXT_TIE_RSRV,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_TIE_RSRV,RESET,&prty_info,n);
		}
	}
#endif
	g++;

	if(*g != 'x'){
		cst_changed |= CALL_PICKUP_USE;
		if(*g == '|'){
			cstv |= CALL_PICKUP_USE;
			send_batch_ma_pckt(MAC_BATCH_EXT_CPU_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_CPU_ACCESS,RESET,&prty_info,n);
		}
	}
	g++;

//#if ((defined BAD_PAYER) || (defined EM_INSTALLED) || (!defined NO_TIE_INIT))
	if(*g != 'x'){
		cst_changed |= TIE_0;
		if(*g == '|'){
			cstv |= TIE_0;
			send_batch_ma_pckt(MAC_BATCH_EXT_TIE_1ST0,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_TIE_1ST0,RESET,&prty_info,n);
		}
	}
//#endif
	g++;

#ifdef CONFERENCE_ACT
	if(*g != 'x'){
		cst_changed |= CONFERENCE_SERVICE;
		if(*g == '|'){
			cstv |= CONFERENCE_SERVICE;
			send_batch_ma_pckt(MAC_BATCH_EXT_CONF_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_CONF_ACCESS,RESET,&prty_info,n);
		}
	}
#endif
	g++;

//#if ((NO_CO != 0) || (defined BAD_PAYER))
	if(*g != 'x'){
		cst_changed |= RING_AGAIN_COT;
		if(*g == '|'){
			cstv |= RING_AGAIN_COT;
			send_batch_ma_pckt(MAC_BATCH_EXT_CO_RSRV,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_CO_RSRV,RESET,&prty_info,n);
		}
	}
//#endif
	g++;

	if(*g != 'x'){
		cst_changed |= DONT_DIST;
		if(*g == '|'){
			cstv |= DONT_DIST;
			send_batch_ma_pckt(MAC_BATCH_EXT_DONTD_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_DONTD_ACCESS,RESET,&prty_info,n);
		}
	}
	g++;

	if(*g != 'x'){
		cst_changed |= INTERNATIONAL;
		if(*g == '|'){
			cstv |= INTERNATIONAL;
			send_batch_ma_pckt(MAC_BATCH_EXT_CO_2ST0,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_CO_2ST0,RESET,&prty_info,n);
		}
	}
	g++;

	if(*g != 'x'){
		cst_changed |= STD;
		if(*g == '|'){
			cstv |= STD;
			send_batch_ma_pckt(MAC_BATCH_EXT_CO_1ST0,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_CO_1ST0,RESET,&prty_info,n);
		}
	}
	g++;

	if(*g != 'x'){
		cst_changed |= FOLLOW_ME;
		if(*g == '|'){
			cstv |= FOLLOW_ME;
			send_batch_ma_pckt(MAC_BATCH_EXT_FM_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_FM_ACCESS,RESET,&prty_info,n);
		}
	}
	g++;

	if(*g != 'x'){
		cst_changed |= CAMP_ON;
		if(*g == '|'){
			cstv |= CAMP_ON;
			send_batch_ma_pckt(MAC_BATCH_EXT_CAMPON_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_CAMPON_ACCESS,RESET,&prty_info,n);
		}
	}
	g++;

	if(*g != 'x'){
		cst_changed |= CALL_BACK;
		if(*g == '|'){
			cstv |= CALL_BACK;
			send_batch_ma_pckt(MAC_BATCH_EXT_CB_ACCESS,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_CB_ACCESS,RESET,&prty_info,n);
		}
	}

	ast_changed=0x0000;
	g=batch_xel.ast;
	astv=0;

	if(*g != 'x'){
		ast_changed |= FEATURE_ACTIVE;
		if(*g == '|'){
			astv |= FEATURE_ACTIVE;
			send_batch_ma_pckt(MAC_BATCH_EXT_FEATURE,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_FEATURE,RESET,&prty_info,n);
		}
	}

	g++;
	if(*g != 'x'){
		ast_changed |= MALICIOUS_CALL_DTCT;
		if(*g == '|'){
			astv |= MALICIOUS_CALL_DTCT;
			send_batch_ma_pckt(MAC_BATCH_EXT_MALICIOUS_DTCT,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_MALICIOUS_DTCT,RESET,&prty_info,n);
		}
	}
	g++;

	if(*g != 'x'){
		ast_changed |= CALL_GEN;
		if(*g == '|'){
			astv |= CALL_GEN;
			send_batch_ma_pckt(MAC_BATCH_EXT_CALL_GEN,SET,&prty_info,n);
		}
		else{
			send_batch_ma_pckt(MAC_BATCH_EXT_CALL_GEN,RESET,&prty_info,n);
		}
	}

	tot_ext=(unsigned int)(prty_info.lst_prty-prty_info.frst_prty+1);
	h=MAX_NAME_LENGHT+2+(EXTENSION_PARAM_BLOCK_LENGTH*prty_info.frst_prty);
	fseek(fp[n],h,SEEK_SET);

	for(i=0;i<tot_ext;i++){
		if ((ctg_changed & 0xff) != 0){
			ctg_in_file=fgetc(fp[n]);
			fseek(fp[n],-1L,SEEK_CUR);
			fputc(((ctg_in_file & (~ctg_changed) ) | ctgv),fp[n]);
			// between consecutive fputc and fgetc,fseek must! be added.
			fseek(fp[n],0L,SEEK_CUR);
		}
		else{
			fseek(fp[n],1,SEEK_CUR);
		}
#if ((defined EM_GROUP_PROGRAM) || (!defined NO_TIE_INIT))
		if (ttg_changed != 0){
			ttg_in_file=fgetc(fp[n]);
			fseek(fp[n],-1L,SEEK_CUR);
			fputc(((ttg_in_file & (~ttg_changed) ) | ttgv),fp[n]);
			// between consecutive fputc and fgetc,fseek must! be added.
			fseek(fp[n],0L,SEEK_CUR);
		}
		else{
			fseek(fp[n],1,SEEK_CUR);
		}
#else
		if ((ctg_changed & 0xff00) != 0){
			ctg_in_file=fgetc(fp[n]);
			fseek(fp[n],-1L,SEEK_CUR);
			fputc(((ctg_in_file & (~(ctg_changed>>8)) ) | (ctgv>>8)),fp[n]);
			fseek(fp[n],0L,SEEK_CUR);
		}
		else{
			fseek(fp[n],1,SEEK_CUR);
		}
#endif

#ifdef X_RAILWAY
		if (emtg_changed != 0){
			emtg_in_file=my_getw(fp[n]);
			fseek(fp[n],-2L,SEEK_CUR);
			my_putw(((emtg_in_file & (~emtg_changed) ) | emtgv),fp[n]);
			fseek(fp[n],0L,SEEK_CUR);
		}
		else{
			fseek(fp[n],2L,SEEK_CUR);
		}
#endif
		if (cst_changed != 0){
			cst_in_file=my_getw(fp[n]);
			fseek(fp[n],-2L,SEEK_CUR);
			my_putw(((cst_in_file & (~cst_changed) ) | cstv),fp[n]);
			fseek(fp[n],0L,SEEK_CUR);
		}
		else{
			fseek(fp[n],2L,SEEK_CUR);
		}
		if (ast_changed != 0){
			ast_in_file=my_getw(fp[n]);
			fseek(fp[n],-2L,SEEK_CUR);
			my_putw(((ast_in_file & (~ast_changed) ) | astv),fp[n]);
			fseek(fp[n],0L,SEEK_CUR);
		}
		else{
			fseek(fp[n],2L,SEEK_CUR);
		}
		fseek(fp[n],2L,SEEK_CUR); // hli
	}
}
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
BYTE send_batch_ma_pckt(BYTE msg_type,BYTE dat,BATCH_PARTY_ST *p,BYTE n){

	BYTE d[16];
#ifdef DEBUG_ACT
	BYTE i;
#endif
#ifdef NET_DEBUG
	fprintf(net_fp,"btch: %x %x\n",msg_type,n);
#endif

	d[0]=BATCH_MA_T | ((dat&0x0f)<<4);
	d[1]=p->prty_type;
	d[2]=msg_type;
	d[3]=p->frst_prty;
	d[4]=p->frst_prty>>8;
	d[5]=p->lst_prty;
	d[6]=p->lst_prty>>8;
#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
#ifdef HOT_STANDBY
	to_nettx_que(d,MA_BATCH_EXT_PCKT_SIZE,LAPB_LINK0);
	to_nettx_que(d,MA_BATCH_EXT_PCKT_SIZE,LAPB_LINK1);
	return OK; // to prevent warning
#else
	return(to_nettx_que(d,MA_BATCH_EXT_PCKT_SIZE,hot_link));
#endif
#endif
#ifdef HDLC_8993
	return(ttxq(d,MA_BATCH_EXT_PCKT_SIZE,n));
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
#ifdef HOT_STANDBY
	if((tcp_link_0 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_0].tcp_no < max_tcp_connection)){
		put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_0].tcp_no].nettx_st),MA_BATCH_EXT_PCKT_SIZE,d);
	}
	else{
		print_error_report("put_pckt_to_variable_size_buf failed\n");
	}
	if((tcp_link_1 < max_sys_oam_tcp_link) && (tcp_link_st[tcp_link_1].tcp_no < max_tcp_connection)){
		put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[tcp_link_1].tcp_no].nettx_st),MA_BATCH_EXT_PCKT_SIZE,d);
	}
	else{
		print_error_report("put_pckt_to_variable_size_buf failed\n");
	}
	return OK; // to prevent warning
#else
	if((hot_link < max_sys_oam_tcp_link) && (tcp_link_st[hot_link].tcp_no < max_tcp_connection)){
		return(put_pckt_to_variable_size_buf(&(tcp_st[tcp_link_st[hot_link].tcp_no].nettx_st),MA_BATCH_EXT_PCKT_SIZE,d));
	}
	else{
		print_error_report("put_pckt_to_variable_size_buf failed\n");
		return FAIL;
	}
#endif
#endif
#ifdef MY_TCPIP_FOR_OAM
#ifdef HOT_STANDBY
	send_oam_pckt_to_endpoint(0,MA_BATCH_EXT_PCKT_SIZE,d);
	send_oam_pckt_to_endpoint(1,MA_BATCH_EXT_PCKT_SIZE,d);
	return OK; // to prevent warning
#else
	return(send_oam_pckt_to_endpoint(hot_link,MA_BATCH_EXT_PCKT_SIZE,d));
#endif
#endif
}
#endif

/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
int batch_edtp(void){  /* edit extensions' parameters */

	int code;

	if (data_entry_flag == SET){
		code=data_entry(saved_win);
		switch (code){
		  case ENTER :
			batch_edtp_part2();
			data_entry_flag=RESET;
			deletw(saved_win);
			batch_ep_2nd_menu_flag = RESET;
			batch_ep_1st_menu_init();
			saved_b=0;
			return code;
		  case ESC :
			data_entry_flag=RESET;
			deletw(saved_win);
			batch_ep_2nd_menu_flag = RESET;
			batch_ep_1st_menu_init();
			saved_b=0;
			return code;
		  default :
			return code;
		}
	}
	return NO_KEY_PRESSED;
}
#endif
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void batch_edtp_init_display(void){  /* edit digital trunk parameters */

	WINDOW *w;
	int off,c;
	unsigned int d;
	FIELD *f,*tf;
	BYTE no_field;


	w=enw(4,2,11,70);
	if(w == NULL){
		caw();	cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp0:error in memory allocation!");
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

	swt(w," EDIT BATCH OF DIGITAL TRUNKS PARAMETERS ");
	draw_window(w);

	off=-2;
	wprompt(w,1,off+3, " Channel No. ³   From:            To:                             ");
	wprompt(w,1,off+4, "ÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ");
	wprompt(w,1,off+5, " Group No.   ³                                                    ");
	wprompt(w,1,off+6, "ÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ");
	wprompt(w,1,off+7, " Signaling   ³   is icc ogc pts eos mtr opr mal imm wfa ids       ");
	wprompt(w,1,off+8, "   Table     ³                                                    ");

	/* field allocation */
	idew(w);

	f=enf(w,25,off+3,"____",batch_dtel.first_cas,DECIMAL);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_dtel.first_cas,"----");

	f=enf(w,40,off+3,"____",batch_dtel.last_cas,DECIMAL);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_exp:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_dtel.last_cas,"----");

	f=enf(w,18,off+5,"__",batch_dtel.gn,DECIMAL);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp1:error in memory allocation!");
		quit(1);
	}
	strcpy(batch_dtel.gn,"xx");

	f=enf(w,18,off+8,"_",&batch_dtel.is,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp2:error in memory allocation!");
		quit(1);
	}
	batch_dtel.is='x';

	f=enf(w,22,off+8,"_",&batch_dtel.icc,BOOLEAN);
	batch_dtel.icc='x';
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}

	f=enf(w,26,off+8,"_",&batch_dtel.ogc,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.ogc='x';

	f=enf(w,30,off+8,"_",&batch_dtel.pts,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.pts='x';

	f=enf(w,34,off+8,"_",&batch_dtel.eos,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.eos='x';

	f=enf(w,38,off+8,"_",&batch_dtel.mtr,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.mtr='x';

	f=enf(w,42,off+8,"_",&batch_dtel.opr,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.opr='x';

	f=enf(w,46,off+8,"_",&batch_dtel.mal,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.mal='x';

	f=enf(w,50,off+8,"_",&batch_dtel.imm,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.imm='x';

	f=enf(w,54,off+8,"_",&batch_dtel.wfa,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.wfa='x';

	f=enf(w,58,off+8,"_",&batch_dtel.ids,BOOLEAN);
	if(f == NULL){
		caw(); cs();
//DOOD : printf->print_error_report
		print_error_report("\nDEP,batch_edtp3:error in memory allocation!");
		quit(1);
	}
	batch_dtel.ids='x';

	no_field=14;

	/* linking up & down fields */
	for(d=0;f=w->ff,d<no_field;f=f->fnxt,d++){

		for(c=0,tf=f;(c < no_field) && (tf != NULL);c++,tf=tf->fnxt);
		f->fdn=tf;

		for(c=no_field,tf=f;c && (tf != NULL);c--,tf=tf->fprv);
		f->fup=tf;
	}
	saved_win=w;
	data_entry_init(saved_win);
}
#endif
#endif
/* ========================================================================= */
/* ========================================================================= */
#ifdef BATCH_PROGRAMMING
#if( (defined EXTERNAL_CAS3_INSTALLED) || (defined CAS1_INSTALLED) )
void batch_edtp_part2(void){  /* edit digital trunk parameters */


	unsigned int ai_changed,ai;
	unsigned int ai_in_file;
	long h;
	unsigned int d,i,tot_cas;
	BATCH_PARTY_ST prty_info;
	BYTE n;

	n=0;
	prty_info.frst_prty=-1;
	prty_info.lst_prty=-1;

	sscanf(batch_dtel.first_cas,SDNS_FS,&prty_info.frst_prty);
	sscanf(batch_dtel.last_cas,SDNS_FS,&prty_info.lst_prty);

#ifdef EXTERNAL_CAS3_INSTALLED
	prty_info.prty_type=CAS3_T;
#else
#ifdef CAS1_INSTALLED
	prty_info.prty_type=CAS1_T;
#endif
#endif

	prty_info.frst_prty--;
	prty_info.lst_prty--;

#ifdef EXTERNAL_CAS3_INSTALLED
	if ((prty_info.frst_prty >= no_cas3[0]) || (prty_info.lst_prty >= no_cas3[0])){
#else
#ifdef CAS1_INSTALLED
	if ((prty_info.frst_prty >= no_cas1[0]) || (prty_info.lst_prty >= no_cas1[0])){
#endif
#endif
		error_message("Invalid Numbers !");
		return;
	}
	if (prty_info.lst_prty < prty_info.frst_prty){
		error_message("Wrong Numbers !");
		return;
	}
	ai=0;
	ai_changed=0;

//	d=0;
	d=(unsigned int)(-1);
	sscanf(batch_dtel.gn,"%d",&d);
//	d--;
	/* for saving trunk group # from 0 */
#ifdef EXTERNAL_CAS3_INSTALLED
	if(d < no_cas3_programmable_group[0]){
#else
#ifdef CAS1_INSTALLED
	if(d < no_cas1_group[0]){
#endif
#endif
		ai_changed |= CAS3_GROUP_MASK;
		ai= d & CAS3_GROUP_MASK;
		send_batch_ma_pckt(ai+MAC_DGTL_TRNK_GRP0,SET,&prty_info,n);
	}

	if (batch_dtel.is != 'x'){
		ai_changed |= IS;
		if(batch_dtel.is == '|'){
			ai |= IS;
			send_batch_ma_pckt(MAC_IN_SERVICE,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_IN_SERVICE,RESET,&prty_info,n);
	}

	if (batch_dtel.icc != 'x'){
		ai_changed |= ICC;
		if(batch_dtel.icc == '|'){
			ai |= ICC;
			send_batch_ma_pckt(MAC_INCOMING_CH,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_INCOMING_CH,RESET,&prty_info,n);
	}
	if (batch_dtel.ogc != 'x'){
		ai_changed |= OGC;
		if(batch_dtel.ogc == '|'){
			ai |= OGC;
			send_batch_ma_pckt(MAC_OGC,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_OGC,RESET,&prty_info,n);
	}

	if (batch_dtel.pts != 'x'){
		ai_changed |= PTS;
		if(batch_dtel.pts == '|'){
			ai |= PTS;
			send_batch_ma_pckt(MAC_PTS,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_PTS,RESET,&prty_info,n);
	}

	if (batch_dtel.eos != 'x'){
		ai_changed |= EOS;
		if(batch_dtel.eos == '|'){
			ai |= EOS;
			send_batch_ma_pckt(MAC_EOS,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_EOS,RESET,&prty_info,n);
	}

	if (batch_dtel.mtr != 'x'){
		ai_changed |= CAS_METERING;
		if(batch_dtel.mtr == '|'){
			ai |= CAS_METERING;
			send_batch_ma_pckt(MAC_METERING,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_METERING,RESET,&prty_info,n);
	}

	if (batch_dtel.opr != 'x'){
		ai_changed |= OPR_RCL;
		if(batch_dtel.opr == '|'){
			ai |= OPR_RCL;
			send_batch_ma_pckt(MAC_OPR_RCL,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_OPR_RCL,RESET,&prty_info,n);
	}

	if (batch_dtel.mal != 'x'){
		ai_changed |= MAL_CALL;
		if(batch_dtel.mal == '|'){
			ai |= MAL_CALL;
			send_batch_ma_pckt(MAC_MAL_CALL,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_MAL_CALL,RESET,&prty_info,n);
	}
	if (batch_dtel.imm != 'x'){
		ai_changed |= IMM_ACK;
		if(batch_dtel.imm == '|'){
			ai |= IMM_ACK;
			send_batch_ma_pckt(MAC_IMM_ACK,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_IMM_ACK,RESET,&prty_info,n);
	}

	if (batch_dtel.wfa != 'x'){
		ai_changed |= WTFR_ACK;
		if(batch_dtel.wfa == '|'){
			ai |= WTFR_ACK;
			send_batch_ma_pckt(MAC_WTFR_ACK,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_WTFR_ACK,RESET,&prty_info,n);
	}

	if (batch_dtel.ids != 'x'){
		ai_changed |= INTER_SIGNAL_DGT_SEND;
		if(batch_dtel.ids == '|'){
			ai |= INTER_SIGNAL_DGT_SEND;
			send_batch_ma_pckt(MAC_INTR_SIGNAL_DGT_SEND,SET,&prty_info,n);
		}
		else
			send_batch_ma_pckt(MAC_INTR_SIGNAL_DGT_SEND,RESET,&prty_info,n);
	}

	tot_cas=(unsigned int)(prty_info.lst_prty-prty_info.frst_prty+1);
#ifdef EM_GROUP_PROGRAM
	h=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_em[n] * 8) + (no_co[n] * 4) + (prty_info.frst_prty * 2);
#elif (defined NO_TIE_INIT)

	h=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_co[n] * 4) + (prty_info.frst_prty * 2);
#else
	h=MAX_NAME_LENGHT + 2 + (no_extension[n] * EXTENSION_PARAM_BLOCK_LENGTH) + (no_tie[n] * 4) + (no_co[n] * 4) + (prty_info.frst_prty * 2);
#endif
	fseek(fp[n],h,SEEK_SET);

	for(i=0;i<tot_cas;i++){
		if (ai_changed != 0){
			ai_in_file=my_getw(fp[n]);
			fseek(fp[n],-2L,SEEK_CUR);
			my_putw(((ai_in_file & (~ai_changed) ) | ai),fp[n]);
			// between consecutive fputw and fgetw,fseek must! be added.
			fseek(fp[n],0L,SEEK_CUR);
		}
	}
	return;
}
#endif
#endif
/* ========================================================================= */
/* ========================================================================= */

#endif // (OS == DOS) || (OS == LINUX_COMPATIBLE)

