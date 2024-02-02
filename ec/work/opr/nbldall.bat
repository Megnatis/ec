
echo #define TON 0 >\ec\work\spec\opr.h
del /Q \t
rd \t
rem deltree /Y \t
md\t

cd\ec\work\opr

if %2==2 goto twoop
if %2==3 goto threeop
if not exist c:\ec\work\spec\%1a.h goto error
if not exist c:\ec\work\spec\%1b.h goto error

echo #include "/ec/work/spec/%1a.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c.h goto cont1
echo #include "/ec/work/spec/%1c.h" >\ec\work\spec\generalc.h
:cont1

del obj1\*.o
call dogc
copy obj1\opr.exe \t
del obj1\opr.exe

rem cd dep
rem call do
rem copy dep.exe \t
rem del dep.exe

rem cd ..\oa
rem call do
rem copy oa.exe \t
rem del oa.exe

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s \t\sa
del s

cd\ec\work\opr

echo #include "/ec/work/spec/%1b.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s \t\sb
del s

if not exist c:\ec\work\spec\%1c.h goto cont2

echo #include "/ec/work/spec/%1c.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s \t\sc
del s
:cont2
cd\ec\work\opr

echo #include "/ec/work/spec/%1a.h" >\ec\work\spec\general.h
goto end

:twoop
if not exist c:\ec\work\spec\%1a0.h goto error
if not exist c:\ec\work\spec\%1b0.h goto error
rem if not exist c:\ec\work\spec\%1a1.h goto error
rem if not exist c:\ec\work\spec\%1b1.h goto error

echo #include "/ec/work/spec/%1a0.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b0.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c0.h goto cont3
echo #include "/ec/work/spec/%1c0.h" >\ec\work\spec\generalc.h
:cont3

del obj1\*.o
call dogc
copy obj1\opr.exe \t\opr1.exe
del obj1\opr.exe

rem cd dep
rem call do
rem copy dep.exe \t
rem del dep.exe

rem cd ..\oa
rem call do
rem copy oa.exe \t
rem del oa.exe

if not exist c:\ec\work\spec\%1a1.h goto creat
if not exist c:\ec\work\spec\%1b1.h goto creat

echo #include "/ec/work/spec/%1a1.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b1.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c1.h goto cont5
echo #include "/ec/work/spec/%1c1.h" >\ec\work\spec\generalc.h
goto cont5
:creat
echo #define TON 1 >\ec\work\spec\opr.h
echo #include "/ec/work/spec/%1a0.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b0.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c0.h goto cont5
echo #include "/ec/work/spec/%1c0.h" >\ec\work\spec\generalc.h
:cont5

del obj1\*.o
call dogc
copy obj1\opr.exe \t\opr2.exe
del obj1\opr.exe

echo #include "/ec/work/spec/%1a0.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s \t\sa
del s

cd\ec\work\opr

echo #include "/ec/work/spec/%1b0.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s  \t\sb
del s

cd\ec\work\opr

if not exist c:\ec\work\spec\%1c0.h goto cont4
echo #include "/ec/work/spec/%1c0.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s  \t\sc
del s

cd\ec\work\opr

:cont4

goto end

:threeop
if not exist c:\ec\work\spec\%1a0.h goto error
if not exist c:\ec\work\spec\%1b0.h goto error

echo #include "/ec/work/spec/%1a0.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b0.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c0.h goto cont7
echo #include "/ec/work/spec/%1c0.h" >\ec\work\spec\generalc.h
:cont7

del obj1\*.o
call dogc
copy obj1\opr.exe \t\opr1.exe
del obj1\opr.exe

rem cd dep
rem call do
rem copy dep.exe \t
rem del dep.exe

rem cd ..\oa
rem call do
rem copy oa.exe \t
rem del oa.exe

if not exist c:\ec\work\spec\%1a1.h goto creat2
if not exist c:\ec\work\spec\%1b1.h goto creat2

echo #include "/ec/work/spec/%1a1.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b1.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c1.h goto cont9
echo #include "/ec/work/spec/%1c1.h" >\ec\work\spec\generalc.h
goto cont9
:creat2
echo #define TON 1 >\ec\work\spec\opr.h
echo #include "/ec/work/spec/%1a0.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b0.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c0.h goto cont9
echo #include "/ec/work/spec/%1c0.h" >\ec\work\spec\generalc.h
:cont9

del obj1\*.o
call dogc
copy obj1\opr.exe \t\opr2.exe
del obj1\opr.exe


if not exist c:\ec\work\spec\%1a2.h goto creat3
if not exist c:\ec\work\spec\%1b2.h goto creat3

echo #include "/ec/work/spec/%1a2.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b2.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c2.h goto cont10
echo #include "/ec/work/spec/%1c2.h" >\ec\work\spec\generalc.h
goto cont10
:creat3
echo #define TON 2 >\ec\work\spec\opr.h
echo #include "/ec/work/spec/%1a0.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b0.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c0.h goto cont11
echo #include "/ec/work/spec/%1c0.h" >\ec\work\spec\generalc.h
:cont11

del obj1\*.o
call dogc
copy obj1\opr.exe \t\opr3.exe
del obj1\opr.exe

echo #include "/ec/work/spec/%1a0.h" >\ec\work\spec\general.h
echo #include "/ec/work/spec/%1b0.h" >\ec\work\spec\generalb.h
if not exist c:\ec\work\spec\%1c0.h goto cont6
echo #include "/ec/work/spec/%1c0.h" >\ec\work\spec\generalc.h
:cont6

echo #include "/ec/work/spec/%1a0.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s \t\sa
del s

cd\ec\work\opr

echo #include "/ec/work/spec/%1b0.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s  \t\sb
del s

cd\ec\work\opr

if not exist c:\ec\work\spec\%1c0.h goto cont8
echo #include "/ec/work/spec/%1c0.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s  \t\sc
del s

cd\ec\work\opr

:cont8

:end

cd\t
pkzip %1
rem zip2exe %1
rem copy %1.exe \ /y
copy %1.zip \ /y

goto finish

:error
echo %1a.h or %1a0.h or %1a1.h does not exist in c:\ec\work\spec

:finish
echo #define TON 0 >\ec\work\spec\opr.h
cd\ec\work\opr
