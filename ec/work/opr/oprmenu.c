                        /***  OPRMENU.C  ***/

#include "omtfp.h"
/*
#define DEBUG_ACTIVE
*/

#ifdef OPERATOR_PC
#ifdef ETHERNET_INSTALLED
extern PACKET_DRIVER_STRUCT pktdrvr_st[PK_MAX];
#endif
extern OPR_CALL_REC ocr;
extern int VSG;
extern CHAT_TYPE attr[][80];
extern BYTE scrnbuf[][80],scrnbuf1[][50][80];
extern BYTE lcdbufptr,key_table[][80],flag;
extern BYTE lsbs[],msbs[],key_code[],trf;
extern struct LED_POS led_table[];
#ifdef NEW_DEP
extern struct LED_POS dep_led_table[];
#endif
extern FILE *fer;
extern STAT_IN_OPR ctst[],ttst[];

#if COMPILER == GCC
extern unsigned int f;
extern unsigned int tptw;
extern BYTE pph;
extern unsigned int time_tab[];
#if (OS == DOS) || (OS == WINDOWS)
_go32_dpmi_seginfo  oldisr;
#endif
//CHEEN start : gom shodeh
#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
extern unsigned char *isa_mem;
#endif
//CHEEN end
#endif


#if COMPILER == GCC
#if (OS == DOS)
_go32_dpmi_seginfo  oldvect;
#endif
#else
void interrupt (*oldvect)(void);
#endif
static CHAT_TYPE dget(BYTE,BYTE);
extern BYTE *oplbuf,*chlbuf;
extern BYTE *foplbuf,*fchlbuf;
extern BYTE savescrn[][80];
#endif


#ifdef DISPLAY_STATUS_ACT
extern BYTE ds[];
#endif

#ifdef DISPLAY_EM_STATUS
extern BYTE dts[];
extern BYTE dtschng[];
#endif

#ifdef DISPLAY_CAS3_STATUS
extern BYTE dcs[];
extern BYTE dcschng[];

extern BYTE dcsict[];
extern BYTE dcsictchng[];

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



#ifndef OPERATOR_PC
extern xdata STAT_IN_OPR ctst[],ttst[];
extern xdata BYTE trf;
extern xdata BYTE sp[];
extern xdata BYTE lpb,lpnb;
#endif

#ifdef OPERATOR_FARSI
extern char lcd_1l_buf[],lcd_2l_buf[];
#endif


extern unsigned int f;

#if ((defined DISPLAY_EM_STATUS) || (defined DISPLAY_CAS1_STATUS))
extern char *emstate_name[];
extern int color[];
#endif
#ifdef DISPLAY_CAS3_STATUS
extern char *ceptstate_name[NO_CAS3_STATES];
extern int cas3_color[];
#ifdef ISUP
extern char *isupstate_name[];
extern int isup_color[];
#endif
#endif

#ifdef NEW_DISPLAY_STATUS
extern BYTE current_page;
#endif
#ifdef NEW_DEP
extern BYTE dep_flag;
#endif

#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
extern BYTE sync_led[];
extern BYTE link_up_led[];
#endif

//CHEEN start
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
extern unsigned int max_sys_oam_tcp_link;
#endif
//CHEEN start

#ifdef DISPLAY_CAS3_STATUS
extern BYTE current_cas3_page;
#endif
//MESR start
#ifdef DISPLAY_BILLING
extern BYTE bill_display_flag;
#endif
//MESR end

#ifdef DISPLAY_ALARM
extern BYTE alarm_display_flag;
#endif

#ifdef OPERATOR_PC
void putlcd(BYTE ch){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        BYTE i;
        if(lcdbufptr<40) {
             i=lcdbufptr+19;
             displscrn(i++,2,ch,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
             if(i<59) cursor(i,2);
             else     cursor(19,3);
        }
        else {
             i=lcdbufptr-21;            /* -40+19 */

             displscrn(i++,3,ch,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
             if(i<59) cursor(i,3);
             else     cursor(19,2);
        }
        lcdbufptr++;
        if(lcdbufptr==80) lcdbufptr=0;
#endif
}
/*
void putlcd(BYTE ch){
        if(lcdbufptr<40) displscrn(lcdbufptr+19,2,ch,7);
        else             displscrn(lcdbufptr-40+19,3,ch,7);
        lcdbufptr++;
        if(lcdbufptr==80) lcdbufptr=0;
}
*/
#endif


#ifdef OPERATOR_PC
BYTE lcdsts(){
        return 0; /* to simulate a "non-busy" lcd */
}
#endif


#ifdef OPERATOR_PC
void lcdcmd(BYTE cmd){
        BYTE i;
        if((cmd & 0x80)==0x80){  /* set DD RAM address */
                i=cmd&0x7f;
                if(i>0x00 && i<0x27) lcdbufptr=i;  /* based on a two line display */
                if(i>0x40 && i<0x67) lcdbufptr=i-0x40+40;
        }
        if((cmd & 0xc0)==0x40){ /* set CG RAM address */
        }
        if((cmd & 0xe0)==0x20){ /* Function Set */
                if((cmd&0x10)==0x10) /* DL=1: 8bit mode */;
                else                 /* DL=0: 4bit mode */;
                if((cmd&0x08)==0x08) /*  N=1: 2 lines   */;
                else                 /*  N=0: 1 line    */;
                if((cmd&0x04)==0x04) /*  F=1: Font N.U. */;
                else                 /*  F=0: Font 5x7  */;
        }
        if((cmd & 0xf0)==0x10){ /* Cursor or Display Shift */
                if((cmd&0x08)==0x08) /* S/C=1: Display shift  */;
                else                 /* S/C=0: Cursor move    */;
                if((cmd&0x04)==0x04) /* R/L=1: Shift to right */;
                else                 /* R/L=0: Shift to left  */;
        }
        if((cmd & 0xf8)==0x08){ /* Display On/Off Control */
                if((cmd&0x04)==0x04) /* D=1: Display on  */;
                else                 /* D=0: Display off */;
                if((cmd&0x02)==0x02) /* C=1: Cursor on   */;
                else                 /* C=0: Cursor off  */;
                if((cmd&0x01)==0x01) /* B=1: Cursor flash enable */;
                else                 /* B=0: No cursor flash     */;
        }
        if((cmd & 0xfc)==0x04){ /* Entry Mode Set */
                if((cmd&0x02)==0x02) /* I/D=1: Increment(Display Shift) */;
                else                 /* I/D=0: Decrement(Display Shift) */;
                if((cmd&0x01)==0x01) /* S=1: Shift the display */;
                else                 /* S=0: Do not Shift      */;
        }
        if((cmd & 0xfe)==0x02){ /* Return Home (cursor) */
                lcdbufptr=0;
        }
        if((cmd & 0xff)==0x01){ /* Clear Display */
                lcdbufptr=0;
                for(i=0;i<80;i++) putlcd(' ');
        }
}
#endif

void init_lcd(){

#ifdef OPERATOR_PC

#ifndef DEBUG_ACTIVE
        lcdcmd(TWO_LINE_INIT);
        lcdcmd(TWO_LINE_INIT);
        lcdcmd(TWO_LINE_INIT);
        lcdcmd(0x06);
        lcdcmd(0x0e);
        lcdcmd(0x01);
#endif

#else
        tolcdq(TWO_LINE_INIT,TO_LCD_IR);
        tolcdq(TWO_LINE_INIT,TO_LCD_IR);
        tolcdq(TWO_LINE_INIT,TO_LCD_IR);
        tolcdq(0x06,TO_LCD_IR);
        tolcdq(0x0e,TO_LCD_IR);
        tolcdq(0x01,TO_LCD_IR);
#endif
}

#ifdef OPERATOR_PC

int reset_mouse(void){
#ifdef HDLC_RS232_ON_OPERATOR_SIDE
        return FAIL;
#else
#if (OS == DOS)
        union REGS regs;
        regs.x.ax=0;
        int86(0x33,&regs,&regs);
        return regs.x.ax;
#endif
#endif
}
void display_mouse_ptr(void){
#ifndef HDLC_RS232_ON_OPERATOR_SIDE
#if (OS == DOS)
        union REGS regs;
        regs.x.ax=1;
        int86(0x33,&regs,&regs);
#endif
#endif
}
void remove_mouse_ptr(void){
#if (OS == DOS)
#ifndef HDLC_RS232_ON_OPERATOR_SIDE
        union REGS regs;
        regs.x.ax=2;
        int86(0x33,&regs,&regs);
#endif
#endif
}
unsigned int get_mouse_status(unsigned int *x, unsigned int *y){
#if (OS == DOS)
        union REGS regs;
        regs.x.ax=3;
        regs.x.bx=0;
        int86(0x33,&regs,&regs);
        *x=regs.x.cx/8;
        *y=regs.x.dx/8;
        return regs.x.bx;
#endif
}
#endif


#ifdef OPERATOR_PC
void init_file(void){
        BYTE i,j; int k,nitem;
        BYTE l;
/*      FILE *fd,*fopen();*/
        BYTE *tmpbuf;
#if (OS == DOS)
        VSG=(vmode()==7 ? 0xb000 : 0xb800);
#endif
#if(OS ==  LINUX_COMPATIBLE)
        VSG=0xb800;
#endif

#if 0
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)  /* if Farsi flag set */
                fd=fopen("oplayof","rb");  /* open Farsi layout file */
        else  /* English layout */
#endif
                fd=fopen("oplayo","rb");  /* open English layout file */
        nitem=fread(tmpbuf,1,2050,fd);
        fclose(fd);

#endif // 0
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)  /* if Farsi flag set */
                tmpbuf=foplbuf;  /* open Farsi layout file */
        else  /* English layout */
#endif
                tmpbuf=oplbuf;  /* open English layout file */
        nitem=2050;
        k=0;
        for(i=0;i<25;i++)
                for(j=0;j<80;j++) {
                        while(tmpbuf[k]==0x0d || tmpbuf[k]==0x0a) k++;
                        if(k>=nitem) goto end;
                        scrnbuf[i][j]=tmpbuf[k++];
                }
end:


#if 0
#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG)  /* if Farsi flag set */
                fd=fopen("chlayof","rb");  /* open Farsi status display */
        else  /* English staus */
#endif
                fd=fopen("chlayo","rb");  /* open English status display */
        nitem=fread(tmpbuf,1,2050,fd);
        fclose(fd);

#endif // 0
        for(l=0;l<70;l++){
                for(k=0;k<1000;k++){
                        if((l*1000+k) >= NO_TOTAL_EXTENSION)
                                sprintf((char *)chlbuf+(k<<2)," %03d",0);
                        else
//                              sprintf((char *)chlbuf+(k<<2)," %03d",(unsigned int)(find_subscriber_no(l*1000+k) % 1000));
                                sprintf((char *)chlbuf+(k<<2),"%04d",(unsigned int)(find_subscriber_no(l*1000+k) % 10000));
                }
                *(chlbuf+4000)=0;
#ifdef OPERATOR_FARSI
                for(k=0;k<4001;k++)
                        if(isdigit(*(chlbuf + k) ) )
                                *(fchlbuf + k) = *(chlbuf + k) + '€' - '0';
                        else
                                *(fchlbuf + k) = *(chlbuf + k);
#endif
#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG)  /* if Farsi flag set */
                        tmpbuf=fchlbuf;  /* open Farsi status display */
                else  /* English status */
#endif
                        tmpbuf=chlbuf;  /* open English status display */
                nitem=4100;
                k=0;
                for(i=0;i<50;i++){
                        for(j=0;j<80;j++) {
                                while(tmpbuf[k]==0x0d || tmpbuf[k]==0x0a) k++;
                                if(k>=nitem) goto end1;
                                scrnbuf1[l][i][j]=tmpbuf[k++];
                        }
                }
end1:
                continue;
        }
        return;
}
#endif


#ifdef OPERATOR_PC
void init_layo(void){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        BYTE i,j;
#if (OS == DOS)
        textmode(C80);
#endif
        for(i=0;i<25;i++)
                for(j=0;j<80;j++) attr[i][j]=MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE);
        for(i=0;i<25;i++)
                for(j=0;j<80;j++)
                        displscrn(j,i,scrnbuf[i][j],attr[i][j]);
        init_lcd();
        set_cursor_type(0x0607);
        cursor(0,25);
#endif
}
#endif


#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void restore_screen(void){
        BYTE i,j;
        for(i=0;i<25;i++)
                for(j=0;j<80;j++)
                        displscrn(j,i,savescrn[i][j],attr[i][j]);
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void save_screen(void){
        CHAT_TYPE chat;
        BYTE i,j;
        for(i=0;i<25;i++)
                for(j=0;j<80;j++) {
                        chat=dget(j,i);
                        attr[i][j]=FIND_ATTR_FROM_CHAT(chat);
                        savescrn[i][j]=FIND_CHAR_FROM_CHAT(chat);
                }
}
#endif
#endif


#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
#ifdef DISPLAY_STATUS_ACT
#ifndef NEW_DISPLAY_STATUS
void chng_layo(BYTE *ds){
        int kext;
        BYTE i,j;
#if 0
        for(kext=0;kext < NO_TOTAL_EXTENSION;kext++){
                i=kext/20;
                if (i>=50) i-=50;
                j=(kext%20)<<2;
                if(ds[kext]==OFF_HOOK){
                        vpoke(VSG,vad(j,i),scrnbuf1[i][j]|0x700);
                        vpoke(VSG,vad(j+1,i),scrnbuf1[i][j+1]|0x700);
                        vpoke(VSG,vad(j+2,i),scrnbuf1[i][j+2]|0x700);
                        vpoke(VSG,vad(j+3,i),scrnbuf1[i][j+3]|0x700);
                }
                else
                        if(ds[kext]==ON_HOOK){
                                vpoke(VSG,vad(j,i),scrnbuf1[i][j]);
                                vpoke(VSG,vad(j+1,i),scrnbuf1[i][j+1]);
                                vpoke(VSG,vad(j+2,i),scrnbuf1[i][j+2]);
                                vpoke(VSG,vad(j+3,i),scrnbuf1[i][j+3]);
                        }
                        else{
                                vpoke(VSG,vad(j,i),scrnbuf1[i][j]|0x400);
                                vpoke(VSG,vad(j+1,i),scrnbuf1[i][j+1]|0x400);
                                vpoke(VSG,vad(j+2,i),scrnbuf1[i][j+2]|0x400);
                                vpoke(VSG,vad(j+3,i),scrnbuf1[i][j+3]|0x400);
                        }
        }
        if(trf & RECALL_QUE_ACT){
                vpoke(VSG,vad(0,49),'R'|0x400);
                vpoke(VSG,vad(1,49),'C'|0x400);
                vpoke(VSG,vad(2,49),'L'|0x400);
        }
        else{
                vpoke(VSG,vad(0,49),'R');
                vpoke(VSG,vad(1,49),'C');
                vpoke(VSG,vad(2,49),'L');
        }
        if(trf & EXTRNL_QUE_ACT){
                vpoke(VSG,vad(4,49),'E'|0x400);
                vpoke(VSG,vad(5,49),'X'|0x400);
                vpoke(VSG,vad(6,49),'T'|0x400);
        }
        else{
                vpoke(VSG,vad(4,49),'E');
                vpoke(VSG,vad(5,49),'X');
                vpoke(VSG,vad(6,49),'T');
        }
        if(trf & INTRNL_QUE_ACT){
                vpoke(VSG,vad(8,49),'I'|0x400);
                vpoke(VSG,vad(9,49),'N'|0x400);
                vpoke(VSG,vad(10,49),'T'|0x400);
        }
        else{
                vpoke(VSG,vad(8,49),'I');
                vpoke(VSG,vad(9,49),'N');
                vpoke(VSG,vad(10,49),'T');
        }
#endif // 0
        for(kext=0;kext < NO_TOTAL_EXTENSION;kext++){
                i=kext/20;
                if (i>=50) i-=50;
                j=(kext%20)<<2;
                if(ds[kext]==OFF_HOOK){
                        kt_print_char(j,i,scrnbuf1[i][j]    ,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                        kt_print_char(j+1,i,scrnbuf1[i][j+1],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                        kt_print_char(j+2,i,scrnbuf1[i][j+2],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                        kt_print_char(j+3,i,scrnbuf1[i][j+3],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
                else
                        if(ds[kext]==ON_HOOK){
                                kt_print_char(j,i,scrnbuf1[i][j]    ,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                                kt_print_char(j+1,i,scrnbuf1[i][j+1],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                                kt_print_char(j+2,i,scrnbuf1[i][j+2],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                                kt_print_char(j+3,i,scrnbuf1[i][j+3],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                        }
                        else{
                                kt_print_char(j,i,scrnbuf1[i][j]    ,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                                kt_print_char(j+1,i,scrnbuf1[i][j+1],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                                kt_print_char(j+2,i,scrnbuf1[i][j+2],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                                kt_print_char(j+3,i,scrnbuf1[i][j+3],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                        }
        }
        if(trf & RECALL_QUE_ACT){
                kt_print_char(0,49,'R',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                kt_print_char(1,49,'C',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                kt_print_char(2,49,'L',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
        }
        else{
                kt_print_char(0,49,'R',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(1,49,'C',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(2,49,'L',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
        }
        if(trf & EXTRNL_QUE_ACT){
                kt_print_char(4,49,'E',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                kt_print_char(5,49,'X',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                kt_print_char(6,49,'T',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
        }
        else{
                kt_print_char(4,49,'E',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(5,49,'X',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(6,49,'T',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
        }
        if(trf & INTRNL_QUE_ACT){
                kt_print_char(8 ,49,'I',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                kt_print_char(9 ,49,'N',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                kt_print_char(10,49,'T',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
        }
        else{
                kt_print_char(8, 49,'I',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(9, 49,'N',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(10,49,'T',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
        }
}
#endif
#endif
#endif
#endif

#ifdef OPERATOR_PC
#ifdef DISPLAY_STATUS_ACT
#ifdef NEW_DISPLAY_STATUS
void opr_update(int kext){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        BYTE i,j,l;
        unsigned int ii;
        ii=kext/20;
        i=ii%50;
        l=ii/50;
#if (NO_TOTAL_EXTENSION > 1000)
        if(l != current_page) return;
#endif
        j=(kext%20)<<2;
#if 0
        if(ds[kext]==OFF_HOOK){
                vpoke(VSG,vad(j  ,i),scrnbuf1[l][i][j  ]|0x200);
                vpoke(VSG,vad(j+1,i),scrnbuf1[l][i][j+1]|0x700);
                vpoke(VSG,vad(j+2,i),scrnbuf1[l][i][j+2]|0x700);
                vpoke(VSG,vad(j+3,i),scrnbuf1[l][i][j+3]|0x700);
        }
        else
                if(ds[kext]==ON_HOOK){
                        vpoke(VSG,vad(j  ,i),scrnbuf1[l][i][j  ]);
                        vpoke(VSG,vad(j+1,i),scrnbuf1[l][i][j+1]);
                        vpoke(VSG,vad(j+2,i),scrnbuf1[l][i][j+2]);
                        vpoke(VSG,vad(j+3,i),scrnbuf1[l][i][j+3]);
                }
                else
                if(ds[kext]==AT_LOCK){
                        vpoke(VSG,vad(j  ,i),scrnbuf1[l][i][j  ] | 0xfa00 );
                        vpoke(VSG,vad(j+1,i),scrnbuf1[l][i][j+1] | 0xf000 /*(clr(WHITE,BLACK,DIM) << 8)*/);
                        vpoke(VSG,vad(j+2,i),scrnbuf1[l][i][j+2] | 0xf000 /*(clr(WHITE,BLACK,DIM) << 8)*/);
                        vpoke(VSG,vad(j+3,i),scrnbuf1[l][i][j+3] | 0xf000 /*(clr(WHITE,BLACK,DIM) << 8)*/);
                }
                else{
                        vpoke(VSG,vad(j  ,i),scrnbuf1[l][i][j  ]|0x200);
                        vpoke(VSG,vad(j+1,i),scrnbuf1[l][i][j+1]|0x400);
                        vpoke(VSG,vad(j+2,i),scrnbuf1[l][i][j+2]|0x400);
                        vpoke(VSG,vad(j+3,i),scrnbuf1[l][i][j+3]|0x400);
                }
#endif // 0
        if(ds[kext]==OFF_HOOK){
                kt_print_char(j  ,i,scrnbuf1[l][i][j  ],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_GREEN));
                kt_print_char(j+1,i,scrnbuf1[l][i][j+1],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                kt_print_char(j+2,i,scrnbuf1[l][i][j+2],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                kt_print_char(j+3,i,scrnbuf1[l][i][j+3],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        }
        else
                if(ds[kext]==ON_HOOK){
                        kt_print_char(j  ,i,scrnbuf1[l][i][j  ],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                        kt_print_char(j+1,i,scrnbuf1[l][i][j+1],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                        kt_print_char(j+2,i,scrnbuf1[l][i][j+2],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                        kt_print_char(j+3,i,scrnbuf1[l][i][j+3],MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                }
                else
                if(ds[kext]==AT_LOCK){
                        kt_print_char(j  ,i,scrnbuf1[l][i][j  ] ,MAKE_ATTR(KT_BLINKING,KT_WHITE,KT_HIGH_INTENSITY,KT_GREEN));
                        kt_print_char(j+1,i,scrnbuf1[l][i][j+1] ,MAKE_ATTR(KT_NO_BLINKING,KT_WHITE,KT_LOW_INTENSITY,KT_BLACK) /*(clr(WHITE,BLACK,DIM) << 8)*/);
                        kt_print_char(j+2,i,scrnbuf1[l][i][j+2] ,MAKE_ATTR(KT_NO_BLINKING,KT_WHITE,KT_LOW_INTENSITY,KT_BLACK) /*(clr(WHITE,BLACK,DIM) << 8)*/);
                        kt_print_char(j+3,i,scrnbuf1[l][i][j+3] ,MAKE_ATTR(KT_NO_BLINKING,KT_WHITE,KT_LOW_INTENSITY,KT_BLACK) /*(clr(WHITE,BLACK,DIM) << 8)*/);
                }
                else{
                        kt_print_char(j  ,i,scrnbuf1[l][i][j  ] ,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_GREEN));
                        kt_print_char(j+1,i,scrnbuf1[l][i][j+1] ,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                        kt_print_char(j+2,i,scrnbuf1[l][i][j+2] ,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                        kt_print_char(j+3,i,scrnbuf1[l][i][j+3] ,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                }
#endif
}
#endif
#endif
#endif

#ifdef OPERATOR_PC
#ifdef DISPLAY_STATUS_ACT
#ifdef NEW_DISPLAY_STATUS
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void chng_layo(void){
#if 0
        if(trf & RECALL_QUE_ACT){
                vpoke(VSG,vad(1,49),'R'|0x400);
                vpoke(VSG,vad(2,49),'C'|0x400);
                vpoke(VSG,vad(3,49),'L'|0x400);
        }
        else{
#if (NO_TOTAL_EXTENSION > 982)
                opr_update(980);
#else
                vpoke(VSG,vad(1,49),'R');
                vpoke(VSG,vad(2,49),'C');
                vpoke(VSG,vad(3,49),'L');
#endif
        }
        if(trf & EXTRNL_QUE_ACT){
                vpoke(VSG,vad(5,49),'E'|0x400);
                vpoke(VSG,vad(6,49),'X'|0x400);
                vpoke(VSG,vad(7,49),'T'|0x400);
        }
        else{
#if (NO_TOTAL_EXTENSION > 982)
                opr_update(981);
#else
                vpoke(VSG,vad(5,49),'E');
                vpoke(VSG,vad(6,49),'X');
                vpoke(VSG,vad(7,49),'T');
#endif
        }
        if(trf & INTRNL_QUE_ACT){
                vpoke(VSG,vad(9,49),'I'|0x400);
                vpoke(VSG,vad(10,49),'N'|0x400);
                vpoke(VSG,vad(11,49),'T'|0x400);
        }
        else{
#if (NO_TOTAL_EXTENSION > 982)
                opr_update(982);
#else
                vpoke(VSG,vad(9,49),'I');
                vpoke(VSG,vad(10,49),'N');
                vpoke(VSG,vad(11,49),'T');
#endif
        }
#endif // 0
        if(trf & RECALL_QUE_ACT){
                kt_print_char(1,49,'R',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                kt_print_char(2,49,'C',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                kt_print_char(3,49,'L',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
        }
        else{
#if (NO_TOTAL_EXTENSION > 982)
                opr_update(980);
#else
                kt_print_char(1,49,'R',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(2,49,'C',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(3,49,'L',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
#endif
        }
        if(trf & EXTRNL_QUE_ACT){
                kt_print_char(5,49,'E',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                kt_print_char(6,49,'X',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                kt_print_char(7,49,'T',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
        }
        else{
#if (NO_TOTAL_EXTENSION > 982)
                opr_update(981);
#else
                kt_print_char(5,49,'E',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(6,49,'X',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(7,49,'T',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
#endif
        }
        if(trf & INTRNL_QUE_ACT){
                kt_print_char(9, 49,'I',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                kt_print_char(10,49,'N',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
                kt_print_char(11,49,'T',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED  ));
        }
        else{
#if (NO_TOTAL_EXTENSION > 982)
                opr_update(982);
#else
                kt_print_char(9,49, 'I',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(10,49,'N',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
                kt_print_char(11,49,'T',MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLACK));
#endif
        }
}
#endif
#endif
#endif
#endif  // OPERATOR_PC

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
#ifdef DISPLAY_EM_STATUS
#define NO_TRUNKS_DISPLAYED_PER_COLUMN  20      // 30 for textmode(C4350)
#define FIRST_ROW_OFFSET                2       // 9  for textmode(C4350)
void disp_trunk_no(void){
        BYTE ktrnk;
        BYTE i,j;
        char trunk_no[5];

        for(ktrnk=0;ktrnk < NO_EM_DISPLAY;ktrnk++){
                i=ktrnk%NO_TRUNKS_DISPLAYED_PER_COLUMN + FIRST_ROW_OFFSET;  // row number
                j=(ktrnk/NO_TRUNKS_DISPLAYED_PER_COLUMN)*26 + 1;     // col number. 80/3 =26, 3 columns in one screen
                sprintf(trunk_no,"%1.2d :",ktrnk+1);    // 4 characters
                poke_s(j,i,trunk_no,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        }
}

void disp_trunk(void){
        BYTE ktrnk;
        BYTE i,j;
        BYTE state;
        for(i=FIRST_ROW_OFFSET,j=5,ktrnk=0;ktrnk < NO_EM_DISPLAY;ktrnk++,i++){
                if(i==(FIRST_ROW_OFFSET+NO_TRUNKS_DISPLAYED_PER_COLUMN)){
                        i=FIRST_ROW_OFFSET;
                        j+=26;
                }
//              i=ktrnk%NO_TRUNKS_DISPLAYED_PER_COLUMN + FIRST_ROW_OFFSET;     // row number
//                j=(ktrnk/NO_TRUNKS_DISPLAYED_PER_COLUMN)*26 + 1 + 4;     // col number
/*
#ifdef DISPLAY_CAS3_STATUS
                if((state=dts[ktrnk])==UNKNOWN){
                        vpoke(VSG,vad(j  ,i),'?'|0x700);
                        vpoke(VSG,vad(j+1,i),'?'|0x700);
                        vpoke(VSG,vad(j+2,i),'?'|0x700);
                        vpoke(VSG,vad(j+3,i),'?'|0x700);
                }
                else{
                        state &= 0x0f;
                        vpoke(VSG,vad(j  ,i),((state&8) ? '1':'0')|0x700);
                        vpoke(VSG,vad(j+1,i),((state&4) ? '1':'0')|0x700);
                        vpoke(VSG,vad(j+2,i),((state&2) ? '1':'0')|0x700);
                        vpoke(VSG,vad(j+3,i),((state&1) ? '1':'0')|0x700);
                        switch(state){
                                case    1:
                                        poke_s(j+5,i,"Seized",WHITE<<8);
                                        break;
                                case    3:
                                        poke_s(j+5,i,"Congestion",WHITE<<8);
                                        break;
                                case    5:
                                        poke_s(j+5,i,"Answered",WHITE<<8);
                                        break;
                                case    9:
                                        poke_s(j+5,i,"CLF/Idle",WHITE<<8);
                                        break;
                                case    0xd:
                                        poke_s(j+5,i,"Seize Ack/CLB/BLK",WHITE<<8);
                                        break;
                                case    0xf:
                                        poke_s(j+5,i,"PTS",WHITE<<8);
                                        break;
                                default:
                                        poke_s(j+5,i,"                 ",WHITE<<8);
                                        break;
                        }
                }
#endif
*/
#ifdef DISPLAY_EM_STATUS
                if(!dtschng[ktrnk]) continue;
                dtschng[ktrnk]=RESET;
                if((state=dts[ktrnk])==OUT_OF_SERVICE){
                        poke_s(j,i,"Out of Service   ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE));        // 17 characters
                }
                else if(state==OGT){
                        poke_s(j-2,i,"O",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
                else if(state==ICT){
                        poke_s(j-2,i,"I",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
                else if(state==IDLE){   // IDLE in omtfp.h equals IDLE in dsmtfp.h
                        poke_s(j,i,"Idle             ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
                else{
#ifdef RMT_EM
                        if ((state >= RMT_EM_STATES_START) && (state <= RMT_EM_STATES_END)){
                                state=state-RMT_EM_STATES_START+EM_CAS3_STATES_SIZE;
                                poke_s(j,i,emstate_name[state],color[state]);
                        }
                        else
#endif
                        {

                                state -= EM_STATES_START;
#if((defined LIM) || (defined TRANSIT_DT_EM))
                                if( state <(NO_EM_STATES+NO_CAS3_STATES) )
#else
                                if( state <NO_EM_STATES )
#endif
                                        poke_s(j,i,emstate_name[state],color[state]);
                                else
                                        poke_s(j,i,"                 ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                        }

                }
#endif
        }
}
#endif
#endif
#endif


#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
#ifdef DISPLAY_CAS3_STATUS
#define NO_CAS3_DISPLAYED_PER_COLUMN    15 //50 //30      // 30 for textmode(C4350)
#define CAS3_FIRST_ROW_OFFSET           5  //9       // 9  for textmode(C4350)
void disp_cas3_no(void){
        unsigned int ktrnk;
        BYTE i,j;
        BYTE k;
        char trunk_no[11];

        sprintf(trunk_no,"%1.4d/%1.2d=%1.2d",(current_cas3_page+1)*NO_CAS3_DISPLAYED_PER_PAGE,NO_CAS3_DISPLAYED_PER_PAGE,current_cas3_page+1);    // 10 characters
        poke_s(1,1,trunk_no,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));  // col,row
        for(k=0,ktrnk=current_cas3_page*NO_CAS3_DISPLAYED_PER_PAGE; (k < NO_CAS3_DISPLAYED_PER_PAGE) && (ktrnk < NO_CAS3_DISPLAY) ;k++,ktrnk++){
                i=ktrnk%NO_CAS3_DISPLAYED_PER_COLUMN + CAS3_FIRST_ROW_OFFSET;  // row number
                j=(k/NO_CAS3_DISPLAYED_PER_COLUMN)*26 + 1;     // col number. 80/3 =26, 3 columns in one screen
                sprintf(trunk_no,"%1.4d :",ktrnk+1);    // 6 characters
                poke_s(j,i,trunk_no,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        }
        for(k=0,ktrnk=current_cas3_page*NO_CAS3_DISPLAYED_PER_PAGE;(k<NO_CAS3_DISPLAYED_PER_PAGE) && (ktrnk<NO_CAS3_DISPLAY);k++,ktrnk++){
                dcs[ktrnk]=UNKNOWN;
                dcschng[ktrnk]=SET;
                dcsict[ktrnk]=UNKNOWN;
                dcsictchng[ktrnk]=SET;
#ifdef DISPLAY_CAS3_LINE_STATUS
                dcsb[ktrnk]=UNKNOWN;
                dcsbchng[ktrnk]=SET;
#endif
        }
}

void disp_cas3(void){
        unsigned int ktrnk;
        BYTE i,j;
        BYTE state,tmpstate;

#ifdef DISPLAY_CAS3_LINE_STATUS
        char sbit[5];
        BYTE bit,b;
#endif
        BYTE k;

        for(i=CAS3_FIRST_ROW_OFFSET,j=7,k=0,ktrnk=current_cas3_page*NO_CAS3_DISPLAYED_PER_PAGE; (k < NO_CAS3_DISPLAYED_PER_PAGE) && (ktrnk < NO_CAS3_DISPLAY) ;ktrnk++,i++,k++){
            if(i==(CAS3_FIRST_ROW_OFFSET+NO_CAS3_DISPLAYED_PER_COLUMN)){
                i=CAS3_FIRST_ROW_OFFSET;
                j+=26;
            }

            if(dcschng[ktrnk]){
                dcschng[ktrnk]=RESET;
                if((state=dcs[ktrnk])==OUT_OF_SERVICE){
                    poke_s(j,i,"OutOf Service",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE));        // 17 characters
                }
                else if(state==UNKNOWN){
                    poke_s(j,i,"Unknown......",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE));        // 17 characters
                }
                else if(state==IDLE){   // IDLE in omtfp.h equals IDLE in dsmtfp.h
                    poke_s(j,i,"Idle.........",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
                else if(state==HOLD){   // HOLD in omtfp.h equals HOLD in dsmtfp.h
                    poke_s(j,i,"Hold.........",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
                else if(state==IN_EXTRNL_QU){   // IN_EXTRNL_QU in omtfp.h equals IN_EXTRNL_QU in dsmtfp.h
                    poke_s(j,i,"In Extrnl Que",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
#ifdef CHANNEL_TEST
                else if(state==CHANNEL_TEST_CONNECTED){   // CHANNEL_TEST_CONNECTED in omtfp.h equals CHANNEL_TEST_CONNECTED in dsmtfp.h
                    poke_s(j,i,"Channel Test.",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
#endif
                else{
                    tmpstate=state-CAS3_STATES_START;
                    if(tmpstate < NO_CAS3_STATES){
                        poke_s(j,i,ceptstate_name[tmpstate],cas3_color[tmpstate]);
                        goto next;
                    }
#ifdef TRANSIT_DT_EM
                    tmpstate=state-EM_STATES_START;
                    if(tmpstate < NO_EM_STATES){
                        poke_s(j,i,emstate_name[tmpstate],color[tmpstate]);
                        goto next;
                    }
#endif
#ifdef ISUP
                    tmpstate=state-ISUP_STATES_START;
                    if(tmpstate < NO_ISUP_STATES){
                        poke_s(j,i,isupstate_name[tmpstate],isup_color[tmpstate]);
                        goto next;
                    }
#endif
                    poke_s(j,i,".............",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                }
            }
            if(dcsictchng[ktrnk]){
                dcsictchng[ktrnk]=RESET;
                if((state=dcsict[ktrnk])==OGT){
                    poke_s(j-2,i,"O",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW));
                }
                else if(state==ICT){
                    poke_s(j-2,i,"I",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW));
                }
                else if(state==UNKNOWN){
                    poke_s(j-2,i,"U",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW));
                }
            }
next:
#ifdef DISPLAY_CAS3_LINE_STATUS
            if(!dcsbchng[ktrnk]) continue;
            dcsbchng[ktrnk]=RESET;
            state=dcs[ktrnk];
            if((bit=dcsb[ktrnk])==UNKNOWN){
#if 0
                    poke_s((j+13),i,"????",BLUE<<8); // .............????  ³  next column
//                  poke_s((j+19),i,"|",RED<<8);
                    poke_s((j+17),i,"|",RED<<8);
#endif // 0
                    poke_s((j+13),i,"????",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE)); // .............????  ³  next column
//                  poke_s((j+19),i,"|",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                    poke_s((j+17),i,"|",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
            }
            else{
                    b=((bit >> 3) & 1);
                    sprintf(sbit,"%1.1d",b);
                    poke_s((j+13),i,sbit,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));//,cas3_color[state]);
                    b=(bit >> 2) & 1;
                    sprintf(sbit,"%1.1d",b);
                    poke_s((j+14),i,sbit,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));//,cas3_color[state]);
                    b=(bit >> 1) & 1;
                    sprintf(sbit,"%1.1d",b);
                    poke_s((j+15),i,sbit,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));//,cas3_color[state]);
                    b=((bit) & 1);
                    sprintf(sbit,"%1.1d",b);
#if 0
                    poke_s((j+16),i,sbit,WHITE<<8);//,cas3_color[state]);
//                  poke_s((j+19),i,"|",RED<<8);
                    poke_s((j+17),i,"|",RED<<8);
#endif // 0
                    poke_s((j+16),i,sbit,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));//,cas3_color[state]);
//                  poke_s((j+19),i,"|",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
                    poke_s((j+17),i,"|",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED));
            }
#endif
        }
}
#endif
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
#ifdef DISPLAY_CAS1_STATUS
#define NO_CAS1_DISPLAYED_PER_COLUMN    30      // 30 for textmode(C4350)
#define CAS1_FIRST_ROW_OFFSET           9       // 9  for textmode(C4350)
void disp_cas1_no(void){
        BYTE ktrnk;
        BYTE i,j;
        char trunk_no[5];

        for(ktrnk=0;ktrnk < NO_CAS1_DISPLAY;ktrnk++){
                i=ktrnk%NO_CAS1_DISPLAYED_PER_COLUMN + CAS1_FIRST_ROW_OFFSET;  // row number
                j=(ktrnk/NO_CAS1_DISPLAYED_PER_COLUMN)*26 + 1;     // col number. 80/3 =26, 3 columns in one screen
                sprintf(trunk_no,"%1.2d :",ktrnk+1);    // 4 characters
                poke_s(j,i,trunk_no,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        }
}

void disp_cas1(void){
        BYTE ktrnk;
        BYTE i,j;
        BYTE state;

#ifdef DISPLAY_CAS1_LINE_STATUS
        char sbit[5];
        BYTE bit,b;
#endif

        for(i=CAS1_FIRST_ROW_OFFSET,j=5,ktrnk=0;ktrnk < NO_CAS1_DISPLAY;ktrnk++,i++){
                if(i==(CAS1_FIRST_ROW_OFFSET+NO_CAS1_DISPLAYED_PER_COLUMN)){
                        i=CAS1_FIRST_ROW_OFFSET;
                        j+=26;
                }

                if(d1schng[ktrnk]){
                        d1schng[ktrnk]=RESET;
                        if((state=d1s[ktrnk])==OUT_OF_SERVICE){
                                poke_s(j,i,"Out of Service   ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE));        // 17 characters
                        }
                        else if(state==OGT){
                                poke_s(j-2,i,"O",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW));
                        }
                        else if(state==ICT){
                                poke_s(j-2,i,"I",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW));
                        }
                        else if(state==IDLE){   // IDLE in omtfp.h equals IDLE in dsmtfp.h
                                poke_s(j,i,"Idle             ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                        }
                        else if(state==HOLD){   // HOLD in omtfp.h equals HOLD in dsmtfp.h
                                poke_s(j,i,"Hold             ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                        }
                        else if(state==IN_EXTRNL_QU){   // IN_EXTRNL_QU in omtfp.h equals IN_EXTRNL_QU in dsmtfp.h
                                poke_s(j,i,"In External Que  ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                        }
                        else{
                                state -= EM_STATES_START;
#ifdef LIM
                                if( state <(NO_EM_STATES+NO_CAS3_STATES) )
#else
                                if( state <NO_EM_STATES )
#endif
                                        poke_s(j,i,emstate_name[state],color[state]);
                                else
                                        poke_s(j,i,"                 ",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
                        }
                }

#ifdef DISPLAY_CAS1_LINE_STATUS
                if(!d1sbchng[ktrnk]) continue;
                d1sbchng[ktrnk]=RESET;
                state=d1s[ktrnk];
                if((bit=d1sb[ktrnk])==UNKNOWN)
                        poke_s((j+17),i,"????",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE));
                else{
                        b=((bit >> 3) & 1);
                        sprintf(sbit,"%1.1d",b);
                        poke_s((j+17),i,sbit,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));//,color[state]);
                        b=(bit >> 2) & 1;
                        sprintf(sbit,"%1.1d",b);
                        poke_s((j+18),i,sbit,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));//,color[state]);
                        b=(bit >> 1) & 1;
                        sprintf(sbit,"%1.1d",b);
                        poke_s((j+19),i,sbit,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));//,color[state]);
                        b=((bit) & 1);
                        sprintf(sbit,"%1.1d",b);
                        poke_s((j+20),i,sbit,MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));//,color[state]);
                }
#endif
        }
}
#endif
#endif
#endif


#ifdef OPERATOR_PC
#ifdef SYSTEM_MANUAL_TEST
#define FIRST_ROW_OFFSET                2       // 9  for textmode(C4350)
void test_menu(){
        BYTE i,j;

        i=FIRST_ROW_OFFSET;  // row number
        j=30;     // col number. 80/3 =26, 3 columns in one screen

#if 0
        poke_s(j,i,"SYSTEM MANUAL TEST",WHITE<<8);
        poke_s(j-25,i+4,"Enter Subscriber No.:",WHITE<<8);
        poke_s(j+15,i+4,"The Subscriber Test Has:",WHITE<<8);
#endif // 0
        poke_s(j,i,"SYSTEM MANUAL TEST",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        poke_s(j-25,i+4,"Enter Subscriber No.:",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        poke_s(j+15,i+4,"The Subscriber Test Has:",MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
        cursor(j-4,i+4);
        ocr.scdc=0;
}
#endif
#endif


#ifdef OPERATOR_PC
void led_set(BYTE addr){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        CHAT_TYPE chat;
        BYTE i,j;
        struct LED_POS *tabpt;
        if( ! ( f & DSF ) ){
                if(addr>=MAX_NO_LED) return;
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
                                cursor(0,25);
#endif
#endif
                remove_mouse_ptr();
#ifdef NEW_DEP
                                if ( (dep_flag == SET)
#ifdef DISPLAY_BILLING
                                         ||(bill_display_flag == SET)
#endif
#ifdef DISPLAY_ALARM
                                         ||(alarm_display_flag == SET)
#endif
                                         )
                                  tabpt=&dep_led_table[addr];
                                else
                                  tabpt=&led_table[addr];
#else
#ifdef DISPLAY_BILLING
                if (bill_display_flag == SET)
                        tabpt=&dep_led_table[addr];
                else
#endif
#ifdef DISPLAY_ALARM
                if (alarm_display_flag == SET)
                        tabpt=&dep_led_table[addr];
                else
#endif
                        tabpt=&led_table[addr];
#endif

                for(i=tabpt->y1;i<=tabpt->y2;i++)
                    for(j=tabpt->x1;j<=tabpt->x2;j++){
                       chat=dget(j,i);
                       displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),tabpt->attr/*0x4f*/);
#ifdef NEW_DEP
//MESR
                                           if ( (dep_flag == SET)
#ifdef DISPLAY_BILLING
                                                        ||(bill_display_flag == SET)
#endif
#ifdef DISPLAY_ALARM
                                                        ||(alarm_display_flag == SET)
#endif
                                                        )
                                                 displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),tabpt->attr/*0x4f*/);
                                           else
                                                 displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),tabpt->attr/*0x4f*/);
#else
#ifdef DISPLAY_BILLING
                                           if (bill_display_flag == SET)
                                                 displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),tabpt->attr/*0x4f*/);
                                           else
#endif
#ifdef DISPLAY_ALARM
                                                 if (alarm_display_flag == SET)
                                                   displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),tabpt->attr/*0x4f*/);
                                                 else
#endif
                                                   displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),tabpt->attr/*0x4f*/);
#endif
                    }
/*
                for(i=led_table[addr].y1;i<=led_table[addr].y2;i++)
                    for(j=led_table[addr].x1;j<=led_table[addr].x2;j++){
                       chat=dget(j,i);
                       displscrn(j,i,chat&255,0x4f);
                    }
*/
                display_mouse_ptr();
        }
#endif
}
#endif

#ifndef OPERATOR_PC
void led_set(unsigned int addr){
        if(addr > 0x80){  /* LED address in on the next LED port */
                lpnb |= (addr >> 8);
                XBYTE[LED_PORT_NEXT]=lpnb;
        }
        else{  /* LED address in on the LED port */
                lpb |=addr;
                XBYTE[LED_PORT]=lpb;
        }
}
#endif

#ifdef OPERATOR_PC
void led_reset(BYTE addr){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        CHAT_TYPE chat;
        BYTE i,j;
        struct LED_POS *tabpt;
        if( ! ( f & DSF ) ){
                if(addr>=MAX_NO_LED) return;
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
                                cursor(0,25);
#endif
#endif
#ifdef NEW_DEP
//MESR
        if ( (dep_flag == SET)
#ifdef DISPLAY_BILLING
           ||(bill_display_flag == SET)
#endif
#ifdef DISPLAY_ALARM
           ||(alarm_display_flag == SET)
#endif
        )
                tabpt=&dep_led_table[addr];
        else
                tabpt=&led_table[addr];
#else
                tabpt=&led_table[addr];
#endif
                remove_mouse_ptr();
                for(i=tabpt->y1;i<=tabpt->y2;i++)
                   for(j=tabpt->x1;j<=tabpt->x2;j++){
                      chat=dget(j,i);
#ifdef NEW_DEP
                        if ( dep_flag == SET)
                                displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),MAKE_ATTR(KT_NO_BLINKING,KT_GREEN,KT_HIGH_INTENSITY,KT_WHITE));
                        else
                                displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
#else
                                displscrn(j,i,FIND_CHAR_FROM_CHAT(chat),MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE));
#endif
                   }
               display_mouse_ptr();
        }
#endif
}
#endif

#ifndef OPERATOR_PC
void led_reset(unsigned int addr){
        if(addr > 0x80){  /* LED address in on the next LED port */
                lpnb &= ~(addr >> 8);
                XBYTE[LED_PORT_NEXT]=lpnb;
        }
        else{  /* LED address in on the LED port */
                lpb &= ~addr;
                XBYTE[LED_PORT]=lpb;
        }
}
#endif


#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void b2led(BYTE a){
        int l;
        BYTE b;
        for(b=0x80,l=5;b;b>>=1,l++)
                if(a & b)
                        led_set(l);
                else
                        led_reset(l);
}
#endif
#endif

#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
int fkp(){  /* find key pressed */

#ifdef OPERATOR_PC
        BYTE sts;
        unsigned int x,y;
        sts=get_mouse_status(&x,&y);
        if(sts&0x01){
                return key_table[y][x];
        }
        else return 0xff;
#else
        BYTE kb,s,a,b,c;
        XBYTE[SD_DIR]=0;
        P1=0;
        if( (kb=(XBYTE[SD_DATA] & 0x7f)) == 0x7f){
                P1=0xff;
                return NULL;
        }
        P1=0xff;
        for(s=0;s<8;s++){
                P1=sp[s];
                if( (XBYTE[SD_DATA] & 0x7f) == kb){
                        P1=0xff;
                        for(a=1;a<0x80;a<<=1){
                                b=~a & 0x7f;
                                if(kb == b){
                                        c=a;
                                        for(b=0;c;c >>=1)
                                                ++b;
                                        return(s*7+b);
                                }
                        }
                        return NULL;
                }
        }
        P1=0xff;
        return NULL;
#endif
}
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
BYTE key_fkp(){
        return key_code[tolower(get_c())];
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void displscrn(BYTE x,BYTE y,CHAT_TYPE ch,CHAT_TYPE  at)
{
        if(x<80 && y<25)
                  kt_print_char(x,y,ch,at);
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void poke_s(BYTE x,BYTE y, char *s,CHAT_TYPE attr)
{
#if 0
        unsigned int y1;
        y1=y*160;
        while(*s)
                vpoke(VSG,y1+(x++<<1),*s++|color);
#endif // 0
        while(*s)
                kt_print_char(x++,y,*s++,attr);
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
static CHAT_TYPE dget(BYTE x,BYTE y)
{
        return kt_read_char(x,y);
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

#pragma inline
/* ibmpc.c */
/* Low-level functions addressing BIOS and PC Hardware */

#if (OS == DOS)
static union REGS rg;
#endif

/* position the cursor */
void cursor(int x,int y)
{
#if (OS == DOS)
        rg.x.ax=0x0200;
        rg.x.bx=0;
        rg.x.dx=((y<<8)&0xff00) + x;
        int86(16,&rg,&rg);
#endif
#if(OS ==  LINUX_COMPATIBLE)
        ncurses_move(x,y);
#endif
}

/* return the cursor position */
void curr_cursor(int *x,int *y)
{
#if (OS == DOS)
        rg.x.ax=0x0300;
        rg.x.bx=0;
        int86(16,&rg,&rg);
        *x=rg.h.dl;
        *y=rg.h.dh;
#endif
#if(OS ==  LINUX_COMPATIBLE)
        ncurses_getyx(*x,*y);
#endif
}

/* set cursor type */
void set_cursor_type(int t)
{
#if (OS == DOS)
        rg.x.ax=0x0100;
        rg.x.bx=0;
        rg.x.cx=t;
        int86(16,&rg,&rg);
#endif
#if(OS ==  LINUX_COMPATIBLE)
                //      ncurses_curs_set(t);
#endif
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

char attrib=7;

/* clear the screen */
void clear_screen()
{
#if (OS == DOS)
        cursor(0,0);
        rg.h.al=' ';
        rg.h.ah=9;
        rg.x.bx=attrib;
        rg.x.cx=2000;
        int86(16,&rg,&rg);
#endif
#if(OS ==  LINUX_COMPATIBLE)
        ncurses_clear();
#endif
}
void clear_large_screen()
{
#if (OS == DOS)
        cursor(0,0);
        rg.h.al=' ';
        rg.h.ah=9;
        rg.x.bx=attrib;
        rg.x.cx=4000;
        int86(16,&rg,&rg);
#endif
#if(OS ==  LINUX_COMPATIBLE)
        ncurses_clear();
#endif
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

/* return the video mode */
int vmode()
{
#if (OS == DOS)
        rg.h.ah=15;
        int86(16,&rg,&rg);
        return rg.h.al;
#endif
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

/* test for scroll lock */
int scroll_lock()
{
#if (OS == DOS)
        rg.x.ax=0x0200;
        int86(0x16,&rg,&rg);
        return rg.h.al & 0x10;
#endif
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

/* get a keyboard character */
BYTE  get_c(void)
{
#if (OS == DOS)
        rg.h.ah=1;
        int86(0x16,&rg,&rg);
        if(!(rg.x.flags & 0x40)) {
             rg.h.ah=0;
             int86(0x16,&rg,&rg);
             if (rg.h.al != 0)
                     return rg.h.al;
             else
                     return (0x80 | rg.h.ah);

#if 0
             if( (rg.h.ah >= 0x3b) && (rg.h.ah <= 0x46) )
                     return rg.h.ah - 0x3b + 14;
                     /* processing F1-F12 keys */

#endif // 0
        }
        return 0;
#endif
#if (OS == LINUX_COMPATIBLE)
                return (ncurses_getch());
#endif
}
/*
BYTE  get_c(void)
{
        rg.h.ah=1;
        int86(0x16,&rg,&rg);
        if(!(rg.x.flags & 0x40)) {
             rg.h.ah=0;
             int86(0x16,&rg,&rg);
             return rg.h.al;
        }
        else return 0;
}
*/
#endif
#endif


#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
/* insert a character and attribute into video RAM */
void vpoke(unsigned vseg,unsigned adr,unsigned chr)
{
#if (COMPILER == GCC)
        kt_farpoke16(vseg,adr,chr);
#else
        if(vseg==(unsigned) 45056L)         /* monochrome mode */
                poke(vseg,adr,chr);
        else  {
                _DI=adr;        /* offset of video character */
                _ES=vseg;       /* video segment */
                asm cld;
                _BX=chr;        /* the attribute and character */
                _DX=986;        /* video status port */
                /* wait for video retrace to start */
                do
                        asm in al,dx;
                while(_AL & 1);
                /* wait for video retrace to stop */
                do
                        asm in al,dx;
                while(!(_AL & 1));
                _AL=_BL;
                asm stosb;      /* store character */
                /* wait for video retrace to start */
                do
                        asm in al,dx;
                while(_AL & 1);
                /* wait for video retrace to stop */
                do
                        asm in al,dx;
                while(!(_AL & 1));
                _AL=_BH;
                asm stosb;      /* store attribute */
        }
#endif // COMPILER
}
#endif
#endif

#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

/* read a character and attribute from video RAM */
int vpeek(unsigned vseg,unsigned adr)
{
#if (COMPILER == GCC)
        return(kt_farpeek16(vseg,adr));
#else
        if(vseg==(unsigned) 45056L)         /* monochrome mode */
                return peek(vseg,adr);
        asm     push ds;
        _DX=986;                /* video status port */
        _DS=vseg;               /* video segment address */
        _SI=adr;                /* video character offset */
        asm cld;
        /* wait for video retrace to start */
        do
                asm in al,dx;
        while(_AL & 1);
        /* wait for video retrace to stop */
        do
                asm in al,dx;
        while(!(_AL & 1));
        asm lodsb;      /* get the character */
        _BL=_AL;
        /* wait for video retrace to start */
        do
                asm in al,dx;
        while(_AL & 1);
        /* wait for video retrace to stop */
        do
                asm in al,dx;
        while(!(_AL & 1));
        asm lodsb;      /* get the attribute */
        _BH=_AL;
        _AX=_BX;
        asm pop ds;
        return _AX;

#endif
}
#endif
#endif

#ifdef OPERATOR_PC

void cntr_set(BYTE lsb,BYTE msb){
#if (OS == DOS)
  BYTE c;
  c=kt_inport8(0x21);
  c=c | (0x01);         /*  mask interrupt 0 timer*/
  kt_outport8(0x21,c);
  kt_outport8(0x43,0x36);  /* timer 0 select, mode 3 16bit */
  kt_outport8(0x40,lsb);
  kt_outport8(0x40,msb);
  c=kt_inport8(0x21);
  c=c & !(0x01);        /*  unmask interrupt 0 timer*/
  kt_outport8(0x21,c);
#endif
}
#endif

#ifdef OPERATOR_PC

void intrrset(unsigned int vector){
#if COMPILER == GCC
#if (OS == DOS)
        _go32_dpmi_set_protected_mode_interrupt_vector(0x1c,&oldisr);
#endif
#else
        setvect(vector,oldvect);  /* reset to old interrupt vector */
#endif
}
#endif

#ifdef OPERATOR_PC

void init_intr(){
#if (COMPILER == GCC)
#if (OS == DOS)
        _go32_dpmi_seginfo  newisr;
        LOCK_FUNCTION(rtc);
        LOCK_VARIABLE(f);
        LOCK_VARIABLE(pph);
        _go32_dpmi_lock_data(&time_tab[0],16*4);
        LOCK_VARIABLE(tptw);

        _go32_dpmi_get_protected_mode_interrupt_vector(0x1c,&oldisr);
        newisr.pm_offset = (int) rtc;
        newisr.pm_selector = _go32_my_cs();
        _go32_dpmi_chain_protected_mode_interrupt_vector(0x1c,&newisr);
#endif
#else
        oldvect=getvect(0x1c);  /* get old interrupt vector */
        setvect(0x1c,rtc);     /* initialize interrupt vector location*/
#endif
#if (OS == DOS)
        cntr_set(0x9c,0x2e);  /* 1.19318 Mhz/0x2e9c=99 hz */
#endif
}
#endif

#ifdef OPERATOR_PC

void quit(int i){
        DIS_SPKR;
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
        clear_screen();
#endif
#ifdef ETHERNET_INSTALLED
        {
            guint i;
            for(i=0;i<PK_MAX;i++){
                if(pktdrvr_st[i].iface_no < MAX_IFACE){
                    pk_stop(pktdrvr_st[i].iface_no,/*(tolower(key) == 'q') ?*/ RESET/*:SET*/);   // or 'r' ?
                }
            }
        }
#endif
#ifndef HDLC_RS232_ON_OPERATOR_SIDE
        remove_mouse_ptr();
#endif
#if (OS == DOS) || (OS == WINDOWS)
        cntr_set(0,0);
        correct_time();
        intrrset(0x1c);

#ifdef OPERATOR_PC
/*
        fprintf(fer,"CO status:\n");
        for(a=0;a < NO_CO;a++)
                fprintf(fer,"%d\t%x\n",a,ctst[a].s);
        fprintf(fer,"TIE status:\n");
        for(a=0;a < NO_TIE;a++)
                fprintf(fer,"%d\t%x\n",a,ttst[a].s);
*/
#endif
#endif

#if (OS == DOS)
        textmode(C80);
#endif
        fcloseall();
        exit(i);
}
#endif

#ifdef OPERATOR_PC

void pc_timer_init(void){
        flag++;
#if (OS == DOS)
        kt_outport8(0x43,0xb6);  /* timer 2 select, mode 3 16bit */
        kt_outport8(0x42,lsbs[flag&0x01]);
        kt_outport8(0x42,msbs[flag&0x01]);
#endif
}
#endif

#ifdef OPERATOR_PC

DATTIM *get_dt(DATTIM *dt){
        BYTE temp;
//      kt_outport8(0x70,13);
//      if(kt_inport8(0x71) & 0x80) {
        kt_outport8(0x70,10);
        if((kt_inport8(0x71) & 0x80)==0) {
                kt_outport8(0x70,9);
                temp=kt_inport8(0x71);
//              if(temp == 0xff) return dt;
                dt->yr=temp;
                if(dt->yr < 50) dt->yr |= 0x2000;
                else            dt->yr |= 0x1900;
                kt_outport8(0x70,8);
                dt->mo=kt_inport8(0x71);
                kt_outport8(0x70,7);
                dt->da=kt_inport8(0x71);
                kt_outport8(0x70,4);
                dt->hr=kt_inport8(0x71);
                kt_outport8(0x70,2);
                dt->mi=kt_inport8(0x71);
                kt_outport8(0x70,0);
                dt->se=kt_inport8(0x71);
                return dt;
        }
        else return dt;
/*
        rg.h.ah=4;
        int86(0x1A,&rg,&rg);
        if(rg.x.flags & 0x40) {
             return NULL;
        }
        else {
             dt->yr=rg.h.cl;
             if(dt->yr < 50) dt->yr |= 0x2000;
             else            dt->yr |= 0x1900;
             dt->mo=rg.h.dh;
             dt->da=rg.h.dl;
        }
        rg.h.ah=2;
        int86(0x1A,&rg,&rg);
        if(rg.x.flags & 0x40) {
             return NULL;
        }
        else {
             dt->hr=rg.h.ch;
             dt->mi=rg.h.cl;
             dt->se=rg.h.dh;
        }
        return dt;
*/
}

BYTE get_and_check_dt(DATTIM *dt){
/* ver1
        kt_outport8(0x70,10);
        if((kt_inport8(0x71) & 0x80)==0) {
                kt_outport8(0x70,9);
                dt->yr=kt_inport8(0x71);
                kt_outport8(0x70,8);
                dt->mo=kt_inport8(0x71);
                kt_outport8(0x70,7);
                dt->da=kt_inport8(0x71);
                kt_outport8(0x70,4);
                dt->hr=kt_inport8(0x71);
                kt_outport8(0x70,2);
                dt->mi=kt_inport8(0x71);
                kt_outport8(0x70,0);
                dt->se=kt_inport8(0x71);
                return OK;
        }
        return FAIL;
*/
/* ver2
        kt_outport8(0x70,(kt_inport8(0x70)&0xc0) | 10);
        if((kt_inport8(0x71) & 0x80)==0) {    // UIP bit is not set
                kt_outport8(0x70,(kt_inport8(0x70)&0xc0) | 9);
                dt->yr=kt_inport8(0x71);
                kt_outport8(0x70,(kt_inport8(0x70)&0xc0) | 8);
                dt->mo=kt_inport8(0x71);
                kt_outport8(0x70,(kt_inport8(0x70)&0xc0) | 7);
                dt->da=kt_inport8(0x71);
                kt_outport8(0x70,(kt_inport8(0x70)&0xc0) | 4);
                dt->hr=kt_inport8(0x71);
                kt_outport8(0x70,(kt_inport8(0x70)&0xc0) | 2);
                dt->mi=kt_inport8(0x71);
                kt_outport8(0x70,(kt_inport8(0x70)&0xc0) | 0);
                dt->se=kt_inport8(0x71);
                return OK;
        }
        return FAIL;
*/
/* ver3 */
#if (OS == DOS)
        __dpmi_regs rg;
        rg.h.ah=4;
        __dpmi_int(0x1a, &rg);
        dt->yr=rg.h.cl;
        dt->mo=rg.h.dh;
        dt->da=rg.h.dl;
        rg.h.ah=2;
        __dpmi_int(0x1a, &rg);
        dt->hr=rg.h.ch;
        dt->mi=rg.h.cl;
        dt->se=rg.h.dh;
        return OK;
#endif
}
#endif

void toal(){  /* turn off all LEDs */
#ifdef OPERATOR_PC
        led_reset(TONE_RNGR_LED);
        led_reset(HOLD1_LED);
        led_reset(HOLD2_LED);
        led_reset(HOLD3_LED);
        led_reset(HOLD4_LED);
        led_reset(EXTRNL_LED);
        led_reset(RECALL_LED);
        led_reset(INTERNAL_LED);
        led_reset(PROG_LED);
        led_reset(SOURCE_LED);
        led_reset(DEST_LED);
        led_reset(PARK_LED);
#else
        XBYTE[LED_PORT]=0;
        XBYTE[LED_PORT_NEXT]=0;
#endif
}

/* =======================================================================*/
/* =======================================================================*/
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void set_led_attr(BYTE led_no,unsigned long bkcolor,unsigned long forcolor,unsigned long blink){
/*
        bit 7    : Blinking 0:off 1:on
        bits 6-4 : Background color
        bit 3    : Character intensity 0:normal 1: high density
        bits 2-0 : Character color
*/

        led_table[led_no].attr=MAKE_ATTR(blink,bkcolor,KT_LOW_INTENSITY,forcolor);
#ifdef NEW_DEP
        dep_led_table[led_no].attr=MAKE_ATTR(blink,bkcolor,KT_LOW_INTENSITY,forcolor);
#endif

}
#endif
/* =======================================================================*/
/* =======================================================================*/
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void init_led_attr(void){

        BYTE addr;
#ifdef HOT_STANDBY
        BYTE n;
#endif
        for(addr=0;addr<MAX_NO_LED;addr++){

                set_led_attr(addr,KT_RED,KT_WHITE,KT_NO_BLINKING);
        }
#ifdef HOT_STANDBY

#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined MY_UDP_FOR_OAM))
        for (n=0;n<NO_LAPB_CH;n++)
#endif
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
        for (n=0;n<max_sys_oam_tcp_link;n++)
#endif
#ifdef MY_TCPIP_FOR_OAM
#ifdef HOT_STANDBY
        for (n=0;n<2;n++)
#else
        for (n=0;n<1;n++)
#endif
#endif
        {
                set_led_attr(sync_led[n],KT_YELLOW,KT_WHITE,KT_NO_BLINKING);
                set_led_attr(link_up_led[n],KT_YELLOW,KT_WHITE,KT_NO_BLINKING);
        }
        set_led_attr(SYNC_LED3,KT_BLUE,KT_WHITE,KT_NO_BLINKING);
        set_led_attr(LINK_UP_LED3,KT_BLUE,KT_WHITE,KT_NO_BLINKING);
#endif
}
#endif
/* =======================================================================*/
/* =======================================================================*/
#ifdef OPERATOR_PC
#define bcd_to_binary(bcd_dat) ((((bcd_dat)>>4)&0x0f)*10) + ((bcd_dat)&0x0f)
void correct_given_time(DATTIM *dt){
        struct kt_time t;
        struct kt_date d;
        d.da_year=bcd_to_binary(dt->yr);
        d.da_mon=bcd_to_binary(dt->mo);
        d.da_day=bcd_to_binary(dt->da);
        t.ti_hour=bcd_to_binary(dt->hr);
        t.ti_min=bcd_to_binary(dt->mi);
        t.ti_sec=bcd_to_binary(dt->se);
        t.ti_hund=50;   // s.th average!
        if(d.da_year < 50) d.da_year += 2000;
        else               d.da_year += 1900;
        kt_settime(&t);
        kt_setdate(&d);
}
void correct_time(void){
        DATTIM dt;
        if(get_and_check_dt(&dt) == FAIL)
                return;
        correct_given_time(&dt);
}
#endif
/* ======================================================================= */
/* ======================================================================= */
void kt_settime(struct kt_time *tm)
{

#if (OS == DOS)
    settime((struct time *)tm);
#endif
#if (OS == LINUX_COMPATIBLE)
        // to be completed.
#endif
}
/* ========================================================================= */
/* ========================================================================= */
void kt_setdate(struct kt_date *dt)
{

#if (OS == DOS)
    setdate((struct date *)dt);
#endif
#if (OS == LINUX_COMPATIBLE)
        // to be completed.
#endif
}


