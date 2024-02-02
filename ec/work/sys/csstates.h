
/**-----------------------------------------------------------------------**/
#ifdef CAS3_INSTALLED

#define  AWAIT_SEIZED_ACKN		CAS3_STATES_START
#define  SEIZED_ACKN			(CAS3_STATES_START + 1)
#define  AWAIT_DT_PTS			(CAS3_STATES_START + 2)
#define  DT_PTS 			(CAS3_STATES_START + 3)
#define  ADDRESSING_DGT 		(CAS3_STATES_START + 4)
#define  AWAIT_DT_EOS			(CAS3_STATES_START + 5)
#define  DT_EOS 			(CAS3_STATES_START + 6)
#define  AWAIT_DT_ANSWERED		(CAS3_STATES_START + 7)
#define  DT_GUARD			(CAS3_STATES_START + 8)
#define  EXT_DT_TALK			(CAS3_STATES_START + 9)
#define  DT_OPR_RCL_CLB 		(CAS3_STATES_START + 0x0a)
#define  DT_METERING			(CAS3_STATES_START + 0x0b)
#define  OGDT_FLASH_DTCT		(CAS3_STATES_START + 0x0c)
#define  DT_OG_BLOCK			(CAS3_STATES_START + 0x0d)
#define  DT_LOCAL_BLOCK 		(CAS3_STATES_START + 0x0e)
#define  AWAIT_DT_ADDRESSING		(CAS3_STATES_START + 0x0f)
#define  AWAIT_DEST_ADDRESS		(CAS3_STATES_START + 0x10)
#define  DT_EXT_RINGING 		(CAS3_STATES_START + 0x11)
#define  DT_EXT_TALK			(CAS3_STATES_START + 0x12)
#define  ICDT_FLASH_DTCT		(CAS3_STATES_START + 0x13)
#define  AWAIT_DT_CLF			(CAS3_STATES_START + 0x14)
#define  AWAIT_SEIZED_ACKN_DGT_READY	(CAS3_STATES_START + 0x15)
#define  AWAIT_DEST_ADDRESSING		(CAS3_STATES_START + 0x16)
#define  AWAIT_CAS3_OPR_NUM		(CAS3_STATES_START + 0x17)
#define  DT_FAS 			(CAS3_STATES_START + 0x18)
#define  EXT_DT_TALK_MALICIOUS		(CAS3_STATES_START + 0x19)
#define  OPR_AWAIT_SEIZED_ACKN		(CAS3_STATES_START + 0x1a)

#define  CAS3_STATES_END	(CAS3_STATES_START + 0x1a)
#define  CHANNEL_TEST_STATES_START	 (CAS3_STATES_END + 1)
#define  NO_CAS3_STATES 	(CAS3_STATES_END - CAS3_STATES_START + 1)


#define  DT_CLF_TO_OPR		0x52
/* if changed at operator side,DT_CLF_TO_OPR must be changed */

#else
#define CHANNEL_TEST_STATES_START	CAS3_STATES_START
#define NO_CAS3_STATES	       0
#endif
/**-----------------------------------------------------------------------**/

/**-----------------------------------------------------------------------**/
/**** CHANNEL_TEST states *****/
/**-----------------------------------------------------------------------**/

#ifdef CHANNEL_TEST

#define 	WTFR_CHANNEL_TEST_DGTS		CHANNEL_TEST_STATES_START+0
#define 	CHANNEL_TEST_CONNECTED		CHANNEL_TEST_STATES_START+1

#define 	CHANNEL_TEST_STATES_END 	CHANNEL_TEST_STATES_START + 1
#define 	CONFERENCE_STATES_START 	(CHANNEL_TEST_STATES_END + 1)
#define 	NO_CHANNEL_TEST_STATES		(CHANNEL_TEST_STATES_END - CHANNEL_TEST_STATES_START + 1)

#else

#define 	CONFERENCE_STATES_START 	CHANNEL_TEST_STATES_START
#define 	NO_CHANNEL_TEST_STATES		0

#endif
/**-----------------------------------------------------------------------**/
