bcc -2 -w -C -ecpfa.exe -Ic:\ec\work\spec -DCARD_NO=0 -mh cpf.c pdul.c  >p
@if not errorlevel 1 goto end
del cpf.exe
@echo 
:end
