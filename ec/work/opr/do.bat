echo #define COMPILER BCC >compiler.h
del *.obj
del opr.exe
bcc @s.bc  >p
@if not errorlevel 1 goto end
@echo 
:end
