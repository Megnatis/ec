rm -f pp
rm -f p1
rm -f opr.exe
rm -f compiler.h
echo "#define COMPILER GCC" >> compiler.h 2>&1
echo "#define OS LINUX_COMPATIBLE" >> compiler.h 2>&1
echo "#define LINUX_OS KURT" >> compiler.h 2>&1
make -f goprgc.mk --ignore-errors
