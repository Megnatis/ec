/* ======================================================================= */
/* ======================================================================= */
void wmpr_isup(BYTE h,char *mlc_buf){
	// rmp_data[h][0] is the mlced
	// rmp_data[h][1] is the mlced_type
	// rmp_data[h][2...rmps[h]-1] is the mlcer
	// first cic, then point_code, then digits of A-number
	unsigned int i,j,mlced=rmp_data[h][0];
	unsigned short cic,point_code;
	char buf[20];
	char isupbuf[70];

	mlc_buf[0]=0;

	get_dt(&sdt);

	cic=rmp_data[h][2];
	point_code=rmp_data[h][3];
	for(i=0,j=4;j<rmps[h];i++,j++)
		buf[i]=rmp_data[h][j]+'0';
	buf[i]=0;

	sprintf(isupbuf,"From ISUP, DPC:%02d-%04d CIC:%04d A#: %s"
		 ,point_code/1024
		 ,point_code%1024
		 ,cic
		 ,buf
	       );
	if(mlced == 0xffff){ /* special case, mlced is unknown */
	    sprintf(mlc_buf,"%s is Malicious for Someone.",isupbuf);
	}
	else{
	    char tmpbuf[50];
#ifdef CAS3_INSTALLED
	    {
		BYTE mlced_type=rmp_data[h][1];
		if(mlced_type == CAS3_T)    // mlced is cas3
		    sprintf(tmpbuf,"Digital Trunk Channel no. %d",mlced+1);
		else
		    sprintf(tmpbuf,/*"%5.4ld"*/SDNS_FS,find_subscriber_no(mlced));
	    }
#else
	    sprintf(tmpbuf,/*"%5.4ld"*/SDNS_FS,find_subscriber_no(mlced));
#endif
	    sprintf(mlc_buf,"%s is Malicious for %s.",isupbuf,tmpbuf);
	}
}
/* ======================================================================= */
/* ======================================================================= */
#ifdef SIP_INSTALLED
void wmpr_sip(BYTE h,char *mlc_buf){
	// rmp_data[h][0] is the mlced
	// rmp_data[h][1] is the mlced_type
	// rmp_data[h][2...rmps[h]-1] is the mlcer
	// digits of A-number
	unsigned int i,j,mlced=rmp_data[h][0];
	char buf[20];
	char sipbuf[70];

	mlc_buf[0]=0;

	get_dt(&sdt);

	for(i=0,j=2;j<rmps[h];i++,j++)
		buf[i]=rmp_data[h][j]+'0';
	buf[i]=0;

	sprintf(sipbuf,"From A#: %s"
		 ,buf
	       );
	if(mlced == 0xffff){ /* special case, mlced is unknown */
	    sprintf(mlc_buf,"%s is Malicious for Someone.",sipbuf);
	}
	else{
	    char tmpbuf[50];
#ifdef CAS3_INSTALLED
	    {
		BYTE mlced_type=rmp_data[h][1];
		if(mlced_type == CAS3_T)    // mlced is cas3
		    sprintf(tmpbuf,"Digital Trunk Channel no. %d",mlced+1);
		else
		    sprintf(tmpbuf,/*"%5.4ld"*/SDNS_FS,find_subscriber_no(mlced));
	    }
#else
	    sprintf(tmpbuf,/*"%5.4ld"*/SDNS_FS,find_subscriber_no(mlced));
#endif
	    sprintf(mlc_buf,"%s is Malicious for %s.",sipbuf,tmpbuf);
	}
}
#endif
/* ======================================================================= */
/* ======================================================================= */
void wmpr(BYTE h,char *mlc_buf){
	unsigned short real_mlced;
	unsigned short mlced,mlcer;
	BYTE mlcer_type;

	mlced=rmp_data[h][0];
	mlcer=rmp_data[h][1];
	mlcer_type=rmp_data[h][2]&0xff;

	mlc_buf[0]=0;

	get_dt(&sdt);

	if(mlced == 0xffff){  /* special case, mlced is unknown */
#ifdef CB_INSTALLED
		if(mlcer_type == CB_T){
		    sprintf(mlc_buf,"Coin Box no. %d is Malicious for Someone.",mlcer+1);
		}
		else
#endif
#ifdef EM_INSTALLED
		if(mlcer_type == EM_T){
		    sprintf(mlc_buf,"E&M Channel no. %d is Malicious for Someone.",mlcer+1);
		}
		else
#endif
#ifdef CAS1_INSTALLED
		if(mlcer_type == CAS1_T){
		    sprintf(mlc_buf,"Digital Trunk Channel no. %d is Malicious for Someone.",mlcer+1);
		}
		else
#endif
#ifdef CAS3_INSTALLED
		if(mlcer_type == CAS3_T){
		    sprintf(mlc_buf,"Digital Trunk Channel no. %d is Malicious for Someone.",mlcer+1);
		}
		else
#endif
#ifdef ISUP
		if(mlcer_type == ISUP_T){
		    sprintf(mlc_buf,"ISUP Channel no. %d is Malicious for Someone.",mlcer+1);
		}
		else
#endif
#ifdef SIP_INSTALLED
		if(mlcer_type == SIP_T){
		    sprintf(mlc_buf,"SIP Channel no. %d is Malicious for Someone.",mlcer+1);
		}
		else
#endif
		{
#ifdef LIM
#if(NO_EXTENSION_C != 0)
		    if(h == 2)
			mlcer += (NO_EXTENSION_A+NO_EXTENSION_B);
		    else
#endif
		    if(h == 1)
			mlcer += NO_EXTENSION_A;
#endif
		    sprintf(mlc_buf,/*"%5.4ld"*/SDNS_FS" is Malicious for Someone.",find_subscriber_no(mlcer));
		}
	}
	else{
#ifdef CAS3_INSTALLED
	    BYTE mlced_type=rmp_data[h][2]>>8;
	    if(mlced_type == CAS3_T){	 // mlced is cas3
		if(mlcer == 0xffff){		// mlcer is unknown
		    sprintf(mlc_buf,"Someone is Malicious for Digital Trunk Channel no. %d.",mlced+1);
		}
		else
#ifdef CB_INSTALLED
		if(mlcer_type == CB_T){
		    sprintf(mlc_buf,"Coin Box no. %d is Malicious for Digital Trunk Channel no. %d.",mlcer+1,mlced+1);
		}
		else
#endif
#ifdef EM_INSTALLED
		if(mlcer_type == EM_T){
		    sprintf(mlc_buf,"E&M Channel no. %d is Malicious for Digital Trunk Channel no. %d.",mlcer+1,mlced+1);
		}
		else
#endif
#ifdef CAS1_INSTALLED
		if(mlcer_type == CAS1_T){
		    sprintf(mlc_buf,"Digital Trunk Channel no. %d is Malicious for Digital Trunk Channel no. %d.",mlcer+1,mlced+1);
		}
		else
#endif
#ifdef CAS3_INSTALLED
		if(mlcer_type == CAS3_T){
		    sprintf(mlc_buf,"Digital Trunk Channel no. %d is Malicious for Digital Trunk Channel no. %d.",mlcer+1,mlced+1);
		}
		else
#endif
#ifdef ISUP
		if(mlcer_type == ISUP_T){
		    sprintf(mlc_buf,"ISUP Channel no. %d is Malicious for Digital Trunk Channel no. %d.",mlcer+1,mlced+1);
		}
		else
#endif
#ifdef SIP_INSTALLED
		if(mlcer_type == SIP_T){
		    sprintf(mlc_buf,"SIP Channel no. %d is Malicious for Digital Trunk Channel no. %d.",mlcer+1,mlced+1);
		}
		else
#endif
		{
#ifdef LIM
#if(NO_EXTENSION_C != 0)
		    if(h == 2)
			mlcer += (NO_EXTENSION_A+NO_EXTENSION_B);
		    else
#endif
		    if(h == 1)
			mlcer += NO_EXTENSION_A;
#endif
		    sprintf(mlc_buf,/*"%5.4ld"*/SDNS_FS" is Malicious for Digital Trunk Channel no. %d.",find_subscriber_no(mlcer),mlced+1);
		}
	    }
	    else
#endif	// CAS3_INSTALLED
	    {
#ifdef LIM
#if(NO_EXTENSION_C != 0)
		if(h == 2)
		    real_mlced=mlced+(NO_EXTENSION_A+NO_EXTENSION_B);
		else
#endif
		if(h == 1)
		    real_mlced=mlced+NO_EXTENSION_A;
		else
#endif
		    real_mlced=mlced;

		if(mlcer == 0xffff){  /* special case, mlcer is unknown */
		    sprintf(mlc_buf,"Someone is Malicious for "/*"%5.4ld"*/SDNS_FS".",find_subscriber_no(real_mlced));
		}
		else{
#ifdef CB_INSTALLED
		    if(mlcer_type == CB_T){
			sprintf(mlc_buf,"Coin Box no. %d is Malicious for "/*"%5.4ld"*/SDNS_FS".",((mlcer & 0x3fff)+1),find_subscriber_no(real_mlced));
		    }
		    else
#endif
#ifdef EM_INSTALLED
		    if(mlcer_type == EM_T){
			sprintf(mlc_buf,"E&M Channel no. %d is Malicious for "/*"%5.4ld"*/SDNS_FS".",mlcer+1,find_subscriber_no(real_mlced));
		    }
		    else
#endif
#ifdef CAS1_INSTALLED
		    if(mlcer_type == CAS1_T){
			sprintf(mlc_buf,"Digital Trunk Channel no. %d is Malicious for "/*"%5.4ld"*/SDNS_FS".",mlcer+1,find_subscriber_no(real_mlced));
		    }
		    else
#endif
#ifdef CAS3_INSTALLED
		    if(mlcer_type == CAS3_T){
			sprintf(mlc_buf,"Digital Trunk Channel no. %d is Malicious for "/*"%5.4ld"*/SDNS_FS".",mlcer+1,find_subscriber_no(real_mlced));
		    }
		    else
#endif
#ifdef ISUP
		    if(mlcer_type == ISUP_T){
			sprintf(mlc_buf,"ISUP Channel no. %d is Malicious for "/*"%5.4ld"*/SDNS_FS".",mlcer+1,find_subscriber_no(real_mlced));
		    }
		    else
#endif
#ifdef SIP_INSTALLED
		    if(mlcer_type == SIP_T){
			sprintf(mlc_buf,"SIP Channel no. %d is Malicious for "/*"%5.4ld"*/SDNS_FS".",mlcer+1,find_subscriber_no(real_mlced));
		    }
		    else
#endif
		    {
#ifdef LIM
#if(NO_EXTENSION_C != 0)
			if(h == 2)
			    mlcer += (NO_EXTENSION_A+NO_EXTENSION_B);
			else
#endif
			if(h == 1)
			    mlcer += NO_EXTENSION_A;
#endif
			sprintf(mlc_buf,/*"%5.4ld"*/SDNS_FS" is Malicious for "/*"%5.4ld"*/SDNS_FS".",find_subscriber_no(mlcer),find_subscriber_no(real_mlced));
		    }
		}
	    }
	}
}
/* ======================================================================= */
/* ======================================================================= */
void find_real_mlced(BYTE h,char *mlced_buf){
	unsigned short mlced;
	mlced=rmp_data[h][0];
#ifdef CAS3_INSTALLED
	BYTE mlced_type=rmp_data[h][2]>>8;
#endif
	mlced_buf[0]=0;
	if((mlced != 0xffff)
#ifdef CAS3_INSTALLED
	   && (mlced_type != CAS3_T)
#endif
	){
	    unsigned int real_mlced;
#ifdef LIM
#if(NO_EXTENSION_C != 0)
	    if(h == 2)
		real_mlced=mlced+(NO_EXTENSION_A+NO_EXTENSION_B);
	    else
#endif
	    if(h == 1)
		real_mlced=mlced+NO_EXTENSION_A;
	    else
#endif
		real_mlced=mlced;
	    sprintf(mlced_buf,"%010"SDNS_BF,find_subscriber_no(real_mlced));
	}
}
/* ======================================================================= */
/* ======================================================================= */
