#c:\asl\asl.exe /Fo=.\alibv2.aml  alibv2.asl
rmdir /S /Q BUILD
#set AGESA_ROOT=C:\Sandbox\Tasks\AGESA_ALIBV2
set path=c:\ASL;%path%
nmake -f AlibKB.mak
