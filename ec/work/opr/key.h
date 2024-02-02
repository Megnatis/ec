

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

#define NO_KEY_PRESSED          0


#if (OS == DOS)
#define KT_BLACK   0
#define KT_BLUE    1
#define KT_GREEN   2
#define KT_CYAN    (KT_GREEN | KT_BLUE)
#define KT_RED     4
#define KT_MAGENTA (KT_RED | KT_BLUE)
#define KT_YELLOW  (KT_RED | KT_GREEN)
#define KT_WHITE   (KT_RED | KT_GREEN | KT_BLUE)

#define KT_BLINKING             0x8000
#define KT_NO_BLINKING          0x0000

#define KT_HIGH_INTENSITY       0x0800
#define KT_LOW_INTENSITY        0x0000

#define MAKE_ATTR(blink,bg,intensity,fg)        (blink|(bg<<12)|intensity|fg<<8)
#define CHANGE_ATTR_FG_COLOR(attr,fg)           attr &= 0xf800;attr |= (fg << 8);
#define FIND_CHAR_FROM_CHAT(chat)                       (chat & 0xff)
#define FIND_ATTR_FROM_CHAT(chat)                       (chat & (~0xff))
#endif


#if(OS ==  LINUX_COMPATIBLE)
#ifdef NCURSES_USED

// note : this block has been copied from /usr/include/ncurses.h
// macros are also renamed.
#define KT_BLACK   0    // COLOR_BLACK
#define KT_BLUE    4    // COLOR_BLUE
#define KT_GREEN   2    // COLOR_GREEN
#define KT_CYAN    6    // COLOR_CYAN
#define KT_RED     1    // COLOR_RED
#define KT_MAGENTA 5    // COLOR_MAGENTA
#define KT_YELLOW  3    // COLOR_YELLOW
#define KT_WHITE   7    // COLOR_WHITE

#define KT_NCURSES_ATTR_SHIFT           8   // NCURSES_ATTR_SHIFT
#define KT_NCURSES_BITS(mask,shift)     ((mask) << ((shift) + KT_NCURSES_ATTR_SHIFT)) // NCURSES_BITS

#define KT_A_ALTCHARSET KT_NCURSES_BITS(1UL,14)
#define KT_A_COLOR              KT_NCURSES_BITS(((1UL) << 8) - 1UL,0)
#define KT_COLOR_PAIR(n)        KT_NCURSES_BITS(n, 0)           // COLOR_PAIR
#define KT_PAIR_NUMBER(a)       (((a) & KT_A_COLOR) >> KT_NCURSES_ATTR_SHIFT) // PAIR_NUMBER

#define KT_BLINKING             KT_NCURSES_BITS(1UL,11) // A_BLINK
#define KT_NO_BLINKING          0

#define KT_HIGH_INTENSITY       KT_NCURSES_BITS(1UL,13) // A_BOLD
#define KT_LOW_INTENSITY        0

#define KT_CHARTEXT                     (KT_NCURSES_BITS(1UL,0) - 1UL)                  // A_CHARTEXT
#define KT_COLOR                        KT_NCURSES_BITS(((1UL) << 8) - 1UL,0)   // A_COLOR

//CHEEN start
//#define MAKE_ATTR(blink,bg,intensity,fg)        (blink|intensity|KT_COLOR_PAIR((bg<<3)|fg)) // our rule
#define MAKE_ATTR(blink,bg,intensity,fg)        (blink|intensity|KT_COLOR_PAIR(((bg<<3)|fg)+1)) // our rule
//CHEEN end
//CHEEN start
// our definition : pair_no = bg << 3 | fg
//#define CHANGE_ATTR_FG_COLOR(attr,fg)           attr &= 0xfffff800;attr |= (fg << 8);
#define CHANGE_ATTR_FG_COLOR(attr,fg)           {unsigned int pair_no;\
                                                 pair_no=KT_PAIR_NUMBER(attr)-1;\
                                                                                                 pair_no=((pair_no&0xf8)|fg)+1;\
                                                                                                 attr &= 0xffff0000;\
                                                                                                 attr |= (pair_no<<8);}

//CHEEN end
#define FIND_CHAR_FROM_CHAT(chat)                               (chat & 0xff)
#define FIND_ATTR_FROM_CHAT(chat)                               (chat & (~0xff))
/*
attr&=~KT_COLOR;\
attr|=KT_COLOR_PAIR(((KT_PAIR_NUMBER(attr)>>3)&0x7)<<3|fg);
*/
#endif
#endif


