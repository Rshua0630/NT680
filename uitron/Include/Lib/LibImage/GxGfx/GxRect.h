/**
    Rectangle and its related calculation utility.

    Rectangle and its related calculation utility.

    @file       GxRect.h
    @ingroup    mIGxgfx
    @note       Nothing

                IPOINT : Point

                ISIZE : Size

                IRECT : Rectangle

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#ifndef _GX_RECT_H_
#define _GX_RECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"
/**
    @addtogroup mIGxgfx
*/
//@{
//--------------------------------------------------------------------------------------
//  geometry
//--------------------------------------------------------------------------------------

//<type>

/**
     Location value.

     Location value.
     @note Nothing.
*/
typedef INT32   LVALUE;


//<function>

/*
//inline functions:

BOOL RANGE_IsInside(LVALUE small_value, LVALUE large_value, LVALUE x);
LVALUE RANGE_Clamp(LVALUE small_value, LVALUE large_value, LVALUE x);

void POINT_Set(IPOINT* pPt, LVALUE x, LVALUE y);
void POINT_SetPoint(IPOINT* pPt, const IPOINT* pPt2);
void POINT_MoveX(IPOINT* pPt, LVALUE dx);
void POINT_MoveY(IPOINT* pPt, LVALUE dy);
void POINT_MoveXY(IPOINT* pPt, LVALUE dx, LVALUE dy);
void POINT_MovePoint(IPOINT* pPt, const IPOINT* pPt2);
BOOL POINT_IsInside4Points(const IPOINT* pPt, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
BOOL POINT_IsInsideRect(const IPOINT* pPt, IRECT* pRect);

void SIZE_Set(ISIZE* pSz, LVALUE w, LVALUE h);
void SIZE_SetSize(ISIZE* pSz, const ISIZE* pSz2);

void RECT_Set(IRECT* pRect, LVALUE x, LVALUE y, LVALUE w, LVALUE h);
void RECT_SetXY(IRECT* pRect, LVALUE x, LVALUE y);
void RECT_SetWH(IRECT* pRect, LVALUE w, LVALUE h);
void RECT_SetPoint(IRECT* pRect, const IPOINT* pPt);
void RECT_SetSize(IRECT* pRect, const ISIZE* pSz);
void RECT_SetRect(IRECT* pRect, const IRECT* pRect2);
LVALUE RECT_GetX1(const IRECT* pRect);
LVALUE RECT_GetY1(const IRECT* pRect);
LVALUE RECT_GetX2(const IRECT* pRect);
LVALUE RECT_GetY2(const IRECT* pRect);
IPOINT RECT_GetPoint(const IRECT* pRect);
IPOINT RECT_GetPoint2(const IRECT* pRect);
IPOINT RECT_GetCenterPoint(const IRECT* pRect);
ISIZE RECT_GetSize(const IRECT* pRect);
void RECT_MoveX(IRECT* pRect, LVALUE dx);
void RECT_MoveY(IRECT* pRect, LVALUE dy);
void RECT_MoveXY(IRECT* pRect, LVALUE dx, LVALUE dy);
void RECT_MovePoint(IRECT* pRect, const IPOINT* pPt2);

void RECT_Normalize(IRECT* pRect);
void RECT_Inflate(IRECT* pRect, LVALUE left, LVALUE right, LVALUE top, LVALUE bottom);
void RECT_Shrink(IRECT* pRect, LVALUE left, LVALUE right, LVALUE top, LVALUE bottom);
*/


/**
     Check value if inside range or not.

     Check value if inside [min~max] range or not.
     @note Nothing

     @param[in] small_value     Minimum value.
     @param[in] large_value     Maximum value.
     @param[in] x               Current value.
     @return
         - @b TRUE:             Inside range.
         - @b FALSE:            Out of range.
*/
#define RANGE_IsInside(small_value, large_value, x) \
	((((x)>=(small_value)) && ((x)<=(large_value)))?TRUE:FALSE)

/**
     Clamp value to inside range.

     Clamp value to inside [min~max] range.
     @note Nothing

     @param[in] small_value     Minimum value.
     @param[in] large_value     Maximum value.
     @param[in] x               Current value.
     @return                    Output value.
*/
#define RANGE_Clamp(small_value, large_value, x) \
	(((x)<(small_value))?(small_value):(((x)>(large_value))?(large_value):(x)))

/**
     Set value of IPOINT.

     Set value of IPOINT.
     @note Nothing

     @param[out] pPt            IPOINT.
     @param[in] x               x value.
     @param[in] y               y value.
*/
_INLINE void POINT_Set(IPOINT *pPt, LVALUE x, LVALUE y)
{
	(pPt)->x = (x);
	(pPt)->y = (y);
}

/**
     Copy value of IPOINT.

     Copy value of IPOINT.
     @note Nothing

     @param[out] pPt            IPOINT.
     @param[in] pPt2            copy IPOINT.
*/
_INLINE void POINT_SetPoint(IPOINT *pPt, const IPOINT *pPt2)
{
	*(pPt) = *(pPt2);
}

/**
     Move x value of IPOINT.

     Move x value of IPOINT.
     @note Nothing

     @param[in,out] pPt         IPOINT.
     @param[in] dx              Shift offset of x.
*/
_INLINE void POINT_MoveX(IPOINT *pPt, LVALUE dx)
{
	(pPt)->x += (dx);
}

/**
     Move y value of IPOINT.

     Move y value of IPOINT.
     @note Nothing

     @param[in,out] pPt         IPOINT.
     @param[in] dy              Shift offset of y.
*/
_INLINE void POINT_MoveY(IPOINT *pPt, LVALUE dy)
{
	(pPt)->y += (dy);
}

/**
     Move x,y value of IPOINT.

     Move x,y value of IPOINT.
     @note Nothing

     @param[in,out] pPt         IPOINT.
     @param[in] dx              Shift offset of x.
     @param[in] dy              Shift offset of y.
*/
_INLINE void POINT_MoveXY(IPOINT *pPt, LVALUE dx, LVALUE dy)
{
	(pPt)->x += (dx);
	(pPt)->y += (dy);
}

/**
     Move x,y value of IPOINT by vector.

     Move x,y value of IPOINT by vector.
     @note Nothing

     @param[in,out] pPt         IPOINT.
     @param[in] pPt2            vector IPOINT.
*/
_INLINE void POINT_MovePoint(IPOINT *pPt, const IPOINT *pPt2)
{
	(pPt)->x += (pPt2)->x;
	(pPt)->y += (pPt2)->y;
}

/**
     Check IPOINT if inside rectangle or not.

     Check IPOINT if inside rectangle or not.
     @note Nothing

     @param[in] pPt             IPOINT.
     @param[in] x1              Left of rectangle.
     @param[in] y1              Top of rectangle.
     @param[in] x2              Right of rectangle.
     @param[in] y2              Bottom of rectangle.
     @return
         - @b TRUE:             Inside rectangle.
         - @b FALSE:            Out of rectangle.
*/
_INLINE BOOL POINT_IsInside4Points(const IPOINT *pPt, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2)
{
	return
		(
			(pPt->x >= x1) &&
			(pPt->x <= x2) &&
			(pPt->y >= y1) &&
			(pPt->y <= y2)
		);
}

/**
     Check IPOINT if inside rectangle or not.

     Check IPOINT if inside rectangle or not.
     @note Nothing

     @param[in] pPt             IPOINT.
     @param[in] pRect           The rectangle.
     @return
         - @b TRUE:             Inside rectangle.
         - @b FALSE:            Out of rectangle.
*/
_INLINE BOOL POINT_IsInsideRect(const IPOINT *pPt, IRECT *pRect)
{
	return
		(
			(pPt->x >= pRect->x) &&
			(pPt->x < (pRect->x + pRect->w)) &&
			(pPt->y >= pRect->y) &&
			(pPt->y < (pRect->y + pRect->h))
		);
}


/**
     Set value of SIZE.

     Set value of SIZE.
     @note Nothing

     @param[out] pSz            SIZE.
     @param[in] w               w value.
     @param[in] h               h value.
*/
_INLINE void SIZE_Set(ISIZE *pSz, LVALUE w, LVALUE h)
{
	(pSz)->w = (w);
	(pSz)->h = (h);
}

/**
     Copy value of SIZE.

     Copy value of SIZE.
     @note Nothing

     @param[out] pSz            SIZE.
     @param[in] pSz2            copy SIZE.
*/
_INLINE void SIZE_SetSize(ISIZE *pSz, const ISIZE *pSz2)
{
	*(pSz) = *(pSz2);
}



/**
     Set value of IRECT.

     Set value of IRECT.
     @note Nothing

     @param[out] pRect          IRECT.
     @param[in] x               x value.
     @param[in] y               y value.
     @param[in] w               w value.
     @param[in] h               h value.
*/
_INLINE void RECT_Set(IRECT *pRect, LVALUE x, LVALUE y, LVALUE w, LVALUE h)
{
	(pRect)->x = (x);
	(pRect)->y = (y);
	(pRect)->w = (w);
	(pRect)->h = (h);
}

/**
     Set x,y value of IRECT.

     Set x,y value of IRECT.
     @note Nothing

     @param[out] pRect          IRECT.
     @param[in] x               x value.
     @param[in] y               y value.
*/
_INLINE void RECT_SetXY(IRECT *pRect, LVALUE x, LVALUE y)
{
	(pRect)->x = (x);
	(pRect)->y = (y);
}

/**
     Set w,h value of IRECT.

     Set w,h value of IRECT.
     @note Nothing

     @param[out] pRect          IRECT.
     @param[in] w               w value.
     @param[in] h               h value.
*/
_INLINE void RECT_SetWH(IRECT *pRect, LVALUE w, LVALUE h)
{
	(pRect)->w = (w);
	(pRect)->h = (h);
}

/**
     Set x,y value of IRECT by IPOINT.

     Set x,y value of IRECT by IPOINT.
     @note Nothing

     @param[out] pRect          IRECT.
     @param[in] pPt             The IPOINT.
*/
_INLINE void RECT_SetPoint(IRECT *pRect, const IPOINT *pPt)
{
	(pRect)->x = (pPt)->x;
	(pRect)->y = (pPt)->y;
}

/**
     Set w,h value of IRECT by ISIZE.

     Set w,h value of IRECT by ISIZE.
     @note Nothing

     @param[out] pRect          IRECT.
     @param[in] pSz             The ISIZE.
*/
_INLINE void RECT_SetSize(IRECT *pRect, const ISIZE *pSz)
{
	(pRect)->w = (pSz)->w;
	(pRect)->h = (pSz)->h;
}

/**
     Copy value of IRECT.

     Copy value of IRECT.
     @note Nothing

     @param[out] pRect          IRECT.
     @param[in] pRect2          copy IRECT.
*/
_INLINE void RECT_SetRect(IRECT *pRect, const IRECT *pRect2)
{
	*(pRect) = *(pRect2);
}

/**
     Get left x value of IRECT.

     Get left x value of IRECT.
     @note Nothing

     @param[in] pRect           IRECT.
     @return                    x1 value.
*/
_INLINE LVALUE RECT_GetX1(const IRECT *pRect)
{
	return (pRect)->x;
}

/**
     Get top y value of IRECT.

     Get top y value of IRECT.
     @note Nothing

     @param[in] pRect           IRECT.
     @return                    y1 value.
*/
_INLINE LVALUE RECT_GetY1(const IRECT *pRect)
{
	return (pRect)->y;
}

/**
     Get right x value of IRECT.

     Get right x value of IRECT.
     @note Nothing

     @param[in] pRect           IRECT.
     @return                    x2 value.
*/
_INLINE LVALUE RECT_GetX2(const IRECT *pRect)
{
	return (pRect)->x + (pRect)->w;
}

/**
     Get bottom y value of IRECT.

     Get bottom y value of IRECT.
     @note Nothing

     @param[in] pRect           IRECT.
     @return                    y2 value.
*/
_INLINE LVALUE RECT_GetY2(const IRECT *pRect)
{
	return (pRect)->y + (pRect)->h;
}

/**
     Get left-top point of IRECT to IPOINT.

     Get left-top point of IRECT to IPOINT.
     @note Nothing

     @param[in] pRect           IRECT.
     @return                    The IPOINT.
*/
_INLINE IPOINT RECT_GetPoint(const IRECT *pRect)
{
	IPOINT Pt;
	(Pt).x = (pRect)->x;
	(Pt).y = (pRect)->y;
	return Pt;
}

/**
     Get right-bottom point of IRECT to IPOINT.

     Get right-bottom point of IRECT to IPOINT.
     @note Nothing

     @param[in] pRect           IRECT.
     @return                    The IPOINT.
*/
_INLINE IPOINT RECT_GetPoint2(const IRECT *pRect)
{
	IPOINT Pt;
	(Pt).x = (pRect)->x + (pRect)->w;
	(Pt).y = (pRect)->y + (pRect)->h;
	return Pt;
}

/**
     Get center point of IRECT to IPOINT.

     Get center point of IRECT to IPOINT.
     @note Nothing

     @param[in] pRect           IRECT.
     @return                    The IPOINT.
*/
_INLINE IPOINT RECT_GetCenterPoint(const IRECT *pRect)
{
	IPOINT Pt;
	(Pt).x = (pRect)->x + ((pRect)->w >> 1);
	(Pt).y = (pRect)->y + ((pRect)->h >> 1);
	return Pt;
}

/**
     Get size of IRECT to ISIZE.

     Get size of IRECT to ISIZE.
     @note Nothing

     @param[in] pRect           IRECT.
     @return                    The ISIZE.
*/
_INLINE ISIZE RECT_GetSize(const IRECT *pRect)
{
	ISIZE Sz;
	(Sz).w = (pRect)->w;
	(Sz).h = (pRect)->h;
	return Sz;
}

/**
     Normalize x,y,w,h value of IRECT.

     Normalize x,y,w,h value of IRECT.
     @note Keep x,y is left-top point, w,h is right-bottom point from left-top point.

     @param[in,out] pRect       IRECT.
*/
_INLINE void RECT_Normalize(IRECT *pRect)
{
	if ((pRect)->w < 0) {
		(pRect)->x += (pRect)->w;
		(pRect)->w = -(pRect)->w;
	}
	if ((pRect)->h < 0) {
		(pRect)->y += (pRect)->h;
		(pRect)->h = -(pRect)->h;
	}
}

/**
     Set value of IRECT.

     Set value of IRECT.
     @note Nothing

     @param[out] pRect          IRECT.
     @param[in] x1              left x value.
     @param[in] y1              top y value.
     @param[in] x2              right x value.
     @param[in] y2              bottom y value.
*/
_INLINE void RECT_SetX1Y1X2Y2(IRECT *pRect, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2)
{
	LVALUE temp;
	if (x2 < x1) {
		temp = x2;
		x2 = x1;
		x1 = temp;
	}
	if (y2 < y1) {
		temp = y2;
		y2 = y1;
		y1 = temp;
	}
	RECT_Set(pRect, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

/**
     Move x value of RECT.

     Move x value of RECT.
     @note Nothing

     @param[in,out] pRect       IRECT.
     @param[in] dx              move x value.
*/
#define RECT_MoveX(pRect, dx)   \
	POINT_MoveX(((IPOINT*)pRect), dx)

/**
     Move y value of RECT.

     Move y value of RECT.
     @note Nothing

     @param[in,out] pRect       IRECT.
     @param[in] dy              move y value.
*/
#define RECT_MoveY(pRect, dy)   \
	POINT_MoveY(((IPOINT*)pRect), dy)

/**
     Move x,y value of RECT.

     Move x,y value of RECT.
     @note Nothing

     @param[in,out] pRect       IRECT.
     @param[in] dx              move x value.
     @param[in] dy              move y value.
*/
#define RECT_MoveXY(pRect, dx, dy)   \
	POINT_MoveXY(((IPOINT*)pRect), dx, dy)

/**
     Move x,y value of RECT by IPOINT.

     Move x,y value of RECT by IPOINT.
     @note Nothing

     @param[in,out] pRect       IRECT.
     @param[in] pPt             move vector IPOINT.
*/
#define RECT_MovePoint(pRect, pPt)   \
	POINT_MovePoint(((IPOINT*)pRect), pPt)


/**
     Inflate range of RECT.

     Inflate range of RECT.
     @note Nothing

     @param[in,out] pRect       IRECT.
     @param[in] left            left side inflate value.
     @param[in] right           right side inflate value.
     @param[in] top             top side inflate value.
     @param[in] bottom          bottom side inflate value.
*/
_INLINE void RECT_Inflate(IRECT *pRect, LVALUE left, LVALUE right, LVALUE top, LVALUE bottom)
{
	(pRect)->x += -(left);
	(pRect)->w += ((left) + (right));
	(pRect)->y += -(top);
	(pRect)->h += ((top) + (bottom));
}


/**
     Shrink range of RECT.

     Shrink range of RECT.
     @note Nothing

     @param[in,out] pRect       IRECT.
     @param[in] left            left side shrink value.
     @param[in] right           right side shrink value.
     @param[in] top             top side shrink value.
     @param[in] bottom          bottom side shrink value.
*/
#define RECT_Shrink(pRect, left, right, top, bottom) \
	RECT_Inflate(pRect, -(left), -(right), -(top), -(bottom))

//@}

#ifdef __cplusplus
} //extern "C"
#endif

#endif//_GX_RECT_H_
