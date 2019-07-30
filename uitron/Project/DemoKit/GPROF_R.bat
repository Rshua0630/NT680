@echo off
set CurrentPRJ=DemoKit
set CurrentDIR=%CD%/%CurrentPRJ%_Data/Release
set OLD_PATH=%PATH%
PATH=C:\NMake;%CD%\..\..\Tools\bin;%PATH%
echo Processing file "gmon.out"
IF NOT EXIST %CurrentDIR%/gmon.out goto :ERROR1
IF NOT EXIST %CurrentDIR%/%CurrentPRJ%_ldmap.txt goto :ERROR2
IF EXIST %CurrentDIR%/gprof.txt del %CurrentDIR%/gprof.txt /q
IF EXIST %CurrentDIR%/gprof.txt.out del %CurrentDIR%/gprof.txt.out /q
IF EXIST %CurrentDIR%/%CurrentPRJ%_ldmap.txt.out del %CurrentDIR%/%CurrentPRJ%_ldmap.txt.out /q
echo Step-1: convert by gprof...
mips-sde-elf-gprof --no-static --no-graph --demangle=auto %CurrentDIR%/%CurrentPRJ%.axf %CurrentDIR%/gmon.out > %CurrentDIR%/gprof.txt
echo Step-2: convert by symutil...
symutil.exe %CurrentDIR%/%CurrentPRJ%_ldmap.txt %CurrentDIR%/gprof.txt
echo Ok! Please check result in file "gprof.txt.out"
pause
goto :END
:ERROR1
echo Can't find file "gmon.out"!
pause
goto :END
:ERROR2
echo Can't find file "ldmap.txt"!
pause
goto :END
:END
PATH=%OLD_PATH%
set OLD_PATH=
set CurrentPRJ=
set CurrentDIR=
