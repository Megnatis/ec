

#define  WPC                    0x37a
#define  WPA                    0x378
#define  RPB                    0x379

#define  PERROR                 0x08
#define  PBUSY                  0x80
#define  PRNT_ACT       0x01
#define  B2PTX          0x02
#define  PQNE           0x04
#define  PRNT_QUE_LEN   (4096*4)

BYTE pq[PRNT_QUE_LEN];  /* printer queue */
int fpq,epq;  /* front & end printer queue */
BYTE pf;  /* printer flag */

int toprnq(BYTE d){
        if( ( (epq+1) & (PRNT_QUE_LEN-1)  )== fpq)  /* if queue full */
                return KT_NULL;
        epq= (epq + 1) & (PRNT_QUE_LEN-1);
        pq[epq]=d;
        pf |= PQNE;
        return OK;
}

BYTE frompq(void){
        BYTE a;
        if(epq == fpq){  /* if queue is empty */
                a=KT_NULL;
                pf &= ~PQNE;
        }
        else{
                fpq= (fpq + 1) & (PRNT_QUE_LEN-1);
                a=pq[fpq];
        }
        return a;
}

BYTE rpb(void){
        return (kt_inport8(RPB));
}
void wpc(BYTE a){
        kt_outport8(WPC,a);
}
void init_prntr(void){
        fpq=epq=0;
        pf=PRNT_ACT;
        kt_outport8(WPC,0x04);
}
void wlb2p(void){  /* write last byte to printer */
        kt_outport8(WPC,0x05);
        kt_outport8(WPC,0x05);
        kt_outport8(WPC,0x04);
}
void wb2p(BYTE d){  /* write byte to printer */
        kt_outport8(WPA,d);
        kt_outport8(WPC,0x05);
        kt_outport8(WPC,0x05);
        kt_outport8(WPC,0x04);
}

void prndrv(void){
        BYTE s;
/*
        if(pf & PQNE)
                lb |= PRINTER_LED;
        else
                lb &= ~PRINTER_LED;
*/
        if(pf & PRNT_ACT){
                s=rpb();
                if(pf & B2PTX){  /* if tx set */
                        if( !(s & PERROR) ){  /* if error occurred */
                                if( !(s & PBUSY)){
                                        wlb2p();
                                }
                                return;
                        }
                        pf &= ~B2PTX;
                        if(pf & PQNE)  /* if queue not empty */
                                if( (s & PBUSY) ){
                                        s=frompq();
                                        if(s != 0xff){
                                                wb2p(s);
                                                pf |= B2PTX;
                                        }
                                }
                        return;
                }
                /* tx reset & queue status set */
                if( (s & PBUSY) ){
                        s=frompq();
                        if(s != 0xff){
                                wb2p(s);
                                pf |= B2PTX;
                        }
                }
        }
        else
                wpc(0x04);
}

void ws2p(char *p){  /* write string to printer */
        if(*p != '\0')
                while(*p != '\0'){
                        if(toprnq(*p) != OK)
                                return;
                        ++p;
                }
}
