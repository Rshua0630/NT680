@echo off
set CurrentDIR=%CD%
pushd C:\Program Files\teraterm
ttpmacro.exe /V %CurrentDIR%\IceMode.ttl
popd
set OLD_PATH=%PATH%
PATH=C:\NMake;%PATH%
start arm-none-eabi-insight --command=init_IceMode.gdb --symbols=%CurrentDIR%/DemoKit_Data/Release/DemoKit.axf
PATH=%OLD_PATH%
set CurrentDIR=
set OLD_PATH=