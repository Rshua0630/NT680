@echo off
set CurrentDIR=%CD%
pushd C:\Program Files\teraterm
start ttpmacro.exe /V %CurrentDIR%\LoadCode.ttl %CurrentDIR:\=/% Release Resume
set CurrentDIR=
popd