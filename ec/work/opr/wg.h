                        /****  window general header  ****/

#define QUICK_DRAW_MAC_DRAW

#if (OS == DOS)
#include <dos.h>
#include <mem.h>
#include <bios.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

#define BCC     1
#define GCC     2
#include "compiler.h"

#if (COMPILER != GCC)
#include <alloc.h>
#endif
#include <stdlib.h>
#include <string.h>
#define         SET                     1
#define         RESET                   0

/*
#define STACK_WINDOW
*/

#if 0
#define RED     4
#define GREEN   2
#define BLUE    1
#define WHITE   (RED | GREEN | BLUE)
#define YELLOW  (RED | GREEN)
#define AQUA    (GREEN | BLUE)
#define MAGENTA (RED | BLUE)
#define BLACK   0
#define BRIGHT  8
#define DIM     0
#endif // 0

#define BORDER  0
#define TITLE   1
#define ACCENT  2
#define NORMAL  3
#define ALL     4

#define TRUE    1
#define FALSE   0
#define OPR_ERROR   -1
#define OK      1

#define SCRNHT  25
#define SCRNWIDTH       80
#define TABS    4

//#define clr(bg,fg,in)   ( (fg) | (bg << 4) | (in) )
#define vad(x,y)        ( (y) * 160 + (x) * 2)

//bit7:blink,bits-654:bg,bits-3210:fg
//#define TERM_ATTR       ((BLUE << 4) | (WHITE | BRIGHT))

#if (OS == DOS)
#define ATTR_HIGH_INTENSITY     0x08 // KT_HIGH_INTENSITY macro can not be used(it is defined for attr+char)
#define ATTR_NO_BLINKING        0x00 // KT_NO_BLINKING macro can not be used(it is defined for attr+char)
#define TEXTATTR_TERM_ATTR      (ATTR_NO_BLINKING|(KT_BLUE << 4) | ATTR_HIGH_INTENSITY  | KT_WHITE)
#endif

#define TERM_ATTR                       MAKE_ATTR(KT_NO_BLINKING,KT_BLUE,KT_HIGH_INTENSITY,KT_WHITE)

#if 0
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
#endif

/* border types */

#define SINGLE_LINE     0
#define DOUBLE_LINE     1
#define SINGLE_TOP_DOUBLE_SIDE  2
#define DOUBLE_TOP_SINGLE_SIDE  3
#define POP_DOWN_MENU           4
#define NO_BORDER               5

#define FIELD_CHAR      '_'

/* field types */

#define ALPHANUMERIC            'A'
#define NUMERIC_SPACE_FILLED    'N'
#define NUMERIC_ZERO_FILLED     'Z'
#define CURRENCY                'C'
#define DATE                    'D'
#define HEX                     'H'
#define PASSWORD                'P'
#define BOOLEAN                 'B'
#define DECIMAL                 'D'

#define reverse_video(a)        a->wcolor[3]=a->wcolor[2];
#define normal_video(a)         a->wcolor[3]=a->pn;


typedef struct field{  /* data entry field description */
        char *fmask;  /* field data entry mask */
        int fprot;  /* field protection */
        char *fbuf;  /* fielf buffer */
        int ftype;  /* field type */
        int frow;  /* field row */
        int fcol;  /* field column */
        void (*fhelp)();  /* field help function */
        char *fhwin;  /* field help window */
        int flx,fly;  /* help window location */
        int (*fvalid)();  /* field validation function */
        struct field *fnxt;  /* next field on template */
        struct field *fprv;  /* previous field on template */
        struct field *fup;  /* up field on template */
        struct field *fdn;  /* down field on template */
        int saved_column;
        char *saved_mask;
        char *saved_buff;
}FIELD;

typedef struct wnd{
        int wv;  /* true if window is visible */
        int hd;  /* true if window was hidden */
        char *sb;  /* points to window's save block */
        char *tl;  /* points to window's title */
        int nwx;  /* north west x coordinate */
        int nwy;  /* north west y coordinate */
        int ww;  /* window width */
        int wh;  /* window height */
        int wsp;  /* scroll pointer */
        int sp;  /* selection pointer */
        int cx;  /* cursor x location */
        int bt;  /* border type */
        CHAT_TYPE wcolor[4];  /* colors for window */
        int wucf;  /* window upper case foreground */
        CHAT_TYPE pn;  /* previous normal color */
        struct wnd *nx;  /* points to the next window */
        struct wnd *pv;  /* points to the previous window */
        FIELD *ff;  /* points to 1st data entry field */
        FIELD *lf;  /* points to last data entry field */
        FIELD *saved_field;
}WINDOW;

typedef struct w_menu{
        char *mname;
        char **mselcs;
        void (**func)(int,int);
}MENU;

void pc(int x,int y);void rcp(int *x,int *y);void cs(void);int fvm(void);
WINDOW *enw(int x,int y,int h,int w);void swb(WINDOW *a,int bt);
void swc(WINDOW *a,int area,CHAT_TYPE bg,CHAT_TYPE fg,CHAT_TYPE inten);
void swi(WINDOW *a,CHAT_TYPE inten);void swt(WINDOW *a,char *t);
void rw(WINDOW *a);void dw(WINDOW *a);void caw(void);void sbg(void);
void deletw(WINDOW *a);void hw(WINDOW *a); void cw(WINDOW *a);
void repos_wnd(WINDOW *a,int x,int y,int z);void wframe(WINDOW *a);
void dtitle(WINDOW *a);void wprintf(WINDOW *a,char *ln,...);
void wputchar(WINDOW *a,int c);void wcursor(WINDOW *a,int x,int y);
int get_selection(WINDOW *a,int s,char *keys);int get_char(void);
void scroll(WINDOW *a,int dir);

CHAT_TYPE *waddr(WINDOW *a,int x,int y);

void displ(WINDOW *a,int x,int y,CHAT_TYPE ch,CHAT_TYPE at);void wvi(void);

CHAT_TYPE dget(WINDOW *a,int x,int y);

void vswap(WINDOW *a,BYTE show);
void vsave(WINDOW *a);void vrstr(WINDOW *a);void acline(WINDOW *a,CHAT_TYPE set);
void add_list(WINDOW *a);void beg_list(WINDOW *a);
void remove_list(WINDOW *a);void insert_list(WINDOW *w1,WINDOW *w2);
int vw(WINDOW **b);void error_message(char *s);void accent(WINDOW *a);
void deaccent(WINDOW *a);void idew(WINDOW *a);
void add_field(WINDOW * a,FIELD *b);
FIELD *enf(WINDOW *a,int cl,int rw,char *msk,char *bf,int ty);
void ddf(WINDOW *a,char *bf,char *msk);void ddvf(WINDOW *a,FIELD *b);
void dafw(WINDOW *a);void caf(WINDOW *a);int spaces(char *c);
int old_read_field(WINDOW *a,FIELD *b);int endstroke(int c);
void wprompt(WINDOW *a,int x,int y,char *s);void insert_status(void);
void sct(int t);void clear_message(void);void menu_select(char *,MENU *);
int old_data_entry(WINDOW *);
WINDOW *open_menu(char *,MENU *,int);
int gethmenu(MENU *,WINDOW *,int);int getvmn(MENU *,WINDOW *,int *,int);
int haccent(MENU *,WINDOW *,int,int);
void dimension(char *[],int *,int *);
void light(MENU *,WINDOW *,int,int);
int read_field(WINDOW *a,FIELD *b);
int read_field_init(WINDOW *a,FIELD *b);
int read_field_hndlr(WINDOW *a,FIELD *b);
int endstroke(int c);
void wprompt(WINDOW *a,int x,int y,char *s);void insert_status(void);
void sct(int t);void ktbg(void);

int data_entry(WINDOW *a);
int data_entry_init(WINDOW *a);
int data_entry_hndlr(WINDOW *a);
void swucf(WINDOW *a,int fg);
void clear_message(void);
void non_error_message(char *);


#if (COMPILER == GCC)
int max(int value1, int value2);
int min(int value1, int value2);
void poke(unsigned int segment, unsigned int offset, short value);
short peek(unsigned int segment, unsigned int offset);
void movmem(void *src, void *dest, unsigned int length);
#endif
#ifdef DISPLAY_BILLING
void bill_bg(void);
#endif
#ifdef DISPLAY_ALARM
void alarm_bg(void);
#endif

void draw_window(WINDOW *a);
