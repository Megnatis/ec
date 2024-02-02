
//ERROR : 0x5500->0x5505

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

//#define ETHERNET_MULTICAST_MODE

/* Attach a packet driver to the system
 * argv[0]: hardware type, must be "packet"
 * argv[1]: software interrupt vector, e.g., x7e
 * argv[2]: interface label, e.g., "my card"
 * argv[3]: receive buffer size in kb
 * argv[4]: maximum transmission unit, bytes, e.g., "1500"
 * argv[5]: IP address (optional)
 */

#ifdef SYSTEM_SIDE
//#define RCV_BUFFER_SIZE_IN_KBYTE		  (400/PK_MAX) //(100/PK_MAX)
#define RCV_BUFFER_SIZE_IN_KBYTE		(300/PK_MAX) //(100/PK_MAX)
#endif
#ifdef OPERATOR_SIDE
#define RCV_BUFFER_SIZE_IN_KBYTE		(200/PK_MAX) //(100/PK_MAX)
#endif

#ifdef ETHERNET_INSTALLED
guint8 pk_attach(unsigned int int_no,guint32 addr){

	IFACE_STRUCT *iface_pt;
	unsigned int class,type,number,basic;
	char sig[9];
	unsigned int iface_no;
	unsigned int i;

	PACKET_DRIVER_STRUCT *pktdrvr_pt;
	unsigned long pkt_addr;
	unsigned short vec[2];
	_go32_dpmi_seginfo dosmem;

#ifdef DEBUG_ACT
	prnt_string("pk_attach:");long2prnt(addr);prnt_char(CR);
#endif
	for(i=0;i<PK_MAX;i++){
		if(pktdrvr_st[i].iface_no >= MAX_IFACE)
			break;
	}
	if(i >= PK_MAX){
		prnt_string("Too many packet drivers\r\n");
		return FAIL;
	}
	/* Verify that there's really a packet driver there, so we don't
	 * go off into the ozone (if there's any left)
	 */
	dosmemget(int_no*4,4,vec);
	pkt_addr = vec[1] * 16 + vec[0];
	if(pkt_addr == 0){
		prnt_string("No packet driver loaded at int:");
		prnt_int(int_no);prnt_char(CR);
		return FAIL;
	}
	dosmemget(pkt_addr+3,9,sig);
	if(strcmp(sig,"PKT DRVR") != 0){
		prnt_string("No packet driver loaded at int\r\n ");
		prnt_int(int_no);
		return FAIL;
	}
//#ifdef DEBUG_ACT
	prnt_string("packet driver loaded at int:");
	prnt_int(int_no);prnt_char(CR);
//#endif
	/* Find out what we've got */
	if(driver_info(int_no,-1,NULL,&class,&type,&number,&basic) == FAIL){
		prnt_string("driver_info call failed\r\n");
		return FAIL;
	}
	pktdrvr_pt = &pktdrvr_st[i];
	dosmem.size = 64*RCV_BUFFER_SIZE_IN_KBYTE  ; /* KB -> paragraphs */
	if(_go32_dpmi_allocate_dos_memory(&dosmem)){
		prnt_string("DOS memory allocation failed\r\n");
		return FAIL;
	}
	pktdrvr_pt->dossize = dosmem.size * 16;
	pktdrvr_pt->dosbase = dosmem.rm_segment * 16;
	pktdrvr_pt->overflows = pktdrvr_pt->wptr = pktdrvr_pt->rptr = pktdrvr_pt->cnt = 0;
	pktdrvr_pt->to_be_added_cnt = 0;

	iface_no=find_free_iface_struct();
	if(iface_no >= MAX_IFACE){
		prnt_string("interface allocation failed\r\n");
		return FAIL;
	}
#ifdef H323_GATEKEEPER_SIDE
	h323_gkpr_st.iface_no=iface_no;
#endif
#ifdef H323_GATEWAY_SIDE
	h323_gw_st.iface_no=iface_no;
#endif
#ifdef PACKET_PROCESSOR_CARD
	rtp_card_st.iface_no=iface_no;
#endif
#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM) ||\
     (defined HTBY_LINK_ON_MY_UDP) || (defined KIATEL_UDP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT) || (defined SYSCONUDP))
	oam_iface_no=iface_no;
#endif
	iface_pt=&iface_st[iface_no];
	iface_pt->next = Ifaces;
	Ifaces = iface_no;
	iface_pt->outq=NULL;
	iface_pt->mtu=1500;	   /* Maximum transmission unit size */
	iface_pt->forw=NULL_IFACE;
	iface_pt->state=IN_USE;
	iface_pt->pktdrvr_no = i;
	iface_pt->addr=addr;
#ifdef H323_GATEKEEPER_SIDE
//	iface_pt->addr=gatekeeper_ip;
#endif
	pktdrvr_pt->int_no = int_no;
	pktdrvr_pt->iface_no = iface_no;

	if(iface_no == 0)
		pktdrvr_pt->rmcb_seginfo.pm_offset = (int)pkint0;
	if(iface_no == 1)
		pktdrvr_pt->rmcb_seginfo.pm_offset = (int)pkint1;

	if((i = _go32_dpmi_allocate_real_mode_callback_retf(&pktdrvr_pt->rmcb_seginfo,
		 &pktdrvr_pt->rmcb_registers)) != 0){
		prnt_string("real mode callback alloc failed:\r\n ");
		return FAIL;
	}
	pktdrvr_pt->handle = access_type(int_no,class,ANYTYPE,iface_no,pktdrvr_pt->rmcb_seginfo.rm_segment,
				 pktdrvr_pt->rmcb_seginfo.rm_offset);
	switch(class){
	case CL_ETHERNET:
		/* Get hardware Ethernet address from driver */
#ifdef ETHERNET_ALL_PACKET_MODE
		{
		    guint err=set_rcv_mode(int_no,pktdrvr_pt->handle,6);
		    prnt_string("set_rcv_mode returns");prnt_int(err);prnt_char(CR);
		}
#endif
#ifdef ETHERNET_MULTICAST_MODE
		{
		guint err;

		err=set_rcv_mode(int_no,pktdrvr_pt->handle,ETH_RCV_MODE_MODE_3_PLUS_ALL_MULTICAST_PACKETS);
		prnt_string("set_rcv_mode returns");prnt_int(err);prnt_char(CR);
		}
#endif
		iface_pt->hwaddr = malloc(EADDR_LEN);
		get_address(int_no,pktdrvr_pt->handle,iface_pt->hwaddr,EADDR_LEN);
//#ifdef DEBUG_ACT
		prnt_string("Interface address is:");
		prnt_byte(iface_pt->hwaddr[0]);prnt_char('-');
		prnt_byte(iface_pt->hwaddr[1]);prnt_char('-');
		prnt_byte(iface_pt->hwaddr[2]);prnt_char('-');
		prnt_byte(iface_pt->hwaddr[3]);prnt_char('-');
		prnt_byte(iface_pt->hwaddr[4]);prnt_char('-');
		prnt_byte(iface_pt->hwaddr[5]);prnt_char(CR);
//#endif
#ifdef USE_ETHERNET_BASED_LICENSE
		if(
			(iface_pt->hwaddr[0] != MAC_ADDR0) ||
			(iface_pt->hwaddr[1] != MAC_ADDR1) ||
			(iface_pt->hwaddr[2] != MAC_ADDR2) ||
			(iface_pt->hwaddr[3] != MAC_ADDR3) ||
			(iface_pt->hwaddr[4] != MAC_ADDR4) ||
			(iface_pt->hwaddr[5] != MAC_ADDR5)
		){
			printf("\nLicense error: 8");
//			license_error(8);
			quit();
		}
#endif
		if(iface_pt->hwaddr[0] & 1){
			prnt_string("Warning! Interface has a multicast address:\r\n");
		}
		break;
	default:
		prnt_string("Packet driver has unsupported class:");
		prnt_int(class);prnt_char(CR);
		return FAIL;
	}
	return OK;
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
guint8 driver_info(unsigned int int_no,short handle,unsigned int *version,unsigned int *class,unsigned int *type,unsigned int *number,unsigned int *basic){

	_go32_dpmi_registers reg;

	memset(&reg,0,sizeof(reg));

	reg.h.ah = DRIVER_INFO;
	reg.x.bx = handle;
	reg.h.al = 0xff;
	_go32_dpmi_simulate_int(int_no,&reg);
	if(reg.x.flags & C_FLAG){
		Derr = reg.h.dh;
		return FAIL;
	}
	if(version != NULL)
		*version = reg.x.bx;
	if(class != NULL)
		*class = reg.h.ch;
	if(type != NULL)
		*type = reg.x.dx;
	if(number != NULL)
		*number = reg.h.cl;
	if(basic != NULL)
		*basic = reg.h.al;
	return OK;
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* Send routine for packet driver					    */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
guint8 pk_send(unsigned int pktdrvr_no,unsigned int mr1){

	_go32_dpmi_registers reg;
	PACKET_DRIVER_STRUCT *pktdrvr_pt;
	unsigned int size;

	pktdrvr_pt = &pktdrvr_st[pktdrvr_no];

	if(mr1 >= NO_MEM_REC){
		prnt_msg(ETHERNET_ERR,0x5500);
		return FAIL;
	}
	size=mem_rec[mr1].size;
/*
	if(size < RUNT){
		unsigned int mr2=mem_rec_req(RUNT);
		if(mr2 >= NO_MEM_REC){
			prnt_msg(ETHERNET_ERR,0x5501);
			return FAIL;
		}
		memcpy(mem_rec[mr2].parm,mem_rec[mr1].parm,size);
		memset(&mem_rec[mr2].parm[size],0x88,RUNT-size);
		size = RUNT;
		mem_rec_rls(mr1);
		mr1=mr2;
	}
*/
//TESTTEST : uncommented
#ifdef ETHERNET_DEBUG_ACT
	prnt_string("pk_send:\r\n");
	print_mr(mr1);
#endif

	dosmemput(mem_rec[mr1].parm,size,_go32_info_block.linear_address_of_transfer_buffer);
	mem_rec_rls(mr1);
	/* Call the packet driver to send it */
	memset(&reg,0,sizeof(reg));
	reg.h.ah = SEND_PKT;
	reg.x.si = _go32_info_block.linear_address_of_transfer_buffer % 16;
	reg.x.ds = _go32_info_block.linear_address_of_transfer_buffer / 16;
	reg.x.cx = size;
	_go32_dpmi_simulate_int(pktdrvr_pt->int_no,&reg);
	if(reg.x.flags & C_FLAG){
		Derr = reg.h.dh;
#ifdef DEBUG_ACT
		prnt_string("send failed:");
		prnt_byte(Derr);prnt_char(CR);
#endif

		return FAIL;
	} else{
#ifdef DEBUG_ACT
		prnt_string("send ok:");
		prnt_int(size);prnt_char(CR);
#endif
		return OK;
	}
	return FAIL;
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* Packet driver receive upcall routine. Called by the packet driver TSR    */
/* twice for each incoming packet: first with ax == 0 to allocate a buffer, */
/* and then with ax == 1 to signal completion of the copy.		    */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
#if 0
//warning : guint->gint
	gint len,blen;
	gint i_state;
	PACKET_DRIVER_STRUCT *pktdrvr_pt;
	guint8 rdat[MAX_ETHERNET_PACKET_SIZE];
	gint i,cadj;

#ifdef DEBUG_ACT
//	prnt_string("pkint-\r\n");
#endif
	i_state = disable();
	/* This is not really legal, since handles are not guaranteed to
	 * be globally unique. But it's extremely expedient.
	 */
	for(i=0;i<PK_MAX;i++){
		if((pktdrvr_st[i].handle == reg->x.bx) &&
		   (pktdrvr_st[i].iface_no == reg->h.dl))
			break;
	}
	if(i == PK_MAX){
		reg->x.es = reg->x.di = 0;
		return; /* Unknown handle */
	}
#endif // 0
/* ======================================================================== */
/* ======================================================================== */
void restore(int state){
	state ? enable() : disable();
}
void generic_pkint(gint i,_go32_dpmi_registers *reg){
	gint len,blen;
	gint i_state;
	PACKET_DRIVER_STRUCT *pktdrvr_pt;

	i_state = disable();
	pktdrvr_pt = &pktdrvr_st[i];

	if(pktdrvr_pt->handle != reg->x.bx){
		restore(i_state);
		reg->x.es = reg->x.di = 0;
		return; /* Unknown handle */
	}

	len = reg->x.cx;

	switch(reg->x.ax){

	  case 0: /* Space allocate call */

	    if((len + sizeof(len)) > (pktdrvr_pt->dossize - pktdrvr_pt->cnt)){
		prnt_msg(ETHERNET_ERR,0x5502);
		/* Buffer overflow */
		reg->x.es = 0;
		reg->x.di = 0;
		pktdrvr_pt->overflows++;
		break;
	    }
	    if((pktdrvr_pt->wptr + len + sizeof(len)) > pktdrvr_pt->dossize){
		/* Not enough room at end of DOS buffer for length
		 * plus data, so write zero length and wrap around
		 */
		unsigned int zero = 0;
//		pktdrvr_pt->cnt += pktdrvr_pt->dossize - pktdrvr_pt->wptr;
		// save for later add
		pktdrvr_pt->to_be_added_cnt += pktdrvr_pt->dossize - pktdrvr_pt->wptr;
		dosmemput(&zero,sizeof(zero),pktdrvr_pt->dosbase+pktdrvr_pt->wptr);
		pktdrvr_pt->wptr = 0;
	    }
	    /* Write length into DOS buffer */
	    dosmemput(&len,sizeof(len),pktdrvr_pt->dosbase+pktdrvr_pt->wptr);
	    pktdrvr_pt->wptr += sizeof(len);
//	    pktdrvr_pt->cnt += sizeof(len);
	    // save for later add
	    pktdrvr_pt->to_be_added_cnt += sizeof(len);
	    pktdrvr_pt->rcvd_len=len;	    // save len
#ifdef ETHERNET_DEBUG_ACT
	    prnt_string("pk_int0:\r\n");
	    prnt_int(len);prnt_char(CR);
#endif
	    /* Pass new pointer to packet driver */
	    reg->x.es = (pktdrvr_pt->dosbase+pktdrvr_pt->wptr) / 16;
	    reg->x.di = (pktdrvr_pt->dosbase+pktdrvr_pt->wptr) % 16;
	    break;

	  case 1: /* Packet complete call */
#ifdef DEBUG_ACT
//	    prnt_string("rcv1\r\n ");
#endif
	    len=pktdrvr_pt->rcvd_len;	    // use saved len, instead of "now rcvd len"
	    blen = (len + sizeof(len) - 1) & (~(sizeof(len)-1));
	    pktdrvr_pt->wptr += blen;

	    pktdrvr_pt->cnt += pktdrvr_pt->to_be_added_cnt;
	    pktdrvr_pt->to_be_added_cnt=0;

	    pktdrvr_pt->cnt += blen;
	    if((pktdrvr_pt->wptr + sizeof(len)) > pktdrvr_pt->dossize){
		/* No room left for another len field, wrap */
		pktdrvr_pt->cnt += pktdrvr_pt->dossize - pktdrvr_pt->wptr;
		pktdrvr_pt->wptr = 0;
	    }
#if 0
// if we want to do it here instead of pktdrvr_rx_hndlr
	    {
		guint8 rdat[MAX_ETHERNET_PACKET_SIZE];
		gint cadj = 0;
		/* Extract size */
		dosmemget(pktdrvr_pt->dosbase+pktdrvr_pt->rptr,sizeof(len),&len);
		if(len == 0){
		    /* Writer wrapped around */
		    cadj += pktdrvr_pt->dossize - pktdrvr_pt->rptr;
		    pktdrvr_pt->rptr = 0;
		    dosmemget(pktdrvr_pt->dosbase,sizeof(len),&len);
		}
		dosmemget(pktdrvr_pt->dosbase+pktdrvr_pt->rptr+sizeof(len),len,&rdat[0]);
		if (write_and_check_size_buffer(&pktdrvr_rx_st[i],len,&rdat[0]) == FAIL){
		    prnt_msg(ETHERNET_ERR,0x5504);
		}
		/* figure length rounded up to next boundary */
		blen = sizeof(len) + ((len + sizeof(len) - 1) & (~(sizeof(len)-1)));
		cadj += blen;
		pktdrvr_pt->rptr += blen;
		if(pktdrvr_pt->rptr + sizeof(len) > pktdrvr_pt->dossize){
		    cadj += pktdrvr_pt->dossize - pktdrvr_pt->rptr;
		    pktdrvr_pt->rptr = 0;
		}
//		disable();
		pktdrvr_pt->cnt -= cadj;
//		enable();
#ifdef ETHERNET_DEBUG_ACT
		{
		    int j;
		    prnt_string("pktdrvr_rx_hndlr:");
		    prnt_int(len);prnt_char(':');
		    for(j=0;j<len;j++){
			prnt_byte(rdat[j]);prnt_char('-');
		    }
		    prnt_char('\r');prnt_char('\n');
		}
#endif
	    }
#endif // 0
	    break;
	  default:
	    prnt_msg(ETHERNET_ERR,0x5505);
	    break;
	}
	restore(i_state);
}
void pkint0(_go32_dpmi_registers *reg){
	generic_pkint(0,reg);
}
void pkint1(_go32_dpmi_registers *reg){
	generic_pkint(1,reg);
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* Shut down the packet interface					    */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
guint8 pk_stop(unsigned int iface_no,BYTE do_terminate){

	IFACE_STRUCT *iface_pt;
	PACKET_DRIVER_STRUCT *pktdrvr_pt;
	_go32_dpmi_seginfo dosmem;

//#ifdef DEBUG_ACT
	prnt_string("pk_stop\r");
//#endif
	iface_pt=&iface_st[iface_no];
	pktdrvr_pt = &pktdrvr_st[iface_pt->pktdrvr_no];
	if(do_terminate){
	    if(terminate(pktdrvr_pt->int_no,pktdrvr_pt->handle) == FAIL){
		prnt_string("release_type error code:");
		prnt_int(Derr);prnt_char(CR);
	    }
	}
	else{
	    /* Call driver's release_type() entry */
	    if(release_type(pktdrvr_pt->int_no,pktdrvr_pt->handle) == FAIL){
		prnt_string("release_type error code:");
		prnt_int(Derr);prnt_char(CR);
	    }
	}
	pktdrvr_pt->iface_no = NULL_IFACE;
	dosmem.size = pktdrvr_pt->dossize/16;
	dosmem.rm_segment = pktdrvr_pt->dosbase / 16;
	_go32_dpmi_free_dos_memory(&dosmem);
	_go32_dpmi_free_real_mode_callback(&pktdrvr_pt->rmcb_seginfo);
	return OK;
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
long access_type(

	unsigned int int_no,
	unsigned int if_class,
	unsigned int if_type,
	unsigned int if_number,
	unsigned int rm_segment,
	unsigned int rm_offset
){
	_go32_dpmi_registers reg;

	memset(&reg,0,sizeof(reg));
	reg.h.ah = ACCESS_TYPE; /* Access_type() function */
	reg.h.al = if_class;	/* Class */
	reg.x.bx = if_type;	/* Type */
	reg.h.dl = if_number;	/* Number */
	reg.x.es = rm_segment;	/* Address of rm receive handler */
	reg.x.di = rm_offset;
//MY CODE
	reg.x.cx = 0; // match all packets
	_go32_dpmi_simulate_int(int_no,&reg);
	if(reg.x.flags & C_FLAG){
		Derr = reg.h.dh;
		return FAIL;
	} else
		return reg.x.ax;
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
guint8 release_type(unsigned int int_no,short handle){

	_go32_dpmi_registers reg;

	memset(&reg,0,sizeof(reg));
	reg.x.bx = handle;
	reg.h.ah = RELEASE_TYPE;
	_go32_dpmi_simulate_int(int_no,&reg);
	if(reg.x.flags & C_FLAG){
		Derr = reg.h.dh;
		return FAIL;
	}
	else{
		return OK;
	}
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
guint8 terminate(unsigned int int_no,short handle){

	_go32_dpmi_registers reg;

	memset(&reg,0,sizeof(reg));
	reg.x.bx = handle;
	reg.h.ah = TERMINATE;
	_go32_dpmi_simulate_int(int_no,&reg);
	if(reg.x.flags & C_FLAG){
		Derr = reg.h.dh;
		return FAIL;
	}
	else{
		return OK;
	}
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
guint8 get_address(unsigned int int_no,short handle,guint8 *buf,unsigned int len){

	_go32_dpmi_registers reg;

	memset(&reg,0,sizeof(reg));
	reg.h.ah = GET_ADDRESS;
	reg.x.bx = handle;
	reg.x.di = _go32_info_block.linear_address_of_transfer_buffer % 16;
	reg.x.es = _go32_info_block.linear_address_of_transfer_buffer / 16;
	reg.x.cx = len;
	_go32_dpmi_simulate_int(int_no,&reg);
	if(reg.x.flags & C_FLAG){
		Derr = reg.h.dh;
		return FAIL;
	}
	dosmemget(_go32_info_block.linear_address_of_transfer_buffer,len,buf);
	return OK;
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
guint8 set_rcv_mode(unsigned int int_no,short handle,unsigned int mode){

	_go32_dpmi_registers reg;

	memset(&reg,0,sizeof(reg));
	reg.h.ah = SET_RCV_MODE;
	reg.x.cx = mode;
	reg.x.bx = handle;
	_go32_dpmi_simulate_int(int_no,&reg);
	if(reg.x.flags & C_FLAG){
		Derr = reg.h.dh;
		return FAIL;
	}
	return OK;
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
void pktdrvr_tx_hndlr(void){

//BENZINBENZIN : 20000->
	guint8 tdat[PKTDRVR_BUF_PACKET_SIZE];
	unsigned int len;
	guint i;

	for(i=0;i<PK_MAX;i++){
		if(pktdrvr_st[i].iface_no >= MAX_IFACE) continue;
//		if (read_and_check_size_buffer(&pktdrvr_tx_st[i],&len,&tdat[0],PKTDRVR_BUF_PACKET_SIZE) == OK){
		while (read_and_check_size_buffer(&pktdrvr_tx_st[i],&len,&tdat[0],PKTDRVR_BUF_PACKET_SIZE) == OK){

			WORD_MR mr1;

			mr1=mem_rec_req(len);
			if(mr1 < NO_MEM_REC){
				mem_cpy(mem_rec[mr1].parm,tdat,len);
#ifdef DEBUG_ACT
/*
				prnt_string("lower level tx:\r\n");
				good_print(tdat,len);
*/
#endif
				pk_send(i,mr1);
			}
		}
	}
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
void pktdrvr_rx_hndlr(void){
	unsigned int len, blen;
	guint i;
	PACKET_DRIVER_STRUCT *pktdrvr_pt;

	for(i=0,pktdrvr_pt=&pktdrvr_st[0];i<PK_MAX;i++,pktdrvr_pt++){
	    if(pktdrvr_pt->iface_no >= MAX_IFACE) continue;
	    while(pktdrvr_pt->cnt > 0){
		guint8 rdat[MAX_ETHERNET_PACKET_SIZE];
		gint cadj = 0;
		/* Extract size */
		dosmemget(pktdrvr_pt->dosbase+pktdrvr_pt->rptr,sizeof(len),&len);
		if(len == 0){
		    /* Writer wrapped around */
		    cadj += pktdrvr_pt->dossize - pktdrvr_pt->rptr;
		    pktdrvr_pt->rptr = 0;
		    dosmemget(pktdrvr_pt->dosbase,sizeof(len),&len);
		}
		dosmemget(pktdrvr_pt->dosbase+pktdrvr_pt->rptr+sizeof(len),len,&rdat[0]);

// if we did it here instead of in generic_pkint, then no need to use
// pktdrvr_rx_st, we may immediately do eproc
/*
		if (write_and_check_size_buffer(&pktdrvr_rx_st[i],len,&rdat[0]) == FAIL){
		    prnt_msg(ETHERNET_ERR,0x5504);
		}
*/
		/* figure length rounded up to next boundary */
		blen = sizeof(len) + ((len + sizeof(len) - 1) & (~(sizeof(len)-1)));
		cadj += blen;
		pktdrvr_pt->rptr += blen;
		if(pktdrvr_pt->rptr + sizeof(len) > pktdrvr_pt->dossize){
		    cadj += pktdrvr_pt->dossize - pktdrvr_pt->rptr;
		    pktdrvr_pt->rptr = 0;
		}
		disable();
		pktdrvr_pt->cnt -= cadj;
		enable();
#ifdef ETHERNET_DEBUG_ACT
		{
		    int j;
		    prnt_string("pktdrvr_rx_hndlr:");
		    prnt_int(len);prnt_char(':');
		    for(j=0;j<len;j++){
			prnt_byte(rdat[j]);prnt_char('-');
		    }
		    prnt_char('\r');prnt_char('\n');
		}
#endif

		{
		    WORD_MR mr1;
		    mr1=mem_rec_req(len);
		    if(mr1 < NO_MEM_REC){
			mem_cpy(mem_rec[mr1].parm,rdat,len);
			eproc(pktdrvr_pt->iface_no,mr1);
		    }
		}

	    }
#if 0
//	    if (read_and_check_size_buffer(&pktdrvr_rx_st[i],&len,&rdat[0],PKTDRVR_BUF_PACKET_SIZE) == OK){
	    while (read_and_check_size_buffer(&pktdrvr_rx_st[i],&len,&rdat[0],PKTDRVR_BUF_PACKET_SIZE) == OK){
		WORD_MR mr1;
		mr1=mem_rec_req(len);
		if(mr1 < NO_MEM_REC){
		    unsigned int iface_no;
		    mem_cpy(mem_rec[mr1].parm,rdat,len);
#ifdef DEBUG_ACT
/*
		    prnt_string("lower level rx:\r\n");
		    good_print(rdat,len);
*/
		    prnt_string("ether_rx:");
		    print_mr(mr1);
#endif
		    iface_no=pktdrvr_st[i].iface_no;
		    eproc(iface_no,mr1);
		}
	    }
#endif //0
	}
}
#endif // ETHERNET_INSTALLED
/* ========================================================================== */
/* ========================================================================== */
#ifdef ETHERNET_INSTALLED
void init_pktdrvr_buf(unsigned int i){

	pktdrvr_tx_st[i].arr = &pktdrvr_tx[i][0];
	pktdrvr_tx_st[i].size = PK_BUF_SIZE;
	pktdrvr_tx_st[i].rear  = pktdrvr_tx_st[i].front=0;

/*
	pktdrvr_rx_st[i].arr = &pktdrvr_rx[i][0];
	pktdrvr_rx_st[i].size = PK_BUF_SIZE;
	pktdrvr_rx_st[i].rear  = pktdrvr_rx_st[i].front=0;
*/
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
void init_pktdrvr_st(void){

	guint i;
	for(i=0;i<PK_MAX;i++){
		pktdrvr_st[i].iface_no=NULL_IFACE;
		init_pktdrvr_buf(i);
	}
#ifdef H323_GATEKEEPER_SIDE
	h323_gkpr_st.iface_no=NULL_IFACE;
#endif
#ifdef H323_GATEWAY_SIDE
	h323_gw_st.iface_no=NULL_IFACE;
#endif
#ifdef PACKET_PROCESSOR_CARD
	rtp_card_st.iface_no=NULL_IFACE;
#endif
#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM) ||\
     (defined HTBY_LINK_ON_MY_UDP) || (defined KIATEL_UDP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT) || (defined SYSCONUDP))
	oam_iface_no=NULL_IFACE;
#endif
}
#endif // ETHERNET_INSTALLED
/* ======================================================================== */
/* ======================================================================== */
#ifdef ETHERNET_INSTALLED
guint8 all_pk_attach(void){
	guint8 ret_value=FAIL;
#ifdef DEBUG_ACT
	prnt_string("all_pk_attach\r");
#endif
#ifdef H323_GATEKEEPER_SIDE
	ret_value=pk_attach(kt_system_st.int_no,gatekeeper_ip);
#endif
#ifdef ROUTER_SYSTEM
	{
	    guint i;
	    for(i=0;i<MAX_ROUTER_PATH;i++){
		if(pk_attach(router_st[i].int_no,router_st[i].addr) == FAIL)
		    break;
	    }
	    if(i == MAX_ROUTER_PATH) ret_value=OK;
	}
#endif
#ifdef H323_GATEWAY_SIDE
	ret_value=pk_attach(kt_system_st.int_no,h323_gw_st.address);
#endif
#ifdef PACKET_PROCESSOR_CARD
	ret_value=pk_attach(kt_system_st.int_no,rtp_card_st.address);
#endif
#if ((defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM) ||\
     (defined HTBY_LINK_ON_MY_UDP) || (defined KIATEL_UDP_FOR_OAM) ||\
     (defined KFTPD_INSTALLED) || (defined KFTP_INSTALLED) ||\
     (defined ROS_FOR_LI_INSTALLED) || (defined TELNETD_INSTALLED) ||\
     (defined HI1_INSTALLED) || (defined HI1_DISTRIBUTOR) ||\
     (defined SYSCON_SERVER) || (defined SYSCON_CLIENT) || (defined SYSCONUDP))
#ifdef DYNAMIC_IP_ALLOCATION
#if ((defined HOT_STANDBY) && (defined SYSTEM_SIDE))
	if (thisside.hot == SET)
#endif
	    ret_value=pk_attach(kt_system_st.int_no, kt_system_st.hot_ipv4_address);
#if ((defined HOT_STANDBY) && (defined SYSTEM_SIDE))
	else
	    ret_value=pk_attach(kt_system_st.int_no, kt_system_st.standby_ipv4_address);
#endif
#else
	ret_value=pk_attach(kt_system_st.int_no, kt_system_st.ipv4_address);
#endif
#endif

	if(ret_value == FAIL){
/*
		clear_screen();
		printf("\nerror in packet driver");
#ifdef SYSTEM_SIDE
		quit();
#endif
#ifdef OPERATOR_SIDE
		quit(2);
#endif
*/
		return FAIL;
	}
	else
		return OK;
}
#endif // ETHERNET_INSTALLED
