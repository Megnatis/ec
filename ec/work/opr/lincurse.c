


#include "compiler.h"

#define DOS                     1
#define ECOS                    2
#define LINUX_COMPATIBLE        3

#define KURT                    1
#define RTEMS                   2

//CHEEN : oomad inja
#include "/ec/work/spec/general.h"

#define DEBUG_ACT

#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
#include <ncurses.h>
#include <stdarg.h>

#include "key.h"
#endif
#endif


/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_printw(const char *fmt,...)
{

        int cnt;
        va_list argp;
        va_start(argp,fmt);
        cnt=vwprintw(stdscr,fmt,argp);
        va_end(argp);
        refresh();
        return cnt;
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_scrollok(unsigned char dat)
{
        return(scrollok(stdscr,dat));
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
void kt_ncurses_init(void)
{
    unsigned char fg_color,bg_color,pair_no;

        initscr();
        //      raw();
        cbreak();
        noecho();
        nodelay(stdscr,TRUE);
        keypad(stdscr,TRUE);
        //      scrollok(stdscr,TRUE);
        nonl();
#ifdef DEBUG_ACT
        //      ncurses_printw("kt_ncurses_init\n");
#endif
        start_color();
        for(fg_color=0;fg_color<8;fg_color++){
                for(bg_color=0;bg_color<8;bg_color++){
//CHEEN start
                        pair_no=((bg_color<<3) | fg_color)+1; // zero can not be used
//                      pair_no=(bg_color<<3) | fg_color);
//CHEEN end
                        init_pair(pair_no,fg_color,bg_color);
                }
        }
//APOLO start
#ifndef SLACKWARE_NCURSES_BUG
        attron(A_ALTCHARSET);
#endif
//APOLO end
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_getch(void)
{

        int ch;

        ch=getch();
        switch(ch){
        case KEY_BACKSPACE:
                ch=BACKSPACE;
                break;
        case 13 :
                ch=ENTER;
                break;
        case KEY_UP:
                ch=UP;
                break;
        case 27:
                ch=ESC;
                break;
        case KEY_LEFT:
                ch=BS;
                break;
        case KEY_RIGHT:
                ch=FWD;
                break;
        case KEY_DOWN:
                ch=DN;
                break;
        case 7:
                ch=BELL;
                break;
        case KEY_DC:
                ch=DEL;
                break;
        case KEY_IC:
                ch=INS;
                break;
        case KEY_F(1):
                ch=F1;
                break;
        case KEY_F(2):
                ch=F2;
                break;
        case KEY_F(3):
                ch=F3;
                break;
        case KEY_F(4):
                ch=F4;
                break;
        case KEY_F(5):
                ch=F5;
                break;
        case KEY_F(6):
                ch=F6;
                break;
        case KEY_F(7):
                ch=F7;
                break;
        case KEY_F(8):
                ch=F8;
                break;
        case KEY_F(9):
                ch=F9;
                break;
        case KEY_F(10):
                ch=F10;
                break;
        case KEY_HOME:
                ch=HOME;
                break;
        case KEY_END:
                ch=END;
                break;
        case KEY_PPAGE:
                ch=PGUP;
                break;
        case KEY_NPAGE:
                ch=PGDN;
                break;
        case 32:
                ch=SPACE;
                break;
        case ERR:
                ch=NO_KEY_PRESSED;
                break;
        default:
                break;
        }
    return(ch);
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_mvaddch(unsigned int col,unsigned int row,unsigned long ch)
{
        int val;
        val=mvaddch(row,col,ch);
        //      move(row,col);
        //      val=echochar(ch);
        refresh();
        return val;
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED

int ncurses_addch(unsigned long ch)
{
        int val;
        val=addch(ch);
        refresh();
        return val;
}

#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
unsigned long ncurses_mvinch(unsigned int col,unsigned int row)
{
        return(mvinch(row,col));
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_move(int col,int row)
{
        int res;

        if((col>=80) || (row>=25)){
                //        setsyx(-1,-1); // is a macro
                //        doupdate();
                //        refresh();
                //        leaveok(1);
                curs_set(0);
                return 0;
        }
        else{
                res=move(row,col);
                curs_set(1);
                refresh();
        }
        return(res);
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
void ncurses_getyx(int col,int row)
{
        getyx(stdscr,row,col);
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_curs_set(int visibility)
{
        return(curs_set(visibility));
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
void ncurses_clear(void)
{
        clear();
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_def_prog_mode(void)
{
        return(def_prog_mode());
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_endwin(void)
{
        return(endwin());
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_refresh(void)
{
        return(refresh());
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
int ncurses_attrset(int attrs)
{
        return(attrset(attrs));
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (OS == LINUX_COMPATIBLE)
#ifdef NCURSES_USED
void  ncurses_bkgdset(int attrs)
{
//CHEEN
        bkgdset(attrs);
}
#endif
#endif





