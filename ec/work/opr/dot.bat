bcc -c -2 -w -C -Ic:\ec\work\spec -mh t.c
tlink c:\bc\lib\c0h+t,t.exe,,ch >p
@if not errorlevel 1 goto end
del t.exe
@echo 
:end
