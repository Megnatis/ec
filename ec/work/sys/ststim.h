
		/*****		states definitions		*****/
#define  IDLE				0
#define  PARK				1
#define  HOLD				2
#define  IN_INTRNL_QU			3
	/* in internal queue */
#define  IN_RCL_QU			4
	/* in recall queue */
#define  IN_EXTRNL_QU			5
	/* in external queue */
#define  AWAIT_ON_HOOK			6
#define  AWAIT_TRNK_RLS 		7
#define  AWAIT_1ST_DGT			8
#define  PATH_ANALYSIS			9
#define  INTRNL 			0x0a
#define  SERVICE_ANALYSIS		0x0b
#define  CALL_WAITING			0x0c
#define  RINGING			0x0d
#define  FLASH_DTCT_RING		0x0e
#define  CONVERSATION			0x0f
#define  INTRNL_CALL			CONVERSATION
#define  FLASH_DTCT_CONVERSATION	0x10
#define  AWAIT_PARK_LOC 		0x11
#define  EXTRNL_CALL			0x12
#define  RING_CALL_BACK 		0x13
#define  COLLECT_DGTS			0x14
//#define  AWAIT_FLASH			  0x15
#define  LOCK				0x15
#define  AWAIT_TRNK_SZR 		0x16
#define  FLASH_DTCT_TRNK		0x17
#define  CO_TRNK_GROUP			0x18
#define  TIE_TRNK_GROUP 		0x19
#define  FLASH_CONFIRM			0x1a
#define  DIAL_OPR			0x1b
#define  OPR_EXT_RINGING		0x1c
#define  OPR_TALK			0x1d
#define  TRNK_AWAIT_EXT 		0x1e
#define  OPR_NIGHT			0x1f
#define  TRUNK_TRUNK_TALK		0x20
#define  EM_TRNK_GROUP			0x21
#define  RINGING_TO_NITE		0x22
// added 20/9/77
#define  SPECIAL_SERVICE_ANALYSIS	0x23

#if ((defined OPR_OPR_TALK) && (defined SYSTEM_SIDE))
#define  OPR_AT_HOLD			0x24
#endif

#ifdef BAD_PAYER
// added on 20/3/78
#define BAD_PAYER_IN_B1 		0x25
#endif
#define WAKE_UP_RINGING 		0x26

/**** E&M states ****/

#include "../sys/emstates.h"

/**** CAS3 states ****/

#include "../sys/csstates.h"

/**** conference states ****/
/**-----------------------------------------------------------------------**/
#ifdef CONFERENCE_ACT
#define CONFERENCE		CONFERENCE_STATES_START
#define CONFERENCE_FAIL 	(CONFERENCE_STATES_START + 1)

#ifdef CONFERENCE_N_PARTY
#define FLASH_DTCT_CONFERENCE	(CONFERENCE_STATES_START + 2)
#define CONFERENCE_STATE_END	(CONFERENCE_STATES_START + 2)
#else
#define CONFERENCE_STATE_END	(CONFERENCE_STATES_START + 1)
#endif

#define NO_CONFERENCE_STATES	(CONFERENCE_STATE_END - CONFERENCE_STATES_START + 1)
#define AMPLIFIER_STATE_START	(CONFERENCE_STATE_END + 1)

#else
#define NO_CONFERENCE_STATES	0
#define AMPLIFIER_STATE_START	CONFERENCE_STATES_START
#endif
/**-----------------------------------------------------------------------**/

/**** amplifier state ****/
/**-----------------------------------------------------------------------**/
#ifdef AMPLIFIER_INSTALLED
#define AMPLIFIER_ON		AMPLIFIER_STATE_START

#define AMPLIFIER_STATE_END	AMPLIFIER_STATE_START
#define NO_AMPLIFIER_STATE	(AMPLIFIER_STATE_END - AMPLIFIER_STATE_START + 1)
#define SIREN_STATE_START	(AMPLIFIER_STATE_END + 1)

#else
#define NO_AMPLIFIER_STATE	0
#define SIREN_STATE_START	AMPLIFIER_STATE_START
#endif
/**-----------------------------------------------------------------------**/

/**** siren state ****/
/**-----------------------------------------------------------------------**/
#ifdef SIREN_INSTALLED
#define SIREN_ON		SIREN_STATE_START

#define SIREN_STATE_END 	SIREN_STATE_START
#define NO_SIREN_STATE		(SIREN_STATE_END - SIREN_STATE_START + 1)
#define LD_STATES_START 	(SIREN_STATE_END + 1)

#else
#define NO_SIREN_STATE		0
#define LD_STATES_START 	SIREN_STATE_START
#endif
/**-----------------------------------------------------------------------**/
/**** LD states *****/
/**-----------------------------------------------------------------------**/

#ifdef LD_INSTALLED
/* LD_OG states : */
#define 	AWAIT_RMT_BIAS_LD		LD_STATES_START
#define 	SEND_DGTS_LD			(LD_STATES_START+1)
#define 	AWAIT_ANSWER_LD 		(LD_STATES_START+2)
#define 	EXT_TALK_LD			(LD_STATES_START+3)
#define 	AWAIT_LD_FORWARD_BIAS_CBK	(LD_STATES_START+4)
#define 	SEC_AWAIT_RMT_BIAS_LD		(LD_STATES_START+5)
/* LD_IC states : */
#define 	AWAIT_LD_DEST_1ST_DGT		(LD_STATES_START+6)
#define 	AWAIT_LD_EXT_NUM		(LD_STATES_START+7)
#define 	LD_EXT_RINGING			(LD_STATES_START+8)
#define 	LD_TALK_EXT			(LD_STATES_START+9)
#define 	AWAIT_LD_ON_HOOK_CLF		(LD_STATES_START+10)
#define 	AWAIT_LD_LOOP_FORWARD_CBK	(LD_STATES_START+11)
#define 	LD_STATES_END			(LD_STATES_START+11)

#define 	RMT_EXT_STATES_START		(LD_STATES_END + 1)

#define 	NO_LD_STATES			(LD_STATES_END - LD_STATES_START + 1)
#else
#define 	RMT_EXT_STATES_START		LD_STATES_START
#define 	NO_LD_STATES		0
#endif
/**-----------------------------------------------------------------------**/
/**-----------------------------------------------------------------------**/
/**** RSU states *****/
/**-----------------------------------------------------------------------**/
#if ((defined RMT_EXT) || (defined RMT_CB))

#define 	AWAIT_LINK_DISCONNECT_ACK	RMT_EXT_STATES_START
#define 	EXT_NOT_ACCESSIBLE		(RMT_EXT_STATES_START+1)

#define 	RMT_EXT_STATES_END		(RMT_EXT_STATES_START+1)
#define 	NO_RMT_EXT_STATES		(RMT_EXT_STATES_END - RMT_EXT_STATES_START + 1)
#define 	EM_RSUSIDE_STATES_START 	(RMT_EXT_STATES_END+1)
#else

#define 	EM_RSUSIDE_STATES_START 	(RMT_EXT_STATES_START)
#define 	NO_RMT_EXT_STATES			0

#endif
/**-----------------------------------------------------------------------**/
/**** EM_ON_RSUSIDE states *****/
/**-----------------------------------------------------------------------**/

#ifdef EM_ON_RSUSIDE

#define 	DTCCS_CONNECT_EM		(EM_RSUSIDE_STATES_START)
#define 	DTCCS_CONNECT_EM_DS		(EM_RSUSIDE_STATES_START+1)
#define 	EM_CONNECT_DTCCS		(EM_RSUSIDE_STATES_START+2)
#define 	EM_CONNECT_DTCCS_DR		(EM_RSUSIDE_STATES_START+3)

#define 	EM_RSUSIDE_STATES_END		(EM_RSUSIDE_STATES_START+3)
#define 	NO_EM_RSUSIDE_STATES		(EM_RSUSIDE_STATES_END - EM_RSUSIDE_STATES_START + 1)
#define 	RMT_EM_STATES_START		(EM_RSUSIDE_STATES_END+1)
#else
#define 	NO_EM_RSUSIDE_STATES		0
#define 	RMT_EM_STATES_START		(EM_RSUSIDE_STATES_START)
#endif
/**-----------------------------------------------------------------------**/
/**** RMT_EM states *****/
/**-----------------------------------------------------------------------**/

#ifdef RMT_EM

#define 	AWAIT_EM_LINK_DISCONNECT_ACK		(RMT_EM_STATES_START)
#define 	AWAIT_EM_LCL_BLK_LINK_DISCONNECT_ACK	(RMT_EM_STATES_START+1)
#define 	AWAIT_EM_RMT_BLK_LINK_DISCONNECT_ACK	(RMT_EM_STATES_START+2)

#define 	RMT_EM_STATES_END			(RMT_EM_STATES_START+2)
#define 	NO_RMT_EM_STATES			(RMT_EM_STATES_END - RMT_EM_STATES_START + 1)
#define 	EXT_RSUSIDE_STATES_START		(RMT_EM_STATES_END+1)
#else
#define 	NO_RMT_EM_STATES			0
#define 	EXT_RSUSIDE_STATES_START		(RMT_EM_STATES_START)
#endif

/**-----------------------------------------------------------------------**/
/**** EXT_ON_RSUSIDE states *****/
/**-----------------------------------------------------------------------**/

#if ((defined EXT_ON_RSUSIDE) || (defined CB_ON_RSUSIDE))
#define 	EXT_CONNECT_CCSV5		(EXT_RSUSIDE_STATES_START)
#define 	CCSV5_CONNECT_EXT		(EXT_RSUSIDE_STATES_START+1)
#define 	FLASH_CHECK			(EXT_RSUSIDE_STATES_START+2)
#define 	EXT_CONNECT_CCSV5_NO_DR 	(EXT_RSUSIDE_STATES_START+3)
#define 	EXT_PATH_CONNECT_FOR_OPR	(EXT_RSUSIDE_STATES_START+4)

#define 	EXT_RSUSIDE_STATES_END		(EXT_RSUSIDE_STATES_START+4)
#define 	NO_EXT_RSUSIDE_STATES		(EXT_RSUSIDE_STATES_END - EXT_RSUSIDE_STATES_START + 1)
#define 	RMT_CAS3_STATES_START		(EXT_RSUSIDE_STATES_END+1)
#else
#define 	NO_EXT_RSUSIDE_STATES		 0
#define 	RMT_CAS3_STATES_START		(EXT_RSUSIDE_STATES_START)
#endif

/**-----------------------------------------------------------------------**/
/**** RMT_CAS3 states *****/
/**-----------------------------------------------------------------------**/

#if ((defined RMT_CAS3) || (defined CAS3_ON_E1_PROCESSOR_EXIST))

#define 	AWAIT_CAS3_LINK_DISCONNECT_ACK		  (RMT_CAS3_STATES_START)
#define 	AWAIT_CAS3_LCL_BLK_LINK_DISCONNECT_ACK	  (RMT_CAS3_STATES_START+1)
#define 	AWAIT_CAS3_RMT_BLK_LINK_DISCONNECT_ACK	  (RMT_CAS3_STATES_START+2)

#define 	RMT_CAS3_STATES_END			  (RMT_CAS3_STATES_START+2)
#define 	NO_RMT_CAS3_STATES			  (RMT_CAS3_STATES_END - RMT_CAS3_STATES_START + 1)
#define 	CAS3_RSUSIDE_STATES_START		  (RMT_CAS3_STATES_END+1)
#else
#define 	NO_RMT_CAS3_STATES			  0
#define 	CAS3_RSUSIDE_STATES_START		  (RMT_CAS3_STATES_START)
#endif

/**-----------------------------------------------------------------------**/
/**** CAS3_ON_RSUSIDE states *****/
/**-----------------------------------------------------------------------**/

#if ((defined CAS3_ON_RSUSIDE) || (defined CAS3_ON_E1_PROCESSOR_SIDE))

#define 	DTCCS_CONNECT_CAS3		  (CAS3_RSUSIDE_STATES_START)
#define 	DTCCS_CONNECT_CAS3_DS		  (CAS3_RSUSIDE_STATES_START+1)
#define 	CAS3_CONNECT_DTCCS		  (CAS3_RSUSIDE_STATES_START+2)
#define 	CAS3_CONNECT_DTCCS_DR		  (CAS3_RSUSIDE_STATES_START+3)

#define 	CAS3_RSUSIDE_STATES_END 	  (CAS3_RSUSIDE_STATES_START+3)
#define 	NO_CAS3_RSUSIDE_STATES		  (CAS3_RSUSIDE_STATES_END - CAS3_RSUSIDE_STATES_START + 1)
#define 	ISUP_STATES_START		  (CAS3_RSUSIDE_STATES_END+1)
#else
#define 	NO_CAS3_RSUSIDE_STATES		  0
#define 	ISUP_STATES_START		  (CAS3_RSUSIDE_STATES_START)
#endif


#ifdef ISUP

#define ISUPI_WAIT_FOR_FURTHER_DIGITS_STATE	(ISUP_STATES_START+0)
#define ISUPI_EXT_RINGING_STATE 		(ISUP_STATES_START+1)
#define ISUPI_ANSWERED_STATE			(ISUP_STATES_START+2)
#define ISUPI_WAIT_FOR_RLC_STATE		(ISUP_STATES_START+3)
#define ISUPI_WAIT_FOR_REANSWER_STATE		(ISUP_STATES_START+4)
#define ISUPO_WAIT_FOR_DIGITS_STATE		(ISUP_STATES_START+5)
#define ISUPO_WAIT_FOR_ACM_STATE		(ISUP_STATES_START+6)
#define ISUPO_WAIT_FOR_ANM_STATE		(ISUP_STATES_START+7)
#define ISUPO_WAIT_FOR_RLC_STATE		(ISUP_STATES_START+8)
#define ISUPO_ANSWERED_STATE			(ISUP_STATES_START+9)
#define ISUPI_WAIT_FOR_CONTINUITY_CHECK_STATE	(ISUP_STATES_START+0xa)
#define ISUPO_WAIT_FOR_MORE_DIGITS_STATE	(ISUP_STATES_START+0xb)
#define HEAR_ANNOUNCEMENT			(ISUP_STATES_START+0xc)
#define ISUPI_FLASH_DETECTED_STATE		(ISUP_STATES_START+0xd)
#define ISUPO_FLASH_DETECTED_STATE		(ISUP_STATES_START+0xe)
#define ISUP_STATES_END 			(ISUP_STATES_START+0xe)
#define NO_ISUP_STATES				(ISUP_STATES_END - ISUP_STATES_START + 1)
#define LE_V51_STATES_START			(ISUP_STATES_END+1)
#else
#define NO_ISUP_STATES				0
#define LE_V51_STATES_START			(ISUP_STATES_START)
#endif
#ifdef LE_V51_INSTALLED
#define AWAIT_V51_LINK_DISCONNECT_COMPLETE	LE_V51_STATES_START

#define LE_V51_STATES_END			(LE_V51_STATES_START)
#define NO_LE_V51_STATES			(LE_V51_STATES_END - LE_V51_STATES_START + 1)
#define LE_V52_STATES_START			(LE_V51_STATES_END+1)
#else
#define NO_LE_V51_STATES			0
#define LE_V52_STATES_START			(LE_V51_STATES_START)
#endif
#ifdef LE_V52_INSTALLED
#define AWAIT_V52_LINK_DISCONNECT_COMPLETE	LE_V52_STATES_START

#define LE_V52_STATES_END			(LE_V52_STATES_START)
#define NO_LE_V52_STATES			(LE_V52_STATES_END - LE_V52_STATES_START + 1)
#define ABIS_STATES_START			(LE_V52_STATES_END + 1)
#else
#define NO_LE_V52_STATES			0
#define ABIS_STATES_START			(LE_V52_STATES_START)
#endif
#ifdef GSM_INSTALLED
#define AWAIT_ABIS_RELEASE_STATE		(ABIS_STATES_START + 0x00)
#define AWAIT_ABIS_RELEASE_COMPLETE_STATE	(ABIS_STATES_START + 0x01)
#define AWAIT_ABIS_DISCONNECT_STATE		(ABIS_STATES_START + 0x02)
#define DUMMY_ABIS_HOLD_STATE			(ABIS_STATES_START + 0x03)
#define DUMMY_AWAIT_ABIS_RELEASE_STATE		(ABIS_STATES_START + 0x04)
#define DUMMY_AWAIT_ABIS_RELEASE_COMPLETE_STATE (ABIS_STATES_START + 0x05)

#define ABIS_STATES_END 			(ABIS_STATES_START + 0x05)
#define NO_ABIS_STATES				(ABIS_STATES_END - ABIS_STATES_START + 1)
#define NET_ISDN_STATES_START			(ABIS_STATES_END + 1)
#else
#define NO_ABIS_STATES				0
#define NET_ISDN_STATES_START			(ABIS_STATES_START)
#endif
#ifdef NET_ISDN_INSTALLED
#define AWAIT_NET_ISDN_RELEASE_STATE		    (NET_ISDN_STATES_START + 0x00)
#define AWAIT_NET_ISDN_RELEASE_COMPLETE_STATE	    (NET_ISDN_STATES_START + 0x01)
#define AWAIT_NET_ISDN_DISCONNECT_STATE 	    (NET_ISDN_STATES_START + 0x02)
#define AWAIT_ISDNO_CALL_PROCEEDING_STATE	    (NET_ISDN_STATES_START + 0x03)
#define NET_ISDN_STATES_END			    (NET_ISDN_STATES_START + 0x03)
#define NO_NET_ISDN_STATES			    (NET_ISDN_STATES_END - NET_ISDN_STATES_START + 1)
#define USERSIDE_PRIMARY_ISDN_STATES_START	    (NET_ISDN_STATES_END + 1)
#else
#define NO_NET_ISDN_STATES			    0
#define USERSIDE_PRIMARY_ISDN_STATES_START	    (NET_ISDN_STATES_START)
#endif
#ifdef USERSIDE_PRIMARY_ISDN_INSTALLED
#define USERSIDE_AWAIT_ISDNO_DIGITS_STATE		(USERSIDE_PRIMARY_ISDN_STATES_START + 0x00)
#define USERSIDE_AWAIT_ISDNO_CALL_PROCEEDING_STATE	(USERSIDE_PRIMARY_ISDN_STATES_START + 0x01)
#define USERSIDE_AWAIT_ISDN_RELEASE_COMPLETE_STATE	(USERSIDE_PRIMARY_ISDN_STATES_START + 0x02)
#define USERSIDE_AWAIT_ISDN_DISCONNECT_STATE		(USERSIDE_PRIMARY_ISDN_STATES_START + 0x03)
#define USERSIDE_AWAIT_ISDN_RELEASE_STATE		(USERSIDE_PRIMARY_ISDN_STATES_START + 0x04)

#define USERSIDE_PRIMARY_ISDN_STATES_END		(USERSIDE_PRIMARY_ISDN_STATES_START + 0x04)

#define NO_USERSIDE_PRIMARY_ISDN_STATES 		(USERSIDE_PRIMARY_ISDN_STATES_END - USERSIDE_PRIMARY_ISDN_STATES_START + 1)
#define H225_CC_STATES_START				(USERSIDE_PRIMARY_ISDN_STATES_END+1)
#else
#define NO_USERSIDE_PRIMARY_ISDN_STATES 		0
#define H225_CC_STATES_START				USERSIDE_PRIMARY_ISDN_STATES_START
#endif
#ifdef H225_CC_INSTALLED
#define H225_CCO_AWAIT_DIGITS_STATE			(H225_CC_STATES_START + 0x00)
#define H225_CCO_AWAIT_CALL_PROCEEDING_STATE		(H225_CC_STATES_START + 0x01)

#define H225_CC_STATES_END				(H225_CC_STATES_START + 0x01)

#define NO_H225_CC_STATES				(H225_CC_STATES_END - H225_CC_STATES_START + 1)
#define H248_MGCSIDE_CP_EXT_STATES_START		(H225_CC_STATES_END + 1)
#else
#define NO_H225_CC_STATES				0
#define H248_MGCSIDE_CP_EXT_STATES_START		H225_CC_STATES_START
#endif // H225_CC_INSTALLED

#ifdef EXT_ON_H248_MGCSIDE
#define AWAIT_H248_MGCSIDE_DEALLOCATION_COMPLETE_STATE	(H248_MGCSIDE_CP_EXT_STATES_START + 0x00)

#define H248_MGCSIDE_CP_EXT_STATES_END			(H248_MGCSIDE_CP_EXT_STATES_START + 0x00)
#define NO_H248_MGCSIDE_CP_EXT_STATES			(H248_MGCSIDE_CP_EXT_STATES_END - H248_MGCSIDE_CP_EXT_STATES_START + 1)
#define SIP_CP_STATES_START				(H248_MGCSIDE_CP_EXT_STATES_END + 1)
#else
#define NO_H248_MGCSIDE_CP_EXT_STATES			0
#define SIP_CP_STATES_START				H248_MGCSIDE_CP_EXT_STATES_START
#endif

#ifdef SIP_INSTALLED
#define SIPO_CP_WTFR_DIGITS_STATE				(SIP_CP_STATES_START+0x00)
#define SIPO_CP_WTFR_RTPMAN_ADD_LOCAL_PARTY_CONFIRM_STATE	(SIP_CP_STATES_START+0x01)
#define SIPO_CP_WTFR_RINGING_STATE				(SIP_CP_STATES_START+0x02)
#define SIPO_CP_WTFR_INVITE_OK_STATE				(SIP_CP_STATES_START+0x03)
#define SIPO_CP_ANSWERED_STATE					(SIP_CP_STATES_START+0x04)
#define SIPO_CP_FLASH_DETECTED_STATE				(SIP_CP_STATES_START+0x05)
#define SIPO_CP_WTFR_BYE_OK_STATE				(SIP_CP_STATES_START+0x06)
#define SIPO_CP_WTFR_INVITE_TERMINATION_CANCEL_OK_STATE 	(SIP_CP_STATES_START+0x07)
#define SIPO_CP_WTFR_CANCEL_OK_STATE				(SIP_CP_STATES_START+0x08)
#define SIPO_CP_WTFR_INVITE_TERMINATION_STATE			(SIP_CP_STATES_START+0x09)
#define SIPI_CP_WTFR_ANSWER_STATE				(SIP_CP_STATES_START+0x0a)
#define SIPI_CP_ANSWERED_STATE					(SIP_CP_STATES_START+0x0b)
#define SIPI_CP_FLASH_DETECTED_STATE				(SIP_CP_STATES_START+0x0c)
#define SIPI_CP_WTFR_REANSWER_STATE				(SIP_CP_STATES_START+0x0d)
#define SIPI_CP_WTFR_BYE_OK_STATE				(SIP_CP_STATES_START+0x0e)
#define SIPI_CP_WTFR_ACK_FOR_NON_2XX_RESPONSE_STATE		(SIP_CP_STATES_START+0x0f)

#define SIP_CP_STATES_END					(SIP_CP_STATES_START+0x0f)
#define NO_SIP_CP_STATES					(SIP_CP_STATES_END - SIP_CP_STATES_START + 1)
#define OPR_USERSIDE_PRIMARY_ISDN_STATES_START			(SIP_CP_STATES_END + 1)
#else
#define NO_SIP_CP_STATES					0
#define OPR_USERSIDE_PRIMARY_ISDN_STATES_START			(SIP_CP_STATES_START)
#endif
#if ((defined USERSIDE_PRIMARY_ISDN_INSTALLED) && (defined OPR_PRI_CONNECTION))

#define OPR_AWAIT_USERSIDE_ISDNO_DIGITS_STATE			(OPR_USERSIDE_PRIMARY_ISDN_STATES_START + 0x00)
#define OPR_AWAIT_USERSIDE_ISDNO_CALL_PROCEEDING_STATE		(OPR_USERSIDE_PRIMARY_ISDN_STATES_START + 0x01)

#define OPR_USERSIDE_PRIMARY_ISDN_STATES_END			(OPR_USERSIDE_PRIMARY_ISDN_STATES_START + 0x01)
#define NO_OPR_USERSIDE_PRIMARY_ISDN_STATES			(OPR_USERSIDE_PRIMARY_ISDN_STATES_END - OPR_USERSIDE_PRIMARY_ISDN_STATES_START + 1)
#define H248_MGCSIDE_CP_ISUP_STATES_START			   (OPR_USERSIDE_PRIMARY_ISDN_STATES_END + 1)
#else
#define NO_OPR_USERSIDE_PRIMARY_ISDN_STATES			0
#define H248_MGCSIDE_CP_ISUP_STATES_START			   OPR_USERSIDE_PRIMARY_ISDN_STATES_START
#endif

#if ((defined ISUP_ON_H248_MGCSIDE) && \
     (defined ISUP_ON_H248_MGCSIDE_WAIT_FOR_DEALLOCATION_COMPLETE_TO_BECOME_IDLE))
#define ISUP_ON_H248_MGCSIDE_ICT_WTFR_DEALLOCATION_COMPLETE_SEND_RLC (H248_MGCSIDE_CP_ISUP_STATES_START + 0x00)
#define ISUP_ON_H248_MGCSIDE_ICT_WTFR_DEALLOCATION_COMPLETE_SEND_REL (H248_MGCSIDE_CP_ISUP_STATES_START + 0x01)
#define ISUP_ON_H248_MGCSIDE_OGT_WTFR_DEALLOCATION_COMPLETE_SEND_RLC (H248_MGCSIDE_CP_ISUP_STATES_START + 0x02)
#define ISUP_ON_H248_MGCSIDE_OGT_WTFR_DEALLOCATION_COMPLETE_SEND_REL (H248_MGCSIDE_CP_ISUP_STATES_START + 0x03)

#define H248_MGCSIDE_CP_ISUP_STATES_END 		(H248_MGCSIDE_CP_ISUP_STATES_START + 0x03)
#define NO_H248_MGCSIDE_CP_ISUP_STATES			(H248_MGCSIDE_CP_ISUP_STATES_END - H248_MGCSIDE_CP_ISUP_STATES_START + 1)
#define next_whatever_STATES_START			(H248_MGCSIDE_CP_ISUP_STATES_END + 1)
#else
#define NO_H248_MGCSIDE_CP_ISUP_STATES			    0
#define next_whatever_STATES_START			H248_MGCSIDE_CP_ISUP_STATES_START
#endif


/**-----------------------------------------------------------------------**/
#define  NO_STATES  (NO_GLOBAL_STATES + NO_EM_STATES + NO_CAS3_STATES +     \
		     NO_CONFERENCE_STATES + NO_AMPLIFIER_STATE +	    \
		     NO_SIREN_STATE + NO_LD_STATES + NO_RMT_EXT_STATES +    \
		     NO_CHANNEL_TEST_STATES + NO_EM_RSUSIDE_STATES+	    \
		     NO_RMT_EM_STATES + NO_EXT_RSUSIDE_STATES + 	    \
		     NO_RMT_CAS3_STATES + NO_CAS3_RSUSIDE_STATES +	    \
		     NO_ISUP_STATES + NO_LE_V51_STATES + NO_LE_V52_STATES + \
		     NO_ABIS_STATES + NO_NET_ISDN_STATES +		    \
		     NO_USERSIDE_PRIMARY_ISDN_STATES + NO_H225_CC_STATES +  \
		     NO_H248_MGCSIDE_CP_EXT_STATES + NO_SIP_CP_STATES +     \
		     NO_OPR_USERSIDE_PRIMARY_ISDN_STATES +		    \
		     NO_H248_MGCSIDE_CP_ISUP_STATES)
/**-----------------------------------------------------------------------**/
#if (NO_STATES >= 0xf0)
#error NO_STATES >= 0xf0
#endif
		/*****		stimulus definition		*****/
#define  OPRSIDE_CLID		0x67

#define  OFF_HOOK		0
#define  ON_HOOK		1
#define  ABONDON		2
#define  AT_RING		ABONDON
/* because ABONDON is not used for ext. at operator,I use it for */
/* status display instead of defining new stimulus for RING state */
#define  TIME_OUT		3
#ifdef CONFERENCE_ACT
#define  TIME_OUT2		4	// this stim is only used in CONFERENCE
					// state, no need to allocate extra code
					// use a code like that of REJECT which
					// never happens in CONFERENCE state,
					// this method is called (by me!) "manual
					// intelligent object oriented programming"!
#endif

#define  OPR_REJECT		4
#define  CNNCT_TO_OPR		5
#define  NO_MATCH		6
#define STILL_UNKNOWN_STIM	0xfe
#define  EXT_PICKED_UP		NO_MATCH
#define  INTERNAL		7
#define  OPR_REQ		8
#define  RING_DTCT		9
#define  EM_CONT_OPR_REQ	RING_DTCT
#define  CAS3_OPR_REQ		RING_DTCT
#define  RECALL 		0xa
#define  TRNK_FREE		0xb
#define  GROUP_BUSY		0xc
#define  CO_GROUP_REQ		0xd
#define  SRC_EXT_REQ		0xe
#define  DST_EXT_REQ		0xf
#define  ATT_RLS		0x10
/* also used for camp on connected party of operator to a busy ext. */
#define  RLS			0x11
#define  OVRD			0x12
#define  MALICIOUS_PARTY	OVRD
	/* because OVRD stimulus is not an incoming stimulus at operator */
	/* side,I use it for malicious party report to operator */
#define  HOLD_XT		0x13
#define  PARK_XT		0x14
#define  TIE_GROUP_REQ		0x15
#define  RING_DISCNT		0x16
#define  EXT_FREE		0x17
#define  EXT_FORWARDED		EXT_FREE
#define  EXT_BUSY		0x18
#define  TRNK_RLS		0x19
#define  MUSIC			0x1a
#define  TONE_TO_OPR		0x1b
#define  AT_LOCK		TONE_TO_OPR
/* because TONE_TO_OPR is not used for ext. at operator as a stimulus,I use it for */
/* status display instead of defining new stimulus for LOCK state */
#define  STAR			0x1c
#define  HASH			0x1d
#define  NIGHT_ACT		0x1e
	/* if chagned,update operator header file */
#define  NIGHT_INACT		0x1f
	/* if chagned,update operator header file */

#define  OPERATOR		0x20
#define  CO_TRNK_REQ		0x21
#define  TIE_TRNK_REQ		0x22
#define  RMV_TONE		0x23
#define  DGT_END		0x24
#define  SERVICE		0x25
#define  CO_TRNK		0x26
#define  TIE_TRNK		0x27
#define  CLASS_VIOL		0x28
#define  EM_PULSE_RQST		0x29
#define  GROUND 		0x2a
#define  OPEN			0x2b
#define  EM_GROUP_REQ		0x2c
	/* if chagned,update operator header file */
#define  NUM			0x2d
	/* if chagned,update operator header file */
#define  X_STATUS_REQ_ACT	0x2e
	/* if chagned,update operator header file */
#define  X_STATUS_REQ_INACT	0x2f
	/* if chagned,update operator header file */
#define  SERIAL_TRANSFER	0x30
	/* if chagned,update operator header file */
#define  FORCE_RELEASE		0x31
	/* if chagned,update operator header file */
#define  OPR_DISC		0x32
	/* if changed,update operator header file */
#define  CNNCT_TO_OPR_EM_CONT	0x33
	/* if changed,update operator header file */

#define  EM_CONT_RQST		0x34

#define  CNNCT_TO_OPR_CAS3	0x35
	/* if changed,update operator header file */

#define CAS1_RQST		0x36

#define  EM_STATUS_REQ_ACT	0x37
	/* if chagned,update operator header file */
#define  EM_STATUS_REQ_INACT	0x38
	/* if chagned,update operator header file */
#define  CAS3_STATUS_REQ_ACT	0x39
	/* if chagned,update operator header file */
#define  CAS3_STATUS_REQ_INACT	0x3a
	/* if chagned,update operator header file */
#define  CAS1_STATUS_REQ_ACT	0x3b
	/* if chagned,update operator header file */
#define  CAS1_STATUS_REQ_INACT	0x3c
	/* if chagned,update operator header file */
// added 20/9/77
#define  SPECIAL_SERVICE_CODE	0x3d
#ifdef BAD_PAYER
// added on 20/3/78
#define  BAD_PAYER_REJECT	0x3e
#endif

#define CAS3_GROUP_REQ		0x3f
//????? start
#ifdef OPR_PRI_CONNECTION
#define  PRI_GROUP_REQ		0x3f
#endif
#ifdef OPR_SIP_CONNECTION
#define  SIP_GROUP_REQ		EM_GROUP_REQ
#endif
// end
	/* if changed,update operator header file */

#if ((defined OPR_OPR_TALK) && (defined SYSTEM_SIDE))
#define SRC_OPR_REQ		0x40   // in omtfp: 0x58, offset=0x58-0x3f
#define DST_OPR_REQ		0x41   // in omtfp: 0x59
#define OPR_FREE		0x42   // in omtfp: 0x5a
#define OPR_BUSY		0x43   // in omtfp: 0x5b
#define OPR_HOLD		0x44   // in omtfp: 0x5c     /* hold extension,trunk */
#define OPR_DEST_READY		0x45   // in omtfp: 0x5d
#define OPR_ON_HOOK		0x46   // in omtfp: 0x5e
#define OPR_CNNCT_TO_OPR	0x47   // in omtfp: 0x5f
#define CNNCT_TO_HOLD_OPR	0x48   // in omtfp: 0x60
#define SRC_CNNCTED		0x49   // in omtfp: 0x61
#endif

#ifdef OPR_OPR_TALK
#define  NO_GLOBAL_STIMS	0x4a
#else
#define  NO_GLOBAL_STIMS	0x40
#endif

#define  CAS3_STIMS_START	NO_GLOBAL_STIMS

/**-----------------------------------------------------------------------**/
#ifdef CAS3_INSTALLED

// CAS3_STIM2PKT start
#define  CEPT_STIM_1		CAS3_STIMS_START
#define  CEPT_STIM_3		(CAS3_STIMS_START + 1)
#define  CEPT_STIM_5		(CAS3_STIMS_START + 2)
#define  CEPT_STIM_7		(CAS3_STIMS_START + 3)
#define  CEPT_STIM_9		(CAS3_STIMS_START + 4)
#define  CEPT_STIM_D		(CAS3_STIMS_START + 5)
#define  CEPT_STIM_F		(CAS3_STIMS_START + 6)
#define  CEPT_STIM_EOS		(CAS3_STIMS_START + 7)
#define  CEPT_STIM_FAS		(CAS3_STIMS_START + 8)
#define  CEPT_STIM_METERING	(CAS3_STIMS_START + 9)
#define  CEPT_STIM_UNKNOWN	(CAS3_STIMS_START + 0xa)
//CAS3_STIM2PKT end

// in the following lines the value of stims are changed.
#define  ICDT_LOCAL_BLOCK	(CAS3_STIMS_START + 0xb)
#define  CAS3_RQST		(CAS3_STIMS_START + 0xc)
#define  DT_OG_LOCAL_BLOCK	(CAS3_STIMS_START + 0xd)
#define  DT_LOCAL_UNBLOCK	(CAS3_STIMS_START + 0xe)
#define  CAS3_STIMS_END 	(CAS3_STIMS_START + 0x0e) /* it was 0x0a! */
#define  NO_CAS3_STIMS		(CAS3_STIMS_END - CAS3_STIMS_START + 1)
#else
#define  CAS3_STIMS_END 	(CAS3_STIMS_START-1)
#define  NO_CAS3_STIMS		0
#endif
//...............................
#if ((defined CAS3_ON_RSUSIDE) || (defined CAS3_ON_E1_PROCESSOR_SIDE))
#define CAS3_STIM2PKT(x)		cas3_stim2pkt[x-CAS3_STIMS_START]
#define MAX_CAS3_STIM2PKT_ARR_SIZE	11
#endif
//................................
/**-----------------------------------------------------------------------**/
/**-----------------------------------------------------------------------**/
#define CAS1_STIMS_START	(CAS3_STIMS_END+1)

#ifdef CAS1_INSTALLED
#define DT_GROUND		CAS1_STIMS_START
#define DT_OPEN 		(CAS1_STIMS_START + 1)

#define CAS1_STIMS_END		(CAS1_STIMS_START + 1)
#define  NO_CAS1_STIMS		(CAS1_STIMS_END - CAS1_STIMS_START + 1)

#else
#define CAS1_STIMS_END		(CAS1_STIMS_START-1)
#define  NO_CAS1_STIMS		0
#endif

#if ( (defined CAS3_INSTALLED) || (defined CAS1_INSTALLED) || (defined EM_PULSE_SIGNALING))
#define  DGT_SEND		(CAS1_STIMS_END + 1)

#define  CEPT_STIMS_END 	(CAS1_STIMS_END + 1)
#define  NO_CEPT_STIMS		1
#else
#define  CEPT_STIMS_END 	(CAS1_STIMS_END)
#define  NO_CEPT_STIMS		0
#endif
/**-----------------------------------------------------------------------**/

#define LD_STIMS_START	      (CEPT_STIMS_END+1)

#ifdef LD_INSTALLED
#define 	LD_RQST 		(LD_STIMS_START + 0)
#define 	LD_NO_RMT_BIAS		(LD_STIMS_START + 1)
#define 	LD_RMT_REVERSE_BIAS	(LD_STIMS_START + 2)
#define 	LD_RMT_FORWARD_BIAS	(LD_STIMS_START + 3)
#define 	LD_ON_HOOK		(LD_STIMS_START + 4)
#define 	LD_OFF_HOOK		(LD_STIMS_START + 5)

#define 	LD_STIMS_END		(LD_STIMS_START + 5)
#define 	NO_LD_STIMS		(LD_STIMS_END - LD_STIMS_START + 1)

#else
#define 	LD_STIMS_END		(LD_STIMS_START-1)
#define 	NO_LD_STIMS		 0
#endif

/**-----------------------------------------------------------------------**/
/* RMT_EXT stims							   */
/**-----------------------------------------------------------------------**/
#define 	RMT_EXT_STIMS_START		(LD_STIMS_END+1)

#if ((defined RMT_EXT) || (defined RMT_CB))
#define 	RSU_OFF_HOOK_NO_CH		(RMT_EXT_STIMS_START + 0)
#define 	EXT_LINK_DISCONNECT_ACK 	(RMT_EXT_STIMS_START + 1)
#define 	EXT_DISCONNECT			(RMT_EXT_STIMS_START + 2)
#define 	EXT_DISCONNECT_IND_STIM 	(RMT_EXT_STIMS_START + 3)
#define 	EXT_ALERTING_STIM		(RMT_EXT_STIMS_START + 4)

#define 	RMT_EXT_STIMS_END		(RMT_EXT_STIMS_START + 4)
#define 	NO_RMT_EXT_STIMS		(RMT_EXT_STIMS_END - RMT_EXT_STIMS_START + 1)

#else
#define 	RMT_EXT_STIMS_END		(RMT_EXT_STIMS_START-1)
#define 	NO_RMT_EXT_STIMS		 0
#endif
/**-----------------------------------------------------------------------**/

/**-----------------------------------------------------------------------**/
#define 	CHANNEL_TEST_STIMS_START	 (RMT_EXT_STIMS_END+1)

#ifdef CHANNEL_TEST
#define 	CHANNEL_TEST_RQST	(CHANNEL_TEST_STIMS_START + 0)

#define 	CHANNEL_TEST_STIMS_END	(CHANNEL_TEST_STIMS_START + 0)
#define 	NO_CHANNEL_TEST_STIMS	(CHANNEL_TEST_STIMS_END - CHANNEL_TEST_STIMS_START + 1)

#else
#define 	CHANNEL_TEST_STIMS_END	(CHANNEL_TEST_STIMS_START-1)
#define 	NO_CHANNEL_TEST_STIMS	0
#endif
/**-----------------------------------------------------------------------**/
/* EM_ON_RSUSIDE stims							   */
/**-----------------------------------------------------------------------**/
#define 	EM_RSUSIDE_STIMS_START		(CHANNEL_TEST_STIMS_END+1)

#ifdef EM_ON_RSUSIDE

// em_stim2cnt array
#define 	SEIZE_PCKT_STIM 		EM_RSUSIDE_STIMS_START
#define 	SZACK_PCKT_STIM 		(EM_RSUSIDE_STIMS_START+1)
#define 	CLF_PCKT_STIM			(EM_RSUSIDE_STIMS_START+2)
#define 	EOS_PCKT_STIM			(EM_RSUSIDE_STIMS_START+3)
#define 	ANSWER_PCKT_STIM		(EM_RSUSIDE_STIMS_START+4)
#define 	CONGESTION_PCKT_STIM		(EM_RSUSIDE_STIMS_START+5)
#define 	METER_PCKT_STIM 		(EM_RSUSIDE_STIMS_START+6)
#define 	CBK_PCKT_STIM			(EM_RSUSIDE_STIMS_START+7)
#define 	RLG_PCKT_STIM			(EM_RSUSIDE_STIMS_START+8)
#define 	FAS_PCKT_STIM			(EM_RSUSIDE_STIMS_START+9)
#define 	BLK_MCI_PCKT_STIM		(EM_RSUSIDE_STIMS_START+0xa)
#define 	IDLE_RLS_PCKT_STIM		(EM_RSUSIDE_STIMS_START+0xb)

#define 	DGT1_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0xc)
#define 	DGT2_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0xd)
#define 	DGT3_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0xe)
#define 	DGT4_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0xf)
#define 	DGT5_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0x10)
#define 	DGT6_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0x11)
#define 	DGT7_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0x12)
#define 	DGT8_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0x13)
#define 	DGT9_PCKT_STIM			(EM_RSUSIDE_STIMS_START+0x14)
#define 	DGT10_PCKT_STIM 		(EM_RSUSIDE_STIMS_START+0x15)

#define 	FIRST_STIM2CNT_ARR_INDEX	SEIZE_PCKT_STIM
#define 	MAX_EM_STIM_2_COUNT		22

// end em_stim2cnt array

// em_stim2pkt array
#define 	SEIZE_SGNL_STIM 		     (EM_RSUSIDE_STIMS_START+0x16)
#define 	METER_EOS_SGNL_STIM		     (EM_RSUSIDE_STIMS_START+0x17)
#define 	SZACK_METER_RLG_EOS_FAS_SGNL_STIM    (EM_RSUSIDE_STIMS_START+0x18)
#define 	SZACK_METER_RLG_FAS_SGNL_STIM	     (EM_RSUSIDE_STIMS_START+0x19)
#define 	SZACK_RLG_FAS_SGNL_STIM 	     (EM_RSUSIDE_STIMS_START+0x1a)
#define 	SZACK_RLG_SGNL_STIM		     (EM_RSUSIDE_STIMS_START+0x1b)
#define 	CBK_ANS_SGNL_STIM		     (EM_RSUSIDE_STIMS_START+0x1c)
#define 	CLF_CONGESTION_SGNL_STIM	     (EM_RSUSIDE_STIMS_START+0x1d)
#define 	BLK_MCI_SGNL_STIM		     (EM_RSUSIDE_STIMS_START+0x1e)
#define 	IDLE_RLS_SGNL_STIM		     (EM_RSUSIDE_STIMS_START+0x1f)
#define 	EM_UNKNOWN_SGNL_STIM		     (EM_RSUSIDE_STIMS_START+0x20)

#define 	FIRST_STIM2PKT_ARR_INDEX	     SEIZE_SGNL_STIM
#define 	MAX_EM_STIM_2_PCKT		     11

// end em_stim2pkt  array

#define 	EM_LINK_DISCONNECT_PCKT_STIM	     (EM_RSUSIDE_STIMS_START+0x21)
#define 	EM_RLS_DGT_SNDR_PCKT_STIM	     (EM_RSUSIDE_STIMS_START+0x22)

#define 	EM_RSUSIDE_STIMS_END		     (EM_RSUSIDE_STIMS_START + 0x22)
#define 	NO_EM_RSUSIDE_STIMS		     (EM_RSUSIDE_STIMS_END - EM_RSUSIDE_STIMS_START + 1)
#else  // EM_ON_RSUSIDE
#define 	EM_RSUSIDE_STIMS_END		     (EM_RSUSIDE_STIMS_START-1)
#define 	NO_EM_RSUSIDE_STIMS		     0
#endif // EM_ON_RSUSIDE

/**-----------------------------------------------------------------------**/
/* RMT_EM stims 							   */
/**-----------------------------------------------------------------------**/
#define 	RMT_EM_STIMS_START		     (EM_RSUSIDE_STIMS_END+1)
#ifdef RMT_EM

#define 	SEIZE_SGNL_STIM 		     (RMT_EM_STIMS_START+0x1)
#define 	METER_EOS_SGNL_STIM		     (RMT_EM_STIMS_START+0x2)
#define 	SZACK_METER_RLG_EOS_FAS_SGNL_STIM    (RMT_EM_STIMS_START+0x3)
#define 	SZACK_METER_RLG_FAS_SGNL_STIM	     (RMT_EM_STIMS_START+0x4)
#define 	SZACK_RLG_FAS_SGNL_STIM 	     (RMT_EM_STIMS_START+0x5)
#define 	SZACK_RLG_SGNL_STIM		     (RMT_EM_STIMS_START+0x6)
#define 	CBK_ANS_SGNL_STIM		     (RMT_EM_STIMS_START+0x7)
#define 	CLF_CONGESTION_SGNL_STIM	     (RMT_EM_STIMS_START+0x8)
#define 	BLK_MCI_SGNL_STIM		     (RMT_EM_STIMS_START+0x9)
#define 	IDLE_RLS_SGNL_STIM		     (RMT_EM_STIMS_START+0xa)

#define 	EM_LINK_DISCONNECT_ACK_STIM	     (RMT_EM_STIMS_START+0xb)
#define 	EM_UNKNOWN_SGNL_STIM		     (RMT_EM_STIMS_START+0xc)
#define 	EM_CALL_FAILURE_STIM		     (RMT_EM_STIMS_START+0xd)
#define 	SEIZE_NO_CH_STIM		     (RMT_EM_STIMS_START+0xe)

#define 	RMT_EM_STIMS_END		     (RMT_EM_STIMS_START + 0xe)
#define 	NO_RMT_EM_STIMS 		     (RMT_EM_STIMS_END - RMT_EM_STIMS_START + 1)
#else  // EM_ON_RSUSIDE
#define 	RMT_EM_STIMS_END		     (RMT_EM_STIMS_START-1)
#define 	NO_RMT_EM_STIMS 	     0
#endif // EM_ON_RSUSIDE

/**-----------------------------------------------------------------------**/
/* EXT_ON_RSUSIDE stims 							   */
/**-----------------------------------------------------------------------**/
#define 	EXT_RSUSIDE_STIMS_START 	(RMT_EM_STIMS_END+1)

#if ((defined EXT_ON_RSUSIDE) || (defined CB_ON_RSUSIDE))
#define 	RING_ON 			(EXT_RSUSIDE_STIMS_START+0x1)
#define 	RING_OFF			(EXT_RSUSIDE_STIMS_START+0x2)
#define 	EXT_LINK_DISCONNECT		(EXT_RSUSIDE_STIMS_START+0x3)
//#define	CONTENTION			(EXT_RSUSIDE_STIMS_START+0x4)
#define 	EXT_RLS_DGT_RCVR		(EXT_RSUSIDE_STIMS_START+0x5)
#define 	CB_METERING_PULSE		(EXT_RSUSIDE_STIMS_START+0x6)
#define 	RING_ON_R1			(EXT_RSUSIDE_STIMS_START+0x7)
//#define	  RECONNECT_VOICE_PATH		  (EXT_RSUSIDE_STIMS_START+0x8)
#define 	CONNECT_VOICE_PATH		(EXT_RSUSIDE_STIMS_START+0x9)
#define 	CONNECT_VOICE_PATH_FOR_OPR	(EXT_RSUSIDE_STIMS_START+0xa)

#define 	EXT_RSUSIDE_STIMS_END		(EXT_RSUSIDE_STIMS_START + 0xa)
#define 	NO_EXT_RSUSIDE_STIMS		(EXT_RSUSIDE_STIMS_END - EXT_RSUSIDE_STIMS_START + 1)

#else  // EXT_ON_RSUSIDE || CB_ON_RSUSIDE

#define 	EXT_RSUSIDE_STIMS_END		(EXT_RSUSIDE_STIMS_START-1)
#define 	NO_EXT_RSUSIDE_STIMS		 0

#endif // EXT_ON_RSUSIDE || CB_ON_RSUSIDE

/**-----------------------------------------------------------------------**/
/* RSUSIDE stims							    */
/**-----------------------------------------------------------------------**/
#define 	RSUSIDE_STIMS_START	    (EXT_RSUSIDE_STIMS_END+1)

#ifdef RSUSIDE
#define 	CONTENTION			(RSUSIDE_STIMS_START+0x1)

#define 	RSUSIDE_STIMS_END		(RSUSIDE_STIMS_START + 0x1)
#define 	NO_RSUSIDE_STIMS		(RSUSIDE_STIMS_END - RSUSIDE_STIMS_START + 1)
#else  // RSUSIDE

#define 	RSUSIDE_STIMS_END		(RSUSIDE_STIMS_START-1)
#define 	NO_RSUSIDE_STIMS		0

#endif // RSUSIDE

/**-----------------------------------------------------------------------**/
/* RMT_CAS3 , CAS3_ON_E1_PROCESSOR_EXIST stims				   */
/**-----------------------------------------------------------------------**/
#define 	RMT_CAS3_STIMS_START		(RSUSIDE_STIMS_END+1)

#if ((defined RMT_CAS3) || (defined CAS3_ON_E1_PROCESSOR_EXIST))

#define 	CAS3_LINK_DISCONNECT_ACK_STIM	(RMT_CAS3_STIMS_START+ 1)
#define 	CAS3_CALL_FAILURE_STIM		(RMT_CAS3_STIMS_START+ 2)
#define 	CAS3_SEIZE_NO_CH_STIM		(RMT_CAS3_STIMS_START+ 3)

#define 	RMT_CAS3_STIMS_END		(RMT_CAS3_STIMS_START + 3)
#define 	NO_RMT_CAS3_STIMS		(RMT_CAS3_STIMS_END - RMT_CAS3_STIMS_START + 1)

#else  // RMT_CAS3 || CAS3_ON_E1_PROCESSOR_EXIST

#define 	RMT_CAS3_STIMS_END		(RMT_CAS3_STIMS_START-1)
#define 	NO_RMT_CAS3_STIMS		0

#endif // RMT_CAS3 || CAS3_ON_E1_PROCESSOR_EXIST

/**-----------------------------------------------------------------------**/
/* CAS3_ON_RSUSIDE , CAS3_ON_E1_PROCESSOR_SIDE stims			   */
/**-----------------------------------------------------------------------**/
#define 	RSUSIDE_CAS3_STIMS_START	    (RMT_CAS3_STIMS_END+1)

#if ((defined CAS3_ON_RSUSIDE) || (defined CAS3_ON_E1_PROCESSOR_SIDE))
//CAS3_STIM2SGNL array start
#define CAS3_DGT1_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x1)
#define CAS3_DGT2_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x2)
#define CAS3_DGT3_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x3)
#define CAS3_DGT4_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x4)
#define CAS3_DGT5_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x5)
#define CAS3_DGT6_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x6)
#define CAS3_DGT7_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x7)
#define CAS3_DGT8_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x8)
#define CAS3_DGT9_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x9)
#define CAS3_DGT10_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0xa)
#define CAS3_IDLE_CLF_RLG_PCKT_STIM		(RSUSIDE_CAS3_STIMS_START+0xb)
#define CAS3_SEIZE_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0xc)
#define CAS3_MALICIOUS_PCKT_STIM		(RSUSIDE_CAS3_STIMS_START+0xd)
#define CAS3_CONGESTION_PCKT_STIM		(RSUSIDE_CAS3_STIMS_START+0xe)
#define CAS3_SZACK_BLK_CLB_PCKT_STIM		(RSUSIDE_CAS3_STIMS_START+0xf)
#define CAS3_ANSWER_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x10)
#define CAS3_EOS_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x11)
#define CAS3_METERING_PCKT_STIM 		(RSUSIDE_CAS3_STIMS_START+0x12)
#define CAS3_FAS_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x13)

//CAS3_STIM2SGNL array end

#define CAS3_LINK_DISCONNECT_PCKT_STIM		(RSUSIDE_CAS3_STIMS_START+0x14)
#define CAS3_RLS_DGT_SNDR_PCKT_STIM		(RSUSIDE_CAS3_STIMS_START+0x15)
#define CAS3_UNKNOWN_PCKT_STIM			(RSUSIDE_CAS3_STIMS_START+0x16)

#define RSUSIDE_CAS3_STIMS_END			(RSUSIDE_CAS3_STIMS_START + 0x16)
#define NO_RSUSIDE_CAS3_STIMS			(RSUSIDE_CAS3_STIMS_END - RSUSIDE_CAS3_STIMS_START + 1)

//.................................
#define CAS3_FIRST_STIM2SGNL_INDEX		CAS3_DGT1_PCKT_STIM
#define CAS3_STIM2SGNL(x)			cas3_stim2sgnl[x-CAS3_FIRST_STIM2SGNL_INDEX]
#define MAX_CAS3_STIM2SGNL_ARR_SIZE		19
//.................................
#else  // CAS3_ON_RSUSIDE || CAS3_ON_E1_PROCESSOR_SIDE

#define RSUSIDE_CAS3_STIMS_END			(RSUSIDE_CAS3_STIMS_START-1)
#define NO_RSUSIDE_CAS3_STIMS			0

#endif // CAS3_ON_RSUSIDE || CAS3_ON_E1_PROCESSOR_SIDE


#define ISUP_STIMS_START			(RSUSIDE_CAS3_STIMS_END+1)
#ifdef ISUP
#define CC_BLOCKING_IND_STIM			(ISUP_STIMS_START+0)
#define CC_UNBLOCKING_IND_STIM			(ISUP_STIMS_START+1)
#define CC_MAINTENANCE_SYSTEM_IND_STIM		(ISUP_STIMS_START+2)
#define CC_BLOCKING_CONF_STIM			(ISUP_STIMS_START+3)
#define CC_UNBLOCKING_CONF_STIM 		(ISUP_STIMS_START+4)
#define CC_STOP_CONF_STIM			(ISUP_STIMS_START+5)
#define CC_RESET_CONF_STIM			(ISUP_STIMS_START+6)
#define CC_SETUP_IND_STIM			(ISUP_STIMS_START+7)
#define CC_SETUP_CONF_STIM			(ISUP_STIMS_START+8)
#define CC_CONTINUITY_REPORT_IND_STIM		(ISUP_STIMS_START+9)
#define CC_CALL_FAILURE_IDLE_CHANNEL_IND_STIM	(ISUP_STIMS_START+0xa)
#define CC_INFO_IND_STIM			(ISUP_STIMS_START+0xb)
#define CC_RELEASE_IND_STIM			(ISUP_STIMS_START+0xc)
#define CC_RELEASE_CONF_STIM			(ISUP_STIMS_START+0xd)
#define CC_RESET_IND_STIM			(ISUP_STIMS_START+0xe)
#define CC_START_RESET_IND_STIM 		(ISUP_STIMS_START+0xf)
#define CC_FORWARD_TRANSFER_IND_STIM		(ISUP_STIMS_START+0x10)
#define CC_SUSPENDED_IND_STIM			(ISUP_STIMS_START+0x11)
#define CC_RESUMED_IND_STIM			(ISUP_STIMS_START+0x12)
#define CC_REATTEMPT_IND_STIM			(ISUP_STIMS_START+0x13)
#define CC_RESET_IND_REATTEMPT_STIM		(ISUP_STIMS_START+0x14)
#define CC_START_RESET_IND_REATTEMPT_STIM	(ISUP_STIMS_START+0x15)
#define CC_ALERT_IND_STIM			(ISUP_STIMS_START+0x16)
#define CC_IBI_IND_STIM 			(ISUP_STIMS_START+0x17)
#define CC_PROG_IND_STIM			(ISUP_STIMS_START+0x18)
#define CC_CONTINUITY_RECHECK_CONF_STIM 	(ISUP_STIMS_START+0x19)
#define CC_CALL_FAILURE_IND_DUAL_SEIZURE_STIM	(ISUP_STIMS_START+0x1a)
#define CC_CHARGING_STIM			(ISUP_STIMS_START+0x1b)
#define ISUP_RQST				(ISUP_STIMS_START+0x1c)
#define CC_CALL_FAILURE_WTFR_RLC_IND_STIM	(ISUP_STIMS_START+0x1d)
#define CC_RELEASE_CONF_AFTER_T5_TIMEOUT_STIM	(ISUP_STIMS_START+0x1e)
#define CC_GROUP_BLOCKING_IND_STIM		(ISUP_STIMS_START+0x1f)
#define CC_GROUP_UNBLOCKING_IND_STIM		(ISUP_STIMS_START+0x20)
#define CC_PROC_IND_STIM			(ISUP_STIMS_START+0x21)

#define ISUP_STIMS_END				(ISUP_STIMS_START + 0x21)
#define NO_ISUP_STIMS				(ISUP_STIMS_END - ISUP_STIMS_START + 1)
#else
#define ISUP_STIMS_END				(ISUP_STIMS_START-1)
#define NO_ISUP_STIMS				0
#endif

#define LE_V51_STIMS_START				(ISUP_STIMS_END+1)

#ifdef LE_V51_INSTALLED
#define LE_V51_ERR_DISCONNECT_SENT_STIM 		(LE_V51_STIMS_START+0)
#define LE_V51_DISCONNECT_COMPLETE_IND_STIM		(LE_V51_STIMS_START+1)
#define LE_V51_ERR_DISCONNECT_COMPLETE_NOT_RCVD_STIM	(LE_V51_STIMS_START+2)
#define LE_V51_ESTABLISH_ACK_IND_STIM			(LE_V51_STIMS_START+3)
#define LE_V51_AN_DISCONNECT_IND_STIM			LE_V51_DISCONNECT_COMPLETE_IND_STIM

#define LE_V51_STIMS_END				(LE_V51_STIMS_START + 3)
#define NO_LE_V51_STIMS 				(LE_V51_STIMS_END - LE_V51_STIMS_START + 1)
#else
#define LE_V51_STIMS_END				(LE_V51_STIMS_START-1)
#define NO_LE_V51_STIMS 				0
#endif

#define FAR_SUBSCRIBER_STIMS_START		(LE_V51_STIMS_END+1)

#if ((defined RMT_EXT) || (defined RMT_CB) || (defined LE_V51_INSTALLED) || \
     (defined LE_V52_INSTALLED) || (defined DECT_INSTALLED) || (defined EXT_ON_H248_MGCSIDE))
#define FAR_SUBSCRIBER_ON_HOOK_STIM		(FAR_SUBSCRIBER_STIMS_START+0)

#define FAR_SUBSCRIBER_STIMS_END		(FAR_SUBSCRIBER_STIMS_START+0)
#define NO_FAR_SUBSCRIBER_STIMS 		(FAR_SUBSCRIBER_STIMS_END - FAR_SUBSCRIBER_STIMS_START + 1)
#else
#define FAR_SUBSCRIBER_STIMS_END		(FAR_SUBSCRIBER_STIMS_START-1)
#define NO_FAR_SUBSCRIBER_STIMS 		0
#endif

#define LE_V52_STIMS_START			(FAR_SUBSCRIBER_STIMS_END+1)

#ifdef LE_V52_INSTALLED
#define LE_V52_NATIONAL_PSTN_RSCMAN_ALLOCATION_COMPLETE_STIM		(LE_V52_STIMS_START+0)
#define LE_V52_NATIONAL_PSTN_RSCMAN_ALLOCATION_REJECT_STIM		(LE_V52_STIMS_START+1)
#define LE_V52_NATIONAL_PSTN_RSCMAN_DEALLOCATION_COMPLETE_STIM		(LE_V52_STIMS_START+2)
#define LE_V52_NATIONAL_PSTN_RSCMAN_DEALLOCATION_REJECT_STIM		(LE_V52_STIMS_START+3)
#define LE_V52_NATIONAL_PSTN_RSCMAN_AUDIT_COMPLET_STIM			(LE_V52_STIMS_START+4)
#define LE_V52_NATIONAL_PSTN_RSCMAN_AUDIT_ERROR_INDICATION_STIM 	(LE_V52_STIMS_START+5)
#define LE_V52_NATIONAL_PSTN_RSCMAN_AN_FAULT_INDICATION_STIM		(LE_V52_STIMS_START+6)
#define LE_V52_NATIONAL_PSTN_PSTN_DISCONNECT_COMPLETE_IND_STIM		(LE_V52_STIMS_START+7)
#define LE_V52_NATIONAL_PSTN_PSTN_ERR_DISCONNECT_SENT_STIM		(LE_V52_STIMS_START+8)
#define LE_V52_NATIONAL_PSTN_PSTN_ERR_DISCONNECT_COMPLETE_NOT_RCVD_STIM (LE_V52_STIMS_START+9)
#define LE_V52_NATIONAL_PSTN_PSTN_ESTABLISH_ACK_IND_STIM		(LE_V52_STIMS_START+0xa)
#define LE_V52_NATIONAL_PSTN_PSTN_AN_DISCONNECT_IND_STIM		(LE_V52_NATIONAL_PSTN_PSTN_DISCONNECT_COMPLETE_IND_STIM)

#define LE_V52_STIMS_END				(LE_V52_STIMS_START + 0xa)
#define NO_LE_V52_STIMS 				(LE_V52_STIMS_END - LE_V52_STIMS_START + 1)
#else
#define LE_V52_STIMS_END				(LE_V52_STIMS_START-1)
#define NO_LE_V52_STIMS 				0
#endif

#define CP_H248_EXT_MGC_STIMS_START			(LE_V52_STIMS_END + 1)

#ifdef EXT_ON_H248_MGCSIDE
#define CP_H248_EXT_MGCI_ALLOCATION_SUCCESS_STIM	(CP_H248_EXT_MGC_STIMS_START+0x0)
#define CP_H248_EXT_MGCI_ALLOCATION_FAILURE_STIM	(CP_H248_EXT_MGC_STIMS_START+0x1)
#define CP_H248_EXT_MGCO_ALLOCATION_SUCCESS_STIM	(CP_H248_EXT_MGC_STIMS_START+0x2)
#define CP_H248_EXT_MGCO_ALLOCATION_FAILURE_STIM	(CP_H248_EXT_MGC_STIMS_START+0x3)
#define CP_H248_EXT_MGC_DEALLOCATION_SUCCESS_STIM	(CP_H248_EXT_MGC_STIMS_START+0x4)
#define CP_H248_EXT_MGC_DEALLOCATION_FAILURE_STIM	(CP_H248_EXT_MGC_STIMS_START+0x5)
#define CP_H248_EXT_MGC_LINK_FAILURE_STIM		(CP_H248_EXT_MGC_STIMS_START+0x6)
#define CP_H248_EXT_MGCI_ALLOCATION_ERROR_STIM		(CP_H248_EXT_MGC_STIMS_START+0x7)
#define CP_H248_EXT_MGCO_ALLOCATION_ERROR_STIM		(CP_H248_EXT_MGC_STIMS_START+0x8)

#define CP_H248_EXT_MGC_STIMS_END			(CP_H248_EXT_MGC_STIMS_START+0x8)
#define NO_CP_H248_EXT_MGC_STIMS			(CP_H248_EXT_MGC_STIMS_END - CP_H248_EXT_MGC_STIMS_START + 1)
#else
#define CP_H248_EXT_MGC_STIMS_END			(CP_H248_EXT_MGC_STIMS_START-1)
#define NO_CP_H248_EXT_MGC_STIMS			0
#endif

#define CP_H248_ISUP_MGC_STIMS_START			(CP_H248_EXT_MGC_STIMS_END + 1)

#ifdef ISUP_ON_H248_MGCSIDE
#define CP_H248_ISUP_MGCI_ALLOCATION_SUCCESS_STIM	(CP_H248_ISUP_MGC_STIMS_START+0x0)
#define CP_H248_ISUP_MGCI_ALLOCATION_FAILURE_STIM	(CP_H248_ISUP_MGC_STIMS_START+0x1)
#define CP_H248_ISUP_MGCO_ALLOCATION_SUCCESS_STIM	(CP_H248_ISUP_MGC_STIMS_START+0x2)
#define CP_H248_ISUP_MGCO_ALLOCATION_FAILURE_STIM	(CP_H248_ISUP_MGC_STIMS_START+0x3)
#define CP_H248_ISUP_MGC_DEALLOCATION_SUCCESS_STIM	(CP_H248_ISUP_MGC_STIMS_START+0x4)
#define CP_H248_ISUP_MGC_DEALLOCATION_FAILURE_STIM	(CP_H248_ISUP_MGC_STIMS_START+0x5)
#define CP_H248_ISUP_MGC_LINK_FAILURE_STIM		(CP_H248_ISUP_MGC_STIMS_START+0x6)
#define CP_H248_ISUP_MGCI_ALLOCATION_ERROR_STIM 	(CP_H248_ISUP_MGC_STIMS_START+0x7)
#define CP_H248_ISUP_MGCO_ALLOCATION_ERROR_STIM 	(CP_H248_ISUP_MGC_STIMS_START+0x8)

#define CP_H248_ISUP_MGC_STIMS_END			(CP_H248_ISUP_MGC_STIMS_START+0x8)
#define NO_CP_H248_ISUP_MGC_STIMS			(CP_H248_ISUP_MGC_STIMS_END - CP_H248_ISUP_MGC_STIMS_START + 1)
#else
#define CP_H248_ISUP_MGC_STIMS_END			(CP_H248_ISUP_MGC_STIMS_START-1)
#define NO_CP_H248_ISUP_MGC_STIMS			0
#endif

#define CP_ABIS_CC_STIMS_START				(CP_H248_ISUP_MGC_STIMS_END + 1)

#ifdef GSM_INSTALLED
#define CP_ABIS_CC_SETUP_STIM				(CP_ABIS_CC_STIMS_START + 0x00)
#define CP_ABIS_CC_REGISTER_STIM			(CP_ABIS_CC_STIMS_START + 0x01)
#define CP_ABIS_CC_DISCONNECT_STIM			(CP_ABIS_CC_STIMS_START + 0x02)
#define CP_ABIS_CC_RELEASE_STIM 			(CP_ABIS_CC_STIMS_START + 0x03)
#define CP_ABIS_CC_RELEASE_COMPLETE_STIM		(CP_ABIS_CC_STIMS_START + 0x04)
#define CP_ABIS_CC_DISCONNECT_SENT_STIM 		(CP_ABIS_CC_STIMS_START + 0x05)
#define CP_ABIS_CC_RELEASE_SENT_STIM			(CP_ABIS_CC_STIMS_START + 0x06)
#define CP_ABIS_CC_CALL_CONFIRMED_STIM			(CP_ABIS_CC_STIMS_START + 0x07)
#define CP_ABIS_CC_MS_FAILURE_STIM			(CP_ABIS_CC_STIMS_START + 0x08)
#define CP_ABIS_CC_ALERTING_STIM			(CP_ABIS_CC_STIMS_START + 0x09)
#define CP_ABIS_CC_HOLD_STIM				(CP_ABIS_CC_STIMS_START + 0x0a)
#define CP_ABIS_CC_RETRIEVE_STIM			(CP_ABIS_CC_STIMS_START + 0x0b)
#define CP_ABIS_CC_CONF_STIM				(CP_ABIS_CC_STIMS_START + 0x0c)
#define CP_ABIS_CC_CONNECT_STIM 			(CP_ABIS_CC_STIMS_START + 0x0d)
#define CP_ABIS_CC_HOLDING_DISCONNECTED_STIM		(CP_ABIS_CC_STIMS_START + 0x0e)
#define CP_ABIS_CC_MS_NOT_REACHABLE_STIM		(CP_ABIS_CC_STIMS_START + 0x0f)
#define CP_ABIS_CC_CLOSE_VOICE_PATH_STIM		(CP_ABIS_CC_STIMS_START + 0x10)
#define CP_ABIS_CC_SPLIT_CONF_STIM			(CP_ABIS_CC_STIMS_START + 0x11)
#define CP_ABIS_CC_RETRIEVE_CONF_STIM			(CP_ABIS_CC_STIMS_START + 0x12)
#define CP_ABIS_CC_HOLD_CONF_STIM			(CP_ABIS_CC_STIMS_START + 0x13)

#define CP_ABIS_CC_STIMS_END				(CP_ABIS_CC_STIMS_START + 0x13)
#define NO_CP_ABIS_CC_STIMS				(CP_ABIS_CC_STIMS_END - CP_ABIS_CC_STIMS_START + 1)
#else
#define CP_ABIS_CC_STIMS_END				(CP_ABIS_CC_STIMS_START - 1 )
#define NO_CP_ABIS_CC_STIMS				0
#endif

#define CP_NET_ISDN_STIMS_START 			(CP_ABIS_CC_STIMS_END + 1)

#ifdef NET_ISDN_INSTALLED
#define CP_NET_Q931_SETUP_STIM				(CP_NET_ISDN_STIMS_START + 0x00)
#define CP_NET_Q931_SETUP_ACK_STIM			(CP_NET_ISDN_STIMS_START + 0x01)
#define CP_NET_Q931_RESUME_STIM 			(CP_NET_ISDN_STIMS_START + 0x02)
#define CP_NET_Q931_SUSPEND_STIM			(CP_NET_ISDN_STIMS_START + 0x03)
#define CP_NET_Q931_INTERNAL_RELEASE_STIM		(CP_NET_ISDN_STIMS_START + 0x04)
#define CP_NET_Q931_RELEASE_INDICATION_STIM		(CP_NET_ISDN_STIMS_START + 0x05)
#define CP_NET_Q931_RELEASE_SENT_STIM			(CP_NET_ISDN_STIMS_START + 0x06)
#define CP_NET_Q931_DISCONNECT_STIM			(CP_NET_ISDN_STIMS_START + 0x07)
#define CP_NET_Q931_DISCONNECT_SENT_STIM		(CP_NET_ISDN_STIMS_START + 0x08)
#define CP_NET_Q931_INFO_STIM				(CP_NET_ISDN_STIMS_START + 0x09)
#define CP_NET_Q931_INTER_DIGIT_TIMEOUT_STIM		(CP_NET_ISDN_STIMS_START + 0x0a)
#define CP_NET_Q931_ALERTING_STIM			(CP_NET_ISDN_STIMS_START + 0x0b)
#define CP_NET_Q931_CONNECT_STIM			(CP_NET_ISDN_STIMS_START + 0x0c)
#define CP_NET_Q931_CALL_PROCEEDING_STIM		(CP_NET_ISDN_STIMS_START + 0x0d)
#define CP_NET_Q931_PROGRESS_STIM			(CP_NET_ISDN_STIMS_START + 0x0e)
#define CP_NET_Q931_NOTIFY_STIM 			(CP_NET_ISDN_STIMS_START + 0x0f)
#define CP_NET_Q932_HOLDING_DISCONNECTED_STIM		(CP_NET_ISDN_STIMS_START + 0x10)
#define CP_NET_Q932_HOLD_STIM				(CP_NET_ISDN_STIMS_START + 0x11)
#define CP_NET_Q932_RETRIEVE_STIM			(CP_NET_ISDN_STIMS_START + 0x12)
#define CP_NET_Q932_REGISTER_STIM			(CP_NET_ISDN_STIMS_START + 0x13)
#define CP_NET_Q931_CLOSE_VOICE_PATH_STIM		(CP_NET_ISDN_STIMS_START + 0x14)

#define CP_NET_ISDN_STIMS_END				(CP_NET_ISDN_STIMS_START + 0x14)
#define NO_CP_NET_ISDN_STIMS				(CP_NET_ISDN_STIMS_END - CP_NET_ISDN_STIMS_START + 1)
#else
#define CP_NET_ISDN_STIMS_END				(CP_NET_ISDN_STIMS_START - 1 )
#define NO_CP_NET_ISDN_STIMS				 0
#endif

#define CP_CR_STIMS_START				(CP_NET_ISDN_STIMS_END + 1)

#ifdef MORE_TIMERS_FOR_CR
#define CP_CR_TMR1_TIMEOUT_STIM 			(CP_CR_STIMS_START + 0x00)
#define CP_CR_TMR2_TIMEOUT_STIM 			(CP_CR_STIMS_START + 0x01)

#define CP_CR_STIMS_END 				(CP_CR_STIMS_START + 0x01)
#define NO_CP_CR_STIMS					(CP_CR_STIMS_END - CP_CR_STIMS_START + 1)
#else
#define CP_CR_STIMS_END 				(CP_CR_STIMS_START - 1 )
#define NO_CP_CR_STIMS					 0
#endif

#define CP_USERSIDE_Q931_STIMS_START			(CP_NET_ISDN_STIMS_END + 1)

#ifdef USERSIDE_PRIMARY_ISDN_INSTALLED
#define PRI_RQST					(CP_USERSIDE_Q931_STIMS_START+ 0x00)
#define CP_USERSIDE_Q931_SETUP_STIM			(CP_USERSIDE_Q931_STIMS_START+ 0x01)
#define CP_USERSIDE_Q931_SETUP_ACK_STIM 		(CP_USERSIDE_Q931_STIMS_START+ 0x02)
#define CP_USERSIDE_Q931_INTERNAL_RELEASE_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x03)
#define CP_USERSIDE_Q931_RELEASE_SENT_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x04)
#define CP_USERSIDE_Q931_RELEASE_INDICATION_STIM	(CP_USERSIDE_Q931_STIMS_START+ 0x05)
#define CP_USERSIDE_Q931_CALL_PROCEEDING_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x06)
#define CP_USERSIDE_Q931_ALERTING_STIM			(CP_USERSIDE_Q931_STIMS_START+ 0x07)
#define CP_USERSIDE_Q931_CONNECT_STIM			(CP_USERSIDE_Q931_STIMS_START+ 0x08)
#define CP_USERSIDE_Q931_CONNECT_ACK_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x09)
#define CP_USERSIDE_Q931_INFO_STIM			(CP_USERSIDE_Q931_STIMS_START+ 0x0a)
#define CP_USERSIDE_Q931_PROGRESS_STIM			(CP_USERSIDE_Q931_STIMS_START+ 0x0b)
#define CP_USERSIDE_Q931_DISCONNECT_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x0c)
#define CP_USERSIDE_Q931_DISCONNECT_SENT_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x0d)
#define CP_USERSIDE_Q931_NOTIFY_STIM			(CP_USERSIDE_Q931_STIMS_START+ 0x0e)
#define CP_USERSIDE_Q931_STATUS_STIM			(CP_USERSIDE_Q931_STIMS_START+ 0x0f)
#define CP_USERSIDE_Q931_SUSPEND_ACK_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x10)
#define CP_USERSIDE_Q931_SUSPEND_REJECT_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x11)
#define CP_USERSIDE_Q931_SUSPEND_TIMEOUT_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x12)
#define CP_USERSIDE_Q931_RESUME_ACK_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x13)
#define CP_USERSIDE_Q931_RESUME_REJECT_STIM		(CP_USERSIDE_Q931_STIMS_START+ 0x14)

#define CP_USERSIDE_Q931_STIMS_END			(CP_USERSIDE_Q931_STIMS_START+ 0x14)
#define NO_CP_USERSIDE_Q931_STIMS			(CP_USERSIDE_Q931_STIMS_END - CP_USERSIDE_Q931_STIMS_START + 1)
#else
#define CP_USERSIDE_Q931_STIMS_END			(CP_USERSIDE_Q931_STIMS_START - 1 )
#define NO_CP_USERSIDE_Q931_STIMS			0
#endif // USERSIDE_PRIMARY_ISDN_INSTALLED

#define DECT_STIMS_START		(CP_USERSIDE_Q931_STIMS_END+1)
#ifdef DECT_INSTALLED
#define DECT_DISCONNECT_IND_STIM	(DECT_STIMS_START + 0)
#define DECT_END_OF_ALERTING_STIM	(DECT_STIMS_START + 1)
#define DECT_PAGE_SUCCESSFUL_STIM	(DECT_STIMS_START + 2)
#define DECT_DIGIT_STIM 		(DECT_STIMS_START + 3)
#define DECT_STIMS_END			(DECT_STIMS_START + 3 + 0x0f)  // allow for 0x10 (from 0-15) digit stims
#define NO_DECT_STIMS			(DECT_STIMS_END - DECT_STIMS_START + 1)
#else
#define DECT_STIMS_END			(DECT_STIMS_START-1)
#define NO_DECT_STIMS			0
#endif

#define CP_H225_Q931_STIMS_START		(DECT_STIMS_END+1)
#ifdef H225_CC_INSTALLED
#define H225_RQST				  (CP_H225_Q931_STIMS_START + 0)
#define CP_H225_Q931_SETUP_STIM 		  (CP_H225_Q931_STIMS_START + 1)
#define CP_H225_Q931_SETUP_ACK_STIM		  (CP_H225_Q931_STIMS_START + 2)
#define CP_H225_Q931_CALL_PROCEEDING_STIM	  (CP_H225_Q931_STIMS_START + 3)
#define CP_H225_Q931_INFO_STIM			  (CP_H225_Q931_STIMS_START + 4)
#define CP_H225_Q931_PROGRESS_STIM		  (CP_H225_Q931_STIMS_START + 5)
#define CP_H225_Q931_ALERTING_STIM		  (CP_H225_Q931_STIMS_START + 6)
#define CP_H225_Q931_CONNECT_STIM		  (CP_H225_Q931_STIMS_START + 7)
#define CP_H225_Q931_INTERNAL_RELEASE_STIM	  (CP_H225_Q931_STIMS_START + 8)
#define CP_H225_Q931_RELEASE_COMPLETE_STIM	  (CP_H225_Q931_STIMS_START + 9)
#define CP_H225_Q931_NOTIFY_STIM		  (CP_H225_Q931_STIMS_START + 0xa)
#define CP_H225_Q931_RTP_ALLOCATION_COMPLETE_STIM (CP_H225_Q931_STIMS_START + 0xb)

#define CP_H225_Q931_STIMS_END			  (CP_H225_Q931_STIMS_START + 0xb)
#define NO_CP_H225_Q931_STIMS			  (CP_H225_Q931_STIMS_END - CP_H225_Q931_STIMS_START + 1)
#else
#define CP_H225_Q931_STIMS_END			  (CP_H225_Q931_STIMS_START-1)
#define NO_CP_H225_Q931_STIMS			  0
#endif
#define CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START		(CP_H225_Q931_STIMS_END + 1)

#ifdef PRIMARY_NET_Q931_ON_H248_MGCSIDE
#define CP_H248_PRIMARY_NET_Q931_MGCI_ALLOCATION_SUCCESS_STIM	(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x0)
#define CP_H248_PRIMARY_NET_Q931_MGCI_ALLOCATION_FAILURE_STIM	(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x1)
#define CP_H248_PRIMARY_NET_Q931_MGCO_ALLOCATION_SUCCESS_STIM	(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x2)
#define CP_H248_PRIMARY_NET_Q931_MGCO_ALLOCATION_FAILURE_STIM	(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x3)
#define CP_H248_PRIMARY_NET_Q931_MGC_DEALLOCATION_SUCCESS_STIM	(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x4)
#define CP_H248_PRIMARY_NET_Q931_MGC_DEALLOCATION_FAILURE_STIM	(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x5)
#define CP_H248_PRIMARY_NET_Q931_MGC_LINK_FAILURE_STIM		(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x6)
#define CP_H248_PRIMARY_NET_Q931_MGCI_ALLOCATION_ERROR_STIM	(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x7)
#define CP_H248_PRIMARY_NET_Q931_MGCO_ALLOCATION_ERROR_STIM	(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x8)
#define CP_H248_PRIMARY_NET_Q931_MGC_STIMS_END			(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START+0x8)
#define NO_CP_H248_PRIMARY_NET_Q931_MGC_STIMS			(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_END - CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START + 1)
#else
#define CP_H248_PRIMARY_NET_Q931_MGC_STIMS_END			(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_START-1)
#define NO_CP_H248_PRIMARY_NET_Q931_MGC_STIMS			0
#endif
#define SIP_CP_STIMS_START					(CP_H248_PRIMARY_NET_Q931_MGC_STIMS_END+1)
#ifdef SIP_INSTALLED
#define SIP_RQST						(SIP_CP_STIMS_START+0x00)
#define SIP_CP_API_100_TRYING_INVITE_RESPONSE_STIM		(SIP_CP_STIMS_START+0x01)
#define SIP_CP_API_180_RINGING_INVITE_RESPONSE_STIM		(SIP_CP_STIMS_START+0x02)
#define SIP_CP_API_183_SESSION_PROGRESS_INVITE_RESPONSE_STIM	(SIP_CP_STIMS_START+0x03)
#define SIP_CP_API_1XX_INFORMATIONAL_INVITE_RESPONSE_STIM	(SIP_CP_STIMS_START+0x04)
#define SIP_CP_API_200_OK_INVITE_RESPONSE_STIM			(SIP_CP_STIMS_START+0x05)
#define SIP_CP_API_3XX_REDIRECTION_INVITE_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x06)
#define SIP_CP_API_4XX_CLIENT_ERROR_INVITE_RESPONSE_STIM	(SIP_CP_STIMS_START+0x07)
#define SIP_CP_API_5XX_SERVER_ERROR_INVITE_RESPONSE_STIM	(SIP_CP_STIMS_START+0x08)
#define SIP_CP_API_1XX_INFORMATIONAL_BYE_RESPONSE_STIM		(SIP_CP_STIMS_START+0x09)
#define SIP_CP_API_200_OK_BYE_RESPONSE_STIM			(SIP_CP_STIMS_START+0x0a)
#define SIP_CP_API_3XX_REDIRECTION_BYE_RESPONSE_STIM		(SIP_CP_STIMS_START+0x0b)
#define SIP_CP_API_4XX_CLIENT_ERROR_BYE_RESPONSE_STIM		(SIP_CP_STIMS_START+0x0c)
#define SIP_CP_API_5XX_SERVER_ERROR_BYE_RESPONSE_STIM		(SIP_CP_STIMS_START+0x0d)
#define SIP_CP_API_1XX_INFORMATIONAL_CANCEL_RESPONSE_STIM	(SIP_CP_STIMS_START+0x0e)
#define SIP_CP_API_200_OK_CANCEL_RESPONSE_STIM			(SIP_CP_STIMS_START+0x0f)
#define SIP_CP_API_3XX_REDIRECTION_CANCEL_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x10)
#define SIP_CP_API_4XX_CLIENT_ERROR_CANCEL_RESPONSE_STIM	(SIP_CP_STIMS_START+0x11)
#define SIP_CP_API_5XX_SERVER_ERROR_CANCEL_RESPONSE_STIM	(SIP_CP_STIMS_START+0x12)
#define SIP_CP_API_1XX_INFORMATIONAL_INFO_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x13)
#define SIP_CP_API_200_OK_INFO_RESPONSE_STIM			(SIP_CP_STIMS_START+0x14)
#define SIP_CP_API_3XX_REDIRECTION_INFO_RESPONSE_STIM		(SIP_CP_STIMS_START+0x15)
#define SIP_CP_API_4XX_CLIENT_ERROR_INFO_RESPONSE_STIM		(SIP_CP_STIMS_START+0x16)
#define SIP_CP_API_5XX_SERVER_ERROR_INFO_RESPONSE_STIM		(SIP_CP_STIMS_START+0x17)
#define SIP_CP_API_1XX_INFORMATIONAL_UPDATE_RESPONSE_STIM	(SIP_CP_STIMS_START+0x18)
#define SIP_CP_API_200_OK_UPDATE_RESPONSE_STIM			(SIP_CP_STIMS_START+0x19)
#define SIP_CP_API_3XX_REDIRECTION_UPDATE_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x1a)
#define SIP_CP_API_4XX_CLIENT_ERROR_UPDATE_RESPONSE_STIM	(SIP_CP_STIMS_START+0x1b)
#define SIP_CP_API_5XX_SERVER_ERROR_UPDATE_RESPONSE_STIM	(SIP_CP_STIMS_START+0x1c)
#define SIP_CP_API_1XX_INFORMATIONAL_REFER_RESPONSE_STIM	(SIP_CP_STIMS_START+0x1d)
#define SIP_CP_API_200_OK_REFER_RESPONSE_STIM			(SIP_CP_STIMS_START+0x1e)
#define SIP_CP_API_3XX_REDIRECTION_REFER_RESPONSE_STIM		(SIP_CP_STIMS_START+0x1f)
#define SIP_CP_API_4XX_CLIENT_ERROR_REFER_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x20)
#define SIP_CP_API_5XX_SERVER_ERROR_REFER_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x21)
#define SIP_CP_API_1XX_INFORMATIONAL_NOTIFY_RESPONSE_STIM	(SIP_CP_STIMS_START+0x22)
#define SIP_CP_API_200_OK_NOTIFY_RESPONSE_STIM			(SIP_CP_STIMS_START+0x23)
#define SIP_CP_API_3XX_REDIRECTION_NOTIFY_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x24)
#define SIP_CP_API_4XX_CLIENT_ERROR_NOTIFY_RESPONSE_STIM	(SIP_CP_STIMS_START+0x25)
#define SIP_CP_API_5XX_SERVER_ERROR_NOTIFY_RESPONSE_STIM	(SIP_CP_STIMS_START+0x26)
#define SIP_CP_API_200_OK_PRACK_RESPONSE_STIM			(SIP_CP_STIMS_START+0x27)
#define SIP_CP_API_3XX_REDIRECTION_PRACK_RESPONSE_STIM		(SIP_CP_STIMS_START+0x28)
#define SIP_CP_API_4XX_CLIENT_ERROR_PRACK_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x29)
#define SIP_CP_API_5XX_SERVER_ERROR_PRACK_RESPONSE_STIM 	(SIP_CP_STIMS_START+0x2a)
#define SIP_CP_API_INVITE_REQUEST_STIM	/* also re-invite */	(SIP_CP_STIMS_START+0x2b)
#define SIP_CP_API_BYE_REQUEST_STIM				(SIP_CP_STIMS_START+0x2c)
#define SIP_CP_API_CANCEL_REQUEST_STIM				(SIP_CP_STIMS_START+0x2d)
#define SIP_CP_API_INFO_REQUEST_STIM				(SIP_CP_STIMS_START+0x2e)
#define SIP_CP_API_UPDATE_REQUEST_STIM				(SIP_CP_STIMS_START+0x2f)
#define SIP_CP_API_REFER_REQUEST_STIM				(SIP_CP_STIMS_START+0x30)
#define SIP_CP_API_NOTIFY_REQUEST_STIM				(SIP_CP_STIMS_START+0x31)
#define SIP_CP_API_ACK_REQUEST_STIM				(SIP_CP_STIMS_START+0x32)
#define SIP_CP_API_ACK_FAILURE_STIM				(SIP_CP_STIMS_START+0x33)

#define SIP_CP_STIMS_END					(SIP_CP_STIMS_START+0x33)
#define NO_SIP_CP_STIMS 					(SIP_CP_STIMS_END-SIP_CP_STIMS_START+1)
#else
#define SIP_CP_STIMS_END					(SIP_CP_STIMS_START-1)
#define NO_SIP_CP_STIMS 					0
#endif
#define CP_SIP_ANNCE_STIMS_START				(SIP_CP_STIMS_END+1)
#ifdef SIP_ANNCE_INSTALLED
#define CP_SIP_ANNCE_ANNOUNCEMENT_FAILURE_STIM			(CP_SIP_ANNCE_STIMS_START+0x00)
#define CP_SIP_ANNCE_ANNOUNCEMENT_STOPPED_STIM			(CP_SIP_ANNCE_STIMS_START+0x01)
#define CP_SIP_ANNCE_ANNOUNCEMENT_ADDRESS_STIM			(CP_SIP_ANNCE_STIMS_START+0x02)

#define CP_SIP_ANNCE_STIMS_END					(CP_SIP_ANNCE_STIMS_START+0x02)
#define NO_CP_SIP_ANNCE_STIMS					(CP_SIP_ANNCE_STIMS_END-CP_SIP_ANNCE_STIMS_START+1)
#else
#define CP_SIP_ANNCE_STIMS_END					(CP_SIP_ANNCE_STIMS_START-1)
#define NO_CP_SIP_ANNCE_STIMS					0
#endif
#define CP_SIP_CONF_STIMS_START 				(CP_SIP_ANNCE_STIMS_END+1)
#ifdef SIP_CONF_INSTALLED
#define CP_SIP_CONF_CONFERENCE_FAILURE_STIM			(CP_SIP_CONF_STIMS_START+0x00)
#define CP_SIP_CONF_CONFERENCE_STOPPED_STIM			(CP_SIP_CONF_STIMS_START+0x01)
#define CP_SIP_CONF_CONFERENCE_ADDRESS_STIM			(CP_SIP_CONF_STIMS_START+0x02)

#define CP_SIP_CONF_STIMS_END					(CP_SIP_CONF_STIMS_START+0x02)
#define NO_CP_SIP_CONF_STIMS					(CP_SIP_CONF_STIMS_END-CP_SIP_CONF_STIMS_START+1)
#else
#define CP_SIP_CONF_STIMS_END					(CP_SIP_CONF_STIMS_START-1)
#define NO_CP_SIP_CONF_STIMS					0
#endif

/**-----------------------------------------------------------------------**/
#define  NO_STIMS	(NO_GLOBAL_STIMS + NO_CAS3_STIMS +		\
			 NO_CAS1_STIMS + NO_CEPT_STIMS +		\
			 NO_LD_STIMS + NO_RMT_EXT_STIMS +		\
			 NO_CHANNEL_TEST_STIMS + NO_EM_RSUSIDE_STIMS +	\
			 NO_RMT_EM_STIMS + NO_EXT_RSUSIDE_STIMS +	\
			 NO_RSUSIDE_STIMS + NO_RMT_CAS3_STIMS + 	\
			 NO_RSUSIDE_CAS3_STIMS + NO_ISUP_STIMS +	\
			 NO_LE_V51_STIMS + NO_FAR_SUBSCRIBER_STIMS +	\
			 NO_LE_V52_STIMS + NO_CP_H248_EXT_MGC_STIMS+	\
			 NO_CP_ABIS_CC_STIMS + NO_CP_NET_ISDN_STIMS+	\
			 NO_CP_CR_STIMS + NO_CP_USERSIDE_Q931_STIMS+	\
			 NO_DECT_STIMS + NO_CP_H225_Q931_STIMS+ 	\
			 NO_CP_H248_ISUP_MGC_STIMS+			\
			 NO_CP_H248_PRIMARY_NET_Q931_MGC_STIMS+ 	\
			 NO_SIP_CP_STIMS + NO_CP_SIP_ANNCE_STIMS+	\
			 NO_CP_SIP_CONF_STIMS)
/**-----------------------------------------------------------------------**/
#if (NO_STIMS >= 0xff)
#error NO_STIMS >= 0xff
#endif

#ifdef CAS3_INSTALLED

#define DT_SEIZED		CEPT_STIM_1
#define DT_CLF			CEPT_STIM_9
#define DT_SEIZED_ACKN		CEPT_STIM_D
#define DT_CONGESTION		CEPT_STIM_3
#define DT_EOS_START		CEPT_STIM_9
#define DT_EOS_END		CEPT_STIM_D
#define DT_ANSWERED		CEPT_STIM_5
#define DT_RLS_GUARD		CEPT_STIM_9
#define DT_PTS_START		CEPT_STIM_F
#define DT_PTS_END		CEPT_STIM_D
#define DT_CLB			CEPT_STIM_D
#define DT_OPR_RCL_START	CEPT_STIM_D
#define DT_OPR_RCL_END		CEPT_STIM_5
#define DT_METERING_START	CEPT_STIM_7
#define DT_METERING_END 	CEPT_STIM_5
#define DT_BLOCK		CEPT_STIM_D
#define DT_UNBLOCK		CEPT_STIM_9
#define DT_IDLE 		CEPT_STIM_9
#define DT_MALICIOUS_CALL	CEPT_STIM_1
#ifdef YAZD2
#define DT_FAS_START		CEPT_STIM_F
#else
#define DT_FAS_START		CEPT_STIM_5
#endif
#define DT_FAS_END		CEPT_STIM_D

#endif

#ifdef CAS3_NEW_SCAN
#define 	CAS3_SEIZE_MALICIOUS_SGNL_STIM		CEPT_STIM_1
#define 	CAS3_CONGESTION_SGNL_STIM		CEPT_STIM_3
#define 	CAS3_ANSWER_SGNL_STIM			CEPT_STIM_5
#define 	CAS3_IDLE_CLF_RLG_SGNL_STIM		CEPT_STIM_9
#define 	CAS3_SZACK_BLK_CLB_SGNL_STIM		CEPT_STIM_D
#define 	CAS3_UNKNOWN_SGNL_STIM			CEPT_STIM_UNKNOWN
#define 	CAS3_EOS_SGNL_STIM			CEPT_STIM_EOS
#define 	CAS3_FAS_SGNL_STIM			CEPT_STIM_FAS
#define 	CAS3_METERING_SGNL_STIM 		CEPT_STIM_METERING
#endif
