
//#define DEBUG_ACT
#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else  // CENTRAL_EXTERN

#include "hdr.h"
#include <stdlib.h>
#include <string.h>

extern char *mess[];
#ifdef KIATEL_DYNAMIC_MEM
extern MEM_REC_ST mem_rec[];
extern unsigned int eq_mem;
extern unsigned int fq_mem;
extern BYTE dump_memory_done;
#endif

#ifdef SPEED_DIALLING
extern SP_NODE_STRUCT *first_sp_node_pt;
#endif

#endif // CENTRAL_EXTERN
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void mem_cpy(BYTE *dst,BYTE *src,unsigned int size){
	memcpy(dst,src,size);
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
int mem_cmp(BYTE *dst,BYTE *src,unsigned int size){
	return(memcmp(dst,src,size));
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
BYTE *mem_req(unsigned int size){
	return((BYTE *)my_malloc(size));
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void mem_rls(BYTE *parm){
	if (parm != MEM_NULL_PARAM){
		my_free(parm);
	}
	else{
		prnt_msg(ERROR,0xa000);
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void init_mem_rec(void){

	unsigned int i;
	MEM_REC_ST *mem_pt;

	for(i=0,mem_pt=&mem_rec[0];i < NO_MEM_REC;i++,mem_pt++){
		mem_pt->state=IN_QUE;
		mem_pt->size=0;
		mem_pt->rp=i+1;
		mem_pt->lp=i-1;
#if ((!defined DO_NOT_BULK_SAVE_ADDR) || (defined SAVE_ADDR_FOR_LARGE_MEM))
#if ((!defined MEM_REC_DOES_NOT_HAVE_BACKTRACK) && (COMPILER == GCC))
		mem_pt->addr[0]=0;
#endif
#endif
	}
	fq_mem=0;
	eq_mem=NO_MEM_REC-1;
	mem_rec[NO_MEM_REC-1].rp=(WORD_MR)(-1); //0xffff;
	mem_rec[0].lp=(WORD_MR)(-1); //0xffff;
#ifdef DO_NOT_PRINT_MEMORY_DUMP
	dump_memory_done=SET;
#else
	dump_memory_done=RESET;
#endif
	return;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void dump_mem_rec(void){
	unsigned int i;
	MEM_REC_ST *mem_pt;
#if (NO_MEM_REC > 65536)
	prnt_long(fq_mem);prnt_char('-');
	prnt_long(eq_mem);prnt_char(CR);
#else
	prnt_int(fq_mem);prnt_char('-');
	prnt_int(eq_mem);prnt_char(CR);
#endif
	for(i=0,mem_pt=&mem_rec[0];i < NO_MEM_REC;i++,mem_pt++){
#ifdef DO_NOT_DUMP_REMAIN_FOR_A_WHILE_MR1
	    if(mem_pt->state == REMAIN_FOR_A_WHILE_STATE){
		continue;
	    }
#endif
	    if(mem_pt->state == IN_QUE){
		continue;
	    }
#if ((defined MEMORY_DEBUG) && (defined SYSTEM_SIDE))
	    if(mem_pt->initialization_level == 0){
		continue;
	    }
#endif
#if (NO_MEM_REC > 65536)
	    prnt_long(i);prnt_char(':');
#else
	    prnt_int(i);prnt_char(':');
#endif
	    prnt_byte(mem_pt->state);prnt_char('-');
	    prnt_int(mem_pt->size);prnt_char('-');
#if (NO_MEM_REC > 65536)
	    prnt_long(mem_pt->lp);prnt_char('-');
	    prnt_long(mem_pt->rp);prnt_char('-');
#else
	    prnt_int(mem_pt->lp);prnt_char('-');
	    prnt_int(mem_pt->rp);prnt_char('-');
#endif
#ifndef DO_NOT_BULK_SAVE_ADDR
#ifndef MEM_REC_DOES_NOT_HAVE_BACKTRACK
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
	    print_addr(mem_pt->addr);
#endif
#endif
#endif
#endif
	    {
		unsigned int j;
		prnt_int(mem_pt->size);prnt_char(':');
		for(j=0;j<mem_pt->size;j++){
		    prnt_byte(mem_pt->parm[j]);prnt_char('-');
		}
		prnt_char(CR);
	    }
	}
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
#if (COMPILER == GCC)
unsigned int mem_rec_req(unsigned int size) __attribute__ ((always_inline));
#endif
unsigned int mem_rec_req(unsigned int size){

	unsigned int i;
	BYTE *parm;

#ifdef MALLOC_DEBUG
	parm=(BYTE *)my_malloc(size+4);
#else
	parm=(BYTE *)my_malloc(size);
#endif
	if(parm == MEM_NULL_PARAM){
		prnt_msg(ERROR,0xa001);
		if(dump_memory_done == RESET){
			dump_memory_done=SET;
			prnt_int(size);prnt_char(CR);
			print_call_frame();
			dump_mem_rec();
#ifndef USE_OS_MALLOC
			dump_memory();
			dump_all_memory();
#endif
		}
#ifdef PC_SYSTEM
		save_debug_file();
#endif
		return (WORD_MR)(-1); //0xffff;
	}
	i=find_free_mem_rec();
	if (i >= NO_MEM_REC){
#ifdef PC_SYSTEM
		save_debug_file();
#endif // PC_SYSTEM
		my_free(parm);
		prnt_msg(ERROR,0xa002);
		return (WORD_MR)(-1); //0xffff;
	}
#ifdef MALLOC_DEBUG
	*((unsigned int *)(parm))=i;
	mem_rec[i].parm=parm+4;
#else
	mem_rec[i].parm=parm;
#endif
	mem_rec[i].state=OUT_OF_QUE;
	mem_rec[i].size=size;
#if ((defined MEMORY_DEBUG) && (defined SYSTEM_SIDE))
	mem_rec[i].initialization_level=initialization_level;
#endif

#ifndef DO_NOT_BULK_SAVE_ADDR
#ifndef MEM_REC_DOES_NOT_HAVE_BACKTRACK
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
	save_addr(mem_rec[i].addr);
#endif
#endif
#endif
#elif (defined SAVE_ADDR_FOR_LARGE_MEM)
#ifndef MEM_REC_DOES_NOT_HAVE_BACKTRACK
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
	if(mem_rec[i].size > 0xff){
	    save_addr(mem_rec[i].addr);
	}
	else{
	    mem_rec[i].addr[0]=0;
	}
#endif
#endif
#endif
#endif


	return i;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
unsigned int find_free_mem_rec(void){

	unsigned int a;
	MEM_REC_ST *mem_pt;

	if(fq_mem >= NO_MEM_REC){
	    prnt_msg(ERROR,0xa006);
	    if(dump_memory_done == RESET){
		dump_memory_done=SET;
#ifdef SYSTEM_SIDE
		dump_used_mem_rec(1);
#else
		dump_mem_rec();
#endif
	    }
	    fq_mem=eq_mem=NULL_MEM_REC;
	    for(a=0,mem_pt=&mem_rec[0];a < NO_MEM_REC;a++,mem_pt++){
		if(mem_pt->state == IN_QUE){
		    if(fq_mem < NULL_MEM_REC){
			mem_rec[fq_mem].lp=a;
		    }
		    else{
			eq_mem=a;
		    }
		    mem_pt->rp=fq_mem;
		    mem_pt->lp=NULL_MEM_REC;
		    fq_mem=a;
/*
		    if(eq_mem < NO_MEM_REC){
			mem_rec[eq_mem].rp=a;
		    }
		    else{
			fq_mem=a;
		    }
		    mem_pt->lp=eq_mem;
		    mem_pt->rp=NULL_MEM_REC;
		    eq_mem=a;
*/
		}
	    }
	    if(fq_mem >= NO_MEM_REC){
		return (WORD_MR)(-1); //0xffff;
	    }
	}
	a=fq_mem;
	mem_pt=&mem_rec[a];
	fq_mem=mem_pt->rp;
	if(fq_mem < NO_MEM_REC)
		mem_rec[fq_mem].lp=(WORD_MR)(-1); //0xffff;
	else
		eq_mem=(WORD_MR)(-1); //0xffff;
	mem_pt->rp=(WORD_MR)(-1); //0xffff;
	return a;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void mem_rec_rls(unsigned int i){

	MEM_REC_ST *mem_pt;
	if(i >= NO_MEM_REC){
		prnt_msg(ERROR,0xa007);
		prnt_long(i);prnt_char(CR);
		print_call_frame();
		return;
	}
	mem_pt=&mem_rec[i];
	if((mem_pt->state != OUT_OF_QUE)
#ifdef DO_NOT_DUMP_REMAIN_FOR_A_WHILE_MR1
	   && (mem_pt->state != REMAIN_FOR_A_WHILE_STATE)
#endif
	){
		prnt_msg(ERROR,0xa008);
		prnt_int(i);prnt_char(CR);
#ifndef DO_NOT_BULK_SAVE_ADDR
#ifndef MEM_REC_DOES_NOT_HAVE_BACKTRACK
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
		print_addr(mem_pt->addr);
#endif
#endif
#endif
#endif
		print_call_frame();
		return;
	}
	if (mem_pt->parm != MEM_NULL_PARAM){
#ifdef MALLOC_DEBUG
		my_free(mem_rec[i].parm-4);
#else
		my_free(mem_rec[i].parm);
#endif
	}
	else{
		prnt_msg(ERROR,0xa004);
	}
	mem_pt->state=IN_QUE;
	mem_pt->lp=eq_mem;
	if(eq_mem < NO_MEM_REC)
		mem_rec[eq_mem].rp=i;
	else
		fq_mem=i;
	eq_mem=i;
	mem_pt->rp=(WORD_MR)(-1); //0xffff;
#ifndef DO_NOT_BULK_SAVE_ADDR
#ifndef MEM_REC_DOES_NOT_HAVE_BACKTRACK
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
	save_addr(mem_pt->addr);
#endif
#endif
#endif
#endif
	return;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef FIFO_RD_WR
unsigned int check_size_of_next_packet_for_read_buffer(BUF *buf){
	if (buf->front == buf->rear) return 0; /*rx_buf is empty*/
	return buf->arr[buf->front];
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef FIFO_RD_WR
BYTE read_buffer(BUF *buf,unsigned int *size,BYTE *dat){

	unsigned int *fq;
	BYTE *arr;
	unsigned int i;

	fq=&(buf->front);
	arr=&(buf->arr[0]);

	if(*fq == buf->rear){
		return FAIL;
	}
/*
	if(*fq >= buf->size){
	    prnt_msg(ERROR,0xa003);
	    prnt_long((long)buf);prnt_char('-');
	    prnt_int(*fq);prnt_char('-');
	    prnt_int(buf->size);prnt_char('-');
	    prnt_int(buf->rear);prnt_char(CR);
	    print_call_frame();
	    return FAIL;
	}
*/
	*size=arr[*fq];
	(*fq)++;
	if (*fq == buf->size){
		*fq=0;
	}

	for(i=0;i<*size;i++){
		if(*fq == buf->rear){
			return FAIL;
		}
		dat[i]=arr[*fq];
		(*fq)++;
		if (*fq == buf->size){
			*fq=0;
		}
	}
	return OK;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef FIFO_RD_WR
BYTE read_buffer_only_data(BUF *buf,unsigned int size,BYTE *dat){

	unsigned int *fq;
	BYTE *arr;
	unsigned int i;

	fq=&(buf->front);
	arr=&(buf->arr[0]);

	if(*fq == buf->rear){
		return FAIL;
	}
	for(i=0;i<size;i++){
		if(*fq == buf->rear){
			return FAIL;
		}
		dat[i]=arr[*fq];
		(*fq)++;
		if (*fq == buf->size){
			*fq=0;
		}
	}
	return OK;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef FIFO_RD_WR
BYTE write_buffer(BUF *buf,unsigned int size,BYTE *dat){

	int free_size;
	unsigned int *eq;
	unsigned int i;
	BYTE *arr;

	eq=&(buf->rear);
	arr=&(buf->arr[0]);

	free_size=buf->front-(*eq);
	if (free_size <= 0) free_size+=buf->size;
	if (free_size <= (size+1)){ // one byte is never used in fifo.
		prnt_msg(BUF_OVERFLOW,0x1301);
		prnt_int(free_size);prnt_char('-');
		prnt_int(size);prnt_char('-');
		prnt_int(*eq);prnt_char('-');
		prnt_int(buf->rear);prnt_char('-');
		prnt_int(buf->front);prnt_char('-');
		prnt_int(buf->size);prnt_char(CR);
#ifdef SYSTEM_SIDE
		print_call_frame();
#endif
		return FAIL;
	}
//?????? only lsb is written
	arr[*eq]=size;
	(*eq)++;
	if (*eq == buf->size)
		*eq=0;

	for(i=0;i<size;i++){
		arr[*eq]=dat[i];
		(*eq)++;
		if (*eq == buf->size)
			*eq=0;
	}
	return OK;
}
#endif

/* ======================================================================== */
/* ======================================================================== */
#ifdef FIFO_RD_WR
BYTE write_buffer_only_data(BUF *buf,unsigned int size,BYTE *dat){

	int free_size;
	unsigned int *eq;
	unsigned int i;
	BYTE *arr;

	eq=&(buf->rear);
	arr=&(buf->arr[0]);

	free_size=buf->front-(*eq);
	if (free_size <= 0) free_size+=buf->size;
	if (free_size < (size+1)){ // one byte is never used in fifo.
		prnt_msg(BUF_OVERFLOW,0x1302);
		prnt_int(free_size);prnt_char('-');
		prnt_int(size);prnt_char('-');
		prnt_int(*eq);prnt_char('-');
		prnt_int(buf->rear);prnt_char('-');
		prnt_int(buf->front);prnt_char('-');
		prnt_int(buf->size);prnt_char(CR);
		return FAIL;
	}
	for(i=0;i<size;i++){
		arr[*eq]=dat[i];
		(*eq)++;
		if (*eq == buf->size)
			*eq=0;
	}
	return OK;
}
#endif
/* ======================================================================== */
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE check_if_data_exist(BUF *buf){

	unsigned int *fq;

	fq=&(buf->front);

	if(*fq == buf->rear){
		return FAIL;
	}
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE put_buf(BUF *buf,BYTE dat){

	unsigned int temp_rear;

	if (buf->rear == buf->size -1)
		temp_rear=0;
	else
		temp_rear = buf->rear + 1;

	if (buf->front == temp_rear){
		prnt_msg(BUF_OVERFLOW,0x4028);
		return FAIL;  /* tx_buf is full*/
	}
	else
		buf->rear=temp_rear;

// new block start
// new strategy,data is overwritten

#if 0
	if (buf->front == temp_rear){
		prnt_msg(LAPB_ERROR,0x4028);
		//suppose that one byte of data is read.
		if (buf->front == buf->size -1)
			buf->front=0;
		else
			buf->front++ ;
		return OK;  /* tx_buf is full*/
	}
	buf->rear=temp_rear;

#endif // 0
// new block end
	buf->arr[buf->rear]=dat;
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE get_buf(BUF *buf,BYTE *dat){

	if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
	if (buf->front == buf->size -1)
		buf->front=0;
	else
		buf->front++ ;
	*dat=buf->arr[buf->front];
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE read_buf(BUF *buf,BYTE *dat){

	unsigned int temp;
	if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
	if (buf->front == buf->size -1)
		temp=0;
	else
		temp=buf->front+1 ;
	*dat=buf->arr[temp];
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
unsigned int check_size_of_next_packet(BUF *buf){
	unsigned int temp;
	if (buf->front == buf->rear) return 0; /*rx_buf is empty*/
	if (buf->front == buf->size -1)
		temp=0;
	else
		temp=buf->front+1 ;
	return buf->arr[temp];
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
void dump_buf(BUF *buf){
	unsigned int temp,i;
	temp=buf->front;
#ifdef LAPB
	prnt_byte(buf->arr[temp]);prnt_char('-');
#else
	prnt_byte(buf->arr[temp]);prnt_char('-');
#endif
	for(i=0;i<10;i++){
		if (temp == buf->rear) {
#ifdef LAPB
			prnt_char('E');
#else
			prnt_char('E');
#endif
			goto ex1;
		}
		if (temp == buf->size -1)
			temp=0;
		else
			temp++;
#ifdef LAPB
		prnt_byte(buf->arr[temp]);prnt_char('-');
#else
		prnt_byte(buf->arr[temp]);prnt_char('-');
#endif
	}
ex1:
#ifdef LAPB
	prnt_char(CR);
#ifdef SEND_LF
	prnt_char(LF);
#endif
#else
	prnt_char(CR);
#ifdef SEND_LF
	prnt_char(LF);
#endif
#endif
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
unsigned int checkfree_buf(BUF *buf){

	if ( buf->rear >= buf->front)
		return (buf->size - 1 - buf->rear + buf->front);
	else
		return ( buf->front - buf->rear - 1 );
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE put_pckt_with_two_bytes_size_to_buf(BUF *buf,unsigned int size,BYTE *dat){

	unsigned int i,free_size;

	if ( buf->rear >= buf->front){
		free_size=buf->size - 1 - buf->rear + buf->front;
	}
	else{
		free_size=buf->front - buf->rear - 1 ;
	}
	if(free_size < (size+2)) return FAIL; // 2 more bytes for size

	buf->rear=((buf->rear == buf->size -1) ? 0 : buf->rear + 1 ) ;
	buf->arr[buf->rear]=(BYTE)size;

	buf->rear=((buf->rear == buf->size -1) ? 0 : buf->rear + 1 ) ;
	buf->arr[buf->rear]=(size>>8)|FIFO_2_BYTES_FOR_SIZE_POS;

	for(i=0;i<size;i++){
		buf->rear=((buf->rear == buf->size -1) ? 0 : buf->rear + 1 ) ;
		buf->arr[buf->rear]=dat[i];
	}
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE put_pckt_with_one_byte_size_to_buf(BUF *buf,BYTE size,BYTE *dat){

	unsigned int i,free_size;

	if ( buf->rear >= buf->front){
		free_size=buf->size - 1 - buf->rear + buf->front;
	}
	else{
		free_size=buf->front - buf->rear - 1 ;
	}
	if(free_size < (size+1)) return FAIL; // 1 more bytes for size

	buf->rear=((buf->rear == buf->size -1) ? 0 : buf->rear + 1 ) ;
	buf->arr[buf->rear]=(BYTE)size;

	for(i=0;i<size;i++){
		buf->rear=((buf->rear == buf->size -1) ? 0 : buf->rear + 1 ) ;
		buf->arr[buf->rear]=dat[i];
	}
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE get_pckt_with_two_bytes_size_from_buf(BUF *buf,unsigned int *size,BYTE *dat){

	unsigned int i;
	BYTE size_lsb,size_msb;

	if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
	buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
	size_lsb=buf->arr[buf->front];

	if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
	buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
	size_msb=buf->arr[buf->front] & (~FIFO_2_BYTES_FOR_SIZE_POS);

	*size = size_lsb | (size_msb << 8);
	for(i=0;i<*size;i++){
		if (buf->front == buf->rear) return FAIL; /* rx_buf is empty */
		buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
		dat[i]=buf->arr[buf->front];
	}
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE get_this_size_of_data_from_buf(BUF *buf,unsigned int size,BYTE *dat){

	unsigned int i;

	for(i=0;i<size;i++){
		if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
		buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
		dat[i]=buf->arr[buf->front];
	}
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE get_pckt_with_one_byte_size_from_buf(BUF *buf,BYTE *size,BYTE *dat){

	BYTE i;

	if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
	buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
	*size=buf->arr[buf->front];

	for(i=0;i<*size;i++){
		if (buf->front == buf->rear) return FAIL; /* rx_buf is empty */
		buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
		dat[i]=buf->arr[buf->front];
	}
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE write_and_check_size_buffer(BUF *buf,unsigned int size,BYTE *dat){

	int free_size;
	unsigned int *eq;
	unsigned int i;
	BYTE *arr;
	unsigned int needed_size;

	eq=&(buf->rear);
	arr=&(buf->arr[0]);

	free_size=buf->front-(*eq);
	if (free_size <= 0) free_size+=buf->size;
	if(size < 0x80)
		needed_size=size+1+1; // 1 for size itself, one byte is never used in fifo.
	else
		needed_size=size+1+1+1; // 1 extra byte for size
	if (free_size < needed_size){
		prnt_msg(BUF_OVERFLOW,0x1303);
		prnt_int(free_size);prnt_char('-');
		prnt_int(size);prnt_char('-');
		prnt_int(*eq);prnt_char('-');
		prnt_int(buf->rear);prnt_char('-');
		prnt_int(buf->front);prnt_char('-');
		prnt_int(buf->size);prnt_char(CR);
#ifdef SYSTEM_SIDE
		print_call_frame();
#endif
		return FAIL;
	}
	if(size < 0x80){
		arr[*eq]=size;
		(*eq)++;
		if (*eq == buf->size)
			*eq=0;
	}
	else{
		arr[*eq]=0x80 | ((size & 0x7f00) >>8);
		(*eq)++;
		if (*eq == buf->size)
			*eq=0;
		arr[*eq]=size;
		(*eq)++;
		if (*eq == buf->size)
			*eq=0;
	}

	for(i=0;i<size;i++){
		arr[*eq]=dat[i];
		(*eq)++;
		if (*eq == buf->size)
			*eq=0;
	}
	return OK;
}
#endif

/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE read_and_check_size_buffer(BUF *buf,unsigned int *size,BYTE *dat,unsigned int max_dat_size){

	unsigned int *fq;
	BYTE *arr,temp;
	unsigned int i,j;
	unsigned int packet_size;

	fq=&(buf->front);
	arr=&(buf->arr[0]);

	if(*fq == buf->rear){
		return FAIL;
	}
	packet_size=arr[*fq];
	(*fq)++;
	if (*fq == buf->size){
		*fq=0;
	}
	if(packet_size & 0x80){
		if(*fq == buf->rear){
			return FAIL;
		}
		temp=arr[*fq];
		(*fq)++;
		if (*fq == buf->size){
			*fq=0;
		}
		packet_size=((packet_size & 0x7f) << 8) | (temp);
	}
	*size=packet_size;
	for(i=0,j=0;i<packet_size;i++){
		if(*fq == buf->rear){
			return FAIL;
		}
		if(j < max_dat_size){
			dat[j]=arr[*fq];
			j++;
		}
		(*fq)++;
		if (*fq == buf->size){
			*fq=0;
		}
	}
	if(packet_size > max_dat_size)
		return FAIL;
	return OK;
}
#endif

/* ======================================================================*/
/* ======================================================================*/
// read_buffer/read_buffer_only_data/write_buffer/
// write_buffer_only_data/check_if_data_exist
// used together in ss7/lapx/isdn/gsm/v5

// put_buf/get_buf/read_buf/check_size_of_next_packet
// checkfree_buf
// used together in lapb,alrm_pnl,htbyscn.

// put_pckt_to_buf is going to be used by htbyss7 (put_buf/get_buf group)
// get_pckt_from_buf is not used, probably is going to be used in htbyss7 (put_buf/get_buf group)

// get_this_size_of_data_from_buf is used with the put_buf/get_buf group

// write_and_check_size_buffer/read_and_check_size_buffer is for the
// read_buffer/write_buffer group. they handle sizes more than 256 and
// should be used together. the read_and_check... also checks not to overwrite
// the array passed to it. read_buffer should be changed to do the same

// dump_buf is not used
/* ======================================================================== */
/* ========================================================================== */
#ifndef USE_MALLOC_FOR_KT_G_MALLOC
#ifdef KIATEL_DYNAMIC_MEM
gpointer kt_g_calloc(size_t num_elements, size_t size){

	gpointer p;
	p=kt_g_malloc(num_elements*size);
	if(p != 0){
		memset(p,0,num_elements*size);
	}
	return p;
}
#endif
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifndef USE_MALLOC_FOR_KT_G_MALLOC
#ifdef KIATEL_DYNAMIC_MEM
gpointer kt_g_malloc (gulong n_bytes){
	gpointer p;

	if(n_bytes == 0){
	    prnt_msg(ERROR,0x1356);
	    print_call_frame();
	    // only warning for now
	}

	guint i=mem_rec_req((unsigned int)(n_bytes+4));

	if(i >= NO_MEM_REC){
		prnt_msg(ERROR,0x1357);
		prnt_int(i);prnt_char(CR);
		return 0;
	}
	*((unsigned int *)(mem_rec[i].parm))=i;
	p=(gpointer)&mem_rec[i].parm[4];
#ifdef DEBUG_ACT
	prnt_char('k');prnt_char('t');prnt_char('_');prnt_char('g');
	prnt_char('_');prnt_char('m');prnt_char('a');prnt_char('l');
	prnt_char('l');prnt_char('o');prnt_char('c');prnt_char(':');
	prnt_int(i);prnt_char(CR);
	print_pointer(p);
#endif
	if(p != 0){
		memset(p,0,(size_t)n_bytes);
	}

	return p;
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifndef USE_MALLOC_FOR_KT_G_MALLOC
#ifdef KIATEL_DYNAMIC_MEM
void kt_g_free (gpointer mem){
	if(mem){
#ifdef DEBUG_ACT
	    prnt_char('k');prnt_char('t');prnt_char('_');prnt_char('g');
	    prnt_char('_');prnt_char('f');prnt_char('r');prnt_char('e');
	    prnt_char('e');prnt_char(':');
	    prnt_int(*((unsigned int *)(mem-4)));prnt_char(CR);
//	    print_pointer(mem);
#endif
	    mem_rec_rls(*((unsigned int *)(mem-4)));
	}
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
#ifndef USE_MALLOC_FOR_KT_G_MALLOC
void *mem_set(void *buffer, int ch, size_t num){
	return(memset(buffer,ch,num));
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void print_my_mr1(gpointer mem){
	if(mem){
		prnt_char('m');prnt_char('y');prnt_char('_');prnt_char('m');prnt_char('r');prnt_char('1');prnt_char(':');
		prnt_int(*((unsigned int *)(mem-4)));prnt_char('-');
		prnt_long((unsigned long)mem);prnt_char(CR);
		prnt_char(CR);
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef FIFO_RD_WR
BYTE put_pckt_to_variable_size_buf(BUF *buf,unsigned int size,BYTE *dat){

	unsigned int i,free_size,s;
	BYTE no_size_bytes;

#ifdef TCP_DEBUG_ACT
	/*
	{
		unsigned int n;

		prnt_string("put_pckt_to_variable_size_buf ");
		prnt_string("size=");
		prnt_int(size);prnt_char('-');
		for(n=0;n<size;n++){
			prnt_byte(dat[n]);prnt_char('-');
		}
		prnt_char(CR);
		prnt_string("buf info: ");
		prnt_int(buf->rear);prnt_char('-');
		prnt_int(buf->front);prnt_char('-');
		prnt_int(((unsigned int)buf)>>16);prnt_int((unsigned int)buf);prnt_char(CR);
	}
	*/
#endif
	if ( buf->rear >= buf->front ){
		free_size=buf->size - buf->rear + buf->front;
	}
	else{
		free_size=buf->front - buf->rear ;
	}
	if(size <= 0xff){
		no_size_bytes=1;
	}
#if (COMPILER == BCC)
	else{
		no_size_bytes=2;
	}
#endif
#if (COMPILER == GCC)
	else if(size <= 0xffff){
		no_size_bytes=2;
	}
	else if(size <= 0xffffff){
		no_size_bytes=3;
	}
	else {
		no_size_bytes=4;
	}
#endif
	// one byte for no_size_bytes and 1..4 bytes for size
	if(free_size < (size+no_size_bytes+1)){
		prnt_msg(BUF_OVERFLOW,0x1361);
		prnt_int(free_size);prnt_char('-');
		prnt_int(size);prnt_char('-');
		prnt_long(buf->size);prnt_char(CR);
		print_call_frame();
		return FAIL;
	}

	buf->rear=((buf->rear == buf->size -1) ? 0 : buf->rear + 1 ) ;
	buf->arr[buf->rear]=no_size_bytes;
	s=size;
	for(i=0;i<no_size_bytes;i++,s>>=8){
		buf->rear=((buf->rear == buf->size -1) ? 0 : buf->rear + 1 ) ;
		buf->arr[buf->rear]=(BYTE)s;
	}
	for(i=0;i<size;i++){
		buf->rear=((buf->rear == buf->size -1) ? 0 : buf->rear + 1 ) ;
		buf->arr[buf->rear]=dat[i];
	}
#ifdef TCP_DEBUG_ACT
	/*
	{
		prnt_string("buf info: ");
		prnt_int(buf->rear);prnt_char('-');
		prnt_int(buf->front);prnt_char('-');
		prnt_int(((unsigned int)buf)>>16);prnt_int((unsigned int)buf);prnt_char(CR);
	}
	*/
#endif
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE get_pckt_from_variable_size_buf(BUF *buf,unsigned int *size,BYTE *dat){

	unsigned int i;
	BYTE no_size_bytes;

	if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/

	buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
	no_size_bytes=buf->arr[buf->front];

	*size=0;
	for(i=0;i<no_size_bytes;i++){
		if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
		buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
		*size= *size | (buf->arr[buf->front] << (i*8));
	}
	for(i=0;i<*size;i++){
		if (buf->front == buf->rear) return FAIL; /* rx_buf is empty */
		buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
		dat[i]=buf->arr[buf->front];
	}
#ifdef TCP_DEBUG_ACT
	/*
	{
		unsigned int n;

		prnt_string("get_pckt_from_variable_size_buf");
		prnt_string("size=");
		prnt_int(*size);prnt_char('-');
		for(n=0;n<*size;n++){
			prnt_byte(dat[n]);prnt_char('-');
		}
		prnt_char(CR);
	}
	*/
#endif
	return OK;
}
#endif
/* ======================================================================= */
/* ======================================================================== */
#ifdef FIFO_RD_WR
BYTE check_variable_size_buf_is_empty(BUF *buf){

	if (buf->front == buf->rear) return OK; /*rx_buf is empty*/
	return FAIL;
}
#endif
/* ======================================================================= */
/* ======================================================================== */
#ifdef FIFO_RD_WR
unsigned int check_free_variable_size_buf(BUF *buf){

	unsigned int free_size;

	if ( buf->rear >= buf->front ){
		free_size=buf->size - buf->rear + buf->front;
	}
	else{
		free_size=buf->front - buf->rear ;
	}
	return free_size;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef FIFO_RD_WR
unsigned int check_used_variable_size_buf(BUF *buf){

	unsigned int used_size;

	if ( buf->rear >= buf->front ){
		used_size = buf->rear - buf->front;
	}
	else{
		used_size = buf->size - buf->front + buf->rear ;
	}
	return used_size;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE get_pckt_size_from_variable_size_buf(BUF *buf,unsigned int *size){

	unsigned int i;
	BYTE no_size_bytes;

	if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/

	buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
	no_size_bytes=buf->arr[buf->front];

	*size=0;
	for(i=0;i<no_size_bytes;i++){
		if (buf->front == buf->rear) return FAIL; /*rx_buf is empty*/
		buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
		*size= *size | (buf->arr[buf->front] << (i*8));
	}
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE get_this_size_of_data_from_variable_size_buf(BUF *buf,unsigned int size,BYTE *dat){

	unsigned int i;

	for(i=0;i<size;i++){
		if (buf->front == buf->rear) return FAIL; /* rx_buf is empty */
		buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
		dat[i]=buf->arr[buf->front];
	}
	return OK;
}
#endif
/* ======================================================================*/
/* ======================================================================*/
#ifdef FIFO_RD_WR
BYTE get_maximum_this_size_of_data_from_variable_size_buf(BUF *buf,unsigned int size,BYTE *dat){

	unsigned int i;

	for(i=0;i<size;i++){
		if (buf->front == buf->rear) return OK; /* rx_buf is empty */
		buf->front=((buf->front == buf->size -1) ? 0 : buf->front + 1 ) ;
		dat[i]=buf->arr[buf->front];
	}
	return OK;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
WORD_MR find_mr1_from_g_pointer(gpointer mem)
{
	if(mem){
		return(*((unsigned int *)(mem-4)));
	}
	else{
		prnt_msg(ERROR,0x2359);
		print_call_frame();
		return NULL_MEM_REC;
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
gpointer find_g_pointer_from_mr1 (WORD_MR mr1)
{
	if(mr1 >= NO_MEM_REC){
		prnt_msg(ERROR,0x2360);
		return NULL_POINTER;
	}
	return (gpointer)&(mem_rec[mr1].parm[4]);
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef KIATEL_DYNAMIC_MEM
guchar **allocate_two_dimensions_char_array(guint nrows,guint ncolumns)
{
	guchar **array1;
	guint i,j;

	array1=(guchar **)kt_g_malloc(nrows * sizeof(guchar *));
	if(array1 == NULL_POINTER){
		return NULL_POINTER;
	}
	for(i=0;i<nrows;i++){
		array1[i]=(guchar *)kt_g_malloc(ncolumns * sizeof(guchar));
		if(array1[i] == NULL_POINTER){
			return NULL_POINTER;
		}
	}
	for(i=0;i<nrows;i++){
		for(j=0;j<ncolumns;j++){
			array1[i][j]=0;
		}
	}
	return(array1);
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#if ((!defined DO_NOT_BULK_SAVE_ADDR) || (defined SAVE_ADDR_FOR_LARGE_MEM))
void print_addr(gptr *addr){
	print_stored_call_frame(addr);
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if ((!defined DO_NOT_BULK_SAVE_ADDR) || (defined SAVE_ADDR_FOR_LARGE_MEM))
void save_addr(gptr *addr){
	store_call_frame(addr);
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifndef DO_NOT_BULK_SAVE_ADDR
void copy_addr(gptr *addr1,gptr *addr2){
	unsigned int i;
	for(i=0;i<BACKTRACK_DEPTH;i++){
		addr1[i]=addr2[i];
		if(addr2[i] == 0) break;
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void print_mr(unsigned int mr1){
	MEM_REC_ST *mem_pt;
	unsigned int i;
	if(mr1 >= NO_MEM_REC){
		prnt_int(mr1);prnt_char(CR);
		return;
	}
	mem_pt=&mem_rec[mr1];
	prnt_int(mr1);prnt_char(':');
	prnt_int(mem_pt->size);prnt_char('=');
	for(i=0;i<mem_pt->size;i++){
		prnt_byte(mem_pt->parm[i]);prnt_char('-');
	}
	prnt_char(CR);
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifndef CENTRAL_EXTERN
#if COMPILER == GCC
#if (OS == DOS)
#include <debug/syms.h>
#endif
#endif
#endif // ndef CENTRAL_EXTERN
/* ======================================================================== */
/* ======================================================================== */
#if ((!defined DO_NOT_BULK_SAVE_ADDR) || (defined SAVE_ADDR_FOR_LARGE_MEM))
#pragma warn -par
void store_call_frame(gptr *addr){
#if (COMPILER == GCC)
#if (OS == DOS)
	unsigned int i;
	unsigned long offs;
	for(i=0;i<BACKTRACK_DEPTH;i++){
		switch(i){
			case 0:
				addr[i]=(gptr)__builtin_return_address(0);
				break;
			case 1:
				addr[i]=(gptr)__builtin_return_address(1);
				break;
			case 2:
				addr[i]=(gptr)__builtin_return_address(2);
				break;
			case 3:
				addr[i]=(gptr)__builtin_return_address(3);
				break;
			case 4:
				addr[i]=(gptr)__builtin_return_address(4);
				break;
			case 5:
				addr[i]=(gptr)__builtin_return_address(5);
				break;
			case 6:
				addr[i]=(gptr)__builtin_return_address(6);
				break;
			case 7:
				addr[i]=(gptr)__builtin_return_address(7);
				break;
			case 8:
				addr[i]=(gptr)__builtin_return_address(8);
				break;
			case 9:
				addr[i]=(gptr)__builtin_return_address(9);
				break;
			default:  // should not happen
				addr[i]=0;
				break;
		}
		if(addr[i] == 0) break;
		syms_val2name (addr[i], &offs);
		if(offs == 0) break;
	}
	if(i < BACKTRACK_DEPTH) addr[i]=0;
#endif
#if (OS == LINUX_COMPATIBLE)
	{
		size_t size, i;
		gptr *array[BACKTRACK_DEPTH];

		size = backtrace ((void *)array , BACKTRACK_DEPTH);
		for(i=0;i<size;i++){
			addr[i]=(gptr)(array[i]);
		}
		if(size < BACKTRACK_DEPTH)
			addr[size]=0;
	}
#endif
#endif
}
#pragma warn +par
#endif
/* ======================================================================== */
/* ======================================================================== */
#if ((!defined DO_NOT_BULK_SAVE_ADDR) || (defined SAVE_ADDR_FOR_LARGE_MEM))
#pragma warn -par
void print_stored_call_frame(gptr *addr){
#if (COMPILER == GCC)
#if (OS == DOS)
	unsigned int i;
	unsigned long offs;
	char *symbol_name;
	for(i=0;i<BACKTRACK_DEPTH;i++){
	    if(addr[i] == 0) break;
	    symbol_name = syms_val2name (addr[i], &offs);
	    if(offs == 0) break;
	    prnt_string(symbol_name);
	    prnt_char('+');prnt_char('0');prnt_char('x');
	    prnt_int(offs);
	    prnt_char(CR);
	}
#endif
#if (OS == LINUX_COMPATIBLE)
	{
	    char **strings;
	    size_t i, size;
	    void *array[10];

	    for(size=0;size<BACKTRACK_DEPTH;size++){
		if(addr[size] == 0) break;
	    }
	    for(i=0;i<size;i++){
		array[i]=(void *)(addr[i]);
	    }
	    strings = backtrace_symbols (array, size);
	    prnt_string("Obtained ");prnt_int(size);prnt_string(" stack frames.\r");

	    for (i = 0; i < size; i++){
		prnt_string(strings[i]);prnt_char(CR);
	    }
	    free(strings);
	}
#endif
#endif
}
#pragma warn +par
#endif
/* ======================================================================== */
/* ======================================================================== */
void kt_backtrace(char *buf){
	buf[0]=0;
#if COMPILER == GCC
#if (OS == DOS)
	unsigned int i;
	gptr addr;
	unsigned long offs;
	char *symbol_name;
	for(i=0;i<10;i++){
	    switch(i){
		case 0:
		    addr=(gptr)__builtin_return_address(0);
		    break;
		case 1:
		    addr=(gptr)__builtin_return_address(1);
		    break;
		case 2:
		    addr=(gptr)__builtin_return_address(2);
		    break;
		case 3:
		    addr=(gptr)__builtin_return_address(3);
		    break;
		case 4:
		    addr=(gptr)__builtin_return_address(4);
		    break;
		case 5:
		    addr=(gptr)__builtin_return_address(5);
		    break;
		case 6:
		    addr=(gptr)__builtin_return_address(6);
		    break;
		case 7:
		    addr=(gptr)__builtin_return_address(7);
		    break;
		case 8:
		    addr=(gptr)__builtin_return_address(8);
		    break;
		case 9:
		    addr=(gptr)__builtin_return_address(9);
		    break;
		default:
		    addr=0;
		    break;
	    }
	    if(addr == 0) break;
	    symbol_name = syms_val2name (addr, &offs);
	    if(offs == 0) break;
	    sprintf(&buf[strlen(buf)],"%s+0x%04lx\r",symbol_name,offs);
/*
	    prnt_string(symbol_name);prnt_char('+');prnt_int(offs);
	    prnt_char(CR);
*/
	}
#endif // OS == DOS
#if (OS == LINUX_COMPATIBLE)
	{
	    void *array[10];
	    size_t size;
	    char **strings;
	    size_t i;

	    size = backtrace (array, 10);
	    strings = backtrace_symbols (array, size);

// writing \r was fine for prnt_...() routines, we ourselves expand it
// to \r\n before writing to file
// however for writine to stderr,... it should be \n

	    sprintf(&buf[strlen(buf)],"Obtained %zu stack frames.\n",size);
	    for(i = 0; i < size; i++){
		sprintf(&buf[strlen(buf)],"%s\n",strings[i]);
	    }
/*
	    prnt_string("Obtained ");prnt_int(size);prnt_string(" stack frames.\r");
	    for(i = 0; i < size; i++){
		prnt_string(strings[i]);prnt_char(CR);
	    }
*/
	    free(strings);
	}
#endif
#endif
}
/* ======================================================================== */
/* ======================================================================== */
void print_call_frame(void){
    char buf[2048];
    kt_backtrace(buf);
    prnt_string(buf);
}
/* ======================================================================== */
/* ======================================================================== */
void init_debug(void){
#if COMPILER == GCC
#if (OS == DOS)
#ifdef SYSTEM_SIDE
	syms_init ("s.exe");
#endif
#ifdef OPERATOR_SIDE
	syms_init ("opr.exe");
#endif
#endif
#endif
}
/*========================================================================*/
/*========================================================================*/
#ifdef MY_TCPIP_FOR_OAM
#ifdef SYSTEM_SIDE
void start_oam_tcp_passive_port(unsigned int rec_no){

	SOCKET_STRUCT lsocket;
	SOCKADDR_IN_STRUCT sock;
	int s;
	USOCK_STRUCT *up;
	guint tcb_no;

//#ifdef DEBUG_ACT
	ws2p("start_oam_tcp_passive_port(\r\n");
//#endif
#if (NO_OPRTR != 0)
	if(rec_no >= NO_OPRTR){
	    mes2prnti(mess[TCP_ERR],0x2626);
	    goto next1;
	}
#ifdef HOT_STANDBY
	if (rd_master_swch() == RESET)
	    lsocket.address=
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 92 <<  0);
	else
#endif
	    lsocket.address=
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 91 <<  0);
	oam[rec_no].link_no=(unsigned int)(-1);
	lsocket.port=3491+rec_no;
	s=socket(AF_INET,SOCK_STREAM,0);
	if((unsigned int)s >= Nsock){
		mes2prnti(mess[TCP_ERR],0x2627);
		goto next1;
	}
	up=&usock_st[s];
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = lsocket.address;
	sock.sin_port	     = lsocket.port;
	bind(s,(SOCKADDR_STRUCT *)&sock,SOCKSIZE);
//	tcb_no=so_tcp_listen(up,1); // TCP_SERVER
	tcb_no=so_tcp_listen(up,0); // TCP_PASSIVE
	if(tcb_no >= MAX_TCB){
		mes2prnti(mess[TCP_ERR],0x2628);
		goto next1;
	}
#ifdef DEBUG_ACT
	prnt_char('O');prnt_char('M');prnt_char(' ');prnt_char('T');prnt_char('C');prnt_char('P');prnt_char(':');
	prnt_int(tcb_no);prnt_char('-');
	prnt_int(lsocket.address>>16);prnt_int(lsocket.address);prnt_char('-');
	prnt_int(lsocket.port);prnt_char(CR);
#endif
	tcb_st[tcb_no].type=OAM_TCP_TYPE;
	tcb_st[tcb_no].user = s;
	tcb_st[tcb_no].rec_no = rec_no;
	oam[rec_no].link_no=tcb_no;
next1:
#endif
	return;
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef MY_TCPIP_FOR_OAM
#ifdef OPERATOR_SIDE

#define FTP_T	3   // like CR_T

extern BYTE mb[];
#ifdef TCP_INSTALLED
extern TCB_STRUCT tcb_st[MAX_TCB];
extern unsigned int tcb0,tcb1;
extern BUF nettx_st0,nettx_st1;
#endif

void mtmaq(BYTE);
#ifdef LIM
void mtcpq(BYTE);
#else
void mtcpq(void);
#endif
void handle_ftp(BYTE size);

void start_oam_tcp_active_port(unsigned int rec_no){

	SOCKET_STRUCT lsocket,fsocket;
	guint tcb_no;

//#ifdef DEBUG_ACT
	prnt_string("start_oam_tcp_active_port start\r\n");
//#endif
	if(rec_no == 0) tcb0=(unsigned int)(-1);
	if(rec_no == 1) tcb1=(unsigned int)(-1);
#if (TON == 0)
	lsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 95 <<  0);
#endif
#if (TON == 1)
	lsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 96 <<  0);
#endif
#if (TON == 2)
	lsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 97 <<  0);
#endif
#if (TON == 3)
	lsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( 98 <<  0);
#endif
	lsocket.port	= 3490;
	fsocket.address =
			(192 << 24) |
			(168 << 16) |
			(100 <<  8) |
			( (91+rec_no) <<  0);
#if (TON == 0)
	fsocket.port	= 3491;
#endif
#if (TON == 1)
	fsocket.port	= 3492;
#endif
	tcb_no=start_active_tcp(&lsocket,&fsocket);
	if(tcb_no >= MAX_TCB){
		prnt_msg(TCP_ERR,0x2631);
		goto next1;
	}
	tcb_st[tcb_no].type=OAM_TCP_TYPE;
	tcb_st[tcb_no].rec_no = rec_no;
	if(rec_no == 0) tcb0=tcb_no;
	if(rec_no == 1) tcb1=tcb_no;
#ifdef DEBUG_ACT
	prnt_char('O');prnt_char('M');prnt_char(' ');prnt_char('T');prnt_char('C');prnt_char('P');prnt_char(':');
	prnt_int(tcb_no);prnt_char('-');
	prnt_int(lsocket.address>>16);prnt_int(lsocket.address);
	prnt_int(lsocket.port);prnt_char('-');
	prnt_int(fsocket.address>>16);prnt_int(fsocket.address);
	prnt_int(fsocket.port);prnt_char(CR);

	prnt_string(("start_oam_tcp_active_port end\r\n");
#endif
next1:
	return;
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef MY_TCPIP_FOR_OAM
BYTE oam_tcp_port_analysis(unsigned int tcb_no,unsigned int mr1){
	unsigned int size,mr2;
	BYTE dat[1024],rec_no;
//#ifdef DEBUG_ACT
	prnt_string("oam_tcp_port_analysis\r\n");
	prnt_int(tcb_no);prnt_char(CR);
	print_mr(mr1);
//#endif
	if (mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x2007);
		return FAIL;
	}
	rec_no=tcb_st[tcb_no].rec_no;
again:
	size=(mem_rec[mr1].parm[2] << 8) | mem_rec[mr1].parm[3];
	if(size < mem_rec[mr1].size){
	    mr2=mem_rec_req(mem_rec[mr1].size-size);
	    mem_cpy(mem_rec[mr2].parm,&mem_rec[mr1].parm[size],mem_rec[mr1].size-size);
	}
	else
	    mr2=NULL_MEM_REC;

	size=size-4;
	mem_cpy(dat,&mem_rec[mr1].parm[4],size);
	mem_rec_rls(mr1);
//...
#ifdef SYSTEM_SIDE
#ifdef BATCH_PROGRAMMING
	if(size >= MAX_OPR_SYS_PCKT_SIZE+1){
	    mes2prnti(mess[TCP_ERR],0x2101);
	    prnt_int(size);prnt_char(CR);
	    goto next;
	}
#else
	if(size >= MIN_I_PKT+1){
	    mes2prnti(mess[TCP_ERR],0x2102);
	    prnt_int(size);prnt_char(CR);
	    goto next;
	}
#endif
	memcpy(mb,dat,size);
	mb[size]=rec_no;
#ifdef BATCH_PROGRAMMING
	if( ((mb[0] & 0x0f) == BATCH_MA_T) || ((mb[0] & 0x0f) == MA_T)){
	    to_btch_maq();
	}
#else
	if( (mb[0] & 0x0f) == MA_T){
	    mtmaq();
	}
#endif
	else{
	    if( (mb[0] & 0x0f) == TERM_T){
		terminal_oam_no=rec_no;
		message_to_term_q(size);
	    }
	    else{
		mtcpq();
	    }
	}

#endif // SYSTEM_SIDE
#ifdef OPERATOR_SIDE
	if(size >= MAX_OPR_SYS_PCKT_SIZE+1){
	    prnt_msg(TCP_ERR,0x2301);
	    prnt_int(size);prnt_char(CR);
	    goto next;
	}
	memcpy(mb,dat,size);
	if( (mb[0] & 0x0f) == MA_T){
	    mtmaq(rec_no);
	}
	else{
	    if( (mb[0] & 0x0f) == TERM_T){
		message_to_term_q(size);
	    }
	    else{
		if( (mb[0] & 0x0f) == FTP_T){
		    handle_ftp(size);
		}
		else{
#ifdef LIM
		    mtcpq(rec_no);
#else
		    mtcpq();
#endif
		}
	    }
	}
#endif // OPERATOR_SIDE
//...
next:
	if(mr2 < NO_MEM_REC){
		mr1=mr2;
		goto again;
	}
	return OK;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef MY_TCPIP_FOR_OAM
BYTE send_oam_pckt_to_endpoint(BYTE n,unsigned int size,BYTE *dat){
//#ifdef DEBUG_ACT
    int i;
    prnt_string("send_oam_pckt_to_endpoint\r\n");
    prnt_int(n);prnt_char('-');
    prnt_int(size);prnt_char(':');
    for(i=0;i<size;i++){
	prnt_byte(dat[i]);prnt_char('-');
    }
    prnt_char(CR);
//#endif
#ifdef SYSTEM_SIDE
    return put_pckt_to_variable_size_buf(&oam[n].nettx_st,size,dat);
#endif
#ifdef OPERATOR_SIDE
    if(n == 0)
	return put_pckt_to_variable_size_buf(&nettx_st0,size,dat);
    if(n == 1)
	return put_pckt_to_variable_size_buf(&nettx_st1,size,dat);
    return FAIL;
#endif
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef MY_TCPIP_FOR_OAM
BYTE send_oam_pckt_to_tcp_port(BYTE n,unsigned int size,BYTE *dat){
	unsigned int tcb_no;
//#ifdef DEBUG_ACT
	int i;
	prnt_string("send_oam_pckt_to_tcp_port\r\n");
	prnt_int(n);prnt_char('-');
	prnt_int(size);prnt_char(':');
	for(i=0;i<size;i++){
		prnt_byte(dat[i]);prnt_char('-');
	}
	prnt_char(CR);
//#endif
#ifdef SYSTEM_SIDE
	tcb_no=oam[n].link_no;
#endif
#ifdef OPERATOR_SIDE
	tcb_no=(n == 0) ? tcb0 : tcb1;
#endif
	if(tcb_no != (unsigned int)(-1)){
	    unsigned int mr1;
	    size+=4;
	    mr1=mem_rec_req(size);
	    if(mr1 >= NO_MEM_REC){
		prnt_msg(TCP_ERR,0x6401);
		return FAIL;
	    }
	    mem_rec[mr1].parm[0]=3;  //ISO_TRANSPORT_SERVICE_ON_TOP_OF_TCP_VERSION_3; // version number
	    mem_rec[mr1].parm[1]=0x00; // reserved field
	    mem_rec[mr1].parm[2]=(BYTE)(size>>8); // length of total packet including these four bytes(msb)
	    mem_rec[mr1].parm[3]=(BYTE)size;	  // length of total packet including these four bytes(lsb)
	    mem_cpy(&mem_rec[mr1].parm[4],dat,size-4);
	    send_tcp(tcb_no,mr1);
	    return OK;
	}
	return FAIL;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef MY_TCPIP_FOR_OAM
BYTE sending_to_tcp_port_is_ok(BYTE n){
	unsigned int tcb_no;
	TCB_STRUCT *tcb_pt;
#ifdef SYSTEM_SIDE
	tcb_no=oam[n].link_no;
#endif
#ifdef OPERATOR_SIDE
	tcb_no=(n == 0) ? tcb0 : tcb1;
#endif
	if(tcb_no == (unsigned int)(-1))
	    return FAIL;
	tcb_pt=&tcb_st[tcb_no];
	switch(tcb_pt->state){
	  case TCP_CLOSED:
	  case TCP_FINWAIT1:
	  case TCP_FINWAIT2:
	  case TCP_CLOSING:
	  case TCP_LAST_ACK:
	  case TCP_TIME_WAIT:
	  case TCP_LISTEN:
	    return FAIL;
	  case TCP_SYN_SENT:
	  case TCP_SYN_RECEIVED:
	  case TCP_ESTABLISHED:
	  case TCP_CLOSE_WAIT:
	    return OK;
	}
	return OK;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef MY_TCPIP_FOR_OAM
void my_tcpip_oam_tx_hndlr(void){
	unsigned int i,size,no_of_ports;
	BYTE dat[4096];
	BUF *buf_ptr[10];
#ifdef SYSTEM_SIDE
	no_of_ports=NO_OPRTR;
	for(i=0;i<no_of_ports;i++){
	    buf_ptr[i]=&oam[i].nettx_st;
	}
#endif
#ifdef OPERATOR_SIDE
#ifdef HOT_STANDBY
	no_of_ports=2;
#else
	no_of_ports=1;
#endif
	for(i=0;i<no_of_ports;i++){
	    buf_ptr[i]= (i == 0) ? &nettx_st0 : &nettx_st1;
	}
#endif
	for(i=0;i<no_of_ports;i++){
	    if(check_variable_size_buf_is_empty(buf_ptr[i]) == OK)
		continue;
	    if(sending_to_tcp_port_is_ok(i)){
		BYTE j;
		for(j=0;j<1;j++){
		    if(get_pckt_from_variable_size_buf(buf_ptr[i],&size,dat) == OK)
			send_oam_pckt_to_tcp_port(i,size,dat);
		    else
			break;
		}
	    }
	}
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if									\
   (									\
     ( ((defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||	\
	(defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
	(defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) || 	\
	(defined SYSCON_SERVER) || (defined SYSCON_CLIENT)		\
       )  && (defined TCP_INSTALLED)					\
     ) ||								\
     (defined MY_TCPIP_FOR_OAM) ||					\
     ( (defined KIATEL_UDP_INSTALLED) &&				\
       ( (defined MY_UDP_FOR_OAM)				  ||	\
	 ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))  ||	\
	 ((defined SYSCONUDP)	       && (defined SYSTEM_SIDE))  ||	\
	 ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE))	\
       )								\
     )									\
   )
#ifdef OPERATOR_SIDE
#ifdef ETHERNET_INSTALLED
extern unsigned int oam_iface_no;
#endif
#ifdef KIATEL_UDP_INSTALLED
extern KIATEL_SYSTEM_STRUCT	kt_system_st;
extern KIATEL_UDP_STRUCT	kt_udp_rec[MAX_KIATEL_UDP_REC];
#endif
#ifdef UDP_INSTALLED
extern UCB_STRUCT ucb_st[MAX_UCB];
extern USOCK_STRUCT usock_st[MAX_USOCK];
#endif
#ifdef LAPB_ON_OPERATOR_SIDE
extern HDLC_ST hdlc_ch[];
extern BYTE sync_flag[];
#endif
#endif
guint add_this_oam_route(guint32 address){

	guint route_no;

#ifdef DEBUG_ACT
	prnt_string("add_this_oam_route start:");
	prnt_int(address>>16);prnt_int(address);
	prnt_char(CR);
#endif
	route_no=rt_lookup(address);
	if(route_no >= NO_IP_ROUTE){
		unsigned long oam_subnet_mask,j;
		BYTE i;
		oam_subnet_mask=kt_system_st.ipv4_subnet_mask;
		for(i=0,j=0x80000000L;(oam_subnet_mask & j) != 0;i++,j>>=1);
		oam_subnet_mask=i;
		route_no=my_rt_add(address,
				oam_subnet_mask,
				0,
				oam_iface_no);
#ifdef DEBUG_ACT
		prnt_string("new route added:");
		prnt_int(address>>16);prnt_int(address);prnt_char('-');
		prnt_int(route_no);prnt_char(CR);
#endif
	}
	return route_no;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
BYTE is_valid_digit(char *ip_str)
{
    while (*ip_str) {
	if (*ip_str >= '0' && *ip_str <= '9')
	    ++ip_str;
	else
	    return 0;
    }
    return 1;
}
BYTE is_valid_ip(char *original_ip_str)
{
    int num, dots = 0;
    char *ptr;

    if (original_ip_str == NULL_POINTER)
	return 0;

    char ip_str[16];
    strncpy(ip_str,original_ip_str,15);

    // See following link for strtok()
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/strtok_r.html
    ptr = strtok(ip_str, ".");

    if (ptr == NULL_POINTER)
	return 0;

    while (ptr) {

	/* after parsing string, it must contain only digits */
	if (!is_valid_digit(ptr))
	    return 0;

	num = atoi(ptr);

	/* check for valid IP */
	if (num >= 0 && num <= 255) {
	    /* parse remaining string */
	    ptr = strtok(NULL_POINTER, ".");
	    if (ptr != NULL_POINTER)
		++dots;
	} else
	    return 0;
    }

    /* valid IP string must contain 3 dots */
    if (dots != 3)
	return 0;
    return 1;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifndef DYNAMIC_IP_ALLOCATION
#if ((defined KIATEL_UDP_INSTALLED) ||\
     (defined OAM_ON_UDP_CLIENT_SERVER_SIDE) ||\
     (defined TCP_INSTALLED) ||\
     ((defined MY_UDP_FOR_OAM) && (defined SYSTEM_SIDE) && (OS == LINUX_COMPATIBLE))\
    )
#define MAX_CHAR_PER_LINE	256
// from 0x60->0x80
#ifndef SOFTWARE_INTERRUPT_VECTOR
#define SOFTWARE_INTERRUPT_VECTOR		0x7e
#endif
void read_kiatel_udp_config_file(void)
{
	gchar local_ipv4_address[16];
	gchar local_ipv4_subnet_mask[16];
#if ((defined KIATEL_UDP_INSTALLED) || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE) )
	gchar remote_ipv4_address[16];
#endif
	guint local_port, remote_port;
#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	guint win_oam_no;
	int i;
#endif
#if ((defined SYSCONUDP) && (defined SYSTEM_SIDE))
	guint sys_con_no;
#endif
#if ((defined MY_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	guint our_oam_no;
#endif
#ifdef OPERATOR_SIDE
	guint control_unit_no;
#endif
#ifdef KIATEL_UDP_INSTALLED
	KIATEL_UDP_STRUCT *kt_udp_pt;
#endif
	FILE *fp;
	gchar *p,line[MAX_CHAR_PER_LINE];

	kt_system_st.int_no=SOFTWARE_INTERRUPT_VECTOR;

#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	for(i=0;i<NO_KIATEL_UDP_OAM_TERMINAL;i++){
		winoam[i].link_type=(BYTE)(-1);
		winoam[i].link_no=(unsigned int)(-1);
		winoam[i].heartbeat=0;
	}
	win_oam_no=0;
#endif
#if ((defined SYSCONUDP) && (defined SYSTEM_SIDE))
	guint j;
	for(j=0;j<NO_KIATEL_UDP_SYSCONUDP;j++){
		sysconudp[j].link_no=(unsigned int)(-1);
	}
	sys_con_no=0;
#endif

#if ((defined MY_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	our_oam_no=0;
#endif

#ifdef SYSTEM_SIDE
#ifdef DTSW_STYLE_CLI
	if((fp=fopen("/usr/local/etc/DTSW/ipsys.dat","rb")) == NULL_POINTER)
#endif
	{ /* file does not exist */
	    strcpy(fnptr,"ipsys.dat");
	    if((fp=fopen(fnbuf,"rb")) == NULL_POINTER){ /* file does not exist */
		ws2p("No ipsys.dat file was found!\r\n");
		return;
	    }
	}
#endif
#ifdef OPERATOR_SIDE
	if((fp=fopen("ipopr.dat","rb")) == NULL_POINTER){ /* file does not exist */
		prnt_string("No ipopr.dat file was found!\r\n");
		return;
	}
	control_unit_no=0;
#endif
	local_port=remote_port=(guint)(-1);

	strcpy(local_ipv4_subnet_mask,"255.255.255.0");

again0: while(!feof(fp)){
	    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
		break;
	    if(line[0]=='*')
		continue;
	    p = strtok(line, " \t\r\n");
	    if(p == NULL_POINTER)
		continue;
	    if(strcmp(p,"StartSystem") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"Interrupt") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    kt_system_st.int_no=(unsigned int)strtol(p,NULL_POINTER,16);
			    }
			    else
			    if(strcmp(p,"Ipv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
#if ((defined KIATEL_UDP_INSTALLED) || (defined TCP_INSTALLED))
				if(p != NULL_POINTER)
				    strcpy(local_ipv4_address, p);
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
				{
				    struct hostent *hp;
				    gchar name_str[1024];

				    gethostname(name_str,1024);
				    hp=gethostbyname(name_str);
				    if(hp != NULL_POINTER){
					if(hp->h_addr_list[0] != NULL_POINTER){
					    // first ip
					    strcpy(local_ipv4_address,inet_ntoa(*(struct in_addr *)(hp->h_addr_list[0])));
					    strcpy(kt_system_st.ipv4_address_str, local_ipv4_address);
					}
				    }
				    if(is_valid_ip(local_ipv4_address) == 0){
					printf("Couldn't get our IP address from our hostname\r\n");
					exit(1);
				    }
				}
#endif
			    }
			    else
			    if(strcmp(p,"Ipv4SubnetMask") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
#if ((defined KIATEL_UDP_INSTALLED) || (defined TCP_INSTALLED))
				if(p != NULL_POINTER)
				    strcpy(local_ipv4_subnet_mask, p);
#endif
			    }
			    else
			    if(strcmp(p,"EndSystem") == 0){
				kt_system_st.ipv4_address=find_ipv4_from_string(local_ipv4_address);
				kt_system_st.ipv4_subnet_mask=find_ipv4_from_string(local_ipv4_subnet_mask);
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	    if(strcmp(p,"StartWinOamTerminal") == 0){
		guchar send_heartbeat=1;
		guchar is_terminal=0;
		guchar send_alarm=1;
		guchar send_traffic=1;
		guchar send_billing=1;
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"LocalPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    local_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"SendHeartBeat") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    send_heartbeat=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"IsTerminal") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    is_terminal=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"SendAlarm") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    send_alarm=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"SendTraffic") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    send_traffic=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"SendBilling") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    send_billing=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"EndWinOamTerminal") == 0){

				guint kt_udp_no;
				if(win_oam_no < NO_KIATEL_UDP_OAM_TERMINAL){
#ifdef KIATEL_UDP_INSTALLED
				    guint ucb_no;
				    SOCKET_STRUCT lsocket;

				    lsocket.address=kt_system_st.ipv4_address;
				    lsocket.port=local_port;
				    if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
					ucb_no=start_udp(&lsocket);
					if(ucb_no >= MAX_UCB){
					    mes2prnti(mess[UDP_ERR],0x3007);
					    return;
					}
				    }
				    else
					usock_st[ucb_st[ucb_no].user].refcnt++;
				    kt_udp_no=find_free_kiatel_udp_rec();
				    if(kt_udp_no >= MAX_KIATEL_UDP_REC){
					mes2prnti(mess[UDP_ERR],0x3008);
					return;
				    }
//				    ucb_st[ucb_no].user=kt_udp_no;
				    kt_udp_pt=&kt_udp_rec[kt_udp_no];
				    kt_udp_pt->state=IN_USE;
				    kt_udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    kt_udp_pt->remote_port=remote_port;
				    kt_udp_pt->local_port=local_port;
				    kt_udp_pt->ucb_no=ucb_no;
				    kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_WIN_OAM_TERMINAL_TYPE;
				    kt_udp_pt->upper_layer_no=win_oam_no;
				    kt_udp_pt->netrx_st.arr = kt_g_malloc(KIATEL_UDP_NETBUF_SIZE);
				    kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
				    kt_udp_pt->netrx_st.size = KIATEL_UDP_NETBUF_SIZE;
//				    kt_udp_pt->rx[]
//				    kt_udp_pt->kt_udp_no;
				    kt_udp_pt->rdy=SET;
				    insert_kiatel_udp_hash_table(kt_udp_no);
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
				    UDP_STRUCT *udp_pt;
				    gint sockfd;
				    struct sockaddr_in my_addr;

				    kt_udp_no=find_free_udp_rec();
				    if(kt_udp_no >= MAX_UDP_REC){
					mes2prnti(mess[UDP_ERR],0x3009);
					return;
				    }
				    udp_pt=&udp_st[kt_udp_no];
				    udp_pt->state=IN_USE;
				    udp_pt->client_flag=SET;
				    udp_pt->upper_layer_type=UDP_WIN_OAM_TYPE;
				    udp_pt->upper_layer_no=win_oam_no;

				    strcpy(udp_pt->remote_address_str, remote_ipv4_address);
				    udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    udp_pt->remote_port=remote_port;
				    strcpy(udp_pt->local_address_str, kt_system_st.ipv4_address_str);
				    udp_pt->local_address=kt_system_st.ipv4_address;
				    udp_pt->local_port=local_port;
				    udp_pt->rdy=SET;
				    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
				    if (sockfd < 0){
					mes2prnti(mess[UDP_ERR],0x3010);
					return;
				    }
				    fcntl(sockfd,F_SETFL,O_NONBLOCK);
				    memset(&my_addr, 0, sizeof(my_addr));
				    my_addr.sin_family = AF_INET;		  // host byte order
				    my_addr.sin_port = htons(udp_pt->local_port); // short ,network byte order
				    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // automatically fill with my IP
				    if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
					if(errno == EADDRINUSE){
					    guint socket_no;
					    SOCKET_STRUCT *socket_pt;
					    close(sockfd);
					    for(socket_no=first_udp_allocated_socket;socket_no<MAX_SOCKET_REC;socket_no=socket_pt->rp){
						socket_pt=&socket_st[socket_no];
						if((socket_pt->address == udp_pt->local_address) &&
						   (socket_pt->port == udp_pt->local_port)){
						    sockfd=socket_pt->sockfd;
						    break;
						}
					    }
					    if(socket_no >= MAX_SOCKET_REC){
						mes2prnti(mess[UDP_ERR],0x3013);
						return;
					    }
					    udp_pt->tx_sockfd=sockfd;
					    udp_pt->rx_sockfd=sockfd;
					    udp_pt->socket_rec_no=socket_no;
					    socket_pt->no_user++;
					}
					else{
					    mes2prnti(mess[UDP_ERR],0x3011);
					    return;
					}
				    }
				    else{
					udp_pt->tx_sockfd=sockfd;
					udp_pt->rx_sockfd=sockfd;
					if(find_free_udp_socket_rec_set_fields(kt_udp_no) >= MAX_SOCKET_REC){
					    mes2prnti(mess[UDP_ERR],0x3012);
					    return;
					}
				    }
				    insert_udp_hash_table(kt_udp_no);
#endif
				    winoam[win_oam_no].link_no=kt_udp_no;
				    winoam[win_oam_no].link_type=LINK_KIATEL_UDP;
				    winoam[win_oam_no].send_heartbeat=send_heartbeat;
				    winoam[win_oam_no].is_terminal=is_terminal;
				    winoam[win_oam_no].send_alarm=send_alarm;
				    winoam[win_oam_no].send_traffic=send_traffic;
				    winoam[win_oam_no].send_billing=send_billing;
				    win_oam_no++;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
#if ((defined SYSCONUDP) && (defined SYSTEM_SIDE))
	    if(strcmp(p,"StartSysConTerminal") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"LocalPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    local_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    if(strcmp(p,"EndSysConTerminal") == 0){
				guint kt_udp_no;
				if(sys_con_no < NO_KIATEL_UDP_SYSCONUDP){
#ifdef KIATEL_UDP_INSTALLED
				    guint ucb_no;
				    SOCKET_STRUCT lsocket;

				    lsocket.address=kt_system_st.ipv4_address;
				    lsocket.port=local_port;
				    if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
					ucb_no=start_udp(&lsocket);
					if(ucb_no >= MAX_UCB){
					    mes2prnti(mess[UDP_ERR],0x3007);
					    return;
					}
				    }
				    else
					usock_st[ucb_st[ucb_no].user].refcnt++;
				    kt_udp_no=find_free_kiatel_udp_rec();
				    if(kt_udp_no >= MAX_KIATEL_UDP_REC){
					mes2prnti(mess[UDP_ERR],0x3008);
					return;
				    }
//				    ucb_st[ucb_no].user=kt_udp_no;
				    kt_udp_pt=&kt_udp_rec[kt_udp_no];
				    kt_udp_pt->state=IN_USE;
				    kt_udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    kt_udp_pt->remote_port=remote_port;
				    kt_udp_pt->local_port=local_port;
				    kt_udp_pt->ucb_no=ucb_no;
				    kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_SYSCONUDP_TYPE;
				    kt_udp_pt->upper_layer_no=sys_con_no;
				    kt_udp_pt->netrx_st.arr = kt_g_malloc(KIATEL_UDP_NETBUF_SIZE);
				    kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
				    kt_udp_pt->netrx_st.size = KIATEL_UDP_NETBUF_SIZE;
//				    kt_udp_pt->rx[]
//				    kt_udp_pt->kt_udp_no;
				    kt_udp_pt->rdy=SET;
				    insert_kiatel_udp_hash_table(kt_udp_no);
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
				    UDP_STRUCT *udp_pt;
				    gint sockfd;
				    struct sockaddr_in my_addr;

				    kt_udp_no=find_free_udp_rec();
				    if(kt_udp_no >= MAX_UDP_REC){
					mes2prnti(mess[UDP_ERR],0x3009);
					return;
				    }
				    udp_pt=&udp_st[kt_udp_no];
				    udp_pt->state=IN_USE;
				    udp_pt->client_flag=SET;
				    udp_pt->upper_layer_type=UDP_SYSCONUDP_TYPE;   // to be defined in future!
				    udp_pt->upper_layer_no=win_oam_no;

				    strcpy(udp_pt->remote_address_str, remote_ipv4_address);
				    udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    udp_pt->remote_port=remote_port;
				    strcpy(udp_pt->local_address_str, kt_system_st.ipv4_address_str);
				    udp_pt->local_address=kt_system_st.ipv4_address;
				    udp_pt->local_port=local_port;
				    udp_pt->rdy=SET;
				    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
				    if (sockfd < 0){
					mes2prnti(mess[UDP_ERR],0x3010);
					return;
				    }
				    fcntl(sockfd,F_SETFL,O_NONBLOCK);
				    memset(&my_addr, 0, sizeof(my_addr));
				    my_addr.sin_family = AF_INET;		  // host byte order
				    my_addr.sin_port = htons(udp_pt->local_port); // short ,network byte order
				    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // automatically fill with my IP
				    if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
					if(errno == EADDRINUSE){
					    guint socket_no;
					    SOCKET_STRUCT *socket_pt;
					    close(sockfd);
					    for(socket_no=first_udp_allocated_socket;socket_no<MAX_SOCKET_REC;socket_no=socket_pt->rp){
						socket_pt=&socket_st[socket_no];
						if((socket_pt->address == udp_pt->local_address) &&
						   (socket_pt->port == udp_pt->local_port)){
						    sockfd=socket_pt->sockfd;
						    break;
						}
					    }
					    if(socket_no >= MAX_SOCKET_REC){
						mes2prnti(mess[UDP_ERR],0x3013);
						return;
					    }
					    udp_pt->tx_sockfd=sockfd;
					    udp_pt->rx_sockfd=sockfd;
					    udp_pt->socket_rec_no=socket_no;
					    socket_pt->no_user++;
					}
					else{
					    mes2prnti(mess[UDP_ERR],0x3011);
					    return;
					}
				    }
				    else{
					udp_pt->tx_sockfd=sockfd;
					udp_pt->rx_sockfd=sockfd;
					if(find_free_udp_socket_rec_set_fields(kt_udp_no) >= MAX_SOCKET_REC){
					    mes2prnti(mess[UDP_ERR],0x3012);
					    return;
					}
				    }
				    insert_udp_hash_table(kt_udp_no);
#endif
				    sysconudp[sys_con_no].link_no=kt_udp_no;
				    sys_con_no++;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
#if ((defined MY_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	    if(strcmp(p,"StartOurOamTerminal") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"LocalPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    local_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"EndOurOamTerminal") == 0){
				if(our_oam_no < NO_OPRTR){
				    guint kt_udp_no;
#if (OS == DOS)
				    guint ucb_no;
				    SOCKET_STRUCT lsocket;
				    lsocket.address=kt_system_st.ipv4_address;
				    lsocket.port=local_port;
				    if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
					ucb_no=start_udp(&lsocket);
					if(ucb_no >= MAX_UCB){
					    mes2prnti(mess[UDP_ERR],0x3007);
					    return;
					}
				    }
				    else
					usock_st[ucb_st[ucb_no].user].refcnt++;
				    kt_udp_no=find_free_kiatel_udp_rec();
				    if(kt_udp_no >= MAX_KIATEL_UDP_REC){
					mes2prnti(mess[UDP_ERR],0x3008);
					return;
				    }
//				    ucb_st[ucb_no].user=kt_udp_no;
				    kt_udp_pt=&kt_udp_rec[kt_udp_no];
				    kt_udp_pt->state=IN_USE;
				    kt_udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    kt_udp_pt->remote_port=remote_port;
				    kt_udp_pt->local_port=local_port;
				    kt_udp_pt->ucb_no=ucb_no;
				    kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_OUR_OAM_TERMINAL_TYPE;
				    kt_udp_pt->upper_layer_no=our_oam_no;
				    kt_udp_pt->netrx_st.arr = (BYTE *)0;
				    kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
				    kt_udp_pt->netrx_st.size = 0;
//				    kt_udp_pt->rx[]
//				    kt_udp_pt->kt_udp_no;
				    kt_udp_pt->rdy=SET;
				    insert_kiatel_udp_hash_table(kt_udp_no);
#endif
#if (OS == LINUX_COMPATIBLE)
				    UDP_STRUCT *udp_pt;
				    gint sockfd;
				    struct sockaddr_in my_addr;

				    kt_udp_no=find_free_udp_rec();
				    if(kt_udp_no >= MAX_UDP_REC){
					mes2prnti(mess[UDP_ERR],0x3009);
					return;
				    }
				    udp_pt=&udp_st[kt_udp_no];
				    udp_pt->state=IN_USE;
				    udp_pt->client_flag=SET;
				    udp_pt->upper_layer_type=UDP_OUR_OAM_TYPE;
				    udp_pt->upper_layer_no=our_oam_no;

				    strcpy(udp_pt->remote_address_str, remote_ipv4_address);
				    udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    udp_pt->remote_port=remote_port;
				    strcpy(udp_pt->local_address_str, kt_system_st.ipv4_address_str);
				    udp_pt->local_address=kt_system_st.ipv4_address;
				    udp_pt->local_port=local_port;
				    udp_pt->rdy=SET;
				    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
				    if (sockfd < 0){
					mes2prnti(mess[UDP_ERR],0x3010);
					return;
				    }
				    fcntl(sockfd,F_SETFL,O_NONBLOCK);
				    memset(&my_addr, 0, sizeof(my_addr));
				    my_addr.sin_family = AF_INET;		  // host byte order
				    my_addr.sin_port = htons(udp_pt->local_port); // short ,network byte order
				    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // automatically fill with my IP
				    if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
					if(errno == EADDRINUSE){
					    guint socket_no;
					    SOCKET_STRUCT *socket_pt;
					    close(sockfd);
					    for(socket_no=first_udp_allocated_socket;socket_no<MAX_SOCKET_REC;socket_no=socket_pt->rp){
						socket_pt=&socket_st[socket_no];
						if((socket_pt->address == udp_pt->local_address) &&
						   (socket_pt->port == udp_pt->local_port)){
						    sockfd=socket_pt->sockfd;
						    break;
						}
					    }
					    if(socket_no >= MAX_SOCKET_REC){
						mes2prnti(mess[UDP_ERR],0x3013);
						return;
					    }
					    udp_pt->tx_sockfd=sockfd;
					    udp_pt->rx_sockfd=sockfd;
					    udp_pt->socket_rec_no=socket_no;
					    socket_pt->no_user++;
					}
					else{
					    mes2prnti(mess[UDP_ERR],0x3011);
					    return;
					}
				    }
				    else{
					udp_pt->tx_sockfd=sockfd;
					udp_pt->rx_sockfd=sockfd;
					if(find_free_udp_socket_rec_set_fields(kt_udp_no) >= MAX_SOCKET_REC){
					    mes2prnti(mess[UDP_ERR],0x3012);
					    return;
					}
				    }
				    insert_udp_hash_table(kt_udp_no);
#endif
				    // or use oam[our_oam_no].link_no instead of FIRST_UDP_HDLC_CH+our_oam_no
				    hdlc_ch[FIRST_UDP_HDLC_CH+our_oam_no].ic_type=IC_HDLC_ON_UDP;
				    hdlc_ch[FIRST_UDP_HDLC_CH+our_oam_no].cs=kt_udp_no;
				    sync_flag[FIRST_UDP_HDLC_CH+our_oam_no] = SET;
				    our_oam_no++;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
#if ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE))
	    if(strcmp(p,"StartOtherControlUnit") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"LocalPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    local_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"EndOtherControlUnit") == 0){
				guint ucb_no, kt_udp_no;
				SOCKET_STRUCT lsocket;
				if(1){
				    lsocket.address=kt_system_st.ipv4_address;
				    lsocket.port=local_port;
				    if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
					ucb_no=start_udp(&lsocket);
					if(ucb_no >= MAX_UCB){
					    mes2prnti(mess[UDP_ERR],0x3007);
					    return;
					}
				    }
				    else
					usock_st[ucb_st[ucb_no].user].refcnt++;
				    kt_udp_no=find_free_kiatel_udp_rec();
				    if(kt_udp_no >= MAX_KIATEL_UDP_REC){
					mes2prnti(mess[UDP_ERR],0x3008);
					return;
				    }
//				    ucb_st[ucb_no].user=kt_udp_no;
				    kt_udp_pt=&kt_udp_rec[kt_udp_no];
				    kt_udp_pt->state=IN_USE;
				    kt_udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    kt_udp_pt->remote_port=remote_port;
				    kt_udp_pt->local_port=local_port;
				    kt_udp_pt->ucb_no=ucb_no;
				    kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_HTBY_TYPE;
				    kt_udp_pt->upper_layer_no=0;
				    kt_udp_pt->netrx_st.arr = (BYTE *)0;
				    kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
				    kt_udp_pt->netrx_st.size = 0;
//				    kt_udp_pt->rx[]
//				    kt_udp_pt->kt_udp_no;
				    kt_udp_pt->rdy=SET;
				    insert_kiatel_udp_hash_table(kt_udp_no);
				    hdlc_ch[HTBY_HDLC_CH].ic_type=IC_HDLC_ON_UDP;
				    hdlc_ch[HTBY_HDLC_CH].cs=kt_udp_no;
				    sync_flag[HTBY_HDLC_CH] = SET;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
#if ((defined MY_UDP_FOR_OAM) && (defined OPERATOR_SIDE))
	    if(strcmp(p,"StartControlUnit") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"LocalPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    local_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"EndControlUnit") == 0){
				guint ucb_no, kt_udp_no;
				SOCKET_STRUCT lsocket;
				if(control_unit_no < 2){
				    lsocket.address=kt_system_st.ipv4_address;
				    lsocket.port=local_port;
				    if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
					ucb_no=start_udp(&lsocket);
					if(ucb_no >= MAX_UCB){
					    prnt_msg(UDP_ERR,0x3007);
					    return;
					}
				    }
				    else
					usock_st[ucb_st[ucb_no].user].refcnt++;
				    kt_udp_no=control_unit_no;
				    if(kt_udp_no >= MAX_KIATEL_UDP_REC){
					prnt_msg(UDP_ERR,0x3008);
					return;
				    }
//				    ucb_st[ucb_no].user=kt_udp_no;
				    kt_udp_pt=&kt_udp_rec[kt_udp_no];
				    kt_udp_pt->state=IN_USE;
				    kt_udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    kt_udp_pt->remote_port=remote_port;
				    kt_udp_pt->local_port=local_port;
				    kt_udp_pt->ucb_no=ucb_no;
				    kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_OUR_OAM_TERMINAL_TYPE;
				    kt_udp_pt->upper_layer_no=control_unit_no;
				    kt_udp_pt->netrx_st.arr = (BYTE *)0;
				    kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
				    kt_udp_pt->netrx_st.size = 0;
//				    kt_udp_pt->rx[]
//				    kt_udp_pt->kt_udp_no;
				    kt_udp_pt->rdy=SET;
				    hdlc_ch[FIRST_UDP_HDLC_CH+control_unit_no].ic_type=IC_HDLC_ON_UDP;
				    hdlc_ch[FIRST_UDP_HDLC_CH+control_unit_no].cs=kt_udp_no;
				    sync_flag[FIRST_UDP_HDLC_CH+control_unit_no] = SET;
				    control_unit_no++;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
	    {
		continue;
	    }
	}

	prnt_string("System Ipv4Address:");
	prnt_string(local_ipv4_address);prnt_char('-');
	prnt_long(kt_system_st.ipv4_address);prnt_char(CR);

#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	{
	    guint i;
	    for(i=0;i<win_oam_no;i++){
		guint kt_udp_no;
		ws2p("win_oam_no:");
		int2prnt(i);toprnq('-');
		kt_udp_no=winoam[i].link_no;
		int2prnt(kt_udp_no);toprnq(CR);
#ifdef KIATEL_UDP_INSTALLED
		if(kt_udp_no < MAX_KIATEL_UDP_REC){
		    KIATEL_UDP_STRUCT *kt_udp_pt;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    ws2p("remote_address:");long2prnt(kt_udp_pt->remote_address);toprnq(CR);
		    ws2p("remote_port:");int2prnt(kt_udp_pt->remote_port);toprnq(CR);
		    ws2p("local_port:");int2prnt(kt_udp_pt->local_port);toprnq(CR);
		}
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
		if(kt_udp_no < MAX_UDP_REC){
		    UDP_STRUCT *udp_pt;
		    udp_pt=&udp_st[kt_udp_no];
		    ws2p("remote_address:");long2prnt(udp_pt->remote_address);toprnq('-');
		    ws2p("remote_port:");int2prnt(udp_pt->remote_port);toprnq('-');
		    ws2p("local_address:");long2prnt(udp_pt->local_address);toprnq('-');
		    ws2p("local_port:");int2prnt(udp_pt->local_port);toprnq(CR);
		}
#endif
	    }
	}
#endif
#if ((defined SYSCONUDP) && (defined SYSTEM_SIDE))
	{
	    guint i;
	    for(i=0;i<sys_con_no;i++){
		guint kt_udp_no;
		ws2p("sys_con_no:");
		int2prnt(i);toprnq('-');
		kt_udp_no=sysconudp[i].link_no;
		int2prnt(kt_udp_no);toprnq(CR);
#ifdef KIATEL_UDP_INSTALLED
		if(kt_udp_no < MAX_KIATEL_UDP_REC){
		    KIATEL_UDP_STRUCT *kt_udp_pt;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    ws2p("remote_address:");long2prnt(kt_udp_pt->remote_address);toprnq(CR);
		    ws2p("remote_port:");int2prnt(kt_udp_pt->remote_port);toprnq(CR);
		    ws2p("local_port:");int2prnt(kt_udp_pt->local_port);toprnq(CR);
		}
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
		if(kt_udp_no < MAX_UDP_REC){
		    UDP_STRUCT *udp_pt;
		    udp_pt=&udp_st[kt_udp_no];
		    ws2p("remote_address:");long2prnt(udp_pt->remote_address);toprnq('-');
		    ws2p("remote_port:");int2prnt(udp_pt->remote_port);toprnq('-');
		    ws2p("local_address:");long2prnt(udp_pt->local_address);toprnq('-');
		    ws2p("local_port:");int2prnt(udp_pt->local_port);toprnq(CR);
		}
#endif
	    }
	}
#endif
#if ((defined MY_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	{
	    guint i;
	    for(i=0;i<our_oam_no;i++){
		guint kt_udp_no;
		ws2p("our_oam_no:");
		int2prnt(i);toprnq('-');
		int2prnt(FIRST_UDP_HDLC_CH+i);toprnq('-');
		kt_udp_no=hdlc_ch[FIRST_UDP_HDLC_CH+i].cs;
		int2prnt(kt_udp_no);toprnq(CR);
#if (OS == DOS)
		if(kt_udp_no < MAX_KIATEL_UDP_REC){
		    KIATEL_UDP_STRUCT *kt_udp_pt;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    ws2p("remote_address:");long2prnt(kt_udp_pt->remote_address);toprnq(CR);
		    ws2p("remote_port:");int2prnt(kt_udp_pt->remote_port);toprnq(CR);
		    ws2p("local_port:");int2prnt(kt_udp_pt->local_port);toprnq(CR);
		}
#endif
#if (OS == LINUX_COMPATIBLE)
		if(kt_udp_no < MAX_UDP_REC){
		    UDP_STRUCT *udp_pt;
		    udp_pt=&udp_st[kt_udp_no];
		    ws2p("remote_address:");long2prnt(udp_pt->remote_address);toprnq(CR);
		    ws2p("remote_address:");ws2p(udp_pt->remote_address_str);toprnq(CR);
		    ws2p("remote_port:");int2prnt(udp_pt->remote_port);toprnq(CR);
		    ws2p("local_port:");int2prnt(udp_pt->local_port);toprnq(CR);
		}
#endif
	    }
	}
#endif
#if ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE))
	{
	    guint i;
	    for(i=0;i<1;i++){
		guint kt_udp_no;
		ws2p("htby:");
		int2prnt(i);toprnq('-');
		int2prnt(HTBY_HDLC_CH);toprnq('-');
		kt_udp_no=hdlc_ch[HTBY_HDLC_CH].cs;
		int2prnt(kt_udp_no);toprnq(CR);
		if(kt_udp_no < MAX_KIATEL_UDP_REC){
		    KIATEL_UDP_STRUCT *kt_udp_pt;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    ws2p("remote_address:");long2prnt(kt_udp_pt->remote_address);toprnq(CR);
		    ws2p("remote_port:");int2prnt(kt_udp_pt->remote_port);toprnq(CR);
		    ws2p("local_port:");int2prnt(kt_udp_pt->local_port);toprnq(CR);
		}
	    }
	}
#endif
#if ((defined MY_UDP_FOR_OAM) && (defined OPERATOR_SIDE))
	{
	    guint i;
	    for(i=0;i<control_unit_no;i++){
		guint kt_udp_no;
		prnt_string("control_unit_no:");
		prnt_int(i);prnt_char('-');
		prnt_int(FIRST_UDP_HDLC_CH+i);prnt_char('-');
		kt_udp_no=hdlc_ch[FIRST_UDP_HDLC_CH+i].cs;
		prnt_int(kt_udp_no);prnt_char(CR);
		if(kt_udp_no < MAX_KIATEL_UDP_REC){
		    KIATEL_UDP_STRUCT *kt_udp_pt;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    prnt_string("remote_address:");long2prnt(kt_udp_pt->remote_address);prnt_char(CR);
		    prnt_string("remote_port:");prnt_int(kt_udp_pt->remote_port);prnt_char(CR);
		    prnt_string("local_port:");prnt_int(kt_udp_pt->local_port);prnt_char(CR);
		}
	    }
	}
#endif
#ifdef SOFTWARE_SECURITY
	{
#if ((defined KIATEL_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	    if(win_oam_no > site_data.no_win_oam){
		char buf[255];
		sprintf(buf,"Illegal number of windows o&m terminals:%d.\r\nYour maximum number of windows o&m terminals is:%d\r\n",win_oam_no,site_data.no_win_oam);
		do_appropriate_exit(buf);
	    }
#endif
#if ((defined MY_UDP_FOR_OAM) && (defined SYSTEM_SIDE))
	    if(our_oam_no > site_data.no_ethernet_oam){
		char buf[255];
		sprintf(buf,"Illegal number of ethernet o&m terminals:%d.\r\nYour maximum number of ethernet o&m terminals is:%d\r\n",our_oam_no,site_data.no_ethernet_oam);
		do_appropriate_exit(buf);
	    }
#endif
	}
#endif
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef DYNAMIC_IP_ALLOCATION
#if ((defined KIATEL_UDP_INSTALLED) || (defined TCP_INSTALLED))
#ifdef OPERATOR_SIDE
#define MAX_CHAR_PER_LINE	256
// from 0x60->0x80
#ifndef SOFTWARE_INTERRUPT_VECTOR
#define SOFTWARE_INTERRUPT_VECTOR		0x7e
#endif
void read_kiatel_udp_config_file(void)
{
	gchar local_ipv4_address[16];
	gchar local_ipv4_subnet_mask[16];
#ifdef KIATEL_UDP_INSTALLED
	gchar remote_ipv4_address[16];
#endif
	guint local_port, remote_port;
#ifdef OPERATOR_SIDE
	guint control_unit_no;
#endif
#ifdef KIATEL_UDP_INSTALLED
	KIATEL_UDP_STRUCT *kt_udp_pt;
#endif
	FILE *fp;
	gchar *p,line[MAX_CHAR_PER_LINE];

	kt_system_st.int_no=SOFTWARE_INTERRUPT_VECTOR;

	if( (fp=fopen("ipopr.dat","rb")) == NULL_POINTER){ /* file does not exist */
		prnt_string("No ipopr.dat file was found!\r\n");
		return;
	}
	control_unit_no=0;
	local_port=remote_port=(guint)(-1);

	strcpy(local_ipv4_subnet_mask,"255.255.255.0");

again0: while(!feof(fp)){
	    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
		break;
	    if(line[0]=='*')
		continue;
	    p = strtok(line, " \t\r\n");
	    if(p == NULL_POINTER)
		continue;
	    if(strcmp(p,"StartSystem") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"Interrupt") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    kt_system_st.int_no=(unsigned int)strtol(p,NULL_POINTER,16);
			    }
			    else
			    if(strcmp(p,"Ipv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
#if ((defined KIATEL_UDP_INSTALLED) || (defined TCP_INSTALLED))
				if(p != NULL_POINTER)
				    strcpy(local_ipv4_address, p);
#endif
			    }
			    else
			    if(strcmp(p,"Ipv4SubnetMask") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
#if ((defined KIATEL_UDP_INSTALLED) || (defined TCP_INSTALLED))
				if(p != NULL_POINTER)
				    strcpy(local_ipv4_subnet_mask, p);
#endif
			    }
			    else
			    if(strcmp(p,"EndSystem") == 0){
				kt_system_st.hot_ipv4_address=find_ipv4_from_string(local_ipv4_address);
				kt_system_st.ipv4_subnet_mask=find_ipv4_from_string(local_ipv4_subnet_mask);
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#ifdef MY_UDP_FOR_OAM
	    if(strcmp(p,"StartControlUnit") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"LocalPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    local_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"EndControlUnit") == 0){
				guint ucb_no, kt_udp_no;
				SOCKET_STRUCT lsocket;
				if(control_unit_no < 2){
				    lsocket.address=kt_system_st.hot_ipv4_address;
				    lsocket.port=local_port;
				    if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
					ucb_no=start_udp(&lsocket);
					if(ucb_no >= MAX_UCB){
					    prnt_msg(UDP_ERR,0x3007);
					    return;
					}
				    }
				    else
					usock_st[ucb_st[ucb_no].user].refcnt++;
				    kt_udp_no=control_unit_no;
				    if(kt_udp_no >= MAX_KIATEL_UDP_REC){
					prnt_msg(UDP_ERR,0x3008);
					return;
				    }
//				    ucb_st[ucb_no].user=kt_udp_no;
				    kt_udp_pt=&kt_udp_rec[kt_udp_no];
				    kt_udp_pt->state=IN_USE;
				    kt_udp_pt->remote_address=find_ipv4_from_string(remote_ipv4_address);
				    kt_udp_pt->remote_port=remote_port;
				    kt_udp_pt->local_port=local_port;
				    kt_udp_pt->ucb_no=ucb_no;
				    kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_OUR_OAM_TERMINAL_TYPE;
				    kt_udp_pt->upper_layer_no=control_unit_no;
				    kt_udp_pt->netrx_st.arr = (BYTE *)0;
				    kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
				    kt_udp_pt->netrx_st.size = 0;
//				    kt_udp_pt->rx[]
//				    kt_udp_pt->kt_udp_no;
				    kt_udp_pt->rdy=SET;
				    hdlc_ch[FIRST_UDP_HDLC_CH+control_unit_no].ic_type=IC_HDLC_ON_UDP;
				    hdlc_ch[FIRST_UDP_HDLC_CH+control_unit_no].cs=kt_udp_no;
				    sync_flag[FIRST_UDP_HDLC_CH+control_unit_no] = SET;
				    control_unit_no++;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
	    {
		continue;
	    }
	}

	fclose(fp);
	prnt_string("System Ipv4Address:");
	prnt_string(local_ipv4_address);prnt_char('-');
	prnt_long(kt_system_st.hot_ipv4_address);prnt_char(CR);

#ifdef MY_UDP_FOR_OAM
	{
	    guint i;
	    for(i=0;i<control_unit_no;i++){
		guint kt_udp_no;
		prnt_string("control_unit_no:");
		prnt_int(i);prnt_char('-');
		prnt_int(FIRST_UDP_HDLC_CH+i);prnt_char('-');
		kt_udp_no=hdlc_ch[FIRST_UDP_HDLC_CH+i].cs;
		prnt_int(kt_udp_no);prnt_char(CR);
		if(kt_udp_no < MAX_KIATEL_UDP_REC){
		    KIATEL_UDP_STRUCT *kt_udp_pt;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    prnt_string("remote_address:");long2prnt(kt_udp_pt->remote_address);prnt_char(CR);
		    prnt_string("remote_port:");prnt_int(kt_udp_pt->remote_port);prnt_char(CR);
		    prnt_string("local_port:");prnt_int(kt_udp_pt->local_port);prnt_char(CR);
		}
	    }
	}
#endif
}
#endif
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef DYNAMIC_IP_ALLOCATION
#if ((defined KIATEL_UDP_INSTALLED) ||\
     (defined OAM_ON_UDP_CLIENT_SERVER_SIDE) || (defined TCP_INSTALLED))
#ifdef SYSTEM_SIDE
#define MAX_CHAR_PER_LINE	256
// from 0x60->0x80
#ifndef SOFTWARE_INTERRUPT_VECTOR
#define SOFTWARE_INTERRUPT_VECTOR		0x7e
#endif
void read_kiatel_udp_config_file(void)
{
	gchar hot_ipv4_address[16];
	gchar local_ipv4_subnet_mask[16];
#if ((defined KIATEL_UDP_INSTALLED) || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE))
	gchar remote_ipv4_address[16];
#ifdef HTBY_LINK_ON_MY_UDP
	gchar standby_ipv4_address[16];
#endif
#endif
	guint hot_port, standby_port, remote_port;
	FILE *fp;
	gchar *p,line[MAX_CHAR_PER_LINE];

	kt_system_st.int_no=SOFTWARE_INTERRUPT_VECTOR;

#ifdef KIATEL_UDP_FOR_OAM
	kt_system_st.win_oam_no=0;
#endif

#ifdef SYSCONUDP
	kt_system_st.sysconudp_no=0;
#endif

#ifdef MY_UDP_FOR_OAM
	kt_system_st.our_oam_no=0;
#endif

	strcpy(fnptr,"ipsys.dat");
	if( (fp=fopen(fnbuf,"rb")) == NULL_POINTER){ /* file does not exist */
		ws2p("No ipsys.dat file was found!\r\n");
		return;
	}
	hot_port=standby_port=remote_port=(guint)(-1);

	strcpy(local_ipv4_subnet_mask,"255.255.255.0");

again0: while(!feof(fp)){
	    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
		break;
	    if(line[0]=='*')
		continue;
	    p = strtok(line, " \t\r\n");
	    if(p == NULL_POINTER)
		continue;
	    if(strcmp(p,"StartSystems") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"Interrupt") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    kt_system_st.int_no=(unsigned int)strtol(p,NULL_POINTER,16);
			    }
			    else
			    if(strcmp(p,"HotIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
#if ((defined KIATEL_UDP_INSTALLED) || (defined TCP_INSTALLED))
				if(p != NULL_POINTER)
				    strcpy(hot_ipv4_address, p);
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
				{
				    struct hostent *hp;
				    gchar name_str[1024];

				    gethostname(name_str,1024);
				    hp=gethostbyname(name_str);
				    if(hp != NULL_POINTER){
					if(hp->h_addr_list[0] != NULL_POINTER){
					    // first ip
					    strcpy(hot_ipv4_address,inet_ntoa(*(struct in_addr *)(hp->h_addr_list[0])));
					    strcpy(kt_system_st.hot_ipv4_address_str, hot_ipv4_address);
					}
				    }
				    if(is_valid_ip(hot_ipv4_address) == 0){
					printf("Couldn't get our IP address from our hostname\r\n");
					exit(1);
				    }
				}
#endif
			    }
			    else
			    if(strcmp(p,"Ipv4SubnetMask") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
#if ((defined KIATEL_UDP_INSTALLED) || (defined TCP_INSTALLED))
				if(p != NULL_POINTER)
				    strcpy(local_ipv4_subnet_mask, p);
#endif
			    }
#ifdef HTBY_LINK_ON_MY_UDP
			    else
			    if(strcmp(p,"HotPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    hot_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"StandbyIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(standby_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"StandbyPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    standby_port=atoi(p);
			    }
#endif
			    else
			    if(strcmp(p,"EndSystems") == 0){
				kt_system_st.hot_ipv4_address=find_ipv4_from_string(hot_ipv4_address);
				kt_system_st.ipv4_subnet_mask=find_ipv4_from_string(local_ipv4_subnet_mask);
#ifdef HTBY_LINK_ON_MY_UDP
				kt_system_st.standby_ipv4_address=find_ipv4_from_string(standby_ipv4_address);
				kt_system_st.htby_link_hot_port=hot_port;
				kt_system_st.htby_link_standby_port=standby_port;
#endif
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#ifdef KIATEL_UDP_FOR_OAM
	    if(strcmp(p,"StartWinOamTerminal") == 0){
		guchar send_heartbeat=1;
		guchar is_terminal=0;
		guchar send_alarm=1;
		guchar send_traffic=1;
		guchar send_billing=1;
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"HotPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    hot_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"StandbyPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    standby_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"SendHeartBeat") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    send_heartbeat=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"IsTerminal") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    is_terminal=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"SendAlarm") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    send_alarm=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"SendTraffic") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    send_traffic=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"SendBilling") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    send_billing=(atoi(p) == 0) ? 0:1;
			    }
			    else
			    if(strcmp(p,"EndWinOamTerminal") == 0){

				if(kt_system_st.win_oam_no < NO_KIATEL_UDP_OAM_TERMINAL){
#if ((defined KIATEL_UDP_INSTALLED) || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE))
				    kt_system_st.win_oam_terminal_hot_port[kt_system_st.win_oam_no]=hot_port;
				    kt_system_st.win_oam_terminal_standby_port[kt_system_st.win_oam_no]=standby_port;
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
				    strcpy(&kt_system_st.win_oam_terminal_remote_ipv4_address_str[kt_system_st.win_oam_no][0], remote_ipv4_address);
#endif
				    kt_system_st.win_oam_terminal_remote_ipv4_address[kt_system_st.win_oam_no]=find_ipv4_from_string(remote_ipv4_address);
				    kt_system_st.win_oam_terminal_remote_port[kt_system_st.win_oam_no]=remote_port;
#endif
				    winoam[kt_system_st.win_oam_no].send_heartbeat=send_heartbeat;
				    winoam[kt_system_st.win_oam_no].is_terminal=is_terminal;
				    winoam[kt_system_st.win_oam_no].send_alarm=send_alarm;
				    winoam[kt_system_st.win_oam_no].send_traffic=send_traffic;
				    winoam[kt_system_st.win_oam_no].send_billing=send_billing;
				    kt_system_st.win_oam_no++;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
#ifdef SYSCONUDP
	    if(strcmp(p,"StartSysConTerminal") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"HotPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    hot_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"StandbyPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    standby_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"EndSysConTerminal") == 0){

				if(kt_system_st.sysconudp_no < NO_KIATEL_UDP_SYSCONUDP){
#if ((defined KIATEL_UDP_INSTALLED) || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE))
				    kt_system_st.sysconudp_hot_port[kt_system_st.sysconudp_no]=hot_port;
				    kt_system_st.sysconudp_standby_port[kt_system_st.sysconudp_no]=standby_port;
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
				    strcpy(&kt_system_st.sysconudp_remote_ipv4_address_str[kt_system_st.sysconudp_no][0], remote_ipv4_address);
#endif
				    kt_system_st.sysconudp_remote_ipv4_address[kt_system_st.sysconudp_no]=find_ipv4_from_string(remote_ipv4_address);
				    kt_system_st.sysconudp_remote_port[kt_system_st.sysconudp_no]=remote_port;
#endif
				    kt_system_st.sysconudp_no++;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
#ifdef MY_UDP_FOR_OAM
	    if(strcmp(p,"StartOurOamTerminal") == 0){
		while(1){
		    if(feof(fp))
			break;
		    if (fgets(line,MAX_CHAR_PER_LINE,fp) == NULL_POINTER)
			break;
		    if(line[0] != '*'){
			p = strtok(line, "= \t\r\n");
			if(p != NULL_POINTER){
			    if(strcmp(p,"HotPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    hot_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"StandbyPort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    standby_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"RemoteIpv4Address") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    strcpy(remote_ipv4_address, p);
			    }
			    else
			    if(strcmp(p,"RemotePort") == 0){
				p=strtok(NULL_POINTER, " \t\r\n");
				if(p != NULL_POINTER)
				    remote_port=atoi(p);
			    }
			    else
			    if(strcmp(p,"EndOurOamTerminal") == 0){
				if(kt_system_st.our_oam_no < NO_OPRTR){
				    kt_system_st.our_oam_terminal_hot_port[kt_system_st.our_oam_no]=hot_port;
				    kt_system_st.our_oam_terminal_standby_port[kt_system_st.our_oam_no]=standby_port;
				    kt_system_st.our_oam_terminal_remote_ipv4_address[kt_system_st.our_oam_no]=find_ipv4_from_string(remote_ipv4_address);
				    kt_system_st.our_oam_terminal_remote_port[kt_system_st.our_oam_no]=remote_port;
				    kt_system_st.our_oam_no++;
				}
				goto again0;
			    }
			}
		    }
		}
	    }
	    else
#endif
	    {
		continue;
	    }
	}
	fclose(fp);
#ifdef SOFTWARE_SECURITY
	{
#ifdef KIATEL_UDP_FOR_OAM
	    if(kt_system_st.win_oam_no > site_data.no_win_oam){
		char buf[255];
		sprintf(buf,"Illegal number of windows o&m terminals:%d.\r\nYour maximum number of windows o&m terminals is:%d\r\n",kt_system_st.win_oam_no,site_data.no_win_oam);
		do_appropriate_exit(buf);
	    }
#endif
#ifdef MY_UDP_FOR_OAM
	    if(kt_system_st.our_oam_no > site_data.no_ethernet_oam){
		char buf[255];
		sprintf(buf,"Illegal number of ethernet o&m terminals:%d.\r\nYour maximum number of ethernet o&m terminals is:%d\r\n",kt_system_st.our_oam_no,site_data.no_ethernet_oam);
		do_appropriate_exit(buf);
	    }
#endif
	}
#endif
}
#endif
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef DYNAMIC_IP_ALLOCATION
#if ((defined KIATEL_UDP_INSTALLED) || (defined TCP_INSTALLED))
#if ((defined SYSTEM_SIDE) && (defined HOT_STANDBY))
void reactivate_kiatel_udp_connections(void)
{
//#ifdef DEBUG_ACT
	ws2p("reactivate_kiatel_udp_connections\r\n");
//#endif
	activate_kiatel_udp_connections();
	if(oam_iface_no < MAX_IFACE){
		if (thisside.hot == SET)
			iface_st[oam_iface_no].addr=kt_system_st.hot_ipv4_address;
		else
			iface_st[oam_iface_no].addr=kt_system_st.standby_ipv4_address;
	}
	send_gratuituos_arp();
}
#endif
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef DYNAMIC_IP_ALLOCATION
#if ((defined KIATEL_UDP_INSTALLED) ||\
     (defined OAM_ON_UDP_CLIENT_SERVER_SIDE) || (defined TCP_INSTALLED))
#ifdef SYSTEM_SIDE
void activate_kiatel_udp_connections(void)
{
	gchar address_str[16];
#ifdef KIATEL_UDP_FOR_OAM
	int i;
#endif
#ifdef KIATEL_UDP_INSTALLED
	KIATEL_UDP_STRUCT *kt_udp_pt;
#endif

#ifdef DEBUG_ACT
	ws2p("activate_kiatel_udp_connections\r\n");
#endif


#ifdef KIATEL_UDP_FOR_OAM
	for(i=0;i<NO_KIATEL_UDP_OAM_TERMINAL;i++){
		winoam[i].link_type=(BYTE)(-1);
		winoam[i].link_no=(unsigned int)(-1);
		winoam[i].heartbeat=0;
	}
#endif
#ifdef SYSCONUDP
	guint j;
	for(j=0;j<NO_KIATEL_UDP_SYSCONUDP;j++){
	    sysconudp[j].link_no=(unsigned int)(-1);
	}
#endif
#ifdef KIATEL_UDP_FOR_OAM
	{
		guint kt_udp_no, win_oam_no;
		for(win_oam_no=0;win_oam_no<kt_system_st.win_oam_no;win_oam_no++){
#ifdef KIATEL_UDP_INSTALLED
			guint ucb_no;
			SOCKET_STRUCT lsocket;
#ifdef HOT_STANDBY
			{
				guint old_udp_no, local_port;

				if (thisside.hot == SET){	// it has been standby in previous connection
					local_port=kt_system_st.win_oam_terminal_standby_port[win_oam_no];
				}
				else{
					local_port=kt_system_st.win_oam_terminal_hot_port[win_oam_no];
				}
				old_udp_no=lookup_kiatel_udp_hash_table(
					kt_system_st.win_oam_terminal_remote_ipv4_address[win_oam_no],
					kt_system_st.win_oam_terminal_remote_port[win_oam_no],
					local_port);
				if(old_udp_no < MAX_KIATEL_UDP_REC) {
					rls_kiatel_udp_rec(old_udp_no);
				}
			}
#endif
#ifdef HOT_STANDBY
			if (thisside.hot == SET)
#endif
			{
				lsocket.address=kt_system_st.hot_ipv4_address;
				lsocket.port=kt_system_st.win_oam_terminal_hot_port[win_oam_no];
			}
#ifdef HOT_STANDBY
			else{
				lsocket.address=kt_system_st.standby_ipv4_address;
				lsocket.port=kt_system_st.win_oam_terminal_standby_port[win_oam_no];
			}
#endif
			if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
				ucb_no=start_udp(&lsocket);
				if(ucb_no >= MAX_UCB){
					mes2prnti(mess[UDP_ERR],0x3007);
					return;
				}
			}
			else
				usock_st[ucb_st[ucb_no].user].refcnt++;
			kt_udp_no=find_free_kiatel_udp_rec();
			if(kt_udp_no >= MAX_KIATEL_UDP_REC){
				mes2prnti(mess[UDP_ERR],0x3008);
				return;
			}
//			ucb_st[ucb_no].user=kt_udp_no;
			kt_udp_pt=&kt_udp_rec[kt_udp_no];
			kt_udp_pt->state=IN_USE;
			kt_udp_pt->remote_address=kt_system_st.win_oam_terminal_remote_ipv4_address[win_oam_no];
			kt_udp_pt->remote_port=kt_system_st.win_oam_terminal_remote_port[win_oam_no];
#ifdef HOT_STANDBY
			if (thisside.hot == SET)
#endif
			{
				kt_udp_pt->local_port=kt_system_st.win_oam_terminal_hot_port[win_oam_no];
			}
#ifdef HOT_STANDBY
			else{
				kt_udp_pt->local_port=kt_system_st.win_oam_terminal_standby_port[win_oam_no];
			}
#endif
			kt_udp_pt->ucb_no=ucb_no;
			kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_WIN_OAM_TERMINAL_TYPE;
			kt_udp_pt->upper_layer_no=win_oam_no;
			kt_udp_pt->netrx_st.arr = kt_g_malloc(KIATEL_UDP_NETBUF_SIZE);
			kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
			kt_udp_pt->netrx_st.size = KIATEL_UDP_NETBUF_SIZE;
//			kt_udp_pt->rx[]
//			kt_udp_pt->kt_udp_no;
			kt_udp_pt->rdy=SET;
			insert_kiatel_udp_hash_table(kt_udp_no);
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
			UDP_STRUCT *udp_pt;
			gint sockfd;
			struct sockaddr_in my_addr;

			kt_udp_no=find_free_udp_rec();
			if(kt_udp_no >= MAX_UDP_REC){
				mes2prnti(mess[UDP_ERR],0x3009);
				return;
			}
			udp_pt=&udp_st[kt_udp_no];
			udp_pt->state=IN_USE;
			udp_pt->client_flag=SET;
			udp_pt->upper_layer_type=UDP_WIN_OAM_TYPE;
			udp_pt->upper_layer_no=win_oam_no;

			strcpy(udp_pt->remote_address_str,  kt_system_st.win_oam_terminal_remote_ipv4_address_str[win_oam_no]);
			udp_pt->remote_address=kt_system_st.win_oam_terminal_remote_ipv4_address[win_oam_no];
			udp_pt->remote_port=kt_system_st.win_oam_terminal_remote_port[win_oam_no];
			strcpy(udp_pt->local_address_str, kt_system_st.hot_ipv4_address_str);
			udp_pt->local_address=kt_system_st.hot_ipv4_address;
			udp_pt->local_port=kt_system_st.win_oam_terminal_hot_port[win_oam_no];
			udp_pt->rdy=SET;
			sockfd = socket(AF_INET, SOCK_DGRAM, 0);
			if (sockfd < 0){
				mes2prnti(mess[UDP_ERR],0x3010);
				return;
			}
			fcntl(sockfd,F_SETFL,O_NONBLOCK);
			memset(&my_addr, 0, sizeof(my_addr));
			my_addr.sin_family = AF_INET;		      // host byte order
			my_addr.sin_port = htons(udp_pt->local_port); // short ,network byte order
			my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // automatically fill with my IP
			if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
				if(errno == EADDRINUSE){
					guint socket_no;
					SOCKET_STRUCT *socket_pt;
					close(sockfd);
					for(socket_no=first_udp_allocated_socket;socket_no<MAX_SOCKET_REC;socket_no=socket_pt->rp){
						socket_pt=&socket_st[socket_no];
						if((socket_pt->address == udp_pt->local_address) &&
						   (socket_pt->port == udp_pt->local_port)){
							sockfd=socket_pt->sockfd;
							break;
						}
					}
					if(socket_no >= MAX_SOCKET_REC){
						mes2prnti(mess[UDP_ERR],0x3013);
						return;
					}
					udp_pt->tx_sockfd=sockfd;
					udp_pt->rx_sockfd=sockfd;
					udp_pt->socket_rec_no=socket_no;
					socket_pt->no_user++;
				}
				else{
					mes2prnti(mess[UDP_ERR],0x3011);
					return;
				}
			}
			else{
				udp_pt->tx_sockfd=sockfd;
				udp_pt->rx_sockfd=sockfd;
				if(find_free_udp_socket_rec_set_fields(kt_udp_no) >= MAX_SOCKET_REC){
					mes2prnti(mess[UDP_ERR],0x3012);
					return;
				}
			}
			insert_udp_hash_table(kt_udp_no);
#endif
			winoam[win_oam_no].link_no=kt_udp_no;
			winoam[win_oam_no].link_type=LINK_KIATEL_UDP ;
		}
	}
#endif

#ifdef SYSCONUDP
	{
		guint kt_udp_no, sys_con_no;
		for(sys_con_no=0;sys_con_no<kt_system_st.sysconudp_no;sys_con_no++){
#ifdef KIATEL_UDP_INSTALLED
		    guint ucb_no;
		    SOCKET_STRUCT lsocket;
#ifdef HOT_STANDBY
		    {
			guint old_udp_no, local_port;
			if (thisside.hot == SET){	// it has been standby in previous connection
			    local_port=kt_system_st.sysconudp_standby_port[sys_con_no];
			}
			else{
			    local_port=kt_system_st.sysconudp_hot_port[sys_con_no];
			}
			old_udp_no=lookup_kiatel_udp_hash_table(
			    kt_system_st.sysconudp_remote_ipv4_address[sys_con_no],
			    kt_system_st.sysconudp_remote_port[sys_con_no],
			    local_port);
			if(old_udp_no < MAX_KIATEL_UDP_REC) {
			    rls_kiatel_udp_rec(old_udp_no);
			}
		    }
#endif
#ifdef HOT_STANDBY
		    if (thisside.hot == SET)
#endif
		    {
			lsocket.address=kt_system_st.hot_ipv4_address;
			lsocket.port=kt_system_st.sysconudp_hot_port[sys_con_no];
		    }
#ifdef HOT_STANDBY
		    else{
			lsocket.address=kt_system_st.standby_ipv4_address;
			lsocket.port=kt_system_st.sysconudp_standby_port[sys_con_no];
		    }
#endif
		    if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
			    ucb_no=start_udp(&lsocket);
			    if(ucb_no >= MAX_UCB){
				    mes2prnti(mess[UDP_ERR],0x3007);
				    return;
			    }
		    }
		    else
			    usock_st[ucb_st[ucb_no].user].refcnt++;
		    kt_udp_no=find_free_kiatel_udp_rec();
		    if(kt_udp_no >= MAX_KIATEL_UDP_REC){
			    mes2prnti(mess[UDP_ERR],0x3008);
			    return;
		    }
//		    ucb_st[ucb_no].user=kt_udp_no;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    kt_udp_pt->state=IN_USE;
		    kt_udp_pt->remote_address=kt_system_st.sysconudp_remote_ipv4_address[sys_con_no];
		    kt_udp_pt->remote_port=kt_system_st.sysconudp_remote_port[sys_con_no];
#ifdef HOT_STANDBY
		    if (thisside.hot == SET)
#endif
		    {
			kt_udp_pt->local_port=kt_system_st.sysconudp_hot_port[sys_con_no];
		    }
#ifdef HOT_STANDBY
		    else{
			kt_udp_pt->local_port=kt_system_st.sysconudp_standby_port[sys_con_no];
		    }
#endif
		    kt_udp_pt->ucb_no=ucb_no;
		    kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_SYSCONUDP_TYPE;
		    kt_udp_pt->upper_layer_no=sys_con_no;
		    kt_udp_pt->netrx_st.arr = kt_g_malloc(KIATEL_UDP_NETBUF_SIZE);
		    kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
		    kt_udp_pt->netrx_st.size = KIATEL_UDP_NETBUF_SIZE;
//		    kt_udp_pt->rx[]
//		    kt_udp_pt->kt_udp_no;
		    kt_udp_pt->rdy=SET;
		    insert_kiatel_udp_hash_table(kt_udp_no);
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
		    UDP_STRUCT *udp_pt;
		    gint sockfd;
		    struct sockaddr_in my_addr;

		    kt_udp_no=find_free_udp_rec();
		    if(kt_udp_no >= MAX_UDP_REC){
			    mes2prnti(mess[UDP_ERR],0x3009);
			    return;
		    }
		    udp_pt=&udp_st[kt_udp_no];
		    udp_pt->state=IN_USE;
		    udp_pt->client_flag=SET;
		    udp_pt->upper_layer_type=UDP_SYSCONUDP_TYPE;  // in future will be defined!
		    udp_pt->upper_layer_no=sys_con_no;

		    strcpy(udp_pt->remote_address_str,	kt_system_st.sysconudp_remote_ipv4_address_str[sys_con_no]);
		    udp_pt->remote_address=kt_system_st.sysconudp_remote_ipv4_address[sys_con_no];
		    udp_pt->remote_port=kt_system_st.sysconudp_remote_port[sys_con_no];
		    strcpy(udp_pt->local_address_str, kt_system_st.hot_ipv4_address_str);
		    udp_pt->local_address=kt_system_st.hot_ipv4_address;
		    udp_pt->local_port=kt_system_st.sysconudp_hot_port[sys_con_no];
		    udp_pt->rdy=SET;
		    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		    if (sockfd < 0){
			    mes2prnti(mess[UDP_ERR],0x3010);
			    return;
		    }
		    fcntl(sockfd,F_SETFL,O_NONBLOCK);
		    memset(&my_addr, 0, sizeof(my_addr));
		    my_addr.sin_family = AF_INET;		  // host byte order
		    my_addr.sin_port = htons(udp_pt->local_port); // short ,network byte order
		    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // automatically fill with my IP
		    if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
			if(errno == EADDRINUSE){
			    guint socket_no;
			    SOCKET_STRUCT *socket_pt;
			    close(sockfd);
			    for(socket_no=first_udp_allocated_socket;socket_no<MAX_SOCKET_REC;socket_no=socket_pt->rp){
				socket_pt=&socket_st[socket_no];
				if((socket_pt->address == udp_pt->local_address) &&
				   (socket_pt->port == udp_pt->local_port)){
				    sockfd=socket_pt->sockfd;
				    break;
				}
			    }
			    if(socket_no >= MAX_SOCKET_REC){
				    mes2prnti(mess[UDP_ERR],0x3013);
				    return;
			    }
			    udp_pt->tx_sockfd=sockfd;
			    udp_pt->rx_sockfd=sockfd;
			    udp_pt->socket_rec_no=socket_no;
			    socket_pt->no_user++;
			}
			else{
			    mes2prnti(mess[UDP_ERR],0x3011);
			    return;
			}
		    }
		    else{
			udp_pt->tx_sockfd=sockfd;
			udp_pt->rx_sockfd=sockfd;
			if(find_free_udp_socket_rec_set_fields(kt_udp_no) >= MAX_SOCKET_REC){
			    mes2prnti(mess[UDP_ERR],0x3012);
			    return;
			}
		    }
		    insert_udp_hash_table(kt_udp_no);
#endif
		    sysconudp[sys_con_no].link_no=kt_udp_no;
		}
	}
#endif

#ifdef MY_UDP_FOR_OAM
	{
		guint ucb_no, kt_udp_no, our_oam_no;
		SOCKET_STRUCT lsocket;
		for(our_oam_no=0;our_oam_no<kt_system_st.our_oam_no;our_oam_no++){
#ifdef HOT_STANDBY
			{
				guint old_udp_no, local_port;

				if (thisside.hot == SET){	// it has been standby in previous connection
					local_port=kt_system_st.our_oam_terminal_standby_port[our_oam_no];
				}
				else{
					local_port=kt_system_st.our_oam_terminal_hot_port[our_oam_no];
				}
				old_udp_no=lookup_kiatel_udp_hash_table(
					kt_system_st.our_oam_terminal_remote_ipv4_address[our_oam_no],
					kt_system_st.our_oam_terminal_remote_port[our_oam_no],
					local_port);
				if(old_udp_no < MAX_KIATEL_UDP_REC) {
					rls_kiatel_udp_rec(old_udp_no);
				}
			}
#endif
#ifdef HOT_STANDBY
			if (thisside.hot == SET)

#endif
			{
				lsocket.address=kt_system_st.hot_ipv4_address;
				lsocket.port=kt_system_st.our_oam_terminal_hot_port[our_oam_no];
			}
#ifdef HOT_STANDBY
			else{
				lsocket.address=kt_system_st.standby_ipv4_address;
				lsocket.port=kt_system_st.our_oam_terminal_standby_port[our_oam_no];
			}
#endif

			if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
				ucb_no=start_udp(&lsocket);
				if(ucb_no >= MAX_UCB){
					mes2prnti(mess[UDP_ERR],0x3007);
					return;
				}
			}
			else
				usock_st[ucb_st[ucb_no].user].refcnt++;
			kt_udp_no=find_free_kiatel_udp_rec();
			if(kt_udp_no >= MAX_KIATEL_UDP_REC){
				mes2prnti(mess[UDP_ERR],0x3008);
				return;
			}
//			ucb_st[ucb_no].user=kt_udp_no;
			kt_udp_pt=&kt_udp_rec[kt_udp_no];
			kt_udp_pt->state=IN_USE;
			kt_udp_pt->remote_address=kt_system_st.our_oam_terminal_remote_ipv4_address[our_oam_no];
			kt_udp_pt->remote_port=kt_system_st.our_oam_terminal_remote_port[our_oam_no];
#ifdef HOT_STANDBY
			if (thisside.hot == SET)
#endif
			{
				kt_udp_pt->local_port=kt_system_st.our_oam_terminal_hot_port[our_oam_no];
			}
#ifdef HOT_STANDBY
			else{
				kt_udp_pt->local_port=kt_system_st.our_oam_terminal_standby_port[our_oam_no];
			}
#endif
			kt_udp_pt->ucb_no=ucb_no;
			kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_OUR_OAM_TERMINAL_TYPE;
			kt_udp_pt->upper_layer_no=our_oam_no;
			kt_udp_pt->netrx_st.arr = (BYTE *)0;
			kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
			kt_udp_pt->netrx_st.size = 0;
//			kt_udp_pt->rx[]
//			kt_udp_pt->kt_udp_no;
			kt_udp_pt->rdy=SET;
			insert_kiatel_udp_hash_table(kt_udp_no);
			// or use oam[our_oam_no].link_no instead of FIRST_UDP_HDLC_CH+our_oam_no
			hdlc_ch[FIRST_UDP_HDLC_CH+our_oam_no].ic_type=IC_HDLC_ON_UDP;
			hdlc_ch[FIRST_UDP_HDLC_CH+our_oam_no].cs=kt_udp_no;
			sync_flag[FIRST_UDP_HDLC_CH+our_oam_no] = SET;
		}
	}
#endif

#ifdef HTBY_LINK_ON_MY_UDP
	{
		guint ucb_no, kt_udp_no;
		SOCKET_STRUCT lsocket;
		{
			guint old_udp_no, local_port, remote_port;
			guint32 remote_address;

			if (thisside.hot == SET){	// it has been standby in previous connection
				local_port=kt_system_st.htby_link_standby_port;
				remote_address=kt_system_st.hot_ipv4_address;
				remote_port=kt_system_st.htby_link_hot_port;
			}
			else{
				local_port=kt_system_st.htby_link_hot_port;
				remote_address=kt_system_st.standby_ipv4_address;
				remote_port=kt_system_st.htby_link_standby_port;
			}
			old_udp_no=lookup_kiatel_udp_hash_table(remote_address, remote_port, local_port);
			if(old_udp_no < MAX_KIATEL_UDP_REC) {
				rls_kiatel_udp_rec(old_udp_no);
			}
		}

		if (thisside.hot == SET)
		{
			lsocket.address=kt_system_st.hot_ipv4_address;
			lsocket.port=kt_system_st.htby_link_hot_port;
		}
		else{
			lsocket.address=kt_system_st.standby_ipv4_address;
			lsocket.port=kt_system_st.htby_link_standby_port;
		}
		if((ucb_no = lookup_ucb(&lsocket)) >= MAX_UCB){
			ucb_no=start_udp(&lsocket);
			if(ucb_no >= MAX_UCB){
				mes2prnti(mess[UDP_ERR],0x3007);
				return;
			}
		}
		else
			usock_st[ucb_st[ucb_no].user].refcnt++;
		kt_udp_no=find_free_kiatel_udp_rec();
		if(kt_udp_no >= MAX_KIATEL_UDP_REC){
			mes2prnti(mess[UDP_ERR],0x3008);
			return;
		}
//		ucb_st[ucb_no].user=kt_udp_no;
		kt_udp_pt=&kt_udp_rec[kt_udp_no];
		kt_udp_pt->state=IN_USE;
		if (thisside.hot == SET)
		{
			kt_udp_pt->remote_address=kt_system_st.standby_ipv4_address;
			kt_udp_pt->remote_port=kt_system_st.htby_link_standby_port;
			kt_udp_pt->local_port=kt_system_st.htby_link_hot_port;
		}
		else{
			kt_udp_pt->remote_address=kt_system_st.hot_ipv4_address;
			kt_udp_pt->remote_port=kt_system_st.htby_link_hot_port;
			kt_udp_pt->local_port=kt_system_st.htby_link_standby_port;
		}
		kt_udp_pt->ucb_no=ucb_no;
		kt_udp_pt->upper_layer_type=KIATEL_UDP_UPPER_LAYER_HTBY_TYPE;
		kt_udp_pt->upper_layer_no=0;
		kt_udp_pt->netrx_st.arr = (BYTE *)0;
		kt_udp_pt->netrx_st.front = kt_udp_pt->netrx_st.rear = 0;
		kt_udp_pt->netrx_st.size = 0;
//		kt_udp_pt->rx[]
//		kt_udp_pt->kt_udp_no;
		kt_udp_pt->rdy=SET;
		insert_kiatel_udp_hash_table(kt_udp_no);
		hdlc_ch[HTBY_HDLC_CH].ic_type=IC_HDLC_ON_UDP;
		hdlc_ch[HTBY_HDLC_CH].cs=kt_udp_no;
		sync_flag[HTBY_HDLC_CH] = SET;
	}
#endif
	prnt_string("System Ipv4Address:");
#ifdef HOT_STANDBY
	if (thisside.hot == SET)
#endif
	{
		change_ipv4_to_string(address_str, kt_system_st.hot_ipv4_address);
		prnt_string(address_str);prnt_char('-');
		prnt_long(kt_system_st.hot_ipv4_address);prnt_char(CR);
	}
#ifdef HOT_STANDBY
	else{
		change_ipv4_to_string(address_str, kt_system_st.standby_ipv4_address);
		prnt_string(address_str);prnt_char('-');
		prnt_long(kt_system_st.standby_ipv4_address);prnt_char(CR);
	}
#endif

#ifdef KIATEL_UDP_FOR_OAM
	{
	    guint i;
	    for(i=0;i<kt_system_st.win_oam_no;i++){
		guint kt_udp_no;
		ws2p("win_oam_no:");
		int2prnt(i);toprnq('-');
		kt_udp_no=winoam[i].link_no;
		int2prnt(kt_udp_no);toprnq(CR);
#ifdef KIATEL_UDP_INSTALLED
		if(kt_udp_no < MAX_KIATEL_UDP_REC){
		    KIATEL_UDP_STRUCT *kt_udp_pt;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    ws2p("remote_address:");
		    change_ipv4_to_string(address_str, kt_udp_pt->remote_address);
		    prnt_string(address_str);prnt_char('-');
		    long2prnt(kt_udp_pt->remote_address);toprnq(CR);
		    ws2p("remote_port:");int2prnt(kt_udp_pt->remote_port);toprnq(CR);
		    ws2p("local_port:");int2prnt(kt_udp_pt->local_port);toprnq(CR);
		}
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
		if(kt_udp_no < MAX_UDP_REC){
		    UDP_STRUCT *udp_pt;
		    udp_pt=&udp_st[kt_udp_no];
		    ws2p("remote_address:");long2prnt(udp_pt->remote_address);toprnq('-');
		    ws2p("remote_port:");int2prnt(udp_pt->remote_port);toprnq('-');
		    ws2p("local_address:");long2prnt(udp_pt->local_address);toprnq('-');
		    ws2p("local_port:");int2prnt(udp_pt->local_port);toprnq(CR);
		}
#endif
	    }
	}
#endif
#ifdef SYSCONUDP
	{
	    guint i;
	    for(i=0;i<kt_system_st.sysconudp_no;i++){
		guint kt_udp_no;
		ws2p("sys_con_no:");
		int2prnt(i);toprnq('-');
		kt_udp_no=sysconudp[i].link_no;
		int2prnt(kt_udp_no);toprnq(CR);
#ifdef KIATEL_UDP_INSTALLED
		if(kt_udp_no < MAX_KIATEL_UDP_REC){
		    KIATEL_UDP_STRUCT *kt_udp_pt;
		    kt_udp_pt=&kt_udp_rec[kt_udp_no];
		    ws2p("remote_address:");
		    change_ipv4_to_string(address_str, kt_udp_pt->remote_address);
		    prnt_string(address_str);prnt_char('-');
		    long2prnt(kt_udp_pt->remote_address);toprnq(CR);
		    ws2p("remote_port:");int2prnt(kt_udp_pt->remote_port);toprnq(CR);
		    ws2p("local_port:");int2prnt(kt_udp_pt->local_port);toprnq(CR);
		}
#endif
#ifdef OAM_ON_UDP_CLIENT_SERVER_SIDE
		if(kt_udp_no < MAX_UDP_REC){
		    UDP_STRUCT *udp_pt;
		    udp_pt=&udp_st[kt_udp_no];
		    ws2p("remote_address:");long2prnt(udp_pt->remote_address);toprnq('-');
		    ws2p("remote_port:");int2prnt(udp_pt->remote_port);toprnq('-');
		    ws2p("local_address:");long2prnt(udp_pt->local_address);toprnq('-');
		    ws2p("local_port:");int2prnt(udp_pt->local_port);toprnq(CR);
		}
#endif
	    }
	}
#endif
#ifdef MY_UDP_FOR_OAM
	{
		guint i;
		for(i=0;i<kt_system_st.our_oam_no;i++){
			guint kt_udp_no;
			ws2p("our_oam_no:");
			int2prnt(i);toprnq('-');
			int2prnt(FIRST_UDP_HDLC_CH+i);toprnq('-');
			kt_udp_no=hdlc_ch[FIRST_UDP_HDLC_CH+i].cs;
			int2prnt(kt_udp_no);toprnq(CR);
			if(kt_udp_no < MAX_KIATEL_UDP_REC){
				KIATEL_UDP_STRUCT *kt_udp_pt;
				kt_udp_pt=&kt_udp_rec[kt_udp_no];
				ws2p("remote_address:");
				change_ipv4_to_string(address_str, kt_udp_pt->remote_address);
				prnt_string(address_str);prnt_char('-');
				long2prnt(kt_udp_pt->remote_address);toprnq(CR);
				ws2p("remote_port:");int2prnt(kt_udp_pt->remote_port);toprnq(CR);
				ws2p("local_port:");int2prnt(kt_udp_pt->local_port);toprnq(CR);
			}
		}
	}
#endif
#ifdef HTBY_LINK_ON_MY_UDP
	{
		guint i;
		for(i=0;i<1;i++){
			guint kt_udp_no;
			ws2p("htby:");
			int2prnt(i);toprnq('-');
			int2prnt(HTBY_HDLC_CH);toprnq('-');
			kt_udp_no=hdlc_ch[HTBY_HDLC_CH].cs;
			int2prnt(kt_udp_no);toprnq(CR);
			if(kt_udp_no < MAX_KIATEL_UDP_REC){
				KIATEL_UDP_STRUCT *kt_udp_pt;
				kt_udp_pt=&kt_udp_rec[kt_udp_no];
				ws2p("remote_address:");
				change_ipv4_to_string(address_str, kt_udp_pt->remote_address);
				prnt_string(address_str);prnt_char('-');
				long2prnt(kt_udp_pt->remote_address);toprnq(CR);
				ws2p("remote_port:");int2prnt(kt_udp_pt->remote_port);toprnq(CR);
				ws2p("local_port:");int2prnt(kt_udp_pt->local_port);toprnq(CR);
			}
		}
	}
#endif
}
#endif
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#if (								   \
      ( 							   \
	( (defined  H248_INSTALLED) || (defined SNMP_INSTALLED) || \
	  (defined M2UA_ON_SGSIDE)  || (defined IUA_ON_SGSIDE)	|| \
	  (defined TCP_INSTALLED)   || (defined RTP_INSTALLED)	|| \
	  (defined SIP_MEDIA_SERVER_SIDE)			   \
	) && (defined SYSTEM_SIDE)				   \
      ) 							   \
      || (defined KIATEL_UDP_INSTALLED) 			   \
      || (defined OAM_ON_UDP_CLIENT_SERVER_SIDE)		   \
    )
guint32 find_ipv4_from_string(gchar *ipv4_str_org)
{
	guint i;
	guint32 ipv4;
	guchar ip_bytes[4];
	gchar *tok;
	gchar ipv4_str[128];

#ifdef DEBUG_ACT
//	printf("ipv4_str=%s   ",ipv4_str);
#endif
	// to prevent destroying the original string
	if(ipv4_str_org == NULL_POINTER)
	    return 0;
	strcpy(ipv4_str, ipv4_str_org);
	for(tok=strtok(ipv4_str,"."),i=0;
	   (tok && (i<4));
	   i++,tok=strtok(NULL_POINTER,".")){
		if(tok == NULL_POINTER){
			prnt_msg(ERROR,0x6219);
			return 0;
		}
		ip_bytes[i]=atoi(tok);
	}
	ipv4=(ip_bytes[0] << 24) |
	     (ip_bytes[1] << 16) |
	     (ip_bytes[2] <<  8) |
	     (ip_bytes[3]      ) ;
	return ipv4;
}
// mesle_adamizad_inet_ntoa(address) is a better one
char *change_ipv4_to_string(char *str,guint32 ipv4){
	sprintf(str,"%u.%u.%u.%u",
		  (BYTE)(ipv4>>24),
		  (BYTE)((ipv4>>16)&0xff),
		  (BYTE)((ipv4>> 8)&0xff),
		  (BYTE)((ipv4)&0xff));
	return str;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
WORD_MR make_copy_of_mr1(WORD_MR mr1)
{
	unsigned int size;
	WORD_MR mr2;

	if(mr1 >= NO_MEM_REC){
		return NULL_MEM_REC;
	}
	size=mem_rec[mr1].size;
	mr2=mem_rec_req(size);
	if(mr2 >= NO_MEM_REC){
		prnt_msg(ERROR,0x2361);
		return NULL_MEM_REC;
	}
	mem_cpy(mem_rec[mr2].parm, mem_rec[mr1].parm, size);
	return mr2;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#if (OS == LINUX_COMPATIBLE)
#include <sys/sysinfo.h>
unsigned long print_available_memory(void)
{
	unsigned long m,p;
	p=getpagesize ();
	ws2p("page size:");long2prnt(p);toprnq(LF);
	m=get_avphys_pages ();
	ws2p("avail mem page:");long2prnt(m);toprnq(LF);
	return m;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
#ifdef DO_NOT_DUMP_REMAIN_FOR_A_WHILE_MR1
// only makes it not to appear in 'active' lists even if it is active
void mark_this_mr1_to_remain_for_a_while(gpointer mem){
	if(mem){
	     WORD_MR mr1;
	     mr1=*((unsigned int *)(mem-4));
	     mem_rec[mr1].state=REMAIN_FOR_A_WHILE_STATE;
	}
}
#endif
#endif
/* ========================================================================== */
/* ========================================================================== */
// to be removed, start
#ifdef KIATEL_DYNAMIC_MEM
#ifdef SYSTEM_SIDE
void dump_used_mem_rec(guchar code){
// 0: memories before init, code:1: memories after init, 2: all memories
	unsigned int i,
	  no_used_mem_after_init=0,
	  no_used_mem_before_init=0,
	  total_no_used_mem=0,
	  no_free_mem=0,
	  no_remain_for_a_while_mem=0;

	MEM_REC_ST *mem_pt;

#ifdef PC_COMPATIBLE
	prnt_string("dump_used_mem_rec start:");
	prnt_byte(code);prnt_char('-');
	sdt=*get_dt(&sdt);prnt_char(' ');
	prnt_byte(sdt.mo);prnt_char('/');
	prnt_byte(sdt.da);prnt_char('/');
	prnt_byte(sdt.yr);prnt_char('-');
	prnt_byte(sdt.hr);prnt_char(':');
	prnt_byte(sdt.mi);prnt_char(':');
	prnt_byte(sdt.se);
#endif
	prnt_char(CR);

	prnt_long(fq_mem);prnt_char('-');
	prnt_long(eq_mem);prnt_char(CR);
	for(i=0,mem_pt=&mem_rec[0];i < NO_MEM_REC;i++,mem_pt++){
#ifdef DO_NOT_DUMP_REMAIN_FOR_A_WHILE_MR1
	    if(mem_pt->state == REMAIN_FOR_A_WHILE_STATE){
		no_remain_for_a_while_mem++;
		continue;
//		goto cont1;
	    }
#endif
	    if(mem_pt->state == IN_QUE){
		no_free_mem++;
		continue;
	    }
	    total_no_used_mem++;
cont1:
#ifdef MEMORY_DEBUG
	    if(mem_pt->initialization_level == 0){
		no_used_mem_before_init++;
		if(code == 1)
		    continue;
	    }
	    else
	    if(mem_pt->initialization_level == 1){
		no_used_mem_after_init++;
		if(code == 0)
		    continue;
	    }
#endif

#ifndef DO_NOT_PRINT_MEM_REC_DETAILS
	    prnt_long(i);prnt_char(':');
	    prnt_byte(mem_pt->state);prnt_char('-');
	    if(mem_pt->size > 0xffff){
	      prnt_long(mem_pt->size);
	    }
	    else{
	      prnt_int(mem_pt->size);
	    }
//	    prnt_int(mem_pt->lp);prnt_char('-');
//	    prnt_int(mem_pt->rp);prnt_char('-');
#ifndef DO_NOT_BULK_SAVE_ADDR
#ifndef MEM_REC_DOES_NOT_HAVE_BACKTRACK
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
	    print_addr(mem_pt->addr);
#endif
#endif
#endif
#elif (defined SAVE_ADDR_FOR_LARGE_MEM)
#ifndef MEM_REC_DOES_NOT_HAVE_BACKTRACK
#ifdef NEW_DEBUG
#if (COMPILER == GCC)
	    if(mem_pt->size > 0xff){
		print_addr(mem_pt->addr);
	    }
#endif
#endif
#endif
#endif
#ifdef DO_NOT_PRINT_MEM_REC_CONTENT
	    prnt_char(CR);
#else
	    {
		unsigned int j;
		if(mem_pt->size > 0xffff){
		  prnt_long(mem_pt->size);
		}
		else{
		  prnt_int(mem_pt->size);
		}
		prnt_char(':');
		if(mem_pt->size < 0x100){
		  for(j=0;j<mem_pt->size;j++){
		    prnt_byte(mem_pt->parm[j]);prnt_char('-');
		  }
		}
		prnt_char(CR);
	    }
#endif
#endif //DO_NOT_PRINT_MEM_REC_DETAILS
	}
#ifdef PC_COMPATIBLE
	prnt_string("no_free_mem:");
#endif
	long2prnt(no_free_mem);prnt_char(CR);
#ifdef PC_COMPATIBLE
	prnt_string("total_no_used_mem:");
#endif
	long2prnt(total_no_used_mem);prnt_char(CR);
#ifdef PC_COMPATIBLE
	prnt_string("no_used_mem_before_init:");
#endif
	long2prnt(no_used_mem_before_init);prnt_char(CR);
#ifdef PC_COMPATIBLE
	prnt_string("no_used_mem_after_init:");
#endif
	long2prnt(no_used_mem_after_init);prnt_char(CR);
#ifdef PC_COMPATIBLE
	prnt_string("no_remain_for_a_while_mem:");
#endif
	long2prnt(no_remain_for_a_while_mem);prnt_char(CR);
#ifdef PC_COMPATIBLE
	prnt_string("dump_used_mem_rec end\r");
#endif
}
#endif
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void find_mr1(gpointer mem){
	if(mem){
	  long2prnt(*((unsigned int *)(mem-4)));prnt_char(LF);
	}
}
#endif
// to be removed, end
/* ========================================================================== */
/* ========================================================================== */
#ifdef SPEED_DIALLING
void free_sp_node(SP_NODE_STRUCT *node_pt){
	kt_g_free(node_pt->number_ptr);
	mem_rec_rls(node_pt->mr1);
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SPEED_DIALLING
void free_sp_branch(SP_NODE_STRUCT *node_pt){
	if(node_pt != NULL_SP_NODE){
	    guchar i;
	    for(i=0;i<NO_SP_VALID_DIGIT;i++){
		free_sp_branch(node_pt->nxt_node_pt[i]);
	    }
	    free_sp_node(node_pt);
	}
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SPEED_DIALLING
SP_NODE_STRUCT *sp_node_req(void){
	SP_NODE_STRUCT *node_pt=NULL_SP_NODE;
	guint i=mem_rec_req(sizeof(SP_NODE_STRUCT));
	if(i < NO_MEM_REC){
	    node_pt=(SP_NODE_STRUCT *)(mem_rec[i].parm);
	    node_pt->number_ptr=NULL_POINTER;
	    node_pt->mr1=i;
	    for(i=0;i<NO_SP_VALID_DIGIT;i++){
		node_pt->nxt_node_pt[i]=NULL_SP_NODE;
	    }
	}
	return node_pt;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SPEED_DIALLING
guchar make_sp_branch(guchar *dgt_pt,guchar size,guchar *number_ptr){
	SP_NODE_STRUCT *node_pt,*nxt_pt;
	guchar i;
	if(size > MAX_NO_SP_DIGIT1){
		prnt_msg(ERROR,0xb409);
		return FAIL;
	}
	node_pt=first_sp_node_pt;
	if(node_pt == NULL_SP_NODE){
		prnt_msg(ERROR,0xb410);
		print_call_frame();
		return FAIL;
	}
	for(i=size;i>0;i--,dgt_pt++){
	    if(*dgt_pt > LAST_VALID_SP_DIGIT){
		prnt_msg(ERROR,0xb411);
		prnt_byte(*dgt_pt);prnt_char('-');
		for(i=0;i<size;i++){
			prnt_byte(dgt_pt[i]);prnt_char('-');
		}
		prnt_char(CR);
		return FAIL;
	    }
	    nxt_pt=node_pt->nxt_node_pt[*dgt_pt];
	    guchar this_node_is_newly_allocated=0;
	    if(nxt_pt == NULL_SP_NODE){
		nxt_pt=sp_node_req();
		if(nxt_pt == NULL_SP_NODE){
		    prnt_msg(ERROR,0xb412);
		    return FAIL;
		}
		node_pt->nxt_node_pt[*dgt_pt]=nxt_pt;
		this_node_is_newly_allocated=1;
	    }
	    if(i == 1){
		if(this_node_is_newly_allocated == 0){
		    // already we had uvwx and now we have uvw
		    // this one is discarded
		    int j,k;
		    prnt_msg(ERROR,0xb415);
		    for(k=i-size,j=0;j<size;j++,k++){
			prnt_byte(dgt_pt[k]);prnt_char('-');
		    }
		    prnt_char(CR);
		    kt_g_free(number_ptr);
		    return OK;
		}
		if(nxt_pt->number_ptr != NULL_POINTER){
		    // already we had uvw and now we have same uvw
		    // this one overrides previous
		    int j,k;
		    prnt_msg(ERROR,0xb416);
		    for(k=i-size,j=0;j<size;j++,k++){
			prnt_byte(dgt_pt[k]);prnt_char('-');
		    }
		    prnt_char(CR);
		    kt_g_free(nxt_pt->number_ptr);
		    nxt_pt->number_ptr=NULL_POINTER;
		}
		nxt_pt->number_ptr=number_ptr;
		return OK;
	    }
	    else{
		if(nxt_pt->number_ptr != NULL_POINTER){
		    // already we had uvw and now we have uvwx
		    // this one overrides previous
		    int j,k;
		    prnt_msg(ERROR,0xb417);
		    for(k=i-size,j=0;j<size;j++,k++){
			prnt_byte(dgt_pt[k]);prnt_char('-');
		    }
		    prnt_char(CR);
		    kt_g_free(nxt_pt->number_ptr);
		    nxt_pt->number_ptr=NULL_POINTER;
		}
		node_pt=nxt_pt;
	    }
	}
	prnt_msg(ERROR,0xb413);
	return FAIL;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SPEED_DIALLING
#define MAX_NP_LINE		255
void read_sp_file(void){
	if(first_sp_node_pt != NULL_SP_NODE){
	    free_sp_branch(first_sp_node_pt);
	}
	first_sp_node_pt=NULL_POINTER;
	FILE *fp;
#ifdef SYSTEM_SIDE
	strcpy(fnptr,"sp.dat");
	if((fp=fopen(fnbuf,"rb")) != NULL_POINTER){
#endif
#ifdef OPERATOR_SIDE
	if((fp=fopen("sp.dat","rb")) != NULL_POINTER){
#endif
	    first_sp_node_pt=sp_node_req();
	    if(first_sp_node_pt != NULL_SP_NODE){
		char blk[MAX_NP_LINE];
		while(fgets(blk,MAX_NP_LINE,fp) != NULL_POINTER){
		    char *p;
		    guchar code1[MAX_NO_SP_DIGIT1];
		    guchar code2[MAX_NO_SP_DIGIT2];
		    guchar i,no_dgt1,no_dgt2;
		    if(blk[0] == '*') continue;
		    p=strtok(blk, ": \t\r\n");
		    if(p == NULL_POINTER){
			prnt_msg(ERROR,0xb431);
			prnt_string(blk);prnt_char(CR);
			continue;
		    }
		    for(i=0;((p[i] != '\0') && (i<MAX_NO_SP_DIGIT1));i++){
			if(!isdigit(p[i])){
			    prnt_msg(ERROR,0xb444);
			    prnt_string(blk);prnt_char(CR);
			    goto again;
			}
			code1[i]=p[i]-'0';
		    }
		    no_dgt1=i;
		    p=strtok(NULL_POINTER, ": \t\r\n");
		    if(p == NULL_POINTER){
			prnt_msg(ERROR,0xb445);
			prnt_string(blk);prnt_char(CR);
			continue;
		    }
		    for(i=0;((p[i] != '\0') && (i<MAX_NO_SP_DIGIT2));i++){
			if(p[i] == '0'){
			    code2[i]=0x0a;
			}
			else
			if(isdigit(p[i])){
			    code2[i]=p[i]-'0';
			}
			else
			if(p[i] == ';'){
			    code2[i]=0x0b;
			}
			else
			if(p[i] == '<'){
			    code2[i]=0x0c;
			}
			else{
			    prnt_msg(ERROR,0xb446);
			    prnt_string(blk);prnt_char(CR);
			    goto again;
			}
		    }
		    no_dgt2=i;
		    guchar *number_ptr=kt_g_malloc(no_dgt2+1);
		    if(number_ptr == NULL_POINTER){
			prnt_msg(ERROR,0xb447);
			prnt_string(blk);prnt_char(CR);
			continue;
		    }
		    number_ptr[0]=no_dgt2;
		    for(i=0;i<no_dgt2;i++){
			number_ptr[i+1]=code2[i];
		    }
		    if(make_sp_branch(code1,no_dgt1,number_ptr) == FAIL){
			prnt_msg(ERROR,0xb408);
		    }
again:		    continue;
		}
	    }
	    fclose(fp);
	}
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SPEED_DIALLING
void find_sp_from_tree(guchar *dgt,guchar no_of_digits,guchar **number_ptr){
	SP_NODE_STRUCT *node_pt;
	BYTE i,*dgt_pt;
	*number_ptr=NULL_POINTER;
	node_pt=first_sp_node_pt;
	if(node_pt == NULL_SP_NODE){
	    return;
	}
	for(i=0,dgt_pt=&dgt[0];i<(no_of_digits+1);i++,dgt_pt++){
	    if(node_pt->number_ptr != NULL_POINTER){
		*number_ptr=node_pt->number_ptr;
		return;
	    }
	    else{
		if(i == no_of_digits){
		    // unknown number
		    return;
		}
		if(*dgt_pt > LAST_VALID_SP_DIGIT){
		    prnt_msg(ERROR,0xb402);
		    prnt_byte(i);prnt_char('-');
		    prnt_byte(*dgt_pt);prnt_char(CR);
		    return;
		}
		node_pt=node_pt->nxt_node_pt[*dgt_pt];
		if(node_pt == NULL_SP_NODE){
		    // unknown dest
		    return;
		}
	    }
	}
	prnt_msg(ERROR,0xb404);
	return;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
