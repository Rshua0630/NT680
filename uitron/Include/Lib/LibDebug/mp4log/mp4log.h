///////////////////////////////////////////////////////////////////
/**
    MP4Log module.

    This module supports storing debug log in gps section in mp4 files.
    Including GPS data, there are up to 10 sub-sections can be logged. And the total log size are 100KB/sec.
    The GPS sub-section always occupies the first one and first 1KB data area.


    @file       mp4log.h
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

*/
#ifndef _MP4LOG_H
#define _MP4LOG_H

#define MP4LOG_NULL_ID                  0xFFFFFFFF      ///<  NULL MP4 subsection ID

#define MP4LOG_MAX_SECTION              10              ///<  MP4Log maximum count of sub-section
#define MP4LOG_MAX_SECTION_SIZE         100             ///<  MP4Log maximum size per second (in unit of KByte)

/**
    Install mp4log semaphore id.

    @return None.

*/
extern void mp4log_InstallID(void) _SECTION(".kercfg_text");

/**
    Open mp4log function.

    @return None.

*/
extern ER mp4log_Open(void);

/**
    Close mp4log function.

    @return None.

*/
extern ER mp4log_Close(void);

/**
    Check whether mp4log function is opened.

    @return
        - @b TRUE : mp4log function is opened.
        - @b FALSE: mp4log function is not opened.

*/
extern BOOL mp4log_IsOpened(void);


/**
    Reset frame counter to 0

    @return None.

*/
extern void mp4log_ResetFrmCnt(void);

/**
    Increase frame counter with Cnt value

    @param[in] Cnt            Add Cnt value to current counter

    @return None.

*/
extern void mp4log_IncFrmCnt(UINT32 Cnt);

/**
    Decrease frame counter with Cnt value

    @param[in] Cnt            Sub Cnt value to current counter. If Cnt > current counter, the current counter will set to 0.

    @return None.

*/
extern void mp4log_DecFrmCnt(UINT32 Cnt);

/**
    Add one sub-section to the list

    @param[in] SecName        A FourCC type sub section name.
    @param[in] Size           Size of this sub-section.
    @param[in] SecID          Sub-section ID.

    @return
        - @b E_OK : Add sub-section success.
        - @b E_SYS: Add sub-section fail.


*/
extern ER mp4log_AddSection(char *SecName, UINT32 Size, UINT32 *SecID);

/**
    Get total log size


    @return
        - @b Log size in bytes

*/
extern UINT32 mp4log_GetTotalLogSize(void);

/**
    Configure log buffer

    Buffer size must >= 2 * log size - 32

    @return
        - @b E_OK : Configure buffer memory success.
        - @b E_SYS: Configure buffer memory fail.

*/
extern ER mp4log_SetMem(UINT32 uiAddr, UINT32 uiSize);

/**
    Toggle the ping-pong buffer

    @param[in] ReadyBuf       Buffer can be set to MovRec.
    @param[in] WorkingBuf     Buffer to log current data.

    @return
        - @b E_OK : Toggle buffer sucess.
        - @b E_SYS: Toggle buffer fail.

*/
extern ER mp4log_ToggleBuf(UINT32 *ReadyBuf, UINT32 *WorkingBuf);

/**
    Add log to buffer

    @param[in] LogID          Sub-Section ID of this log.
    @param[in] Buf            Buffer address.
    @param[in] Size           Size to log (bytes).

    @return
        - @b E_OK : Toggle buffer sucess.
        - @b E_SYS: Toggle buffer fail.

*/
extern ER mp4log_AddLog(UINT32 LogID, char *Buf, UINT32 Size);

#endif //_MP4LOG_H
