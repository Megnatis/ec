del p
del p1
del opr.exe
djecho -o compiler.h #define COMPILER GCC
rem djecho -a compiler.h #define OS WINDOWS
rem msmake oprgc.mk
djmake -f goprgc.mk
rem copy /B ..\sys\pmodstub.exe+opr.out opr.exe
djredir -oa c:\ec\work\sys\pall echo building opr for
djredir -oa c:\ec\work\sys\pall type c:\ec\work\spec\general.h
djredir -oa c:\ec\work\sys\pall type c:\ec\work\spec\opr.h
djredir -oa c:\ec\work\sys\pall type p
djredir -oa c:\ec\work\sys\pall type p1
