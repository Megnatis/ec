
#ifdef SIP_EXT_INSTALLED
#else
#define NO_SIP_EXT_SUBSCRIBER	0
#endif

#ifdef WE_DO_HAVE_AN_V52
#define A52_SYSTEM_DEBUG_ACT   // a52man.c,a52lcntl.c,a52cmmn.c,a52ptcn.c
#define EXTENDED_CCSV5
#define LOCAL_V52_PSTN_ACCESS
//#define REMOTE_V52_PSTN_ACCESS
#if ((defined LOCAL_V52_PSTN_ACCESS) || (defined REMOTE_V52_PSTN_ACCESS))
#define V52_PSTN_ACCESS
#endif

#define KLM_TEST

#define LOGICAL_CHANNEL_0_ID		0
//#define LOGICAL_CHANNEL_0_ID		  1   // some site preferred this

#define AN_V52_INSTALLED
#define LAPV5_INSTALLED

#ifdef AN_V52_INSTALLED
#if (NO_E1_IN_COREDECT >= 2)
#define V52_PROTECTION_SUPPORTED
#endif
#define NO_V52_INTERFACE		1
#define NO_LINK_PER_V52_INTERFACE	NO_E1_IN_COREDECT
#ifdef V52_PROTECTION_SUPPORTED
#define NO_HW_CHANNEL_PER_V52_INTERFACE 2
#else
#define NO_HW_CHANNEL_PER_V52_INTERFACE 1
#endif
#define NO_V52_DIGITAL_TRUNK		(NO_LINK_PER_V52_INTERFACE)
#define NO_V52_LINK			NO_V52_DIGITAL_TRUNK
#define AN_V52_EXTENSION_START		0
#define NO_PSTN_PORT_PER_V52_INTERFACE	(NO_RSU_EXT+NO_SIP_EXT_SUBSCRIBER)
#define NO_V52_PSTN_PORT		(NO_PSTN_PORT_PER_V52_INTERFACE)
#define NO_V52_SUBSCRIBER		NO_V52_PSTN_PORT
//#define NO_V52_BEARER_CHANNEL 	  ((30*2)+(31*2))
#if (NO_E1_IN_COREDECT >= 2)
#define NO_V52_BEARER_CHANNEL		(((30*2)+(31*(NO_E1_IN_COREDECT-2)))*NO_V52_INTERFACE)
#else
#define NO_V52_BEARER_CHANNEL		((30*NO_E1_IN_COREDECT)*NO_V52_INTERFACE)
#endif
#define NO_V52_LOGICAL_CHANNEL		NO_V52_INTERFACE
#ifdef V52_PROTECTION_SUPPORTED
#define NO_V52_PHYSICAL_CHANNEL 	(2)
#else
#define NO_V52_PHYSICAL_CHANNEL 	(1)
#endif
#endif // AN_V52_INSTALLED

#ifdef LAPV5_INSTALLED
#define LAPV5_9076
#define NO_LAPV5_HW_CHANNEL		(NO_V52_PHYSICAL_CHANNEL)
#ifdef V52_PROTECTION_SUPPORTED
#define NO_LAPV5_CHANNEL		(6) // LAPV5_CNTRL_DL,LAPV5_PSTN_DL,LAPV5_BCC_DL,LAPV5_LNKCNTRL_DL,LAPV5_PTCN1_DL,LAPV5_PTCN2_DL
#else
#define NO_LAPV5_CHANNEL		(4) // LAPV5_CNTRL_DL,LAPV5_PSTN_DL,LAPV5_BCC_DL,LAPV5_LNKCNTRL_DL
#endif
#endif // LAPV5_INSTALLED

#define LAPV5_USER_SIDE
#endif // WE_DO_HAVE_AN_V52

#ifdef KIATEL_UDP_FOR_OAM
#define TFTPD_INSTALLED
#endif

#if (OS ==  LINUX_COMPATIBLE)
#define KURT_DISABLED
#define OPRSIDE_LINUX_USER_SPACE_ISA_MEMORY_IO
#define NCURSES_USED
#define LINUX_USER_SPACE_ISA_MEMORY_IO
#ifdef TFTPD_INSTALLED
#define UDP_ON_SERVER_SIDE
#endif
#ifdef KIATEL_UDP_FOR_OAM
#define OAM_ON_UDP_CLIENT_SERVER_SIDE
#endif

//... newly added start
#ifdef TCPIP_ON_SYS_OAM_SYSTEM_SIDE
#define SERVER_TCP_ON_SYSTEM_SIDE
#define OAM0_IP_STRING	"192.168.100.95"
#define OAM1_IP_STRING	"192.168.100.98"
#define MAX_SYS_OAM_TCP_LINK	NO_OPRTR
#ifdef HTBY_LINK_ON_TCPIP
#define MAX_TCP_GROUP		2
#else
#define MAX_TCP_GROUP		1
#endif
#endif

#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
#define CLIENT_TCP_ON_OPERATOR_SIDE
#define MASTER_IP_STRING	"192.168.100.96"
#ifdef HTBY_LINK_ON_TCPIP
#define SLAVE_IP_STRING 	"192.168.100.94"
#define MAX_SYS_OAM_TCP_LINK	2
#else
#define MAX_SYS_OAM_TCP_LINK	1
#endif
#define MAX_TCP_GROUP		1
#endif
//... newly added end

#endif //(OS ==  LINUX_COMPATIBLE)

#ifndef NO_OPRTR
#define NO_OPRTR	1
#endif

#define IN_DEPTH_BACKTRACK
#define SEND_ANNOUNCEMENT_INSTEAD_OF_CONGESTION
#define DO_NOT_WRITE_TO_SCREEN

#if ((defined RSU_CCSV5_INSTALLED) && (!defined WE_DO_HAVE_AN_V52))
#define RMT_EXT
#endif

#ifdef RSU_CCSV5_INSTALLED
#define GENERIC_RSU	   // as long as all rsu are same type, we may use this one
//#define FIRST_RSU_EXT_NO		  (NO_EXTENSION-NO_RSU_EXT-NO_KIATEL_GSM_SUBSCRIBER-NO_V52_SUBSCRIBER)
#define FIRST_RSU_EXT_NO		(NO_ANALOG_EXTENSION)
#define NO_CCSV5_CHANNEL		(NO_CCSV5_DIGITAL_TRUNK * 30)
#define NO_CCSV5_GROUP			NO_RSU
#ifdef SYSTEM_SIDE
#define HDLC_9076
#endif
#ifdef CB_INSTALLED
#define RMT_CB
#endif
#else
#define NO_RSU				0
#define NO_RSU_EXT			0
#endif

#if (!defined WE_DO_HAVE_AN_V52)
#define CLID
#define CLID_HW
#define DTMF
#endif // (!defined WE_DO_HAVE_AN_V52)

#ifdef CLID	// if CALL_PROCESSOR, should specify which clid
#define DTMF_CLID
#define V23_CLID  // if dsp_clid, always v23_clid
#endif

#ifdef V23_CLID
#define DSP_CLID
// what happens to three following if rsu is EC512?!
#define CLID_TIMER_FOR_DSP
#define SEND_DATE_AND_TIME
#define CLIDCW	// only in call_processor need be defined and not in rsuside
		// only dsp_clid supports this
#endif

#ifdef DSP_CLID
//#define  DSP_CLID_DEBUG
#endif

#define IMMEDIATE_RINGING	// if LINE_INSTALLED

#define USE_16554
//#define LICENSE_CHECK
//#define BILLING_ON_HARD_DISK

#ifdef DTMF
#define DSP_DTMF
//#define DTMF_CARD_INSTALLED
#endif

#define REANSWER_FOR_LOCAL_CALL

#ifdef WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
#define SEND_PRIMARY_DTMF_ON_CO
#define PRIMARY_NET_Q931_ON_E1
#ifdef SAIAN_ERTEBAT
#define NO_PRIMARY_ISDN_USER		1
#elif (defined KIATEL_TEST3)
#define NO_PRIMARY_ISDN_USER		4
#else
#define NO_PRIMARY_ISDN_USER		2
#endif
#define NO_EXTENSION_PER_PRIMARY_USER	30
#define PRIMARY_NET_Q931
#define PRIMARY_NET_ISDN_INSTALLED
#endif

#ifdef WE_DO_HAVE_BASIC_NETSIDE_ISDN
#define BASIC_NET_Q931
#define LOCAL_BASIC_NET_ISDN_INSTALLED
#define BASIC_NET_ISDN_INSTALLED
#define ISDN_CARD_INSTALLED
#define NO_ISDN_PORT_PER_CARD		8
#define NO_EXTENSION_PER_BASIC_USER	1
#endif


#if ((defined WE_DO_HAVE_PRIMARY_NETSIDE_ISDN) || (defined WE_DO_HAVE_BASIC_NETSIDE_ISDN))
// following line should be here, is used in q931ces.c for NET_Q931 in general
#define MAX_DATA_LINK_PER_LAPD_BASIC_NETSIDE_Q931_HW_CHANNEL	   2	// quite fishy!, for q931ces.c
#define NET_Q931_LAPD
#define ISDN_USER_HAS_POTS_SUPPLEMENTARY_SERVICES
#define DYNAMIC_EXT_INSTALLED
#define MORE_TIMERS_FOR_CR
#define NET_Q931
#define NET_Q931_FOR_CP
#define NET_ISDN_INSTALLED
//#define NET_ISDN_SUPPLEMENTARY_SERVICE
//#define NET_Q932
#ifdef LOCAL_BASIC_NET_ISDN_INSTALLED
#define NO_LOCAL_BASIC_ISDN_USER	(NO_ISDN_PORT_PER_CARD*NO_ISDN_CARD)
#else
#define NO_LOCAL_BASIC_ISDN_USER	0
#endif

#ifdef REMOTE_BASIC_NET_ISDN_INSTALLED
#define NO_REMOTE_BASIC_ISDN_USER
#define NO_BASIC_ISDN_USER_PER_RSU
#else
#define NO_REMOTE_BASIC_ISDN_USER	0
#endif

#ifdef BASIC_NET_ISDN_INSTALLED
#define NO_BASIC_ISDN_USER			(NO_LOCAL_BASIC_ISDN_USER+NO_REMOTE_BASIC_ISDN_USER)
#define NO_TOTAL_BASIC_B_CHANNEL		(NO_BASIC_ISDN_USER*2)
#define BASIC_ISDN_STATIC_EXTENSION_START	(ISDN_STATIC_EXTENSION_START)
#define PRIMARY_ISDN_STATIC_EXTENSION_START	(ISDN_STATIC_EXTENSION_START+NO_BASIC_ISDN_USER)
#define NO_BASIC_NETSIDE_STATIC_EXTENSION	(NO_BASIC_ISDN_USER*NO_EXTENSION_PER_BASIC_USER)
#else
#define NO_BASIC_ISDN_USER			0
#define NO_TOTAL_BASIC_B_CHANNEL		0
#define PRIMARY_ISDN_STATIC_EXTENSION_START	ISDN_STATIC_EXTENSION_START
#define NO_BASIC_NETSIDE_STATIC_EXTENSION	0
#endif

#ifdef PRIMARY_NET_ISDN_INSTALLED
#define FIRST_PRIMARY_ISDN_USER 		NO_BASIC_ISDN_USER
//#define NO_PRIMARY_ISDN_USER			  1
#define NO_TOTAL_PRIMARY_B_CHANNEL		(NO_PRIMARY_ISDN_USER*30)
#define NO_PRIMARY_NETSIDE_STATIC_EXTENSION	(NO_PRIMARY_ISDN_USER*NO_EXTENSION_PER_PRIMARY_USER)
#else
#define NO_PRIMARY_ISDN_USER			0
#define NO_TOTAL_PRIMARY_B_CHANNEL		0
#define NO_PRIMARY_NETSIDE_STATIC_EXTENSION	0
#endif

#define NO_ISDN_USER			(NO_BASIC_ISDN_USER+NO_PRIMARY_ISDN_USER)
#define NO_TOTAL_B_CHANNEL		(NO_TOTAL_BASIC_B_CHANNEL+NO_TOTAL_PRIMARY_B_CHANNEL)

#ifdef ISDN_CARD_INSTALLED

#if ( NO_ISDN_PORT_PER_CARD >= 4 )
#define MAX_LAPD_HW_CHANNEL_SCAN	4
#else
#define MAX_LAPD_HW_CHANNEL_SCAN	NO_ISDN_PORT_PER_CARD
#endif

#define OPCARD_MUX1_HDLC_CH		2	// D,C,B1,B2 (B1 is used   for 64kbit/s data)
#define OPCARD_MUX1_HDLC_ST		27
#define NO_ISDN_MUX			1
#define ISDN_CARD_START_SLOT		1
#define NO_ISDN_CARD			1
#define HDLC_MUX_ON_OPCARD
#define OPCARD_HDLC_MUX_8952
#endif // ISDN_CARD_INSTALLED

#define LAPD_INSTALLED

#ifdef LAPD_INSTALLED

#ifdef BASIC_NET_Q931
#define MAX_DATA_LINK_PER_LAPD_BASIC_NETSIDE_Q931_HW_CHANNEL	   2
#define NO_LAPD_BASIC_NETSIDE_Q931_HW_CHANNEL  (NO_ISDN_PORT_PER_CARD*NO_ISDN_CARD)
#define NO_LAPD_BASIC_NETSIDE_Q931_CHANNEL     (NO_LAPD_BASIC_NETSIDE_Q931_HW_CHANNEL* \
						MAX_DATA_LINK_PER_LAPD_BASIC_NETSIDE_Q931_HW_CHANNEL)
#endif

#ifdef PRIMARY_NET_Q931
#define LAPD_9076
#define PRIMARY_LAPD
#define NETSIDE_PRIMARY_LAPD
//#define PRIMARY_LAPD_ON_TS1
#define PRIMARY_LAPD_ON_TS16
#define NO_NETSIDE_PRIMARY_DIGITAL_TRUNK       (NO_PRIMARY_ISDN_USER)
#define NO_TRUNK_PER_NETSIDE_PRIMARY_LAPD_LINK 1
#define NO_LAPD_PRIMARY_NETSIDE_Q931_HW_CHANNEL (NO_PRIMARY_ISDN_USER)
#define MAX_DATA_LINK_PER_LAPD_PRIMARY_NETSIDE_Q931_HW_CHANNEL 1
#define NO_LAPD_PRIMARY_NETSIDE_Q931_CHANNEL	(NO_LAPD_PRIMARY_NETSIDE_Q931_HW_CHANNEL* \
						 MAX_DATA_LINK_PER_LAPD_PRIMARY_NETSIDE_Q931_HW_CHANNEL)
#endif // PRIMARY_NET_Q931

#endif // LAPD_INSTALLED

#endif	// ((defined WE_DO_HAVE_PRIMARY_NETSIDE_ISDN) || (defined WE_DO_HAVE_BASIC_NETSIDE_ISDN))

#ifdef WE_DO_HAVE_USERSIDE_ISDN
#define TIMER_FOR_UPRI_CALL

//#define LAPD_DEBUG_ACT
//#define PRINT_RECEIVED_LAPX_PACKET
//#define PRINT_TRANSMITTED_LAPX_PACKET
//#define PRINT_RECEIVING_Q931_PRIMARY_PACKET
//#define LAPD_HW_DEBUG_ACT

#define USERSIDE_NO_ISDN_USER			NO_USERSIDE_PRIMARY_DIGITAL_TRUNK

#define NO_OF_ENOUGH_DIGITS_FOR_SETUP		3  // make it 3, in order to at least dial two digits after PRI_REQ to check STD(0) and international(00)
#define PRI_REQ 		0x08

#define USERSIDE_PRIMARY_LAPD
#define PRIMARY_LAPD
//#define PRIMARY_LAPD_ON_TS1
#define PRIMARY_LAPD_ON_TS16
//define USERSIDE_NO_ISDN_INTERFACE		1      not used
#define USERSIDE_LOCAL_ISDN_INSTALLED
#define USERSIDE_PRIMARY_ISDN_INSTALLED
#define USERSIDE_PRIMARY_Q931
#define USERSIDE_Q931
#define USERSIDE_NO_TOTAL_B_CHANNEL		(NO_USERSIDE_PRIMARY_DIGITAL_TRUNK*30)

//#define MORE_TIMERS_FOR_CR
//#define USERSIDE_PRIMARY_ISDN_SUPPLEMENTARY_SERVICE
//#define USERSIDE_Q932
#define NO_TRUNK_PER_USERSIDE_PRIMARY_LAPD_LINK 1

//#define USERSIDE_NO_ISDN_SUBSCRIBER		  USERSIDE_NO_ISDN_USER   // only used in this header, not used anymore
#define DYNAMIC_EXT_INSTALLED
#define LAPD_INSTALLED

#ifdef LAPD_INSTALLED
//#define LAPX_USER_SIDE
#define LAPD_9076
#define NO_LAPD_USERSIDE_Q931_HW_CHANNEL	(USERSIDE_NO_ISDN_USER)
#define MAX_DATA_LINK_PER_LAPD_USERSIDE_Q931_HW_CHANNEL 1
#define NO_LAPD_USERSIDE_Q931_CHANNEL		(NO_LAPD_USERSIDE_Q931_HW_CHANNEL* \
						 MAX_DATA_LINK_PER_LAPD_USERSIDE_Q931_HW_CHANNEL)
#endif
#endif	// WE_DO_HAVE_USERSIDE_ISDN



#ifdef WE_DO_HAVE_GSM

//#define LAPD_DEBUG_ACT	  // bcast.c,lapdl3.c,lapx.c,
				// lapdrtns.c,lapdcme.c,lapdlme.c
//#define GSM_DEBUG_ACT 	  // abis_ic.c,abis_og.c,abisrtcp.c,abispkt3.c
//#define GSM_MM_RR_DEBUG_ACT	  // abismm.c,abisrr.c,abisch.c
//#define CR_DEBUG_ACT		  // cr123.c, cr46789.c
//#define CO_DEBUG_ACT		  // cr202124.c, cr252629.c
//#define SS7_DEBUG_ACT 	  // isup_og.c, isup_ic.c

//#define IMEI_CHECK
#define NO_GSM_INTERFACE		1
#define NO_SUBSCRIBER_PER_GSM_INTERFACE 120  //500 //32
#define NO_TRUNK_PER_GSM_INTERFACE	1

#define NO_KIATEL_GSM_SUBSCRIBER	(NO_GSM_INTERFACE*NO_SUBSCRIBER_PER_GSM_INTERFACE)
#define GSM_AGENT_SUPPLEMENTARY_SERVICE
#define KIATEL_GSM_AGENT

// start of block for old_cc
#define GSM_SUBSCRIBER_START		(NO_ANALOG_EXTENSION+NO_RSU_EXT)
#define GSM_INSTALLED
// end of block for old_cc

#define NO_GSM_CELL_REC 	3
#define MAX_NO_TRX_PER_CELL	1	// 1+1+1

#define CELL_0_NO_TRX		1
#define CELL_1_NO_TRX		1
#define CELL_2_NO_TRX		1

#define CELL_0_CELL_NO		0
#define CELL_1_CELL_NO		1  //2
#define CELL_2_CELL_NO		2

#define CHANNEL_CONFIG_1	0	// BCCH+FCCH+SCH+CCCH
#define CHANNEL_CONFIG_2	1	// BCCH+FCCH+SCH+CCCH+SDCCH/4+SACCH/4
#define CHANNEL_CONFIG_3	2	// BCCH+FCCH+SCH+CCCH+SDCCH/4+SACCH/4+CBCH
#define CHANNEL_CONFIG_4	3	// BCCH+CCCH
#define CHANNEL_CONFIG_5	4	// SDCCH/8+SACCH/8
#define CHANNEL_CONFIG_6	5	// SDCCH/8+SACCH/8+CBCH
#define CHANNEL_CONFIG_7	6	// TCH/F+FACCH/F+SACCH/F
#define CHANNEL_CONFIG_8	7	// TCH/FH+FACCH/FH+SACCH/FH

#define CHANNEL_CONFIG_CELL_0_TRX_0_TS0  CHANNEL_CONFIG_2
#define CHANNEL_CONFIG_CELL_0_TRX_0_TS1  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_0_TS2  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_0_TS3  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_0_TS4  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_0_TS5  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_0_TS6  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_0_TS7  CHANNEL_CONFIG_7

#define CHANNEL_CONFIG_CELL_0_TRX_1_TS0  CHANNEL_CONFIG_5
#define CHANNEL_CONFIG_CELL_0_TRX_1_TS1  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_1_TS2  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_1_TS3  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_1_TS4  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_1_TS5  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_1_TS6  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_1_TS7  CHANNEL_CONFIG_7

#define CHANNEL_CONFIG_CELL_0_TRX_2_TS0  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_2_TS1  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_2_TS2  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_2_TS3  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_2_TS4  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_2_TS5  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_2_TS6  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_2_TS7  CHANNEL_CONFIG_7

#define CHANNEL_CONFIG_CELL_0_TRX_3_TS0  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_3_TS1  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_3_TS2  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_3_TS3  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_3_TS4  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_3_TS5  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_3_TS6  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_0_TRX_3_TS7  CHANNEL_CONFIG_7

#define CHANNEL_CONFIG_CELL_1_TRX_0_TS0  CHANNEL_CONFIG_2
#define CHANNEL_CONFIG_CELL_1_TRX_0_TS1  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_0_TS2  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_0_TS3  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_0_TS4  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_0_TS5  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_0_TS6  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_0_TS7  CHANNEL_CONFIG_7

#define CHANNEL_CONFIG_CELL_1_TRX_1_TS0  CHANNEL_CONFIG_5
#define CHANNEL_CONFIG_CELL_1_TRX_1_TS1  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_1_TS2  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_1_TS3  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_1_TS4  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_1_TS5  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_1_TS6  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_1_TRX_1_TS7  CHANNEL_CONFIG_7

#define CHANNEL_CONFIG_CELL_2_TRX_0_TS0  CHANNEL_CONFIG_2
#define CHANNEL_CONFIG_CELL_2_TRX_0_TS1  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_0_TS2  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_0_TS3  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_0_TS4  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_0_TS5  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_0_TS6  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_0_TS7  CHANNEL_CONFIG_7

#define CHANNEL_CONFIG_CELL_2_TRX_1_TS0  CHANNEL_CONFIG_5
#define CHANNEL_CONFIG_CELL_2_TRX_1_TS1  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_1_TS2  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_1_TS3  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_1_TS4  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_1_TS5  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_1_TS6  CHANNEL_CONFIG_7
#define CHANNEL_CONFIG_CELL_2_TRX_1_TS7  CHANNEL_CONFIG_7

#define FREQ_ONE		1	// 1 ... 124
#define FREQ_18 		18	// 1 ... 124
#define FREQ_21 		21	// 1 ... 124

#define FREQ_3			3	// 1 ... 124
#define FREQ_20 		20	// 1 ... 124
#define FREQ_23 		23	// 1 ... 124

#define FREQ_5			5	// 1 ... 124
#define FREQ_7			7	// 1 ... 124

#define KIATEL_ARFCN_CELL_0_TRX_0     FREQ_ONE	   // 1 ... 124
#define KIATEL_ARFCN_CELL_0_TRX_1     FREQ_3	   // 1 ... 124
#define KIATEL_ARFCN_CELL_0_TRX_2     FREQ_5	   // 1 ... 124
#define KIATEL_ARFCN_CELL_0_TRX_3     FREQ_7	   // 1 ... 124

#define KIATEL_ARFCN_CELL_1_TRX_0     FREQ_18	   // 1 ... 124
#define KIATEL_ARFCN_CELL_1_TRX_1     FREQ_20	   // 1 ... 124

#define KIATEL_ARFCN_CELL_2_TRX_0     FREQ_21	   // 1 ... 124
#define KIATEL_ARFCN_CELL_2_TRX_1     FREQ_23	   // 1 ... 124

#define KIATEL_BCCH_ARFCN_CELL_0       FREQ_ONE     // 1 ... 124
#define KIATEL_BCCH_ARFCN_CELL_1       FREQ_18	    // 1 ... 124
#define KIATEL_BCCH_ARFCN_CELL_2       FREQ_21	    // 1 ... 124

#define CELL_0_TRX_0_RSL_TEI_VALUE		       1
#define CELL_0_TRX_1_RSL_TEI_VALUE		       2	// 4
#define CELL_0_TRX_2_RSL_TEI_VALUE		       3
#define CELL_0_TRX_3_RSL_TEI_VALUE		       4

#define CELL_1_TRX_0_RSL_TEI_VALUE		       5
#define CELL_1_TRX_1_RSL_TEI_VALUE		       6

#define CELL_2_TRX_0_RSL_TEI_VALUE		       7
#define CELL_2_TRX_1_RSL_TEI_VALUE		       8

//#define NOT_USE_TCH_IN_TS0	  // just for the cases that we have programmed
// the second and onward trx in each cell to have 8 traffic channels, however
// bts itself has been commissioned to have just 7 traffic channel per trx even
// if all are in one cell

#define STAR_TRAU_CONFIG
#define MAX_NO_TRAU_SYS 	    (NO_GSM_CELL_REC*MAX_NO_TRX_PER_CELL)
//#define NO_TRAU_PER_TRAU_SYS	  7	not used anymore
#define MAX_TRAU_PER_TRAU_SYS	8	// used only for some initialization value


#define BTS_LAPD_ON_TS1

//#define DYNAMIC_TRAU
#define STATIC_TRAU

#define AGENT_VOICE_PATH_CONNECTION_ON_REQUEST
#define CC_VOICE_PATH_CONNECTION_ON_REQUEST

//#define WE_DO_HAVE_SPARE_TRAU   // not supported for new_cc
//#define USE_GSM_TRAU_ON_DIGITAL_TRUNK_FOR_DUMP
#define GSM_TRAU_ON_DIGITAL_TRUNK
//#define TRAU_LOOPBACK

//#define GSM_TRAU_ON_DSP

#ifdef GSM_TRAU_ON_DSP
#define GSM_TRAU_ON_DSP_64KBIT_STBUS	12   // first st-bus of one card before last one
#define GSM_TRAU_ON_DSP_16KBIT_STBUS	(GSM_TRAU_ON_DSP_64KBIT_STBUS+1)
#endif

//#define PRINT_LOW_LEVEL_PACKETS

#define SIEMENS_BS240
//#define NO_TCH_PER_TRX		  7
// above not used anymore, used to be used only for some initialization value
//#define NO_SDCCH_PER_TRX		  4
// above not used anymore, used to be used only for some initialization value
//#define GSM_TEST
#ifdef GSM_TEST
#define CHAHAR_TA_YEKI
#endif

#define KIATEL_OAM_SIGNALING_TSC	7
#define KIATEL_OAM_TRAFFIC_TSC		7
#define KIATEL_SDCCH_TSC		7
#define KIATEL_TCH_TSC			7
// BCCH tsc should be same as KIATEL_GSM_BS_COLOR_CODE in abisoam.h

#define KIATEL_GSM_MS_POWER			5
//========================================
//????
// the time the MS shall wait before attepmting another channel request.
// this field is coded as the binary representation of the T3122 timeout value
// in seconds.
#define GSM_T3122_TIMEOUT_VALUE 		3 // is sent in immediate assignment reject
//========================================
//siemens(10.5.2.4)
#define KIATEL_GSM_CELL_RESELECT_HYSTERESIS	RXLEV_6_DB_FOR_LA_RESELECT
#define KIATEL_GSM_MS_TXPWR_MAX_CCH		0
#define KIATEL_GSM_RXLEV_ACCESS_MIN		0
//========================================
//siemens(10.5.2.29)
#define KIATEL_GSM_RACH_CNTRL_MAX_NO_RETX	MAX_1_RETX
#define KIATEL_GSM_RACH_CNTRL_NO_SLOTS_2_SPREAD_TX SPREAD_TX_ON_8_SLOTS
//========================================
//siemens(10.5.2.11)
#define KIATEL_GSM_BS_AG_BLKS_RES		0 // no blocks for access grant
#define KIATEL_GSM_BS_PA_MFRMS			3 // multiframe period for transmission of paging request messages to the same paging subgroup.
						  // 0 means 2 multiframes
						  // 1 means 3 multiframes
						  // ......
						  // 7 means 9 multiframes
#define KIATEL_GSM_BS_PA_MFRMS_VALUE		(KIATEL_GSM_BS_PA_MFRMS+2)
#define KIATEL_GSM_CCCH_CONF			ONE_CHANNEL_FOR_CCCH_COMBINED_WITH_SDCCH // no physical channels used for CCCH (combined with SDCCH or not)
#define KIATEL_GSM_T3212_TIMEOUT_VALUE		0 // timeout value for periodic updating in decihours.(1-255)
//========================================
//siemens(10.5.2.3.)
#define KIATEL_GSM_RADIO_LINK_TIMEOUT		0x08 // 36
//========================================
#define WHERE_TO_PUT_AGCH_TS_NO 		0
#define WHERE_TO_PUT_PCH_TS_NO			0
#define WHERE_TO_PUT_BCCH_TS_NO 		0
//========================================
//siemens(10.5.2.28)
#define KIATEL_GSM_MS_DEFAULT_POWER_LEVEL	3
//========================================
//Mobile Country Code
#define KIATEL_GSM_MCC_DIGIT_1			4
#define KIATEL_GSM_MCC_DIGIT_2			3
#define KIATEL_GSM_MCC_DIGIT_3			2
//Mobile Network Code
#define KIATEL_GSM_MNC_DIGIT_1			1
#define KIATEL_GSM_MNC_DIGIT_2			2
//siemens Location Area Code
#define KIATEL_GSM_LAC				0x0001
//siemens Identity Value (10.5.1.1)
#define CELL_0_IDENTITY_VALUE			  0x0001
#define CELL_1_IDENTITY_VALUE			  0x0002
#define CELL_2_IDENTITY_VALUE			  0x0003
//========================================
#define MAX_NO_ABIS_TRX_REC			(NO_GSM_CELL_REC*MAX_NO_TRX_PER_CELL)
#define MAX_NO_ABIS_SDCCH_REC			(NO_GSM_CELL_REC*MAX_NO_TRX_PER_CELL*8)  // having maximum of either one sdcch/8 timeslot or two sdcch/4 timeslot in each trx of each cell
#define MAX_NO_ABIS_TCH_REC			(NO_GSM_CELL_REC*MAX_NO_TRX_PER_CELL*8)  // all timeslots be traffic in each trx of each cell
#define MAX_NO_KIATEL_GSM_PCH_REC		(NO_GSM_CELL_REC*1) // having maximum of one ccch timeslot in each trx of each cell

#ifdef STATIC_TRAU
#define MAX_NO_GSM_TRAU_REC			MAX_NO_ABIS_TCH_REC
#else
#define NO_GSM_TRAU_REC 			7
#endif

#define NO_ABIS_SDCCH_GROUP			NO_GSM_CELL_REC //1
#define NO_ABIS_TCH_GROUP			NO_GSM_CELL_REC //1

#define MO_EARLY_ASSIGNMENT
//#define MO_LATE_ASSIGNMENT

#define MAX_PAGING_REC_PER_SUBSCRIBER		MAX_NO_KIATEL_GSM_PCH_REC // (suppose that ms can be paged on all trx)
#define OAM_DEFAULT_PCKT_RESPONSE_COUNT 	(10*(1000/20))
#define KIATEL_GSM_RACK_0			0
#define KIATEL_GSM_BTS_0			0

//#define KIATEL_GSM_TRX_0			  0	not used anymore
//#define KIATEL_GSM_TRX_1			  1	not used anymore
//#define KIATEL_GSM_TRX_2			  2	not used anymore

//#define KIATEL_GSM_NO_RADIO_TRAFFIC_CHANNEL	  NO_ABIS_TCH_REC // 7	not used anymore
//#define KIATEL_GSM_NO_RADIO_SIGNALING_CHANNEL   (1*NO_GSM_CELL_REC) // 1   not used anymore
//#define NO_RADIO_TRAFFIC_CHANNEL_PER_TRX	  7	  // not used anymore
//#define NO_RADIO_SIGNALING_CHANNEL_PER_TRX	  1	  // not used anymore


#define NO_ABIS_OAM_REC 			1
//#define NO_ABIS_RADIO_SIGNALING_REC		  KIATEL_GSM_NO_RADIO_SIGNALING_CHANNEL // 1
//#define NO_ABIS_RADIO_TRAFFIC_REC		  KIATEL_GSM_NO_RADIO_TRAFFIC_CHANNEL // 7

#define NO_ABIS_RADIO_SIGNALING_REC		(NO_GSM_CELL_REC*MAX_NO_TRX_PER_CELL*1) // having maximum of one timeslot in each trx of each cell which carries BCCH/CCCH/DCCH
#define NO_ABIS_RADIO_TRAFFIC_REC		MAX_NO_ABIS_TCH_REC



//#define OML_TEI_VALUE 		    20		not used anymore


#define GSM_ABIS_ON_DIGITAL_TRUNK
#define ABIS_LAPD_ON_DIGITAL_TRUNK
//#define ABIS_FIRST_OAM_TEI_VALUE		  20
#define ABIS_FIRST_OAM_TEI_VALUE		18
//#define MAX_ABIS_RSL_DATA_LINK_FOR_PAGING	1
//#define MAX_HDLC_PER_DIGITAL_TRUNK		1
#define NO_ABIS_DIGITAL_TRUNK			1

#ifdef GSM_TRAU_ON_DIGITAL_TRUNK
//#define NO_TRAU_SYS_PER_E1	  1
//#define NO_TRAU_DIGITAL_TRUNK   (MAX_NO_TRAU_SYS) //1

#define NO_TRAU_SYS_PER_E1	3
#define NO_TRAU_DIGITAL_TRUNK	((MAX_NO_TRAU_SYS+NO_TRAU_SYS_PER_E1-1)/(NO_TRAU_SYS_PER_E1))
// may be the actual needed number of e1 to be used and initialized should be
// calculated instead and not set to some maximum value. the actual needed
// value is indeed NO_TRAU_SYS
#else
#define NO_TRAU_DIGITAL_TRUNK			0
#endif

#define NO_TRUNK_PER_ABIS_LAPD_LINK		1
//#define NO_ABIS_DATA_LINK			  (1+NO_ABIS_TRX_REC)// 2
#define NO_ABIS_DATA_LINK			(NO_ABIS_OAM_REC+MAX_NO_ABIS_TRX_REC)
//#define NO_TRX_PER_ABIS_HW_LAPD_CHANNEL	  NO_ABIS_TRX_REC // 1	not used anymore

#define LAPD_INSTALLED
#ifdef LAPD_INSTALLED
#define LAPD_9076
//#define NO_LAPD_Q931_HW_CHANNEL		  0
//#define FIRST_LAPD_GSM_HW_CHANNEL		  NO_LAPD_Q931_HW_CHANNEL
#define NO_LAPD_GSM_HW_CHANNEL			1
//#define NO_LAPD_GSM_CHANNEL			  (1+NO_ABIS_TRX_REC) // 2
#define NO_LAPD_GSM_CHANNEL			NO_ABIS_DATA_LINK
//#define NO_MGMT_BCAST_LAPD_CHANNEL		  NO_LAPD_HW_CHANNEL
//#define NO_CC_BCAST_LAPD_CHANNEL		  0
//#define MAX_DATA_LINK_PER_LAPD_GSM_HW_CHANNEL       (1+NO_ABIS_TRX_REC) // 2
#define MAX_DATA_LINK_PER_LAPD_GSM_HW_CHANNEL	    NO_ABIS_DATA_LINK
#endif

#define LAPX_NETWORK_SIDE
#else
#define NO_KIATEL_GSM_SUBSCRIBER	0
#endif // WE_DO_HAVE_GSM

#ifdef WE_DO_HAVE_V52
#define NO_E1_IN_COREDECT	2 // 1

#define KLM_TEST

#define LOGICAL_CHANNEL_0_ID		0
#define KIATEL_INTERFACE_0_VARIANT	0
#define KIATEL_INTERFACE_0_IDENT	0

#define LE_V52_INSTALLED
#define LAPV5_INSTALLED

#ifdef LE_V52_INSTALLED
#if (NO_E1_IN_COREDECT >= 2)
#define V52_PROTECTION_SUPPORTED
#endif
#define NO_V52_INTERFACE		1
#define NO_LINK_PER_V52_INTERFACE	NO_E1_IN_COREDECT
#ifdef V52_PROTECTION_SUPPORTED
#define NO_HW_CHANNEL_PER_V52_INTERFACE 2
#else
#define NO_HW_CHANNEL_PER_V52_INTERFACE 1
#endif
#define NO_V52_DIGITAL_TRUNK		(NO_V52_INTERFACE*NO_LINK_PER_V52_INTERFACE)
#define NO_V52_LINK			NO_V52_DIGITAL_TRUNK
#define LE_V52_EXTENSION_START		(NO_ANALOG_EXTENSION+NO_RSU_EXT+NO_KIATEL_GSM_SUBSCRIBER)
#define NO_PSTN_PORT_PER_V52_INTERFACE	600
#define NO_V52_PSTN_PORT		(NO_V52_INTERFACE*NO_PSTN_PORT_PER_V52_INTERFACE)
#define NO_V52_SUBSCRIBER		NO_V52_PSTN_PORT
#define NO_V52_BEARER_CHANNEL		((30*NO_E1_IN_COREDECT)*NO_V52_INTERFACE)
#define NO_V52_LOGICAL_CHANNEL		NO_V52_INTERFACE
#ifdef V52_PROTECTION_SUPPORTED
#define NO_V52_PHYSICAL_CHANNEL 	(2*NO_V52_INTERFACE)
#else
#define NO_V52_PHYSICAL_CHANNEL 	(1*NO_V52_INTERFACE)
#endif
#endif

#ifdef LAPV5_INSTALLED
#define LAPV5_9076
#define NO_LAPV5_HW_CHANNEL		(NO_V52_PHYSICAL_CHANNEL)
#ifdef V52_PROTECTION_SUPPORTED
#define NO_LAPV5_CHANNEL		(6*NO_V52_INTERFACE) // LAPV5_CNTRL_DL,LAPV5_PSTN_DL,LAPV5_BCC_DL,LAPV5_LNKCNTRL_DL,LAPV5_PTCN1_DL,LAPV5_PTCN2_DL
#else
#define NO_LAPV5_CHANNEL		(4*NO_V52_INTERFACE) // LAPV5_CNTRL_DL,LAPV5_PSTN_DL,LAPV5_BCC_DL,LAPV5_LNKCNTRL_DL
#endif
#endif

#define LAPX_NETWORK_SIDE
#endif // WE_DO_HAVE_V52

#if ((!defined WE_DO_HAVE_V52) && (!defined WE_DO_HAVE_AN_V52))
#define NO_V52_SUBSCRIBER		0
#endif


//#define CLOCK_8MHZ_USED_FOR_16550

#ifdef WE_DO_HAVE_SS7
#define ISUP_ON_E1
#define CAN_FLASH_ON_ISUP
#define SS7NET_DEBUG_ACT
//#define NO_LOAD_SHARING_ON_ROUTES	   // packets go on the main route (which is adjacent)

#define NEW_PRINTER

//#define FISU_CHECK			// strategy is mtpdaedr just to check for repeatitive fisu
#define FISU_LSSU_CHECK 		// new strategy in mtpdaedr to check for both repeatitive fisu and lssu
//#define PRINT_LOW_LEVEL_PACKETS	  // print sending and receiving fisu,lssu with time tag
//#define PRINT_FOR_LOW_LEVEL_MTP_RTNS	  // activate DEBUG_ACT of mtpiac and mtplsc
//#define PRINT_SENDING_PACKET	  // in mtpdaedt, print sending MSU
//#define PRINT_RECEIVING_PACKET  // in mtprc, print receiving MSU

#define NO_OF_ENOUGH_DIGITS_FOR_IAM	4
#define SEND_IAM_THEN_SAM
//#define SEND_ONLY_IAM
#define DARYA		 // SITE specific, 1 or more  links, 1 dest point
#define SS7_REALLY_ON_TS1
#define NET_CNFG_FILE		"bhcnfg"

#define KIATEL_SIGNALING_POINT_CODE	((1<<10) | 5) //DEST_CODE_0
//to be moved to site headers
// ==============================
#define DEST_CODE_0		((1<<10) | 1)
//#define DEST_CODE_1		  ((1<<10) | 2)
//#define DEST_CODE_2		  0x02
//#define DEST_CODE_3		  0x03
#define NO_DEST_POINT		1
#define NO_ADJACENT_SP		1
#define NO_ROUTE		1
#define NO_LINK 		1
#define NO_LINKSET		1
#define NO_CMB_LINKSET		1

//adj_sp_st[][]
#define NO_ROUTE_VIA_STP	0  // 0->1
#define NO_ISUP_VOICE_GROUP	1
// ===============================

//#define REVERSE_ISUP_CHANNEL_LINKLIST
#define NO_OF_COMPLETE_ISUP_DIGITS		11
#define MTP_INTERFRAME_TIME_FILL
#define SS7
#ifdef SS7

#define 	NO_SLS					16
//#define	  NO_ISUP_GROUP 			  3

#define MTP
#define ISUP
#define EXTENDED_LX_ISUP
#define EXTENDED_LX_BULK_BILLING_CONVERSATION_TIMER    (50 * 180)    /* 180 sec */
#define SEND_SERVICE_TO_ISUP
#define MTP_9076
#define NO_MTP		NO_LINK
//#define NO_SS7_GROUP	  1
#endif
#ifdef ISUP
#define NO_ISUP_DIGITAL_TRUNK	2
#define ISUP_REQ		0x08
#endif

#ifdef SS7
#define NO_SS7_DIGITAL_TRUNK	NO_ISUP_DIGITAL_TRUNK
#else
#define NO_SS7_DIGITAL_TRUNK	0
#endif
#endif // WE_DO_HAVE_SS7

#define NEW_DR_STRATEGY
#define HOME_METERING
#define STAND_ALONE_EC1K
#define EC1K_SYSTEM
#define PAGE_ADDRESSING_ON_EC1K
#define MT90826_INSTALLED

#if (	\
      (defined RSUSIDE) || \
      (defined WE_DO_HAVE_AN_V52) || \
      (defined RMT_EXT) || \
      (defined M2UA_ON_SGSIDE) || \
      (defined ISUP_ON_H248_MGSIDE) || \
      (defined PRIMARY_NET_Q931_ON_H248_MGSIDE) || \
      (defined RSU_CCSV5_INSTALLED) || \
      (defined WE_DO_HAVE_V52) || \
      (defined WE_DO_HAVE_CAS3) || \
      (defined WE_DO_HAVE_SS7) || \
      (defined WE_DO_HAVE_GSM) || \
      (defined WE_DO_HAVE_USERSIDE_ISDN) || \
      (defined WE_DO_HAVE_PRIMARY_NETSIDE_ISDN) || \
      (defined CASV5_CO) \
    )
#define CEPT_INSTALLED
#define MT9076
#endif

#if (!defined WE_DO_HAVE_AN_V52)
#define XST_DGT_RCVR

#ifdef CEPT_INSTALLED
#define CEPT_DGT_RCVR
#define CEPT_DGT_SEND
#endif

#endif // (!defined WE_DO_HAVE_AN_V52)

//#define DT_CARD_INSTALLED	 no need to define, defined in dsmtfp.h if necessary
#ifdef MT9076
#define MT9076_INSTALLED
#endif

#define PC_COMPATIBLE
#define DEBUG_ON_MONITOR
#define KEYBOARD_INSTALLED
#define WARNING_DISABLE
#define NEW_HEADER
#define LINE_INSTALLED
#define CPU_LOAD_CHECK		// should be defined in order that alarm panel works
#define DISPLAY_ALARM
#define SEND_ANNOUNCEMENT
#define DELAYED_SCAN
#if ((NO_OPRTR != 0) || (defined KEYBOARD_INSTALLED))
#define BACKUP_FILE
#endif
#define NEW_DEP
#define NO_MT8952		0

#if (NO_OPRTR != 0)
#if ((!defined TCPIP_ON_SYS_OAM_SYSTEM_SIDE) && (!defined MY_TCPIP_FOR_OAM) && (!defined MY_UDP_FOR_OAM) /*&& (!defined KIATEL_UDP_FOR_OAM)*/)
#define 	HDLC_RS232
#endif
#endif

#if ((defined RSU_CCSV5_INSTALLED) || (defined HDLC_RS232) || (defined MY_UDP_FOR_OAM))
#define LAPB
#endif

// ?????
//#define BEHNAMIR

#ifdef SYSTEM_SIDE
#ifdef HDLC_RS232
#define 	FIRST_RS232_HDLC_CH		NO_RSU
#define 	LAST_RS232_HDLC_CH		(FIRST_RS232_HDLC_CH+NO_NEW_HDLC_COM_PORT)
// NO_SRL_LINK= No_operator + alarm panel port
#endif
#endif
#ifdef OPERATOR_SIDE
#ifdef HDLC_RS232
#define HDLC_RS232_ON_OPERATOR_SIDE
#define 	FIRST_RS232_HDLC_CH		0
#define 	LAST_RS232_HDLC_CH		(FIRST_RS232_HDLC_CH+NO_NEW_HDLC_COM_PORT)
#endif
#endif

#ifdef MY_UDP_FOR_OAM
#ifdef SYSTEM_SIDE
#define FIRST_UDP_HDLC_CH	NO_RSU
#define NO_HDLC_UDP_PORT	NO_OPRTR
#define LAST_UDP_HDLC_CH	(FIRST_UDP_HDLC_CH+NO_HDLC_UDP_PORT)
#endif
#ifdef OPERATOR_SIDE
#define FIRST_UDP_HDLC_CH	0
#ifdef HOT_STANDBY
#define NO_HDLC_UDP_PORT	2
#else
#define NO_HDLC_UDP_PORT	1
#endif
#define LAST_UDP_HDLC_CH	(FIRST_UDP_HDLC_CH+NO_HDLC_UDP_PORT)
#endif
#endif

//#define	  COM1
//#define	  LOW_PRIORITY		  // if defined, this station assumes lower priority in contention case, otherwise assumes higher

#if (((defined KFTPD_INSTALLED) && (defined SYSTEM_SIDE)) || \
     (defined MY_TCPIP_FOR_OAM)|| \
     ((defined MY_UDP_FOR_OAM) && (OS == DOS)) || \
     ((defined KIATEL_UDP_FOR_OAM) && (defined KIATEL_UDP_INSTALLED) && (defined SYSTEM_SIDE)) || \
     ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
#define SOFTWARE_INTERRUPT_VECTOR		0x7e
#define ARP_INSTALLED
#define IP_INSTALLED
#define ETHERNET_INSTALLED
#define MAX_IFACE	PK_MAX
#define PK_MAX		1
#define NO_IP_ROUTE	10
#endif
#if ((defined MY_TCPIP_FOR_OAM) || ((defined KFTPD_INSTALLED) && (defined SYSTEM_SIDE)))
#define TCP_INSTALLED
#endif
#if (((defined MY_UDP_FOR_OAM) && (OS == DOS)) || \
    ((defined KIATEL_UDP_FOR_OAM) && (defined KIATEL_UDP_INSTALLED) && (defined SYSTEM_SIDE)) || \
    ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)))
#define UDP_INSTALLED
#endif

#if (defined ALARM_PANEL_ON_OTHER_RS232)
#define 	SERIAL_PORT	      // hardware (layer 1) of hot/standby link or alarm panel link
#define 	POLL_UART_16550
#endif

#if ((defined HDLC_RS232) || (defined SERIAL_PORT))
#define 	UART_16550
// in HDLC_RS232,
// if we do not use USE_INTERRUPT_FOR_HDLC_RS232 (which we don't),
// then above is used only in init_com_port(), smllyr1.c
// in SERIAL_PORT,
// if we do not use interrupt (which we don't by defining POLL_UART_16550),
// then above is used only in init_serial_port(), layer1.c
#endif

#define 	FIFO_RD_WR
#define 	BATCH_PROGRAMMING
#ifdef BATCH_PROGRAMMING
#define 	NEW_ANALOG_TRUNK_GROUPING
#endif

//#define TCI_DPI	  /* to include proper "dpi" info. */
//#define TCI_CAS3	  /* to include proper "dti" info. */

#include "/ec/work/spec/reverse.h"

#ifndef DO_NOT_DEFINE_TCI	// really stupid!!
#define TCI		/* to include installation name */
#endif
#define SEND_ANNOUNCEMENT
#if ((!defined WE_DO_HAVE_PRIMARY_NETSIDE_ISDN) && (!defined WE_DO_HAVE_USERSIDE_ISDN) && (!defined USE_EXTRNL_CALL_TIMER_FOR_ALL_DOD_CALLS))
// can't define BAD_PAYER with USER PRIMARY, because of common usage of B2 bit
#define BAD_PAYER     // check with send_announcement, also check moh (features) with send_announcement
#endif
#define ALARM_ENABLE
//#define SYSTEM_MANUAL_TEST
#define NEW_DEBUG
#define DEP_MANUAL_TEST

/* installation date: */

#define INSTALLATION_NAME	"TCI"
//#define INSTALLATION_NAME	  "KIATEL FACTORY"
/* max. 20 characters */

/*
#define SI_DEBUG_ACT
*/
/* serial interface debug active, can be put locally in files */
/*
#define SI_DEBUG_Q_ACT
*/
/* serial interface debug queue processing routines, should be defined if
   SI_DEBUG_ACT is defined here or in one/some files */
#if 0
#define SI_DEBUG_ON_U
/* if SI_DEBUG_ACT in some files, then debug info. is sent on U interface
   instead of serial interface */
#ifdef SI_DEBUG_ON_U
#define SI_DEBUG_OPR
/* to which operator link SI_DEBUG data should be sent */
#endif
#endif // 0
			/****  timers macros  ****/

#ifndef DIAL_TICK
#ifdef DEBUG_DSP_DTMF
#define  DIAL_TICK		9000
#else
#define  DIAL_TICK		500
#endif
#endif
		/* Dial timer = 500 * 20 msec */
#define  FLASH_TICK		25 //35 // if flash time is too long
			/* Flash timer = 25*20 msec */
#define  TRNK_RLS_TICK		25
			/* trunk release = 25*20 msec */
#define  TRNK_SZ_TICK		1
			/* trunk seize = 1*20 msec */
#define  TRNK_REDIAL_TICK	75
			/* trunk redial tick = 75 * 20 msec */

#define  HOT_LINE_TRNK_SZ_TICK	(50 * TRNK_SZ_TICK)
	/* hot line trunk seize = 50*20 msec,if TRNK_SZ_TICK is used */
	/* we will have problem in distingushing btwn off hook & flash */

#define  HOLD_TICK		6000
			/* hold timer = 6000*20 msec */
#define  PARK_TICK		(2*HOLD_TICK)
#define  LXD_VALID_TICK 	3000
			/* last ext. dialled valid = 3000 * 20 msec */

#define  ANSWER_TICK		1500
			/* answer timer = 1500*20 msec */
#define  CALL_WAITING_TICK	6000
			/* 6000 * 20 msec = 2 min */
#define  TRUNK_RECALL_TICK	12000
			/* 12000 * 20 msec = 240 sec */
#define  OPR_EXT_ANSWER_TICK	1500
			/* 1500 * 20 msec = 30 sec */
#define  OPR_TRUNK_ANSWER_TICK	1500
			/* 1500 * 20 msec = 30 sec */
#define  OPR_EM_ANSWER_TICK	1500
			/* 1500 * 20 msec = 30 sec */
#define  OPR_CAS3_ANSWER_TICK	1500
			/* 1500 * 20 msec = 30 sec */
#define  ANSWER_2ND_TICK	1500
			/* 1500 * 20 msec = 30 sec. */
/* on 1374/3/7 ANSWER_2ND_TICK changed from 2 min. to 30 sec. */
#define  NITE_OP_ANSWER_TICK	 500
			/*  500 * 20 msec = 10 sec. */

#define SERIAL_NITE_OPERATION
#define NO_CAMP_ON_TO_NITE


#define DEFINE_VALUE	1
/*
#define MH88610
*/
#define MH88612
/*
#define PBL3766
*/
#define PRE_INSTALL
/* If define operator can change operator receive gain,ext. codec gain, */
/* analog trunk gain,E & M trunk gain. */
/* In the final installed software these items can not be changed */

#ifdef CALL_PROCESSOR
#define CONFERENCE_ACT
#endif

#ifdef CONFERENCE_ACT
#define DSP_CONFERENCE
#ifndef DSP_CONFERENCE
//#define CONFERENCE_ST_BUS	  26 /* conference st_bus is dst7 from switching matrix */
//#define CONFERENCE_N_PARTY
//#define TWO_M116
#ifdef TWO_M116
#define CONFERENCE_ST_BUS_2	14
#endif
#endif
#endif

#ifdef SUPPORT_CONFERENCE
#define DSP_CONFERENCE
#endif

#define OPR_START_TIME_DGTS	6
	/* after operator dialled OPR_START_TIME_DGTS digits */
	/* the timing on the call will be activated */
/*
#define SEND_LF
*/
/* send line feed to printer */

	/* number of operator */

/* this operator number */
#include "/ec/work/spec/opr.h"
#ifdef HDLC_8993
#define  OPRT_ADDR	(1 << (TON+1) )
#endif


#define OPR_RCVR_GAIN		0x30
	/* operator receiver gain,-3 db */
#ifdef CALL_PROCESSOR	       // call processing routines included
#define MUSIC_TAPE_RECORDER
#define R1_SIGNALING
#define BILLING_REQUIRED

#define HUNTING_FOR_SUBSCRIBERS
#define HOT_LINE_SERVICE_ACTIVE

#define SIREN_INSTALLED
#ifdef SIREN_INSTALLED
#define SIREN_EXTENSION 	(NO_EXTENSION)
#endif

#define AMPLIFIER_INSTALLED
#ifdef AMPLIFIER_INSTALLED
#define AMPLIFIER_EXTENSION	(NO_EXTENSION)
#define NO_AMPLIFIER_TIMER
#ifdef LINE_INSTALLED
#define ALSO_CLOSE_AMPLIFIER_EXTENSION_RELAY
#endif
#endif
/*
#define MALICIOUS_CALL
*/
	/* malicious call is detected by code (118) and sent on serial */
	/* currently it is sent thru function "si()" which is activated only
	   by SI_FOR_BULK_BILLING. It may easily be sent on U or elsewhere by
	   copying the corresponding code in si() to elsewhere */
#define MALICIOUS_PRINT
#define MALICIOUS_PRINT_ON_U
#ifdef MALICIOUS_PRINT_ON_U
#define MALICIOUS_PRINT_OPR	0
#endif
	/* malicious call is detected by code (118) and sent on printer */
	/* also detected by dialling "4" after flashing and sent on printer */
	/* second method has the advantage of reporting the type of malicious*/
	/* for example CO, TIE, ... */
/*
#define OPR_MALICIOUS_DTCT_ACT
*/
	/* malicious call is detected by operator overriding the suspicious
	   call and sent on op.lcd. (it does not need MALICIOUS_CALL) */
#endif // CALL_PROCESSOR

/*
#define CO_VER
*/


/*
#define EM_INSTALLED
*/
/* E & M trunks installed */

#ifdef EM_INSTALLED
#define EM_CARD_INSTALLED
#define NO_EM
#define EM_CARD_START_SLOT
#define EM_REQ
#define OPR_EM_CONNECTION
/* if defined operator can go on E & M trunks */
#define EM_PULSE_SIGNALING
#define EM_CONTINUOUS_SIGNALING
#define EM_GROUP_DIALLED
/* currently, ifdef EM_CONTINUOUS_SIGNALING, then should define also
   EM_GROUP_DIALLED and vice versa */

#ifdef EM_GROUP_DIALLED
#define NO_EM_GROUP_DIAL_DGT
#define EM_GROUP_DEFAULT
#endif

#define NO_EM_GROUP
/* ifndef EM_GROUP_DIALLED then there should be 2 EM_GROUPs
   for ICT and OGT  */


#ifdef EM_CONTINUOUS_SIGNALING
#define TARASHOH_PROOF
#ifdef TARASHOH_PROOF
#define TARASHOH_PROOF_TIMER	5
/* 5 * 20msec = 100msec */
#endif
#define EM_CONT_RESOLVED_TIMER	 4
/* on bothways E & M trunks,for resolving contention */
/* 25 * 20 msec */
/* 4 * 20 msec = 80 msec */
#define EM_CONT_RESOLVED_TIMER_FOR_SEND 5
/* 5 * 20 msec = 100 msec */
/* should be more than previous timer */

#define SELF_CENTER_TANDEM
/* if define means this center is a tandem center */

#ifdef SELF_CENTER_TANDEM
#define SELF_CENTER_GROUP_NUMBER
#define SELF_CENTER_GROUP_NUMBER_D2D1
#define TRANSIT_CAPABILITY
#define SPECIAL_CASE
	/* if TRANSIT_CAPABILITY, by SPECIAL_CASE, incoming is checked
	   and if is not dest_transit, then all transit requests of it
	   will be routed to TEHRAN (group 0) */
#endif

#ifdef OPR_EM_CONNECTION
#define EM_CONT_SEP_OPR_Q
#define EM_TRUNK_CONNECTION
#endif

#endif


#endif

#ifdef CEPT_INSTALLED
/*
#define CAS1_INSTALLED
*/
#ifdef CAS1_INSTALLED
#define NO_CAS1_DIGITAL_TRUNK	1
#define NO_CAS1_GROUP		1	/* only groups for OG channels arenecessary to be mentioned */
					/* this is used only for allocatin xxchg[]...*/
#define NO_CAS1_OG_CH		15
#define NO_CAS1_IC_CH		15
#define CAS1_REQ		0x0a
#else
#define NO_CAS1_DIGITAL_TRUNK	0
#define NO_CAS1_OG_CH		0
#define NO_CAS1_IC_CH		0
#endif

#ifdef WE_DO_HAVE_CAS3
#define TRANSIT_DT_DT	// only for follow me
#define REANSWER_FOR_CAS3_CALL
#define OPR_CAS3_PROGRAM
#define CAS3_INSTALLED
#endif

#ifdef CAS3_INSTALLED

// in KTNUM && HDLC_RS232, OPR_DT_CONNECTION rings to operator,
// however DT_DIAL_TO_OPR, rings to subscriber
// operator software knows cas3 only by OPR_DT_CONNECTION
//#ifdef ACTIVE_OPR_EXT_NO
#define OPR_DT_CONNECTION
//#else
#define DT_DIAL_TO_OPR
//#endif

#if (!defined EM_HISTORICAL_REPORT)
#define CAS3_HISTORICAL_REPORT
#endif

#ifdef CAS3_HISTORICAL_REPORT
#define CAS3_HISTORICAL_REPORT_CHANNEL_SEIZE_DGT	0x1d
#endif

#define TIMEOUT_FOR_DT_ANSWER	// rls the line if answer does not come in a certain period

#if (!defined DISPLAY_EM_STATUS)
#define DISPLAY_CAS3_STATUS

#ifdef DISPLAY_CAS3_STATUS
#define DISPLAY_CAS3_LINE_STATUS	// also show line status (3 bits)
#endif
#endif


#define NO_CAS3_GROUP		9	/* only groups for OG channels are necessary to be mentioned */
					/* this is used only for allocatin xxchg[]...*/
#define CAS3_ICT_GROUP		(NO_CAS3_GROUP-1)

#define NO_CAS3_IC_CH		 (NO_CAS3_DIGITAL_TRUNK*15)	/* used for std connection */
#define NO_CAS3_OG_CH		 (NO_CAS3_DIGITAL_TRUNK*15)	/* used for std connection */
#define NO_LIM_IC_CH		 0	/* used for lim connection */
#define NO_LIM_OG_CH		 0	/* used for lim connection */
#define TOTAL_NO_CAS3_IC_CH	(NO_CAS3_IC_CH + NO_LIM_IC_CH)

#define CAS3_REQ		0x08

/*
#define TIMER_EXT_ANSWER_IC_CEPT
*/
/* timer before ext. answer IC */

#define TIMER_DGT_RECEIVE_CEPT
/* timer for digit receiver on CEPT */

#if 0
#define TIMER_ON_OGC_ANSWER		/* not used */
#endif // 0
/* set timer on outgoing channel for answer */

/* signaling specification */
/*
#define SIGNALING_WITH_PTS
#define SIGNALING_WITH_OPR_RCL
*/
#define SIGNALING_WITH_EOS		DEFINE_VALUE
#define SIGNALING_WITH_METERING 	DEFINE_VALUE
#define SIGNALING_MALICIOUS_CALL	DEFINE_VALUE

/* if define R1_SIGNALING, SIGNALING_WITH_EOS should also be defined */
/* trunk channel should also support s_eos */

#ifndef EXTENDED_LX_ISUP
//#define EXTENDED	I think we should NOT define extended for those with ktnum.pln
#endif
#ifdef EXTENDED
#define SEND_SERVICE_TO_STD
#define ARTIMAN 	// ignore metering pulses coming from extended link
#endif

#ifdef EXTENDED
#define EXTENDED_LX_CAS3     /* this system is an extension of a LX */
#define EXTENDED_LX_BULK_BILLING_CONVERSATION_TIMER    (50 * 180)    /* 20 sec */
#endif

#ifdef EXTENDED_LX_CAS3
#define GROUPING_FOR_EXTENDED_LX
#define CAS3_OGT_GROUP_FOR_LX		1
#define CAS3_COMMON_GROUP_FOR_STD_LX	2
#endif

//#define CAS3_OGT_GROUP_FOR_OSS	  3	  // SPECIAL_SERVICE_CODE_1ST_DGT (1xx) goes on it, if not included in fdn file

//#define CAS3_OGT_GROUP_FOR_117	  4	  // 117
#define SECOND_AND_THIRD_DIGIT_OF_117	0x32	// set for 132

//#define CAS3_OGT_GROUP_FOR_LX5	  5
#define DIGIT_FOR_LX5			0xff

//#define CAS3_OGT_GROUP_FOR_INTERNATIONAL 6	  // if defined, international traffic shall go on it

//#define CAS3_OGT_GROUP_FOR_OTHER_LX	  7
//#define DECIDE_ON_4TH_DIGIT
//#define LX_1ST_DGT_FOR_OTHER_LX	  4


#else
#define NO_CAS3_DIGITAL_TRUNK	0
#define NO_CAS3_IC_CH		0
#endif

#if ((SIGNALING_WITH_PTS == DEFINE_VALUE) || (defined CAS1_INSTALLED) )
#define NO_DIGIT_FOR_ROUTING		2
#define NO_DIGIT_FOR_FINAL_ROUTING	4
#endif

//#define CASV5

#if ((!defined CASV5) && (!defined CASV5_CO))
#define NO_DIGITAL_TRUNK   (NO_CAS3_DIGITAL_TRUNK+NO_CAS1_DIGITAL_TRUNK)
#else
#define NO_DIGITAL_TRUNK   (NO_CAS3_DIGITAL_TRUNK+NO_CAS1_DIGITAL_TRUNK+NO_CASV5_DIGITAL_TRUNK)
#endif

#endif	// CEPT_INSTALLED


#ifdef R1_SIGNALING
#define R1_SIGNALINGP
//#define DSP_R1
#define TOG_R1
#define OFFICE_CODE_NO_1	DIGIT10
#define OFFICE_CODE_NO_2	DIGIT1
#define OFFICE_CODE_NO_3	DIGIT2
#define OFFICE_CODE_NO_4	DIGIT3
#define OFFICE_CODE_NO_5	DIGIT4
#define OFFICE_CODE_NO_6	DIGIT5
#endif


#define OPERATOR_PC

/*
#define TRUNK_RING_DISCONNECT_REPORT
*/


//#define OPR_EXIT_PASSWORD


#ifdef OPR_EXIT_PASSWORD
#define EXIT_PASSWORD		("gmafhh")
#define EXIT_PASSWORD_LENGHT	6
#endif


#define TIME_DATE_DISPLAY


#if ((NO_OPRTR != 0) || (defined KEYBOARD_INSTALLED))
#define OPR_EXT_PROGRAM
/* operator can program ext. services */
#endif
#if (NO_OPRTR!=0)
#define DISPLAY_STATUS_ACT
#endif

#ifdef MUSIC_TAPE_RECORDER
#define TAPE_RECORDER_EXT	(NO_EXTENSION)
#endif

#ifdef BILLING_REQUIRED
/* select kind of billing */
/*
#define ON_LINE_BILLING_ACT
*/

#define DETAIL_BILLING		DEFINE_VALUE

#define BULK_BILLING

#if ((defined DETAIL_BILLING) && (defined BULK_BILLING))
#define REPORT_BULK_IN_DETAIL
#endif

#endif

#ifdef DETAIL_BILLING
#if (NO_OPRTR == 0)
// we may need following as a 'dummy' in order that pmamftx in mc_b.c work
// case of having KIATEL_UDP_FOR_OAM but (NO_OPRTR == 0)
#define DETAIL_BILLING_OPR		  0
#endif
#if (NO_OPRTR == 1)
#define DISPLAY_BILLING
#define DETAIL_BILLING_OPR		  0
#endif

#if (NO_OPRTR >= 2)
//#define BILLING_BACKUP
#define DISPLAY_BILLING
#define DETAIL_BILLING_OPR		0
#define SECOND_DETAIL_BILLING_OPR	1
#endif
#endif	// DETAIL_BILLING

#ifdef REPORT_BULK_BILLING
#define BULK_BILLING_OPR		1
#define SECOND_BULK_BILLING_OPR 	0
#if 0
#define SI_FOR_BULK_BILLING	/* activate serial interface for bulk billing and malicious ... */
#endif // 0
#endif


/*
#define NO_TIME_OUT_ON_TIE_TRUNK
*/

/*
#define CO_TEST_VERSION
*/

#ifdef DOD16	// no difference!
#define NO_ANALOG_CARD		(NO_ANALOG_TRUNK / 8)	// some maximum
#else
#define NO_ANALOG_CARD		(NO_ANALOG_TRUNK / ANALOG_TRUNK_PER_CARD)
#endif

#define NO_NON_ISDN_STATIC_EXTENSION  ((NO_ANALOG_EXTENSION)+NO_RSU_EXT+NO_KIATEL_GSM_SUBSCRIBER+NO_V52_SUBSCRIBER+NO_SIP_EXT_SUBSCRIBER)

/*
#if ((defined WE_DO_HAVE_USERSIDE_ISDN) || (defined WE_DO_HAVE_PRIMARY_NETSIDE_ISDN)\
					|| (defined WE_DO_HAVE_BASIC_NETSIDE_ISDN))
*/
#ifdef DYNAMIC_EXT_INSTALLED
#define NO_STATIC_EXTENSION	(NO_NON_ISDN_STATIC_EXTENSION+NO_NETSIDE_STATIC_EXTENSION)
#elif (defined AN_V52_INSTALLED)
#if ((defined SIP_INSTALLED) && (NO_SIP_EXT_SUBSCRIBER == 0))
#define NO_EXTENSION		1	// only to have EXT_INSTALLED and compile with CALL_PROCESSOR
#else
#define NO_EXTENSION		(NO_NON_ISDN_STATIC_EXTENSION+NO_SIP_EXT_SUBSCRIBER)
#endif
#else
#define NO_EXTENSION		NO_NON_ISDN_STATIC_EXTENSION
#endif

#if ((defined WE_DO_HAVE_PRIMARY_NETSIDE_ISDN) || (defined WE_DO_HAVE_BASIC_NETSIDE_ISDN))
#define ISDN_STATIC_EXTENSION_START	NO_NON_ISDN_STATIC_EXTENSION
#define NO_NETSIDE_STATIC_EXTENSION	(NO_PRIMARY_NETSIDE_STATIC_EXTENSION + \
					 NO_BASIC_NETSIDE_STATIC_EXTENSION)
#else
#define NO_NETSIDE_STATIC_EXTENSION	0
#endif

#ifdef WE_DO_HAVE_USERSIDE_ISDN
#define USERSIDE_ISDN_STATIC_EXTENSION_START   NO_NON_ISDN_STATIC_EXTENSION
#endif

#if ((defined EXT_ON_H248_MGCSIDE) || (defined H248_RGW_INSTALLED))
#define FIRST_H248_EXTENSION	0
#endif

#ifndef CASV5_CO
#define NO_CASV5_CO				0
#define NO_CASV5_DIGITAL_TRUNK			0
#endif

#define NO_CO			(NO_ANALOG_TRUNK+NO_CASV5_CO)

#if (NO_CO != 0)
#define DSP_CLID_RCVR
#define ACD
#endif

#ifdef DSP_CLID_RCVR
//#define DSP_CLID_RCVR_DEBUG_ACT
//#define CONTINUOUS_DSP_CLID_RCVR_DEBUG_ACT
// following is good if we have CLID_HW_ORG i.e.
// ((defined CLID_HW) &&
//  ((defined CALL_PROCESSOR) || (defined EXT_ON_RSUSIDE) || (defined H248_RGW_INSTALLED)))
#define LARGE_CLID_QUE
// following two are good if CO_CARD_INSTALLED
#define FASTER_DOD_RING_DETECTION
#define DELAYED_RING
#endif

#ifndef RMT_CB
//#define CB_CARD_INSTALLED	// currently we can not have both due to definition of NO_RSU_CB in dsmtfp.h
#endif

#if ((defined RMT_CB) || (defined CB_CARD_INSTALLED))
#define CB_INSTALLED
#endif

#ifdef CB_INSTALLED
#ifdef RMT_CB
#define NO_CB			(16 * NO_RSU)
#endif
#ifdef CB_CARD_INSTALLED
#define NO_CB			16
#define CB_CARD_START_SLOT	0
#endif
#else
#define NO_CB			0
#endif

/*
#define DIAL_CO_GROUP
*/

#define NO_TIE_TRUNK		0

/* NO_ANALOG_TRUNK = NO_TIE_TRUNK+NO_CO */
/* it has been chosen for now that by default analog cards always start
   with tie trunks, and after tie trunks start co trunks */

#define AST_DEFAULT		( CALL_GEN			\
				  | MALICIOUS_CALL_DTCT 	\
				  | FEATURE_ACTIVE		\
				)


#ifdef CLID
#ifdef MAHGOL_TEST
#define CST_DEFAULT		( STD				\
				  | CAMP_ON			\
				  | DONT_DIST			\
				  | FOLLOW_ME			\
				  | WAKE_UP_IN_CST		\
				  | CONFERENCE_SERVICE		\
				  | DTMF_ALLOWED		\
				  | HOT_LINE_ACCESS		\
				  | INTERNATIONAL	 )
#else
#define CST_DEFAULT		( STD				\
				  | DTMF_ALLOWED		\
				  | HOT_LINE_ACCESS		\
				  | INTERNATIONAL	 )
#endif
#else
#define CST_DEFAULT		( STD				\
				  | DTMF_ALLOWED		\
				  | INTERNATIONAL		\
				  | CALL_BACK			\
				  | CAMP_ON			\
				  | FOLLOW_ME			\
				  | CONFERENCE_SERVICE		\
				  | CALL_PICKUP_USE		\
				)

#endif



#if (NO_ANALOG_TRUNK != 0)
#define ANALOG_CARD_START_SLOT	0  // to support pc104=5
#endif


#define EXT_CODEC_GAIN			0
#define FX_CODEC_GAIN			0
#if (NO_ANALOG_TRUNK != 0)
#define ANALOG_TRUNK_CODEC_GAIN 	0x07 /*0x07*/
#endif
/* 30/3/76, changed from 0x07 (7db gain in A/D) to 0x00 (0db gain in A/D)
   to solve ringing problem in opr-co voice connection */
/*  ANALOG_TRUNK_CODEC_GAIN may need not be changed for "after-install version" */
#define EM_CODEC_GAIN			0x38


//#define ORIG
#ifdef ORIG
#include "/ec/work/spec/orig.h"
#endif

#define GLOBAL_NIGHT_EXT	0

#define NO_ACTIVE_OPR		NO_OPRTR


/*
#define TRUNK_TRUNK_CONNECTION
*/
/* for connecting 2 analog trunk through switch */


#if(NO_TIE_TRUNK == 0)
/* if no TIE is available,we have to define 1 TIE trunk for variables */
/* declaration,and no TIE initialization on picc[] */

#define NO_TIE_INIT
#define NO_TIE		1

#else
#define NO_TIE		NO_TIE_TRUNK
#endif

	/****  numbering plan macros  ****/
#define NEW_NUMBERING_PLAN
/* if defined, may change NO_INTRNL_DGT from 4 to 3, also may change
   FIRST_DIRECTORY_... to other than 3200/4000/4100/4200 */

#define NO_INTRNL_DGT			3

#define F_D_N				0x300
#define INTRNL_1ST_DGT			(F_D_N >> ((NO_INTRNL_DGT-1) * 4) )
//#define INTRNL_1ST_DGT_2		  (INTRNL_1ST_DGT + 1)

#define SERVICE_1ST_DGT 		9

//#define TWO_DIGIT_SERVICE_CODES
#ifdef TWO_DIGIT_SERVICE_CODES
#define SERVICE_1ST_DGT_2		 (SERVICE_1ST_DGT + 1)
#endif

#define SPECIAL_SERVICE_CODE_HANDLING

#ifdef SPECIAL_SERVICE_CODE_HANDLING
#define SPECIAL_SERVICE_1ST_DGT 	1
#endif

#define OPERATOR_NUM			0x0a
//#define ONLY_ONE_OPRTR_CAN_ANSWER

#define CONFIRM_CODE			2  //8
#define CNSLT_TRNS_CODE 		8  //9	//3

#ifdef MALICIOUS_PRINT
#define MALICIOUS_DTCT_CODE		6
#endif

#ifdef CONFERENCE_ACT
#define ESTABLISH_CONFERENCE_CODE	7
#endif

#if(NO_CO != 0)
#define CO_REQ				9

#ifndef DIAL_CO_GROUP
//#define SPECIAL_DIGIT_FOR_CO_ACCESS
#endif

#ifdef SPECIAL_DIGIT_FOR_CO_ACCESS
#define SPECIAL_CO_REQ			0x06
#define SPECIAL_CO_GROUP		5	// last co group
#endif

#ifndef DIAL_CO_GROUP
//#define ONE_DIGIT_FOR_CO_GROUPS
#endif

#ifdef ONE_DIGIT_FOR_CO_GROUPS
#define CO_REQ1 			0x0b
#define CO_REQ2 			0x0c
#endif
#endif

#ifndef NO_TIE_INIT
#define TIE_REQ
#endif


#if (NO_OPRTR != 0)
#ifdef OPERATOR_PC
#define PARAMETER_DOWN_UP_LOAD		DEFINE_VALUE
#if (NO_OPRTR > 1)
//#define ONLY_THIS_OPRTR_CAN_UPLOAD
#endif
#endif
#endif


#if(NO_CO != 0)
#define CTG_DEFAULT		 0x00
/* all extensions have access to CO trunk & CO group & only group 0 allowed */
#endif


#ifndef NO_TIE_INIT
#define TTG_DEFAULT
#else
#define TTG_DEFAULT		0
#endif


#if ((defined PARAMETER_DOWN_UP_LOAD) || (defined BACKUP_FILE))

/*
#define OPR_NIGHT_PROG
*/

#ifdef OPR_NIGHT_PROG
#define OPR_NIGHT_0
#define OPR_NIGHT_1
#define OPR_NIGHT_2
#define OPR_NIGHT_3
#endif

/*
#define HOT_LINE_PROG
*/

#ifdef HOT_LINE_PROG
#define DESTINATION_HOT_LINE_0_T
#define DESTINATION_HOT_LINE_0_I
#define DESTINATION_HOT_LINE_1_T
#define DESTINATION_HOT_LINE_1_I
#define DESTINATION_HOT_LINE_2_T
#define DESTINATION_HOT_LINE_2_I
#define DESTINATION_HOT_LINE_3_T
#define DESTINATION_HOT_LINE_3_I
/* destination hot lines type & index */
#endif



#ifdef NO_CAS3_DIGITAL_TRUNK
/* NO_CAS3 is only used for CAS3 parameters upload/download/view/edit */
#define NO_CAS3 	(NO_CAS3_DIGITAL_TRUNK * 30)
#else
#define NO_CAS3 	0
#endif

// lim ogt group is the last group and should not be programmble thru "dep"
#if (NO_CAS3 != 0)

#ifdef LIM
#define NO_CAS3_PROGRAMMABLE_GROUP	(NO_CAS3_GROUP-1)
#else
#define NO_CAS3_PROGRAMMABLE_GROUP	(NO_CAS3_GROUP)
#endif

#endif

#if ( (defined EM_INSTALLED) && (defined EM_CONTINUOUS_SIGNALING) && (defined EM_GROUP_DIALLED) && (defined NO_TIE_INIT) )
#define EM_GROUP_PROGRAM
#endif

#if ((defined NO_CAS1_DIGITAL_TRUNK) && (NO_CAS3 == 0))
// for the time being we have either NO_CAS3 or NO_CAS1, not both
/* NO_CAS1 is only used for CAS1 parameters upload/download/view/edit */
#define NO_CAS1 	(NO_CAS1_IC_CH + NO_CAS1_OG_CH)
#else
#define NO_CAS1 	0
#endif

#if (NO_CAS1 != 0)
#define NO_CAS1_PROGRAMMABLE_GROUP	(NO_CAS1_GROUP)
#endif

// remember to correct NO_LOAD_PARM also if you included NO_CAS1

#if (NO_ACTIVE_OPR != 0)
#define NO_OPR_NITX_PARAMS 4   // NO_OPR_NITX
#else
#define NO_OPR_NITX_PARAMS 0
#endif

#if ((defined SIP_INSTALLED) && (defined REGISTER_SIP_TERMINAL))
#define NO_SIP	MAX_REMOTE_SIP_STATIC_USER_AGENT
#else
#define NO_SIP	0
#endif


#ifdef EM_GROUP_PROGRAM
#ifdef X_RAILWAY
#define NO_LOAD_PARM1  ( (NO_EXTENSION * 5) + (NO_EM * 4) + (NO_CO * 2) + (NO_CAS3 * 1) + (NO_CAS1 * 1) + NO_OPR_NITX_PARAMS + 4 + NO_EM_GROUP + NO_CB)
#else
#define NO_LOAD_PARM1  ( (NO_EXTENSION * 4) + (NO_EM * 4) + (NO_CO * 2) + (NO_CAS3 * 1) + (NO_CAS1 * 1) + NO_OPR_NITX_PARAMS + 4 + NO_EM_GROUP + NO_CB)
#endif
#elif (defined NO_TIE_INIT)
//#define NO_LOAD_PARM1  ( (NO_EXTENSION * 4) + (NO_CO * 2) + (NO_CAS3 * 1) + (NO_CAS1 * 1) + NO_OPR_NITX_PARAMS + 4 + NO_CB)
#if (NO_COT_GROUP > 16)
#define NO_LOAD_PARM1  ( (NO_EXTENSION * 7) + (NO_CO * 2) + (NO_CAS3 * 1) + (NO_CAS1 * 1) + NO_OPR_NITX_PARAMS + 4 + NO_CB  + (NO_SIP*2))
#else
#define NO_LOAD_PARM1  ( (NO_EXTENSION * 4) + (NO_CO * 2) + (NO_CAS3 * 1) + (NO_CAS1 * 1) + NO_OPR_NITX_PARAMS + 4 + NO_CB  + (NO_SIP*2))
#endif
#else
#define NO_LOAD_PARM1  ( (NO_EXTENSION * 4) + (NO_TIE * 2) + (NO_CO * 2) + (NO_CAS3 * 1) + (NO_CAS1 * 1) + NO_OPR_NITX_PARAMS + 4 + NO_CB)
#endif
/* number of parameters for down or up load in int */

#ifdef R1_SIGNALINGP
#define NO_LOAD_PARM	(NO_LOAD_PARM1 + 3)
#else
#define NO_LOAD_PARM	(NO_LOAD_PARM1)
#endif

#if ( (defined PARAMETER_DOWN_UP_LOAD) && ( (NO_OPRTR==1) || (defined ONLY_THIS_OPRTR_CAN_UPLOAD) ) )
/*
#define AUTO_UPLOAD
*/
#endif
#ifdef AUTO_UPLOAD
#define AUTO_UPLOAD_OPR 	0
#define MAX_NAME_LENGTH  20+2
/* last two bytes reserved for "file_changed" parameter */

#else
#define MAX_NAME_LENGTH  20
#endif

#define VALID_PARM_FILE_SIZE  (MAX_NAME_LENGTH + 2 + NO_LOAD_PARM * 2 + 2)
/* valid parameters file size 20=installation name,2=current password, */
/* 2= file checksum */



/*
#define DAY_NIGHT_PARAMETERS_FILE
*/

#endif


#define OPERATOR_FARSI


#define DEFAULT_LINE_CARD_INDEX
/* if line cards are consecutive */

#define DEFAULT_ANALOG_TRUNK_INDEX
/* if analog trunk cards are consecutive */


#ifdef OPERATOR_PC
#define SHIT_TILDE		0x7e
#define NO_OPR_CARD_EXIT_KEY	SHIT_TILDE
/* when operator card not installed by entering this key we can quit */
/* operator program. */
#endif

#ifndef MAX_NAME_LENGTH
#define MAX_NAME_LENGTH  20	/* to cover the case: detail billing with no parameter down/up load */
#endif
/****  end of installation specification  ****/

#if ((defined DISPLAY_EM_STATUS) || (defined DISPLAY_CAS3_STATUS))
#define DISPLAY_TRUNK_STATUS
#endif

#if ( (defined SI_FOR_BULK_BILLING) || ((defined SI_DEBUG_Q_ACT)&&(!defined SI_DEBUG_ON_U)) )
#define SERIAL_INTERFACE_ACTIVE
#endif


#ifdef EM_INSTALLED
#define EM_DGT_RCVR
#define EM_DGT_SEND
#endif

#if(NO_ANALOG_TRUNK != 0)
#define CO_TIE_DGT_SEND
#if ((NO_OPRTR != 0) || (defined KEYBOARD_INSTALLED))
#define OPR_ANALOG_TRUNK_PROGRAM
#endif
#endif

/*
#define NO_OPR_INTRNL_QU
*/

#ifdef DTMF
#define SEND_DTMF_ON_CO_THRU_VOICE
#endif

#ifdef DTMF_CARD_INSTALLED
#define NO_DTMF_CARD_IN_SYS	1
#if (NO_ANALOG_TRUNK != 0)
#define DTMF_CARD_START_SLOT	((NO_ANALOG_TRUNK/8)+ANALOG_CARD_START_SLOT)
#else
#define DTMF_CARD_START_SLOT	1
#endif
#define DTMF_CODEC_GAIN 	0
#define MUIDTMFC
#define LINE_CARD_START_SLOT	(DTMF_CARD_START_SLOT+1)
#else
#if (NO_ANALOG_TRUNK != 0)
#define LINE_CARD_START_SLOT	((NO_ANALOG_TRUNK/8)+ANALOG_CARD_START_SLOT)
#else
#define LINE_CARD_START_SLOT	1
#endif
#endif

/*
#define UPDATE_DATE_TIME
*/

#if 0
#define NO_OP_OVRD    /* operator can not override extensions */
#endif // 0

/*
#define OP_FIRST_ZERO_ON_CO_CONTROL
*/
#ifdef OP_FIRST_ZERO_ON_CO_CONTROL
#define OP_FIRST_ZERO_ON_CO_DEFAULT  /* SET/RESET */
#endif

#define TOLL_BARRING_ON_COs
#ifdef TOLL_BARRING_ON_COs
#define TOLL_ACCESS_DEFAULT	       SET
#endif

#if 0
#define SPECIAL_XFER	 /* xfer co to extensions by op in order that
			    ext. dials himself */
#endif // 0

#define EASY_XFER	 /* flash, dial,code... = flash, 2, 117 ,.... */

#ifdef EASY_XFER
#define NO_EASY_XFER_CODE  /* flash, dial,... = flash, 2, 117 ,... */
#ifdef	NO_EASY_XFER_CODE
#define EASY_XFER_CODE	 INTRNL_1ST_DGT
#ifdef	INTRNL_1ST_DGT_2
#define EASY_XFER_CODE_2 INTRNL_1ST_DGT_2
#endif
#else
#define EASY_XFER_CODE	 5
#endif
#endif

/*
#define IMMEDIATE_RINGING
*/
/****************************************************************************/
/****************************************************************************/
#if (NO_INTRNL_DGT == 7)
#define h2i(x) ( ((x>>24) * 1000000L) + (((x>>20)%0x10) * 100000L) + (((x>>16)%0x10) * 10000L) + ((x>>12)%0x10) * 1000 + ((x>>8)%0x10) * 100 + ((x>>4)%0x10) * 10 + (x%0x10))
#endif
#if (NO_INTRNL_DGT == 6)
#define h2i(x) ( ((x>>20) * 100000L) + (((x>>16)%0x10) * 10000L) + ((x>>12)%0x10) * 1000 + ((x>>8)%0x10) * 100 + ((x>>4)%0x10) * 10 + (x%0x10))
#endif
#if (NO_INTRNL_DGT == 5)
#define h2i(x) (((x>>16) * 10000L) + ((x>>12)%0x10) * 1000 + ((x>>8)%0x10) * 100 + ((x>>4)%0x10) * 10 + (x%0x10))
#endif
#if (NO_INTRNL_DGT <= 4)
#define h2i(x) ((x>>12) * 1000 + ((x>>8)%0x10) * 100 + ((x>>4)%0x10) * 10 + (x%0x10))
#endif
/*
#define LIM
*/
/* currently if you defined LIM, should have also defined NEW_NUMBERING_PLAN */
/* also if you have not defined LIM and have defined NEW_NUMBERING_PLAN, then
   should also define NO_TOTAL_EXTENSION */

#ifdef LIM

#define MULTI_CENTER


#define A_CENTER	0
#define B_CENTER	1
#define C_CENTER	2

#define SELF_CENTER
/*
#define FOLLOW_ME_FOR_ICDT
*/
/* follow me for incoming digital trunk */


#define BUSYT_FROM_SRC_CENTER

/* if define for OG digital trunk after receiving congestion source center */
/* will send busy tone to its extension */

#define NO_EXTENSION_A
#define NO_EXTENSION_B
#define NO_EXTENSION_C
#define NO_TOTAL_EXTENSION    (NO_EXTENSION_A+NO_EXTENSION_B+NO_EXTENSION_C)

#if (NO_INTRNL_DGT == 7)
#define i2h(x) ( ((x/1000000L)<<24) + (((x/100000L)%10)<<20) + (((x/10000)%10)<<16) + (((x/1000)%10)<<12) + (((x/100)%10)<<8) + (((x/10)%10)<<4) + (x%10))
#endif
#if (NO_INTRNL_DGT == 6)
#define i2h(x) ( ((x/100000L)<<20) + (((x/10000)%10)<<16) + (((x/1000)%10)<<12) + (((x/100)%10)<<8) + (((x/10)%10)<<4) + (x%10))
#endif
#if (NO_INTRNL_DGT == 5)
#define i2h(x) (((x/10000)<<16) + (((x/1000)%10)<<12) + (((x/100)%10)<<8) + (((x/10)%10)<<4) + (x%10))
#endif
#if (NO_INTRNL_DGT <= 4)
#define i2h(x) (((x/1000)<<12) + (((x/100)%10)<<8) + (((x/10)%10)<<4) + (x%10))
#endif

#if (SELF_CENTER == A_CENTER)
#define FIRST_DIRECTORY_NUM	F_D_N
#endif
#if (SELF_CENTER == B_CENTER)
#define FIRST_DIRECTORY_NUM	(i2h((h2i(F_D_N) + NO_EXTENSION_A)))
#endif
#if (SELF_CENTER == C_CENTER)
#define FIRST_DIRECTORY_NUM	(i2h((h2i(F_D_N) + NO_EXTENSION_A + NO_EXTENSION_B)))
#endif

#if (SELF_CENTER == A_CENTER)
#define OTHER_LIMS_CAN_DIAL_OPR
#endif

#define EM_PULSE_INSTALLED_ON_OTHER_LIM
#ifdef	EM_PULSE_INSTALLED_ON_OTHER_LIM
#define EM_REQ
#define TRANSIT_DT_EM_ON_OTHER_LIM
#define LX_1ST_DGT	3
#define EXTENDED_LX_EM	   /* this system is an extension of a LX */
#define EXTENDED_LX_BULK_BILLING_CONVERSATION_TIMER    50 * 20	  /* 20 sec */
#endif

#define EM_CONT_INSTALLED_ON_OTHER_LIM
#ifdef	EM_CONT_INSTALLED_ON_OTHER_LIM
#define EM_REQ
#endif

#define CAS1_INSTALLED_ON_OTHER_LIM
#ifdef	CAS1_INSTALLED_ON_OTHER_LIM
#define CAS1_REQ
#endif

#define CO_INSTALLED_ON_OTHER_LIM
#ifdef	CO_INSTALLED_ON_OTHER_LIM
#define CO_REQ
#define CTG_DEFAULT		 0xc1
#endif

//#define TRANSIT_DT_DT_ON_OTHER_LIM
//#define LX_1ST_DGT	  3

//#define EXTENDED_LX_CAS3
//#define EXTENDED_LX_BULK_BILLING_CONVERSATION_TIMER	 (50 * 20)    /* 20 sec */

#else

#define FIRST_DIRECTORY_NUM	F_D_N
#define NO_TOTAL_EXTENSION	NO_EXTENSION

#endif



#if ((defined LIM) || (defined MULTI_CENTER))
#define NO_CENTER
/* total number of destination centers for digital trunks */
/* A,B,C & T regardless of their installation */
#define NO_DT_GROUP
/* number of digital trunk groups used for LIM (or CAS3 STD) connection */
/* this is checked against the contents of dcdtg[] */
/* should be equal to NO_CAS3_GROUP */
#endif

#define FIRST_DIRECTORY_DECIMAL  h2i(FIRST_DIRECTORY_NUM)

