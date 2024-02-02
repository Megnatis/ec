
#define BCC	1
#define GCC	2

#define DOS			1
#define ECOS			2
#define LINUX_COMPATIBLE	3

#define KURT			    1
#define RTEMS			    2
#define RTLINUX 		    3

#include	"compiler.h"

#ifndef OS
#define OS  DOS
#endif

