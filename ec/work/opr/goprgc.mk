
DOS=1
KURT=0
WINDOWS=0

ifeq ($(WINDOWS),1)
CC = gccw32
CFLAGS1 = -O3 -c -Wall -I /ec/work/spec
CFLAGS2 = -O3 -c -Wall -I /ec/work/spec -I. -D FILE_IS_COMPILED_FOR_OPR
endif

ifeq ($(DOS),1)
CC = gcc
CFLAGS1 = -O3 -c -Wall -Wno-unknown-pragmas -Wno-unused-label -I /ec/work/spec
CFLAGS2 = -O3 -c -Wall -Wno-unknown-pragmas -Wno-unused-label -I /ec/work/spec -I. -D FILE_IS_COMPILED_FOR_OPR
endif

ifeq ($(KURT),1)
TOPLEVEL	= /usr/src/kurt-2.4.18
BIN_DIR 	= $(TOPLEVEL)/bin
LIB_DIR 	= $(TOPLEVEL)/lib
INCLUDE_DIR	= $(TOPLEVEL)/include
KURTLINUX	= /usr/src/linux

CC = gcc
#CFLAGS1 = -O3 -g -c -Wall -Wno-unknown-pragmas -Wno-unused-label -I /ec/work/spec				   -D_LINUX_TIME_H -D__i386_UACCESS_H -I$(INCLUDE_DIR) -I$(KURTLINUX)/include/
#CFLAGS2 = -O3 -g -c -Wall -Wno-unknown-pragmas -Wno-unused-label -I /ec/work/spec -I. -D FILE_IS_COMPILED_FOR_OPR -D_LINUX_TIME_H -D__i386_UACCESS_H -I$(INCLUDE_DIR) -I$(KURTLINUX)/include/
CFLAGS1 =  -O3 -g -c -Wall -Wno-unknown-pragmas 		  -I /ec/work/spec				   -D_LINUX_TIME_H -D__i386_UACCESS_H -I$(INCLUDE_DIR) -I$(KURTLINUX)/include/
CFLAGS2 =  -O3 -g -c -Wall -Wno-unknown-pragmas 		  -I /ec/work/spec -I. -D FILE_IS_COMPILED_FOR_OPR -D_LINUX_TIME_H -D__i386_UACCESS_H -I$(INCLUDE_DIR) -I$(KURTLINUX)/include/
endif

OBJ-DIR = obj1
SYS-DIR = /ec/work/sys
OPR-DIR = /ec/work/opr
TCP-DIR = /ec/work/sys/tcpip

OBJ-OPR = \
      $(OBJ-DIR)/omain.o      $(OBJ-DIR)/oprtn.o      $(OBJ-DIR)/oprmenu.o  \
      $(OBJ-DIR)/pdul.o       $(OBJ-DIR)/db.o	      $(OBJ-DIR)/onet.o     \
      $(OBJ-DIR)/wf.o	      $(OBJ-DIR)/params.o     $(OBJ-DIR)/cpf.o	    \
      $(OBJ-DIR)/depm.o       $(OBJ-DIR)/deprtn.o     $(OBJ-DIR)/ep.o	    \
      $(OBJ-DIR)/filevar1.o   $(OBJ-DIR)/filevar2.o   $(OBJ-DIR)/filevar3.o \
      $(OBJ-DIR)/filevar5.o   $(OBJ-DIR)/dep_parm.o \
      $(OBJ-DIR)/mnul_tst.o   $(OBJ-DIR)/gcc_func.o   $(OBJ-DIR)/dpi_file.o \
      $(OBJ-DIR)/btch_ep.o    $(OBJ-DIR)/lincurse.o

OBJ-SYS = \
      $(OBJ-DIR)/lapb.o       $(OBJ-DIR)/sml.o	      $(OBJ-DIR)/smllyr1.o  \
      $(OBJ-DIR)/glb_rtns.o   $(OBJ-DIR)/lintcp.o \
      $(OBJ-DIR)/malloc.o

OBJ-TCP = \
      $(OBJ-DIR)/arp.o	  \
      $(OBJ-DIR)/ip.o	  \
      $(OBJ-DIR)/iprx.o   \
      $(OBJ-DIR)/iptx.o   \
      $(OBJ-DIR)/tcp1.o   \
      $(OBJ-DIR)/tcp2.o   \
      $(OBJ-DIR)/tcprx.o  \
      $(OBJ-DIR)/tcptx.o  \
      $(OBJ-DIR)/pktdrvr.o \
      $(OBJ-DIR)/misc.o   \
      $(OBJ-DIR)/udp.o

OBJ = $(OBJ-OPR) $(OBJ-SYS) $(OBJ-TCP)

ifeq ($(WINDOWS),1)
define run-compile-1
	djredir -oa p -ea p $(CC)  $(CFLAGS1) $(<) -o $(OBJ-DIR)/$(@F)
endef
define run-compile-2
	djredir -oa p -ea p $(CC)  $(CFLAGS2) $(<) -o $(OBJ-DIR)/$(@F)
endef
endif
ifeq ($(DOS),1)
define run-compile-1
	djredir -oa p -ea p $(CC)  $(CFLAGS1) $(<) -o $(OBJ-DIR)/$(@F)
endef
define run-compile-2
	djredir -oa p -ea p $(CC)  $(CFLAGS2) $(<) -o $(OBJ-DIR)/$(@F)
endef
endif
ifeq ($(KURT),1)
define run-compile-1
	$(CC)  $(CFLAGS1) $< -o $(OBJ-DIR)/$(@F) >>pp 2>&1
endef
define run-compile-2
	$(CC)  $(CFLAGS2) $< -o $(OBJ-DIR)/$(@F) >>pp 2>&1
endef
endif
ifeq ($(WINDOWS),1)
$(OBJ-DIR)/opr.exe : $(OBJ)
	djredir -o $(OBJ-DIR)/opr.map -e p1 $(CC) $(OBJ)    -Wl,-M -ldbg -o $(OBJ-DIR)/opr.exe
endif
ifeq ($(DOS),1)
$(OBJ-DIR)/opr.exe : $(OBJ)
	djredir -o $(OBJ-DIR)/opr.map -e p1 $(CC) $(OBJ)    -Wl,-M -ldbg -o $(OBJ-DIR)/opr.exe
endif
ifeq ($(KURT),1)
$(OBJ-DIR)/opr.exe : $(OBJ)
#	$(CC) $(OBJ) -Wl,-M -L$(LIB_DIR) -L/usr/lib	      -lpthread -lkurt -ldsk -o $(OBJ-DIR)/opr.exe &>p1 >&$(OBJ-DIR)/opr.map
	$(CC) $(OBJ) -Wl,-M -L$(LIB_DIR) -L/usr/lib -lncurses -lpthread -lkurt	     -o $(OBJ-DIR)/opr.exe &>p1 >&$(OBJ-DIR)/opr.map
endif
$(OBJ-OPR) : $(OBJ-DIR)/%.o: %.c depv.h omtfp.h opr_net.h wg.h wv.h ov.h ovpc.h depmtfp.h $(SYS-DIR)/lapb.var $(SYS-DIR)/sml.var $(SYS-DIR)/emmacro.h
	$(run-compile-1)
$(OBJ-SYS) : $(OBJ-DIR)/%.o: $(SYS-DIR)/%.c omtfp.h opr_net.h
	$(run-compile-2)
$(OBJ-TCP) : $(OBJ-DIR)/%.o: $(TCP-DIR)/%.c omtfp.h opr_net.h
	$(run-compile-2)

