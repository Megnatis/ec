

/*************	Time-stamp: <2006-05-25 13:42:05 Administrator>   ************/
//ERROR :0x5000-0x5028

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
#endif // CENTRAL_EXTERN

#if 0
1)ip_send
2)net_route
3)ip_route_hndlr
4)q_pkt
5)if_tx
6)enet_send
7)enet_output
#endif // 0
/* ======================================================================== */
/* Send an IP datagram. Modeled after the example interface on page 32 of   */
/* RFC 791								    */
/* ======================================================================== */
#ifdef IP_INSTALLED
guint8 ip_send(
guint32 source,  /* source address */
guint32 dest,	 /* Destination address */
guint8 protocol, /* Protocol */
guint8 tos,	 /* Type of service */
guint8 ttl,	 /* Time-to-live */
WORD_MR mr1,	 /* Data portion of datagram */
guint id,	 /* Optional identification */
char df 	 /* Don't-fragment flag */
){
	IP_STRUCT ip;		/* IP header */
	unsigned int length;

	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5000);
		return FAIL;
	}
#ifdef DEBUG_ACT
	prnt_string("ip_send:\r");
	print_mr(mr1);
#endif
	Ip_mib[ipOutRequests].value++;
	length=mem_rec[mr1].size;
	if(source == INADDR_ANY){
		source = locaddr(dest);
	}
//football start
	if(id == 0){
		id = Id_cntr++;
	}
	if(ttl == 0){
		ttl = Ip_mib[ipDefaultTTL].value;
	}
	/* Fill in IP header */
	ip.version = IPVERSION;
	ip.tos = tos;
	ip.length = IPLEN + length;
	ip.id = id;
	ip.offset = 0;
	ip.mf = 0;
	ip.df = df;
	ip.congest = 0;
	ip.ttl = ttl;
	ip.protocol = protocol;
	ip.source = source;
	ip.dest = dest;
	ip.optlen = 0;
//football end

	htonip(&ip,&mr1,IP_CS_NEW);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5001);
		return FAIL;
	}
	if(ismyaddr(ip.dest) != NULL_IFACE){

#ifdef DEBUG_ACT
		prnt_string("loopback\r");
#endif
/* can happen when ip.dest is our own address e.g.
   res_arp() doing icmp_output(&ip_no,mr1,ICMP_QUENCH,0,NULL)
		prnt_msg(IP_ERR,0x5002);
		prnt_long(source);toprnq('-');
		prnt_long(dest);toprnq('-');
		prnt_byte(protocol);toprnq('-');
		print_mr(mr1);
		print_call_frame();
*/
		/* Pretend it has been sent by the loopback interface before */
		/* it appears in the receive queue			     */
		net_route(LOOPBACK_IFACE_NO,mr1);
	}
	else{
#ifdef DEBUG_ACT
	{
		prnt_string("net_route from ip_send\r\n");
		print_mr(mr1);
	}
#endif
		net_route(NULL_IFACE,mr1);
	}
	return OK;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* Given a destination address, return the IP address of the local	    */
/* interface that will be used to reach it. If there is no route	    */
/* to the destination, pick the first non-loopback address.		    */
/* ======================================================================== */
#ifdef IP_INSTALLED
guint32 locaddr(guint32 addr){

	IP_ROUTE_STRUCT *route_pt;
	IFACE_STRUCT *iface_pt;
	unsigned int route_no,iface_no;

	if(ismyaddr(addr) != NULL_IFACE){
/* can happen when addr is our own address e.g.
   res_arp() doing icmp_output(&ip_no,mr1,ICMP_QUENCH,0,NULL)
		prnt_msg(IP_ERR,0x5025);
*/
		return addr;	/* Loopback case */
	}
	if((route_no = rt_lookup(addr)) < NO_IP_ROUTE){
		route_pt=&ip_route_st[route_no];
		iface_no = route_pt->iface_no;
		if(iface_no == ENCAP_IFACE_NO){
			if((route_no = rt_lookup(route_pt->gateway)) < NO_IP_ROUTE){
				route_pt=&ip_route_st[route_no];
				iface_no = route_pt->iface_no;
			}
			else{
				iface_no=NULL_IFACE;
			}
		}
	}
	else{
#ifdef DEBUG_ACT
		prnt_msg(IP_ERR,0x5003);
#endif
		iface_no=NULL_IFACE;
	}
	if(iface_no == NULL_IFACE){
		/* No route currently exists, so just pick the first real */
		/* interface and use its address			  */

		for(iface_no=Ifaces;iface_no < MAX_IFACE;iface_no=iface_st[iface_no].next){
			if((iface_no != LOOPBACK_IFACE_NO) && (iface_no != ENCAP_IFACE_NO)){
				break;
			}
		}
	}
	if((iface_no == NULL_IFACE) || (iface_no == LOOPBACK_IFACE_NO)){
#ifdef DEBUG_ACT
		prnt_msg(IP_ERR,0x5004);
#endif
		return 0;	/* No dice */
	}

	iface_pt=&iface_st[iface_no];
	if(iface_pt->forw < MAX_IFACE)
		return iface_st[iface_pt->forw].addr;
	else
		return iface_pt->addr;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* Return iface pointer if 'addr' belongs to one of our interfaces,         */
/* NULL otherwise.							    */
/* This is used to tell if an incoming IP datagram is for us, or if it	    */
/* has to be routed.							    */
/* ======================================================================== */
#ifdef IP_INSTALLED
unsigned int ismyaddr(guint32 addr){

	unsigned int iface_no;

	if(addr == INADDR_ANY){
		return LOOPBACK_IFACE_NO;
	}
	for(iface_no=Ifaces;iface_no < MAX_IFACE;iface_no=iface_st[iface_no].next){
		if(addr == iface_st[iface_no].addr){
			return iface_no;
		}
	}
	return NULL_IFACE;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* put mr1 into Hopper for network task 				    */
/* returns 0 if OK							    */
/* ======================================================================== */
#ifdef IP_INSTALLED
guint8 net_route(unsigned int iface_no,WORD_MR mr1){

	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5015);
		return FAIL;
	}
	return(to_hopper_q(iface_no,mr1,RXBROADCAST_RESET));
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* Look up target in hash table, matching the entry having the largest number */
/* of leading bits in common. Return default route if not found;	      */
/* if default route not set, return NULL				      */
/* ======================================================================== */
#ifdef IP_INSTALLED
unsigned int rt_lookup(guint32 target){

	int bits;
	gint32 tsave;
	gint32 mask;
	ROUTE_CACHE_STRUCT *route_cache_pt;
	unsigned int route_no;

	Rtlookups++;
	/* Examine cache first */
	route_cache_pt = &Rt_cache[hash_ip(target)];
	if((target == route_cache_pt->target) &&
	   ((route_no = route_cache_pt->route_no) < NO_IP_ROUTE)){
		Rtchits++;
		return route_no;
	}
	tsave = target;

	mask = ~0;	/* All ones */
	for(bits = 31;bits >= 0; bits--){
		target &= mask;
		route_no = Routes[bits][hash_ip(target)];
		if(route_no < NO_IP_ROUTE){
		    for(;route_no < NO_IP_ROUTE;route_no=ip_route_st[route_no].next){

			    if(   (ip_route_st[route_no].target != target)
			       || ((ip_route_st[route_no].iface_no == ENCAP_IFACE_NO) &&
				   (ip_route_st[route_no].gateway == tsave))
			      )
				    continue;
			    /* Stash in cache and return */
			    route_cache_pt->target = tsave;
			    route_cache_pt->route_no = route_no;
			    return route_no;
		    }
		}
		mask <<= 1;
	}
	if(R_default >= NO_IP_ROUTE){
		prnt_msg(IP_ERR,0x5006);
		return NULL_IP_ROUTE;
	}
	if(ip_route_st[R_default].iface_no < MAX_IFACE){
		route_cache_pt->target = tsave;
		route_cache_pt->route_no = R_default;
		return R_default;
	}
	else{
		return NULL_IP_ROUTE;
	}
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Add an IP datagram to an interface output queue, sorting first by	      */
/* the precedence field in the IP header, and secondarily by an 	      */
/* "interactive" flag set by peeking at the transport layer to see            */
/* if the packet belongs to what appears to be an interactive session.	      */
/* A layer violation, yes, but a useful one...				      */
/* ========================================================================== */
#ifdef IP_INSTALLED
guint8 q_pkt(unsigned int iface_no,guint32 gateway,IP_STRUCT *ip_pt,WORD_MR mr1,int ckgood){

//	int i;
	IFACE_STRUCT *iface_pt;
	IFACE_PACKET_STRUCT *this_pkt_pt,*last_pkt_pt,*pkt_pt;

	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5026);
		return FAIL;
	}
	if(iface_no >= MAX_IFACE){
		prnt_msg(IP_ERR,0x5017);
		mem_rec_rls(mr1);
		return FAIL;
	}
	iface_pt=&iface_st[iface_no];
	iface_pt->ipsndcnt++;
	htonip(ip_pt,&mr1,ckgood);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5018);
		return FAIL;
	}
	/* create priority field consisting of tos with 2 unused
	 * low order bits stripped, one of which we'll use as an
	 * "interactive" flag.
	*/
	this_pkt_pt=create_my_pkt_rec();
	if(this_pkt_pt == NULL) {
		prnt_msg(IP_ERR,0x5028);
		return FAIL;
	}
	this_pkt_pt->tos = (ip_pt->tos & 0xfc);
	this_pkt_pt->gateway = gateway;
	this_pkt_pt->mr1=mr1;

	if(iface_pt->outq == NULL){
		/* Queue empty, no priority decisions to be made
		 * This is the usual case for fast networks like Ethernet,
		 * so we can avoid some time-consuming stuff
		 */
		iface_pt->outq = this_pkt_pt;
	} else {
#if 0
		/* See if this packet references a "priority" TCP port number */
		if((ip_pt->protocol == TCP_PTCL) && (ip_pt->offset == 0)){
			// to be completed when tcp is implemented.
			/* Extract a copy of the TCP header */
			if(dup_p(&tbp,*bpp,sizeof(struct qhdr)+IPLEN+
			 ip_pt->optlen,TCPLEN+TCP_MAXOPT) >= TCPLEN){
				ntohtcp(&tcp,&tbp);

				for(i=0;Tcp_interact[i] != -1;i++){
					if(tcp.source == Tcp_interact[i]
					 || tcp.dest == Tcp_interact[i]){
						qhdr.tos |= 1;
						break;
					}
				}
			}
			free_p(&tbp);
		}
#endif // 0
		/* Search the queue looking for the first packet with precedence */
		/* lower than our packet					 */

		last_pkt_pt=NULL;
		for(pkt_pt=iface_pt->outq;pkt_pt!=NULL;last_pkt_pt=pkt_pt,pkt_pt=pkt_pt->next){
			if(this_pkt_pt->tos > pkt_pt->tos){
				break;	/* Add it just before pkt_no */
			}
		}
		this_pkt_pt->next = pkt_pt;
		if(last_pkt_pt == NULL){
			/* First on queue */
			iface_pt->outq = this_pkt_pt;
		}
		else {
			last_pkt_pt->next = this_pkt_pt;
		}
	}
	return OK;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
void ttldec_all_iface(void){
//	  for(i=0;i<MAX_IFACE;i++){
	guint iface_no;
	for(iface_no=Ifaces;iface_no < MAX_IFACE;iface_no=iface_st[iface_no].next){
		ttldec(iface_no);
	}
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Decrement the IP TTL field in each packet on the send queue. If	      */
/* a TTL goes to zero, discard the packet.				      */
/* ========================================================================== */
#ifdef IP_INSTALLED
void ttldec(unsigned int iface_no){

	IFACE_PACKET_STRUCT *pkt_pt,*prev_pkt_pt,*next_pkt_pt;
	IFACE_STRUCT *iface_pt;
	IP_STRUCT ip;
	WORD_MR mr1;

	prev_pkt_pt = NULL;
	iface_pt=&iface_st[iface_no];
	for(pkt_pt=iface_pt->outq;pkt_pt!=NULL;prev_pkt_pt=pkt_pt,pkt_pt=next_pkt_pt){

		next_pkt_pt = pkt_pt->next;
		mr1=pkt_pt->mr1;
		if(mr1 >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x5019);
			return;
		}
		ntohip(&ip,&mr1);
		if(mr1 >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x5020);
			return;
		}
//TENNIS???????????????? : commented
//		ntohip(&ip,&mr1);
		if(--ip.ttl == 0){
			/* Drop packet */
			icmp_output(&ip,mr1,ICMP_TIME_EXCEED,0,NULL);
			if(prev_pkt_pt == NULL)      /* First on queue */
				iface_pt->outq=next_pkt_pt;
			else
				prev_pkt_pt->next=next_pkt_pt;
//TENNIS : commented
//			mem_rec_rls(mr1);
			rls_my_pkt_rec(pkt_pt);
			pkt_pt=prev_pkt_pt;
			continue;
		}
		/* Put IP and queue headers back, restore to queue */
		htonip(&ip,&mr1,0);
		if(mr1 >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x5021);
			return;
		}
//TENNIS???????????????? : commented (not necessary)
#if 0
		if(prev_pkt_pt == NULL)      /* First on queue */
			iface_pt->outq=pkt_pt;
		else
			prev_pkt_pt->next=pkt_pt;
		pkt_pt->next=next_pkt_pt;
#endif // 0
//TENNIS???????????????? : commented

	}
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* General purpose interface transmit task, one for each device that can      */
/* send IP datagrams. It waits on the interface's IP output queue (outq),     */
/* extracts IP datagrams placed there in priority order by ip_route(),	      */
/* and sends them to the device's send routine.                               */
/* ========================================================================== */
#ifdef IP_INSTALLED
void if_tx(unsigned int iface_no){


	IFACE_STRUCT *iface_pt;
	IFACE_PACKET_STRUCT *pkt_pt;

#ifdef DEBUG_ACT
	prnt_string("if_tx\r\n");
#endif
	if(iface_no >= MAX_IFACE){
		prnt_msg(IP_ERR,0x5007);
		return;
	}
	iface_pt=&iface_st[iface_no];
	pkt_pt=iface_pt->outq;
	if (pkt_pt == NULL){
		prnt_msg(IP_ERR,0x5016);
		return;
	}
	iface_pt->outq=pkt_pt->next;
	if (pkt_pt->mr1 > NO_MEM_REC){
		prnt_msg(IP_ERR,0x5029);
		return;
	}
	enet_send(pkt_pt->mr1,iface_no,pkt_pt->gateway,pkt_pt->tos);
	rls_my_pkt_rec(pkt_pt);
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Send an IP datagram on Ethernet					      */
/* ========================================================================== */
#ifdef IP_INSTALLED
guint8 enet_send(WORD_MR mr1,unsigned int iface_no,guint32 gateway,guint8 tos _U_){

	guint8 *egate;
#ifdef DEBUG_ACT
	prnt_string("enet_send\r\n");
	print_mr(mr1);
#endif
	if (iface_no >= MAX_IFACE){
		prnt_msg(IP_ERR,0x5022);
		return FAIL;
	}
	if (mr1 > NO_MEM_REC){
		prnt_msg(IP_ERR,0x5030);
		return FAIL;
	}
	egate = res_arp(iface_no,ARP_ETHER,gateway,mr1);
	if(egate != NULL){
		return enet_output(iface_no,egate,iface_st[iface_no].hwaddr,IP_TYPE,mr1);
	}
#ifdef DEBUG_ACT
	// not necessarily an error
	prnt_msg(IP_ERR,0x5008);
#endif
	return FAIL;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Send a packet with Ethernet header					      */
/* ========================================================================== */
#ifdef IP_INSTALLED
guint8 enet_output(
unsigned int iface_no,	/* Pointer to interface control block */
guint8	*dest,		  /* Destination Ethernet address */
guint8	*source,	  /* Source Ethernet address */
guint type,	 /* Type field */
WORD_MR mr1		/* Data field */
){
	ETHER_STRUCT ep;
	unsigned int pktdrvr_no;
#ifdef DEBUG_ACT
	prnt_string("enet_output\r\n");
	prnt_byte(dest[0]);prnt_byte(dest[1]);prnt_byte(dest[2]);prnt_byte(dest[3]);prnt_byte(dest[4]);prnt_byte(dest[5]);prnt_char('-');
	prnt_byte(source[0]);prnt_byte(source[1]);prnt_byte(source[2]);prnt_byte(source[3]);prnt_byte(source[4]);prnt_byte(source[5]);prnt_char(CR);
	print_mr(mr1);
#endif
	mem_cpy(ep.dest,dest,EADDR_LEN);
	mem_cpy(ep.source,source,EADDR_LEN);
	ep.type = type;
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5023);
		return FAIL;
	}
	htonether(&ep,&mr1);
#ifdef DEBUG_ACT
	prnt_string("enet_output2\r\n");
	print_mr(mr1);
#endif
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5009);
		return FAIL;
	}
	if (iface_no >= MAX_IFACE){
		prnt_msg(IP_ERR,0x5024);
		mem_rec_rls(mr1);
		return FAIL;
	}
	pktdrvr_no=iface_st[iface_no].pktdrvr_no;
	if(pktdrvr_no >= PK_MAX){
		prnt_msg(IP_ERR,0x5010);
		mem_rec_rls(mr1);
		return FAIL;
	}
	if (write_and_check_size_buffer(&pktdrvr_tx_st[pktdrvr_no],mem_rec[mr1].size,mem_rec[mr1].parm) == FAIL){
		prnt_msg(IP_ERR,0x5011);
		mem_rec_rls(mr1);
		return FAIL;
	}
	mem_rec_rls(mr1);
	return OK;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Convert Ethernet header in host form to network mbuf 		      */
/* ========================================================================== */
#ifdef IP_INSTALLED
void htonether(ETHER_STRUCT *ether,WORD_MR *mr3){

	guint8 *cp;
	WORD_MR mr1,mr2;
	unsigned int size;

	mr1=*mr3;
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5012);
		*mr3=NULL_MEM_REC;
		return;
	}
	size=mem_rec[mr1].size;
	mr2=mem_rec_req(size+ETHERLEN);
	if(mr2 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5013);
		*mr3=NULL_MEM_REC;
		mem_rec_rls(mr1);
		return;
	}
	mem_cpy(&(mem_rec[mr2].parm[ETHERLEN]),mem_rec[mr1].parm,size);
	cp = mem_rec[mr2].parm;
	mem_cpy(cp,ether->dest,EADDR_LEN);
	cp += EADDR_LEN;
	mem_cpy(cp,ether->source,EADDR_LEN);
	cp += EADDR_LEN;
	put16(cp,ether->type);

	mem_rec_rls(*mr3);
	*mr3=mr2;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
IFACE_PACKET_STRUCT *create_my_pkt_rec(void){

	IFACE_PACKET_STRUCT *pkt_pt;
	WORD_MR mr1;

	mr1=mem_rec_req(sizeof(IFACE_PACKET_STRUCT));
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5014);
		return (IFACE_PACKET_STRUCT *)NULL;
	}
	pkt_pt=(IFACE_PACKET_STRUCT *)(mem_rec[mr1].parm);
	pkt_pt->my_mr1=mr1;
	pkt_pt->next=NULL;
	return pkt_pt;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
void rls_my_pkt_rec(IFACE_PACKET_STRUCT *pkt_pt){

	if(pkt_pt->my_mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x5027);
		return;
	}
	mem_rec_rls(pkt_pt->my_mr1);
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef IP_INSTALLED
extern FILE *opr_gonz_fp;
void iface_outq_hndlr(void){
	unsigned int iface_no;
	for(iface_no=Ifaces;iface_no < MAX_IFACE;iface_no=iface_st[iface_no].next){
//		if(iface_st[iface_no].outq != NULL){
		while(iface_st[iface_no].outq != NULL){
			if_tx(iface_no);
		}
	}
}
#endif // IP_INSTALLED
