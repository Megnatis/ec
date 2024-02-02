#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include "omtfp.h"
/*
typedef unsigned char BYTE;
typedef struct dattim{
        BYTE yr;
        BYTE mo;
        BYTE da;
        BYTE hr;
        BYTE mi;
        BYTE se;
} DATTIM;
*/

DATTIM sdt;  /* system date time */
long count=0;
char in[MAX_NAME_LENGHT];  /* installation name */

DATTIM *get_dt(DATTIM *dt){

        outportb(0x70,13);
        if(inportb(0x71) & 0x80) {
                outportb(0x70,9);
                dt->yr=inportb(0x71);
                outportb(0x70,8);
                dt->mo=inportb(0x71);
                outportb(0x70,7);
                dt->da=inportb(0x71);
                outportb(0x70,4);
                dt->hr=inportb(0x71);
                outportb(0x70,2);
                dt->mi=inportb(0x71);
                outportb(0x70,0);
                dt->se=inportb(0x71);
                return dt;
        }
        else{
                count++;
                return dt;
        }
}

void winr(FILE *a){  /* write installation name record */
        int b,c;
        char *e;
        fputc(INSTALLATION_NAME_RECORD,a);
        b=strlen(in);
        fputc(b,a);
        c =b + INSTALLATION_NAME_RECORD;
        for(e=in;b;b--,e++){
                fputc(*e,a);
                c += *e;
        }
        c = ~c;
        c &= 0xff;
        fputc(c,a);
}

void wdr(FILE *a){  /* write date record */
        int c;
        fputc(DATE_RECORD,a);
        fputc(DATE_RECORD_SIZE,a);
        c=DATE_RECORD + DATE_RECORD_SIZE;
        sdt=*get_dt(&sdt);
        fputc(sdt.yr,a);
        c += sdt.yr;
        fputc(sdt.mo,a);
        c += sdt.mo;
        fputc(sdt.da,a);
        c += sdt.da;
        c = ~c;
        c &= 0xff;
        fputc(c,a);
        /* save checksum */
}

unsigned int cdbfn(){  /* creat detail billing file name */
        FILE *nf;
        if( (nf=fopen(DETAIL_BILLING_NAME_FILE,"rb")) == NULL){
        /* detail billing file name,file does not exist */
                if( (nf=fopen(DETAIL_BILLING_NAME_FILE,"wb")) == NULL)
                        return NIL;
                fprintf(nf,FIRST_DETAIL_BILLING_FILE);
                fclose(nf);
                nf=fopen(FIRST_DETAIL_BILLING_FILE,"w+b");
                if(nf == NULL)
                        return NIL;
                winr(nf);
                wdr(nf);
        }
        fclose(nf);
        return OK;
}

//BYTE b[45535];

FILE *flp;
BYTE org;  /* operator receive gain */
BYTE op_can_dial_on_co;
int i;
void main(void){
        printf("\n%x",cdbfn());
        printf("\n%x",cdbfn());
        printf("\n%x",cdbfn());


/* file read and write checking */

#if 0
        if( (flp=fopen(LOCAL_PARAM_FILE,"r+b")) == NULL){ /* file does not exist */
                org=OPR_RCVR_GAIN;
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
                op_can_dial_on_co=OP_FIRST_ZERO_ON_CO_DEFAULT;
#endif
                if( (flp=fopen(LOCAL_PARAM_FILE,"wb")) == NULL){
                        printf("\nOPPC: cannot create local parameters file!\n");
                }
                else{
                        putc(org,flp);
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
                        putc(op_can_dial_on_co,flp);
#endif
                        fclose(flp);
                }
        }
        else{
                org=getc(flp);
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
                op_can_dial_on_co=getc(flp);
#endif
                fclose(flp);
        }

        scanf("%x",&i);
        org=i;
        printf("\n %x",org);
        scanf("%x",&i);
        op_can_dial_on_co=i;
        printf("\n %x %x",org,op_can_dial_on_co);

        flp=fopen(LOCAL_PARAM_FILE,"r+b");
        putc(org,flp);
        fclose(flp);

        flp=fopen(LOCAL_PARAM_FILE,"r+b");
        fseek(flp,1L,SEEK_SET);
        putc(op_can_dial_on_co,flp);
        fclose(flp);

#endif // 0

/*      pointer arithmetic checking
        BYTE *d;
        long l;
        d=&b[22];
        l=(long)(d-&b[10]);
        printf("\n%d",l);
*/

/*      date read checking
        unsigned long i=1;

        for(;i!=0;i++){
                sdt=*get_dt(&sdt);
                if(
                      (sdt.yr == 0xff) ||
                      (sdt.mo == 0xff) ||
                      (sdt.da == 0xff) ||
                      (sdt.hr == 0xff) ||
                      (sdt.mi == 0xff) ||
                      (sdt.se == 0xff)          )
                        printf("\n %x %x %x %x %x %x ",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se);
                else if((i%0x40000L)==0)
                        printf("\n %x %x %x %x %x %x ",sdt.yr,sdt.mo,sdt.da,sdt.hr,sdt.mi,sdt.se);
                if(kbhit()) break;
        }
        printf("\n%ld %ld",i,count);
        printf("\n%x %x",CST_DEFAULT,AST_DEFAULT);
*/

/*      checksum calculation
        unsigned a,b;
        b=0;
        for(;;){
        scanf("%x",&a);
        b+=a;
        if(a==0) break;
        }
        b=~b;
        b&=0xff;
        printf("\n%x",b);
*/
}
