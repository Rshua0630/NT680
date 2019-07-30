/**
 * Novatek lens UART command.
 * 
 * Lens UART command implementation.
 * 
 * @file LensUartCmd.c
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#include <stdio.h>
#include "Type.h"
#include "SxCmd.h"
#include "Lens.h"
#include "LensAPI.h"


#define __MODULE__          LENS_UART_CMD
#define __DBGLVL__          2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    // *=All, [mark]=CustomClass
#include "DebugModule.h"


// lens command
BOOL Cmd_lens_zoom_init(CHAR* strCmd);
BOOL Cmd_lens_focus_init(CHAR* strCmd);
BOOL Cmd_lens_aperture_init(CHAR* strCmd);
BOOL Cmd_lens_get_zpos(CHAR* strCmd);
BOOL Cmd_lens_set_zpos(CHAR* strCmd);
BOOL Cmd_lens_get_fpos(CHAR* strCmd);
BOOL Cmd_lens_set_fpos(CHAR* strCmd);
BOOL Cmd_lens_get_apos(CHAR* strCmd);
BOOL Cmd_lens_set_apos(CHAR* strCmd);
BOOL Cmd_lens_set_sste(CHAR* strCmd);
BOOL Cmd_lens_set_iste(CHAR* strCmd);
BOOL Cmd_lens_retract(CHAR* strCmd);


SXCMD_BEGIN(lens, "lens command")
SXCMD_ITEM("lzi",   Cmd_lens_zoom_init,     "zoom init")
SXCMD_ITEM("lfi",   Cmd_lens_focus_init,    "focus init")
SXCMD_ITEM("lai",   Cmd_lens_aperture_init, "aperture init")
SXCMD_ITEM("gzp",   Cmd_lens_get_zpos,      "get zoom position")
SXCMD_ITEM("szp %", Cmd_lens_set_zpos,      "set zoom position")
SXCMD_ITEM("gfp",   Cmd_lens_get_fpos,      "get focus position")
SXCMD_ITEM("sfp %", Cmd_lens_set_fpos,      "set focus position")
SXCMD_ITEM("gap",   Cmd_lens_get_apos,      "get aperture position")
SXCMD_ITEM("sap %", Cmd_lens_set_apos,      "set aperture position")
SXCMD_ITEM("sss %", Cmd_lens_set_sste,      "set shutter state")
SXCMD_ITEM("sis %", Cmd_lens_set_iste,      "set IR cut state")
SXCMD_ITEM("sap",   Cmd_lens_retract,       "lens retract")
SXCMD_END()


void Lens_InstallCmd(void)
{
    SxCmd_AddTable(lens);
}

BOOL Cmd_lens_zoom_init(CHAR* strCmd)
{
    Lens_Init(LENS_INIT_ZOOM);
    DBG_MSG("zoom init...\r\n");

    return TRUE;
}

BOOL Cmd_lens_focus_init(CHAR* strCmd)
{
    Lens_Init(LENS_INIT_FOCUS);
    DBG_MSG("focus init...\r\n");

    return TRUE;
}

BOOL Cmd_lens_aperture_init(CHAR* strCmd)
{
    Lens_Init(LENS_INIT_APERTURE);
    DBG_MSG("aperture init...\r\n");

    return TRUE;
}

BOOL Cmd_lens_get_zpos(CHAR* strCmd)
{
    DBG_DUMP("get zoom sec=%d, pos=%d\r\n", Lens_ZoomGetSection(ZOOM_CURR_SECTION), Lens_ZoomGetPosition(ZOOM_CURR_SECTION_POS));

    return TRUE;
}

BOOL Cmd_lens_set_zpos(CHAR* strCmd)
{
    INT32 zoom_pos;

    sscanf_s(strCmd, "%d", &zoom_pos);
    DBG_MSG("set zoom pos=%d\r\n", zoom_pos);

    Lens_ZoomDoAction(zoom_pos);

    return TRUE;
}

BOOL Cmd_lens_get_fpos(CHAR* strCmd)
{
    DBG_DUMP("get focus pos=%d\r\n", Lens_FocusGetPosition());

    return TRUE;
}

BOOL Cmd_lens_set_fpos(CHAR* strCmd)
{
    INT32 focus_pos;

    sscanf_s(strCmd, "%d", &focus_pos);
    DBG_MSG("set focus pos=%d\r\n", focus_pos);

    Lens_FocusDoAction(FOCUS_RUN, focus_pos);

    return TRUE;
}

BOOL Cmd_lens_get_apos(CHAR* strCmd)
{
    DBG_DUMP("get aperture pos=%d\r\n", Lens_ApertureGetPosition());

    return TRUE;
}

BOOL Cmd_lens_set_apos(CHAR* strCmd)
{
    UINT32 pos_cmd;
    IRIS_POS iris_pos;

    sscanf_s(strCmd, "%d", &pos_cmd);    // 0~6
    pos_cmd = (pos_cmd > 6) ? 6 : pos_cmd;
    DBG_MSG("set aperture pos=%d\r\n", pos_cmd);

    iris_pos = (IRIS_POS)pos_cmd;
    Lens_ApertureSetPosition(iris_pos);

    return TRUE;
}

BOOL Cmd_lens_set_sste(CHAR* strCmd)
{
    UINT32 cmd_param;

    sscanf_s(strCmd, "%u", &cmd_param);
    DBG_MSG("set shutter param=%u\r\n", cmd_param);

    Lens_ShutterSetState(MOTOR_SHUTTER_NORMAL, cmd_param);

    return TRUE;
}

BOOL Cmd_lens_set_iste(CHAR* strCmd)
{
    UINT32 pos_cmd;
    IRCUT_POS ircut_pos;

    sscanf_s(strCmd, "%u", &pos_cmd);
    DBG_MSG("set IR cut state=%s\r\n", (pos_cmd != 0) ? "CLOSE" : "OPEN");

    ircut_pos = (pos_cmd != 0 ? IRCUT_POS_CLOSE : IRCUT_POS_OPEN);
    Lens_IRCUTSetState(ircut_pos, 80);    // 80 msec excited time

    return TRUE;
}

BOOL Cmd_lens_retract(CHAR* strCmd)
{
    Lens_Retract();
    DBG_MSG("Lens Retract...\r\n");

    return TRUE;
}


