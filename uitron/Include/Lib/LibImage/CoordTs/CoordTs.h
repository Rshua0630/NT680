/**
    Coordinate Transformation

    Given a global array to create a coordinate transformation handle. Use the
    handle to operate any coordinate transformation between two different
    coordinate.

    @file       CoordTs.h
    @ingroup    mCOORDTS

    Example:
     @code
     {
        //global array
        __ALIGNED(4) static UINT8 m_LcdToHdmi[COORDTS_REQ_MEM_SIZE]={0};
        COORDTS_HANDLE g_hTs = NULL;

        void system_startup()
        {
            COORDTS_CREATE Create={0};
            COORDTS_ORIGIN Origin={0};
            Create.uiApiVer = COORDTS_API_VERSION;
            Create.pRegBuf = m_LcdToHdmi;
            Create.uiRegBufSize = sizeof(m_LcdToHdmi);
            g_hTs = CoordTs_Create(&Create);

            Origin.hHandle=g_hTs;
            Origin.SizeOrigin.uiWidth = lcd width;
            Origin.SizeOrigin.uiHeight = lcd height;
            CoordTs_SetOrigin(&Origin);
        }

        void display_init()
        {
            COORDTS_TARGET Target={0};
            Target.hHandle=g_hTs;
            Target.SizeTarget.uiWidth = hdmi width;
            Target.SizeTarget.uiHeight = hdmi height;
            Target.pRcRegion = NULL;
            CoordTs_SetTarget(&Target);
        }

        URECT rcSrc[] = {
                {0,0,959,479},
                {80,90,323,442},
                {30,60,90,77}
        };
        void coordinate_trans()
        {
            COORDTS_TRANS  Trans={0};
            Trans.hHandle=g_hTs;
            Trans.bInverse=FALSE;
            Trans.Method = COORDTS_METHOD_FULL_FIT;
            Trans.pRcSrc = rc;
            Trans.pRcDst = rc2;
            Trans.uiRectNum = sizeof(rc)/sizeof(rc[0]);
            CoordTs_TransRect(&Trans);
        }
     }
     @endcode

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _COORDTS_H
#define _COORDTS_H

/**
    @addtogroup mCOORDTS
*/
//@{

#define COORDTS_API_VERSION 0x12012012U ///<API version
#define COORDTS_REQ_MEM_SIZE 56 ///< require memory for each handle

/**
     Error code.

     CoordTs function error code.
*/
typedef enum _COORDTS_ER {
	COORDTS_ER_OK                =   0, ///< no error
	COORDTS_ER_INVALID_HANDLE    =  -1, ///< invalid handle
	COORDTS_ER_SYS               =  -2, ///< unknown system error
	COORDTS_ER_MEM_NOT_ENOUGH    =  -3, ///< memory not enough
	COORDTS_ER_FLOW              =  -4, ///< the calling flow is wrong
	COORDTS_ER_PARAM             =  -5, ///< error parameters
	ENUM_DUMMY4WORD(COORDTS_ER)
} COORDTS_ER;

/**
     Coordinate methods

     The CoordTs_TransRect () input parameter
*/
typedef enum _COORDTS_METHOD {
	COORDTS_METHOD_FULL_FIT = 0,    ///< no matter COORDTS_TARGET::pRcRegion, just full scale to target size.
	COORDTS_METHOD_ABSOULTE,        ///< absoulte coordinate transformation. if COORDTS_TARGET::pRcRegion is not NULL, the left and top are added an offset to fit new coordinate
	COORDTS_METHOD_INCREMENTAL,     ///< incremental coordinate transformation if COORDTS_TARGET::pRcRegion is not NULL, the left and top are not added an offset to fit new coordinate (just scaled)
	ENUM_DUMMY4WORD(COORD_METHOD)
} COORDTS_METHOD;

typedef void *COORDTS_HANDLE;  ///< module handle

/**
     CoordTs_Create() input structure
*/
typedef struct _COORDTS_CREATE {
	UINT32 uiApiVer;    ///< just assign COORDTS_API_VERSION
	UINT8 *pRegBuf;     ///< requirement memory, a global array for this module used. must be _ALIGNED(4)
	UINT32 uiRegBufSize;///< assigned pRegBuf size. (UNIT: byte)
} COORDTS_CREATE, *PCOORDTS_CREATE;

/**
     CoordTs_SetOrigin() input structure
*/
typedef struct _COORDTS_ORIGIN {
	COORDTS_HANDLE  hHandle;    ///< module handle
	USIZE           SizeOrigin; ///< original coordination, normal case is soruce size.
} COORDTS_ORIGIN, *PCOORDTS_ORIGIN;

/**
     CoordTs_SetTarget() input structure
*/
typedef struct _COORDTS_TARGET {
	COORDTS_HANDLE  hHandle;    ///< module handle
	USIZE           SizeTarget; ///< target coordination, normal case is target size.
	URECT          *pRcRegion;  ///< it can be a NULL. it use a case as a 4:3 ratio LCD wants to mapping in 4:3 ratio rectangle in 16:9 HDMI
} COORDTS_TARGET, *PCOORDTS_TARGET;

/**
     CoordTs_TransRect() input structure
*/
typedef struct _COORDTS_TRANS {
	COORDTS_HANDLE  hHandle;    ///< module handle
	BOOL            bInverse;   ///< inverse transform, that has precision issue, the result maybe not equal to original
	COORDTS_METHOD  Method;     ///< transform method, refer to COORDTS_METHOD
	URECT          *pRcSrc;     ///< source rectangles. the source rectangles address can be the same with target
	URECT          *pRcDst;     ///< destination rectangles.
	UINT32          uiRectNum;  ///< total rectangles
} COORDTS_TRANS, *PCOORDTS_TRANS;

/**
     Create a handle

     Create a handle for coordinate transformation. The coordinate transformation
     needs working buffer that size is COORDTS_REQ_MEM_SIZE bytes.

     @param[in] pCreate    refer to COORDTS_CREATE
     @return Description of data returned.
         - @b NULL:    failed to create
         - @b NON-NULL: Success
*/
extern COORDTS_HANDLE CoordTs_Create(const COORDTS_CREATE *pCreate);

/**
     Set base coordinate

     Original coordinate. For example, LCD -> HDMI coordinate transformation. the
     LCD coordinate is original. the HDMI coordinate transformation is target.

     @param[in] pOrigin    refer to COORDTS_ORIGIN
     @return Description of data returned.
         - @b COORDTS_ER_OK:    Success
         - @b COORDTS_ER_INVALID_HANDLE: invalid handle
*/
extern COORDTS_ER CoordTs_SetOrigin(const COORDTS_ORIGIN *pOrigin);

/**
     Set target coordinate

     Target coordinate. For example, LCD -> HDMI coordinate transformation. the
     LCD coordinate is original. the HDMI coordinate transformation is target.

     @param[in] pTarget    refer to COORDTS_TARGET
     @return Description of data returned.
         - @b COORDTS_ER_OK:    Success
         - @b COORDTS_ER_INVALID_HANDLE: invalid handle
*/
extern COORDTS_ER CoordTs_SetTarget(const COORDTS_TARGET *pTarget);

/**
     Coordinate transformation

     After original and target coordinate are set, the rectangle can be transformation
     between original and target coordinate.

     @param[in] pTrans    refer to COORDTS_TRANS
     @return Description of data returned.
         - @b COORDTS_ER_OK:    Success
         - @b COORDTS_ER_INVALID_HANDLE: invalid handle
         - @b COORDTS_ER_FLOW: may the CoordTs_SetOrigin and CoordTs_SetTarget are
                               not called before.
*/
extern COORDTS_ER CoordTs_TransRect(const COORDTS_TRANS *pTrans);

//@}
#endif

