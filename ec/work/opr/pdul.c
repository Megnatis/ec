                        /****  parameters down up load  ****/

//CPULOAD : putw->my_putw,getw->my_getw

#include "omtfp.h"


#ifdef PARAMETER_DOWN_UP_LOAD

extern char in[];
// HDLC_RS232
extern BYTE pswd_d4d3,pswd_d2d1;
extern unsigned int stm;
extern int pbs;
// CPULOAD
//extern unsigned int buf[],parmb[],f;
extern unsigned short buf[];
extern unsigned int f;
//extern unsigned int parmb[];
extern unsigned short parmb[];

#ifdef DAY_NIGHT_PARAMETERS_FILE
extern char *pfna[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE];
#else
extern char *pfna[MAX_NO_OPR_CARD][1];
#endif

extern int no_load_parm[MAX_NO_OPR_CARD];
extern int valid_parm_file_size[MAX_NO_OPR_CARD];

extern int card;

#endif

#ifdef PARAMETER_DOWN_UP_LOAD
int cpf(char *fn,BYTE h){  /* create parameters' file,file name pointed to by fn */
        int a;
        FILE *fp;
        char in[MAX_NAME_LENGHT];  /* installation name */
        strcpy(in,INSTALLATION_NAME);
        if( (fp=fopen(fn,"w+b")) == NULL)
                return FILE_CREATION_FAIL;
        fprintf(fp,"%s",in);
        fputc(0x1a,fp);
        a=MAX_NAME_LENGHT - strlen(in) - 1;
#ifdef AUTO_UPLOAD
        a=a-2;
#endif
        for(;a;a--)
                fputc(' ',fp);
#ifdef AUTO_UPLOAD
        my_putw(RESET,fp);
#endif
        my_putw( (PSWD_D4D3 << 8) | PSWD_D2D1,fp);
        for(a=0;a < no_load_parm[h];a++)
                my_putw(parmb[a],fp);
        my_putw(0,fp);  /* for fcs */
        cfcs(fp);
        fclose(fp);
        return FILE_CREATION_OK;
}
#endif



#ifdef PARAMETER_DOWN_UP_LOAD
int vfcs(FILE *fp){  /* validate file check sum */

        long nb;  /* number of block */
        long rb;         /* remaind bytes */
        unsigned int fcs;
        int a;
        long fsize;
        fseek(fp,0,SEEK_SET);
        fseek(fp,0,SEEK_END);
        fsize=ftell(fp);
        fseek(fp,0,SEEK_SET);
        fsize /= 2;
        /* find file size in int block */
        nb= (fsize / READ_BLOCK_SIZE);
        rb= (fsize % READ_BLOCK_SIZE);
        fcs=0;
        for(;nb;nb--){
                if(fread(buf,READ_BLOCK_SIZE * 2,1,fp) != 1)
                        return INVALID_FILE;
                for(a=0;a < READ_BLOCK_SIZE;a++)
                        fcs +=buf[a];
        }
/* 6/12/76 should check rb not be zero, otherwise fread returns 0
   and checksum will not be checked to be OK
*/
        if(rb != 0){
                if(fread(buf,(unsigned int) (rb * 2),1,fp) != 1)
                        return INVALID_FILE;
                for(a=0;a < rb;a++)
                        fcs +=buf[a];
        }
// CPULOAD
        if((unsigned short)fcs == 0xffff)
                return VALID_FILE;
        else{
                return INVALID_FILE;
        }
}
#endif


#ifdef PARAMETER_DOWN_UP_LOAD
void cfcs(FILE *fp){  /* calculate file check sum */
        long nb;  /* number of block */
        long rb;         /* remaind bytes */
        unsigned int fcs;
        int a;
        long fsize;
        fseek(fp,0,SEEK_SET);
        fseek(fp,0,SEEK_END);
        fsize=ftell(fp);
        fsize /= 2;
        /* find file size in int block */
        nb=fsize / READ_BLOCK_SIZE;
        rb=fsize % READ_BLOCK_SIZE;
        rb -= FILE_CHECK_SUM_SIZE;
        fcs=0;
        fseek(fp,0,SEEK_SET);
        for(;nb;nb--){
                if( (a=fread(buf,READ_BLOCK_SIZE * 2,1,fp)) != 1)
                        return;
                for(a=0;a < READ_BLOCK_SIZE;a++)
                        fcs +=buf[a];
        }
/* 6/12/76 should check rb not be zero, otherwise fread returns 0
   and checksum will not be created
*/
        if(rb != 0){
                if(fread(buf,(unsigned int) (rb * 2),1,fp) != 1)
                        return;
                for(a=0;a < rb;a++)
                        fcs +=buf[a];
        }
        fcs= ~fcs;
        fseek(fp,0,SEEK_CUR);
        my_putw(fcs,fp);
}
#endif


#ifdef PARAMETER_DOWN_UP_LOAD
int vpf(FILE *fp,BYTE h){
/* validate parameters file,if valid file,parameters is loaded to parmb[] */
        int a,b;
        long fsize;
        char n[MAX_NAME_LENGHT];
        char in[MAX_NAME_LENGHT];  /* installation name */
        fseek(fp,0,SEEK_SET);
        fseek(fp,0,SEEK_END);
        fsize=ftell(fp);
        if(fsize != valid_parm_file_size[h]){
                fclose(fp);
                return RESET;
        }
        fseek(fp,0,SEEK_SET);
        for(b=0;((a=getc(fp)) != 0x1a) && (b<MAX_NAME_LENGHT);b++)
                n[b]=a;
        if(b==MAX_NAME_LENGHT){
                fclose(fp);
                return RESET;
        }
        n[b]='\0';
// 31/3/78: for some unknown reasons, after power up (first program run),
// and before reaching here (for example by procedure of SABM, olrtn6, initialize,
// make_param_files, vpf, then "in" is not correct and contains in[0]=0xff,
// in[1]=0; funny enough if you put a printf in start of make_param_files()
// (whatever print) it will be o.k.! For the time being we correct this
// by again copying to "in"
        strcpy(in,INSTALLATION_NAME);
        if(strcmp(n,in) != 0){
                fclose(fp);
                return RESET;
        }

        fseek(fp,MAX_NAME_LENGHT,SEEK_SET);
        if(vfcs(fp) == INVALID_FILE){
                fclose(fp);
                return RESET;
        }

        fseek(fp,MAX_NAME_LENGHT+2,SEEK_SET);
        for(a=0;a < no_load_parm[h];a++)
                parmb[a]=my_getw(fp);
        fclose(fp);
        return OK;
}
#endif


#ifdef PARAMETER_DOWN_UP_LOAD
void csp(OMAR a){  /* collect system's parameters */
        int c;
        int h;
        c=a.wt.i;
        h=a.sys;
#ifdef HOT_STANDBY
        if ( c == ( MA_SYS_P | MAC_THIS_SYSTEM_IS_HOT)){
                mac_this_sys_is_hot_rtn(a);
                f &= ~PARM_DOWN;
                unmask_stm();  /* unmask all tasks */
                tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
                if(f & OPR_LANG_FLAG){
                        str2lcd("           . ¢ª  éìø—õ  ‘û¤—õ¤‘• –ê‘þ¤¢");
                        str2lcd("             :¢øªþõ ¥‘è ¢þ¢› ü¥þ¤ùõ‘÷¤“");
                }
                else
#endif
                        {
                        str2lcd("DOWN LOAD INTERRUPTED.                     ");
                        str2lcd("START NEW PROGRAM:");

                        }
               return;
        }
#endif  // HOT_STANDBY
        if(c != (MA_SYS_P | MAC_DOWN_LOAD_PARAMETERS) )
                return;
        parmb[pbs]=a.parm;
        ++pbs;
        if(pbs < no_load_parm[h]){
                if( (pbs & 0x0f) == 0x0f)
                        str2lcd(". ");
                return;
        }
        f &= ~PARM_DOWN;
// HDLC_RS232
        unmask_stm();  /* unmask all tasks */

#ifdef DAY_NIGHT_PARAMETERS_FILE
        cpf(pfna[h][0],h);
        /* if day & night files are defined,download parameters are saved */
        /* in day parameters file */
#else
        cpf(pfna[h][0],h);
#endif
        tolcdq(0xff,CLEAR_LCD);

#ifdef OPERATOR_FARSI
        if(f & OPR_LANG_FLAG){
                str2lcd("         .¢þ¨¤ ô‘õ— ù“ ‘û¤—õ¤‘• –ê‘þ¤¢");
                str2lcd("             :¢øªþõ ¥‘è ¢þ¢› ü¥þ¤ùõ‘÷¤“");
        }
        else{
#endif
                str2lcd("DOWN LOAD FINISHED.                     ");
                str2lcd("START NEW PROGRAM:");

#ifdef OPERATOR_FARSI
        }
#endif
}
#endif


#ifdef PARAMETER_DOWN_UP_LOAD

#ifdef DAY_NIGHT_PARAMETERS_FILE
void unp(void){  /* update new password */
        FILE *fp;
        int h;

        /* update password in day file */
        for(h=0;h<card;h++){
                if( (fp=fopen(pfna[h][0],"r+b")) == NULL)
                        cpf(pfna[h][0],h);
                else{
                        fseek(fp,MAX_NAME_LENGHT,SEEK_SET);
                        my_putw( (pswd_d4d3 << 8) | pswd_d2d1,fp);
                        cfcs(fp);
                        fclose(fp);
                }

                /* update password in night file */
                if( (fp=fopen(pfna[h][1],"r+b")) == NULL)
                        cpf(pfna[h][1],h);
                else{
                        fseek(fp,MAX_NAME_LENGHT,SEEK_SET);
                        my_putw( (pswd_d4d3 << 8) | pswd_d2d1,fp);
                        cfcs(fp);
                        fclose(fp);
                }
        }
}
#else

void unp(void){  /* update new password */
        FILE *fp;
        int h;
        for(h=0;h<card;h++){
                if( (fp=fopen(pfna[h][0],"r+b")) == NULL){
                        cpf(pfna[h][0],h);
                        return;
                }
                fseek(fp,MAX_NAME_LENGHT,SEEK_SET);
                my_putw( (pswd_d4d3 << 8) | pswd_d2d1,fp);
                cfcs(fp);
                fclose(fp);
        }
}
#endif

#endif


#ifdef PARAMETER_DOWN_UP_LOAD
void gcp(void){  /* get current password */
        FILE *fp;
        unsigned int a;

#ifdef DAY_NIGHT_PARAMETERS_FILE
        /* get from day file of first opr card */
        if( (fp=fopen(pfna[0][0],"rb")) == NULL){
                cpf(pfna[0][0],0);
                a=(PSWD_D4D3 << 8) | PSWD_D2D1;
        }
        else{
                fseek(fp,MAX_NAME_LENGHT,SEEK_SET);
                a=my_getw(fp);
                fclose(fp);
        }
#else
        if( (fp=fopen(pfna[0][0],"rb")) == NULL){
                cpf(pfna[0][0],0);
                a=(PSWD_D4D3 << 8) | PSWD_D2D1;
        }
        else{
                fseek(fp,MAX_NAME_LENGHT,SEEK_SET);
                a=my_getw(fp);
                fclose(fp);
        }
#endif
        pswd_d2d1=a & 0xff;
        pswd_d4d3=a >> 8;
}
#endif
