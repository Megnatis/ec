
/* ======================================================================= */
/* ======================================================================= */
void mxxdbr(unsigned int a,BYTE h){  /* make ext. to ext. detail billing record */
	unsigned short d;
	DBR *bp;
	BYTE *b,t,*bsave;
	unsigned int *e;
	bp=&dbr[a];
// size shall be calculated at the end
//	bp->s=EXT_EXT_RECORD_SIZE + 3;	  /* for record type,record size,record check sum */
//	bp->ri[RECORD_SIZE_POSITION]=EXT_EXT_RECORD_SIZE;
#ifdef BEHNAMIR
	d=db[h][0];	  /* source index to d */
	t=db[h][MAX_DETAIL_BILLING_INFO_SIZE] >> 4;
#else
	d=db[h][0] >> 4;  /* source index to d */
	t=db[h][0] & 0x0f;
#endif
#ifdef CB_INSTALLED
	if(t == CB_T)
		bp->ri[RECORD_TYPE_POSITION]=CB_EXT_RECORD;
	else
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	if(t == OPR_T)
	    bp->ri[RECORD_TYPE_POSITION]=OPR_EXT_RECORD;
	else
#endif
	if(t == XST_T)
		bp->ri[RECORD_TYPE_POSITION]=EXT_EXT_RECORD;
	else{
		bp->ri[RECORD_TYPE_POSITION]=TRUNK_EXT_RECORD;
		d=(d & 0x0fff) | ((t & 0x0f) << 12);
	}
#ifdef LIM
#if(NO_EXTENSION_C != 0)
	if(h==2)
		d+=(NO_EXTENSION_A+NO_EXTENSION_B);
	else
#endif
	if(h==1)
		d+=NO_EXTENSION_A;
#endif
	b=&bp->ri[RECORD_FIRST_BYTE];
	*b=d;
	b++;
	d >>= 8;
	*b=d;
	b++;
	/* copy source to record */

#ifdef BEHNAMIR
	d=db[h][1];	  /* destination index to d */
#else
	d=db[h][1] >> 4;  /* destination index to d */
#endif
#ifdef LIM
#if(NO_EXTENSION_C != 0)
	if(h==2)
		d+=(NO_EXTENSION_A+NO_EXTENSION_B);
	else
#endif
	if(h==1)
		d+=NO_EXTENSION_A;
#endif
	*b=d;
	b++;
	d >>= 8;
	*b=d;
	b++;
	/* copy destination to record */

#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
	b+=3;
#else
#ifdef SYSTEM_SIDE
	b+=3;	// leave it free to be filled by operator
#endif
#ifdef OPERATOR_SIDE
	sdt=*get_dt(&sdt);
	*b=sdt.hr;
	b++;
	*b=sdt.mi;
	b++;
	*b=sdt.se;
	b++;
	/* save current time = end time of conversation within a gaurd margin */
#endif
#endif

	d=db[h][2];
	*b=d;
	b++;
	d >>= 8;
	*b=d;
	b++;
	/* copy time of conversation */
	e=&db[h][3];

#ifdef RECORD_A_NUMBER
	{
	    BYTE i,no_dgt;
	    d=*e;     // no_dgt
	    e++;
	    no_dgt=d;
	    if(bp->ri[RECORD_TYPE_POSITION] == TRUNK_EXT_RECORD){
		*b=no_dgt;
		b++;
	    }
	    if(no_dgt <= 20){
		for(i=0;i<no_dgt;){
		    d=*e;
		    e++;
		    if(bp->ri[RECORD_TYPE_POSITION] == TRUNK_EXT_RECORD){
			*b=d&0xff;
			b++;
			*b=(d>>8)&0xff;
			b++;
		    }
		    i+=4;
		}
	    }
	}
#endif

#ifdef REPORT_BULK_IN_DETAIL
	d=*e;
	*b=d;
	b++;
	d >>= 8;
	*b=d;
	b++;
	e++;
#endif
#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
	d=*e;	  // yr,mo
	e++;
	billing_sdt.yr=(d&0xff00) >> 8;
	billing_sdt.mo=(d&0x00ff);
	d=*e;		// da,hr
	e++;
	billing_sdt.da=(d&0xff00) >> 8;
	bp->ri[6]=(d&0x00ff);
	d=*e;		// mi,se
	e++;
	bp->ri[7]=(d&0xff00) >> 8;
	bp->ri[8]=(d&0x00ff);
#endif
	d=*e;	// dummy line to avoid warning when we have neither of:
		//#ifdef RECORD_A_NUMBER
		//#ifdef REPORT_BULK_IN_DETAIL
		//#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR


// start, calculate record size
	bsave=&bp->ri[RECORD_SIZE_POSITION];
	d=(unsigned short)(b - bsave);
	d--;
	bp->ri[RECORD_SIZE_POSITION]=d;
	/* save record size */

	bp->s=d + 3;  /* for record type,size & checksum */
// end, calculate record size
// *b is the place to put checksum
}
/* ======================================================================= */
/* ======================================================================= */
void copy_digits_no_prev_nibble(BYTE **b_ptr,BYTE h,unsigned int **e2){
	long d=db[h][POSITION_NUMBER_DIGIT_IN_DB];
	unsigned int *e,f;
	BYTE *b=(*b_ptr);
	{
	    *b=d;
	    b++;
	    /* copy number of digits */

	    for(e=&db[h][POSITION_D4D3_D2D1];d > 0;e++){
		f=*e;  /* process lower byte of f */
		*b=f;
		d--;
		d--;
		b++;
		if(d > 0){  /* process higher byte of f */
		    *b=f >> 8;
		    d--;
		    d--;
		    b++;
		}
		else{
		    e++;
		    break;
		}
	    }
	    /* copy digits */
	}
	(*e2)=e;
	(*b_ptr)=b;
}
/* ======================================================================= */
/* ======================================================================= */
void copy_digits_add_prev_nibble(BYTE **b_ptr,BYTE h,unsigned int **e2){
	long d=db[h][POSITION_NUMBER_DIGIT_IN_DB];
	unsigned int *e,f;
	BYTE prev_nibble;
	BYTE *position_of_nd;
#define TRUNK_TYPE (db[h][1] & 0x0f)
	BYTE *b=(*b_ptr);

		// add first dialled zero

		position_of_nd=b;
		b++;
		/* for now, don't copy number of digits, to be decided later, only save its address */

#if (NO_CO != 0)
		if(TRUNK_TYPE == CO_T)
			prev_nibble=CO_REQ;
		else
#endif
#ifdef EM_INSTALLED
		if(TRUNK_TYPE == EM_T)
			prev_nibble=EM_REQ;
		else
#endif
#ifdef CAS3_INSTALLED
		if(TRUNK_TYPE == CAS3_T)
			prev_nibble=CAS3_REQ;
		else
#endif
#ifdef CAS1_INSTALLED
		if(TRUNK_TYPE == CAS1_T)
			prev_nibble=CAS1_REQ;
		else
#endif
#ifndef NO_TIE_INIT
		if(TRUNK_TYPE == TIE_T)
			prev_nibble=TIE_REQ;
		else
#endif
			prev_nibble=0x0a;
		for(e=&db[h][POSITION_D4D3_D2D1];d > 0;e++){
			f=*e;  /* process lower byte of f */
			*b=((f&0x000f)<<4) | prev_nibble ;
			prev_nibble=(f&0x00f0) >> 4;
			d--;
			d--;
			b++;
			if(d > 0){  /* process higher byte of f */
				f >>= 8;
				*b=((f&0x0f)<<4) | prev_nibble ;
				prev_nibble=(f&0xf0) >> 4;
				d--;
				d--;
				b++;
			}
			else{
				e++;
				break;
			}
		}
		if(prev_nibble != 0x0f){
			*b=prev_nibble | 0xf0;
			b++;
			*position_of_nd=db[h][POSITION_NUMBER_DIGIT_IN_DB]+2;
		}
		else
			*position_of_nd=db[h][POSITION_NUMBER_DIGIT_IN_DB];
		/* copy digits, and also copy number of digits */
	(*e2)=e;
	(*b_ptr)=b;
}
/* ======================================================================= */
/* ======================================================================= */
void mxtdbr(unsigned int a,BYTE h){  /* make ext. to trunk detail billing record */
	long d;
	DBR *bp;
	BYTE *b,*bsave;
	BYTE trunk_type,t;
#if ((defined EM_INSTALLED) && (defined EM_PULSE_SIGNALING) && (defined EXTENDED_LX_EM) && (defined GROUPING_FOR_EXTENDED_LX)) \
		  || ((defined CAS3_INSTALLED) && (defined EXTENDED_LX_CAS3) && (defined GROUPING_FOR_EXTENDED_LX)) \
		  || ((defined CAS1_INSTALLED) && (defined EXTENDED_LX_CAS1) && (defined GROUPING_FOR_EXTENDED_LX)) \
		  || ((defined CAS1_INSTALLED) && (defined TRANSIT_CAS1))
	unsigned int trunk_no_flag;
#endif
	unsigned int *e;

	if(db[h][POSITION_NUMBER_DIGIT_IN_DB] > 36){
		// db[][] has reserved room for 9 16-bit words each packing 4 digits hence at most 36 digits
		db[h][POSITION_NUMBER_DIGIT_IN_DB]=36;
	}

	bp=&dbr[a];
#ifdef BEHNAMIR
	d=db[h][0];	  /* source index to d */
	t=db[h][MAX_DETAIL_BILLING_INFO_SIZE] >> 4;
#else
	d=db[h][0] >> 4;  /* source index to d */
	t=db[h][0] & 0x0f;
#endif
#ifdef CB_INSTALLED
	if(t == CB_T)
		bp->ri[RECORD_TYPE_POSITION]=CB_TRUNK_RECORD;
	else
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	if(t == OPR_T)
		bp->ri[RECORD_TYPE_POSITION]=OPR_TRUNK_RECORD;
	else
#endif
	if(t == XST_T)
		bp->ri[RECORD_TYPE_POSITION]=EXT_TRUNK_RECORD;
	else{
		bp->ri[RECORD_TYPE_POSITION]=TRUNK_TRUNK_RECORD;
		d=(d & 0x0fff) | ((t & 0x0f) << 12);
	}
#ifdef LIM
#if(NO_EXTENSION_C != 0)
	if(h==2)
		d+=(NO_EXTENSION_A+NO_EXTENSION_B);
	else
#endif
	if(h==1)
		d+=NO_EXTENSION_A;
#endif
	b=&bp->ri[RECORD_FIRST_BYTE];
	*b=d;
	b++;
	d >>= 8;
	*b=d;
	b++;
	/* copy source to record */

	d=db[h][1];

#if ((defined EM_INSTALLED) && (defined EM_PULSE_SIGNALING) && (defined EXTENDED_LX_EM) && (defined GROUPING_FOR_EXTENDED_LX)) \
		  || ((defined CAS3_INSTALLED) && (defined EXTENDED_LX_CAS3) && (defined GROUPING_FOR_EXTENDED_LX)) \
		  || ((defined CAS1_INSTALLED) && (defined EXTENDED_LX_CAS1) && (defined GROUPING_FOR_EXTENDED_LX)) \
		  || ((defined CAS1_INSTALLED) && (defined TRANSIT_CAS1))
	trunk_no_flag=(unsigned int)d & 0x8000 ? SET : RESET;
	d &= 0x7fff;
#endif

#ifdef BEHNAMIR
	trunk_type=db[h][MAX_DETAIL_BILLING_INFO_SIZE] & 0x0f;
#else
	trunk_type=d & 0x0f;  /* destination type */
	d >>= 4;
#endif
	*b=trunk_type;
	b++;

	*b=d;
	b++;
	/* copy destination to record */

#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
	b+=3;
#else
#ifdef SYSTEM_SIDE
	b+=3;	// leave it free to be filled by operator
#endif
#ifdef OPERATOR_SIDE
	sdt=*get_dt(&sdt);
	*b=sdt.hr;
	b++;
	*b=sdt.mi;
	b++;
	*b=sdt.se;
	b++;
	/* save current time = end time of conversation within a gaurd margin */
#endif
#endif

	d=db[h][2];
	*b=d;
	b++;
	d >>= 8;
	*b=d;
	b++;
	/* copy time of conversation */

#if ((NO_ANALOG_TRUNK != 0) && (defined KIATEL_TREE_BASED_NP))
	if(trunk_type == CO_T){
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
	}
#endif
	if(trunk_type == PRI_NETWORK_T){
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
	}
	if(trunk_type == PRI_T){
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
	}
	if(trunk_type == SERVICE_T){
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
	}
//SHADOWSHADOW start : to be asked
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	if(trunk_type == OPR_T){
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
	}
#endif
//SHADOWSHADOW end
#ifdef NEW_CC_DETAIL_BILLING
	if(trunk_type == NEWCC_T){
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
	}
#endif

#ifdef TRANSIT_DT_EM
	if(trunk_type == CAS3_T){
#if ((defined EXTENDED_LX_CAS3) && (defined GROUPING_FOR_EXTENDED_LX))
		if(trunk_no_flag){
			copy_digits_add_prev_nibble(&b,h,&e);
			goto next;
		}
		else{
			copy_digits_no_prev_nibble(&b,h,&e);
			goto next;
		}
#endif
#if ((defined EXTENDED_LX_CAS3) && (!defined GROUPING_FOR_EXTENDED_LX))
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
#endif
#if (!defined EXTENDED_LX_CAS3)
		if(h == 0){
			copy_digits_add_prev_nibble(&b,h,&e);
			goto next;
		}
		else{
			copy_digits_no_prev_nibble(&b,h,&e);
			goto next;
		}
#endif
	}
	else
	if(trunk_type == EM_T){
#if ((defined EXTENDED_LX_EM) && (defined GROUPING_FOR_EXTENDED_LX))
		if(trunk_no_flag){
			copy_digits_add_prev_nibble(&b,h,&e);
			goto next;
		}
		else{
			copy_digits_no_prev_nibble(&b,h,&e);
			goto next;
		}
#endif
#if ((defined EXTENDED_LX_EM) && (!defined GROUPING_FOR_EXTENDED_LX))
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
#endif
#if (!defined EXTENDED_LX_EM)
		copy_digits_add_prev_nibble(&b,h,&e);
		goto next;
#endif
	}
#ifdef LIM
	else
	if(trunk_type == LIM_T){
		copy_digits_no_prev_nibble(&b,h,&e);
		goto next;
	}
#endif
#endif	// TRANSIT_DT_EM

#ifdef ISUP
	if(trunk_type == ISUP_T)
		goto no_prev_nibble;
#endif
#ifdef SIP_INSTALLED
	if(trunk_type == SIP_T)
		goto no_prev_nibble;
#endif
#ifdef NO_TIE_INIT
	if(trunk_type == 0x0a)	// case of add_office_code
		goto no_prev_nibble;
#endif

#if ( ( (!defined EXTENDED_LX_EM) && (!defined EXTENDED_LX_CAS3) && (!defined EXTENDED_LX_CAS1) ) \
      || ((defined EM_INSTALLED) && (defined EM_PULSE_SIGNALING) && (defined EXTENDED_LX_EM) && (defined GROUPING_FOR_EXTENDED_LX))		\
      || ((defined CAS3_INSTALLED) && (defined EXTENDED_LX_CAS3) && (defined GROUPING_FOR_EXTENDED_LX)) 					\
      || ((defined CAS1_INSTALLED) && (defined EXTENDED_LX_CAS1) && (defined GROUPING_FOR_EXTENDED_LX)) 					\
      || ((defined CAS1_INSTALLED) && (defined TRANSIT_CAS1))											\
    )

#if ((defined LIM) || \
     ((defined EM_INSTALLED) && (defined EM_PULSE_SIGNALING) && (defined EXTENDED_LX_EM) && (defined GROUPING_FOR_EXTENDED_LX)) || \
     ((defined CAS3_INSTALLED) && (defined EXTENDED_LX_CAS3) && (defined GROUPING_FOR_EXTENDED_LX)) || \
     ((defined CAS1_INSTALLED) && (defined EXTENDED_LX_CAS1) && (defined GROUPING_FOR_EXTENDED_LX)) || \
     ((defined CAS1_INSTALLED) && (defined TRANSIT_CAS1)) \
    )
	if(
#if ((defined LIM) &&  \
     !((defined EM_INSTALLED) && (defined EM_PULSE_SIGNALING) && (defined EXTENDED_LX_EM) && (defined GROUPING_FOR_EXTENDED_LX)) && \
     !((defined CAS3_INSTALLED) && (defined EXTENDED_LX_CAS3) && (defined GROUPING_FOR_EXTENDED_LX)) && \
     !((defined CAS1_INSTALLED) && (defined EXTENDED_LX_CAS1) && (defined GROUPING_FOR_EXTENDED_LX)) && \
     !((defined CAS1_INSTALLED) && (defined TRANSIT_CAS1)) \
    )
	((trunk_type != LIM_T) && (h==0))
#endif
#if (	\
      ((defined EM_INSTALLED) && (defined EM_PULSE_SIGNALING) && (defined EXTENDED_LX_EM) && (defined GROUPING_FOR_EXTENDED_LX)) || \
      ((defined CAS3_INSTALLED) && (defined EXTENDED_LX_CAS3) && (defined GROUPING_FOR_EXTENDED_LX)) || \
      ((defined CAS1_INSTALLED) && (defined EXTENDED_LX_CAS1) && (defined GROUPING_FOR_EXTENDED_LX)) || \
      ((defined CAS1_INSTALLED) && (defined TRANSIT_CAS1)) \
    )
	(trunk_no_flag)
#endif
	)
#endif
	{
		copy_digits_add_prev_nibble(&b,h,&e);
		goto next;
	}
#if ((defined LIM) || \
     ((defined EM_INSTALLED) && (defined EM_PULSE_SIGNALING) && (defined EXTENDED_LX_EM) && (defined GROUPING_FOR_EXTENDED_LX)) || \
     ((defined CAS3_INSTALLED) && (defined EXTENDED_LX_CAS3) && (defined GROUPING_FOR_EXTENDED_LX)) || \
     ((defined CAS1_INSTALLED) && (defined EXTENDED_LX_CAS1) && (defined GROUPING_FOR_EXTENDED_LX)) || \
     ((defined CAS1_INSTALLED) && (defined TRANSIT_CAS1)) \
    )
	else
#endif
#endif

	{
#if ( (defined EXTENDED_LX_EM)	 ||    \
      (defined EXTENDED_LX_CAS3) ||    \
      (defined EXTENDED_LX_CAS1) ||    \
      (defined LIM)		 ||    \
      (defined ISUP)		 ||    \
      (defined NO_TIE_INIT)	       \
    )
no_prev_nibble:
		copy_digits_no_prev_nibble(&b,h,&e);
#endif
	}

next:

#ifdef RECORD_A_NUMBER
	{
	    BYTE i,no_dgt;
	    d=*e;     // no_dgt
	    e++;
	    no_dgt=d;
	    if(bp->ri[RECORD_TYPE_POSITION] == TRUNK_TRUNK_RECORD){
		*b=no_dgt;
		b++;
	    }
	    if(no_dgt <= 20){
		for(i=0;i<no_dgt;){
		    d=*e;
		    e++;
		    if(bp->ri[RECORD_TYPE_POSITION] == TRUNK_TRUNK_RECORD){
			*b=d&0xff;
			b++;
			*b=(d>>8)&0xff;
			b++;
		    }
		    i+=4;
		}
	    }
	}
#endif

#ifdef REPORT_BULK_IN_DETAIL
	d=*e;
	*b=d;
	b++;
	d >>= 8;
	*b=d;
	b++;
	e++;
#endif
#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
	d=*e;		// yr,mo
	e++;
	billing_sdt.yr=(d&0xff00) >> 8;
	billing_sdt.mo=(d&0x00ff);
	d=*e;		// da,hr
	e++;
	billing_sdt.da=(d&0xff00) >> 8;
	bp->ri[6]=(d&0x00ff);
	d=*e;		// mi,se
	e++;
	bp->ri[7]=(d&0xff00) >> 8;
	bp->ri[8]=(d&0x00ff);
#endif

	/* b points to checksum position */
// 27/6/77, there are some signs that following may have resulted incorrect
// we found records (44), whose size was either 91 or 92 and indeed 91 or 92
// bytes recorded in dbrf file including the first bytes being a correct
// billing record of type 44 (ext to trunk). So we added type casting.
	bsave=&bp->ri[RECORD_SIZE_POSITION];
	d=(long)(b - bsave);
	d--;
	bp->ri[RECORD_SIZE_POSITION]=d;
	/* save record size */

	bp->s=d + 3;  /* for record type,size & checksum */
// *b is the place to put checksum
}
/* ======================================================================= */
/* ======================================================================= */
BYTE rdb(BYTE h,unsigned int a){  /* report detail billing */
	unsigned int t;

#if ((!defined DO_REPORT_ICT_TRUNK_BILLING) && (!defined RECORD_A_NUMBER))
#ifdef BEHNAMIR
	t=db[h][MAX_DETAIL_BILLING_INFO_SIZE] >> 4;
#else
	t=db[h][0] & 0x0f;
#endif
	if( ( t != XST_T)  /* if source not extension */
#ifdef CB_INSTALLED
	    && (t != CB_T)  /* if source not coin box */
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	    && (t != OPR_T)  /* if source not operator */
#endif
	)
		return FAIL;
#endif

#ifdef BEHNAMIR
	t=db[h][MAX_DETAIL_BILLING_INFO_SIZE] & 0x0f;
#else
	t=db[h][1] & 0x0f;  /* destination type */
#endif
	switch(t){
		case XST_T:
				mxxdbr(a,h);
				return OK;
		case CO_T:
#ifndef NO_TIE_INIT
		case TIE_T:
#else
		case 0x0a:	// used for add_office_code
#endif
#ifdef EM_INSTALLED
		case EM_T:	// also case NEWCC_T(!), this also applies to dosbill(!)
#endif
#ifdef CAS3_INSTALLED
		case CAS3_T:
#endif
#ifdef ISUP
		case ISUP_T:
#endif
#ifdef CAS1_INSTALLED
		case CAS1_T:
#endif
#ifdef LIM
		case LIM_T:
#endif
		case SERVICE_T:
		case PRI_T:
		case PRI_NETWORK_T:
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
		case OPR_T:
#endif
#ifdef SIP_INSTALLED
		case SIP_T:
#endif
				mxtdbr(a,h);
				return OK;
		default:
#ifdef SYSTEM_SIDE
				mes2prnti(mess[ERROR],0x920b);
				byte2prnt(t);
				toprnq(CR);
#endif
				return FAIL;
	}
}
/* ======================================================================= */
/* ======================================================================= */
#ifdef STORE_SUBSCRIBER_DIRECTORY_NUMBER_IN_BILLING_FILE
void store_subscriber_directory_number(unsigned int a){
	SDNST src,dst;
	DBR *bp=&dbr[a];
	switch(bp->ri[0]){
	  case EXT_EXT_RECORD:
#if ((defined SYSTEM_SIDE) && (defined LIM) && (SELF_CENTER != A_CENTER))
#if (SELF_CENTER == B_CENTER)
	    src=find_subscriber_no((bp->ri[2]|(bp->ri[3]<<8))-NO_EXTENSION_A);
cont1:	    dst=find_subscriber_no((bp->ri[4]|(bp->ri[5]<<8))-NO_EXTENSION_A);
#endif
#if (SELF_CENTER == C_CENTER)
	    src=find_subscriber_no((bp->ri[2]|(bp->ri[3]<<8))-(NO_EXTENSION_A+NO_EXTENSION_B));
cont1:	    dst=find_subscriber_no((bp->ri[4]|(bp->ri[5]<<8))-(NO_EXTENSION_A+NO_EXTENSION_B));
#endif
#else
#ifdef SYSTEM_SIDE
	    {
		unsigned int ext=bp->ri[2]|(bp->ri[3]<<8);
		if(ext >= NO_EXTENSION){
		    int i;
		    mes2prnti(mess[ERROR],0x9209);
		    int2prnt(ext);toprnq('-');byte2prnt(bp->s);toprnq(CR);
		    for(i=0;i<bp->s;i++){
			byte2prnt(bp->ri[i]);toprnq(CR);
		    }
		    src=0;
		    goto cont1;
		}
	    }
#endif
	    src=find_subscriber_no(bp->ri[2]|(bp->ri[3]<<8));
cont1:	    dst=find_subscriber_no(bp->ri[4]|(bp->ri[5]<<8));
#endif
	    // store them just before checksum
	    bp->s+=(SDNS_NBB*2);
	    bp->ri[RECORD_TYPE_POSITION]+=1;
	    bp->ri[RECORD_SIZE_POSITION]+=(SDNS_NBB*2);
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    bp->ri[RECORD_TYPE_POSITION]+=4;
	    bp->ri[bp->s-11]=src/0x100000000LL;
	    bp->ri[bp->s-10]=src/0x1000000L;
	    bp->ri[bp->s-9] =src/0x10000L;
	    bp->ri[bp->s-8] =src/0x100L;
	    bp->ri[bp->s-7] =src;
	    bp->ri[bp->s-6]=dst/0x100000000LL;
	    bp->ri[bp->s-5]=dst/0x1000000L;
	    bp->ri[bp->s-4]=dst/0x10000L;
	    bp->ri[bp->s-3]=dst/0x100L;
	    bp->ri[bp->s-2]=dst;
#else
	    bp->ri[bp->s-9]=src/0x1000000L;
	    bp->ri[bp->s-8]=src/0x10000L;
	    bp->ri[bp->s-7]=src/0x100L;
	    bp->ri[bp->s-6]=src;
	    bp->ri[bp->s-5]=dst/0x1000000L;
	    bp->ri[bp->s-4]=dst/0x10000L;
	    bp->ri[bp->s-3]=dst/0x100L;
	    bp->ri[bp->s-2]=dst;
#endif
	    break;
#ifdef CB_INSTALLED
	  case CB_EXT_RECORD:
	    src=(bp->ri[2]|(bp->ri[3]<<8))+first_cb_number;
	    goto cont1;
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	  case OPR_EXT_RECORD:
	    src=(bp->ri[2]|(bp->ri[3]<<8));
	    goto cont1;
#endif
	  case EXT_TRUNK_RECORD:
#if ((defined SYSTEM_SIDE) && (defined LIM) && (SELF_CENTER != A_CENTER))
#if (SELF_CENTER == B_CENTER)
	    src=find_subscriber_no((bp->ri[2]|(bp->ri[3]<<8))-NO_EXTENSION_A);
#endif
#if (SELF_CENTER == C_CENTER)
	    src=find_subscriber_no((bp->ri[2]|(bp->ri[3]<<8))-(NO_EXTENSION_A+NO_EXTENSION_B));
#endif
#else
#ifdef SYSTEM_SIDE
	    {
		unsigned int ext=bp->ri[2]|(bp->ri[3]<<8);
		if(ext >= NO_EXTENSION){
		    int i;
		    mes2prnti(mess[ERROR],0x920a);
		    int2prnt(ext);toprnq('-');byte2prnt(bp->s);toprnq(CR);
		    for(i=0;i<bp->s;i++){
			byte2prnt(bp->ri[i]);toprnq(CR);
		    }
		    src=0;
		    goto cont2;
		}
	    }
#endif
	    src=find_subscriber_no(bp->ri[2]|(bp->ri[3]<<8));
#endif

cont2:	    bp->s+=SDNS_NBB;
	    bp->ri[RECORD_TYPE_POSITION]+=1;
	    bp->ri[RECORD_SIZE_POSITION]+=SDNS_NBB;
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    bp->ri[RECORD_TYPE_POSITION]+=4;
	    bp->ri[bp->s-6]=src/0x100000000LL;
#endif
	    bp->ri[bp->s-5]=src/0x1000000L;
	    bp->ri[bp->s-4]=src/0x10000L;
	    bp->ri[bp->s-3]=src/0x100L;
	    bp->ri[bp->s-2]=src;
	    break;
#ifdef CB_INSTALLED
	  case CB_TRUNK_RECORD:
	    src=(bp->ri[2]|(bp->ri[3]<<8))+first_cb_number;
	    goto cont2;
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	  case OPR_TRUNK_RECORD:
	    src=(bp->ri[2]|(bp->ri[3]<<8));
	    goto cont2;
#endif
	  case TRUNK_EXT_RECORD:
#ifdef RECORD_A_NUMBER
	    bp->ri[RECORD_TYPE_POSITION]+=1;
#endif
	    src=0; // dummy
	    goto cont1;
	  case TRUNK_TRUNK_RECORD:
#ifdef RECORD_A_NUMBER
	    bp->ri[RECORD_TYPE_POSITION]+=1;
#endif
	    src=0; // dummy
	    goto cont2;
	}
}
#endif
/* ======================================================================= */
/* ======================================================================= */
void calculate_checksum(unsigned int a){
	// bp->s-1 as the last byte is checksum
	int i,c=0;
	BYTE size;
	DBR *bp=&dbr[a];
	size=bp->s-1;
	for(i=0;i<size;i++){
		c+=bp->ri[i];
	}
	c = ~c;
	c &= 0xff;
	bp->ri[size]=c;
}
/* ======================================================================= */
/* ======================================================================= */
