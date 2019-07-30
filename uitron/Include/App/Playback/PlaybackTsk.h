/**
    Header file for Playback application.

    This file is the header file that define the API and data type for Playback application.

    @file       PlaybackTsk.h
    @ingroup    mIAPPPlay
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _PLAYBACKAPI_H
#define _PLAYBACKAPI_H

#include "SysKer.h"
#include "ExifDef.h"
#include "JpgHeader.h"
#include "GxImage.h"


/**
    @addtogroup mIAPPPlay
*/
//@{

//------------------------------------------------------------------------------
// Definitions
//------------------------------------------------------------------------------
/**
    @name Playback file format

    The format and attribute of file.

    @note For _PLAY_OBJ::uiPlayFileFmt.
*/
//@{
#define PBFMT_UNKNOWN               0x00000000  ///< File format: Unknown
#define PBFMT_JPG                   0x00000001  ///< File format: JPEG
#define PBFMT_WAV                   0x00000002  ///< File format: WAV
#define PBFMT_AVI                   0x00000004  ///< File format: AVI
#define PBFMT_ASF                   0x00000008  ///< File format: ASF
#define PBFMT_RAW                   0x00000010  ///< File format: RAW
#define PBFMT_JPGMEMO               0x00000010  ///< File attribute: JPEG with Voice-Memo
#define PBFMT_MP4                   0x00000020  ///< File format: MP4
#define PBFMT_READONLY              0x00000040  ///< File attribute: Read only
#define PBFMT_MPG                   0x00000080  ///< File format: MPEG
#define PBFMT_MOVMJPG               0x00000100  ///< File format: MOV with motion JPEG
#define PBFMT_TS                    0x00000200  ///< File format: TS
#define PB_SUPPORT_FMT              (PBFMT_JPG|PBFMT_WAV|PBFMT_AVI|PBFMT_MOVMJPG|PBFMT_ASF|PBFMT_MPG|PBFMT_TS) ///< Default file format for Playback
//@}

/**
    @name Playback image orientation

    @note For #PBPRMID_ROTATE_DIR.
*/
//@{
#define PLAY_ROTATE_DIR_UNKNOWN     0xFF        ///< Rotation direction: Initialed (unknown)
#define PLAY_ROTATE_DIR_0           PLAY_ROTATE_DIR_UNKNOWN
#define PLAY_ROTATE_DIR_90          0x00        ///< Rotation direction:  90 deg
#define PLAY_ROTATE_DIR_180         0x01        ///< Rotation direction: 180 deg
#define PLAY_ROTATE_DIR_270         0x02        ///< Rotation direction: 270 deg
#define PLAY_ROTATE_DIR_HOR         0x03        ///< Rotation direction: Horizontal
#define PLAY_ROTATE_DIR_VER         0x04        ///< Rotation direction: Vertical
//@}

/**
    @name Playback photo display direction

    @note For #PBPRMID_DISP_DIRECTION.
*/
//@{
#define PB_DISPDIR_HORIZONTAL       0x01        ///< set display direction to Horizontal
#define PB_DISPDIR_VERTICAL         0x02        ///< set display direction to Vertical
//@}

/**
    @name Playback function Enable Flag

    @note For #PBPRMID_EN_FLAGS.
*/
//@{
#define PB_ENABLE_KEEP_ASPECT_RATIO         0x00000001  ///< Keep aspect ratio (letterboxing or pillarboxing)
#define PB_ENABLE_SHOW_BACKGROUND_IN_THUMB  0x00000002  ///< Show background in thumbnail mode
#define PB_ENABLE_SEARCH_FILE_WITHOUT_DCF   0x00000004  ///< Search file without DCF
#define PB_ENABLE_SHOW_BG_IN_MOVIE          0x00000008  ///< Show background in Movie file
#define PB_ENABLE_SHOW_BG_IN_BADFILE        0x00000010  ///< Show background in Bad file
#define PB_ENABLE_PLAY_FILE_BY_DATE         0x00000020  ///< Play file by date
#define PB_ENABLE_THUMB_WITH_DIFF_FOLDER    0x00000040  ///< Thumbnail with diff folder
#define PB_ENABLE_TV_PLUGIN                 0x00000080  ///< Represent TV in plun in
#define PB_ENABLE_SPEEDUP_SCREENNAIL        0x00000100  ///< Represent screennail is appended after primary image
//@}

/**
    @name Playback thumbnail mode showing method

    @note For #PBPRMID_THUMB_SHOW_METHOD.
*/
//@{
#define PB_SHOW_THUMBNAIL_ONE_BY_ONE        0x00000001  ///< Show thumbnail image one bye one
#define PB_SHOW_THUMBNAIL_IN_THE_SAME_TIME  0x00000002  ///< Show thumbnail image in the same time
//@}

/**
    @name Playback task state

    @note For #PBPRMID_PLAYBACK_STATUS.
*/
//@{
#define PB_STA_UNKNOWN              0x00000000  ///< Unknow
#define PB_STA_NOIMAGE              0x00000001  ///< No image
#define PB_STA_ERR_FILE             0x00000002  ///< Error file
#define PB_STA_ERR_DECODE           0x00000004  ///< Decode error
#define PB_STA_ERR_WRITE            0x00000008  ///< Write error
#define PB_STA_DONE                 0x00000010  ///< Done
#define PB_STA_BUSY                 0x00000020  ///< Busy
#define PB_STA_STANDBY              0x00000040  ///< Standby
#define PB_STA_INITFAIL             0x00000080  ///< Init fail
#define PB_STA_ERR_TASK             0x00000100  ///< Task doesn't start
//@}

/**
    @name Playback single view command

    @note For _PLAY_SINGLE_OBJ::PlayCommand.
*/
//@{
#define PB_SINGLE_CURR              0x01        ///< Play current image
#define PB_SINGLE_NEXT              0x02        ///< Play next image
#define PB_SINGLE_PREV              0x04        ///< Play previous image
#define PB_SINGLE_PRIMARY           0x08        ///< Play screennail image, or primary image
#define PB_SINGLE_THUMB             0x40        ///< Play thumbnail image only
#define PB_SINGLE_NO_HIDDEN         0x80        ///< Play primary image only
//@}

/**
    @name Playback thumbnails view command

    @note For _PLAY_BROWSER_OBJ::BrowserCommand.
*/
//@{
#define PB_BROWSER_CURR             0x01        ///< Browse current thumbnail page
#define PB_BROWSER_NEXT             0x02        ///< Browse next thumbnail page
#define PB_BROWSER_PREV             0x04        ///< Browse previous thumbnail page
#define PB_BROWSER_THUMB            0x40        ///< Browse thumbnail image only
//@}

/**
    @name Playback zooming command

    @note For PB_PlayZoomMode(), PB_PlayZoomPanMode().
*/
//@{
#define PLAYZOOM_IN                 0x01        ///< Zoom in
#define PLAYZOOM_OUT                0x02        ///< Zoom out
#define PLAYZOOM_UP                 0x04        ///< Zoom - scroll up
#define PLAYZOOM_DOWN               0x08        ///< Zoom - scroll down
#define PLAYZOOM_LEFT               0x10        ///< Zoom - scroll left
#define PLAYZOOM_RIGHT              0x20        ///< Zoom - scroll right
#define PLAYZOOM_USER               0x40        ///< User defined zooming command
//@}

/**
    @name Playback mode

    @note For PBPRMID_PLAYBACK_MODE.
*/
//@{
#define PLAYMODE_THUMB              9           ///< Thumbnail mode
#define PLAYMODE_PRIMARY            10          ///< Single view mode: JPEG
#define PLAYMODE_AVI                11          ///< Single view mode: AVI
#define PLAYMODE_ROTATE             12          ///< Rotae display
#define PLAYMODE_WAV                13          ///< Single view mode: WAV
#define PLAYMODE_ASF                14          ///< Single view mode: ASF
#define PLAYMODE_MPG                16          ///< Single view mode: MPEG
#define PLAYMODE_MOVMJPG            17          ///< Single view mode: Motion JPEG
#define PLAYMODE_ONLYTHUMB          33          ///< First decode always decode only thumbnail
#define PLAYMODE_PICBRIDGE          55          ///< Picture bridge mode
#define PLAYMODE_USERJPGOUT         77          ///< User JPEG mode
#define PLAYMODE_UNKNOWN            99          ///< Initial mode when Playback is started
//@}

#define    PB_DISPSRV_DRAW                0x00000001
#define    PB_DISPSRV_FLIP                0x00000002
#define    PB_DISPSRV_PUSH                0x00000004
#define    PB_DISPSRV_DRAW_MASK           0x0000000F
#define    PB_DISPSRV_SUSPEND             0x00000010
#define    PB_DISPSRV_SUS_MASK            0x000000F0

//------------------------------------------------------------------------------
// Enumerations
//------------------------------------------------------------------------------
/**
    Playback show specific file in video1/video2

    @note For _PLAY_SPECFILE_OBJ::PlayFileVideo.
*/
typedef enum {
	PLAY_SPEC_FILE_IN_VIDEO_1,          ///<0, Display in video1
	PLAY_SPEC_FILE_IN_VIDEO_2           ///<1, Display in video2
} PB_SPEC_VDO;

/**
    Playback show specific file with/wo clear buffer

    @note For _PLAY_SPECFILE_OBJ::PlayFileClearBuf.
*/
typedef enum {
	PLAY_SPEC_FILE_WITH_CLEAR_BUF,      ///<0, Clear buffer
	PLAY_SPEC_FILE_WITH_NOT_CLEAR_BUF   ///<1, Not clear buffer
} PB_SPEC_CLR;

/**
    Playback parameters enumeration

    @note For PB_GetParam(), PB_SetParam().
*/
typedef enum _PB_PRMID {
	PBPRMID_FILE_ATTR_LOCK      = 0x01, ///< File attribute: Read Only
	PBPRMID_JPEG_WITH_VOICE     = 0x02, ///< JPEG with Voice-Memo

	PBPRMID_PLAYBACK_MODE       = 0x10, ///< Current Playback mode
	PBPRMID_PLAYBACK_STATUS     = 0x11, ///< Current status of playback task
	PBPRMID_PLAYBACK_OBJ        = 0x12, ///< Pointer to a playback object setting structure
	PBPRMID_INFO_IMG            = 0x13, ///< Pointer to the current image configuration structure
	PBPRMID_INFO_VDO            = 0x14, ///< Pointer to the current video configuration structure
	PBPRMID_ROTATE_DIR          = 0x15, ///< Current image rotation direction
	PBPRMID_MAX_PANELSZ         = 0x16, ///< MAX panel size for Playback
	PBPRMID_EN_FLAGS            = 0x17, ///< Function Enable flag of Playback
	PBPRMID_BG_COLOR            = 0x18, ///< Default background color

	PBPRMID_THUMB_CURR_IDX      = 0x20, ///< Current browsed file index in current Thumbnail page
	PBPRMID_THUMB_CURR_NUM      = 0x21, ///< Current decoded-thumb-file numbers in current Thumbnail page
	PBPRMID_THUMB_FMT_ARRAY     = 0x22, ///< Pointer to an array of file formats of current Thumbnail page
	PBPRMID_THUMB_DEC_ARRAY     = 0x23, ///< Pointer to an array of decoded results of current Thumbnail page
	PBPRMID_THUMB_SEQ_ARRAY     = 0x24, ///< Pointer to an array of file sequence IDs of current Thumbnail page
	PBPRMID_THUMB_VDO_LTH_ARRAY = 0x25, ///< Pointer to an array of video times of current Thumbnail page
	PBPRMID_THUMB_SHOW_METHOD   = 0x26, ///< Thumbnail mode showing method (#PB_SHOW_THUMBNAIL_ONE_BY_ONE/#PB_SHOW_THUMBNAIL_IN_THE_SAME_TIME)
	PBPRMID_THUMB_LAYOUT_ARRAY  = 0x27, ///< Pointer to an array of configuration the playback thumbnails mode appearance
	PBPRMID_THUMB_LAYOUT_ARRAY2 = 0x28, ///< Pointer to an array of configuration the playback thumbnails mode appearance for dual view

	PBPRMID_ZOOM_INDEX          = 0x30, ///< Current zoom-table-index
	PBPRMID_ZOOM_LVL_TBL        = 0x31, ///< Pointer to a Zooming Level Table
	PBPRMID_PAN_CURX            = 0x32, ///< Current X addr - left top corner of PBZoom sub-image relative to the main image left top corner ( (0,0) point)
	PBPRMID_PAN_CURY            = 0x33, ///< Current Y addr - left top corner of PBZoom sub-image relative to the main image left top corner ( (0,0) point)
	PBPRMID_PAN_MAXX            = 0x34, ///< MAX X addr - the MAX PBZoom image scrolling range
	PBPRMID_PAN_MAXY            = 0x35, ///< MAX Y addr - the MAX PBZoom image scrolling range

	PBPRMID_NAMEID_FILE         = 0x40, ///< Current DCF file ID
	PBPRMID_NAMEID_DIR          = 0x41, ///< Current DCF directory ID
	PBPRMID_DATABUF_ADDR        = 0x42, ///< Current data buffer address
	PBPRMID_DATABUF_SIZE        = 0x43, ///< Current data buffer size
	PBPRMID_CURR_FILEPATH       = 0x44, ///< Current file path
	PBPRMID_CURR_FILESEQ        = 0x45, ///< Current file sequence
	PBPRMID_CURR_FILESIZE       = 0x46, ///< Current file size
	PBPRMID_CURR_FILEFMT        = 0x47, ///< Current file format
	PBPRMID_TOTAL_FILE_COUNT    = 0x48, ///< Total File count

	PBPRMID_DISP_DIRECTION      = 0x50, ///< Display direction (#PB_DISPDIR_HORIZONTAL/#PB_DISPDIR_VERTICAL)
	PBPRMID_AUTO_ROTATE         = 0x51, ///< Auto-rotate image according to EXIF-Orientation (TRUE/FALSE)

	PBPRMID_SLIDE_SPEED_TBL     = 0x60, ///< Pointer to a Slide Speed Table for slide effect functions

	PBPRMID_DUAL_DISP           = 0x70, ///< Dual display
	PBPRMID_SRC_SIZE            = 0x71, ///< Source video size (PUSIZE structure)
	PBPRMID_SRC_REGION          = 0x72, ///< Source video region (PIRECT structure)
	PBPRMID_DST_REGION          = 0x73, ///< Destination video region (PIRECT structure)
	PBPRMID_VDO_WIN_ARRAY       = 0x74, ///< Pointer to an array of video window (PURECT structure)
	PBPRMID_VDO_RECT_ARRAY      = 0x75, ///< Pointer to an array of 1st video rectangle (PURECT structure)
	PBPRMID_THUMB_RECT          = 0x76, ///< Pointer to an array of thumb nail layout rectangle (PURECT structure)
	PBPRMID_THUMB_WIDTH_ARRAY   = 0x77, ///< Pointer to an array of thumb nail width
	PBPRMID_THUMB_HEIGHT_ARRAY  = 0x78, ///< Pointer to an array of thumb nail height
	PBPRMID_EXIF_EXIST          = 0x79, ///< Current file exists EXIF
	PBPRMID_EXIF_ORIENT         = 0x7A, ///< Current file EXIF orientation

	PBPRMID_FILEDB_HANDLE       = 0x90, ///< File handle to communicate with file DB
	PBPRMID_FILELIST_OBJ        = 0x91, ///< File List Object pointer
	PBPRMID_DEC_IMG_CALLBACK    = 0x92, ///< Callback function for decoding image files
	PBPRMID_DEC_VIDEO_CALLBACK  = 0x93, ///< Callback function for decoding video files
	PBPRMID_CFG4NEXT_CALLBACK   = 0x94, ///< Callback function for doing something before trigger next (DspSrv)
	PBPRMID_DISP_TRIG_CALLBACK  = 0x95, ///< Callback function for handling display event
	PBPRMID_ONDRAW_CALLBACK     = 0x96, ///< Callback function for drawing video frame
	ENUM_DUMMY4WORD(PB_PRMID)
} PB_PRMID;

/**
    Playback error code.
*/
typedef enum _PB_ERR {
	PBERR_OK                    = 0x00, ///< Operation succeeded

	PBERR_NOT_SUPPORT           = 0xF0, ///< Unsupported function
	PBERR_PAR                   = 0xF1, ///< Parameters are not valid
	PBERR_FAIL                  = 0xFF, ///< Operation failed

	ENUM_DUMMY4WORD(PB_ERR)
} PB_ERR;

/**
     Playback wait mode.

     @note For PB_WaitCommandFinish().
*/
typedef enum _PB_WAIT_MODE {
	PB_WAIT_NOWAIT              = 0x00, ///< No wait
	PB_WAIT_INFINITE            = 0xFF, ///< Wait until operations completed
	ENUM_DUMMY4WORD(PB_WAIT_MODE)
} PB_WAIT_MODE;

/**
     Playback edit image operation.

     @note For _PB_EDIT_CONFIG::Operation.
*/
typedef enum _PB_EDIT_IMAGE {
	PBEDIT_RESIZE,                      ///< Resize image to specific size and save file
	PBEDIT_REQTY,                       ///< Re-quality for the current image and save file
	PBEDIT_ROTATE,                      ///< Update current JPEG EXIF-Orientation tag and save file
	PBEDIT_ROTATE_DISP,                 ///< Rotate image for display only
	PBEDIT_CROP,                        ///< Crop the opened image and save as a new file
	PBEDIT_CUSTOMIZE_DISP,              ///< Set Customize effect for display only
	PBEDIT_CUSTOMIZE,                   ///< Set Customize effect and save file
	PBEDIT_CUSTOMIZE_PARA,              ///< Set Customize parameters (if necessary)
	ENUM_DUMMY4WORD(PB_EDIT_IMAGE)
} PB_EDIT_IMAGE;

/**
    Playback screen control type.

    Screen control to sync display with OSD.

    @note For PB_ScreenControl().
*/
typedef enum {
	PB_LOCK_SCREEN  = 0,                ///< The result of image operation will not show on display
	PB_FLUSH_SCREEN = 1,                ///< Show the result of image operation on display
	ENUM_DUMMY4WORD(PB_SCREEN_CTRL_TYPE)
} PB_SCREEN_CTRL_TYPE;

/**
    Type of the file to be loaded.

    @note For PB_QuasiLoadFile().
*/
typedef enum _PB_FILE_TYPE {
	PB_FILE_JPEG = 0,                   ///< The primary image of JPEG file
	PB_FILE_JPEG_THUMBNAIL,             ///< The thumbnail image of JPEG file
	PB_FILE_SCREENNAIL,                 ///< The screennail image of JPEG file
	PB_FILE_AVI,                        ///< The 1st frame of AVI file
	PB_FILE_AVI_THUMBNAIL,              ///< The thumbnail image of AVI file
	ENUM_DUMMY4WORD(PB_FILE_TYPE)
} PB_FILE_TYPE;

/**
     Display index

     There are two display engines have be supported.

     @note For PB_ConfigVdoWIN(), PB_Set1stVideoFrame().
*/
typedef enum _PB_DISP_IDX {
	PBDISP_IDX_PRI = 0,                 ///< primary display
	PBDISP_IDX_SEC,                     ///< secondary display
	PBDISP_IDX_MAX,
	ENUM_DUMMY4WORD(PB_DISP_IDX)
} PB_DISP_IDX;


/**
    Playback decoding image type.

    This info indicates Playback module is decoding thumbnail or primary image.

    @note For PB_DECIMG_CB.
*/
typedef enum {
	PB_DECIMG_THUMBNAIL,              ///< Playback module is decoding thumbnail image.
	PB_DECIMG_PRIMARY,                ///< Playback module is decoding primary image.
	ENUM_DUMMY4WORD(PB_DECIMG_TYPE)
} PB_DECIMG_TYPE;

typedef enum {
	PB_KEEPAR_NONE,                 // scale to full screen
	PB_KEEPAR_PILLARBOXING,         // add vertical mattes
	PB_KEEPAR_LETTERBOXING,         // add horizontal mattes
	ENUM_DUMMY4WORD(PB_KEEPAR)
} PB_KEEPAR;

typedef enum {
	PB_VIEW_STATE_NONE = 0,
	PB_VIEW_STATE_SINGLE,
	PB_VIEW_STATE_ROTATE,
	PB_VIEW_STATE_SPEZOOM,
	PB_VIEW_STATE_THUMB,
	PB_VIEW_STATE_BYPASS,
	ENUM_DUMMY4WORD(PB_VIEW_STATE)
} PB_VIEW_STATE;

typedef enum {
	PB_SET_FLG_BROWSER_END = 0,
	PB_SET_FLG_DRAW_END,
	PB_SET_FLG_TRIGGER,
	ENUM_DUMMY4WORD(PB_SET_FLG)
} PB_SET_FLG;

//------------------------------------------------------------------------------
// Function prototype
//------------------------------------------------------------------------------
/**
    Callback function prototype for decoding image file.

    The callback function is invoked everytime the image file is playbacked.
    User could decide where the image file should be decoded by returning TRUE or not by returning FLASE.
*/
typedef BOOL (*PB_DECIMG_CB)(PB_DECIMG_TYPE DecType);


/**
    Callback function prototype for decoding video file.

    The callback function is invoked everytime the video file is playbacked.
    User could decide where the video file should be decoded by returning TRUE or not by returning FLASE.
*/
typedef BOOL (*PB_DECVIDEO_CB)(UINT32 uiUserDataAddr, UINT32 uiUserDataSize);

/**
    Callback function prototype for doing something before DspSrv tirgger next image.

    The callback function is invoked everytime the image/video is playbacked.
    User could rotate image or do other operations before DspSrv tirgger next image.
*/
typedef void (*PB_CFG4NEXT_CB)(void);

/**
    Callback function prototype for triggering next display.

    The callback function is invoked everytime next display is needed to be triggered.
    User could keep source image and tirgger display task to draw the next image.
*/
typedef void (*PB_DISPTRIG_CB)(PIMG_BUF pimg_buf, BOOL trigger_draw);

/**
    Callback function prototype for drawing the image buffer that to be displayed.

    The callback function is invoked everytime next display is needed to be triggered.
    User could decide how to process the source image.
*/
typedef void (*PB_ONDRAW_CB)(PB_VIEW_STATE view_state);
//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------
/**
    The configuration object for Playback (PB_Open).

    @note For PB_Open().
*/
typedef struct _PLAY_OBJ {
	UINT32 uiMemoryAddr;            ///< Memory buffer starting address
	UINT32 uiMemorySize;            ///< Memory buffer size
	UINT32 uiPlayFileFmt;           ///< File format for playback of still image & video
} PLAY_OBJ, *PPLAY_OBJ;

/**
    Playback Edit Image configuration.

    Configuration structure, containing the ID of the operation and its parameters.

    @note
    -# For PB_EditImage().
    -# Unless stated differently the values of puiParam[] should always be zero.
*/
typedef struct _PB_EDIT_CONFIG {
	PB_EDIT_IMAGE Operation;        ///< The image editing operation ID
	UINT32        puiParam[4];      ///< The parameters for the specific editing operation
} PB_EDIT_CONFIG, *PPB_EDIT_CONFIG;

/**
    The command object for single view.

    Structure for playback mode command object.

    @note For PB_PlaySingleMode().
*/
typedef struct _PLAY_SINGLE_OBJ {
	UINT32 PlayCommand;             ///< Playback single view command
	UINT8  JumpOffset;              ///< The jump-offset of read next/previous file(s)
} PLAY_SINGLE_OBJ, *PPLAY_SINGLE_OBJ;

/**
    The command object for thumbnails view.

    Structure for thumbnail mode command object.

    @note For PB_PlayBrowserMode().
*/
typedef struct _PLAY_BROWSER_OBJ {
	UINT8  BrowserCommand;          ///< Playback thumbnails view command
	UINT8  HorNums;                 ///< How many images in horizontal direction, HorNums * VerNums must <= 25
	UINT8  VerNums;                 ///< How many images in vertical direction, HorNums * VerNums must <= 25
	UINT32 slideEffectFunc;         ///< Slide effect function, refer to SlideEffect.h
	UINT8  JumpOffset;              ///< The jump-offset of read next/previous file(s)
	BOOL   bReDecodeImages;         ///< Used to re-decode all thumb images or not
	///<    - @b FALSE: It means #JumpOffset is still in the same page
	///<    - @b TRUE:  It means Playback has to decode all images in another page
} PLAY_BROWSER_OBJ, *PPLAY_BROWSER_OBJ;


/**
    The command object for display specific file.

    Structure for showing specific file in specific video layer (#PB_SPEC_VDO) and specific size.

    @note For PB_PlaySpecFile().
*/
typedef struct _PLAY_SPECFILE_OBJ {
	PB_SPEC_VDO PlayFileVideo;      ///< Playback show specific file in video1/video2 #PB_SPEC_VDO
	PB_SPEC_CLR PlayFileClearBuf;   ///< Playback show specific file with/wo clear buffer #PB_SPEC_CLR
	URECT       PlayRect;           ///< Pointer of the rectangle array of frame buffer layout
	BOOL        bDisplayWoReDec;    ///< For accelerating 2nd same specific file
	///<    - @b FALSE: Force current image to re-decode for display
	///<    - @b TRUE:  Draw image w/o re-decode it for accelerating 2nd same specific file
} PLAY_SPECFILE_OBJ, *PPLAY_SPECFILE_OBJ;

/**
    The configuration object for capturing screen.

    Structure for capturing screen to specific image size.

    @note For PB_CaptureScreen().
*/
typedef struct _PB_LOGO_INFO {
	UINT32 uiDstWidth;              ///< [in]  The width of captured image ('0' is special case: using panel width)
	UINT32 uiDstHeight;             ///< [in]  The height of captured image ('0' is special case: using panel height)
	UINT32 uiFileSize;              ///< [in]  The upper bound size of JPEG file ('0' is special case: the file size isn't limited)
	///< [out] The size of captured JPEG file
	UINT32 uiFileAddr;              ///< [out] The starting address of captured JPEG file
} PB_LOGO_INFO, *PPB_LOGO_INFO;

/**
    Structure for coordinate System Convertion
*/
typedef struct {
	UINT32 uiGlobalCoordX;      ///< [in] global coordinate x            => ex. (140, 0, 320, 240)
	UINT32 uiGlobalCoordY;      ///< [in] global coordinate y
	UINT32 uiGlobalCoordW;      ///< [in] global coordinate width
	UINT32 uiGlobalCoordH;      ///< [in] global coordinate height
	UINT32 uiGlobalCoord_RngW;  ///< [in] global coordinate range width  => ex. (0, 0, ,640, 240)
	UINT32 uiGlobalCoord_RngH;  ///< [in] global coordinate range height

	UINT32 uiClientCoord_OriX;  ///< [in] client coordinate origin x     => ex. (10, 60, 320, 120)
	UINT32 uiClientCoord_OriY;  ///< [in] client coordinate origin y
	UINT32 uiClientCoord_RngW;  ///< [in] client coordinate range width
	UINT32 uiClientCoord_RngH;  ///< [in] client coordinate range height

	URECT *prcOut;           ///< [out] output coordinate
} COORDSYS_CONV, *PCOORDSYS_CONV;


//------------------------------------------------------------------------------
// Export Functions
//------------------------------------------------------------------------------
/**
    Install Playback task, flag and semaphore ID

    @return void
*/
extern void PB_InstallID(void) _SECTION(".kercfg_text");

/**
    Open Playback mode.

    Open playback mode and start playback task.

    @param[in] pPlayObj Pointer of the configuration object for Playback: #_PLAY_OBJ
    @return
        - @b PBERR_OK:   Success.
        - @b PBERR_FAIL: Fail.

    Example:
    @code
    {
        PLAY_OBJ PlayObj;

        PlayObj.uiMemoryAddr = 0x80000000;
        PlayObj.uiMemorySize = 0x00200000;
        PlayObj.uiPlayFileFmt = (PBFMT_JPG | PBFMT_AVI);
        PB_Open(&PlayObj);
    }
    @endcode
*/
extern PB_ERR PB_Open(PPLAY_OBJ pPlayObj);

/**
    Close Playback mode.

    Close playback mode and terminate playback task.

    @param[in] TimeOut Playback wait mode
    @return
        - @b PBERR_OK:   Success.
        - @b PBERR_FAIL: Fail.
*/
extern PB_ERR PB_Close(PB_WAIT_MODE TimeOut);

/**
    Get Playback parameter.

    To get the value of a specific Playback parameter.

    @param[in] ParamID    The identifier of the Playback parameter to be set.
    @param[in,out] puiVal Pointer where to write the value read from the Playback for the specific parameter.
    @return
        - @b PBERR_OK:  Success.
        - @b PBERR_PAR: Parameters are not valid.
*/
extern PB_ERR PB_GetParam(PB_PRMID ParamID, UINT32 *puiVal);

/**
    Set Playback parameter.

    To set a specific value to some Playback parameters.

    @param[in] ParamID The identifier of the Playback parameter to be set.
    @param[in] uiValue The value to be set to the specific Playback parameter.
    @return
        - @b PBERR_OK:  Success.
        - @b PBERR_PAR: Parameters are not valid.
*/
extern PB_ERR PB_SetParam(PB_PRMID ParamID, UINT32 uiValue);

/**
    Wait Playback command finish.

    Wait to last command of Playback be finished.

    @param[in] TimeOut Playback wait mode: #_PB_WAIT_MODE.
    @return
        - @b Task state: Playback task state according to the responsive state.
*/
extern INT32 PB_WaitCommandFinish(PB_WAIT_MODE TimeOut);

//-------------------------[    Async Functions    ]----------------------------
/**
    Edit image.

    This function executes an image editing operation.
    @note
    -# Image editing is allowed only in Playback mode of JPEG file format
    -# This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] pConfig Pointer to the Playback Edit Image configuration structure: #_PB_EDIT_CONFIG.
        - @b Operation - The operation to be performed:
            - @b #PBEDIT_RESIZE
                - @b puiParam[0] Width of the output image size.
                - @b puiParam[1] Height of the output image size.
                - @b puiParam[2] If overwrite the currently file.
            - @b #PBEDIT_ROTATE
                - @b puiParam[0] JPEG EXIF-Orientation.
                - @b puiParam[1] If display the edited image after the change is applied.
                - @b puiParam[2] If overwrite the currently file.
            - @b #PBEDIT_ROTATE_DISP
                - @b puiParam[0] JPEG EXIF-Orientation.
            - @b #PBEDIT_CROP
                - @b puiParam[0] Horizontal start of the cropping boundaries.
                - @b puiParam[1] Vertical start of the cropping boundaries.
                - @b puiParam[2] Width of the cropping boundaries.
                - @b puiParam[3] Height of the cropping boundaries.
            - @b #PBEDIT_CUSTOMIZE_DISP
                - @b puiParam[0] The Customize Edit function pointer.
                - @b puiParam[1] The required size of working buffer.
            - @b #PBEDIT_CUSTOMIZE
                - @b puiParam[0] The Customize Edit function pointer.
                - @b puiParam[1] The required size of working buffer.
                - @b puiParam[2] If overwrite the currently file.
    @return
        - @b PBERR_OK:          Success.
        - @b PBERR_PAR:         Invalid parameter(s).
        - @b PBERR_NOT_SUPPORT: Unsupported operation.

    Example:
    @code
    {
        PB_EDIT_CONFIG Config = {0};

        Config.Operation = PBEDIT_RESIZE;
        Config.puiParam[0] = 640;
        Config.puiParam[1] = 480;
        Config.puiParam[2] = FALSE;
        PB_EditImage(&Config);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    @endcode
*/
extern PB_ERR PB_EditImage(PPB_EDIT_CONFIG pConfig);

/**
    Play single image.

    Display single image and a first frame of video clips.
    @note This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] pPlaySingleObj Pointer of the command object for single view: #_PLAY_SINGLE_OBJ.
    @return void.
*/
extern void PB_PlaySingleMode(PPLAY_SINGLE_OBJ pPlaySingleObj);

/**
    Browse the image in thumbnails view.

    Display a Thumbnail page drawing thumbnail images and first frames of video clip.
    @note
    -# The horizontal images number * vertical images number can't exceed 25.
    -# This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] pPlayBrowserObj Pointer of the command object for thumbnails view: #_PLAY_BROWSER_OBJ.
    @return
        - @b PBERR_OK:    The job is sent to playback task and you have to wait for the result.
        - @b PBERR_PAR:   Invalid parameter(s).
        - @b PB_STA_DONE: This command is done.
*/
extern INT32 PB_PlayBrowserMode(PPLAY_BROWSER_OBJ pPlayBrowserObj);

/**
    Do Playback image zooming.

    @note This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] PlayCommand Playback zooming command (#PLAYZOOM_IN/#PLAYZOOM_OUT..).
    @return void.

    Example:
    @code
    {
        PB_PlayZoomMode(PLAYZOOM_IN);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    @endcode
*/
extern void PB_PlayZoomMode(UINT32 PlayCommand);

/**
    Do Playback image scrolling.

    @note This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] PlayCommand Playback zooming command (#PLAYZOOM_UP/#PLAYZOOM_DOWN/#PLAYZOOM_LEFT/#PLAYZOOM_RIGHT).
    @param[in] MoveOffsetX Horizontal scrolling offset.
    @param[in] MoveOffsetY Vertical scrolling offset.
    @return
        - @b PBERR_OK:    The job is sent to playback task and you have to wait for the result.
        - @b PBERR_PAR:   Invalid parameter(s).
        - @b PB_STA_DONE: This command is done, can not be scrolling anymore.

    Example:
    @code
    {
        PB_PlayZoomPanMode(PLAYZOOM_UP, 20, 60);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    @endcode
*/
extern INT32 PB_PlayZoomPanMode(UINT32 PlayCommand, UINT32 MoveOffsetX, UINT32 MoveOffsetY);

/**
    Do Playback digital zoom by user setting.

    @note This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] LeftUp_X the start-Left-Up-pixel-X of this picture
    @param[in] LeftUp_Y the start-Left-Up-pixel-Y of this picture
    @param[in] RightDown_X the end-Right-Down-pixel-X of this picture
    @param[in] RightDown_Y the end-Right-Down-pixel-Y of this picture
    @return
        - @b PBERR_OK:    The job is sent to playback task and you have to wait for the result.
        - @b PBERR_PAR:   Invalid parameter(s).

    Example:
@code
{
    PB_PlayZoomUserSetting(0, 0, 160, 120);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
}
@endcode
*/
extern INT32 PB_PlayZoomUserSetting(UINT32 LeftUp_X, UINT32 LeftUp_Y, UINT32 RightDown_X, UINT32 RightDown_Y);

/**
    Display spec file in video1/video2 and spec size.

    @note This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] pPlayFileObj Pointer of the command object for display specific file: #_PLAY_SPECFILE_OBJ.
    @return void.
*/
extern void PB_PlaySpecFile(PPLAY_SPECFILE_OBJ pPlayFileObj);

/**
    Capture screen to specific image size.

    @note This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in,out] pPBLogoInfo Pointer to the configuration object for capturing screen: #_PB_LOGO_INFO.
    @return void.

    Example:
    @code
    {
        PB_LOGO_INFO LogoInfo = {0};

        LogoInfo.uiDstWidth  = 640;
        LogoInfo.uiDstHeight = 480;
        LogoInfo.uiFileSize  = 38400;
        PB_CaptureScreen(&LogoInfo);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    @endcode
*/
extern void PB_CaptureScreen(PPB_LOGO_INFO pPBLogoInfo);


/**
    Do opening specific file.

    @note This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] PlayDirId  The DCF directory ID.
    @param[in] PlayFileId The DCF file ID.
    @return void.

    Example:
    @code
    {
        PB_OpenSpecFile(100, 33);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    @endcode
*/
extern void PB_OpenSpecFile(UINT32 PlayDirId, UINT32 PlayFileId);

/**
    Do opening specific file by sequence ID.

    @note This API is an Async Function (Means: need to call #PB_WaitCommandFinish() to wait for the operation to finish).

    @param[in] uiSeqID    The file sequence ID.
    @param[in] bOnlyQuery Only query file but doesn't read all file data.
    @return void.

    Example:
    @code
    {
        PB_OpenSpecFileBySeq(3, FALSE);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    @endcode
*/
extern void PB_OpenSpecFileBySeq(UINT32 uiSeqID, BOOL bOnlyQuery);

//-----------------------------[ get info ]-------------------------------------
/**
    Get the JPEG info of current image before decoding.

    @param[in] bParseEXIF
        - @b TRUE:  Parse EXIF and primary image informations.
        - @b FALSE: Only parse primary image information.
    @return pJPGInfo Pointer to JPEG decode configuration (@b NULL means parsing NG).

    Example:
    @code
    {
        PJPGHEAD_DEC_CFG pJPGDecCfg;

        pJPGDecCfg = PB_GetCurrImageInfoBeforeDec(FALSE);
    }
    @endcode
*/
extern PJPGHEAD_DEC_CFG PB_GetCurrImageInfoBeforeDec(BOOL bParseEXIF);

/**
    Using this API to check if a drawable file (JPEG, MJPEG, etc) can be displayed successfully without actually displaying it.

    @param[in] FileType  The type of the file to be loaded: #_PB_FILE_TYPE.
    @return
        - @b PBERR_OK:   Success.
        - @b PBERR_FAIL: Fail.

    Example:
    @code
    {
        PB_ERR LoadRslt;

        PB_OpenSpecFileBySeq(1, FALSE);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        LoadRslt = PB_QuasiLoadFile(PB_FILE_JPEG);
    }
@endcode
*/
extern PB_ERR PB_QuasiLoadFile(PB_FILE_TYPE FileType);

//-----------------------------[ setting  ]-------------------------------------
/**
    Set BRC (Bit Rate Control) parameters for re-encoding PRIMARY image in Playback mode.

    @param[in] uiCompressRatio Target bitstream compressed ratio.
    @param[in] uiUpBoundRatio  Upper bound ratio of target size.
    @param[in] uiLowBoundRatio Lower bound ratio of target size.
    @param[in] uiLimitCnt      The maximum times to re-encode.
    @return void.
*/
extern void PB_SetExpectJPEGSize(UINT32 uiCompressRatio, UINT32 uiUpBoundRatio, UINT32 uiLowBoundRatio, UINT32 uiLimitCnt);

/**
    Set video output window (draw image in this specific area).

    @param[in] DispIdx Display index: #_PB_DISP_IDX.
    @param[in] pRect Pointer to the rectangle of video window
    @return void
*/
extern void PB_ConfigVdoWIN(PB_DISP_IDX DispIdx, PURECT pRect);

/**
    Update the first video frame object.

    @param[in] DispIdx Display index: #_PB_DISP_IDX.
    @param[in] pRect Pointer to the rectangle of the display position for 1st video clip frame
    @return void.
*/
extern void PB_Set1stVideoFrame(PB_DISP_IDX DispIdx, PURECT pRect);

//-----------------------------[  others  ]-------------------------------------
/**
    Control the operation result to show on display immediately or not.
    @note This API is only valid for one PB operation.

    @param[in] CtrlType Playback screen control type: #PB_SCREEN_CTRL_TYPE.
    @return void.

    Example:
    @code
    {
        PB_ScreenControl(PB_LOCK_SCREEN);
        PB_PlayZoomMode(PLAYZOOM_IN);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);

        // Update OSD here..

        PB_ScreenControl(PB_FLUSH_SCREEN);
    }
    @endcode
*/
extern void PB_ScreenControl(PB_SCREEN_CTRL_TYPE CtrlType);


//-----------------------------[  system  ]-------------------------------------
/**
    Enter Sleep mode

    @return void
*/
extern void PB_Sleep(void);

/**
    Leave Sleep mode

    @return void
*/
extern void PB_Wakeup(void);

/**
    Pause DMA

    @return void
*/
extern void PB_PauseDMA(void);

/**
    Resume DMA

    @return void
*/
extern void PB_ResumeDMA(void);

/**
    Pause Display

    @param[in] bWaitFinish If wait until operations completed.
    @return void
*/
extern void PB_PauseDisp(BOOL bWaitFinish);

/**
    Resume Display

    @param[in] bWaitFinish If wait until operations completed.
    @return void
*/
extern void PB_ResumeDisp(BOOL bWaitFinish);

/**
    Change display size for TV/LCD.

    @param[in] uiDisp_w The width of the display target.
    @param[in] uiDisp_h The height of the display target.
    @return void
*/
extern void PB_ChangeDisplaySize(UINT32 uiDisp_w, UINT32 uiDisp_h);

#if 0
extern BOOL PB_DispSrv_OnLock(UINT32 *puiLocked, IMG_BUF *pImg);
#else
extern BOOL PB_LockThumb(UINT32 uiIdx);
#endif

extern BOOL PB_UnlockThumb(UINT32 uiIdx);

extern void PB_SetPBFlag(PB_SET_FLG flag);

extern INT32 PB_GetImageRatioTrans(UINT32 ImageWidth, UINT32 ImageHeight, UINT32 DisplayWidth, UINT32 DisplayHeight);

extern void PB_CoordSysConvert(PCOORDSYS_CONV pCSConv);

//*****[ Will REMOVE the following code when FileList merged with FileDB ]******
//
// [Sort by date]
//
#define PLAY_FILE_ALL               0
#define PLAY_FILE_BY_DAY            1
#define PLAY_FILE_BY_MONTH          2
#define PLAY_FILE_BY_YEAR           3

#define PB_GET_DAY_FROM_DATE(x)     (x & 0x1F)
#define PB_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)
#define PB_GET_YEAR_FROM_DATE(x)    (((x >> 9) & 0x7F)+1980)

typedef enum {
	PLAYDATE_INDEX_TO_1ST,
	PLAYDATE_INDEX_TO_CURR,
	PLAYDATE_INDEX_TO_LAST,
	PLAYDATE_INDEX_MAX
} PLAYDATE_INDEX;

extern void   PB_PlayDeletByDate(UINT32 Year, UINT32 Month, UINT32 Day);
extern UINT32 PB_SetBuffForFileDB(UINT32 u32MemAddr, UINT32 u32MaxFileNum);
extern INT32  PB_PlayByDateInit(void);
extern void   PB_PlayByDateUnInit(void);
extern UINT16 PB_GetTotalPlayDay(void);
extern UINT16 PB_GetFirstPlayDay(void);
extern UINT16 PB_GetLastPlayDay(void);
extern UINT16 PB_GetNextPlayDay(void);
extern UINT16 PB_GetPrevPlayDay(void);
extern UINT16 PB_GetCurrPlayDay(void);
extern UINT16 PB_GetCurrPlayDaySeq(void);
extern UINT16 PB_GetCurrPlayYearSeq(void);
extern UINT16 PB_GetCurrPlayDaySeqInThisYear(void);
extern UINT16 PB_GetPlayDaysNuminYear(UINT16 SpecYear);
extern UINT16 PB_GetTotalPlayMonth(void);
extern UINT16 PB_GetFirstPlayMonth(void);
extern UINT16 PB_GetNextPlayMonth(void);
extern UINT16 PB_GetPrevPlayMonth(void);
extern UINT16 PB_GetTotalPlayYear(void);
extern UINT16 PB_GetFirstPlayYear(void);
extern UINT16 PB_GetNextPlayYear(void);
extern UINT16 PB_GetPrevPlayYear(void);
extern UINT16 PB_GetTotalFilesNumOfSpecDate(void);
extern void   PB_SetPlayDate(UINT32 DateType, UINT16 Date, PLAYDATE_INDEX CurrIdxTo);
extern void   PB_SetPlayDateToPrev(void);
extern void   PB_SetPlayDateToNext(void);
//*********[ Will REMOVE above code when FileList merged with FileDB ]**********


//@}
#endif
