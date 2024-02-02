
//DYNAMIC start : ??????????????
//#include <general.h>
#include "omtfp.h"
//DYNAMIC end

//DYNAMIC : commented
//#ifdef NEW_DEP
extern int no_extension[];
extern int no_my_extension[];
extern int no_co[];
extern int no_tie[];
extern int no_em[];
extern int no_cas3[];
extern int no_cas1[];
extern int no_em_group[];
//CBADDED
extern int no_cb[];
//DYNAMIC start
extern unsigned int no_cas1_channel[];
extern unsigned int no_cas3_channel[];
extern GRP_TYPE no_cas3_programmable_group[];
extern GRP_TYPE no_cas1_group[];
//DYNAMIC end
void dep_fill_params(void){

	no_extension[0]=NO_EXTENSION;
	no_my_extension[0]=NO_MY_EXTENSION;
	no_co[0]=NO_CO;
	no_tie[0]=NO_TIE_TRUNK;
#ifdef NO_EM
	no_em[0]=NO_EM;
#else
	no_em[0]=0;
#endif
//DYNAMIC : #ifdef NO_CAS3 -->
#ifdef EXTERNAL_CAS3_INSTALLED
	no_cas3[0]=NO_CAS3;
//DYNAMIC : this line added.
	no_cas3_programmable_group[0]=NO_CAS3_PROGRAMMABLE_GROUP;
#else
	no_cas3[0]=0;
//DYNAMIC : this line added.
	no_cas3_programmable_group[0]=0;
#endif
//DYNAMIC start : this block added.
#ifdef CAS3_INSTALLED
	no_cas3_channel[0]=NO_CAS3_CHANNEL;
#else
	no_cas3_channel[0]=0;
#endif
//DYNAMIC end
//DYNAMIC : #ifdef NO_CAS1 -->
#ifdef CAS1_INSTALLED
	no_cas1[0]=NO_CAS1;
//DYNAMIC : this line added.
	no_cas1_channel[0]=NO_CAS1_CHANNEL;
	no_cas1_group[0]=NO_CAS1_GROUP;
#else
	no_cas1[0]=0;
//DYNAMIC : this line added.
	no_cas1_channel[0]=0;
	no_cas1_group[0]=0;
#endif

#ifdef NO_EM_GROUP
	no_em_group[0]=NO_EM_GROUP;
#else
	no_em_group[0]=0;
#endif
//CBADDED start
#ifdef CB_INSTALLED
	no_cb[0]=NO_CB;
#else
	no_cb[0]=0;
#endif
//CBADDED end
#ifdef LIM

#undef NO_EXTENSION
#undef NO_CO
#undef NO_TIE_TRUNK
#undef NO_EM

#undef NO_DIGITAL_TRUNK
#undef NO_CAS3_DIGITAL_TRUNK
#undef NO_CAS3_GROUP
#undef NO_CAS3_IC_CH
#undef NO_CAS3_OG_CH
#undef LIM_OGT_GROUP
#undef LIM2_OGT_GROUP
#undef ANALOG_CARD_START_SLOT
#undef NO_CAS3
#undef NO_DT_GROUP
#undef CAS1_INSTALLED
#undef NO_CAS1
//CBADDED start
#undef NO_CB
#undef CB_INSTALLED
//CBADDED end
#undef NO_CAS1_DIGITAL_TRUNK
#undef NO_CAS1_GROUP
#undef NO_CAS1_IC_CH
#undef NO_CAS1_OG_CH

/* following undefs only for suppressing warning messages */

#undef NO_LINE_CARD_IN_SYS
#undef LINE_CARD_START_SLOT
#undef SELF_CENTER
#undef FIRST_DIRECTORY_NUM
#undef SELF_CENTER_GROUP_NUMBER
#undef SELF_CENTER_GROUP_NUMBER_D2D1
#undef NO_ANALOG_CARD
#undef NO_ANALOG_TRUNK
#undef DTMF_CARD_START_SLOT
/* 17/9/76: also for undefining previously "in lim 'a' defined" items.*/
#undef EM_INSTALLED
#undef NO_EM
#undef NO_CAS3
#undef NO_EM_GROUP
#undef GLOBAL_NIGHT_EXT
#undef CONFERENCE_ST_BUS
#undef NO_LOAD_PARM1

#include <generalb.h>

	no_extension[1]=NO_EXTENSION;
	no_my_extension[1]=NO_EXTENSION;
	no_co[1]=NO_CO;
	no_tie[1]=NO_TIE_TRUNK;
#ifdef NO_EM
	no_em[1]=NO_EM;
#else
	no_em[1]=0;
#endif
//DYNAMIC : #ifdef NO_CAS3 -->
#ifdef EXTERNAL_CAS3_INSTALLED
	no_cas3[1]=NO_CAS3;
//DYNAMIC : this line added.
	no_cas3_programmable_group[1]=NO_CAS3_PROGRAMMABLE_GROUP;
#else
	no_cas3[1]=0;
//DYNAMIC : this line added.
	no_cas3_programmable_group[1]=0;
#endif
//DYNAMIC start : this block added.
#ifdef CAS3_INSTALLED
	no_cas3_channel[1]=NO_CAS3_CHANNEL;
#else
	no_cas3_channel[1]=0;
#endif
//DYNAMIC end
//DYNAMIC : #ifdef NO_CAS1 -->
#ifdef CAS1_INSTALLED
	no_cas1[1]=NO_CAS1;
//DYNAMIC : this line added.
	no_cas1_channel[1]=NO_CAS1_CHANNEL;
	no_cas1_group[1]=NO_CAS1_GROUP;
#else
	no_cas1[1]=0;
//DYNAMIC : this line added.
	no_cas1_channel[1]=0;
	no_cas1_group[1]=0;
#endif
#ifdef NO_EM_GROUP
	no_em_group[1]=NO_EM_GROUP;
#else
	no_em_group[1]=0;
#endif
//CBADDED start
#ifdef CB_INSTALLED
	no_cb[1]=NO_CB;
#else
	no_cb[1]=0;
#endif

//CBADDED end
#if(NO_EXTENSION_C != 0)

#undef NO_EXTENSION
#undef NO_CO
#undef NO_TIE_TRUNK
#undef NO_EM

#undef NO_DIGITAL_TRUNK
#undef NO_CAS3_DIGITAL_TRUNK
#undef NO_CAS3_GROUP
#undef NO_CAS3_IC_CH
#undef NO_CAS3_OG_CH
#undef LIM_OGT_GROUP
#undef LIM2_OGT_GROUP
#undef ANALOG_CARD_START_SLOT
#undef NO_CAS3
#undef NO_DT_GROUP
#undef CAS1_INSTALLED
#undef NO_CAS1
//CBADDED start
#undef NO_CB
#undef CB_INSTALLED
//CBADDED end
#undef NO_CAS1_DIGITAL_TRUNK
#undef NO_CAS1_GROUP
#undef NO_CAS1_IC_CH
#undef NO_CAS1_OG_CH

/* following undefs only for suppressing warning messages */

#undef NO_LINE_CARD_IN_SYS
#undef LINE_CARD_START_SLOT
#undef SELF_CENTER
#undef FIRST_DIRECTORY_NUM
#undef SELF_CENTER_GROUP_NUMBER
#undef SELF_CENTER_GROUP_NUMBER_D2D1
#undef NO_ANALOG_CARD
#undef NO_ANALOG_TRUNK
#undef DTMF_CARD_START_SLOT
/* 17/9/76: also for undefining previously "in lim 'a' defined" items.*/
#undef EM_INSTALLED
#undef NO_EM
#undef NO_CAS3
#undef NO_EM_GROUP
#undef GLOBAL_NIGHT_EXT
#undef CONFERENCE_ST_BUS
#undef NO_LOAD_PARM1

#include <generalc.h>

	no_extension[2]=NO_EXTENSION;
	no_my_extension[2]=NO_EXTENSION;
	no_co[2]=NO_CO;
	no_tie[2]=NO_TIE_TRUNK;
#ifdef NO_EM
	no_em[2]=NO_EM;
#else
	no_em[2]=0;
#endif
//DYNAMIC : #ifdef NO_CAS3 -->
#ifdef EXTERNAL_CAS3_INSTALLED
	no_cas3[2]=NO_CAS3;
//DYNAMIC : this line added.
	no_cas3_programmable_group[2]=NO_CAS3_PROGRAMMABLE_GROUP;
#else
	no_cas3[2]=0;
//DYNAMIC : this line added.
	no_cas3_programmable_group[2]=0;
#endif
//DYNAMIC start : this block added.
#ifdef CAS3_INSTALLED
	no_cas3_channel[2]=NO_CAS3_CHANNEL;
#else
	no_cas3_channel[2]=0;
#endif
//DYNAMIC end
//DYNAMIC : #ifdef NO_CAS1 -->
#ifdef CAS1_INSTALLED
	no_cas1[2]=NO_CAS1;
//DYNAMIC : this line added.
	no_cas1_channel[2]=NO_CAS1_CHANNEL;
	no_cas1_group[2]=NO_CAS1_GROUP;
#else
	no_cas1[2]=0;
//DYNAMIC : this line added.
	no_cas1_channel[2]=0;
	no_cas1_group[2]=0;
#endif
#ifdef NO_EM_GROUP
	no_em_group[2]=NO_EM_GROUP;
#else
	no_em_group[2]=0;
#endif
//CBADDED start
#ifdef CB_INSTALLED
	no_cb[2]=NO_CB;
#else
	no_cb[2]=0;
#endif
//CBADDED end
#endif	// (NO_EXTENSION_C != 0)

#endif	// LIM
}
//DYNAMIC : commented
//#endif // NEW_DEP
