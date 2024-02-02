
//NEW_DEP
//#include "..\omtfp.h"
#include "omtfp.h"
#ifdef NEW_DEP
//DOOD start:some changes
#if (OS == DOS)
#include <conio.h>
#include <process.h>
#endif
#include <string.h>
#include <stdlib.h>
//DOOD end
#include "wg.h"


#include "depmtfp.h"
long alarm_line_ptr[MAX_FILE_LINE];
#endif
