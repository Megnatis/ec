
deltree /Y \t
md\t

cd\ec\work\opr

if %2==2 goto twoop
if not exist c:\ec\work\spec\%1a.h goto error
if not exist c:\ec\work\spec\%1b.h goto error

echo #include "c:\ec\work\spec\%1a.h" >\ec\work\spec\general.h
echo #include "c:\ec\work\spec\%1b.h" >\ec\work\spec\generalb.h

call do
copy opr.exe \t
del opr.exe

cd dep
call do
copy dep.exe \t
del dep.exe

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

del z1
del z2
del obj1\*.obj

call c2r
copy z1 \t\z1a
copy z2 \t\z2a
del z1
del z2



cd\ec\work\opr

echo #include "c:\ec\work\spec\%1b.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s \t\sb
del s

del z1
del z2
del obj1\*.obj

call c2r
copy z1 \t\z1b
copy z2 \t\z2b
del z1
del z2



cd\ec\work\opr

echo #include "c:\ec\work\spec\%1a.h" >\ec\work\spec\general.h
goto end

:twoop
if not exist c:\ec\work\spec\%1a0.h goto error
if not exist c:\ec\work\spec\%1b0.h goto error
if not exist c:\ec\work\spec\%1a1.h goto error
if not exist c:\ec\work\spec\%1b1.h goto error

echo #include "c:\ec\work\spec\%1a0.h" >\ec\work\spec\general.h
echo #include "c:\ec\work\spec\%1b0.h" >\ec\work\spec\generalb.h

call do
copy opr.exe \t\opr1.exe
del opr.exe


cd dep
call do
copy dep.exe \t
del dep.exe

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

del z1
del z2
del obj1\*.obj

call c2r
copy z1 \t\z1a
copy z2 \t\z2a
del z1
del z2



cd\ec\work\opr

echo #include "c:\ec\work\spec\%1b0.h" >\ec\work\spec\general.h

cd\ec\work\sys

del s
del obj1\*.obj

call c2r188
copy s  \t\sb
del s

del z1
del z2
del obj1\*.obj

call c2r
copy z1 \t\z1b
copy z2 \t\z2b
del z1
del z2



cd\ec\work\opr

echo #include "c:\ec\work\spec\%1a1.h" >\ec\work\spec\general.h
echo #include "c:\ec\work\spec\%1b1.h" >\ec\work\spec\generalb.h

call do
copy opr.exe \t\opr2.exe
del opr.exe


echo #include "c:\ec\work\spec\%1a0.h" >\ec\work\spec\general.h
echo #include "c:\ec\work\spec\%1b0.h" >\ec\work\spec\generalb.h

:end

cd\t
pkzip %1
zip2exe %1
copy %1.exe \ /y

goto finish

:error
echo %1a.h or %1a0.h or %1a1.h does not exist in c:\ec\work\spec

:finish
cd\ec\work\opr
