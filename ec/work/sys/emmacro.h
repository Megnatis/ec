
#define  EM_OGT_GROUP		0
#ifndef EM_ICT_GROUP
#define  EM_ICT_GROUP		1
#endif

#define  EM_OGT 		0x01
#define  EM_ICT 		0x02
#define  SYS_INTER_SIGNAL_DGT_SEND  0x04	// for EM_PULSE
#define  ANSWER_TX_IC		0x08
#define  ANSWER_RX_OG		0x10
#define  IMMEDIATE_ACK		0x20	/* only for pulse signalling */
					/* in cont. signalling is managed by t1 */
#define  WAIT_FOR_ACK		0x40	/* only for pulse signalling */
//#define  NOT_SEND_IMMEDIATE_ACK 0x40	  /* in cont. signalling is now managed by t2 */
#define SEND_RCVD_DTMF_DIGITS_ON_VOICE_PATH	0x04	// for EM_CONT
#define RECV_DTMF_DIGITS_ON_EM		0x20		// for EM_CONT
#define SEND_DIGITS_AS_DTMF_ON_EM	0x40		// for EM_CONT

#define  EOS_NEEDED		0x80
/* in em_cont, if eos_needed, then after seize, we do not wait for any
   ack (ground), we shall assume to get it after sending digits, however,
   currently, we do not really check this eos to come after sending digits,
   and if it does not come at all it works anyway, and if it comes is ignored
   because cp routines currently don't expect any ground after sending
   digits (we have generated an "artificial" ground for cp routines after seize
   if eos_needed is set) */
/* 23/12/76: we are going to use EOS_NEEDED for NIOC e&m lines, immediate start
   not wink start */
/* on ict, we do not send ack(ground) after getting seize, we shall send it
   later as an answer */

