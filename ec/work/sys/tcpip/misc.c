
/*************  Time-stamp: <2008-04-02 09:04:32 Administrator>   ************/
//ERROR:0x6200-0x6217

//#define DEBUG_ACT
#define HANG_DEBUG
#define COMPILER_NULL_IS_USED

// in order that NULL be correctly defined:
// opr is ok, we have not defined NULL in omtfp.h hence it is compiler defined
// in sys with CENTRAL_EXTERN is ok, however with ndef CENTRAL_EXTERN,
// NULL in hdr.h (dsmtfp.h) is defined as -1

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else
#include "hdr.h"
#include "/ec/work/sys/tcpip/tcpipvar.ext"
#ifdef KIATEL_DYNAMIC_MEM
extern MEM_REC_ST mem_rec[];
#endif
#endif // CENTRAL_EXTERN

//SHARAB
#ifdef KA9Q_TCP_DEBUG_ACT
//#define DEBUG_ACT
#endif

/* ======================================================================== */
/* Put a long in host order into a char array in network order              */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
guint8 *put32(guint8 *cp,guint32 x){

        *cp++ = x >> 24;
        *cp++ = x >> 16;
        *cp++ = x >> 8;
        *cp++ = x;
        return cp;
}
#endif
/* ======================================================================== */
/* Put a short in host order into a char array in network order             */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
guint8 *put16(guint8 *cp,unsigned int x){

        *cp++ = x >> 8;
        *cp++ = x;

        return cp;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
unsigned int get16(guint8 *cp){

        unsigned int x;

        x = *cp++;
        return (x << 8) | *cp;
}
#endif
/* ======================================================================== */
/* Machine-independent, alignment insensitive network-to-host long conversion*/
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
guint32 get32(guint8 *cp){

        guint32 rval;

        rval = *cp++;
        rval <<= 8;
        rval |= *cp++;
        rval <<= 8;
        rval |= *cp++;
        rval <<= 8;
        rval |= *cp;

        return rval;
}
#endif
/* ======================================================================== */
/* Perform end-around-carry adjustment                                      */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
unsigned int eac(guint32 sum){       /* Carries in high order 16 bits */

        unsigned int csum;

        while((csum = (unsigned int)(sum >> 16)) != 0)
                sum = csum + (sum & 0xffffL);
        return (unsigned int)(sum & 0xffffL);   /* Chops to 16 bits */
}
#endif
/* ======================================================================== */
/* Checksum a mbuf chain, with optional pseudo-header                       */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
#ifdef IP_INSTALLED
unsigned int cksum(PSEUDO_HEADER_STRUCT *ph, WORD_MR mr1, unsigned int len){
{
        guint32 sum, csum;
        guint8 *up;
        unsigned int csum1;
        int swap = 0;
        sum = 0;
        /* Sum pseudo-header, if present */
#ifdef DEBUG_ACT
        prnt_string("cksum:mr\r\n");print_mr(mr1);
#endif
        if(ph != NULL){
#ifdef DEBUG_ACT
                prnt_string("ph->source:");prnt_long(ph->source);prnt_char(CR);
                prnt_string("ph->dest:");prnt_long(ph->dest);prnt_char(CR);
                prnt_string("ph->protocol:");prnt_int(ph->protocol);prnt_char(CR);
                prnt_string("ph->length:");prnt_int(ph->length);prnt_char(CR);
#endif
                sum = hiword(ph->source);
                sum += loword(ph->source);
                sum += hiword(ph->dest);
                sum += loword(ph->dest);
                sum += ph->protocol;
                sum += ph->length;
        }

        /* Now do each mbuf on the chain */
        up = mem_rec[mr1].parm;
        csum = 0;

        if(((long)up) & 1){
                /* Handle odd leading byte */
                if(swap)
                        csum = *up++;
                else
                        csum = (unsigned int)*up++ << 8;
                len--;
                swap = !swap;
        }
        if(len > 1){
                /* Have the primitive checksumming routine do most of
                 * the work. At this point, up is guaranteed to be on
                 * a short boundary
                 */
                csum1 = lcsum((guint16 *)up, len >> 1);
                if(swap)
                        csum1 = (csum1 << 8) | (csum1 >> 8);
                csum += csum1;
        }
        /* Handle odd trailing byte */
        if(len & 1){
                if(swap)
                        csum += up[--len];
                else
                        csum += (unsigned int)up[--len] << 8;
                swap = !swap;
        }
        sum += csum;
        return ~eac(sum) & 0xffff;
        /* Do final end-around carry, complement and return */
}
#if 0
{
//SHARAB : this method is also correct if if(ph != NULL){} block for tcp is added.
guint16 word16;
guint32 cksum=0;
guint16 i,result;
        // make 16 bit words out of every two adjacent 8 bit words in the packet
        // and add them up
        for (i=0;i<len;i=i+2){
                word16 =((mem_rec[mr1].parm[i]<<8)&0xFF00)+(mem_rec[mr1].parm[i+1]&0xFF);
                cksum = cksum + (guint32) word16;
        }

        // take only 16 bits out of the 32 bit sum and add up the carries
        while (cksum>>16)
          cksum = (cksum & 0xFFFF)+(cksum >> 16);

        // one's complement the result
        cksum = ~cksum;

        result=((guint16) cksum);
        return result;
}
#endif // 0
}
#endif
#endif
/* ======================================================================== */
/* ======================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
unsigned int lcsum(guint16 *buf,unsigned int cnt){

        guint32 sum = 0L;

        while(cnt-- != 0)
                sum += *buf++;
        while(sum > 65535L)
                sum = (sum & 0xffff) + (sum >> 16);
        return (unsigned int)(((sum >> 8) | (sum << 8)) & 0xffff);
}
#endif
/* ======================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
WORD_MR my_pullup_only(WORD_MR mr1,unsigned int cnt){

        unsigned int old_size,new_size;
        WORD_MR mr2;

        if(mr1 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6216);
                return NULL_MEM_REC;
        }
        old_size=mem_rec[mr1].size;
        if(cnt >= old_size){
                prnt_msg(ERROR,0x6200);
                return NULL_MEM_REC;
        }
        new_size=old_size-cnt;
        if(new_size > 0){
                mr2=mem_rec_req(new_size);
                if(mr2 >= NO_MEM_REC){
                        prnt_msg(ERROR,0x6201);
                        return NULL_MEM_REC;
                }
                mem_cpy(mem_rec[mr2].parm,&(mem_rec[mr1].parm[cnt]),new_size);
        }
        else{
                prnt_msg(ERROR,0x6217);
                mr2=NULL_MEM_REC;
        }
        mem_rec_rls(mr1);
        return mr2;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
WORD_MR my_pullup_copy_data(WORD_MR mr1,guint8 *dat,unsigned int cnt){

        unsigned int old_size,new_size;
        WORD_MR mr2;

        if(mr1 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6215);
                return NULL_MEM_REC;
        }
        old_size=mem_rec[mr1].size;
        if(cnt > old_size){
                prnt_msg(ERROR,0x6202);
//SHARAB : oomad inja
                mem_cpy(dat,mem_rec[mr1].parm,old_size);
//TENNIS : added
                mem_rec_rls(mr1);
                return NULL_MEM_REC;
        }
//SHARAB : oomad inja
        mem_cpy(dat,mem_rec[mr1].parm,cnt);
        new_size=old_size-cnt;
        if(new_size > 0){
                mr2=mem_rec_req(new_size);
                if(mr2 >= NO_MEM_REC){
                        prnt_msg(ERROR,0x6203);
//TENNIS : added
                        mem_rec_rls(mr1);
                        return NULL_MEM_REC;
                }
//SHARAB : raft baala
//              mem_cpy(dat,mem_rec[mr1].parm,cnt);
                mem_cpy(mem_rec[mr2].parm,&(mem_rec[mr1].parm[cnt]),new_size);
        }
        else{
                mr2=NULL_MEM_REC;
        }
        mem_rec_rls(mr1);
        return mr2;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
WORD_MR my_append(WORD_MR mr1,WORD_MR mr2){


        WORD_MR mr3;
        unsigned int size1,size2;

        if(mr1 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6204);
                return NULL_MEM_REC;
        }
        if(mr2 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6205);
                return NULL_MEM_REC;
        }
        size1=mem_rec[mr1].size;
        size2=mem_rec[mr2].size;

        mr3=mem_rec_req(size1+size2);
        if(mr3 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6206);
                return NULL_MEM_REC;
        }
        mem_cpy(mem_rec[mr3].parm,mem_rec[mr1].parm,size1);
        mem_cpy(&(mem_rec[mr3].parm[size1]),mem_rec[mr2].parm,size2);
        mem_rec_rls(mr1);
        mem_rec_rls(mr2);
        return mr3;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
WORD_MR my_pushdown(WORD_MR mr1,unsigned int cnt){

        unsigned int size;
        WORD_MR mr2;

        if(mr1 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6207);
                return NULL_MEM_REC;
        }
        size=mem_rec[mr1].size;
        mr2=mem_rec_req(size+cnt);
        if(mr2 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6208);
                mem_rec_rls(mr1);
                return NULL_MEM_REC;
        }
        mem_cpy(&(mem_rec[mr2].parm[cnt]),mem_rec[mr1].parm,size);
        mem_rec_rls(mr1);
        return mr2;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
WORD_MR my_pushdown_copy_data(WORD_MR mr1,guint8 *dat,unsigned int cnt){

        unsigned int size;
        WORD_MR mr2;

        if(mr1 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6209);
                return NULL_MEM_REC;
        }
        size=mem_rec[mr1].size;
        mr2=mem_rec_req(size+cnt);
        if(mr2 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6210);
                mem_rec_rls(mr1);
                return NULL_MEM_REC;
        }
        mem_cpy(mem_rec[mr2].parm,dat,cnt);
        mem_cpy(&(mem_rec[mr2].parm[cnt]),mem_rec[mr1].parm,size);
        mem_rec_rls(mr1);
        return mr2;
}
#endif
/* ========================================================================== */
/* Copy and delete "cnt" bytes from beginning of packet. Return number of       */
/* bytes actually pulled off                                                    */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
unsigned int my_pullup_linklist_copy_data(WORD_MR *mr3,guint8 *dat_pt,unsigned int cnt){

        WORD_MR mr1,new_mr1,mr2;
        unsigned int n,tot,size;
//SHARAB
#ifdef DEBUG_ACT
//      prnt_string("my_pullup_linklist_copy_data:");
#endif
        mr1=*mr3;
        if(mr1 >= NO_MEM_REC){
                prnt_msg(ERROR,0x6211);
                return 0;
        }
//SHARAB
#ifdef DEBUG_ACT
//      prnt_int(mem_rec[mr1].size);prnt_char('-');
//      prnt_int(cnt);prnt_char(CR);
#endif
        tot = 0;
        while((cnt != 0) && (mr1 < NO_MEM_REC)){
                size=mem_rec[mr1].size;
                n = min(cnt,size);
                if(dat_pt != NULL){
                        if(n == 1){     /* Common case optimization */
                                *dat_pt = mem_rec[mr1].parm[0];
                                dat_pt++;
                        }
                        else{
                                mem_cpy(dat_pt,mem_rec[mr1].parm,n);
                                dat_pt += n;
                        }
                }
                tot += n;
                cnt -= n;
#if 0
                bp->data += n;
                bp->cnt -= n;
                if(bp->cnt == 0){
                        free_mbuf(&bp);
                        *bph = bp;
                }
#endif // 0
                size-=n;
                if(size == 0){
                        new_mr1=mem_rec[mr1].rp;
//SHARAB
#ifdef DEBUG_ACT
//                      prnt_long(mr1);prnt_char('-');
//                      prnt_long(new_mr1);prnt_char(CR);
#endif
                        mem_rec_rls(mr1);
                        mr1=new_mr1;
                }
                else{
                        mr2=mem_rec_req(size);
                        if(mr2 >= NO_MEM_REC){
                                prnt_msg(ERROR,0x6212);
                                return 0;
                        }
                        mem_cpy(mem_rec[mr2].parm,&(mem_rec[mr1].parm[n]),size);
                        mem_rec[mr2].rp=mem_rec[mr1].rp;
                        mem_rec_rls(mr1);
                        *mr3=mr2;
                        return tot;
                }
        }
        *mr3=mr1;
        return tot;
}
#endif
/* ========================================================================== */
/* Copy and delete "cnt" bytes from beginning of packet. Return number of       */
/* bytes actually pulled off                                                    */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
unsigned int my_pullup_linklist_only(WORD_MR *mr3,unsigned int cnt){

        WORD_MR mr1,new_mr1,mr2;
        unsigned int n,tot,size;

        mr1=*mr3;
        if(mr1 >= NO_MEM_REC){
//              prnt_msg(ERROR,0x6213);
//              prnt_int(mr1);prnt_char('-');
//              prnt_int(cnt);prnt_char(CR);
//              print_call_frame();
                return 0;
        }
        tot = 0;
        while((cnt != 0) && (mr1 < NO_MEM_REC)){
                size=mem_rec[mr1].size;
                n = min(cnt,size);
                tot += n;
                cnt -= n;
#if 0
                bp->data += n;
                bp->cnt -= n;
                if(bp->cnt == 0){
                        free_mbuf(&bp);
                        *bph = bp;
                }
#endif // 0
                size-=n;
                if(size == 0){
                        new_mr1=mem_rec[mr1].rp;
                        mem_rec_rls(mr1);
                        mr1=new_mr1;
                }
                else{
                        mr2=mem_rec_req(size);
                        if(mr2 >= NO_MEM_REC){
                                prnt_msg(ERROR,0x6214);
                                return 0;
                        }
                        mem_cpy(mem_rec[mr2].parm,&(mem_rec[mr1].parm[n]),size);
                        mem_rec[mr2].rp=mem_rec[mr1].rp;
                        mem_rec_rls(mr1);
                        *mr3=mr2;
                        return tot;
                }
        }
        *mr3=mr1;
        return tot;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void rls_mr1_list(WORD_MR mr1){

        WORD_MR new_mr1;

        while(mr1 < NO_MEM_REC){
                new_mr1=mem_rec[mr1].rp;
                mem_rec_rls(mr1);
                mr1=new_mr1;
        }
}
#endif
/* ========================================================================== */
/* Pull a 32-bit integer in host order from buffer in network byte order.     */
/* On error, return 0. Note that this is indistinguishable from a normal      */
/* return.                                                                    */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
guint32 pull32(WORD_MR *mr3){

        guint8 buf[4];
        WORD_MR mr1;

        mr1=*mr3;

        mr1=my_pullup_copy_data(mr1,buf,4);
        if(mr1 >= NO_MEM_REC){
                *mr3=mr1;
                return -1;              /* Nothing left */
        }
#if 0
        if(pullup(bpp,buf,4) != 4){
                /* Return zero if insufficient buffer */
                return 0;
        }
#endif // 0
        *mr3=mr1;
        return get32(buf);
}
#endif
/* ========================================================================== */
/* Pull a 16-bit integer in host order from buffer in network byte order.     */
/* Return -1 on error                                                         */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
long pull16(WORD_MR *mr3){

        guint8 buf[2];
        WORD_MR mr1;

        mr1=*mr3;
        mr1=my_pullup_copy_data(mr1,buf,2);
        if(mr1 >= NO_MEM_REC){
                *mr3=mr1;
                return -1;              /* Nothing left */
        }
#if 0
        if(pullup(bpp,buf,2) != 2){
                return -1;              /* Nothing left */
        }
#endif // 0
        *mr3=mr1;
        return get16(buf);
}
#endif
/* ========================================================================== */
/* Pull single byte from mbuf */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
int pull8(WORD_MR *mr3){

        guint8 c;
        WORD_MR mr1;

        mr1=*mr3;
        mr1=my_pullup_copy_data(mr1,&c,1);
        if(mr1 >= NO_MEM_REC){
                *mr3=mr1;
                return -1;              /* Nothing left */
        }
#if 0
        if(pullup(bpp,&c,1) != 1)
                return -1;              /* Nothing left */
#endif // 0
        *mr3=mr1;
        return c;
}
#endif
/* ========================================================================== */
/* Append packet to end of packet queue */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
void enqueue(WORD_MR *q_mr1,WORD_MR mr1){

        WORD_MR p;

#ifdef DEBUG_ACT
        prnt_string("enqueue\r\n");
#endif
        if(mr1 >= NO_MEM_REC)
                return;
        if(*q_mr1 == NULL_MEM_REC){
                /* List is empty, stick at front */
                *q_mr1=mr1;
        }
        else {
                for(p=*q_mr1;mem_rec[p].rp<NO_MEM_REC;p=mem_rec[p].rp){}
                mem_rec[p].rp=mr1;
        }
}
#endif
/* ========================================================================== */
/* Unlink a packet from the head of the queue */
/* ========================================================================== */
#ifdef KIATEL_DYNAMIC_MEM
WORD_MR dequeue(WORD_MR *q_mr1){

        WORD_MR p;

        if(*q_mr1 == NULL_MEM_REC)
                return NULL_MEM_REC;
        p=*q_mr1;
        *q_mr1 = mem_rec[p].rp;
        mem_rec[p].rp=NULL_MEM_REC;
        return p;
}
#endif
