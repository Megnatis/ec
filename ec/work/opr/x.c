
#include <stdio.h>
#include <process.h>
#include <pc.h>
#include <dos.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <dpmi.h>
#include <go32.h>
#include <sys/stat.h>
#include <sys/farptr.h>
static union REGS rg;
typedef unsigned char BYTE;
#define CR              0x0d
#define LF              0x0a
#define vad(x,y)        ( (y) * 160 + (x) * 2)

#define TERM_ATTR       0x1f
#define BACKSPACE       8

#define ENTER   13
#define UP      200
#define ESC     27
#define BS      203
#define FWD     205
#define DN      208
#define BELL    7
#define DEL     211
#define INS     210
#define F1      187
#define F2      188
#define F3      189
#define F4      190
#define F5      191
#define F6      192
#define F7      193
#define F8      194
#define F9      195
#define F10     196
#define HOME    199
#define END     207
#define PGUP    201
#define PGDN    209
#define SPACE   32

#define MAX_OPR_SYS_PCKT_SIZE   16
BYTE command_buf[120];
BYTE buf_ptr=0;
BYTE scrn_save[256][80];
BYTE scrn_save_line=0;
BYTE scrn_save_column=0;
BYTE command_buf_save[120];
BYTE buf_ptr_save=0;
BYTE scroll_pointer=24;
BYTE scroll_flag=0;
unsigned int VSG;
int vmode()
{
        rg.h.ah=15;
        int86(16,&rg,&rg);
        return rg.h.al;
}
void vpoke(unsigned vseg,unsigned adr,unsigned chr)
{
        _farpokew(_dos_ds,(vseg<<4)+adr,chr);
}

void displscrn(BYTE x,BYTE y,BYTE ch,BYTE at)
{
        int vch=(ch&255) | (at<<8);

        if(x<80 && y<25)
                vpoke(VSG,vad(x,y),vch);
}
void print_char(BYTE c){
        putch(c);
        if(c==LF){
                return;
        }
        if(c==CR){
                while(scrn_save_column<80)
                        scrn_save[scrn_save_line][scrn_save_column++]=' ';
                scrn_save_column=0;
                scrn_save_line++;
                return;
        }
        scrn_save[scrn_save_line][scrn_save_column++]=c;
        if(scrn_save_column==80){
                scrn_save_column=0;
                scrn_save_line++;
        }

}
BYTE  get_c(void)
{
        rg.h.ah=1;
        int86(0x16,&rg,&rg);
        if(!(rg.x.flags & 0x40)) {
             rg.h.ah=0;
             int86(0x16,&rg,&rg);
             if (rg.h.al != 0)
                     return rg.h.al;
             else
                     return (0x80 | rg.h.ah);

        }
        return 0;
}
void send(void){
        BYTE i,j,k;
        BYTE send_buf[MAX_OPR_SYS_PCKT_SIZE];
        send_buf[0]=0xff;
        for(j=0;j<buf_ptr;){
                for(i=1;(i<MAX_OPR_SYS_PCKT_SIZE) && (j<buf_ptr);i++){
                        send_buf[i]=command_buf[j++];
                }
/*
                printf("\n");
                for(k=0;k<i;k++)
                        printf("%c",send_buf[k]);
                printf("\n");
                for(k=0;k<i;k++)
                        printf("%x ",send_buf[k]);
                printf("\n");
*/
        }
}
void cursor(int x,int y)
{
        rg.x.ax=0x0200;
        rg.x.bx=0;
        rg.x.dx=((y<<8)&0xff00) + x;
        int86(16,&rg,&rg);
}
void restore_terminal(void){
        BYTE i,j,k;
        scroll_pointer=24;
        k=scrn_save_line-scroll_pointer;
        for(j=0;j<25;j++,k++){
                for(i=0;i<80;i++){
                        displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                }
        }
        cursor(scrn_save_column,24);
}
BYTE terminal(void){
        BYTE a,h,i,j,k;
        a=get_c();
        if(a == 0)
                return 1;
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
                return 1;
        }
        else if(a == PGDN){
                if(scroll_pointer >= (24+25)){
                        scroll_pointer-=25;
                        k=scrn_save_line-scroll_pointer;
                        for(j=0;j<25;j++,k++){
                                for(i=0;i<80;i++){
                                        displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                                }
                        }
                        cursor(0,24);
                }
                return 1;
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
                return 1;
        }
        else if(a == DN){
                if(scroll_pointer >= (24+1)){
                        scroll_pointer--;
                        k=scrn_save_line-scroll_pointer;
                        for(j=0;j<25;j++,k++){
                                for(i=0;i<80;i++){
                                        displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                                }
                        }
                        cursor(0,24);
                }
                return 1;
        }
        if(scroll_pointer != 24){
                scroll_pointer=24;
                k=scrn_save_line-scroll_pointer;
                for(j=0;j<25;j++,k++){
                        for(i=0;i<80;i++){
                                displscrn(i,j,scrn_save[k][i],TERM_ATTR);
                        }
                }
                cursor(scrn_save_column,24);
        }
        if(a == BACKSPACE){
                if(buf_ptr > 0){
                        buf_ptr--;
                        print_char(BACKSPACE);
                        print_char(' ');
                        print_char(BACKSPACE);
                }
        }
        else if((a == F5) || (a == ESC)){
                for(i=0;i<buf_ptr;i++)
                        print_char(BACKSPACE);
                for(i=0;i<buf_ptr;i++)
                        print_char(' ');
                for(i=0;i<buf_ptr;i++)
                        print_char(BACKSPACE);
                buf_ptr=0;
        }
        else if(a == F3){
                for(i=buf_ptr;i<buf_ptr_save;i++){
                        print_char(command_buf[i]=command_buf_save[i]);
                }
                buf_ptr=i;
        }
        else if(a == F1){
                if(buf_ptr<buf_ptr_save){
                        print_char(command_buf[buf_ptr]=command_buf_save[buf_ptr]);
                        buf_ptr++;
                }
        }
        else{
                print_char(a);
                if(a == CR)
                        print_char(LF);
                command_buf[buf_ptr++]=a;
        }
        if( a == '\\'){
                textattr(0x07);
                return 1;
        }
        if( a == '`'){
                return 0;
        }
        if (a == CR) {
                send();
                for(i=0;i<buf_ptr-1;i++){       // do not copy the ending CR
                        command_buf_save[i]=command_buf[i];
                }
                buf_ptr_save=buf_ptr-1;
                buf_ptr=0;
        }
        return 1;
}
void main(void){
        BYTE i,j;
        VSG=(vmode()==7 ? 0xb000 : 0xb800);
        for(i=0;i<100;i++)
                for(j=0;j<80;j++)
                        scrn_save[i][j]=' ';
        textattr(TERM_ATTR);
        restore_terminal();
        while(terminal());
}
