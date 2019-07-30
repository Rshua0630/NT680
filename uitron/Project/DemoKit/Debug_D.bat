@echo off
set CurrentDIR=%CD%
pushd C:\Program Files\teraterm
ttpmacro.exe /V %CurrentDIR%\LoadCode.ttl %CurrentDIR:\=/% Debug Halt
popd
set OLD_PATH=%PATH%
PATH=C:\NMake;%PATH%
PATH=%OLD_PATH%
set CurrentDIR=
set OLD_PATH=