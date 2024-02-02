

#if 0
		   4   4		  4	4
		ÚÄÄÄÄÂÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÂÄÄÄÄ¿
in_use block	³  1 ³size³		 ³size³  1 ³
		³    ³	| ³   data	 ³  | ³    ³
		³    ³0x01³		 ³0x01³    ³
		ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ
		ÚÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÄÂÄÄÄÂÄÄÄÄÂÄÄÄÄ¿
free   block	³  1 ³size³ptr ³buckt³	 ³size³  1 ³
		³    ³ &  ÃÄÄÄÄÁÄÄÄÄÄÁÄÄÄ´  & ³    ³
		³    ³0xfe³    data	 ³0xfe³    ³
		ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ
		ÚÄÄÄÄÂÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÂÄÄÄÄ¿
contiguous	³  1 ³size³		 ³size³size³		  ³size³  1 ³
in_use blocks	³    ³	| ³   data	 ³  | ³  | ³   data	  ³  | ³    ³
		³    ³0x01³		 ³0x01³0x01³		  ³0x01³    ³
		ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ

		     bp 		      AFTER(bp)
		ÚÄÄÄÄ^ÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄ^ÄÄÄÄ¿
		³  1 ³size³		 ³size³  1 ³
		³    ³	  ³   data	 ³    ³    ³
		³    ³	  ³		 ³    ³    ³
		^ÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄ^ÄÄÄÄÁÄÄÄÄÙ
		BEFSZ(bp)		 ENDSZ(bp)


		BEFORE(bp)		      bp
		^ÄÄÄÄÂÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄ^ÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÂÄÄÄÄ¿
		³  1 ³size³		 ³size³size³		  ³size³  1 ³
		³    ³	  ³   data	 ³    ³    ³   data	  ³    ³    ³
		³    ³	  ³		 ³    ³    ³		  ³    ³    ³
		ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ

		     bp 		      AFTER(bp)
		ÚÄÄÄÄ^ÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄ^ÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÂÄÄÄÄ¿
		³  1 ³size³		 ³size³size³		  ³size³  1 ³
		³    ³	  ³   data	 ³    ³    ³   data	  ³    ³    ³
		³    ³	  ³		 ³    ³    ³		  ³    ³    ³
		ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ

#endif // 0

#define COMPILER_NULL_IS_USED
//#define DEBUG_ACT

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else  // CENTRAL_EXTERN
#include "hdr.h"
#undef NULL
#ifdef KIATEL_DYNAMIC_MEM
#include <stdlib.h>
#include <string.h>
#if (COMPILER == GCC)
#include <unistd.h>
#endif
#ifndef NULL
#define NULL		0
#endif
extern BLOCK *my_slop;
extern BLOCK *my_freelist[];
#if NUMSMALL
extern BLOCK *my_smallblocks[];
#endif
extern unsigned char dynamic_mem_arr[];
extern unsigned long curr_mem_indx;
extern BLOCK *expected_sbrk;
extern MEM_REC_ST mem_rec[];
#endif // KIATEL_DYNAMIC_MEM
#endif // CENTRAL_EXTERN

#ifndef USE_OS_MALLOC

#ifdef GENERATE_FOR_64BIT
#define PTRSZ		8
#else
#define PTRSZ		4
#endif

#define BEFORE(bp)	((BLOCK *)((char *)bp - *(int *)((char *)bp - PTRSZ) - (2*PTRSZ)))
#define BEFSZ(bp)	(*(KT_SIZE_T *)((char *)bp - PTRSZ))
#define ENDSZ(bp)	(*(KT_SIZE_T *)((char *)bp + bp->size + PTRSZ))
#define AFTER(bp)	((BLOCK *)((char *)bp + bp->size + (2*PTRSZ)))
#define DATA(bp)	((char *)&(bp->next))
#ifdef DEBUG_ACT
#define CHECK(p) do { my_check(p); my_assert(p->size == ENDSZ(p)); my_consistency(); } while (0)
#define CHECK1(p) do { my_check(p); my_assert(p->size == ENDSZ(p)); } while (0)
#else
#if ((defined MALLOC_DEBUG) && (defined SYSTEM_SIDE))
#define CHECK(p)  if(p->size != ENDSZ(p)){				\
		    ws2p("mfail:\r\n");                                 \
		    guchar *parm=(guchar *)(p)+4;			\
		    guint mr1=*((guint *)(parm));			\
		    print_mr(mr1);					\
#ifndef DO_NOT_BULK_SAVE_ADDR
		    print_addr(mem_rec[mr1].addr);			\
3endif
		    int2prnt(p->size);toprnq('-');                      \
		    int2prnt(ENDSZ(p));toprnq(CR);			\
		    print_call_frame(); 				\
		    toprnq(CR); 					\
		    ENDSZ(p)=p->size;					\
		  }
#else
#define CHECK(p)
#endif
#endif
#define RET(rv) CHECK(rv); ENDSZ(rv) |= 1; rv->size |= 1; return DATA(rv)

#ifdef GENERATE_FOR_64BIT
#define ALIGN		16
#else
#define ALIGN		8
#endif
#define SMALL		(NUMSMALL*ALIGN)

#define MIN_SAVE_EXTRA	64
#define BIG_BLOCK	4096
/* ======================================================================== */
/* ======================================================================== */
//mycode :this routine added
#ifdef KIATEL_DYNAMIC_MEM
void *my_sbrk(unsigned int size){

//???
	void *ptr;
	if ((curr_mem_indx+size) <  MAX_DYNAMIC_MEM_SIZE){
// ?????????????
#if (COMPILER == GCC)
		ptr=&dynamic_mem_arr[curr_mem_indx];
#endif
#if (COMPILER == BCC)
		ptr=&dynamic_mem_arr[(unsigned int)curr_mem_indx];
#endif
		curr_mem_indx+=size;
		return(ptr);
	}
	else{
//???
		return ((void *)(-1));
	}
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
//mycode :this routine added
#ifdef KIATEL_DYNAMIC_MEM
void dynamic_mem_init(void){
	unsigned char i;
//???
	my_slop = 0;
	for(i=0;i<30;i++)
		my_freelist[i]=0;
	curr_mem_indx=0;
	expected_sbrk=0;
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
#ifdef DEBUG_ACT
#ifdef KIATEL_DYNAMIC_MEM
//static void my_check(BLOCK *b){
void my_check(BLOCK *b){

	prnt_char('c');prnt_char('h');prnt_char('e');prnt_char('c');prnt_char('k');prnt_char(':');
	prnt_int((unsigned int)b);prnt_char('-');
	prnt_int(b->size);prnt_char('-');
	prnt_int((unsigned int)&(ENDSZ(b)));prnt_char('-');
	prnt_int(ENDSZ(b));prnt_char(CR);
}
#endif // KIATEL_DYNAMIC_MEM
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef DEBUG_ACT
#ifdef KIATEL_DYNAMIC_MEM
//static void my_consistency() {
void my_consistency(void) {
#if 0
	int b;
	BLOCK *bl;

	if (my_slop)
		CHECK1(my_slop);
	32 is not correct.array size is 30!
	for (b=0; b<32; b++)
		for (bl=my_freelist[b]; bl; bl=bl->next)
			CHECK1(bl);
#endif //0
}
#endif // KIATEL_DYNAMIC_MEM
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
// ???
//static inline int my_size2bucket(KT_SIZE_T size){
int my_size2bucket(KT_SIZE_T size){

	int rv=0;
	// a method to have linked lists of blocks with different sizes.
	// 0..3   :  rv=0
	// 4..7   :  rv=1
	// 8..15  :  rv=2
	// 16..31 :  rv=3
	// ....
	size>>=2;
	while (size){
		rv++;
		size>>=1;
	}
	return rv;
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
// ???
//static inline int my_b2bucket(BLOCK *b){
int my_b2bucket(BLOCK *b){

	if (b->bucket == -1)
		b->bucket = my_size2bucket(b->size);
	return b->bucket;
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
// ???
//static inline BLOCK *my_split_block(BLOCK *b, KT_SIZE_T size){
BLOCK *my_split_block(BLOCK *b, KT_SIZE_T size){

#if 0
	     b
	ÚÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄ¿
	³  1 ³bsize³	   data 				 ³bsize³  1 ³
	³    ³	   ³						 ³     ³    ³
	³    ³	   ³						 ³     ³    ³
	ÀÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÙ

	<-- requested block-><----------------- remained block ------------->
				   rv
	ÚÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄ¿
	³  1 ³ size³  data   ³ size³new  ³ptr  ³buckt³		 ³new  ³  1 ³
	³    ³	   ³	     ³	   ³bsizeÃÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄ´bsize³    ³
	³    ³	   ³	     ³	   ³	 ³	 data		 ³     ³    ³
	ÀÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÙ

#endif // 0
	BLOCK *rv = (BLOCK *)((char *)b + size+(2*PTRSZ));
#ifdef DEBUG_ACT
	prnt_char('s');prnt_char('p');prnt_char('l');prnt_char('i');prnt_char('t');prnt_char(':');
	prnt_int(b->size);prnt_char('-');
	prnt_int((unsigned int)b);prnt_char('-');
	prnt_int(size);prnt_char('-');
	prnt_int((unsigned int)rv);prnt_char(CR);
#endif
	rv->size = b->size - size - (2*PTRSZ); // two "size" bytes  for new block
	rv->bucket = -1;
	b->size = size;
	ENDSZ(b) = b->size;
	ENDSZ(rv) = rv->size;
	CHECK(b);
	CHECK(rv);
	return rv;
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void *my_malloc(KT_SIZE_T size){
	int b, chunk_size;
	BLOCK *rv, **prev;
//	static BLOCK *expected_sbrk = 0;

	if (size<ALIGN) size = ALIGN;
	size = (size+(ALIGN-1))&~(ALIGN-1);
#ifdef DEBUG_ACT
	prnt_char('m');prnt_char('a');prnt_char('l');prnt_char('l');prnt_char('o');prnt_char('c');prnt_char(':');
	prnt_int(size);prnt_char(CR);
#endif

#if NUMSMALL
	if (size < SMALL){
		rv = my_smallblocks[size/ALIGN];
		if (rv){
			my_smallblocks[size/ALIGN] = rv->next;
			return DATA(rv);
		}
	}
#endif
	if (my_slop && my_slop->size >= size){
		rv = my_slop;
#ifdef DEBUG_ACT
		prnt_char('s');prnt_char('l');prnt_char('o');prnt_char('p');prnt_char(':');
		prnt_int(my_slop->size);prnt_char('-');
		prnt_int((unsigned int)my_slop);prnt_char(CR);
#endif
		if (my_slop->size >= size+MIN_SAVE_EXTRA){
			my_slop = my_split_block(my_slop, size);
#ifdef DEBUG_ACT
			prnt_char('s');prnt_char('l');prnt_char('o');prnt_char('p');prnt_char(':');
			prnt_int(my_slop->size);prnt_char('-');
			prnt_int((unsigned int)my_slop);prnt_char(CR);
#endif
		}
		else
			my_slop = 0;
		RET(rv);
	}
	b = my_size2bucket(size);
	prev = &(my_freelist[b]);
	for (rv=my_freelist[b]; rv; prev=&(rv->next), rv=rv->next){
		if (rv->size >= size && rv->size < size+size/4){
			*prev = rv->next;
			RET(rv);
		}
	}
	while (b < 30){
		prev = &(my_freelist[b]);
#ifdef DEBUG_ACT
		prnt_char('c');prnt_char('h');prnt_char('e');prnt_char('c');prnt_char('k');prnt_char(' ');
		prnt_char('b');prnt_char('u');prnt_char('c');prnt_char('k');prnt_char('e');prnt_char('t');prnt_char(CR);
		prnt_int(b);prnt_char(CR);
#endif
		for (rv=my_freelist[b]; rv; prev=&(rv->next), rv=rv->next)
			if (rv->size >= size){
#ifdef DEBUG_ACT
				prnt_char('f');prnt_char('o');prnt_char('u');prnt_char('n');prnt_char('d');prnt_char(' ');
				prnt_char('s');prnt_char('i');prnt_char('z');prnt_char('e');prnt_char(CR);
				prnt_int(rv->size);prnt_char('-');
				prnt_int((unsigned int)rv);prnt_char(CR);
#endif
				*prev = rv->next;
				if (rv->size >= size+MIN_SAVE_EXTRA){
#ifdef DEBUG_ACT
#endif
					if (my_slop){
						b = my_b2bucket(my_slop);
#ifdef DEBUG_ACT
#endif
						my_slop->next = my_freelist[b];
						my_freelist[b] = my_slop;
					}
					my_slop = my_split_block(rv, size);
#ifdef DEBUG_ACT
					prnt_char('s');prnt_char('l');prnt_char('o');prnt_char('p');prnt_char(' ');
					prnt_char('s');prnt_char('i');prnt_char('z');prnt_char('e');prnt_char(CR);
					prnt_int(my_slop->size);prnt_char('-');
					prnt_int((unsigned int)my_slop);prnt_char(CR);
#endif
				}
				RET(rv);
			}
			b++;
	}

	chunk_size = size+16; /* two ends plus two placeholders */
	rv = (BLOCK *)my_sbrk(chunk_size);
	if (rv == (BLOCK *)(-1))
		return 0;
#ifdef DEBUG_ACT
	prnt_char('s');prnt_char('b');prnt_char('r');prnt_char('k');prnt_char(':');
	prnt_int(chunk_size);prnt_char('-');
	prnt_int((unsigned int)rv);prnt_char('-');
	prnt_int((unsigned int)expected_sbrk);prnt_char(CR);
#endif
	if (rv == expected_sbrk){
		expected_sbrk = (BLOCK *)((char *)rv + chunk_size);
		/* absorb old end-block-marker */
#ifdef DEBUG_ACT
		prnt_char('g');prnt_char('o');prnt_char('t');prnt_char(' ');
		prnt_char('e'); prnt_char('x');prnt_char('p');prnt_char(' ');
		prnt_char('s');prnt_char('b');prnt_char('r');prnt_char('k');prnt_char(CR);
#endif
		rv = (BLOCK *)((char *)rv - PTRSZ);
	}
	else
	{
		expected_sbrk = (BLOCK *)((char *)rv + chunk_size);
#ifdef DEBUG_ACT
		prnt_char('d');prnt_char('i');prnt_char('s');prnt_char('c');prnt_char('o');prnt_char('n');prnt_char('n');prnt_char('e');prnt_char('n');prnt_char('n');prnt_char('e');
		prnt_char('s');prnt_char('b');prnt_char('r');prnt_char('k');prnt_char(CR);
#endif
		/* build start-block-marker */
		rv->size = 1;
		rv = (BLOCK *)((char *)rv + PTRSZ);
		chunk_size -= (2*PTRSZ);
	}
	rv->size = chunk_size - (2*PTRSZ);
	ENDSZ(rv) = rv->size;
	AFTER(rv)->size = 1;
	CHECK(rv);
	RET(rv);
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
// ???
//static inline BLOCK *my_merge(BLOCK *a, BLOCK *b, BLOCK *c){
BLOCK *my_merge(BLOCK *a, BLOCK *b, BLOCK *c){

#if 0

	"a" block should be placed before "b" block.
	"c" is the block which has already been free.

    a=c=BEFORE(bp)		    b=bp
	^ÄÄÄÄÂÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄ^ÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÂÄÄÄÄ¿
	³  1 ³size³		 ³size³size³		  ³size³  1 ³
	³    ³	  ³   data	 ³    ³    ³   data	  ³    ³    ³
	³    ³	  ³		 ³    ³    ³		  ³    ³    ³
	ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ

	   a=bp 		  b=c=AFTER(bp)
	ÚÄÄÄÄ^ÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄ^ÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÂÄÄÄÄ¿
	³  1 ³size³		 ³size³size³		  ³size³  1 ³
	³    ³	  ³   data	 ³    ³    ³   data	  ³    ³    ³
	³    ³	  ³		 ³    ³    ³		  ³    ³    ³
	ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ

#endif // 0

	int bu;
	BLOCK *bp, **bpp;
#ifdef DEBUG_ACT
	prnt_char('m');prnt_char('e');prnt_char('r');prnt_char('g');prnt_char('e');prnt_char(CR);
	prnt_int(a->size);prnt_char('-');
	prnt_int((unsigned int)a);prnt_char('-');
	prnt_int(b->size);prnt_char('-');
	prnt_int((unsigned int)b);prnt_char(CR);
	if((unsigned int)a == (unsigned int)c){
	    if(a->size > (((unsigned int)b - (unsigned int)a)+20)){
		BYTE *d=(BYTE *)a;
//		BLOCK *e=BEFORE(a);;
		BYTE *e=(BYTE *)((unsigned int)a - 1000);
		while((unsigned int)d != (unsigned int)b){
			prnt_byte(*d);prnt_char('-');
			d++;
		}
		prnt_char(CR);
		d=(BYTE *)e;
		while((unsigned int)d != (unsigned int)a){
			prnt_byte(*d);prnt_char('-');
			d++;
		}
		prnt_char(CR);
	    }
	}
#endif
	CHECK(a);
	CHECK(b);
	CHECK(c);

	if (c == my_slop){
#ifdef DEBUG_ACT
		prnt_char('s');prnt_char('n');prnt_char('i');prnt_char('p');prnt_char('-');
		prnt_char('s');prnt_char('l');prnt_char('o');prnt_char('p');prnt_char(CR);
		prnt_int(my_slop->size);prnt_char('-');
		prnt_int((unsigned int)my_slop);prnt_char(CR);
#endif
		my_slop = 0;
	}
	bu = my_b2bucket(c);
#ifdef DEBUG_ACT
	prnt_char('b');prnt_char('u');prnt_char('c');prnt_char('k');prnt_char('e');prnt_char('t');prnt_char(CR);
	prnt_int(c->size);prnt_char('-');
	prnt_int((unsigned int)c);prnt_char('-');
	prnt_int(bu);prnt_char(CR);
#endif
	bpp = my_freelist+bu;
	for (bp=my_freelist[bu]; bp; bpp=&(bp->next), bp=bp->next){
#ifdef DEBUG_ACT
		prnt_int((unsigned int)bp);prnt_char(CR);
#endif
		if (bp == c){ // "c" is removed from its my_freelist
#ifdef DEBUG_ACT
			prnt_char('s');prnt_char('n');prnt_char('i');prnt_char('p');prnt_char(CR);
			prnt_int(bp->size);prnt_char('-');
			prnt_int((unsigned int)bp);prnt_char('-');
			prnt_int(bu);prnt_char(CR);
#endif
			*bpp = bp->next;
			break;
		}
	}
	CHECK(c);

	a->size += b->size + (2*PTRSZ);
	a->bucket = -1;
	ENDSZ(a) = a->size;

	CHECK(a);
	return a;
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void my_free(void *ptr){
	int b;
	BLOCK *block;
	if (ptr == 0)
		return;
	block = (BLOCK *)((char *)ptr-PTRSZ);

#if NUMSMALL
	if (block->size < SMALL){
		block->next = my_smallblocks[block->size/ALIGN];
		my_smallblocks[block->size/ALIGN] = block;
		return;
	}
#endif

	block->size &= ~1;
	ENDSZ(block) &= ~1;
	block->bucket = -1;
#ifdef DEBUG_ACT
	prnt_char('f');prnt_char('r');prnt_char('e');prnt_char('e');prnt_char(CR);
	prnt_int(block->size);prnt_char('-');
	prnt_int((unsigned int)block);prnt_char(CR);
#endif

	CHECK(block);
	if (! (AFTER(block)->size & 1)){
		CHECK(AFTER(block));
	}
	if (! (BEFSZ(block) & 1)){
		CHECK(BEFORE(block));
		block = my_merge(BEFORE(block), block, BEFORE(block));
	}
	CHECK(block);
	if (! (AFTER(block)->size & 1)){
		CHECK(AFTER(block));
		block = my_merge(block, AFTER(block), AFTER(block));
	}
	CHECK(block);

	b = my_b2bucket(block);
	block->next = my_freelist[b];
	my_freelist[b] = block;
	CHECK(block);
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void *my_realloc(void *ptr, KT_SIZE_T size){

	BLOCK *b;
	char *newptr;
	KT_SIZE_T copysize;

	if (ptr == 0)
		return(my_malloc(size));
	b = (BLOCK *)((char *)ptr-PTRSZ);
	copysize = b->size & ~1;
	if (size <= copysize){
#if 0
		if (copysize < 2*MIN_SAVE_EXTRA || (size >= copysize-512 && size >= copysize/2))
#endif //0
			return ptr;
// warning :unreachable code,It is commented to prevent warning.
#if 0
		copysize = size;
#endif //0
	}
	newptr = (char *)my_malloc(size);
#ifdef DEBUG_ACT
	prnt_char('r');prnt_char('e');prnt_char('a');prnt_char('l');prnt_char('l');prnt_char('o');prnt_char('c');prnt_char(CR);
	prnt_int(size);prnt_char('-');
	prnt_int(b->size);prnt_char('-');
	prnt_int((unsigned int)b);prnt_char('-');
	prnt_int((unsigned int)ptr);prnt_char('-');
	prnt_int((unsigned int)newptr);prnt_char('-');
	prnt_int(copysize);prnt_char(CR);
#endif
// warning : Nonportable pointer conversion,It is commented to prevent warning.
	if (newptr == (char *)NULL)
		return (void *)NULL;
	memcpy(newptr, ptr, copysize);
	my_free(ptr);
	return newptr;
}
#endif // KIATEL_DYNAMIC_MEM
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void my_assert(int a){
	if(a == 0){
		prnt_char('a');prnt_char('s');prnt_char('s');prnt_char('e');prnt_char('r');prnt_char('t');
		prnt_char(' ');
		prnt_char('f');prnt_char('a');prnt_char('i');prnt_char('l');prnt_char('e');prnt_char('d');
		prnt_char(':');
		prnt_int(a);
		prnt_char(CR);
#ifdef SEND_LF
		prnt_char(LF);
#endif
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef FIFO_RD_WR
void long_2_prnt(unsigned long w2p){
	prnt_int((int)(w2p/0x10000L));
	prnt_int((int)(w2p%0x10000L));
}
#endif
#ifdef KIATEL_DYNAMIC_MEM
void dump_all_memory(void){
	unsigned int i,j,k;
#ifdef SYSTEM_SIDE
#if ((defined EC2K_SYSTEM) || (defined EC1K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PU2K_SYSTEM) || (defined MGW672_SYSTEM))
	disable_wdt();
#endif
#endif
	long2prnt((unsigned long)&dynamic_mem_arr[0]);
	prnt_char(CR);
	k=0;j=0;
	while(k<MAX_DYNAMIC_MEM_SIZE){
		prnt_int(j);prnt_char(':');
		for(i=0;(i<16) && (k<MAX_DYNAMIC_MEM_SIZE);i++,k++){
			if(i==8)
				prnt_char('-');
			else
				prnt_char(' ');
			prnt_byte(dynamic_mem_arr[k]);
		}
		j+=0x10;
		prnt_char(CR);
	}
#ifdef SYSTEM_SIDE
#if ((defined EC2K_SYSTEM) || (defined EC1K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PU2K_SYSTEM) || (defined MGW672_SYSTEM))
	enable_wdt();
#endif
#endif
}
void dump_memory(void){
	unsigned int i;BLOCK *r;
#ifdef SYSTEM_SIDE
#if ((defined EC2K_SYSTEM) || (defined EC1K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PU2K_SYSTEM) || (defined MGW672_SYSTEM))
	disable_wdt();
#endif
#endif
	long2prnt((unsigned long)curr_mem_indx);prnt_char(CR);
	long2prnt((unsigned long)my_slop);prnt_char(CR);
	r=my_slop;	// my_slop->next is not valid
	if(r != 0){
		prnt_int(r->size);prnt_char('-');
		prnt_int(r->bucket);prnt_char('-');
		r=r->next;
		long2prnt((unsigned long)r);prnt_char(CR);
	}
	for(i=0;i<30;i++){
		prnt_byte(i);prnt_char(':');
		long2prnt((unsigned long)my_freelist[i]);prnt_char(CR);
		r=my_freelist[i];
		while(r != 0){
			prnt_int(r->size);prnt_char('-');
			prnt_int(r->bucket);prnt_char('-');
			r=r->next;
			long2prnt((unsigned long)r);prnt_char(CR);
		}
	}
#ifdef SYSTEM_SIDE
#if ((defined EC2K_SYSTEM) || (defined EC1K_SYSTEM) || (defined EC10K_SYSTEM) || (defined PU2K_SYSTEM) || (defined MGW672_SYSTEM))
	enable_wdt();
#endif
#endif
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void print_pointer(gpointer mem){
	unsigned int mr1;
	unsigned char *ptr;
	BLOCK *bp;
	prnt_char('m');prnt_char('e');prnt_char('m');prnt_char('=');
	long2prnt((unsigned long)mem);prnt_char(CR);
	if(mem == 0) return;
	mr1=*((unsigned int *)(mem-4));
	prnt_char('m');prnt_char('r');prnt_char('1');prnt_char('=');
	prnt_int(mr1);prnt_char(CR);
	if(mr1 >= NO_MEM_REC) return;
	prnt_char('s');prnt_char('t');prnt_char('a');prnt_char('t');prnt_char('e');prnt_char('=');
	prnt_byte(mem_rec[mr1].state);
	prnt_char('s');prnt_char('i');prnt_char('z');prnt_char('e');prnt_char('=');
	prnt_int(mem_rec[mr1].size);
	ptr=mem_rec[mr1].parm;
	prnt_char('p');prnt_char('a');prnt_char('r');prnt_char('m');prnt_char('=');
	long2prnt((unsigned long)ptr);prnt_char(CR);
	if (ptr == 0) return;
	bp = (BLOCK *)((char *)ptr-PTRSZ);
	prnt_char('B');prnt_char('E');prnt_char('F');prnt_char('O');prnt_char('R');prnt_char('E');prnt_char('=');
	long2prnt(BEFORE(bp)->size);
	prnt_char('B');prnt_char('E');prnt_char('F');prnt_char('S');prnt_char('Z');prnt_char('=');
	long2prnt(BEFSZ(bp));
	prnt_char('S');prnt_char('I');prnt_char('Z');prnt_char('E');prnt_char('=');
	long2prnt(bp->size);
	prnt_char('D');prnt_char('A');prnt_char('T');prnt_char('A');prnt_char('=');
	prnt_byte(*DATA(bp));
	prnt_char('E');prnt_char('N');prnt_char('D');prnt_char('S');prnt_char('Z');prnt_char('=');
	long2prnt(ENDSZ(bp));
	prnt_char('A');prnt_char('F');prnt_char('T');prnt_char('E');prnt_char('R');prnt_char('=');
	long2prnt(AFTER(bp)->size);
	prnt_char(CR);
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if ((defined MALLOC_AUDIT) && (defined SYSTEM_SIDE))
void malloc_audit(void){
	guint mr1;
	MEM_REC_ST *mem_pt;

	for(mr1=0,mem_pt=&mem_rec[0];mr1 < NO_MEM_REC;mr1++,mem_pt++){
	    if(mem_pt->state != IN_QUE){
#ifdef MALLOC_DEBUG
		guchar *parm=mem_rec[mr1].parm-4;
#else
		guchar *parm=mem_rec[mr1].parm;
#endif
		BLOCK *block=(BLOCK *)(parm-PTRSZ);
		guint size=block->size & (~1);
		guint endsz=(*(guint *)((guchar *)block + size + PTRSZ)) & (~1);
		if(size != endsz){
		    ws2p("mfailaudit:\r\n");
		    print_mr(mr1);
#ifndef DO_NOT_BULK_SAVE_ADDR
		    print_addr(mem_rec[mr1].addr);
#endif
		    int2prnt(size);toprnq('-');
		    int2prnt(endsz);toprnq(CR);
		    (*(guint *)((guchar *)block + size + PTRSZ))=size | 1;
		}

	    }
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */

#endif // USE_OS_MALLOC
