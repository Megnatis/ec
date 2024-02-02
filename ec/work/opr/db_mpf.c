
#ifdef MALICIOUS_PRINT_ON_U
#if ((defined MY_UDP_FOR_OAM) || (TON == MALICIOUS_PRINT_OPR))
#ifdef ALARM_ENABLE
#if (TON == ALARM_OPR)
extern FILE *flog;
#endif
#endif
#ifdef NEW_DEP
extern FILE *malicious_fp;
#endif
#endif
#if (TON == MALICIOUS_PRINT_OPR)
extern unsigned int mpxst[];     /* malicious print extension number(maliciouser) */
extern BYTE rmps[];  /* receive malicious print size in int */
extern unsigned int rmp_data[][MAX_MALICIOUS_RECORD_SIZE];
extern DATTIM sdt;
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef MALICIOUS_PRINT_ON_U
#if (TON == MALICIOUS_PRINT_OPR)
#include "/ec/work/opr/dbmpfcom.c"        // common files
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef MALICIOUS_PRINT_ON_U
#if ((defined MY_UDP_FOR_OAM) || (TON == MALICIOUS_PRINT_OPR))
extern char exchange_name[];
extern char exchange_office_code[];
extern char exchange_area_code[];
void handle_malicious_info(char *mlced_buf,char *mlc_buf){
        if(mlced_buf[0] != 0){
            char filename[50];
            FILE *fp_mlced;
            int i;
            if((i=strlen(mlced_buf)) <= 8)
                sprintf(filename,"%s.mlc",mlced_buf);
            else
                sprintf(filename,"%s.mlc",&mlced_buf[i-8]);
//          fp_mlced=fopen(filename,"a+b");     // read/write/create
            fp_mlced=fopen(filename,"r+b");     // read and write, not create
            if(fp_mlced == NULL){
                fp_mlced=fopen(filename,"wb");  // create
                fprintf(fp_mlced,"\r\n%s %s %s",
                               exchange_name,
                               exchange_office_code,
                               exchange_area_code);
            }
            else
                fseek(fp_mlced,0L,SEEK_END);
            fprintf(fp_mlced,"\r\n%s",mlc_buf);
            fclose(fp_mlced);
        }
        {
            FILE *a;
#if (COMPILER == BCC)
#ifdef NEW_DEP
            a=_fsopen(MALICIOUS_FILE,"r+t",0);  // read and write, 0 is SH_COMPAT
#else
            a=fopen(MALICIOUS_FILE,"r+t");      /* read and write */
#endif
#endif
#if (COMPILER == GCC)
            a=fopen(MALICIOUS_FILE,"r+t");      // read and write
#endif
            fseek(a,0L,SEEK_END);
            fprintf(a,"\n%s",mlc_buf);
            fclose(a);
        }
#ifdef DISPLAY_ALARM
        show_alarm_without_date(mlc_buf);
#endif
#if ((defined ALARM_ENABLE) && (TON == ALARM_OPR))
        alarm();
        {
#ifdef NEW_DEP
            fpos_t pos;
            fgetpos(flog,&pos);
            fseek(flog,0,SEEK_END);
#endif
            fprintf(flog,"Malicious Report. For details refer to Malicious file.");
#ifdef NEW_DEP
            fsetpos(flog,&pos);
#endif
        }
#endif
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef MALICIOUS_PRINT_ON_U
#if ((defined MY_UDP_FOR_OAM) || (TON == MALICIOUS_PRINT_OPR))
unsigned int cmpf(void){  /* creat malicious print file */
        FILE *nf;
        if( (nf=fopen(MALICIOUS_FILE,"rt")) == NULL){        /* read only */
        /* malicious file (or its directory) does not exist */
                if( (nf=fopen(MALICIOUS_FILE,"wt")) == NULL){  /* creat */
                        /* file can not be created, may be directory does not exist */
#if COMPILER == GCC
                        if(mkdir(MALICIOUS_DIRECTORY,S_IWUSR) != 0){
#else
                        if(mkdir(MALICIOUS_DIRECTORY) != 0){
#endif
                                /* directory can not be created */
#ifdef NEW_DEP
                                malicious_fp=NULL;
#endif
                                return FAIL;
                        }
                        else{   /* directory created, try again for creating file */
                                if( (nf=fopen(MALICIOUS_FILE,"wt")) == NULL){
#ifdef NEW_DEP
                                        malicious_fp=NULL;
#endif
                                        return FAIL;
                                }
                                else
                                        goto write;
                        }
                }
                else{
write:              fprintf(nf,"\n%s %s %s",
                               exchange_name,
                               exchange_office_code,
                               exchange_area_code);
                }
        }
        fclose(nf);
#ifdef NEW_DEP
#if (COMPILER == BCC)
        malicious_fp=_fsopen(MALICIOUS_FILE,"r+t",0);  // read and write, 0 is SH_COMPAT
#endif
#if (COMPILER == GCC)
        malicious_fp=fopen(MALICIOUS_FILE,"r+t");  // read and write
#endif
#endif
        return OK;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
