/**
    Header file for Display library

    Header file for Display library.

    @file       ide.h
    @ingroup    mIDrvDisp_IDE
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _IDE_H
#define _IDE_H

#include "Type.h"

/**
    @addtogroup mIDrvDisp_IDE
*/
//@{

/**
    Display plane(Osd/Video) Enable

    These definitions are used for ide_setAllWindowEn()/ide_getWindowEn() to enable/disable
    the osd/video layers.

    @name   Display plane(Osd/Video) Enable
*/
//@{
#define DISPLAY_OSD1_EN                 0x02
#define DISPLAY_OSD2_EN                 0x04
#define DISPLAY_VIDEO1_EN               0x08
#define DISPLAY_VIDEO2_EN               0x10
//@}

/**
    Display device type.

    Select display device.
\n  The IDE engine could output various type of interface format as specified in ide_setDevice().
*/
//@{
typedef enum {
	//DISPLAY_DEVICE_CASIO1G          = 0x0,
	DISPLAY_DEVICE_CASIO2G          = 0x1,
	DISPLAY_DEVICE_AU               = 0x2,
	DISPLAY_DEVICE_TOPPOLY          = 0x3,
	DISPLAY_DEVICE_CCIR656          = 0x4,
	DISPLAY_DEVICE_CCIR601          = 0x5,
	DISPLAY_DEVICE_TV               = 0x6,
	DISPLAY_DEVICE_HDMI_24BIT       = 0x7,
	DISPLAY_DEVICE_HDMI_16BIT       = 0x8,
	DISPALY_DEVICE_PARALLEL         = 0x9,
	DISPLAY_DEVICE_CCIR656_16BIT    = 0xA,
	DISPLAY_DEVICE_CCIR601_16BIT    = 0xB,
	DISPLAY_DEVICE_MI               = 0xC,
	DISPLAY_DEVICE_MIPIDSI          = 0xD,
	DISPLAY_DEVICE_RGB_16BIT        = 0xE,
	DISPLAY_DEVICE_OUTPUT_DRAM      = 0xF,

	ENUM_DUMMY4WORD(IDE_DEVICE_TYPE)
} IDE_DEVICE_TYPE;
//@}


/**
    Video buffer operation mode

    Select video buffer operation mode. Used in ide_setV1BufOp()/ide_setV2BufOp().
*/
//@{
typedef enum {
	IDE_VIDEO_BJMODE_CANT_CROSS_WRITE =   0,   ///> Automatically change buffer according to IPE/IME signal
	IDE_VIDEO_BJMODE_RW_INDEPENDENT =     1,   ///> Automatically change buffer according to IDE's VSync signal
	IDE_VIDEO_BJMODE_BUFFER_REPEAT =      2,    ///> Repeat the same buffer until user change buffer

	ENUM_DUMMY4WORD(IDE_BJMODE)
} IDE_BJMODE;
//@}

/**
    Total video buffer number

    Select the total video buffer number in ide_setV1BufOp()/ide_setV2BufOp().
*/
//@{
typedef enum {
	IDE_VIDEO_BUFFER_NUM_1 =              0,   ///> Only 1 buffer (Buffer 0)
	IDE_VIDEO_BUFFER_NUM_2 =              1,   ///> Total 2 buffers (Buffer 0, 1)
	IDE_VIDEO_BUFFER_NUM_3 =              2,   ///> Total 3 buffers (Buffer 0, 1, 2)
	IDE_VIDEO_BUFFER_NUM_4 =              3,    ///> Total 4 buffers (Buffer 0, 1, 2, 3)

	ENUM_DUMMY4WORD(IDE_BUF_NUM)
} IDE_BUF_NUM;
//@}

/**
    Video buffer in operation

    Select the active buffer in current usage.
\n  These definitions are used in ide_setV1BufOp()/ ide_setV2BufOp()/ ide_chV1Buf()/ ide_chV2Buf().
*/
//@{
typedef enum {
	IDE_VIDEO_BUFFER_OPT_0 =             0,   ///> Buffer 0 in operation
	IDE_VIDEO_BUFFER_OPT_1 =             1,   ///> Buffer 1 in operation
	IDE_VIDEO_BUFFER_OPT_2 =             2,   ///> Buffer 2 in operation
	IDE_VIDEO_BUFFER_OPT_3 =             3,    ///> Buffer 3 in operation

	ENUM_DUMMY4WORD(IDE_OP_BUF)
} IDE_OP_BUF;
//@}

/**
    Video/OSD buffer horizontal read mode

    IDE can flip the image on the LCD screen without changing the buffer content.
    These definitions select the screen show of horizontal direction from RIGHT or from LEFT.
\n  These definitions are used in ide_setV1ReadOrder()/ ide_setV2ReadOrder()/ ide_setO1ReadOrder()/ ide_setO2ReadOrder().
*/
//@{
typedef enum {
	IDE_BUFFER_READ_L2R = 0,                  ///> Read from left to right
	IDE_BUFFER_READ_R2L = 1,                   ///> Read from right to left

	ENUM_DUMMY4WORD(IDE_HOR_READ)
} IDE_HOR_READ;
//@}

/**
    Video/OSD buffer vertical read mode

    IDE can flip the image on the LCD screen without changing the buffer content.
    These definitions select the screen show of vertical direction from TOP or from BOTTOM.
\n  These definitions are used in ide_setV1ReadOrder()/ ide_setV2ReadOrder()/ ide_setO1ReadOrder()/ ide_setO2ReadOrder().
*/
//@{
typedef enum {
	IDE_BUFFER_READ_T2B = 0,                  ///> Read from top to bottom
	IDE_BUFFER_READ_B2T = 1,                   ///> Read from bottom to top

	ENUM_DUMMY4WORD(IDE_VER_READ)
} IDE_VER_READ;
//@}

/**
    Video hardware auto handshake sync source select.

\n  These definitions are used in ide_setV1Src().
*/
//@{
typedef enum {
	IDE_SYNC_IME_PATH1 = 0,                  ///> sync with ime path1
	IDE_SYNC_IME_PATH2 = 1,                  ///> sync with ime path2
	IDE_SYNC_IME_PATH3 = 2,                   ///> sync with ime path3

	ENUM_DUMMY4WORD(IDE_SYNC_SRC)
} IDE_SYNC_SRC;
//@}


/**
    Video buffer selection for buffer adddress setting

    This definition is used to dedicate which buffer address is selected to set in ide_setVideoBufAddr().

    @name   Video buffer selection for buffer adddress setting
*/
//@{
#define IDE_VIDEO_ADDR_SEL_ALL              (0x00)      ///< Select Video buffer All (0/1/2)
#define IDE_VIDEO_ADDR_SEL_BUFFER0          (0x01 << 0) ///< Select Video buffer 0
#define IDE_VIDEO_ADDR_SEL_BUFFER1          (0x01 << 1) ///< Select Video buffer 1
#define IDE_VIDEO_ADDR_SEL_BUFFER2          (0x01 << 2) ///< Select Video buffer 2
//@}

/**
    Video window ID select.

    This is selection ID to dedicate which video is selcted to change the settings.
\n  This definition is used  in ide_setVideoBufAddr()/ ide_configVideoCst()/ ide_enableVideo()/ ... etc.
*/
//@{
typedef enum {
	IDE_VIDEOID_1 = 1,                      ///< IDE VIDEO plane 1
	IDE_VIDEOID_2 = 2,                      ///< IDE VIDEO plane 2

	ENUM_DUMMY4WORD(IDE_VIDEOID)
} IDE_VIDEOID;
//@}

/**
    OSD window ID select.

    This is selection ID to dedicate which osd is selcted to change the settings.
\n  This definition is used  in ide_setOsdBufAddr()()/ ide_enableOsd()/ ide_configOsdCst() ... etc.
*/
//@{
typedef enum {
	IDE_OSDID_1 = 1,    ///< OSD plane 1
	IDE_OSDID_2 = 2,    ///< OSD plane 2, some chip revision may not have second OSD plane
	IDE_OSDID_3 = 3,    ///< OSD plane 3, might same as IDE_VIDEOID_1
	IDE_OSDID_4 = 4,     ///< OSD plane 4, might same as IDE_VIDEOID_2

	ENUM_DUMMY4WORD(IDE_OSDID)
} IDE_OSDID;
//@}

/**
    Video blending with Video Key operation select.

    Video1/Video2 blends with VideoKey definitions.
\n  This is for argument of ide_set_video_blend() and return value of
    ide_get_video_blend();
*/
//@{
typedef enum {
	IDE_VIDEO_BLEND_VIDEOCK      = 0x00,    ///< VideoCK
	IDE_VIDEO_BLEND_VIDEOCK7_8   = 0x01,    ///< VideoCK * (7/8) + Video * (1/8)
	IDE_VIDEO_BLEND_VIDEOCK3_4   = 0x02,    ///< VideoCK * (3/4) + Video * (1/4)
	IDE_VIDEO_BLEND_VIDEOCK1_2   = 0x03,    ///< VideoCK * (1/2) + Video * (1/2)
	IDE_VIDEO_BLEND_VIDEOCK1_4   = 0x04,    ///< VideoCK * (1/4) + Video * (3/4)
	IDE_VIDEO_BLEND_VIDEOCK1_8   = 0x05,    ///< VideoCK * (1/8) + Video * (7/8)
	IDE_VIDEO_BLEND_VIDEO1OR2    = 0x06,     ///< Video1 or Video2

	ENUM_DUMMY4WORD(IDE_VIDEO_BLEND_OP)
} IDE_VIDEO_BLEND_OP;
//@}

#if 1

/**
    Video1 blending with Video2 operation select.

    Video1 blends with Video2 definitions.
*/
//@{
typedef enum {
	IDE_VIDEO_BLEND_VIDEO2       = 0x00,    ///< Video2
	IDE_VIDEO_BLEND_VIDEO2_7_8   = 0x01,    ///< Video2 * (7/8) + Video1 * (1/8)
	IDE_VIDEO_BLEND_VIDEO2_3_4   = 0x02,    ///< Video2 * (3/4) + Video1 * (1/4)
	IDE_VIDEO_BLEND_VIDEO2_1_2   = 0x03,    ///< Video2 * (1/2) + Video1 * (1/2)
	IDE_VIDEO_BLEND_VIDEO2_1_4   = 0x04,    ///< Video2 * (1/4) + Video1 * (3/4)
	IDE_VIDEO_BLEND_VIDEO2_1_8   = 0x05,    ///< Video2 * (1/8) + Video1 * (7/8)
	IDE_VIDEO_BLEND_VIDEO1       = 0x06,     ///< Video1

	ENUM_DUMMY4WORD(IDE_V1_V2_BLEND_OP)
} IDE_V1_V2_BLEND_OP;
//@}
#endif

/**
    Video color key select.

    This is for argument of ide_set_video_colorkey_op() and
    return value of ide_get_video_colorkey_op().
*/
//@{
typedef enum {
	IDE_VIDEO_COLORKEY_VIDEO1OR2 = 0x00,    ///< videock = video1 or video2;
	IDE_VIDEO_COLORKEY_YSMALLKEY = 0x01,    ///< videock = (video_Y < VDO_YKEY) ?
	IDE_VIDEO_COLORKEY_YEQUKEY   = 0x02,    ///< videock = (video_Y == VDO_KEY && video_CB == VDO_CBKEY && video_CR == VDO_CRKEY) ?
	IDE_VIDEO_COLORKEY_YBIGKEY   = 0x03,     ///< videock = (video_Y > VDO_YKEY) ?

	ENUM_DUMMY4WORD(IDE_VIDEO_COLORKEY_OP)
} IDE_VIDEO_COLORKEY_OP;
//@}

/**
    OSD color key select.

    This is for argument of ide_setOsd1ColorKeyOp() and ide_setOsd2ColorKeyOp
    return value of ide_getOsd1ColorKeyOp() or ide_getOsd2ColorKeyOp()
*/
//@{
typedef enum {
	IDE_OSD_COLORKEY_YEQUKEY = 0x00,      ///< osdck = (osd_Y == OSD_KEY && osd_CB == OSD_CBKEY && osd_CR == OSD_CRKEY) ?
	IDE_OSD_COLORKEY_YSMALLKEY = 0x01,    ///< osdck = (osd_Y < OSD_YKEY) ?
	IDE_OSD_COLORKEY_YBIGKEY   = 0x02,    ///< osdck = (osd_Y > OSD_YKEY) ?

	ENUM_DUMMY4WORD(IDE_OSD_COLORKEY_OP)
} IDE_OSD_COLORKEY_OP;
//@}


/**
    Video color key compare source select.

    This is for argument of ide_set_video_colorkey_sel() and
    return value of ide_get_video_colorkey_sel()
*/
//@{
typedef enum {
	IDE_VIDEO_COLORKEY_COMPAREVIDEO2 = 0x00, ///< Colorkey will compare with video2
	IDE_VIDEO_COLORKEY_COMPAREVIDEO1 = 0x01,  ///< Colorkey will compare with video1

	ENUM_DUMMY4WORD(IDE_VIDEO_COLORKEY_SEL)
} IDE_VIDEO_COLORKEY_SEL;
//@}

/**
    OSD blending operation select.

    This is definition for osd1/osd2 blending operation usage in ide_setOsdBlendOp().
*/
//@{
typedef enum {
	IDE_OSD_BLEND_OSD2           = 0x00,    ///< OSD2 only
	IDE_OSD_BLEND_OSD2_7_8       = 0x01,    ///< OSD2 * (7/8) + OSD1 * (1/8)
	IDE_OSD_BLEND_OSD2_3_4       = 0x02,    ///< OSD2 * (3/4) + OSD1 * (1/4)
	IDE_OSD_BLEND_OSD2_1_2       = 0x03,    ///< OSD2 * (1/2) + OSD1 * (1/2)
	IDE_OSD_BLEND_OSD2_1_4       = 0x04,    ///< OSD2 * (1/4) + OSD1 * (3/4)
	IDE_OSD_BLEND_OSD2_1_8       = 0x05,    ///< OSD2 * (1/8) + OSD1 * (7/8)
	IDE_OSD_BLEND_OSD1           = 0x06,     ///< OSD1 only

	ENUM_DUMMY4WORD(IDE_OSD_BLEND_OP)
} IDE_OSD_BLEND_OP;
//@}

/**
    OSD palette set select.

    OSD's palette has two banks.
    This definition is used to select osd to use the lower/higher 256 entities as palette.
    Two osd can not use the same bank of palette.
*/
//@{
typedef enum {
	IDE_PALETTE_LOW256           = 0x00,    ///< IDE Palette will use lower 256
	IDE_PALETTE_HIGH256          = 0x01,     ///< IDE palette will use higher 256

	ENUM_DUMMY4WORD(IDE_PALETTE_SEL)
} IDE_PALETTE_SEL;
//@}

/**
    OSD & Video blending operation select.

    This definition is used to select the blending relation between the osd and video layer.
*/
//@{
typedef enum {
	IDE_OV_BLEND_VIDEO          = 0x00,     ///< Video only
	IDE_OV_BLEND_VIDEO_7_8      = 0x01,     ///< Video * (7/8) + OSD * (1/8)
	IDE_OV_BLEND_VIDEO_3_4      = 0x02,     ///< Video * (3/4) + OSD * (1/4)
	IDE_OV_BLEND_VIDEO_1_2      = 0x03,     ///< Video * (1/2) + OSD * (1/2)
	IDE_OV_BLEND_VIDEO_1_4      = 0x04,     ///< Video * (1/4) + OSD * (3/4)
	IDE_OV_BLEND_VIDEO_1_8      = 0x05,     ///< Video * (1/8) + OSD * (7/8)
	IDE_OV_BLEND_OSD            = 0x06,     ///< OSD only
	IDE_OV_BLEND_VIDEO_AND_OSD  = 0x07,     ///< Video & OSD
	IDE_OV_BLEND_VIDEO_OR_OSD   = 0x08,     ///< Video | OSD
	IDE_OV_BLEND_VIDEO_XOR_OSD  = 0x09,     ///< Video ^ OSD
	IDE_OV_BLEND_NOT_VIDEO      = 0x0A,      ///< !Video

	ENUM_DUMMY4WORD(IDE_OV_BLEND_OP)
} IDE_OV_BLEND_OP;
//@}

/**
    @name Display ICST output type select.
*/
//@{
typedef enum {
	ICST_CCIR601    = 0,                    ///< ICST type 601
	ICST_ITU_BT_709 = 1,                    ///< ICST type 709

	ENUM_DUMMY4WORD(ICST_TYPE)
} ICST_TYPE;
//@}

/**
    Display ICST output type select.

    Not available in this chip version, just for backward compatible usage.
*/
//@{
typedef enum {
	CST_RGB2YCBCR = 0,                     ///< CST RGB to YCBCR
	CST_YCBCR2RGB = 1,                     ///< CST YCBCR to RGB

	ENUM_DUMMY4WORD(CST_SEL)
} CST_SEL;
//@}

/**
    CST accuracy select.

    Color space transform accuracy selecion. Used in ide_setIcstCoef()/ ide_setV1CstCoef()/ ... etc.
*/
//@{
typedef enum {
	ACCRCY_1_2_8 = 0,                    ///< CST accuracy 1_2_8
	ACCRCY_1_3_7 = 1,                    ///< CST accuracy 1_3_7

	ENUM_DUMMY4WORD(ACCRCY_SEL)
} ACCRCY_SEL;
//@}

/**
    IDE frame buffer pixel format.

    IDE input frame buffer format selection for video and osd.
*/
//@{
typedef enum {
	COLOR_1_BIT             = 0,    // Color format 1 bit
	COLOR_2_BIT             = 1,    // Color format 2 bit
	COLOR_4_BIT             = 2,    // Color format 4 bit
	COLOR_8_BIT             = 3,    // Color format 8 bit
	COLOR_YCBCR444          = 4,    // Color format YCBCR444
	COLOR_YCBCR422          = 5,    // Color format YCBCR422
	COLOR_YCBCR420          = 6,    // Color format YCBCR420
	COLOR_ARGB4565          = 7,    // Color format ARGB4565
	COLOR_ARGB8565          = 8,    // Color format ARGB8565
	COLOR_YCC422P           = 9,    // Color format YCBCR422Pack
	COLOR_YCC420P           = 10,   // Color format YCBCR420Pack
	COLOR_ARGB8888          = 11,   // Color format ARGB8888
	COLOR_ARGB4444          = 12,   // Color format ARGB4444
	COLOR_ARGB1555          = 13,   // Color format ARGB1555

	ENUM_DUMMY4WORD(IDE_COLOR_FORMAT)
} IDE_COLOR_FORMAT;
//@}

/**
    Determine which buffer is selected.

    Used in ide_setVideoBufferContent() to set video buffer content.
*/
//@{
typedef enum {
	IDE_VIDEO_BUFFERID_0,       ///< Buffer0 will be selected
	IDE_VIDEO_BUFFERID_1,       ///< Buffer1 will be selected
	IDE_VIDEO_BUFFERID_2,       ///< Buffer2 will be selected
	IDE_VIDEO_BUFFERID_3,       ///< Buffer3 will be selected
	IDE_VIDEO_BUFFERID_CURRENT,  ///< Current Operating buffer will be selected

	ENUM_DUMMY4WORD(IDE_BUFFERID)
} IDE_BUFFERID;
//@}

/**
    IDE LCD pixel direction.

    Set the output pixel RGB direction.
*/
//@{
typedef enum {
	IDE_PDIR_RGB = 0,   ///< R...G...B...
	IDE_PDIR_RBG = 1,    ///< R...B...G...

	ENUM_DUMMY4WORD(IDE_PDIR)
} IDE_PDIR;
//@}

/**
    IDE RGB format begin color.

    Used in ide_setOdd()/ ide_setEven() to set the interlaced mode odd/even begin color.
*/
//@{
typedef enum {
	IDE_LCD_R = 0,      ///< Color begin from R
	IDE_LCD_G = 1,      ///< Color begin from G
	IDE_LCD_B = 2,       ///< Color begin from B

	ENUM_DUMMY4WORD(IDE_PORDER)
} IDE_PORDER;
//@}

/**
    IDE dithering valid bits.

    Select the dithering function valid bits.
*/
//@{
typedef enum {
	IDE_DITHER_4BITS = 0,   ///< Dithering 4 valid bits.
	IDE_DITHER_5BITS = 1,   ///< Dithering 5 valid bits.
	IDE_DITHER_6BITS = 2,   ///< Dithering 6 valid bits.
	IDE_DITHER_7BITS = 3,    ///< Dithering 7 valid bits.

	ENUM_DUMMY4WORD(IDE_DITHER_VBITS)
} IDE_DITHER_VBITS;
//@}

/**
    IDE output component selection.

    Output component selection when parallel interface is selected.
*/
//@{
typedef enum {
	IDE_COMPONENT_R = 0,    ///< Select output component as R
	IDE_COMPONENT_G = 1,    ///< Select output component as G
	IDE_COMPONENT_B = 2,    ///< Select output component as B

	IDE_COMPONENT_Y = 0,    ///< Select output component as Y
	IDE_COMPONENT_CB = 1,   ///< Select output component as Cb
	IDE_COMPONENT_CR = 2,    ///< Select output component as Cr

	ENUM_DUMMY4WORD(IDE_OUT_COMP)
} IDE_OUT_COMP;
//@}

/**
    TV src select.

    Select the TV src.
*/
//@{
typedef enum {
	IDE_TV_SRC_IDE = 0,     ///< TV source is IDE1
	IDE_TV_SRC_IDE2 = 1,     ///< TV source is IDE2

	ENUM_DUMMY4WORD(IDE_TV_SRC)
} IDE_TV_SRC;
//@}


/**
    TV mode select.

    Select the TV mode.
*/
//@{
typedef enum {
	IDE_TV_NTSC_M = 0,      ///< TV mode is NTSC(M)
	IDE_TV_PAL_BDGHI = 1,   ///< TV mode is PAL(BDGHI)
	IDE_TV_PAL_M = 2,       ///< TV mode is PAL(M)
	IDE_TV_PAL_NC = 3,       ///< TV mode is PAL(NC)

	ENUM_DUMMY4WORD(IDE_TV_MODE)
} IDE_TV_MODE;
//@}

/**
    Defines IDE palette capability.

    Defines the current palette capability of this chip version.
*/
//@{
typedef enum {
	IDE_PAL_CAP_16X2 = 0,   ///< palette support 16 entities and 2 banks.
	IDE_PAL_CAP_256X1 = 1,  ///< palette support 256 entities and 1 bank.
	IDE_PAL_CAP_256X2 = 2,   ///< palette support 256 entities and 2 banks.

	ENUM_DUMMY4WORD(IDE_PAL_CAP)
} IDE_PAL_CAP;
//@}

/**
    Defines IDE alpha blending layer ID

    Defines IDE alpha blending layer ID
*/
//@{
typedef enum {
	IDE_BLEND_V2 = 0,                           ///< Layer V2
	IDE_BLEND_FD = 1,                           ///< Layer FD
	IDE_BLEND_LINE = 2,                          ///< Layer FD
	IDE_BLEND_O1 = 3,                           ///< Layer O1 (if FMT = ARGB1555 => source alpha = 1)
	IDE_BLEND_O1_GLBALPHA5 = 4,                 ///< Layer O1 & FMT=ARGB1555&source alpha = 0

	ENUM_DUMMY4WORD(IDE_BLEND_LAYER)
} IDE_BLEND_LAYER;
//@}

/**
    Defines IDE alpha blending type.

    Defines IDE alpha blending type.
*/
//@{
typedef enum {
	IDE_NO_ALPHA = 0,           ///< Disable alpha blending
	IDE_GLOBAL_ALPHA = 1,       ///< Select Global alpha blending
	IDE_SOURCE_ALPHA = 2,       ///< Select Source alpha blending

	ENUM_DUMMY4WORD(IDE_ALPHA_TYPE)
} IDE_ALPHA_TYPE;
//@}

/**
   Defines IDE scaling method

   Defines IDE scaling method
*/
//@{
typedef enum {
	IDE_SCALEMETHOD_DROP = 0,       ///< scaling method drop
	IDE_SCALEMETHOD_BILINEAR = 1,   ///< scaling method bilinear

	ENUM_DUMMY4WORD(IDE_SCALE_METHOD)
} IDE_SCALE_METHOD;
//@}


/**
   Defines IDE FD number

   Defines IDE FD number
*/
//@{
typedef enum {
	IDE_FD_0 = 0x00000001,        ///< FD 0
	IDE_FD_1 = 0x00000002,        ///< FD 1
	IDE_FD_2 = 0x00000004,        ///< FD 2
	IDE_FD_3 = 0x00000008,        ///< FD 3
	IDE_FD_4 = 0x00000010,        ///< FD 4
	IDE_FD_5 = 0x00000020,        ///< FD 5
	IDE_FD_6 = 0x00000040,        ///< FD 6
	IDE_FD_7 = 0x00000080,        ///< FD 7
	IDE_FD_8 = 0x00000100,        ///< FD 8
	IDE_FD_9 = 0x00000200,        ///< FD 9
	IDE_FD_10 = 0x00000400,       ///< FD 10
	IDE_FD_11 = 0x00000800,       ///< FD 11
	IDE_FD_12 = 0x00001000,       ///< FD 12
	IDE_FD_13 = 0x00002000,       ///< FD 13
	IDE_FD_14 = 0x00004000,       ///< FD 14
	IDE_FD_15 = 0x00008000,       ///< FD 15
	IDE_FD_16 = 0x00010000,       ///< FD 16
	IDE_FD_17 = 0x00020000,       ///< FD 17
	IDE_FD_18 = 0x00040000,       ///< FD 18
	IDE_FD_19 = 0x00080000,       ///< FD 19
	IDE_FD_20 = 0x00100000,       ///< FD 20
	IDE_FD_21 = 0x00200000,       ///< FD 21
	IDE_FD_22 = 0x00400000,       ///< FD 22
	IDE_FD_23 = 0x00800000,       ///< FD 23
	IDE_FD_24 = 0x01000000,       ///< FD 24
	IDE_FD_25 = 0x02000000,       ///< FD 25
	IDE_FD_26 = 0x04000000,       ///< FD 26
	IDE_FD_27 = 0x08000000,       ///< FD 27
	IDE_FD_28 = 0x10000000,       ///< FD 28
	IDE_FD_29 = 0x20000000,       ///< FD 29
	IDE_FD_30 = 0x40000000,       ///< FD 30
	IDE_FD_31 = 0x80000000,       ///< FD 31


	ENUM_DUMMY4WORD(IDE_FD_NUM)
} IDE_FD_NUM;
//@}

/**
   Defines IDE Line number

   Defines IDE Line number
*/
//@{
typedef enum {
	IDE_LINE_0 = 0x00000001,        ///< LINE 0
	IDE_LINE_1 = 0x00000002,        ///< LINE 1
	IDE_LINE_2 = 0x00000004,        ///< LINE 2
	IDE_LINE_3 = 0x00000008,        ///< LINE 3
	IDE_LINE_4 = 0x00000010,        ///< LINE 4
	IDE_LINE_5 = 0x00000020,        ///< LINE 5
	IDE_LINE_6 = 0x00000040,        ///< LINE 6
	IDE_LINE_7 = 0x00000080,        ///< LINE 7
	IDE_LINE_8 = 0x00000100,        ///< LINE 8
	IDE_LINE_9 = 0x00000200,        ///< LINE 9
	IDE_LINE_10 = 0x00000400,       ///< LINE 10
	IDE_LINE_11 = 0x00000800,       ///< LINE 11
	IDE_LINE_12 = 0x00001000,       ///< LINE 12
	IDE_LINE_13 = 0x00002000,       ///< LINE 13
	IDE_LINE_14 = 0x00004000,       ///< LINE 14
	IDE_LINE_15 = 0x00008000,       ///< LINE 15

	ENUM_DUMMY4WORD(IDE_LINE_NUM)
} IDE_LINE_NUM;
//@}

/**
    IDE channel ID

    @note For ide_setConfig()
*/
//@{
typedef enum {
	IDE_ID_1,                          ///< IDE Controller
	IDE_ID_2,                          ///< IDE2 Controller

	ENUM_DUMMY4WORD(IDE_ID)
} IDE_ID;
//@}

/**
    Define YCC Clamp

    Define YCC Clamp
*/
//@{
typedef enum {
	IDE_YCCCLAMP_NOCLAMP,             ///< YCC no clamp
	IDE_YCCCLAMP_1_254,               ///< YCC clamp to 1 to 254
	IDE_YCCCLAMP_16_235,              ///< YCC clamp to 16 to 235

	ENUM_DUMMY4WORD(IDE_YCCCLAMP)
} IDE_YCCCLAMP;
//@}

/**
   Defines TV Test mode

   Defines TV Test mode
*/
//@{
typedef enum {
	IDETV_DAC_TESTMODE_SEL_COLORBAR = 0,    ///< TV test mode color bar
	IDETV_DAC_TESTMODE_SEL_RAMP,            ///< TV test mode ramp
	IDETV_DAC_TESTMODE_SEL_FIX,             ///< TV test mode fix value

	ENUM_DUMMY4WORD(IDETV_DAC_TESTMODE_SEL)
} IDETV_DAC_TESTMODE_SEL;
//@}

typedef enum {
	IDETV_CONFIG_ID_TVMODE,
	IDETV_CONFIG_ID_TESTMODE_EN,
	IDETV_CONFIG_ID_TESTMODE_SEL,
	IDETV_CONFIG_ID_BRL,
	IDETV_CONFIG_ID_CKPN_POL,
	IDETV_CONFIG_ID_BLL,
	IDETV_CONFIG_ID_LLVEL_CLAMP,
	IDETV_CONFIG_ID_YLPS_BYPASS,
	IDETV_CONFIG_ID_IRE_SETUP,
	IDETV_CONFIG_ID_Y_SCALE,
	IDETV_CONFIG_ID_CB_SCALE,
	IDETV_CONFIG_ID_CR_SCALE,

	//DAC relative
	IDETV_CONFIG_ID_DAC_PD,

	ENUM_DUMMY4WORD(IDETV_CONFIG_ID)
} IDETV_CONFIG_ID;

typedef enum {
	IDETV_CONFIGCOEF_ID_CHROMA_FILTER,
	IDETV_CONFIGCOEF_ID_Y_FILTER,

	ENUM_DUMMY4WORD(IDETV_CONFIGCOEF_ID)
} IDETV_CONFIGCOEF_ID;


/**
    IDE configuration ID

    @note For IDE_setConfig()
*/
typedef enum {
	//
	IDE_CONFIG_DITHER_EN,              ///<
	IDE_CONFIG_DITHER_FREERUN,         ///<
	IDE_CONFIG_DISPDEV,
	IDE_CONFIG_PDIR,
	IDE_CONFIG_LCDL0,
	IDE_CONFIG_LCDL1,
	IDE_CONFIG_HSINV,
	IDE_CONFIG_VSINV,
	IDE_CONFIG_HVLDINV,
	IDE_CONFIG_VVLDINV,
	IDE_CONFIG_CLKINV,
	IDE_CONFIG_FLDINV,
	IDE_CONFIG_RGBDSEL,
	IDE_CONFIG_DEINV,
	IDE_CONFIG_OUTDDR,
	IDE_CONFIG_THROUGHSEL,

	//
	IDE_CONFIG_IDEEN,
	IDE_CONFIG_LAYEREN,

	//TG
	IDE_CONFIG_TG_HSYNC,
	IDE_CONFIG_TG_HTOTAL,
	IDE_CONFIG_TG_HSYNCDLY,
	IDE_CONFIG_TG_HVALIDST,
	IDE_CONFIG_TG_HVALIDED,
	IDE_CONFIG_TG_VSYNC,
	IDE_CONFIG_TG_VTOTAL,
	IDE_CONFIG_TG_VSYNCDLY,
	IDE_CONFIG_TG_FLD0_VVALIDST,
	IDE_CONFIG_TG_FLD0_VVALIDED,
	IDE_CONFIG_TG_FLD1_VVALIDST,
	IDE_CONFIG_TG_FLD1_VVALIDED,
	IDE_CONFIG_TG_INTERLACE_EN,
	IDE_CONFIG_TG_FLD0_CBLKST,
	IDE_CONFIG_TG_FLD0_CBLKED,
	IDE_CONFIG_TG_FLD1_CBLKST,
	IDE_CONFIG_TG_FLD1_CBLKED,
	IDE_CONFIG_TG_CFIDST,
	IDE_CONFIG_TG_CFIDED,

	IDE_CONFIG_LINE_COMPARATIVE_METHOD,

	//

	ENUM_DUMMY4WORD(IDE_CONFIG_ID)
} IDE_CONFIG_ID;


/**
    Display buffer and window size.

    Main structure to dedicate the frame buffer size and the display window size.
*/
//@{
typedef struct {
	UINT32  uiWidth;        ///< Display buffer width
	UINT32  uiHeight;       ///< Display buffer height
	UINT32  uiWinWidth;     ///< Display window width
	UINT32  uiWinHeight;    ///< Display window height
} LCDSIZE, *PLCDSIZE;
//@}

/**
    Video buffer address.

    Main structure to dedicate the frame buffer starting address.
*/
//@{
typedef struct {
	UINT32   B0_Y;          ///<Buffer0 Y starting address
	UINT32   B0_CB;         ///<Buffer0 Cb/CbCrPack starting address
	UINT32   B0_CR;         ///<Buffer0 Cr starting address (not used in CbCr packed format)
	UINT32   B1_Y;          ///<Buffer1 Y starting address
	UINT32   B1_CB;         ///<Buffer1 Cb/CbCrPack starting address
	UINT32   B1_CR;         ///<Buffer1 Cr starting address (not used in CbCr packed format)
	UINT32   B2_Y;          ///<Buffer2 Y starting address
	UINT32   B2_CB;         ///<Buffer2 Cb/CbCrPack starting address
	UINT32   B2_CR;         ///<Buffer2 Cr starting address (not used in CbCr packed format)
	UINT32   B3_Y;          ///<Buffer3 Y starting address
	UINT32   B3_CB;         ///<Buffer3 Cb/CbCrPack starting address
	UINT32   B3_CR;         ///<Buffer3 Cr starting address (not used in UVP format)
	UINT8    uiBufSel;      ///<individual buffer number selection. It must be the logic OR operation of IDE_VIDEO_ADDR_SEL_BUFFER0, IDE_VIDEO_ADDR_SEL_BUFFER1 and IDE_VIDEO_ADDR_SEL_BUFFER2
} VIDEO_BUF_ADDR;
//@}

/**
    Video buffer dimension and attribute.

    Main structure to dedicate the video buffer attributes.
*/
//@{
typedef struct {
	UINT32   Buf_W;             ///<Buffer width
	UINT32   Buf_H;             ///<Buffer height
	UINT32   Buf_Lineoffset;    ///<Buffer line offset
	BOOL     Buf_L2R;           ///<Read direction, IDE_BUFFER_READ_L2R or IDE_BUFFER_READ_R2L
	BOOL     Buf_T2B;           ///<Read direction, IDE_BUFFER_READ_T2B or IDE_BUFFER_READ_B2T
	UINT32   Buf_BJmode;        ///<Buffer operation mode, refer to IDE_VIDEO_BJMODE_XX definition
	UINT32   Buf_Opt;           ///<Buffer in operation, IDE_VIDEO_BUFFER_OPT_0 ~ 2
	UINT32   Buf_Num;           ///<Total buffer number, refer to IDE_VIDEO_BUFFER_NUM_X definition
} VIDEO_BUF_ATTR;
//@}

/**
    OSD buffer dimension and attribute.

    Main structure to dedicate the osd buffer attributes.
*/
//@{
typedef struct {
	UINT32   Buf_W;             ///<Buffer width
	UINT32   Buf_H;             ///<Buffer height
	UINT32   Buf_Lineoffset;    ///<Buffer line offset
	BOOL     Buf_L2R;           ///<Read direction, IDE_BUFFER_READ_L2R or IDE_BUFFER_READ_R2L
	BOOL     Buf_T2B;           ///<Read direction, IDE_BUFFER_READ_T2B or IDE_BUFFER_READ_B2T
} OSD_BUF_ATTR;
//@}

/**
    YCbCr color pixel components.

    Used in ide_setVideoBufferContent()/ide_convertRGB2YCbCr() to specify YCbCr values.
*/
//@{
typedef struct {
	UINT8    ColorY;            ///<Y Color Value
	UINT8    ColorCB;           ///<CB Color Value
	UINT8    ColorCR;           ///<CR Color Value
} YUVCOLOR, *PYUVCOLOR;
//@}

/**
    RGB color pixel components.

    Used in ide_convertRGB2YCbCr() to specify RGB balues.
*/
//@{
typedef struct {
	UINT8    ColorR;            ///<R Color Value
	UINT8    ColorG;            ///<G Color Value
	UINT8    ColorB;            ///<B Color Value
} RGBCOLOR, *PRGBCOLOR;
//@}

/**
    OSD display dimension and format.

    Used in ide_setVideoWinAttr()/ ide_setOsdWinAttr() to setup video/osd  window attributes.
*/
//@{
typedef struct {
	UINT32   Source_W;          ///<Buffer width
	UINT32   Source_H;          ///<Buffer height
	UINT32   Des_W;             ///<Window width
	UINT32   Des_H;             ///<Window height
	UINT32   Win_Format;        ///<Window format, refer to IDE_COLOR_FORMAT type
	UINT32   Win_X;             ///<Window start position X
	UINT32   Win_Y;             ///<Window start position Y
	UINT32   High_Addr;         ///<Select palette
} VOSD_WINDOW_ATTR;
//@}

/**
    OSD palette entry definition.

    Used in ide_setPaletteEntry() to setup specified palette entry content.
*/
//@{
typedef struct {
	UINT32   Entry;
	YUVCOLOR OSDColor;
	UINT8    Blend_OP;
	UINT8    Blink;
} PALETTE_ENTRY, *PPALETTE_ENTRY;
//@}

/**
    IDE Interrupt Call Back Function

    IDE Interrupt Call Back Function
*/
//@{
typedef void (*FPIDEInterruptCallBack)(void);
//@}

//new API
extern ER    idec_open(IDE_ID id);
extern BOOL  idec_isOpened(IDE_ID id);
extern ER    idec_close(IDE_ID id);
extern ER    idec_setConfig(IDE_ID IDEID, IDE_CONFIG_ID configID, UINT32 configContext);



//-------------------------------------------------
// IDE register programm APIs
//-------------------------------------------------

// IDE control
void idec_setDithering(IDE_ID id, BOOL b_en, BOOL b_freerun);
void idec_setDevice(IDE_ID id, IDE_DEVICE_TYPE uidevice);
void idec_setPdir(IDE_ID id, IDE_PDIR b_pdir);
void idec_setOdd(IDE_ID id, IDE_PORDER uiODD);
void idec_setEven(IDE_ID id, IDE_PORDER uiEVEN);
void idec_setHsInv(IDE_ID id, BOOL b_inv);
void idec_setVsInv(IDE_ID id, BOOL b_inv);
void idec_setHvldInv(IDE_ID id, BOOL b_inv);
void idec_setVvldInv(IDE_ID id, BOOL b_inv);
void idec_setClkInv(IDE_ID id, BOOL b_inv);
void idec_setFldInv(IDE_ID id, BOOL b_inv);
void idec_setDeInv(IDE_ID id, BOOL b_inv);
void idec_setRgbd(IDE_ID id, BOOL b_en);
void idec_setThrough(IDE_ID id, BOOL b_en);
void idec_setHdmiDdr(IDE_ID id, BOOL b_en);
void idec_set_dram_out_format(IDE_ID id, BOOL is_yuv422);

#if 0
void idec_setReset(IDE_ID id);
#endif
void idec_setEn(IDE_ID id, BOOL b_en);
void idec_setLoad(IDE_ID id);
void idec_setDevNo(IDE_ID id, BOOL b_no);
void idec_setClk1_2(IDE_ID id, BOOL b_en);
void idec_setHLPFEn(IDE_ID id, BOOL b_en);
BOOL idec_getHLPFEn(IDE_ID id);

BOOL idec_getEn(IDE_ID id);
BOOL idec_getLoad(IDE_ID id);
BOOL idec_getThrough(IDE_ID id);
BOOL idec_getRgbd(IDE_ID id);
IDE_DEVICE_TYPE idec_getDevice(IDE_ID id);

UINT32 idec_getWindowEn(IDE_ID id);
void idec_setAllWindowDis(IDE_ID id);
void idec_setAllWindowEn(IDE_ID id, UINT32 uiWins);

// Scaling control
//void idec_setDimCvert(void);
//void idec_setCvertFactor(UINT32 uiHFT, UINT32 uiVFT);

// Palette Control
void idec_setPalEntry(IDE_ID id, UINT32 uiEntry, UINT8 uiColorY, UINT8 uiColorCB, UINT8 uiColorCR, UINT8 uiAlpha);
void idec_getPalEntry(IDE_ID id, UINT32 uiEntry, UINT8 *uiColorY, UINT8 *uiColorCB, UINT8 *uiColorCR, UINT8 *uiAlpha);
void idec_setPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_getPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
//void idec_getShadowPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_setPaletteGroupACrCbY(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_getPaletteGroupACrCbY(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_setPal(IDE_ID id, UINT32 uiEntry, UINT32 uiPalette);
void idec_getPal(IDE_ID id, UINT32 uiEntry, UINT32 *uiPalette);

// Misc
void idec_setBackground(IDE_ID id, UINT8 uiColorY, UINT8 uiColorCB, UINT8 uiColorCR);
void idec_getBackground(IDE_ID id, UINT8 *uiColorY, UINT8 *uiColorCB, UINT8 *uiColorCR);



// Timing
void idec_setHorTiming(IDE_ID id, UINT32 uiHsynct, UINT32 uiHtotal, UINT32 uiHvalidst, UINT32 uiHvalided);
void idec_setVerTiming(IDE_ID id, UINT32 uiVsynct, UINT32 uiVtotal, UINT32 uiODDVvalidst, UINT32 uiODDVvalided, UINT32 uiEVENVvalidst, UINT32 uiEVNEVvalided);
void idec_getHorTiming(IDE_ID id, UINT32 *puiHsynct, UINT32 *puiHtotal, UINT32 *puiHvalidst, UINT32 *puiHvalided);
void idec_getVerTiming(IDE_ID id, UINT32 *puiVsynct, UINT32 *puiVtotal, UINT32 *puiODDVvalidst, UINT32 *puiODDVvalided, UINT32 *puiEVENVvalidst, UINT32 *puiEVNEVvalided);
void idec_setInterLoadMode(IDE_ID id, BOOL b2field);

void idec_setSyncDelay(IDE_ID id, UINT8 uiHsDealy, UINT8 uiVsDelay);
void idec_setInterlace(IDE_ID id, BOOL b_inter);
void idec_setStartField(IDE_ID id, BOOL b_oddst);
BOOL idec_getInterlace(IDE_ID id);
BOOL idec_getStartField(IDE_ID id);
BOOL idec_getCurField(IDE_ID id);
void idec_setDigitalTiming(IDE_ID id, UINT32 uiCoddBlkSt, UINT32 uiCoddBlkEd, UINT32 uiCevenBlkSt, UINT32 uiCevenBlkEd, UINT32 uiCfidSt, UINT32 uiCfidEd);

// CSB
void idec_fillGamma(IDE_ID id, UINT8 *uipgamma);
void idec_getGamma(IDE_ID id, UINT8 *uipgamma);
void idec_fillRGBGamma(IDE_ID id, UINT8 *uiprgamma, UINT8 *uipggamma, UINT8 *uipbgamma);
void idec_getRGBGamma(IDE_ID id, UINT8 *uiprgamma, UINT8 *uipggamma, UINT8 *uipbgamma);
void idec_setCtrst(IDE_ID id, UINT32 uiCTRST);
UINT32 idec_getCtrst(IDE_ID id);
void idec_setBrt(IDE_ID id, UINT32 uiBRT);
INT8 idec_getBrt(IDE_ID id);
void idec_setCmults(IDE_ID id, UINT32 uiCMULTS);
UINT32 idec_getCmults(IDE_ID id);

void idec_setClamp(IDE_ID id,  IDE_YCCCLAMP clamp);
IDE_YCCCLAMP idec_getClamp(IDE_ID id);
void idec_setCex(IDE_ID id, BOOL b_cex);
BOOL idec_getCex(IDE_ID id);
void idec_setYCex(IDE_ID id, BOOL b_ycex);
BOOL idec_getYCex(IDE_ID id);

void idec_setIcst0(IDE_ID id, BOOL b_en);
BOOL idec_getIcst0(IDE_ID id);
void idec_setIcst(IDE_ID id, BOOL b_en);
BOOL idec_getIcst(IDE_ID id);
void idec_setCst1(IDE_ID id, BOOL b_en);
BOOL idec_getCst1(IDE_ID id);


// color control
void idec_setColorCtrlEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCtrlEn(IDE_ID id);
void idec_setColorCompAdjEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCompAdjEn(IDE_ID id);
void idec_setColorCtrlHueAdjEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCtrlHueAdjEn(IDE_ID id);
void idec_setColorCompYConEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCompYConEn(IDE_ID id);
void idec_setColorCompCConEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCompCConEn(IDE_ID id);
void idec_setColorCtrlHue(IDE_ID id, UINT8 *hue24tbl);
void idec_getColorCtrlHue(IDE_ID id, UINT8 *hue24tbl);
void idec_setColorCtrlInt(IDE_ID id, INT8 *intensity24tbl);
void idec_getColorCtrlInt(IDE_ID id, INT8 *intensity24tbl);
void idec_setColorCtrlSat(IDE_ID id, INT8 *sat24tbl);
void idec_getColorCtrlSat(IDE_ID id, INT8 *sat24tbl);
void idec_setColorCtrlDds(IDE_ID id, UINT8 *dds8tbl);
void idec_getColorCtrlDds(IDE_ID id, UINT8 *dds8tbl);
void idec_setColorCtrlIntSatOfs(IDE_ID id, INT8 int_ofs, INT8 sat_ofs);
void idec_getColorCtrlIntSatOfs(IDE_ID id, INT8 *int_ofs, INT8 *sat_ofs);
void idec_setColorCompYCon(IDE_ID id, UINT8 cony);
void idec_getColorCompYCon(IDE_ID id, UINT8 *cony);
void idec_setColorCompCCon(IDE_ID id, UINT8 conc);
void idec_getColorCompCCon(IDE_ID id, UINT8 *conc);
void idec_setColorCompYOfs(IDE_ID id, INT8 yofs);
void idec_getColorCompYOfs(IDE_ID id, INT8 *yofs);
void idec_setColorCompCOfs(IDE_ID id, UINT8 cbofs, UINT8 crofs);
void idec_getColorCompCOfs(IDE_ID id, UINT8 *cbofs, UINT8 *crofs);

void idec_setCsbEn(IDE_ID id, BOOL b_en);
BOOL idec_getCsbEn(IDE_ID id);
void idec_setGammaEn(IDE_ID id, BOOL b_en);
BOOL idec_getGammaEn(IDE_ID id);
void idec_setDitherVbits(IDE_ID id, IDE_DITHER_VBITS b_rsel, IDE_DITHER_VBITS b_gsel, IDE_DITHER_VBITS b_bsel);
void idec_setOutComp(IDE_ID id, IDE_OUT_COMP uiComp0, IDE_OUT_COMP uiComp1, IDE_OUT_COMP uiComp2, BOOL b_BitSwp, BOOL b_Len);
void idec_getOutComp(IDE_ID id, IDE_OUT_COMP *uiComp0, IDE_OUT_COMP *uiComp1, IDE_OUT_COMP *uiComp2, BOOL *b_BitSwp, BOOL *b_Len);

// Interrupt
UINT32 idec_getInterruptStatus(IDE_ID id);
void idec_clearInterruptStatus(IDE_ID id, UINT32 uiINTstatus);
void idec_setInterruptEn(IDE_ID id, UINT32 uiINTen);
void idec_clrInterruptEn(IDE_ID id, UINT32 uiINT);
UINT32 idec_getInterruptEn(IDE_ID id);

// FCST
void idec_setFcstCoef(IDE_ID id, UINT32 *uipCoef);
void idec_getFcstCoef(IDE_ID id, UINT8 * CST0_COEF0, UINT8 * CST0_COEF1, UINT8 * CST0_COEF2, UINT8 * CST0_COEF3);

// ICST0
void idec_setIcst0PreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
void idec_getIcst0PreOffset(IDE_ID id, INT32 *uiY, INT32 *uiCb, INT32 *uiCr);
void idec_setIcst0Coef(IDE_ID id, UINT32 *uipCoef);
void idec_getIcst0Coef(IDE_ID id, INT32 *uipCoef);
void idec_setOutOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
void idec_getOutOffset(IDE_ID id, INT32 *uiY, INT32 *uiCb, INT32 *uiCr);
void idec_setOutLimit(IDE_ID id, UINT8 uiYLow, UINT8 uiYUp, UINT8 uiCbLow, UINT8 uiCbUp, UINT8 uiCrLow, UINT8 uiCrUp);

// ICST
void idec_setIcstCoef(IDE_ID id, UINT32 *uipCoef);
void idec_getIcstCoef(IDE_ID id, INT32 *uipCoef);


// Alpha Blending
void idec_setAlphaBlending(IDE_ID id, IDE_BLEND_LAYER uiLayer, IDE_ALPHA_TYPE uiAlphaType, UINT8 uiGlobalAlpha);
void idec_getAlphaBlending(IDE_ID id, IDE_BLEND_LAYER uiLayer, IDE_ALPHA_TYPE *uiAlphaType, UINT8 *uiGlobalAlpha);


// Shawdow register
//void idec_setO1PalShwEn(IDE_ID id, BOOL bEn);
void idec_setLBReadEn(IDE_ID id, BOOL bEn);
//void idec_setFDExpEn(IDE_ID id, BOOL bEn);



// Sub-Pixel interpolation
void idec_setSubpixel(IDE_ID id, BOOL bOddLine, BOOL bR, BOOL bG, BOOL bB);
void idec_getSubpixel(IDE_ID id, BOOL bOddLine, BOOL *bR, BOOL *bG, BOOL *bB);

// FD
void idec_setFDAllEn(IDE_ID id, UINT32 uiEn);
void idec_setFDAllDis(IDE_ID id, UINT32 uiDis);
UINT32 idec_getFDAllEn(IDE_ID id);
void idec_setFDEn(IDE_ID id, IDE_FD_NUM uiNum);
void idec_setFDDis(IDE_ID id, IDE_FD_NUM uiNum);
void idec_setFDWinPos(IDE_ID id, IDE_FD_NUM uiNum, UINT32 uiX, UINT32 uiY);
void idec_getFDWinPos(IDE_ID id, IDE_FD_NUM uiNum, UINT32 *uiX, UINT32 *uiY);
void idec_setFDWinDim(IDE_ID id, IDE_FD_NUM uiNum, UINT32 uiWinW, UINT32 uiWinH);
void idec_getFDWinDim(IDE_ID id, IDE_FD_NUM uiNum, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_setFDWinBord(IDE_ID id, IDE_FD_NUM uiNum, UINT32 uiBorH, UINT32 uiBorV);
void idec_getFDWinBord(IDE_ID id, IDE_FD_NUM uiNum, UINT32 *uiBorH, UINT32 *uiBorV);
void idec_setFDColor(IDE_ID id, IDE_FD_NUM uiNum, UINT8 uiY, UINT8 uiCB, UINT8 uiCR);
void idec_getFDColor(IDE_ID id, IDE_FD_NUM uiNum, UINT8 *uiY, UINT8 *uiCB, UINT8 *uiCR);
void idec_getLineColor(IDE_ID id, IDE_LINE_NUM uiNum, UINT8 *uiA, UINT8 *uiY, UINT8 *uiCB, UINT8 *uiCR);
void idec_getLinePara(IDE_ID id, IDE_LINE_NUM uiNum, UINT32 sub_line, BOOL *sign_a, BOOL *sign_b, UINT32 *a, UINT32 *b, UINT32 *slope, UINT32 *compare);
void idec_setLineAllDis(IDE_ID id, UINT32 uiDis);
UINT32 idec_getLineAllEn(IDE_ID id);
void idec_setLineEn(IDE_ID id, IDE_LINE_NUM uiNum);
void idec_setLineDis(IDE_ID id, IDE_LINE_NUM uiNum);
ER idec_setLineSourceAlpha(IDE_ID id, IDE_LINE_NUM uiNum, UINT8 source_alpha);


ER idec_drawLine(IDE_ID id, IDE_LINE_NUM uiNum, UINT32 uiX1, UINT32 uiY1, UINT32 uiX2, UINT32 uiY2, UINT32 uiBoarder, UINT8 uiY, UINT8 uiCB, UINT8 uiCR);


//-------------------------------------------------
// IDE Video register programm APIs
//-------------------------------------------------

// Video control
void idec_setV1En(IDE_ID id, BOOL b_en);
void idec_setV2En(IDE_ID id, BOOL b_en);
BOOL idec_getV1En(IDE_ID id);
BOOL idec_getV2En(IDE_ID id);

// Video buffer
void idec_setV1Buf0Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV1Buf1Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV1Buf2Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV1Buf0Odd(IDE_ID id, BOOL b_odd);
void idec_setV1Buf1Odd(IDE_ID id, BOOL b_odd);
void idec_setV1Buf2Odd(IDE_ID id, BOOL b_odd);
void idec_setV1BufDim(IDE_ID id, UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void idec_setV1ReadOrder(IDE_ID id, IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
void idec_setV1BufOp(IDE_ID id, IDE_BJMODE uiBJMODE, IDE_OP_BUF uiOPTBUF, IDE_BUF_NUM uiBUFNUM);
void idec_chV1Buf(IDE_ID id, IDE_OP_BUF uiOPTBUF);
//void idec_setV1Cst(IDE_ID id, BOOL b_en, CST_SEL b_sel);
void idec_setV1Src(IDE_ID id, IDE_SYNC_SRC b_sel);

void idec_setV2Buf0Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV2Buf1Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV2Buf2Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV2Buf0Odd(IDE_ID id, BOOL b_odd);
void idec_setV2Buf1Odd(IDE_ID id, BOOL b_odd);
void idec_setV2Buf2Odd(IDE_ID id, BOOL b_odd);
void idec_setV2BufDim(IDE_ID id, UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void idec_setV2ReadOrder(IDE_ID id, IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
void idec_setV2BufOp(IDE_ID id, IDE_BJMODE uiBJMODE, IDE_OP_BUF uiOPTBUF, IDE_BUF_NUM uiBUFNUM);
void idec_chV2Buf(IDE_ID id, IDE_OP_BUF uiOPTBUF);
//void idec_setV2Cst(IDE_ID id, BOOL b_en, CST_SEL b_sel);
void idec_setV2Src(IDE_ID id, IDE_SYNC_SRC b_sel);

void idec_getV1Buf0Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV1Buf1Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV1Buf2Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV1Buf0Odd(IDE_ID id, BOOL *b_odd);
void idec_getV1Buf1Odd(IDE_ID id, BOOL *b_odd);
void idec_getV1Buf2Odd(IDE_ID id, BOOL *b_odd);
void idec_getV1BufDim(IDE_ID id, UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void idec_getV1BufOp(IDE_ID id, IDE_BJMODE *puiBJMODE, IDE_OP_BUF *puiOPTBUF, IDE_BUF_NUM *puiBUFNUM);
void idec_getV1ReadOrder(IDE_ID id, IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

void idec_getV2Buf0Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV2Buf1Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV2Buf2Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV2Buf0Odd(IDE_ID id, BOOL *b_odd);
void idec_getV2Buf1Odd(IDE_ID id, BOOL *b_odd);
void idec_getV2Buf2Odd(IDE_ID id, BOOL *b_odd);
void idec_getV2BufDim(IDE_ID id, UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void idec_getV2BufOp(IDE_ID id, IDE_BJMODE *puiBJMODE, IDE_OP_BUF *puiOPTBUF, IDE_BUF_NUM *puiBUFNUM);
void idec_getV2ReadOrder(IDE_ID id, IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

// Video Window
void idec_setV1ScaleCtrl(IDE_ID id, BOOL b_Hscaleup, BOOL b_Vscaleup);
void idec_setV1ScaleFactor(IDE_ID id, UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF, BOOL b_vsub);
void idec_setV1VsfInit(IDE_ID id, UINT32 uiInit0, UINT32 uiInit1);
void idec_setV1WinDim(IDE_ID id, UINT32 uiWinW, UINT32 uiWinH);
void idec_setV1WinPos(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_setV1Fmt(IDE_ID id, IDE_COLOR_FORMAT uiFmt);
void idec_setV2Hsm(IDE_ID id, IDE_SCALE_METHOD hsm);

void idec_setV2ScaleCtrl(IDE_ID id, BOOL b_Hscaleup, BOOL b_Vscaleup);
void idec_setV2ScaleFactor(IDE_ID id, UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF, BOOL b_vsub);
void idec_setV2VsfInit(IDE_ID id, UINT32 uiInit0, UINT32 uiInit1);
void idec_setV1Hsm(IDE_ID id, IDE_SCALE_METHOD hsm);
void idec_setV2WinDim(IDE_ID id, UINT32 uiWinW, UINT32 uiWinH);
void idec_setV2WinPos(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_setV2Fmt(IDE_ID id, IDE_COLOR_FORMAT uiFmt);

void idec_getV1ScaleFactor(IDE_ID id, UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF, BOOL *b_vsub);
void idec_getV1WinDim(IDE_ID id, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_getV1WinPos(IDE_ID id, UINT32 *uiX, UINT32 *uiY);
void idec_getV1Fmt(IDE_ID id, IDE_COLOR_FORMAT *uiFmt);
void idec_getV1Hsm(IDE_ID id, IDE_SCALE_METHOD *hsm);

void idec_getV2ScaleFactor(IDE_ID id, UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF, BOOL *b_vsub);
void idec_getV2WinDim(IDE_ID id, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_getV2WinPos(IDE_ID id, UINT32 *uiX, UINT32 *uiY);
void idec_getV2Fmt(IDE_ID id, IDE_COLOR_FORMAT *uiFmt);
void idec_getV2Hsm(IDE_ID id, IDE_SCALE_METHOD *hsm);

// Video Color key and Blending
void idec_setVideoColorKey(IDE_ID id, UINT8 uiCkY, UINT8 uiCkCb, UINT8 uiCkCr);
//void ide_setVideoBlendOp(IDE_VIDEO_BLEND_OP uiOp);
void idec_setVideoColorKeySel(IDE_ID id, IDE_VIDEO_COLORKEY_SEL b_sel);
void idec_setVideoColorKeyOp(IDE_ID id, IDE_VIDEO_COLORKEY_OP uiCkOp);

void idec_getVideoColorKey(IDE_ID id, UINT8 *uiCkY, UINT8 *uiCkCb, UINT8 *uiCkCr);
//void ide_getVideoBlendOp(IDE_ID id, IDE_VIDEO_BLEND_OP *uiOp);
void idec_getVideoColorKeySel(IDE_ID id, IDE_VIDEO_COLORKEY_SEL *b_sel);
void idec_getVideoColorKeyOp(IDE_ID id, IDE_VIDEO_COLORKEY_OP *uiCkOp);
void idec_setV1LowPassEn(IDE_ID id, BOOL b_en);
BOOL idec_getV1LowPassEn(IDE_ID id);
void idec_setV1LowPassCoef(IDE_ID id, UINT8 *uipcoef);
void idec_getV1LowPassCoef(IDE_ID id, UINT8 *uipcoef);

// Video Color Space Transform
//void idec_setV1CstPreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setV1CstPostOffst(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setV1CstCoef(IDE_ID id, ACCRCY_SEL b_accuracy, UINT32 *uipCoef);

//void idec_setV2CstPreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setV2CstPostOffst(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setV2CstCoef(IDE_ID id, ACCRCY_SEL b_accuracy, UINT32 *uipCoef);


// Video Auto Blinking
void idec_setV1Blink(IDE_ID id, BOOL b_ovr, BOOL b_und, BOOL b_sel);
void idec_setV1Count(IDE_ID id, UINT8 uiCnt);
void idec_setV1OvrExpThreshold(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV1OvrExpColor(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV1UndExpThreshold(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV1UndExpColor(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV2Blink(IDE_ID id, BOOL b_ovr, BOOL b_und, BOOL b_sel);
void idec_setV2Count(IDE_ID id, UINT8 uiCnt);
void idec_setV2OvrExpThreshold(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV2OvrExpColor(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV2UndExpThreshold(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV2UndExpColor(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
UINT8 idec_getV1Count(IDE_ID id);
UINT8 idec_getV2Count(IDE_ID id);
void idec_getV1OvrExpThreshold(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV2OvrExpThreshold(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV1UndExpThreshold(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV2UndExpThreshold(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV1OvrExpColor(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV2OvrExpColor(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV1UndExpColor(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV2UndExpColor(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_setV2LowPassEn(IDE_ID id, BOOL b_en);
BOOL idec_getV2LowPassEn(IDE_ID id);
void idec_setV2LowPassCoef(IDE_ID id, UINT8 *uipcoef);
void idec_getV2LowPassCoef(IDE_ID id, UINT8 *uipcoef);


//-------------------------------------------------
// IDE OSD register programm APIs
//-------------------------------------------------

// OSD control
void idec_setO1En(IDE_ID id, BOOL b_en);
//void idec_setO2En(IDE_ID id, BOOL b_en);
BOOL idec_getO1En(IDE_ID id);
//BOOL idec_getO2En(IDE_ID id);

// OSD buffer
void idec_setO1BufAddr(IDE_ID id, UINT32 uiaddr);
void idec_setO1BufAlphaAddr(IDE_ID id, UINT32 uiaddr);
void idec_setO1BufDim(IDE_ID id, UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void idec_setO1ReadOrder(IDE_ID id, IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
//void ide_setO1Cst(IDE_ID id, BOOL b_en, CST_SEL b_sel);

#if 0
void idec_setO2BufAddr(IDE_ID id, UINT32 uiaddr);
void idec_setO2BufDim(IDE_ID id, UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void idec_setO2ReadOrder(IDE_ID id, IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
//void ide_setO2Cst(IDE_ID id, BOOL b_en, CST_SEL b_sel);
#endif

void idec_getO1BufAddr(IDE_ID id, UINT32 *uiaddr);
void idec_getO1AlphaBufAddr(IDE_ID id, UINT32 *uiaddr);
void idec_getO1BufDim(IDE_ID id, UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void idec_getO1ReadOrder(IDE_ID id, IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

#if 0
void idec_getO2BufAddr(IDE_ID id, UINT32 *uiaddr);
void idec_getO2BufDim(IDE_ID id, UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void idec_getO2ReadOrder(IDE_ID id, IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);
#endif

// OSD Window
void idec_setO1ScaleCtrl(IDE_ID id, BOOL b_Hscaleup, BOOL b_Vscaleup);
void idec_setO1ScaleFactor(IDE_ID id, UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF, BOOL b_vsub);
void idec_setO1VsfInit(IDE_ID id, UINT32 uiInit0, UINT32 uiInit1);
void idec_setO1Hsm(IDE_ID id, IDE_SCALE_METHOD hsm);
void idec_setO1Vsm(IDE_ID id, IDE_SCALE_METHOD vsm);
void idec_setO1WinDim(IDE_ID id, UINT32 uiWinW, UINT32 uiWinH);
void idec_setO1WinPos(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_setO1PaletteSel(IDE_ID id, IDE_PALETTE_SEL uiPsel);
void idec_setO1PaletteHighAddr(IDE_ID id, UINT8 uiHiAddr);
void idec_setO1Fmt(IDE_ID id, IDE_COLOR_FORMAT uiFmt);

#if 0
void idec_setO2ScaleCtrl(IDE_ID id, BOOL b_Hscaleup, BOOL b_Vscaleup);
void idec_setO2ScaleFactor(IDE_ID id, UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF, BOOL b_vsub);
void idec_setO2VsfInit(IDE_ID id, UINT32 uiInit0, UINT32 uiInit1);
void idec_setO2Hsm(IDE_ID id, IDE_SCALE_METHOD hsm);
void idec_setO2WinDim(IDE_ID id, UINT32 uiWinW, UINT32 uiWinH);
void idec_setO2WinPos(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_setO2PaletteSel(IDE_ID id, IDE_PALETTE_SEL uiPsel);
void idec_setO2PaletteHighAddr(IDE_ID id, UINT8 uiHiAddr);
void idec_setO2Fmt(IDE_ID id, IDE_COLOR_FORMAT uiFmt);
#endif

void idec_getO1ScaleFactor(IDE_ID id, UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF, BOOL *b_vsub);
void idec_getO1WinDim(IDE_ID id, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_getO1WinPos(IDE_ID id, UINT32 *uiX, UINT32 *uiY);
void idec_getO1PaletteSel(IDE_ID id, IDE_PALETTE_SEL *uiPsel);
void idec_getO1PaletteHighAddr(IDE_ID id, UINT8 *uiHiAddr);
void idec_getO1Fmt(IDE_ID id, IDE_COLOR_FORMAT *uiFmt);
void idec_getO1Hsm(IDE_ID id, IDE_SCALE_METHOD *hsm);

#if 0
void idec_getO2ScaleFactor(IDE_ID id, UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF, BOOL *b_vsub);
void idec_getO2WinDim(IDE_ID id, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_getO2WinPos(IDE_ID id, UINT32 *uiX, UINT32 *uiY);
void idec_getO2PaletteSel(IDE_ID id, IDE_PALETTE_SEL *uiPsel);
void idec_getO2PaletteHighAddr(IDE_ID id, UINT8 *uiHiAddr);
void idec_getO2Fmt(IDE_ID id, IDE_COLOR_FORMAT *uiFmt);
#endif

// OSD Color key and Blending
void idec_setOsdColorKeyEn(IDE_ID id, BOOL b_en);
void idec_setOsdColorKey(IDE_ID id, UINT8 uiCk);
//void ide_setOsdBlendOp(IDE_ID id, IDE_OSD_BLEND_OP uiOp);

void idec_getOsdColorKey(IDE_ID id, UINT8 *uiCk);
void idec_getOsdColorKeyEn(IDE_ID id, BOOL *b_en);
//void idec_getOsdBlendOp(IDE_ID id, IDE_OSD_BLEND_OP *uiOp);

void idec_setOsd1ColorKeyEn(IDE_ID id, BOOL b_en);
void idec_getOsd1ColorKeyEn(IDE_ID id, BOOL *b_en);
void idec_setOsd1ColorKeyOp(IDE_ID id, IDE_OSD_COLORKEY_OP ck_op);
void idec_getOsd1ColorKeyOp(IDE_ID id, IDE_OSD_COLORKEY_OP *ck_op);
void idec_setOsd1ColorKey(IDE_ID id, UINT8 uiKey_y, UINT8 uiKey_cb, UINT8 uiKey_cr);
void idec_getOsd1ColorKey(IDE_ID id, UINT8 *uiKey_y, UINT8 *uiKey_cb, UINT8 *uiKey_cr);
#if 0
void idec_setOsd2ColorKeyEn(IDE_ID id, BOOL b_en);
void idec_getOsd2ColorKeyEn(IDE_ID id, BOOL *b_en);
void idec_setOsd2ColorKeyOp(IDE_ID id, IDE_OSD_COLORKEY_OP ck_op);
void idec_getOsd2ColorKeyOp(IDE_ID id, IDE_OSD_COLORKEY_OP *ck_op);
void idec_setOsd2ColorKey(IDE_ID id, UINT8 uiKey_y, UINT8 uiKey_cb, UINT8 uiKey_cr);
void idec_getOsd2ColorKey(IDE_ID id, UINT8 *uiKey_y, UINT8 *uiKey_cb, UINT8 *uiKey_cr);

void idec_setOSDLayerSwap(IDE_ID id, BOOL bSwap);
BOOL idec_getOSDLayerSwap(IDE_ID id);
#endif


// OSD Color Space Transform
//void idec_setO1CstPreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setO1CstPostOffst(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setO1CstCoef(IDE_ID id, ACCRCY_SEL b_accuracy, UINT32 *uipCoef);

//void idec_setO2CstPreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setO2CstPostOffst(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setO2CstCoef(IDE_ID id, ACCRCY_SEL b_accuracy, UINT32 *uipCoef);
void idec_setO1LowPassEn(IDE_ID id, BOOL b_en);
BOOL idec_getO1LowPassEn(IDE_ID id);
void idec_setO1LowPassCoef(IDE_ID id, UINT8 *uipcoef);
void idec_getO1LowPassCoef(IDE_ID id, UINT8 *uipcoef);

#if 0
void idec_setO2LowPassEn(IDE_ID id, BOOL b_en);
BOOL idec_getO2LowPassEn(IDE_ID id);
void idec_setO2LowPassCoef(IDE_ID id, UINT8 *uipcoef);
void idec_getO2LowPassCoef(IDE_ID id, UINT8 *uipcoef);
#endif

//-------------------------------------------------
// IDE Public Functions
//-------------------------------------------------

BOOL idec_setVideoBufAddr(IDE_ID id, IDE_VIDEOID VideoID, VIDEO_BUF_ADDR *pV_Buf_Addr);
BOOL idec_getVideoBufAddr(IDE_ID id, IDE_VIDEOID VideoID, VIDEO_BUF_ADDR *pV_Buf_Addr);
BOOL idec_setVideoBufAttr(IDE_ID id, IDE_VIDEOID VideoID, VIDEO_BUF_ATTR *pV_Buf_attr);
BOOL idec_getVideoBufAttr(IDE_ID id, IDE_VIDEOID VideoID, VIDEO_BUF_ATTR *pV_Buf_attr);
BOOL idec_setVideoWinAttrEx(IDE_ID id, IDE_VIDEOID VideoID, VOSD_WINDOW_ATTR *pV_Win_attr, BOOL bLoad);
BOOL idec_setVideoWinAttr(IDE_ID id, IDE_VIDEOID VideoID, VOSD_WINDOW_ATTR *pV_Win_attr);
void idec_setVideoVsfInit(IDE_ID id, IDE_VIDEOID VideoID, UINT32 uiInit0, UINT32 uiInit1);

BOOL idec_setOsdBufAddr(IDE_ID id, IDE_OSDID OSDID, UINT32 uiOSD_Addr);
BOOL idec_getOsdBufAddr(IDE_ID id, IDE_OSDID OSDID, UINT32 *puiOSD_Addr);
BOOL idec_setOsdBufAttr(IDE_ID id, IDE_OSDID OSDID, OSD_BUF_ATTR *pOSD_Buf_attr);
BOOL idec_getOsdBufAttr(IDE_ID id, IDE_OSDID OSDID, OSD_BUF_ATTR *pOSD_Buf_attr);
BOOL idec_setOsdWinAttrEx(IDE_ID id, IDE_OSDID OSDID, VOSD_WINDOW_ATTR *pOSD_Win_attr, BOOL bLoad);
BOOL idec_setOsdWinAttr(IDE_ID id, IDE_OSDID OSDID, VOSD_WINDOW_ATTR *pOSD_Win_attr);
void idec_setOsdVsfInit(IDE_ID id, IDE_OSDID OSDID, UINT32 uiInit0, UINT32 uiInit1);

void idec_setPaletteEntry(IDE_ID id, PALETTE_ENTRY *pPalette_entry);
void idec_getPaletteEntry(IDE_ID id, PALETTE_ENTRY *pPalette_entry);
void idec_setPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_getPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);

void idec_waitFrameEnd(IDE_ID id);
void idec_waitYUVOutputDone(IDE_ID id);


void idec_disableVideo(IDE_ID id, IDE_VIDEOID VideoID);
void idec_enableVideo(IDE_ID id, IDE_VIDEOID VideoID);
BOOL idec_getVideoEnable(IDE_ID id, IDE_VIDEOID VideoID);

void idec_disableOsd(IDE_ID id, IDE_OSDID OSDID);
void idec_enableOsd(IDE_ID id, IDE_OSDID OSDID);
BOOL idec_getOsdEnable(IDE_ID id, IDE_OSDID OSDID);
IDE_PAL_CAP idec_getPalCapability(IDE_ID id);

void idec_setVideoBufferContent(IDE_ID id, IDE_VIDEOID VIDEO_ID, IDE_BUFFERID BUFFER_ID, PYUVCOLOR pYCbCr);
void idec_convertRGB2YCbCr(IDE_ID id, PRGBCOLOR pRGB, PYUVCOLOR pYCbCr);

//void idec_configVideoCst(IDE_VIDEOID VideoID, BOOL b_en, CST_SEL sel);
//void idec_configOsdCst(IDE_OSDID OSDID, BOOL b_en, CST_SEL sel);
//void idec_configIcst(BOOL b_en, CST_SEL sel);
void idec_configIcst(IDE_ID id, BOOL b_en, CST_SEL sel);
void idec_getconfigIcst(IDE_ID id, BOOL *b_en, CST_SEL *sel);
void idec_configOutputLimit(IDE_ID id, BOOL b_sel);
void idec_setConstantWindowOffset(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_getConstantWindowOffset(IDE_ID id, UINT32 *uiX, UINT32 *uiY);


//-------------------------------------------------
// IDE register programm APIs
//-------------------------------------------------

// IDE control
#define ide_setDithering(b_en, b_freerun)       idec_setDithering(IDE_ID_1, b_en, b_freerun)
#define ide_setDevice(uidevice)                 idec_setDevice(IDE_ID_1, uidevice)
#define ide_setPdir(b_pdir)                     idec_setPdir(IDE_ID_1, b_pdir)
#define ide_setOdd(uiODD)                       idec_setOdd(IDE_ID_1, uiODD)
#define ide_setEven(uiEVEN)                     idec_setEven(IDE_ID_1, uiEVEN)
#define ide_setHsInv(b_inv)                     idec_setHsInv(IDE_ID_1, b_inv)
#define ide_setVsInv(b_inv)                     idec_setVsInv(IDE_ID_1, b_inv)
#define ide_setHvldInv(b_inv)                   idec_setHvldInv(IDE_ID_1, b_inv)
#define ide_setVvldInv(b_inv)                   idec_setVvldInv(IDE_ID_1, b_inv)
#define ide_setClkInv(b_inv)                    idec_setClkInv(IDE_ID_1, b_inv)
#define ide_setFldInv(b_inv)                    idec_setFldInv(IDE_ID_1, b_inv)
#define ide_setDeInv(b_inv)                     idec_setDeInv(IDE_ID_1, b_inv)
#define ide_setRgbd(b_en)                       idec_setRgbd(IDE_ID_1, b_en)
#define ide_setThrough(b_en)                    idec_setThrough(IDE_ID_1, b_en)
#define ide_setHdmiDdr(b_en)                    idec_setHdmiDdr(IDE_ID_1, b_en)
#define ide_setReset()                          //idec_setReset(IDE_ID_1)
#define ide_setEn(b_en)                         idec_setEn(IDE_ID_1, b_en)
#define ide_setLoad()                           idec_setLoad(IDE_ID_1)
#define ide_setDevNo(b_no)                      idec_setDevNo(IDE_ID_1, b_no)
#define ide_setHLPFEn(b_en)                     idec_setHLPFEn(IDE_ID_1, b_en)
#define ide_getHLPFEn()                         idec_getHLPFEn(IDE_ID_1)

#define ide_getEn()                             idec_getEn(IDE_ID_1)
#define ide_getLoad()                           idec_getLoad(IDE_ID_1)
#define ide_getThrough()                        idec_getThrough(IDE_ID_1)
#define ide_getRgbd()                           idec_getRgbd(IDE_ID_1)
#define ide_getDevice()                         idec_getDevice(IDE_ID_1)

#define ide_getWindowEn()                       idec_getWindowEn(IDE_ID_1)
#define ide_setAllWindowDis()                   idec_setAllWindowDis(IDE_ID_1)
#define ide_setAllWindowEn(uiWins)              idec_setAllWindowEn(IDE_ID_1, uiWins)

// Palette Control
#define ide_setPalEntry(uiEntry, uiColorY, uiColorCB, uiColorCR, uiAlpha)     idec_setPalEntry(IDE_ID_1, uiEntry, uiColorY, uiColorCB, uiColorCR, uiAlpha)
#define ide_getPalEntry(uiEntry, uiColorY, uiColorCB, uiColorCR, uiAlpha)     idec_getPalEntry(IDE_ID_1, uiEntry, uiColorY, uiColorCB, uiColorCR, uiAlpha)
#define ide_setPal(uiEntry, uiPalette)          idec_setPal(IDE_ID_1, uiEntry, uiPalette)
#define ide_getPal(uiEntry, uiPalette)          idec_getPal(IDE_ID_1, uiEntry, uiPalette)

// Misc
#define ide_setBackground(uiColorY, uiColorCB, uiColorCR)    idec_setBackground(IDE_ID_1, uiColorY, uiColorCB, uiColorCR)
#define ide_getBackground(uiColorY, uiColorCB, uiColorCR)    idec_getBackground(IDE_ID_1, uiColorY, uiColorCB, uiColorCR)


// Timing
#define ide_setHorTiming(uiHsynct, uiHtotal, uiHvalidst, uiHvalided)      idec_setHorTiming(IDE_ID_1, uiHsynct, uiHtotal, uiHvalidst, uiHvalided)
#define ide_setVerTiming(uiVsynct, uiVtotal, uiODDVvalidst, uiODDVvalided, uiEVENVvalidst, uiEVNEVvalided)         idec_setVerTiming(IDE_ID_1, uiVsynct, uiVtotal, uiODDVvalidst, uiODDVvalided, uiEVENVvalidst, uiEVNEVvalided)
#define ide_getHorTiming(puiHsynct, puiHtotal, puiHvalidst, puiHvalided)  idec_getHorTiming(IDE_ID_1, puiHsynct, puiHtotal, puiHvalidst, puiHvalided)
#define ide_getVerTiming(puiVsynct, puiVtotal, puiODDVvalidst, puiODDVvalided, puiEVENVvalidst, puiEVNEVvalided)   idec_getVerTiming(IDE_ID_1, puiVsynct, puiVtotal, puiODDVvalidst, puiODDVvalided, puiEVENVvalidst, puiEVNEVvalided)

#define ide_setSyncDelay(uiHsDealy, uiVsDelay)  idec_setSyncDelay(IDE_ID_1, uiHsDealy, uiVsDelay)
#define ide_setInterlace(b_inter)               idec_setInterlace(IDE_ID_1, b_inter)
#define ide_setStartField(b_oddst)              idec_setStartField(IDE_ID_1, b_oddst)
#define ide_getInterlace()                      idec_getInterlace(IDE_ID_1)
#define ide_getStartField()                     idec_getStartField(IDE_ID_1)
#define ide_getCurField()                       idec_getCurField(IDE_ID_1)
#define ide_setDigitalTiming(uiCoddBlkSt, uiCoddBlkEd, uiCevenBlkSt, uiCevenBlkEd, uiCfidSt, uiCfidEd)             idec_setDigitalTiming(IDE_ID_1, uiCoddBlkSt, uiCoddBlkEd, uiCevenBlkSt, uiCevenBlkEd, uiCfidSt, uiCfidEd)

// CSB
#define ide_fillGamma(uipgamma)                 idec_fillGamma(IDE_ID_1, uipgamma)
#define ide_getGamma(uipgamma)                  idec_getGamma(IDE_ID_1, uipgamma)
#define ide_setCtrst(uiCTRST)                   idec_setCtrst(IDE_ID_1, uiCTRST)
#define ide_getCtrst()                          idec_getCtrst(IDE_ID_1)
#define ide_setBrt(uiBRT)                       idec_setBrt(IDE_ID_1, uiBRT)
#define ide_getBrt()                            idec_getBrt(IDE_ID_1)
#define ide_setCmults(uiCMULTS)                 idec_setCmults(IDE_ID_1, uiCMULTS)
#define ide_getCmults()                         idec_getCmults(IDE_ID_1)
#define ide_setCex(b_cex)                       idec_setCex(IDE_ID_1, b_cex)
#define ide_setYCex(b_ycex)                     idec_setYCex(IDE_ID_1, b_ycex)
#define ide_getYCex()                           idec_getYCex(IDE_ID_1)

#define ide_setIcst0(b_en)                      idec_setIcst0(IDE_ID_1, b_en)
#define ide_getIcst0()                          idec_getIcst0(IDE_ID_1)
#define ide_setIcst(b_en)                       idec_setIcst(IDE_ID_1, b_en)
#define ide_getIcst()                           idec_getIcst(IDE_ID_1)
#define ide_setCst1(b_en)                       idec_setCst1(IDE_ID_1, b_en)
#define ide_getCst1()                           idec_getCst1(IDE_ID_1)

#define ide_setCsbEn(b_en)                      idec_setCsbEn(IDE_ID_1, b_en)
#define ide_getCsbEn()                          idec_getCsbEn(IDE_ID_1)
#define ide_setGammaEn(b_en)                    idec_setGammaEn(IDE_ID_1, b_en)
#define ide_getGammaEn()                        idec_getGammaEn(IDE_ID_1)
#define ide_setDitherVbits(b_rsel, b_gsel, b_bsel)    idec_setDitherVbits(IDE_ID_1, b_rsel, b_gsel, b_bsel)
#define ide_setOutComp(uiComp0, uiComp1, uiComp2, b_BitSwp, b_Len)    idec_setOutComp(IDE_ID_1, uiComp0, uiComp1, uiComp2, b_BitSwp, b_Len)
#define ide_getOutComp(uiComp0, uiComp1, uiComp2, b_BitSwp, b_Len)    idec_getOutComp(IDE_ID_1, uiComp0, uiComp1, uiComp2, b_BitSwp, b_Len)

// Interrupt
#define ide_getInterruptStatus()                idec_getInterruptStatus(IDE_ID_1)
#define ide_clearInterruptStatus(uiINTstatus)   idec_clearInterruptStatus(IDE_ID_1, uiINTstatus)
#define ide_setInterruptEn(uiINTen)             idec_setInterruptEn(IDE_ID_1, uiINTen)
#define ide_clrInterruptEn(uiINT)               idec_clrInterruptEn(IDE_ID_1, uiINT)

// Color Space Transform
#define ide_setFcstCoef(uipCoef)                idec_setFcstCoef(IDE_ID_1, uipCoef)

// ICST0
#define ide_setIcst0PreOffset(uiY, uiCb, uiCr)  idec_setIcst0PreOffset(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getIcst0PreOffset(uiY, uiCb, uiCr)  idec_getIcst0PreOffset(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setIcst0Coef(uipCoef)               idec_setIcst0Coef(IDE_ID_1, uipCoef)
#define ide_getIcst0Coef(uipCoef)               idec_getIcst0Coef(IDE_ID_1, uipCoef)
#define ide_setOutOffset(uiY, uiCb, uiCr)       idec_setOutOffset(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getOutOffset(uiY, uiCb, uiCr)       idec_getOutOffset(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setOutLimit(uiYLow, uiYUp, uiCbLow, uiCbUp, uiCrLow, uiCrUp)    idec_setOutLimit(IDE_ID_1, uiYLow, uiYUp, uiCbLow, uiCbUp, uiCrLow, uiCrUp)

// ICST
#define ide_setIcstCoef(uipCoef)                idec_setIcstCoef(IDE_ID_1, uipCoef)
#define ide_getIcstCoef(uipCoef)                idec_getIcstCoef(IDE_ID_1, uipCoef)


// Alpha Blending
#define ide_setAlphaBlending(uiLayer, uiAlphaType, uiGlobalAlpha)    idec_setAlphaBlending(IDE_ID_1, uiLayer, uiAlphaType, uiGlobalAlpha)

// Sub-Pixel interpolation
#define ide_setSubpixel(bOddLine, bR, bG, bB)   idec_setSubpixel(IDE_ID_1, bOddLine, bR, bG, bB)
#define ide_getSubpixel(bOddLine, bR, bG, bB)   idec_getSubpixel(IDE_ID_1, bOddLine, bR, bG, bB)

//-------------------------------------------------
// IDE Video register programm APIs
//-------------------------------------------------

// Video control
#define ide_setV1En(b_en)                       idec_setV1En(IDE_ID_1, b_en)
#define ide_setV2En(b_en)                       idec_setV2En(IDE_ID_1, b_en)
#define ide_getV1En()                           idec_getV1En(IDE_ID_1)
#define ide_getV2En()                           idec_getV2En(IDE_ID_1)

// Video buffer
#define ide_setV1Buf0Addr(uiYaddr, uiCBaddr, uiCRaddr)    idec_setV1Buf0Addr(IDE_ID_1, uiYaddr, uiCBaddr, uiCRaddr)
#define ide_setV1Buf1Addr(uiYaddr, uiCBaddr, uiCRaddr)    idec_setV1Buf1Addr(IDE_ID_1, uiYaddr, uiCBaddr, uiCRaddr)
#define ide_setV1Buf2Addr(uiYaddr, uiCBaddr, uiCRaddr)    idec_setV1Buf2Addr(IDE_ID_1, uiYaddr, uiCBaddr, uiCRaddr)
#define ide_setV1Buf0Odd(b_odd)                 idec_setV1Buf0Odd(IDE_ID_1, b_odd)
#define ide_setV1Buf1Odd(b_odd)                 idec_setV1Buf1Odd(IDE_ID_1, b_odd)
#define ide_setV1Buf2Odd(b_odd)                 idec_setV1Buf2Odd(IDE_ID_1, b_odd)
#define ide_setV1BufDim(uiBW, uiBH, uiLineoffset)         idec_setV1BufDim(IDE_ID_1, uiBW, uiBH, uiLineoffset)
#define ide_setV1ReadOrder(b_L2R, b_T2B)        idec_setV1ReadOrder(IDE_ID_1, b_L2R, b_T2B)
#define ide_setV1BufOp(uiBJMODE, uiOPTBUF, uiBUFNUM)      idec_setV1BufOp(IDE_ID_1, uiBJMODE, uiOPTBUF, uiBUFNUM)
#define ide_chV1Buf(uiOPTBUF)                   idec_chV1Buf(IDE_ID_1, uiOPTBUF)
#define ide_setV1Src(b_sel)                     idec_setV1Src(IDE_ID_1, b_sel)

#define ide_setV2Buf0Addr(uiYaddr, uiCBaddr, uiCRaddr)    idec_setV2Buf0Addr(IDE_ID_1, uiYaddr, uiCBaddr, uiCRaddr)
#define ide_setV2Buf1Addr(uiYaddr, uiCBaddr, uiCRaddr)    idec_setV2Buf1Addr(IDE_ID_1, uiYaddr, uiCBaddr, uiCRaddr)
#define ide_setV2Buf2Addr(uiYaddr, uiCBaddr, uiCRaddr)    idec_setV2Buf2Addr(IDE_ID_1, uiYaddr, uiCBaddr, uiCRaddr)
#define ide_setV2Buf0Odd(b_odd)                 idec_setV2Buf0Odd(IDE_ID_1, b_odd)
#define ide_setV2Buf1Odd(b_odd)                 idec_setV2Buf1Odd(IDE_ID_1, b_odd)
#define ide_setV2Buf2Odd(b_odd)                 idec_setV2Buf2Odd(IDE_ID_1, b_odd)
#define ide_setV2BufDim(uiBW, uiBH, uiLineoffset)         idec_setV2BufDim(IDE_ID_1, uiBW, uiBH, uiLineoffset)
#define ide_setV2ReadOrder(b_L2R, b_T2B)        idec_setV2ReadOrder(IDE_ID_1, b_L2R, b_T2B)
#define ide_setV2BufOp(uiBJMODE, uiOPTBUF, uiBUFNUM)      idec_setV2BufOp(IDE_ID_1, uiBJMODE, uiOPTBUF, uiBUFNUM)
#define ide_chV2Buf(uiOPTBUF)                   idec_chV2Buf(IDE_ID_1, uiOPTBUF)
#define ide_setV2Src(b_sel)                     idec_setV2Src(IDE_ID_1, b_sel)

#define ide_getV1Buf0Addr(puiYaddr, puiCBaddr, puiCRaddr) idec_getV1Buf0Addr(IDE_ID_1, puiYaddr, puiCBaddr, puiCRaddr)
#define ide_getV1Buf1Addr(puiYaddr, puiCBaddr, puiCRaddr) idec_getV1Buf1Addr(IDE_ID_1, puiYaddr, puiCBaddr, puiCRaddr)
#define ide_getV1Buf2Addr(puiYaddr, puiCBaddr, puiCRaddr) idec_getV1Buf2Addr(IDE_ID_1, puiYaddr, puiCBaddr, puiCRaddr)
#define ide_getV1Buf0Odd(b_odd)                 idec_getV1Buf0Odd(IDE_ID_1, b_odd)
#define ide_getV1Buf1Odd(b_odd)                 idec_getV1Buf1Odd(IDE_ID_1, b_odd)
#define ide_getV1Buf2Odd(b_odd)                 idec_getV1Buf2Odd(IDE_ID_1, b_odd)
#define ide_getV1BufDim(puiBW, puiBH, puiLineoffset)      idec_getV1BufDim(IDE_ID_1, puiBW, puiBH, puiLineoffset)
#define ide_getV1BufOp(puiBJMODE, puiOPTBUF, puiBUFNUM)   idec_getV1BufOp(IDE_ID_1, puiBJMODE, puiOPTBUF, puiBUFNUM)
#define ide_getV1ReadOrder(pb_L2R, pb_T2B)      idec_getV1ReadOrder(IDE_ID_1, pb_L2R, pb_T2B)

#define ide_getV2Buf0Addr(puiYaddr, puiCBaddr, puiCRaddr) idec_getV2Buf0Addr(IDE_ID_1, puiYaddr, puiCBaddr, puiCRaddr)
#define ide_getV2Buf1Addr(puiYaddr, puiCBaddr, puiCRaddr) idec_getV2Buf1Addr(IDE_ID_1, puiYaddr, puiCBaddr, puiCRaddr)
#define ide_getV2Buf2Addr(puiYaddr, puiCBaddr, puiCRaddr) idec_getV2Buf2Addr(IDE_ID_1, puiYaddr, puiCBaddr, puiCRaddr)
#define ide_getV2Buf0Odd(b_odd)                 idec_getV2Buf0Odd(IDE_ID_1, b_odd)
#define ide_getV2Buf1Odd(b_odd)                 idec_getV2Buf1Odd(IDE_ID_1, b_odd)
#define ide_getV2Buf2Odd(b_odd)                 idec_getV2Buf2Odd(IDE_ID_1, b_odd)
#define ide_getV2BufDim(puiBW, puiBH, puiLineoffset)      idec_getV2BufDim(IDE_ID_1, puiBW, puiBH, puiLineoffset)
#define ide_getV2BufOp(puiBJMODE, puiOPTBUF, puiBUFNUM)   idec_getV2BufOp(IDE_ID_1, puiBJMODE, puiOPTBUF, puiBUFNUM)
#define ide_getV2ReadOrder(pb_L2R, pb_T2B)      idec_getV2ReadOrder(IDE_ID_1, pb_L2R, pb_T2B)

// Video Window
#define ide_setV1ScaleCtrl(b_Hscaleup, b_Vscaleup)        idec_setV1ScaleCtrl(IDE_ID_1, b_Hscaleup, b_Vscaleup)
#define ide_setV1ScaleFactor(uiHSF, b_sub, uiVSF, b_vsub) idec_setV1ScaleFactor(IDE_ID_1, uiHSF, b_sub, uiVSF, b_vsub)
#define ide_setV1VsfInit(uiInit0, uiInit1)      idec_setV1VsfInit(IDE_ID_1, uiInit0, uiInit1)
#define ide_setV1WinDim(uiWinW, uiWinH)         idec_setV1WinDim(IDE_ID_1, uiWinW, uiWinH)
#define ide_setV1WinPos(uiX, uiY)               idec_setV1WinPos(IDE_ID_1, uiX, uiY)
#define ide_setV1Fmt(uiFmt)                     idec_setV1Fmt(IDE_ID_1, uiFmt)
#define ide_setV2Hsm(hsm)                       idec_setV2Hsm(IDE_ID_1, hsm)

#define ide_setV2ScaleCtrl(b_Hscaleup, b_Vscaleup)        idec_setV2ScaleCtrl(IDE_ID_1, b_Hscaleup, b_Vscaleup)
#define ide_setV2ScaleFactor(uiHSF, b_sub, uiVSF, b_vsub) idec_setV2ScaleFactor(IDE_ID_1, uiHSF, b_sub, uiVSF, b_vsub)
#define ide_setV2VsfInit(uiInit0, uiInit1)                idec_setV2VsfInit(IDE_ID_1, uiInit0, uiInit1)
#define ide_setV1Hsm(hsm)                                 idec_setV1Hsm(IDE_ID_1, hsm)
#define ide_setV2WinDim(uiWinW, uiWinH)                   idec_setV2WinDim(IDE_ID_1, uiWinW, uiWinH)
#define ide_setV2WinPos(uiX, uiY)                         idec_setV2WinPos(IDE_ID_1, uiX, uiY)
#define ide_setV2Fmt(uiFmt)                               idec_setV2Fmt(IDE_ID_1, uiFmt)

#define ide_getV1ScaleFactor(uiHSF, b_sub, uiVSF, b_vsub) idec_getV1ScaleFactor(IDE_ID_1, uiHSF, b_sub, uiVSF, b_vsub)
#define ide_getV1WinDim(uiWinW, uiWinH)                   idec_getV1WinDim(IDE_ID_1, uiWinW, uiWinH)
#define ide_getV1WinPos(uiX, uiY)                         idec_getV1WinPos(IDE_ID_1, uiX, uiY)
#define ide_getV1Fmt(uiFmt)                               idec_getV1Fmt(IDE_ID_1, uiFmt)

#define ide_getV2ScaleFactor(uiHSF, b_sub, uiVSF, b_vsub) idec_getV2ScaleFactor(IDE_ID_1, uiHSF, b_sub, uiVSF, b_vsub)
#define ide_getV2WinDim(uiWinW, uiWinH)                   idec_getV2WinDim(IDE_ID_1, uiWinW, uiWinH)
#define ide_getV2WinPos(uiX, uiY)                         idec_getV2WinPos(IDE_ID_1, uiX, uiY)
#define ide_getV2Fmt(uiFmt)                               idec_getV2Fmt(IDE_ID_1, uiFmt)

// Video Color key and Blending
#define ide_setVideoColorKey(uiCkY, uiCkCb, uiCkCr)       idec_setVideoColorKey(IDE_ID_1, uiCkY, uiCkCb, uiCkCr)
#define ide_setVideoColorKeySel(b_sel)                    idec_setVideoColorKeySel(IDE_ID_1, b_sel)
#define ide_setVideoColorKeyOp(uiCkOp)                    idec_setVideoColorKeyOp(IDE_ID_1, uiCkOp)

#define ide_getVideoColorKey(uiCkY, uiCkCb, uiCkCr)       idec_getVideoColorKey(IDE_ID_1, uiCkY, uiCkCb, uiCkCr)
#define ide_getVideoColorKeySel(b_sel)                    idec_getVideoColorKeySel(IDE_ID_1, b_sel)
#define ide_getVideoColorKeyOp(uiCkOp)                    idec_getVideoColorKeyOp(IDE_ID_1, uiCkOp)


// Video Auto Blinking
#define ide_setV1Blink(b_ovr, b_und, b_sel)         idec_setV1Blink(IDE_ID_1, b_ovr, b_und, b_sel)
#define ide_setV1Count(uiCnt)                       idec_setV1Count(IDE_ID_1, uiCnt)
#define ide_setV1OvrExpThreshold(uiY, uiCb, uiCr)   idec_setV1OvrExpThreshold(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setV1OvrExpColor(uiY, uiCb, uiCr)       idec_setV1OvrExpColor(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setV1UndExpThreshold(uiY, uiCb, uiCr)   idec_setV1UndExpThreshold(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setV1UndExpColor(uiY, uiCb, uiCr)       idec_setV1UndExpColor(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setV2Blink(b_ovr, b_und, b_sel)         idec_setV2Blink(IDE_ID_1, b_ovr, b_und, b_sel)
#define ide_setV2Count(uiCnt)                       idec_setV2Count(IDE_ID_1, uiCnt)
#define ide_setV2OvrExpThreshold(uiY, uiCb, uiCr)   idec_setV2OvrExpThreshold(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setV2OvrExpColor(uiY, uiCb, uiCr)       idec_setV2OvrExpColor(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setV2UndExpThreshold(uiY, uiCb, uiCr)   idec_setV2UndExpThreshold(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_setV2UndExpColor(uiY, uiCb, uiCr)       idec_setV2UndExpColor(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getV1Count()                            idec_getV1Count(IDE_ID_1)
#define ide_getV2Count()                            idec_getV2Count(IDE_ID_1)
#define ide_getV1OvrExpThreshold(uiY, uiCb, uiCr)   idec_getV1OvrExpThreshold(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getV2OvrExpThreshold(uiY, uiCb, uiCr)   idec_getV2OvrExpThreshold(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getV1UndExpThreshold(uiY, uiCb, uiCr)   idec_getV1UndExpThreshold(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getV2UndExpThreshold(uiY, uiCb, uiCr)   idec_getV2UndExpThreshold(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getV1OvrExpColor(uiY, uiCb, uiCr)       idec_getV1OvrExpColor(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getV2OvrExpColor(uiY, uiCb, uiCr)       idec_getV2OvrExpColor(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getV1UndExpColor(uiY, uiCb, uiCr)       idec_getV1UndExpColor(IDE_ID_1, uiY, uiCb, uiCr)
#define ide_getV2UndExpColor(uiY, uiCb, uiCr)       idec_getV2UndExpColor(IDE_ID_1, uiY, uiCb, uiCr)

//-------------------------------------------------
// IDE OSD register programm APIs
//-------------------------------------------------

// OSD control
#define ide_setO1En(b_en)                           idec_setO1En(IDE_ID_1, b_en)
//#define ide_setO2En(b_en)                           idec_setO2En(IDE_ID_1, b_en)
#define ide_getO1En()                               idec_getO1En(IDE_ID_1)
//#define ide_getO2En()                               idec_getO2En(IDE_ID_1)

// OSD buffer
#define ide_setO1BufAddr(uiaddr)                    idec_setO1BufAddr(IDE_ID_1, uiaddr)
#define ide_setO1BufAlphaAddr(uiaddr)               idec_setO1BufAlphaAddr(IDE_ID_1, uiaddr)
#define ide_setO1BufDim(uiBW, uiBH, uiLineoffset)   idec_setO1BufDim(IDE_ID_1, uiBW, uiBH, uiLineoffset)
#define ide_setO1ReadOrder(b_L2R, b_T2B)            idec_setO1ReadOrder(IDE_ID_1, b_L2R, b_T2B)

#if 0
#define ide_setO2BufAddr(uiaddr)                    idec_setO2BufAddr(IDE_ID_1, uiaddr)
#define ide_setO2BufDim(uiBW, uiBH, uiLineoffset)   idec_setO2BufDim(IDE_ID_1, uiBW, uiBH, uiLineoffset)
#define ide_setO2ReadOrder(b_L2R, b_T2B)            idec_setO2ReadOrder(IDE_ID_1, b_L2R, b_T2B)
#endif

#define ide_getO1BufAddr(uiaddr)                    idec_getO1BufAddr(IDE_ID_1, uiaddr)
#define ide_getO1AlphaBufAddr(uiaddr)               idec_getO1AlphaBufAddr(IDE_ID_1, uiaddr)
#define ide_getO1BufDim(puiBW, puiBH, puiLineoffset)    idec_getO1BufDim(IDE_ID_1, puiBW, puiBH, puiLineoffset)
#define ide_getO1ReadOrder(pb_L2R, pb_T2B)          idec_getO1ReadOrder(IDE_ID_1, pb_L2R, pb_T2B)

#if 0
#define ide_getO2BufAddr(uiaddr)                    idec_getO2BufAddr(IDE_ID_1, uiaddr)
#define ide_getO2BufDim(puiBW, puiBH, puiLineoffset)    idec_getO2BufDim(IDE_ID_1, puiBW, puiBH, puiLineoffset)
#define ide_getO2ReadOrder(pb_L2R, pb_T2B)          idec_getO2ReadOrder(IDE_ID_1, pb_L2R, pb_T2B)
#endif

// OSD Window
#define ide_setO1ScaleCtrl(b_Hscaleup, b_Vscaleup)        idec_setO1ScaleCtrl(IDE_ID_1, b_Hscaleup, b_Vscaleup)
#define ide_setO1ScaleFactor(uiHSF, b_sub, uiVSF, b_vsub) idec_setO1ScaleFactor(IDE_ID_1, uiHSF, b_sub, uiVSF, b_vsub)
#define ide_setO1VsfInit(uiInit0, uiInit1)                idec_setO1VsfInit(IDE_ID_1, uiInit0, uiInit1)
#define ide_setO1Hsm(hsm)                                 idec_setO1Hsm(IDE_ID_1, hsm)
#define ide_setO1WinDim(uiWinW, uiWinH)                   idec_setO1WinDim(IDE_ID_1, uiWinW, uiWinH)
#define ide_setO1WinPos(uiX, uiY)                         idec_setO1WinPos(IDE_ID_1, uiX, uiY)
//Not use since NT96650 => removed
//#define ide_setO1PaletteSel(uiPsel)                     idec_setO1PaletteSel(IDE_ID_1, uiPsel)
#define ide_setO1PaletteHighAddr(uiHiAddr)                idec_setO1PaletteHighAddr(IDE_ID_1, uiHiAddr)
#define ide_setO1Fmt(uiFmt)                               idec_setO1Fmt(IDE_ID_1, uiFmt)

#if 0
#define ide_setO2ScaleCtrl(b_Hscaleup, b_Vscaleup)        idec_setO2ScaleCtrl(IDE_ID_1, b_Hscaleup, b_Vscaleup)
#define ide_setO2ScaleFactor(uiHSF, b_sub, uiVSF, b_vsub) idec_setO2ScaleFactor(IDE_ID_1, uiHSF, b_sub, uiVSF, b_vsub)
#define ide_setO2VsfInit(uiInit0, uiInit1)                idec_setO2VsfInit(IDE_ID_1, uiInit0, uiInit1)
#define ide_setO2WinDim(uiWinW, uiWinH)                   idec_setO2WinDim(IDE_ID_1, uiWinW, uiWinH)
#define ide_setO2WinPos(uiX, uiY)                         idec_setO2WinPos(IDE_ID_1, uiX, uiY)
#define ide_setO2PaletteSel(uiPsel)                       idec_setO2PaletteSel(IDE_ID_1, uiPsel)
#define ide_setO2PaletteHighAddr(uiHiAddr)                idec_setO2PaletteHighAddr(IDE_ID_1, uiHiAddr)
#define ide_setO2Fmt(uiFmt)                               idec_setO2Fmt(IDE_ID_1, uiFmt)
#define ide_setOSDLayerSwap(bSwap)                        idec_setOSDLayerSwap(IDE_ID_1,bSwap)
#endif
//#define ide_setFDLineLayerSwap(bSwap)                        idec_setFDLineLayerSwap(IDE_ID_1,bSwap)

#define ide_getO1ScaleFactor(uiHSF, b_sub, uiVSF, b_vsub) idec_getO1ScaleFactor(IDE_ID_1, uiHSF, b_sub, uiVSF, b_vsub)
#define ide_getO1WinDim(uiWinW, uiWinH)                   idec_getO1WinDim(IDE_ID_1, uiWinW, uiWinH)
#define ide_getO1WinPos(uiX, uiY)                         idec_getO1WinPos(IDE_ID_1, uiX, uiY)
#define ide_getO1PaletteSel(uiPsel)                       idec_getO1PaletteSel(IDE_ID_1, uiPsel)
#define ide_getO1PaletteHighAddr(uiHiAddr)                idec_getO1PaletteHighAddr(IDE_ID_1, uiHiAddr)
#define ide_getO1Fmt(uiFmt)                               idec_getO1Fmt(IDE_ID_1, uiFmt)

#if 0
#define ide_getO2ScaleFactor(uiHSF, b_sub, uiVSF, b_vsub) idec_getO2ScaleFactor(IDE_ID_1, uiHSF, b_sub, uiVSF, b_vsub)
#define ide_getO2WinDim(uiWinW, uiWinH)                   idec_getO2WinDim(IDE_ID_1, uiWinW, uiWinH)
#define ide_getO2WinPos(uiX, uiY)                         idec_getO2WinPos(IDE_ID_1, uiX, uiY)
#define ide_getO2PaletteSel(uiPsel)                       idec_getO2PaletteSel(IDE_ID_1, uiPsel)
#define ide_getO2PaletteHighAddr(uiHiAddr)                idec_getO2PaletteHighAddr(IDE_ID_1, uiHiAddr)
#define ide_getO2Fmt(uiFmt)                               idec_getO2Fmt(IDE_ID_1, uiFmt)
#define ide_getOSDLayerSwap()                             idec_getOSDLayerSwap(IDE_ID_1)
#endif

// OSD Color key and Blending
#define ide_setOsdColorKeyEn(b_en)                        idec_setOsdColorKeyEn(IDE_ID_1, b_en)
#define ide_setOsdColorKey(uiCk)                          ide_setOsdColorKey(IDE_ID_1, uiCk)
//void ide_setOsdBlendOp(IDE_OSD_BLEND_OP uiOp);

#define ide_getOsdColorKey(uiCk)                          idec_getOsdColorKey(IDE_ID_1, uiCk)
#define ide_getOsdColorKeyEn(b_en)                        idec_getOsdColorKeyEn(IDE_ID_1, b_en)
//void ide_getOsdBlendOp(IDE_OSD_BLEND_OP *uiOp);

// OSD Color Space Transform
//void ide_setO1CstPreOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setO1CstPostOffst(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setO1CstCoef(ACCRCY_SEL b_accuracy, UINT32 *uipCoef);

//void ide_setO2CstPreOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setO2CstPostOffst(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setO2CstCoef(ACCRCY_SEL b_accuracy, UINT32 *uipCoef);

//-------------------------------------------------
// TV Encoder register programm APIs
//-------------------------------------------------
//*These api will be remove begin --> add idetv_setConfig to config
ER   tv_open(void);
ER   tv_close(void);
BOOL tv_isOpened(void);
void ide_setTvUsePostICST(BOOL b_sel);
void ide_setTvUsePostGamma(BOOL b_sel);
void ide_setTvOutOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
void ide_setTvOutLimit(UINT8 uiYLow, UINT8 uiYUp, UINT8 uiCbLow, UINT8 uiCbUp, UINT8 uiCrLow, UINT8 uiCrUp);

void ide_setTvMode(IDE_TV_MODE uiTVmode);
void ide_setTvTestMode(BOOL b_en);
void ide_setTvTestModeSel(UINT8 uisel);
void ide_setTvTestModeFixValue(UINT32 uival);
void ide_setTvSlaveMode(BOOL b_en);
void ide_setTvSacleSwap(BOOL b_en);
void ide_setTvDataSwap(BOOL b_en);
void ide_setTvSinCosSwap(BOOL b_en);
void ide_setTvSearchEn(BOOL b_en);
void ide_setTvCkpnPol(BOOL b_sel);
void ide_setTvBurst(UINT8 uiBurst);
void ide_setTvBll(UINT8 uiBll);
void ide_setTvBrl(UINT8 uiBrl);
void ide_setTvNtscSetupLevel(UINT8 uiNtscSetup);
void ide_setTvSrc(IDE_TV_SRC src);
void ide_setTvScale(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setTvPowerDown(BOOL b_pd);
BOOL ide_getTvPowerDown(void);
void ide_setTvSela(UINT8 uiSela);
void ide_setTvLowLevel(UINT32 uiLevel);
void ide_setTvTrim(UINT8 uiTrim);
void ide_getTvTrim(UINT8 *uiTrim);

UINT8 ide_getTvBll(void);
UINT8 ide_getTvBrl(void);
UINT8 ide_getTvNtscSetupLevel(void);
void ide_getTvScale(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);

void ide_setTvBypassLPF(BOOL bEn);
BOOL ide_getTvBypassLPF(void);
void ide_setTvBypassNotch(BOOL bEn);
BOOL ide_getTvBypassNotch(void);
void ide_setTvChromaFilter(BOOL bPrag, UINT8 *uiTaps);
void ide_setTvYFilter(BOOL bPrag, UINT8 *uiTaps);

//*These api will be remove end --> add idetv_setConfig to config
void   idetv_setConfig(IDETV_CONFIG_ID ConfigID, UINT32 uiConfig);
UINT32 idetv_getConfig(IDETV_CONFIG_ID ConfigID);
void   idetv_setCoef(IDETV_CONFIGCOEF_ID ConfigID, UINT8 *uiCoef);
void   idetv_getCoef(IDETV_CONFIGCOEF_ID ConfigID, UINT8 *uiCoef);



//-------------------------------------------------
// IDE Public Functions
//-------------------------------------------------

#define ide_setVideoBufAddr(VideoID, pV_Buf_Addr)          idec_setVideoBufAddr(IDE_ID_1, VideoID, pV_Buf_Addr)
#define ide_getVideoBufAddr(VideoID, pV_Buf_Addr)          idec_getVideoBufAddr(IDE_ID_1, VideoID, pV_Buf_Addr)
#define ide_setVideoBufAttr(VideoID, pV_Buf_attr)          idec_setVideoBufAttr(IDE_ID_1, VideoID, pV_Buf_attr)
#define ide_getVideoBufAttr(VideoID, pV_Buf_attr)          idec_getVideoBufAttr(IDE_ID_1, VideoID, pV_Buf_attr)
#define ide_setVideoWinAttrEx(VideoID, pV_Win_attr, bLoad) idec_setVideoWinAttrEx(IDE_ID_1, VideoID, pV_Win_attr, bLoad)
#define ide_setVideoWinAttr(VideoID, pV_Win_attr)          idec_setVideoWinAttr(IDE_ID_1, VideoID, pV_Win_attr)
#define ide_setVideoVsfInit(VideoID, uiInit0, uiInit1)     idec_setVideoVsfInit(IDE_ID_1, VideoID, uiInit0, uiInit1)

#define ide_setOsdBufAddr(OSDID, uiOSD_Addr)               idec_setOsdBufAddr(IDE_ID_1, OSDID, uiOSD_Addr)
#define ide_getOsdBufAddr(OSDID, puiOSD_Addr)              idec_getOsdBufAddr(IDE_ID_1, OSDID, puiOSD_Addr)
#define ide_setOsdBufAttr(OSDID, pOSD_Buf_attr)            idec_setOsdBufAttr(IDE_ID_1, OSDID, pOSD_Buf_attr)
#define ide_getOsdBufAttr(OSDID, pOSD_Buf_attr)            idec_getOsdBufAttr(IDE_ID_1, OSDID, pOSD_Buf_attr)
#define ide_setOsdWinAttrEx(OSDID, pOSD_Win_attr, bLoad)   idec_setOsdWinAttrEx(IDE_ID_1, OSDID, pOSD_Win_attr, bLoad)
#define ide_setOsdWinAttr(OSDID, pOSD_Win_attr)            idec_setOsdWinAttr(IDE_ID_1, OSDID, pOSD_Win_attr)
#define ide_setOsdVsfInit(OSDID, uiInit0, uiInit1)         idec_setOsdVsfInit(IDE_ID_1, OSDID, uiInit0, uiInit1)

#define ide_setPaletteEntry(pPalette_entry)                idec_setPaletteEntry(IDE_ID_1, pPalette_entry)
#define ide_getPaletteEntry(pPalette_entry)                idec_getPaletteEntry(IDE_ID_1, pPalette_entry)
#define ide_setPaletteGroup(uiStart, uiNumber, pPalette_entry)        idec_setPaletteGroup(IDE_ID_1, uiStart, uiNumber, pPalette_entry)
#define ide_getPaletteGroup(uiStart, uiNumber, pPalette_entry)        idec_getPaletteGroup(IDE_ID_1, uiStart, uiNumber, pPalette_entry)
#define ide_getShadowPaletteGroup(uiStart, uiNumber, pPalette_entry) //idec_getShadowPaletteGroup(IDE_ID_1, uiStart, uiNumber, pPalette_entry)
#define ide_setPaletteGroupACrCbY(uiStart, uiNumber, pPalette_entry)  idec_setPaletteGroupACrCbY(IDE_ID_1, uiStart, uiNumber, pPalette_entry)
#define ide_getPaletteGroupACrCbY(uiStart, uiNumber, pPalette_entry)  idec_getPaletteGroupACrCbY(IDE_ID_1, uiStart, uiNumber, pPalette_entry)
#define ide_waitFrameEnd()                                 idec_waitFrameEnd(IDE_ID_1)
#define ide_waitYUVOutputDone()                            idec_waitYUVOutputDone(IDE_ID_1)

#define ide_disableVideo(VideoID)                          idec_disableVideo(IDE_ID_1, VideoID)
#define ide_enableVideo(VideoID)                           idec_enableVideo(IDE_ID_1, VideoID)
#define ide_getVideoEnable(VideoID)                        idec_getVideoEnable(IDE_ID_1, VideoID)

#define ide_disableOsd(OSDID)                              idec_disableOsd(IDE_ID_1, OSDID)
#define ide_enableOsd(OSDID)                               idec_enableOsd(IDE_ID_1, OSDID)
#define ide_getOsdEnable(OSDID)                            idec_getOsdEnable(IDE_ID_1, OSDID)
#define ide_getPalCapability()                             idec_getPalCapability(IDE_ID_1)

#define ide_setVideoBufferContent(VIDEO_ID, BUFFER_ID, pYCbCr)         idec_setVideoBufferContent(IDE_ID_1, VIDEO_ID, BUFFER_ID, pYCbCr)
#define ide_convertRGB2YCbCr(pRGB, pYCbCr)                 idec_convertRGB2YCbCr(IDE_ID_1, pRGB, pYCbCr)

//void ide_configVideoCst(IDE_VIDEOID VideoID, BOOL b_en, CST_SEL sel);
//void ide_configOsdCst(IDE_OSDID OSDID, BOOL b_en, CST_SEL sel);
//void ide_configIcst(BOOL b_en, CST_SEL sel);
#define ide_configIcst(b_en, sel)                          idec_configIcst(IDE_ID_1, b_en, sel)
#define ide_getconfigIcst(b_en, sel)                       idec_getconfigIcst(IDE_ID_1, b_en, sel)
#define ide_configOutputLimit(b_sel)                       idec_configOutputLimit(IDE_ID_1, b_sel)
#define ide_setConstantWindowOffset(uiX, uiY)              idec_setConstantWindowOffset(IDE_ID_1, uiX, uiY)
#define ide_getConstantWindowOffset(uiX, uiY)              idec_getConstantWindowOffset(IDE_ID_1, uiX, uiY)


//-------------------------------------------------
// IDE OS Level Functions
//-------------------------------------------------

//ID ide_getLockStatus(void);
#if 0
void ide_attach(void);
void ide_detach(void);
void ide_init(void);
#endif
#define ide_open()          idec_open(IDE_ID_1)
#define ide_close()         idec_close(IDE_ID_1)
#define ide_isOpened()      idec_isOpened(IDE_ID_1)
//void ide_registerMiInterruptHandler(FPIDEInterruptCallBack IntrHdl);
#define IDE1_OSD2_EXIST     0
//@}

//#include "ide_compat.h"

#endif //_IDE_H

