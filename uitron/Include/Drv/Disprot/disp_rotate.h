/**
    Display object extension option of Rotation Task's header file

    @file       disp_rotate.h
    @ingroup    mISYSDisp
    @note       The rotation task is used in the MI LCD display to rotate 90 degrees of the display content
                to avoid tearing effect.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DISPROTATE_H
#define _DISPROTATE_H

//#include "Oscommon.h"
#include "display.h"


/**
    Display Rotation Task Buffer Seelct ID
*/
typedef enum {
	DISPROT_BUF_Y,
	DISPROT_BUF_U,
	DISPROT_BUF_V,

	DISPROT_BUFSEL_MAX,
	ENUM_DUMMY4WORD(DISPROT_BUFSEL)
} DISPROT_BUFSEL;

/**
    Display Rotation Task Rotation Direction Select ID
*/
typedef enum {
	DISPROT_ROTATE_NONE,
	DISPROT_ROTATE90CW,
	DISPROT_ROTATE90CCW,

	ENUM_DUMMY4WORD(DISPROT_DIR)
} DISPROT_DIR;


/**
    The control operation code for disprot_setParameter().
*/
typedef enum {
	/* SET control group */
	DISPROT_OP_SET_ROTATE,
	DISPROT_OP_SET_BUF_FMT,
	DISPROT_OP_SET_INPUT_DIM,
	DISPROT_OP_SET_OUTBUF_ADDR,

	/* GET control group */
	DISPROT_OP_GET_OUTPUT_DIM,
	DISPROT_OP_GET_ROTATE,

	/* CHECK control group */
	DISPROT_OP_CHK_OUTBUF_SIZE,

	ENUM_DUMMY4WORD(DISPROT_OP)
} DISPROT_OP;



/**
    The control operation parameters for disprot_setParameter().
*/
typedef struct {
	union {
		/* SET control group */

		/**
		    Set Rotation Operation to specified Display engine Layer.

		    This structure is used for disprot_setParameter() operation code "DISPROT_OP_SET_ROTATE" to set the rotation select to
		    specified "DispLayer" layer of the specified display engine "DispID".
		    Current Version support for video layer format: DISPBUFFORMAT_YUV420 only.
		    This structure shuld be used like this:
		\n  DispRot.SEL.SET_ROTATE.DispID       = DISP_1;
		\n  DispRot.SEL.SET_ROTATE.DispLayer    = DISPLAYER_VDO1;
		\n  DispRot.SEL.SET_ROTATE.RotOp        = DISPROT_ROTATE90CW;
		\n  disprot_setParameter(DISPROT_OP_SET_ROTATE, &DispRot);
		*/
		struct { // DISPROT_OP_SET_ROTATE
			DISP_ID             DispID;         ///< Specified Display Engine's ID.
			DISPLAYER           DispLayer;      ///< Specified Display layer's ID.
			DISPROT_DIR         RotDir;          ///< The rotation option selected.
		} SET_ROTATE;

		/**
		    Set Rotation buffer format to specified Display engine Layer.

		    This structure is used for disprot_setParameter() operation code "DISPROT_OP_SET_BUF_FMT" to set the buffer format to
		    specified "DispLayer" layer of the specified display engine "DispID".
		    Current Version support for video layer format: DISPBUFFORMAT_YUV420 only.
		    This structure shuld be used like this:
		\n  DispRot.SEL.SET_BUF_FMT.DispID       = DISP_1;
		\n  DispRot.SEL.SET_BUF_FMT.DispLayer    = DISPLAYER_VDO1;
		\n  DispRot.SEL.SET_BUF_FMT.BufFmt       = DISPBUFFORMAT_YUV420;
		\n  disprot_setParameter(DISPROT_OP_SET_BUF_FMT, &DispRot);
		*/
		struct { // DISPROT_OP_SET_BUF_FMT
			DISP_ID             DispID;         ///< Specified Display Engine's ID.
			DISPLAYER           DispLayer;      ///< Specified Display layer's ID.
			DISPBUFFORMAT       BufFmt;         ///< The rotation option selected.
		} SET_BUF_FMT;

		/**
		    Set Rotation input buffer dimension to specified Display engine Layer.

		    This structure is used for disprot_setParameter() operation code "DISPROT_OP_SET_INPUT_DIM" to set the input buffer dimension to
		    specified "DispLayer" layer of the specified display engine "DispID".
		    Current Version support for video layer format: DISPBUFFORMAT_YUV420 only.
		    After assigning the input buffer dimension, use can get the output buffer dimension needed to allocate output buffer size
		    through the operation code "DISPROT_OP_GET_OUTPUT_DIM"
		    This structure shuld be used like this:
		\n  DispRot.SEL.SET_INPUT_DIM.DispID       = DISP_1;
		\n  DispRot.SEL.SET_INPUT_DIM.DispLayer    = DISPLAYER_VDO1;
		\n  DispRot.SEL.SET_INPUT_DIM.uiSrcWidth   = 320;
		\n  DispRot.SEL.SET_INPUT_DIM.uiSrcHeight  = 240;
		\n  DispRot.SEL.SET_INPUT_DIM.uiSrcLoff    = 320;
		\n  disprot_setParameter(DISPROT_OP_SET_INPUT_DIM, &DispRot);
		*/
		struct { // DISPROT_OP_SET_INPUT_DIM
			DISP_ID             DispID;         ///< Specified Display Engine's ID.
			DISPLAYER           DispLayer;      ///< Specified Display layer's ID.

			UINT32              uiSrcWidth;     ///< Input Buffer Width in BYTE count.
			UINT32              uiSrcHeight;    ///< Input Buffer Hight in LINE count.
			UINT32              uiSrcLoffY;     ///< Input Buffer Line-Offset for plane Y in BYTE count. Must be multiple of 4 bytes.
			UINT32              uiSrcLoffUV;    ///< Input Buffer Line-Offset for plane U/V in BYTE count. Must be multiple of 4 bytes.
		} SET_INPUT_DIM;

		/**
		    Set Rotation output buffer address to specified Display engine Layer.

		    This structure is used for disprot_setParameter() operation code "DISPROT_OP_SET_OUTBUF_ADDR" to set the output buffer address
		    to specified "DispLayer" layer of the specified display engine "DispID".
		    The display rotation task is fixed using 2 blocks of dram buffer. The application need only assign 2 buffer address.
		    This structure shuld be used like this:
		\n  DispRot.SEL.SET_OUTBUF_ADDR.DispID                  = DISP_1;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.DispLayer               = DISPLAYER_VDO1;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_0][DISPROT_BUF_Y]= 0x400000;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_0][DISPROT_BUF_U]= 0x480000;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_0][DISPROT_BUF_V]= 0x500000;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_1][DISPROT_BUF_Y]= 0x580000;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_1][DISPROT_BUF_U]= 0x600000;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_1][DISPROT_BUF_V]= 0x680000;
		\n  disprot_setParameter(DISPROT_OP_SET_OUTBUF_ADDR, &DispRot);
		*/
		struct { // DISPROT_OP_SET_OUTBUF_ADDR
			DISP_ID             DispID;                                     ///< Specified Display Engine's ID.
			DISPLAYER           DispLayer;                                  ///< Specified Display layer's ID.
			UINT32              uiBufAddr[DISPACTBUF_NUM][DISPROT_BUFSEL_MAX];///< Assigned Buffer Address
		} SET_OUTBUF_ADDR;




		/* GET control group */

		/**
		    Get Rotation output buffer dimension of the specified Display engine Layer.

		    This structure is used for disprot_setParameter() operation code "DISPROT_OP_GET_OUTPUT_DIM" to get the output buffer dimension to
		    specified "DispLayer" layer of the specified display engine "DispID".
		    This structure shuld be used like this:
		\n  DispRot.SEL.GET_OUTPUT_DIM.DispID       = DISP_1;
		\n  DispRot.SEL.GET_OUTPUT_DIM.DispLayer    = DISPLAYER_VDO1;
		\n  disprot_setParameter(DISPROT_OP_GET_OUTPUT_DIM, &DispRot);
		\n  "DispRot.SEL.GET_OUTPUT_DIM.uiOutWidth"/"DispRot.SEL.GET_OUTPUT_DIM.uiOutHeight"/"DispRot.SEL.GET_OUTPUT_DIM.uiOutLoff"
		\n  are the output dimension of the display engine layer by the rotation task.
		*/
		struct { // DISPROT_OP_GET_OUTPUT_DIM
			DISP_ID             DispID;         ///< Specified Display Engine's ID.
			DISPLAYER           DispLayer;      ///< Specified Display layer's ID.

			UINT32              uiOutWidth;     ///< Output Buffer Width in BYTE count.
			UINT32              uiOutHeight;    ///< Output Buffer Hight in LINE count.
			UINT32              uiOutLoff;      ///< Output Buffer Line-Offset in BYTE count.
		} GET_OUTPUT_DIM;

		/**
		    Get Rotation Operation configuration of specifying Display engine Layer.

		    This structure is used for disprot_setParameter() operation code "DISPROT_OP_GET_ROTATE" to get the rotation select to
		    specified "DispLayer" layer of the specified display engine "DispID".
		    Current Version support for video layer format: DISPBUFFORMAT_YUV420 only.
		    This structure shuld be used like this:
		\n  DispRot.SEL.GET_ROTATE.DispID       = DISP_1;
		\n  DispRot.SEL.GET_ROTATE.DispLayer    = DISPLAYER_VDO1;
		\n  disprot_setParameter(DISPROT_OP_GET_ROTATE, &DispRot);
		\n  And the DispRot.SEL.GET_ROTATE.RotDir is the selected current configuration.
		*/
		struct { // DISPROT_OP_GET_ROTATE
			DISP_ID             DispID;         ///< Specified Display Engine's ID. This is input paraemter.
			DISPLAYER           DispLayer;      ///< Specified Display layer's ID. This is input paraemter.
			DISPROT_DIR         RotDir;         ///< The rotation option selected. This is output paraemter.
		} GET_ROTATE;


		/* CHECK control group */

		/**
		    Check the Rotation Task allocated output buffer size is enough or not for the specified Display engine Layer.

		    This structure is used for disprot_setParameter() operation code "DISPROT_OP_CHK_OUTBUF_SIZE" to check the output buffer size
		    to is enough or not for the specified "DispLayer" layer of the specified display engine "DispID".
		    The checked results are outputing by the uart message.
		    This structure shuld be used like this:
		\n  DispRot.SEL.GET_OUTPUT_DIM.DispID                   = DISP_1;
		\n  DispRot.SEL.GET_OUTPUT_DIM.DispLayer                = DISPLAYER_VDO1;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPROT_BUF_Y]= 0x80000;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPROT_BUF_U]= 0x20000;
		\n  DispRot.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPROT_BUF_V]= 0x20000;
		\n  disprot_setParameter(DISPROT_OP_CHK_OUTBUF_SIZE, &DispRot);
		*/
		struct { // DISPROT_OP_CHK_OUTBUF_SIZE
			DISP_ID             DispID;                         ///< Specified Display Engine's ID.
			DISPLAYER           DispLayer;                      ///< Specified Display layer's ID.

			UINT32              uiBufSize[DISPROT_BUFSEL_MAX];  ///< Check the allocated buffer size is enough or not.
		} CHK_OUTBUF_SIZE;

	} SEL;
} DISPROT_PARAM, *PDISPROT_PARAM;



/**
    The rotation task trigger operation parameters
*/
typedef struct {
	DISP_ID             DispID;                         ///< Specify Display ID to apply rotation operation
	DISPLAYER           DispLayer;                      ///< Specify Display Layer to apply rotation operation

	//DISPACTBUF          DispActBuf;
	UINT32              SrcBufAddr[DISPROT_BUFSEL_MAX]; ///< Specify Source Buffer Address

	BOOL                bLoadDisp;
} DISPROT_TRIG, *PDISPROT_TRIG;



//
//  Extern APIs
//
extern ER       disprot_open(void);
extern BOOL     disprot_isOpened(void);
extern ER       disprot_close(void);
extern ER       disprot_lock(void);
extern ER       disprot_unlock(void);
extern ER       disprot_setParameter(DISPROT_OP RotOp, PDISPROT_PARAM pRotParam);
extern ER       disprot_trigger(PDISPROT_TRIG pDispRotTrig);
extern void     disprot_InstallID(void) _SECTION(".kercfg_text");

#endif
