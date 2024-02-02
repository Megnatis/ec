                        /****  detail billing  ****/

#include "omtfp.h"
#include "wg.h"

#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
extern BYTE rdbs[];
extern unsigned int adbrqf,adbrqe;
extern BYTE cd;
#ifdef BEHNAMIR
extern unsigned int db[MAX_NO_OPR_CARD][MAX_DETAIL_BILLING_INFO_SIZE+1];  /* detail billing */
#else
extern unsigned int db[MAX_NO_OPR_CARD][MAX_DETAIL_BILLING_INFO_SIZE];  /* detail billing */
#endif
#endif
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
extern DBR dbr[];
extern unsigned int fdbrqf,fdbrqe;
extern char in[];
extern FILE *fer;
extern DATTIM sdt;
extern DATTIM billing_sdt;
#endif
#endif

#ifdef DISPLAY_BILLING
extern BYTE bill_display_flag;
extern BYTE bill_line;
extern BYTE scroll_flag;
extern WINDOW *bill_w;
#endif

#ifdef CB_INSTALLED
extern unsigned long first_cb_number;
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )

#include        <fcntl.h>
#include        <io.h>
#if (OS == DOS)
#define KT_NORMAL_FILE_MODE     _A_NORMAL
#endif
#if (OS == LINUX_COMPATIBLE) || (OS == WINDOWS)
#define KT_NORMAL_FILE_MODE     S_IRUSR | S_IWUSR
#endif

#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
unsigned int kt_file_open(char *filename,int mode,int *handle){
#if (OS == DOS)
#ifdef LONG_BILLING_FILENAME
        if((*handle=_open(filename,mode)) != -1)
                return OK;
#else
        if(_dos_open(filename,mode,handle) == 0)
                return OK;
#endif
#endif
#if (OS == LINUX_COMPATIBLE) || (OS == WINDOWS)
        int ret_value;
        if((ret_value=open(filename,mode)) >= 0){
                *handle=ret_value;
                return OK;
        }
#endif
        return FAIL;
}
unsigned int kt_file_creat(char *filename,int mode,int *handle){
#if (OS == DOS)
#ifdef LONG_BILLING_FILENAME
        if((*handle=_creat(filename,mode)) != -1)
                return OK;
#else
        if(_dos_creat(filename,mode,handle) == 0)
                return OK;
#endif
#endif
#if (OS == LINUX_COMPATIBLE) || (OS == WINDOWS)
        int ret_value;
        if((ret_value=creat(filename,mode)) >= 0){
                *handle=ret_value;
                return OK;
        }
#endif
        return FAIL;
}
unsigned int kt_file_close(int handle){
#if (OS == DOS)
        if(_dos_close(handle) == 0)
                return OK;
#endif
#if (OS == LINUX_COMPATIBLE) || (OS == WINDOWS)
        if(close(handle) == 0)
                return OK;
#endif
        return FAIL;
}
unsigned int kt_file_read(int handle,void *buffer,unsigned int count ,unsigned int *result){
#if (OS == DOS)
        if(_dos_read(handle,buffer,count,result) == 0)
                return OK;
#endif
#if (OS == LINUX_COMPATIBLE) || (OS == WINDOWS)
        int ret_value;
        if((ret_value=read(handle,buffer,count)) >= 0){
                *result=ret_value;
                return OK;
        }
#endif
        return FAIL;
}
unsigned int kt_file_write(int handle,void *buffer,unsigned int count,unsigned int *result){
#if (OS == DOS)
        if(_dos_write(handle,buffer,count,result) == 0)
                return OK;
#endif
#if (OS == LINUX_COMPATIBLE) || (OS == WINDOWS)
        int ret_value;
        if((ret_value=write(handle,buffer,count)) >= 0){
                *result=ret_value;
                return OK;
        }
#endif
        return FAIL;
}
long kt_filelength(int handle){
        return filelength(handle);
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
unsigned int ffdbr(){  /* find free detail billing record */
        unsigned int a;
        a=fdbrqf;
        if(a >= NO_DETAIL_BILLING_RECORDS)  return (unsigned int)(-1);
        fdbrqf=dbr[a].rp;
        if(fdbrqf >= NO_DETAIL_BILLING_RECORDS)  /* if last record */
                fdbrqe=(unsigned int)(-1);
        else
                dbr[fdbrqf].lp=(unsigned int)(-1);
        return a;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
void rdbr(unsigned int a){  /* release detail billing record */
        dbr[a].rp=(unsigned int)(-1);
        dbr[a].lp=fdbrqe;
        if(fdbrqe < NO_DETAIL_BILLING_RECORDS)
                dbr[fdbrqe].rp=a;
        else
                fdbrqf=a;
        fdbrqe=a;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
void toadbq(unsigned int a){  /* to active detail billing queue. */
        dbr[a].rp=(unsigned int)(-1);
        if(adbrqe >= NO_DETAIL_BILLING_RECORDS)
                adbrqf=a;
        else
                dbr[adbrqe].rp=a;
        dbr[a].lp=adbrqe;
        adbrqe=a;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
unsigned int fromadbq(){  /* from active detail billing queue */
        unsigned int a;
        if(adbrqf >= NO_DETAIL_BILLING_RECORDS)
                return (unsigned int)(-1);
        a=adbrqf;
        adbrqf=dbr[a].rp;
        if(adbrqf >= NO_DETAIL_BILLING_RECORDS)
                adbrqe=(unsigned int)(-1);
        else
                dbr[adbrqf].lp=(unsigned int)(-1);
        return a;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
unsigned int cdbfn(void){  /* creat detail billing file name */
#ifndef SEND_BILLING_TIME_FROM_SYS_TO_OPR
        int handle;
        unsigned int cnt;
        if (kt_file_open(DETAIL_BILLING_NAME_FILE, O_RDWR, &handle) != OK) {
        // detail billing file name,file does not exist
                if (kt_file_creat(DETAIL_BILLING_NAME_FILE, KT_NORMAL_FILE_MODE, &handle) != OK)
                        return FAIL;
                if (kt_file_write(handle, FIRST_DETAIL_BILLING_FILE,BILLING_FILE_NAME_SIZE,&cnt) != OK);
                kt_file_close(handle);
                if (kt_file_creat(FIRST_DETAIL_BILLING_FILE, KT_NORMAL_FILE_MODE, &handle) != OK)
                        return FAIL;
                winr(handle);
                wdr(handle);
        }
        kt_file_close(handle);
#endif
        return OK;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
extern SITE_PARAMETERS site_data;
void wr_installation_name_record(int handle){  /* write installation name record */

        char install_name[255/*MAX_NAME_LENGTH*/];
        BYTE dat[255/*MAX_NAME_LENGTH+3*/];
        BYTE chksum,*dat_pt;
        char *e;
        unsigned int cnt;
        BYTE dat_size,b,i;

//      strcpy(install_name,INSTALLATION_NAME);
        strcpy(install_name,"OFC:");
        i=strlen(install_name);
        for(b=0;b<site_data.no_office_code_dgts;b++,i++){
            install_name[i]=site_data.office_code_dgts[b] < 10 ?
                     site_data.office_code_dgts[b] + '0' :
                     site_data.office_code_dgts[b] - 10 + 'a';
        }
        install_name[i]=0;
        i=strlen(install_name);
        sprintf(&install_name[i]," FDN:"SDNS_FS,site_data.directory_start[0]);

        dat_pt=&dat[0];
        *dat_pt=INSTALLATION_NAME_RECORD;
        chksum=*dat_pt;dat_pt++;

        dat_size=strlen(install_name);
        *dat_pt=dat_size;
        chksum+=*dat_pt;dat_pt++;

        for(e=install_name,b=dat_size;b;b--,e++){
                *dat_pt=*e;
                chksum += *e;dat_pt++;
        }
        *dat_pt=~chksum;

        /* seek to the end of the file */
        if (lseek(handle, 0L, SEEK_END) == -1L){
                return;
        }
        if (kt_file_write(handle, dat,(dat_size+3) ,&cnt) != OK){
                return;
        }
}
void winr(int handle){  /* write installation name record */
        wr_installation_name_record(handle);
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
void wr_date_record(int handle,BYTE yr,BYTE mo,BYTE da){  /* write date record */

        BYTE dat[DATE_RECORD_SIZE+3];
        BYTE chksum=0;
        BYTE *dat_pt;
        unsigned int cnt;

        dat_pt=&dat[0];

        *dat_pt=DATE_RECORD;      // indx=0
        chksum+=*dat_pt;dat_pt++;

        *dat_pt=DATE_RECORD_SIZE; // indx=1
        chksum+=*dat_pt;dat_pt++;

        *dat_pt=yr;               // indx=2
        chksum+=*dat_pt;dat_pt++;

        *dat_pt=mo;               // indx=3
        chksum+=*dat_pt;dat_pt++;

        *dat_pt=da;               // indx=4
        chksum+=*dat_pt;dat_pt++;

        *dat_pt=~chksum;          // indx=5

        /* seek to the end of the file */
        if (lseek(handle, 0L, SEEK_END) == -1L){
                return;
        }

        if (kt_file_write(handle, dat,(DATE_RECORD_SIZE+3) ,&cnt) != OK){
                return;
        }

}
void wdr(int handle){  /* write date record */
        sdt=*get_dt(&sdt);
        wr_date_record(handle,sdt.yr,sdt.mo,sdt.da);
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
void mdr(BYTE yr,BYTE mo,BYTE da){  /* make date record */
        DBR *bp;
        BYTE *b;
        BYTE chksum=0;
        unsigned int a=ffdbr();
        if(a == (unsigned int)(-1))
            return;
        bp=&dbr[a];
        bp->s=DATE_RECORD_SIZE + 3;  /* for record type,record size,record check sum */
        b=&bp->ri[0];
        *b=DATE_RECORD;      chksum+=*b;b++;
        *b=DATE_RECORD_SIZE; chksum+=*b;b++;
        *b=yr;               chksum+=*b;b++;
        *b=mo;               chksum+=*b;b++;
        *b=da;               chksum+=*b;b++; // *b now is the place to put checksum

//      toadbq(a);  /* put to active detail billing queue */
        *b=~chksum;
        write_billing_record_to_file(a);
        rdbr(a);
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
#include "/ec/work/opr/db_com.c"        // common files
#endif
#endif
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
#include "/ec/work/opr/db_com2.c"       // common files
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
#define DAY_START       8
#define DAY_END         13
#define MIDDAY_START    DAY_END
#define MIDDAY_END      21
#define NIGHT_START     MIDDAY_END
#define NIGHT_END       DAY_START
#define DAY_DURATION    (DAY_END-DAY_START)
#define MIDDAY_DURATION (MIDDAY_END-MIDDAY_START)
#define NIGHT_DURATION  (NIGHT_END-NIGHT_START+24)
#define day_num         1344    //      105   // 1.75
#define day_den         10      //      1
#define midday_num      1602    //      135   // 2.25
#define midday_den      10      //      1
#define night_num       192     //      189   // 3.15
#define night_den       1       //      1

unsigned int calculate_local_pulse(unsigned int a){
  DBR *bp=&dbr[a];
  BYTE call_hr=bp->ri[6];
  BYTE call_min=bp->ri[7];
  BYTE call_sec=bp->ri[8];
  unsigned long duration=bp->ri[9]|(bp->ri[10]<<8);
  unsigned int pulse = 0;
  unsigned long day_dur, midday_dur, night_dur;
  long end_time;
  ldiv_t dv;
  unsigned long call_time;

  call_time=((unsigned long) call_hr * (unsigned long) 3600 +
      (unsigned long) call_min * 60 + (unsigned long) call_sec);

  if ((call_hr >= DAY_START) && (call_hr < DAY_END))        // ¥ø¤
    if ((long) (call_time) - (long) (duration) < DAY_START * (unsigned long) 3600)
      {
        day_dur = call_time - (DAY_START * (unsigned long) 3600);
        night_dur = duration - day_dur;
        dv = ldiv (day_dur * day_den, (unsigned long) day_num);
        if (dv.rem)
          dv.quot++;
        pulse += (unsigned int) dv.quot;
        if (night_dur <= (NIGHT_DURATION) * (long) 3600)
          {
            dv = ldiv (night_dur * night_den, (unsigned long) night_num);
            if (dv.rem)
              dv.quot++;
            pulse += (unsigned int) dv.quot;
          }
        else
          {
            dv = ldiv (NIGHT_DURATION * (unsigned long) 3600 * night_den,
                    (unsigned long) night_num);
            if (dv.rem)
              dv.quot++;
            pulse += (unsigned int) dv.quot;
            dv = ldiv ((night_dur - (NIGHT_DURATION * 3600)) * midday_den,
                    (unsigned long) midday_num);
            if (dv.rem)
              dv.quot++;
            pulse += (unsigned int) dv.quot;
          }
      }
    else
      {
        dv = ldiv ((duration) * day_den, (unsigned long) day_num);
        if (dv.rem)
          dv.quot++;
        pulse += (unsigned int) dv.quot;
      }
  else if ((call_hr >= MIDDAY_START) && (call_hr < MIDDAY_END))     // ¥ø¤
    if ((long) (call_time) - (long) (duration) < MIDDAY_START * (unsigned long) 3600)
      {
        midday_dur = call_time - (MIDDAY_START * (unsigned long) 3600);
        day_dur = duration - midday_dur;
        dv = ldiv (midday_dur * midday_den, (unsigned long) midday_num);
        if (dv.rem)
          dv.quot++;
        pulse += (unsigned int) dv.quot;
        if (day_dur <= DAY_DURATION * (long) 3600)
          {
            dv = ldiv (day_dur * day_den, (unsigned long) day_num);
            if (dv.rem)
              dv.quot++;
            pulse += (unsigned int) dv.quot;
          }
        else
          {
            dv = ldiv (DAY_DURATION * (unsigned long) 3600 * night_den,
                    (unsigned long) night_num);
            if (dv.rem)
              dv.quot++;
            pulse += (unsigned int) dv.quot;
            dv = ldiv ((day_dur - (DAY_DURATION * 3600)) * night_den,
                    (unsigned long) night_num);
            if (dv.rem)
              dv.quot++;
            pulse += (unsigned int) dv.quot;
          }
      }
    else
      {
        dv = ldiv ((duration) * midday_den, (unsigned long) midday_num);
        if (dv.rem)
          dv.quot++;
        pulse += (unsigned int) dv.quot;
      }
  else
    {                           // ’‹ª
      if (call_hr < NIGHT_START)
        end_time = call_time + (24 * (unsigned long) 3600);
      else
        end_time = call_time;
      if (end_time - duration < (NIGHT_START * (long) 3600))
        {
          night_dur = end_time - (NIGHT_START * (long) 3600);
          midday_dur = duration - night_dur;
          dv = ldiv (night_dur * night_den, (unsigned long) night_num);
          if (dv.rem)
            dv.quot++;
          pulse += (unsigned int) dv.quot;
          if (midday_dur <= (MIDDAY_DURATION) * (long) 3600)
            {
              dv = ldiv (midday_dur * midday_den, (unsigned long) midday_num);
              if (dv.rem)
                dv.quot++;
              pulse += (unsigned int) dv.quot;
            }
          else
            {
              dv = ldiv (MIDDAY_DURATION * (unsigned long) 3600 * midday_den,
                      (unsigned long) midday_num);
              if (dv.rem)
                dv.quot++;
              pulse += (unsigned int) dv.quot;
              dv = ldiv ((midday_dur - (MIDDAY_DURATION * 3600)) * day_den,
                      (unsigned long) day_num);
              if (dv.rem)
                dv.quot++;
              pulse += (unsigned int) dv.quot;
            }
        }
      else
        {
          dv = ldiv ((duration) * night_den,(unsigned long) night_num);
          if (dv.rem)
            dv.quot++;
          pulse += (unsigned int) dv.quot;

        }
    }

  return pulse;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
void adjust_local_pulse(unsigned int a){
        unsigned int cnt;
        DBR *bp=&dbr[a];
        BYTE count_location;
        if (    (bp->ri[0] == EXT_EXT_RECORD)
             || (bp->ri[0] == TRUNK_EXT_RECORD)
#ifdef CB_INSTALLED
             || (bp->ri[0] == CB_EXT_RECORD)
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
             || (bp->ri[0] == OPR_EXT_RECORD)
#endif
        ){
            cnt=calculate_local_pulse(a);
            count_location=bp->s-3;
            bp->ri[count_location]=cnt%256;
            bp->ri[count_location+1]=(cnt/256) |
                     (bp->ri[count_location+1] & 0x80);     // used_for_conference
        }
        else
        if (    (bp->ri[0] == (EXT_EXT_RECORD+1))
             || (bp->ri[0] == (TRUNK_EXT_RECORD+1))
#ifdef RECORD_A_NUMBER
             || (bp->ri[0] == (TRUNK_EXT_RECORD+2))
#endif
#ifdef CB_INSTALLED
             || (bp->ri[0] == (CB_EXT_RECORD+1))
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
             || (bp->ri[0] == (OPR_EXT_RECORD+1))
#endif
        ){
            cnt=calculate_local_pulse(a);
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
            count_location=bp->s-13;
#else
            count_location=bp->s-11;
#endif
            bp->ri[count_location]=cnt%256;
            bp->ri[count_location+1]=(cnt/256) |
                     (bp->ri[count_location+1] & 0x80);     // used_for_conference
        }
#ifdef LIM
        else
        if(bp->ri[4] == LIM_T){    // dst_type
            if (    (bp->ri[0] == EXT_TRUNK_RECORD)
                 || (bp->ri[0] == TRUNK_TRUNK_RECORD)
#ifdef CB_INSTALLED
                 || (bp->ri[0] == CB_TRUNK_RECORD)
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
                 || (bp->ri[0] == OPR_TRUNK_RECORD)
#endif
            ){
                cnt=calculate_local_pulse(a);
                count_location=bp->s-3;
                bp->ri[count_location]=cnt%256;
                bp->ri[count_location+1]=(cnt/256) |
                         (bp->ri[count_location+1] & 0x80);     // used_for_conference
            }
            else
            if (    (bp->ri[0] == (EXT_TRUNK_RECORD+1))
                 || (bp->ri[0] == (TRUNK_TRUNK_RECORD+1))
#ifdef RECORD_A_NUMBER
                 || (bp->ri[0] == (TRUNK_TRUNK_RECORD+2))
#endif
#ifdef CB_INSTALLED
                 || (bp->ri[0] == (CB_TRUNK_RECORD+1))
#endif
#ifdef NEW_DETAIL_BILLING_FOR_OPERATOR
                 || (bp->ri[0] == (OPR_TRUNK_RECORD+1))
#endif
            ){
                cnt=calculate_local_pulse(a);
#ifdef SUBSCRIBER_DIRECTORY_NUMBER_IS_LONG
                count_location=bp->s-8;
#else
                count_location=bp->s-7;
#endif
                bp->ri[count_location]=cnt%256;
                bp->ri[count_location+1]=(cnt/256) |
                         (bp->ri[count_location+1] & 0x80);     // used_for_conference
            }
        }
#endif
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
void adjust_billing_time(unsigned int a){
        DBR *bp=&dbr[a];
        sdt=*get_dt(&sdt);
        bp->ri[6]=sdt.hr;
        bp->ri[7]=sdt.mi;
        bp->ri[8]=sdt.se;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
void write_billing_record_to_file(unsigned int a){
        unsigned int cnt;
        DBR *bp;
//      FILE *b;
//      b=cdbrf();
//      if(b == NULL){
        int handle;BYTE ret_value;
        ret_value=cdbrf(&handle);
        if(ret_value != OK){
        /* if detail billing record file can not be accessed */
        /* report will be lost or output to the err.dbg file */
#ifdef OPERATOR_PC
                int c,d;
                fprintf(fer,"DB RCRD FILE CAN NOT BE ACCESSED:\n");
                for(d=0,c=dbr[a].s,bp=&dbr[a];d < c;d++)
                        fprintf(fer,"%x ",bp->ri[d]);
                fprintf(fer,"\n");
#endif
                return;
        }
        bp=&dbr[a];
//      fseek(b,0,SEEK_END);
//      fwrite(bp->ri,bp->s,1,b);
//      fclose(b);
        lseek(handle,0L,SEEK_END);
        if (kt_file_write(handle, bp->ri,bp->s ,&cnt) != OK);
        kt_file_close(handle);

#ifdef BILLING_BACKUP
        setdisk(3);
        if(chdir("d:\\opr") == 0){
//              b=cdbrf();
//              if(b == NULL){
                ret_value=cdbrf(&handle);
                if(ret_value != OK){
                        setdisk(2);
                        return;
                }
//              fseek(b,0,SEEK_END);
//              fwrite(bp->ri,bp->s,1,b);
//              fclose(b);
                lseek(handle,0L,SEEK_END);
                if (kt_file_write(handle, bp->ri,bp->s ,&cnt) != OK);
                kt_file_close(handle);
        }
        setdisk(2);
#endif
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
#ifdef DISPLAY_BILLING
void display_billing(unsigned int a){
        char buffer[81];
#ifndef PRINT_BILLING
        if (bill_display_flag == SET)
#endif
        {

            prepare_billing_info_for_display(a,buffer);
#ifdef PRINT_BILLING
            ws2p(buffer);
            toprnq(CR);toprnq(LF);
            toprnq(CR);toprnq(LF);
            toprnq(CR);toprnq(LF);
            toprnq(CR);toprnq(LF);
            if (bill_display_flag == SET)
#endif
            {
#if (OS == DOS) || (OS == LINUX_COMPATIBLE)
                if (bill_line == 18){
                        scroll_flag=SET;
                }
                else{
                        bill_line++;
                }
                if (scroll_flag)
                        scroll(bill_w,UP);
                wprompt(bill_w,0,bill_line,buffer);
#endif
            }
        }
}
#endif
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
void dbrc(){  /* detail billing records collection */
        unsigned int a;
        a=fromadbq();
        if(a == (unsigned int)(-1))  /* if active detail billing queue empty */
                return;
#ifdef STORE_SUBSCRIBER_DIRECTORY_NUMBER_IN_BILLING_FILE
        store_subscriber_directory_number(a);
#endif
#ifdef CPU_EC512
        adjust_local_pulse(a);
#endif
        calculate_checksum(a);
#ifdef DISPLAY_BILLING
        display_billing(a);
#endif
        write_billing_record_to_file(a);
        rdbr(a);
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
BYTE cdbrf(int *handle){  /* current detail billing record file */
        int handle1,handle2;unsigned int cnt;
        char fn[BILLING_FILE_NAME_SIZE + 1];

        if (kt_file_open(DETAIL_BILLING_NAME_FILE, O_RDWR, &handle1) != OK) {
                /* file name does not exist */
                if (kt_file_creat(DETAIL_BILLING_NAME_FILE, KT_NORMAL_FILE_MODE, &handle1) != OK)
                        return FAIL;
                if (kt_file_write(handle1, FIRST_DETAIL_BILLING_FILE,BILLING_FILE_NAME_SIZE,&cnt) != OK);
                kt_file_close(handle1);
                if (kt_file_creat(FIRST_DETAIL_BILLING_FILE, KT_NORMAL_FILE_MODE, &handle1) != OK)
                        return FAIL;
                winr(handle1);  /* write installation name record */
#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
                wr_date_record(handle1,billing_sdt.yr,billing_sdt.mo,billing_sdt.da);  /* write date record */
#else
                wdr(handle1);  /* write date record */
#endif
                *handle=handle1;
                return OK;
        }
        lseek(handle1,-(BILLING_FILE_NAME_SIZE),SEEK_END);
        if (kt_file_read(handle1, fn, BILLING_FILE_NAME_SIZE, &cnt) != OK);
        fn[BILLING_FILE_NAME_SIZE]='\0';
        if (kt_file_open(fn, O_RDWR, &handle2) != OK) {
                /* if detail billing record file does not exist */
                if (kt_file_creat(fn, KT_NORMAL_FILE_MODE, &handle2) == OK){
                        winr(handle2);  /* write installation name record */
#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
                        wr_date_record(handle2,billing_sdt.yr,billing_sdt.mo,billing_sdt.da);  /* write date record */
#else
                        wdr(handle2);  /* write date record */
#endif
                        kt_file_close(handle2);
                }
                kt_file_close(handle1);
                *handle=handle2;
                return OK;
        }
        lseek(handle2,0,SEEK_END);
#ifdef CLOSE_DBRF_ON_SIZE
        if(kt_filelength(handle2) >= (0xffff - MAX_DETAIL_BILLING_RECORD) ){
        /* billing file can not hold another record */
                char *c;
                char dbfbs[34]; /* detail billing file backup string */
                kt_file_close(handle2);

                strcpy(dbfbs,"copy ");
                strcat(dbfbs,fn);
                strcat(dbfbs," ");
                strcat(dbfbs,fn);
                strcat(dbfbs,".bak");
//              strcat(dbfbs," > NUL:");
                kt_system(dbfbs);

                c=&fn[FILE_NUMBER_IN_FILE_NAME];
                sscanf(c,"%d",(unsigned int *)&b);
                b++;
                sprintf(c,"%"BILLING_FILE_NUMBER_SIZE_STRING"."BILLING_FILE_NUMBER_SIZE_STRING"d",(unsigned int)b);
                lseek(handle1,0,SEEK_END);
                if (kt_file_write(handle1, fn,strlen(fn),&cnt) != OK);
                kt_file_close(handle1);
                if (kt_file_creat(fn, KT_NORMAL_FILE_MODE, &handle1) != OK)
                        return FAIL;
                winr(handle1);  /* write installation name record */
#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
                wr_date_record(handle1,billing_sdt.yr,billing_sdt.mo,billing_sdt.da);  /* write date record */
                // better not use CLOSE_DBRF_ON_SIZE with SEND_BILLING_TIME_FROM_SYS_TO_OPR
#else
                wdr(handle1);  /* write date record */
#endif
                *handle=handle1;
                return OK;
        }
#endif
        kt_file_close(handle1);
        *handle=handle2;
        return OK;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
BYTE create_new_dbrf_file(BYTE yr,BYTE mo,BYTE da){
        int handle1,handle2;unsigned int cnt;
        long b;
        unsigned int b1;
        char fn[BILLING_FILE_NAME_SIZE + 1];
        char *c;
        char dbfbs[34+2+2]; /* detail billing file backup string */
        if (kt_file_open(DETAIL_BILLING_NAME_FILE, O_RDWR, &handle1) != OK) {
                /* file name does not exist */
                if (kt_file_creat(DETAIL_BILLING_NAME_FILE, KT_NORMAL_FILE_MODE, &handle1) != OK)
                        return FAIL;
                if (kt_file_write(handle1, FIRST_DETAIL_BILLING_FILE,BILLING_FILE_NAME_SIZE,&cnt) != OK);
                kt_file_close(handle1);
                if (kt_file_creat(FIRST_DETAIL_BILLING_FILE, KT_NORMAL_FILE_MODE, &handle1) != OK)
                        return FAIL;
                winr(handle1);  /* write installation name record */
                wr_date_record(handle1,yr,mo,da);  /* write date record */
                kt_file_close(handle1);
                return OK;
        }
        lseek(handle1,-(BILLING_FILE_NAME_SIZE),SEEK_END);
        if (kt_file_read(handle1, fn, BILLING_FILE_NAME_SIZE, &cnt) != OK);
        fn[BILLING_FILE_NAME_SIZE]='\0';
        if (kt_file_open(fn, O_RDWR, &handle2) != OK) {
                /* if detail billing record file does not exist */
                if (kt_file_creat(fn, KT_NORMAL_FILE_MODE, &handle2) == OK){
                        winr(handle2);  /* write installation name record */
                        wr_date_record(handle2,yr,mo,da);  /* write date record */
                        kt_file_close(handle2);
                }
                kt_file_close(handle1);
                return OK;
        }
        kt_file_close(handle2);

        strcpy(dbfbs,"copy ");
        strcat(dbfbs,fn);
        strcat(dbfbs," ");
        strcat(dbfbs,fn);
        strcat(dbfbs,".bak");
//      strcat(dbfbs," > NUL:");
        kt_system(dbfbs);

        c=&fn[FILE_NUMBER_IN_FILE_NAME];
        sscanf(c,"%d",&b1);b=b1;
        b++;
        sprintf(c,"%"BILLING_FILE_NUMBER_SIZE_STRING"."BILLING_FILE_NUMBER_SIZE_STRING"d",(unsigned int)b);
        lseek(handle1,0,SEEK_END);
        if (kt_file_write(handle1, fn,strlen(fn),&cnt) != OK);
        kt_file_close(handle1);
        if (kt_file_creat(fn, KT_NORMAL_FILE_MODE, &handle1) != OK)
                return FAIL;
        winr(handle1);  /* write installation name record */
        wr_date_record(handle1,yr,mo,da);  /* write date record */
        kt_file_close(handle1);
        return OK;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if( (TON == DETAIL_BILLING_OPR) || ( (defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR) ) )
void ud(){  /* update date */
        sdt=*get_dt(&sdt);
        if(sdt.da == 0xff)
                return;
        if(sdt.da == 0x00)
                return;
        /* because of an unkown reason get_dt() sometimes return ff for day */
        if(cd != sdt.da){
#ifndef SEND_BILLING_TIME_FROM_SYS_TO_OPR
#ifdef CLOSE_DBRF_ON_DAY
                sdt=*get_dt(&sdt);
                create_new_dbrf_file(sdt.yr,sdt.mo,sdt.da);
#else  // CLOSE_DBRF_ON_SIZE
                sdt=*get_dt(&sdt);
                mdr(sdt.yr,sdt.mo,sdt.da);
#endif
#endif
                cd=sdt.da;
        }
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef DETAIL_BILLING
#if ((defined MY_UDP_FOR_OAM) ||    \
     (TON == DETAIL_BILLING_OPR) || \
     ((defined SECOND_DETAIL_BILLING_OPR) && (TON == SECOND_DETAIL_BILLING_OPR)) \
    )
#ifdef SEND_BILLING_TIME_FROM_SYS_TO_OPR
#define decimal2bcd(i) ((((i)/10) << 4) | ((i)%10))
void write_date_record(unsigned short packed_date){
        BYTE yr,mo,da;
        yr=decimal2bcd((packed_date & 0x7e00) >> 9);
        mo=decimal2bcd((packed_date & 0x01e0) >> 5);
        da=decimal2bcd(packed_date & 0x001f);
        mdr(yr,mo,da);
        billing_sdt.yr=yr;
        billing_sdt.mo=mo;
        billing_sdt.da=da;
}
void create_new_dbrf_file_with_sys_time(unsigned short packed_date){
        BYTE yr,mo,da;
        yr=decimal2bcd((packed_date & 0x7e00) >> 9);
        mo=decimal2bcd((packed_date & 0x01e0) >> 5);
        da=decimal2bcd(packed_date & 0x001f);
        if(create_new_dbrf_file(yr,mo,da) == FAIL){
        }
#ifdef BILLING_BACKUP
        setdisk(3);
        if(chdir("d:\\opr") == 0){
                if(create_new_dbrf_file(yr,mo,da) == FAIL){
                }
        }
        setdisk(2);
#endif
}
#endif
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */

// bulk billing handling block

#include "/ec/work/opr/db_bbf.c"

/* ======================================================================= */
/* ======================================================================= */

// malicious file handling block

#include "/ec/work/opr/db_mpf.c"

/* ======================================================================= */
/* ======================================================================= */

// printer handling block

#include "/ec/work/opr/db_prn.c"

/* ======================================================================= */
/* ======================================================================= */
