

//#include "dsmtfp.h"
//#define       DEBUG_ACT

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else  // CENTRAL_EXTERN

#include "hdr.h"
#if (OS == DOS)
#include <dos.h>
#endif
#ifdef HDLC_RS232

#if COMPILER == GCC
#if (OS == DOS)
extern _go32_dpmi_seginfo  old_serial1;
extern _go32_dpmi_seginfo  old_serial2;
#endif
#endif

extern char *mess[];

#if COMPILER != GCC
extern void interrupt (*old_serial1)(void);          /* old serial_port ISR */
extern void interrupt (*old_serial2)(void);          /* old serial_port ISR */
#endif

extern BYTE com_hwtx[NO_NEW_HDLC_COM_PORT][COM_HWBUF_SIZE];
extern BYTE com_hwrx[NO_NEW_HDLC_COM_PORT][COM_HWBUF_SIZE];

extern BUF com_hwrx_st[];
extern BUF com_hwtx_st[];

extern BYTE hdlc_dbg_flg[];
extern SRL_PORT_ST srl_port[];

#ifdef SML_DEBUG
extern FILE *sml_fp;
#endif

#endif

#endif // CENTRAL_EXTERN

/* ======================================================================== */
/* Function : serial_handler (Hardware Dependent)                           */
/*------------------------------------------------------------------------- */
/* new ISR for serial port.                                                 */
/* ======================================================================== */
#ifdef HDLC_RS232
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
#if COMPILER == GCC
void serial1_handler(void){
#else
void interrupt serial1_handler(void){
#endif

        BYTE dat,st;
        SRL_PORT_ST *srl_pt;
        BYTE indx;

        /*------------------------------------------------------------------- */
        /* IIR (Interrupt Identification  Register):                          */
        /* bit  b0    : 0 if interrupt pending                                */
        /* bits b2-b1 : 10:(Rx character available)                           */
        /*              01:(TxD empty)                                        */
        /*------------------------------------------------------------------- */

        /* Check if Rx Data Interrupt Request. */

        srl_pt=&srl_port[0];
        indx=FIRST_NEW_HDLC_COM_PORT;

#ifdef UART_16550
        st=inportb_srl(srl_pt->iir) & 0x0f;
#else
        st=inportb_srl(srl_pt->iir) & 0x07;
#endif
#ifdef DEBUG_ACT
        if (hdlc_dbg_flg[indx] == SET){
                prnt_char('s');prnt_char('t');prnt_char(':');prnt_byte(st);
        }
#endif
#ifdef UART_16550
        if ( ((st & 0x0c)==0x04) || ((st & 0x0c)==0x0c) ){                 /* RxD available interrupt */
#else
        if ( (st & 0x04)==0x04 ){                 /* RxD available interrupt */
#endif
                dat=inportb_srl(srl_pt->dpr);                 /* If yes ,puts it to rx_buf. */
                if (put_buf(&com_hwrx_st[indx],dat)==FAIL) {
                        prnt_msg(BUF_OVERFLOW,0x50d0);
                }  /* Error Code should be saved*/
                else{
#ifdef DEBUG_ACT
                        prnt_char('r');prnt_char(':');
                        prnt_byte(dat);
                        prnt_char(' ');
#endif
                }
        }

        /* Check if Tx Data Interrupt Request. */

        if ( (st & 0x02) ==0x02 ){                              /* TxD register empty Interrupt  */
                if (get_buf(&com_hwtx_st[indx],&dat)==OK){    /* tx_buf[] is not empty */
                        outportb_srl(srl_pt->dpr,dat);         /* out data to TxD register */
#ifdef DEBUG_ACT
                        prnt_char('t');prnt_char(':');
                        prnt_byte(dat);prnt_char(' ');
#endif
                }
                else{
#ifndef UART_16550
                      outportb_srl(srl_pt->ier,0x01);       /* disable TxD register empty interrupt,for preventing */
                                                /* interrupt request when we have no data to transmit. */
#endif                                          /* bit 0: RxD interrupt enable.bit 1: TxD interrupt enable. */
                }
        }
#ifdef SYSTEM_SIDE
        kt_outport8(0x20,0x20);
#endif
#ifdef OPERATOR_SIDE
        kt_outport8(0x20,0x20);
#endif
}
#endif // USE_INTERRUPT_FOR_HDLC_RS232
#endif
/* ======================================================================== */
/* Function : serial_handler (Hardware Dependent)                           */
/*------------------------------------------------------------------------- */
/* new ISR for serial port.                                                 */
/* ======================================================================== */
#ifdef HDLC_RS232
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
#if COMPILER == GCC
void serial2_handler(void){
#else
void interrupt serial2_handler(void){
#endif

        BYTE dat,st;
        SRL_PORT_ST *srl_pt;
        BYTE indx;

        /*------------------------------------------------------------------- */
        /* IIR (Interrupt Identification  Register):                          */
        /* bit  b0    : 0 if interrupt pending                                */
        /* bits b2-b1 : 10:(Rx character available)                           */
        /*              01:(TxD empty)                                        */
        /*------------------------------------------------------------------- */

        /* Check if Rx Data Interrupt Request. */

        srl_pt=&srl_port[1];
        indx=1-FIRST_NEW_HDLC_COM_PORT;

#ifdef UART_16550
        st=inportb_srl(srl_pt->iir) & 0x0f;
#else
        st=inportb_srl(srl_pt->iir) & 0x07;
#endif
        if (hdlc_dbg_flg[indx] == SET){
                prnt_char('s');prnt_char('t');prnt_char(':');prnt_byte(st);
        }
#ifdef UART_16550
        if ( ((st & 0x0c)==0x04) || ((st & 0x0c)==0x0c) ){                 /* RxD available interrupt */
#else
        if ( (st & 0x04)==0x04 ){                 /* RxD available interrupt */
#endif
                dat=inportb_srl(srl_pt->dpr);                 /* If yes ,puts it to rx_buf. */
                if (put_buf(&com_hwrx_st[indx],dat)==FAIL) {
                        prnt_msg(BUF_OVERFLOW,0x50d1);
                }  /* Error Code should be saved*/
                else{
#ifdef DEBUG_ACT
                        prnt_char('r');prnt_char(':');
                        prnt_byte(dat);
                        prnt_char(' ');
#endif
                }
        }

        /* Check if Tx Data Interrupt Request. */

        if ( (st & 0x02) ==0x02 ){                              /* TxD register empty Interrupt  */
                if (get_buf(&com_hwtx_st[indx],&dat)==OK){    /* tx_buf[] is not empty */
                        outportb_srl(srl_pt->dpr,dat);         /* out data to TxD register */
#ifdef DEBUG_ACT
                        prnt_char('t');prnt_char(':');
                        prnt_byte(dat);prnt_char(' ');
#endif
                }
                else{
#ifndef UART_16550
                      outportb_srl(srl_pt->ier,0x01);       /* disable TxD register empty interrupt,for preventing */
                                                /* interrupt request when we have no data to transmit. */
#endif                                          /* bit 0: RxD interrupt enable.bit 1: TxD interrupt enable. */
                }
        }
#ifdef SYSTEM_SIDE
        kt_outport8(0x20,0x20);
#endif
#ifdef OPERATOR_SIDE
        kt_outport8(0x20,0x20);
#endif
}
#endif // USE_INTERRUPT_FOR_HDLC_RS232
#endif
/* ========================================================================= */
/* Function : init_serial_port                                               */
/* ------------------------------------------------------------------------- */
/* initializes serial port and serial buffers.                               */
/* ========================================================================= */

#ifdef HDLC_RS232
#ifndef SERIAL_INTERFACE_ACTIVE         // CPU_EC512
void init_com_port(BYTE n){
        SRL_PORT_ST *srl_pt;

//      init_com_regs();
        srl_pt=&srl_port[n];

#ifdef OPERATOR_SIDE
        if(srl_pt->should_write_to_376)
            kt_outport8(0x376,1);
#endif
        outportb_srl(srl_pt->lcr,0x87);
#ifdef DEBUG_ACT
        prnt_char('n');prnt_char(':');prnt_byte(n);prnt_char(CR);
#endif
        if (inportb_srl(srl_pt->lcr) != 0x87){
                srl_pt->act=RESET;
#ifdef OPERATOR_SIDE
                if(srl_pt->should_write_to_376)
                    kt_outport8(0x376,0);
#endif
                return;
        }
        srl_pt->act=SET;
        outportb_srl(srl_pt->lcr,0x80);   /* DLAB bit=1 (enable DLAB)    */
#ifdef CLOCK_8MHZ_USED_FOR_16550
        outportb_srl(srl_pt->dpr,52);     /* LSB divide count (DLAB=1)   */
#else
#if ((defined OPERATOR_SIDE) && (defined CPU_EC512))
        outportb_srl(srl_pt->dpr,48);     /* LSB divide count (DLAB=1)   */
#else
        outportb_srl(srl_pt->dpr,12);     /* LSB divide count (DLAB=1)   */
#endif
#endif
        outportb_srl(srl_pt->ier,0x00);   /* MSB divide count (DLAB=1)   */
                                      /* 24  , baud rate = 4800      */
                                      /* 24*8, baud rate =  600      */
                                      /* 24*2, baud rate = 2400      */
                                      /* 12, baud rate = 9600        */
        outportb_srl(srl_pt->lcr,0x07);   /* bits 1-0 : character length */
                                      /*            00:5 bits, 01:6 bits, 10:7 bits, 11:8bits */
                                      /* bit 2    : no. of stop bits */
                                      /*            0:1 stop bit, 1:2 stop bits(6,7,8 bits length) */
                                      /* bits 3-5 : parity mode bits */
                                      /*            0xx:no parity, 100:odd parity, 101: even parity */
                                      /*            110:high parity, 111:low parity */
                                      /* bit 6    : set break */
                                      /* bit 7    : DLAB (divisor latch access) bit */

                                      /* now programmed for 8,n,2 */
//      outportb_srl(srl_pt->mcr,0x09);
                                      /* modem outputs, DTR active, enable Out2
                                         to allow interrupts to effect */

        outportb_srl(srl_pt->ier,0x00);   /* disable interrupts */

#ifdef USE_INTERRUPT_FOR_HDLC_RS232
#if COMPILER == GCC
#if (OS == DOS)
        LOCK_FUNCTION(srl_pt->isr);
        LOCK_VARIABLE(...........);
        LOCK_VARIABLE(...........);
        _go32_dpmi_get_protected_mode_interrupt_vector(SRL_VECT,&srl_pt->old_isr);
        {
            _go32_dpmi_seginfo  newisr;
            newisr.pm_offset = (int) srl_pt->isr;
            newisr.pm_selector = _go32_my_cs();
            _go32_dpmi_chain_protected_mode_interrupt_vector(srl_pt->srl_vect,&newisr);
        }
#endif // OS == DOS
#else
        srl_pt->old_isr=getvect(srl_pt->srl_vect);        /* get old interrupt vector */
        setvect(srl_pt->srl_vect,srl_pt->isr);    /* initialize interrupt vector location*/
#endif

#if (OS == DOS)
        {
            BYTE data;
#ifdef SYSTEM_SIDE
            data=kt_inport8(0x21);
#endif
#ifdef OPERATOR_SIDE
            data=kt_inport8(0x21);
#endif
            data=data & !(0x10);
#ifdef SYSTEM_SIDE
            kt_outport8(0x21,data); /* set serial_port interrupt */
#endif
#ifdef OPERATOR_SIDE
            kt_outport8(0x21,data); /* set serial_port interrupt */
#endif
        }
#endif // OS == DOS
#endif

        inportb_srl(srl_pt->dpr);   /* empty data port register */
#ifdef UART_16550

//      outportb_srl(srl_pt->fcr,0xc1);
        outportb_srl(srl_pt->fcr,0xc7);
        /* bit0:FIFO enable:1
           bit1:RCVR FIFO reset:1
           bit2:XMIT FIFO reset:1
           bit3:DMA mode select:0
           bits 4,5:reserved
           bits 7,6:RCVR trigger level:11
        */
#endif
#ifndef USE_INTERRUPT_FOR_HDLC_RS232
        outportb_srl(srl_pt->ier,0x00);  /* bit 0: Rx data interrupt disable */
                                     /* bit 1: Tx data interrupt disable */
#else
        outportb_srl(srl_pt->ier,0x03);  /* bit 0: Rx data interrupt enable */
                                     /* bit 1: Tx data interrupt enable */
#endif
#ifdef OPERATOR_SIDE
        if(srl_pt->should_write_to_376)
            kt_outport8(0x376,0);
#endif
}
#endif
#endif
/* ======================================================================== */
/* Function : reset_serial_vector (Hardware Dependent)                      */
/*------------------------------------------------------------------------- */
/* reset serial port handler to the initial routine.                        */
/* ======================================================================== */

#ifdef HDLC_RS232
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
void reset_com_port(BYTE n){
#if COMPILER == GCC
#if (OS == DOS)
        _go32_dpmi_set_protected_mode_interrupt_vector(srl_port[n].srl_vect,&srl_port[n].old_isr);
#endif // OS == DOS
#else
        setvect(srl_port[n].srl_vect,srl_port[n].old_isr);  /* reset to old interrupt vector */
#endif
}
#endif // USE_INTERRUPT_FOR_HDLC_RS232
#endif
/* ======================================================================= */
/* Function : disable_serial_interrupt (Hardware Dependent)                 */
/* ======================================================================= */

#ifdef HDLC_RS232
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
void disable_com_interrupt(BYTE n){

        outportb_srl(srl_port[n].ier,0x00);      /* disable interrupts */
}
#endif // USE_INTERRUPT_FOR_HDLC_RS232
#endif
/* ======================================================================= */
/* Function : enable_serial_interrupt (Hardware Dependent)                 */
/* ======================================================================= */

#ifdef HDLC_RS232
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
void enable_com_interrupt(BYTE n){

        outportb_srl(srl_port[n].ier,0x03);     /* bit 0: Rx data interrupt enable */
                                /* bit 1: Tx data interrupt enable */
}
#endif // USE_INTERRUPT_FOR_HDLC_RS232
#endif
/* ======================================================================= */
/* ======================================================================= */

#if 0
#ifdef POLL_UART_16550
#ifdef HDLC_RS232
void com_uart_fifo_handler(void){

        BYTE i,st,dat;
        BYTE n,indx;
        SRL_PORT_ST *srl_pt;

        /* LSR bits :
           bit0:data ready
           bit1:overrun error
           bit2:parity error
           bit3:framing error
           bit4:break interrupt
           bit5:transmitter holding register
           bit6:transmitter empty
           bit7:error in receiver fifo
        */
        for(n=FIRST_NEW_HDLC_COM_PORT,indx=n-FIRST_NEW_HDLC_COM_PORT ;n<LAST_NEW_HDLC_COM_PORT;n++,indx++){

                srl_pt=&srl_port[n];
                //if com port is not installed.
                if (srl_pt->act == RESET) continue;
                st=inportb_srl(srl_pt->lsr);                 /* If yes ,puts it to rx_buf. */
                while (st&0x01){
                        dat=inportb_srl(srl_pt->dpr);
                        if((st&0x0e)==0){
                                if (put_buf(&com_hwrx_st[indx],dat)==FAIL){
                                         prnt_msg(BUF_OVERFLOW,0x50d2);
                                }
                        }
#ifdef DEBUG_ACT
                        prnt_char('r');prnt_char(':');
                        prnt_byte(dat);
                        prnt_char(' ');
#endif
                        st=inportb_srl(srl_pt->lsr);

                // bit0 is reset by reading all of the data in the FIFO
                }

                if (st&0x20){
#ifdef DEBUG_ACT
                        prnt_char('s');prnt_char('t');prnt_char(':');
                        prnt_byte(st);prnt_char('-');
                        prnt_char(CR);
#endif
                        /*
                           since there is no bit for checking the level of data
                           in tx FIFO,I put max 16 bytes to FIFO when it is empty.
                        */
                        for (i=0;i<16;i++){
                                if (get_buf(&com_hwtx_st[indx],&dat)==OK){
                                        outportb_srl(srl_pt->dpr,dat);
#ifdef DEBUG_ACT
                                        prnt_char('t');prnt_char(':');
                                        prnt_byte(dat);prnt_char(' ');
#endif
                                }
                                else{
                                        break;
                                }
                        }
                }
        }
}
#endif
#endif


#endif // 0
/* ======================================================================= */
/* ======================================================================= */
/* ======================================================================= */

#ifdef HDLC_RS232
#ifndef USE_INTERRUPT_FOR_HDLC_RS232
#ifdef SERIAL_INTERFACE_ACTIVE         // CPU_EC512
void com_uart_tx_fifo_handler(void){
        BYTE i,st,dat;
        BUF *pt;
        st=si_rcntrl();
        if(st & (FE_8251 | OE_8251 | PE_8251) ){  /* if error */
                si_wcntrl(0x37);
                /* RTS, error reset, DTR, rcv & xmit enable */
                si_rrx_data();  /* empty rcv buffer */
                return;
        }
        if(st & TXRDY_8251){  /* if tx ready */
                pt=&com_hwtx_st[0];
                if (get_buf(pt,&dat)==OK){
                        si_wcntrl(0x37);
                        for(i=0;i<0x2;i++);
                        si_wtx_data(dat);
                }
        }
}
#else
void com_uart_tx_fifo_handler(void){

#if ((defined OPERATOR_SIDE) && (defined CPU_EC512))
#else
        BYTE i;
#endif
        BYTE st,dat;
        BYTE n,indx;
        SRL_PORT_ST *srl_pt;
        BUF *pt;
        unsigned int data_port;
#ifdef SML_DEBUG
        BYTE data_exist;
#endif
/*
        considering overhead of LAPB protocol and this layer protocol,
        it is possible to have a packet with more than 16 bytes.
        so,I put tx_handler in 20msec tick and rx_handler in 10msec tick
        to be sure that before sending second packet ,first packet is read.
*/

        /* LSR bits :
           bit0:data ready
           bit1:overrun error
           bit2:parity error
           bit3:framing error
           bit4:break interrupt
           bit5:transmitter holding register
           bit6:transmitter empty
           bit7:error in receiver fifo
        */
        for(n=FIRST_NEW_HDLC_COM_PORT,indx=n-FIRST_NEW_HDLC_COM_PORT ;n<LAST_NEW_HDLC_COM_PORT;n++,indx++){

                srl_pt=&srl_port[n];
                //if com port is not installed.
                if (srl_pt->act == RESET) continue;
#ifdef OPERATOR_SIDE
                if(srl_pt->should_write_to_376)
                    kt_outport8(0x376,1);
#endif
                st=inportb_srl(srl_pt->lsr);                 /* If yes ,put it to rx_buf. */
                if (st&0x40){
#ifdef DEBUG_ACT
                        prnt_char('s');prnt_char('t');prnt_char(':');
                        prnt_byte(st);prnt_char('-');
                        prnt_char(CR);
#endif
                        /*
                           since there is no bit for checking the level of data
                           in tx FIFO,I put max 16 bytes to FIFO when it is empty.
                        */
                        pt=&com_hwtx_st[indx];
                        data_port=srl_pt->dpr;
#ifdef SML_DEBUG
                        data_exist=RESET;
#endif
#if ((defined OPERATOR_SIDE) && (defined CPU_EC512))
// if this is the operator of an ec512, it should not send more than one
// byte in every tick
#else
                        for (i=0;i<FIFO_SIZE_16550;i++)
#endif
                        {
                                if (get_buf(pt,&dat)==OK){
                                        outportb_srl(data_port,dat);
#ifdef SML_DEBUG
                                        data_exist=SET;
                                        fprintf(sml_fp,"t:%x ",dat);
#endif
#ifdef DEBUG_ACT
                                        prnt_char('t');prnt_char(':');
                                        prnt_byte(dat);prnt_char(' ');
#endif
                                }
                                else{
                                        break;
                                }
                        }
#ifdef SML_DEBUG
                        if (data_exist)
                                fprintf(sml_fp,"\n");
#endif
                }
#ifdef OPERATOR_SIDE
                if(srl_pt->should_write_to_376)
                    kt_outport8(0x376,0);
#endif
        }
}
#endif
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
/* ======================================================================= */

#ifdef HDLC_RS232
#ifndef USE_INTERRUPT_FOR_HDLC_RS232
#ifdef SERIAL_INTERFACE_ACTIVE         // CPU_EC512
void com_uart_rx_fifo_handler(void){
        BYTE st,dat;
        BUF *pt;
        st=si_rcntrl();
        if(st & (FE_8251 | OE_8251 | PE_8251) ){  /* if error */
                si_wcntrl(0x37);
                /* RTS, error reset, DTR, rcv & xmit enable */
                si_rrx_data();  /* empty rcv buffer */
                return;
        }
        if(st & RXRDY_8251){  /* if rx ready */
                dat=si_rrx_data();
                pt=&com_hwrx_st[0];
                if (put_buf(pt,dat)==FAIL){
                        prnt_msg(BUF_OVERFLOW,0x50d3);
                }
        }
}
#else
void com_uart_rx_fifo_handler(void){

        BYTE st,dat;
        BYTE n,indx;
        SRL_PORT_ST *srl_pt;
        BYTE cnt;
        BUF *pt;

        /* LSR bits :
           bit0:data ready
           bit1:overrun error
           bit2:parity error
           bit3:framing error
           bit4:break interrupt
           bit5:transmitter holding register
           bit6:transmitter empty
           bit7:error in receiver fifo
        */
        for(n=FIRST_NEW_HDLC_COM_PORT,indx=n-FIRST_NEW_HDLC_COM_PORT ;n<LAST_NEW_HDLC_COM_PORT;n++,indx++){

                srl_pt=&srl_port[n];
                //if com port is not installed.
                if (srl_pt->act == RESET) continue;
#ifdef OPERATOR_SIDE
                if(srl_pt->should_write_to_376)
                    kt_outport8(0x376,1);
#endif
                st=inportb_srl(srl_pt->lsr);                 /* If yes ,puts it to rx_buf. */

                cnt=FIFO_SIZE_16550;
                pt=&com_hwrx_st[indx];
                while (st&0x01){
                        dat=inportb_srl(srl_pt->dpr);
                        /* bit1:overrun error,bit2:parity error,bit3:framing error*/
                        if((st&0x0e)==0){
                                if (put_buf(pt,dat)==FAIL){
                                        prnt_msg(BUF_OVERFLOW,0x50d3);
                                }
                        }
#ifdef DEBUG_ACT
                        prnt_char('r');prnt_char(':');
                        prnt_byte(dat);
                        prnt_char(' ');
#endif
                        // to prevent remaining in this loop,if there is an error.
                        if (cnt-- == 0){
                                prnt_msg(SERIAL_ERROR,0x1666);
                                break;
                        }
                        st=inportb_srl(srl_pt->lsr);

                // bit0 is reset by reading all of the data in the FIFO
                }
#ifdef OPERATOR_SIDE
                if(srl_pt->should_write_to_376)
                    kt_outport8(0x376,0);
#endif
        }
}
#endif
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef HDLC_RS232
#ifndef SERIAL_INTERFACE_ACTIVE         // CPU_EC512
void init_com_regs(void){

        BYTE n;
        SRL_PORT_ST *srl_pt;

#if (NO_NEW_SRL_LINK > 0)
        srl_pt=&srl_port[0];
        srl_pt->start_addr=SERIAL1_START;
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
        srl_pt->srl_vect=SRL1_VECT;
        srl_pt->isr=serial1_handler;
        srl_pt->old_isr=old_serial1;
#endif
#endif

#if (NO_NEW_SRL_LINK > 1)
        srl_pt=&srl_port[1];
        srl_pt->start_addr=SERIAL2_START;
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
        srl_pt->srl_vect=SRL2_VECT;
        srl_pt->isr=serial2_handler;
        srl_pt->old_isr=old_serial2;
#endif
#endif

#if (NO_NEW_SRL_LINK > 2)
        srl_pt=&srl_port[2];
        srl_pt->start_addr=SERIAL3_START;
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
        srl_pt->srl_vect=SRL3_VECT;
        srl_pt->isr=serial1_handler;
        srl_pt->old_isr=old_serial1;
#endif
#endif
#if (NO_NEW_SRL_LINK > 3)
        srl_pt=&srl_port[3];
        srl_pt->start_addr=SERIAL4_START;
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
        srl_pt->srl_vect=SRL4_VECT;
        srl_pt->isr=serial2_handler;
        srl_pt->old_isr=old_serial2;
#endif
#endif

#ifdef OPERATOR_SIDE
        for(n=FIRST_NEW_HDLC_COM_PORT;n<LAST_NEW_HDLC_COM_PORT;n++)
                srl_pt->should_write_to_376=RESET;
#endif

#ifdef OPERATOR_SIDE
        kt_outport8(0x3f0+0xb,0x87);   // lcr register of default com1
        if(kt_inport8(0x3f0+0xb) != 0x87){
//          kt_outport8(0x376,0);
            kt_outport8(0x168+0xb,0x87);
            if(kt_inport8(0x168+0xb) == 0x87){
                srl_port[0].start_addr=0x168;
            }
        }
#if (NO_NEW_SRL_LINK >= 2)
        kt_outport8(0x2f0+0xb,0x87);   // lcr register of default com2
        if(kt_inport8(0x2f0+0xb) != 0x87){
//          kt_outport8(0x376,1);
            kt_outport8(0x168+0xb,0x87);
            if(kt_inport8(0x168+0xb) == 0x87){
                srl_port[1].start_addr=0x168;
            }
        }
        if((srl_port[0].start_addr == 0x168) && (srl_port[1].start_addr == 0x168)){
            srl_port[1].should_write_to_376=SET;
            kt_outport8(0x376,0);
        }
#endif
#endif

        for(n=FIRST_NEW_HDLC_COM_PORT;n<LAST_NEW_HDLC_COM_PORT;n++){
                srl_pt=&srl_port[n];
                srl_pt->dpr=srl_pt->start_addr+DPR_OFFSET;
                srl_pt->ier=srl_pt->start_addr+IER_OFFSET;
                srl_pt->iir=srl_pt->start_addr+IIR_OFFSET;
#ifdef  UART_16550
                srl_pt->fcr=srl_pt->start_addr+FCR_OFFSET;
#endif
                srl_pt->lcr=srl_pt->start_addr+LCR_OFFSET;
                srl_pt->mcr=srl_pt->start_addr+MCR_OFFSET;
                srl_pt->lsr=srl_pt->start_addr+LSR_OFFSET;
                srl_pt->msr=srl_pt->start_addr+MSR_OFFSET;
        }
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef HDLC_RS232
void init_com_bufs(BYTE indx){

        com_hwtx_st[indx].arr = &com_hwtx[indx][0];
        com_hwtx_st[indx].front = com_hwtx_st[indx].rear = 0;
        com_hwtx_st[indx].size = COM_HWBUF_SIZE;

        com_hwrx_st[indx].arr = &com_hwrx[indx][0];
        com_hwrx_st[indx].front = com_hwrx_st[indx].rear = 0;
        com_hwrx_st[indx].size = COM_HWBUF_SIZE;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef HDLC_RS232
#ifdef USE_INTERRUPT_FOR_HDLC_RS232
void enable_com_trx_int(BYTE indx){
        outportb_srl(srl_port[indx].ier,0x03);
        /* bit 0: Rx data interrupt enable */
        /* bit 1: Tx data interrupt enable */
}
#endif // USE_INTERRUPT_FOR_HDLC_RS232
#endif
/*===========================================================================*/
/*===========================================================================*/
//LEMON : || (defined MGW672_SYSTEM) added
#if ((defined HDLC_RS232) || (defined SERIAL_PORT))
#if ((defined EC2K_SYSTEM) || (defined EC1K_SYSTEM) || (defined EC10K_SYSTEM)  ||  \
     (defined PU2K_SYSTEM) || (defined PC_SYSTEM)   || (defined MGW672_SYSTEM) ||  \
     (defined OPERATOR_SIDE))
BYTE inportb_srl(unsigned int port){
#ifdef SYSTEM_SIDE
        return(kt_inport8(port));
#endif
#ifdef OPERATOR_SIDE
        return(kt_inport8(port));
#endif
}
#endif
#endif
/*===========================================================================*/
/*===========================================================================*/
//LEMON : || (defined MGW672_SYSTEM) added
#if ((defined HDLC_RS232) || (defined SERIAL_PORT))
#if ((defined EC2K_SYSTEM) || (defined EC1K_SYSTEM) || (defined EC10K_SYSTEM)  ||  \
     (defined PU2K_SYSTEM) || (defined PC_SYSTEM)   || (defined MGW672_SYSTEM) ||  \
     (defined OPERATOR_SIDE))
void outportb_srl(unsigned int port,BYTE dat){
#ifdef SYSTEM_SIDE
        kt_outport8(port,dat);
#endif
#ifdef OPERATOR_SIDE
        kt_outport8(port,dat);
#endif
}
#endif
#endif //HDLC_RS232
/*===========================================================================*/
/*===========================================================================*/
