/* ======================================================================= */
/* ======================================================================= */
#if (((defined SYSTEM_SIDE) && (defined KIATEL_UDP_FOR_OAM)) || \
     ((defined OPERATOR_SIDE) && (defined DISPLAY_BILLING)))
void prepare_billing_info_for_display(unsigned int a,char *buffer){
	SDNST dst=0,src=0; //just to avoid warning
	BYTE call_hr,call_min,call_sec;
	BYTE dur_hr,dur_min,dur_sec;
	unsigned int cnt,duration;
	BYTE record_type,no_dgt;
	BYTE dgt,j;
	char dgt_string[37]; // one extra for terminating null
	BYTE ch,k;
	char used_for_conference;
	BYTE dst_type=0; //just to avoid warning
	BYTE src_type=0; //just to avoid warning
	DBR *bp=&dbr[a];
	BYTE count_location=bp->s-3;	// to avoid warning
	if (	(bp->ri[0] == EXT_EXT_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (EXT_EXT_RECORD+5))
#else
	     || (bp->ri[0] == (EXT_EXT_RECORD+1))
#endif
	     || (bp->ri[0] == TRUNK_EXT_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (TRUNK_EXT_RECORD+5))
#else
	     || (bp->ri[0] == (TRUNK_EXT_RECORD+1))
#endif
#ifdef RECORD_A_NUMBER
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (TRUNK_EXT_RECORD+6))
#else
	     || (bp->ri[0] == (TRUNK_EXT_RECORD+2))
#endif
#endif
#ifdef CB_INSTALLED
	     || (bp->ri[0] == CB_EXT_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (CB_EXT_RECORD+5))
#else
	     || (bp->ri[0] == (CB_EXT_RECORD+1))
#endif
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	     || (bp->ri[0] == OPR_EXT_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (OPR_EXT_RECORD+5))
#else
	     || (bp->ri[0] == (OPR_EXT_RECORD+1))
#endif
#endif
	){
	    if (bp->ri[0] == EXT_EXT_RECORD){
		    src=find_subscriber_no(bp->ri[2]|(bp->ri[3]<<8));
		    dst=find_subscriber_no(bp->ri[4]|(bp->ri[5]<<8));
		    count_location=bp->s-3;
	    }
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (EXT_EXT_RECORD+5)){
		    src=((SDNST)bp->ri[bp->s-11] << 32) |
			(bp->ri[bp->s-10] << 24) |
			(bp->ri[bp->s-9] << 16) |
			(bp->ri[bp->s-8] <<  8) |
			(bp->ri[bp->s-7]      );
		    dst=((SDNST)bp->ri[bp->s-6] << 32) |
			(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-13;
	    }
#else
	    else
	    if (bp->ri[0] == (EXT_EXT_RECORD+1)){
		    src=(bp->ri[bp->s-9] << 24) |
			(bp->ri[bp->s-8] << 16) |
			(bp->ri[bp->s-7] <<  8) |
			(bp->ri[bp->s-6]      );
		    dst=(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-11;
	    }
#endif
	    else
	    if (bp->ri[0] == TRUNK_EXT_RECORD){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    dst=find_subscriber_no(bp->ri[4]|(bp->ri[5]<<8));
		    count_location=bp->s-3;
	    }
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (TRUNK_EXT_RECORD+5)){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    dst=((SDNST)bp->ri[bp->s-6] << 32) |
			(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-13;
	    }
#else
	    else
	    if (bp->ri[0] == (TRUNK_EXT_RECORD+1)){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    dst=(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-11;
	    }
#endif
#ifdef RECORD_A_NUMBER
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (TRUNK_EXT_RECORD+6)){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    dst=((SDNST)bp->ri[bp->s-6] << 32) |
			(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-13;
	    }
#else
	    else
	    if (bp->ri[0] == (TRUNK_EXT_RECORD+2)){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    dst=(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-11;
	    }
#endif
#endif
#ifdef CB_INSTALLED
	    else
	    if (bp->ri[0] == CB_EXT_RECORD){
		    src=(bp->ri[2]|(bp->ri[3]<<8))+first_cb_number;
		    dst=find_subscriber_no(bp->ri[4]|(bp->ri[5]<<8));
		    count_location=bp->s-3;
	    }
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (CB_EXT_RECORD+5)){
		    src=((SDNST)bp->ri[bp->s-11] << 32) |
			(bp->ri[bp->s-10] << 24) |
			(bp->ri[bp->s-9] << 16) |
			(bp->ri[bp->s-8] <<  8) |
			(bp->ri[bp->s-7]      );
		    dst=((SDNST)bp->ri[bp->s-6] << 32) |
			(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-13;
	    }
#else
	    else
	    if (bp->ri[0] == (CB_EXT_RECORD+1)){
		    src=(bp->ri[bp->s-9] << 24) |
			(bp->ri[bp->s-8] << 16) |
			(bp->ri[bp->s-7] <<  8) |
			(bp->ri[bp->s-6]      );
		    dst=(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-11;
	    }
#endif
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	    else
	    if (bp->ri[0] == OPR_EXT_RECORD){
		    src=(bp->ri[2]|(bp->ri[3]<<8));
		    dst=find_subscriber_no(bp->ri[4]|(bp->ri[5]<<8));
		    count_location=bp->s-3;
	    }
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (OPR_EXT_RECORD+5)){
		    src=((SDNST)bp->ri[bp->s-11] << 32) |
			(bp->ri[bp->s-10] << 24) |
			(bp->ri[bp->s-9] << 16) |
			(bp->ri[bp->s-8] <<  8) |
			(bp->ri[bp->s-7]      );
		    dst=((SDNST)bp->ri[bp->s-6] << 32) |
			(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-13;
	    }
#else
	    else
	    if (bp->ri[0] == (OPR_EXT_RECORD+1)){
		    src=(bp->ri[bp->s-9] << 24) |
			(bp->ri[bp->s-8] << 16) |
			(bp->ri[bp->s-7] <<  8) |
			(bp->ri[bp->s-6]      );
		    dst=(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-11;
	    }
#endif
#endif
	    record_type=bp->ri[0];
	    call_hr=bp->ri[6];
	    call_min=bp->ri[7];
	    call_sec=bp->ri[8];
	    duration=bp->ri[9]|(bp->ri[10]<<8);
	    dur_hr=duration/3600;
	    dur_min=(duration%3600)/60;
	    dur_sec=(duration%60);

	    used_for_conference=' ';
#ifdef RECORD_A_NUMBER
	    BYTE start=11;
	    BYTE nd=bp->ri[start];
	    char a_number[21];
	    a_number[0]=0;
	    if(nd <= 20){
		BYTE i,no_byte;;
		no_byte=(nd+1)/2;
		j=0;
		for(i=0;i<no_byte;i++){
		    dgt=bp->ri[start+1+i] & 0x0f;
		    if(dgt == 0xa) dgt='0';
		    else if(dgt == 0xb) dgt='*';
		    else if(dgt == 0xc) dgt='#';
		    else if(dgt <= 0x9) dgt+='0';
		    else dgt=dgt-0xa+'a';
		    a_number[j++]=dgt;
		    dgt=(bp->ri[start+1+i] & 0xf0) >> 4;
		    if(dgt == 0xa) dgt='0';
		    else if(dgt == 0xb) dgt='*';
		    else if(dgt == 0xc) dgt='#';
		    else if(dgt <= 0x9) dgt+='0';
		    else dgt=dgt-0xa+'a';
		    a_number[j++]=dgt;
		}
		a_number[nd]=0;
	    }
#endif
#ifdef REPORT_BULK_IN_DETAIL
	    cnt=bp->ri[count_location  ]
	      |(bp->ri[count_location+1]<<8);
	    if(cnt & 0x8000){
		    cnt &= 0x7fff;
		    used_for_conference='*';
	    }
#else
	    cnt=0;
#endif
	    if(    (bp->ri[0] == TRUNK_EXT_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
		|| (bp->ri[0] == (TRUNK_EXT_RECORD+5))
#else
		|| (bp->ri[0] == (TRUNK_EXT_RECORD+1))
#endif
#ifdef RECORD_A_NUMBER
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
		|| (bp->ri[0] == (TRUNK_EXT_RECORD+6))
#else
		|| (bp->ri[0] == (TRUNK_EXT_RECORD+2))
#endif
#endif
	      )
	    {
#ifdef RECORD_A_NUMBER
	      if(a_number[0] != 0){
		sprintf(buffer,
		"%-20s %-5"SDNS_BF"%c   %-16"SDNS_BF"     %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		a_number,src,used_for_conference,dst,record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
	      }
	      else
#endif
	      {
		sprintf(buffer,
		"%-5"SDNS_BF"   %02d%c                   %-16"SDNS_BF"     %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		src,src_type,used_for_conference,dst,record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
	      }
	    }
	    else{
		sprintf(buffer,
		"%-10"SDNS_BF"%c                   %-16"SDNS_BF"     %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		src,used_for_conference,dst,record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
	    }
	}
	else
	if (	(bp->ri[0] == EXT_TRUNK_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (EXT_TRUNK_RECORD+5))
#else
	     || (bp->ri[0] == (EXT_TRUNK_RECORD+1))
#endif
	     || (bp->ri[0] == TRUNK_TRUNK_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (TRUNK_TRUNK_RECORD+5))
#else
	     || (bp->ri[0] == (TRUNK_TRUNK_RECORD+1))
#endif
#ifdef RECORD_A_NUMBER
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (TRUNK_TRUNK_RECORD+6))
#else
	     || (bp->ri[0] == (TRUNK_TRUNK_RECORD+2))
#endif
#endif
#ifdef CB_INSTALLED
	     || (bp->ri[0] == CB_TRUNK_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (CB_TRUNK_RECORD+5))
#else
	     || (bp->ri[0] == (CB_TRUNK_RECORD+1))
#endif
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	     || (bp->ri[0] == OPR_TRUNK_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	     || (bp->ri[0] == (OPR_TRUNK_RECORD+5))
#else
	     || (bp->ri[0] == (OPR_TRUNK_RECORD+1))
#endif
#endif
	){
	    if (bp->ri[0] == EXT_TRUNK_RECORD){
		    src=find_subscriber_no(bp->ri[2]|(bp->ri[3]<<8));
		    count_location=bp->s-3;
	    }
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (EXT_TRUNK_RECORD+5)){
		    src=((SDNST)bp->ri[bp->s-6] << 32) |
			(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-8;
	    }
#else
	    else
	    if (bp->ri[0] == (EXT_TRUNK_RECORD+1)){
		    src=(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-7;
	    }
#endif
	    else
	    if (bp->ri[0] == TRUNK_TRUNK_RECORD){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    count_location=bp->s-3;
	    }
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (TRUNK_TRUNK_RECORD+5)){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    count_location=bp->s-8;
	    }
#else
	    else
	    if (bp->ri[0] == (TRUNK_TRUNK_RECORD+1)){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    count_location=bp->s-7;
	    }
#endif
#ifdef RECORD_A_NUMBER
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (TRUNK_TRUNK_RECORD+6)){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    count_location=bp->s-8;
	    }
#else
	    else
	    if (bp->ri[0] == (TRUNK_TRUNK_RECORD+2)){
		    src=bp->ri[2]|(bp->ri[3]<<8);
		    src_type=src >> 12;
		    src=src & 0x0fff;
		    count_location=bp->s-7;
	    }
#endif
#endif
#ifdef CB_INSTALLED
	    else
	    if (bp->ri[0] == CB_TRUNK_RECORD){
		    src=(bp->ri[2]|(bp->ri[3]<<8))+first_cb_number;
		    count_location=bp->s-3;
	    }
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (CB_TRUNK_RECORD+5)){
		    src=((SDNST)bp->ri[bp->s-6] << 32) |
			(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-8;
	    }
#else
	    else
	    if (bp->ri[0] == (CB_TRUNK_RECORD+1)){
		    src=(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-7;
	    }
#endif
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
	    else
	    if (bp->ri[0] == OPR_TRUNK_RECORD){
		    src=(bp->ri[2]|(bp->ri[3]<<8));
		    count_location=bp->s-3;
	    }
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
	    else
	    if (bp->ri[0] == (OPR_TRUNK_RECORD+5)){
		    src=((SDNST)bp->ri[bp->s-6] << 32) |
			(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-8;
	    }
#else
	    else
	    if (bp->ri[0] == (OPR_TRUNK_RECORD+1)){
		    src=(bp->ri[bp->s-5] << 24) |
			(bp->ri[bp->s-4] << 16) |
			(bp->ri[bp->s-3] <<  8) |
			(bp->ri[bp->s-2]      );
		    count_location=bp->s-7;
	    }
#endif
#endif
	    record_type=bp->ri[0];
	    dst_type=bp->ri[4];
	    dst=bp->ri[5];
	    call_hr=bp->ri[6];
	    call_min=bp->ri[7];
	    call_sec=bp->ri[8];
	    duration=bp->ri[9]|(bp->ri[10]<<8);
	    dur_hr=duration/3600;
	    dur_min=(duration%3600)/60;
	    dur_sec=(duration%60);
	    no_dgt=bp->ri[11]/2;
	    k=0;
	    for(j=0;j<no_dgt;j++){
		dgt=bp->ri[12+j];
		ch=dgt & 0xf;
		if(ch>=0x0a)
		    ch=0;
		dgt_string[k++]=ch+'0';
		if(k == 37-1) break;
		ch=dgt >> 4;
		if(ch != 0x0f){
		    if(ch>=0x0a)
			ch=0;
		    dgt_string[k++]=ch+'0';
		    if(k == 37-1) break;
		}
	    }
	    dgt_string[k]=0;
	    used_for_conference=' ';
#ifdef RECORD_A_NUMBER
	    BYTE start=12+(bp->ri[11]/2);
	    BYTE nd=bp->ri[start];
	    char a_number[21];
	    a_number[0]=0;
	    if(nd <= 20){
		BYTE i,no_byte;;
		no_byte=(nd+1)/2;
		j=0;
		for(i=0;i<no_byte;i++){
		    dgt=bp->ri[start+1+i] & 0x0f;
		    if(dgt == 0xa) dgt='0';
		    else if(dgt == 0xb) dgt='*';
		    else if(dgt == 0xc) dgt='#';
		    else if(dgt <= 0x9) dgt+='0';
		    else dgt=dgt-0xa+'a';
		    a_number[j++]=dgt;
		    dgt=(bp->ri[start+1+i] & 0xf0) >> 4;
		    if(dgt == 0xa) dgt='0';
		    else if(dgt == 0xb) dgt='*';
		    else if(dgt == 0xc) dgt='#';
		    else if(dgt <= 0x9) dgt+='0';
		    else dgt=dgt-0xa+'a';
		    a_number[j++]=dgt;
		}
		a_number[nd]=0;
	    }
#endif
#ifdef REPORT_BULK_IN_DETAIL
	    cnt=bp->ri[count_location  ]
	      |(bp->ri[count_location+1]<<8);
	    if(cnt & 0x8000){
		cnt &= 0x7fff;
		used_for_conference='*';
	    }
#else
	    cnt=0;
#endif
	    if(    (bp->ri[0] == TRUNK_TRUNK_RECORD)
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
		|| (bp->ri[0] == (TRUNK_TRUNK_RECORD+5))
#else
		|| (bp->ri[0] == (TRUNK_TRUNK_RECORD+1))
#endif
#ifdef RECORD_A_NUMBER
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
		|| (bp->ri[0] == (TRUNK_TRUNK_RECORD+6))
#else
		|| (bp->ri[0] == (TRUNK_TRUNK_RECORD+2))
#endif
#endif
	      )
	    {
	      if(dst_type == OPR_T){
#ifdef RECORD_A_NUMBER
		if(a_number[0] != 0){
		  sprintf(buffer,
		    "%-20s%-5"SDNS_BF"%c    %-5"SDNS_BF" %02d             %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		    a_number,src,used_for_conference,dst, dst_type, record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
		}
		else
#endif
		{
		  sprintf(buffer,
		    "%-5"SDNS_BF"   %02d%c                   %5"SDNS_BF" %02d             %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		    src,src_type,used_for_conference,dst, dst_type, record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
		}
	      }
	      else{
#ifdef RECORD_A_NUMBER
		if(a_number[0] != 0){
		  sprintf(buffer,
		    "%-20s %5"SDNS_BF"%c   %-20s %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		    a_number,src,used_for_conference,dgt_string,/*dst,*/record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
		}
		else
#endif
		{
		  sprintf(buffer,
		    "%-5"SDNS_BF"   %02d%c                   %-20s %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		    src,src_type,used_for_conference,dgt_string,/*dst,*/record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
		}
	      }
	    }
	    else{
	      if(dst_type == OPR_T){
		sprintf(buffer,
		  "%-10"SDNS_BF"%c                   %-5"SDNS_BF" %02d             %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		  src,used_for_conference,dst, dst_type, record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
	      }
	      else{
		sprintf(buffer,
		  "%-10"SDNS_BF"%c                   %-20s %03d %02x:%02x:%02x %02d:%02d:%02d %05d",
		  src,used_for_conference,dgt_string,/*dst,*/record_type,call_hr,call_min,call_sec,dur_hr,dur_min,dur_sec,cnt);
	      }
	    }
	}
}
#endif
/* ======================================================================= */
/* ======================================================================= */
