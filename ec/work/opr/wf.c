			/****  window functions  ****/
#include "omtfp.h"

#if (OS == DOS) || (OS == LINUX_COMPATIBLE)

#include "wg.h"
#include "wv.h"

#if (COMPILER == GCC)
#if (OS == DOS)
#include <sys/farptr.h>
#endif
#endif
extern WINDOW *saved_win;
unsigned char get_c(void);
void quit(int i);

#ifdef DISPLAY_ALARM
extern BYTE alarm_display_flag;
extern BYTE alarm_line,alarm_scroll_flag;
extern WINDOW *alarm_w,*sync_alarm_w;
#endif


/* ###################################################################### */

void pc(int x,int y){  /* position cursor */
#if (OS == DOS)
	r.x.ax=0x0200;
	r.x.bx=0;
	r.x.dx=( (y << 8) & 0xff00) | x;
	int86(0x10,&r,&r);
#endif
#if(OS ==  LINUX_COMPATIBLE)
	ncurses_move(x,y);
#endif
}

/* ###################################################################### */

void rcp(int *x,int *y){  /* return cursor position */
#if (OS == DOS)
	r.x.ax=0x0300;
	r.x.bx=0;
	int86(0x10,&r,&r);
	*x=r.h.dl;
	*y=r.h.dh;
#endif
#if(OS ==  LINUX_COMPATIBLE)
	ncurses_getyx(*x,*y);
#endif
}

/* ###################################################################### */

void sct(int t){  /* set cursor type */
#if (OS == DOS)
	r.x.ax=0x0100;
	r.x.bx=0;
	r.x.cx=t;
	int86(0x10,&r,&r);
#endif
#if(OS ==  LINUX_COMPATIBLE)
	ncurses_curs_set(t);
#endif
}

/* ###################################################################### */

void cs(){  /* clear screen */
#if (OS == DOS)
	pc(0,0);
	r.h.al= ' ';
	r.h.ah=9;
	r.x.bx=ca;
	r.x.cx=2000;
	int86(0x10,&r,&r);
#endif
#if(OS ==  LINUX_COMPATIBLE)
	ncurses_clear();
#endif
}

/* ###################################################################### */

int fvm(){  /* find video mode */
#if (OS == DOS)
	r.h.ah=15;
	int86(0x10,&r,&r);
	return r.h.al;
#endif
#if(OS ==  LINUX_COMPATIBLE)
	return 0; // 7 is monochrome
#endif
}

/* ###################################################################### */

WINDOW *enw(int x,int y,int h,int w){  /* establish new window */
	WINDOW *a;
	if( (a= (WINDOW *) malloc(sizeof(WINDOW)) ) == NULL)
		return NULL;
	a->tl="";
	a->wh=min(h,SCRNHT);
	a->ww=min(w,SCRNWIDTH);
	a->nwx=max(0,min(x,SCRNWIDTH - a->ww) );
	a->nwy=max(0,min(y,SCRNHT - a->wh) );
	a->cx=0;
	a->wsp=0;
	a->sp=1;
	a->bt=0;
	a->wv=a->hd=0;
	a->nx=a->pv=NULL;
	a->ff=a->lf=NULL;
	a->wcolor[BORDER] = a->wcolor[NORMAL] = a->wcolor[TITLE]=
//	clr(BLACK,WHITE,BRIGHT);
	MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_HIGH_INTENSITY,KT_WHITE);
#if 0
	a->pn=clr(BLACK,WHITE,BRIGHT);
	a->wcolor[ACCENT]=clr(WHITE,BLACK,DIM);
#endif // 0
	a->pn=MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_HIGH_INTENSITY,KT_WHITE);
	a->wcolor[ACCENT]=MAKE_ATTR(KT_NO_BLINKING,KT_WHITE,KT_LOW_INTENSITY,KT_BLACK);
	a->wucf=KT_BLACK;
	if( (a->sb = malloc(a->ww * a->wh * CHAT_SIZE) ) == (char *) 0 ){
		free(a);
		return NULL;
	}
	add_list(a);
#ifndef STACK_WINDOWS
#ifndef QUICK_DRAW_MAC_DRAW
	cw(a);
	wframe(a);
#endif
#endif
	return a;
}

/* ###################################################################### */

void swucf(WINDOW *a,int fg){  /* set window's upper case foreground */
	if(vw(&a))
		a->wucf=fg;
}

/* ###################################################################### */

void swb(WINDOW *a,int bt){  /* set window's border */
	if(vw(&a)){
		a->bt=bt;
		rw(a);
	}
}

/* ###################################################################### */

void swc(WINDOW *a,int area,CHAT_TYPE bg,CHAT_TYPE fg,CHAT_TYPE inten){
/* set window's color */
	if(fvm() == 7){
		if( (bg != KT_WHITE) && (bg != KT_BLACK) )
			return;
		if( (fg != KT_WHITE) && (fg != KT_BLACK) )
			return;
	}
	if(vw(&a)){
		if(area == ALL)
		    while(area)
//			  a->wcolor[--area] = clr(bg,fg,inten);
			  a->wcolor[--area] = MAKE_ATTR(KT_NO_BLINKING,bg,inten,fg);
		else
//		    a->wcolor[area] = clr(bg,fg,inten);
		    a->wcolor[area] = MAKE_ATTR(KT_NO_BLINKING,bg,inten,fg);
		rw(a);
	}
}

/* ###################################################################### */

void swi(WINDOW *a,CHAT_TYPE inten){	/* set window's intensity */
	int b;
	if(vw(&a)){
		for(b=0;b < ALL;b++){
//			a->wcolor[b] &= ~BRIGHT;
			a->wcolor[b] &= ~KT_HIGH_INTENSITY;
			a->wcolor[b] |= inten;
		}
		rw(a);
	}
}

/* ###################################################################### */

void swt(WINDOW *a,char *t){  /* set window's title */
	if(vw(&a)){
		a->tl=t;
		rw(a);
	}
}

/* ###################################################################### */

void rw(WINDOW *a){  /* redraw window */
#ifdef QUICK_DRAW_MAC_DRAW
#else
#ifndef STACK_WINDOW
	int x,y;
	CHAT_TYPE c,atr;
	for(y=1;y < (a->wh -1);y++)
		for(x=1;x < (a->ww - 1);x++){
			c=dget(a,x,y);
//			if( ( (c >> 8) & 0xff) == a->pn){
			if( FIND_ATTR_FROM_CHAT(c)  == a->pn){
				atr= a->wcolor[NORMAL];
			}
			else{
				atr= a->wcolor[ACCENT];
			}
//			displ(a,x,y,c & 0xff,atr);
			displ(a,x,y,FIND_CHAR_FROM_CHAT(c),atr);
		}
	wframe(a);
#endif
	a->pn=a->wcolor[NORMAL];
#endif
}

/* ###################################################################### */

void dw(WINDOW *a){  /* display window */
	if(vw(&a) && !a->wv){
		a->wv=1;
#ifdef STACK_WINDOW
		if(a->hd){
			a->hd=0;
			vrstr(a);
		}
		else{
			vsave(a);
			cw(a);
			wframe(a);
		}
#else
		vswap(a,FALSE);
#endif
	}
}

/* ###################################################################### */

void caw(){  /* close all windows */
	WINDOW *a,*b;
	for(b=wllt;b;){
		a=b->pv;
		deletw(b);
		b=a;
	}
}

/* ###################################################################### */

void deletw(WINDOW *a){  /* delete window */
	FIELD *b;
	for(b=a->ff;b != NULL;b=b->fnxt)
		free(b);
	if(vw(&a)){
		a->wucf=KT_BLACK;
		hw(a);
		free(a->sb);
		remove_list(a);
		free(a);
	}
}

/* ###################################################################### */

void hw(WINDOW *a){  /* hide window */
	if(vw(&a)){
#ifndef STACK_WINDOW
		vswap(a,TRUE);
#else
		vrstr(a);
#endif
		a->hd=1;
		a->wv=0;
	}
}
/* ###################################################################### */

#ifndef STACK_WINDOW
void repos_wnd(WINDOW *a,int x,int y,int z){
/* reposition the window in its 3-axis plane */
	WINDOW *b;
	int x1,y1;
	CHAT_TYPE chat;
	if( !vw(&a) )
		return;
	b=enw(x + a->nwx,y + a->nwy,a->wh,a->ww);
	b->tl=a->tl;
	b->bt=a->bt;
	b->wcolor[BORDER]=a->wcolor[BORDER];
	b->wcolor[TITLE]=a->wcolor[TITLE];
	b->wcolor[ACCENT]=a->wcolor[ACCENT];
	b->wcolor[NORMAL]=a->wcolor[NORMAL];
	b->wsp=a->wsp;
	b->cx=a->cx;
	if(z != 1){
		remove_list(b);
		if(z == 0)
			insert_list(b,a);
		else
			beg_list(b);
	}
	for(y1=0;y1 < b->wh;y1++)
		for(x1=0;x1 < b->ww;x1++){
			chat=dget(a,x1,y1);
//			displ_w(b,x1,y1,chat & 0xff,(chat >> 8) & 0xff);
//			displ(b,x1,y1,chat & 0xff,(chat >> 8) & 0xff);
			displ(b,x1,y1,FIND_CHAR_FROM_CHAT(chat),FIND_ATTR_FROM_CHAT(chat));

		}
	b->wv=1;
	vswap(b,TRUE);
	hw(a);
	free(a->sb);
	remove_list(a);
	*a=*b;
	insert_list(a,b);
	remove_list(b);
	free(b);
}
#endif

/* ###################################################################### */

void cw(WINDOW *a){  /* clear window */
	int x1,y1;
	if(vw(&a)){
		for(y1=1;y1 < (a->wh - 1);y1++){
			for(x1=1;x1 < (a->ww - 1);x1++){
				displ(a,x1,y1,' ',a->wcolor[NORMAL]);
			}
		}
	}
}

/* ###################################################################### */

void wframe(WINDOW *a){  /* window frame display */
	int x1,y1;
	if(!vw(&a))
		return;
	/* window's title */
	displ(a,0,0,wcs[a->bt].nw,a->wcolor[BORDER]);
	dtitle(a);
	displ(a,a->ww - 1,0,wcs[a->bt].ne,a->wcolor[BORDER]);
	/* window's sides */
	for(y1=1;y1 < (a->wh - 1);y1++){
		displ(a,0,y1,wcs[a->bt].side,a->wcolor[BORDER]);
		displ(a,a->ww - 1,y1,wcs[a->bt].side,a->wcolor[BORDER]);
	}
	/* window's bottom */
	displ(a,0,y1,wcs[a->bt].sw,a->wcolor[BORDER]);
	for(x1=1;x1 < (a->ww - 1);x1++)
		displ(a,x1,y1,wcs[a->bt].line,a->wcolor[BORDER]);
	displ(a,x1,y1,wcs[a->bt].se,a->wcolor[BORDER]);
}

/* ###################################################################### */

void dtitle(WINDOW *a){  /* display window's title */
	int x1,i,ln;
	char *s;
	if(!vw(&a))
		return;
	x1=1;
	s=a->tl;
	if(s){
		ln=strlen(s);
		if(ln > a->ww - 2)
			i=0;
		else
			i=( (a->ww - 2 - ln) / 2);
		for(;i;i--,x1++)
			displ(a,x1,0,wcs[a->bt].line,a->wcolor[BORDER]);
		while(*s && x1 < (a->ww - 1) )
			displ(a,x1++,0,*s++,a->wcolor[TITLE]);
	}
	for(;x1 < (a->ww - 1);x1++)
		displ(a,x1,0,wcs[a->bt].line,a->wcolor[BORDER]);
}

/* ###################################################################### */

void wprintf(WINDOW *a,char *ln,...){  /* window oriented printf */
	char dlin[100],*dl;
	va_list ap;
	dl=dlin;
	if(vw(&a)){
		va_start(ap,ln);
		vsprintf(dlin,ln,ap);
		va_end(ap);
		for(;*dl;dl++)
			wputchar(a,*dl);
	}
}

/* ###################################################################### */

void wputchar(WINDOW *a,int c){  /* write a character to the window */
	if(!vw(&a))
		return;
	switch(c){
		case '\n':      if(a->wsp == (a->wh - 3) )
					scroll(a,UP);
				else
					a->wsp++;
				a->cx=0;
				break;
		case '\t':      do
					displ(a,(a->cx++) + 3,a->wsp + 1,' ',a->wcolor[NORMAL]);
				while( (a->cx % TABS) && (a->cx + 1) < (a->ww - 1) );
				break;
		case 0x0d:     break; /* to avoid printing enter char */
		default:	if( (a->cx + 1) < (a->ww - 1) ){
					displ(a,a->cx + 1,a->wsp + 1,c,a->wcolor[NORMAL]);
					a->cx++;
				}
				/* string exceed the width of the window are truncated */
				break;
	}
}

/* ###################################################################### */

void wcursor(WINDOW *a,int x,int y){  /* set window cursor */
	if(vw(&a) && (x < (a->ww - 1) ) && (y < (a->wh - 1) ) ){
		a->cx=x;
		a->wsp=y;
		pc(a->nwx + x + 1,a->nwy + y + 1);
	}
}

/* ###################################################################### */

int get_selection(WINDOW *a,int s,char *keys){	/* get user selection */
	int ky;
	unsigned char c;
	if(!vw(&a))
		return 0;
	c=0;
	accent(a);
	c=get_c();
	if ( c == 0 ) return NO_KEY_PRESSED;
	deaccent(a);
	switch(c){
		case UP:	if(a->sp > 1)
					a->sp--;
				else
					a->sp=a->wsp + 1;
				return NO_KEY_PRESSED;
		case DN:	if(a->sp < (a->wsp + 1) )
					a->sp++;
				else
					a->sp=1;
				return NO_KEY_PRESSED;
		case '\r':
		case ESC:
		case FWD:
		case BS:	break;
		default:	if(keys)
				   for(ky=0;*(keys + ky);ky++)
					if( *(keys+ky)==toupper(c)
					   || *(keys+ky)==tolower(c) )
						return ky + 1;
				break;
	}
	return c == '\r' ? a->sp : c == ESC ? 0 : c;
}

/* ###################################################################### */

int get_char(){
#if (OS == DOS)
	int c;
	while(1){
		r.h.ah=1;
		int86(0x16,&r,&r);
		if(r.x.flags & 0x40)
			continue;
		r.h.ah=0;
		int86(0x16,&r,&r);
		if(r.h.al == 0)
			c=r.h.ah | 0x80;
		else
			c=r.h.al;
		break;
	}
	return c;
#endif
#if (OS == LINUX_COMPATIBLE)
		return (ncurses_getch());
#endif
}

/* ###################################################################### */

void scroll(WINDOW *a,int dir){  /* scroll window's contents up or down */

	int row,col;
	CHAT_TYPE chat;
	if(!vw(&a))
		return;
	row=a->wh - 1;
	if( (a->nx == NULL) && (a->wh > 3) && a->wv){
#if (OS == DOS)
		r.h.ah=dir == UP ? 6 : 7;
		r.h.al=1;
		r.h.bh=a->wcolor[NORMAL];
		r.h.cl=a->nwx + 1;
		r.h.ch=a->nwy + 1;
		r.h.dl=a->nwx + a->ww - 2;
		r.h.dh=a->nwy + a->wh - 2;
		int86(0x10,&r,&r);
#endif
		return;
	}
	/* because of a bug in ROM BIOS of IBM PC scroll of 1 line is avoided */
	if(dir == UP){
		for(row=2;row < (a->wh - 1);row++)
			for(col=1;col < (a->ww - 1);col++){
				chat=dget(a,col,row);
//				displ(a,col,row-1,chat & 0xff,(chat >> 8) & 0xff);
				displ(a,col,row-1,FIND_CHAR_FROM_CHAT(chat),FIND_ATTR_FROM_CHAT(chat));
			}
		for(col=1;col < (a->ww - 1);col++)
			displ(a,col,row - 1,' ',a->wcolor[NORMAL]);
	}
	else{
		for(row=(a->wh - 2);row > 1;row--)
			for(col=1;col < (a->ww - 1);col++){
				chat=dget(a,col,row - 1);
//				displ(a,col,row,chat & 0xff,(chat >> 8) & 0xff);
				displ(a,col,row,FIND_CHAR_FROM_CHAT(chat),FIND_ATTR_FROM_CHAT(chat));
			}
		for(col=1;col < (a->ww - 1);col++)
			displ(a,col,row,' ',a->wcolor[NORMAL]);
	}
}

/* ###################################################################### */

#ifndef STACK_WINDOW
CHAT_TYPE *waddr(WINDOW *a,int x,int y){ /* compute address of a window's display character */
	CHAT_TYPE *vp;
	WINDOW *nxt;
	if(!a->wv){
//		return (int *)(a->sb + y * (a->ww * 2) + x * 2);
		return (CHAT_TYPE *)(a->sb + y * (a->ww * CHAT_SIZE) + x * CHAT_SIZE);
	}
	nxt=a->nx;
	x += a->nwx;
	y += a->nwy;
	while(nxt){
		if(nxt->wv)
			if( (x >= nxt->nwx) &&
			    (x <= (nxt->nwx + nxt->ww - 1) ) )
				if( (y >= nxt->nwy) &&
				    (y <= (nxt->nwy + nxt->wh - 1) ) ){
					x -= nxt->nwx;
					y -= nxt->nwy;
					vp= (CHAT_TYPE *)
//					    (  (nxt->sb)+y*(nxt->ww*2)+x*2);
					    (  (nxt->sb)+y*(nxt->ww * CHAT_SIZE)+x * CHAT_SIZE);
					return vp;
				}
		nxt=nxt->nx;
	}
	return NULL;  /* caller should address video RAM */
}
#endif

/* ###################################################################### */

#ifndef STACK_WINDOW
void displ(WINDOW *a,int x,int y,CHAT_TYPE ch,CHAT_TYPE at){
/* display a character to a window */
	CHAT_TYPE *vp;
	CHAT_TYPE vch;

	if(a->wucf)
		if(isupper(ch)){
		CHANGE_ATTR_FG_COLOR(at,a->wucf);
#if 0
			at &= 0xf8;
			at |= a->wucf;
#endif // 0
		}
//	vch=(ch & 0xff) | (at << 8);
	vch=FIND_CHAR_FROM_CHAT(ch) | FIND_ATTR_FROM_CHAT(at);
	if( (vp=waddr(a,x,y)) != NULL){
		*vp=vch;
	}
	else{
//		poke(vsa,vad(x + a->nwx,y + a->nwy),vch);
		kt_print_char(x + a->nwx,y + a->nwy,FIND_CHAR_FROM_CHAT(ch),FIND_ATTR_FROM_CHAT(at));
	}
}
#endif

/* ###################################################################### */

#ifndef STACK_WINDOW
CHAT_TYPE dget(WINDOW *a,int x,int y){ /* get a displayed character from a window */
	CHAT_TYPE *vp;
	if( (vp= waddr(a,x,y)) != NULL){
		return *vp;
	}
//	return peek(vsa,vad(x + a->nwx,y + a->nwy) );
	return kt_read_char(x + a->nwx,y + a->nwy );
}
#endif

/* ###################################################################### */

#ifndef STACK_WINDOW
void vswap(WINDOW *a,BYTE show){
/* swap the video image with the save buffer */
	int x,y;
	CHAT_TYPE *bf,chat;
	bf=(CHAT_TYPE *)a->sb;
	for(y=0;y < a->wh;y++){
		for(x=0;x < a->ww;x++){
		    chat= *bf;
		    *bf++=dget(a,x,y);
#ifdef QUICK_DRAW_MAC_DRAW
		    if(show)
#endif
//			displ(a,x,y,chat & 0xff,(chat >> 8) & 0xff);
			displ(a,x,y,FIND_CHAR_FROM_CHAT(chat),FIND_ATTR_FROM_CHAT(chat));
		}
	}
}
#endif

/* ###################################################################### */

#ifdef STACK_WINDOW
void vsave(WINDOW *a){
/* save video memory into the save buffer */
	int x,y;
	CHAT_TYPE *bf;
	bf=(int *) a->sb;
	for(y=0;y < a->wh;y++)
		for(x=0;x < a->ww;x++)
//			*bf++=peek(vsa,vad(x + a->nwx,y + a->nwy) );
			*bf++=kt_read_char(x + a->nwx,y + a->nwy );
}
#endif

/* ###################################################################### */

#ifdef STACK_WINDOW
void vrstr(WINDOW *a){
/* restore video memory from the save buffer */
	int x,y;
	CHAT_TYPE *bf;
	bf=(int *) a->sb;
	for(y=0;y < a->wh;y++)
		for(x=0;x < a->ww;x++)
//			poke(vsa,vad(x + a->nwx,y + a->nwy),*bf++);
		  {kt_print_char(x + a->nwx,y + a->nwy,FIND_CHAR_FROM_CHAT(*bf),FIND_ATTR_FROM_CHAT(*bf));bf++;}
}

#endif

/* ###################################################################### */

void acline(WINDOW *a,CHAT_TYPE set){  /* (de)accent the line where select points */
	int x;
	CHAT_TYPE ch;
	if(!vw(&a))
		return;
	for(x=1;x < (a->ww - 1);x++){
//		ch=dget(a,x,a->sp) & 0xff;
		ch=FIND_CHAR_FROM_CHAT(dget(a,x,a->sp));
		displ(a,x,a->sp,ch,set);
	}
}

/* ###################################################################### */

void accent(WINDOW *a){
	acline(a,a->wcolor[ACCENT]);
}

/* ###################################################################### */

void deaccent(WINDOW *a){
	acline(a,a->wcolor[NORMAL]);
}

/* ###################################################################### */

void add_list(WINDOW *a){  /* add a window to the end of the list */
	if(wllt){
		a->pv=wllt;
		wllt->nx=a;
	}
	wllt=a;
	if(!wllh)
		wllh=a;
}

/* ###################################################################### */

void beg_list(WINDOW *a){  /* add a window to the beginning of the list */
	if(wllh){
		a->nx=wllh;
		wllh->pv=a;
	}
	wllh=a;
	if(!wllt)
		wllt=a;
}

/* ###################################################################### */

void remove_list(WINDOW *a){  /* remove a window from the list */
	if(a->nx)
		a->nx->pv=a->pv;
	if(a->pv)
		a->pv->nx=a->nx;
	if(wllh == a)
		wllh=a->nx;
	if(wllt == a)
		wllt=a->pv;
	a->nx=a->pv=NULL;
}

/* ###################################################################### */

void insert_list(WINDOW *w1,WINDOW *w2){  /* insert w1 after w2 */
	w1->pv=w2;
	w1->nx=w2->nx;
	w2->nx=w1;
	if(w1->nx == NULL)
		wllt=w1;
	else
		w1->nx->pv=w1;
}
/* ###################################################################### */

#ifndef STACK_WINDOW
int vw(WINDOW **b){  /* verify the presence of a window in the list */
	WINDOW *a;
	a=wllh;
	if(*b == NULL)
		*b=wllt;
	else
		while(a != NULL){
			if(*b == a)
				break;
			a=a->nx;
		}
	return a != NULL;
}
#endif

/* ###################################################################### */

void error_message(char *s){  /* error message */
	ewnd=enw(50,22,3,max(10,strlen(s) + 2) );
//	swc(ewnd,ALL,RED,YELLOW,BRIGHT);
	swc(ewnd,ALL,KT_RED,KT_YELLOW,KT_HIGH_INTENSITY);
	swt(ewnd," ERROR! ");
	dw(ewnd);
	draw_window(ewnd);
	wprintf(ewnd,s);
	activate_bell();
	error_message_flag=SET;
}

/* ###################################################################### */

void non_error_message(char *s){  /* non_error message */
	ewnd=enw(50,22,3,max(10,strlen(s) + 2) );
//	swc(ewnd,ALL,AQUA,YELLOW,BRIGHT);
	swc(ewnd,ALL,KT_CYAN,KT_YELLOW,KT_HIGH_INTENSITY);
	dw(ewnd);
	draw_window(ewnd);
	wprintf(ewnd,s);
	non_error_message_flag=SET;
}

/* ###################################################################### */

void clear_message(){
	if(ewnd)
		deletw(ewnd);
	ewnd=NULL;
}

/* ###################################################################### */

void wvi(){  /* window variables initialization */
#if (OS == DOS)
	vsa=(fvm() == 7 ? 0xb000 : 0xb800);
#endif
#if(OS ==  LINUX_COMPATIBLE)
	vsa=0xb800;
#endif
	ewnd=NULL;
	wllh=wllt=NULL;
	ca=7;
	im=FALSE;
}

/* ###################################################################### */

void sbg(){  /* screen background */
#if 0
	int a;
	for(a=0;a < 80 * 25 * 2;a +=2)
		poke(vsa,a,(0x71 << 8) | 176);
#endif // 0
	unsigned int x,y;
	for(x=0;x<25;x++)
	  for(y=0;y<80;y++)
		kt_print_char(x,y,176,MAKE_ATTR(KT_NO_BLINKING,KT_WHITE,KT_LOW_INTENSITY,KT_BLUE));
}

/* ###################################################################### */

void ktbg(){  /* kiatel background */

	unsigned char i;

	ktbg_w=enw(0,0,25,80);
	if(ktbg_w == NULL){
		caw();	cs();
		print_error_report("\nDEPM,deph:error in memory allocation!");
		quit(1);
	}
	dw(ktbg_w);
	swb(ktbg_w,DOUBLE_LINE);
//	swc(ktbg_w,ALL,GREEN,WHITE,BRIGHT);
	swc(ktbg_w,ALL,KT_GREEN,KT_WHITE,KT_HIGH_INTENSITY);
//	swt(ktbg_w,"PARAMETERS HELP");
	draw_window(ktbg_w);
	for(i=0;i<19;i++){
		wprompt(ktbg_w,1,i,"KIATEL KIATEL KIATEL KIATEL KIATEL KIATEL KIATEL KIATEL KIATEL KIATEL KIATEL");
	}
		wprompt(ktbg_w,0,19,"ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ");
		wprompt(ktbg_w,1,20,"   ÚÄÄÄÂÄÄÄ¿   I ÚÄÄ¿   E ÚÄÄ¿    ÚÄÄÄÂÄÄÄ¿    ÚÄÄ¿ R   ÚÄÄ¿ E    ÚÄÄÄÂÄÄÄ¿   ");
		wprompt(ktbg_w,1,21,"   ³ S ³ L ³   N ³°°³   X ³°°³    ³ S ³ L ³    ³°°³ C   ³°°³ M    ³ S ³ L ³   ");
		wprompt(ktbg_w,1,22,"   ÀÄÄÄÁÄÄÄÙ   T ÀÄÄÙ   T ÀÄÄÙ    ÀÄÄÄÁÄÄÄÙ    ÀÄÄÙ L   ÀÄÄÙ R    ÀÄÄÄÁÄÄÄÙ   ");
/*
	int a,b,c;
	for(b=0;b < 25;b++){
		c=b * 80 * 2;
		poke(vsa,c,(0x2f << 8) | ' ');
		c += 2;
		poke(vsa,c,(0x2f << 8) | ' ');
		for(a=0;a < 11;a++){
			c += 2;
			poke(vsa,c,(0x2f << 8) | 'K');
			c += 2;
			poke(vsa,c,(0x2f << 8) | 'I');
			c += 2;
			poke(vsa,c,(0x2f << 8) | 'A');
			c += 2;
			poke(vsa,c,(0x2f << 8) | 'T');
			c += 2;
			poke(vsa,c,(0x2f << 8) | 'E');
			c += 2;
			poke(vsa,c,(0x2f << 8) | 'L');
			c += 2;
			poke(vsa,c,(0x2f << 8) | ' ');
		}
		c += 2;
		poke(vsa,c,(0x2f << 8) | ' ');
	}
*/
}

/* ###################################################################### */

void idew(WINDOW *a){  /* initialize data entry window */
	FIELD *b,*c;
	for(b=a->ff;b;){
		c=b->fnxt;
		free(b);
		b=c;
	}
	a->ff=NULL;
}

/* ###################################################################### */

FIELD *enf(WINDOW *a,int cl,int rw,char *msk,char *bf,int ty){
/* establish new field */
	FIELD *b;
	if( (b= (FIELD *) malloc(sizeof(FIELD)) ) == NULL)
		return NULL;
	b->fmask=msk;
	b->frow=rw;
	b->fcol=cl;
	b->fbuf=bf;
	b->ftype=ty;
	b->fprot=0;
	b->fnxt=b->fprv=NULL;
	b->fup=b->fdn=NULL;
	b->fvalid=NULL;
	b->fhelp=NULL;
	b->fhwin=NULL;
	b->flx=b->fly=0;
	add_field(a,b);
	return b;
}

/* ###################################################################### */

void add_field(WINDOW * a,FIELD *b){  /* add a field to the end of the list */
	if(a->lf){
		b->fprv=a->lf;
		a->lf->fnxt=b;
	}
	a->lf=b;
	if(!a->ff)
		a->ff=b;
}

/* ###################################################################### */

void ddf(WINDOW *a,char *bf,char *msk){  /* display a data field */
	for(;*msk;msk++)
		wputchar(a,*msk != FIELD_CHAR ? *msk : *bf++);
}

/* ###################################################################### */

void ddvf(WINDOW *a,FIELD *b){	/* display data value of a field */
	if(b->ftype != PASSWORD){
		wcursor(a,b->fcol,b->frow);
		ddf(a,b->fbuf,b->fmask);
	}
}

/* ###################################################################### */

void dafw(WINDOW *a){  /* display all field of a window */
	FIELD *b;
	for(b=a->ff;b != NULL;b=b->fnxt)
		ddvf(a,b);
}

/* ###################################################################### */

void caf(WINDOW *a){  /* clear all field */
	FIELD *b;
	char *bf,*msk;
	for(b=a->ff;b != NULL;b=b->fnxt)
		for(bf=b->fbuf,msk=b->fmask;*msk;msk++)
			if(*msk == FIELD_CHAR)
				*bf++=' ';
	dafw(a);
}

/* ###################################################################### */

int old_read_field(WINDOW *a,FIELD *b){  /* read a field from the keyboard */
	char *mask,*buff;
	int done,c,column;
	mask=b->fmask;
	buff=b->fbuf;
	done=FALSE;
	for(column=b->fcol;*mask && (*mask != FIELD_CHAR);column++,mask++);
	while(1){
		wcursor(a,column,b->frow);
		c=get_char();
		if(b->ftype == 'A')
			c=toupper(c);
		clear_message();
		switch(c){
			case '\b':
			case BS:	if(buff == b->fbuf){
						done= c == BS;
						break;
					}
					--buff;
					do{
						--mask;
						--column;
					}while(*mask != FIELD_CHAR);
					if(c == BS)
						break;
			case DEL:	movmem(buff+1,buff,strlen(buff));
					*(buff + strlen(buff) )=' ';
					wcursor(a,column,b->frow);
					ddf(a,buff,mask);
					break;
			case SPACE:
			case FWD:	do{
						column++;
						mask++;
					}while(*mask && (*mask != FIELD_CHAR) );
					buff++;
					break;
			case INS:	im ^= TRUE;
					insert_status();
					break;
			default:	if(endstroke(c)){
						done=TRUE;
						break;
					}
					if(b->ftype == HEX){
					  if(!isdigit(c)){
					     c=toupper(c);
					     if( (c != 'A') && (c != 'B') &&
						 (c != 'C') && (c != 'D') &&
						 (c != 'E') && (c != 'F') ){
						 error_message("HEX NUMBER ONLY");
						 break;
					     }
					  }
					}
					if(im){
					  movmem(buff,buff+1,strlen(buff)-1);
					  ddf(a,buff,mask);
					  wcursor(a,column,b->frow);
					}
					*buff++=c;
					if(b->ftype != PASSWORD)
						wputchar(a,c);
					do{
						column++;
						mask++;
					}while(*mask && (*mask != FIELD_CHAR) );
					if(!*mask)
						c=FWD;
					break;
		}
		if(!*mask)
			done=TRUE;
		if(done)
			break;
	}
	return c;
}

/* ###################################################################### */

int endstroke(int c){  /* test c for an ending keystroke */
	switch(c){
		case '\r':
		case '\n':
		case '\t':
		case ESC:
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
		case PGUP:
		case PGDN:
		case HOME:
		case END:
		case UP:
		case DN:	return TRUE;
		default:	return FALSE;
	}
}

/* ###################################################################### */

int spaces(char *c){  /* test for spaces */
	for(;*c == ' ';c++);
	return !*c;
}

/* ###################################################################### */

int old_data_entry(WINDOW *a){	/* process data entry for a screen template */
	FIELD *b;
	int exitcode=0,done;
	done=FALSE;
	dafw(a);
	b=a->ff;
	/* collect data from keyboard into screen */
	while( (b != NULL) && (done == FALSE) ){
		wcursor(a,b->fcol,b->frow);
		if(b->fprot == FALSE){
			reverse_video(a);
			ddvf(a,b);
			exitcode=old_read_field(a,b);
		}
		else
			exitcode=FWD;
		normal_video(a);
		ddvf(a,b);
		switch(exitcode){
			case DN:
			case '\t':
			case FWD:	b=b->fnxt;
					if(b == NULL)
						b=a->ff;
					break;
			case UP:
			case BS:	b=b->fprv;
					if(b == NULL)
						b=a->lf;
					break;
			default:	done=endstroke(exitcode);
					break;
		}
	}
	return exitcode;
}

/* ###################################################################### */

void wprompt(WINDOW *a,int x,int y,char *s){  /* display a window prompt */
	wcursor(a,x,y);
	wprintf(a,s);
	pc(80,25);
}

/* ###################################################################### */

void insert_status(){  /* set insert/exchange cursor shape */
#if (OS == DOS)
	sct(im ? 0x0106 : 0x0607);
#endif
}


/* menu bar functions */

/* ###################################################################### */

void menu_select(char *name,MENU *mn){	/* display & process a menu */
	WINDOW *hmenu;
	int sx,sy;
	int hsel=1,vsel;

	rcp(&sx,&sy);  /* save cursor position */
	pc(0,26);  /* position cursor */
	hmenu=open_menu(name,mn,hsel);
	while((hsel=gethmenu(mn,hmenu,hsel)) != 0 ){
		vsel=1;
		while((vsel=getvmn(mn,hmenu,&hsel,vsel)) != 0 ){
			deletw(hmenu);
			( *(mn+hsel-1)->func[vsel-1])(hsel,vsel);
			hmenu=open_menu(name,mn,hsel);
		}
	}
	deletw(hmenu);
	pc(sx,sy);
}

/* ###################################################################### */

WINDOW *open_menu(char *mnm,MENU *mn,int hsel){  /* open a horizontal menu */
	int i;
	WINDOW *hmenu;
	hmenu=enw(0,0,3,80);  /* establish new window */
	swt(hmenu,mnm);  /* set window's title */
#if 0
	swc(hmenu,ALL,MAGENTA,GREEN,BRIGHT);  /* set window's color */
	swc(hmenu,NORMAL,MAGENTA,YELLOW,BRIGHT);
	swc(hmenu,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
	swc(hmenu,ALL,KT_MAGENTA,KT_GREEN,KT_HIGH_INTENSITY);  /* set window's color */
	swc(hmenu,NORMAL,KT_MAGENTA,KT_YELLOW,KT_HIGH_INTENSITY);
	swc(hmenu,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
	dw(hmenu);  /* display window */
	draw_window(hmenu);
	for(i=0;(mn+i)->mname;i++)
		wprintf(hmenu," %-11.11s ",(mn+i)->mname);
	light(mn,hmenu,hsel,1);
	pc(0,26);
	return hmenu;
}

/* ###################################################################### */

int gethmenu(MENU *mn,WINDOW *hmenu,int hsel){	/* get a horizontal selection */
	int sel;
	light(mn,hmenu,hsel,1);
	while(TRUE){
		switch(sel=get_char()){
			case FWD:
			case BS:	hsel=haccent(mn,hmenu,hsel,sel);
					break;
			case ESC:	return 0;
			case '\r':      return hsel;
			default:	activate_bell();
					break;
		}
	}
}

/* ###################################################################### */

int getvmn(MENU *mn,WINDOW *hmenu,int *hsel,int vsel){
/* pop down a vertical menu */
	WINDOW *vmenu;
	int ht=10,wd=20;
	char **mp;
	while(1){
		dimension( (mn + *hsel - 1)->mselcs,&ht,&wd);
		vmenu=enw(2 + (*hsel - 1) * 12,2,ht,wd);
#if 0
		swc(vmenu,ALL,MAGENTA,GREEN,BRIGHT);
		swc(vmenu,NORMAL,MAGENTA,WHITE,BRIGHT);
		swc(vmenu,ACCENT,BLUE,WHITE,BRIGHT);
#endif // 0
		swc(vmenu,ALL,KT_MAGENTA,KT_GREEN,KT_HIGH_INTENSITY);
		swc(vmenu,NORMAL,KT_MAGENTA,KT_WHITE,KT_HIGH_INTENSITY);
		swc(vmenu,ACCENT,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
		swb(vmenu,POP_DOWN_MENU);
		dw(vmenu);
		draw_window(vmenu);
		for(mp=(mn + *hsel - 1)->mselcs;*mp;mp++)
			wprintf(vmenu,"\n%s",*mp);
		vsel=get_selection(vmenu,vsel,"");
		deletw(vmenu);
		if( (vsel == FWD) || (vsel == BS) ){
			*hsel=haccent(mn,hmenu,*hsel,vsel);
			vsel=1;
		}
		else
			return vsel;
	}
}

/* ###################################################################### */

int haccent(MENU *mn,WINDOW *hmenu,int hsel,int sel){
/* manage the horizontal menu selection accent */
	switch(sel){
		case FWD:	light(mn,hmenu,hsel,0);
				if( (mn + hsel)->mname)
					hsel++;
				else
					hsel=1;
				light(mn,hmenu,hsel,1);
				break;
		case BS:	light(mn,hmenu,hsel,0);
				if(hsel == 1)
					while( (mn + hsel)->mname)
						hsel++;
				else
					--hsel;
				light(mn,hmenu,hsel,1);
				break;
		default:	break;
	}
	return hsel;
}

/* ###################################################################### */

void dimension(char *sl[],int *ht,int *wd){
/* compute a menu's height & width */
	for(*ht=*wd=0;sl[ *ht ];++(*ht) )
		*wd=max(*wd,strlen(sl[*ht]) );
	*ht += 2;
	*wd += 2;
}

/* ###################################################################### */

void light(MENU *mn,WINDOW *hmenu,int hsel,int d){
/* accent a horizontal menu selection */
	if(d)
		reverse_video(hmenu);
	wcursor(hmenu,(hsel - 1) * 13 + 2,0);
	wprintf(hmenu,(mn + hsel - 1)->mname);
	normal_video(hmenu);
	pc(0,26);
}

/* ###################################################################### */

int data_entry(WINDOW *a){

	if (data_entry_flag == SET){
		return(data_entry_hndlr(a));
	}
	else{
		data_entry_flag = SET;
		return(data_entry_init(a));
	}
}

/* ###################################################################### */

int data_entry_init(WINDOW *a){
	dafw(a);
	a->saved_field=a->ff;
	data_entry_flag=SET;
	return OK;
}

/* ###################################################################### */

int data_entry_hndlr(WINDOW *a){  /* process data entry for a screen template */

	FIELD *b;
	int exitcode;
	b=a->saved_field;

	/* collect data from keyboard into screen */
	if(b->fprot == FALSE){
		if ( read_field_flag == RESET){
		    reverse_video(a);
		    ddvf(a,b);
		}
		exitcode=read_field(a,b);
		if ( read_field_flag == SET)
			return exitcode;
	}
	else
		exitcode=FWD;
	normal_video(a);
	ddvf(a,b);
	switch(exitcode){
		case DN:	if(b->fdn != NULL){
					b=b->fdn;
					break;
				}
		case '\r':
		case '\t':
		case FWD:	b=b->fnxt;
				if(b == NULL)
					b=a->ff;
				break;
		case UP:
				if(b->fup != NULL){
					b=b->fup;
					break;
				}
		case BS:	b=b->fprv;
				if(b == NULL)
					b=a->lf;
				break;
		default:	/*done=*/endstroke(exitcode);
				break;
	}
	a->saved_field=b;
	return exitcode;
}

/* #########################################################################*/

int read_field(WINDOW *a,FIELD *b){
	if (read_field_flag == SET){
		return(read_field_hndlr(a,b));
	}
	else{
		return(read_field_init(a,b));
	}
}

/* #########################################################################*/

int read_field_init(WINDOW *a,FIELD *b){  /* read a field from the keyboard */

	char *mask;
	int column;

	mask=b->fmask;
	for(column=b->fcol;*mask && (*mask != FIELD_CHAR);column++,mask++);

	b->saved_column=column;
	b->saved_mask=mask;
	b->saved_buff=b->fbuf;
	read_field_flag=SET;
	return 0;

#if 0
	while(1){
		wcursor(a,column,b->frow);
		c=get_char();
		if(b->ftype == 'A')
			c=toupper(c);
		clear_message();
		switch(c){
			case '\b':
			case BS:	if(buff == b->fbuf){
						done= c == BS;
						break;
					}
					--buff;
					do{
						--mask;
						--column;
					}while(*mask != FIELD_CHAR);
					if(c == BS)
						break;
			case DEL:	movmem(buff+1,buff,strlen(buff));
					*(buff + strlen(buff) )=' ';
					wcursor(a,column,b->frow);
					ddf(a,buff,mask);
					break;
			case SPACE:
			case FWD:	do{
						column++;
						mask++;
					}while(*mask && (*mask != FIELD_CHAR) );
					buff++;
					break;
			case INS:	im ^= TRUE;
					insert_status();
					break;
			default:	if(endstroke(c)){
						done=TRUE;
						break;
					}
					if(b->ftype == 'B'){
						if(c == '1')
							c='|';
						if(c == '0')
							c='-';
						if( (c != '|') && (c != '-') ){
							error_message("INVALID CHARACTER");
							break;
						}
					}
					if(b->ftype == 'D'){
						if(!isdigit(c)){
							error_message("INVALID CHARACTER");
							break;
						}
					}
					if(b->ftype == 'A'){
						if(isdigit(c)){
							error_message("INVALID CHARACTER");
							break;
						}
					}
					if(im){
					  movmem(buff,buff+1,strlen(buff)-1);
					  ddf(a,buff,mask);
					  wcursor(a,column,b->frow);
					}
					*buff++=c;
					if(b->ftype != 'P')
					/* if type not password */
						wputchar(a,c);
					else
						wputchar(a,' ');
					do{
						column++;
						mask++;
					}while(*mask && (*mask != FIELD_CHAR) );
					if(!*mask)
						c=FWD;
					break;
		}
		if(!*mask)
			done=TRUE;
		if(done)
			break;
	}
	return c;

#endif // 0
}

/* #########################################################################*/

int read_field_hndlr(WINDOW *a,FIELD *b){  /* read a field from the keyboard */

	char *mask,*buff;
	int done,column;
	unsigned char c;
	done=FALSE;
	mask=b->saved_mask;
	buff=b->saved_buff;
	column=b->saved_column;
	wcursor(a,column,b->frow);
	c=get_c();
	if ( c == NO_KEY_PRESSED ) return c;
	if(b->ftype == 'A')
		c=toupper(c);
	clear_message();
	switch(c){
		case '\b':
		case BS:	if(buff == b->fbuf){
					done= c == BS;
					break;
				}
				--buff;
				do{
					--mask;
					--column;
				}while(*mask != FIELD_CHAR);
				if(c == BS)
					break;
		case DEL:	movmem(buff+1,buff,strlen(buff));
				*(buff + strlen(buff) )=' ';
				wcursor(a,column,b->frow);
				ddf(a,buff,mask);
				break;
		case SPACE:
		case FWD:	do{
					column++;
					mask++;
				}while(*mask && (*mask != FIELD_CHAR) );
				buff++;
				break;
		case INS:	im ^= TRUE;
				insert_status();
				break;
		default:	if(endstroke(c)){
					done=TRUE;
					break;
				}
				if(b->ftype == 'B'){
					if(c == '1')
						c='|';
					if(c == '0')
						c='-';
					if( (c != '|') && (c != '-') ){
						error_message("INVALID CHARACTER");
						break;
					}
				}
				if(b->ftype == 'D'){
					if(!isdigit(c)){
						error_message("INVALID CHARACTER");
						break;
					}
				}
				if(b->ftype == 'A'){
					if(isdigit(c)){
						error_message("INVALID CHARACTER");
						break;
					}
				}
				if(im){
				  movmem(buff,buff+1,strlen(buff)-1);
				  ddf(a,buff,mask);
				  wcursor(a,column,b->frow);
				}
				*buff++=c;
				if(b->ftype != 'P')
				/* if type not password */
					wputchar(a,c);
				else
					wputchar(a,' ');
				do{
					column++;
					mask++;
				}while(*mask && (*mask != FIELD_CHAR) );
				if(!*mask)
					c=FWD;
				break;
	}
	if(!*mask)
		done=TRUE;
	if(done){
		read_field_flag=RESET;
	}
	b->saved_mask=mask;
	b->saved_buff=buff;
	b->saved_column=column;
	return c;
}

/* ###################################################################### */

#ifdef DISPLAY_BILLING	      // commented in order not to include omtfp.h in this file
void bill_bg(void){  /* kiatel background */

	bill_w=enw(0,0,21,80);
	if(bill_w == NULL){
		caw();	cs();
		print_error_report("\nDEPM,deph:error in memory allocation!");
		quit(1);
	}
	dw(bill_w);
	swb(bill_w,NO_BORDER);
#if 0
	swc(bill_w,ALL,BLACK,WHITE,BRIGHT);
	swc(bill_w,NORMAL,BLUE,WHITE,BRIGHT);
	swc(bill_w,TITLE,RED,YELLOW,BRIGHT);
#endif // 0
	swc(bill_w,ALL,KT_BLACK,KT_WHITE,KT_HIGH_INTENSITY);
	swc(bill_w,NORMAL,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
	swc(bill_w,TITLE,KT_RED,KT_YELLOW,KT_HIGH_INTENSITY);
//	swt(bill_w," Source       Destination  Trnk No.  Call Type  Start Time  Duration   Count  ");
//	swt(bill_w,"    Source       Destination         Call Type  Start Time  Duration   Count  ");
	swt(bill_w,"Source                        Destination         Type StrtTime Duration Count");
	draw_window(bill_w);

	sync_bill_w=enw(0,20,5,80);
	if(sync_bill_w == NULL){
		caw();	cs();
		print_error_report("\nDEPM,deph:error in memory allocation!");
		quit(1);
	}
	dw(sync_bill_w);
	swb(sync_bill_w,DOUBLE_LINE);
//	swc(sync_bill_w,ALL,BLACK,WHITE,BRIGHT);
	swc(sync_bill_w,ALL,KT_BLACK,KT_WHITE,KT_HIGH_INTENSITY);
	draw_window(sync_bill_w);
	wprompt(sync_bill_w,1,0,"   ÚÄÄÄÂÄÄÄ¿   I ÚÄÄ¿   E ÚÄÄ¿    ÚÄÄÄÂÄÄÄ¿    ÚÄÄ¿ R   ÚÄÄ¿ E    ÚÄÄÄÂÄÄÄ¿   ");
	wprompt(sync_bill_w,1,1,"   ³ S ³ L ³   N ³°°³   X ³°°³    ³ S ³ L ³    ³°°³ C   ³°°³ M    ³ S ³ L ³   ");
	wprompt(sync_bill_w,1,2,"   ÀÄÄÄÁÄÄÄÙ   T ÀÄÄÙ   T ÀÄÄÙ    ÀÄÄÄÁÄÄÄÙ    ÀÄÄÙ L   ÀÄÄÙ R    ÀÄÄÄÁÄÄÄÙ   ");
}
#endif

/* ###################################################################### */

#ifdef DISPLAY_ALARM	      // commented in order not to include omtfp.h in this file
void alarm_bg(void){  /* kiatel background */

	alarm_w=enw(0,0,21,80);
	if(alarm_w == NULL){
		caw();	cs();
		print_error_report("\nDEPM,alarm_bg:error in memory allocation!");
		quit(1);
	}
	dw(alarm_w);
	swb(alarm_w,NO_BORDER);
#if 0
	swc(alarm_w,ALL,BLACK,WHITE,BRIGHT);
	swc(alarm_w,NORMAL,BLUE,WHITE,BRIGHT);
	swc(alarm_w,TITLE,RED,YELLOW,BRIGHT);
#endif // 0
	swc(alarm_w,ALL,KT_BLACK,KT_WHITE,KT_HIGH_INTENSITY);
	swc(alarm_w,NORMAL,KT_BLUE,KT_WHITE,KT_HIGH_INTENSITY);
	swc(alarm_w,TITLE,KT_RED,KT_YELLOW,KT_HIGH_INTENSITY);
	swt(alarm_w," ON-LINE ALARM/EVENT LOG ");
	draw_window(alarm_w);

	sync_alarm_w=enw(0,20,5,80);
	if(sync_alarm_w == NULL){
		caw();	cs();
		print_error_report("\nDEPM,alarm_bg:error in memory allocation!");
		quit(1);
	}
	dw(sync_alarm_w);
	swb(sync_alarm_w,DOUBLE_LINE);
//	swc(sync_alarm_w,ALL,BLACK,WHITE,BRIGHT);
	swc(sync_alarm_w,ALL,KT_BLACK,KT_WHITE,KT_HIGH_INTENSITY);
	draw_window(sync_alarm_w);
	wprompt(sync_alarm_w,1,0,"   ÚÄÄÄÂÄÄÄ¿   I ÚÄÄ¿   E ÚÄÄ¿    ÚÄÄÄÂÄÄÄ¿    ÚÄÄ¿ R   ÚÄÄ¿ E    ÚÄÄÄÂÄÄÄ¿   ");
	wprompt(sync_alarm_w,1,1,"   ³ S ³ L ³   N ³°°³   X ³°°³    ³ S ³ L ³    ³°°³ C   ³°°³ M    ³ S ³ L ³   ");
	wprompt(sync_alarm_w,1,2,"   ÀÄÄÄÁÄÄÄÙ   T ÀÄÄÙ   T ÀÄÄÙ    ÀÄÄÄÁÄÄÄÙ    ÀÄÄÙ L   ÀÄÄÙ R    ÀÄÄÄÁÄÄÄÙ   ");
}
#endif

/* ###################################################################### */


/* ###################################################################### */

void draw_window(WINDOW *a){
#ifdef QUICK_DRAW_MAC_DRAW
    if(vw(&a)){
	int x,y;

	for(y=1;y < (a->wh -1);y++){
		for(x=1;x < (a->ww - 1);x++){
/*
			CHAT_TYPE c,atr;
			c=dget(a,x,y);
			if( FIND_ATTR_FROM_CHAT(c) == a->pn){
				atr= a->wcolor[NORMAL];
			}
			else{
				atr= a->wcolor[ACCENT];
			}
			displ(a,x,y,FIND_CHAR_FROM_CHAT(c),atr);
*/
			displ(a,x,y,' ',a->wcolor[NORMAL]);
		}
	}
	wframe(a);
	a->pn=a->wcolor[NORMAL];
    }
#endif
}

/* ###################################################################### */
#endif // (OS == DOS) || (OS == LINUX_COMPATIBLE)

#ifdef DISPLAY_ALARM
void send_alarm_to_screen(char *buffer){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
	if (alarm_display_flag == SET){
		if (alarm_line == 18)
			alarm_scroll_flag=SET;
		else
			alarm_line++;
		if (alarm_scroll_flag)
			scroll(alarm_w,UP);
		wprompt(alarm_w,0,alarm_line,buffer);
	}
#endif
}
void show_alarm_without_date(char *fmt, ...){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
	char buffer[255];
	va_list argptr;
	if (alarm_display_flag == SET){
		va_start(argptr, fmt);
		vsprintf(buffer,fmt,argptr);
		va_end(argptr);
		send_alarm_to_screen(buffer);
	}
#endif
}
extern DATTIM sdt;
void show_alarm(char *fmt, ...){
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
	char buffer[81];
	char date_buffer[120];
	va_list argptr;
	if (alarm_display_flag == SET){
		va_start(argptr, fmt);
		vsprintf(buffer,fmt,argptr);
		va_end(argptr);
		sdt=*get_dt(&sdt);
		sprintf(date_buffer,"%4x-%02x-%02x %02x:%02x:%02x: ",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se);
		strcat(date_buffer,buffer);
		send_alarm_to_screen(date_buffer);
	}
#endif
}
#endif
