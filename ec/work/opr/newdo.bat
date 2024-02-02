bcc -c -2 -w -C -eopr.exe -Ic:\ec\work\spec -mh omain.c oprtn.c pdul.c db.c onet.c wf.c params.c cpf.c >p
bcc -S -c -2 -w -C -Ic:\ec\work\spec -mh oprmenu.c >p
tasm oprmenu; >p
tlink c:\bc\lib\c0h+omain+oprtn+oprmenu+pdul+db+onet+wf+params+cpf,opr.exe,opr.map,ch >p
@if not errorlevel 1 goto end
del opr.exe
@echo 
:end
