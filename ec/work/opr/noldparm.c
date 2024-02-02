

#include "\ec\work\spec\general.h"

#ifndef NO_CAS3
#define NO_CAS3         0
#endif

#ifndef NO_CAS1
#define NO_CAS1         0
#endif


int no_load_parm[3];
valid_parm_file_size[3];

#ifdef PARAMETER_DOWN_UP_LOAD
void main(void){

        valid_parm_file_size[0]=VALID_PARM_FILE_SIZE;
        no_load_parm[0]=NO_LOAD_PARM;

#ifdef LIM

#undef VALID_PARM_FILE_SIZE
#undef NO_LOAD_PARM

/* following undefs only for suppressing warning messages */
#undef NO_EXTENSION
#undef NO_LINE_CARD_IN_SYS
#undef LINE_CARD_START_SLOT
#undef SELF_CENTER
#undef FIRST_DIRECTORY_NUM
#undef TON
#undef OPRT_ADDR
#undef SELF_CENTER_GROUP_NUMBER
#undef SELF_CENTER_GROUP_NUMBER_D2D1
#undef NO_ANALOG_CARD
#undef NO_CO
#undef NO_ANALOG_TRUNK
#undef EM_INSTALLED
#undef NO_EM
//#undef EM_GROUP_PROGRAM
#undef NO_EM_GROUP
#undef DTMF_CARD_START_SLOT
#undef NO_LOAD_PARM1
#undef GLOBAL_NIGHT_EXT

#undef NO_DIGITAL_TRUNK
#undef NO_CAS3_DIGITAL_TRUNK
#undef NO_CAS3_GROUP
#undef NO_CAS3_IC_CH
#undef NO_CAS3_OG_CH
#undef LIM_OGT_GROUP
#undef ANALOG_CARD_START_SLOT
#undef NO_CAS3
#undef NO_DT_GROUP
#undef CAS1_INSTALLED
#undef NO_CAS1
#undef NO_CAS1_DIGITAL_TRUNK
#undef NO_CAS1_GROUP
#undef NO_CAS1_IC_CH
#undef NO_CAS1_OG_CH

#undef CB_INSTALLED
#undef NO_CB


#include "\ec\work\spec\generalb.h"

#ifndef NO_EM_GROUP
#define NO_EM_GROUP     0
#endif

#ifndef NO_EM
#define NO_EM           0
#endif

#ifndef NO_CAS3
#define NO_CAS3         0
#endif

#ifndef NO_CAS1
#define NO_CAS1         0
#endif

        valid_parm_file_size[1]=VALID_PARM_FILE_SIZE;
        no_load_parm[1]=NO_LOAD_PARM;

#if(NO_EXTENSION_C != 0)

#undef VALID_PARM_FILE_SIZE
#undef NO_LOAD_PARM

/* following undefs only for suppressing warning messages */
#undef NO_EXTENSION
#undef NO_LINE_CARD_IN_SYS
#undef LINE_CARD_START_SLOT
#undef SELF_CENTER
#undef FIRST_DIRECTORY_NUM
#undef TON
#undef OPRT_ADDR
#undef SELF_CENTER_GROUP_NUMBER
#undef SELF_CENTER_GROUP_NUMBER_D2D1
#undef NO_ANALOG_CARD
#undef NO_CO
#undef NO_ANALOG_TRUNK
#undef EM_INSTALLED
#undef NO_EM
//#undef EM_GROUP_PROGRAM
#undef NO_EM_GROUP
#undef DTMF_CARD_START_SLOT
#undef NO_LOAD_PARM1
#undef GLOBAL_NIGHT_EXT

#undef NO_DIGITAL_TRUNK
#undef NO_CAS3_DIGITAL_TRUNK
#undef NO_CAS3_GROUP
#undef NO_CAS3_IC_CH
#undef NO_CAS3_OG_CH
#undef LIM_OGT_GROUP
#undef ANALOG_CARD_START_SLOT
#undef NO_CAS3
#undef NO_DT_GROUP
#undef CAS1_INSTALLED
#undef NO_CAS1
#undef NO_CAS1_DIGITAL_TRUNK
#undef NO_CAS1_GROUP
#undef NO_CAS1_IC_CH
#undef NO_CAS1_OG_CH

#undef CB_INSTALLED
#undef NO_CB


#include "\ec\work\spec\generalc.h"

#ifndef NO_EM_GROUP
#define NO_EM_GROUP     0
#endif

#ifndef NO_EM
#define NO_EM           0
#endif

#ifndef NO_CAS3
#define NO_CAS3         0
#endif

#ifndef NO_CAS1
#define NO_CAS1         0
#endif

        valid_parm_file_size[2]=VALID_PARM_FILE_SIZE;
        no_load_parm[2]=NO_LOAD_PARM;

#endif  //# (NO_EXTENSION_C != 0)

#endif  // LIM
        printf("\n%d %d %d",no_load_parm[0],no_load_parm[1],no_load_parm[2]);
}
#endif  // PARAMETER_DOWN_UP_LOAD

