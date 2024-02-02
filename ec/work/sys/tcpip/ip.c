
/*************	Time-stamp: <2009-06-11 12:43:22 savoj>   ************/
// ERROR : 0x7000-0x7040

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

/* ======================================================================== */
/* Compute hash function on IP address					    */
/* ======================================================================== */
#ifdef IP_INSTALLED
guint hash_ip(guint32 addr){

	unsigned int ret;

	ret = hiword(addr);
	ret ^= loword(addr);
	return ret % HASHMOD;
}
#endif // IP_INSTALLED
/* ======================================================================= */
/* ======================================================================= */
#ifdef IP_INSTALLED
void init_hopper_q(void){

	hopperq_f=hopperq_e=0;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
guint8 to_hopper_q(unsigned int iface_no,WORD_MR mr1,guint8 rxbroadcast){

#ifdef DEBUG_ACT
	prnt_string("to_hopper_q\r\n");
	prnt_int(hopperq_e);prnt_char('-');
	prnt_int(hopperq_f);prnt_char(CR);
#endif
	if( ((hopperq_e+1) & (HOPPER_QUE_LEN-1)) == hopperq_f){
		prnt_msg(IP_ERR,0x7015);
		mem_rec_rls(mr1);
		return FAIL;	/* queue is full */
	}
	hopperq_e= (hopperq_e + 1) & (HOPPER_QUE_LEN-1);
	hopperq[hopperq_e].iface_no=iface_no;
	hopperq[hopperq_e].mr1=mr1;
	hopperq[hopperq_e].rxbroadcast=rxbroadcast;
	return OK;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
guint8 from_hopper_q(HOPPER_QUE_STRUCT **a){

	if(hopperq_f == hopperq_e){
		return FAIL;
	}
	else{
		hopperq_f= (hopperq_f + 1) & (HOPPER_QUE_LEN-1);
		*a=&hopperq[hopperq_f];
		return OK;
	}
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Route an IP datagram. This is the "hopper" through which all IP datagrams, */
/* coming or going, must pass.						      */
/*									      */
/* "rxbroadcast" is set to indicate that the packet came in on a subnet       */
/* broadcast. The router will kick the packet upstairs regardless of the      */
/* IP destination address.						      */
/* ========================================================================== */
#ifdef IP_INSTALLED
int ip_route_hndlr(void){

    unsigned int in_iface_no;	    /* Input interface */
    WORD_MR mr1;		    /* Input packet */
    gint rxbroadcast;		    /* True if packet had link broadcast address */
    HOPPER_QUE_STRUCT *pt;

    IFACE_STRUCT *in_iface_pt;	    /* Input interface, possibly forwarded */
    IP_STRUCT ip;		    /* IP header being processed */
    guint ip_len;	     /* IP header length */
    guint length;	     /* Length of data portion */
    guint32 gateway;		     /* Gateway IP address */
    unsigned int out_iface_no;	    /* Output interface, possibly forwarded */
    IFACE_STRUCT *out_iface_pt;     /* Output interface, possibly forwarded */
    guint offset;	     /* Starting offset of current datagram */
    guint mf_flag;	     /* Original datagram MF flag */
    int strict = 0;		    /* Strict source routing flag */
    guint opt_len;	    /* Length of current option */
    guint8 *opt;	 /* -> beginning of current option */
    int i;
    int ckgood = IP_CS_OLD; /* Has good checksum without modification */
    int pointer;	    /* Relative pointer index for sroute/rroute */
    union icmp_args icmp_args;
    IP_ROUTE_STRUCT *route_pt;
    unsigned int route_no;

//  if (from_hopper_q(&pt) == FAIL){
//	    return FAIL;
//  }
    while (from_hopper_q(&pt) != FAIL){
	mr1=pt->mr1;
	in_iface_no=pt->iface_no;
	rxbroadcast=pt->rxbroadcast;

	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x7020);
		goto next; // return FAIL;
	}
	if(in_iface_no < MAX_IFACE){
		Ip_mib[ipInReceives].value++; /* Not locally generated */
		in_iface_pt=&iface_st[in_iface_no];
		in_iface_pt->iprecvcnt++;
	}
	if(mem_rec[mr1].size < IPLEN){
		prnt_msg(IP_ERR,0x7009);
		/* The packet is shorter than a legal IP header */
		Ip_mib[ipInHdrErrors].value++;
		mem_rec_rls(mr1);
		goto next; // return FAIL;
	}
	/* Sneak a peek at the IP header's IHL field to find its length */
	ip_len = (mem_rec[mr1].parm[0] & 0xf) << 2;
	if(ip_len < IPLEN){
		prnt_msg(IP_ERR,0x7010);
		prnt_int(mem_rec[mr1].parm[0]);prnt_char('-');
		prnt_int(ip_len);prnt_char('-');
		/* The IP header length field is too small */
		Ip_mib[ipInHdrErrors].value++;
		mem_rec_rls(mr1);
		goto next; // return FAIL;
	}
	if(cksum(NULL,mr1,ip_len) != 0){
		/* Bad IP header checksum; discard */
		prnt_msg(IP_ERR,0x7011);
		print_mr(mr1);
		Ip_mib[ipInHdrErrors].value++;
		mem_rec_rls(mr1);
		goto next; // return FAIL;
	}
#ifdef DEBUG_ACT
	prnt_string("ip_route_hndlr1\r\n");
	print_mr(mr1);
#endif
	/* Extract IP header */
	ntohip(&ip,&mr1);
//TENNIS start
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x7040);
		goto next;
	}
//TENNIS end
#ifdef DEBUG_ACT
	prnt_string("ip_route_hndlr\r\n");
	prnt_char('s');prnt_char('r');prnt_char('c');prnt_char(':');prnt_int(ip.source>>16);prnt_int(ip.source);prnt_char('-');
	prnt_char('d');prnt_char('s');prnt_char('t');prnt_char(':');prnt_int(ip.dest>>16);prnt_int(ip.dest);prnt_char(CR);
	print_mr(mr1);
#endif
	if(ip.version != IPVERSION){
		/* We can't handle this version of IP */
		prnt_msg(IP_ERR,0x7012);
		Ip_mib[ipInHdrErrors].value++;
		mem_rec_rls(mr1);
		goto next; // return FAIL;
	}
#if 0
	/* If we're running low on memory, return a source quench */
	if(!rxbroadcast && availmem() != 0){
		prnt_msg(IP_ERR,0x7013);
		icmp_output(&ip,mr1,ICMP_QUENCH,0,NULL);
	 }
#endif // 0
	/* Process options, if any. Also compute length of secondary IP
	 * header in case fragmentation is needed later
	 */
	strict = 0;
	for(i=0;i<ip.optlen;i += opt_len){
		/* First check for the two special 1-byte options */
		switch(ip.options[i] & OPT_NUMBER){
		case IP_EOL:
			goto no_opt;	/* End of options list, we're done */
		case IP_NOOP:
			opt_len = 1;
			continue;	/* No operation, skip to next option */
		}
		/* Not a 1-byte option, so ensure that there's at least
		 * two bytes of option left, that the option length is
		 * at least two, and that there's enough space left for
		 * the specified option length.
		 */
		if(((ip.optlen - i) < 2)
		 || ((opt_len = ip.options[i+1]) < 2)
		 || (ip.optlen - i < opt_len)){
			prnt_msg(IP_ERR,0x7028);
			/* Truncated option, send ICMP and drop packet */
			if(!rxbroadcast){

				icmp_args.pointer = IPLEN + i;
				icmp_output(&ip,mr1,ICMP_PARAM_PROB,0,&icmp_args);
			}

//TENNIS start :else added
			else{
				mem_rec_rls(mr1);
			}
//TENNIS end
			goto next; // return FAIL;
		}
		opt = &ip.options[i];

		switch(opt[0] & OPT_NUMBER){
		case IP_SSROUTE:	/* Strict source route & record route */
			strict = 1;	/* note fall-thru */
		case IP_LSROUTE:	/* Loose source route & record route */
			/* Source routes are ignored unless we're in the
			 * destination field
			 */
			if(opt_len < 3){
				prnt_msg(IP_ERR,0x7029);
				/* Option is too short to be a legal sroute.
				 * Send an ICMP message and drop it.
				 */
				if(!rxbroadcast){

					icmp_args.pointer = IPLEN + i;
					icmp_output(&ip,mr1,ICMP_PARAM_PROB,0,&icmp_args);
				}
//TENNIS start :else added
				else{
					mem_rec_rls(mr1);
				}
//TENNIS end
				goto next; // return FAIL;
			}
			if(ismyaddr(ip.dest) == NULL_IFACE)
				break;	/* Skip to next option */
			pointer = opt[2];
			if(pointer + 4 > opt_len)
				break;	/* Route exhausted; it's for us */

			/* Put address for next hop into destination field,
			 * put our address into the route field, and bump
			 * the pointer. We've already ensured enough space.
			 */
			ip.dest = get32(&opt[pointer]);
			put32(&opt[pointer],locaddr(ip.dest));
			opt[2] += 4;
			ckgood = IP_CS_NEW;
			break;
		case IP_RROUTE: /* Record route */
			if(opt_len < 3){
				prnt_msg(IP_ERR,0x7030);
				/* Option is too short to be a legal rroute.
				 * Send an ICMP message and drop it.
				 */
				if(!rxbroadcast){

					icmp_args.pointer = IPLEN + i;
					icmp_output(&ip,mr1,ICMP_PARAM_PROB,0,&icmp_args);
				}
//TENNIS start :else added
				else{
					mem_rec_rls(mr1);
				}
//TENNIS end
				goto next; // return FAIL;
			}
			pointer = opt[2];
			if(pointer + 4 > opt_len){
				prnt_msg(IP_ERR,0x7031);
				/* Route area exhausted; send an ICMP msg */
				if(!rxbroadcast){

					icmp_args.pointer = IPLEN + i;
					icmp_output(&ip,mr1,ICMP_PARAM_PROB,0,&icmp_args);
//TENNIS : line added
					mr1=NULL_MEM_REC;
				}
				/* Also drop if odd-sized */
				if(pointer != opt_len){
//TENNIS : if added
					if(mr1 < NO_MEM_REC){
						mem_rec_rls(mr1);
					}

					goto next; // return FAIL;
				}
			} else {
				/* Add our address to the route.
				 * We've already ensured there's enough space.
				 */
				put32(&opt[pointer],locaddr(ip.dest));
				opt[2] += 4;
				ckgood = IP_CS_NEW;
			}
			break;
		}
	}
no_opt:

#ifdef AFSHANI_TEST
	if(ip.dest != 0xc0a8646e){ // 192.168.100.110
		ip_recv(in_iface_no,&ip,mr1,rxbroadcast,0);
		goto next_ok; // return OK;
	}
#endif

	/* See if it's a broadcast or addressed to us, and kick it upstairs */
	if((ismyaddr(ip.dest) != NULL_IFACE) || rxbroadcast /*||
		(WantBootp && bootp_validPacket(&ip, mr1))*/){
#ifdef	GWONLY
	/* We're only a gateway, we have no host level protocols */
		if(!rxbroadcast)
			icmp_output(&ip,mr1,ICMP_DEST_UNREACH,
			 ICMP_PROT_UNREACH,NULL);
		Ip_mib[ipInUnknownProtos].value++;
//TENNIS : commented
//		mem_rec_rls(mr1);
#else
		ip_recv(in_iface_no,&ip,mr1,rxbroadcast,0);
#if 0
//test only
if (write_and_check_size_buffer(&ip_rx_st[in_iface_no],mem_rec[mr1].size,mem_rec[mr1].parm) == FAIL){
}
//test only
#endif // 0
#endif
		goto next_ok; // return OK;
	}
	/* Packet is not destined to us. If it originated elsewhere, count */
	/* it as a forwarded datagram.					   */

	if(in_iface_no < MAX_IFACE)
		Ip_mib[ipForwDatagrams].value++;

	/* Adjust the header checksum to allow for the modified TTL */
	ip.checksum += 0x100;
	if((ip.checksum & 0xff00) == 0)
		ip.checksum++;	/* end-around carry */

	/* Decrement TTL and discard if zero. We don't have to check    */
	/* rxbroadcast here because it's already been checked           */

	if(--ip.ttl == 0){
		prnt_msg(IP_ERR,0x7032);
		/* Send ICMP "Time Exceeded" message */
		icmp_output(&ip,mr1,ICMP_TIME_EXCEED,0,NULL);
		Ip_mib[ipInHdrErrors].value++;
//TENNIS : commented
//		mem_rec_rls(mr1);
		goto next; // return FAIL;
	}
	/* Look up target address in routing table */
	if((route_no = rt_lookup(ip.dest)) >= NO_IP_ROUTE){
#ifdef DEBUG_ACT
		prnt_msg(IP_ERR,0x7033);
		prnt_string("No route exists:\r\n");prnt_int(route_no);
#endif
#ifdef H323_GATEWAY_SIDE
		route_no=add_this_h323_gw_route(ip.dest);
		if(route_no >= NO_IP_ROUTE)
#endif
#ifdef RTP_PROCESSING
		route_no=add_this_rtp_route(ip.dest);
		if(route_no >= NO_IP_ROUTE)
#endif
#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM) ||\
     (defined HTBY_LINK_ON_MY_UDP) || (defined KIATEL_UDP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT) || (defined SYSCONUDP))
		route_no=add_this_oam_route(ip.dest);
		if(route_no >= NO_IP_ROUTE)
#endif
		{
			/* No route exists, return unreachable message (we already
			 * know this can't be a broadcast)
			 */
			icmp_output(&ip,mr1,ICMP_DEST_UNREACH,ICMP_HOST_UNREACH,NULL);
//TENNIS : commented
//			mem_rec_rls(mr1);
			Ip_mib[ipOutNoRoutes].value++;
			goto next; // return FAIL;
		}
//TESTTEST end
	}
#ifdef DEBUG_ACT
	prnt_string("route is found:");prnt_int(route_no);prnt_char(CR);
#endif
	route_pt=&ip_route_st[route_no];
	route_pt->uses++;

	/* Check for output forwarding and divert if necessary */
	out_iface_no = route_pt->iface_no;
	if(out_iface_no >= MAX_IFACE){
//TENNIS : added
		mem_rec_rls(mr1);
		prnt_msg(IP_ERR,0x7021);
		prnt_int(out_iface_no);prnt_char(CR);
		goto next; // return FAIL;
	}
	out_iface_pt=&iface_st[out_iface_no];
	if(out_iface_pt->forw < MAX_IFACE)
		out_iface_no = out_iface_pt->forw;

	/* Find gateway; zero gateway in routing table means "send direct" */
	if(route_pt->gateway == 0)
		gateway = ip.dest;
	else
		gateway = route_pt->gateway;

	if(strict && gateway != ip.dest){
		prnt_msg(IP_ERR,0x7008);
		/* Strict source routing requires a direct entry
		 * Again, we know this isn't a broadcast
		 */
//TENNIS : commented
		icmp_output(&ip,mr1,ICMP_DEST_UNREACH,ICMP_ROUTE_FAIL,NULL);
//		mem_rec_rls(mr1);
		Ip_mib[ipOutNoRoutes].value++;
		goto next; // return FAIL;
	}
#ifdef	IPSEC
	if(sec_output(out_iface_pt,&ip,bpp) != 0){
		/* We inserted a security header, recompute hdr checksum */
		ckgood = IP_CS_NEW;	/* Recompute IP checksum */
	}
#endif
	if(ip.length <= out_iface_pt->mtu){
		/* Datagram smaller than interface MTU; put header
		 * back on and send normally.
		 */
		/*int ret_val=*/q_pkt(out_iface_no,gateway,&ip,mr1,ckgood);
		goto next; // return ret_val;
	}
	/* Fragmentation needed */
	if(ip.df){
		prnt_msg(IP_ERR,0x7034);
		/* Don't Fragment set; return ICMP message and drop */

		icmp_args.mtu = out_iface_pt->mtu;
		icmp_output(&ip,mr1,ICMP_DEST_UNREACH,ICMP_FRAG_NEEDED,&icmp_args);
//TENNIS : commented
//		mem_rec_rls(mr1);
		Ip_mib[ipFragFails].value++;
		goto next; // return FAIL;
	}
	/* Create fragments */
	offset = ip.offset;		/* Remember starting offset */
	mf_flag = ip.mf;	  /* Save original MF flag */
	length = ip.length - ip_len;	/* Length of data portion */
	while(length != 0){		/* As long as there's data left */
		unsigned int fragsize;		/* Size of this fragment's data */
		WORD_MR mr2;	/* Data portion of fragment */

		/* After the first fragment, should remove those		*/
		/* options that aren't supposed to be copied on fragmentation   */

		if(length + ip_len <= out_iface_pt->mtu){
			/* Last fragment; send all that remains */
			fragsize = length;
			ip.mf = mf_flag;  /* Pass original MF flag */
		} else {
			/* More to come, so send multiple of 8 bytes */
			fragsize = (out_iface_pt->mtu - ip_len) & 0xfff8;
			ip.mf = 1;
		}
		ip.length = fragsize + ip_len;

		/* Duplicate the fragment */
		mr2=mem_rec_req(fragsize);
		if(mr2 >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x7035);
			mem_rec_rls(mr1);
			Ip_mib[ipFragFails].value++;
			goto next; // return FAIL;
		}
		mem_cpy(mem_rec[mr2].parm,&(mem_rec[mr1].parm[ip.offset-offset]),fragsize);
//		dup_p(&f_data,mr1,ip.offset-offset,fragsize);
		if(q_pkt(out_iface_no,gateway,&ip,mr2,IP_CS_NEW)== FAIL){
			prnt_msg(IP_ERR,0x7036);
			mem_rec_rls(mr1);
			Ip_mib[ipFragFails].value++;
			goto next; // return FAIL;
		}
		Ip_mib[ipFragCreates].value++;
		ip.offset += fragsize;
		length -= fragsize;
	}
	Ip_mib[ipFragOKs].value++;
	mem_rec_rls(mr1);
next:
next_ok:
	continue;
    }
    return OK;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* Convert IP header in host format to network mr1			    */
/* If cflag != 0, take checksum from structure, 			    */
/* otherwise compute it automatically.					    */
/* ======================================================================== */
#ifdef IP_INSTALLED
void htonip(IP_STRUCT *ip_pt,WORD_MR *mr3,int cflag){

	guint hdr_len;
	guint8 *cp;
	guint fl_offs,size;
	WORD_MR mr1,mr2;

	mr1=*mr3;
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x7039);
//TENNIS
		*mr3=NULL_MEM_REC;
		return;
	}
	hdr_len = IPLEN + ip_pt->optlen;
	if(hdr_len > IPLEN + IP_MAXOPT)
		hdr_len = IPLEN + IP_MAXOPT;
	size=mem_rec[mr1].size;
	mr2=mem_rec_req(size+hdr_len);
	if(mr2 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x7019);
//TENNIS
		*mr3=NULL_MEM_REC;
		return;
	}
	/* Insert specified amount of contiguous new space at the beginning */
	mem_cpy(&(mem_rec[mr2].parm[hdr_len]),mem_rec[mr1].parm,size);
	cp = mem_rec[mr2].parm;

	*cp++ = (ip_pt->version << 4) | (hdr_len >> 2);
	*cp++ = ip_pt->tos;
	cp = put16(cp,ip_pt->length);
	cp = put16(cp,ip_pt->id);
	fl_offs = ip_pt->offset >> 3;
	if(ip_pt->congest)
		fl_offs |= 0x8000;
	if(ip_pt->df)
		fl_offs |= 0x4000;
	if(ip_pt->mf)
		fl_offs |= 0x2000;

	cp = put16(cp,fl_offs);
	*cp++ = ip_pt->ttl;
	*cp++ = ip_pt->protocol;
	cp = put16(cp,cflag ? ip_pt->checksum : 0);
	cp = put32(cp,ip_pt->source);
	cp = put32(cp,ip_pt->dest);
	if(ip_pt->optlen != 0)
		mem_cpy(cp,ip_pt->options,min(ip_pt->optlen,IP_MAXOPT));
	/* If requested, recompute checksum and insert into header */
	if(!cflag){
		put16(&(mem_rec[mr2].parm[10]),cksum(NULL,mr2,hdr_len));
	}
	mem_rec_rls(mr1);
	*mr3=mr2;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* Extract an IP header from mbuf */
/* ======================================================================== */
#ifdef IP_INSTALLED
int ntohip(IP_STRUCT *ip,WORD_MR *mr3){

	int ihl;
	guint fl_offs;
	guint8 ipbuf[IPLEN];
	WORD_MR mr1;

	mr1=*mr3;
	mr1=my_pullup_copy_data(mr1,ipbuf,IPLEN);
	if(mr1 >= NO_MEM_REC){
		prnt_msg(IP_ERR,0x7016);
//TENNIS
		*mr3=NULL_MEM_REC;
		return -1;
	}
/*
	if(pullup(bpp,ipbuf,IPLEN) != IPLEN)
		return -1;

*/
	ip->version = (ipbuf[0] >> 4) & 0xf;
	ip->tos = ipbuf[1];
	ip->length = get16(&ipbuf[2]);
	ip->id = get16(&ipbuf[4]);
	fl_offs = get16(&ipbuf[6]);
	ip->offset = (fl_offs & 0x1fff) << 3;
	ip->mf = (fl_offs & 0x2000) ? 1 : 0;
	ip->df = (fl_offs & 0x4000) ? 1 : 0;
	ip->congest = (fl_offs & 0x8000) ? 1 : 0;
	ip->ttl = ipbuf[8];
	ip->protocol = ipbuf[9];
	ip->checksum = get16(&ipbuf[10]);
	ip->source = get32(&ipbuf[12]);
	ip->dest = get32(&ipbuf[16]);

	ihl = (ipbuf[0] & 0xf) << 2;
	if(ihl < IPLEN){
		prnt_msg(IP_ERR,0x7017);
		/* Bogus packet; header is too short */
		ip->optlen = 0;
//TENNIS
		*mr3=NULL_MEM_REC;
		return -1;
	}
	if ( (ip->optlen = ihl - IPLEN) != 0 ) {
//		if ( pullup(bpp,ip->options,ip->optlen) < ip->optlen ){
		mr1=my_pullup_copy_data(mr1,ip->options,ip->optlen);
		if(mr1 >= NO_MEM_REC){
			prnt_msg(IP_ERR,0x7018);
//TENNIS
			*mr3=NULL_MEM_REC;
			return -1;
		}
	}
	*mr3=mr1;
	return ihl;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
unsigned int my_rt_add(guint32 target,guint bits,guint32 gateway,guint iface_no){	   /* Target IP address prefix */
	gint32	metric;
	gint32	ttl;
	guint8 private;
	/* zero gateway in routing table means "send direct" */
	metric=0;
	ttl=MAXTTL;
	private=0;
#ifdef DEBUG_ACT
	prnt_string("my_rt_add:");prnt_char(CR);
	prnt_string("target:");prnt_int(target>>16);prnt_int(target);prnt_char('-');
	prnt_string("bits:");prnt_int(bits);prnt_char('-');
	prnt_string("gateway:");prnt_int(gateway>>16);prnt_int(gateway);prnt_char('-');
	prnt_string("iface_no:");prnt_int(iface_no);prnt_char('-');
	prnt_string("ip:");prnt_int(iface_st[iface_no].addr>>16);prnt_int(iface_st[iface_no].addr);prnt_char(CR);
#endif
	return(rt_add(target,bits,gateway,iface_no,metric,ttl,private));
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
/* Add an entry to the IP routing table. Returns 0 on success, -1 on failure */
#ifdef IP_INSTALLED
unsigned int rt_add(
guint32  target,	  /* Target IP address prefix */
unsigned int bits,	/* Size of target address prefix in bits (0-32) */
guint32  gateway,	  /* Optional gateway to be reached via interface */
unsigned int iface_no,	/* Interface to which packet is to be routed */
gint32	 metric,	  /* Metric for this route entry */
gint32	 ttl _U_,	  /* Lifetime of this route entry in sec */
guint8	 private _U_	    /* Inhibit advertising this entry ? */
){
	IP_ROUTE_STRUCT *rp;
	unsigned int route_no,hp;
	int i;

	if(iface_no >= MAX_IFACE){
#ifdef DEBUG_ACT
		prnt_msg(IP_ERR,0x7000);
		prnt_int(iface_no);prnt_char(CR);
#endif
		return NULL_IP_ROUTE;
	}
	if(bits > 32)
		bits = 32;		/* Bulletproofing */

	if((bits == 32) && (ismyaddr(target) !=  NULL_IFACE)   ){
		prnt_msg(IP_ERR,0x7006);
		return NULL_IP_ROUTE;	/* Don't accept routes to ourselves */
	}

	/* Mask off don't-care bits of target */
	target &= ~0L << (32-bits);

	/* Encapsulated routes must specify gateway, and it can't be    */
	/* ourselves							*/

	if((iface_no == ENCAP_IFACE_NO) && ((gateway == 0) || (ismyaddr(gateway) != NULL_IFACE))){
		prnt_msg(IP_ERR,0x7007);
		return NULL_IP_ROUTE;
	}
	for(i=0;i<HASHMOD;i++)
		Rt_cache[i].route_no = NULL_IP_ROUTE; /*Flush cache */

	/* Zero bits refers to the default route */
	if(bits == 0){
		route_no = R_default;
	}
	else {
		route_no = rt_blookup(target,bits);
	}
	if(route_no == NULL_IP_ROUTE){
		/* The target is not already in the table, so create a new */
		/* entry and put it in. 				   */

		route_no=find_free_ip_route_rec();
		if(route_no >= NO_IP_ROUTE){
			prnt_msg(IP_ERR,0x7037);
			prnt_int(route_no);prnt_char(CR);
			return NULL_IP_ROUTE;
		}
		rp=&ip_route_st[route_no];
		rp->state=IN_USE;
		/* Insert at head of table */
		rp->prev = NULL_IP_ROUTE;
		hp = Routes[bits-1][hash_ip(target)];
		rp->next = hp;
		if(rp->next != NULL_IP_ROUTE)
			ip_route_st[rp->next].prev = route_no;
		Routes[bits-1][hash_ip(target)] = route_no;
		rp->uses = 0;
	}
	rp=&ip_route_st[route_no];
	rp->target = target;
	rp->bits = bits;
	rp->gateway = gateway;
	rp->metric = metric;
	rp->iface_no = iface_no;
#if 0
	rp->flags.rtprivate = private;	/* Should anyone be told of this route? */
	rp->timer.func = rt_timeout;  /* Set the timer field */
	rp->timer.arg = (void *)rp;
	set_timer(&rp->timer,ttl*1000L);
	stop_timer(&rp->timer);
	start_timer(&rp->timer); /* start the timer if appropriate */
#endif // 0

	return route_no;
}
#endif // IP_INSTALLED
/* ========================================================================== */
#ifdef IP_INSTALLED
int my_rt_drop(guint32 target){

	unsigned int bits;

	bits=32;
	return(rt_drop(target, bits));
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Remove an entry from the IP routing table. Returns 0 on success, -1
 * if entry was not in table.
 */
/* ========================================================================== */
#ifdef IP_INSTALLED
int
rt_drop(
guint32 target,
unsigned int bits
){
	IP_ROUTE_STRUCT *route_pt;
	unsigned int route_no;
	int i;

	for(i=0;i<HASHMOD;i++){
		Rt_cache[i].route_no = NULL_IP_ROUTE;	    /* Flush the cache */
	}
	if(bits == 0){
		if(R_default >= NO_IP_ROUTE){
			prnt_msg(IP_ERR,0x7026);
			return -1;
		}
		/* Nail the default entry */
//		stop_timer(&R_default.timer);
//		R_default.iface_no = NULL;
		ip_route_st[R_default].iface_no = NULL_IFACE;
		return 0;
	}
	if(bits > 32){
		bits = 32;
	}
	/* Mask off target according to width */
	target &= ~0L << (32-bits);

	route_no = Routes[bits-1][hash_ip(target)];
	if(route_no >= NO_IP_ROUTE){
		prnt_msg(IP_ERR,0x7024);
		return -1;
	}
	/* Search appropriate chain for existing entry */
	for(;route_no < NO_IP_ROUTE;route_no=ip_route_st[route_no].next){
		if(ip_route_st[route_no].target == target)
			break;
	}
	if(route_no >= NO_IP_ROUTE){	  /* Not in table */
		prnt_msg(IP_ERR,0x7025);
		return -1;
	}
	route_pt=&ip_route_st[route_no];

//	stop_timer(&route_pt->timer);
	if(route_pt->next < NO_IP_ROUTE)
		ip_route_st[route_pt->next].prev = route_pt->prev;
	if(route_pt->prev < NO_IP_ROUTE)
		ip_route_st[route_pt->prev].next = route_pt->next;
	else
		Routes[bits-1][hash_ip(target)] = route_pt->next;

//	free(route_pt);
	rls_ip_route_rec(route_no);
	return 0;
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* Search routing table for entry with specific width			      */
/* ========================================================================== */
#ifdef IP_INSTALLED
unsigned int rt_blookup(guint32 target,unsigned int bits){

	unsigned int route_no;

	if(bits == 0){
		if(R_default >= NO_IP_ROUTE){
			prnt_msg(IP_ERR,0x7022);
			return NULL_IP_ROUTE;
		}
		if(ip_route_st[R_default].iface_no != NULL_IFACE){
			return R_default;
		}
		else{
			return NULL_IP_ROUTE;
		}
	}
	/* Mask off target according to width */
	target &= ~0L << (32-bits);

	for(route_no = Routes[bits-1][hash_ip(target)];route_no < NO_IP_ROUTE ;route_no=ip_route_st[route_no].next){
		if(ip_route_st[route_no].target == target){
			return route_no;
		}
	}
	return NULL_IP_ROUTE;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef IP_INSTALLED
void init_ip_route_st(void){

	unsigned int i;
	IP_ROUTE_STRUCT *route_pt;

	for(i=0,route_pt=&ip_route_st[0];i < NO_IP_ROUTE;i++,route_pt++){
		route_pt->state=IN_QUE;
//		route_pt->target
//		route_pt->bits
//		route_pt->gateway
//		route_pt->metric
//		route_pt->iface_no
//		route_pt->uses
		route_pt->next=i+1;
		route_pt->prev=i-1;
	}
	fq_ip_route_st=0;
	eq_ip_route_st=NO_IP_ROUTE-1;
	ip_route_st[NO_IP_ROUTE-1].next=NULL_IP_ROUTE;
	ip_route_st[0].prev=NULL_IP_ROUTE;
	return;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef IP_INSTALLED
unsigned int find_free_ip_route_rec(void){

	unsigned int route_no;
	IP_ROUTE_STRUCT *route_pt;

#ifdef DEBUG_ACT
	prnt_string("find_free_ip_route_rec\r\n");
	prnt_int(fq_ip_route_st);prnt_char('-');
	prnt_int(eq_ip_route_st);prnt_char(CR);
#endif
	if(fq_ip_route_st >= NO_IP_ROUTE){
		prnt_msg(IP_ERR,0x7001);
		return NULL_IP_ROUTE;
	}
	route_no=fq_ip_route_st;
	route_pt=&ip_route_st[route_no];
	fq_ip_route_st=route_pt->next;
	if(fq_ip_route_st < NO_IP_ROUTE)
		ip_route_st[fq_ip_route_st].prev=NULL_IP_ROUTE;
	else
		eq_ip_route_st=NULL_IP_ROUTE;
	route_pt->next=NULL_IP_ROUTE;
	route_pt->state=OUT_OF_QUE;
	return route_no;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef IP_INSTALLED
void rls_ip_route_rec(unsigned int route_no){

	IP_ROUTE_STRUCT *route_pt;
	if(route_no >= NO_IP_ROUTE){
		prnt_msg(IP_ERR,0x7002);
		prnt_int(route_no);prnt_char(CR);
		return;
	}
	route_pt=&ip_route_st[route_no];
	if(route_pt->state == IN_QUE){
		prnt_msg(IP_ERR,0x7003);
		prnt_int(route_no);prnt_char(CR);
		return;
	}
	route_pt->state=IN_QUE;
//	route_pt->target
//	route_pt->bits
//	route_pt->gateway
//	route_pt->metric
//	route_pt->iface_no
//	route_pt->uses
	route_pt->prev=eq_ip_route_st;
	if(eq_ip_route_st < NO_IP_ROUTE)
		ip_route_st[eq_ip_route_st].next=route_no;
	else
		fq_ip_route_st=route_no;
	eq_ip_route_st=route_no;
	route_pt->next=NULL_IP_ROUTE;
	return;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef IP_INSTALLED
unsigned int find_free_iface_struct(void){

	unsigned int i;

	for(i=0;i<MAX_IFACE;i++){
		if(iface_st[i].state == IDLE){
			iface_st[i].state=OUT_OF_QUE;
			return i;
		}
	}
	return NULL_IFACE;
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef IP_INSTALLED
void init_iface_struct(void){

	unsigned int i;

	for(i=0;i<MAX_IFACE;i++){
		memset(&iface_st[i],0,sizeof(IFACE_STRUCT));
		iface_st[i].state=IDLE;
	}
}
#endif // IP_INSTALLED
/* ======================================================================== */
/* Given an IP address, return the MTU of the local interface used to	    */
/* reach that destination. This is used by TCP to avoid local fragmentation */
/* ======================================================================== */
#ifdef IP_INSTALLED
#ifndef GWONLY
guint ip_mtu(guint32 addr){

	IFACE_STRUCT *iface_pt;
	IP_ROUTE_STRUCT *route_pt;
	unsigned int route_no,iface_no;

	route_no = rt_lookup(addr);
	if(route_no >= NO_IP_ROUTE){
#ifdef DEBUG_ACT
//		prnt_msg(IP_ERR,0x7038);
//		prnt_int(route_no);prnt_char(CR);
#endif
		return 0;
	}
	route_pt=&ip_route_st[route_no];
	if(route_pt->iface_no == ENCAP_IFACE_NO){
		/* Recurse to the actual hardware interface */
		return ip_mtu(route_pt->gateway) - IPLEN;     /* no options? */
	}
	iface_no = route_pt->iface_no;
	if(iface_no >= MAX_IFACE){
		prnt_msg(IP_ERR,0x7023);
		return 0;
	}
	iface_pt=&iface_st[iface_no];
#ifdef	IPSEC
	if(iface_pt->forw < MAX_IFACE){
		return iface_st[iface_pt->forw].mtu - sec_overhead(addr);
	else
		return iface_pt->mtu - sec_overhead(addr);
#else
	if(iface_pt->forw < MAX_IFACE)
		return iface_st[iface_pt->forw].mtu;
	else
		return iface_pt->mtu;
#endif
}
#endif
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
void ip_init(void){

	unsigned int i;
	IP_ROUTE_STRUCT *rp;

	init_ip_route_st();
	for(i=0;i<NUMIPMIB;i++){
		Ip_mib[i].value=0;
	}
	Id_cntr=0;	 /* Datagram serial number */
	Ip_mib[ipDefaultTTL].value=MAXTTL;
//OSTAD start
//	R_default=NULL_IP_ROUTE;
	R_default=find_free_ip_route_rec();
	if(R_default >= NO_IP_ROUTE){
		prnt_msg(IP_ERR,0x7027);
	}
	else{
		rp=&ip_route_st[R_default];
		rp->state=IN_USE;
		rp->prev = NULL_IP_ROUTE;
		rp->next = NULL_IP_ROUTE;
		rp->uses = 0;
		rp->target = 0;
		rp->bits = 0;
		rp->gateway = 0;
		rp->metric = 16;  // RIP_INFINITY
		rp->iface_no = NULL_IFACE;
#if 0
		rp->flags.rtprivate = private;	/* Should anyone be told of this route? */
		rp->timer.func = rt_timeout;  /* Set the timer field */
		rp->timer.arg = (void *)rp;
		set_timer(&rp->timer,ttl*1000L);
		stop_timer(&rp->timer);
		start_timer(&rp->timer); /* start the timer if appropriate */
#endif // 0
	}
//OSTAD end
	Rtlookups=0;
	Rtchits=0;
	Ifaces=NULL_IFACE;
	for(i=0;i<HASHMOD;i++){
		Arp_tab[i]=NULL;
	}
	mem_set(&Arp_stat,0,sizeof(ARP_STAT_STRUCT));
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef IP_INSTALLED
void init_ip_buf(unsigned int i){

	ip_tx_st[i].arr = &ip_tx[i][0];
	ip_tx_st[i].size = IP_BUF_SIZE;
	ip_tx_st[i].rear  = ip_tx_st[i].front=0;

	ip_rx_st[i].arr = &ip_rx[i][0];
	ip_rx_st[i].size = IP_BUF_SIZE;
	ip_rx_st[i].rear  = ip_rx_st[i].front=0;

}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
// MAHGOL : #ifdef ICMP_INSTALLED->
#ifdef IP_INSTALLED
/* Convert an internet address (in host byte order) to a dotted decimal ascii
 * string, e.g., 255.255.255.255\0
 */
char *
mesle_adamizad_inet_ntoa(gint32 a)
{
	static char buf[16];

	sprintf(buf,"%u.%u.%u.%u",
		hibyte(hiword(a)),
		lobyte(hiword(a)),
		hibyte(loword(a)),
		lobyte(loword(a)) );
	return buf;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef ICMP_INSTALLED
/* Select from an array of strings, or return ascii number if out of range */
char *
smsg(char *msgs[],unsigned nmsgs,unsigned n)
{
	static char buf[16];

	if(n < nmsgs && msgs[n] != NULL)
		return msgs[n];
	sprintf(buf,"%u",n);
	return buf;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#define MAXINT16 0xffff 	/* Largest 16-bit integer */
/* ========================================================================== */
/* ========================================================================== */
#ifdef ICMP_INSTALLED
/* Read the Clock global variable, with interrupts off to avoid possible
 * inconsistency on 16-bit machines
 */
gint32
rdclock(void)
{
	int i_state;
	gint32 rval;

	i_state = disable();
	rval = Clock;
	restore(i_state);
	return rval;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef ICMP_INSTALLED
/* clockbits - Read low order bits of timer 0 (the TOD clock)
 * This works only for the 8254 chips used in ATs and 386s.
 *
 * The timer runs in mode 3 (square wave mode), counting down
 * by twos, twice for each cycle. So it is necessary to read back the
 * OUTPUT pin to see which half of the cycle we're in. I.e., the OUTPUT
 * pin forms the most significant bit of the count. Unfortunately,
 * the 8253 in the PC/XT lacks a command to read the OUTPUT pin...
 *
 * The PC's clock design is soooo brain damaged...
 */
guint
clockbits(void)
{
	int i_state;
	unsigned int stat,count;

	do {
		i_state = disable();
		kt_outport8(0x43,0xc2);    /* latch timer 0 count and status for reading */
		stat = kt_inport8(0x40);   /* get status of timer 0 */
		count = kt_inport8(0x40);  /* lsb of count */
		count |= kt_inport8(0x40) << 8;    /* msb of count */
		restore(i_state);	/* no more chip references */
	} while(stat & 0x40);		/* reread if NULL COUNT bit set */
	stat = (stat & 0x80) << 8;	/* Shift OUTPUT to msb of 16-bit word */
	count >>= 1;			/* count /= 2 */
	if(count == 0)
		return stat ^ 0x8000;	/* return complement of OUTPUT bit */
	else
		return count | stat;	/* Combine OUTPUT with counter */
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef ICMP_INSTALLED
/* Return time since startup in milliseconds. Resolution is improved
 * below 55 ms (the clock tick interval) by reading back the instantaneous
 * 8254 counter value and combining it with the global clock tick counter.
 *
 * Reading the 8254 is a bit tricky since a tick could occur asynchronously
 * between the two reads. The tick counter is examined before and after the
 * hardware counter is read. If the tick counter changes, try again.
 * Note: the hardware counter counts down from 65536.
 */
gint32
msclock(void)
{
	gint32 hi;
	guint lo;
	guint64 x;

	do {
		hi = rdclock();
		lo = clockbits();
	} while(hi != rdclock());

	x = ((guint64)hi << 16) - lo;
	return (x * 11) / 13125;
}
#endif
/* ========================================================================== */
/* Process an incoming ICMP packet					      */
/* ========================================================================== */
#ifdef ICMP_INSTALLED
void
echo_proc(
gint32 source,
gint32 dest,
struct icmp *icmp,
WORD_MR mr1
){
	gint32 timestamp,rtt;

	mr1=my_pullup_copy_data(mr1,(guint8 *)&timestamp,sizeof(timestamp));
	if(mr1 >= NO_MEM_REC){
		return;
	}

	if(Icmp_echo && icmp->args.echo.id == MAXINT16){
		/* Compute round trip time */
		char buf[80];
		rtt = msclock() - timestamp;
		sprintf("%s: rtt %lu\n",mesle_adamizad_inet_ntoa(source),rtt);
		ws2p(buf);
	}
	mem_rec_rls(mr1);
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef ICMP_INSTALLED
void
icmp_input(
unsigned int iface_no,	/* Incoming interface (ignored) */
IP_STRUCT *ip,		/* Pointer to decoded IP header structure */
WORD_MR mr1,		/* Pointer to ICMP message */
int rxbroadcast,
gint32 said _U_
){
	struct icmplink *ipp;
	struct icmp icmp;	/* ICMP header */
	IP_STRUCT oip;		/* Offending datagram header */
	guint type;		/* Type of ICMP message */
	guint length;

	icmpInMsgs++;
	if(rxbroadcast){
		/* Broadcast ICMP packets are to be IGNORED !! */
		icmpInErrors++;
		mem_rec_rls(mr1);
		return;
	}
	length = ip->length - IPLEN - ip->optlen;
	if(cksum(NULL,mr1,length) != 0){
		/* Bad ICMP checksum; discard */
		icmpInErrors++;
		mem_rec_rls(mr1);
		return;
	}
	ntohicmp(&icmp,&mr1);

	/* Process the message. Some messages are passed up to the protocol
	 * module for handling, others are handled here.
	 */
	type = icmp.type;

	switch(type){
	case ICMP_TIME_EXCEED:	/* Time-to-live Exceeded */
	case ICMP_DEST_UNREACH: /* Destination Unreachable */
	case ICMP_QUENCH:	/* Source Quench */
	case ICMP_IPSP: 	/* Bad security packet */
		switch(type){
		case ICMP_TIME_EXCEED:	/* Time-to-live Exceeded */
			icmpInTimeExcds++;
			break;
		case ICMP_DEST_UNREACH: /* Destination Unreachable */
			icmpInDestUnreachs++;
			break;
		case ICMP_QUENCH:	/* Source Quench */
			icmpInSrcQuenchs++;
			break;
		}
		ntohip(&oip,&mr1);	 /* Extract offending IP header */
		if(Icmp_trace){
			char buf[80];
			sprintf(buf,"ICMP from %s:",mesle_adamizad_inet_ntoa(ip->source));
			prnt_string(buf);
			sprintf(buf," dest %s %s",mesle_adamizad_inet_ntoa(oip.dest),
			 smsg(Icmptypes,ICMP_TYPES,type));
			prnt_string(buf);
			switch(type){
			case ICMP_TIME_EXCEED:
				sprintf(buf," %s\n",
				 smsg(Exceed,NEXCEED,icmp.code));
				prnt_string(buf);
				break;
			case ICMP_DEST_UNREACH:
				sprintf(buf," %s\n",
				 smsg(Unreach,NUNREACH,icmp.code));
				prnt_string(buf);
				break;
			case ICMP_IPSP:
				sprintf(buf," %s\n",smsg(Said_icmp,NIPSP,icmp.code));
				prnt_string(buf);
				break;
			default:
				sprintf(buf," %u\n",icmp.code);
				prnt_string(buf);
				break;
			}
		}
		for(ipp = Icmplink;ipp->funct != NULL;ipp++)
			if(ipp->proto == oip.protocol)
				break;
		if(ipp->funct != NULL){
			(*ipp->funct)(ip->source,oip.source,oip.dest,icmp.type,
			 icmp.code,mr1);
		}
		break;
	case ICMP_ECHO: 	/* Echo Request */
		/* Change type to ECHO_REPLY, recompute checksum,
		 * and return datagram.
		 */
		icmpInEchos++;
		icmp.type = ICMP_ECHO_REPLY;
		htonicmp(&icmp,&mr1);
		icmpOutEchoReps++;
		ip_send(ip->dest,ip->source,ICMP_PTCL,ip->tos,0,mr1,0,0);
		return;
	case ICMP_REDIRECT:	/* Redirect */
		icmpInRedirects++;
		ntohip(&oip,&mr1);	 /* Extract offending IP header */
		if(Icmp_trace){
			printf("ICMP from %s:",mesle_adamizad_inet_ntoa(ip->source));
			printf(" dest %s %s",mesle_adamizad_inet_ntoa(oip.dest),
			 smsg(Icmptypes,ICMP_TYPES,type));
			printf(" new gateway %s\n",mesle_adamizad_inet_ntoa(icmp.args.address));
		}
		break;
	case ICMP_PARAM_PROB:	/* Parameter Problem */
		icmpInParmProbs++;
		break;
	case ICMP_ECHO_REPLY:	/* Echo Reply */
		icmpInEchoReps++;
		echo_proc(ip->source,ip->dest,&icmp,mr1);
		break;
	case ICMP_TIMESTAMP:	/* Timestamp */
		icmpInTimestamps++;
		break;
	case ICMP_TIME_REPLY:	/* Timestamp Reply */
		icmpInTimestampReps++;
		break;
	case ICMP_INFO_RQST:	/* Information Request */
		break;
	case ICMP_INFO_REPLY:	/* Information Reply */
		break;
	}
	mem_rec_rls(mr1);
}
#endif // ICMPP_INSTALLED
/* ========================================================================== */
/* Return an ICMP response to the sender of a datagram. 		      */
/* Unlike most routines, the callER frees the mbuf.			      */
/* ========================================================================== */
#ifdef IP_INSTALLED
int
icmp_output(
IP_STRUCT *ip_pt _U_,  /* Header of offending datagram */
WORD_MR mr1 _U_,       /* Data portion of datagram - FREED BY CALLER */
guint8 type _U_,	 /* Codes to send */
guint8 code _U_,
union icmp_args *args _U_
){
#ifdef DEBUG_ACT
	prnt_string("icmp_output");
#endif
#ifdef ICMP_INSTALLED
	WORD_MR mr2;
	struct icmp icmp;	/* ICMP protocol header */
	guint dlen;		 /* Length of data portion of offending pkt */
//	guint length;		 /* Total length of reply */

	if(ip_pt == NULL)
		return -1;
	if(ip_pt->protocol == ICMP_PTCL){
		/* Peek at type field of ICMP header to see if it's safe to
		 * return an ICMP message
		 */
		switch(mem_rec[mr1].parm[0]){
		case ICMP_ECHO_REPLY:
		case ICMP_ECHO:
		case ICMP_TIMESTAMP:
		case ICMP_TIME_REPLY:
		case ICMP_INFO_RQST:
		case ICMP_INFO_REPLY:
			break;	/* These are all safe */
		default:
			/* Never send an ICMP error message about another
			 * ICMP error message!
			 */
			return -1;
		}
	}
	icmp.type = type;
	icmp.code = code;
	icmp.args.unused = 0;
	switch(icmp.type){
	case ICMP_PARAM_PROB:
		icmpOutParmProbs++;
		icmp.args.pointer = args->pointer;
		break;
	case ICMP_REDIRECT:
		icmpOutRedirects++;
		icmp.args.address = args->address;
		break;
	case ICMP_ECHO:
		icmpOutEchos++;
		break;
	case ICMP_ECHO_REPLY:
		icmpOutEchoReps++;
		break;
	case ICMP_INFO_RQST:
		break;
	case ICMP_INFO_REPLY:
		break;
	case ICMP_TIMESTAMP:
		icmpOutTimestamps++;
		break;
	case ICMP_TIME_REPLY:
		icmpOutTimestampReps++;
		icmp.args.echo.id = args->echo.id;
		icmp.args.echo.seq = args->echo.seq;
		break;
	case ICMP_ADDR_MASK:
		icmpOutAddrMasks++;
		break;
	case ICMP_ADDR_MASK_REPLY:
		icmpOutAddrMaskReps++;
		break;
	case ICMP_DEST_UNREACH:
		if(icmp.code == ICMP_FRAG_NEEDED)
			icmp.args.mtu = args->mtu;
		icmpOutDestUnreachs++;
		break;
	case ICMP_TIME_EXCEED:
		icmpOutTimeExcds++;
		break;
	case ICMP_QUENCH:
		icmpOutSrcQuenchs++;
		break;
	}
	icmpOutMsgs++;
	/* Compute amount of original datagram to return.
	 * We return the original IP header, and up to 8 bytes past that.
	 */
	dlen = min(8,mem_rec[mr1].size);
//	length = dlen + ICMPLEN + IPLEN + ip_pt->optlen;
	/* Take excerpt from data portion */
	mr2=mem_rec_req(dlen);
	if(mr2 >= NO_MEM_REC){
		mem_rec_rls(mr1);
		return -1;
	}
	mem_cpy(&(mem_rec[mr2].parm[0]),&(mem_rec[mr1].parm[0]),dlen);
	mem_rec_rls(mr1);
	/* Recreate and tack on offending IP header */
	htonip(ip_pt,&mr2,IP_CS_NEW);
	/* Now stick on the ICMP header */
	htonicmp(&icmp,&mr2);
	return ip_send(INADDR_ANY,ip_pt->source,ICMP_PTCL,ip_pt->tos,0,mr2,0,0);
#else
	if(mr1 < NO_MEM_REC){
		mem_rec_rls(mr1);
	}
	return 0;
#endif
}
#endif // IP_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef ICMP_INSTALLED
/* Generate ICMP header in network byte order, link data, compute checksum */
void
htonicmp(
struct icmp *icmp,
WORD_MR *mr3
){
	guint8 *cp;
	guint checksum;
	WORD_MR mr1;

	mr1=*mr3;
	if(mr1 >= NO_MEM_REC){
		*mr3=NULL_MEM_REC;
		return;
	}
	mr1=my_pushdown(mr1,ICMPLEN);
	cp = &(mem_rec[mr1].parm[0]);

	*cp++ = icmp->type;
	*cp++ = icmp->code;
	cp = put16(cp,0);		/* Clear checksum */
	switch(icmp->type){
	case ICMP_DEST_UNREACH:
		if(icmp->code == ICMP_FRAG_NEEDED){
			/* Deering/Mogul max MTU indication */
			cp = put16(cp,0);
			cp = put16(cp,icmp->args.mtu);
		} else
			cp = put32(cp,0L);
		break;
	case ICMP_PARAM_PROB:
		*cp++ = icmp->args.pointer;
		*cp++ = 0;
		cp = put16(cp,0);
		break;
	case ICMP_REDIRECT:
		cp = put32(cp,icmp->args.address);
		break;
	case ICMP_ECHO:
	case ICMP_ECHO_REPLY:
	case ICMP_TIMESTAMP:
	case ICMP_TIME_REPLY:
	case ICMP_INFO_RQST:
	case ICMP_INFO_REPLY:
		cp = put16(cp,icmp->args.echo.id);
		cp = put16(cp,icmp->args.echo.seq);
		break;
	default:
		cp = put32(cp,0L);
		break;
	}
	/* Compute checksum, and stash result */
	checksum = cksum(NULL,mr1,mem_rec[mr1].size);
	cp = &(mem_rec[mr1].parm[2]);
	cp = put16(cp,checksum);
	*mr3=mr1;
}
#endif
/* ========================================================================== */
/* ========================================================================== */
#ifdef ICMP_INSTALLED
/* Pull off ICMP header */
int
ntohicmp(
struct icmp *icmp,
WORD_MR *mr3
){
	guint8 icmpbuf[8];
	WORD_MR mr1;

	if(icmp == (struct icmp *)NULL)
		return -1;

	mr1=*mr3;
	mr1=my_pullup_copy_data(mr1,icmpbuf,8);
	if(mr1 >= NO_MEM_REC){
		*mr3=NULL_MEM_REC;
		return -1;
	}
	icmp->type = icmpbuf[0];
	icmp->code = icmpbuf[1];
	switch(icmp->type){
	case ICMP_DEST_UNREACH:
		/* Retrieve Deering/Mogul MTU value */
		if(icmp->code == ICMP_FRAG_NEEDED)
			icmp->args.mtu = get16(&icmpbuf[6]);
		break;
	case ICMP_PARAM_PROB:
		icmp->args.pointer = icmpbuf[4];
		break;
	case ICMP_REDIRECT:
		icmp->args.address = get32(&icmpbuf[4]);
		break;
	case ICMP_ECHO:
	case ICMP_ECHO_REPLY:
	case ICMP_TIMESTAMP:
	case ICMP_TIME_REPLY:
	case ICMP_INFO_RQST:
	case ICMP_INFO_REPLY:
		icmp->args.echo.id = get16(&icmpbuf[4]);
		icmp->args.echo.seq = get16(&icmpbuf[6]);
		break;
	}
	*mr3=mr1;
	return 0;
}
#endif
