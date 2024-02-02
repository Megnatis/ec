
#ifdef REPORT_BULK_BILLING
#if( (TON == BULK_BILLING_OPR) || ((defined SECOND_BULK_BILLING_OPR) && (TON == SECOND_BULK_BILLING_OPR)) )
extern char bbfbs[]; /* bulk billing file backup string */
extern unsigned int backup_index;

void wbbr(unsigned int s,unsigned int count){
        unsigned int where;
        unsigned int xst;
        unsigned int xstcount;
        FILE *a;

        xst = s & (~WHICH_COUNT_MASK);
        if (xst >= NO_TOTAL_EXTENSION){
// 13/1/77, may put some debugging message to error file
                return;
        }
        where = s & WHICH_COUNT_MASK;
        if( where == STD_COUNT_FLAG){
// 13/1/77, try to eliminate sort of "unreasonable" counts
                if(count > 1500)
                        return;
                where = xst << 2;
                /* "where" (in bytes) is start of extension "s" billing record
                   whose size is 4 bytes (two for std count, two for local count) */
/* 7/9/76:
   after one block, another block starts which has again 4 bytes
   per extension, two for local extra count, and two for
   service count. This is for compatibility with previous versions of
   bulk billing file.
*/
        }
        else if( where == LOCAL_COUNT_FLAG){
                if(count > 5)
                        return;
                where = (xst << 2) + 2;
        }
        else if( where == LOCAL_EXTRA_COUNT_FLAG){
                if(count > 100)
                        return;
                where = (xst << 2) + (NO_TOTAL_EXTENSION << 2);
        }
/*
        else if( where == SERVICE_COUNT_FLAG){
                if(count > 5)
                        return;
                where = (xst << 2) + 2 + (NO_TOTAL_EXTENSION << 2);
        }
*/
#ifdef R1_SIGNALING
        else if( where == INTERNATIONAL_COUNT_FLAG){
                if(count > 5)
                        return;
                where = (xst << 2) + 2 + (NO_TOTAL_EXTENSION << 2);
        }
#endif
        else
                return;

        _dos_setfileattr(BULK_BILLING_FILE,FA_NORMAL);

        a=fopen(BULK_BILLING_FILE,"r+b");       /* read and write */
        /* by "r+", if during following lines, and before fclose s.th.
           happens, last file state is preserved */
        fseek(a,where,SEEK_SET);
        fread(&xstcount,2,1,a);
        /* my_getw(a); */
        xstcount += count;
        fseek(a,where,SEEK_SET);
        fwrite(&xstcount,2,1,a);
        /* my_putw(a,); */
        fclose(a);

        _dos_setfileattr(BULK_BILLING_FILE,FA_RDONLY | FA_SYSTEM | FA_HIDDEN);
}
unsigned int cbbf(void){  /* creat bulk billing file */
        FILE *nf;
        unsigned int xstcount=0;
        unsigned int i;

        _dos_setfileattr(BULK_BILLING_FILE,FA_NORMAL);
        _dos_setfileattr(BULK_BILLING_BACKUP_FILE,FA_NORMAL);

        if( (nf=fopen(BULK_BILLING_FILE,"rb")) == NULL){        /* read only */
        /* bulk billing file (or its directory) does not exist */
                if( (nf=fopen(BULK_BILLING_FILE,"wb")) == NULL){  /* creat */
                        /* file can not be created, may be directory does not exist */
#if COMPILER == GCC
                        if(mkdir(BULK_BILLING_DIRECTORY,S_IWUSR) != 0){
#else
                        if(mkdir(BULK_BILLING_DIRECTORY) != 0)
#endif
                                /* directory can not be created */
                                return NIL;
                        else    /* directory created, try again for creating file */
                                if( (nf=fopen(BULK_BILLING_FILE,"wb")) == NULL)
                                        return NIL;
                }
                for(i=0; i< NO_TOTAL_EXTENSION; i++){
                        fwrite(&xstcount,2,1,nf);       /* for std count */
                        fwrite(&xstcount,2,1,nf);       /* for local count */
                }
                for(i=0; i< NO_TOTAL_EXTENSION; i++){
                        fwrite(&xstcount,2,1,nf);       /* for local extra count */
                        fwrite(&xstcount,2,1,nf);       /* for service count */
                }
        }
        fclose(nf);

        strcpy(bbfbs,"copy ");
        strcat(bbfbs,BULK_BILLING_FILE);
        strcat(bbfbs," ");
        strcat(bbfbs,BULK_BILLING_BACKUP_FILE);
//      strcat(bbfbs," /Y > NUL:");

        if(kt_system(bbfbs) != 0){ /* earlier dos version, does not support /Y */
                strcpy(bbfbs,"copy ");
                strcat(bbfbs,BULK_BILLING_FILE);
                strcat(bbfbs," ");
                strcat(bbfbs,BULK_BILLING_BACKUP_FILE);
//              strcat(bbfbs," > NUL:");
                kt_system(bbfbs);
        }

        _dos_setfileattr(BULK_BILLING_FILE,FA_RDONLY | FA_SYSTEM | FA_HIDDEN);
        _dos_setfileattr(BULK_BILLING_BACKUP_FILE,FA_RDONLY | FA_SYSTEM | FA_HIDDEN);

        backup_index=0;

        return OK;
}

void bbf_backup(void){   /* backup bulk billing file */
        if( (++backup_index) >= BACKUP_INTERVAL){
                force_bbf_backup();
                backup_index=0;
        }
}

void force_bbf_backup(void){   /* backup bulk billing file */

        _dos_setfileattr(BULK_BILLING_FILE,FA_NORMAL);
        _dos_setfileattr(BULK_BILLING_BACKUP_FILE,FA_NORMAL);

        kt_system(bbfbs);

        _dos_setfileattr(BULK_BILLING_FILE,FA_RDONLY | FA_SYSTEM | FA_HIDDEN);
        _dos_setfileattr(BULK_BILLING_BACKUP_FILE,FA_RDONLY | FA_SYSTEM | FA_HIDDEN);
}
#endif
#endif

