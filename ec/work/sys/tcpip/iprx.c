

/*************	Time-stamp: <2007-06-27 12:11:47 Administrator>   ************/
//ERROR : 0x6000-0x6027

//#define DEBUG_ACT

#define HANG_DEBUG
#define COMPILER_NULL_IS_USED

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else
#include "hdr.h"
#include "/ec/work/sys/tcpip/tcpipvar.ext"
#ifdef KIATEL_DYNAMIC_MEM
extern MEM_REC_ST mem_rec[];
#endif
#ifdef IP_INSTALLED
extern TMR timer[TIMER_NO];
#endif
#endif // CENTRAL_EXTERN

/* ========================================================================== */
/* Extract Ethernet header						      */
/* ========================================================================== */
#ifdef IP_INSTALLED
int ntohether(ETHER_STRUCT *ether,WORD_MR *mr3){

	WORD_MR mr1,mr2;
	int size;
	guint8 buf[2];

	mr1=*mr3;
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6000);
		*mr3=NULL_MEM_REC;
		return 0;
	}
	size=mem_rec[mr1].size;
	if (size <= ((2*EADDR_LEN)+2) ){
		prnt_msg(IP_ERR,0x6001);
		mem_rec_rls(mr1);
		*mr3=NULL_MEM_REC;
		return 0;
	}
	mr2=mem_rec_req(size-(2*EADDR_LEN)-2);
	if(mr2 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6002);
		mem_rec_rls(mr1);
		*mr3=NULL_MEM_REC;
		return 0;
	}
	mem_cpy(ether->dest,&(mem_rec[mr1].parm[0]),EADDR_LEN);
	mem_cpy(ether->source,&(mem_rec[mr1].parm[EADDR_LEN]),EADDR_LEN);
	mem_cpy(buf,&(mem_rec[mr1].parm[2*EADDR_LEN]),2);
	ether->type=get16(buf);
	mem_cpy(mem_rec[mr2].parm,&(mem_rec[mr1].parm[(2*EADDR_LEN)+2]),(size-(2*EADDR_LEN)-2));
	mem_rec_rls(mr1);
	*mr3=mr2;
	return ETHERLEN;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Process incoming Ethernet packets. Shared by all ethernet drivers.	      */
/* ========================================================================== */
#ifdef IP_INSTALLED
void eproc(unsigned int iface_no,WORD_MR mr1){

	ETHER_STRUCT hdr;

	/* Remove Ethernet header and kick packet upstairs */
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6027);
		return;
	}
#ifdef DEBUG_ACT
	prnt_string("eproc1\r\n");
	print_mr(mr1);
#endif
	ntohether(&hdr,&mr1);
#ifdef DEBUG_ACT
	prnt_string("eproc2\r\n");
	print_mr(mr1);
#endif
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6003);
		return;
	}
	switch(hdr.type){
	case IP_TYPE:
//		ip_route_hndlr(iface_no,mr1,hdr.dest[0] & 1);
		to_hopper_q(iface_no,mr1,hdr.dest[0] & 1);
		break;
	case ARP_TYPE:
	case REVARP_TYPE:
#ifdef DEBUG_ACT
		prnt_string("ARP_OR_REVARP_TYPE\r\n");
#endif
		arp_input(iface_no,mr1);
		break;
	default:
//		prnt_msg(IP_ERR,0x6027);
		mem_rec_rls(mr1);
		break;
	}
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Reassemble incoming IP fragments and dispatch completed datagrams	      */
/* to the proper transport module					      */
/* ========================================================================== */
#ifdef IP_INSTALLED
void ip_recv(
unsigned int iface_no,	/* Incoming interface */
IP_STRUCT *ip_pt,	/* Extracted IP header */
WORD_MR mr1,		/* Data portion */
int rxbroadcast,	/* True if received on subnet broadcast address */
gint32 spi		/* Security association, if any */
){
	/* Function to call with completed datagram */
#if 0
	struct raw_ip *rp;
	struct mbuf *bp1;
	int rxcnt = 0;
	struct iplink *ipp;
#endif // 0

	/* If we have a complete packet, call the next layer
	 * to handle the result. Note that fraghandle passes back
	 * a length field that does NOT include the IP header
	 */
#ifdef DEBUG_ACT
	prnt_string("ip_recv\r\n");
	print_mr(mr1);
#endif
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6004);
		return;
	}
	if (fraghandle(ip_pt,&mr1) == -1){
		return; 	/* Not done yet */
	}
	Ip_mib[ipInDelivers].value++;
//MY TEMP CODE start
//	htonip(ip_pt,&mr1,IP_CS_OLD);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6007);
		return;
	}
#ifdef ROUTER_SYSTEM
#ifdef DEBUG_ACT
	prnt_string("Routed:");
	prnt_int(ip_pt->source>>16);prnt_int(ip_pt->source);prnt_char('-');
	prnt_int(ip_pt->dest>>16);prnt_int(ip_pt->dest);prnt_char(CR);
#endif
#ifdef AFSHANI_TEST
	if((ip_pt->dest == 0xc0a8646f) ||  // 192.168.100.111
	   (ip_pt->dest == 0xc0a86471))    // 192.168.100.113
	{
	    ip_pt->dest=0xc0a8646e;	// 192.168.100.110
	    ip_pt->source=0xc0a86471;	// 192.168.100.113
#endif
	    ip_send(
		ip_pt->source,	 /* source address */
		ip_pt->dest,	 /* Destination address */
		ip_pt->protocol, /* Protocol */
		ip_pt->tos,	 /* Type of service */
		ip_pt->ttl,	 /* Time-to-live */
		mr1,		 /* Data portion of datagram */
		ip_pt->id,	 /* Optional identification */
		ip_pt->df	 /* Don't-fragment flag */
	    );
#ifdef AFSHANI_TEST
	}
	else
	       mem_rec_rls(mr1);
#endif
	return;
#endif
// remove extra bytes of ip packet (if any)
	{
	    unsigned int length=ip_pt->length-IPLEN-ip_pt->optlen;
	    if(length < mem_rec[mr1].size){
		mem_rec[mr1].size=length;
		// no need to copy mr1 to another mr, extra bytes are there until mem_rec_rls(mr1)
	    }
	}
	switch(ip_pt->protocol){
#ifdef TCP_INSTALLED
	  case TCP_PTCL :
// test only
		tcp_input(iface_no,ip_pt,mr1,rxbroadcast,spi);
		return;
#endif
#ifdef UDP_INSTALLED
	  case UDP_PTCL :
// test only
		udp_input(iface_no,ip_pt,mr1,rxbroadcast,spi);
		return;
#endif
	  case ICMP_PTCL :
#ifdef ICMP_INSTALLED
		icmp_input(iface_no,ip_pt,mr1,rxbroadcast,spi);
#else
		mem_rec_rls(mr1);
#endif
		return;
	  default	:
		prnt_msg(IP_ERR,0x6024);
		prnt_int(ip_pt->protocol);prnt_char(CR);
		print_mr(mr1);
		mem_rec_rls(mr1);
		return;
	}
	mem_rec_rls(mr1);
//MY TEMP CODE end
#if 0

	for(rp = Raw_ip;rp != NULL;rp = rp->next){
		if(rp->protocol != ip_pt->protocol)
			continue;
		rxcnt++;
		/* Duplicate the data portion, and put the header back on */
		dup_p(&bp1,*bpp,0,len_p(*bpp));
		if(bp1 != NULL){
			htonip(ip_pt,&bp1,IP_CS_OLD);
			enqueue(&rp->rcvq,&bp1);
			if(rp->r_upcall != NULL)
				(*rp->r_upcall)(rp);
		} else {
			free_p(&bp1);
		}
	}
	/* Look it up in the transport protocol table */
	for(ipp = Iplink;ipp->funct != NULL;ipp++){
		if(ipp->proto == ip->protocol)
			break;
	}
	if(ipp->funct != NULL){
		/* Found, call transport protocol */
		(*ipp->funct)(iface,ip_pt,bpp,rxbroadcast,spi);
	} else {
		/* Not found */
		if(rxcnt == 0){
			/* Send an ICMP Protocol Unknown response... */
			Ip_mib[ipInUnknownProtos].value++;
			/* ...unless it's a broadcast */
			if(!rxbroadcast){
				icmp_output(ip_pt,mr1,ICMP_DEST_UNREACH,
				 ICMP_PROT_UNREACH,NULL);
			}
		}
		free_p(bpp);
	}
#endif // 0
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Process IP datagram fragments					      */
/* If datagram is complete, return its length (MINUS header);		      */
/* otherwise return FAIL							*/
/* ========================================================================== */
#ifdef IP_INSTALLED
int fraghandle(
IP_STRUCT *ip_pt,  /* IP header, host byte order */
WORD_MR *mr3	   /* The fragment itself */
){
	REASM_STRUCT *reasm_pt; /* Pointer to reassembly descriptor */
	FRAG_STRUCT *lastfrag_pt,*nextfrag_pt,*frag_pt;
	WORD_MR mr1,mr2;
	unsigned int i;
	unsigned int last;		/* Index of first byte beyond fragment */
	unsigned int timer_no;

	mr1=*mr3;
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6028);
		return -1;
	}
	last = ip_pt->offset + ip_pt->length - (IPLEN + ip_pt->optlen);

	reasm_pt = lookup_reasm(ip_pt);
	if((ip_pt->offset == 0) && (!ip_pt->mf)){
		/* Complete datagram received. Discard any earlier fragments */
		if(reasm_pt != NULL){
			free_reasm(reasm_pt);
			Ip_mib[ipReasmOKs].value++;
		}
		*mr3=mr1;
		return ip_pt->length;
	}

	Ip_mib[ipReasmReqds].value++;
	if(reasm_pt == NULL){
		/* First fragment; create new reassembly descriptor */
		if((reasm_pt = creat_reasm(ip_pt)) == NULL){
			/* No space for descriptor, drop fragment */
			Ip_mib[ipReasmFails].value++;
			mem_rec_rls(mr1);
			*mr3=NULL_MEM_REC;
			return -1;
		}
	}
	/* Keep restarting timer as long as we keep getting fragments */
	timer_no=reasm_pt->timer_no;
	if(timer_no < TIMER_NO){
		stoptimer((unsigned int)reasm_pt,IP_REASSEMBLE_SDL_T);
		reasm_pt->timer_no=NULL_TIMER;
	}
	reasm_pt->timer_no=setimer(IP_REASM_TIMEOUT_COUNT,(unsigned int)reasm_pt,IP_REASSEMBLE_SDL_T);

	/* If this is the last fragment, we now know how long the	*/
	/* entire datagram is; record it				*/

	if(!ip_pt->mf)
		reasm_pt->length = last;

	/* Set nextfrag_pt to the first fragment which begins after us, */
	/* and lastfrag_pt to the last fragment which begins before us	*/

	lastfrag_pt = NULL;
	for(nextfrag_pt = reasm_pt->fraglist;nextfrag_pt != NULL;nextfrag_pt = nextfrag_pt->next){
		if(nextfrag_pt->offset > ip_pt->offset){
			break;
		}
		lastfrag_pt = nextfrag_pt;
	}
	/* Check for overlap with preceeding fragment */
	if((lastfrag_pt != NULL)  && (ip_pt->offset < lastfrag_pt->last)){
		/* Strip overlap from new fragment */
		i = lastfrag_pt->last - ip_pt->offset;
//		pullup(bpp,NULL,i);
//		if(*bpp == NULL)
//			return -1;	/* Nothing left */
		mr1=my_pullup_only(mr1,i);
		if(mr1 >= NO_MEM_REC){
		       *mr3=NULL_MEM_REC;
			return -1;	  /* Nothing left */
		}
		ip_pt->offset += i;
	}
	/* Look for overlap with succeeding segments */
	for(; nextfrag_pt != NULL; nextfrag_pt = frag_pt){
		frag_pt = nextfrag_pt->next;   /* save in case we delete fp */

		if(nextfrag_pt->offset >= last)
			break;	/* Past our end */
		/* Trim the front of this entry; if nothing is
		 * left, remove it.
		 */
		i = last - nextfrag_pt->offset;
//		pullup(&nextfrag_pt->buf,NULL,i);
		nextfrag_pt->buf=my_pullup_only(nextfrag_pt->buf,i);
		if(nextfrag_pt->buf >= NO_MEM_REC){
			/* superseded; delete from list */
			if(nextfrag_pt->prev != NULL)
				nextfrag_pt->prev->next = nextfrag_pt->next;
			else
				reasm_pt->fraglist = nextfrag_pt->next;
			if(frag_pt->next != NULL)
				nextfrag_pt->next->prev = nextfrag_pt->prev;
			freefrag(nextfrag_pt);
		} else
			nextfrag_pt->offset = last;
	}
	/* Lastfrag now points, as before, to the fragment before us;
	 * nextfrag_pt points at the next fragment. Check to see if we can
	 * join to either or both fragments.
	 */
	i = INSERT;
	if((lastfrag_pt != NULL) && (lastfrag_pt->last == ip_pt->offset))
		i |= APPEND;
	if((nextfrag_pt != NULL) && (nextfrag_pt->offset == last))
		i |= PREPEND;
	switch(i){
	  case INSERT:	  /* Insert new desc between lastfrag_pt and nextfrag_pt */
		frag_pt = newfrag(ip_pt->offset,last,mr1);
		frag_pt->prev = lastfrag_pt;
		frag_pt->next = nextfrag_pt;
		if(lastfrag_pt != NULL)
			lastfrag_pt->next = frag_pt;   /* Middle of list */
		else
			reasm_pt->fraglist = frag_pt;	  /* First on list */
		if(nextfrag_pt != NULL)
			nextfrag_pt->prev = frag_pt;
		break;
	  case APPEND:	  /* Append to lastfrag_pt */
		lastfrag_pt->buf=my_append(lastfrag_pt->buf,mr1);
		if(lastfrag_pt->buf >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x6019);
			return -1;
		}
		lastfrag_pt->last = last;  /* Extend forward */
		break;
	  case PREPEND:   /* Prepend to nextfrag_pt */
		mr2 = nextfrag_pt->buf;
		if(mr2 >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x6027);
			return -1;
		}
		nextfrag_pt->buf = mr1;
		nextfrag_pt->buf=my_append(nextfrag_pt->buf,mr2);
		if(nextfrag_pt->buf >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x6020);
			return -1;
		}
		nextfrag_pt->offset = ip_pt->offset;  /* Extend backward */
		break;
	  case (APPEND|PREPEND):
		/* Consolidate by appending this fragment and nextfrag_pt  */
		/* to lastfrag_pt and removing the nextfrag_pt descriptor  */

		lastfrag_pt->buf=my_append(lastfrag_pt->buf,mr1);
		if(lastfrag_pt->buf >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x6021);
			return -1;
		}
		lastfrag_pt->buf=my_append(lastfrag_pt->buf,nextfrag_pt->buf);
		if(lastfrag_pt->buf >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x6022);
			return -1;
		}
		nextfrag_pt->buf = NULL_MEM_REC;
		lastfrag_pt->last = nextfrag_pt->last;

		/* Finally unlink and delete the now unneeded nextfrag_pt */
		lastfrag_pt->next = nextfrag_pt->next;
		if(nextfrag_pt->next != NULL)
			nextfrag_pt->next->prev = lastfrag_pt;
		freefrag(nextfrag_pt);
		break;
	}
	if((reasm_pt->fraglist->offset == 0) &&
	   ((reasm_pt->fraglist->next == NULL) && (reasm_pt->length != 0))){

		/* We've gotten a complete datagram, so extract it from the */
		/* reassembly buffer and pass it on.			    */

		mr1 = reasm_pt->fraglist->buf;
		if(mr1 >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x6023);
			return -1;
		}
		reasm_pt->fraglist->buf = NULL_MEM_REC;
		/* Tell IP the entire length */
		ip_pt->length = reasm_pt->length + (IPLEN + ip_pt->optlen);
		free_reasm(reasm_pt);
		Ip_mib[ipReasmOKs].value++;
		ip_pt->offset = 0;
		ip_pt->mf = 0;
		*mr3=mr1;
		return ip_pt->length;
	} else{
		*mr3=mr1;
		return -1;
	}
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
REASM_STRUCT *lookup_reasm(IP_STRUCT *ip_pt){

	REASM_STRUCT *reasm_pt;
	REASM_STRUCT *rplast_pt = NULL;

	for(reasm_pt = Reasmq;reasm_pt != NULL;rplast_pt=reasm_pt,reasm_pt = reasm_pt->next){
		if(ip_pt->id == reasm_pt->id && ip_pt->source == reasm_pt->source
		 && ip_pt->dest == reasm_pt->dest && ip_pt->protocol == reasm_pt->protocol){
			if(rplast_pt != NULL){
				/* Move to top of list for speed */
				rplast_pt->next = reasm_pt->next;
				reasm_pt->next = Reasmq;
				Reasmq = reasm_pt;
			}
			return reasm_pt;
		}

	}
	return NULL;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Create a reassembly descriptor, put at head of reassembly list	      */
/* ========================================================================== */
#ifdef IP_INSTALLED
REASM_STRUCT *creat_reasm(IP_STRUCT *ip_pt){

	REASM_STRUCT *reasm_pt;
	WORD_MR mr1;

	mr1=mem_rec_req(sizeof(REASM_STRUCT));
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6008);
		return NULL;
	}
	reasm_pt=(REASM_STRUCT *)mem_rec[mr1].parm;
	mem_set(reasm_pt,0,sizeof(REASM_STRUCT));
	reasm_pt->my_mr1=mr1;

//	if((reasm_pt = (struct reasm *)calloc(1,sizeof(struct reasm))) == NULL)
//		return reasm_pt;      /* No space for descriptor */
	reasm_pt->source = ip_pt->source;
	reasm_pt->dest = ip_pt->dest;
	reasm_pt->id = ip_pt->id;
	reasm_pt->protocol = ip_pt->protocol;
	reasm_pt->timer_no=setimer(IP_REASM_TIMEOUT_COUNT,(unsigned int)reasm_pt,IP_REASSEMBLE_SDL_T);
//	reasm_pt->timer_no.func = ip_pt_timeout;
//	reasm_pt->timer_no.arg = reasm_pt;

	reasm_pt->fraglist=NULL;  /* Head of data fragment chain */
	reasm_pt->length=0;
	reasm_pt->next = Reasmq;
	Reasmq = reasm_pt;
	return reasm_pt;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Free all resources associated with a reassembly descriptor		      */
/* ========================================================================== */
#ifdef IP_INSTALLED
void free_reasm(REASM_STRUCT *r){

	REASM_STRUCT *reasm_pt;
	REASM_STRUCT *rplast_pt = NULL;
	FRAG_STRUCT *fp;

	for(reasm_pt = Reasmq;reasm_pt != NULL;rplast_pt = reasm_pt,reasm_pt=reasm_pt->next){
		if(r == reasm_pt)
			break;
	}
	if(reasm_pt == NULL){
		prnt_msg(IP_ERR,0x6009);
		return; /* Not on list */
	}
	stoptimer((unsigned int)reasm_pt,IP_REASSEMBLE_SDL_T);
	reasm_pt->timer_no=NULL_TIMER;
	/* Remove from list of reassembly descriptors */
	if(rplast_pt != NULL)
		rplast_pt->next = reasm_pt->next;
	else
		Reasmq = reasm_pt->next;

	/* Free any fragments on list, starting at beginning */
	while((fp = reasm_pt->fraglist) != NULL){
		reasm_pt->fraglist = fp->next;
		freefrag(fp);
	}
	mem_rec_rls(reasm_pt->my_mr1);
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Handle reassembly timeouts by deleting all reassembly resources	      */
/* ========================================================================== */
#ifdef IP_INSTALLED
void ip_timeout(unsigned int timer_no){
	TMR *tp;
	REASM_STRUCT *reasm_pt;

	if(timer_no  >= TIMER_NO){
		return;
	}
	tp=&timer[timer_no];
	if(tp->it.t != IP_REASSEMBLE_SDL_T){
		return;
	}
	reasm_pt=(REASM_STRUCT *)tp->it.i;
	free_reasm(reasm_pt);
	Ip_mib[ipReasmFails].value++;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Create a fragment							      */
/* ========================================================================== */
#ifdef IP_INSTALLED
FRAG_STRUCT *newfrag(unsigned int offset,unsigned int last,WORD_MR mr1){

	FRAG_STRUCT *fp;
	WORD_MR mr2;

	mr2=mem_rec_req(sizeof(FRAG_STRUCT));
	if(mr2 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6010);
		mem_rec_rls(mr1);
		return NULL;
	}
	fp=(FRAG_STRUCT *)mem_rec[mr2].parm;
	mem_set(fp,0,sizeof(FRAG_STRUCT));
	fp->my_mr1=mr2;
	fp->buf = mr1;
	fp->offset = offset;
	fp->last = last;
	return fp;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Delete a fragment, return next one on queue				      */
/* ========================================================================== */
#ifdef IP_INSTALLED
void freefrag(FRAG_STRUCT *fp){

	WORD_MR my_mr1;

	if(fp == NULL){
		prnt_msg(IP_ERR,0x6011);
		return;
	}
	my_mr1=fp->my_mr1;
	if(my_mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x6023);
		return;
	}
	if(fp->buf < NO_MEM_REC){
		mem_rec_rls(fp->buf);
	}
	mem_rec_rls(my_mr1);
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Delete a fragment, return next one on queue				      */
/* ========================================================================== */
