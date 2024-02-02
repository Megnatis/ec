
#include "omtfp.h"
#include <string.h>
#if COMPILER == GCC
//DOOD start: #ifdef added
#if (OS == DOS)
#include <sys/farptr.h>
#endif
extern int VSG;
#if (OS == LINUX_COMPATIBLE)
#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
unsigned char *isa_mem;
int tty_fd,mem_fd;
#endif
#ifdef TCP_DEBUG_ACT
extern FILE *tcp_fer;
#endif

#endif
//DOOD end
#endif
/* ======================================================================== */
/* ======================================================================== */

#if (COMPILER == GCC)
int max(int value1, int value2){
   return ( (value1 > value2) ? value1 : value2);
}
#endif
/* ======================================================================== */
/* ======================================================================== */

#if (COMPILER == GCC)
int min(int value1, int value2){
   return ( (value1 > value2) ? value2 : value1);
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (COMPILER == GCC)
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void poke(unsigned int segment, unsigned int offset, short value){
// DOOD start:
//      _farpokew(_dos_ds,(segment<<4)+offset,value);
        kt_farpoke16(segment,offset,value);
// DOOD end
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (COMPILER == GCC)
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
short peek(unsigned int segment, unsigned int offset){
// DOOD start:
//      return(_farpeekw(_dos_ds,(segment<<4)+offset));
        return(kt_farpeek16(segment,offset));
// DOOD end
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (COMPILER == GCC)
void movmem(void *src, void *dest, unsigned int length){
        memmove(dest,src,length);
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (COMPILER == GCC)
char *ltoa(long value, char *string, int radix){
//DOOD : itoa -> kt_itoa
        return(kt_itoa((int)value,string,radix));
}
#endif
/* ======================================================================== */
/* ======================================================================== */
int my_getw(FILE *stream){

#if (COMPILER == GCC)
        unsigned short dat;
#endif

#if (COMPILER == GCC)
        fread(&dat,2,1,stream);
        return dat;
#else
        return(getw(stream));
#endif
}
/* ======================================================================== */
/* ======================================================================== */
int my_putw(int w, FILE *stream){

#if (COMPILER == GCC)
        unsigned int dat;
#endif

#if (COMPILER == GCC)
        dat=w;
        return(fwrite(&dat,2,1,stream));
#else
        return(putw(w,stream));
#endif
}
/* ======================================================================== */
/* ======================================================================== */
//DOOD start : block added
char *kt_itoa(int value,char *string,int radix){
#if (OS == DOS)
    return itoa(value,string,radix);
#endif
#if (OS == WINDOWS)
    return _itoa(value,string,radix);
#endif
#if (OS == LINUX_COMPATIBLE)
{
        char tmp[33];
        char *tp = tmp;
        int i;
        unsigned v;
        int sign;
        char *sp;

        if (radix > 36 || radix <= 1){
                errno = EDOM;
                return 0;
        }

        sign = (radix == 10 && value < 0);
        if (sign)
                v = -value;
        else
                v = (unsigned)value;
        while (v || tp == tmp){
                i = v % radix;
                v = v / radix;
                if (i < 10)
                        *tp++ = i+'0';
                else
                        *tp++ = i + 'a' - 10;
        }

        if (string == 0)
                string = (char *)malloc((tp-tmp)+sign+1);
        sp = string;

        if (sign)
                *sp++ = '-';
        while (tp > tmp)
                *sp++ = *--tp;
        *sp = 0;
        return string;

}
#endif
}
/* =========================================================================*/
/* =========================================================================*/
#if (OS == LINUX_COMPATIBLE)
#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
void open_user_space_isa_mem(void)
{

//  int vc=-1;
//  char fname[30];
    int page_size/*,fd*/;

        /* ensure that the open will get a file descriptor greater than 2,      */
        /* else problems can occur with stdio functions under certain strange   */
        /* conditions .                                                                                                                 */

        if(fcntl(0,F_GETFD) < 0) open("/dev/null" ,O_RDONLY);
        if(fcntl(1,F_GETFD) < 0) open("/dev/null" ,O_WRONLY);
        if(fcntl(2,F_GETFD) < 0) open("/dev/null" ,O_WRONLY);
#if 0
        if((fd=open("/dev/console",O_RDWR)) < 0){
                ws2p("can't open /dev/console\n");
                exit(1);
        }
        if(ioctl(tty_fd,VT_OPENQRY,&vc) < 0){
                ws2p("error1 ioctl(VT_OPENQRY)\n");
                exit(1);
        }
    if(vc <= 0){
                ws2p("error2 ioctl(VT_OPENQRY)\n");
                exit(1);
        }
        sprintf(fname,"/dev/tty%d",vc);
        close(fd);
        /* change our control terminal */
        if((tty_fd = open(fname,O_RDWR)) >= 0){
          //            fflush(stdin);
                        fflush(stdout);
                        fflush(stderr);
          //            close(0);
                        close(1);
                        close(2);
          //            dup(tty_fd);
          //            dup(tty_fd);
          //            dup(tty_fd);
                ioctl(tty_fd,VT_ACTIVATE,vc);
        }
        else{
                ws2p("tty open error\n");
                exit(1);
        }
#endif // 0
        /* open /dev/mem */
    if ((mem_fd = open("/dev/mem", O_RDWR) ) < 0) {
        ws2p("VGAlib: can't open /dev/mem \n");
                exit(1);
    }

        page_size=getpagesize();
    /* mmap graphics memory */
    if ((isa_mem = (unsigned char *)malloc(ISA_MEMORY_SIZE + (page_size-1))) == NULL) {
        ws2p("VGAlib: allocation error \n");
                exit(1);
    }
    if ((unsigned long)isa_mem % page_size)
        isa_mem += page_size - ((unsigned long)isa_mem % page_size);

    isa_mem = (unsigned char *)mmap(
        (caddr_t)isa_mem,
        ISA_MEMORY_SIZE,
        PROT_READ|PROT_WRITE,
        MAP_SHARED|MAP_FIXED,
        mem_fd,
        ISA_MEMORY_BASE
    );
    if ((long)isa_mem < 0) {
        ws2p("VGAlib: mmap error \n");
        exit(1);
    }
}
#endif
#endif
/* =========================================================================*/
/* =========================================================================*/
#if (OS == LINUX_COMPATIBLE)
#ifdef OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
void close_user_space_isa_mem(void)
{
        munmap(isa_mem,ISA_MEMORY_SIZE);
#if 0
        open("/dev/stdin"  ,O_RDONLY);
        open("/dev/stdout" ,O_WRONLY);
        open("/dev/stderr" ,O_WRONLY);

        close(tty_fd);
#endif // 0
}
#endif
#endif
//DOOD end
/* ======================================================================== */
/* ======================================================================== */
int print_error_report(const char *fmt, ...)
{
//DOODDOOD : to be asked

        int cnt;
        va_list argp;
        va_start(argp,fmt);
//CHEEN start
#ifdef TCP_DEBUG_ACT
        cnt=vfprintf(tcp_fer,fmt,argp);
#else
        cnt=vfprintf(stdout,fmt,argp);
#endif
//CHEEN end
        va_end(argp);
//CHEEN start
#ifdef TCP_DEBUG_ACT
        fflush(tcp_fer);
#endif
//CHEEN end
        return cnt;
}
/* ======================================================================== */
/* ======================================================================== */
#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
void kt_print_char(unsigned int col, unsigned int row, CHAT_TYPE ch,CHAT_TYPE attr)
{

#if (OS == DOS)
        unsigned int segment,offset;

        segment=VSG;
        offset=vad(col,row);
//      poke(segment,offset,(short)((attr<<8)&0xff00)|ch);
        poke(segment,offset,(short)(FIND_ATTR_FROM_CHAT(attr)| FIND_CHAR_FROM_CHAT(ch)));
#endif
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
        //    ncurses_mvaddch(y,x,value);
        {

//APOLO start
#ifdef SLACKWARE_NCURSES_BUG
          ch=FIND_CHAR_FROM_CHAT(ch);
          ch=((ch > 32) && (ch < 127)) ? ch : ch | KT_A_ALTCHARSET;
          ncurses_mvaddch(col,row,FIND_ATTR_FROM_CHAT(attr) | ch);
#else
          ncurses_mvaddch(col,row,FIND_ATTR_FROM_CHAT(attr) | FIND_CHAR_FROM_CHAT(ch));
#endif
//APOLO end
        }
#endif
#endif
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef OPERATOR_PC
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
CHAT_TYPE kt_read_char(unsigned int col,unsigned int row)
{
#if (OS == DOS)
        unsigned int segment,offset;

        segment=VSG;
        offset=vad(col,row);
        return((CHAT_TYPE)peek(segment,offset));
#endif
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
        {
            //  CHAT_TYPE value=ncurses_mvinch(col,row) & (~0x00400000);
            CHAT_TYPE value=ncurses_mvinch(col,row) & (KT_BLINKING | KT_HIGH_INTENSITY | KT_CHARTEXT | KT_COLOR);
            return(value);
        }
#endif
#endif
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
int kt_system(const char *cmd)
{

#if (OS == DOS) || (OS == WINDOWS)
        return(system(cmd));
#endif

#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
        int res;
        ncurses_def_prog_mode();
        ncurses_endwin();
#endif
        res=system(cmd);
#ifdef NCURSES_USED
        ncurses_refresh();
        return res;
#endif
#endif
}





