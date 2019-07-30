; /*++
;
; Copyright (c) 2005  Novatek Microelectronics Corporation
;
; Module Name:
;     StdCMac.h
;
; Abstract:
;     Standard ARM ASM Macros for shared header file of ARM ASM and C
;     You should include this file first and then include the shared header file
;     For more information, please refer to http://www.arm.com/support/faqdev/1208.html
;
; Environment:
;     For nt96610
;
; Notes:
;     Copyright (c) 2005 Novatek Microelectronics Corporation.  All Rights Reserved.
;
; Revision History:
;     ??/??/??: Created by ARM
;     03/17/05: Modify by Chris Hsu for NT96610 uITRON project
; --*/

;
Global vars for arg parsing

GBLS    _arg0
GBLS    _arg1

;
_spaces_remove
;
remove start and end spaces from global variable wstring

MACRO
_spaces_remove $wstring
WHILE(("*" : CC: $wstring) : RIGHT: 1 = " ")
$wstring    SETS($wstring : LEFT: (: LEN: $wstring - 1))
WEND
WHILE(($wstring : CC: "*") : LEFT: 1 = " ")
$wstring    SETS($wstring : RIGHT: (: LEN: $wstring - 1))
WEND
MEND

;
_lbracket_remove
;
Attempt to remove a single left bracket - error if not there

MACRO
_lbracket_remove $s
ASSERT  $s:
LEFT: 1 = "("
		  $s  SETS    $s:
		  RIGHT:
		  (: LEN: $s - 1)
			  _spaces_remove $s
			  MEND

			  ;
_rbracket_remove
;
Attempt to remove a single right bracket - error if not there
;
then removes excess spaces

MACRO
_rbracket_remove $s
ASSERT  $s:
RIGHT: 1 = ")"
		   $s  SETS    $s:
		   LEFT:
		   (: LEN: $s - 1)
		   _spaces_remove $s
		   MEND

		   ;
_comment_remove
;
Remove any comment from line end and then strip any spaces

MACRO
_comment_remove $s
_spaces_remove $s
IF(("**": CC: $s): RIGHT: 2) = "*/"
							   WHILE($s: RIGHT: 2) <> "/*"
							   $s      SETS $s:
							   LEFT:
							   (: LEN: $s - 1)
							   WEND
							   $s  SETS $s:
							   LEFT:
							   (: LEN: $s - 2)
							   _spaces_remove $s
							   ENDIF
							   MEND

							   ;
_arg_remove
;
Pull an argument from the front of a spaces stripped string

MACRO
_arg_remove $s, $arg
LCLA    _arglen
LCLL    _ok
_arglen SETA    0
_ok SETL    {TRUE}
WHILE    _ok
IF _arglen >= :
LEN:
$s
_ok     SETL {FALSE}    ;
break if used up input string
ELSE
$arg        SETS($s: LEFT: (_arglen + 1))
{
:
	RIGHT: 1    ;
}
next character
IF $arg = " "
		  _ok       SETL {FALSE}
		  ELSE
		  _arglen       SETA _arglen + 1
		  ENDIF
		  ENDIF
		  WEND
		  $arg    SETS $s:
		  LEFT:
		  _arglen
		  $s  SETS $s:
		  RIGHT:
		  (: LEN: $s - _arglen)
		  _spaces_remove $s
		  MEND

		  ;
ifndef
;
Purpose:
Allow #ifndef for common C headers (Just for guarded C header file)
		;
Syntax :
#<space/tab>ifndef<spaces><symbol></*comment*/>

MACRO
ifndef $a
MEND

;
define
;
Purpose:
Allow #defines for common C/Assembler headers
;
Syntax :
#<space/tab>define<spaces><symbol><spaces><value></*comment*/>

MACRO
$la define $a
_arg0   SETS    "$a"
ASSERT  "$la" = "#"    ;
syntax:
# define fred 1
_comment_remove _arg0
_arg_remove _arg0, _arg1
IF "$_arg0"<>""
$_arg1  EQU    $_arg0
ENDIF
MEND


;
COMMENT
;
Purpose:
Allow comments in common C / Assembler headers
;
Syntax :
COMMENT < anything you like! >

MACRO
COMMENT $a, $b, $c, $d, $e, $f, $g, $h
MEND

;
local labels use label$l to get a local label and LOCAL to start a new
;
area

GBLA LocalCount
GBLS l
LocalCount SETA 1
l    SETS "x$LocalCount"

;
increment local variable number

MACRO
LOCAL
LocalCount SETA LocalCount + 1
l   SETS "x$LocalCount"
MEND

END
