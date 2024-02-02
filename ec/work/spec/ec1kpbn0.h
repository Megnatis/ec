

#define TIMER_FOR_SIP_TRUNK_CALLS
#define TIMER_FOR_INTERNAL_CALLS

#define SPEED_DIALLING
#ifdef SPEED_DIALLING
#define USE_DTMF_SENDER_FOR_SPEED_DIALLING
#endif

//#define MY_SDP_PARSER_TEST
#ifdef MY_SDP_PARSER_TEST
#define SDP_PARSER_DEBUG_ACT
#define ws2p myws2p
void myws2p(char *s);
#endif

#define AUDIT_DOD_RELAY

//#define SIP_TERMINAL_DO_XFER_BY_END_INSTEAD_OF_REFER

#define SCHEDULED_GNX
#define MOODI_NEMOOD
#define NEW_CALL_XFER_METHOD

// following defined, only because in some sites we have a crash like
// save_addr+15 from setimer+355 from rtn63+1842 which is after calling
// store_call_frame() in save_addr()
// Let's see increasing BACKTRACK_DEPTH from 1 to 10 has any effect
#define IN_DEPTH_BACKTRACK

// following made processor completely engaged for KIATEL_FACTORY
//#define MALLOC_AUDIT
/*
#ifdef SYSTEM_SIDE
#define TCPIP_ON_SYS_OAM_OPERATOR_SIDE
#endif
#ifdef OPERATOR_SIDE
#define TCPIP_ON_SYS_OAM_SYSTEM_SIDE
#endif
#define NO_OPRTR 1
*/

//#define MY_SIP_PARSER_TEST
#ifdef MY_SIP_PARSER_TEST
#define SIP_PARSER_DEBUG_ACT
#define ws2p myws2p
void myws2p(char *s);
#endif

#define DO_NOT_DEFINE_TCI // stupid thing only in ec10k0.h, and now in ec1knew.h

#define HIGH_MEMORY
#define NO_DR 250 // PARS_GENERATOR
#define DR_INDEX	unsigned int


#define DOD16
#ifdef DOD16
#define SEND_SILENCE_INSTEAD_OF_CODEC_POWER_DOWN
#endif
#define SEND_OPERATOR_DTMF_ON_CO
#define HUAWEI_DOES_NOT_SEND_IN_BAND_INFORMATION_IS_NOW_AVILABLE

#define HANDLER_DURATION_CHECK
#define REPORT_ONLY_OVERLOAD
#define OVERLOAD_THRESHOLD	(3*0x2e9c)

//#define FLASH_DEBUG_ACT
//#define CR_DEBUG_ACT		// cr123.c, cr46789.c
//#define CO_DEBUG_ACT			// cr202124.c, cr252629.c
//#define OPR_DEBUG_ACT   // cr314041.c,cr323334.c,cr424345.c,cr464748.c,
//#define ISDN_DEBUG_ACT
//#define USER_DEBUG_ACT  // userrtcp.c, u931cp.c
//#define EM_DTMF_SENDER_DEBUG_ACT

// RING_AGAIN_COT (B3) is used for FOLLOW_ME service disntiushing, assuming BAD_PAYER is not defined

//#define SAIAN_ERTEBAT
//#define GHOM_RAILWAY
//#define BEHIN
//#define KIATEL_TEST	// kiatel factory, 2 network pri, 2 user pri, ethernet, 3 operator
//#define KIATEL_TEST2	 // no network pri, 2 user pri, no rsu, no ethernet, 1 operator, some print
//#define PARS_GENERATOR  // 2 network pri, 2 user pri, with ethernet, 5 operator
//#define KIATEL_TEST3	  // 4 network pri, with ethernet, 1 operator
//#define ISDN_TEST	  // basic isdn, with ethernet, 1 operator
#define SIP_TEST  // no operator or one operator, 4 user primary
// default is 2 user pri, 2 rsu, 5 operator, ethernet defined in ether.h
//#define KERMAN  // 5e1 rsu , 3264 subscriber, 2 user pri, 5 operator


#ifdef SIP_TEST

// either of followings
// followings are upri/sip converters
//#define OLOOM_PEZESHKI_KERMAN
//#define KERMAN_MOODY
//#define BANK_MELLI_KERMAN
//#define IRAN_SYSTEM
// followings have sip/upri/ext/dod/opr
//#define KIATEL_FACTORY
//#define KIATEL_MIRDAMAD
//#define NAFT
//#define MALIAT	// edare maliate ostane azarbayejane sharghi
//#define RAFSANJAN	  // bimarestane vali asr rafsanjan
//#define ALBORZ	  // amoozesh parvaresh ostan alborz
//#define KERMAN_EGHTESAD // sazmane omoore eghtesad va daraii kerman
//#define KERMAN_HOSPITAL // bimarestan shahid afzali-poor kerman
//#define ISFAHAN_HOSPITAL_1 // bimarestan emam-hosein, first site
//#define ISFAHAN_HOSPITAL_2 // bimarestan emam-hosein, second site, emam-hasan?
//#define VARZESH_KHORASAN     // edareh kol varzesh va javanan khorasan shomal
//#define NAFT_SHAHR	  // sherkat naft kermanshah
//#define KERMAN_KHAZANE	  // khazane-dari kerman
//#define KIATEL_FINAL_TEST_FACTORY // teste nahaaii karkhane kiatel
//#define ORUMIEH_ZENDAN  // edareh omoore zendanhaye orumieh
//#define NAFT_ILAM	  // mantaghe ilam shekat naft kermanshah
//#define BEHZISTI_RAZAVI  // sazman behzisti ostan khorasan razavi
//#define AMOOZESH_KORDESTAN // edareh amoozesh parvaresh ostan kordestan
//#define BOVIR_STANDARD // edareh standard ostan kohkilooye bovir ahmad
//#define KERMAN_STANDARD // edareh standard kerman
//#define ABFAZELAB_KORDESTAN // edareh ab o fazelab ostan kordestan
//#define SOLAS_KERMANSHAH // bimarestan solas babajani ostan kermanshah
//#define ISFAHAN_GHAZA // sazmane ghaza va daru ostan isfahan
//#define AMOOZESH_2_KORDESTAN // edareh amoozesh parvaresh nahieh 2 ostan kordestan
//#define SANAYE_DEFA	  // sazmane sanaye defa
//#define BIMARESTAN_PAVEH  // bimarestane paveh
//#define DADGOSTARI_KAMYARAN	  // dadgostarti kamyaran, ostane kermanshah
//#define POLYACRYL_ISFAHAN	  // polyacryl factory, isfahan
//#define TRAKTORSAZI	  // traktorsazi tabriz
//#define SHAHRAKE_SANATI_KORDESTAN	  // sherkate shahrakhahye sanati ostane kordestan
//#define BARGH_KORDESTAN // sherkate tozi bargh ostan koredestan, now used for edare ab kordestan AAB_KORDESTAN
//#define KESHAVARZI_KORDESTAN // edareh keshavarzi ostan kordestan
//#define ENTEGHAL_KHOON_AZ	  // sazman enteghal khoon azarbayejan gharbi
//#define GERASH_FARS	  // daneshgah oloom pezeshki gerash ostan fars
//#define LANJAN  // bimarestan shohadaye lanjan ostan esfahan
//#define SHAFA   // daneshgahe oloom pezeshki kerman, sakhteman shafa
//#define PEZESHKI_GHOM    // daneshgah oloom pezeshki ghom
//#define BIMARESTAN_TAROM   // bimarestan shahrestan tarom
//#define REFAH_KORDESTAN  // site sarparastei bank refah ostan kordestan
//#define BEHDASHT_TAROM  // site shabakeh behdasht shahrestan tarom ostan zanjan
//#define ZAHRA_MIANDOAB  // site bimarestan hazrat zahra shahrestan miandoab ostan azar gharbi
//#define BORKHAR_BEHDASHT  // site markaz behdasht shahrestan bodkhar ostan esfahan
//#define RAHDARI_ZANJAN  // site edareh rahdari ostan zanjan
//#define KHANALI_TEST_SITE_2	// system test dovvom khadamat
//#define KESHAVARZI_KORDESTAN_3 // site sevom jahad keshavarzi ostan kordestan
//#define KESHAVARZI_KORDESTAN_4 // site chaharom jahad keshavarzi ostan kordestan
//#define KESHAVARZI_KORDESTAN_5 // site panjom jahad keshavarzi ostan kordestan
//#define SHIRAZ_ENG_UNIV // site daneshkade mohandesi daneshgah shiraz
//#define SHIRAZ_AGR_UNIV // site daneshkade keshavarzi daneshgah shiraz
//#define PASTOR_KARAJ	  // site institute pasteur karaj
//#define KHOR_ISFAHAN	  // site bimarestan khor ostan isfahan
//#define ZAKARIA_KHORASAN // site araazi zakaria edareh kol farhang va ershade eslami khorasan razavi
//#define DR_GHARAI_TEST  // test Dr. Gharaaii?
//#define GHAEM_MASHAD	  // site bimarestan hazrat ghaem mashad
#define KHANALI_TEST_SITE_1  // narmafzare test karkhane
//#define DARAI_ZANJAN_1  // edare maliat va darai zanjan sakhtemane avval
//#define DARAI_ZANJAN_2  // edare maliat va darai zanjan sakhtemane dovvom
//#define ZARAND_HOSPITAL   // bimarestane zarand ostane kerman
//#define SABALAN_HOSPITAL  // bimarestane sabalan ostane ardabil
//#define AZAD_ZANJAN	  // daneshgahe azad eslami vahede zanjan
//#define DARYAII_MAHMOODABAD  // daneshgahe oloome daryaii mahmood abad
//#define BASTAR_ISFAHAN  // bastare darman oloom pezeshki isfahan
//#define CHAMRAN_SHIRAZ // bimarestane shahid chmaran shiraz
//#define PEZESHKI_JIROFT // daneshgahe oloom pezeshki jiroft
//#define ESLAMSHAHR_HOSPITAL // bimarestan emam reza eslamshahr
//#define PEZESHKI_TORBAT // daneshgahe oloom pezeshki torbat heydarieh
//#define PEZESHKI_ARAK   // daneshgahe oloom pezeshki arak
//#define DARAI_TABRIZ	  // edareh omoor eghtesadi va daraii tabriz
//#define PEYMANIE_JAHROM // markaz darmani peymanie jahrom ostan fars
//#define FACTORY_TINY_TEST
//#define MASHAD_HASHEMI  // site sakhteman avval bimarestane shaid hasheminejad mashad
//#define NAFT_ABADAN	  // site daneshkade san'at naft abadan
//#define NAFT_AHVAZ	 // site daneshkade san'at naft ahvaz
//#define GHANUNI_KERMAN  // site sazmane pezeshki ghanuni ostan kerman

//#ifdef CHAMRAN_SHIRAZ
#define DEBUG_AMPLIFIER
#define AUDIT_AMPLIFIER
#define DEBUG_AMPLIFIER2
//#endif

#ifdef CHAMRAN_SHIRAZ
//#define SIP_IVR_LOG
#endif

#if ((defined PEYMANIE_JAHROM) /*|| (defined CHAMRAN_SHIRAZ)*/)
#define OPERATOR_SIDE_DEBUG_ACT
#endif

#ifdef SIP_TEST
#ifdef FACTORY_TINY_TEST
#define NO_CR 4
#else
#define CHECK_HW_INFO
#endif
#endif

#if ((defined ABFAZELAB_KORDESTAN) || (defined ZARAND_HOSPITAL))
// problem of 3cx sip phone contacting primary,
// receiving sdp in 183 progress, then needs it also in 200 ok
#define ALWAYS_SEND_SDP_IN_200_OK
#endif

#ifdef REFAH_KORDESTAN
#define DEBUG_CRASHED_U931_RT_CP
#endif

//#ifdef ESLAMSHAHR_HOSPITAL
//#define OPERATOR_SIDE_DEBUG_ACT
//#endif

#if ((defined KIATEL_FACTORY) || (defined KHANALI_TEST_SITE_1) || (defined KHANALI_TEST_SITE_2) /*|| (defined POLYACRYL_ISFAHAN)*/)
//#define PRINT_RECEIVED_DIGIT
#define SIP_LEVEL1_DEBUG_ACT
#define RTP_DEBUG_ACT
#define CR_DEBUG_ACT	// cr123.c, cr46789.c
#define CO_DEBUG_ACT	// cr202124.c, cr252629.c
#define OPR_DEBUG_ACT	// cr314041.c,cr323334.c,cr424345.c,cr464748.c,
			// cr505152.c,cr535460.c,cr55.c
#define ISDN_DEBUG_ACT
#define USER_DEBUG_ACT	// userrtcp.c, u931cp.c
#define OPERATOR_SIDE_DEBUG_ACT
//#define BILLING_DEBUG_ACT // bill.c,mc_b.c
#endif // ((defined KIATEL_FACTORY) || ...)

#ifdef KESHAVARZI_KORDESTAN
#define KESHAVARZI_KORDESTAN_1
//#define KESHAVARZI_KORDESTAN_2
#endif

#if (/*(defined BOVIR_STANDARD) ||*/ (defined SHIRAZ_AGR_UNIV))
#define NO_COT_GROUP   64
#endif

#if ((defined ISFAHAN_HOSPITAL_1)  || \
     (defined ABFAZELAB_KORDESTAN) || \
     (defined SOLAS_KERMANSHAH)    || \
     (defined ISFAHAN_GHAZA)	   || \
     (defined AMOOZESH_2_KORDESTAN)|| \
     (defined POLYACRYL_ISFAHAN)   || \
     (defined TRAKTORSAZI)	   || \
     (defined LANJAN)		   || \
     (defined SHIRAZ_ENG_UNIV)	   || \
     (defined SHIRAZ_AGR_UNIV)	   || \
     (defined KHANALI_TEST_SITE_1) || \
     (defined DARAI_ZANJAN_1)	   || \
     (defined DARAI_ZANJAN_2)	   || \
     (defined ZARAND_HOSPITAL)	   || \
     (defined CHAMRAN_SHIRAZ)	   || \
     (defined PEZESHKI_JIROFT)	   || \
     (defined ESLAMSHAHR_HOSPITAL) || \
     (defined SABALAN_HOSPITAL)    || \
     (defined PEZESHKI_TORBAT)	   || \
     (defined ZAHRA_MIANDOAB)	   || \
     (defined PEZESHKI_ARAK)	   || \
     (defined BARGH_KORDESTAN)	   || \
     (defined MASHAD_HASHEMI)	   || \
     (defined NAFT_ABADAN)	   || \
     (defined NAFT_AHVAZ)	   || \
     (defined GHANUNI_KERMAN))
#define NO_OPRTR	2
#endif
#if (defined GHAEM_MASHAD)
#define NO_OPRTR	5
#endif
#ifdef KERMAN_HOSPITAL
#define NO_OPRTR	3
#endif

#ifdef KIATEL_FACTORY
#define MAX_PERMITTED_GONZALES_LENGTH  500000000L
#else
#define MAX_PERMITTED_GONZALES_LENGTH  100000000L
#endif

#if ((defined KIATEL_FACTORY) || (defined ZAHRA_MIANDOAB) || (defined KHANALI_TEST_SITE_1))
#define SEND_SDP_IN_REINVITE_RESPONSE	// grandstream terminal needs this
#endif

#ifdef KIATEL_FACTORY
#define AUTO_PRINT_MEMORY
#if ((defined AUTO_PRINT_MEMORY) && (defined SYSTEM_SIDE))
#define PRNT_QUE_LEN   (4096*4096)  // (4096*256)
#endif
#endif

#if ((defined KIATEL_FACTORY) || (defined KIATEL_MIRDAMAD) || (defined NAFT) \
  || (defined MALIAT) || (defined RAFSANJAN) || (defined ALBORZ) \
  || (defined KERMAN_EGHTESAD) || (defined KERMAN_HOSPITAL) \
  || (defined ISFAHAN_HOSPITAL_1) || (defined ISFAHAN_HOSPITAL_2) \
  || (defined VARZESH_KHORASAN) \
  || (defined NAFT_SHAHR) \
  || (defined KERMAN_KHAZANE) \
  || (defined KIATEL_FINAL_TEST_FACTORY) \
  || (defined ORUMIEH_ZENDAN) \
  || (defined NAFT_ILAM) \
  || (defined BEHZISTI_RAZAVI) \
  || (defined AMOOZESH_KORDESTAN) \
  || (defined BOVIR_STANDARD) \
  || (defined KERMAN_STANDARD) \
  || (defined ABFAZELAB_KORDESTAN) \
  || (defined SOLAS_KERMANSHAH) \
  || (defined ISFAHAN_GHAZA) \
  || (defined AMOOZESH_2_KORDESTAN) \
  || (defined SANAYE_DEFA) \
  || (defined BIMARESTAN_PAVEH) \
  || (defined DADGOSTARI_KAMYARAN) \
  || (defined POLYACRYL_ISFAHAN) \
  || (defined TRAKTORSAZI) \
  || (defined SHAHRAKE_SANATI_KORDESTAN) \
  || (defined BARGH_KORDESTAN) \
  || (defined KESHAVARZI_KORDESTAN) \
  || (defined ENTEGHAL_KHOON_AZ) \
  || (defined GERASH_FARS) \
  || (defined LANJAN) \
  || (defined SHAFA) \
  || (defined PEZESHKI_GHOM) \
  || (defined BIMARESTAN_TAROM) \
  || (defined REFAH_KORDESTAN) \
  || (defined BEHDASHT_TAROM) \
  || (defined ZAHRA_MIANDOAB) \
  || (defined BORKHAR_BEHDASHT) \
  || (defined RAHDARI_ZANJAN) \
  || (defined KHANALI_TEST_SITE_2) \
  || (defined KESHAVARZI_KORDESTAN_3) \
  || (defined KESHAVARZI_KORDESTAN_4) \
  || (defined KESHAVARZI_KORDESTAN_5) \
  || (defined SHIRAZ_ENG_UNIV) \
  || (defined SHIRAZ_AGR_UNIV) \
  || (defined PASTOR_KARAJ) \
  || (defined KHOR_ISFAHAN) \
  || (defined ZAKARIA_KHORASAN) \
  || (defined DR_GHARAI_TEST) \
  || (defined GHAEM_MASHAD) \
  || (defined KHANALI_TEST_SITE_1) \
  || (defined DARAI_ZANJAN_1) \
  || (defined DARAI_ZANJAN_2) \
  || (defined ZARAND_HOSPITAL) \
  || (defined SABALAN_HOSPITAL) \
  || (defined AZAD_ZANJAN) \
  || (defined DARYAII_MAHMOODABAD) \
  || (defined BASTAR_ISFAHAN) \
  || (defined CHAMRAN_SHIRAZ) \
  || (defined PEZESHKI_JIROFT) \
  || (defined ESLAMSHAHR_HOSPITAL) \
  || (defined PEZESHKI_TORBAT) \
  || (defined PEZESHKI_ARAK) \
  || (defined KIATEL_MIRDAMAD) \
  || (defined DARAI_TABRIZ) \
  || (defined PEYMANIE_JAHROM) \
  || (defined FACTORY_TINY_TEST) \
  || (defined MASHAD_HASHEMI) \
  || (defined NAFT_ABADAN) \
  || (defined NAFT_AHVAZ) \
  || (defined GHANUNI_KERMAN))
// following line for attacks, sip_server_xact1_rtn105(), 109(), xact2_rtn104()
#define TIMER_NO	5000
#define TIMER_INDEX	unsigned int
// following line for attacks we had code a006 temporarily
#ifdef SYSTEM_SIDE
#define NO_MEM_REC  (524288)
#endif

#define MY_UDP_FOR_OAM

#ifndef NO_OPRTR
#define NO_OPRTR 1
#endif
#if (defined GHANUNI_KERMAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT       24    // 24 terminals
#elif (defined NAFT_AHVAZ)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT       160   // 160 terminals
#elif (defined NAFT_ABADAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT       160   // 160 terminals
#elif (defined MASHAD_HASHEMI)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	40   // 40 terminals
#elif (defined FACTORY_TINY_TEST)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	10   // 10 terminals
#elif (defined PEYMANIE_JAHROM)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	20   // 20 terminals
#elif (defined DARAI_TABRIZ)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	68   // 68 terminals
#elif (defined KIATEL_MIRDAMAD)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	32   // 32 terminals
#elif (defined PEZESHKI_ARAK)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	24   // 24 terminals
#elif (defined PEZESHKI_TORBAT)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	68   // 68 terminals
#elif (defined ESLAMSHAHR_HOSPITAL)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	64   // 64 terminals
#elif (defined PEZESHKI_JIROFT)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	35   // 35 terminals
#elif (defined CHAMRAN_SHIRAZ)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	35   // 35 terminals
#elif (defined BASTAR_ISFAHAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	18   // 18 terminals
#elif (defined DARYAII_MAHMOODABAD)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	5   // 5 terminals
#elif (defined AZAD_ZANJAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	24   // 24 terminals
#elif (defined SABALAN_HOSPITAL)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	64   // 64 terminals
#elif (defined ZARAND_HOSPITAL)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50   // 50 terminals
#elif (defined DARAI_ZANJAN_2)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	10   // 10 terminals
#elif (defined DARAI_ZANJAN_1)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	10   // 10 terminals
#elif (defined KHANALI_TEST_SITE_1)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50   // 50 terminals
#elif (defined GHAEM_MASHAD)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	1000   // 1000 terminals
#elif (defined DR_GHARAI_TEST)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	10   // 10 terminals
#elif (defined ZAKARIA_KHORASAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	33   // 33 terminals
#elif (defined KHOR_ISFAHAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	5   // 5 terminals
#elif (defined PASTOR_KARAJ)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50   // 50 terminals
#elif (defined SHIRAZ_AGR_UNIV)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	1000   // 1000 terminals
#elif (defined SHIRAZ_ENG_UNIV)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	1000   // 1000 terminals
#elif (defined KESHAVARZI_KORDESTAN_5)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50     // 50 terminals
#elif (defined KESHAVARZI_KORDESTAN_4)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50     // 50 terminals
#elif (defined KESHAVARZI_KORDESTAN_3)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50     // 50 terminals
#elif (defined KHANALI_TEST_SITE_2)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	100    // 100 terminals
#elif (defined RAHDARI_ZANJAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	300    // 300 terminals
#elif (defined BORKHAR_BEHDASHT)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	20    // 20 terminals
#elif (defined ZAHRA_MIANDOAB)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	64    // 64 terminals
#elif (defined BEHDASHT_TAROM)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	20    // 20 terminals
#elif (defined REFAH_KORDESTAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50    // 50 terminals
#elif (defined BIMARESTAN_TAROM)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	21    // 20 terminals, 1 trunk
#elif (defined PEZESHKI_GHOM)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	101    // 100 terminals, 1 trunk
#elif (defined SHAFA)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	6    // 5 terminals, 1 trunk
#elif (defined LANJAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	7    // 5 terminals, 2 trunks
#elif (defined GERASH_FARS)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	202    // 200 terminals, 2 trunks
#elif (defined ENTEGHAL_KHOON_AZ)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	7      // 5 terminals, 2 trunks
#elif (defined KESHAVARZI_KORDESTAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50	// 50 terminals
#elif (defined BARGH_KORDESTAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	128 //52      // 50 terminals, 2 sip trunks
#elif (defined SHAHRAKE_SANATI_KORDESTAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	10	// 10 terminals
#elif (defined TRAKTORSAZI)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50	// 50 terminals
#elif (defined POLYACRYL_ISFAHAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50	// 50 terminals
#elif (defined DADGOSTARI_KAMYARAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	51	// 50 terminals, 1 trunk
#elif (defined BIMARESTAN_PAVEH)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	101	 // 100 terminals, 1 trunk
#elif (defined SANAYE_DEFA)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	105	 // 100 terminals, 5 trunks
#elif (defined AMOOZESH_2_KORDESTAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	52	// 50 terminals, 2 trunks
#elif (defined ISFAHAN_GHAZA)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	12	// 10 terminals, 2 trunks
#elif (defined SOLAS_KERMANSHAH)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	1002	  // 1000 terminals, 2 trunks
#elif (defined ABFAZELAB_KORDESTAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	102	 // 100 terminals, 2 trunks
#elif (defined KERMAN_STANDARD)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	101	 // 100 terminals, 1 trunk
#elif (defined BOVIR_STANDARD)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	51	// 50 terminals, 1 trunk
#elif (defined AMOOZESH_KORDESTAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	500	 // 500 terminals
#elif (defined BEHZISTI_RAZAVI)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	1001	  // 1000 terminals + 1 trunk
#elif (defined NAFT_ILAM)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50	// 50 terminals
#elif (defined ORUMIEH_ZENDAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50	// 50 terminals
#elif (defined NAFT_SHAHR)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	51	// 50 terminals + 1 trunk
#elif (defined NAFT_SHAHR)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	58	// 50 terminals + 8 trunk
#elif (defined VARZESH_KHORASAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	200
#elif (defined ISFAHAN_HOSPITAL_1)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	101
#elif (defined ISFAHAN_HOSPITAL_2)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	101
#elif (defined KERMAN_HOSPITAL)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	100
#elif (defined KERMAN_EGHTESAD)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	100
#elif (defined ALBORZ)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	200
#else
// following is for operator to compile
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	500
#endif

#define EXT_USES_RTP
#if (defined DR_GHARAI_TEST)
#else
#define DOD_USES_RTP
#endif
#define OPR_SIP_CONNECTION
#endif

#if (defined BANK_MELLI_KERMAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	30
#elif (defined OLOOM_PEZESHKI_KERMAN)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	101
#elif (defined KERMAN_MOODY)
#define MAX_REMOTE_SIP_STATIC_USER_AGENT	50
#endif

#ifdef IRAN_SYSTEM
//#define CONVERT_SIP_INFO_TO_DTMF
#endif

#endif // SIP_TEST

#if ((defined KERMAN) || (defined RAFSANJAN) || (defined KIATEL_FINAL_TEST_FACTORY) || (defined GHAEM_MASHAD))
#define MAX_MT9076_ON_EC1K	7
#endif

#ifdef ISDN_TEST
#define WE_DO_HAVE_BASIC_NETSIDE_ISDN
#endif

#ifdef SAIAN_ERTEBAT
#define CHECK_HW_INFO
#endif

#ifdef KIATEL_TEST2
#define HUNTING_FOR_ICT_USER_ISDN
#endif

#define TONE_TO_BE_HEARD_WHEN_THE_A_PARTY_GOES_ON_HOOK BUSYT

#if (defined KIATEL_TEST2)
#define CR_DEBUG_ACT	      // cr123.c, cr46789.c
//#define CO_DEBUG_ACT			// cr202124.c, cr252629.c
#define DO_NOT_DEFINE_SOFTWARE_SECURITY
//#define OPERATOR_SIDE_DEBUG_ACT // oprtn.c
//#define OPR_DEBUG_ACT   // cr314041.c,cr323334.c,cr424345.c,cr464748.c,
			  // cr505152.c,cr535460.c,cr55.c
#define USER_DEBUG_ACT	// userrtcp.c, u931cp.c
//#define ISDN_DEBUG_ACT
#endif


/* these were used in usr1pri.h for kiatel mirdamad office
#define SUPPORT_PREVIOUS_NUMBERING_VER2 // works on numbering plan
#define SEND_TIME_DATE_FROM_OPR_TO_SYS
*/

#if (OS == DOS)
#ifdef SAIAN_ERTEBAT
#define WE_DO_HAVE_ETHERNET	1
#elif (defined KIATEL_TEST)
#define WE_DO_HAVE_ETHERNET	1
#elif ((defined GHOM_RAILWAY) || (defined BEHIN))
#define WE_DO_HAVE_ETHERNET	0
#elif (defined KIATEL_TEST2)
#define WE_DO_HAVE_ETHERNET	0
#elif (defined PARS_GENERATOR)
#define WE_DO_HAVE_ETHERNET	1
#elif (defined KIATEL_TEST3)
#define WE_DO_HAVE_ETHERNET	1
#elif (defined ISDN_TEST)
#define WE_DO_HAVE_ETHERNET	1
#elif (defined KERMAN)
#define WE_DO_HAVE_ETHERNET	1
#else
#include "ether.h"
#endif
#endif // (OS == DOS)

#if (OS ==  LINUX_COMPATIBLE)
#define KIATEL_UDP_FOR_OAM
#endif

#if (WE_DO_HAVE_ETHERNET != 0)
#define SYSTEM_HAS_ETHERNET	// only in this file
#define ALARM_PANEL_ON_OTHER_RS232
#endif


/* remember to at least:
   generate ..._dpi.c,
   include it in cur_dpi.c,
   and change cur_in.c
*/

#define DO_NOT_ENABLE_WDT

// following for new mi, payame noore shahrekord
//#define SEND_BUSYT_TO_UNANSWERED_ANALOG_TRUNK

// followings only for ghom railway, oloom-pezeshki kerman, traktorsazi tabriz, ostandari ghom
#define SEPERATE_STD_INTERNATIONAL_FOR_CO_GROUPS
// following is now used for access to sip trunk groups
#define SEPERATE_STD_INTERNATIONAL_FOR_CAS3_GROUPS
#define SEPERATE_STD_INTERNATIONAL_FOR_PRI_GROUPS

#ifdef BEHIN
#define LOCK_ON_2ND_E1	// only for behin and daraii_kerman
#endif
//#define ONE_DIGIT_FOR_DIALLING_OP

#ifdef SAIAN_ERTEBAT
#define NO_OPRTR	   2
#elif (defined GHOM_RAILWAY)
#define NO_OPRTR	   2
#elif defined BEHIN
#define NO_OPRTR	   1
#elif (defined KIATEL_TEST2)
#define NO_OPRTR	   1
#elif (defined KIATEL_TEST3)
#define NO_OPRTR	   1
#elif (defined ISDN_TEST)
#define NO_OPRTR	   1
#elif (defined SIP_TEST)
#ifndef NO_OPRTR
#define NO_OPRTR 0	// otherwise, HDLC_RS232 shall be defined
#endif
#elif (defined KERMAN)
#define NO_OPRTR	   5
#else
#define NO_OPRTR	   5
#endif
#define FIRST_NEW_HDLC_COM_PORT 0

#ifndef GHOM_RAILWAY
#if (NO_OPRTR != 0)
#define NEW_DETAIL_BILLING_FOR_OPERATOR
#define OPR_PRI_CONNECTION
#endif
#define NEW_DETAIL_BILLING
#define DO_REPORT_ICT_TRUNK_BILLING
#define RECORD_A_NUMBER
#define DO_REPORT_ICT_PRI_BILLING
#endif


// 16-400, 2 pri,ktnum, ostandari ghom
// 16-64, 2 cas, 2 v5.2 kiatel
// 16-64, no e1, long flash timer, mi
// 16-304, gharazi hospital zahedan, 2 pri, Amplifier, ktnum
// 16-208, no e1, bandar abbas
// 32-304, no e1, karbalaa
// 16-304, Amplifier, daadgostari kerman, with ktnum, 2 user pri, 1 e1 rsu
// 16-400, no e1, havaa-fazaa, with ktnum, CODES 8,9 instead of 2,8
// 32-300, 2 pri, daadgostari golestan
// 56-300, no e1, ktnum, abhar, azad university
// 32-304, 2 pri, behin, with ktnum, 1 cas, LOCK_ON_2ND_E1
// payam airport, like behin with timer for calls
// 32-400, no e1, ktnum, ardebil hospital, 2 operators
// 32-400, no e1, ktnum, parsgenerator	(ACD)
// 16-(208+112) 2 pri, 1 rsu, ktnum, daraii kerman
// 14*8-(20+34)*16, ktnum, yazd shahid sadooghi hospital, 3 operator, has rsu, timer for calls
// 32-18*16 1 user pri, 1 network pri, ktnum, home metering, hoze elmiye tabriz
// 24-496, ktnum, ghom railway, 2 operator, 2 pri, 2 cas, seperate...
// 32-304, no e1, payame noor shahrekord, 2 operator(pc), new mi
// 2*8-(32*16+34*16+34*16), oloom-pezeshki kerman, ktnum, 2 pri, 2 e1 for 2 rsu, timer, seperate...
// payam airport like oloom-pezeshki kerman with  2 operators
// 48-400, traktorsazi tabriz, 2 pri, 2 operator, ktnum, seperate ...,timer
// 56-304, siman shahrekord, 2 operator
// 16-320, 2 cas, acd, ktnum, daneshgahe rafsanjan



#define KIATEL_TREE_BASED_NP
#define MAX_KIATEL_IC_ROUTE	2   // should be defined, however for now not used in CALL_PROCESSOR

//#define  DO_NOT_CHECK_ON_HOOK_TWICE	// if flash time is too short, now has changed, refer to extscn()

//#define WE_DO_HAVE_V52
#ifdef SAIAN_ERTEBAT
#define WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
#elif ((defined GHOM_RAILWAY) || (defined BEHIN))
#define WE_DO_HAVE_CAS3
#elif (defined KIATEL_TEST)
#define WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
#elif (defined PARS_GENERATOR)
#define WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
#elif (defined KIATEL_TEST2)
#elif (defined KIATEL_TEST3)
#define WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
#elif (defined ISDN_TEST)
#define WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
#elif (defined SIP_TEST)
#if (defined NAFT_AHVAZ)
#define RSU_CCSV5_INSTALLED
#elif (defined NAFT_ABADAN)
#define RSU_CCSV5_INSTALLED
#elif (defined MASHAD_HASHEMI)
#define RSU_CCSV5_INSTALLED
#elif (defined ESLAMSHAHR_HOSPITAL)
#define RSU_CCSV5_INSTALLED
#elif (defined DARYAII_MAHMOODABAD)
#define RSU_CCSV5_INSTALLED
#elif (defined KHANALI_TEST_SITE_1)
#define RSU_CCSV5_INSTALLED
#elif (defined ZAHRA_MIANDOAB)
#define RSU_CCSV5_INSTALLED
#elif (defined TRAKTORSAZI)
#define RSU_CCSV5_INSTALLED
#elif (defined POLYACRYL_ISFAHAN)
#define RSU_CCSV5_INSTALLED
#elif (defined KIATEL_FINAL_TEST_FACTORY)
#define RSU_CCSV5_INSTALLED
#elif (defined GHAEM_MASHAD)
#define RSU_CCSV5_INSTALLED
#elif (defined RAFSANJAN)
#define RSU_CCSV5_INSTALLED
#elif (defined KERMAN_HOSPITAL)
#define RSU_CCSV5_INSTALLED
#elif (defined KERMAN_EGHTESAD)
#define RSU_CCSV5_INSTALLED
#elif (defined SHIRAZ_ENG_UNIV)
#define RSU_CCSV5_INSTALLED
#elif (defined SHIRAZ_AGR_UNIV)
#define RSU_CCSV5_INSTALLED
#elif (defined PASTOR_KARAJ)
#define RSU_CCSV5_INSTALLED
#endif // SIP_TEST
//#define WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
#elif (defined KERMAN)
//#define WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
#define RSU_CCSV5_INSTALLED
#else
#define RSU_CCSV5_INSTALLED
#endif

//#define WE_DO_HAVE_SS7
//#define WE_DO_HAVE_GSM

#ifdef SAIAN_ERTEBAT
#elif (defined KIATEL_TEST3)
#elif (defined ISDN_TEST)
#define WE_DO_HAVE_USERSIDE_ISDN
#elif (defined PEZESHKI_TORBAT)
#elif (defined ESLAMSHAHR_HOSPITAL)
#elif (defined DARYAII_MAHMOODABAD)
#elif (defined KHOR_ISFAHAN)
#elif (defined KESHAVARZI_KORDESTAN_4)
#elif (defined KESHAVARZI_KORDESTAN_3)
#elif (defined KERMAN_KHAZANE)
#elif (defined SHAHRAKE_SANATI_KORDESTAN)
#elif (defined ENTEGHAL_KHOON_AZ)
#elif (defined BIMARESTAN_TAROM)
#elif (defined BEHDASHT_TAROM)
#elif (defined BORKHAR_BEHDASHT)
#elif (defined DR_GHARAI_TEST)
#elif (defined FACTORY_TINY_TEST)
#else
#define WE_DO_HAVE_USERSIDE_ISDN
#endif

//#define WE_DO_HAVE_PRIMARY_NETSIDE_ISDN
//#define CASV5_CO

#define TIMER_FOR_EXTRNL_CALL
#ifdef TIMER_FOR_EXTRNL_CALL
#define USE_EXTRNL_CALL_TIMER_FOR_ALL_DOD_CALLS // uses B1/SIREN_ACCESS, do not define BAD_PAYER or SIREN_INSTALLED
#define EXTRNL_CALL_TICK       15000	// 5min
#ifdef USE_EXTRNL_CALL_TIMER_FOR_ALL_DOD_CALLS
#define DO_NOT_SET_TIMER_FOR_XFERED_CALLS
#endif
#endif

#ifdef WE_DO_HAVE_CAS3
#ifdef BEHIN
#define NO_CAS3_DIGITAL_TRUNK	1
#else
#define NO_CAS3_DIGITAL_TRUNK	2
#endif
#endif

#ifdef CASV5_CO
#define NO_ANALOG_TRUNK 			32  //16
#define NO_CASV5_CO				60
#define NO_CASV5_DIGITAL_TRUNK			(NO_CASV5_CO/30)
#define START_OF_CASV5_CO			NO_ANALOG_TRUNK
#define DIGITAL_TRUNK_INDEX_OF_CASV5_START	NO_CAS3_DIGITAL_TRUNK
#else
#define NO_CASV5_CO				0
#define NO_CASV5_DIGITAL_TRUNK			0
#endif

#ifdef SAIAN_ERTEBAT
#define NO_ANALOG_TRUNK 	16
#elif (defined GHOM_RAILWAY)
#define NO_ANALOG_TRUNK 	24
#elif defined BEHIN
#define NO_ANALOG_TRUNK 	32
#elif (defined KIATEL_TEST3)
#define NO_ANALOG_TRUNK 	0
#elif (defined ISDN_TEST)
#define NO_ANALOG_TRUNK 	(8*16)
#elif (defined SIP_TEST)
#ifdef DOD_USES_RTP
#if (defined GHANUNI_KERMAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined NAFT_AHVAZ)
#define NO_ANALOG_TRUNK 	40
#elif (defined NAFT_ABADAN)
#define NO_ANALOG_TRUNK 	40
#elif (defined MASHAD_HASHEMI)
#define NO_ANALOG_TRUNK 	40
#elif (defined FACTORY_TINY_TEST)
#define NO_ANALOG_TRUNK 	16
#elif (defined PEYMANIE_JAHROM)
#define NO_ANALOG_TRUNK 	16
#elif (defined DARAI_TABRIZ)
#define NO_ANALOG_TRUNK 	16
#elif (defined KIATEL_MIRDAMAD)
#define NO_ANALOG_TRUNK 	32
#elif (defined PEZESHKI_ARAK)
#define NO_ANALOG_TRUNK 	32
#elif (defined PEZESHKI_TORBAT)
#define NO_ANALOG_TRUNK 	32
#elif (defined ESLAMSHAHR_HOSPITAL)
#define NO_ANALOG_TRUNK 	32
#elif (defined PEZESHKI_JIROFT)
#define NO_ANALOG_TRUNK 	32
#elif (defined CHAMRAN_SHIRAZ)
#define NO_ANALOG_TRUNK 	32
#elif (defined BASTAR_ISFAHAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined DARYAII_MAHMOODABAD)
#define NO_ANALOG_TRUNK 	32
#elif (defined AZAD_ZANJAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined SABALAN_HOSPITAL)
#define NO_ANALOG_TRUNK 	32
#elif (defined ZARAND_HOSPITAL)
#define NO_ANALOG_TRUNK 	32
#elif (defined DARAI_ZANJAN_2)
#define NO_ANALOG_TRUNK 	32
#elif (defined DARAI_ZANJAN_1)
#define NO_ANALOG_TRUNK 	32
#elif (defined KHANALI_TEST_SITE_1)
#define NO_ANALOG_TRUNK 	64
#elif (defined GHAEM_MASHAD)
#define NO_ANALOG_TRUNK 	112
#elif (defined ZAKARIA_KHORASAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined KHOR_ISFAHAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined PASTOR_KARAJ)
#define NO_ANALOG_TRUNK 	64
#elif (defined SHIRAZ_AGR_UNIV)
#define NO_ANALOG_TRUNK 	64
#elif (defined SHIRAZ_ENG_UNIV)
#define NO_ANALOG_TRUNK 	64
#elif (defined KESHAVARZI_KORDESTAN_5)
#define NO_ANALOG_TRUNK 	32
#elif (defined KESHAVARZI_KORDESTAN_4)
#define NO_ANALOG_TRUNK 	32
#elif (defined KESHAVARZI_KORDESTAN_3)
#define NO_ANALOG_TRUNK 	32
#elif (defined KHANALI_TEST_SITE_2)
#define NO_ANALOG_TRUNK 	64
#elif (defined RAHDARI_ZANJAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined BORKHAR_BEHDASHT)
#define NO_ANALOG_TRUNK 	32
#elif (defined ZAHRA_MIANDOAB)
#define NO_ANALOG_TRUNK 	64  //48
#elif (defined BEHDASHT_TAROM)
#define NO_ANALOG_TRUNK 	32
#elif (defined REFAH_KORDESTAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined BIMARESTAN_TAROM)
#define NO_ANALOG_TRUNK 	32
#elif (defined PEZESHKI_GHOM)
#define NO_ANALOG_TRUNK 	32
#elif (defined SHAFA)
#define NO_ANALOG_TRUNK 	16
#elif (defined LANJAN)
#define NO_ANALOG_TRUNK 	64
#elif (defined GERASH_FARS)
#define NO_ANALOG_TRUNK 	64
#elif (defined ENTEGHAL_KHOON_AZ)
#define NO_ANALOG_TRUNK 	64
#elif (defined KESHAVARZI_KORDESTAN)
#define NO_ANALOG_TRUNK 	48
#elif (defined BARGH_KORDESTAN)
#define NO_ANALOG_TRUNK 	32 //48
#elif (defined SHAHRAKE_SANATI_KORDESTAN)
#define NO_ANALOG_TRUNK 	48
#elif (defined TRAKTORSAZI)
#define NO_ANALOG_TRUNK 	80
#elif (defined POLYACRYL_ISFAHAN)
#define NO_ANALOG_TRUNK 	48
#elif (defined DADGOSTARI_KAMYARAN)
#define NO_ANALOG_TRUNK 	48
#elif (defined BIMARESTAN_PAVEH)
#define NO_ANALOG_TRUNK 	32
#elif (defined SANAYE_DEFA)
#define NO_ANALOG_TRUNK 	32
#elif (defined AMOOZESH_2_KORDESTAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined ISFAHAN_GHAZA)
#define NO_ANALOG_TRUNK 	48
#elif (defined SOLAS_KERMANSHAH)
#define NO_ANALOG_TRUNK 	48
#elif (defined ABFAZELAB_KORDESTAN)
#define NO_ANALOG_TRUNK 	48
#elif (defined KERMAN_STANDARD)
#define NO_ANALOG_TRUNK 	48
#elif (defined BOVIR_STANDARD)
#define NO_ANALOG_TRUNK 	48
#elif (defined AMOOZESH_KORDESTAN)
#define NO_ANALOG_TRUNK 	48
#elif (defined BEHZISTI_RAZAVI)
#define NO_ANALOG_TRUNK 	48
#elif (defined NAFT_ILAM)
#define NO_ANALOG_TRUNK 	48
#elif (defined ORUMIEH_ZENDAN)
#define NO_ANALOG_TRUNK 	48
#elif (defined KIATEL_FINAL_TEST_FACTORY)
#define NO_ANALOG_TRUNK 	16
#elif (defined KERMAN_KHAZANE)
#define NO_ANALOG_TRUNK 	32
#elif (defined NAFT_SHAHR)
#define NO_ANALOG_TRUNK 	48
#elif (defined VARZESH_KHORASAN)
#define NO_ANALOG_TRUNK 	32
#elif (defined ISFAHAN_HOSPITAL_1)
#define NO_ANALOG_TRUNK 	64
#elif (defined ISFAHAN_HOSPITAL_2)
#define NO_ANALOG_TRUNK 	64
#elif (defined KERMAN_HOSPITAL)
#define NO_ANALOG_TRUNK 	16
#elif (defined KERMAN_EGHTESAD)
#define NO_ANALOG_TRUNK 	64
#elif (defined ALBORZ)
#elif (defined KIATEL_FACTORY)
#define NO_ANALOG_TRUNK 	32   //16
#else
#define NO_ANALOG_TRUNK 	16
#endif
#else  // DOD_USES_RTP
#define NO_ANALOG_TRUNK 	0
#endif // DOD_USES_RTP
#elif (defined KERMAN)
#define NO_ANALOG_TRUNK 	(8*16)	// 8 cards, each 16 dod, some maximum
#else
#ifdef DOD16	// no difference!
#define NO_ANALOG_TRUNK 	(8*16)	// 8 cards, each 16 dod
#else
#define NO_ANALOG_TRUNK 	(16*8)	//(8*8)
#endif
#endif



#ifdef SAIAN_ERTEBAT
#define NO_ANALOG_EXTENSION	32
#elif (defined GHOM_RAILWAY)
#define NO_ANALOG_EXTENSION	496
#elif (defined BEHIN)
#define NO_ANALOG_EXTENSION	304
#elif (defined ISDN_TEST)
#define NO_ANALOG_EXTENSION	(32*16)
#elif (defined KERMAN)
#define NO_ANALOG_EXTENSION	416	// slots 0-22,31-34  can be used, max total is (23+4)*16=432
					// slots 23-30 used for extra 3 e1
#elif (defined RAFSANJAN)
#define NO_ANALOG_EXTENSION	352	// slots 0-22 can be used, max total is 23*16=368
					// slots 23-30 used for extra 3 e1, 31-34 used for rtp
					// in practice, slot 0 is used for dod, slot 7 is blank
					// so practical maximum is 336
#elif (defined GHANUNI_KERMAN)
#define NO_ANALOG_EXTENSION	96
#elif (defined NAFT_AHVAZ)
#define NO_ANALOG_EXTENSION	400
#elif (defined NAFT_ABADAN)
#define NO_ANALOG_EXTENSION	400
#elif (defined MASHAD_HASHEMI)
#define NO_ANALOG_EXTENSION	400
#elif (defined FACTORY_TINY_TEST)
#define NO_ANALOG_EXTENSION	16
#elif (defined PEYMANIE_JAHROM)
#define NO_ANALOG_EXTENSION	320
#elif (defined DARAI_TABRIZ)
#define NO_ANALOG_EXTENSION	224
#elif (defined KIATEL_MIRDAMAD)
#define NO_ANALOG_EXTENSION	112
#elif (defined PEZESHKI_ARAK)
#define NO_ANALOG_EXTENSION	448
#elif (defined PEZESHKI_TORBAT)
#define NO_ANALOG_EXTENSION	384
#elif (defined ESLAMSHAHR_HOSPITAL)
#define NO_ANALOG_EXTENSION	448
#elif (defined PEZESHKI_JIROFT)
#define NO_ANALOG_EXTENSION	160
#elif (defined CHAMRAN_SHIRAZ)
#define NO_ANALOG_EXTENSION	208
#elif (defined BASTAR_ISFAHAN)
#define NO_ANALOG_EXTENSION	64
#elif (defined DARYAII_MAHMOODABAD)
#define NO_ANALOG_EXTENSION	144  //64
#elif (defined AZAD_ZANJAN)
#define NO_ANALOG_EXTENSION	208
#elif (defined SABALAN_HOSPITAL)
#define NO_ANALOG_EXTENSION	368
#elif (defined ZARAND_HOSPITAL)
#define NO_ANALOG_EXTENSION	400
#elif (defined DARAI_ZANJAN_2)
#define NO_ANALOG_EXTENSION	256
#elif (defined DARAI_ZANJAN_1)
#define NO_ANALOG_EXTENSION	352
#elif (defined GHAEM_MASHAD)
#define NO_ANALOG_EXTENSION	240
#elif (defined DR_GHARAI_TEST)
#define NO_ANALOG_EXTENSION	16
#elif (defined ZAKARIA_KHORASAN)
#define NO_ANALOG_EXTENSION	224
#elif (defined KHOR_ISFAHAN)
#define NO_ANALOG_EXTENSION	320
#elif (defined PASTOR_KARAJ)
#define NO_ANALOG_EXTENSION	416
#elif (defined SHIRAZ_AGR_UNIV)
#define NO_ANALOG_EXTENSION	416
#elif (defined SHIRAZ_ENG_UNIV)
#define NO_ANALOG_EXTENSION	416
#elif (defined KESHAVARZI_KORDESTAN_5)
#define NO_ANALOG_EXTENSION	256
#elif (defined KESHAVARZI_KORDESTAN_4)
#define NO_ANALOG_EXTENSION	96
#elif (defined KESHAVARZI_KORDESTAN_3)
#define NO_ANALOG_EXTENSION	96
#elif (defined KHANALI_TEST_SITE_2)
#define NO_ANALOG_EXTENSION	256
#elif (defined RAHDARI_ZANJAN)
#define NO_ANALOG_EXTENSION	160
#elif (defined BORKHAR_BEHDASHT)
#define NO_ANALOG_EXTENSION	64
#elif (defined ZAHRA_MIANDOAB)
#define NO_ANALOG_EXTENSION	416  //368
#elif (defined BEHDASHT_TAROM)
#define NO_ANALOG_EXTENSION	96
#elif (defined REFAH_KORDESTAN)
#define NO_ANALOG_EXTENSION	96
#elif (defined BIMARESTAN_TAROM)
#define NO_ANALOG_EXTENSION	96
#elif (defined PEZESHKI_GHOM)
#define NO_ANALOG_EXTENSION	256
#elif (defined SHAFA)
#define NO_ANALOG_EXTENSION	320
#elif (defined LANJAN)
#define NO_ANALOG_EXTENSION	320
#elif (defined GERASH_FARS)
#define NO_ANALOG_EXTENSION	128
#elif (defined ENTEGHAL_KHOON_AZ)
#define NO_ANALOG_EXTENSION	128
#elif (defined KESHAVARZI_KORDESTAN)
#define NO_ANALOG_EXTENSION	96
#elif (defined BARGH_KORDESTAN)
#define NO_ANALOG_EXTENSION	144 //128
#elif (defined SHAHRAKE_SANATI_KORDESTAN)
#define NO_ANALOG_EXTENSION	80
#elif (defined TRAKTORSAZI)
#define NO_ANALOG_EXTENSION	320
#elif (defined POLYACRYL_ISFAHAN)
#define NO_ANALOG_EXTENSION	416
#elif (defined DADGOSTARI_KAMYARAN)
#define NO_ANALOG_EXTENSION	144
#elif (defined BIMARESTAN_PAVEH)
#define NO_ANALOG_EXTENSION	128
#elif (defined SANAYE_DEFA)
#define NO_ANALOG_EXTENSION	64
#elif (defined AMOOZESH_2_KORDESTAN)
#define NO_ANALOG_EXTENSION	96
#elif (defined ISFAHAN_GHAZA)
#define NO_ANALOG_EXTENSION	160
#elif (defined SOLAS_KERMANSHAH)
#define NO_ANALOG_EXTENSION	272
#elif (defined ABFAZELAB_KORDESTAN)
#define NO_ANALOG_EXTENSION	112
#elif (defined KERMAN_STANDARD)
#define NO_ANALOG_EXTENSION	96
#elif (defined BOVIR_STANDARD)
#define NO_ANALOG_EXTENSION	160
#elif (defined AMOOZESH_KORDESTAN)
#define NO_ANALOG_EXTENSION	144
#elif (defined BEHZISTI_RAZAVI)
#define NO_ANALOG_EXTENSION	256
#elif (defined NAFT_ILAM)
#define NO_ANALOG_EXTENSION	200
#elif (defined ORUMIEH_ZENDAN)
#define NO_ANALOG_EXTENSION	112
#elif (defined KIATEL_FINAL_TEST_FACTORY)
#define NO_ANALOG_EXTENSION	320
#elif (defined KERMAN_KHAZANE)
#define NO_ANALOG_EXTENSION	96
#elif (defined NAFT_SHAHR)
#define NO_ANALOG_EXTENSION	256
#elif (defined VARZESH_KHORASAN)
#define NO_ANALOG_EXTENSION	160
#elif (defined ISFAHAN_HOSPITAL_1)
#define NO_ANALOG_EXTENSION	480
#elif (defined ISFAHAN_HOSPITAL_2)
#define NO_ANALOG_EXTENSION	480
#elif (defined KERMAN_HOSPITAL)
#define NO_ANALOG_EXTENSION	480
#elif (defined KERMAN_EGHTESAD)
#define NO_ANALOG_EXTENSION	256
#elif (defined ALBORZ)
#define NO_ANALOG_EXTENSION	96
#else
/*
#ifdef DOD16
#define NO_ANALOG_EXTENSION	((34-(NO_ANALOG_TRUNK/16))*16) // (34*16)
#else
#define NO_ANALOG_EXTENSION	((34-(NO_ANALOG_TRUNK/8))*16) // (34*16)
#endif
*/
// now they can define by dodslot.dat
#define NO_ANALOG_EXTENSION	(34*16)
#endif

#ifdef WE_DO_HAVE_USERSIDE_ISDN
#ifdef SAIAN_ERTEBAT
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined SIP_TEST)
#if (defined GHANUNI_KERMAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined NAFT_AHVAZ)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined NAFT_ABADAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined MASHAD_HASHEMI)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined PEYMANIE_JAHROM)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined DARAI_TABRIZ)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined KIATEL_MIRDAMAD)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined PEZESHKI_ARAK)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined PEZESHKI_JIROFT)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined CHAMRAN_SHIRAZ)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined BASTAR_ISFAHAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined AZAD_ZANJAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined SABALAN_HOSPITAL)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined KESHAVARZI_KORDESTAN_5)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined ZARAND_HOSPITAL)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined DARAI_ZANJAN_2)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined DARAI_ZANJAN_1)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined KHANALI_TEST_SITE_1)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined GHAEM_MASHAD)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	3
#elif (defined ZAKARIA_KHORASAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined PASTOR_KARAJ)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined SHIRAZ_AGR_UNIV)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined SHIRAZ_ENG_UNIV)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined KHANALI_TEST_SITE_2)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	4
#elif (defined RAHDARI_ZANJAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined ZAHRA_MIANDOAB)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined REFAH_KORDESTAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined OLOOM_PEZESHKI_KERMAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined PEZESHKI_GHOM)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined SHAFA)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined LANJAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined GERASH_FARS)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined KESHAVARZI_KORDESTAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined BARGH_KORDESTAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined TRAKTORSAZI)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined POLYACRYL_ISFAHAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined DADGOSTARI_KAMYARAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined BIMARESTAN_PAVEH)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined SANAYE_DEFA)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined AMOOZESH_2_KORDESTAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined ISFAHAN_GHAZA)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined SOLAS_KERMANSHAH)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined ABFAZELAB_KORDESTAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined KERMAN_STANDARD)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined BOVIR_STANDARD)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined AMOOZESH_KORDESTAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined BEHZISTI_RAZAVI)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined NAFT_ILAM)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined ORUMIEH_ZENDAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined KIATEL_FINAL_TEST_FACTORY)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined RAFSANJAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined NAFT_SHAHR)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	1
#elif (defined VARZESH_KHORASAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined ISFAHAN_HOSPITAL_1)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined ISFAHAN_HOSPITAL_2)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined KERMAN_HOSPITAL)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined KERMAN_EGHTESAD)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#elif (defined ALBORZ)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#else
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	4
#endif
#elif (defined KERMAN)
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#else
#define NO_USERSIDE_PRIMARY_DIGITAL_TRUNK	2
#endif
#endif


#if (NO_ANALOG_TRUNK != 0)
#define CO_CARD_INSTALLED
#endif

#ifdef SYSTEM_HAS_ETHERNET
//#define ETHERNET_DEBUG_ACT
#define KIATEL_UDP_FOR_OAM
#define MY_UDP_FOR_OAM
//#define HTBY_LINK_ON_MY_UDP
#define KFTPD_INSTALLED
#define TELNETD_INSTALLED
#endif

#ifdef KFTPD_INSTALLED
//#define KA9Q_TCP_DEBUG_ACT
//#define FTP_DEBUG_ACT
#define KA9Q_MSDOS
#define UNIX_DIR_LIST
#define MD5AUTHENTICATE
#endif

#if (((defined MY_UDP_FOR_OAM) && (OS == DOS)) || \
     ((defined HTBY_LINK_ON_MY_UDP) && (defined SYSTEM_SIDE)) || \
     ((defined KIATEL_UDP_FOR_OAM)  && (defined SYSTEM_SIDE) && (OS == DOS)) \
    )
#define KIATEL_UDP_INSTALLED
#endif

#ifdef KIATEL_UDP_FOR_OAM
#if (OS == DOS)
#define NO_KIATEL_UDP_OAM_TERMINAL	NO_OPRTR   // 3
#endif
#if (OS ==  LINUX_COMPATIBLE)
#if ((defined KIATEL_FACTORY) || (defined KHANALI_TEST_SITE_2) || \
     (defined KHANALI_TEST_SITE_1) || (defined ISFAHAN_HOSPITAL_1) || \
     (defined DARAI_ZANJAN_1) || (defined DARAI_ZANJAN_2) || \
     (defined ZARAND_HOSPITAL) || (defined SABALAN_HOSPITAL) || \
     (defined AZAD_ZANJAN) || (defined DARYAII_MAHMOODABAD) || \
     (defined BASTAR_ISFAHAN) || (defined CHAMRAN_SHIRAZ) || \
     (defined PEZESHKI_JIROFT) || (defined ESLAMSHAHR_HOSPITAL) || \
     (defined PEZESHKI_TORBAT) || (defined PEZESHKI_ARAK) || \
     (defined BOVIR_STANDARD) || (defined PEZESHKI_GHOM) || \
     (defined KIATEL_MIRDAMAD) || (defined DARAI_TABRIZ) || \
     (defined PEYMANIE_JAHROM) || (defined MASHAD_HASHEMI) || \
     (defined NAFT_ABADAN) || (defined NAFT_AHVAZ) || \
     (defined GHANUNI_KERMAN))
#define NO_KIATEL_UDP_OAM_TERMINAL	 3
#else
#define NO_KIATEL_UDP_OAM_TERMINAL	 2
#endif
#define BILLING_ON_HARD_DISK
#endif
#endif

#define CALL_PROCESSOR

#ifdef RSU_CCSV5_INSTALLED
#if (defined NAFT_AHVAZ)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(112)
#elif (defined NAFT_ABADAN)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(112)
#elif (defined MASHAD_HASHEMI)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(128)
#elif (defined ESLAMSHAHR_HOSPITAL)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(80)
#elif (defined DARYAII_MAHMOODABAD)
#define NO_RSU				2
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(128)
#elif (defined KHANALI_TEST_SITE_1)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(544)
#elif (defined ZAHRA_MIANDOAB)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(112)
#elif (defined TRAKTORSAZI)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(240)
#elif (defined GHAEM_MASHAD)
#define NO_RSU				3
#define NO_CCSV5_DIGITAL_TRUNK		4
#define NO_RSU_EXT			(NO_RSU*34*16)
#elif (defined POLYACRYL_ISFAHAN)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(192)
#elif ((defined KERMAN) || (defined RAFSANJAN) || (defined KIATEL_FINAL_TEST_FACTORY))
#define NO_RSU				5
#define NO_CCSV5_DIGITAL_TRUNK		5
#define NO_RSU_EXT			(NO_RSU*34*16)
#elif (defined KERMAN_HOSPITAL)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			256
#elif (defined KERMAN_EGHTESAD)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			112
#elif (defined SHIRAZ_ENG_UNIV)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			192
#elif (defined SHIRAZ_AGR_UNIV)
#define NO_RSU				1
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			384
#elif (defined PASTOR_KARAJ)
#define NO_RSU				2
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			592
#else
#define NO_RSU				2
#define NO_CCSV5_DIGITAL_TRUNK		2
#define NO_RSU_EXT			(NO_RSU*34*16)
#endif
#endif

#ifdef SIP_TEST
#define BEHNAMIR		// 16 bit index, 8 bit type, and related stuff
#define SIP_INSTALLED
#define RTP_INSTALLED
#define DO_NOT_USE_RTPMAN_HANDLER
#define QUICK_PRINT
#ifdef QUICK_PRINT
#define ENHANCED_PRINT
#endif
/*
#define PRINT_NOW
#define SAVE_GONZALES_ON_EACH_WRITE
*/
//#define MAX_PERMITTED_GONZALES_LENGTH   300000000L
#endif // SIP_TEST

//... start of sip

#ifdef SIP_INSTALLED

#define SIP_IVR
#define USE_SIP_MUSIC_PLAYER
#define USE_SIP_CWT_PLAYER

#if ((defined SABALAN_HOSPITAL) || (defined KIATEL_FACTORY) ||\
     (defined ESLAMSHAHR_HOSPITAL) || (defined KHANALI_TEST_SITE_1))
#define INTERCEPTION_UNLAWFUL_INSTALLED
#endif

#if ((defined INTERCEPTION_UNLAWFUL_INSTALLED) || (defined USE_SIP_MUSIC_PLAYER))
#define DYNAMIC_EXT_INSTALLED
#endif

#define SUPPORT_SIP_VIDEO
//#define SIP_GOOYA_OPERATOR  what is this?!!

#define SEPERATE_STD_INTERNATIONAL_FOR_SIP_ON_CO_GROUPS
#define SEPERATE_STD_INTERNATIONAL_FOR_SIP_ON_PRI_GROUPS
#define SEPERATE_STD_INTERNATIONAL_FOR_SIP_ON_SIP_GROUPS

#define LIMIT_NUMBER_OF_INCOMING_SIP_CALL_PER_USER
#ifdef LIMIT_NUMBER_OF_INCOMING_SIP_CALL_PER_USER
#define MAX_NUMBER_OF_INCOMING_SIP_CALL_FOR_SIP_TERMINAL	2
#define MAX_NUMBER_OF_INCOMING_SIP_CALL_FOR_SIP_TRUNK		50
#endif

#define SEND_ANNOUNCE_TO_SIP
#define SIP_TERMINAL_SENDS_BYE_INSTEAD_OF_CANCEL
#define SEND_SIP_DTMF_ON_CO

#define MEMORY_DEBUG
#define IN_DEPTH_BACKTRACK

#define SUPPRESS_UNNECESSARY_SIP_ERROR_MESSAGES
//#define SIP_LEVEL1_DEBUG_ACT
//#define SIP_LEVEL2_DEBUG_ACT
#ifndef SIP_LEVEL1_DEBUG_ACT
#define SIP_ON_REQUEST_DEBUG_ACT
#endif

#define REGISTER_SIP_TERMINAL

#ifdef REGISTER_SIP_TERMINAL
#define SIP_TERMINAL_CALL_FORWARD
// following can also be used for systems not having sip/rtp
#define SIP_CALL_PICKUP
#define SIP_BLF
#ifdef SIP_BLF
#define XML_INSTALLED
#define BLF_FOR_ANALOG
#if (NO_ANALOG_TRUNK != 0)
#define BLF_FOR_DOD
#endif
#define DO_NOT_ALLOW_MORE_THAN_ONE_BLF_SUBSCRIPTION
// following assumes USE_OPTIONS_TO_CHECK_IF_SIP_TERMINAL_IS_CONNECTED is defined
//#define USE_BLF_FOR_PRESENCE

#define MAX_SIP_REGISTRAR_REC	((MAX_REMOTE_SIP_STATIC_USER_AGENT+ \
				  NO_ANALOG_TRUNK+		    \
				  NO_ANALOG_EXTENSION+		    \
				  NO_RSU_EXT			    \
				 )*2)

#endif
#define ACCEPT_SIP_PACKET_FROM_NEW_IP
#define DES_INSTALLED
#define SEND_OPTIONS_FOR_TERMINALS
#ifdef SEND_OPTIONS_FOR_TERMINALS
#define USE_OPTIONS_TO_CHECK_IF_SIP_TERMINAL_IS_CONNECTED
#ifdef USE_OPTIONS_TO_CHECK_IF_SIP_TERMINAL_IS_CONNECTED
#define DISCONNECT_ANSWERED_SIP_CALL_IF_TERMINAL_IS_DISCONNECTED
#endif
#endif
#endif

//#define SIP_PARSER_DEBUG_ACT
//#define SDP_PARSER_DEBUG_ACT

#define USE_FIXED_PORT_5061_AS_OUR_LOCAL_PORT
//#define SIP_PRACK_REQUIRED

#define KIATEL_MGC_MGC_TEST
#define MIME_INSTALLED
#define SIP_ON_UDP_INSTALLED
#define UDP_ON_SERVER_SIDE
#define TEL_INSTALLED
#define NO_OF_COMPLETE_SIP_DIGITS     11
#define CAN_FLASH_ON_SIP
#define DO_NOT_ACTIVATE_REANSWER_FOR_ICT_SIP

#define KEEP_SERVER_XACT1_WHICH_HAS_SEND_2XX_RESPONSE_TILL_ACK_ARRIVES
// following is to reject packets which should be sent to our server port,
// however are mistakenly sent to our call-initiating client port
// like re-invite received for a sipo or a bye received for sipo
// generally with this defined, we assume all 'requests' should only be
// received on our server port (UAS)
// Testing with some sip terminal proved we are wrong. Certain sip terminal
// sends BYE for our ogt sip to our client port
//#define REJECT_PACKETS_ON_OUR_CLIENT_PORT_WHICH_SHOULD_HAVE_BEEN_SENT_TO_OUR_SERVER_PORT
#define INCLUDE_OG_FLAG_IN_SIP_API_HASH
#define DO_NOT_INCLUDE_ANONYMOUS_CONTACT_FIELD_IN_SIP_TRYING_RESPONSE
#define DO_NOT_INCLUDE_ANONYMOUS_CONTACT_FIELD_IN_SIP_CANCEL_RESPONSE
#define DO_NOT_LEAVE_OGT_SIP_CONTACT_FIELD_AS_ANONYMOUS
#define DO_NOT_SET_ICT_SIP_CONTACT_FIELD_AS_ANONYMOUS
#define SEND_REINVITE_FOR_SUSPEND
// we need to have SEND_REINVITE_FOR_SUSPEND for following to work
#define SEND_REINVITE_FOR_RTP_CHANGE
#ifdef SEND_REINVITE_FOR_SUSPEND
// if above is defined, it is good to also have two followings
#define SAVE_SIP_SDP_INFO
#ifdef SAVE_SIP_SDP_INFO
#define INCREMENT_SDP_SESSION_VERSION_FOR_REINVITE_RESPONSE
#endif
#endif

#define UDP_ON_CLIENT_SIDE
#define SDP_INSTALLED
#define FLEX_VERSION_2_5_33

#define DO_NOT_DEFINE_CB
#define DO_NOT_DEFINE_R1_SIGNALING
#define DO_NOT_DEFINE_CONFERENCE
#endif // SIP_INSTALLED

//... end of sip

#ifdef RTP_INSTALLED
//#define RTP_DEBUG_ACT
#define WHICH_SLOTS_FOR_RTP
#define INITIALIZE_RTP_CARD_NEW_METHOD
#ifdef INITIALIZE_RTP_CARD_NEW_METHOD
#define EC1K_RTP_SLOT_START			0	// only to also support old method
#define MAX_RTP_PROCESSOR_CARD			1
#define MAX_RTP_CH_PER_PROCESSOR_CARD		256
#else
#define EC1K_RTP_SLOT_START			0
#define MAX_RTP_PROCESSOR_CARD			1
#define MAX_RTP_CH_PER_PROCESSOR_CARD		16  //8
#endif
#define KIATEL_RTP_PROCESSOR_CARD
#define GW_CUPP_CARD9_DEFAULT_IPV4		"192.168.100.90"
#define RTPMAN_REMOTE_UDP_PORT			4000
#define RTPMAN_LOCAL_UDP_PORT			4000
#endif // RTP_INSTALLED

#ifdef RTP_INSTALLED
#define SOCKET_INSTALLED
#endif

#include "/ec/work/spec/ec1knew.h"
