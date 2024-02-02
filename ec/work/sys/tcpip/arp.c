
// ERROR : 0x9000-0x9027

//#define DEBUG_ACT
#define HANG_DEBUG
#define COMPILER_NULL_IS_USED

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else
#include "hdr.h"
#include "/ec/work/sys/tcpip/tcpipvar.ext"
#ifdef ARP_INSTALLED
extern TMR timer[TIMER_NO];
#endif
#ifdef KIATEL_DYNAMIC_MEM
extern MEM_REC_ST mem_rec[];
#endif
#endif // CENTRAL_EXTERN

/* ========================================================================== */
/* Handle incoming ARP packets. This is almost a direct implementation of
 * the algorithm on page 5 of RFC 826, except for:
 * 1. Outgoing datagrams to unresolved addresses are kept on a queue
 *    pending a reply to our ARP request.
 * 2. The names of the fields in the ARP packet were made more mnemonic.
 * 3. Requests for IP addresses listed in our table as "published" are
 *    responded to, even if the address is not our own.
 */
/* ========================================================================== */
#ifdef ARP_INSTALLED
void arp_input(
unsigned int iface_no,
WORD_MR mr1
){
	ARP_STRUCT arp;
	ARP_TAB_STRUCT *ap_pt;
	ARP_TYPE_STRUCT *at_pt;
	int i;
	IFACE_STRUCT *iface_pt;

#ifdef DEBUG_ACT
	prnt_string("arp_input:");byte2prnt(iface_no);prnt_char(CR);
	print_mr(mr1);
#endif
	if(iface_no >= MAX_IFACE){
		prnt_msg(ARP_ERR,0x9000);
		return;
	}
	iface_pt=&iface_st[iface_no];
	Arp_stat.recv++;
	if(ntoharp(&arp,&mr1) == -1){	 /* Convert into host format */
		prnt_msg(ARP_ERR,0x9001);
		return;
	}
	if(arp.hardware >= NHWTYPES){
		prnt_msg(ARP_ERR,0x9002);
		/* Unknown hardware type, ignore */
		Arp_stat.badtype++;
		return;
	}
	at_pt = &Arp_type[arp.hardware];
	if(arp.protocol != at_pt->iptype){
		prnt_msg(ARP_ERR,0x9003);
		prnt_int(arp.protocol);prnt_char('-');
		prnt_int(at_pt->iptype);prnt_char(CR);
		/* Unsupported protocol type, ignore */
		Arp_stat.badtype++;
		return;
	}
	if((arp.hwalen > MAXHWALEN) || (arp.pralen != sizeof(guint32))){
		prnt_msg(ARP_ERR,0x9004);
		/* Incorrect protocol addr length (different hw addr lengths
		 * are OK since AX.25 addresses can be of variable length)
		 */
		Arp_stat.badlen++;
		return;
	}
	if(mem_cmp(arp.shwaddr,at_pt->bdcst,at_pt->hwalen) == 0){
		prnt_msg(ARP_ERR,0x9005);
		/* This guy is trying to say he's got the broadcast address! */
		Arp_stat.badaddr++;
		return;
	}
//JASHN start
#ifdef DEBUG_ACT
        if(arp.sprotaddr == arp.tprotaddr)
        {
                gchar address_str[16];
                
                change_ipv4_to_string(address_str, arp.sprotaddr);
                prnt_string("arp_input:gratuituos arp received:");prnt_string(address_str);prnt_char(CR);
        }
#endif
//JASHN end        
	/* If this guy is already in the table, update its entry
	 * unless it's a manual entry (noted by the lack of a timer)
	 */
	ap_pt = NULL;	   /* ap plays the role of merge_flag in the spec */
	if( ((ap_pt = arp_lookup(arp.hardware,arp.sprotaddr)) != NULL)
		/* && (dur_timer(ap_pt->timer_no) != 0) */
		&& (ap_pt->timer_no < TIMER_NO)
	){
		ap_pt = arp_add(arp.sprotaddr,arp.hardware,arp.shwaddr,0);
	}
	/* See if we're the address they're looking for */
	if(ismyaddr(arp.tprotaddr) != NULL_IFACE){
		if(ap_pt == NULL)  /* Only if not already in the table */
			arp_add(arp.sprotaddr,arp.hardware,arp.shwaddr,0);

		if(arp.opcode == ARP_REQUEST){
			/* Swap sender's and target's (us) hardware and protocol
			 * fields, and send the packet back as a reply
			 */
			mem_cpy(arp.thwaddr,arp.shwaddr,(unsigned int)arp.hwalen);
			/* Mark the end of the sender's AX.25 address
			 * in case he didn't
			 */
#ifdef	AX25
			if(arp.hardware == ARP_AX25)
				arp.thwaddr[arp.hwalen-1] |= E;
#endif
			mem_cpy(arp.shwaddr,iface_pt->hwaddr,at_pt->hwalen);
			arp.tprotaddr = arp.sprotaddr;
			arp.sprotaddr = iface_pt->addr;
			arp.opcode = ARP_REPLY;
			if((mr1 = htonarp(&arp)) >= NO_MEM_REC){
				prnt_msg(ARP_ERR,0x9006);
				return;
			}
#if 0
			if(iface_pt->forw != NULL)
				(*iface_pt->forw->output)(iface_pt->forw,
				 arp.thwaddr,iface_pt->forw->hwaddr,at_pt->arptype,mr1);
			else
				(*iface_pt->output)(iface,arp.thwaddr,
				 iface_pt->hwaddr,at_pt->arptype,mr1);
#endif // 0
			if(iface_pt->forw < MAX_IFACE){
				enet_output(iface_pt->forw,arp.thwaddr,iface_st[iface_pt->forw].hwaddr,at_pt->arptype,mr1);
			}
			else{
				enet_output(iface_no,arp.thwaddr,iface_pt->hwaddr,at_pt->arptype,mr1);
			}
			Arp_stat.inreq++;
		} else {
			Arp_stat.replies++;
		}
	}
	else if ((arp.opcode == ARP_REQUEST) &&
		 ((ap_pt = arp_lookup(arp.hardware,arp.tprotaddr)) != NULL) &&
		 (ap_pt->pub)
		){
		/* Otherwise, respond if the guy he's looking for is
		 * published in our table.
		 */
		mem_cpy(arp.thwaddr,arp.shwaddr,(unsigned int)arp.hwalen);
		mem_cpy(arp.shwaddr,ap_pt->hw_addr,at_pt->hwalen);
		arp.tprotaddr = arp.sprotaddr;
		arp.sprotaddr = ap_pt->ip_addr;
		arp.opcode = ARP_REPLY;
		if((mr1 = htonarp(&arp)) >= NO_MEM_REC){
			prnt_msg(ARP_ERR,0x9007);
			return;
		}
#if 0
		if(iface_pt->forw != NULL)
			(*iface_pt->forw->output)(iface_pt->forw,
			 arp.thwaddr,iface_pt->forw->hwaddr,at_pt->arptype,bpp);
		else
			(*iface_pt->output)(iface,arp.thwaddr,
			 iface_pt->hwaddr,at_pt->arptype,bpp);
#endif // 0
		if(iface_pt->forw < MAX_IFACE){
			enet_output(iface_pt->forw,arp.thwaddr,iface_st[iface_pt->forw].hwaddr,at_pt->arptype,mr1);
		}
		else{
			enet_output(iface_no,arp.thwaddr,iface_pt->hwaddr,at_pt->arptype,mr1);
		}
		Arp_stat.inreq++;
	} else if(arp.opcode == REVARP_REQUEST){
		for(i=0;i<HASHMOD;i++)
			for(ap_pt = Arp_tab[i];ap_pt != NULL;ap_pt = ap_pt->next)
				if(mem_cmp(ap_pt->hw_addr,arp.thwaddr,at_pt->hwalen) == 0)
					goto found;
found:	if(ap_pt != NULL && ap_pt->pub){
			mem_cpy(arp.shwaddr,iface_pt->hwaddr,at_pt->hwalen);
			arp.tprotaddr = ap_pt->ip_addr;
			arp.sprotaddr = iface_pt->addr;
			arp.opcode = REVARP_REPLY;
			if((mr1 = htonarp(&arp)) >= NO_MEM_REC){
				prnt_msg(ARP_ERR,0x9008);
				return;
			}
#if 0
			if(iface_pt->forw != NULL)
				(*iface_pt->forw->output)(iface_pt->forw,
				 arp.thwaddr,iface_pt->forw->hwaddr,REVARP_TYPE,bpp);
			else
				(*iface_pt->output)(iface,arp.thwaddr,
				 iface_pt->hwaddr,REVARP_TYPE,bpp);
#endif // 0
			if(iface_pt->forw < MAX_IFACE)
				enet_output(iface_pt->forw,arp.thwaddr,iface_st[iface_pt->forw].hwaddr,REVARP_TYPE,mr1);
			else
				enet_output(iface_no,arp.thwaddr,iface_pt->hwaddr,REVARP_TYPE,mr1);
			Arp_stat.inreq++;
		}
	}
	else{
		// ignore it, e.g. gratuitous
	}
}
#endif // ARP_INSTALLED
/* ======================================================================== */
/* Add an IP-addr / hardware-addr pair to the ARP table */
/* ======================================================================== */
#ifdef ARP_INSTALLED
ARP_TAB_STRUCT*
arp_add(
guint32 ipaddr, 		 /* IP address, host order */
ARP_HWTYPE_ENUM hardware,	/* Hardware type */
guint8 *hw_addr,		  /* Hardware address, if known; NULL otherwise */
int pub 			/* Publish this entry? */
){

//	struct mbuf *bp;
	ARP_TAB_STRUCT *ap_pt;
	ARP_TYPE_STRUCT *at_pt;
	unsigned hashval;
	WORD_MR mr1,mr2;
	unsigned int cnt;

//JASHN start : raft payeen
#if 0
#ifdef DEBUG_ACT
	{
		unsigned char i;
		prnt_string("arp_add:");prnt_int(ipaddr>>16);prnt_int(ipaddr);prnt_char('-');
		if(hw_addr != NULL){
			for(i=0;i<6;i++){byte2prnt(hw_addr[i]);}prnt_char(CR);
		}
		else{
			prnt_char(CR);
		}
	}
#endif
#endif // 0        
//JASHN end        
	if(hardware >=NHWTYPES){
		prnt_msg(ARP_ERR,0x9009);
		return NULL;	/* Invalid hardware type */
	}
	at_pt = &Arp_type[hardware];

	if((ap_pt = arp_lookup(hardware,ipaddr)) == NULL){
//JASHN start : oomad inja, some changes
#ifdef DEBUG_ACT
                {
                        unsigned char i;
                        gchar address_str[16];

                        prnt_string("arp_add:");
                        change_ipv4_to_string(address_str, ipaddr);
                        prnt_string(address_str);prnt_char('-');
                        if(hw_addr != NULL){
                                for(i=0;i<6;i++){prnt_byte(hw_addr[i]);prnt_char(':');}prnt_char(CR);
                        }
                        else{
                                prnt_char(CR);
                        }
                }
#endif
//JASHN end        
		/* New entry */
		mr1=mem_rec_req(sizeof(ARP_TAB_STRUCT));
		if(mr1 >= NO_MEM_REC){
			prnt_msg(ARP_ERR,0x9010);
			return NULL;
		}
		ap_pt=(ARP_TAB_STRUCT *)mem_rec[mr1].parm;
		ap_pt->my_mr1=mr1;
		mr2=mem_rec_req(at_pt->hwalen);
//		mr2=mem_rec_req(EADDR_LEN);

		if(mr2 >= NO_MEM_REC){
			prnt_msg(ARP_ERR,0x9011);
			return NULL;
		}
		ap_pt->hw_addr=mem_rec[mr2].parm;
		ap_pt->my_hw_addr_mr1=mr2;
//		ap_pt->timer.func = arp_drop;
//		ap_pt->timer.arg = ap;
		ap_pt->hardware = hardware;
		ap_pt->ip_addr = ipaddr;
		ap_pt->pending=NULL_MEM_REC;

		/* Put on head of hash chain */
		hashval = hash_ip(ipaddr);
		ap_pt->prev = NULL;
		ap_pt->next = Arp_tab[hashval];
		Arp_tab[hashval] = ap_pt;
		if(ap_pt->next != NULL){
			ap_pt->next->prev = ap_pt;
		}
	}
	if(hw_addr == NULL){
		/* Await response */
		ap_pt->state = ARP_PENDING;
//		set_timer(&ap_pt->timer,Arp_type[hardware].pendtime * 1000L);
		cnt=change_to_kiatel_count(Arp_type[hardware].pendtime * 1000L);
		ap_pt->timer_no=setimer(cnt,(unsigned int)ap_pt,ARP_SDL_T);
#ifdef DEBUG_ACT
		prnt_string("ap_pt=");
		prnt_int((unsigned int)ap_pt>>16);prnt_int((unsigned int)ap_pt);prnt_char(CR);
#endif
	} else {
		/* Response has come in, update entry and run through queue */
		ap_pt->state = ARP_VALID;
		if(ap_pt->timer_no < TIMER_NO){
			stoptimer((unsigned int)ap_pt,ARP_SDL_T);
			ap_pt->timer_no=NULL_TIMER;
		}
//		set_timer(&ap_pt->timer,ARPLIFE*1000L);
		cnt=change_to_kiatel_count(ARPLIFE*1000L);
		ap_pt->timer_no=setimer(cnt,(unsigned int)ap_pt,ARP_SDL_T);
#ifdef DEBUG_ACT
		prnt_string("ap_pt=");
		prnt_int((unsigned int)ap_pt>>16);prnt_int((unsigned int)ap_pt);prnt_char(CR);
#endif

		mem_cpy(ap_pt->hw_addr,hw_addr,at_pt->hwalen);
//		mem_cpy(ap_pt->hw_addr,hw_addr,EADDR_LEN);
		ap_pt->pub = pub;
		while((mr1 = dequeue(&ap_pt->pending)) < NO_MEM_REC){
//			ip_route(NULL,&bp,0);
			to_hopper_q(NULL,mr1,0);
		}
	}
//	start_timer(&ap_pt->timer);
	return ap_pt;
}
#endif // ARP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef ARP_INSTALLED
void arp_timeout(unsigned int timer_no){

	TMR *tp;
	struct arp_tab *ap_pt;

	if(timer_no  >= TIMER_NO){
		prnt_msg(ARP_ERR,0x9019);
		return;
	}
	tp=&timer[timer_no];
	if(tp->it.t != ARP_SDL_T){
		prnt_msg(ARP_ERR,0x9020);
		return;
	}
	ap_pt=(struct arp_tab *)(tp->it.i);
#ifdef DEBUG_ACT
	prnt_string("ap_pt=");
	prnt_int((unsigned int)ap_pt>>16);prnt_int((unsigned int)ap_pt);prnt_char(CR);
#endif
	if(ap_pt != NULL_POINTER){
//TENNIS???????????? : uncommented
		arp_drop(ap_pt);
	}
}
#endif // ARP_INSTALLED
/* ========================================================================== */
/* Remove an entry from the ARP table */
/* ========================================================================== */
#ifdef ARP_INSTALLED
void
arp_drop(struct arp_tab *ap_pt)
{
	WORD_MR mr1;

#ifdef DEBUG_ACT
	prnt_string("arp_drop\r\n");
#endif
	if(ap_pt == NULL){
		prnt_msg(ARP_ERR,0x9021);
		return;
	}
//JASHN start
#ifdef DEBUG_ACT
        {
                unsigned char i;
                prnt_long(ap_pt->ip_addr);prnt_char('-');
                if(ap_pt->hw_addr != NULL){
                        for(i=0;i<6;i++){byte2prnt(ap_pt->hw_addr[i]);}prnt_char(CR);
                }
                else{
                        prnt_char(CR);
                }
        }
        
#endif
//JASHN end        
//	stop_timer(&ap_pt->timer); /* Shouldn't be necessary */
	if(ap_pt->next != NULL){
		ap_pt->next->prev = ap_pt->prev;
	}
	if(ap_pt->prev != NULL){
		ap_pt->prev->next = ap_pt->next;
	}
	else{
		Arp_tab[hash_ip(ap_pt->ip_addr)] = ap_pt->next;
	}
//	free_q(&ap_pt->pending);
	while((mr1 = dequeue(&ap_pt->pending)) < NO_MEM_REC){
		mem_rec_rls(mr1);
	}
//	free(ap_pt->hw_addr);
	mem_rec_rls(ap_pt->my_hw_addr_mr1);
//	free(ap_pt);
	mem_rec_rls(ap_pt->my_mr1);
}
#endif // ARP_INSTALLED
/* ========================================================================== */
/* Resolve an IP address to a hardware address; if not found,		      */
/* initiate query and return NULL.  If an address is returned, the	      */
/* interface driver may send the packet; if NULL is returned,		      */
/* res_arp() will have saved the packet on its pending queue,		      */
/* so no further action (like freeing the packet) is necessary. 	      */
/* ========================================================================== */
#ifdef ARP_INSTALLED
guint8 *res_arp(
unsigned int iface_no,		/* Pointer to interface block */
ARP_HWTYPE_ENUM hardware,	/* Hardware type */
guint32 target, 		 /* Target IP address */
WORD_MR mr1			/* IP datagram to be queued if unresolved */
){
	ARP_TAB_STRUCT *ap_pt;
	IP_STRUCT ip_no;

#ifdef DEBUG_ACT
	prnt_string("res_arp start\r\n");
#endif
	if(((ap_pt = arp_lookup(hardware,target)) != NULL) &&
	    (ap_pt->state == ARP_VALID))
		return ap_pt->hw_addr;
	if(ap_pt != NULL){
		/* Earlier packets are already pending, kick this one back
		 * as a source quench
		 */
		ntohip(&ip_no,&mr1);
//TENNIS start : block added
		if(mr1 >= NO_MEM_REC){
			prnt_msg(ARP_ERR,0x9022);
			return NULL;
		}
//TENNIS end
		icmp_output(&ip_no,mr1,ICMP_QUENCH,0,NULL);
//TENNIS
//		mem_rec_rls(mr1);
	} else {
		/* Create an entry and put the datagram on the
		 * queue pending an answer
		 */
		ap_pt = arp_add(target,hardware,NULL,0);
		if(ap_pt == NULL){
			prnt_msg(ARP_ERR,0x9015);
			return NULL;
		}
		enqueue(&ap_pt->pending,mr1);
		arp_output(iface_no,hardware,target);
	}
#ifdef DEBUG_ACT
	prnt_string("res_arp end\r\n");
#endif
	return NULL;
}
#endif // ARP_INSTALLED
/* ========================================================================== */
/* Look up the given IP address in the ARP table			      */
/* ========================================================================== */
#ifdef ARP_INSTALLED
ARP_TAB_STRUCT *arp_lookup(ARP_HWTYPE_ENUM hardware,guint32 ipaddr){

	ARP_TAB_STRUCT *ap_pt;

#ifdef DEBUG_ACT
	prnt_string("arp_lookup:");prnt_int(ipaddr>>16);prnt_int(ipaddr);prnt_char(CR);
#endif
	for(ap_pt = Arp_tab[hash_ip(ipaddr)]; ap_pt != NULL; ap_pt = ap_pt->next){
#ifdef DEBUG_ACT
//		prnt_int(ap_pt->ip_addr>>16);prnt_int(ap_pt->ip_addr);prnt_char(CR);
#endif
		if((ap_pt->ip_addr == ipaddr) && (ap_pt->hardware == hardware)){
#ifdef DEBUG_ACT
			prnt_string("arp found\r\n");
#endif
			break;
		}
	}
	return ap_pt;
}
#endif // ARP_INSTALLED
/* ========================================================================== */
/* Send an ARP request to resolve IP address target_ip */
/* ========================================================================== */
#ifdef ARP_INSTALLED
void arp_output(
unsigned int iface_no,
ARP_HWTYPE_ENUM hardware,
guint32 target)
{
	ARP_STRUCT arp;
	WORD_MR mr1;
	ARP_TYPE_STRUCT *at_pt;
	IFACE_STRUCT *iface_pt;

#ifdef DEBUG_ACT
	prnt_string("arp_output start\r\n");
#endif
	if (iface_no >= MAX_IFACE){
		prnt_msg(ARP_ERR,0x9017);
		return;
	}
	iface_pt=&iface_st[iface_no];
	at_pt = &Arp_type[hardware];
//	if(iface_pt->output == NULL)
//		return;

	arp.hardware = hardware;
	arp.protocol = at_pt->iptype;
	arp.hwalen = at_pt->hwalen;
	arp.pralen = sizeof(guint32);
	arp.opcode = ARP_REQUEST;
	mem_cpy(arp.shwaddr,iface_pt->hwaddr,at_pt->hwalen);
	arp.sprotaddr = iface_pt->addr;
	mem_set(arp.thwaddr,0,at_pt->hwalen);
	arp.tprotaddr = target;
	if((mr1 = htonarp(&arp)) >= NO_MEM_REC){
//JASHN : 9017->9025
		prnt_msg(ARP_ERR,0x9025);
		return;
	}
#if 0
	(*iface_pt->output)(iface,at_pt->bdcst,
		iface_pt->hwaddr,at_pt->arptype,&bp);
#endif // 0
	enet_output(iface_no,at_pt->bdcst,iface_pt->hwaddr,at_pt->arptype,mr1);
	Arp_stat.outreq++;
#ifdef DEBUG_ACT
	prnt_string("arp_output end\r\n");
#endif
}
#endif // ARP_INSTALLED
/* ========================================================================== */
/* Copy a host format arp structure into mbuf for transmission */
/* ========================================================================== */
#ifdef ARP_INSTALLED
WORD_MR htonarp(ARP_STRUCT *arp_pt){

	WORD_MR mr1;
	guint8 *buf;

#ifdef DEBUG_ACT
//	prnt_string("htonarp\r\n");
#endif
	if(arp_pt == (ARP_STRUCT *)NULL){
		prnt_msg(ARP_ERR,0x9012);
		return NULL_MEM_REC;
	}

//	bp = ambufw(ARPLEN + 2 * arp_pt->hwalen);
//	buf = bp->data;
	mr1=mem_rec_req(ARPLEN + 2 * arp_pt->hwalen);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(ARP_ERR,0x9013);
		return NULL_MEM_REC;
	}
	buf=mem_rec[mr1].parm;
	buf = put16(buf,arp_pt->hardware);
	buf = put16(buf,arp_pt->protocol);
	*buf++ = arp_pt->hwalen;
	*buf++ = arp_pt->pralen;
	buf = put16(buf,arp_pt->opcode);
	memcpy(buf,arp_pt->shwaddr,arp_pt->hwalen);
	buf += arp_pt->hwalen;
	buf = put32(buf,arp_pt->sprotaddr);
	memcpy(buf,arp_pt->thwaddr,arp_pt->hwalen);
	buf += arp_pt->hwalen;
	buf = put32(buf,arp_pt->tprotaddr);

//	bp->cnt = buf - bp->data;
	mem_rec[mr1].size=buf-mem_rec[mr1].parm;
	return mr1;
}
#endif // ARP_INSTALLED
/* ========================================================================== */
/* Convert an incoming ARP packet into a host-format structure */
/* ========================================================================== */
#ifdef ARP_INSTALLED
int ntoharp(ARP_STRUCT *arp_pt,WORD_MR *mr3){

	WORD_MR mr1;

#ifdef DEBUG_ACT
//	prnt_string("ntoharp\r\n");
#endif
	if((arp_pt == (ARP_STRUCT *)NULL) || (*mr3 >= NO_MEM_REC)){
		prnt_msg(ARP_ERR,0x9014);
		return -1;
	}

	mr1=*mr3;
	arp_pt->hardware = pull16(&mr1);
	arp_pt->protocol = pull16(&mr1);
//	arp_pt->hwalen = PULLCHAR(&mr1);
//	arp_pt->pralen = PULLCHAR(&mr1);
	arp_pt->hwalen = pull8(&mr1);
	arp_pt->pralen = pull8(&mr1);
	arp_pt->opcode = pull16(&mr1);
//	pullup(bpp,arp_pt->shwaddr,arp_pt->hwalen);
	mr1=my_pullup_copy_data(mr1,arp_pt->shwaddr,arp_pt->hwalen);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(ARP_ERR,0x9023);
		return -1;
	}
	arp_pt->sprotaddr = pull32(&mr1);
//	pullup(bpp,arp_pt->thwaddr,arp_pt->hwalen);
	mr1=my_pullup_copy_data(mr1,arp_pt->thwaddr,arp_pt->hwalen);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(ARP_ERR,0x9024);
		return -1;
	}
	arp_pt->tprotaddr = pull32(&mr1);

	/* Get rid of anything left over */
//	free_p(bpp);
	if(mr1 < NO_MEM_REC){
		mem_rec_rls(mr1);
	}
	*mr3=NULL_MEM_REC;
	return 0;
}
#endif // ARP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef ARP_INSTALLED
void init_arp_type_array(void){

	unsigned int i;
	for(i=0;i<NHWTYPES;i++){
		Arp_type[i].hwalen=0;
		Arp_type[i].iptype=0;
		Arp_type[i].arptype=0;
		Arp_type[i].pendtime=0;
		Arp_type[i].bdcst=NULL;
//		Arp_type[i].format=NULL;
//		Arp_type[i].scan=NULL;
	}
#ifdef	NETROM
	Arp_type[ARP_NETROM].hwalen=AXALEN;
	Arp_type[ARP_NETROM].iptype=0;
	Arp_type[ARP_NETROM].arptype=0;
	Arp_type[ARP_NETROM].pendtime=0;
	Arp_type[ARP_NETROM].bdcst=NULL;
//	Arp_type[ARP_NETROM].format=pax25;
//	Arp_type[ARP_NETROM].scan=setcall;
#endif

#ifdef	ETHER
	Arp_type[ARP_ETHER].hwalen=EADDR_LEN;
	Arp_type[ARP_ETHER].iptype=IP_TYPE;
	Arp_type[ARP_ETHER].arptype=ARP_TYPE;
	Arp_type[ARP_ETHER].pendtime=1;
	Arp_type[ARP_ETHER].bdcst=Ether_bdcst;
//	Arp_type[ARP_ETHER].format=pether;
//	Arp_type[ARP_ETHER].scan=gether;
#endif

#ifdef	AX25
	Arp_type[ARP_AX25].hwalen=AXALEN;
	Arp_type[ARP_AX25].iptype=PID_IP;
	Arp_type[ARP_AX25].arptype=PID_ARP;
	Arp_type[ARP_AX25].pendtime=10;
	Arp_type[ARP_AX25].bdcst=Ax25multi[0];
//	Arp_type[ARP_AX25].format=pax25;
//	Arp_type[ARP_AX25].scan=setcall;
#endif

#ifdef	ARCNET
	Arp_type[ARP_ARCNET].hwalen=AADDR_LEN;
	Arp_type[ARP_ARCNET].iptype=ARC_IP;
	Arp_type[ARP_ARCNET].arptype=ARC_ARP;
	Arp_type[ARP_ARCNET].pendtime=1;
	Arp_type[ARP_ARCNET].bdcst=ARC_bdcst;
//	Arp_type[ARP_ARCNET].format=parc;
//	Arp_type[ARP_ARCNET].scan=garc;
#endif

	for(i=0;i<6;i++)
		Ether_bdcst[i]=0xff;
}
#endif // ARP_INSTALLED
//JASHN start
/* ========================================================================== */
/* Send an ARP request to resolve IP address target_ip */
/* ========================================================================== */
#ifdef ARP_INSTALLED
#if ((defined HOT_STANDBY) && (defined SYSTEM_SIDE))
void send_gratuituos_arp(void)
{
//      ARP_TAB_STRUCT *ap_pt;

//JASHNTEST start
//#ifdef DEBUG_ACT
	prnt_string("send_gratuituos_arp\r\n");
//#endif
//JASHNTEST end

#ifdef DYNAMIC_IP_ALLOCATION
#if 0
        if((ap_pt = arp_lookup(ARP_ETHER, kt_system_st.hot_ipv4_address)) != NULL_POINTER) {
                if(ap_pt->timer_no < TIMER_NO){
                        stoptimer((unsigned int)ap_pt,ARP_SDL_T);
                }
                arp_drop(ap_pt);
        }
        if((ap_pt = arp_lookup(ARP_ETHER, kt_system_st.standby_ipv4_address)) != NULL_POINTER) {
                if(ap_pt->timer_no < TIMER_NO){
                        stoptimer((unsigned int)ap_pt,ARP_SDL_T);
                }
                arp_drop(ap_pt);
        }
#endif // 0        
        if (thisside.hot == SET)
        {
                gratuituos_arp_output(oam_iface_no, ARP_ETHER, kt_system_st.hot_ipv4_address);
        }
        else{
                gratuituos_arp_output(oam_iface_no, ARP_ETHER, kt_system_st.standby_ipv4_address);
        }
#endif
}
#endif
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef ARP_INSTALLED
void gratuituos_arp_output(unsigned int iface_no, ARP_HWTYPE_ENUM hardware, guint32 target)
{
	ARP_STRUCT arp;
	WORD_MR mr1;
	ARP_TYPE_STRUCT *at_pt;
	IFACE_STRUCT *iface_pt;

#ifdef DEBUG_ACT
	prnt_string("gratuituos_arp_output start\r\n");
#endif
	if (iface_no >= MAX_IFACE){
		prnt_msg(ARP_ERR,0x9026);
		return;
	}
	iface_pt=&iface_st[iface_no];
	at_pt = &Arp_type[hardware];
//	if(iface_pt->output == NULL)
//		return;

	arp.hardware = hardware;
	arp.protocol = at_pt->iptype;
	arp.hwalen = at_pt->hwalen;
	arp.pralen = sizeof(guint32);
//JASHNJASHN
	arp.opcode = /*ARP_REQUEST*/ARP_REPLY;
	mem_cpy(arp.shwaddr,iface_pt->hwaddr,at_pt->hwalen);
	arp.sprotaddr = iface_pt->addr;
	mem_set(arp.thwaddr,0xff,at_pt->hwalen);
	arp.tprotaddr = target;
	if((mr1 = htonarp(&arp)) >= NO_MEM_REC){
		prnt_msg(ARP_ERR,0x9027);
		return;
	}
#if 0
	(*iface_pt->output)(iface,at_pt->bdcst,
		iface_pt->hwaddr,at_pt->arptype,&bp);
#endif // 0
	enet_output(iface_no,at_pt->bdcst,iface_pt->hwaddr,at_pt->arptype,mr1);
	Arp_stat.outreq++;
#ifdef DEBUG_ACT
	prnt_string("gratuituos_arp_output end\r\n");
#endif
}
#endif 
//JASHN end
