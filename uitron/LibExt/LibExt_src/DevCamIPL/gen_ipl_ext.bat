@echo off
@cls
echo *****************************************
echo ****** gen ipl ext template *******
echo *****************************************
echo.

:INPUT_SRC_FOLDER_NAME
echo please input template folder name^(ex: ipl_sample_ff^):
set /p src_folder_name=

if "%src_folder_name%" == "" (
  echo ERROR! template folder name is null.
  goto INPUT_SRC_FOLDER_NAME
)

if not exist %src_folder_name% (
  echo ERROR! %src_folder_name% is not exist.
  goto INPUT_SRC_FOLDER_NAME
)

:INPUT_FIND_STR
echo please input find string^(ex: sample^):
set /p find_str=

if "%find_str%" == "" (
  echo ERROR! str is null.
  goto INPUT_FIND_STR
)

:INPUT_REPLACE_STR
echo please input replace string^(ex: imx123lqt^):
set /p replace_str=

if "%replace_str%" == "" (
  echo ERROR! str is null.
  goto INPUT_REPLACE_STR
)

echo process.........start
::---------------------------------------------
::							copy folder
::---------------------------------------------
call set dst_folder_name=%%src_folder_name:%find_str%=%replace_str%%%

if "%src_folder_name%" == "%dst_folder_name%" (
	echo ERROR! can not find %find_str%
	goto END
)

echo copy %src_folder_name% to %dst_folder_name% folder:

if not exist %dst_folder_name% (

  ::--------- copy folder ---------
  xcopy %src_folder_name%\*.* %dst_folder_name%\ /D > NUL

  if "%errorlevel%" EQU "0" (
    echo created successfully
  ) else (
    echo ERROR! while creating
    goto END
  )
) else (
  echo ERROR! already exists
  goto END
)

::---------------------------------------------
::				file rename & scan content
::---------------------------------------------
echo copy and scan file:
setlocal enabledelayedexpansion
for %%i in (%dst_folder_name%\*.*) do (

  set "src_file_name=%%~nxi"
  set "dst_file_name=!src_file_name:%find_str%=%replace_str%!"

  echo !src_file_name! ^-^> !dst_file_name!

  ::--------- file rename ---------
  ren %dst_folder_name%\!src_file_name! !dst_file_name!

  ::--------- replace content(lower & upper case) ---------
	call:FindAndReplaceStr_UL %find_str% %replace_str% %dst_folder_name%/!dst_file_name!

)
endlocal

::---------------------------------------------
::          add prototype to ipl_obj_drv.h
::---------------------------------------------
setlocal enabledelayedexpansion
for /F "tokens=1,2* delims== " %%i in (%dst_folder_name%\Makefile) do (
	if "%%i" == "PRJ_NAME" (
		set "prj_name=%%j"
		set "ipl_drv_func=%%j_EXT_LIB_TAB"
		find /c /i "!ipl_drv_func!" ".\..\..\..\Include\DrvExt\IPLOBJ\ipl_obj_drv.h" > NUL
		if !ERRORLEVEL! EQU 0 (
			echo ERROR declare !ipl_drv_func!^(void^) already exists
			endlocal
			goto END
		)
		set "ipl_drv_add_line=extern IPL_EXT_LIB_TAB *%%j_EXT_LIB_TAB(void);"
		call:FindAndBeforeAddStr "#endif" "!ipl_drv_add_line!" "./../../../Include/DrvExt/IPLOBJ/ipl_obj_drv.h"
	  endlocal
		goto END
	)
)
echo ERROR get project name fail
endlocal

:END
echo process.........done
pause
exit

::---------------------------------------------
::							sub function
::---------------------------------------------
:LCase
:UCase
:: Converts to upper/lower case variable contents
:: Syntax: CALL :UCase _VAR1 _VAR2
:: Syntax: CALL :LCase _VAR1 _VAR2
:: _VAR1 = Variable NAME whose VALUE is to be converted to upper/lower case
:: _VAR2 = NAME of variable to hold the converted value
:: Note: Use variable NAMES in the CALL, not values (pass "by reference")

SET _UCase=A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
SET _LCase=a b c d e f g h i j k l m n o p q r s t u v w x y z
SET _Lib_UCase_Tmp=!%1!
IF /I "%0"==":UCase" SET _Abet=%_UCase%
IF /I "%0"==":LCase" SET _Abet=%_LCase%
FOR %%Z IN (%_Abet%) DO SET _Lib_UCase_Tmp=!_Lib_UCase_Tmp:%%Z=%%Z!
SET %2=%_Lib_UCase_Tmp%
GOTO:EOF

:FindAndReplaceStr_UL
:: %~1 find string
:: %~2 replace string
:: %~3 scan file name

setlocal
set "sed=c:\NMake\sed"
set "find_tmp=%1"
set "replace_tmp=%2"
set find_str_u=
set find_str_l=
set replace_str_u=
set replace_str_l=
call:UCase find_tmp find_str_u
call:LCase find_tmp find_str_l
call:UCase replace_tmp replace_str_u
call:LCase replace_tmp replace_str_l

%sed% -i -e 's/%find_str_u%/%replace_str_u%/g' %~3
%sed% -i -e 's/%find_str_l%/%replace_str_l%/g' %~3
endlocal
goto:eof

:FindAndBeforeAddStr
:: %~1 find string
:: %~2 add string
:: %~3 scan file name

setlocal 
set "sed=c:\NMake\sed"
set "find_tmp=%~1"
set "replace_tmp=%~2"
set "file_tmp=%~3"
%sed% -i -e '/%find_tmp%/i %replace_tmp%' %file_tmp%
endlocal
goto:eof