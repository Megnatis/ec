
//NEW_DEP
//#include "..\omtfp.h"
#include "omtfp.h"
#ifdef NEW_DEP
//DOOD start:
#if (OS == DOS)
#include <conio.h>
#include <process.h>
#include <string.h>
#include <stdlib.h>
#endif
//DOOD end
#include "wg.h"


#include "depmtfp.h"
long malicious_line_ptr[MAX_FILE_LINE];
#endif
